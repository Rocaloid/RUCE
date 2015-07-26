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
#include <ios>
#include <sndfile.h>
#include "fast-random.hpp"
#include "option-manager.hpp"
#include "pcm-file.hpp"
#include "tuner.hpp"

namespace RUCE {

struct Synthesizer::Private {
    Tuner tuner;
    FastRandom fastrand;
    PCMFile input_file;
    PCMFile output_file;
    static const int32_t output_sample_rate = 44100;
    double output_pitch_freq;
    std::vector<float> buffer;
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

    //p->input_file.open(option_manager.get_input_file_name(), std::ios_base::in, 0, 0, 0);
    p->output_file.open(option_manager.get_output_file_name(), std::ios_base::out, SF_FORMAT_WAV | SF_FORMAT_DOUBLE, 1, p->output_sample_rate);

    size_t sample_count = size_t(std::ceil(option_manager.get_required_length() * p->output_sample_rate));
    p->buffer = std::vector<float>(sample_count);

    p->output_pitch_freq = p->tuner.midi_id_to_freq(uint8_t(option_manager.get_output_pitch()));

    return *this;
}

Synthesizer &Synthesizer::synth_unit() {
    // STUB

    double pitch = option_manager.get_output_pitch();
    static const size_t pillars = 13;
    double phrases[2][pillars] = { 0 };
    size_t i = 0;
    for(auto &s : p->buffer) {
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
        i++;
    }
    p->output_file.write(p->buffer.data(), p->buffer.size());

    return *this;
}

}
