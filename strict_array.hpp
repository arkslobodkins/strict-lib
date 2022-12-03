#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <numeric>

#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
#include <quadmath.h>
#endif

// compile with -DSTRICT_ARRAY_DEBUG_ON to enable range checking, default is OFF
#ifndef STRICT_ARRAY_DEBUG_ON
#define STRICT_ARRAY_DEBUG_OFF
#endif

// compile with -DSTRICT_ARRAY_DIVISION_ON to enable division by 0 checking, default is OFF
#ifndef STRICT_ARRAY_DIVISION_ON
#define STRICT_ARRAY_DIVISION_OFF
#endif

// No aliasing
#if defined __GNUC__ || defined __clang__ || defined __INTEL_LLVM_COMPILER || defined __INTEL_COMPILER
#define array_restrict_ptr __restrict
#else
#define array_restrict_ptr
#endif

#ifdef STRICT_ARRAY_DEBUG_OFF
#define ASSERT_STRICT_ARRAY_DEBUG(condition) ((void)0)
#else
#define ASSERT_STRICT_ARRAY_DEBUG(condition) assert(condition)
#endif

static inline std::string trace_err(const char* file, const char* func, int line)
{ return "file: " + std::string(file) + ", function: " + std::string(func) + ", line: " + std::to_string(line); }

#define STRICT_ARRAY_THROW_OUT_OF_RANGE()                                                   \
   do {                                                                                     \
   throw std::out_of_range{"OUT OF RANGE! " + trace_err(__FILE__, __func__, __LINE__)};     \
   } while(0)

#define STRICT_ARRAY_THROW_ZERO_DIVISION()                                                   \
   do {                                                                                      \
   throw std::out_of_range{"ZERO DIVISION! " + trace_err(__FILE__, __func__, __LINE__)};     \
   } while(0)

