// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <utility>

#include "strict_auxiliary.hpp"
#include "strict_array_ops.hpp"
#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_slicearray.hpp"
#include "strict_val.hpp"

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array : private Base1D
{
public:
   using size_type = strict_int;
   using value_type = StrictVal<T>;
   using real_type = T;

   // expr_type and slice_type inform expression templates and SliceArrays
   // about how the object should be stored.
   using expr_type = const Array<T> &;
   using slice_type = Array<T> &;

   explicit Array();

   template<IntegerType IntType>
      explicit Array(IntType size);

   template<IntegerType IntType>
      explicit Array(IntType size, value_type val);

   explicit Array(std::initializer_list<value_type> list);
   Array(const Array & A);
   Array(Array && a) noexcept;

   Array & operator=(value_type val) &;
   Array & operator=(std::initializer_list<value_type> list) &;
   Array & operator=(const Array & A) &;
   Array & operator=(Array && A) & noexcept;

   // construct and assign any one-dimensional type of the same real_type
   template<OneDimBaseType OneDimBaseT>
      Array(const OneDimBaseT & A);

   template<OneDimBaseType OneDimBaseT>
      Array & operator=(const OneDimBaseT & A) &;

   ~Array();

   Array & operator+=(value_type val) &;
   Array & operator-=(value_type val) &;
   Array & operator*=(value_type val) &;
   Array & operator/=(value_type val) &;

   template<OneDimBaseType OneDimBaseT>
      Array & operator+=(const OneDimBaseT & A) &;

   template<OneDimBaseType OneDimBaseT>
      Array & operator-=(const OneDimBaseT & A) &;

   template<OneDimBaseType OneDimBaseT>
      Array & operator*=(const OneDimBaseT & A) &;

   template<OneDimBaseType OneDimBaseT>
      Array & operator/=(const OneDimBaseT & A) &;

   void swap(Array & A) noexcept;

   template<IntegerType IntType>
      void resize(IntType size);

   void resize_and_assign(Array && A);

   template<OneDimBaseType OneDimBaseT>
      void resize_and_assign(const OneDimBaseT & A);

   template<IntegerType IntType>
      [[nodiscard]] inline value_type & operator[](IntType i);

   template<IntegerType IntType>
      [[nodiscard]] inline const value_type & operator[](IntType i) const;

   [[nodiscard]] inline value_type & operator[](internal::Last);
   [[nodiscard]] inline const value_type & operator[](internal::Last) const;

   STRICT_GENERATE_SLICES()

   [[nodiscard]] size_type size() const { ASSERT_STRICT_DEBUG(sz > -1); return sz; }
   [[nodiscard]] bool empty() const { ASSERT_STRICT_DEBUG(sz > -1); return sz == 0; }

   [[nodiscard]] value_type* data() & { return !empty() ? &elem[0] : nullptr; }
   [[nodiscard]] const value_type* data() const & { return !empty() ? &elem[0] : nullptr; }

   template<RealType U>
      [[nodiscard]] Array<U> convert_type() const; // conversion chosen by the user;

   STRICT_GENERATE_ITERATORS()

private:
   value_type* elem;
   size_type sz;

   template<typename F> void apply0(F f);
   template<OneDimBaseType OneDimBaseT, typename F>
      void apply1(const OneDimBaseT & A, F f);
};

template<IntegerType T>
[[nodiscard]] Array<T> random(Size size, Low<T> low = Low<T>{}, High<T> high = High<T>{T(1)});

