/*
    Rocaloid
    Copyright (C) 2015 Icenowy Zheng <icenowy@outlook.com>

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

#ifndef RUCE_VECTOR_INTERPOLATOR_HPP
#define RUCE_VECTOR_INTERPOLATOR_HPP

#include <cstdint>
#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace RUCE {

/**
 * Given a discrete series of values, figure out the `index`-th value, where `index` is a real number
 *
 * Different interpolating algorithms can be used. See `LinearVectorInterpolator` and `QuadraticVectorInterpolator`.
 */
template<typename T>
class VectorInterpolator {
public:
    virtual T operator() (const T vector[], size_t length, double index) const = 0;
};

template<typename T>
class LinearVectorInterpolator : public VectorInterpolator<T> {
public:
    T operator() (const T vector[], size_t length, double index) const {
        if(index < 0)
            throw std::out_of_range();
        else if(index == length-1) 
            return vector[size_t(index)];
        else {
            double int_part;
            double frac_part = std::modf(index, &int_part);
            if(int_part >= length-1)
                throw std::out_of_range();
            size_t index_int = int_part;
            return interp_2(vector[index_int], vector[index_int+1], frac_part);
        }
    }
private:
    static T interp_2(T y0, T y1, double residual) {
        return residual != 0 ?
            (y1 - y0) * residual + y0 :
            y0;
    }
};

template<typename T>
class QuadraticVectorInterpolator : public VectorInterpolator<T> {
public:
    T operator() (const T vector[], size_t length, double index) const {
        if(index < 0)
            throw std::out_of_range();
        else if(index == length-1)
            return vector[size_t(index)];
        else {
            double int_part;
            double frac_part = std::modf(index, &int_part);
            if(int_part >= length-1)
                throw std::out_of_range();
            else if(frac_part == 0)
                return vector[size_t(int_part)];
            else if(index < 1.5)
                return interp_3(vector[0], vector[1], vector[2], index);
            else if(index >= length-2.5)
                return interp_3(vector[length-3], vector[length-2], vector[length-1], index-(length-3));
            else {
                size_t i = size_t(int_part);
                return frac_part >= 0.5 ?
                    interp_3(vector[i], vector[i+1], vector[i+2], frac_part) :
                    interp_3(vector[i-1], vector[i], vector[i+1], frac_part+1);
            }
        }
    }
private:
    static T interp_3(T y0, T y1, T y2, double residual) {
        // y = a*x^2 + b*x + c
        T a = (  y0   - y1*2 + y2) / 2;
        T b = (- y0*3 + y1*4 - y2) / 2;
        const T &c = y0;
        return (a*residual + b) * residual + c;
    }
};

}

#endif
