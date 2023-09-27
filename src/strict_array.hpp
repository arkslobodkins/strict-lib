// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <algorithm>         // min, fill, copy
#include <chrono>            // microseconds, duration_cast
#include <cstdlib>           // rand, srand, size_t
#include <ctime>             // time
#include <initializer_list>  // initializer_list
#include <utility>           // pair, move, swap, exchange
#include <vector>            // vector

#include "strict_auxiliary.hpp"
#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_slicearray.hpp"
#include "strict_val.hpp"

namespace strict {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array1D : private Base1D
{
public:
   using size_type = strict_int;
   using value_type = StrictVal<T>;
   using real_type = T;
   // expr_type and slice_type inform expression templates and SliceArrays
   // about how the object should be stored.
   using expr_type = const Array1D<T> &;
   using slice_type = Array1D<T> &;

   explicit Array1D();
   explicit Array1D(ImplicitInt size);
   explicit Array1D(ImplicitInt size, value_type val);
   template<LinearIteratorType L> explicit Array1D(L b, L e);

   Array1D(std::initializer_list<value_type> list);
   Array1D(const Array1D & A);
   Array1D(Array1D && a) noexcept;

   Array1D & operator=(value_type val) &;
   Array1D & operator=(std::initializer_list<value_type> list) &;
   Array1D & operator=(const Array1D & A) &;
   Array1D & operator=(Array1D && A) & noexcept;

