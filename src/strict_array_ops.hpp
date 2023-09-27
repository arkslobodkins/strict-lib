// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <algorithm>   // sort, copy, shuffle
#include <cmath>       // log10
#include <cstdlib>     // size_t
#include <iostream>    // cout, endl, ostream
#include <fstream>     // ofstream
#include <functional>  // function
#include <memory>      // unique_ptr, make_unique
#include <random>      // random_device, mt19937
#include <string>      // string
#include <type_traits> // remove_reference_t, is_const_v
#include <utility>     // pair
#include <vector>      // vector

#include "strict_auxiliary.hpp"
#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_val.hpp"

// generic functions that work on Arrays, SliceArrays, and their expression templates.

namespace strict {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
requires (BaseType<std::remove_reference_t<T>>)
using OneVarFptr = std::function<bool(const ValueTypeOf<T> &)>;

template<typename T>
requires (OneDimBaseType<std::remove_reference_t<T>>)
using TwoVarFptr1 = std::function<bool(const ValueTypeOf<T> &, const ValueTypeOf<T> &)>;

template<typename T>
requires (TwoDimBaseType<std::remove_reference_t<T>>)
using TwoVarFptr2 = std::function<bool(const ValueTypeOf<T> &, const ValueTypeOf<T> &)>;

enum class skip_line : char { yes, no };


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//1D output
template<OneDimBaseType OneDimBaseT>
std::ostream & operator<<(std::ostream & os, const OneDimBaseT & A);


template<OneDimBaseType OneDimBaseT>
void print(const OneDimBaseT & A, const std::string & name, skip_line skip = skip_line::no);


template<OneDimBaseType OneDimBaseT>
void print_to_file(const std::string & file_path, const OneDimBaseT & A, const std::string & name = "");


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2D output
template<TwoDimBaseType TwoDimBaseT>
std::ostream & operator<<(std::ostream & os, const TwoDimBaseT & A);


template<TwoDimBaseType TwoDimBaseT>
void print(const TwoDimBaseT & A, const std::string & name, skip_line skip = skip_line::no);


template<TwoDimBaseType TwoDimBaseT>
void print_to_file(const std::string & file_path, const TwoDimBaseT & A, const std::string & name = "");


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
[[nodiscard]] auto prod(const BaseT & A);


template<FloatingBaseType BaseT>
[[nodiscard]] auto stable_prod(const BaseT & A);


template<BaseType BaseT>
[[nodiscard]] auto sum(const BaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_sum(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto mean(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_mean(const FloatBaseT & A);


template<BaseType BaseT>
[[nodiscard]] auto min(const BaseT & A);


template<BaseType BaseT>
[[nodiscard]] auto max(const BaseT & A);


template<BaseType BaseT>
[[nodiscard]] auto min_index(const BaseT & A); // returns std::pair<strict_int, StrictVal<real_type>>


template<BaseType BaseT>
[[nodiscard]] auto max_index(const BaseT & A); // returns std::pair<strict_int, StrictVal<real_type>>


template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool all_finite(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool has_inf(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool has_nan(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_inf(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2_scaled(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2_scaled(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1_scaled(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1_scaled(const FloatBaseT & A);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_lp(const FloatBaseT & A, ImplicitInt p);


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm_lp(const FloatBaseT & A, ImplicitInt p);


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
[[nodiscard]] auto dot_prod(const OneDimBaseT1 & A1, const OneDimBaseT2 & A2);


template<TwoDimBaseType TwoDimBaseT1, TwoDimBaseType TwoDimBaseT2>
[[nodiscard]] auto dot_prod(const TwoDimBaseT1 & A1, const TwoDimBaseT2 & A2);


template<OneDimFloatingBaseType OneDimFloatBaseT1, OneDimFloatingBaseType OneDimFloatBaseT2>
[[nodiscard]] auto stable_dot_prod(const OneDimFloatBaseT1 & A1, const OneDimFloatBaseT2 & A2);


template<TwoDimFloatingBaseType TwoDimFloatBaseT1, TwoDimFloatingBaseType TwoDimFloatBaseT2>
[[nodiscard]] auto stable_dot_prod(const TwoDimFloatBaseT1 & A1, const TwoDimFloatBaseT2 & A2);


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, OneDimIntegerBaseType OneDimBaseT3>
[[nodiscard]] auto polynomial(const OneDimBaseT1 & coeffs, const OneDimBaseT2 & x, const OneDimBaseT3 & powers);


template<OneDimFloatingBaseType OneDimBaseT1, OneDimFloatingBaseType OneDimBaseT2, OneDimIntegerBaseType OneDimBaseT3>
[[nodiscard]] auto stable_polynomial(const OneDimBaseT1 & coeffs, const OneDimBaseT2 & x, const OneDimBaseT3 & powers);


template<BaseType BaseT>
[[nodiscard]] bool has_zero(const BaseT & A);


template<BaseType BaseT>
[[nodiscard]] bool all_positive(const BaseT & A);


template<BaseType BaseT>
[[nodiscard]] bool all_non_negative(const BaseT & A);


template<BaseType BaseT>
[[nodiscard]] bool all_negative(const BaseT & A);


template<BaseType BaseT>
[[nodiscard]] bool any_satisfy(const BaseT & A, OneVarFptr<BaseT> f);


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
[[nodiscard]] bool any_satisfy(const OneDimBaseT1 & A, const OneDimBaseT2 & B, TwoVarFptr1<OneDimBaseT1> f);


template<TwoDimBaseType TwoDimBaseT1, TwoDimBaseType TwoDimBaseT2>
[[nodiscard]] bool any_satisfy(const TwoDimBaseT1 & A, const TwoDimBaseT2 & B, TwoVarFptr2<TwoDimBaseT1> f);


template<BaseType BaseT>
[[nodiscard]] bool all_satisfy(const BaseT & A, OneVarFptr<BaseT> f);


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
[[nodiscard]] bool all_satisfy(const OneDimBaseT1 & A, const OneDimBaseT2 & B, TwoVarFptr1<OneDimBaseT1> f);


template<TwoDimBaseType TwoDimBaseT1, TwoDimBaseType TwoDimBaseT2>
[[nodiscard]] bool all_satisfy(const TwoDimBaseT1 & A, const TwoDimBaseT2 & B, TwoVarFptr2<TwoDimBaseT1> f);


template<BaseType BaseT>
[[nodiscard]] std::unique_ptr<RealTypeOf<BaseT>[]> unique_blas_array(const BaseT & A);


template<typename T>
requires (BaseType<std::remove_reference_t<T>>)
[[nodiscard]] auto within_range(T && A, Low<RealTypeOf<T>> low, High<RealTypeOf<T>> high);


template<typename T>
requires (BaseType<std::remove_reference_t<T>>)
[[nodiscard]] auto within_cond(T && A, OneVarFptr<T> f);


template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void apply(T && A, std::function<void(ValueTypeOf<T> &)> f);


template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_increasing(T && A);


template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_decreasing(T && A);


template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void shuffle(T && A);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {
   std::string smart_spaces(strict_int max_ind, strict_int ind)
   {
      auto count_digit = [](strict_int number) -> strict_int {
         if(!number) {
            return 1;
         }
         return static_cast<strict_int>(std::log10(number)) + 1;
      };

      strict_int max_digits = count_digit(max_ind);
      strict_int ind_digits = count_digit(ind);
      return std::string(static_cast<std::string::size_type>(1 + max_digits-ind_digits), 32);
   }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT>
std::ostream & operator<<(std::ostream & os, const OneDimBaseT & A)
{
   if(A.empty()) {
      os << __func__ << " : empty Array1D based object" << std::endl;
   }
   for(strict_int i = 0; i < A.size(); ++i) {
      os << "[" << i << "] ="
         << internal::smart_spaces(A.size(), i) << A[i] << std::endl;
   }
   return os;
}


template<OneDimBaseType OneDimBaseT>
void print(const OneDimBaseT & A, const std::string & name, skip_line skip)
{
   if(A.empty()) {
      std::cout << __func__ << " : " << name << " -> empty Array1D based object" << std::endl;
   }
   for(strict_int i = 0; i < A.size(); ++i) {
      std::cout << name << "[" << i << "] ="
         << internal::smart_spaces(A.size(), i) << A[i] << std::endl;
   }
   if(skip == skip_line::yes) {
      std::cout << std::endl;
   }
}


template<OneDimBaseType OneDimBaseT>
void print_to_file(const std::string & file_path, const OneDimBaseT & A, const std::string & name)
{
   std::ofstream ofs{file_path};
   ASSERT_STRICT_DEBUG(ofs);

   if(A.empty()) {
      ofs << __func__ << " : " << name << " -> empty Array1D based object" << std::endl;
   }
   for(strict_int i = 0; i < A.size(); ++i) {
      ofs << name << "[" << i << "] ="
         << internal::smart_spaces(A.size(), i) << A[i] << std::endl;
   }
}


template<TwoDimBaseType TwoDimBaseT>
std::ostream & operator<<(std::ostream & os, const TwoDimBaseT & A)
{
   if(A.empty()) {
      os << __func__ << " : empty Array2D based object" << std::endl;
   }

   for(strict_int i = 0; i < A.rows(); ++i) {
      for(strict_int j = 0; j < A.cols(); ++j) {
         os << "[" << i << "][" << j << "] ="
            << internal::smart_spaces(A.rows(), i) << A(i, j) << "   ";
      }
      os << std::endl;
   }
   return os;
}


template<TwoDimBaseType TwoDimBaseT>
void print(const TwoDimBaseT & A, const std::string & name, skip_line skip)
{
   if(A.empty()) {
      std::cout << __func__ << " : " << name << " -> empty Array2D based object" << std::endl;
   }

   for(strict_int i = 0; i < A.rows(); ++i) {
      for(strict_int j = 0; j < A.cols(); ++j) {
         std::cout  << name << "[" << i << "][" << j << "] ="
            << internal::smart_spaces(A.rows(), i) << A(i, j) << "   ";
      }
      std::cout << std::endl;
   }

   if(skip == skip_line::yes) {
      std::cout << std::endl;
   }
}


template<TwoDimBaseType TwoDimBaseT>
void print_to_file(const std::string & file_path, const TwoDimBaseT & A, const std::string & name)
{
   std::ofstream ofs{file_path};
   ASSERT_STRICT_DEBUG(ofs);

   if(A.empty()) {
      ofs << __func__ << " : empty Array2D based object" << std::endl;
   }

   for(strict_int i = 0; i < A.rows(); ++i) {
      for(strict_int j = 0; j < A.cols(); ++j) {
         ofs << "[" << i << "][" << j << "] ="
            << internal::smart_spaces(A.rows(), i) << A(i, j) << "   ";
      }
      ofs << std::endl;
   }
}


template<BaseType BaseT>
[[nodiscard]] auto prod(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto p = A[0];
   for(strict_int i = 1; i < A.size(); ++i) {
      p *= A[i];
   }
   return p;
}


template<FloatingBaseType BaseT>
[[nodiscard]] auto stable_prod(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<BaseT>;

   Array1D<real_type> res(A.size()-1);
   Array1D<real_type> offset(A.size()-1);
   auto p = A[0];
   for(strict_int i = 1; i < A.size(); ++i) {
      res[i-1] = two_prods(p, A[i]).second;
      p *= A[i];
   }

   StrictVal<real_type> prod_temp = real_type(1);
   for(strict_int i = 0; i < offset.size(); ++i) {
      offset[i] = prod_temp * res[last-i];
      prod_temp *= A[last-i];
   }

   return p + stable_sum(offset);
}


template<BaseType BaseT>
[[nodiscard]] auto sum(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto s = A[0];
   for(strict_int i = 1; i < A.size(); ++i) {
      s += A[i];
   }
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
   for(strict_int i = 0; i < A.size(); ++i) {
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


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto mean(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   return sum(A)/real_type(A.size());
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_mean(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   return stable_sum(A)/real_type(A.size());
}


template<BaseType BaseT>
[[nodiscard]] auto min(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto min_elem = A[0];
   for(strict_int i = 1; i < A.size(); ++i) {
      min_elem = mins(A[i], min_elem);
   }
   return min_elem;
}


template<BaseType BaseT>
[[nodiscard]] auto max(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto max_elem = A[0];
   for(strict_int i = 1; i < A.size(); ++i) {
      max_elem = maxs(A[i], max_elem);
   }
   return max_elem;
}


template<BaseType BaseT>
[[nodiscard]] auto min_index(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<BaseT>;

   std::pair<strict_int, value_type> min = {0, A[0]};
   for(strict_int i = 1; i < A.size(); ++i)
      if(A[i] < min.second)
         min = {i, A[i]};
   return min;
}


template<BaseType BaseT>
[[nodiscard]] auto max_index(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using value_type = ValueTypeOf<BaseT>;

   std::pair<strict_int, value_type> max = {0, A[0]};
   for(strict_int i = 1; i < A.size(); ++i)
      if(A[i] > max.second)
         max = {i, A[i]};
   return max;
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool all_finite(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return all_satisfy(A, [](auto x) { return isfinites(x); });
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool has_inf(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return any_satisfy(A, [](auto x) { return isinfs(x); });
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool has_nan(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return any_satisfy(A, [](auto x) { return isnans(x); });
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_inf(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return max(abs(A));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return sqrts(dot_prod(A, A));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return sqrts(stable_dot_prod(A, A));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   return norm2(A)/sqrts(strict_cast<real_type>(A.size()));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm2_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   return stable_norm2(A)/sqrts(strict_cast<real_type>(A.size()));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return sum(abs(A));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return stable_sum(abs(A));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm1_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   return norm1(A)/real_type(A.size());
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm1_scaled(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<FloatBaseT>;
   return stable_norm1(A)/real_type(A.size());
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_lp(const FloatBaseT & A, ImplicitInt p)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(p.get() > 0);
   using real_type = RealTypeOf<FloatBaseT>;
   auto s = sum(abs(pow_int(A, p.get())));
   return pows<real_type>(s, real_type(1)/real_type(p.get()));
}


template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto stable_norm_lp(const FloatBaseT & A, ImplicitInt p)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(p.get() > 0);

   using real_type = RealTypeOf<FloatBaseT>;
   auto pw = p.get();
   auto sz = A.size();

   auto coeff = const1D(Size{sz}, CVal<real_type>{real_type(1)});
   auto P = const1D(Size{sz}, CVal<real_type>{pw});
   if constexpr(OneDimBaseType<FloatBaseT>) {
      auto s = stable_polynomial(coeff, abs(A), P);
      return pows<real_type>(s, real_type(1)/real_type(pw));
   }
   else {
      auto s = stable_polynomial(coeff, abs(A.view1D()), P);
      return pows<real_type>(s, real_type(1)/real_type(pw));
   }
}


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
[[nodiscard]] auto dot_prod(const OneDimBaseT1 & A1, const OneDimBaseT2 & A2)
{
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(A1.size() == A2.size());
   return sum(A1 * A2);
}


template<TwoDimBaseType TwoDimBaseT1, TwoDimBaseType TwoDimBaseT2>
[[nodiscard]] auto dot_prod(const TwoDimBaseT1 & A1, const TwoDimBaseT2 & A2)
{
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(A1.rows() == A2.rows());
   ASSERT_STRICT_DEBUG(A1.cols() == A2.cols());
   return sum(A1 * A2);
}


template<OneDimFloatingBaseType OneDimFloatBaseT1, OneDimFloatingBaseType OneDimFloatBaseT2>
[[nodiscard]] auto stable_dot_prod(const OneDimFloatBaseT1 & A1, const OneDimFloatBaseT2 & A2)
{
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(A1.size() == A2.size());
   return stable_sum(two_prod(A1, A2).first) + stable_sum(two_prod(A1, A2).second);
}


template<TwoDimFloatingBaseType TwoDimFloatBaseT1, TwoDimFloatingBaseType TwoDimFloatBaseT2>
[[nodiscard]] auto stable_dot_prod(const TwoDimFloatBaseT1 & A1, const TwoDimFloatBaseT2 & A2)
{
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(A1.rows() == A2.rows());
   ASSERT_STRICT_DEBUG(A1.cols() == A2.cols());
   auto A1V = A1.view1D(), A2V = A2.view1D();
   return stable_sum(two_prod(A1V, A2V).first) + stable_sum(two_prod(A1V, A2V).second);
}


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, OneDimIntegerBaseType OneDimBaseT3>
[[nodiscard]] auto polynomial(const OneDimBaseT1 & coeffs, const OneDimBaseT2 & x, const OneDimBaseT3 & powers)
{
   ASSERT_STRICT_DEBUG(!x.empty());
   ASSERT_STRICT_DEBUG(coeffs.size() == x.size());
   ASSERT_STRICT_DEBUG(x.size() == powers.size());
   ASSERT_STRICT_DEBUG(all_non_negative(powers));

   ValueTypeOf<OneDimBaseT1> z{};
   for(strict_int i{}; i < x.size(); ++i)
      z += coeffs[i] * pows_int(x[i], powers[i]);

   return z;
}


template<OneDimFloatingBaseType OneDimBaseT1, OneDimFloatingBaseType OneDimBaseT2, OneDimIntegerBaseType OneDimBaseT3>
[[nodiscard]] auto stable_polynomial(const OneDimBaseT1 & coeffs, const OneDimBaseT2 & x, const OneDimBaseT3 & powers)
{
   ASSERT_STRICT_DEBUG(!x.empty());
   ASSERT_STRICT_DEBUG(coeffs.size() == x.size());
   ASSERT_STRICT_DEBUG(x.size() == powers.size());
   ASSERT_STRICT_DEBUG(all_non_negative(powers));

   auto S = pow_prod(x, powers).first;
   auto R = pow_prod(x, powers).second;
   auto coeff_pair = merge(coeffs, coeffs);
   auto SR = merge(S, R);
   return stable_dot_prod(coeff_pair, SR);
}


template<BaseType BaseT>
[[nodiscard]] bool has_zero(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_zero = []<RealType T> (const StrictVal<T> & x) { return x == T(0); };
   return any_satisfy(A, is_zero);
}


template<BaseType BaseT>
[[nodiscard]] bool all_positive(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_positive = []<RealType T> (const StrictVal<T> & x) { return x > T(0); };
   return all_satisfy(A, is_positive);
}


template<BaseType BaseT>
[[nodiscard]] bool all_non_negative(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_non_negative = []<RealType T> (const StrictVal<T> & x) { return x >= T(0); };
   return all_satisfy(A, is_non_negative);
}


template<BaseType BaseT>
[[nodiscard]] bool all_negative(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto is_negative = []<RealType T> (const StrictVal<T> & x) { return x < T(0); };
   return all_satisfy(A, is_negative);
}


template<BaseType BaseT>
[[nodiscard]] bool any_satisfy(const BaseT & A, OneVarFptr<BaseT> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto x : A)
      if(f(x))
         return true;
   return false;
}


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
[[nodiscard]] bool any_satisfy(const OneDimBaseT1 & A, const OneDimBaseT2 & B, TwoVarFptr1<OneDimBaseT1> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.size() == B.size());
   for(strict_int i = 0; i < A.size(); ++i)
      if(f(A[i], B[i]))
         return true;
   return false;
}


template<TwoDimBaseType TwoDimBaseT1, TwoDimBaseType TwoDimBaseT2>
[[nodiscard]] bool any_satisfy(const TwoDimBaseT1 & A, const TwoDimBaseT2 & B, TwoVarFptr2<TwoDimBaseT1> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.rows() == B.rows());
   ASSERT_STRICT_DEBUG(A.cols() == B.cols());
   return any_satisfy(A.view1D(), B.view1D(), f);
}


template<BaseType BaseT>
[[nodiscard]] bool all_satisfy(const BaseT & A, OneVarFptr<BaseT> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto x : A)
      if(!f(x))
         return false;
   return true;
}


template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
[[nodiscard]] bool all_satisfy(const OneDimBaseT1 & A, const OneDimBaseT2 & B, TwoVarFptr1<OneDimBaseT1> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.size() == B.size());
   for(strict_int i = 0; i < A.size(); ++i)
      if(!f(A[i], B[i]))
         return false;
   return true;
}


template<TwoDimBaseType TwoDimBaseT1, TwoDimBaseType TwoDimBaseT2>
[[nodiscard]] bool all_satisfy(const TwoDimBaseT1 & A, const TwoDimBaseT2 & B, TwoVarFptr2<TwoDimBaseT1> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.rows() == B.rows());
   ASSERT_STRICT_DEBUG(A.cols() == B.cols());
   return all_satisfy(A.view1D(), B.view1D(), f);
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
requires (BaseType<std::remove_reference_t<T>>)
[[nodiscard]] auto within_range(T && A, Low<RealTypeOf<T>> low, High<RealTypeOf<T>> high)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(high.get() >= low.get());

   auto l = low.get();
   auto h = high.get();

   std::vector<ImplicitInt> indexes;
   for(strict_int i = 0; i < A.size(); ++i) {
      if(A[i] >= l && A[i] <= h) {
         indexes.push_back(i);
      }
   }
   return A[indexes];
}


template<typename T>
requires (BaseType<std::remove_reference_t<T>>)
[[nodiscard]] auto within_cond(T && A, OneVarFptr<T> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());

   std::vector<ImplicitInt> indexes;
   for(strict_int i = 0; i < A.size(); ++i) {
      if(f(A[i])) {
         indexes.push_back(i);
      }
   }
   return A[indexes];
}


// !std::is_const for better error messages
// remove_reference_t is used since is_const_v is false for const &
template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void apply(T && A, std::function<void(ValueTypeOf<T> &)> f)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto & x : A) {
      f(x);
   }
}


// !std::is_const for better error messages
// remove_reference_t is used since is_const_v is false for const &
template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_increasing(T && A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   std::sort(A.begin(), A.end(), [](auto a, auto b) { return a < b; });
}


// !std::is_const for better error messages
// remove_reference_t is used since is_const_v is false for const &
template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void sort_decreasing(T && A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   std::sort(A.begin(), A.end(), [](auto a, auto b) { return a > b; });
}


// !std::is_const for better error messages
// remove_reference_t is used since is_const_v is false for const &
template<typename T>
requires (DirectBaseType<std::remove_reference_t<T>> && !std::is_const_v<std::remove_reference_t<T>>)
void shuffle(T && A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   std::random_device rd;
   std::mt19937 g(rd());
   std::shuffle(A.begin(), A.end(), g);
}


}


#endif

