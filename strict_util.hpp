// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <chrono>
#include "strict_val.hpp"

#define STRICT_TIME(a)                                        \
do {                                                          \
   strict_array::timer t;                                     \
   a;                                                         \
   std::printf("%s took: %.4e seconds\n", #a, t.wall_time()); \
   } while (0)

namespace strict_array {

struct timer
{
   timer() { start = std::chrono::high_resolution_clock::now(); }
   void restart() { start = std::chrono::high_resolution_clock::now(); }

   double wall_time()
   {
      return double(std::chrono::duration_cast<std::chrono::nanoseconds>
         (std::chrono::high_resolution_clock::now() - start).count()) / 1.e9;
   }

private:
   std::chrono::system_clock::time_point start;
};


template<FloatingType T>
bool within_tol_abs(StrictVal<T> val1, StrictVal<T> val2, T tol = T{1.e-14})
{ return abss(val1 - val2) < tol; }

}

#endif
