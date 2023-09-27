// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include "strict_auxiliary.hpp"
#include "strict_val.hpp"

namespace strict::math {


StrictVal<strict_int> factorial(ImplicitInt n)
{
   ASSERT_STRICT_DEBUG(n.get() > -1);
   strict_int nf = n.get();
   return nf == 0 ? strict_cast<strict_int>(1) : nf * factorial(nf-1);
}


StrictVal<strict_int> binom_coeff(ImplicitInt n, ImplicitInt k)
{
   ASSERT_STRICT_DEBUG(k.get() > -1);
   ASSERT_STRICT_DEBUG(n.get() >= k.get());
   strict_int d1 = n.get() - k.get();
   strict_int d2 = k.get();

   strict_int prod = 1;
   for(strict_int f1 = d1+1, f2 = 1; f1 <= d1+d2; ++f1, ++f2) {
      prod = prod * f1/f2; // can prove that (prod * f1) is divisible by f2
   }
   return prod;
}


}

#endif
