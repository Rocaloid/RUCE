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
#include "tuner.hpp"
#include "utils.hpp"

namespace RUCE {

CmdlineParser::CmdlineParser(OptionManager &option_manager) : 
    option_manager(option_manager) {
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

void CmdlineParser::print_help(const WTF8::u8string &argv0) const {
    WTF8::cerr << "Usage: "
               << argv0
               << " <input file> <output file> <pitch percent> <velocity> [<flags> [<offset> <length require> [<fixed length> [<end blank> [<volume> [<modulation> [<pitch bend>...]]]]]]]"
               << std::endl << std::endl
               << "For more information, see http://shinta0806be.ldblog.jp/archives/8298940.html"
               << std::endl << std::endl;
}

void CmdlineParser::log_argv(const std::vector<WTF8::u8string> &argv) const {
    WTF8::clog << "Args:";
    for(const auto &argi : argv) {
        WTF8::clog << ' ' << argi;
    }
    WTF8::clog << std::endl;
}

void CmdlineParser::analyze_argv(const std::vector<WTF8::u8string> &argv) {
    option_manager.input_file_name = argv[1];
    option_manager.output_file_name = argv[2];
    try {
        option_manager.output_pitch = tuner.note_name_to_midi_id(argv[3]);
    } catch(Tuner::TunerError) {
        WTF8::cerr << "ERROR: Invalid note name: \"" << argv[3] << '"' << std::endl;
        std::exit(1);
    }
    auto argc = argv.size();
    size_t argi = 4;
    try {
        option_manager.note_velocity = strtonum(std::strtod, argv[4].c_str())/100;
        ++argi;
        option_manager.synth_flags = argc > 5 ? argv[5] : "";
        ++argi;
        option_manager.left_blank = argc > 6 ? strtonum(std::strtod, argv[6].c_str())/1000 : 0;
        ++argi;
        option_manager.required_length = argc > 7 ? strtonum(std::strtod, argv[7].c_str())/1000 : -1;
        ++argi;
        option_manager.vowel_length = argc > 8 ? strtonum(std::strtod, argv[8].c_str())/1000 : 0;
        ++argi;
        option_manager.right_blank = argc > 9 ? strtonum(std::strtod, argv[9].c_str())/1000 : 0;
        ++argi;
        option_manager.note_volume = argc > 10 ? strtonum(std::strtod, argv[10].c_str())/100 : 1;
        ++argi;
        option_manager.note_modulation = argc > 11 ? strtonum(std::strtod, argv[11].c_str())/100 : 0;
        ++argi;
        option_manager.tempo = 120;
        if(argc > 12) {
            const auto &argv12 = argv[12];
            if(argv12.length() != 0 && argv12[0] == '!')
                option_manager.tempo = strtonum(std::strtod, &argv[12].c_str()[1]);
        }
        option_manager.pitch_bend_str = argc > 13 ? argv[13] : "";
    } catch(StrToNumError) {
        WTF8::cerr << "ERROR: Invalid argument #" << argi << ": \"" << argv[argi] << '"' << std::endl;
        std::exit(1);
    }
}

}

