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

#include "time-mapper.hpp"
#include <cmath>
#include <algorithm>
#include "option-manager.hpp"

namespace RUCE {

TimeMapper::TimeMapper(OptionManager &option_manager) :
    option_manager(option_manager) {
}

/**
 * Map UTAU note velocity to consonant length
 *
 * 0 => 2.0
 * 1 => 1.0
 * 2 => 0.5
 */
double TimeMapper::map_consonant_length() {
    if(cached_consonant_length < 0) {
        cached_consonant_length = std::max(option_manager.get_consonant_length() * 2 / std::pow(2, option_manager.get_note_velocity()), 0.0);
    }
    return cached_consonant_length;
}

/**
 * Map output timestamp to input timestamp
 *
 *                       0 => left_blank
 * mapped_consonant_length => left_blank + consonant_length
 *         required_length => input_file_length - right_blank
 */
double TimeMapper::map_backward(double time_in_sink, double input_file_length) {
    double mapped_consonant_length = map_consonant_length();
    double x1, x2, y1, y2;
    if(time_in_sink <= mapped_consonant_length) {
        x1 = 0;
        x2 = mapped_consonant_length;
        y1 = option_manager.get_left_blank();
        y2 = option_manager.get_left_blank() + option_manager.get_consonant_length();
    } else {
        x1 = mapped_consonant_length;
        x2 = option_manager.get_required_length();
        y1 = option_manager.get_left_blank() + option_manager.get_consonant_length();
        y2 = input_file_length - option_manager.get_right_blank();
    }
    if(x1 != x2) {
        double k = (y2-y1) / (x2-x1);
        return (time_in_sink-x1)*k + y1;
    } else {
        return (y1+y2) / 2;
    }
}

/**
 * Map input timestamp to output timestamp
 *
 *                      left_blank => 0
 *   left_blank + consonant_length => mapped_consonant_length
 * input_file_length - right_blank => required_length
 */
double TimeMapper::map_forward(double time_in_source, double input_file_length) {
    double left_blank = option_manager.get_left_blank();
    double consonant_length = option_manager.get_consonant_length();
    double x1, x2, y1, y2;
    if(time_in_source <= left_blank + consonant_length) {
        x1 = left_blank;
        x2 = left_blank + consonant_length;
        y1 = 0;
        y2 = map_consonant_length();
    } else {
        x1 = left_blank + consonant_length;
        x2 = input_file_length - option_manager.get_right_blank();
        y1 = map_consonant_length();
        y2 = option_manager.get_required_length();
    }
    if(x1 != x2) {
        double k = (y2-y1) / (x2-x1);
        return (time_in_source-x1)*k + y1;
    } else {
        return (y1+y2) / 2;
    }
}

}
