#pragma once

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

#include "error_handling.hpp"

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined __GNUC__ && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
   #define STRICT_ARRAY_QUADRUPLE_PRECISION
   #include <quadmath.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace strict_array {

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);
using float32 = float;
using float64 = double;

template<typename T, typename U>
concept SameType = std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value;

template<typename T>
concept IntegerType = std::is_same<std::remove_cv_t<T>, std::remove_cv_t<short>>::value         ||
                      std::is_same<std::remove_cv_t<T>, std::remove_cv_t<int>>::value           ||
                      std::is_same<std::remove_cv_t<T>, std::remove_cv_t<long int>>::value      ||
                      std::is_same<std::remove_cv_t<T>, std::remove_cv_t<long long int>>::value;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
   static_assert(sizeof(__float128) == 16);
   using float128 = __float128;
   template<typename T> concept FloatingType = std::is_floating_point<T>::value || SameType<float128, T>;
#else
   template<typename T> concept FloatingType = std::is_floating_point<T>::value;
#endif
template<typename T> concept RealType = FloatingType<T> || IntegerType<T>;

#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
   template<typename T> concept QuadType = SameType<T, float128>;
   template<typename T> concept NotQuadType = RealType<T> && !QuadType<T>;
   template<typename T> concept StandardFloatType = FloatingType<T> && !QuadType<T>;
#else
   template<typename T> concept NotQuadType = RealType<T>;
   template<typename T> concept StandardFloatType = FloatingType<T>;
#endif

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

template<StandardFloatType T> [[nodiscard]] constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2);