   // construct and assign any one-dimensional type of the same real_type
   template<OneDimBaseType OneDimBaseT> Array1D(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> Array1D & operator=(const OneDimBaseT & A) &;

   ~Array1D();

   Array1D & operator+=(value_type val) &;
   Array1D & operator-=(value_type val) &;
   Array1D & operator*=(value_type val) &;
   Array1D & operator/=(value_type val) &;

   template<IntegerType U = T> Array1D & operator%=(value_type val) &;
   template<IntegerType U = T> Array1D & operator<<=(value_type val) &;
   template<IntegerType U = T> Array1D & operator>>=(value_type val) &;

   template<OneDimBaseType OneDimBaseT> Array1D & operator+=(const OneDimBaseT & A) &;
   template<OneDimBaseType OneDimBaseT> Array1D & operator-=(const OneDimBaseT & A) &;
   template<OneDimBaseType OneDimBaseT> Array1D & operator*=(const OneDimBaseT & A) &;
   template<OneDimBaseType OneDimBaseT> Array1D & operator/=(const OneDimBaseT & A) &;

   template<OneDimIntegerBaseType OneDimIntBaseT> Array1D & operator%=(const OneDimIntBaseT & A) &;
   template<OneDimIntegerBaseType OneDimIntBaseT> Array1D & operator<<=(const OneDimIntBaseT & A) &;
   template<OneDimIntegerBaseType OneDimIntBaseT> Array1D & operator>>=(const OneDimIntBaseT & A) &;

   void swap(Array1D & A) noexcept;
   void swap(Array1D && A) noexcept;

   void resize(ImplicitInt size);

   template<OneDimBaseType OneDimBaseT> void resize_and_assign(const OneDimBaseT & A);
   void resize_and_assign(Array1D && A); // optimized implementation

   void remove_element(ImplicitInt i);
   void remove_element(internal::Last);
   void remove_elements(const std::vector<ImplicitInt> & indexes);

   [[nodiscard]] inline value_type & operator[](ImplicitInt i);
   [[nodiscard]] inline const value_type & operator[](ImplicitInt i) const;
   [[nodiscard]] inline value_type & operator[](internal::Last);
   [[nodiscard]] inline const value_type & operator[](internal::Last) const;

   [[nodiscard]] inline size_type size() const noexcept;
   [[nodiscard]] inline bool empty() const noexcept;

   STRICT_GENERATE_SLICES1D()
   STRICT_GENERATE_ITERATORS()

private:
   value_type* elem;
   size_type sz;

   template<typename F> void apply0(F f);
   template<OneDimBaseType OneDimBaseT, typename F>
      void apply1([[maybe_unused]] const OneDimBaseT & A, F f);
};

template<IntegerType T>
[[nodiscard]] Array1D<T> random(Size size, Low<T> low = Low<T>{}, High<T> high = High<T>{T(1)});

template<FloatingType T>
[[nodiscard]] Array1D<T> random(Size size, Low<T> low = Low<T>{}, High<T> high = High<T>{T(1)});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// binary operations
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator+(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator-(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator*(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator/(const T1 & A, const T2 & B);

template<OneDimBaseType T> [[nodiscard]] auto operator+(ValueTypeOf<T> val, const T & B);
template<OneDimBaseType T> [[nodiscard]] auto operator-(ValueTypeOf<T> val, const T & B);
template<OneDimBaseType T> [[nodiscard]] auto operator*(ValueTypeOf<T> val, const T & B);
template<OneDimBaseType T> [[nodiscard]] auto operator/(ValueTypeOf<T> val, const T & B);

template<OneDimBaseType T> [[nodiscard]] auto operator+(const T & A, ValueTypeOf<T> val);
template<OneDimBaseType T> [[nodiscard]] auto operator-(const T & A, ValueTypeOf<T> val);
template<OneDimBaseType T> [[nodiscard]] auto operator*(const T & A, ValueTypeOf<T> val);
template<OneDimBaseType T> [[nodiscard]] auto operator/(const T & A, ValueTypeOf<T> val);

template<OneDimIntegerBaseType T1, OneDimIntegerBaseType T2> [[nodiscard]] auto operator%(const T1 & A, const T2 & B);
template<OneDimIntegerBaseType T1, OneDimIntegerBaseType T2> [[nodiscard]] auto operator<<(const T1 & A, const T2 & B);
template<OneDimIntegerBaseType T1, OneDimIntegerBaseType T2> [[nodiscard]] auto operator>>(const T1 & A, const T2 & B);

template<OneDimIntegerBaseType T> [[nodiscard]] auto operator%(ValueTypeOf<T> val, const T & B);
template<OneDimIntegerBaseType T> [[nodiscard]] auto operator<<(ValueTypeOf<T> val, const T & B);
template<OneDimIntegerBaseType T> [[nodiscard]] auto operator>>(ValueTypeOf<T> val, const T & B);

template<OneDimIntegerBaseType T> [[nodiscard]] auto operator%(const T & A, ValueTypeOf<T> val);
template<OneDimIntegerBaseType T> [[nodiscard]] auto operator<<(const T & A, ValueTypeOf<T> val);
template<OneDimIntegerBaseType T> [[nodiscard]] auto operator>>(const T & A, ValueTypeOf<T> val);

template<OneDimFloatingBaseType T1, OneDimFloatingBaseType T2>
[[nodiscard]] auto two_prod(const T1 & A, const T2 & B);

template<OneDimFloatingBaseType T1, IntegerBaseType T2>
[[nodiscard]] auto pow_prod(const T1 & A, const T2 & B);

// unary operations
template<OneDimBaseType T>
[[nodiscard]] auto operator+(const T & A);

template<OneDimBaseType T>
[[nodiscard]] auto operator-(const T & A);

template<OneDimBaseType T>
[[nodiscard]] auto abs(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto exp(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto log(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto log2(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto log10(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto sqrt(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto sin(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto cos(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto tan(const T & A);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow_int(const T & A, ImplicitInt p);

template<OneDimFloatingBaseType T>
[[nodiscard]] auto fast_pow_int(const T & A, ImplicitInt p);

template<OneDimBaseType T>
[[nodiscard]] auto reverse(const T & A);

// other operations
template<RealType T>
[[nodiscard]] auto const1D(Size size, CVal<T> c);

template<RealType T>
[[nodiscard]] auto e_unit(Index j, Size size);

template<RealType T>
[[nodiscard]] auto sequence(Size size, Start<T> start = Start<T>{}, Incr<T> incr = Incr<T>{T(1)});

template<FloatingType T>
[[nodiscard]] auto linspace(Size size, Start<T> start = Start<T>{}, End<T> end = End<T>{T(1)});

[[nodiscard]] auto range(ImplicitInt sz);

template<RealType T>
[[nodiscard]] auto index_range(const Array1D<T> & A);

template<RealType T, OneDimBaseType OneDimBaseT>
[[nodiscard]] auto convert_type(const OneDimBaseT & A); // expression template that converts to a different type

template<OneDimBaseType T1, OneDimBaseType... T2>
[[nodiscard]] auto merge(const T1 & first, const T2 &... all);

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto merge(const T1 & first, const T2 & last);

template<OneDimBaseType T1>
[[nodiscard]] auto view_expr(const T1 & A);

template<OneDimBaseType T1>
[[nodiscard]] auto without_ind(const T1 & A, const std::vector<ImplicitInt> & indexes);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array1D<T>::Array1D() :
   elem{},
   sz{}
{}


template<RealType T>
Array1D<T>::Array1D(ImplicitInt size) :
   elem{new StrictVal<T>[static_cast<std::size_t>(size.get())]},
   sz{size.get()}
{
   ASSERT_STRICT_DEBUG(sz > -1);
}


template<RealType T>
Array1D<T>::Array1D(ImplicitInt size, StrictVal<T> val) :
   Array1D(size)
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
}


template<RealType T>
template<LinearIteratorType L>
Array1D<T>::Array1D(L b, L e) : Array1D(abss<strict_int>(e-b))
{
   ASSERT_STRICT_DEBUG(e >= b);
   for(auto it = this->begin(); b != e; ++b, ++it)
      *it = *b;
}


template<RealType T>
Array1D<T>::Array1D(std::initializer_list<StrictVal<T>> list) :
   Array1D(static_cast<size_type>(list.size()))
{
   std::copy(list.begin(), list.end(), begin());
}


template<RealType T> Array1D<T>::Array1D(const Array1D<T> & A) :
   Array1D(A.size())
{
   std::copy(A.begin(), A.end(), begin());
}


template<RealType T>
Array1D<T>::Array1D(Array1D<T> && A) noexcept :
   elem{std::exchange(A.elem, {})},
   sz{std::exchange(A.sz, {})}
{}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array1D<T> & Array1D<T>::operator=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   std::fill(begin(), end(), val);
   return *this;
}


template<RealType T>
Array1D<T> & Array1D<T>::operator=(std::initializer_list<StrictVal<T>> list) &
{
   ASSERT_STRICT_DEBUG(size() == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}


template<RealType T>
Array1D<T> & Array1D<T>::operator=(const Array1D<T> & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.sz);
   std::copy(A.begin(), A.end(), begin());
   return *this;
}


template<RealType T>
Array1D<T> & Array1D<T>::operator=(Array1D<T> && A) & noexcept
{
   ASSERT_STRICT_DEBUG(sz == A.sz);
   if(this != &A) {
      delete[] elem;
      elem = A.elem;

      A.elem = {};
      A.sz = {};
   }
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array1D<T>::Array1D(const OneDimBaseT & A) : Array1D(A.size())
{
   std::copy(A.begin(), A.end(), begin());
}


template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array1D<T> & Array1D<T>::operator=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array1D<T>::~Array1D()
{
   delete[] elem;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array1D<T> & Array1D<T>::operator+=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}


template<RealType T>
Array1D<T> & Array1D<T>::operator-=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}


template<RealType T>
Array1D<T> & Array1D<T>::operator*=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}


template<RealType T>
Array1D<T> & Array1D<T>::operator/=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}


// note: U = T by default
template<RealType T>
template<IntegerType U>
Array1D<T> & Array1D<T>::operator%=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>); // static_assert in case T is not IntegerType and
                                  // one attempts to break the type system
   apply0([&](size_type i) { elem[i] %= val; } );
   return *this;
}

template<RealType T>
template<IntegerType U>
Array1D<T> & Array1D<T>::operator<<=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   apply0([&](size_type i) { elem[i] <<= val; } );
   return *this;
}


template<RealType T>
template<IntegerType U>
Array1D<T> & Array1D<T>::operator>>=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   apply0([&](size_type i) { elem[i] >>= val; } );
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array1D<T> & Array1D<T>::operator+=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}


template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array1D<T> & Array1D<T>::operator-=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}


template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array1D<T> & Array1D<T>::operator*=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}


template<RealType T>
template<OneDimBaseType OneDimBaseT>
Array1D<T> & Array1D<T>::operator/=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}


template<RealType T>
template<OneDimIntegerBaseType OneDimIntBaseT>
Array1D<T> & Array1D<T>::operator%=(const OneDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] %= A[i]; });
   return *this;
}


