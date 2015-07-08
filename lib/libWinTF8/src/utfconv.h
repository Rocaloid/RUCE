/*
  Copyright (c) 2014 StarBrilliant <m13253@hotmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms are permitted
  provided that the above copyright notice and this paragraph are
  duplicated in all such forms and that any documentation,
  advertising materials, and other materials related to such
  distribution and use acknowledge that the software was developed by
  StarBrilliant.
  The name of StarBrilliant may not be used to endorse or promote
  products derived from this software without specific prior written
  permission.

  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
#pragma once
#ifndef WTF8_UTFCONV_H_INCLUDED_
#define WTF8_UTFCONV_H_INCLUDED_

#ifdef __cplusplus
#include <cstddef>
#include <stdexcept>
#include <string>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
namespace WTF8 {

class unicode_conversion_error : public std::runtime_error {
public:
    unicode_conversion_error(const char *what) : std::runtime_error(what) {}
    unicode_conversion_error() : std::runtime_error("Can not convert string to Unicode") {}
};

/* Microsoft VC++ Runtime behaves differently on different versions,
   we will just reimplement them ourselves. */
std::wstring utf8_to_wide(const std::string &utf8str, bool strict = false);
std::string wide_to_utf8(const std::wstring &widestr, bool strict = false);
std::string utf8_validify(const std::string &utf8str, bool strict = false);
size_t utf8_count_codepoints(const std::string &utf8str, bool strict = false);

};
#endif

#ifdef __cplusplus
extern "C" {
#endif

static const size_t WTF8_UNICODE_CONVERT_ERROR = ~(size_t) 0 /* -1 */;

size_t WTF8_utf8_to_wide(wchar_t *widestr, const char *utf8str, int strict, size_t bufsize);
size_t WTF8_wide_to_utf8(char *utf8str, const wchar_t *widestr, int strict, size_t bufsize);
size_t WTF8_utf8_validify(char *validstr, const char *utf8str, int strict, size_t bufsize);
size_t WTF8_utf8_count_codepoints(const char *utf8str, int strict);

#ifdef __cplusplus
}
#endif

#endif
