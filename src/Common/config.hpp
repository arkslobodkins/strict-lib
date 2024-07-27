//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
//
//  This file is part of the strict-lib library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//


#pragma once


#include <iostream>
#if __has_include(<version>)
#include <version>
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined __GNUG__ || defined __INTEL_COMPILER || defined __INTEL_LLVM_COMPILER || defined __clang__
#define STRICT_RESTRICT __restrict
#else
#define STRICT_RESTRICT
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUAD_PRECISION

#ifdef __GNUG__

#if __has_include(<quadmath.h>)
#include <quadmath.h>
#else
#error QUADRUPLE PRECISION IS NOT PROVIDED ON THIS COMPILER. TO ENABLE IT, COMPILE WITH GCC COMPILER THAT SUPPORTS QUAD PRECISION.
#endif

#else
#error QUADRUPLE PRECISION IS NOT PROVIDED ON THIS COMPILER. TO ENABLE IT, COMPILE WITH GCC COMPILER THAT SUPPORTS QUAD PRECISION.
#endif

#endif


#ifdef STRICT_STACKTRACE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cpp_lib_stacktrace

#include <stacktrace>
#define STRICT_PRINT_STACKTRACE                                      \
   do {                                                              \
      auto s = std::stacktrace::current();                           \
      for(std::stacktrace::size_type i = 0; i < s.size() - 4; ++i) { \
         std::cout << s[i] << '\n';                                  \
      }                                                              \
      std::cout << "..." << std::endl;                               \
   } while(0)

#else

#error STACKTRACE IS NOT AVAILABLE. compile without STRICT_STACKTRACE or compile with C++23 staacktrace support.
#define STRICT_PRINT_STACKTRACE ((void)0)

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else
#define STRICT_PRINT_STACKTRACE ((void)0)
#endif


namespace slib {


static inline void config_info() {
#ifndef STRICT_DEBUG_OFF
   std::cout << "compute mode: DEBUG" << '\n';
#else
   std::cout << "compute mode: RELEASE" << '\n';
#endif

#ifndef STRICT_ERROR_EXCEPTIONS
   std::cout << "error handling: ASSERTIONS" << '\n';
#else
   std::cout << "error handling: EXCEPTIONS" << '\n';
#endif

#ifndef STRICT_QUAD_PRECISION
   std::cout << "quadruple precision: OFF" << '\n';
#else
   std::cout << "quadruple precision: ON" << '\n';
#endif

#ifndef STRICT_STACKTRACE
   std::cout << "C++23 stacktrace: OFF" << '\n';
#else
   std::cout << "C++23 stacktrace: ON" << '\n';
#endif
}


}  // namespace slib

