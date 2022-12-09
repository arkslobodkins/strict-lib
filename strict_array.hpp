#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

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

#define STRICT_ARRAY_THROW_ZERO_DIVISION()                                                    \
   do {                                                                                       \
   throw std::runtime_error{"ZERO DIVISION! " + trace_err(__FILE__, __func__, __LINE__)};     \
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
template<typename T> concept FloatingType = std::is_floating_point<T>::value || SameType<float128, T>;
#else
template<typename T> concept FloatingType = std::is_floating_point<T>::value;
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
template<ArrayBaseType T1, typename Op> class UnaryExpr;
template<ArrayBaseType T1, ArrayBaseType T2, OperationType Op> class BinExpr;
template<ArrayBaseType T1, RealType T2, OperationType Op> class BinExprValLeft;
template<ArrayBaseType T1, RealType T2, OperationType Op> class BinExprValRight;

#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
#define STRICT_ARRAY_QUADRUPLE_PRECISION
template<typename T> concept QuadArrayBaseType = std::is_same<float128, typename T::value_type>::value && ArrayBaseType<T>;
template<typename T> concept NotQuadArrayBaseType = !std::is_same<float128, typename T::value_type>::value && ArrayBaseType<T>;
#else
template<typename T> concept NotQuadArrayBaseType = ArrayBaseType<T>;
#endif

#if defined __AVX512F__ || defined __AVX512CD__ || defined __AVX512BW__ || defined __AVX512DQ__ || defined __AVX512VL__
constexpr inline std::size_t bytes_width() { return 64; }
#elif defined __AVX2__
constexpr inline std::size_t bytes_width() { return 32; }
#elif defined __AVX__
constexpr inline std::size_t bytes_width() { return 32; }
#else
constexpr inline std::size_t bytes_width() { return 16; }
#endif

template<RealType T>
struct IndexVal
{
public:
   explicit IndexVal(T & x) : x(x){}
   IndexVal(const IndexVal &) = delete;
   const IndexVal & operator=(const IndexVal & index_val) { x = index_val.x; return *this; }
   template<RealType U> const IndexVal & operator=(U val) { static_assert(SameType<T, U>); x = val; return *this; }
   template<RealType U> operator U () const { return x; }

   const IndexVal & operator++() { ++x; return *this; }
   const IndexVal & operator--() { --x; return *this; }
   T operator++(int) { T old = x; ++x; return old; }
   T operator--(int) { T old = x; --x; return old; }
   template<RealType U> const IndexVal & operator+=(U val) { static_assert(SameType<T, U>); x += val; return *this; }
   template<RealType U> const IndexVal & operator-=(U val) { static_assert(SameType<T, U>); x -= val; return *this; }
   template<RealType U> const IndexVal & operator*=(U val) { static_assert(SameType<T, U>); x *= val; return *this; }
   template<RealType U> const IndexVal & operator/=(U val) {
      #ifdef STRICT_ARRAY_DIVISION_ON
      if(val == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
      #endif
      static_assert(SameType<T, U>); x /= val; return *this;
   }
   const IndexVal & operator+=(const IndexVal & index_val) { x += index_val.x; return *this; }
   const IndexVal & operator-=(const IndexVal & index_val) { x -= index_val.x; return *this; }
   const IndexVal & operator*=(const IndexVal & index_val) { x *= index_val.x; return *this; }
   const IndexVal & operator/=(const IndexVal & index_val) {
      #ifdef STRICT_ARRAY_DIVISION_ON
      if(index_val.x == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
      #endif
      x /= index_val.x; return *this;
   }

   T & x;
};

template<RealType T, RealType U> auto operator+(const IndexVal<T> & v1, const IndexVal<U> & v2) { return v1.x + v2.x; }
template<RealType T, RealType U> auto operator-(const IndexVal<T> & v1, const IndexVal<U> & v2) { return v1.x - v2.x; }
template<RealType T, RealType U> auto operator*(const IndexVal<T> & v1, const IndexVal<U> & v2) { return v1.x * v2.x; }
template<RealType T, RealType U> auto operator/(const IndexVal<T> & v1, const IndexVal<U> & v2) { return v1.x / v2.x; }
template<RealType T, RealType U> auto operator+(const IndexVal<T> & v, U val) { return v.x + val; }
template<RealType T, RealType U> auto operator-(const IndexVal<T> & v, U val) { return v.x - val; }
template<RealType T, RealType U> auto operator*(const IndexVal<T> & v, U val) { return v.x * val; }
template<RealType T, RealType U> auto operator/(const IndexVal<T> & v, U val) { return v.x / val; }
template<RealType T, RealType U> auto operator+(T val, const IndexVal<U> & v) { return val + v.x; }
template<RealType T, RealType U> auto operator-(T val, const IndexVal<U> & v) { return val - v.x; }
template<RealType T, RealType U> auto operator*(T val, const IndexVal<U> & v) { return val * v.x; }
template<RealType T, RealType U> auto operator/(T val, const IndexVal<U> & v) { return val / v.x; }

template<RealType T, RealType U> bool operator==(const IndexVal<T> & v, U val) { return v.x == val; }
template<RealType T, RealType U> bool operator==(T val, const IndexVal<U> & v) { return v.x == val; }
template<RealType T, RealType U> bool operator==(const IndexVal<T> v1, const IndexVal<U> & v2) { return v1.x == v2.x; }
template<RealType T, RealType U> bool operator<(const IndexVal<T> & v, U val) { return v.x < val; }
template<RealType T, RealType U> bool operator<(T val, const IndexVal<U> & v) { return v.x < val; }
template<RealType T, RealType U> bool operator<(const IndexVal<T> v1, const IndexVal<U> & v2) { return v1.x < v2.x; }
template<RealType T, RealType U> bool operator<=(const IndexVal<T> & v, U val) { return v.x <= val; }
template<RealType T, RealType U> bool operator<=(T val, const IndexVal<U> & v) { return v.x <= val; }
template<RealType T, RealType U> bool operator<=(const IndexVal<T> v1, const IndexVal<U> & v2) { return v1.x <= v2.x; }
template<RealType T, RealType U> bool operator>(const IndexVal<T> & v, U val) { return v.x > val; }
template<RealType T, RealType U> bool operator>(T val, const IndexVal<U> & v) { return v.x > val; }
template<RealType T, RealType U> bool operator>(const IndexVal<T> v1, const IndexVal<U> & v2) { return v1.x > v2.x; }
template<RealType T, RealType U> bool operator>=(const IndexVal<T> & v, U val) { return v.x >= val; }
template<RealType T, RealType U> bool operator>=(T val, const IndexVal<U> & v) { return v.x >= val; }
template<RealType T, RealType U> bool operator>=(const IndexVal<T> v1, const IndexVal<U> & v2) { return v1.x >= v2.x; }
template<RealType T, RealType U> bool operator!=(const IndexVal<T> & v, U val) { return v.x != val; }
template<RealType T, RealType U> bool operator!=(T val, const IndexVal<U> & v) { return v.x != val; }
template<RealType T, RealType U> bool operator!=(const IndexVal<T> v1, const IndexVal<U> & v2) { return v1.x != v2.x; }

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
   template<RealType U> Array(std::initializer_list<U> list);
   Array(const Array & a);
   Array(Array && a);

   template<RealType U> const Array & operator=(U val);
   const Array & operator=(const Array & a);
   const Array & operator=(Array && a);

   template<ArrayExprType ArrExpr> Array(const ArrExpr & expr);
   template<ArrayExprType ArrExpr> const Array & operator=(const ArrExpr & expr);

   template<IntegerType S> Array & remove_element(S index);
   template<IntegerType S> Array & resize(S size);
   void resize_and_assign(const Array & a);

   ~Array();

   [[nodiscard]] inline size_type size() const;
   template<IntegerType S> inline IndexVal<T> operator[](S i);
   template<IntegerType S> inline const IndexVal<T> operator[](S i) const;
   template<IntegerType S> inline T & index(S i);
   template<IntegerType S> inline const T & index(S i) const;

   [[nodiscard]] const Array & operator+() const;
   [[nodiscard]] auto operator-() const; // returns expression template

   template<RealType U> const Array & operator+=(U val);
   template<RealType U> const Array & operator-=(U val);
   template<RealType U> const Array & operator*=(U val);
   template<RealType U> const Array & operator/=(U val);

   template<ArrayBaseType ArrayType> const Array & operator+=(const ArrayType & A);
   template<ArrayBaseType ArrayType> const Array & operator-=(const ArrayType & A);
   template<ArrayBaseType ArrayType> const Array & operator*=(const ArrayType & A);
   template<ArrayBaseType ArrayType> const Array & operator/=(const ArrayType & A);

   [[nodiscard]] bool empty() const { return sz == size_type(0) ? true : false; }
   [[nodiscard]] T* data() & { return sz ? elem : nullptr; }
   [[nodiscard]] T* begin() & { return sz ? elem : nullptr; }
   [[nodiscard]] T* end() & { return sz ? elem+sz : nullptr; }
   [[nodiscard]] const T* data() const & { return sz ? elem : nullptr; }
   [[nodiscard]] const T* begin() const & { return sz ? elem : nullptr; }
   [[nodiscard]] const T* end() const & { return sz ? elem+sz : nullptr; }

   template<IntegerType S1, IntegerType S2> [[nodiscard]] Array sub_array(S1 first, S2 last);
   template<IntegerType U1, IntegerType U2> void fill_random(U1 low, U2 high);
   template<FloatingType U1, FloatingType U2> void fill_random(U1 low, U2 high);

   void sort_increasing() &;
   void sort_decreasing() &;

   template<RealType U1, RealType U2> [[nodiscard]] std::vector<T*> within_range(U1 low, U2 high) &;
   template<RealType U1, RealType U2> [[nodiscard]] std::vector<const T*> within_range(U1 low, U2 high) const &;

private:
   size_type sz;
   T* array_restrict_ptr elem;

   inline bool is_valid_index(size_type index) const;
   template<typename F> void apply0(F f);
   template<ArrayBaseType ArrayType, typename F> void apply1(const ArrayType & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<NotQuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A);

template<IntegerType S, RealType T1, RealType T2>
[[nodiscard]] Array<T1> array_random(S size, T1 low, T2 high);

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
[[nodiscard]] auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto norm_inf(const ArrayType & A);

template<NotQuadArrayBaseType ArrayType>
[[nodiscard]] auto norm2(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool does_contain_zero(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool is_positive(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] bool is_nonnegative(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto abs(const ArrayType & A);  // returns expression template

template<ArrayBaseType ArrayType>
[[nodiscard]] auto sum(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto min(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto max(const ArrayType & A);

template<ArrayBaseType ArrayType>
[[nodiscard]] auto min_index(const ArrayType & A);  // returns std::pair<size_type, value_type>

template<ArrayBaseType ArrayType>
[[nodiscard]] auto max_index(const ArrayType & A);  // returns std::pair<size_type, value_type>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All arithmetic operators below return expression templates
template<ArrayBaseType T1, ArrayBaseType T2> auto operator+(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> auto operator-(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> auto operator*(const T1 & A, const T2 & B);
template<ArrayBaseType T1, ArrayBaseType T2> auto operator/(const T1 & A, const T2 & B);
template<ArrayBaseType T, RealType U> auto operator+(const T & A, U val);
template<ArrayBaseType T, RealType U> auto operator-(const T & A, U val);
template<ArrayBaseType T, RealType U> auto operator*(const T & A, U val);
template<ArrayBaseType T, RealType U> auto operator/(const T & A, U val);
template<ArrayBaseType T, RealType U> auto operator+(U val, const T & B);
template<ArrayBaseType T, RealType U> auto operator-(U val, const T & B);
template<ArrayBaseType T, RealType U> auto operator*(U val, const T & B);
template<ArrayBaseType T, RealType U> auto operator/(U val, const T & B);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
template<QuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A);

template<QuadArrayBaseType ArrayType>
auto norm2(const ArrayType & A);
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T>::Array() : sz{size_type(0)}, elem{nullptr}
{}

template<RealType T> template<IntegerType S>
Array<T>::Array(S size)
   : sz{size},
     elem{new (std::align_val_t(bytes_width())) T[size]{}}
{ static_assert(SameType<size_type, S>); }

template<RealType T> template<IntegerType S, RealType U>
Array<T>::Array(S size, U val)
   : sz{size},
     elem(new (std::align_val_t(bytes_width())) T[size])
{
   static_assert(SameType<size_type, S>);
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] = val; } );
}

template<RealType T> template<RealType U>
Array<T>::Array(std::initializer_list<U> list)
   : Array(static_cast<size_type>(list.size()))
{
   static_assert(SameType<T, U>);
   std::copy(list.begin(), list.end(), begin());
}

template<RealType T>
Array<T>::Array(const Array<T> & a)
   : sz{a.size()},
     elem{new (std::align_val_t(bytes_width())) T[a.size()]}
{ std::copy(a.begin(), a.end(), begin()); }

template<RealType T>
Array<T>::Array(Array && a) : sz{a.sz}, elem{a.elem}
{
   a.elem = nullptr;
   a.sz = size_type(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
const Array<T> & Array<T>::operator=(const U val)
{
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] = val; } );
   return *this;
}

template<RealType T>
const Array<T> & Array<T>::operator=(const Array<T> & a)
{
   if(this != &a) std::copy(a.begin(), a.end(), begin());
   return *this;
}

template<RealType T>
const Array<T> & Array<T>::operator=(Array<T> && a)
{
   if(this != &a) {
      ASSERT_STRICT_ARRAY_DEBUG(sz == a.sz);

      delete[] elem;
      elem = a.elem;
      a.elem = nullptr;
      a.sz = size_type(0);
   }

   return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayExprType ArrExpr>
Array<T>::Array(const ArrExpr & expr)
   : sz{expr.size()}, elem{new (std::align_val_t(bytes_width())) T[expr.size()]}
{
   static_assert(SameType<size_type, typename ArrExpr::size_type>);
   static_assert(SameType<T, typename ArrExpr::value_type>);
   apply1(expr, [&](size_type i) { elem[i] = expr[i]; });
}

template<RealType T> template<ArrayExprType ArrExpr>
const Array<T> & Array<T>::operator=(const ArrExpr & expr)
{
   static_assert(SameType<size_type, typename ArrExpr::size_type>);
   static_assert(SameType<T, typename ArrExpr::value_type>);
   apply1(expr, [&](size_type i) { elem[i] = expr[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<IntegerType S>
Array<T> & Array<T>::remove_element(S index)
{
   static_assert(SameType<size_type, S>);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   ASSERT_STRICT_ARRAY_DEBUG(is_valid_index(index));
   for(size_type i = index; i < sz-1; ++i)
      elem[i] = elem[i+1];
   --sz;
   return *this;
}

template<RealType T> template<IntegerType S>
Array<T> & Array<T>::resize(S size)
{
   static_assert(SameType<size_type, S>);
   if(size == sz) return *this;

   delete[] elem;
   ASSERT_STRICT_ARRAY_DEBUG(size > size_type(-1));
   elem = new (std::align_val_t(bytes_width())) T[size]{};
   sz = size;
   return *this;
}

template<RealType T>
void Array<T>::resize_and_assign(const Array<T> & a)
{
   resize(a.size());
   *this = a;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T>::~Array() { delete[] elem; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline auto Array<T>::size() const ->size_type { return sz; }

template<RealType T> template<IntegerType S>
inline IndexVal<T> Array<T>::operator[](S i)
{
   static_assert(SameType<size_type, S>);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!is_valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return IndexVal{elem[i]};
}

template<RealType T> template<IntegerType S>
inline const IndexVal<T> Array<T>::operator[](S i) const
{
   static_assert(SameType<size_type, S>);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!is_valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return IndexVal{elem[i]};
}

template<RealType T> template<IntegerType S>
inline T & Array<T>::index(S i)
{
   static_assert(SameType<size_type, S>);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!is_valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T> template<IntegerType S>
inline const T & Array<T>::index(S i) const
{
   static_assert(SameType<size_type, S>);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   #ifdef STRICT_ARRAY_DEBUG_ON
   if(!is_valid_index(i)) STRICT_ARRAY_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
const Array<T> & Array<T>::operator+() const
{
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   return *this;
}

template<RealType T>
auto Array<T>::operator-() const
{ return UnaryExpr(*this, [](T x){return -x;}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
const Array<T> & Array<T>::operator+=(const U val)
{
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}

template<RealType T> template<RealType U>
const Array<T> & Array<T>::operator-=(const U val)
{
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}

template<RealType T> template<RealType U>
const Array<T> & Array<T>::operator*=(const U val)
{
   static_assert(SameType<T, U>);
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}

template<RealType T> template<RealType U>
const Array<T> & Array<T>::operator/=(const U val)
{
   static_assert(SameType<T, U>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(val == T(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator+=(const ArrayType & A)
{
   static_assert(SameType<T, typename ArrayType::value_type>);
   apply1(A, [&](size_type i) { elem[i] += A.index(i); });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator-=(const ArrayType & A)
{
   static_assert(SameType<T, typename ArrayType::value_type>);
   apply1(A, [&](size_type i) { elem[i] -= A.index(i); });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator*=(const ArrayType & A)
{
   static_assert(SameType<T, typename ArrayType::value_type>);
   apply1(A, [&](size_type i) { elem[i] *= A.index(i); });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
const Array<T> & Array<T>::operator/=(const ArrayType & A)
{
   static_assert(SameType<T, typename ArrayType::value_type>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(does_contain_zero(A)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   apply1(A, [&](size_type i) { elem[i] /= A.index(i); });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<IntegerType S1, IntegerType S2>
Array<T> Array<T>::sub_array(S1 first, S2 last)
{
   static_assert(SameType<size_type, S1>);
   static_assert(SameType<size_type, S2>);
   ASSERT_STRICT_ARRAY_DEBUG(is_valid_index(first));
   ASSERT_STRICT_ARRAY_DEBUG(is_valid_index(last));
   ASSERT_STRICT_ARRAY_DEBUG(last >= first);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   size_type sub_sz = last - first + size_type(1);

   Array<T> sub_array(sub_sz);
   for(size_type i = size_type(0); i < sub_sz; ++i)
      sub_array.index(i) = elem[first+i];
   return sub_array;
}

template<RealType T> template<IntegerType U1, IntegerType U2>
void Array<T>::fill_random(U1 low, U2 high)
{
   static_assert(SameType<T, U1>);
   static_assert(SameType<U1, U2>);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());

   for(auto & x : *this)
      x = low + T(std::rand() % (T(1)+high-low));
}

template<RealType T> template<FloatingType U1, FloatingType U2>
void Array<T>::fill_random(U1 low, U2 high)
{
   static_assert(SameType<T, U1>);
   static_assert(SameType<U1, U2>);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());

   for(auto & x : *this)
      x = low + (high - low) * T(std::rand()) / T(RAND_MAX);
}

template<RealType T>
void Array<T>::sort_increasing() &
{
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   std::sort(begin(), end(), [](T a, T b) { return a < b; });
}

template<RealType T>
void Array<T>::sort_decreasing() &
{
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   std::sort(begin(), end(), [](T a, T b) { return a > b; });
}

template<RealType T> template<RealType U1, RealType U2>
std::vector<T*> Array<T>::within_range(U1 low, U2 high) &
{
   static_assert(SameType<T, U1>);
   static_assert(SameType<U1, U2>);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());

   std::vector<T*> x{};
   for(auto it = begin(); it != end(); ++it)
      if(*it >= low && *it <= high)
         x.push_back(it);
   return x;
}

template<RealType T> template<RealType U1, RealType U2>
std::vector<const T*> Array<T>::within_range(U1 low, U2 high) const &
{
   static_assert(SameType<T, U1>);
   static_assert(SameType<U1, U2>);
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);
   ASSERT_STRICT_ARRAY_DEBUG(!empty());

   std::vector<const T*> x{};
   for(auto it = begin(); it != end(); ++it)
      if(*it >= low && *it <= high)
         x.push_back(it);
   return x;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline bool Array<T>::is_valid_index(size_type index) const
{
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   if(index < size_type(0) || index > sz-size_type(1))
      return false;
   return true;
}

template<RealType T> template<typename F>
void Array<T>::apply0(F f)
{
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   for(size_type i = size_type(0); i < sz; ++i)
      f(i);
}

template<RealType T> template<ArrayBaseType ArrayType, typename F>
void Array<T>::apply1(const ArrayType & A, F f)
{
   (void)A;
   ASSERT_STRICT_ARRAY_DEBUG(!empty());
   ASSERT_STRICT_ARRAY_DEBUG(sz == A.size());
   for(size_type i = size_type(0); i < sz; ++i)
      f(i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType S, RealType T1, RealType T2>
Array<T1> array_random(S size, T1 low, T2 high)
{
   static_assert(SameType<typename Array<T1>::size_type, S>);
   static_assert(SameType<T1, T2>);
   ASSERT_STRICT_ARRAY_DEBUG(size > S(0));
   ASSERT_STRICT_ARRAY_DEBUG(high >= low);

   Array<T1> a(size);
   a.fill_random(low, high);
   return a;
}

template<NotQuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A)
{
   using T = typename ArrayType::value_type;
   using sz_T = typename ArrayType::size_type;

   int num_digits{};
   if(SameType<T, double>)           num_digits = std::numeric_limits<double>::digits10 + 1;
   else if(SameType<T, long double>) num_digits = std::numeric_limits<long double>::digits10 + 1;
   else if(SameType<T, float>)       num_digits = std::numeric_limits<float>::digits10 + 1;
   else                              num_digits = (int)std::cout.precision();

   for(sz_T i = sz_T(0); i < A.size(); ++i)
      os << std::setprecision(num_digits) << A.index(i) << std::endl;
   return os;
}

template<ArrayBaseType ArrayType1, ArrayBaseType ArrayType2>
auto dot_prod(const ArrayType1 & A1, const ArrayType2 & A2)
{
   static_assert(SameType<typename ArrayType1::size_type, typename ArrayType2::size_type>);
   static_assert(SameType<typename ArrayType1::value_type, typename ArrayType2::value_type>);
   ASSERT_STRICT_ARRAY_DEBUG(!A1.empty());
   ASSERT_STRICT_ARRAY_DEBUG(A1.size() == A2.size());

   using sz_T = typename ArrayType1::size_type;
   typename ArrayType1::value_type prod{};
   for(sz_T i = sz_T(0); i < A1.size(); ++i)
      prod += A1.index(i) * A2.index(i);
   return prod;
}

template<ArrayBaseType ArrayType>
auto norm_inf(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());
   auto real_abs = [](T x) { return x < T(0) ? -x : x; };

   T max_abs = real_abs(A.index(sz_T(0)));
   for(sz_T i = sz_T(1); i < A.size(); ++i) {
      T abs_i = real_abs(A.index(i));
      max_abs = abs_i > max_abs ? abs_i : max_abs;
   }
   return max_abs;
}

template<NotQuadArrayBaseType ArrayType>
auto norm2(const ArrayType & A)
{
   using sz_T [[maybe_unused]] = typename ArrayType::size_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());
   return std::sqrt(dot_prod(A, A));
}

template<ArrayBaseType ArrayType>
bool does_contain_zero(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   for(sz_T i = sz_T(0); i < A.size(); ++i)
      if(A.index(i) == typename ArrayType::value_type(0)) return true;
   return false;
}

template<ArrayBaseType ArrayType>
bool is_positive(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   for(sz_T i = sz_T(0); i < A.size(); ++i)
      if(A.index(i) <= typename ArrayType::value_type(0)) return false;
   return true;
}

template<ArrayBaseType ArrayType>
bool is_nonnegative(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   for(sz_T i = sz_T(0); i < A.size(); ++i)
      if(A.index(i) < typename ArrayType::value_type(0)) return false;
   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
auto abs(const ArrayType & A)
{
   using T = typename ArrayType::value_type;
   using sz_T [[maybe_unused]] = typename ArrayType::size_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());
   return UnaryExpr(A, [](T x){return x < T(0) ? -x : x;});
}

template<ArrayBaseType ArrayType>
auto sum(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   typename ArrayType::value_type s{};
   for(sz_T i = sz_T(0); i < A.size(); ++i)
      s += A.index(i);
   return s;
}

template<ArrayBaseType ArrayType>
auto min(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   T m = A.index(sz_T(0));
   for(sz_T i = sz_T(1); i < A.size(); ++i)
      if(A.index(i) < m) m = A.index(i);
   return m;
}

template<ArrayBaseType ArrayType>
auto max(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   T m = A.index(sz_T(0));
   for(sz_T i = sz_T(1); i < A.size(); ++i)
      if(A.index(i) > m) m = A.index(i);
   return m;
}

template<ArrayBaseType ArrayType>
auto min_index(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   std::pair<sz_T, T> min = {sz_T(0), A.index(sz_T(0))};
   for(sz_T i = sz_T(1); i < A.size(); ++i)
      if(A.index(i) < min.second)
         min = {i, A.index(i)};
   return min;
}

template<ArrayBaseType ArrayType>
auto max_index(const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   using T = typename ArrayType::value_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());

   std::pair<sz_T, T> max = {sz_T(0), A.index(sz_T(0))};
   for(sz_T i = sz_T(1); i < A.size(); ++i)
      if(A.index(i) > max.second)
         max = {i, A.index(i)};
   return max;
}

#if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
template<QuadArrayBaseType ArrayType>
std::ostream & operator<<(std::ostream & os, const ArrayType & A)
{
   using sz_T = typename ArrayType::size_type;
   int width = 46;
   char buf[128];
   for(sz_T i = sz_T(0); i < A.size(); ++i) {
      quadmath_snprintf (buf, sizeof(buf), "%+-#*.32Qe", width, A.index(i));
      os << buf << std::endl;
   }
   return os;
}

template<QuadArrayBaseType ArrayType>
auto norm2(const ArrayType & A)
{
   using sz_T [[maybe_unused]] = typename ArrayType::size_type;
   ASSERT_STRICT_ARRAY_DEBUG(!A.empty());
   return sqrtq(dot_prod(A, A));
}
#endif

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
// delete assignment operator and also copy constructors for all expression templates, since unnamed RVO is mandatory since C++17
template<ArrayBaseType T1, typename Op>
class UnaryExpr : private ArrayBase, private ArrayExpr
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   UnaryExpr(const T1 & a, Op op) : sz(a.size()), A(a), op(op)
   { ASSERT_STRICT_ARRAY_DEBUG(!a.empty()); }
   UnaryExpr(const UnaryExpr &) = delete;
   UnaryExpr & operator=(const UnaryExpr &) = delete;

   template<IntegerType S> value_type operator[](S i) const {
      static_assert(SameType<size_type, S>);
      return op(A.index(i));
   }
   template<IntegerType S> value_type index(S i) const {
      static_assert(SameType<size_type, S>);
      return op(A.index(i));
   }
   size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return sz == size_type(0) ? true : false; }

private:
   const size_type sz;
   const T1 & A;
   Op op;
};

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
      ASSERT_STRICT_ARRAY_DEBUG(!a.empty());
   }
   BinExpr(const BinExpr &) = delete;
   BinExpr & operator=(const BinExpr &) = delete;

   template<IntegerType S> value_type operator[](S i) const {
      static_assert(SameType<size_type, S>);
      return op(A.index(i), B.index(i));
   }
   template<IntegerType S> value_type index(S i) const {
      static_assert(SameType<size_type, S>);
      return op(A.index(i), B.index(i));
   }
   size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return sz == size_type(0) ? true : false; }

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
   {
      static_assert(SameType<typename T1::value_type, T2>);
      ASSERT_STRICT_ARRAY_DEBUG(!b.empty());
   }
   BinExprValLeft(const BinExprValLeft &) = delete;
   BinExprValLeft & operator=(const BinExprValLeft &) = delete;

   template<IntegerType S> value_type operator[](S i) const {
      static_assert(SameType<size_type, S>);
      return op(val, B.index(i));
   }
   template<IntegerType S> value_type index(S i) const {
      static_assert(SameType<size_type, S>);
      return op(val, B.index(i));
   }
   size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return sz == size_type(0) ? true : false; }

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
   {
      static_assert(SameType<typename T1::value_type, T2>);
      ASSERT_STRICT_ARRAY_DEBUG(!a.empty());
   }
   BinExprValRight(const BinExprValRight &) = delete;
   BinExprValRight & operator=(const BinExprValRight &) = delete;

   template<IntegerType S> value_type operator[](S i) const {
      static_assert(SameType<size_type, S>);
      return op(A.index(i), val);
   }
   template<IntegerType S> value_type index(S i) const {
      static_assert(SameType<size_type, S>);
      return op(A.index(i), val);
   }
   size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return sz == size_type(0) ? true : false; }

private:
   const size_type sz;
   const T1 & A;
   const T2 val;
   Op op;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T1, ArrayBaseType T2>
auto operator+(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   return BinExpr<T1, T2, Plus>(A, B, Plus{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator-(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   return BinExpr<T1, T2, Minus>(A, B, Minus{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator*(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   return BinExpr<T1, T2, Mult>(A, B, Mult{});
}

template<ArrayBaseType T1, ArrayBaseType T2>
auto operator/(const T1 & A, const T2 & B)
{
   static_assert(SameType<typename T1::value_type, typename T2::value_type>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(does_contain_zero(B)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExpr<T1, T2, Divide>(A, B, Divide{});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValRight<T, U, Plus>(A, val, Plus{});
}

template<ArrayBaseType T, RealType U>
auto operator-(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValRight<T, U, Minus>(A, val, Minus{});
}

template<ArrayBaseType T, RealType U>
auto operator*(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValRight<T, U, Mult>(A, val, Mult{});
}

template<ArrayBaseType T, RealType U>
auto operator/(const T & A, U val)
{
   static_assert(SameType<typename T::value_type, U>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(val == U(0)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExprValRight<T, U, Divide>(A, val, Divide{});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValLeft<T, U, Plus>(B, val, Plus{});
}

template<ArrayBaseType T, RealType U>
auto operator-(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValLeft<T, U, Minus>(B, val, Minus{});
}

template<ArrayBaseType T, RealType U>
auto operator*(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   return BinExprValLeft<T, U, Mult>(B, val, Mult{});
}

template<ArrayBaseType T, RealType U>
auto operator/(U val, const T & B)
{
   static_assert(SameType<typename T::value_type, U>);
   #ifdef STRICT_ARRAY_DIVISION_ON
   if(does_contain_zero(B)) STRICT_ARRAY_THROW_ZERO_DIVISION();
   #endif
   return BinExprValLeft<T, U, Divide>(B, val, Divide{});
}

}

#endif
