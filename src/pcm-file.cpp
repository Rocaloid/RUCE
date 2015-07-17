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

#include "pcm-file.hpp"
#include <cstdio>
#include <ios>
#include <libwintf8/fileio.h>
#include <libwintf8/u8str.h>
#ifdef _WIN32
#include <windows.h>
#define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#endif
#include <sndfile.h>

namespace RUCE {

struct PCMFile::Private {
    FILE *file = nullptr;

    SNDFILE *sf = nullptr;
    SF_INFO sf_info;
    static SF_VIRTUAL_IO sf_vio_func;
    static sf_count_t sf_vio_get_filelen_func(void *user_data);
    static sf_count_t sf_vio_seek_func(sf_count_t offset, int whence, void *user_data);
    static sf_count_t sf_vio_read_func(void *ptr, sf_count_t count, void *user_data);
    static sf_count_t sf_vio_write_func(const void *ptr, sf_count_t count, void *user_data);
    static sf_count_t sf_vio_tell_func(void *user_data);
};

SF_VIRTUAL_IO PCMFile::Private::sf_vio_func = {
    PCMFile::Private::sf_vio_get_filelen_func,
    PCMFile::Private::sf_vio_seek_func,
    PCMFile::Private::sf_vio_read_func,
    PCMFile::Private::sf_vio_write_func,
    PCMFile::Private::sf_vio_tell_func
};

PCMFile::PCMFile() {
}

PCMFile::PCMFile(const WTF8::u8string &filename, std::ios_base::openmode mode) {
    open(filename, mode);
}

PCMFile::~PCMFile() {
    close();
}

PCMFile &PCMFile::open(const WTF8::u8string &filename, std::ios_base::openmode mode) {
    close();
    const char *cmode = "rb";
    int sfmode = SFM_READ;
    if((mode & (std::ios_base::in | std::ios_base::out)) == (std::ios_base::in | std::ios_base::out)) {
        cmode = "r+b";
        sfmode = SFM_RDWR;
    } else if(mode & std::ios_base::out) {
        cmode = "wb";
        sfmode = SFM_WRITE;
    }
    p->file = WTF8::fopen(filename.c_str(), cmode);
    if(!p->file)
        throw FileError();
    p->sf = sf_open_virtual(&p->sf_vio_func, sfmode, &p->sf_info, p->file);
    if(!p->sf) {
        close();
        throw FileError();
    }
    return *this;
}

bool PCMFile::is_open() const {
    return p->sf != nullptr;
}

PCMFile &PCMFile::close() {
    if(p->sf) {
        sf_close(p->sf);
        p->sf = nullptr;
    }
    if(p->file) {
        WTF8::fclose(p->file);
        p->file = nullptr;
    }
    return *this;
}

size_t PCMFile::read(short *output_buf, size_t frames) {
    return sf_read_short(p->sf, output_buf, frames);
}

size_t PCMFile::read(int *output_buf, size_t frames) {
    return sf_read_int(p->sf, output_buf, frames);
}

size_t PCMFile::read(float *output_buf, size_t frames) {
    return sf_read_float(p->sf, output_buf, frames);
}

size_t PCMFile::read(double *output_buf, size_t frames) {
    return sf_read_double(p->sf, output_buf, frames);
}

size_t PCMFile::write(const short *input_buf, size_t frames) {
    return sf_write_short(p->sf, const_cast<short *>(input_buf), frames);
}

size_t PCMFile::write(const int *input_buf, size_t frames) {
    return sf_write_int(p->sf, const_cast<int *>(input_buf), frames);
}

size_t PCMFile::write(const float *input_buf, size_t frames) {
    return sf_write_float(p->sf, const_cast<float *>(input_buf), frames);
}

size_t PCMFile::write(const double *input_buf, size_t frames) {
    return sf_write_double(p->sf, const_cast<double *>(input_buf), frames);
}

SNDFILE *PCMFile::get_sndfile() const {
    return p->sf;
}

SF_INFO &PCMFile::get_sndfile_info() const {
    return p->sf_info;
}

sf_count_t PCMFile::Private::sf_vio_get_filelen_func(void *user_data) {
    FILE *file = reinterpret_cast<FILE *>(user_data);
    off_t old_offset = ftello(file);
    if(old_offset < 0)
        throw FileError();
    if(fseeko(file, 0, SEEK_END) != 0)
        throw FileError();
    off_t file_size = ftello(file);
    if(fseeko(file, old_offset, SEEK_SET) != 0 || file_size < 0)
        throw FileError();
    return file_size;
}

sf_count_t PCMFile::Private::sf_vio_seek_func(sf_count_t offset, int whence, void *user_data) {
    FILE *file = reinterpret_cast<FILE *>(user_data);
#ifdef _WIN32
    if(_fseeki64(file, offset, whence) != 0)
        throw FileError();
    __int64 new_offset = _ftelli64(file);
#else
    if(fseeko(file, offset, whence) != 0)
        throw FileError();
    off_t new_offset = ftello(file);
#endif
    if(new_offset < 0)
        throw FileError();
    return new_offset;
}

sf_count_t PCMFile::Private::sf_vio_read_func(void *ptr, sf_count_t count, void *user_data) {
    FILE *file = reinterpret_cast<FILE *>(user_data);
    return std::fread(ptr, 1, count, file);
}

sf_count_t PCMFile::Private::sf_vio_write_func(const void *ptr, sf_count_t count, void *user_data) {
    FILE *file = reinterpret_cast<FILE *>(user_data);
    return std::fwrite(ptr, 1, count, file);
}

sf_count_t PCMFile::Private::sf_vio_tell_func(void *user_data) {
    FILE *file = reinterpret_cast<FILE *>(user_data);
#ifdef _WIN32
    __int64 offset = _ftelli64(file);
#else
    off_t offset = ftello(file);
#endif
    if(offset < 0)
        throw FileError();
    return offset;
}

}
