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
#include <cassert>
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
        m_buffer((new sample_fmt[length])),
        m_data(m_buffer),
        m_left(0),
        m_right(length) {
        fill(0);
    }
    /**
     * Make a new SignalSegment
     */
    SignalSegment(ssize_t left_bound, ssize_t right_bound) :
        m_buffer(new sample_fmt[std::max(right_bound - left_bound, ssize_t(0))]),
        m_data(m_buffer - left_bound),
        m_left(left_bound),
        m_right(std::max(right_bound, left_bound)) {
        fill(0);
    }
    /**
     * Make a new SignalSegment by copying an existing one
     */
    SignalSegment(const SignalSegment &other) :
        m_buffer(new sample_fmt[other.m_right - other.m_left]),
        m_data(m_buffer - other.m_left),
        m_left(other.m_left),
        m_right(other.m_right) {
        assert(&other.m_buffer[0] == &other.m_data[other.m_left]);
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] = other.m_data[i];
    }
    /**
     * Make a new SignalSegment by moving an existing one
     */
    SignalSegment(SignalSegment &&other) :
        m_buffer(other.m_buffer),
        m_data(other.m_data),
        m_left(other.m_left),
        m_right(other.m_right) {
        other.m_buffer = nullptr;
        other.m_data = nullptr;
        other.m_left = 0;
        other.m_right = 0;
        assert(&m_buffer[0] == &m_data[m_left]);
    }
    /**
     * Make a new SignalSegment by copying out a subsegment
     */
    SignalSegment(const SignalSegment &other, ssize_t left_bound, ssize_t right_bound) :
        m_left(left_bound),
        m_right(right_bound) {
        assert(m_right > m_left);
        m_buffer = new sample_fmt[m_right - m_left];
        m_data = m_buffer - m_left;

        assert(&other.m_buffer[0] == &other.m_data[other.m_left]);
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
        assert(&m_buffer[0] == &m_data[m_left]);
        if(m_buffer != nullptr)
            delete[] m_buffer;
        m_buffer = nullptr;
        m_data = nullptr;
        m_left = 0;
        m_right = 0;
    }
    /**
     * Copy the data and bounding information from another SignalSegment
     */
    SignalSegment &operator= (const SignalSegment &other) {
        assert(&other.m_buffer[0] == &other.m_data[other.m_left]);
        if(this == &other)
            return *this;
        this->~SignalSegment();
        m_buffer = (new sample_fmt[other.m_right - other.m_left]);
        m_data = m_buffer - other.m_left;
        m_left = other.m_left;
        m_right = other.m_right;
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] = other.m_data[i];
        return *this;
    }
    /**
     * Moving the data and bounding information from another SignalSegment
     */
    SignalSegment &operator= (SignalSegment &&other) {
        assert(&other.m_buffer[0] == &other.m_data[other.m_left]);
        if(this == &other)
            return *this;
        this->~SignalSegment();
        m_buffer = other.m_buffer;
        m_data = other.m_data;
        m_left = other.m_left;
        m_right = other.m_right;
        other.m_buffer = nullptr;
        other.m_data = nullptr;
        other.m_left = 0;
        other.m_right = 0;
        return *this;
    }
    /**
     * Get a sample at the specific index
     *
     * For out-of-bound index, return 0
     */
    sample_fmt operator[](ssize_t index) const {
        assert(&m_buffer[0] == &m_data[m_left]);
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
        assert(&m_buffer[0] == &m_data[m_left]);
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
    ssize_t left() const { return m_left; }
    /**
     * Get the right bound of this SignalSegment
     */
    ssize_t right() const { return m_right; }
    /**
     * Get the raw data, for operations without bounding check
     *
     * data[left] is the first sample,
     * data[right-1] is the last
     */
    const sample_fmt *data() const { return m_data; }
    /**
     * Get the raw data, for operations without bounding check
     *
     * data[left] is the first sample,
     * data[right-1] is the last
     */
    sample_fmt *data() { return m_data; }
    /**
     * Get the raw buffer, that is, data + left_bound
     *
     * buffer[0] is the first sample,
     * buffer[right-left-1] is the last
     */
    const sample_fmt *buffer() const { return m_buffer; }
    /**
     * Get the raw buffer, that is, data + left_bound
     *
     * buffer[0] is the first sample,
     * buffer[right-left-1] is the last
     */
    sample_fmt *buffer() { return m_buffer; }

    /**
     * Fill with a specific value
     */
    SignalSegment &fill(sample_fmt value) {
        assert(&m_buffer[0] == &m_data[m_left]);
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] = value;
        return *this;
    }
    /**
     * Add a specific value
     */
    SignalSegment &operator+= (sample_fmt rhs) {
        assert(&m_buffer[0] == &m_data[m_left]);
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] += rhs;
        return *this;
    }
    /**
     * Subtract a specific value
     */
    SignalSegment &operator-= (sample_fmt rhs) {
        assert(&m_buffer[0] == &m_data[m_left]);
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] -= rhs;
        return *this;
    }
    /**
     * Multiply by a specific value
     */
    SignalSegment &operator*= (sample_fmt rhs) {
        assert(&m_buffer[0] == &m_data[m_left]);
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] *= rhs;
        return *this;
    }
    /**
     * Divide by a specific value
     */
    SignalSegment &operator/= (sample_fmt rhs) {
        assert(&m_buffer[0] == &m_data[m_left]);
        for(ssize_t i = m_left; i < m_right; i++)
            m_data[i] /= rhs;
        return *this;
    }
    /**
     * Add another SignalSegment
     */
    SignalSegment &operator+= (const SignalSegment &rhs) {
        assert(&m_buffer[0] == &m_data[m_left]);
        assert(&rhs.m_buffer[0] == &rhs.m_data[rhs.m_left]);
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
        assert(&m_buffer[0] == &m_data[m_left]);
        assert(&rhs.m_buffer[0] == &rhs.m_data[rhs.m_left]);
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
        assert(&m_buffer[0] == &m_data[m_left]);
        assert(&rhs.m_buffer[0] == &rhs.m_data[rhs.m_left]);
        ssize_t common_left = std::max(m_left, rhs.m_left);
        ssize_t common_right = std::min(m_right, rhs.m_right);
        for(ssize_t i = m_left; i < common_left; i++)
            m_data[i] = 0;
        for(ssize_t i = common_left; i < common_right; i++)
            m_data[i] *= rhs.m_data[i];
/*
  FIXME:
  A crash happened here!
Args:
  "C:\Program Files\UTAU\Rocaloid.exe" "C:\users\brilliant\Application Data\UTAU\voice\Yong Qi_CVVChinese_Koi\part3\mu_mu.wav" "C:\users\brilliant\My Documents\VOCALOID\UTAU\yuhizaka_zhongwen.cache\202_mu_f4_E4_utCr59.wav" E4 100 "" 3647.212 450 220.258 -467.467 100 0 !174 DVDUDSDQDPDNDLDKDIDHDGDFDDDCDAC9C5C0CuCoChCaCRCJCAB3BtBjBZBPBFA7AyAqAiAaAUAOAJAFACAA#13#ABABACADAEAEAFAGAHAHAI#4#AHAHAGAEADABAA/+/8/5/3/1/z/x/w/u/t/s/s/t/u/w/x/z/1/3/5/7/9//ABACAEAFAGAHAHAI#1#
  wine: Unhandled page fault on write access to 0x00000000 at address 0x402271 (thread 000b), starting debugger...
Backtrace:
=>0 0x00402271 in rocaloid (+0x2271) (0x010afc98)
  1 0x00403487 in rocaloid (+0x3486) (0x010afd18)
  2 0x004013e2 in rocaloid (+0x13e1) (0x010afdf0)
*/
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
        assert(&m_buffer[0] == &m_data[m_left]);
        assert(&rhs.m_buffer[0] == &rhs.m_data[rhs.m_left]);
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
        assert(&m_buffer[0] == &m_data[m_left]);
        m_data += shift_amount;
        m_left -= shift_amount;
        m_right -= shift_amount;
        assert(&m_buffer[0] == &m_data[m_left]);
        return *this;
    }
    /**
     * Shift right the samples
     */
    SignalSegment &operator>>= (ssize_t shift_amount) {
        assert(&m_buffer[0] == &m_data[m_left]);
        m_data -= shift_amount;
        m_left += shift_amount;
        m_right += shift_amount;
        assert(&m_buffer[0] == &m_data[m_left]);
        return *this;
    }
protected:
    sample_fmt *m_buffer = nullptr;
    sample_fmt *m_data = nullptr;
    ssize_t m_left = 0;
    ssize_t m_right = 0;
    sample_fmt zero = 0;
};

}

#endif
