// Copyright 2013, ARM Limited
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of ARM Limited nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef VIXL_GLOBALS_H
#define VIXL_GLOBALS_H

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#include <stdint.h>

// Get the standard printf format macros for C99 stdint types.
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "platform.h"

#ifdef __ANDROID__
#ifndef LOG_TAG
#define LOG_TAG "VIXL"
#endif
#include <cutils/log.h>
#endif

typedef uint8_t byte;

const int KBytes = 1024;
const int MBytes = 1024 * KBytes;
const int GBytes = 1024 * MBytes;

#ifdef __ANDROID__
  #define VIXL_ABORT() ALOGE("in %s, line %i", __FILE__, __LINE__); abort()
#else
  #define VIXL_ABORT() printf("in %s, line %i", __FILE__, __LINE__); abort()
#endif

#ifdef DEBUG
  #define VIXL_ASSERT(condition) assert(condition)
  #define VIXL_CHECK(condition) VIXL_ASSERT(condition)
#ifdef __ANDROID__
  #define VIXL_UNIMPLEMENTED() ALOGD("UNIMPLEMENTED @%s:%d\t", __FILE__, __LINE__); VIXL_ABORT()
  #define VIXL_UNREACHABLE() printf("UNREACHABLE @%s:%d\t", __FILE__, __LINE__); VIXL_ABORT()
#else
  #define VIXL_UNIMPLEMENTED() printf("UNIMPLEMENTED\t"); VIXL_ABORT()
  #define VIXL_UNREACHABLE() printf("UNREACHABLE\t"); VIXL_ABORT()
#endif
#else
  #define VIXL_ASSERT(condition) ((void) 0)
  #define VIXL_CHECK(condition) assert(condition)
  #define VIXL_UNIMPLEMENTED() ((void) 0)
  #define VIXL_UNREACHABLE() ((void) 0)
#endif

template <typename T> inline void USE(T) {}

#define VIXL_ALIGNMENT_EXCEPTION() printf("ALIGNMENT EXCEPTION\t"); VIXL_ABORT()

#endif  // VIXL_GLOBALS_H
