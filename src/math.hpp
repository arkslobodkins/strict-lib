//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once


#include "Common/common.hpp"


namespace slib::math {


STRICT_CONSTEXPR_INLINE static Strict<long int> factorial(ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > -1_sl);
   index_t nf = n.get();
   return nf == 0_sl ? 1_sl : nf * factorial(nf - 1_sl);
}


STRICT_CONSTEXPR_INLINE static Strict<long int> binom_coeff(ImplicitInt n, ImplicitInt k) {
   ASSERT_STRICT_DEBUG(k.get() > -1_sl);
   ASSERT_STRICT_DEBUG(n.get() >= k.get());
   index_t d1 = n.get() - k.get();
   index_t d2 = k.get();

   index_t prod = 1_sl;
   for(index_t f1 = d1 + 1_sl, f2 = 1_sl; f1 <= d1 + d2; ++f1, ++f2) {
      prod = prod * f1 / f2;  // can prove that (prod * f1) is divisible by f2
   }
   return prod;
}


}  // namespace slib::math