template<RealType T>
template<OneDimIntegerBaseType OneDimIntBaseT>
Array1D<T> & Array1D<T>::operator<<=(const OneDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] <<= A[i]; });
   return *this;
}


template<RealType T>
template<OneDimIntegerBaseType OneDimIntBaseT>
Array1D<T> & Array1D<T>::operator>>=(const OneDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] >>= A[i]; });
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
void Array1D<T>::swap(Array1D<T> & A) noexcept
{
   std::swap(elem, A.elem);
   std::swap(sz, A.sz);
}


template<RealType T>
void Array1D<T>::swap(Array1D<T> && A) noexcept
{
   this->swap(A);
}


// implements strong exception guarantee
// preserves values of the remaining elements
template<RealType T>
void Array1D<T>::resize(ImplicitInt size)
{
   ASSERT_STRICT_DEBUG(size.get() > -1);
   size_type size_new = size.get();
   if(size_new != sz) {
      Array1D<T> temp(size_new);
      std::copy(begin(), begin() + std::min<size_type>(sz, size_new), temp.begin());
      this->swap(temp);
   }
}


// strong guarantee is not necessary, assignment should never throw
template<RealType T>
template<OneDimBaseType OneDimBaseT>
void Array1D<T>::resize_and_assign(const OneDimBaseT & A)
{
   resize(A.size());
   *this = A;
}


