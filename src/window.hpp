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

#ifndef RUCE_WINDOW_HPP
#define RUCE_WINDOW_HPP

#include "signal-segment.hpp"

namespace RUCE {

class Window : public SignalSegment {
protected:
    Window(size_t size) : SignalSegment(size) {}
};

class HannWindow : public Window {
public:
    HannWindow(size_t size);
};

class BlackmanHarrisWindow : public Window {
public:
    BlackmanHarrisWindow(size_t size);
};

}

#endif
