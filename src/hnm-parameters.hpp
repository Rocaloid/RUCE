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

#ifndef RUCE_HNM_PARAMETERS_HPP
#define RUCE_HNM_PARAMETERS_HPP

#include <array>
#include <complex>
#include <cstdint>
#include <valarray>
#include <vector>
#include <llsm.h>
#include "spectrum.hpp"
#include "utils.hpp"

namespace RUCE {

class HNMParameters {
public:
    HNMParameters();
    ~HNMParameters();
    llsm_parameters parameters;
};

// Some operators to make interpolation work
template <typename T>
std::array<T, HNMParameters::max_pillars> &operator+= (std::array<T, HNMParameters::max_pillars> &lhs, const std::array<T, HNMParameters::max_pillars> &rhs) {
    for(size_t i = 0; i < HNMParameters::max_pillars; i++) {
        lhs[i] += rhs[i];
    }
    return lhs;
}
template <typename T>
std::array<T, HNMParameters::max_pillars> &operator-= (std::array<T, HNMParameters::max_pillars> &lhs, const std::array<T, HNMParameters::max_pillars> &rhs) {
    for(size_t i = 0; i < HNMParameters::max_pillars; i++) {
        lhs[i] -= rhs[i];
    }
    return lhs;
}
template <typename T>
std::array<T, HNMParameters::max_pillars> &operator*= (std::array<T, HNMParameters::max_pillars> &lhs, double rhs) {
    for(size_t i = 0; i < HNMParameters::max_pillars; i++) {
        lhs[i] *= rhs;
    }
    return lhs;
}
template <typename T>
std::array<T, HNMParameters::max_pillars> &operator/= (std::array<T, HNMParameters::max_pillars> &lhs, double rhs) {
    for(size_t i = 0; i < HNMParameters::max_pillars; i++) {
        lhs[i] /= rhs;
    }
    return lhs;
}

}
#endif
