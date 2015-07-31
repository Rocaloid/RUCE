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

#ifndef RUCE_SPECTRUM_HPP
#define RUCE_SPECTRUM_HPP

#include <complex>
#include <cstdint>
#include <vector>
#include "proxy-ptr.hpp"

namespace RUCE {

class SignalSegment;

class Spectrum {
public:
    Spectrum(size_t fftsize);
    ~Spectrum();
    Spectrum &init(size_t fftsize);

    /**
     * Do an FFT with the given `SignalSegment`,
     * the result is stored in this object.
     * Use `get_spectrum` to retrieve the result.
     */
    Spectrum &fft_analyze(const SignalSegment &signal);
    /**
     * Do an IFFT with the spectrum stored in this object,
     * and return a new `SignalSegment`
     *
     * Normalization gain is `n`.
     */
    SignalSegment ifft_analyze();

    std::vector<std::complex<double>> &get_spectrum();
    const std::vector<std::complex<double>> &get_spectrum() const;
    std::vector<double> get_magnitude() const;
    std::vector<double> get_phase() const;

    /* Ultility functions related to FFT */
    double hertz_to_bin(double heartz, int32_t sample_rate) const {
        return heartz * fftsize / sample_rate;
    }
    double bin_to_hertz(double bin, int32_t sample_rate) const {
        return bin * sample_rate / fftsize;
    }
    /**
     * Given a number `n`, find the smallest number `k >= m` that is a power of 2
     *
     * Thanks to the algorithm http://stackoverflow.com/a/1322548/2557927
     */
    static size_t next_pow2(size_t n, size_t minimal = 2) {
        if(n <= minimal)
            return minimal;
        --n;
        for(size_t i = 1; i < 8 * sizeof n; i *= 2)
            n |= n >> i;
        return n+1;
    }
    /**
     * FFTShift in frequency domain
     *
     * The process is in place
     */
    Spectrum &fftshift();
    /**
     * IFFTShift in frequency domain
     *
     * The process is in place
     */
    Spectrum &ifftshift();
    /**
     * FFTShift in time domain
     *
     * The process is out of place
     */
    static SignalSegment fftshift(const SignalSegment &signal);
    /**
     * IFFTShift in time domain
     *
     * The process is out of place
     */
    static SignalSegment ifftshift(const SignalSegment &signal);
private:
    struct Private;
    proxy_ptr<Private> p;
    size_t fftsize = 0;
};

}

#endif
