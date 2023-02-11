// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include "strict_val.hpp"

namespace strict_array {

template<FloatingType T>
bool within_tol_abs(StrictVal<T> val1, StrictVal<T> val2, T tol = T{1.e-14})
{ return abss(val1 - val2) < tol; }

}

#endif
