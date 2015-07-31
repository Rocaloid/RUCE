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

#ifndef RUCE_TIME_MAPPER_HPP
#define RUCE_TIME_MAPPER_HPP

#include "option-manager.hpp"

namespace RUCE {

class TimeMapper {
public:
    TimeMapper(OptionManager &option_manager);
    double map_consonant_length();
    double map_forward(double time_in_source, double input_file_length);
    double map_backward(double time_in_sink, double input_file_length);
protected:
    OptionManager &option_manager;
private:
    double cached_consonant_length = -1;
};

}

#endif
