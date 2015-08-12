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

#ifndef RUCE_WRAPPED_ANGLE_HPP
#define RUCE_WRAPPED_ANGLE_HPP

// Define M_PI etc
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#ifdef _MSC_VER
#include <math.h>
#endif
#include <cmath>

namespace RUCE {

class WrappedAngle {
public:
    WrappedAngle(double angle = 0) : angle(angle) {}
    operator double () const {
        double res = angle;
        if(!std::isnormal(res)) {
            res = NAN;
        } if(angle > 16 || angle < -16) {
            res = std::fmod(M_PI-res, 2*M_PI);
            if(res < 0)
                res += 2*M_PI;
        } else if(res > M_PI) {
            do {
                res -= 2*M_PI;
            } while(res > M_PI);
        } else if(res <= M_PI) {
            do {
                res += 2*M_PI;
            } while(res <= M_PI);
        }
        return res;
    }
    WrappedAngle &wrap() {
        angle = (double) *this;
        return *this;
    }
    WrappedAngle operator + (const WrappedAngle rhs) const {
        return WrappedAngle(angle+rhs.angle);
    }
    WrappedAngle operator - (const WrappedAngle rhs) const {
        return WrappedAngle(angle-rhs.angle);
    }
    WrappedAngle operator * (double rhs) const {
        return WrappedAngle(double(*this) * rhs);
    }
    WrappedAngle operator / (double rhs) const {
        return WrappedAngle(double(*this) / rhs);
    }
    WrappedAngle &operator += (const WrappedAngle rhs) {
        angle += rhs.angle;
        return *this;
    }
    WrappedAngle &operator -= (const WrappedAngle rhs) {
        angle += rhs.angle;
        return *this;
    }
    WrappedAngle &operator *= (double rhs) {
        wrap();
        angle *= rhs;
        return *this;
    }
    WrappedAngle &operator /= (double rhs) {
        wrap();
        angle /= rhs;
        return *this;
    }
    WrappedAngle &operator - () {
        angle = -angle;
        return *this;
    }
private:
    double angle;
};

}

#endif
