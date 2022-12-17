#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <cmath>
#include <iomanip>
#include <limits>

#include "error_handling.hpp"
#include "value_traits.hpp"

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
struct StrictVal
{
public:
   using value_type = T;
   explicit constexpr StrictVal() = default;
   constexpr StrictVal(const StrictVal &) = default;
   constexpr StrictVal & operator=(const StrictVal &) = default;

   template<RealType U> constexpr inline StrictVal(U val);
   template<RealType U> constexpr inline StrictVal & operator=(U val);
   template<RealType U> constexpr inline operator U () const; // safe conversion
   template<RealType U> constexpr inline U convert() const;   // conversion chosen by the user;

   [[nodiscard]] constexpr StrictVal operator+() const { return *this; }
   [[nodiscard]] constexpr StrictVal operator-() const { return StrictVal{-T(*this)}; }
   constexpr StrictVal operator++() { ++x; return *this; }
   constexpr StrictVal operator--() { --x; return *this; }
   constexpr StrictVal operator++(int) { StrictVal old{x}; ++x; return old; }
   constexpr StrictVal operator--(int) { StrictVal old{x}; --x; return old; }

   constexpr inline StrictVal operator+=(StrictVal strict_val);
   constexpr inline StrictVal operator-=(StrictVal strict_val);
   constexpr inline StrictVal operator*=(StrictVal strict_val);
   constexpr inline StrictVal operator/=(StrictVal strict_val);

   template<IntegerType U = T> constexpr inline StrictVal operator%=(StrictVal strict_val);
   template<IntegerType U = T> constexpr inline StrictVal operator<<=(StrictVal strict_val);
   template<IntegerType U = T> constexpr inline StrictVal operator>>=(StrictVal strict_val);

private:
   T x{};
};

