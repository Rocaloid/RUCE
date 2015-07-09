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

#ifndef RUCE_OPTION_MANAGER_HPP
#define RUCE_OPTION_MANAGER_HPP

#include <libwintf8/u8str.h>

namespace RUCE {

/**
 * Store global status
 */
class OptionManager {
public:
    OptionManager();
    WTF8::u8string GetInputFile();
private:
    WTF8::u8string mInputFile;
    friend class CmdlineParser;
    static int mOptionManagerCount;
};

}

#endif