template<FloatingType T>
[[nodiscard]] Array<T> random(Size size, Low<T> low = Low<T>{}, High<T> high = High<T>{T(1)});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// binary operations
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator+(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator-(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator*(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator/(const T1 & A, const T2 & B);

template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(StrictVal<U> val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(StrictVal<U> val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(StrictVal<U> val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(StrictVal<U> val, const T & B);

template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, StrictVal<U> val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, StrictVal<U> val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, StrictVal<U> val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, StrictVal<U> val);

template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(U val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(U val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(U val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(U val, const T & B);

template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, U val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, U val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, U val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, U val);

// unary operations
template<OneDimBaseType T> [[nodiscard]] auto operator+(const T & A);
template<OneDimBaseType T> [[nodiscard]] auto operator-(const T & A);
template<OneDimBaseType T> [[nodiscard]] auto abs(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p);
template<OneDimFloatingBaseType T, IntegerType IntT> [[nodiscard]] auto pow_int(const T & A, StrictVal<IntT> p);
template<OneDimFloatingBaseType T, IntegerType IntT> [[nodiscard]] auto pow_int(const T & A, IntT p);
template<OneDimFloatingBaseType T> [[nodiscard]] auto exp(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto log(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto sqrt(const T & A);

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto two_prod(const T1 & A, const T2 & B);

template<RealType T>
[[nodiscard]] auto e_unit(Index j, Size size);

template<RealType T>
[[nodiscard]] auto sequence(Size size, Start<T> start = Start<T>{}, Incr<T> incr = Incr<T>{T(1)});

template<RealType T>
[[nodiscard]] auto linspace(Size size, Start<T> start = Start<T>{}, End<T> end = End<T>{T(1)});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T>::Array() :
   elem{},
   sz{}
{}

template<RealType T>
template<IntegerType IntType>
Array<T>::Array(IntType size) :
   elem{new StrictVal<T>[static_cast<std::size_t>(size)]},
   sz{size}
{
   ASSERT_STRICT_DEBUG(sz > -1);
}

template<RealType T>
template<IntegerType IntType>
Array<T>::Array(IntType size, StrictVal<T> val) :
   Array(size)
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
}

template<RealType T>
Array<T>::Array(std::initializer_list<StrictVal<T>> list) :
   Array(static_cast<size_type>(list.size()))
{
   std::copy(list.begin(), list.end(), begin());
}

template<RealType T> Array<T>::Array(const Array<T> & A) :
   Array(A.size())
{
   std::copy(A.begin(), A.end(), begin());
}

template<RealType T>
Array<T>::Array(Array<T> && A) noexcept :
   elem{std::exchange(A.elem, {})},
   sz{std::exchange(A.sz, {})}
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T> & Array<T>::operator=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   std::fill(begin(), end(), val);
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(std::initializer_list<StrictVal<T>> list) &
{
   ASSERT_STRICT_DEBUG(size() == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(const Array<T> & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.sz);
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(Array<T> && A) & noexcept
{
   ASSERT_STRICT_DEBUG(sz == A.sz);
   Array<T> temp(std::move(A));
   swap(temp);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array<T>::Array(const OneDimBaseT & A) : Array(A.size())
{
   std::copy(A.begin(), A.end(), begin());
}

template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array<T> & Array<T>::operator=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::~Array()
{
   delete[] elem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T> & Array<T>::operator+=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator-=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator*=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator/=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array<T> & Array<T>::operator+=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array<T> & Array<T>::operator-=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array<T> & Array<T>::operator*=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array<T> & Array<T>::operator/=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
void Array<T>::swap(Array<T> & A) noexcept
{
   std::swap(elem, A.elem);
   std::swap(sz, A.sz);
}

template<RealType T>
template<IntegerType IntType>
void Array<T>::resize(IntType size)
{
   Array<T> temp(size);
   std::copy(begin(), begin() + std::min<strict_int>(sz, size), temp.begin());
   swap(temp);
}

template<RealType T>
void Array<T>::resize_and_assign(Array<T> && A)
{
   resize(A.size());
   *this = std::move(A);
}

template<RealType T>
template<OneDimBaseType OneDimBaseT>
void Array<T>::resize_and_assign(const OneDimBaseT & A)
{
   resize(A.size());
   *this = A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<IntegerType IntType>
[[nodiscard]] inline StrictVal<T> & Array<T>::operator[](IntType i)
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[i];
}

template<RealType T>
template<IntegerType IntType>
[[nodiscard]] inline const StrictVal<T> & Array<T>::operator[](IntType i) const
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[i];
}

template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array<T>::operator[](internal::Last)
{
   return elem[sz-1];
}

template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array<T>::operator[](internal::Last) const
{
   return elem[sz-1];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<RealType U>
[[nodiscard]] Array<U> Array<T>::convert_type() const
{
   Array<U> A(size());
   for(size_type i = 0; i < size(); ++i) {
      A[i] = strict_cast<U>((*this)[i]);
   }
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<typename F>
void Array<T>::apply0(F f)
{
   for(size_type i = 0; i < sz; ++i) {
      f(i);
   }
}

template<RealType T>
template<OneDimBaseType OneDimBaseT, typename F>
void Array<T>::apply1(const OneDimBaseT & A, F f)
{
   (void)A;
   for(size_type i = 0; i < sz; ++i) {
      f(i);
   }
}

template<IntegerType T>
[[nodiscard]] Array<T> random(Size size, Low<T> low, High<T> high)
{
   auto sz = size.get();
   auto l = low.get();
   auto h = high.get();
   ASSERT_STRICT_DEBUG(sz > 0);
   ASSERT_STRICT_DEBUG(h >= l);

   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(sz);
   long long int diff_range = real_cast<long long int>(h - l) + 1;
   for(auto & x : A) {
      x = l + T(std::rand() % diff_range);
   }
   return A;
}

template<FloatingType T>
[[nodiscard]] Array<T> random(Size size, Low<T> low, High<T> high)
{
   auto sz = size.get();
   auto l = low.get();
   auto h = high.get();
   ASSERT_STRICT_DEBUG(sz > 0);
   ASSERT_STRICT_DEBUG(h >= l);

   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(sz);
   for(auto & x : A) {
      x = l + (h - l) * T(std::rand()) / T(RAND_MAX);
   }
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UnaryPlus : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return +strict_val;
   }
};

struct UnaryMinus : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return -strict_val;
   }
};

struct UnaryAbs : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return abss(strict_val);
   }
};

template<RealType T>
struct UnaryPow : private UnaryOperation
{
   UnaryPow(StrictVal<T> p) : p{p} {}

   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return pows(strict_val, p);
   }
private:
   StrictVal<T> p;
};

template<IntegerType T>
struct UnaryPowInt : private UnaryOperation
{
   UnaryPowInt(StrictVal<T> p) : p{p} {}

   template<RealType U>
   StrictVal<U> operator()(StrictVal<U> strict_val) const {
      return pows_int(strict_val, p);
   }
private:
   StrictVal<T> p;
};

struct UnaryExp : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return exps(strict_val);
   }
};

struct UnaryLog : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return logs(strict_val);
   }
};

struct UnarySqrt : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return sqrts(strict_val);
   }
};

struct Plus : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left + right;
   }
};

