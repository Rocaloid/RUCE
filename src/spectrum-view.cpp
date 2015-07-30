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

#include "spectrum-view.hpp"
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <ooura-fft.h>
#include "signal-segment.hpp"

namespace RUCE {

struct SpectrumView::Private {
    std::vector<std::complex<double>> spectrum;
    std::vector<double> fftdata;
    std::vector<int> ip;
    std::vector<double> w;
};

SpectrumView &SpectrumView::init(size_t fftsize) {
    fftsize = fftsize;
    p->spectrum = std::vector<std::complex<double>>(fftsize);
    p->fftdata = std::vector<double>(fftsize*2);
    p->ip = std::vector<int>(size_t(std::ceil(std::sqrt(fftsize)))+2);
    p->w = std::vector<double>((fftsize+1)/2);
    return *this;
}

SpectrumView &SpectrumView::fft_analysize(const SignalSegment &signal) {
    ssize_t padding = signal.size() - ssize_t(fftsize);
    if(padding < 0)
        throw std::range_error("Input signal is larger than FFT size");
    SignalSegment padded_signal = SignalSegment(signal, signal.left() - padding/2, signal.right() + (padding+1)/2);

    assert(padded_signal.size() == ssize_t(fftsize));
    assert(p->fftdata.size() == fftsize*2);

    auto fftdata = p->fftdata.data();
    const auto padded_signal_buffer = padded_signal.buffer();
    for(size_t i = 0; i < fftsize; i++) {
        fftdata[i*2] = padded_signal_buffer[i];
        fftdata[i*2+1] = 0;
    }

    cdft(fftsize*2, 1, fftdata, p->ip.data(), p->w.data());

    assert(p->spectrum.size() == fftsize);

    auto spectrum = p->spectrum.data();
    for(size_t i = 0; i < fftsize; i++) {
        spectrum[i] = std::complex<double>(fftdata[i*2], fftdata[i*2+1]);
    }

    return *this;
}

SignalSegment SpectrumView::ifft_analysize() {
    assert(p->spectrum.size() == fftsize);
    assert(p->fftdata.size() == fftsize*2);

    auto fftdata = p->fftdata.data();
    const auto spectrum = p->spectrum.data();
    for(size_t i = 0; i < fftsize; i++) {
        fftdata[i*2] = spectrum[i].real();
        fftdata[i*2+1] = spectrum[i].imag();
    }

    cdft(fftsize*2, -1, fftdata, p->ip.data(), p->w.data());

    SignalSegment result_signal(fftsize);

    auto result_signal_buffer = result_signal.buffer();
    for(size_t i = 0; i < fftsize; i++) {
        result_signal_buffer[i] = fftdata[i*2];
    }

    return result_signal;
}

std::vector<std::complex<double>> &SpectrumView::get_spectrum() {
    return p->spectrum;
}

const std::vector<std::complex<double>> &SpectrumView::get_spectrum() const {
    return p->spectrum;
}

std::vector<double> SpectrumView::get_intensity() const {
    assert(p->spectrum.size() == fftsize);

    std::vector<double> intensity(fftsize);

    auto iters = p->spectrum.cbegin();
    auto iteri = intensity.begin();
    while(iteri != intensity.end()) {
        *iteri = std::abs(*iters);
        ++iters; ++iteri;
    }

    return intensity;
}

std::vector<double> SpectrumView::get_phase() const {
    assert(p->spectrum.size() == fftsize);

    std::vector<double> phase(fftsize);

    auto iters = p->spectrum.cbegin();
    auto iterp = phase.begin();
    while(iterp != phase.end()) {
        *iterp = std::arg(*iters);
        ++iters; ++iterp;
    }

    return phase;
}

}
