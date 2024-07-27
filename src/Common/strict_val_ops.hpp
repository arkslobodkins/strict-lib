// Arkadijs Slobodkins, 2023


#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <utility>

#include "auxiliary_types.hpp"
#include "constants.hpp"
#include "strict_val.hpp"


namespace slib {


template <typename T>
using StrictPair = std::pair<Strict<T>, Strict<T>>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> clamps(Strict<T> x, Strict<T> low, Strict<T> high);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> clamps(Strict<T> x, Low<T> low, High<T> high);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> pows_int(Strict<T> x, ImplicitInt p);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> fast_pows_int(Strict<T> x, ImplicitInt p);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> exps_int(ImplicitInt p);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> fmas(Strict<T> x, Strict<T> y, Strict<T> z);


template <Floating T>
STRICT_NODISCARD_INLINE StrictPair<T> two_sums(Strict<T> x, Strict<T> y);


template <Floating T>
STRICT_NODISCARD_INLINE StrictPair<T> two_prods(Strict<T> x, Strict<T> y);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_open(Strict<T> x, Strict<T> low, Strict<T> high);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_open(Value<T> x, Low<T> low, High<T> high);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_closed(Strict<T> x, Strict<T> low, Strict<T> high);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_closed(Value<T> x, Low<T> low, High<T> high);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> clamps(Strict<T> x, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   if constexpr(Floating<T>) {
      ASSERT_STRICT_DEBUG(!isnans(x));
      ASSERT_STRICT_DEBUG(!isnans(low));
      ASSERT_STRICT_DEBUG(!isnans(high));
   }
   return Strict{std::clamp(x.val(), low.val(), high.val())};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> clamps(Strict<T> x, Low<T> low, High<T> high) {
   return clamps(x, low.get(), high.get());
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> pows_int(Strict<T> x, ImplicitInt p) {
   return pows(x, strict_cast<T>(p.get()));
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR Strict<T> fast_pows_int(Strict<T> x, const ImplicitInt p) {
   auto res = One<T>;
   auto power = abss(p.get());
   for(;;) {
      if(strict_cast<bool>(power & 1_sl)) {
         res *= x;
      }
      power >>= 1_sl;
      if(!strict_cast<bool>(power)) {
         break;
      }
      x *= x;
   }
   return p.get() >= 0_sl ? res : invs(res);
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> exps_int(ImplicitInt p) {
   return exps(strict_cast<T>(p.get()));
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> fmas(Strict<T> x, Strict<T> y, Strict<T> z) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::fma(T{x}, T{y}, T{z})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{fmaq(T{x}, T{y}, T{z})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_INLINE StrictPair<T> two_sums(Strict<T> x, Strict<T> y) {
   volatile T r = x.val() + y.val();
   volatile T z = r - x.val();
   T s = ((x.val() - (r - z)) + (y.val() - z));
   return StrictPair<T>{r, s};
}


template <Floating T>
STRICT_NODISCARD_INLINE StrictPair<T> two_prods(Strict<T> x, Strict<T> y) {
   auto r = x * y;
   auto s = fmas(x, y, -r);
   return StrictPair<T>{r, s};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_open(Strict<T> x, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return x > low && x < high;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_open(Value<T> x, Low<T> low, High<T> high) {
   return in_open(x.get(), low.get(), high.get());
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_closed(Strict<T> x, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return x >= low && x <= high;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_closed(Value<T> x, Low<T> low, High<T> high) {
   return in_closed(x.get(), low.get(), high.get());
}


}  // namespace slib
