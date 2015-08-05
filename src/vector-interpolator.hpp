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
#include <functional>
#include <stdexcept>
#include <algorithm>

namespace RUCE {

/**
 * Given a discrete series of values, figure out the `index`-th value, where `index` is a real number
 *
 * Different interpolating algorithms can be used. See `LinearVectorInterpolator` and `QuadraticVectorInterpolator`.
 *
 * The argument `getter` is a function that can fetch a given element based on an integral index
 *
 * Demo:
 *   LinearVectorInterpolator interpolator;
 *   std::array<double, 4> my_array {{ 0, 1, 2, 3 }}
 *   double value_at_1_5 = interpolator([&](size_t index) {
 *       return my_array[index];
 *   }, my_array.size(), 1.5);
 */
template<typename T>
class VectorInterpolator {
public:
    virtual T operator() (std::function<T (size_t index)> getter, size_t length, double index) const = 0;
};

template<typename T>
class LinearVectorInterpolator : public VectorInterpolator<T> {
public:
    T operator() (std::function<T (size_t index)> getter, size_t length, double index) const {
        if(index < 0)
            throw std::out_of_range("Index out of range");
        else if(index == length-1) 
            return getter(size_t(index));
        else {
            double int_part;
            double frac_part = std::modf(index, &int_part);
            if(int_part >= length-1)
                throw std::out_of_range("Index out of range");
            size_t i = size_t(int_part);
            return interp_2(getter(i), getter(i+1), frac_part);
        }
    }
private:
    static T interp_2(T y0, T y1, double residual) {
        if(residual != 0) {
            // Use in-place arithmatic to avoid copying
            // Because T may be an array or something else
            y1 -= y0;
            y1 *= residual;
            y1 += y0;
            return y1; // (y1 - y0) * residual + y0
        } else {
            return y0;
        }
    }
};

template<typename T>
class QuadraticVectorInterpolator : public VectorInterpolator<T> {
public:
    T operator() (std::function<T (size_t index)> getter, size_t length, double index) const {
        if(index < 0)
            throw std::out_of_range("Index out of range");
        else if(index == length-1)
            return getter(size_t(index));
        else {
            double int_part;
            double frac_part = std::modf(index, &int_part);
            if(int_part >= length-1)
                throw std::out_of_range("Index out of range");
            else if(frac_part == 0)
                return getter(size_t(int_part));
            else if(index < 1.5)
                return interp_3(getter(0), getter(1), getter(2), index-1);
            else if(index >= length-2.5)
                return interp_3(getter(length-3), getter(length-2), getter(length-1), index-(length-2));
            else {
                size_t i = size_t(int_part);
                return frac_part >= 0.5 ?
                    interp_3(getter(i), getter(i+1), getter(i+2), frac_part-1) :
                    interp_3(getter(i-1), getter(i), getter(i+1), frac_part);
            }
        }
    }
private:
    static T interp_3(T y_1, T y0, T y1, double residual) {
        // y = a*x^2 + b*x + c
        T a = (y_1 + y1) / 2 - y0;
        T b = (y_1 - y1) / 2;
        const T &c = y0;
        return (a*residual + b) * residual + c;
    }
};

}

#endif
