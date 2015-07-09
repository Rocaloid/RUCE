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

#include "cmdline-parser.hpp"
#include <cstdlib>
#include <vector>
#include <libwintf8/termio.h>
#include <libwintf8/u8str.h>
#include "option-manager.hpp"

namespace RUCE {

CmdlineParser::CmdlineParser(OptionManager &option_manager) : 
    mOptionManager(option_manager) {
}

void CmdlineParser::parse_argv(const std::vector<WTF8::u8string> &argv) {
    if(argv.size() >= 5) {
        log_argv(argv);
        analyze_argv(argv);
    } else {
        print_help(argv[0]);
        std::exit(argv.size() == 1 ? 0 : 1);
    }
}

void CmdlineParser::print_help(const WTF8::u8string &argv0) {
    WTF8::cerr << "Usage: "
               << argv0
               << " <input file> <output file> <pitch percent> <velocity> [<flags> [<offset> <length require> [<fixed length> [<end blank> [<volume> [<modulation> [<pitch bend>...]]]]]]]"
               << std::endl << std::endl
               << "For more information, see http://shinta0806be.ldblog.jp/archives/8298940.html"
               << std::endl << std::endl;
}

void CmdlineParser::log_argv(const std::vector<WTF8::u8string> &argv) {
    WTF8::clog << "Args:";
    for(const auto &argi : argv) {
        WTF8::clog << ' ' << argi;
    }
    WTF8::clog << std::endl;
}

void CmdlineParser::analyze_argv(const std::vector<WTF8::u8string> &argv) {
    const unsigned STATE_BINARY = 0;
    const unsigned STATE_INPUT_FILE = 1;
    unsigned state = STATE_BINARY;

    for(const auto &argi : argv) {
    	switch(state)
	{
	    case STATE_BINARY:
	        // just drop it
		state++;
		break;
	    case STATE_INPUT_FILE:
	        mOptionManager.m_input_file = argi;
		WTF8::clog << "Input File: " << argi << std::endl;
		state++;
		break;
	    default:
	        WTF8::clog << "Dropped paramater: " << argi << std::endl;
	}
    }
}

}

