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

// Define M_PI etc
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "window.hpp"
#include <cmath>
#include "signal-segment.hpp"

namespace RUCE {

HannWindow::HannWindow(size_t size) :
    Window(size) {
    auto window_buffer = buffer();
    double a = M_PI / (size-1);
    for(size_t i = 0; i < size; i++) {
        double b = std::sin(a * i);
        window_buffer[i] = b * b;
    }
}

BlackmanHarrisWindow::BlackmanHarrisWindow(size_t size) :
    Window(size) {
    auto window_buffer = buffer();
    static const double a0 = 0.35875;
    static const double a1 = 0.48829;
    static const double a2 = 0.14128;
    static const double a3 = 0.01168;
    double b = M_PI / (size-1);
    for(size_t i = 0; i < size; i++) {
        double c = b*i;
        window_buffer[i] = a0 - std::cos(c*2)*a1 + std::cos(c*4)*a2 - std::cos(c*6)*a3;
    }
}

}
