/*
    Rocaloid
    Copyright (C) <YEAR> <YOUR NAME>

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
    void check_params();
    void prepare();
    void synth_unit();
protected:
    OptionManager &option_manager;
private:
    struct Private;
    proxy_ptr<Private> p;
};

}

#endif
