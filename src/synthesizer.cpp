/*
    Rocaloid
    Copyright (C) 2015 StarBrilliant <m13253@hotmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3.0 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program.  If not,
    see <http://www.gnu.org/licenses/>.
*/

// Define M_PI etc
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "synthesizer.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <array>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <libwintf8/termio.h>
#include <sndfile.h>
#include "fast-random.hpp"
#include "f0-tracker.hpp"
#include "hnm-parameters.hpp"
#include "option-manager.hpp"
#include "pcm-file.hpp"
#include "signal-segment.hpp"
#include "spectrum.hpp"
#include "time-mapper.hpp"
#include "tuner.hpp"
#include "utils.hpp"
#include "vector-interpolator.hpp"
#include "window.hpp"

/*
 * TODO: Naming suggestions as following,
 *  harmony -> harmonic (n. in this case)
 *  f0 (when representing the first spectral peak at fundamental freq) -> first harmonic
 *  source -> input/in
 *  sink -> output/out
 *  pillar_idx -> harmonic_idx
 *  magnitude (when representing the factor multiplied by a sine wave) -> amplitude
 *    (magnitude usually appears when we're describing the spectrum; amplitude is for time domain)
 */

namespace RUCE {

struct Synthesizer::Private {
    PCMFile input_file;
    PCMFile output_file;
    int32_t input_sample_rate;
    int32_t output_sample_rate;
    int64_t input_file_frames;
    int64_t output_file_frames;
    /**
     * The buffer related to voice source (input)
     */
    SignalSegment source_buffer;
    /**
     * The buffer related to voice sink (output)
     */
    SignalSegment sink_buffer;
    HNMParameters source_hnm_parameters;
    HNMParameters sink_hnm_parameters;
    Tuner tuner;
    F0Tracker f0_tracker;
    FastRandom fast_random;
};

Synthesizer::Synthesizer(OptionManager &option_manager) :
    option_manager(option_manager) {
}

Synthesizer::~Synthesizer() {
}

Synthesizer &Synthesizer::check_params() {
    if(option_manager.get_required_length() < 0) {
        option_manager.set_required_length(1);
    }

    return *this;
}

Synthesizer &Synthesizer::prepare() {
    p->input_file.open(option_manager.get_input_file_name(), std::ios_base::in, 0, 1, 0);
    p->input_sample_rate = p->input_file.sample_rate();
    p->input_file_frames = p->input_file.frames();
    if(p->input_file.channels() != 1) {
        WTF8::cerr << "Error: Input file must have only one channel." << std::endl;
        std::exit(1);
    }

    p->output_sample_rate = p->input_sample_rate;
    p->output_file_frames = size_t(std::ceil(option_manager.get_required_length() * p->output_sample_rate));
    p->output_file.open(option_manager.get_output_file_name(), std::ios_base::out, SF_FORMAT_WAV | SF_FORMAT_DOUBLE, 1, p->output_sample_rate);

    p->sink_buffer = SignalSegment(p->output_file_frames);

    return *this;
}

Synthesizer &Synthesizer::read_source() {
    // Some voicebanks provide negative right_blank value
    if(option_manager.get_right_blank() < 0) {
        option_manager.set_right_blank(double(p->input_file_frames)/double(p->input_sample_rate) + option_manager.get_right_blank() - option_manager.get_left_blank());
    }

    int64_t left_bound = int64_t(std::round(option_manager.get_left_blank() * p->input_sample_rate));
    int64_t right_bound = p->input_file_frames - int64_t(std::round(option_manager.get_right_blank() * p->input_sample_rate));
    p->source_buffer = SignalSegment(left_bound, right_bound);
    if(left_bound >= 0) {
        p->input_file.seek(left_bound, SEEK_SET);
        p->input_file.read(p->source_buffer.buffer(), p->source_buffer.size());
    } else {
        p->input_file.seek(0, SEEK_SET);
        p->input_file.read(&p->source_buffer.buffer()[left_bound], p->source_buffer.size() - left_bound);
    }

    return *this;
}

Synthesizer &Synthesizer::track_f0() {
    p->f0_tracker.track(p->source_buffer, p->input_sample_rate);
    size_t count = 0;
    double avg_f0 = 0;
    for(const auto &i : p->f0_tracker.get_result())
        if(i != 0) {
            avg_f0 += i;
            ++count;
        }
    if(count != 0)
        WTF8::clog << "Average voice source frequency: " << (avg_f0 / count) << " Hz" << std::endl;
    else
        WTF8::clog << "Warning: Can not determine voice source frequency" << std::endl;

    return *this;
}

Synthesizer &Synthesizer::analyze() {
    static const ssize_t source_fft_size = Spectrum::next_pow2(p->input_sample_rate/48);
    static const ssize_t source_window_hop = source_fft_size/2;
    static const double source_max_harmony = 8000;
    static const auto max_pillars = HNMParameters::max_pillars;
    assert((source_fft_size & 1) == 0);

    p->source_hnm_parameters.window_size = source_fft_size;
    p->source_hnm_parameters.window_hop = source_window_hop;
    p->source_hnm_parameters.first_window_mid = p->source_buffer.left();

    Spectrum source_spectrum(source_fft_size);

    for(auto source_window_mid = p->source_buffer.left(); source_window_mid < p->source_buffer.right()+source_fft_size/2; source_window_mid += source_window_hop) {
        double source_f0 = p->f0_tracker.get_result(source_window_mid);

        // Copy the source and window it
        ssize_t source_window_half_size = source_f0 > 0 ? ssize_t(std::round(3 * p->input_sample_rate / source_f0 / 2)) : source_fft_size;
        if(source_window_half_size > source_fft_size/2)
            source_window_half_size = source_fft_size/2;
        SignalSegment source_segment(p->source_buffer, source_window_mid-source_window_half_size, source_window_mid+source_window_half_size);
        source_segment <<= source_segment.left() + source_window_half_size;
        Window source_window = HannWindow(source_window_half_size*2);
        source_window <<= source_window_half_size;
        source_segment *= source_window;

        // Do FFT analysis
        source_segment = source_spectrum.fftshift(SignalSegment(source_segment, -source_fft_size/2, source_fft_size/2));
        source_spectrum.fft_analyze(source_segment);

        // Convert magnitude to log scale
        std::vector<double> source_magnitude = source_spectrum.get_magnitude();
        for(double &mag : source_magnitude)
            mag = mag > 0 ? std::log10(mag*2/source_window.sum()) : -HUGE_VAL;

        // Prepare output values
        std::array<double, max_pillars> source_harmony_frequencies {{ 0 }};
        std::array<double, max_pillars> source_harmony_magnitudes {{ 0 }};
        std::array<std::complex<double>, max_pillars> source_harmony_phases_difference;
        for(auto &i : source_harmony_phases_difference) {
            i = std::complex<double>(1.0, 0.0);
        }
        std::valarray<double> source_noise_magnitudes(source_fft_size/2);

        if(source_f0 != 0) {
            // [0] is identical to [1], for the convenience of interpolation, you should use [1]
            source_harmony_frequencies[0] = source_f0;
            source_harmony_frequencies[1] = source_f0;
            // Optimal peak finding
            for(size_t pillar_idx = 2; pillar_idx < max_pillars; pillar_idx++) {
                double source_harmony_frequency = source_f0 * pillar_idx;
                if(source_harmony_frequency >= std::min(source_max_harmony, double(p->input_sample_rate)/2))
                    break;
                ssize_t source_harmony_bin_low = std::max(ssize_t(std::round(source_spectrum.hertz_to_bin(source_harmony_frequency - source_f0/3, p->input_sample_rate))), ssize_t(1));
                ssize_t source_harmony_bin_high = std::min(ssize_t(std::round(source_spectrum.hertz_to_bin(source_harmony_frequency + source_f0/3, p->input_sample_rate))), ssize_t(source_magnitude.size()/2-1));
                ssize_t peak_bin = ssize_t(std::round(source_spectrum.hertz_to_bin(source_harmony_frequency, p->input_sample_rate))); // The highest bin
                double peak_magnitude = 0; // The value of the highest bin
                for(ssize_t bin = source_harmony_bin_low; bin <= source_harmony_bin_high; bin++) {
                    if(source_magnitude[bin] > peak_magnitude) {
                        peak_bin = bin;
                        peak_magnitude = source_magnitude[bin];
                    }
                }

                double y_1 = source_magnitude[peak_bin-1];
                double y0 = source_magnitude[peak_bin];
                double y1 = source_magnitude[peak_bin+1];
                double subbin_correction = clamp((y_1-y1)/(2*y1-4*y0+2*y_1), -1.0, 1.0);
                source_harmony_frequency = source_spectrum.bin_to_hertz(peak_bin + subbin_correction, p->input_sample_rate);
                if(!(source_harmony_frequency >= 0 && source_harmony_frequency < std::min(source_max_harmony, double(p->input_sample_rate)/2)))
                    source_harmony_frequency = 0;
                source_harmony_frequencies[pillar_idx] = source_harmony_frequency;
            }

            // Extract sinusold parameters
            std::vector<std::complex<double>> source_phase = source_spectrum.get_phase();
            for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
                double source_harmony_frequency = source_harmony_frequencies[pillar_idx];
                if(source_harmony_frequency == 0)
                    continue;

                // Magnitude
                QuadraticVectorInterpolator<double> quadratic_vector_interpolator;
                try {
                    source_harmony_magnitudes[pillar_idx] = std::pow(10, quadratic_vector_interpolator([&](size_t index) {
                        return source_magnitude[index];
                    }, source_magnitude.size(), source_spectrum.hertz_to_bin(source_harmony_frequency, p->input_sample_rate)));
                } catch(std::out_of_range) {
                }

                // Phase
                LinearVectorInterpolator<std::complex<double>> linear_vector_interpolator;
                try {
                    source_harmony_phases_difference[pillar_idx] = linear_vector_interpolator([&](size_t index) {
                        return source_phase[index];
                    }, source_phase.size(), source_spectrum.hertz_to_bin(source_harmony_frequency, p->input_sample_rate));
                } catch(std::out_of_range) {
                }

            }

            source_harmony_magnitudes[0] = source_harmony_magnitudes[1];

            // Convert absolute phases to relative phases to f0
            double base_harmony_phase_angle = std::arg(source_harmony_phases_difference[1]);
            for(size_t pillar_idx = 2; pillar_idx < max_pillars; pillar_idx++) {
                double source_harmony_frequency = source_harmony_frequencies[pillar_idx];
                if(source_harmony_frequency != 0) {
                    double phase_angle = std::arg(source_harmony_phases_difference[pillar_idx]);
                    phase_angle -= base_harmony_phase_angle*(source_harmony_frequency/source_harmony_frequencies[1]);
                    source_harmony_phases_difference[pillar_idx] = std::polar(1.0, phase_angle);
                } else
                    source_harmony_phases_difference[pillar_idx] = std::complex<double>(1.0, 0.0);
            }
            source_harmony_phases_difference[0] = std::complex<double>(1.0, 0.0);
            source_harmony_phases_difference[1] = std::complex<double>(1.0, 0.0);
        }

        // Model noise
        // STUB A temporary method
        for(ssize_t bin = 1; bin < source_fft_size/2; bin++) {
            source_noise_magnitudes[bin] = std::pow(10, source_magnitude[bin]);
        }
        for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
            double source_harmony_frequency = source_harmony_frequencies[pillar_idx];
            if(source_harmony_frequency == 0)
                continue;
            ssize_t cut_low = std::max(ssize_t(std::round(source_spectrum.hertz_to_bin(source_harmony_frequency - source_harmony_frequency/32, p->input_sample_rate))), ssize_t(1));
            ssize_t cut_high = std::min(ssize_t(std::round(source_spectrum.hertz_to_bin(source_harmony_frequency + source_harmony_frequency/32, p->input_sample_rate))), source_fft_size/2-1);
            for(ssize_t i = cut_low; i <= cut_high; i++) {
                source_noise_magnitudes[i] = 0;
            }
        }

