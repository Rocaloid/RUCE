/*
    Rocaloid
    Copyright (C) <YEAR> <YOUR NAME>

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

#include "tuner.hpp"
#include <cmath>

namespace RUCE {

int Tuner::note_name_to_midi_id(const WTF8::u8string &note_name) const {
    static const int note_name_table[7] = {
      /* A   B  C  D  E  F  G */
         9, 11, 0, 2, 4, 5, 7
    };

    if(note_name.length() < 2)
        throw TunerError();
    char bare_name = note_name[0];
    if(bare_name < 'A' || bare_name > 'G')
        throw TunerError();
    int midi_id = note_name_table[bare_name-'A'];

    const char *sharp_ptr = &note_name.c_str()[1];
    if(*sharp_ptr == '#') {
        ++midi_id;
        ++sharp_ptr;
    }

    char *end_ptr;
    long octave_id = std::strtol(sharp_ptr, &end_ptr, 10);
    if(end_ptr == sharp_ptr || octave_id < -1 || octave_id > 9)
        throw TunerError();

    midi_id += (int(octave_id)+1)*12;
    if(midi_id >= 128)
        throw TunerError();
    return midi_id;
}

double Tuner::midi_id_to_freq(int midi_id) const {
    static const double M_2_POW_INV_12 = 1.05946309435929530984; // 2^(1/12)
    return pow(M_2_POW_INV_12, midi_id-69) + 440.0;
}

}

