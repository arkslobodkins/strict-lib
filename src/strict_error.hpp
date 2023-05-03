// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <iostream>
#include <stdexcept>
#include <string>

#define STRICT_THROW_OUT_OF_RANGE()                                                         \
   do {                                                                                     \
      throw std::out_of_range{"OUT OF RANGE! " + trace_err(__FILE__, __func__, __LINE__)};  \
   } while(0)

#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_DEBUG(condition)                                      \
   do {                                                                     \
      if(!(condition)) {                                                    \
         std::cerr << trace_err(__FILE__, __func__, __LINE__) <<            \
         ":\nassertion " << (#condition) << " failed" << std::endl;         \
         std::abort();                                                      \
      }                                                                     \
   } while(0)
#else
#define ASSERT_STRICT_DEBUG(condition) ((void)0)
#endif

namespace strict_array
{
   inline std::string trace_err(const char* file, const char* func, int line)
   {
      return "file: " + std::string(file) + ", function: " + std::string(func) + ", line: " + std::to_string(line);
   }
}

#endif