namespace strict_array {

static_assert(sizeof(long int) == 8);
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
#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
static_assert(sizeof(__float128) == 16);
using float128 = __float128;

template<typename T>
concept FloatingType = std::is_floating_point<T>::value || SameType<float128, T>;

#else
template<typename T>
concept FloatingType = std::is_floating_point<T>::value;
#endif

template<typename T>
concept RealType = FloatingType<T> || IntegerType<T>;

class ArrayExpr{};
class ArrayBase{};
class Operation{};
template<typename T> concept ArrayBaseType = std::is_base_of<ArrayBase, T>::value;
template<typename T> concept ArrayExprType = std::is_base_of<ArrayExpr, T>::value;
template<typename T> concept OperationType = std::is_base_of<Operation, T>::value;

// Forward declarations(expression templates)
template<ArrayBaseType T1, ArrayBaseType T2, OperationType Op> class BinExpr;
template<ArrayBaseType T1, OperationType Op> class BinExprValLeft;
template<ArrayBaseType T1, OperationType Op> class BinExprValRight;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array : private ArrayBase
{
public:
   using size_type = long int;
   using value_type = T;

   explicit Array();
   template<IntegerType S> explicit Array(S size);
   template<IntegerType S, RealType U> explicit Array(S size, U val);
   Array(std::initializer_list<T>);
   Array(const Array & a);
   Array(Array && a);

   template<RealType U> Array & operator=(U val);
   Array & operator=(const Array & a);
   Array & operator=(Array && a);

   template<ArrayExprType ArrExpr> Array(const ArrExpr & expr);
   template<ArrayExprType ArrExpr> Array & operator=(const ArrExpr & expr);

   ~Array();

   inline size_type size() const;
   template<IntegerType S> inline T & operator[](S i);
   template<IntegerType S> inline const T & operator[](S i) const;

   const Array & operator+() const;
   Array operator-() const;

   template<RealType U> Array & operator+=(U val);
   template<RealType U> Array & operator-=(U val);
   template<RealType U> Array & operator*=(U val);
   template<RealType U> Array & operator/=(U val);

   template<ArrayBaseType ArrayType> Array & operator+=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator-=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator*=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator/=(const ArrayType & A);

   T* data() { return sz ? elem : nullptr; }
   T* begin() { return sz ? elem : nullptr; }
   T* end() { return sz ? elem+sz : nullptr; }
   const T* data() const { return sz ? elem : nullptr; }
   const T* begin() const { return sz ? elem : nullptr; }
   const T* end() const { return sz ? elem+sz : nullptr; }

   template<IntegerType S> Array & resize(S size);
   Array & resize_and_assign(const Array & a);

   T min() const;
   T max() const;
   std::pair<size_type, T> min_index() const;
   std::pair<size_type, T> max_index() const;

   template<IntegerType S1, IntegerType S2> Array sub_array(S1 first, S2 last);
   T sum() const;
   template<FloatingType U1, FloatingType U2> void fill_random(U1 low, U2 high);

   bool does_contain_zero() const;
   bool positive() const;
   bool non_negative() const;

private:
   size_type sz;
   T* array_restrict_ptr elem;

   template<typename F> void apply0(F f);
   template<ArrayBaseType ArrayType, typename F> void apply1(const ArrayType & A, F f);
};

template<FloatingType T>
std::ostream & operator<<(std::ostream & os, Array<T> A)
{
   if(SameType<T, double> || SameType<T, long double>) {
      for(auto e : A) os << std::setprecision(16) << e << std::endl;
   }
   else if(SameType<T, float>) {
      for(auto e : A) os << std::setprecision(8) << e << std::endl;
   }
   #if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
   else if(SameType<T, float128>) {
      int width = 46;
      char buf[128];
      for(auto e : A)
      {
         quadmath_snprintf (buf, sizeof(buf), "%+-#*.32Qe", width, e);
         os << buf << std::endl;
      }
   }
   #endif
   else {
      for(auto e : A) os << e << std::endl;
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType S, FloatingType T1, FloatingType T2>
Array<T1> array_random(S size, T1 low, T2 high);

template<RealType T>
T dot_prod(const Array<T> & v1, const Array<T> & v2);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T>::Array() : sz{0}, elem{nullptr}
{}

template<RealType T> template<IntegerType S>
Array<T>::Array(S size) : sz{size}, elem{new T[size]{}}
{ static_assert(SameType<size_type, S>); }

template<RealType T> template<IntegerType S, RealType U>
Array<T>::Array(S size, U val) : sz{size}, elem{new T[size]}
{
   static_assert(SameType<T, U>);
   static_assert(SameType<size_type, S>);
   ASSERT_STRICT_ARRAY_DEBUG(size >= 1);
   std::fill(begin(), end(), val);
}

template<RealType T>
Array<T>::Array(std::initializer_list<T> l) : Array(static_cast<size_type>(l.size()))
{ std::copy(l.begin(), l.end(), begin()); }

template<RealType T>
Array<T>::Array(const Array<T> & a) : sz{a.size()}, elem{new T[a.size()]}
{ std::copy(a.begin(), a.end(), begin()); }

template<RealType T>
Array<T>::Array(Array && a) : sz{a.sz}, elem{a.elem}
{
   a.elem = nullptr;
   a.sz = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
Array<T> & Array<T>::operator=(const U val)
{
   static_assert(SameType<T, U>);
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(const Array<T> & a)
{
   if(this != &a)
      apply1(a, [&](size_type i) { elem[i] = a[i]; });
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(Array<T> && a)
{
   if(this != &a) {
      ASSERT_STRICT_ARRAY_DEBUG(sz == a.sz);

      delete[] elem;
      elem = a.elem;
      a.elem = nullptr;
      a.sz = 0;
   }

   return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayExprType ArrExpr>
Array<T>::Array(const ArrExpr & expr)
   : sz{expr.size()}, elem{new T[expr.size()]}
{
   static_assert(SameType<T, typename ArrExpr::value_type>);
   apply1(expr, [&](size_type i) { elem[i] = expr[i]; });
}

template<RealType T> template<ArrayExprType ArrExpr>
Array<T> & Array<T>::operator=(const ArrExpr & expr)
{
   static_assert(SameType<T, typename ArrExpr::value_type>);
   apply1(expr, [&](size_type i) { elem[i] = expr[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T>::~Array() { delete[] elem; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline typename Array<T>::size_type Array<T>::size() const { return sz; }

template<RealType T> template<IntegerType S>
inline T & Array<T>::operator[](S i)
{
   static_assert(SameType<size_type, S>);
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(i < 0 || i > sz-1) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T> template<IntegerType S>
inline const T & Array<T>::operator[](S i) const
{
   static_assert(SameType<size_type, S>);
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(i < 0 || i > sz-1) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
const Array<T> & Array<T>::operator+() const { return *this; }

template<RealType T>
Array<T> Array<T>::operator-() const { return *this * T(-1); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
Array<T> & Array<T>::operator+=(const U val)
{
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] += val; } ); return *this;
}

template<RealType T> template<RealType U>
Array<T> & Array<T>::operator-=(const U val)
{
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] -= val; } ); return *this;
}

template<RealType T> template<RealType U>
Array<T> & Array<T>::operator*=(const U val)
{
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] *= val; } ); return *this;
}

template<RealType T> template<RealType U>
Array<T> & Array<T>::operator/=(const U val)
{
   static_assert(SameType<T, U>);
   #ifdef QUAD_DIVISION_ON
   if(val == 0) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator+=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator-=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator*=(const ArrayType & A)
{
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator/=(const ArrayType & A)
{
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(A.does_contain_zero()) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<IntegerType S>
Array<T> & Array<T>::resize(S size)
{
   static_assert(SameType<size_type, S>);
   if(size == sz) return *this;

   delete[] elem;
   ASSERT_STRICT_ARRAY_DEBUG(size > -1);
   elem = new T[size]{};
   sz = size;
   return *this;
}

template<RealType T>
Array<T> & Array<T>::resize_and_assign(const Array<T> & a)
{
   resize(a.size());
   return *this = a;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
T Array<T>::min() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   return *std::min_element(begin(), end());
}

template<RealType T>
T Array<T>::max() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   return *std::max_element(begin(), end());
}

template<RealType T>
std::pair<typename Array<T>::size_type, T> Array<T>::min_index() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   auto min = std::min_element(begin(), end());
   return {min - begin(), *min};
}

template<RealType T>
std::pair<typename Array<T>::size_type, T> Array<T>::max_index() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   auto max = std::max_element(begin(), end());
   return {max - begin(), *max};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<IntegerType S1, IntegerType S2>
Array<T> Array<T>::sub_array(S1 first, S2 last)
{
   static_assert(SameType<size_type, S1>);
   static_assert(SameType<size_type, S2>);
   size_type sub_sz = last - first + 1;
   Array<T> sub_array(sub_sz);
   for(size_type i = 0; i < sub_sz; ++i)
      sub_array[i] = elem[first+i];
   return sub_array;
}

template<RealType T>
T Array<T>::sum() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   return std::accumulate(begin(), end(), T(0.));
}

template<RealType T> template<FloatingType U1, FloatingType U2>
void Array<T>::fill_random(U1 low, U2 high)
{
   static_assert(SameType<T, U1>);
   static_assert(SameType<U1, U2>);

   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(auto & x : *this)
      x = low + (high - low) * T(std::rand()) / T(RAND_MAX);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
bool Array<T>::does_contain_zero() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(auto x : *this)
      if(x == T(0)) return true;
   return false;
}

template<RealType T>
bool Array<T>::positive() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(auto x : *this)
      if(x <= T(0)) return false;
   return true;
}

template<RealType T>
bool Array<T>::non_negative() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(auto x : *this)
      if(x < T(0)) return false;
   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType S, FloatingType T1, FloatingType T2>
Array<T1> array_random(S size, T1 low, T2 high)
{
   static_assert(SameType<T1, T2>);
   static_assert(SameType<typename Array<T1>::size_type, S>);
   Array<T1> a(size);
   a.fill_random(low, high);
   return a;
}

template<RealType T>
T dot_prod(const Array<T> & v1, const Array<T> & v2)
{
   ASSERT_STRICT_ARRAY_DEBUG(v1.size() == v2.size());
   return std::inner_product(v1.begin(), v1.end(), v2.begin(), T(0));
}

template<RealType T>
template<typename F> void Array<T>::apply0(F f)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<RealType T>
template<ArrayBaseType ArrayType, typename F>
void Array<T>::apply1(const ArrayType & A, F f)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz == A.size());
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Plus : private Operation
{ template<RealType T> T operator()(const T left, const T right) const { return left + right; } };

struct Minus : private Operation
{ template<RealType T> T operator()(const T left, const T right) const { return left - right; } };

struct Mult : private Operation
{ template<RealType T> T operator()(const T left, const T right) const { return left * right; } };

struct Divide : private Operation
{ template<RealType T> T operator()(const T left, const T right) const { return left / right; } };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2, OperationType Op>
class BinExpr : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   BinExpr(const T1 & a, const T2 & b, Op op) : sz(a.size()), A(a), B(b), op(op)
   {
      static_assert( SameType<typename T1::value_type, typename T2::value_type> );
      ASSERT_STRICT_ARRAY_DEBUG(a.size() == b.size());
   }

   value_type operator[](size_type i) const { return op(A[i], B[i]); }
   size_type size() const { return sz; }
   bool does_contain_zero() const;

private:
   const size_type sz;
   const T1 & A;
   const T2 & B;
   Op op;
};

template<ArrayBaseType T1, ArrayBaseType T2, OperationType Op>
bool BinExpr<T1, T2, Op>::does_contain_zero() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      if((*this)[i] == 0.) return true;
   return false;
}

template<ArrayBaseType T1, OperationType Op>
class BinExprValLeft : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;

   template<RealType T2>
   BinExprValLeft(const T1 & b, T2 val, Op op) : sz(b.size()), B(b), val(val), op(op)
   { static_assert( SameType<typename T1::value_type, T2> ); }

   value_type operator[](size_type i) const { return op(val, B[i]); }
   size_type size() const { return sz; }
   bool does_contain_zero() const;

private:
   const size_type sz;
   const T1 & B;
   const value_type val;
   Op op;
};

template<ArrayBaseType T1, OperationType Op>
bool BinExprValLeft<T1, Op>::does_contain_zero() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      if((*this)[i] == 0.) return true;
   return false;
}