struct Minus : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left - right;
   }
};

struct Mult : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left * right;
   }
};

struct Divide : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left / right;
   }
};

struct BinaryTwoProdFirst : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return two_prod(left, right).first;
   }
};

struct BinaryTwoProdSecond : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return two_prod(left, right).second;
   }
};

#define STRICT_GENERATE_USING_EXPR_TYPES(OneDimObjectType)            \
   using size_type = typename OneDimObjectType::size_type;            \
   using value_type = typename OneDimObjectType::value_type;          \
   using real_type = typename OneDimObjectType::real_type;            \

#define STRICT_GENERATE_EXPR_COPY_ASSIGN(StrictClassName)             \
   StrictClassName(const StrictClassName &) = default;                \
   StrictClassName & operator=(const StrictClassName &) = delete;     \

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT>
class StandardUnitVectorExpr : private Base1D
{
public:
   STRICT_GENERATE_CONST_ITERATORS()
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = StandardUnitVectorExpr<OneDimBaseT>;

   explicit StandardUnitVectorExpr(size_type j, size_type size) : j{j}, sz{size} {
      ASSERT_STRICT_DEBUG(j > -1); ASSERT_STRICT_DEBUG(size > j);
   }
   STRICT_GENERATE_EXPR_COPY_ASSIGN(StandardUnitVectorExpr)

   template<IntegerType IntType>
   [[nodiscard]] value_type operator[](IntType i) const {
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, i)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return j == i ? real_type{1} : real_type{0};
   }

   [[nodiscard]] size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return false; }

private:
   size_type j;
   size_type sz;
};

