// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_val.hpp"

namespace strict_array {

template<BaseType BaseT>
std::ostream & operator<<(std::ostream & os, const BaseT & A);

template<BaseType BaseT>
void print(const BaseT & A, const std::string & name);

template<BaseType BaseT>
[[nodiscard]] auto sum(const BaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_sum(const FloatBaseT & A);

template<BaseType BaseT>
[[nodiscard]] auto min(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] auto max(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] auto min_index(const BaseT & A);  // returns std::pair<size_type, StrictVal<real_type>>

template<BaseType BaseT>
[[nodiscard]] auto max_index(const BaseT & A);  // returns std::pair<size_type, StrictVal<real_type>>

template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool all_finite(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_inf(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2_scaled(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2_scaled(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1_scaled(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1_scaled(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_lp(const FloatBaseT & A, int p);

template<BaseType BaseT1, BaseType BaseT2>
[[nodiscard]] auto dot_prod(const BaseT1 & A1, const BaseT2 & A2);

template<FloatingBaseType FloatBaseT1, FloatingBaseType FloatBaseT2>
[[nodiscard]] auto stable_dot_prod(const FloatBaseT1 & A1, const FloatBaseT2 & A2);

template<BaseType BaseT>
[[nodiscard]] bool does_contain_zero(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] bool all_positive(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] bool all_negative(const BaseT & A);

// requires not needed, only for better error messages
template<DirectBaseType T, typename F>
requires (!std::is_const_v<T>)
void apply(T & A, F f);

// requires not needed, only for better error messages
template<DirectBaseType T, typename F, typename Cond>
requires (!std::is_const_v<T>)
void apply_if(T & A, F f, Cond c);

template<BaseType BaseT, typename F>
[[nodiscard]] bool any_satisfy(const BaseT & A, F f);

template<BaseType BaseT, typename F>
[[nodiscard]] bool all_satisfy(const BaseT & A, F f);

// only for non-const lvalue references
template<DirectBaseType T>
[[nodiscard]] auto within_range(T & A, ValueTypeOf<T> low, ValueTypeOf<T> high);

// only for non-const lvalue references
template<DirectBaseType T, typename Cond>
[[nodiscard]] auto within_cond(T & A, Cond c);

// only for const lvalue references
template<typename T>
requires (BaseType<std::remove_reference_t<T>> && std::is_lvalue_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>)
[[nodiscard]] auto within_range(T && A, ValueTypeOf<T> low, ValueTypeOf<T> high);

// only for const lvalue references
template<typename T, typename Cond>
requires (BaseType<std::remove_reference_t<T>> && std::is_lvalue_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>)
[[nodiscard]] auto within_cond(T && A, Cond c);

template<BaseType BaseT>
[[nodiscard]] std::unique_ptr<RealTypeOf<BaseT>[]> unique_blas_array(const BaseT & A);

template<typename T>
requires (DirectBaseType<std::decay_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_increasing(T && A);

template<typename T>
requires (DirectBaseType<std::decay_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_decreasing(T && A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {
   template<BaseType BaseT>
   std::string smart_spaces(SizeTypeOf<BaseT> max_ind, SizeTypeOf<BaseT> ind)
   {
      using size_type = SizeTypeOf<BaseT>;
      auto count_digit = [](size_type number) -> size_type {
         if(!number) return 1;
         return static_cast<size_type>(std::log10(number)) + 1;
      };

      size_type max_digits = count_digit(max_ind);
      size_type ind_digits = count_digit(ind);
      return std::string(static_cast<std::basic_string<char>::size_type>(1+max_digits-ind_digits), 32);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
std::ostream & operator<<(std::ostream & os, const BaseT & A)
{
   if(A.empty()) {
      std::cout << __func__ << " : empty Array based object" << std::endl;
   }
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      os << "[" << i << "] ="
         << internal::smart_spaces<BaseT>(A.size(), i) << A[i] << std::endl;
   }
   return os;
}

template<BaseType BaseT>
void print(const BaseT & A, const std::string & name)
{
   if(A.empty()) {
      std::cout << __func__ << " : " << name  << " -> empty Array based object" << std::endl;
   }
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      std::cout << name << "[" << i << "] ="
         << internal::smart_spaces<BaseT>(A.size(), i) << A[i] << std::endl;
   }
}

template<BaseType BaseT>
[[nodiscard]] auto sum(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto s = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      s += A[i];
   return s;
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_sum(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   using value_type = ValueTypeOf<FloatBaseT>;

   real_type s{};
   real_type c{};
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      volatile real_type t = s + A[i];
      if(abss(value_type{s}) >= abss(A[i])) {
         volatile real_type z = s - t;
         c += z + A[i];
      }
      else {
         volatile real_type z = A[i] - t;
         c += z + s;
      }
      s = t;
   }
   return value_type{s + c};
}

template<BaseType BaseT>
[[nodiscard]] auto min(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto min_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      min_elem = mins(A[i], min_elem);
   return min_elem;
}

template<BaseType BaseT>
[[nodiscard]] auto max(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto max_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      max_elem = maxs(A[i], max_elem);
   return max_elem;
}

template<BaseType BaseT>
[[nodiscard]] auto min_index(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using size_type = SizeTypeOf<BaseT>;
   using value_type = ValueTypeOf<BaseT>;

   std::pair<size_type, value_type> min = {0, A[0]};
   for(size_type i = 1; i < A.size(); ++i)
      if(A[i] < min.second)
         min = {i, A[i]};
   return min;
}

template<BaseType BaseT>
[[nodiscard]] auto max_index(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using size_type = SizeTypeOf<BaseT>;
   using value_type = ValueTypeOf<BaseT>;

   std::pair<size_type, value_type> max = {0, A[0]};
   for(size_type i = 1; i < A.size(); ++i)
      if(A[i] > max.second)
         max = {i, A[i]};
   return max;
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool all_finite(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto x : A)
      if(!isfinites(x)) return false;
   return true;
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_inf(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto max_abs = abss(A[0]);
   for(decltype(A.size()) i = 1; i < A.size(); ++i) {
      auto abs_i = abss(A[i]);
      max_abs  = maxs(abs_i, max_abs);
   }
   return max_abs;
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return sqrts(dot_prod(A, A));
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<FloatBaseT>;
   return norm2(A)/sqrts(value_type(A.size()));
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return sqrts(stable_dot_prod(A, A));
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<FloatBaseT>;
   return stable_norm2(A)/sqrts(value_type(A.size()));
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return sum(abs(A));
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<FloatBaseT>;
   return norm1(A)/value_type(A.size());
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return stable_sum(abs(A));
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<FloatBaseT>;
   return stable_norm1/value_type(A.size());
}

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_lp(const FloatBaseT & A, int p)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   auto s = sum(abs(pow_int(A, p)));
   return pows<real_type>(s, real_type(1)/real_type(p));
}

template<BaseType BaseT1, BaseType BaseT2>
[[nodiscard]] auto dot_prod(const BaseT1 & A1, const BaseT2 & A2)
{
   static_assert(SameType<typename BaseT1::base_type, typename BaseT2::base_type>);
   ASSERT_STRICT_DEBUG(A1.size() == A2.size());
   ASSERT_STRICT_DEBUG(!A1.empty());
   return sum(A1 * A2);
}

template<FloatingBaseType FloatBaseT1, FloatingBaseType FloatBaseT2>
[[nodiscard]] auto stable_dot_prod(const FloatBaseT1 & A1, const FloatBaseT2 & A2)
{
   static_assert(SameType<typename FloatBaseT1::base_type, typename FloatBaseT2::base_type>);
   ASSERT_STRICT_DEBUG(A1.size() == A2.size());
   ASSERT_STRICT_DEBUG(!A1.empty());
   return stable_sum(two_prod(A1, A2).first) + stable_sum(two_prod(A1, A2).second);
}

template<BaseType BaseT>
[[nodiscard]] bool does_contain_zero(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<BaseT>;
   for(auto x : A)
      if(x == real_type{0}) return true;
   return false;
}

template<BaseType BaseT>
[[nodiscard]] bool all_positive(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<BaseT>;
   for(auto x : A)
      if(x <= real_type{0}) return false;
   return true;
}

template<BaseType BaseT>
[[nodiscard]] bool all_negative(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<BaseT>;
   for(auto x : A)
      if(x >= real_type{0}) return false;
   return true;
}

template<DirectBaseType T, typename F>
requires (!std::is_const_v<T>)
void apply(T & A, F f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto & x : A)
      f(x);
}

template<DirectBaseType T, typename F, typename Cond>
requires (!std::is_const_v<T>)
void apply_if(T & A, F f, Cond c)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto & x : A)
      if(c(x)) f(x);
}

template<BaseType BaseT, typename F>
[[nodiscard]] bool any_satisfy(const BaseT & A, F f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto x : A)
      if(f(x))
         return true;
   return false;
}

template<BaseType BaseT, typename F>
[[nodiscard]] bool all_satisfy(const BaseT & A, F f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto x : A)
      if(!f(x))
         return false;
   return true;
}

template<DirectBaseType T>
[[nodiscard]] auto within_range(T & A, ValueTypeOf<T> low, ValueTypeOf<T> high)
{
   using size_type = SizeTypeOf<std::decay_t<T>>;
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(high >= low);

   std::vector<size_type> indexes;
   for(size_type i = 0; i < A.size(); ++i)
      if(A[i] >= low && A[i] <= high)
         indexes.push_back(i);

   if(!indexes.empty())
      return std::optional<decltype(A[indexes])>(A[indexes]);
   else
      return std::optional<decltype(A[indexes])>(std::nullopt);
}

template<DirectBaseType T, typename Cond>
[[nodiscard]] auto within_cond(T & A, Cond c)
{
   using size_type = SizeTypeOf<std::decay_t<T>>;
   ASSERT_STRICT_DEBUG(!A.empty());

   std::vector<size_type> indexes;
   for(size_type i = 0; i < A.size(); ++i)
      if(c(A[i])) indexes.push_back(i);

   if(!indexes.empty())
      return std::optional<decltype(A[indexes])>(A[indexes]);
   else
      return std::optional<decltype(A[indexes])>(std::nullopt);
}

template<typename T>
requires (BaseType<std::remove_reference_t<T>> && std::is_lvalue_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>)
[[nodiscard]] auto within_range(T && A, ValueTypeOf<T> low, ValueTypeOf<T> high)
{
   using size_type = SizeTypeOf<std::decay_t<T>>;
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(high >= low);

   std::vector<size_type> indexes;
   for(size_type i = 0; i < A.size(); ++i)
      if(A[i] >= low && A[i] <= high)
         indexes.push_back(i);

   if(!indexes.empty())
      return std::optional<decltype(A[indexes])>(A[indexes]);
   else
      return std::optional<decltype(A[indexes])>(std::nullopt);
}

template<typename T, typename Cond>
requires (BaseType<std::remove_reference_t<T>> && std::is_lvalue_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>)
[[nodiscard]] auto within_cond(T && A, Cond c)
{
   using size_type = SizeTypeOf<std::decay_t<T>>;
   ASSERT_STRICT_DEBUG(!A.empty());

   std::vector<size_type> indexes;
   for(size_type i = 0; i < A.size(); ++i)
      if(c(A[i])) indexes.push_back(i);

   if(!indexes.empty())
      return std::optional<decltype(A[indexes])>(A[indexes]);
   else
      return std::optional<decltype(A[indexes])>(std::nullopt);
}

template<BaseType BaseT>
[[nodiscard]] std::unique_ptr<RealTypeOf<BaseT>[]> unique_blas_array(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<BaseT>;
   auto blas_array = std::make_unique<real_type[]>(static_cast<std::size_t>(A.size()));
   std::copy(A.begin(), A.end(), blas_array.get());
   return blas_array;
}

template<typename T>
requires (DirectBaseType<std::decay_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_increasing(T && A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   std::sort(A.begin(), A.end(), [](auto a, auto b) { return a < b; });
}

template<typename T>
requires (DirectBaseType<std::decay_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_decreasing(T && A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   std::sort(A.begin(), A.end(), [](auto a, auto b) { return a > b; });
}

}

#endif

