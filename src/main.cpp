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

#include <iostream>
#include <libwintf8/argv.h>
#include <libwintf8/initcon.h>
#include <libwintf8/termio.h>
#include "cmdline-parser.hpp"
#include "option-manager.hpp"

int main() {
    using namespace RUCE;

    OptionManager option_manager; // full lifetime object

    WTF8::set_console_font();
    WTF8::cerr << "Rocaloid synthesizer, version Nanodesu" << std::endl << std::endl;

    {
        CmdlineParser cmdline_parser(option_manager);
        cmdline_parser.parse_argv(WTF8::getargv());
    }

    return 0;
}
