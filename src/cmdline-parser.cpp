/*
  Rocaloid
  Copyright (C) 2015 StarBrilliant <m13253@hotmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cmdline-parser.hpp"
#include <cstdlib>
#include <vector>
#include <libwintf8/termio.h>
#include <libwintf8/u8str.h>
#include "option-manager.hpp"

namespace RUCE {

CmdlineParser::CmdlineParser(OptionManager &option_manager) {
}

void CmdlineParser::ParseArgv(const std::vector<WTF8::u8string> &argv) {
    if(argv.size() >= 5) {
        LogArgv(argv);
    } else {
        PrintHelp(argv[0]);
        std::exit(argv.size() == 1 ? 0 : 1);
    }
}

void CmdlineParser::PrintHelp(const WTF8::u8string &argv0) {
    WTF8::cerr << "Usage: "
               << argv0
               << "<input file> <output file> <pitch percent> <velocity> [<flags> [<offset> <length require> [<fixed length> [<end blank> [<volume> [<modulation> [<pitch bend>...]]]]]]]"
               << std::endl << std::endl
               << "For more information, see http://shinta0806be.ldblog.jp/archives/8298940.html"
               << std::endl << std::endl;
}

void CmdlineParser::LogArgv(const std::vector<WTF8::u8string> &argv) {
    WTF8::clog << "Args:";
    for(const auto &argi : argv) {
        WTF8::clog << ' ' << argi;
    }
    WTF8::clog << std::endl;
}

}

