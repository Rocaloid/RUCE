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
#include "vector-interpolator.hpp"
#include "window.hpp"
#include "wrapped-angle.hpp"

namespace RUCE {

struct Synthesizer::Private {
    PCMFile input_file;
    PCMFile output_file;
    int32_t input_sample_rate;
    int32_t output_sample_rate;
    int64_t input_file_frames;
    int64_t output_file_frames;
    SignalSegment source_buffer;
    SignalSegment sink_buffer;
    HNMParameters hnm_parameters;
    Tuner tuner;
    F0Tracker f0_tracker;
    FastRandom fastrand;
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
    WTF8::clog << "Average output sink frequency:  " << p->tuner.midi_id_to_freq(option_manager.get_output_pitch()) << " Hz" << std::endl;
/*
    WTF8::clog << "F0:";
    for(const auto &i : p->f0_tracker.get_result()) {
        WTF8::clog << ' ' << i;
    }
    WTF8::clog << std::endl;
*/

    return *this;
}

Synthesizer &Synthesizer::analyze() {
    static const ssize_t source_window_size = 1024;
    static const ssize_t source_window_hop = source_window_size/2;
    static const size_t max_pillars = 128;

    p->hnm_parameters.analysis_window_size = source_window_size;
    p->hnm_parameters.analysis_window_hop = source_window_hop;
    p->hnm_parameters.max_pillars = max_pillars;
    p->hnm_parameters.first_window_mid = p->source_buffer.left();

    SignalSegment source_window = HannWindow(source_window_size);
    Spectrum source_spectrum(Spectrum::next_pow2(source_window_size));

    for(auto source_window_mid = p->source_buffer.left(); source_window_mid < p->source_buffer.right()+source_window_size/2; source_window_mid += source_window_hop) {
        double source_f0 = p->f0_tracker.get_result(source_window_mid);

        // Copy the source and window it
        SignalSegment source_segment(p->source_buffer, source_window_mid-source_window_size/2, source_window_mid+source_window_size/2);
        source_segment <<= source_segment.left();
        source_segment *= source_window;

        // Do FFT analysis
        source_segment = source_spectrum.fftshift(source_segment);
        source_spectrum.fft_analyze(source_segment);

        // Omit peak finding
        for(size_t pillar_idx = 0; pillar_idx < max_pillars; pillar_idx++) {
        }
    }

    return *this;
}

Synthesizer &Synthesizer::adjust_synth_params() {
    // STUB

    return *this;
}

Synthesizer &Synthesizer::synthesize() {
    static const ssize_t source_window_size = 1024;
    static const ssize_t sink_window_size = 1024;
    static const size_t max_pillars = 128;
    assert((source_window_size & 1) == 0);
    assert((sink_window_size & 1) == 0);

    SignalSegment source_segment;
    SignalSegment sink_segment;
    SignalSegment source_window = HannWindow(source_window_size);
    SignalSegment sink_window = HannWindow(sink_window_size);
    Spectrum source_spectrum(Spectrum::next_pow2(source_window_size));
    TimeMapper time_mapper(option_manager);

    double base_pitch = option_manager.get_output_pitch();
    double last_sink_phase = 0;
    double last_sink_f0 = 0;
    ssize_t sink_window_hop = sink_window_size/2;

    for(auto sink_window_mid = p->sink_buffer.left(); sink_window_mid < p->sink_buffer.right()+sink_window_size/2; sink_window_mid += sink_window_hop) {
        double sink_timestamp = double(sink_window_mid) / p->output_sample_rate;
        double source_timestamp = time_mapper.map_backward(sink_timestamp, double(p->input_file_frames) / p->input_sample_rate);
        ssize_t sink_timestamp_frames = sink_window_mid;
        ssize_t source_timestamp_frames = ssize_t(std::round(source_timestamp * p->input_sample_rate));
        double sink_f0 = p->tuner.midi_id_to_freq(base_pitch + option_manager.get_pitch_bend(sink_timestamp));
        double source_f0 = p->f0_tracker.get_result(source_timestamp_frames);

        // Copy the source and window it
        source_segment = SignalSegment(p->source_buffer, source_timestamp_frames-source_window_size/2, source_timestamp_frames+source_window_size/2);
        source_segment <<= source_segment.left();
        source_segment *= source_window;

        // Do FFT analysis
        source_segment = source_spectrum.fftshift(source_segment);
        source_spectrum.fft_analyze(source_segment);
        auto source_magnitude = source_spectrum.get_magnitude();
        auto source_phase = source_spectrum.get_phase();

        // Convert magnitude to log scale
        for(double &mag : source_magnitude)
            mag = mag > 0 ? std::log10(mag / sink_window_size) : -HUGE_VAL;

        // Extract sinusold parameters
        std::array<double, max_pillars> pillar_magnitude {{ 0 }};
        std::array<double, max_pillars> pillar_phase {{ 0 }};
        for(size_t pillar_idx = 0; pillar_idx < std::min(size_t(p->input_sample_rate/(source_f0*2)), max_pillars); pillar_idx++) {
            double harmonic_freq = source_f0 * pillar_idx;
            static QuadraticVectorInterpolator<double> quadratic_vector_interpolator;
            try {
                pillar_magnitude[pillar_idx] = std::pow(10, quadratic_vector_interpolator([&](size_t index) {
                    return source_magnitude[index];
                }, source_magnitude.size(), source_spectrum.hertz_to_bin(harmonic_freq, p->input_sample_rate)));
            } catch(std::out_of_range) {
            }
            static LinearVectorInterpolator<WrappedAngle> linear_vector_interpolator;
            try {
                pillar_phase[pillar_idx] = linear_vector_interpolator([&](size_t index) {
                    return source_phase[index];
                }, source_phase.size(), source_spectrum.hertz_to_bin(harmonic_freq, p->input_sample_rate));
            } catch(std::out_of_range) {
            }
        }
        for(size_t pillar_idx = 1; pillar_idx < max_pillars; pillar_idx++) {
            pillar_phase[pillar_idx] -= pillar_phase[0];
        }
        pillar_phase[0] = 0;
        pillar_phase.fill(0); // Debug

        // Construct harmonics
        sink_segment = SignalSegment(-sink_window_size/2, sink_window_size/2);
        last_sink_phase += (sink_f0 + last_sink_f0) * sink_window_hop * M_PI / p->output_sample_rate;
        for(auto sink_segment_idx = sink_segment.left(); sink_segment_idx < sink_segment.right(); sink_segment_idx++) {
            for(size_t pillar_idx = 0; pillar_idx < std::min(size_t(p->output_sample_rate/(sink_f0*2)), max_pillars); pillar_idx++) {
                if(sink_f0*pillar_idx*2 >= p->output_sample_rate)
                    break;
                double omega = 2 * M_PI * sink_f0 / p->output_sample_rate;
                sink_segment[sink_segment_idx] += std::sin((omega*sink_segment_idx + last_sink_phase) * pillar_idx + pillar_phase[pillar_idx]) * pillar_magnitude[pillar_idx];
            }
        }

        // Window the segment and mix to the sink
        sink_segment <<= sink_segment.left();
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
