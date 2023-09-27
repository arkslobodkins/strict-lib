// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <chrono>
#include <format>
#include <iostream>
#include <limits>
#include <optional>
#include "strict_val.hpp"
#include "strict_array_ops.hpp"

#define STRICT_TIME(a)                                                       \
do {                                                                         \
   strict::util::timer t;                                                    \
   a;                                                                        \
   std::cout << std::format("{} took: {:.4e} seconds\n", #a, t.wall_time()); \
   } while (0)


namespace strict::util {


struct timer
{
   explicit timer() {
      start = std::chrono::high_resolution_clock::now();
   }

   void restart() {
      start = std::chrono::high_resolution_clock::now();
   }

   [[nodiscard]] StrictVal<double> wall_time() {
      return double(std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start).count()) / 1.e9;
   }

private:
   std::chrono::system_clock::time_point start;
};


template<FloatingType T>
struct default_tol
{
   static constexpr StrictVal<T> x{100 * std::numeric_limits<T>::epsilon()};
};


// removed FLT128_EPSILON constant since it requires GCC to be compiled with GCC extensions
// introduced negligible roundoff
#ifdef STRICT_QUADRUPLE_PRECISION
template<>
struct default_tol<float128>
{
   static constexpr StrictVal<float128> x{float128(100 * 1.9259299443872358530559779425849273e-34)};
};
#endif


template<FloatingType T>
[[nodiscard]] bool within_tol_abs(StrictVal<T> val1, StrictVal<T> val2,
                                  StrictVal<T> tol = default_tol<T>::x)
{
   ASSERT_STRICT_DEBUG(tol >= T(0));
   return abss(val1 - val2) <= tol;
}


template<FloatingType T>
[[nodiscard]] bool within_tol_rel(StrictVal<T> val1, StrictVal<T> val2,
                                  StrictVal<T> tol = default_tol<T>::x,
                                  StrictVal<T> near_zero = default_tol<T>::x)
{
   ASSERT_STRICT_DEBUG(tol >= T(0));
   ASSERT_STRICT_DEBUG(near_zero >= T(0));

   auto abs1 = abss(val1);
   auto abs2 = abss(val2);
   if(maxs(abs1, abs2) <= near_zero) {
      return true;
   }
   return abss(val1 - val2) / maxs(abs1, abs2) <= tol;
}


template<OneDimFloatingBaseType T1, OneDimFloatingBaseType T2>
[[nodiscard]] bool within_tol_abs(const T1 & A, const T2 & B,
                                  StrictVal<RealTypeOf<T1>> tol = default_tol<RealTypeOf<T1>>::x)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.size() == B.size());
   return all_satisfy(A, B, [tol](const auto & x, const auto & y) { return within_tol_abs(x, y, tol); });
}


template<TwoDimFloatingBaseType T1, TwoDimFloatingBaseType T2>
[[nodiscard]] bool within_tol_abs(const T1 & A, const T2 & B,
                                  StrictVal<RealTypeOf<T1>> tol = default_tol<RealTypeOf<T1>>::x)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.rows() == B.rows());
   ASSERT_STRICT_DEBUG(A.cols() == B.cols());
   return all_satisfy(A, B, [tol](const auto & x, const auto & y) { return within_tol_abs(x, y, tol); });
}


template<OneDimFloatingBaseType T1, OneDimFloatingBaseType T2>
[[nodiscard]] bool within_tol_rel(const T1 & A, const T2 & B,
                                  StrictVal<RealTypeOf<T1>> tol = default_tol<RealTypeOf<T1>>::x,
                                  StrictVal<RealTypeOf<T1>> near_zero = default_tol<RealTypeOf<T1>>::x)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.size() == B.size());
   return all_satisfy(A, B, [tol, near_zero](const auto & x, const auto & y) { return within_tol_rel(x, y, tol, near_zero); });
}


template<TwoDimFloatingBaseType T1, TwoDimFloatingBaseType T2>
[[nodiscard]] bool within_tol_rel(const T1 & A, const T2 & B,
                                  StrictVal<RealTypeOf<T1>> tol = default_tol<RealTypeOf<T1>>::x,
                                  StrictVal<RealTypeOf<T1>> near_zero = default_tol<RealTypeOf<T1>>::x)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(A.rows() == B.rows());
   ASSERT_STRICT_DEBUG(A.cols() == B.cols());
   return all_satisfy(A, B, [tol, near_zero](const auto & x, const auto & y) { return within_tol_rel(x, y, tol, near_zero); });
}


template<FloatingType T>
[[nodiscard]] StrictVal<T> abs_error(StrictVal<T> approx, StrictVal<T> exact)
{
   return abss(approx - exact);
}


template<FloatingType T>
[[nodiscard]] std::optional<StrictVal<T>> rel_error(StrictVal<T> approx, StrictVal<T> exact)
{
   if(approx == T(0) || exact == T(0)) {
      return {std::nullopt};
   }
   return {abss((approx - exact) / exact)};
}


template<OneDimFloatingBaseType T1, OneDimFloatingBaseType T2>
[[nodiscard]] ValueTypeOf<T1> max_abs_error(const T1 & approx, const T2 & exact)
{
   ASSERT_STRICT_DEBUG(!approx.empty());
   ASSERT_STRICT_DEBUG(approx.size() == exact.size());
   return norm_inf(approx - exact);
}


template<TwoDimFloatingBaseType T1, TwoDimFloatingBaseType T2>
[[nodiscard]] ValueTypeOf<T1> max_abs_error(const T1 & approx, const T2 & exact)
{
   ASSERT_STRICT_DEBUG(!approx.empty());
   ASSERT_STRICT_DEBUG(approx.rows() == exact.rows());
   ASSERT_STRICT_DEBUG(approx.cols() == exact.cols());
   return norm_inf(approx - exact);
}


template<OneDimFloatingBaseType T1, OneDimFloatingBaseType T2>
[[nodiscard]] std::optional<ValueTypeOf<T1>> max_rel_error(const T1 & approx, const T2 & exact)
{
   ASSERT_STRICT_DEBUG(!approx.empty());
   ASSERT_STRICT_DEBUG(approx.size() == exact.size());

   if(has_zero(approx) || has_zero(exact)) {
      return {std::nullopt};
   }
   return {norm_inf((approx - exact) / exact)};
}


template<TwoDimFloatingBaseType T1, TwoDimFloatingBaseType T2>
[[nodiscard]] std::optional<ValueTypeOf<T1>> max_rel_error(const T1 & approx, const T2 & exact)
{
   ASSERT_STRICT_DEBUG(!approx.empty());
   ASSERT_STRICT_DEBUG(approx.rows() == exact.rows());
   ASSERT_STRICT_DEBUG(approx.cols() == exact.cols());
   return max_rel_error(approx.view1D(), exact.view1D());
}


}

#endif
