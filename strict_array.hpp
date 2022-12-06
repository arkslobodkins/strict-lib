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
template<ArrayBaseType T1, RealType T2, OperationType Op> class BinExprValLeft;
template<ArrayBaseType T1, RealType T2, OperationType Op> class BinExprValRight;

#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
#define STRICT_ARRAY_QUADRUPLE_PRECISION
template<typename T>
   concept QuadArrayBaseType = std::is_same<float128, typename T::value_type>::value && ArrayBaseType<T>;

template<typename T>
   concept NotQuadArrayBaseType = !(std::is_same<float128, typename T::value_type>::value && ArrayBaseType<T>);
#else
concept NotQuadArrayBaseType = true;
#endif

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
   template<RealType U> Array(std::initializer_list<U>);
   Array(const Array & a);
   Array(Array && a);

   template<RealType U> Array & operator=(U val);
   Array & operator=(const Array & a);
   Array & operator=(Array && a);

   template<ArrayExprType ArrExpr> Array(const ArrExpr & expr);
   template<ArrayExprType ArrExpr> Array & operator=(const ArrExpr & expr);

   template<IntegerType S> Array & resize(S size);
   Array & resize_and_assign(const Array & a);

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

   template<IntegerType S1, IntegerType S2> Array sub_array(S1 first, S2 last);
   template<IntegerType U1, IntegerType U2> void fill_random(U1 low, U2 high);
   template<FloatingType U1, FloatingType U2> void fill_random(U1 low, U2 high);

private:
   size_type sz;
   T* array_restrict_ptr elem;

   inline bool is_valid_index(size_type index);
   template<typename F> void apply0(F f);
   template<ArrayBaseType ArrayType, typename F> void apply1(const ArrayType & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType S, RealType T1, RealType T2>
Array<T1> array_random(S size, T1 low, T2 high);

template<NotQuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A);

#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
template<QuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A);
#endif

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2);

template<ArrayBaseType ArrayType>
bool does_contain_zero(const ArrayType & A);

template<ArrayBaseType ArrayType>
bool is_positive(const ArrayType & A);

template<ArrayBaseType ArrayType>
bool is_nonnegative(const ArrayType & A);

template<ArrayBaseType ArrayType>
auto sum(const ArrayType & A);

template<ArrayBaseType ArrayType>
auto min(const ArrayType & A);

template<ArrayBaseType ArrayType>
auto max(const ArrayType & A);

template<ArrayBaseType ArrayType>
auto min_index(const ArrayType & A);

template<ArrayBaseType ArrayType>
auto max_index(const ArrayType & A);

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
   static_assert(SameType<size_type, S>);
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] = val; } );
}

template<RealType T> template<RealType U>
Array<T>::Array(std::initializer_list<U> l) : Array(static_cast<size_type>(l.size()))
{
   static_assert(SameType<T, U>);
   std::copy(l.begin(), l.end(), begin());
}