        // Push the result of this window
        p->source_hnm_parameters.harmony_frequencies.push_back(std::move(source_harmony_frequencies));
        p->source_hnm_parameters.harmony_magnitudes.push_back(std::move(source_harmony_magnitudes));
        p->source_hnm_parameters.harmony_phases_difference.push_back(std::move(source_harmony_phases_difference));
        p->source_hnm_parameters.noise_magnitudes.push_back(std::move(source_noise_magnitudes));
    }

    return *this;
}

Synthesizer &Synthesizer::adjust_synth_params() {
    static const ssize_t sink_fft_size = 256;
    static const ssize_t sink_window_size = sink_fft_size;
    static const ssize_t sink_window_hop = sink_window_size/2;
    static const double sink_max_harmony = 8000;
    static const auto max_pillars = HNMParameters::max_pillars;
    assert((sink_window_size & 1) == 0);

    p->sink_hnm_parameters.window_size = sink_window_size;
    p->sink_hnm_parameters.window_hop = sink_window_hop;
    p->sink_hnm_parameters.first_window_mid = p->sink_buffer.left();

    double base_pitch = option_manager.get_output_pitch();

    SignalSegment sink_window = HannWindow(sink_window_size);
    TimeMapper time_mapper(option_manager);
    std::vector<double> freq_domain_mapper(p->input_sample_rate/2);

    for(auto sink_window_mid = p->sink_buffer.left(); sink_window_mid <= p->sink_buffer.right()+sink_window_size/2; sink_window_mid += sink_window_hop) {
        // Map the timestamp in sink to source
        double sink_timestamp = double(sink_window_mid) / p->output_sample_rate;
        double source_timestamp = time_mapper.map_backward(sink_timestamp, double(p->input_file_frames) / p->input_sample_rate);
        double source_timestamp_frames = source_timestamp * p->input_sample_rate;
        double source_timestamp_windows = p->source_hnm_parameters.frame_to_window_idx(source_timestamp_frames);

        // Fetch source harmonic parameters
        LinearVectorInterpolator<std::array<double, max_pillars>> frequency_interpolator;
        LinearVectorInterpolator<std::array<double, max_pillars>> magnitude_interpolator;
        LinearVectorInterpolator<std::array<std::complex<double>, max_pillars>> phase_interpolator;
        std::array<double, max_pillars> source_harmony_frequencies;
        try {
            source_harmony_frequencies = magnitude_interpolator([&](size_t index) {
                const std::array<double, max_pillars> &res = p->source_hnm_parameters.harmony_frequencies[index];
                if(res[1] == 0)
                    throw std::out_of_range("No frequency information for unvoiced phone");
                return res;
            }, p->source_hnm_parameters.harmony_frequencies.size(), source_timestamp_windows);
        } catch(std::out_of_range) {
            continue;
        }
        std::array<double, max_pillars> source_harmony_magnitudes = magnitude_interpolator([&](size_t index) {
            return p->source_hnm_parameters.harmony_magnitudes[index];
        }, p->source_hnm_parameters.harmony_magnitudes.size(), source_timestamp_windows);
        std::array<std::complex<double>, max_pillars> source_harmony_phases_difference = phase_interpolator([&](size_t index) {
            return p->source_hnm_parameters.harmony_phases_difference[index];
        }, p->source_hnm_parameters.harmony_phases_difference.size(), source_timestamp_windows);

        // Map source frequency scale to array indexes
        for(auto &i : freq_domain_mapper)
            i = 0;
        for(size_t pillar_idx = max_pillars-1; pillar_idx > 1; pillar_idx--) {
            double x1 = source_harmony_frequencies[pillar_idx-1];
            double x2 = source_harmony_frequencies[pillar_idx];
            if(x1 != x2) {
                ssize_t x1i = clamp<ssize_t>(ssize_t(std::ceil(x1)), 0, freq_domain_mapper.size()-1);
                ssize_t x2i = clamp<ssize_t>(ssize_t(std::floor(x2)), 0, freq_domain_mapper.size()-1);
                double k = 1/(x2-x1);
                for(ssize_t x = x1i; x <= x2i; x++)
                    freq_domain_mapper[x] = pillar_idx - (x2-x)*k;
            }
        }

        // Calculate sink harmony frequencies
        double sink_f0 = p->tuner.midi_id_to_freq(base_pitch + option_manager.get_pitch_bend(sink_timestamp));
        std::array<double, max_pillars> sink_harmony_frequencies {{ 0 }};
        sink_harmony_frequencies[0] = sink_f0;
        for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
            double sink_harmony_frequency = sink_f0 * pillar_idx;
            if(sink_harmony_frequency*2 >= std::min(sink_max_harmony*2, double(p->output_sample_rate)))
                break;
            sink_harmony_frequencies[pillar_idx] = sink_harmony_frequency;
        }

        // Apply a +12dB/octave filter to reduce the influence of glottal excitement
        for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
            source_harmony_magnitudes[pillar_idx] *= (
                (source_harmony_frequencies[pillar_idx]/source_harmony_frequencies[1]) *
                (source_harmony_frequencies[pillar_idx]/source_harmony_frequencies[1]));
        }

        // Calculate sink harmony magnitudes
        std::array<double, max_pillars> sink_harmony_magnitudes {{ 0 }};
        for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
            LinearVectorInterpolator<double> linear_vector_interpolator; // Better with a cubic one
            try {
                sink_harmony_magnitudes[pillar_idx] = linear_vector_interpolator([&](size_t index) {
                    return source_harmony_magnitudes[index];
                }, source_harmony_magnitudes.size(), freq_domain_mapper.at(sink_harmony_frequencies[pillar_idx]));
            } catch(std::out_of_range) {
                continue;
            }
            // Apply the -12dB/octave filter back
            sink_harmony_magnitudes[pillar_idx] *= (
                (sink_f0/sink_harmony_frequencies[pillar_idx])*
                (sink_f0/sink_harmony_frequencies[pillar_idx]));
        }
        // Amplify the magnitudes according to the magnitude of f0
        // Silly approach, but should solve most situations when f0 can roughly represent the volume
        if(sink_harmony_magnitudes[1] != 0) {
            for(size_t pillar_idx = 2; pillar_idx < max_pillars; pillar_idx++) {
                sink_harmony_magnitudes[pillar_idx] *=
                    source_harmony_magnitudes[1]/sink_harmony_magnitudes[1];
            }
        }

        // Calculate sink harmony phases
        std::array<std::complex<double>, max_pillars> sink_harmony_phases_difference {{ 0 }};
        for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
            LinearVectorInterpolator<std::complex<double>> linear_vector_interpolator;
            try {
                sink_harmony_phases_difference[pillar_idx] = linear_vector_interpolator([&](size_t index) {
                    return source_harmony_phases_difference[index];
                }, source_harmony_phases_difference.size(), freq_domain_mapper.at(sink_harmony_frequencies[pillar_idx]));
            } catch(std::out_of_range) {
                continue;
            }
        }

        // Fetch source noise parameters
        LinearVectorInterpolator<std::valarray<double>> noise_magnitude_interpolator;
        std::valarray<double> source_noise_magnitudes = noise_magnitude_interpolator([&](size_t index) {
            return p->source_hnm_parameters.noise_magnitudes[index];
        }, p->source_hnm_parameters.noise_magnitudes.size(), source_timestamp_windows);

        // Calculate sink noise magnitudes
        std::valarray<double> sink_noise_magnitudes(sink_fft_size/2);
        for(size_t bin = 1; bin < sink_fft_size/2; bin++) {
            LinearVectorInterpolator<double> linear_vector_interpolator;
            try {
                sink_noise_magnitudes[bin] = linear_vector_interpolator([&](size_t index) {
                    return source_noise_magnitudes[index];
                }, source_noise_magnitudes.size(), double(bin) * (p->source_hnm_parameters.window_size * p->output_sample_rate) / (sink_fft_size * p->input_sample_rate));
            } catch(std::out_of_range) {
                continue;
            }
        }

        // Push the result of this window
        p->sink_hnm_parameters.harmony_frequencies.push_back(std::move(sink_harmony_frequencies));
        p->sink_hnm_parameters.harmony_magnitudes.push_back(std::move(sink_harmony_magnitudes));
        p->sink_hnm_parameters.harmony_phases_difference.push_back(std::move(sink_harmony_phases_difference));
        p->sink_hnm_parameters.noise_magnitudes.push_back(std::move(sink_noise_magnitudes));
    }

    return *this;
}

