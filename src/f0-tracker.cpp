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

#include "f0-tracker.hpp"
#include <cassert>
#include <SPTK-extracted.h>
#include "signal-segment.hpp"
#include "spectrum.hpp"
#include "utils.hpp"
#include "vector-interpolator.hpp"
#include "window.hpp"

namespace RUCE {

struct F0Tracker::Private {
    std::vector<float> output;
    ssize_t frame_hop = 0;
    ssize_t left_bound = 0;
    int32_t sample_rate;
    void optimize(const SignalSegment &segment);
    static void callback(void *callback_param, float result);
};

F0Tracker::F0Tracker() {
}

F0Tracker::~F0Tracker() {
}

F0Tracker &F0Tracker::track(const SignalSegment &segment, int32_t sample_rate, ssize_t frame_hop, double min_freq, double max_freq, double threshold) {
    assert(frame_hop > 0);
    p->frame_hop = frame_hop;
    p->left_bound = segment.left();
    p->sample_rate = sample_rate;
    auto length = segment.size();
    const auto &segment_buffer = segment.buffer();
    std::vector<float_list> input_vector(length);
    auto input_list = input_vector.data();
    for(auto i = 0; i < length; i++) {
        input_list[i].f = segment_buffer[i] * 65535; // RAPT requires [-65536, 65535]
        input_list[i].next = &input_list[i+1];
    }
    input_list[length - 1].next = nullptr;
    p->output.clear();
    //rapt(input_list, length, sample_rate, frame_hop, min_freq, max_freq, threshold, 1, p->callback, &p->output);
    swipe(input_list, length, sample_rate, frame_hop, min_freq, max_freq, threshold, 1, p->callback, &p->output);

    p->optimize(segment);
    return *this;
}

const std::vector<float> &F0Tracker::get_result() const {
    assert(p->frame_hop > 0);
    return p->output;
}

float F0Tracker::get_result(ssize_t sample_idx) const {
    assert(p->frame_hop > 0);
    sample_idx -= p->left_bound;
    sample_idx += p->frame_hop/2;
    ssize_t frame_idx = sample_idx / p->frame_hop;
    if(frame_idx >= 0 && frame_idx < ssize_t(p->output.size()))
        return p->output[frame_idx];
    else
        return 0;
}

void F0Tracker::Private::optimize(const SignalSegment &segment) {
    auto window_size = 2048;//Spectrum::next_pow2(sample_rate/50);
    Spectrum spectrum(window_size);

    for(size_t window_id = 0; window_id < output.size(); window_id++) {
        SignalSegment window = HannWindow(window_size);

        SignalSegment windowed_segment(segment, left_bound + window_id*frame_hop - window_size/2, left_bound + window_id*frame_hop + window_size/2);
        windowed_segment <<= windowed_segment.left();
        windowed_segment *= window;
        spectrum.fftshift(windowed_segment);
        spectrum.fft_analyze(windowed_segment);
        std::vector<double> magnitude = spectrum.get_magnitude();

        // Find f0 with optimal peak finding
        double f0_hertz = output[window_id];
        if(f0_hertz == 0)
            continue;
        ssize_t f0_bin_low = std::max(ssize_t(std::round(spectrum.hertz_to_bin(f0_hertz - f0_hertz/3, sample_rate))), ssize_t(1));
        ssize_t f0_bin_high = std::min(ssize_t(std::round(spectrum.hertz_to_bin(f0_hertz + f0_hertz/3, sample_rate))), ssize_t(window_size/2-1));
        ssize_t f0_bin = size_t(std::round(spectrum.hertz_to_bin(f0_hertz, sample_rate)));
        double f0_magnitude = 0; // The value of the highest bin
        for(ssize_t bin = f0_bin_low; bin <= f0_bin_high; bin++) {
            if(magnitude[bin] > f0_magnitude) {
                f0_bin = bin;
                f0_magnitude = magnitude[bin];
            }
        }
        std::complex<double> phase0 = spectrum.get_phase()[f0_bin];

        // Move the window backward by 1 sample
        static const ssize_t move_step = 1;
        windowed_segment = SignalSegment(segment, left_bound + window_id*frame_hop - window_size/2 - move_step, left_bound + window_id*frame_hop + window_size/2 - move_step);
        windowed_segment <<= windowed_segment.left();
        windowed_segment *= window;
        spectrum.fftshift(windowed_segment);
        spectrum.fft_analyze(windowed_segment);
        std::complex<double> phase1 = spectrum.get_phase()[f0_bin];

        double f0_hertz_corrected = (std::arg(phase0)-std::arg(phase1)) * sample_rate / (2 * M_PI) / move_step;
        if(f0_hertz_corrected < f0_hertz - f0_hertz/3 || f0_hertz_corrected <= f0_hertz + f0_hertz/3) {
            if(f0_bin > 0) {
                // Use alternative correction method
                double y_1 = magnitude[f0_bin-1];
                double y0 = magnitude[f0_bin];
                double y1 = magnitude[f0_bin+1];
                double subbin_correction = clamp((y_1-y1)/(2*y1-4*y0+2*y_1), -1.0, 1.0);
                f0_hertz_corrected = spectrum.bin_to_hertz(f0_bin + subbin_correction, sample_rate);
            } else {
                f0_hertz_corrected = f0_hertz;
            }
        }
        output[window_id] = f0_hertz_corrected;
    }
}

void F0Tracker::Private::callback(void *callback_param, float result) {
    reinterpret_cast<decltype (F0Tracker::Private::output) *>(callback_param)->push_back(result);
}

}
