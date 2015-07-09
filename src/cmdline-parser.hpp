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

#ifndef RUCE_CMDLINE_PARSER_HPP
#define RUCE_CMDLINE_PARSER_HPP

#include <vector>
#include <libwintf8/u8str.h>
#include "option-manager.hpp"

namespace RUCE {

/**
 * Parse command line arguments, check errors and store them
 */
class CmdlineParser {
public:
    CmdlineParser(OptionManager &option_manager);
    void ParseArgv(const std::vector<WTF8::u8string> &argv);
    void PrintHelp(const WTF8::u8string &argv0);
private:
    void LogArgv(const std::vector<WTF8::u8string> &argv);
    void AnalyseArgv(const std::vector<WTF8::u8string> &argv);
};

}

#endif
