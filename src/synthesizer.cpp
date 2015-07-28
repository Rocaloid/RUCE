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
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ios>
#include <iostream>
#include <libwintf8/termio.h>
#include <sndfile.h>
#include "fast-random.hpp"
#include "f0-tracker.hpp"
#include "option-manager.hpp"
#include "pcm-file.hpp"
#include "signal-segment.hpp"
#include "tuner.hpp"

namespace RUCE {

struct Synthesizer::Private {
    PCMFile input_file;
    PCMFile output_file;
    int32_t input_sample_rate;
    int32_t output_sample_rate;
    int64_t input_file_frames;
    int64_t output_file_frames;;
    SignalSegment source_buffer;
    SignalSegment sink_buffer;
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
    // STUB

    if(option_manager.get_required_length() < 0) {
        option_manager.set_required_length(1);
    }

    return *this;
}

Synthesizer &Synthesizer::prepare() {
    // STUB

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
    int64_t left_bound = int64_t(option_manager.get_left_blank() * p->input_sample_rate);
    int64_t right_bound = p->output_file_frames - int64_t(option_manager.get_right_blank() * p->input_sample_rate);
    p->source_buffer = SignalSegment(left_bound, right_bound);
    p->input_file.seek(left_bound, SEEK_SET);
    p->input_file.read(p->source_buffer.buffer(), p->source_buffer.size());

    return *this;
}

Synthesizer &Synthesizer::track_f0() {
    p->f0_tracker.track(p->source_buffer, p->input_sample_rate);
    WTF8::clog << "F0:";
    for(const auto &i : p->f0_tracker.get_result()) {
        WTF8::clog << ' ' << i;
    }
    WTF8::clog << std::endl;

    return *this;
}

Synthesizer &Synthesizer::synth_unit() {
    // STUB

    double pitch = option_manager.get_output_pitch();
    static const size_t pillars = 13;
    double phrases[2][pillars] = { 0 };
    for(auto i = p->sink_buffer.left(); i < p->sink_buffer.right(); i++) {
        auto &s = p->sink_buffer[i];
        s = p->fastrand() / 16;
        double instant_pitch = pitch + option_manager.get_pitch_bend(double(i) / p->output_sample_rate);
        for(size_t j = 1; j < pillars; j++) {
            static const double am_freq[pillars] = {
                0, 1.9, 7.6, 3.8, 13.3, 15.4, 14.2, 14.1, 7.9, 7.6, 50.4, 10.7, 19.8
            };
            phrases[0][j] += 2.0 * j * p->tuner.midi_id_to_freq(instant_pitch + p->fastrand()*2 - 1) - am_freq[j];
            phrases[1][j] += 2.0 * j * p->tuner.midi_id_to_freq(instant_pitch + p->fastrand()*2 - 1) + am_freq[j];
        }
        static const double intensity[2][pillars] = {
            { 0, 1.1/2, 1.2/2, 1.5/4, 1.3/16, 1.3/128, 1.3/128, 1.3/128, 1.2/16, 1.1/16, 1.2/16, 1.1/16, 1.2/128 },
            { 0, 0.9/2, 0.8/2, 0.5/4, 0.3/16, 0.3/128, 0.3/128, 0.3/128, 0.8/16, 0.9/16, 0.8/16, 0.9/16, 0.8/128 }
        };
        for(size_t j = 1; j < pillars; j++) {
            s += std::sin(M_PI * phrases[0][j] / p->output_sample_rate) * intensity[0][j];
            s += std::cos(M_PI * phrases[1][j] / p->output_sample_rate) * intensity[1][j];
        }
        s *= option_manager.get_note_volume()/8;
    }
    p->output_file.write(p->sink_buffer.buffer(), p->sink_buffer.size());

    return *this;
}

}