// strong guarantee is not necessary, assignment should never throw
template<RealType T>
void Array1D<T>::resize_and_assign(Array1D<T> && A)
{
   resize(A.size());
   *this = std::move(A);
}


// implements strong exception guarantee
template<RealType T>
void Array1D<T>::remove_element(ImplicitInt i)
{
   ASSERT_STRICT_DEBUG(!empty());
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, i.get()));

   Array1D<T> temp(size()-1);
   size_type ind = i.get();

   for(size_type j = 0; j < ind; ++j) {
      temp[j] = (*this)[j];
   }
   for(size_type j = ind; j < size()-1; ++j) {
      temp[j] = (*this)[j+1];
   }

   this->swap(temp);
}


// implements strong exception guarantee
template<RealType T>
void Array1D<T>::remove_element(internal::Last last)
{
   this->remove_element(size()-1-last.get());
}


// implements strong exception guarantee
template<RealType T>
void Array1D<T>::remove_elements(const std::vector<ImplicitInt> & indexes)
{
   // primarily for optimization
   if(indexes.empty()) {
      return;
   }

   auto cmpl = without_ind(*this, indexes);
   Array1D<T> temp = cmpl;
   this->swap(temp);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array1D<T>::operator[](ImplicitInt i)
{
   ASSERT_STRICT_DEBUG(!empty());
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i.get())) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[i.get()];
}


template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array1D<T>::operator[](ImplicitInt i) const
{
   ASSERT_STRICT_DEBUG(!empty());
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i.get())) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[i.get()];
}


template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array1D<T>::operator[](internal::Last last)
{
   return operator[](sz-1-last.get());
}


template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array1D<T>::operator[](internal::Last last) const
{
   return operator[](sz-1-last.get());
}


template<RealType T>
[[nodiscard]] inline auto Array1D<T>::size() const noexcept -> size_type
{
   ASSERT_STRICT_DEBUG(sz > -1);
   return sz;
}


