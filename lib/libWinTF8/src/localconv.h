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
#ifndef WTF8_LOCALCONV_H_INCLUDED_
#define WTF8_LOCALCONV_H_INCLUDED_

#ifdef __cplusplus
#include <cstddef>
#include <string>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
namespace WTF8 {

std::string utf8_to_local(const std::string &utf8str, bool strict = false);
std::string local_to_utf8(const std::string &localstr, bool strict = false);

/* When interacting with third-party libraries which does not support Unicode,
   filenames that already exist may be converted to DOS short filename */
std::string utf8_to_dos_filename(const std::string &utf8_filename);

}

#endif

#ifdef __cplusplus
extern "C" {
#endif

size_t WTF8_utf8_to_local(char *localstr, const char *utf8str, int strict, size_t bufsize);
size_t WTF8_local_to_utf8(char *utf8str, const char *localstr, int strict, size_t bufsize);
size_t WTF8_utf8_to_dos_filename(char *dos_filename, const char *utf8_filename, size_t bufsize);

#ifdef __cplusplus
}
#endif

#endif
