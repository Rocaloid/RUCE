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

#ifndef RUCE_OPTION_MANAGER_HPP
#define RUCE_OPTION_MANAGER_HPP

#include <libwintf8/u8str.h>

namespace RUCE {

/**
 * Store global status
 */
class OptionManager {
    friend class CmdlineParser;
public:
    OptionManager();
    WTF8::u8string get_input_file_name() const { return input_file_name; }
    WTF8::u8string get_output_file_name() const { return output_file_name; }
    int get_output_pitch() const { return output_pitch; }
protected:
    WTF8::u8string input_file_name;
    WTF8::u8string output_file_name;
    int output_pitch;
};

}

#endif