template<RealType T>
[[nodiscard]] inline bool Array1D<T>::empty() const noexcept
{
   ASSERT_STRICT_DEBUG(sz > -1);
   return sz == 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<typename F>
void Array1D<T>::apply0(F f)
{
   for(size_type i = 0; i < sz; ++i) {
      f(i);
   }
}


template<RealType T>
template<OneDimBaseType OneDimBaseT, typename F>
void Array1D<T>::apply1([[maybe_unused]] const OneDimBaseT & A, F f)
{
   for(size_type i = 0; i < sz; ++i) {
      f(i);
   }
}


template<IntegerType T>
[[nodiscard]] Array1D<T> random(Size size, Low<T> low, High<T> high)
{
   auto sz = size.get();
   auto l = low.get();
   auto h = high.get();
   ASSERT_STRICT_DEBUG(h >= l);

   using namespace std::chrono;
   microseconds ms = duration_cast<microseconds>
      (system_clock::now().time_since_epoch());
   std::srand(static_cast<unsigned>(std::time(0)) + static_cast<unsigned>(ms.count()));

   Array1D<T> A(sz);
   long long int diff_range = real_cast<long long int>(h - l) + 1;
   for(auto & x : A) {
      x = l + T(std::rand() % diff_range);
   }
   return A;
}


template<FloatingType T>
[[nodiscard]] Array1D<T> random(Size size, Low<T> low, High<T> high)
{
   auto sz = size.get();
   auto l = low.get();
   auto h = high.get();
   ASSERT_STRICT_DEBUG(h >= l);

   using namespace std::chrono;
   microseconds ms = duration_cast<microseconds>
      (system_clock::now().time_since_epoch());
   std::srand(static_cast<unsigned>(std::time(0)) + static_cast<unsigned>(ms.count()));

   Array1D<T> A(sz);
   for(auto & x : A) {
      x = l + (h - l) * T(std::rand()) / T(RAND_MAX);
   }
   return A;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UnaryCopy : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return strict_val;
   }
};


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


template<FloatingType T>
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
struct UnaryFastPowInt : private UnaryOperation
{
   UnaryFastPowInt(T p) : p{p} {}

   template<FloatingType U>
   StrictVal<U> operator()(StrictVal<U> strict_val) const {
      return fast_pows_int(strict_val, p);
   }
private:
   StrictVal<T> p;
};


struct UnaryExp : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return exps(strict_val);
   }
};


struct UnaryLog : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return logs(strict_val);
   }
};


struct UnaryLog2 : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return log2s(strict_val);
   }
};


struct UnaryLog10 : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return log10s(strict_val);
   }
};


struct UnarySqrt : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return sqrts(strict_val);
   }
};


struct UnarySin : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return sins(strict_val);
   }
};


struct UnaryCos : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return coss(strict_val);
   }
};


struct UnaryTan : private UnaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return tans(strict_val);
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


struct Modulo : private BinaryOperation
{
   template<IntegerType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left % right;
   }
};


struct LeftShift : private BinaryOperation
{
   template<IntegerType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left << right;
   }
};


struct RightShift : private BinaryOperation
{
   template<IntegerType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left >> right;
   }
};


struct BinaryTwoProdFirst : private BinaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return two_prods(left, right).first;
   }
};


struct BinaryTwoProdSecond : private BinaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return two_prods(left, right).second;
   }
};


struct BinaryPowProdFirst : private BinaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> left, ImplicitInt right) const {
      return std::get<0>(pow_prods(left, right.get()));
   }
};


