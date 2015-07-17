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

#ifndef RUCE_PCM_FILE_HPP
#define RUCE_PCM_FILE_HPP

#include "proxy-ptr.hpp"
#include <ios>
#include <stdexcept>
#include <libwintf8/u8str.h>

typedef struct SNDFILE_tag SNDFILE;
typedef struct SF_INFO SF_INFO;

namespace RUCE {

class PCMFile {
public:
    PCMFile();
    PCMFile(const WTF8::u8string &filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    PCMFile &open(const WTF8::u8string &filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    ~PCMFile();
    bool is_open() const;
    PCMFile &close();

    // frames = samples * channels
    size_t read(short *output_buf, size_t frames);
    size_t read(int *output_buf, size_t frames);
    size_t read(float *output_buf, size_t frames);
    size_t read(double *output_buf, size_t frames);

    size_t write(const short *input_buf, size_t frames);
    size_t write(const int *input_buf, size_t frames);
    size_t write(const float *input_buf, size_t frames);
    size_t write(const double *input_buf, size_t frames);

    SNDFILE *get_sndfile() const;
    SF_INFO &get_sndfile_info() const;

    class FileError;
private:
    struct Private;
    proxy_ptr<Private> p;
};

class PCMFile::FileError : public std::runtime_error {
public:
    FileError() : std::runtime_error("File operation failed") {}
};

}

#endif