template<RealType T>
Array<T>::Array(const Array<T> & a) : sz{a.size()}, elem{new T[a.size()]}
{ apply1(a, [&](size_type i) { elem[i] = a[i]; } ); }

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
   apply0([&](size_type i) { elem[i] = val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(const Array<T> & a)
{
   if(this != &a) apply1(a, [&](size_type i) { elem[i] = a[i]; });
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
   static_assert(SameType<size_type, typename ArrExpr::size_type>);
   static_assert(SameType<T, typename ArrExpr::value_type>);
   apply1(expr, [&](size_type i) { elem[i] = expr[i]; });
}

template<RealType T> template<ArrayExprType ArrExpr>
Array<T> & Array<T>::operator=(const ArrExpr & expr)
{
   static_assert(SameType<size_type, typename ArrExpr::size_type>);
   static_assert(SameType<T, typename ArrExpr::value_type>);
   apply1(expr, [&](size_type i) { elem[i] = expr[i]; });
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
Array<T>::~Array() { delete[] elem; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline typename Array<T>::size_type Array<T>::size() const { return sz; }

template<RealType T> template<IntegerType S>
inline T & Array<T>::operator[](S i)
{
   static_assert(SameType<size_type, S>);
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!is_valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T> template<IntegerType S>
inline const T & Array<T>::operator[](S i) const
{
   static_assert(SameType<size_type, S>);
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!is_valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
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
   static_assert(SameType<T, typename ArrayType::value_type>);
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator-=(const ArrayType & A)
{
   static_assert(SameType<T, typename ArrayType::value_type>);
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator*=(const ArrayType & A)
{
   static_assert(SameType<T, typename ArrayType::value_type>);
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator/=(const ArrayType & A)
{
   static_assert(SameType<T, typename ArrayType::value_type>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(does_contain_zero(A)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
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

template<RealType T> template<IntegerType U1, IntegerType U2>
void Array<T>::fill_random(U1 low, U2 high)
{
   static_assert(SameType<T, U1>);
   static_assert(SameType<U1, U2>);
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(auto & x : *this)
      x = low + T(std::rand() % (1+high-low));
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline bool Array<T>::is_valid_index(size_type index)
{
   if(index < 0 || index > sz-1)
      return false;
   return true;
}

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
   ASSERT_STRICT_ARRAY_DEBUG(sz == A.size());
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType S, RealType T1, RealType T2>
Array<T1> array_random(S size, T1 low, T2 high)
{
   static_assert(SameType<typename Array<T1>::size_type, S>);
   static_assert(SameType<T1, T2>);
   Array<T1> a(size);
   a.fill_random(low, high);
   return a;
}

template<NotQuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A)
{
   using T = typename ArrayType::value_type;
   using sz_T = typename ArrayType::size_type;

   if(SameType<T, double> || SameType<T, long double>) {
      for(sz_T i = 0; i < A.size(); ++i) {
         os << std::setprecision(16) << A[i] << std::endl;
      }
   }
   else if(SameType<T, float>) {
      for(sz_T i = 0; i < A.size(); ++i) {
         os << std::setprecision(8) << A[i] << std::endl;
      }
   }
   else {
      for(sz_T i = 0; i < A.size(); ++i) {
         os << A[i] << std::endl;
      }
   }
   return os;
}

#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
template<QuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   int width = 46;
   char buf[128];
   for(sz_T i = 0; i < A.size(); ++i) {
      quadmath_snprintf (buf, sizeof(buf), "%+-#*.32Qe", width, A[i]);
      os << buf << std::endl;
   }
   return os;
}
#endif

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2)
{
   static_assert(SameType<typename ArrayType1::size_type, typename ArrayType2::size_type>);
   static_assert(SameType<typename ArrayType1::value_type, typename ArrayType2::value_type>);
   ASSERT_STRICT_ARRAY_DEBUG(A1.size() == A2.size());
   typename ArrayType1::value_type prod{};
   for(typename ArrayType1::size_type i = 0; i < A1.size(); ++i)
      prod += A1[i] * A2[i];
   return prod;
}

template<ArrayBaseType ArrayType>
bool does_contain_zero(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(A.size() > 0);
   for(typename ArrayType::size_type i = 0; i < A.size(); ++i)
      if(A[i] == typename ArrayType::value_type(0)) return true;
   return false;
}

template<ArrayBaseType ArrayType>
bool is_positive(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(typename ArrayType::size_type i = 0; i < A.size(); ++i)
      if(A[i] <= typename ArrayType::value_type(0)) return false;
   return true;
}

template<ArrayBaseType ArrayType>
bool is_nonnegative(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   for(typename ArrayType::size_type i = 0; i < A.size(); ++i)
      if(A[i] < typename ArrayType::value_type(0)) return false;
   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
auto sum(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   typename ArrayType::value_type s{};
   for(typename ArrayType::size_type i = 0; i < A.size(); ++i)
      s += A[i];
   return s;
}

template<ArrayBaseType ArrayType>
auto min(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;

   T m = A[sz_T(0)];
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] < m) m = A[i];
   return m;
}

template<ArrayBaseType ArrayType>
auto max(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;

   T m = A[sz_T(0)];
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] > m) m = A[i];
   return m;
}

template<ArrayBaseType ArrayType>
auto min_index(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;

   std::pair<sz_T, T> min = {0, A[sz_T(0)]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] < min.second)
         min = {i, A[i]};
   return min;
}

template<ArrayBaseType ArrayType>
auto max_index(const ArrayType & A)
{
   ASSERT_STRICT_ARRAY_DEBUG(sz > 0);
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;

   std::pair<sz_T, T> max = {0, A[sz_T(0)]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] > max.second)
         max = {i, A[i]};
   return max;
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
      static_assert(SameType<typename T1::value_type, typename T2::value_type>);
      ASSERT_STRICT_ARRAY_DEBUG(a.size() == b.size());
   }
   BinExpr(const BinExpr &) = default;
   BinExpr & operator=(const BinExpr &) = default;

   value_type operator[](size_type i) const { return op(A[i], B[i]); }
   size_type size() const { return sz; }

private:
   const size_type sz;
   const T1 & A;
   const T2 & B;
   Op op;
};

template<ArrayBaseType T1, RealType T2, OperationType Op>
class BinExprValLeft : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;

   BinExprValLeft(const T1 & b, T2 val, Op op) : sz(b.size()), B(b), val(val), op(op)
   { static_assert(SameType<typename T1::value_type, T2>); }
   BinExprValLeft(const BinExprValLeft &) = default;
   BinExprValLeft & operator=(const BinExprValLeft &) = default;

   value_type operator[](size_type i) const { return op(val, B[i]); }
   size_type size() const { return sz; }

private:
   const size_type sz;
   const T1 & B;
   const T2 val;
   Op op;
};

template<ArrayBaseType T1, RealType T2, OperationType Op>
class BinExprValRight : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;

   BinExprValRight(const T1 & a, T2 val, Op op) : sz(a.size()), A(a), val(val), op(op)
   { static_assert(SameType<typename T1::value_type, T2>); }
   BinExprValRight(const BinExprValRight &) = default;
   BinExprValRight & operator=(const BinExprValRight &) = default;

   value_type operator[](size_type i) const { return op(A[i], val); }
   size_type size() const { return sz; }

private:
   const size_type sz;
   const T1 & A;
   const T2 val;
   Op op;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Plus> operator+(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   return BinExpr<T1, T2, Plus>(A, B, Plus{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Minus> operator-(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   return BinExpr<T1, T2, Minus>(A, B, Minus{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Mult> operator*(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   return BinExpr<T1, T2, Mult>(A, B, Mult{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
BinExpr<T1, T2, Divide> operator/(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(does_contain_zero(B)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExpr<T1, T2, Divide>(A, B, Divide{});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
BinExprValRight<T, U, Plus> operator+(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValRight<T, U, Plus>(A, val, Plus{});
}

template<ArrayBaseType T, RealType U>
BinExprValRight<T, U, Minus> operator-(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValRight<T, U, Minus>(A, val, Minus{});
}

template<ArrayBaseType T, RealType U>
BinExprValRight<T, U, Mult> operator*(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValRight<T, U, Mult>(A, val, Mult{});
}

template<ArrayBaseType T, RealType U>
BinExprValRight<T, U, Divide> operator/(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(val == U(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExprValRight<T, U, Divide>(A, val, Divide{});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
BinExprValLeft<T, U, Plus> operator+(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValLeft<T, U, Plus>(B, val, Plus{});
}

template<ArrayBaseType T, RealType U>
BinExprValLeft<T, U, Minus> operator-(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValLeft<T, U, Minus>(B, val, Minus{});
}

template<ArrayBaseType T, RealType U>
BinExprValLeft<T, U, Mult> operator*(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValLeft<T, U, Mult>(B, val, Mult{});
}

template<ArrayBaseType T, RealType U>
BinExprValLeft<T, U, Divide> operator/(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(does_contain_zero(B)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExprValLeft<T, U, Divide>(B, val, Divide{});
}

}

#endif