Synthesizer &Synthesizer::synthesize() {
    static const ssize_t sink_fft_size = p->sink_hnm_parameters.window_size;
    static const ssize_t sink_window_size = p->sink_hnm_parameters.window_size;
    static const ssize_t sink_window_hop = p->sink_hnm_parameters.window_hop;
    static const auto max_pillars = HNMParameters::max_pillars;
    assert((sink_window_size & 1) == 0);

    WTF8::clog << "Average output sink frequency:  " << p->tuner.midi_id_to_freq(option_manager.get_output_pitch()) << " Hz" << std::endl;

    Window sink_window = HannWindow(sink_window_size);
    Spectrum sink_noise_spectrum(sink_fft_size);

    double last_sink_phase = 0;
    double last_sink_f0 = 0;
    double omega = 2 * M_PI / p->output_sample_rate;

    for(ssize_t sink_timestamp_windows = 0; sink_timestamp_windows < ssize_t(p->sink_hnm_parameters.harmony_frequencies.size()); sink_timestamp_windows++) {
        double sink_timestamp_frames = p->sink_hnm_parameters.window_to_frame_idx(sink_timestamp_windows);

        // Fetch sink harmony frequencies
        const auto &sink_harmony_frequencies = p->sink_hnm_parameters.harmony_frequencies[sink_timestamp_windows];
        double sink_f0 = sink_harmony_frequencies[0];
        last_sink_phase += (sink_f0 + last_sink_f0) * sink_window_hop * M_PI / p->output_sample_rate;

        // Fetch sink harmony magnitudes
        const auto &sink_harmony_magnitudes = p->sink_hnm_parameters.harmony_magnitudes[sink_timestamp_windows];

        // Fetch sink harmony phases
        const auto &sink_harmony_phases_difference = p->sink_hnm_parameters.harmony_phases_difference[sink_timestamp_windows];

        // Synthesis harmonies
        SignalSegment sink_segment(-sink_window_size/2, sink_window_size/2);
        for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
            double sink_harmony_frequency = sink_harmony_frequencies[pillar_idx];
            if(sink_harmony_frequency == 0)
                continue;

            double harmony_magnitude = sink_harmony_magnitudes[pillar_idx];
            double harmony_phase_angle_difference = std::arg(sink_harmony_phases_difference[pillar_idx]);
            if(!std::isfinite(harmony_phase_angle_difference)) {
                harmony_phase_angle_difference = 0;
                WTF8::cerr << "Warning: Phase construct failure at frame #" << std::round(sink_timestamp_frames) << ", harmony #" << pillar_idx << std::endl;
            }
            double harmonic_central_phase_angle = last_sink_phase*sink_harmony_frequency/sink_f0 + harmony_phase_angle_difference;

            for(auto sink_segment_idx = sink_segment.left(); sink_segment_idx < sink_segment.right(); sink_segment_idx++) {
                sink_segment[sink_segment_idx] += std::cos(
                    omega*sink_segment_idx*sink_harmony_frequency + harmonic_central_phase_angle) * harmony_magnitude;
            }
        }
        sink_segment <<= sink_segment.left();

        // Fetch sink noise magnitudes
        std::valarray<double> &sink_noise_magnitudes = p->sink_hnm_parameters.noise_magnitudes[sink_timestamp_windows];
        std::vector<std::complex<double>> &sink_noise_ifft_data = sink_noise_spectrum.get_spectrum();
        assert(ssize_t(sink_noise_magnitudes.size()) == sink_fft_size/2);
        assert(ssize_t(sink_noise_ifft_data.size()) == sink_fft_size);
        for(ssize_t bin = 1; bin < sink_fft_size/2; bin++) {
            sink_noise_ifft_data[bin] = std::polar(sink_noise_magnitudes[bin]*sink_window.sum()/4, p->fast_random() * 2 * M_PI);
        }
        for(ssize_t bin = 1; bin < sink_fft_size/2; bin++) {
            sink_noise_ifft_data[sink_fft_size-bin] = std::conj(sink_noise_ifft_data[bin]);
        }
        SignalSegment noise_segment = sink_noise_spectrum.ifft_analyze();
        sink_segment += noise_segment;

        // Window the segment and mix to the sink
        sink_segment *= sink_window;
        sink_segment >>= sink_timestamp_frames - sink_window_size/2;
        p->sink_buffer += sink_segment;

        last_sink_f0 = sink_f0;
    }

    p->sink_buffer *= option_manager.get_note_volume();
    return *this;
}

Synthesizer &Synthesizer::write_sink() {
    p->output_file.write(p->sink_buffer.buffer(), p->sink_buffer.size());

    return *this;
}

}
