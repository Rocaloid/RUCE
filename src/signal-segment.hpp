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

#ifndef RUCE_SIGNAL_SEGMENT_HPP
#define RUCE_SIGNAL_SEGMENT_HPP

#include <algorithm>
#include <utility>
#include "utils.hpp"

namespace RUCE {

/**
 * Represent a segment of PCM data in time domain
 *
 * Data are restricted in a region from [left_bound, right_bound)
 * Provide operators to do some common task
 */
class SignalSegment {
public:
    typedef double sample_fmt;
    /**
     * Make a new SignalSegment with left_bond = 0
     */
    SignalSegment(ssize_t length = 0) :
        m_data((new sample_fmt[length])),
        m_left(0),
        m_right(length) {
        *this = sample_fmt(0);
    }
    /**
     * Make a new SignalSegment
     */
    SignalSegment(ssize_t left_bound, ssize_t right_bound) :
        m_data((new sample_fmt[std::max(right_bound - left_bound, ssize_t(0))]) - left_bound),
        m_left(left_bound),
        m_right(std::max(right_bound, left_bound)) {
        *this = sample_fmt(0);
    }
    /**
     * Make a new SignalSegment by copying an existing one
     */
    SignalSegment(const SignalSegment &other) :
        m_data((new sample_fmt[other.m_right - other.m_left]) - other.m_left),
        m_left(other.m_left),
        m_right(other.m_right) {
    }
    /**
     * Make a new SignalSegment by moving an existing one
     */
    SignalSegment(SignalSegment &&other) :
        m_data(other.m_data),
        m_left(other.m_left),
        m_right(other.m_right) {
        other.m_data = nullptr;
        other.m_left = 0;
        other.m_right = 0;
    }
    /**
     * Make a new SignalSegment by copying out a subsegment
     */
    SignalSegment(const SignalSegment &other, ssize_t left_bound, ssize_t right_bound) :
        m_data((new sample_fmt[std::max(right_bound - left_bound, ssize_t(0))]) - left_bound),
        m_left(left_bound),
        m_right(std::max(right_bound, left_bound)) {
        ssize_t common_left = std::max(m_left, other.m_left);
        ssize_t common_right = std::min(m_right, other.m_right);
        for(ssize_t i = m_left; i < common_left; i++)
            m_data[i] = 0;
        for(ssize_t i = common_left; i < common_right; i++)
            m_data[i] = other.m_data[i];
        for(ssize_t i = common_right; i < m_right; i++)
            m_data[i] = 0;
    }
    ~SignalSegment() {
        if(m_data + m_left != nullptr)
            delete[] (m_data + m_left);
        m_data = nullptr;
        m_left = 0;
        m_right = 0;
    }
    /**
     * Copy the data and bounding information from another SignalSegment
     */
    SignalSegment &operator= (const SignalSegment &other) {
        if(this == &other)
            return *this;
        this->~SignalSegment();
        m_data = (new sample_fmt[other.m_right - other.m_left]) - other.m_left;
        m_left = other.m_left;
        m_right = other.m_right;
        return *this;
    }
    /**
     * Moving the data and bounding information from another SignalSegment
     */
    SignalSegment &operator= (SignalSegment &&other) {
        std::swap(m_data, other.m_data);
        std::swap(m_left, other.m_left);
        std::swap(m_right, other.m_right);
        return *this;
    }
    /**
     * Get a sample at the specific index
     *
     * For out-of-bound index, return 0
     */
    sample_fmt operator[](ssize_t index) const {
        if(index >= m_left && index < m_right) {
            return m_data[index];
        } else {
            return 0;
        }
    }
    /**
     * Get a sample at the specific index
     *
     * For out-of-bound index, return 0
     */
    sample_fmt &operator[](ssize_t index) {
        if(index >= m_left && index < m_right) {
            return m_data[index];
        } else {
            zero = 0;
            return zero;
        }
    }
    /**
     * Get the size of this SignalSegment (right_bound - left_bound)
     */
    ssize_t size() const { return m_right - m_left; }
    /**
     * Get the left bound of this SignalSegment
     */
    ssize_t left_bound() const { return m_left; }
    /**
     * Get the right bound of this SignalSegment
     */
    ssize_t right_bound() const { return m_right; }
    /**
     * Get the raw data, for operations without bounding check
     */
    const sample_fmt *data() const { return m_data; }
    /**
     * Get the raw data, for operations without bounding check
     */
    sample_fmt *data() { return m_data; }

    /**
     * Fill with a specific value
     */
    SignalSegment &operator= (sample_fmt value) {
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] = value;
        return *this;
    }
    /**
     * Add a specific value
     */
    SignalSegment &operator+= (sample_fmt rhs) {
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] += rhs;
        return *this;
    }
    /**
     * Subtract a specific value
     */
    SignalSegment &operator-= (sample_fmt rhs) {
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] -= rhs;
        return *this;
    }
    /**
     * Multiply by a specific value
     */
    SignalSegment &operator*= (sample_fmt rhs) {
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] *= rhs;
        return *this;
    }
    /**
     * Divide by a specific value
     */
    SignalSegment &operator/= (sample_fmt rhs) {
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] /= rhs;
        return *this;
    }
    /**
     * Add another SignalSegment
     */
    SignalSegment &operator+= (const SignalSegment &rhs) {
        ssize_t common_left = std::max(m_left, rhs.m_left);
        ssize_t common_right = std::min(m_right, rhs.m_right);
        for(ssize_t i = common_left; i < common_right; i++)
            m_data[i] += rhs.m_data[i];
        return *this;
    }
    /**
     * Subtract another SignalSegment
     */
    SignalSegment &operator-= (const SignalSegment &rhs) {
        ssize_t common_left = std::max(m_left, rhs.m_left);
        ssize_t common_right = std::min(m_right, rhs.m_right);
        for(ssize_t i = common_left; i < common_right; i++)
            m_data[i] -= rhs.m_data[i];
        return *this;
    }
    /**
     * Multiply by another SignalSegment
     *
     * Note that the uncommon samples are filled with 0
     */
    SignalSegment &operator*= (const SignalSegment &rhs) {
        ssize_t common_left = std::max(m_left, rhs.m_left);
        ssize_t common_right = std::min(m_right, rhs.m_right);
        for(ssize_t i = m_left; i < common_left; i++)
            m_data[i] = 0;
        for(ssize_t i = common_left; i < common_right; i++)
            m_data[i] *= rhs.m_data[i];
        for(ssize_t i = common_right; i < m_right; i++)
            m_data[i] = 0;
        return *this;
    }
    /**
     * Divide by another SignalSegment
     *
     * Note that the uncommon samples are not touched
     */
    SignalSegment &operator/= (const SignalSegment &rhs) {
        ssize_t common_left = std::max(m_left, rhs.m_left);
        ssize_t common_right = std::min(m_right, rhs.m_right);
        for(ssize_t i = common_left; i < common_right; i++)
            m_data[i] /= rhs.m_data[i];
        return *this;
    }
    /**
     * Shift left the samples
     */
    SignalSegment &operator<<= (ssize_t shift_amount) {
        m_data += shift_amount;
        m_left -= shift_amount;
        m_right -= shift_amount;
        return *this;
    }
    /**
     * Shift right the samples
     */
    SignalSegment &operator>>= (ssize_t shift_amount) {
        m_data -= shift_amount;
        m_left += shift_amount;
        m_right += shift_amount;
        return *this;
    }
protected:
    sample_fmt *m_data = nullptr;
    ssize_t m_left = 0;
    ssize_t m_right = 0;
    sample_fmt zero = 0;
};

}

#endif
