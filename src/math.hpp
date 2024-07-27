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
