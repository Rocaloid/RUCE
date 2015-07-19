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

#include "synthesizer.hpp"
#include <cmath>
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

void Synthesizer::check_params() {
    // STUB

    if(option_manager.get_required_length_msec() < 0) {
        option_manager.set_required_length_msec(1000);
    }
}

void Synthesizer::prepare() {
    // STUB

    //p->input_file.open(option_manager.get_input_file_name(), std::ios_base::in, 0, 0, 0);
    p->output_file.open(option_manager.get_output_file_name(), std::ios_base::out, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, p->output_sample_rate);

    size_t milli_sample_count = size_t(option_manager.get_required_length_msec()) * p->output_sample_rate;
    size_t sample_count = milli_sample_count / 1000;
    if(sample_count*1000 + 500 <= milli_sample_count) // Round
        sample_count++;
    p->buffer = std::vector<float>(sample_count);

    p->output_pitch_freq = p->tuner.midi_id_to_freq(uint8_t(option_manager.get_output_pitch()));
}

void Synthesizer::synth_unit() {
    // STUB

    double pitch = option_manager.get_output_pitch();
    double phrases[13] = { 0 };
    size_t i = 0;
    for(auto &s : p->buffer) {
        s = p->fastrand() / 16;
        double instant_pitch = pitch + option_manager.get_pitch_bend(double(i) / p->output_sample_rate);
        for(size_t j = 1; j < 13; j++) {
            phrases[j] += 2.0 * j * p->tuner.midi_id_to_freq(instant_pitch + p->fastrand()*4 - 2);
        }
        static const double intensity[13] = { 0, 1, 1, 1, 1.0/8, 1.0/64, 1.0/64, 1.0/64, 1.0/8, 1.0/8, 1.0/8, 1.0/8, 1.0/64 };
        for(size_t j = 1; j < 13; j++) {
            s += std::sin(M_PI * phrases[j] / p->output_sample_rate) * intensity[j];
        }
        s *= option_manager.get_note_volume()/8;
        i++;
    }
    p->output_file.write(p->buffer.data(), p->buffer.size());
}

}