template<NotQuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
   template<QuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
   template<QuadType T> [[nodiscard]] constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2);
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
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(T(strict_val) == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
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
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(T(strict_val) == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
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
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(T(v2) == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
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
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(val == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
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
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(T(strict_val) == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return StrictVal<T>(T(val / T(strict_val)));
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

template<StandardFloatType T> constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2)
{
   auto r = v1 * v2;
   auto s = std::fma(T(v1), T(v2), T(-r));
   return std::pair<StrictVal<T>, StrictVal<T>>{r, s};
}

#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
template<QuadType T> constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2)
{
   auto r = v1 * v2;
   auto s = fmaq(T(v1), T(v2), T(-r));
   return std::pair<StrictVal<T>, StrictVal<T>>{r, s};
}
#endif

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

#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
template<QuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   int width = 39;
   char buf[128];
   quadmath_snprintf(buf, sizeof(buf), "%+-#*.32Qe", width, T(strict_val));
   os << buf;
   return os;
}
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ArrayBase{};
class ArrayExpr{};
class Operation{};

template<typename T> concept ArrayBaseType = std::is_base_of<ArrayBase, T>::value;
template<typename T> concept ArrayExprType = std::is_base_of<ArrayExpr, T>::value;
template<typename T> concept OperationType = std::is_base_of<Operation, T>::value;

// Forward declarations(expression templates)
template<ArrayBaseType T1, typename Op> class UnaryExpr;
template<ArrayBaseType T1, ArrayBaseType T2, OperationType Op> class BinExpr;
template<ArrayBaseType T1, RealType T2, OperationType Op> class BinExprValLeft;
template<ArrayBaseType T1, RealType T2, OperationType Op> class BinExprValRight;

template<typename T> concept FloatingArrayBaseType = ArrayBaseType<T> && FloatingType<typename T::value_type>;
template<typename T> concept StandardFloatingArrayBaseType = ArrayBaseType<T> && StandardFloatType<typename T::value_type>;
#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
template<typename T> concept QuadFloatingArrayBaseType = ArrayBaseType<T> && QuadType<typename T::value_type>;
#endif
template<typename T> concept IntegerArrayBaseType = ArrayBaseType<T> && IntegerType<typename T::value_type>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array : private ArrayBase
{
public:
   using size_type = long long int;
   using value_type = T;

   explicit Array();
   explicit Array(size_type size);
   explicit Array(size_type size, StrictVal<T> val);
   Array(std::initializer_list<StrictVal<T>> list);
   Array(const Array & A);
   Array(Array && a) noexcept;

   Array & operator=(StrictVal<T> val);
   Array & operator=(const Array & A);
   Array & operator=(Array && A) noexcept;

   template<ArrayExprType ArrExpr> Array(const ArrExpr & expr);
   template<ArrayExprType ArrExpr> const Array & operator=(const ArrExpr & expr);

   ~Array();

   [[nodiscard]] const Array & operator+() const;
   [[nodiscard]] auto operator-() const; // returns expression template

   const Array & operator+=(StrictVal<T> val);
   const Array & operator-=(StrictVal<T> val);
   const Array & operator*=(StrictVal<T> val);
   const Array & operator/=(StrictVal<T> val);

   template<ArrayBaseType ArrayType> const Array & operator+=(const ArrayType & A);
   template<ArrayBaseType ArrayType> const Array & operator-=(const ArrayType & A);
   template<ArrayBaseType ArrayType> const Array & operator*=(const ArrayType & A);
   template<ArrayBaseType ArrayType> const Array & operator/=(const ArrayType & A);

   void swap(Array & A) noexcept;
   void resize(size_type size);
   void resize_and_assign(const Array & A);

   [[nodiscard]] StrictVal<T> & operator[](size_type i);
   [[nodiscard]] const StrictVal<T> & operator[](size_type i) const;

   [[nodiscard]] StrictVal<T>* begin() & { return sz > 0 ? &elem[0] : nullptr; }
   [[nodiscard]] const StrictVal<T>* begin() const & { return sz > 0 ? &elem[0] : nullptr; }
   [[nodiscard]] StrictVal<T>* end() & { return sz > 0 ? &elem[sz] : nullptr; }
   [[nodiscard]] const StrictVal<T>* end() const & { return sz > 0 ? &elem[sz] : nullptr; }

   [[nodiscard]] size_type size() const { return sz; }
   [[nodiscard]] StrictVal<T>* data() & { return sz > 0 ? &elem[0] : nullptr; }
   [[nodiscard]] const StrictVal<T>* data() const & { return sz > 0 ? &elem[0] : nullptr; }

   [[nodiscard]] std::vector<StrictVal<T>*> within_range(StrictVal<T> low, StrictVal<T> high) &;
   [[nodiscard]] std::vector<const StrictVal<T>*> within_range(StrictVal<T> low, StrictVal<T> high) const &;
   [[nodiscard]] Array sub_array(size_type first, size_type last);

   void sort_increasing();
   void sort_decreasing();

private:
   size_type sz;
   StrictVal<T>* elem;

   bool valid_index(size_type index) const;
   template<typename F> void apply0(F f);
   template<ArrayBaseType ArrayType, typename F> void apply1(const ArrayType & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator+(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator-(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator*(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> [[nodiscard]] auto operator/(const T1 & A, const T2 & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(StrictVal<U> val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, StrictVal<U> val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(U val, const T & B);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, U val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, U val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, U val);
template<ArrayBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, U val);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A);

template<IntegerType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high);

template<FloatingType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high);

template<FloatingArrayBaseType ArrayType>
[[nodiscard]] auto sum(const ArrayType & A);

template<IntegerArrayBaseType ArrayType>
[[nodiscard]] auto sum(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto max(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto min(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto min_index(const ArrayType & A);  // returns std::pair<size_type, StrictVal<value_type>>

template<ArrayBaseType ArrayType>
[[nodiscard]] auto max_index(const ArrayType & A);  // returns std::pair<size_type, StrictVal<value_type>>

template<FloatingArrayBaseType ArrayType>
[[nodiscard]] auto norm_inf(const ArrayType & A);

template<StandardFloatingArrayBaseType ArrayType>
[[nodiscard]] auto norm2(const ArrayType & A);

#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
template<QuadFloatingArrayBaseType ArrayType>
[[nodiscard]] auto norm2(const ArrayType & A);
#endif

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
[[nodiscard]] auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool does_contain_zero(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool all_positive(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool all_negative(const ArrayType & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::Array() :
   sz{0},
   elem{nullptr}
{}

template<RealType T> Array<T>::Array(size_type size) :
   sz{size},
   elem{new StrictVal<T>[static_cast<std::size_t>(size)]}
{}

template<RealType T> Array<T>::Array(size_type size, StrictVal<T> val) :
   sz{size},
   elem{new StrictVal<T>[static_cast<std::size_t>(size)]}
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
}

template<RealType T> Array<T>::Array(std::initializer_list<StrictVal<T>> list)
   : Array(static_cast<size_type>(list.size()))
{
   std::copy(list.begin(), list.end(), begin());
}

template<RealType T> Array<T>::Array(const Array<T> & A) :
   Array(A.size())
{
   std::copy(A.begin(), A.end(), begin());
}

template<RealType T> Array<T>::Array(Array<T> && A) noexcept :
   sz{A.sz},
   elem{A.elem}
{
   A.elem = 0;
   A.sz = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T> & Array<T>::operator=(StrictVal<T> val)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
   return *this;
}

template<RealType T> Array<T> & Array<T>::operator=(const Array<T> & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz == A.sz);
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

template<RealType T> Array<T> & Array<T>::operator=(Array<T> && A) noexcept
{
   ASSERT_STRICT_ARRAY_DEBUG(sz == A.sz);
   delete[] elem;
   elem = A.elem;

   A.elem = nullptr;
   A.sz = 0;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<ArrayExprType ArrExpr> Array<T>::Array(const ArrExpr & expr)
   : Array(expr.size())
{
   for(size_type i = 0; i < sz; ++i)
      elem[i] = expr[i];
}

template<RealType T>
template<ArrayExprType ArrExpr> const Array<T> & Array<T>::operator=(const ArrExpr & expr)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz == expr.size());
   for(size_type i = 0; i < sz; ++i)
      elem[i] = expr[i];
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::~Array()
{ delete[] elem; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> const Array<T> & Array<T>::operator+() const
{ return *this; }

template<RealType T> auto Array<T>::operator-() const
{ return UnaryExpr(*this, [](T x){return -x;}); }

template<RealType T>
const Array<T> & Array<T>::operator+=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}

template<RealType T>
const Array<T> & Array<T>::operator-=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}

template<RealType T>
const Array<T> & Array<T>::operator*=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}

template<RealType T>
const Array<T> & Array<T>::operator/=(StrictVal<T> val)
{
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator+=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator-=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator*=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator/=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> void Array<T>::swap(Array<T> & A) noexcept
{
   StrictVal<T>* temp_elem = A.elem;
   size_type temp_sz = A.sz;

   A.elem = elem;
   A.sz = sz;

   elem = temp_elem;
   sz = temp_sz;
}

template<RealType T> void Array<T>::resize(size_type size)
{
   Array<T> temp(size);
   for(size_type i = 0; i < std::min(sz, size); ++i)
      temp.elem[i] = elem[i];
   swap(temp);
}

template<RealType T> void Array<T>::resize_and_assign(const Array<T> & A)
{
   resize(A.size());
   *this = A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
StrictVal<T> & Array<T>::operator[](size_type i)
{
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
const StrictVal<T> & Array<T>::operator[](size_type i) const
{
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
std::vector<StrictVal<T>*> Array<T>::within_range(StrictVal<T> low, StrictVal<T> high) &
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   std::vector<StrictVal<T>*> v;
   for(size_type i = 0; i < sz; ++i)
      if(elem[i] >= low && elem[i] <= high)
         v.push_back(&elem[i]);
   return v;
}

template<RealType T>
std::vector<const StrictVal<T>*> Array<T>::within_range(StrictVal<T> low, StrictVal<T> high) const &
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   std::vector<const StrictVal<T>*> v;
   for(size_type i = 0; i < sz; ++i)
      if(elem[i] >= low && elem[i] <= high)
         v.push_back(&elem[i]);
   return v;
}

template<RealType T> void Array<T>::sort_decreasing()
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   std::sort(begin(), end(), [](auto a, auto b) { return a > b; });
}

template<RealType T> void Array<T>::sort_increasing()
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   std::sort(begin(), end(), [](auto a, auto b) { return a < b; });
}

template<RealType T>
Array<T> Array<T>::sub_array(size_type first, size_type last)
{
   ASSERT_STRICT_ARRAY_DEBUG(valid_index(first));
   ASSERT_STRICT_ARRAY_DEBUG(valid_index(last));
   ASSERT_STRICT_ARRAY_DEBUG(last >= first);
   Array s(last-first+1);
   for(size_type i = 0; i < s.size(); ++i)
      s.elem[i] = elem[first+i];
   return s;
}

template<RealType T>
bool Array<T>::valid_index(size_type index) const
{
   if(index < 0 || index > sz-1)
      return false;
   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<typename F>
void Array<T>::apply0(F f)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<RealType T> template<ArrayBaseType ArrayType, typename F>
void Array<T>::apply1(const ArrayType & A, F f)
{
   (void)A;
   ASSERT_STRICT_ARRAY_DEBUG(sz == A.sz);
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

namespace internal {
   template<RealType T>
   std::string smart_spaces(typename Array<T>::size_type max_ind, typename Array<T>::size_type ind)
   {
      using sz_T = typename Array<T>::size_type;
      auto count_digit = [](sz_T number) -> sz_T {
         if(!number) return 1;
         return (sz_T)std::log10(number) + 1;
      };

      sz_T max_digits = count_digit(max_ind);
      sz_T ind_digits = count_digit(ind);
      return std::string(static_cast<std::basic_string<char>::size_type>(1+max_digits-ind_digits), 32);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Plus : private Operation
{
   template<RealType T>
   auto operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left + right;
   }
};

struct Minus : private Operation
{
   template<RealType T>
   auto operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left - right;
   }
};

struct Mult : private Operation
{
   template<RealType T>
   auto operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left * right;
   }
};

struct Divide : private Operation
{
   template<RealType T>
   auto operator()(const StrictVal<T> left, const StrictVal<T> right) const {
      return left / right;
   }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, typename Op>
class UnaryExpr : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T::size_type;
   using value_type = typename T::value_type;

   UnaryExpr(const T & A, Op op) : sz(A.size()), A(A), op(op) { ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0); }
   UnaryExpr(const UnaryExpr &) = delete;
   UnaryExpr & operator=(const UnaryExpr &) = delete;

   [[nodiscard]] decltype(auto) operator[](size_type i) const { return op(A[i]); }
   [[nodiscard]] size_type size() const { return sz; }

private:
   const size_type sz;
   const T & A;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2, OperationType Op>
class BinExpr : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;

   BinExpr(const T1 & A, const T2 & B, Op op) : sz(A.size()), A(A), B(B), op(op) {
      static_assert(SameType<typename T1::value_type, typename T2::value_type>);
      ASSERT_STRICT_ARRAY_DEBUG(A.size() == B.size());
      ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   }
   BinExpr(const BinExpr &) = delete;
   BinExpr & operator=(const BinExpr &) = delete;

   [[nodiscard]] decltype(auto) operator[](size_type i) const { return op(A[i], B[i]); }
   [[nodiscard]] size_type size() const { return sz; }

private:
   const size_type sz;
   const T1 & A;
   const T2 & B;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, RealType T2, OperationType Op>
class BinExprValLeft : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;

   BinExprValLeft(const T1 & B, T2 val, Op op) : sz(B.size()), B(B), val(val), op(op) {
      static_assert(SameType<typename T1::value_type, T2>);
      ASSERT_STRICT_ARRAY_DEBUG(B.size() > 0);
   }
   BinExprValLeft(const BinExprValLeft &) = delete;
   BinExprValLeft & operator=(const BinExprValLeft &) = delete;

   [[nodiscard]] decltype(auto) operator[](size_type i) const { return op(val, B[i]); }
   [[nodiscard]] size_type size() const { return sz; }

private:
   const size_type sz;
   const T1 & B;
   const StrictVal<T2> val;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, RealType T2, OperationType Op>
class BinExprValRight : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;

   BinExprValRight(const T1 & A, T2 val, Op op) : sz(A.size()), A(A), val(val), op(op) {
      static_assert(SameType<typename T1::value_type, T2>);
      ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   }
   BinExprValRight(const BinExprValRight &) = delete;
   BinExprValRight & operator=(const BinExprValRight &) = delete;

   [[nodiscard]] decltype(auto) operator[](size_type i) const { return op(A[i], val); }
   [[nodiscard]] size_type size() const { return sz; }

private:
   const size_type sz;
   const T1 & A;
   const StrictVal<T2> val;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2>
auto operator+(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Plus{}); }

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator-(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Minus{}); }

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator*(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Mult{}); }

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator/(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U(val), Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U(val), Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U(val), Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U(val), Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U(val), Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U(val), Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U(val), Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U(val), Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(U val, const T & B)
{ return BinExprValLeft(B, val, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(U val, const T & B)
{ return BinExprValLeft(B, val, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(U val, const T & B)
{ return BinExprValLeft(B, val, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(U val, const T & B)
{ return BinExprValLeft(B, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(const T & A, U val)
{ return BinExprValRight(A, val, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(const T & A, U val)
{ return BinExprValRight(A, val, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(const T & A, U val)
{ return BinExprValRight(A, val, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(const T & A, U val)
{ return BinExprValRight(A, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType> std::ostream & operator<<(std::ostream & os, const ArrayType & A)
{
   using T = typename ArrayType::value_type;
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      os << "Array[" << i << "] =" << internal::smart_spaces<T>(A.size(), i) << A[i] << std::endl;
   }
   return os;
}

template<IntegerType T>
Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_ARRAY_DEBUG(size > 0);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   long int diff_range = (high - low).template convert<long int>() + 1;
   for(auto & x : A)
      x = low + T(std::rand() % diff_range);
   return A;
}

template<FloatingType T>
Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_ARRAY_DEBUG(size > 0);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   for(auto & x : A)
      x = low + (high - low) * T(std::rand()) / T(RAND_MAX);
   return A;
}

template<FloatingArrayBaseType ArrayType>
auto sum(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using T = typename ArrayType::value_type;

    T sum{};
    T c{};
    for(decltype(A.size()) i = 0; i < A.size(); ++i) {
        T y = A[i] - c;
        T t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return StrictVal<T>{sum};
}

template<IntegerArrayBaseType ArrayType>
auto sum(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   auto sum = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      sum += A[i];
   return sum;
}

template<ArrayBaseType ArrayType>
auto max(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   auto max_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      max_elem = max(A[i], max_elem);
   return max_elem;
}

template<ArrayBaseType ArrayType>
auto min(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   auto min_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      min_elem = min(A[i], min_elem);
   return min_elem;
}

template<ArrayBaseType ArrayType>
auto max_index(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;

   std::pair<sz_T, StrictVal<T>> max = {0, A[0]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] > max.second)
         max = {i, A[i]};
   return max;
}

template<ArrayBaseType ArrayType>
auto min_index(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;

   std::pair<sz_T, StrictVal<T>> min = {0, A[0]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] < min.second)
         min = {i, A[i]};
   return min;
}

template<FloatingArrayBaseType ArrayType>
auto norm_inf(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   auto max_abs = abs(A[0]);
   for(decltype(A.size()) i = 1; i < A.size(); ++i) {
      auto abs_i = abs(A[i]);
      max_abs  = max(abs_i, max_abs);
   }
   return max_abs;
}

template<StandardFloatingArrayBaseType ArrayType>
auto norm2(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using T = typename ArrayType::value_type;
   return StrictVal<T>{std::sqrt(T(dot_prod(A, A)))};
}

#ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
template<QuadFloatingArrayBaseType ArrayType>
auto norm2(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using T = typename ArrayType::value_type;
   return StrictVal<T>{sqrtq(T(dot_prod(A, A)))};
}
#endif

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2)
{
   ASSERT_STRICT_ARRAY_DEBUG(A1.size() == A2.size());
   ASSERT_STRICT_ARRAY_DEBUG(A1.size() > 0);
   return sum(A1 * A2);
}

template<ArrayBaseType ArrayType>
bool does_contain_zero(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using T = typename ArrayType::value_type;
   for(auto x : A)
      if(x == T(0)) return true;
   return false;
}

template<ArrayBaseType ArrayType>
bool all_positive(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using T = typename ArrayType::value_type;
   for(auto x : A)
     if(x <= T(0)) return false;
   return true;
}

template<ArrayBaseType ArrayType>
bool all_negative(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   using T = typename ArrayType::value_type;
   for(auto x : A)
     if(x >= T(0)) return false;
   return true;
}

}

#endif
