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


#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "config.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace slib {

class StrictException : public std::exception {
public:
   explicit StrictException(std::string msg) : msg_{std::move(msg)} {
   }

   [[nodiscard]] const char* what() const noexcept override {
      return msg_.c_str();
   }

private:
   std::string msg_;
};

}  // namespace slib

#ifndef STRICT_DEBUG_OFF
#define NORMAL_ASSERT_STRICT_DEBUG_MSG(condition, msg)                                                  \
   do {                                                                                                 \
      if(!(condition)) {                                                                                \
         STRICT_PRINT_STACKTRACE;                                                                       \
         std::cerr << msg << trace_err(__FILE__, __func__, __LINE__) << ":\nassertion " << (#condition) \
                   << " failed" << std::endl;                                                           \
         std::abort();                                                                                  \
      }                                                                                                 \
   } while(false)

#define NORMAL_ASSERT_STRICT_DEBUG(condition) NORMAL_ASSERT_STRICT_DEBUG_MSG(condition, "")

#else
#define NORMAL_ASSERT_STRICT_DEBUG_MSG(condition, msg) ((void)0)
#define NORMAL_ASSERT_STRICT_DEBUG(condition) ((void)0)
#endif

// *********************************************************************************************************
// //
// *********************************************************************************************************
// //
// *********************************************************************************************************
// //
#ifndef STRICT_ERROR_EXCEPTIONS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_DEBUG_MSG(condition, msg)                                                         \
   do {                                                                                                 \
      if(!(condition)) {                                                                                \
         STRICT_PRINT_STACKTRACE;                                                                       \
         std::cerr << msg << trace_err(__FILE__, __func__, __LINE__) << ":\nassertion " << (#condition) \
                   << " failed" << std::endl;                                                           \
         std::abort();                                                                                  \
      }                                                                                                 \
   } while(false)

#define ASSERT_STRICT_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "")

#else
#define ASSERT_STRICT_DEBUG_MSG(condition, msg) ((void)0)
#define ASSERT_STRICT_DEBUG(condition) ((void)0)
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_DEBUG_MSG(condition, msg)                                                           \
   do {                                                                                                   \
      if(!(condition)) {                                                                                  \
         throw slib::StrictException{msg + trace_err(__FILE__, __func__, __LINE__) + ":\n" + (#condition) \
                                     + " failed, exception thrown"};                                      \
      }                                                                                                   \
   } while(false)

#define ASSERT_STRICT_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "")

#else
#define ASSERT_STRICT_DEBUG_MSG(condition, msg) ((void)0)
#define ASSERT_STRICT_DEBUG(condition) ((void)0)
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
// *********************************************************************************************************
// //
// *********************************************************************************************************
// //
// *********************************************************************************************************
// //

// *********************************************************************************************************
// //
// *********************************************************************************************************
// //
// *********************************************************************************************************
// //
#ifndef STRICT_ERROR_EXCEPTIONS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_STRICT_ALWAYS_MSG(condition, msg)                                                        \
   do {                                                                                                 \
      if(!(condition)) {                                                                                \
         STRICT_PRINT_STACKTRACE;                                                                       \
         std::cerr << msg << trace_err(__FILE__, __func__, __LINE__) << ":\nassertion " << (#condition) \
                   << " failed" << std::endl;                                                           \
         std::abort();                                                                                  \
      }                                                                                                 \
   } while(false)
#define ASSERT_STRICT_ALWAYS(condition) ASSERT_STRICT_ALWAYS_MSG(condition, "")
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_STRICT_ALWAYS_MSG(condition, msg)                                                          \
   do {                                                                                                   \
      if(!(condition)) {                                                                                  \
         throw slib::StrictException{msg + trace_err(__FILE__, __func__, __LINE__) + ":\n" + (#condition) \
                                     + " failed, exception thrown"};                                      \
      }                                                                                                   \
   } while(false)
#define ASSERT_STRICT_ALWAYS(condition) ASSERT_STRICT_ALWAYS_MSG(condition, "")

#define REQUIRE_STRICT_THROW(condition)                                                          \
   do {                                                                                          \
      try {                                                                                      \
         condition;                                                                              \
         STRICT_PRINT_STACKTRACE;                                                                \
         std::cerr << trace_err(__FILE__, __func__, __LINE__) << ":\nassertion " << (#condition) \
                   << " did not throw exception" << std::endl;                                   \
         std::abort();                                                                           \
      } catch(const slib::StrictException&) {                                                    \
      }                                                                                          \
   } while(false)

#define REQUIRE_STRICT_NOT_THROW(condition)                                                      \
   do {                                                                                          \
      try {                                                                                      \
         condition;                                                                              \
      } catch(const slib::StrictException&) {                                                    \
         STRICT_PRINT_STACKTRACE;                                                                \
         std::cerr << trace_err(__FILE__, __func__, __LINE__) << ":\nassertion " << (#condition) \
                   << " threw exception" << std::endl;                                           \
         std::abort();                                                                           \
      }                                                                                          \
   } while(false)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
// *********************************************************************************************************
// //
// *********************************************************************************************************
// //
// *********************************************************************************************************
// //

#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_RANGE_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "OUT OF RANGE!\n")
#else
#define ASSERT_STRICT_RANGE_DEBUG(condition) ((void)0)
#endif

#define ASSERT_STRICT_RANGE_ALWAYS(condition) ASSERT_STRICT_ALWAYS_MSG(condition, "OUT OF RANGE!\n")

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_DIVISION_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "INTEGER DIVISION BY 0!\n")
#else
#define ASSERT_STRICT_DIVISION_DEBUG(condition) ((void)0)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_REMAINDER_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "MODULO DIVISION BY 0!\n")
#else
#define ASSERT_STRICT_REMAINDER_DEBUG(condition) ((void)0)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_SHIFT_FIRST_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "FIRST SHIFT OPERAND HAS A NEGATIVE VALUE!\n")
#else
#define ASSERT_STRICT_SHIFT_FIRST_DEBUG(condition) ((void)0)
#endif

#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_SHIFT_SECOND_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "SECOND SHIFT OPERAND HAS A NEGATIVE VALUE!\n")
#else
#define ASSERT_STRICT_SHIFT_SECOND_DEBUG(condition) ((void)0)
#endif

namespace slib {
inline std::string trace_err(const char* file, const char* func, int line) {
   return std::string("file: ") + std::string(file) + std::string(", function: ") + std::string(func)
        + std::string(", line: ") + std::to_string(line);
}
}  // namespace slib