template<ArrayBaseType T1, OperationType Op>
class BinExprValRight : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;

   template<RealType T2>
   BinExprValRight(const T1 & a, T2 val, Op op) : sz(a.size()), A(a), val(val), op(op)
   { static_assert( SameType<typename T1::value_type, T2> ); }

   value_type operator[](size_type i) const { return op(A[i], val); }
   size_type size() const { return sz; }
   bool does_contain_zero() const;

private:
   const size_type sz;
   const T1 & A;
   const value_type val;
   Op op;
};

template<ArrayBaseType T1, OperationType Op>
bool BinExprValRight<T1, Op>::does_contain_zero() const
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(size_type i = 0; i < sz; ++i)
      if((*this)[i] == 0.) return true;
   return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Plus> operator+(const T1 & A, const T2 & B)
{
   static_assert( SameType<typename T1::value_type, typename T2::value_type> );
   return BinExpr<T1, T2, Plus>(A, B, Plus{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Minus> operator-(const T1 & A, const T2 & B)
{
   static_assert( SameType<typename T1::value_type, typename T2::value_type> );
   return BinExpr<T1, T2, Minus>(A, B, Minus{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Mult> operator*(const T1 & A, const T2 & B)
{
   static_assert( SameType<typename T1::value_type, typename T2::value_type> );
   return BinExpr<T1, T2, Mult>(A, B, Mult{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Divide> operator/(const T1 & A, const T2 & B)
{
   static_assert( SameType<typename T1::value_type, typename T2::value_type> );
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(B.does_contain_zero()) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExpr<T1, T2, Divide>(A, B, Divide{});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
BinExprValRight<T, Plus> operator+(const T & A, U val)
{
   static_assert( SameType<typename T::value_type, U> );
   return BinExprValRight<T, Plus>(A, val, Plus{});
}

template<ArrayBaseType T, RealType U>
BinExprValRight<T, Minus> operator-(const T & A, U val)
{
   static_assert( SameType<typename T::value_type, U> );
   return BinExprValRight<T, Minus>(A, val, Minus{});
}

template<ArrayBaseType T, RealType U>
BinExprValRight<T, Mult> operator*(const T & A, U val)
{
   static_assert( SameType<typename T::value_type, U> );
   return BinExprValRight<T, Mult>(A, val, Mult{});
}

template<ArrayBaseType T, RealType U>
BinExprValRight<T, Divide> operator/(const T & A, U val)
{
   static_assert( SameType<typename T::value_type, U> );
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(val == 0) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExprValRight<T, Divide>(A, val, Divide{});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
BinExprValLeft<T, Plus> operator+(U val, const T & B)
{
   static_assert( SameType<typename T::value_type, U> );
   return BinExprValLeft<T, Plus>(B, val, Plus{});
}

template<ArrayBaseType T, RealType U>
BinExprValLeft<T, Minus> operator-(U val, const T & B)
{
   static_assert( SameType<typename T::value_type, U> );
   return BinExprValLeft<T, Minus>(B, val, Minus{});
}

template<ArrayBaseType T, RealType U>
BinExprValLeft<T, Mult> operator*(U val, const T & B)
{
   static_assert( SameType<typename T::value_type, U> );
   return BinExprValLeft<T, Mult>(B, val, Mult{});
}

template<ArrayBaseType T, RealType U>
BinExprValLeft<T, Divide> operator/(U val, const T & B)
{
   static_assert( SameType<typename T::value_type, U> );
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(B.does_contain_zero()) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExprValLeft<T, Divide>(B, val, Divide{});
}

}

#endif
