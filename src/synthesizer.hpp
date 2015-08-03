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

#ifndef RUCE_SYNTHESIZER_HPP
#define RUCE_SYNTHESIZER_HPP

#include "option-manager.hpp"
#include "proxy-ptr.hpp"

namespace RUCE {

class Synthesizer {
public:
    Synthesizer(OptionManager &option_manager);
    ~Synthesizer();
    Synthesizer &check_params();
    Synthesizer &prepare();
    Synthesizer &read_source();
    Synthesizer &track_f0();
    Synthesizer &analyze();
    Synthesizer &adjust_synth_params();
    Synthesizer &synthesize();
    Synthesizer &write_sink();
protected:
    OptionManager &option_manager;
private:
    struct Private;
    proxy_ptr<Private> p;
};

}

#endif
