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

#ifndef RUCE_CMDLINE_PARSER_HPP
#define RUCE_CMDLINE_PARSER_HPP

#include <vector>
#include <libwintf8/u8str.h>
#include "option-manager.hpp"
#include "tuner.hpp"

namespace RUCE {

/**
 * Parse command line arguments, check errors and store them
 */
class CmdlineParser {
public:
    CmdlineParser(OptionManager &option_manager);
    void parse_argv(const std::vector<WTF8::u8string> &argv);
    void print_help(const WTF8::u8string &argv0) const;
protected:
    OptionManager &option_manager;
    Tuner tuner;
private:
    void log_argv(const std::vector<WTF8::u8string> &argv) const;
    void analyze_argv(const std::vector<WTF8::u8string> &argv);
    int16_t decode_pitch_bend(const char pitch_bend_str[2]) const;
};

}

#endif
