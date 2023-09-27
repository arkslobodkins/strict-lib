// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include "strict_concepts.hpp"
#include "strict_error.hpp"

#if defined (__INTEL_COMPILER) || defined (__INTEL_LLVM_COMPILER)
#include <mathimf.h>
#endif

namespace strict {

template<RealType T> struct StrictVal;

using Strict32 = StrictVal<float32>;
using Strict64 = StrictVal<float64>;
#ifdef STRICT_QUADRUPLE_PRECISION
using Strict128 = StrictVal<float128>;
#endif

template<typename T>
concept StrictType =
requires(T a)
{
   typename T::value_type;
   StrictVal<T>{a};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
struct StrictVal
{
public:
   using value_type = T;
   explicit constexpr StrictVal() = default;
   constexpr StrictVal(const StrictVal &) = default;
   StrictVal & operator=(const StrictVal &) & = default;

   template<RealType U> constexpr inline StrictVal(U val);
   template<RealType U> inline StrictVal & operator=(U val) &;
   template<RealType U> [[nodiscard]] constexpr inline operator U () const; // safe conversion

   [[nodiscard]] constexpr StrictVal operator+() const { return *this; }
   [[nodiscard]] constexpr StrictVal operator-() const { return StrictVal{T(-T{*this})}; }

   StrictVal & operator++() & { ++x; return *this; }
   StrictVal & operator--() & { --x; return *this; }
   StrictVal operator++(int) & { StrictVal old{x}; ++x; return old; }
   StrictVal operator--(int) & { StrictVal old{x}; --x; return old; }

   // not constexpr since these operations are only applicable to lvalues
   inline StrictVal & operator+=(StrictVal strict_val) &;
   inline StrictVal & operator-=(StrictVal strict_val) &;
   inline StrictVal & operator*=(StrictVal strict_val) &;
   inline StrictVal & operator/=(StrictVal strict_val) &;
   template<IntegerType U = T> inline StrictVal & operator%=(StrictVal<U> strict_val) &;
   template<IntegerType U = T> inline StrictVal & operator<<=(StrictVal<U> strict_val) &;
   template<IntegerType U = T> inline StrictVal & operator>>=(StrictVal<U> strict_val) &;

   // not constexpr since these operations are only applicable to lvalues
   template<RealType U> inline StrictVal & operator+=(U val) &;
   template<RealType U> inline StrictVal & operator-=(U val) &;
   template<RealType U> inline StrictVal & operator*=(U val) &;
   template<RealType U> inline StrictVal & operator/=(U val) &;
   template<IntegerType U = T> inline StrictVal & operator%=(U val) &;
   template<IntegerType U = T> inline StrictVal & operator<<=(U val) &;
   template<IntegerType U = T> inline StrictVal & operator>>=(U val) &;

private:
   T x{};
};

template<RealType T, RealType U> [[nodiscard]] constexpr inline T real_cast(U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline T real_cast(StrictVal<U> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> strict_cast(U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> strict_cast(StrictVal<U> strict_val);

// not constexpr since these operations are only applicable to lvalues
template<RealType T, RealType U> inline U & operator+=(U & val, StrictVal<T> strict_val);
template<RealType T, RealType U> inline U & operator-=(U & val, StrictVal<T> strict_val);
template<RealType T, RealType U> inline U & operator*=(U & val, StrictVal<T> strict_val);
template<RealType T, RealType U> inline U & operator/=(U & val, StrictVal<T> strict_val);
template<IntegerType T, IntegerType U> inline U & operator%=(U & val, StrictVal<T> strict_val);
template<IntegerType T, IntegerType U> inline U & operator<<=(U & val, StrictVal<T> strict_val);
template<IntegerType T, IntegerType U> inline U & operator>>=(U & val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator+(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator-(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator*(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator/(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator%(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator<<(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator>>(StrictVal<T> v1, StrictVal<T> v2);

template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator+(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator-(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator*(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator/(StrictVal<T> strict_val, U val);
template<IntegerType T, IntegerType U> [[nodiscard]] constexpr inline StrictVal<T> operator%(StrictVal<T> strict_val, U val);
template<IntegerType T, IntegerType U> [[nodiscard]] constexpr inline StrictVal<T> operator<<(StrictVal<T> strict_val, U val);
template<IntegerType T, IntegerType U> [[nodiscard]] constexpr inline StrictVal<T> operator>>(StrictVal<T> strict_val, U val);

template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator+(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator-(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator*(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline StrictVal<T> operator/(U val, StrictVal<T> strict_val);
template<IntegerType T, IntegerType U> [[nodiscard]] constexpr inline StrictVal<T> operator%(U val, StrictVal<T> strict_val);
template<IntegerType T, IntegerType U> [[nodiscard]] constexpr inline StrictVal<T> operator<<(U val, StrictVal<T> strict_val);
template<IntegerType T, IntegerType U> [[nodiscard]] constexpr inline StrictVal<T> operator>>(U val, StrictVal<T> strict_val);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> [[nodiscard]] constexpr inline bool operator==(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator<(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator>(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator<=(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator>=(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator!=(StrictVal<T> v1, StrictVal<T> v2);

template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator==(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator<(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator>(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator<=(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator>=(StrictVal<T> strict_val, U val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator!=(StrictVal<T> strict_val, U val);

template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator==(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator<(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator>(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator<=(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator>=(U val, StrictVal<T> strict_val);
template<RealType T, RealType U> [[nodiscard]] constexpr inline bool operator!=(U val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> abss(StrictVal<T> v);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> mins(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> maxs(StrictVal<T> v1, StrictVal<T> v2);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> logs(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> log2s(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> log10s(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> tans(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline bool isinfs(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline bool isnans(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline bool isfinites(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> pows(StrictVal<T> v, StrictVal<T> p);
template<StandardFloatingType T, IntegerType IntT> [[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, StrictVal<IntT> p);
template<StandardFloatingType T, IntegerType IntT> [[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, IntT p);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> fmas(StrictVal<T> v1, StrictVal<T> v2, StrictVal<T> v3);

template<IntegerType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
template<StandardFloatingType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);

#ifdef STRICT_QUADRUPLE_PRECISION
template<QuadType T> [[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> logs(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> log2s(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> log10s(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> tans(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline bool isinfs(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline bool isnans(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline bool isfinites(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> pows(StrictVal<T> v, StrictVal<T> p);
template<QuadType T, IntegerType IntT> [[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, StrictVal<IntT> p);
template<QuadType T, IntegerType IntT> [[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, IntT p);
template<QuadType T, IntegerType IntT> [[nodiscard]] inline StrictVal<T> fmas(StrictVal<T> v1, StrictVal<T> v2, StrictVal<T> v3);

template<QuadType T> std::ostream & operator<<(std::ostream & os, T val);
template<QuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
#endif

template <RealType T> [[nodiscard]] StrictVal<T> sign(StrictVal<T> v);
template<FloatingType T, IntegerType IntT> [[nodiscard]] inline StrictVal<T> fast_pows_int(StrictVal<T> v, StrictVal<IntT> p);
template<FloatingType T, IntegerType IntT> [[nodiscard]] inline StrictVal<T> fast_pows_int(StrictVal<T> v, IntT p);

template<FloatingType T> [[nodiscard]] inline auto two_prods(StrictVal<T> v1, StrictVal<T> v2);

template<StandardFloatingType T, IntegerType IntT> [[nodiscard]] inline auto pow_prods(StrictVal<T> v, StrictVal<IntT> p);
template<StandardFloatingType T, IntegerType IntT> [[nodiscard]] inline auto pow_prods(StrictVal<T> v, IntT p);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<RealType U>
constexpr inline StrictVal<T>::StrictVal(U val) : x{val}
{
   static_assert(SameType<T, U>);
}

template<RealType T>
template<RealType U>
inline StrictVal<T> & StrictVal<T>::operator=(U val) &
{
   static_assert(SameType<T, U>);
   x = val;
   return *this;
}

template<RealType T>
template<RealType U>
[[nodiscard]] constexpr inline StrictVal<T>::operator U () const
{
   static_assert(SameType<T, U>);
   return x;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline StrictVal<T> & StrictVal<T>::operator+=(StrictVal<T> strict_val) &
{
   x += strict_val.x;
   return *this;
}

template<RealType T>
inline StrictVal<T> & StrictVal<T>::operator-=(StrictVal<T> strict_val) &
{
   x -= strict_val.x;
   return *this;
}

template<RealType T>
inline StrictVal<T> & StrictVal<T>::operator*=(StrictVal<T> strict_val) &
{
   x *= strict_val.x;
   return *this;
}

template<RealType T>
inline StrictVal<T> & StrictVal<T>::operator/=(StrictVal<T> strict_val) &
{
   if constexpr(IntegerType<T>) {
      ASSERT_STRICT_DEBUG(strict_val.x != 0);
   }

   x /= strict_val.x;
   return *this;
}

// static assertions are necessary since additional type U is introduced
template<RealType T>
template<IntegerType U>
inline StrictVal<T> & StrictVal<T>::operator%=(StrictVal<U> strict_val) &
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(strict_val.x != 0);
   x %= strict_val.x;
   return *this;
}

template<RealType T>
template<IntegerType U>
inline StrictVal<T> & StrictVal<T>::operator<<=(StrictVal<U> strict_val) &
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(x > -1);
   ASSERT_STRICT_DEBUG(strict_val.x > -1);
   x <<= strict_val.x;
   return *this;
}

template<RealType T>
template<IntegerType U>
inline StrictVal<T> & StrictVal<T>::operator>>=(StrictVal<U> strict_val) &
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(x > -1);
   ASSERT_STRICT_DEBUG(strict_val.x > -1);
   x >>= strict_val.x;
   return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<RealType U>
inline StrictVal<T> & StrictVal<T>::operator+=(U val) &
{
   static_assert(SameType<T, U>);
   x += val;
   return *this;
}

template<RealType T>
template<RealType U>
inline StrictVal<T> & StrictVal<T>::operator-=(U val) &
{
   static_assert(SameType<T, U>);
   x -= val;
   return *this;
}

template<RealType T>
template<RealType U>
inline StrictVal<T> & StrictVal<T>::operator*=(U val) &
{
   static_assert(SameType<T, U>);
   x *= val;
   return *this;
}

template<RealType T>
template<RealType U>
inline StrictVal<T> & StrictVal<T>::operator/=(U val) &
{
   static_assert(SameType<T, U>);
   if constexpr(IntegerType<T>) {
      ASSERT_STRICT_DEBUG(val != 0);
   }

   x /= val;
   return *this;
}

template<RealType T>
template<IntegerType U>
inline StrictVal<T> & StrictVal<T>::operator%=(U val) &
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(val != 0);
   x %= val;
   return *this;
}

template<RealType T>
template<IntegerType U>
inline StrictVal<T> & StrictVal<T>::operator<<=(U val) &
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(x > -1);
   ASSERT_STRICT_DEBUG(val > -1);
   x <<= val;
   return *this;
}

template<RealType T>
template<IntegerType U>
inline StrictVal<T> & StrictVal<T>::operator>>=(U val) &
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(x > -1);
   ASSERT_STRICT_DEBUG(val > -1);
   x >>= val;
   return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, RealType U>
[[nodiscard]] constexpr inline T real_cast(U val)
{
   return static_cast<T>(val);
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline T real_cast(StrictVal<U> strict_val)
{
   return real_cast<T, U>(U(strict_val));
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> strict_cast(U val)
{
   return real_cast<T, U>(val);
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> strict_cast(StrictVal<U> strict_val)
{
   return real_cast<T, U>(strict_val);
}

template<RealType T, RealType U>
inline U & operator+=(U & val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   val += T{strict_val};
   return val;
}

template<RealType T, RealType U>
inline U & operator-=(U & val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   val -= T{strict_val};
   return val;
}

template<RealType T, RealType U>
inline U & operator*=(U & val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   val *= T{strict_val};
   return val;
}

template<RealType T, RealType U>
inline U & operator/=(U & val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   if constexpr(IntegerType<T>) {
      ASSERT_STRICT_DEBUG(T{strict_val} != 0);
   }

   val /= T{strict_val};
   return val;
}

template<IntegerType T, IntegerType U>
inline U & operator%=(U & val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(T{strict_val} != 0);
   val %= T{strict_val};
   return val;
}

template<IntegerType T, IntegerType U>
inline U & operator<<=(U & val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(val > -1);
   ASSERT_STRICT_DEBUG(T{strict_val} > -1);
   val <<= T{strict_val};
   return val;
}

template<IntegerType T, IntegerType U>
inline U & operator>>=(U & val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(val > -1);
   ASSERT_STRICT_DEBUG(T{strict_val} > -1);
   val >>= T{strict_val};
   return val;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] constexpr inline StrictVal<T> operator+(StrictVal<T> v1, StrictVal<T> v2)
{
   return StrictVal<T>{T(T{v1} + T{v2})};
}

template<RealType T>
[[nodiscard]] constexpr inline StrictVal<T> operator-(StrictVal<T> v1, StrictVal<T> v2)
{
   return StrictVal<T>{T(T{v1} - T{v2})};
}

template<RealType T>
[[nodiscard]] constexpr inline StrictVal<T> operator*(StrictVal<T> v1, StrictVal<T> v2)
{
   return StrictVal<T>{T(T{v1} * T{v2})};
}

template<RealType T>
[[nodiscard]] constexpr inline StrictVal<T> operator/(StrictVal<T> v1, StrictVal<T> v2)
{
   if constexpr(IntegerType<T>) {
      ASSERT_STRICT_DEBUG(T{v2} != 0);
   }

   return StrictVal<T>{T(T{v1} / T{v2})};
}

template<IntegerType T>
[[nodiscard]] constexpr inline StrictVal<T> operator%(StrictVal<T> v1, StrictVal<T> v2)
{
   ASSERT_STRICT_DEBUG(T{v2} != 0);
   return StrictVal<T>{T(T{v1} % T{v2})};
}

template<IntegerType T>
[[nodiscard]] constexpr inline StrictVal<T> operator<<(StrictVal<T> v1, StrictVal<T> v2)
{
   ASSERT_STRICT_DEBUG(T{v1} > -1);
   ASSERT_STRICT_DEBUG(T{v2} > -1);
   return StrictVal<T>{T(T(v1) << T{v2})};
}

template<IntegerType T>
[[nodiscard]] constexpr inline StrictVal<T> operator>>(StrictVal<T> v1, StrictVal<T> v2)
{
   ASSERT_STRICT_DEBUG(T{v1} > -1);
   ASSERT_STRICT_DEBUG(T{v2} > -1);
   return StrictVal<T>{T(T{v1} >> T{v2})};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator+(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return StrictVal<T>{T(T{strict_val} + val)};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator-(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return StrictVal<T>{T(T{strict_val} - val)};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator*(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return StrictVal<T>{T(T{strict_val} * val)};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator/(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   if constexpr(IntegerType<T>) {
      ASSERT_STRICT_DEBUG(val != 0);
   }

   return StrictVal<T>{T(T{strict_val} / val)};
}

template<IntegerType T, IntegerType U>
[[nodiscard]] constexpr inline StrictVal<T> operator%(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(val != 0);
   return StrictVal<T>{T(T{strict_val} % val)};
}

template<IntegerType T, IntegerType U>
[[nodiscard]] constexpr inline StrictVal<T> operator<<(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(T{strict_val} > -1);
   ASSERT_STRICT_DEBUG(val > -1);
   return StrictVal<T>{T(T{strict_val} << val)};
}

template<IntegerType T, IntegerType U>
[[nodiscard]] constexpr inline StrictVal<T> operator>>(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(T{strict_val} > -1);
   ASSERT_STRICT_DEBUG(val > -1);
   return StrictVal<T>{T(T{strict_val} >> val)};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator+(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return StrictVal<T>{T(val + T{strict_val})};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator-(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return StrictVal<T>{T(val - T{strict_val})};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator*(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return StrictVal<T>{T(val * T{strict_val})};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline StrictVal<T> operator/(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   if constexpr(IntegerType<T>) {
      ASSERT_STRICT_DEBUG(T{strict_val} != 0);
   }

   return StrictVal<T>{T(val / T{strict_val})};
}

template<IntegerType T, IntegerType U>
[[nodiscard]] constexpr inline StrictVal<T> operator%(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(T{strict_val} != 0);
   return StrictVal<T>{T(val % T{strict_val})};
}

template<IntegerType T, IntegerType U>
[[nodiscard]] constexpr inline StrictVal<T> operator<<(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(val > -1);
   ASSERT_STRICT_DEBUG(T{strict_val} > -1);
   return StrictVal<T>{T(val << T{strict_val})};
}

template<IntegerType T, IntegerType U>
[[nodiscard]] constexpr inline StrictVal<T> operator>>(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_DEBUG(val > -1);
   ASSERT_STRICT_DEBUG(T{strict_val} > -1);
   return StrictVal<T>{T(val >> T{strict_val})};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] constexpr inline bool operator==(StrictVal<T> v1, StrictVal<T> v2)
{
   return T{v1} == T{v2};
}

template<RealType T>
[[nodiscard]] constexpr inline bool operator<(StrictVal<T> v1, StrictVal<T> v2)
{
   return T{v1} < T{v2};
}

template<RealType T>
[[nodiscard]] constexpr inline bool operator>(StrictVal<T> v1, StrictVal<T> v2)
{
   return T{v1} > T{v2};
}

template<RealType T>
[[nodiscard]] constexpr inline bool operator<=(StrictVal<T> v1, StrictVal<T> v2)
{
   return T{v1} <= T{v2};
}

template<RealType T>
[[nodiscard]] constexpr inline bool operator>=(StrictVal<T> v1, StrictVal<T> v2)
{
   return T{v1} >= T{v2};
}

template<RealType T>
[[nodiscard]] constexpr inline bool operator!=(StrictVal<T> v1, StrictVal<T> v2)
{
   return T{v1} != T{v2};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator==(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return T{strict_val} == val;
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator<(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return T{strict_val} < val;
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator>(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return T{strict_val} > val;
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator<=(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return T{strict_val} <= val;
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator>=(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return T{strict_val} >= val;
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator!=(StrictVal<T> strict_val, U val)
{
   static_assert(SameType<T, U>);
   return T{strict_val} != val;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator==(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return val == T{strict_val};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator<(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return val < T{strict_val};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator>(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return val > T{strict_val};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator<=(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return val <= T{strict_val};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator>=(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return val >= T{strict_val};
}

template<RealType T, RealType U>
[[nodiscard]] constexpr inline bool operator!=(U val, StrictVal<T> strict_val)
{
   static_assert(SameType<T, U>);
   return val != T{strict_val};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] constexpr inline StrictVal<T> abss(StrictVal<T> v)
{
   return T{v} > T(0) ? v : -v;
}

template<RealType T>
[[nodiscard]] constexpr inline StrictVal<T> mins(StrictVal<T> v1, StrictVal<T> v2)
{
   return v1 < v2 ? v1 : v2;
}

template<RealType T>
[[nodiscard]] constexpr inline StrictVal<T> maxs(StrictVal<T> v1, StrictVal<T> v2)
{
   return v1 > v2 ? v1 : v2;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v)
{
   return StrictVal<T>{std::exp(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> logs(StrictVal<T> v)
{
   return StrictVal<T>{std::log(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> log2s(StrictVal<T> v)
{
   return StrictVal<T>{std::log2(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> log10s(StrictVal<T> v)
{
   return StrictVal<T>{std::log10(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v)
{
   return StrictVal<T>{std::sqrt(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v)
{
   return StrictVal<T>{std::sin(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v)
{
   return StrictVal<T>{std::cos(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> tans(StrictVal<T> v)
{
   return StrictVal<T>{std::tan(T{v})};
}

template<StandardFloatingType T>
[[nodiscard]] inline bool isinfs(StrictVal<T> v)
{
   return std::isinf(T{v});
}

template<StandardFloatingType T>
[[nodiscard]] inline bool isnans(StrictVal<T> v)
{
   return std::isnan(T{v});
}

template<StandardFloatingType T>
[[nodiscard]] inline bool isfinites(StrictVal<T> v)
{
   // std::isfinite was not giving the correct result when compiled with one of the
   // oneAPI versions. The problem might be a bug in the standard.
   #if defined (__INTEL_COMPILER) || defined (__INTEL_LLVM_COMPILER)
   return finite(T{v});
   #else
   return std::isfinite(T{v});
   #endif
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> pows(StrictVal<T> v, StrictVal<T> p)
{
   return StrictVal<T>{std::pow(T{v}, T{p})};
}

// strict_cast is applied because pow might return double when T is float
template<StandardFloatingType T, IntegerType IntT>
[[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, StrictVal<IntT> p)
{
   return StrictVal<T>{strict_cast<T>(std::pow(T{v}, IntT{p}))};
}

// strict_cast is applied because pow might return double when T is float
template<StandardFloatingType T, IntegerType IntT>
[[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, IntT p)
{
   return StrictVal<T>{strict_cast<T>(std::pow(T{v}, p))};
}

template<StandardFloatingType T>
[[nodiscard]] inline StrictVal<T> fmas(StrictVal<T> v1, StrictVal<T> v2, StrictVal<T> v3)
{
   return StrictVal<T>{std::fma(T{v1}, T{v2}, T{v3})};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType T>
std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   if(strict_val >= T(0)) {
      os << "+";
   }
   os << T{strict_val};
   return os;
}

template<StandardFloatingType T>
std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   if(strict_val >= T(0)) {
      os << "+";
   }

   int num_digits = std::numeric_limits<T>::digits10;
   os << std::scientific << std::showpoint << std::setprecision(num_digits) << T{strict_val};
   return os;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUADRUPLE_PRECISION
template<QuadType T>
[[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v)
{
   return StrictVal<T>{expq(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> logs(StrictVal<T> v)
{
   return StrictVal<T>{logq(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> log2s(StrictVal<T> v)
{
   return StrictVal<T>{log2q(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> log10s(StrictVal<T> v)
{
   return StrictVal<T>{log10q(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v)
{
   return StrictVal<T>{sqrtq(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v)
{
   return StrictVal<T>{sinq(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v)
{
   return StrictVal<T>{cosq(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> tans(StrictVal<T> v)
{
   return StrictVal<T>{tanq(T{v})};
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> pows(StrictVal<T> v, StrictVal<T> p)
{
   return StrictVal<T>{powq(T{v}, T{p})};
}

template<QuadType T, IntegerType IntT>
[[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, StrictVal<IntT> p)
{
   return StrictVal<T>{powq(T{v}, IntT{p})};
}

template<QuadType T, IntegerType IntT>
[[nodiscard]] inline StrictVal<T> pows_int(StrictVal<T> v, IntT p)
{
   return StrictVal<T>{powq(T{v}, p)};
}

template<QuadType T>
[[nodiscard]] inline bool isinfs(StrictVal<T> v)
{
   return isinfq(T{v});
}

template<QuadType T>
[[nodiscard]] inline bool isnans(StrictVal<T> v)
{
   return isnanq(T{v});
}

template<QuadType T>
[[nodiscard]] inline bool isfinites(StrictVal<T> v)
{
   return finiteq(T{v});
}

template<QuadType T>
[[nodiscard]] inline StrictVal<T> fmas(StrictVal<T> v1, StrictVal<T> v2, StrictVal<T> v3)
{
   return StrictVal<T>{fmaq(T{v1}, T{v2}, T{v3})};
}

template<QuadType T>
std::ostream & operator<<(std::ostream & os, T val)
{
   os << StrictVal<T>{val};
   return os;
}

template<QuadType T>
std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   int width = 41;
   char buf[128];
   quadmath_snprintf(buf, sizeof(buf), "%+-#*.34Qe", width, T{strict_val});
   os << buf;
   return os;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <RealType T>
[[nodiscard]] StrictVal<T> sign(StrictVal<T> v)
{
    return strict_cast<T>((T(0) < v) - (v < T(0)));
}

template<FloatingType T, IntegerType IntT>
[[nodiscard]] inline StrictVal<T> fast_pows_int(StrictVal<T> v, StrictVal<IntT> p)
{
   return fast_pows_int(v, IntT{p});
}

template<FloatingType T, IntegerType IntT>
[[nodiscard]] inline StrictVal<T> fast_pows_int(StrictVal<T> v, IntT p)
{
   T res(1);
   IntT power = abss<IntT>(p);
   for(;;) {
      if(power & 1) {
         res *= v;
      }
      power >>= 1;
      if(!power) {
         break;
      }
      v *= v;
   }
   return p >= 0 ? res : T(1)/res;
}

template<FloatingType T>
[[nodiscard]] inline auto two_prods(StrictVal<T> v1, StrictVal<T> v2)
{
   auto r = v1 * v2;
   auto s = fmas(v1, v2, -r);
   return std::pair<StrictVal<T>, StrictVal<T>>{r, s};
}

template<StandardFloatingType T, IntegerType IntT>
[[nodiscard]] inline auto pow_prods(StrictVal<T> v, StrictVal<IntT> p)
{
   return pow_prods(v, IntT{p});
}

template<StandardFloatingType T, IntegerType IntT>
[[nodiscard]] inline auto pow_prods(StrictVal<T> x, IntT p)
{
   ASSERT_STRICT_DEBUG(p > -1);
   strict_int pw = p;
   using Tuple = std::tuple<StrictVal<T>, StrictVal<T>>;
   using value_type = StrictVal<T>;

   if(pw == 0) {
      return Tuple{T(1), T(0)};
   }
   else if(pw == 1) {
      return Tuple{x, T(0)};
   }
   else if(pw == 2) {
      auto [r, s] = two_prods(x, x);
      return Tuple{r, s};
   }
   else if(pw == 3) {
      auto [r, s] = two_prods(x, x);
      auto [rp1, sp1] = two_prods(r, x);
      return Tuple{rp1, x*s + sp1};
   }
   else if(pw == 4) {
      auto [r, s] = two_prods(x, x);
      auto [r2, s2] = two_prods(r, r);
      return Tuple{r2, T(2)*r*s + s2};
   }
   else if(pw == 5) {
      auto [r, s] = two_prods(x, x);
      auto [r2, s2] = two_prods(r, r);
      auto [rp1, sp1] = two_prods(r2, x);
      return Tuple{rp1, T(2)*x*r*s + x*s2 + sp1};
   }
   else if(pw%2 == 0) {
      auto [r, s] = two_prods(x, x);
      auto n = pw / 2;
      auto sz = n-1;

      value_type* rtemp = new value_type[2*sz];
      value_type* stemp = &rtemp[sz];
      rtemp[0] = T(1);
      rtemp[1] = r;
      for(strict_int i = 2; i < sz; ++i) {
         auto [rcur, scur] = two_prods(rtemp[i-1], r);
         rtemp[i] = rcur;
         stemp[i-2] = scur;
      }
      auto [rn_m1, sn_m1] = two_prods(rtemp[sz-1], r);
      stemp[sz-2] = sn_m1;
      auto [rn, sn] = two_prods(rn_m1, r);
      stemp[sz-1] = sn;

      StrictVal<T> sum_loc{};
      for(strict_int i = 0; i < sz; ++i) {
         sum_loc += rtemp[i] * stemp[sz-1-i];
      }

      delete[] rtemp;
      return Tuple{rn, T(n)*rn_m1*s + sum_loc};
   }
   else {
      auto [r, s] = two_prods(x, x);
      auto n = pw / 2;
      auto sz = n-1;

      value_type* rtemp = new value_type[2*sz];
      value_type* stemp = &rtemp[sz];
      rtemp[0] = T(1);
      rtemp[1] = r;
      for(strict_int i = 2; i < sz; ++i) {
         auto [rcur, scur] = two_prods(rtemp[i-1], r);
         rtemp[i] = rcur;
         stemp[i-2] = scur;
      }
      auto [rn_m1, sn_m1] = two_prods(rtemp[sz-1], r);
      stemp[sz-2] = sn_m1;
      auto [rn, sn] = two_prods(rn_m1, r);
      stemp[sz-1] = sn;

      StrictVal<T> sum_loc{};
      for(strict_int i = 0; i < sz; ++i) {
         sum_loc += rtemp[i] * stemp[sz-1-i];
      }

      auto [rn_p1, sn_p1] = two_prods(rn, x);

      delete[] rtemp;

      return Tuple{rn_p1, T(n)*x*rn_m1*s + x*sum_loc + sn_p1};
   }
}

}

#endif