template<OneDimBaseType OneDimBaseT>
class SequenceExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = SequenceExpr<OneDimBaseT>;

   explicit SequenceExpr(value_type start, size_type size, value_type incr)
      : start{start}, sz{size}, incr{incr} {
      ASSERT_STRICT_DEBUG(size > 0);
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(SequenceExpr)

   template<IntegerType IntType>
   [[nodiscard]] value_type operator[](IntType i) const {
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, i)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return start + incr * static_cast<real_type>(i);
   }

   [[nodiscard]] value_type operator[](internal::Last) const {
      return start + incr * static_cast<real_type>(size()-1);
   }

   STRICT_GENERATE_CONST_SLICES()
   STRICT_GENERATE_CONST_ITERATORS()

   [[nodiscard]] size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return false; }

private:
   value_type start;
   size_type sz;
   value_type incr;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT, UnaryOperationType Op>
class UnaryExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = UnaryExpr<OneDimBaseT, Op>;

   explicit UnaryExpr(const OneDimBaseT & A, Op op) : A{A}, op{op} { ASSERT_STRICT_DEBUG(!A.empty()); }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(UnaryExpr)

   template<IntegerType IntType>
   [[nodiscard]] value_type operator[](IntType i) const {
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, i)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(A[i]);
   }

   [[nodiscard]] value_type operator[](internal::Last) const {
      return op(A[size()-1]);
   }

   STRICT_GENERATE_CONST_SLICES()
   STRICT_GENERATE_CONST_ITERATORS()

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

private:
   typename OneDimBaseT::expr_type A;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, BinaryOperationType Op>
class BinExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1)
   using expr_type = BinExpr<OneDimBaseT1, OneDimBaseT2, Op>;

   explicit BinExpr(const OneDimBaseT1 & A, const OneDimBaseT2 & B, Op op) : A{A}, B{B}, op{op} {
      static_assert(SameType<typename OneDimBaseT1::value_type, typename OneDimBaseT2::value_type>);
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(A.size() == B.size());
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(BinExpr)

   template<IntegerType IntType>
   [[nodiscard]] value_type operator[](IntType i) const {
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, i)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(A[i], B[i]);
   }

   [[nodiscard]] value_type operator[](internal::Last) const {
      return op(A[size()-1], B[size()-1]);
   }

   STRICT_GENERATE_CONST_SLICES()
   STRICT_GENERATE_CONST_ITERATORS()

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

private:
   typename OneDimBaseT1::expr_type A;
   typename OneDimBaseT2::expr_type B;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
class BinExprValLeft : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1)
   using expr_type = BinExprValLeft<OneDimBaseT1, T2, Op>;

   explicit BinExprValLeft(const OneDimBaseT1 & B, T2 val, Op op) : B{B}, val{val}, op{op} {
      static_assert(SameType<RealTypeOf<OneDimBaseT1>, T2>);
      ASSERT_STRICT_DEBUG(!B.empty());
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(BinExprValLeft)

   template<IntegerType IntType>
   [[nodiscard]] value_type operator[](IntType i) const {
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, i)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(val, B[i]);
   }

   [[nodiscard]] value_type operator[](internal::Last) const {
      return op(B[size()-1], val);
   }

   STRICT_GENERATE_CONST_SLICES()
   STRICT_GENERATE_CONST_ITERATORS()

   [[nodiscard]] size_type size() const { return B.size(); }
   [[nodiscard]] bool empty() const { return B.empty(); }

private:
   typename OneDimBaseT1::expr_type B;
   StrictVal<T2> val;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
class BinExprValRight : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1)
   using expr_type = BinExprValRight<OneDimBaseT1, T2, Op>;

   explicit BinExprValRight(const OneDimBaseT1 & A, T2 val, Op op) : A{A}, val{val}, op{op} {
      static_assert(SameType<RealTypeOf<OneDimBaseT1>, T2>);
      ASSERT_STRICT_DEBUG(!A.empty());
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(BinExprValRight)

   template<IntegerType IntType>
   [[nodiscard]] value_type operator[](IntType i) const {
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, i)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(A[i], val);
   }

   [[nodiscard]] value_type operator[](internal::Last) const {
      return op(A[size()-1], val);
   }

   STRICT_GENERATE_CONST_SLICES()
   STRICT_GENERATE_CONST_ITERATORS()

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

