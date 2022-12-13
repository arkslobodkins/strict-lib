#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <cassert>
#include <stdexcept>
#include <string>

#define STRICT_ARRAY_THROW_OUT_OF_RANGE()                                                   \
   do {                                                                                     \
   throw std::out_of_range{"OUT OF RANGE! " + trace_err(__FILE__, __func__, __LINE__)};     \
   } while(0)

#define STRICT_ARRAY_THROW_ZERO_DIVISION()                                                    \
   do {                                                                                       \
   throw std::runtime_error{"ZERO DIVISION! " + trace_err(__FILE__, __func__, __LINE__)};     \
   } while(0)

#ifdef STRICT_ARRAY_DEBUG_ON
#define ASSERT_STRICT_ARRAY_DEBUG(condition) assert(condition)
#else
#define ASSERT_STRICT_ARRAY_DEBUG(condition) ((void)0)
#endif

namespace strict_array
{
   static inline std::string trace_err(const char* file, const char* func, int line)
   {
      return "file: " + std::string(file) + ", function: " + std::string(func) + ", line: " + std::to_string(line);
   }
}

#endif
