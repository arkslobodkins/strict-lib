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


#include <algorithm>  // sort, shuffle
#include <concepts>   // invocable
#include <memory>     // unique_ptr, make_unique
#include <random>     // random_device, mt19937
#include <tuple>      // tuple
#include <utility>    // pair, declval, forward
#include <vector>     // vector

#include "Common/common.hpp"
#include "Expr/array_expr1D.hpp"


// generic functions that work on Arrays, SliceArrays, and their expression templates.
namespace slib {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base, typename F> concept ForCallable
    = std::invocable<F, ValueTypeOf<Base>> && SameAs<void, decltype(std::declval<F>()(ValueTypeOf<Base>{}))>;


template <typename Base, typename F> concept CallableArgs1
    = std::invocable<F, ValueTypeOf<Base>>
   && SameAs<StrictBool, decltype(std::declval<F>()(ValueTypeOf<Base>{}))>;


template <typename Base1, typename Base2, typename F> concept CallableArgs2
    = std::invocable<F, ValueTypeOf<Base1>, ValueTypeOf<Base2>>
   && SameAs<StrictBool, decltype(std::declval<F>()(ValueTypeOf<Base1>{}, ValueTypeOf<Base2>{}))>;


template <typename Base, typename F> concept SortableArgs
    = std::invocable<F, ValueTypeOf<Base>, ValueTypeOf<Base>>
   && SameAs<StrictBool, decltype(std::declval<F>()(ValueTypeOf<Base>{}, ValueTypeOf<Base>{}))>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <RealBaseType Base>
STRICT_CONSTEXPR auto sum(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto prod(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto mean(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto min(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto max(const Base& A);


// returns std::pair<index_t, Strict<real_type>>
// or
// returns std::tuple<index_t, index_t, Strict<real_type>>
// depending on the type
template <RealBaseType Base>
STRICT_CONSTEXPR auto min_index(const Base& A);


// returns std::pair<index_t, Strict<real_type>>
// or
// returns std::tuple<index_t, index_t, Strict<real_type>>
// depending on the type
template <RealBaseType Base>
STRICT_CONSTEXPR auto max_index(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool all_finite(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_inf(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_nan(const Base& A);


template <RealBaseType Base1, RealBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR auto dot_prod(const Base1& A1, const Base2& A2);


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto norm_inf(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto norm1(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto norm1_scaled(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm2(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm2_scaled(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm_lp(const Base& A, ImplicitInt p);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm_lp_scaled(const Base& A, ImplicitInt p);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto polynomial(const Base& coeffs, ValueTypeOf<Base> x);


template <OneDimRealBaseType Base1, OneDimSignedIntegerBaseType Base2>
STRICT_CONSTEXPR_2026 auto polynomial(const Base1& coeffs, ValueTypeOf<Base1> x, const Base2& powers);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2, OneDimSignedIntegerBaseType Base3>
STRICT_CONSTEXPR_2026 auto gpolynomial(const Base1& coeffs, const Base2& x, const Base3& powers);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool has_zero(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_pos(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_neg(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_pos(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_neg(const Base& A);


template <BaseType Base, typename F>
   requires CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, F f);


template <BaseType Base1, BaseType Base2, typename F>
   requires(CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool none_of(const Base1& A1, const Base2& A2, F f);


template <BaseType Base, typename F>
   requires CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, F f);


template <BaseType Base1, BaseType Base2, typename F>
   requires(CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool any_of(const Base1& A1, const Base2& A2, F f);


template <BaseType Base, typename F>
   requires CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, F f);


template <BaseType Base1, BaseType Base2, typename F>
   requires(CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool all_of(const Base1& A1, const Base2& A2, F f);


// A is allowed to be empty
template <RealBaseType Base>
STRICT_CONSTEXPR_2023 std::unique_ptr<RealTypeOf<Base>[]> blas_array(const Base& A);


// A is allowed to be empty
// returns a slice array of all elements that are inside the interval (low, high)
// only applies to one-dimensional types
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high);


// Same as above
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, Low<RealTypeOf<Base>> low, High<RealTypeOf<Base>> high);


// A is allowed to be empty
// returns a slice array of all elements that are inside the interval [low, high]
// only applies to one-dimensional types
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high);


// Same as above
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, Low<RealTypeOf<Base>> low, High<RealTypeOf<Base>> high);


// A is allowed to be empty
// returns a slice array of all elements that evaluate true for f
// only applies to one-dimensional types
template <typename Base, typename F>
   requires(OneDimRealBaseType<RemoveRef<Base>> && CallableArgs1<Base, F> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_cond_range(Base&& A, F f);


// A is allowed to be empty
template <typename Base, typename F>
   requires(ForCallable<Base, F>)
STRICT_CONSTEXPR void for_each(Base&& A, F f);


// A is allowed to be empty
template <typename Base, typename F>
   requires(NonConstBaseType<RemoveRef<Base>> && SortableArgs<Base, F> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort(Base&& A, F f);


// A is allowed to be empty
template <typename Base>
   requires(NonConstBaseType<RemoveRef<Base>> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_increasing(Base&& A);


// A is allowed to be empty
template <typename Base>
   requires(NonConstBaseType<RemoveRef<Base>> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_decreasing(Base&& A);


// A is allowed to be empty
template <typename Base>
   requires(NonConstBaseType<RemoveRef<Base>> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
void shuffle(Base&& A);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {
template <RealBaseType Base>
STRICT_CONSTEXPR auto rec_sum(const Base& A) {
   ValueTypeOf<Base> s{};
   for(index_t i = 0_sl; i < A.size(); ++i) {
      s += A.index(i);
   }
   return s;
}
}  // namespace internal


template <RealBaseType Base>
STRICT_CONSTEXPR auto sum(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   index_t block_size = 64_sl;
   index_t nblocks = A.size() / block_size;
   index_t rem = A.size() % block_size;

   Array1D<RealTypeOf<Base>> sb(nblocks);
   for(index_t i = 0_sl; i < nblocks; ++i) {
      for(index_t j = 0_sl; j < block_size; ++j) {
         sb[i] += A.index(i * block_size + j);
      }
   }

   ValueTypeOf<Base> r{};
   for(index_t i = 0_sl; i < rem; ++i) {
      r += A.index(A.size() - 1_sl - i);
   }
   return internal::rec_sum(sb) + r;
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto prod(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto p = A.index(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      p *= A.index(i);
   }
   return p;
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto mean(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return sum(A) / value_type_cast<Base>(A.size());
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto min(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto min_elem = A.index(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      min_elem = mins(A.index(i), min_elem);
   }
   return min_elem;
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto max(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto max_elem = A.index(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      max_elem = maxs(A.index(i), max_elem);
   }
   return max_elem;
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto min_index(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<Base>;

   if constexpr(OneDimBaseType<Base>) {
      std::pair<index_t, value_type> min = {0_sl, A.index(0)};
      for(index_t i = 1_sl; i < A.size(); ++i) {
         if(auto xi = A.index(i); xi < min.second) {
            min = {i, xi};
         }
      }
      return min;
   } else {
      std::tuple<index_t, index_t, value_type> min = {0_sl, 0_sl, A.index(0, 0)};
      for(index_t i = 0_sl; i < A.rows(); ++i) {
         for(index_t j = 0_sl; j < A.cols(); ++j) {
            if(auto xij = A.index(i, j); xij < std::get<2>(min)) {
               min = {i, j, xij};
            }
         }
      }
      return min;
   }
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto max_index(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<Base>;

   if constexpr(OneDimBaseType<Base>) {
      std::pair<index_t, value_type> max = {0_sl, A.index(0)};
      for(index_t i = 1_sl; i < A.size(); ++i) {
         if(auto xi = A.index(i); xi > max.second) {
            max = {i, xi};
         }
      }
      return max;
   } else {
      std::tuple<index_t, index_t, value_type> max = {0_sl, 0_sl, A.index(0, 0)};
      for(index_t i = 0_sl; i < A.rows(); ++i) {
         for(index_t j = 0_sl; j < A.cols(); ++j) {
            if(auto xij = A.index(i, j); xij > std::get<2>(max)) {
               max = {i, j, xij};
            }
         }
      }
      return max;
   }
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool all_finite(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return all_of(A, [](auto x) { return isfinites(x); });
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_inf(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return any_of(A, [](auto x) { return isinfs(x); });
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_nan(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return any_of(A, [](auto x) { return isnans(x); });
}


template <RealBaseType Base1, RealBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR auto dot_prod(const Base1& A1, const Base2& A2) {
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   return sum(A1 * A2);
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto norm_inf(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return max(abs(A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto norm1(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return sum(abs(A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto norm1_scaled(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return mean(abs(A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm2(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return sqrts(dot_prod(A, A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm2_scaled(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return norm2(A) / sqrts(value_type_cast<Base>(A.size()));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm_lp(const Base& A, ImplicitInt p) {
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(p.get() > 0_sl);
   auto s = sum(abs(pow_int(A, p.get())));
   return pows(s, invs(value_type_cast<Base>(p.get())));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto norm_lp_scaled(const Base& A, ImplicitInt p) {
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(p.get() > 0_sl);
   return norm_lp(A, p) / pows(value_type_cast<Base>(A.size()), invs(value_type_cast<Base>(p.get())));
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto polynomial(const Base& coeffs, ValueTypeOf<Base> x) {
   auto s = coeffs[last];
   for(index_t i = coeffs.size() - 2_sl; i >= 0_sl; --i) {
      s = coeffs.index(i) + s * x;
   }
   return s;
}


template <OneDimRealBaseType Base1, OneDimSignedIntegerBaseType Base2>
STRICT_CONSTEXPR_2026 auto polynomial(const Base1& coeffs, ValueTypeOf<Base1> x, const Base2& powers) {
   ASSERT_STRICT_DEBUG(!coeffs.empty());
   ASSERT_STRICT_DEBUG(same_size(coeffs, powers));
   ASSERT_STRICT_DEBUG(all_non_neg(powers));
   auto X = const1D(Size{coeffs.size()}, Value{x});
   return gpolynomial(coeffs, X, powers);
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2, OneDimSignedIntegerBaseType Base3>
STRICT_CONSTEXPR_2026 auto gpolynomial(const Base1& coeffs, const Base2& x, const Base3& powers) {
   ASSERT_STRICT_DEBUG(!x.empty());
   ASSERT_STRICT_DEBUG(same_size(coeffs, x, powers));
   ASSERT_STRICT_DEBUG(all_non_neg(powers));

   ValueTypeOf<Base1> z{};
   for(index_t i = 0_sl; i < x.size(); ++i) {
      z += coeffs.index(i) * pows_int(x.index(i), powers.index(i));
   }
   return z;
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool has_zero(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_zero = []<Real T>(const Strict<T>& x) { return x == Zero<T>; };
   return any_of(A, is_zero);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_pos(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_positive = []<Real T>(const Strict<T>& x) { return x > Zero<T>; };
   return all_of(A, is_positive);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_neg(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_negative = []<Real T>(const Strict<T>& x) { return x < Zero<T>; };
   return all_of(A, is_negative);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_pos(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_non_positive = []<Real T>(const Strict<T>& x) { return x <= Zero<T>; };
   return all_of(A, is_non_positive);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_neg(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_non_negative = []<Real T>(const Strict<T>& x) { return x >= Zero<T>; };
   return all_of(A, is_non_negative);
}


template <BaseType Base, typename F>
   requires CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, F f) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return !any_of(A, f);
}


template <BaseType Base1, BaseType Base2, typename F>
   requires(CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool none_of(const Base1& A1, const Base2& A2, F f) {
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   return !any_of(A1, A2, f);
}


template <BaseType Base, typename F>
   requires CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, F f) {
   ASSERT_STRICT_DEBUG(!A.empty());
   for(index_t i = 0_sl; i < A.size(); ++i) {
      if(f(A.index(i))) {
         return true_sb;
      }
   }
   return false_sb;
}


template <BaseType Base1, BaseType Base2, typename F>
   requires(CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool any_of(const Base1& A1, const Base2& A2, F f) {
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      if(f(A1.index(i), A2.index(i))) {
         return true_sb;
      }
   }
   return false_sb;
}


template <BaseType Base, typename F>
   requires CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, F f) {
   ASSERT_STRICT_DEBUG(!A.empty());
   for(index_t i = 0_sl; i < A.size(); ++i) {
      if(!f(A.index(i))) {
         return false_sb;
      }
   }
   return true_sb;
}


template <BaseType Base1, BaseType Base2, typename F>
   requires(CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool all_of(const Base1& A1, const Base2& A2, F f) {
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      if(!f(A1.index(i), A2.index(i))) {
         return false_sb;
      }
   }
   return true_sb;
}


template <RealBaseType Base>
STRICT_CONSTEXPR_2023 std::unique_ptr<RealTypeOf<Base>[]> blas_array(const Base& A) {
   if(A.empty()) {
      return nullptr;
   }

   using real_type = RealTypeOf<Base>;
   auto blas_array = std::make_unique<real_type[]>(to_size_t(A.size()));
   auto* ptr = blas_array.get();
   for(index_t i = 0_sl; i < A.size(); ++i) {
      ptr[i.val()] = A.index(i).val();
   }
   return blas_array;
}


template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return in_cond_range(std::forward<Base>(A), [low, high](auto x) { return x > low && x < high; });
}


// Same as above
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, Low<RealTypeOf<Base>> low, High<RealTypeOf<Base>> high) {
   return in_open_range(std::forward<Base>(A), low.get(), high.get());
}


template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return in_cond_range(std::forward<Base>(A), [low, high](auto x) { return x >= low && x <= high; });
}


template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, Low<RealTypeOf<Base>> low, High<RealTypeOf<Base>> high) {
   return in_closed_range(std::forward<Base>(A), low.get(), high.get());
}


template <typename Base, typename F>
   requires(OneDimRealBaseType<RemoveRef<Base>> && CallableArgs1<Base, F> && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_cond_range(Base&& A, F f) {
   std::vector<ImplicitInt> indexes;
   std::ranges::copy_if(irange(A), std::back_inserter(indexes), [&](auto i) { return bool{f(A.index(i))}; });
   return A(indexes);
}


// !IsConst is not used since A might not be modified
// Removed NonConstBaseType requirement so that for each can be
// called for expression templates as well
template <typename Base, typename F>
   requires(ForCallable<Base, F>)
STRICT_CONSTEXPR void for_each(Base&& A, F f) {
   for(index_t i = 0_sl; i < A.size(); ++i) {
      f(A.index(i));
   }
}


// A is allowed to be empty
template <typename Base, typename F>
   requires(NonConstBaseType<RemoveRef<Base>> && SortableArgs<Base, F> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort(Base&& A, F f) {
   std::sort(A.begin(), A.end(), f);
}


template <typename Base>
   requires(NonConstBaseType<RemoveRef<Base>> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_increasing(Base&& A) {
   std::sort(A.begin(), A.end(), [](const auto& a, const auto& b) { return a < b; });
}


template <typename Base>
   requires(NonConstBaseType<RemoveRef<Base>> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_decreasing(Base&& A) {
   std::sort(A.begin(), A.end(), [](const auto& a, const auto& b) { return a > b; });
}


template <typename Base>
   requires(NonConstBaseType<RemoveRef<Base>> && !IsConst<RemoveRef<Base>>
            && !ArrayOneDimRealTypeRvalue<Base>)
void shuffle(Base&& A) {
   std::mt19937 g{std::random_device{}()};
   std::shuffle(A.begin(), A.end(), g);
}


}  // namespace slib