template<RealType T> constexpr inline auto operator+=(T & val, StrictVal<T> strict_val);
template<RealType T> constexpr inline auto operator-=(T & val, StrictVal<T> strict_val);
template<RealType T> constexpr inline auto operator*=(T & val, StrictVal<T> strict_val);
template<RealType T> constexpr inline auto operator/=(T & val, StrictVal<T> strict_val);
template<IntegerType T> constexpr inline auto operator%=(T & val, StrictVal<T> strict_val);
template<IntegerType T> constexpr inline auto operator<<=(T & val, StrictVal<T> strict_val);
template<IntegerType T> constexpr inline auto operator>>=(T & val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline auto operator+(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline auto operator-(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline auto operator*(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline auto operator/(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator%(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator<<(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator>>(StrictVal<T> v1, StrictVal<T> v2);

template<RealType T> [[nodiscard]] constexpr inline auto operator+(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline auto operator-(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline auto operator*(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline auto operator/(StrictVal<T> strict_val, T val);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator%(StrictVal<T> strict_val, T val);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator<<(StrictVal<T> strict_val, T val);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator>>(StrictVal<T> strict_val, T val);

template<RealType T> [[nodiscard]] constexpr inline auto operator+(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline auto operator-(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline auto operator*(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline auto operator/(T val, StrictVal<T> strict_val);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator%(T val, StrictVal<T> strict_val);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator<<(T val, StrictVal<T> strict_val);
template<IntegerType T> [[nodiscard]] constexpr inline auto operator>>(T val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline bool operator==(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator<(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator>(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator<=(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator>=(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator!=(StrictVal<T> v1, StrictVal<T> v2);

template<RealType T> [[nodiscard]] constexpr inline bool operator==(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<=(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>=(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator!=(StrictVal<T> strict_val, T val);

template<RealType T> [[nodiscard]] constexpr inline bool operator==(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<=(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>=(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator!=(T val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline auto abs(StrictVal<T> v);
template<RealType T> [[nodiscard]] constexpr inline auto min(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline auto max(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline auto min(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline auto max(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline auto min(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline auto max(StrictVal<T> strict_val, T val);

template<StandardFloatingType T> [[nodiscard]] constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2);
template<StandardFloatingType T> [[nodiscard]] inline auto exp(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline auto sin(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline auto cos(StrictVal<T> v);

template<NotQuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
#ifdef STRICT_QUADRUPLE_PRECISION
   template<QuadType T> [[nodiscard]] constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2);
   template<QuadType T> [[nodiscard]] inline auto exp(StrictVal<T> v);
   template<QuadType T> [[nodiscard]] inline auto sin(StrictVal<T> v);
   template<QuadType T> [[nodiscard]] inline auto cos(StrictVal<T> v);

   template<QuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
constexpr inline StrictVal<T>::StrictVal(U val) : x(val)
{
   static_assert(SameType<T, U>);
}

template<RealType T> template<RealType U>
constexpr inline StrictVal<T> & StrictVal<T>::operator=(U val)
{
   static_assert(SameType<T, U>);
   x = val;
   return *this;
}

template<RealType T> template<RealType U>
constexpr inline StrictVal<T>::operator U () const
{
   static_assert(SameType<T, U>);
   return x;
}

template<RealType T> template<RealType U>
constexpr inline U StrictVal<T>::convert() const
{
   return U(x);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator+=(StrictVal<T> strict_val)
{ x += T(strict_val); return *this; }

template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator-=(StrictVal<T> strict_val)
{ x -= T(strict_val); return *this; }

template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator*=(StrictVal<T> strict_val)
{ x *= T(strict_val); return *this; }

template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator/=(StrictVal<T> strict_val)
{
   #ifdef STRICT_DIVISION_ON
   if(T(strict_val) == T(0)) STRICT_THROW_ZERO_DIVISION();
   #endif
   x /= T(strict_val);
   return *this;
}

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator%=(StrictVal<T> strict_val)
{ x %= T(strict_val); return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator<<=(StrictVal<T> strict_val)
{ x <<= T(strict_val); return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator>>=(StrictVal<T> strict_val)
{ x >>= T(strict_val); return *this; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline auto operator+=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val += T(strict_val)}; }

template<RealType T> constexpr inline auto operator-=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val -= T(strict_val)}; }

template<RealType T> constexpr inline auto operator*=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val *= T(strict_val)}; }

template<RealType T> constexpr inline auto operator/=(T & val, StrictVal<T> strict_val)
{
   #ifdef STRICT_DIVISION_ON
   if(T(strict_val) == T(0)) STRICT_THROW_ZERO_DIVISION();
   #endif
   return StrictVal<T>{val /= T(strict_val)};
}

template<IntegerType T> constexpr inline auto operator%=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val %= T(strict_val)}; }

template<IntegerType T> constexpr inline auto operator<<=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val <<= T(strict_val)}; }

template<IntegerType T> constexpr inline auto operator>>=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val >>= T(strict_val)}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline auto operator+(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T(T(v1) + T(v2))}; }

template<RealType T> constexpr inline auto operator-(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T(T(v1) - T(v2))}; }

template<RealType T> constexpr inline auto operator*(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T(T(v1) * T(v2))}; }

template<RealType T> constexpr inline auto operator/(StrictVal<T> v1, StrictVal<T> v2)
{
   #ifdef STRICT_DIVISION_ON
   if(T(v2) == T(0)) STRICT_THROW_ZERO_DIVISION();
   #endif
   return StrictVal<T>{T(T(v1) / T(v2))};
}

template<IntegerType T> constexpr inline auto operator%(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T(T(v1) % T(v2))}; }

template<IntegerType T> constexpr inline auto operator<<(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T(T(v1) << T(v2))}; }

template<IntegerType T> constexpr inline auto operator>>(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T(T(v1) >> T(v2))}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline auto operator+(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T(T(strict_val) + val)}; }

template<RealType T> constexpr inline auto operator-(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T(T(strict_val) - val)}; }

template<RealType T> constexpr inline auto operator*(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T(T(strict_val) * val)}; }

template<RealType T> constexpr inline auto operator/(StrictVal<T> strict_val, T val)
{
   #ifdef STRICT_DIVISION_ON
   if(val == T(0)) STRICT_THROW_ZERO_DIVISION();
   #endif
   return StrictVal<T>{T(T(strict_val) / val)};
}

template<IntegerType T> constexpr inline auto operator%(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T(T(strict_val) % val)}; }

template<IntegerType T> constexpr inline auto operator<<(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T(T(strict_val) << val)}; }

template<IntegerType T> constexpr inline auto operator>>(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T(T(strict_val) >> val)}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline auto operator+(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T(val + T(strict_val))}; }

template<RealType T> constexpr inline auto operator-(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T(val - T(strict_val))}; }

template<RealType T> constexpr inline auto operator*(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T(val * T(strict_val))}; }

template<RealType T> constexpr inline auto operator/(T val, StrictVal<T> strict_val)
{
   #ifdef STRICT_DIVISION_ON
   if(T(strict_val) == T(0)) STRICT_THROW_ZERO_DIVISION();
   #endif
   return StrictVal<T>{T(val / T(strict_val))};
}

template<IntegerType T> constexpr inline auto operator%(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T(val % T(strict_val))}; }

template<IntegerType T> constexpr inline auto operator<<(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T(val << T(strict_val))}; }

template<IntegerType T> constexpr inline auto operator>>(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T(val >> T(strict_val))}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline bool operator==(StrictVal<T> v1, StrictVal<T> v2)
{ return T(v1) == T(v2); }

template<RealType T> constexpr inline bool operator<(StrictVal<T> v1, StrictVal<T> v2)
{ return T(v1) < T(v2); }

template<RealType T> constexpr inline bool operator>(StrictVal<T> v1, StrictVal<T> v2)
{ return T(v1) > T(v2); }

template<RealType T> constexpr inline bool operator<=(StrictVal<T> v1, StrictVal<T> v2)
{ return T(v1) <= T(v2); }

template<RealType T> constexpr inline bool operator>=(StrictVal<T> v1, StrictVal<T> v2)
{ return T(v1) >= T(v2); }

template<RealType T> constexpr inline bool operator!=(StrictVal<T> v1, StrictVal<T> v2)
{ return T(v1) != T(v2); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline bool operator==(StrictVal<T> strict_val, T val)
{ return T(strict_val) == val; }

template<RealType T> constexpr inline bool operator<(StrictVal<T> strict_val, T val)
{ return T(strict_val) < val; }

template<RealType T> constexpr inline bool operator>(StrictVal<T> strict_val, T val)
{ return T(strict_val) > val; }

template<RealType T> constexpr inline bool operator<=(StrictVal<T> strict_val, T val)
{ return T(strict_val) <= val; }

template<RealType T> constexpr inline bool operator>=(StrictVal<T> strict_val, T val)
{ return T(strict_val) >= val; }

template<RealType T> constexpr inline bool operator!=(StrictVal<T> strict_val, T val)
{ return T(strict_val) != val; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline bool operator==(T val, StrictVal<T> strict_val)
{ return val == T(strict_val); }

template<RealType T> constexpr inline bool operator<(T val, StrictVal<T> strict_val)
{ return val < T(strict_val); }

template<RealType T> constexpr inline bool operator>(T val, StrictVal<T> strict_val)
{ return val > T(strict_val); }

template<RealType T> constexpr inline bool operator<=(T val, StrictVal<T> strict_val)
{ return val <= T(strict_val); }

template<RealType T> constexpr inline bool operator>=(T val, StrictVal<T> strict_val)
{ return val >= T(strict_val); }

template<RealType T> constexpr inline bool operator!=(T val, StrictVal<T> strict_val)
{ return val != T(strict_val); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline auto abs(StrictVal<T> v)
{ return T(v) > T(0) ? v : -v; }

template<RealType T> constexpr inline auto min(StrictVal<T> v1, StrictVal<T> v2)
{ return v1 < v2 ? v1 : v2; }

template<RealType T> constexpr inline auto max(StrictVal<T> v1, StrictVal<T> v2)
{ return v1 > v2 ? v1 : v2; }

template<RealType T> constexpr inline auto min(T val, StrictVal<T> strict_val)
{ return val < strict_val ? StrictVal<T>{val} : strict_val; }

template<RealType T> constexpr inline auto max(T val, StrictVal<T> strict_val)
{ return val > strict_val ? StrictVal<T>{val} : strict_val; }

template<RealType T> constexpr inline auto min(StrictVal<T> strict_val, T val)
{ return strict_val < val ? strict_val : StrictVal<T>{val}; }

template<RealType T> constexpr inline auto max(StrictVal<T> strict_val, T val)
{ return strict_val > val ? strict_val : StrictVal<T>{val}; }

template<StandardFloatingType T> constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2)
{
   auto r = v1 * v2;
   auto s = std::fma(T(v1), T(v2), T(-r));
   return std::pair<StrictVal<T>, StrictVal<T>>{r, s};
}

template<StandardFloatingType T> [[nodiscard]] inline auto exp(StrictVal<T> v)
{ return StrictVal<T>{std::exp(T(v))}; }

template<StandardFloatingType T> [[nodiscard]] inline auto sin(StrictVal<T> v)
{ return StrictVal<T>{std::sin(T(v))}; }

template<StandardFloatingType T> [[nodiscard]] inline auto cos(StrictVal<T> v)
{ return StrictVal<T>{std::cos(T(v))}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<NotQuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   int num_digits{};
   if(SameType<T, float>)            num_digits = std::numeric_limits<float>::digits10 + 1;
   else if(SameType<T, double>)      num_digits = std::numeric_limits<double>::digits10 + 1;
   else if(SameType<T, long double>) num_digits = std::numeric_limits<long double>::digits10 + 1;
   else                              num_digits = int(std::cout.precision());

   os << std::setprecision(num_digits) << T(strict_val);
   return os;
}

#ifdef STRICT_QUADRUPLE_PRECISION
template<QuadType T> constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2)
{
   auto r = v1 * v2;
   auto s = fmaq(T(v1), T(v2), T(-r));
   return std::pair<StrictVal<T>, StrictVal<T>>{r, s};
}

template<QuadType T> [[nodiscard]] inline auto exp(StrictVal<T> v)
{ return StrictVal<T>{expq(T(v))}; }

template<QuadType T> [[nodiscard]] inline auto sin(StrictVal<T> v)
{ return StrictVal<T>{sinq(T(v))}; }

template<QuadType T> [[nodiscard]] inline auto cos(StrictVal<T> v)
{ return StrictVal<T>{cosq(T(v))}; }

template<QuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   int width = 39;
   char buf[128];
   quadmath_snprintf(buf, sizeof(buf), "%+-#*.32Qe", width, T(strict_val));
   os << buf;
   return os;
}
#endif

}

#endif
