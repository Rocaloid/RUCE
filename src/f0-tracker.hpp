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

#ifndef RUCE_F0_TRACKER_HPP
#define RUCE_F0_TRACKER_HPP

#include <cstdint>
#include <vector>
#include "signal-segment.hpp"
#include "proxy-ptr.hpp"
#include "utils.hpp"

namespace RUCE {

class F0Tracker {
public:
    F0Tracker();
    ~F0Tracker();
    F0Tracker &track(const SignalSegment &segment, int32_t sample_rate, ssize_t frame_hop = 80, double min_freq = 60, double max_freq = 1000, double threshold = 0.1);
    const std::vector<float> &get_result() const;
    float get_result(ssize_t sample_idx) const;
private:
    struct Private;
    proxy_ptr<Private> p;
};

}

#endif
