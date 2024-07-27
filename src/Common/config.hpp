//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


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

