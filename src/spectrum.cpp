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

#include "spectrum.hpp"
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <ooura-fft.h>
#include "signal-segment.hpp"

namespace RUCE {

struct Spectrum::Private {
    std::vector<std::complex<double>> spectrum;
    std::vector<double> fftdata;
    std::vector<int> ip;
    std::vector<double> w;
};

Spectrum::Spectrum(size_t fftsize) {
    init(fftsize);
}

Spectrum::~Spectrum() {
}

Spectrum &Spectrum::init(size_t fftsize) {
    this->fftsize = fftsize;
    p->spectrum = std::vector<std::complex<double>>(fftsize);
    p->fftdata = std::vector<double>(fftsize*2);
    p->ip = std::vector<int>(size_t(std::ceil(std::sqrt(fftsize)))+2);
    p->w = std::vector<double>((fftsize+1)/2);
    return *this;
}

Spectrum &Spectrum::fft_analyze(const SignalSegment &signal) {
    assert(p->fftdata.size() == fftsize*2);
    if(ssize_t(fftsize) != signal.size())
        throw std::range_error("Input signal is different than FFT size");

    const auto signal_buffer = signal.buffer();
    auto fftdata = p->fftdata.data();
    for(size_t i = 0; i < fftsize; i++) {
        fftdata[i*2] = signal_buffer[i];
        fftdata[i*2+1] = 0;
    }

    cdft(fftsize*2, -1, fftdata, p->ip.data(), p->w.data());

    assert(p->spectrum.size() == fftsize);
    auto &spectrum = p->spectrum;
    for(size_t i = 0; i < fftsize; i++) {
        spectrum[i] = std::complex<double>(fftdata[i*2], fftdata[i*2+1]);
    }

    return *this;
}

SignalSegment Spectrum::ifft_analyze() {
    assert(p->spectrum.size() == fftsize);
    assert(p->fftdata.size() == fftsize*2);

    const auto spectrum = p->spectrum.data();
    auto fftdata = p->fftdata.data();
    for(size_t i = 0; i < fftsize; i++) {
        fftdata[i*2] = spectrum[i].real();
        fftdata[i*2+1] = spectrum[i].imag();
    }

    cdft(fftsize*2, 1, fftdata, p->ip.data(), p->w.data());

    SignalSegment result_signal(fftsize);
    auto result_signal_buffer = result_signal.buffer();
    for(size_t i = 0; i < fftsize; i++) {
        result_signal_buffer[i] = fftdata[i*2];
    }

    return result_signal;
}

std::vector<std::complex<double>> &Spectrum::get_spectrum() {
    return p->spectrum;
}

const std::vector<std::complex<double>> &Spectrum::get_spectrum() const {
    return p->spectrum;
}

std::vector<double> Spectrum::get_magnitude() const {
    assert(p->spectrum.size() == fftsize);

    std::vector<double> magnitude(fftsize);

    auto iters = p->spectrum.cbegin();
    auto iterm = magnitude.begin();
    while(iterm != magnitude.end()) {
        *iterm = std::abs(*iters);
        ++iters; ++iterm;
    }

    return magnitude;
}

std::vector<std::complex<double>> Spectrum::get_phase() const {
    assert(p->spectrum.size() == fftsize);

    std::vector<std::complex<double>> phase = p->spectrum;

    for(std::complex<double> &i : phase) {
        //i = std::polar(1.0, std::arg(i));
        i /= std::abs(i);
        if(!std::isfinite(i.real()) && !std::isfinite(i.imag())) {
            i = 0;
        }
    }

    return phase;
}

Spectrum &Spectrum::fftshift() {
    assert(p->spectrum.size() == fftsize);
    size_t half_floor = fftsize/2;
    size_t half_ceil = (fftsize+1)/2;
    assert(half_floor + half_ceil == fftsize);
    decltype (p->spectrum) tmp(half_floor);

    auto spectrum = p->spectrum.data();
    auto tmp_data = tmp.data();
    std::memcpy(tmp_data, &spectrum[half_ceil], half_floor * sizeof spectrum[0]);
    std::memmove(&spectrum[half_floor], spectrum, half_ceil * sizeof spectrum[0]);
    std::memcpy(spectrum, tmp_data, half_floor * sizeof spectrum[0]);

    return *this;
}

Spectrum &Spectrum::ifftshift() {
    assert(p->spectrum.size() == fftsize);
    size_t half_floor = fftsize/2;
    size_t half_ceil = (fftsize+1)/2;
    assert(half_floor + half_ceil == fftsize);
    decltype (p->spectrum) tmp(half_floor);

    auto spectrum = p->spectrum.data();
    auto tmp_data = tmp.data();
    std::memcpy(tmp_data, spectrum, half_floor * sizeof spectrum[0]);
    std::memmove(spectrum, &spectrum[half_floor], half_ceil * sizeof spectrum[0]);
    std::memcpy(&spectrum[half_ceil], tmp_data, half_floor * sizeof spectrum[0]);

    return *this;
}

SignalSegment Spectrum::fftshift(const SignalSegment &signal) {
    ssize_t signal_size = signal.size();
    ssize_t half_floor = signal_size/2;
    ssize_t half_ceil = (signal_size+1)/2;
    assert(half_floor + half_ceil == signal_size);

    SignalSegment result(signal_size);
    const auto signal_buffer = signal.buffer();
    auto result_buffer = result.buffer();

    std::memcpy(result_buffer, &signal_buffer[half_ceil], half_floor * sizeof result_buffer[0]);
    std::memcpy(&result_buffer[half_floor], signal_buffer, half_ceil * sizeof result_buffer[0]);

    return result;
}

SignalSegment Spectrum::ifftshift(const SignalSegment &signal) {
    ssize_t signal_size = signal.size();
    ssize_t half_floor = signal_size/2;
    ssize_t half_ceil = (signal_size+1)/2;
    assert(half_floor + half_ceil == signal_size);

    SignalSegment result(signal_size);
    const auto signal_buffer = signal.buffer();
    auto result_buffer = result.buffer();

    std::memcpy(result_buffer, &signal_buffer[half_floor], half_ceil * sizeof result_buffer[0]);
    std::memcpy(&result_buffer[half_ceil], signal_buffer, half_floor * sizeof result_buffer[0]);

    return result;
}

}
