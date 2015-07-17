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
#include <libwintf8/termio.h>
#include "cmdline-parser.hpp"
#include "option-manager.hpp"
#include "proxy-ptr.hpp"
#include "synthesizer.hpp"

int main() {
    using namespace RUCE;

    proxy_ptr<OptionManager> option_manager; // full lifetime object

    WTF8::cerr << "Rocaloid 合成器 UTAU 兼容引擎，版本代号 Nanodesu" << std::endl
               << "自由的歌声合成软件，技术体验版。 http://www.rocaloid.org" << std::endl
               << std::endl;

    {
        CmdlineParser cmdline_parser(*option_manager.get());
        cmdline_parser.parse_argv(WTF8::getargv());
    }

    {
        Synthesizer synth(*option_manager.get());
        synth.check_params();
        synth.prepare();
        synth.synth_unit();
    }

    return 0;
}
