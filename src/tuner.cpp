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

double Tuner::midi_id_to_freq(uint8_t midi_id) const {
    static const double M_2_POW_INV_12 = 1.05946309435929530984; // 2^(1/12)
    return pow(M_2_POW_INV_12, int(midi_id)-69) + 440.0;
}

}

