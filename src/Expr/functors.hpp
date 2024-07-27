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


#include "../Common/auxiliary_types.hpp"
#include "../Common/strict_val.hpp"


namespace slib {


struct UnaryPlus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return +x;
   }
};


struct UnaryMinus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      if constexpr(Integer<T>) {
         static_assert(SignedInteger<T>);
      }
      return -x;
   }
};


struct UnaryNot {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return ~x;
   }

   template <Boolean T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return !x;
   }
};


struct UnaryAbs {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return abss(x);
   }
};


struct UnaryExp {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return exps(x);
   }
};


struct UnaryLog {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return logs(x);
   }
};


struct UnaryLog2 {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return log2s(x);
   }
};


struct UnaryLog10 {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return log10s(x);
   }
};


struct UnarySqrt {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return sqrts(x);
   }
};


struct UnaryCbrt {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return cbrts(x);
   }
};


struct UnarySin {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return sins(x);
   }
};


struct UnaryCos {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return coss(x);
   }
};


struct UnaryTan {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return tans(x);
   }
};


template <Floating T>
struct UnaryPow {
   STRICT_CONSTEXPR_2026 explicit UnaryPow(Strict<T> p) : p_{p} {
   }

   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return pows(x, p_);
   }

private:
   Strict<T> p_;
};


template <Integer T>
struct UnaryFastPowInt {
   STRICT_CONSTEXPR explicit UnaryFastPowInt(Strict<T> p) : p_{p} {
   }

   template <Floating U>
   STRICT_CONSTEXPR Strict<U> operator()(Strict<U> x) const {
      return fast_pows_int(x, p_);
   }

private:
   Strict<T> p_;
};


struct UnaryInv {
   template <Floating T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return invs(x);
   }
};


template <Builtin T>
struct UnaryCast {
   template <Builtin U>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<U> x) const {
      return strict_cast<T>(x);
   }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BinaryPlus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x + y;
   }
};


struct BinaryMinus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x - y;
   }
};


struct BinaryMult {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x * y;
   }
};


struct BinaryDivide {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x / y;
   }
};


struct BinaryModulo {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x % y;
   }
};


struct BinaryRightShift {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x << y;
   }
};


struct BinaryLeftShift {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x >> y;
   }
};


struct BinaryBitwiseAnd {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x & y;
   }
};


struct BinaryBitwiseOr {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x | y;
   }
};


struct BinaryBitwiseXor {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x ^ y;
   }
};


struct BinaryTwoProdFirst {
   template <Floating T>
   Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return two_prods(x, y).first;
   }
};


struct BinaryTwoProdSecond {
   template <Floating T>
   Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return two_prods(x, y).second;
   }
};


}  // namespace slib