struct BinaryPowProdSecond : private BinaryOperation
{
   template<FloatingType T>
   StrictVal<T> operator()(StrictVal<T> left, ImplicitInt right) const {
      return std::get<1>(pow_prods(left, right.get()));
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
// Generates standard unit vector on the fly.
// Involves redundant work if assigned to Arrays that only contain zeros.

// empty objects are not allowed(must be of at least size 1)
template<OneDimBaseType OneDimBaseT>
class StandardUnitVectorExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = StandardUnitVectorExpr<OneDimBaseT>;
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit StandardUnitVectorExpr(size_type j, size_type size)
      : j{j}, sz{size} {
         ASSERT_STRICT_DEBUG(j > -1);
         ASSERT_STRICT_DEBUG(size > j);
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(StandardUnitVectorExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return j == ind ? real_type(1) : real_type(0);
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return sz;
   }

   [[nodiscard]] bool empty() const noexcept {
      return false;
   }

private:
   size_type j;
   size_type sz;
};


// sequences make sure that type of OneDimBaseType
// is Array<T>. Although it is somewhat redundant,
// it conveniently and correctly generates
// size_type, value_type, etc.

template<OneDimBaseType OneDimBaseT>
class SequenceExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = SequenceExpr<OneDimBaseT>;
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit SequenceExpr(value_type start, size_type size, value_type incr)
      : start{start}, sz{size}, incr{incr} {
         ASSERT_STRICT_DEBUG(sz > -1);
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(SequenceExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return start + incr * static_cast<real_type>(ind);
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return sz;
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

private:
   value_type start;
   size_type sz;
   value_type incr;
};


class IndexRangeExpr : private Base1D
{
public:
   using size_type = strict_int;
   using value_type = StrictVal<strict_int>;
   using real_type = strict_int;
   using expr_type = IndexRangeExpr;

   // slices are intentionally not generated
   STRICT_GENERATE_CONST_ITERATORS()

   explicit IndexRangeExpr(size_type size) : sz{size} {
      ASSERT_STRICT_DEBUG(sz > -1);
   }

   // primarily intended to be used inside range-based for loop, disable
   // copy and assignment
   IndexRangeExpr(const IndexRangeExpr &) = delete;
   IndexRangeExpr & operator=(const IndexRangeExpr &) = delete;

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return ind;
   }

   [[nodiscard]] size_type size() const noexcept {
      return sz;
   }

private:
   size_type sz;
};


template<OneDimBaseType OneDimBaseT, RealType T>
class ConvertExpr : private Base1D
{
public:
   // STRICT_GENERATE_USING_EXPR_TYPES is not used
   // here since value_type and real_type are different
   using size_type = typename OneDimBaseT::size_type;
   using value_type = StrictVal<T>;
   using real_type = T;
   using expr_type = ConvertExpr<OneDimBaseT, T>;

   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   // empty object is allowed
   explicit ConvertExpr(const OneDimBaseT & A) : A{A}
   {}

   STRICT_GENERATE_EXPR_COPY_ASSIGN(ConvertExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return strict_cast<T>(A[ind]);
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return A.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

private:
   typename OneDimBaseT::expr_type A;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT>
class WithoutExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = WithoutExpr<OneDimBaseT>;
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit WithoutExpr(const OneDimBaseT & A, const std::vector<ImplicitInt> & v) : A{A}, v{internal::complement_set(v, A.size())}
   {}

   STRICT_GENERATE_EXPR_COPY_ASSIGN(WithoutExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return A[v[i.get()]];
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return v.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

private:
   typename OneDimBaseT::expr_type A;
   std::vector<ImplicitInt> v;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT, UnaryOperationType Op>
class UnaryExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = UnaryExpr<OneDimBaseT, Op>;
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit UnaryExpr(const OneDimBaseT & A, Op op) : A{A}, op{op}
   {}

   STRICT_GENERATE_EXPR_COPY_ASSIGN(UnaryExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(A[ind]);
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return A.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

private:
   typename OneDimBaseT::expr_type A;
   Op op;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT>
class ReverseExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT)
   using expr_type = ReverseExpr<OneDimBaseT>;
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit ReverseExpr(const OneDimBaseT & A) : A{A}
   {}

   STRICT_GENERATE_EXPR_COPY_ASSIGN(ReverseExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return A[size()-1-ind];
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return A.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

private:
   typename OneDimBaseT::expr_type A;
};


template<RealType T>
class ConstExpr : private Base1D
{
public:
   using size_type = strict_int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using expr_type = ConstExpr<T>;

   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit ConstExpr(ImplicitInt sz, StrictVal<T> c) : sz{sz.get()}, c{c} {
      ASSERT_STRICT_DEBUG(sz.get() > -1);
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(ConstExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return c;
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return sz;
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

private:
   StrictVal<T> c;
   strict_int sz;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
class MergeExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1)
   using expr_type = MergeExpr<OneDimBaseT1, OneDimBaseT2>;
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit MergeExpr(const OneDimBaseT1 & A, const OneDimBaseT2 & B) : A{A}, B{B} {
      static_assert(SameType<typename OneDimBaseT1::value_type, typename OneDimBaseT2::value_type>);
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(MergeExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return ind < A.size() ? A[ind] : B[ind - A.size()];
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return A.size() + B.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return A.empty() && B.empty();
   }

private:
   typename OneDimBaseT1::expr_type A;
   typename OneDimBaseT2::expr_type B;
};


// OneDimBase types of different types are allowed
// (e.x. raising T1 to the integer powers of T2)
template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, BinaryOperationType Op>
class BinExpr : private Base1D
{
public:
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1)
   using expr_type = BinExpr<OneDimBaseT1, OneDimBaseT2, Op>;
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit BinExpr(const OneDimBaseT1 & A, const OneDimBaseT2 & B, Op op)
      : A{A}, B{B}, op{op} {
         ASSERT_STRICT_DEBUG(A.size() == B.size());
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(BinExpr)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(A[ind], B[ind]);
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return A.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

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
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit BinExprValLeft(const OneDimBaseT1 & B, T2 val, Op op)
      : B{B}, val{val}, op{op} {
         static_assert(SameType<RealTypeOf<OneDimBaseT1>, T2>);
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(BinExprValLeft)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(val, B[ind]);
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return B.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

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
   STRICT_GENERATE_CONST_SLICES1D()
   STRICT_GENERATE_CONST_ITERATORS()

   explicit BinExprValRight(const OneDimBaseT1 & A, T2 val, Op op)
      : A{A}, val{val}, op{op} {
         static_assert(SameType<RealTypeOf<OneDimBaseT1>, T2>);
   }

   STRICT_GENERATE_EXPR_COPY_ASSIGN(BinExprValRight)

   [[nodiscard]] value_type operator[](ImplicitInt i) const {
      auto ind = i.get();
      #ifndef STRICT_DEBUG_OFF
      if(!internal::valid_index(*this, ind)) {
         STRICT_THROW_OUT_OF_RANGE();
      }
      #endif
      return op(A[ind], val);
   }

   [[nodiscard]] value_type operator[](internal::Last last) const {
      return operator[](size()-1-last.get());
   }

   [[nodiscard]] size_type size() const noexcept {
      return A.size();
   }

   [[nodiscard]] bool empty() const noexcept {
      return !size();
   }

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
template<OneDimBaseType T>
[[nodiscard]] auto operator+(ValueTypeOf<T> val, const T & B)
{
   return BinExprValLeft{B, RealTypeOf<T>{val}, Plus{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto operator-(ValueTypeOf<T> val, const T & B)
{
   return BinExprValLeft{B, RealTypeOf<T>{val}, Minus{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto operator*(ValueTypeOf<T> val, const T & B)
{
   return BinExprValLeft{B, RealTypeOf<T>{val}, Mult{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto operator/(ValueTypeOf<T> val, const T & B)
{
   return BinExprValLeft{B, RealTypeOf<T>{val}, Divide{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T>
[[nodiscard]] auto operator+(const T & A, ValueTypeOf<T> val)
{
   return BinExprValRight{A, RealTypeOf<T>{val}, Plus{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto operator-(const T & A, ValueTypeOf<T> val)
{
   return BinExprValRight{A, RealTypeOf<T>{val}, Minus{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto operator*(const T & A, ValueTypeOf<T> val)
{
   return BinExprValRight{A, RealTypeOf<T>{val}, Mult{}};
}

template<OneDimBaseType T>
[[nodiscard]] auto operator/(const T & A, ValueTypeOf<T> val)
{
   return BinExprValRight{A, RealTypeOf<T>{val}, Divide{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimIntegerBaseType T1, OneDimIntegerBaseType T2>
[[nodiscard]] auto operator%(const T1 & A, const T2 & B)
{
   return BinExpr{A, B, Modulo{}};
}

template<OneDimIntegerBaseType T1, OneDimIntegerBaseType T2>
[[nodiscard]] auto operator<<(const T1 & A, const T2 & B)
{
   return BinExpr{A, B, LeftShift{}};
}

template<OneDimIntegerBaseType T1, OneDimIntegerBaseType T2>
[[nodiscard]] auto operator>>(const T1 & A, const T2 & B)
{
   return BinExpr{A, B, RightShift{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimIntegerBaseType T>
[[nodiscard]] auto operator%(ValueTypeOf<T> val, const T & B)
{
   return BinExprValLeft{B, RealTypeOf<T>{val}, Modulo{}};
}

template<OneDimIntegerBaseType T>
[[nodiscard]] auto operator<<(ValueTypeOf<T> val, const T & B)
{
   return BinExprValLeft{B, RealTypeOf<T>{val}, LeftShift{}};
}

template<OneDimIntegerBaseType T>
[[nodiscard]] auto operator>>(ValueTypeOf<T> val, const T & B)
{
   return BinExprValLeft{B, RealTypeOf<T>{val}, RightShift{}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimIntegerBaseType T>
[[nodiscard]] auto operator%(const T & A, ValueTypeOf<T> val)
{
   return BinExprValRight{A, RealTypeOf<T>{val}, Modulo{}};
}

template<OneDimIntegerBaseType T>
[[nodiscard]] auto operator<<(const T & A, ValueTypeOf<T> val)
{
   return BinExprValRight{A, RealTypeOf<T>{val}, LeftShift{}};
}

template<OneDimIntegerBaseType T>
[[nodiscard]] auto operator>>(const T & A, ValueTypeOf<T> val)
{
   return BinExprValRight{A, RealTypeOf<T>{val}, RightShift{}};
}

template<OneDimFloatingBaseType T1, OneDimFloatingBaseType T2>
[[nodiscard]] auto two_prod(const T1 & A, const T2 & B)
{
   return std::pair{BinExpr{A, B, BinaryTwoProdFirst{}},
      BinExpr{A, B, BinaryTwoProdSecond{}}};
}

template<OneDimFloatingBaseType T1, IntegerBaseType T2>
[[nodiscard]] auto pow_prod(const T1 & A, const T2 & B)
{
   return std::pair{BinExpr{A, B, BinaryPowProdFirst{}},
      BinExpr{A, B, BinaryPowProdSecond{}}};
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
[[nodiscard]] auto log2(const T & A)
{
   return UnaryExpr{A, UnaryLog2{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto log10(const T & A)
{
   return UnaryExpr{A, UnaryLog10{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto sqrt(const T & A)
{
   return UnaryExpr{A, UnarySqrt{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto sin(const T & A)
{
   return UnaryExpr{A, UnarySin{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto cos(const T & A)
{
   return UnaryExpr{A, UnaryCos{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto tan(const T & A)
{
   return UnaryExpr{A, UnaryTan{}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p)
{
   return UnaryExpr{A, UnaryPow{p}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow_int(const T & A, ImplicitInt p)
{
   return UnaryExpr{A, UnaryPow{strict_cast<RealTypeOf<T>>(p.get())}};
}

template<OneDimFloatingBaseType T>
[[nodiscard]] auto fast_pow_int(const T & A, ImplicitInt p)
{
   return UnaryExpr{A, UnaryFastPowInt{p.get()}};
}

template<OneDimBaseType T>
[[nodiscard]] auto reverse(const T & A)
{
   return ReverseExpr{A};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] auto const1D(Size size, CVal<T> c)
{
   return ConstExpr{size.get(), c.get()};
}

// StandardUnitVectorExpr cannot be empty
template<RealType T>
[[nodiscard]] auto e_unit(Index j, Size size)
{
   return StandardUnitVectorExpr<Array1D<T>>{j.get(), size.get()};
}

template<RealType T>
[[nodiscard]] auto sequence(Size size, Start<T> start, Incr<T> incr)
{
   return SequenceExpr<Array1D<T>>
      {start.get(), size.get(), incr.get()};
}

template<FloatingType T>
[[nodiscard]] auto linspace(Size size, Start<T> start, End<T> end)
{
   ASSERT_STRICT_DEBUG(size.get() > 0);
   auto sz = size.get();
   return SequenceExpr<Array1D<T>>
      {start.get(), sz, ( end.get() - start.get() )/strict_cast<T>(sz-1)};
}

[[nodiscard]] auto range(ImplicitInt sz)
{
   return IndexRangeExpr{sz.get()};
}

template<RealType T>
[[nodiscard]] auto index_range(const Array1D<T> & A)
{
   return IndexRangeExpr{A.size()};
}

template<RealType T, OneDimBaseType OneDimBaseT>
[[nodiscard]] auto convert_type(const OneDimBaseT & A)
{
   return ConvertExpr<OneDimBaseT, T>{A};
}

template<OneDimBaseType T1, OneDimBaseType... T2>
[[nodiscard]] auto merge(const T1 & first, const T2 &... all)
{
   return MergeExpr{first, merge(all...)};
}

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto merge(const T1 & first, const T2 & last)
{
   return MergeExpr{first, last};
}

template<OneDimBaseType T1>
[[nodiscard]] auto view_expr(const T1 & A)
{
   return UnaryExpr{A, UnaryCopy{}};
}

template<OneDimBaseType T1>
[[nodiscard]] auto without_ind(const T1 & A, const std::vector<ImplicitInt> & indexes)
{
   return WithoutExpr{A, indexes};
}

}

#endif
