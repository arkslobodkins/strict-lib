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

template<FloatingType T>
bool within_tol_rel(StrictVal<T> val1, StrictVal<T> val2, T tol = T{1.e-14})
{
   auto abs1 = abss(val1);
   auto abs2 = abss(val2);
   return abss(val1 - val2) / maxs(abs1, abs2) < tol;
}

template<FloatingType T>
bool within_tol_rel_balanced(StrictVal<T> val1, StrictVal<T> val2, T tol = T{1.e-14}, T bal = T{1.e-14})
{
   auto abs1 = abss(val1);
   auto abs2 = abss(val2);
   if(maxs(abs1, abs2) < bal)
      return within_tol_abs(val1, val2, tol);
   return abss(val1 - val2) / maxs(abs1, abs2) < tol;
}

}

#endif