private:
   typename OneDimBaseT1::expr_type A;
   StrictVal<T2> val;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator+(const T1 & A, const T2 & B)
{
   return BinExpr{A, B, Plus{}};
}

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator-(const T1 & A, const T2 & B)
{
   return BinExpr{A, B, Minus{}};
}

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator*(const T1 & A, const T2 & B)
{
   return BinExpr{A, B, Mult{}};
}

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator/(const T1 & A, const T2 & B)
{
   return BinExpr{A, B, Divide{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(StrictVal<U> val, const T & B)
{
   return BinExprValLeft{B, U{val}, Plus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(StrictVal<U> val, const T & B)
{
   return BinExprValLeft{B, U{val}, Minus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(StrictVal<U> val, const T & B)
{
   return BinExprValLeft{B, U{val}, Mult{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(StrictVal<U> val, const T & B)
{
   return BinExprValLeft{B, U{val}, Divide{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(const T & A, StrictVal<U> val)
{
   return BinExprValRight{A, U{val}, Plus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(const T & A, StrictVal<U> val)
{
   return BinExprValRight{A, U{val}, Minus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(const T & A, StrictVal<U> val)
{
   return BinExprValRight{A, U{val}, Mult{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(const T & A, StrictVal<U> val)
{
   return BinExprValRight{A, U{val}, Divide{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(U val, const T & B)
{
   return BinExprValLeft{B, val, Plus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(U val, const T & B)
{
   return BinExprValLeft{B, val, Minus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(U val, const T & B)
{
   return BinExprValLeft{B, val, Mult{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(U val, const T & B)
{
   return BinExprValLeft{B, val, Divide{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(const T & A, U val)
{
   return BinExprValRight{A, val, Plus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(const T & A, U val)
{
   return BinExprValRight{A, val, Minus{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(const T & A, U val)
{
   return BinExprValRight{A, val, Mult{}};
}

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(const T & A, U val)
{
   return BinExprValRight{A, val, Divide{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T>
[[nodiscard]] auto operator+(const T & A)
{
   return UnaryExpr{A, UnaryPlus{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto operator-(const T & A)
{
   return UnaryExpr{A, UnaryMinus{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto abs(const T & A)
{
   return UnaryExpr{A, UnaryAbs{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p)
{
   return UnaryExpr{A, UnaryPow{p}};
}

template<OneDimFloatingBaseType T, IntegerType IntT>
[[nodiscard]] auto pow_int(const T & A, StrictVal<IntT> p)
{
   return UnaryExpr{A, UnaryPowInt<IntT>{p}};
}

template<OneDimFloatingBaseType T, IntegerType IntT>
[[nodiscard]] auto pow_int(const T & A, IntT p)
{
   return UnaryExpr{A, UnaryPowInt<IntT>{p}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto exp(const T & A)
{
   return UnaryExpr{A, UnaryExp{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto log(const T & A)
{
   return UnaryExpr{A, UnaryLog{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto sqrt(const T & A)
{
   return UnaryExpr{A, UnarySqrt{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto two_prod(const T1 & A, const T2 & B)
{
   return std::pair{BinExpr{A, B, BinaryTwoProdFirst{}}, BinExpr{A, B, BinaryTwoProdSecond{}}};
}

template<RealType T>
[[nodiscard]] auto e_unit(Index j, Size size)
{
   return StandardUnitVectorExpr<Array<T>> {j.get(), size.get()};
}

template<RealType T>
[[nodiscard]] auto sequence(Size size, Start<T> start, Incr<T> incr)
{
   return SequenceExpr<Array<T>> {start.get(), size.get(), incr.get()};
}

template<RealType T>
[[nodiscard]] auto linspace(Size size, Start<T> start, End<T> end)
{
   auto sz = size.get();
   ASSERT_STRICT_DEBUG(sz > 1);
   return SequenceExpr<Array<T>>
      {start.get(), sz, ( end.get() - start.get() )/strict_cast<T>(sz-1)};
}

}

#endif
