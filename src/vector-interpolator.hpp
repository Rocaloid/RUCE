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

template<typename T>
class VectorInterpolator {
public:
    VectorInterpolator();
    T operator() (T vector[], size_t length, double index) {
        if(index < 0 || index > length)
            throw std::out_of_range("index of interpolator out of range");
        return do_interpolate(vector,length,index);
    }
    virtual T do_interpolate (T vector[], size_t length, double index) = 0;
};

template<typename T>
class LinearVectorInterpolator : VectorInterpolator<T>{
public:
    LinearVectorInterpolator();
    T do_interpolate (T vector[], size_t length, double index) {
        if(index == 0 || index == length) 
            return vector[size_t(index)];
        size_t left = (size_t)floor(index);
        size_t right = (size_t)ceil(index);
        double decimal_part = index - left;
        return vector[left] + (vector[right]-vector[left]) * decimal_part;
    }
};

template<typename T>
class QuadraticVectorInterpolator : VectorInterpolator<T>{
public:
    QuadraticVectorInterpolator(T d) : diff(d);
    T abs(T a) {
       return (a<0) ? -a : a;
    }
    T maxdistance(T a, T b, T c)
    {
        return std::max(abs(a-b),std::max(abs(a-c),abs(b-c)));
    }
    T do_interpolate (T vector[], size_t length, double index) {
        if(index == 0 || index == length) 
            return vector[size_t(index)];
        size_t p1, p2, p3;
        if(index > 1 && index < length - 1) {
            size_t p2 = (size_t)round(index);
            size_t p1 = p2 - 1;
            size_t p3 = p2 + 1;
        }
        else if(index < 1) {
            p1 = 0;
            p2 = 1;
            p3 = 2;
        }
        else if(index > length - 1) {
            p1 = length - 3;
            p2 = length - 2;
            p3 = length - 1;
        }
        if(maxdistance(vector[p1],vector[p2],vector[p3]) < diff)
            throw std::range_error("Vector too flat!");
        double a,b,c;
        a = ((vector[p3]-vector[p1])/(p3-p1) - (vector[p2]-vector[p1])/(p2-p1))/(p3-p2);
        b = (vector[p2]-vector[p1])/(p2-p1) - (p3+p1)*a;
        c = vector[p1] - p1*p1*a - p1*b;
        return index*index*a + index*b + c;
    }
private:
    T diff;
};

}

#endif
