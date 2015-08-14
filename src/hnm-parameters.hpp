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
#include <vector>
#include "utils.hpp"

namespace RUCE {

class HNMParameters {
public:
    ssize_t window_size;
    ssize_t window_hop;
    static const size_t max_pillars = 128;
    /**
     * The middle point of the first window
     * measured in frames
     */
    ssize_t first_window_mid;
    /**
     * The first index is window index,
     * the second index is harmony index.
     *
     * harmony_frequencies[w][0] is the estimated f0
     * harmony_frequencies[w][i] is the frequency of each harmony peak
     */
    std::vector<std::array<double, max_pillars>> harmony_frequencies;
    /**
     * harmony_magnitude_factor[w][i] is the linear magnitude of each harmony
     * multiplied by i^2
     */
    std::vector<std::array<double, max_pillars>> harmony_magnitudes;
    /**
     * harmony_phase_difference[w][i] is the angle of the phase of each harmony
     * minus by the phase of f0, stored in complex number format
     * The phase are measured at the center of the window
     */
    std::vector<std::array<std::complex<double>, max_pillars>> harmony_phases_difference;

    // Timestamp conversion functions
    double frame_to_window_idx(ssize_t frame_idx) const {
        return double(frame_idx-first_window_mid) / double(window_hop);
    }
    double frame_to_window_idx(double frame_idx) const {
        return (frame_idx-first_window_mid) / double(window_hop);
    }
    ssize_t window_to_frame_idx(double window_idx) const {
        return ssize_t(std::round(window_idx*window_hop)) + first_window_mid;
    }
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
