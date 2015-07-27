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
#include "utils.hpp"

namespace RUCE {

struct F0Tracker::Private{
    std::vector<float> output;
    ssize_t frame_hop = 0;
    static void callback(void *callback_param, float result);
};

F0Tracker::F0Tracker() {
}

F0Tracker::~F0Tracker() {
}

F0Tracker &F0Tracker::track(const SignalSegment &segment, int32_t sample_rate, ssize_t frame_hop, double min_freq, double max_freq, double threshold) {
    assert(frame_hop > 0);
    p->frame_hop = 0;
    auto length = segment.size();
    const auto &segment_buffer = segment.buffer();
    std::vector<float_list> input_vector(length);
    auto input_list = input_vector.data();
    for(auto i = 0; i < length; i++) {
        input_list[i].f = segment_buffer[i] * 65535;
        input_list[i].next = &input_list[i+1];
    }
    input_list[length - 1].next = nullptr;
    p->output.clear();
    rapt(input_list, length, sample_rate * 0.001, frame_hop, min_freq, max_freq, threshold, 1, p->callback, &p->output);
    return *this;
}

const std::vector<float> &F0Tracker::get_result() const {
    assert(p->frame_hop > 0);
    return p->output;
}

float F0Tracker::get_result(ssize_t sample_idx) const {
    assert(p->frame_hop > 0);
    ssize_t frame_idx = sample_idx / p->frame_hop;
    if(frame_idx >= 0 && frame_idx < ssize_t(p->output.size()))
        return p->output[frame_idx];
    else
        return 0;
}

void F0Tracker::Private::callback(void *callback_param, float result) {
    reinterpret_cast<decltype (F0Tracker::Private::output) *>(callback_param)->push_back(result);
}

}
