// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>

#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_iterator.hpp"
#include "strict_val.hpp"
#include "strict_array_ops.hpp"

namespace strict_array {

class ArrayBase1D : protected Base {};
class ArrayExpr1D : protected Expr, protected ArrayBase1D {};
template<typename T> concept ArrayBaseType1D = BaseOf<ArrayBase1D, T>;
template<typename T> concept ArrayExprType1D = BaseOf<ArrayExpr1D, T>;

class SliceArrayBase1D : protected Base {};
class SliceArrayExpr1D : protected Expr, protected SliceArrayBase1D {};
template<typename T> concept SliceArrayBaseType1D = BaseOf<SliceArrayBase1D, T>;
template<typename T> concept SliceArrayExprType1D = BaseOf<SliceArrayExpr1D, T>;

template<typename T> concept OneDimBaseType = ArrayBaseType1D<T> || SliceArrayBaseType1D<T>;
template<typename T> concept OneDimFloatingBaseType = OneDimBaseType<T> && FloatingBaseType<T>;

class Slice;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array : private ArrayBase1D
{
public:
   using size_type = long long int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using base_type = ArrayBase1D;
   using expr_base_type = ArrayExpr1D;
   using expr_type = const Array<T> &;
   using slice_type = Array<T> &;

   explicit Array();
   explicit Array(size_type size);
   explicit Array(size_type size, StrictVal<T> val);
   explicit Array(std::initializer_list<StrictVal<T>> list);
   Array(const Array & A);
   Array(Array && a) noexcept;

   Array & operator=(StrictVal<T> val) &;
   Array & operator=(std::initializer_list<StrictVal<T>> list) &;
   Array & operator=(const Array & A) &;
   Array & operator=(Array && A) & noexcept;

   // assign either Array, SliceArray, or their expression template
   template<OneDimBaseType OneDimBaseT> Array & Assign(const OneDimBaseT & A) &;

   template<ArrayExprType1D ArrayExprT1D> Array(const ArrayExprT1D & expr);
   template<ArrayExprType1D ArrayExprT1D> Array & operator=(const ArrayExprT1D & expr) &;

   ~Array();

   Array & operator+=(StrictVal<T> val) &;
   Array & operator-=(StrictVal<T> val) &;
   Array & operator*=(StrictVal<T> val) &;
   Array & operator/=(StrictVal<T> val) &;

   template<ArrayBaseType1D ArrayBaseT1D> Array & operator+=(const ArrayBaseT1D & A) &;
   template<ArrayBaseType1D ArrayBaseT1D> Array & operator-=(const ArrayBaseT1D & A) &;
   template<ArrayBaseType1D ArrayBaseT1D> Array & operator*=(const ArrayBaseT1D & A) &;
   template<ArrayBaseType1D ArrayBaseT1D> Array & operator/=(const ArrayBaseT1D & A) &;

   void swap(Array & A) noexcept;
   void resize(size_type size);
   void resize_and_assign(const Array & A);

   [[nodiscard]] inline StrictVal<T> & operator[](size_type i);
   [[nodiscard]] inline const StrictVal<T> & operator[](size_type i) const;

   [[nodiscard]] inline auto sl(size_type first, size_type last);
   [[nodiscard]] inline auto sl(size_type first, size_type last) const;

   [[nodiscard]] inline auto sl(Slice slice);
   [[nodiscard]] inline auto sl(Slice slice) const;

   [[nodiscard]] StrictVal<T> & front() { return elem[0]; }
   [[nodiscard]] StrictVal<T> & back() { return elem[sz-1]; }
   [[nodiscard]] const StrictVal<T> & front() const { return elem[0]; }
   [[nodiscard]] const StrictVal<T> & back() const { return elem[sz-1]; }

   [[nodiscard]] auto begin() { return iterator{*this, 0}; }
   [[nodiscard]] auto end() { return iterator{*this, size()}; }
   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() { return std::reverse_iterator{end()}; }
   [[nodiscard]] auto rend() { return std::reverse_iterator{begin()}; }
   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

   [[nodiscard]] size_type size() const { ASSERT_STRICT_DEBUG(sz > -1); return sz; }
   [[nodiscard]] bool empty() const { ASSERT_STRICT_DEBUG(sz > -1); return sz == 0; }

   [[nodiscard]] StrictVal<T>* data() & { return !empty() ? &elem[0] : nullptr; }
   [[nodiscard]] const StrictVal<T>* data() const & { return !empty() ? &elem[0] : nullptr; }

   template<RealType U> [[nodiscard]] Array<U> convert() const; // conversion chosen by the user;

   void sort_increasing();
   void sort_decreasing();

private:
   StrictVal<T>* elem;
   size_type sz;

   template<typename F> void apply0(F f);
   template<ArrayBaseType1D ArrayBaseT1D, typename F>
      void apply1(const ArrayBaseT1D & A, F f);
};

template<OneDimBaseType OneDimBaseT>
[[nodiscard]] auto ConstructArray(const OneDimBaseT & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto two_prod(const T1 & A, const T2 & B);

template<OneDimBaseType T> [[nodiscard]] auto operator+(const T & A);
template<OneDimBaseType T> [[nodiscard]] auto operator-(const T & A);
template<OneDimBaseType T> [[nodiscard]] auto abs(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p);
template<OneDimFloatingBaseType T> [[nodiscard]] auto pow_int(const T & A, StrictVal<int> p);
template<OneDimFloatingBaseType T> [[nodiscard]] auto exp(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto log(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto sqrt(const T & A);
template<RealType T> [[nodiscard]] auto e_unit(long long int j, long long int size);
template<RealType T> [[nodiscard]] auto e_slice_unit(long long int j, long long int size);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low = T{0}, StrictVal<T> high = T{1});

template<FloatingType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low = T{0}, StrictVal<T> high = T{1});

template<RealType T>
[[nodiscard]] Array<T> array_iota(typename Array<T>::size_type size, StrictVal<T> val = T{0});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {
   template<BaseType BaseT>
   bool valid_index(const BaseT & A, typename BaseT::size_type index)
   { return index > -1 && index < A.size(); }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Slice
{
public:
   Slice(long long int start, long long int size, long long int stride)
      : m_start{start}, m_size{size}, m_stride{stride}
   {
      ASSERT_STRICT_DEBUG(start > -1);
      ASSERT_STRICT_DEBUG(size > 0);
      ASSERT_STRICT_DEBUG(stride > 0);
   }

   [[nodiscard]] long long int start() const { return m_start; }
   [[nodiscard]] long long int size() const { return m_size; }
   [[nodiscard]] long long int stride() const { return m_stride; }

   template<BaseType BaseT>
   bool valid(const BaseT & A) const
   {
      return internal::valid_index(A, m_start) &&
         internal::valid_index(A, m_start + m_stride * (m_size-1));
   }

private:
   long long int m_start;
   long long int m_size;
   long long int m_stride;
};

//template<DirectBaseType DirectBaseT>
//class RandSliceArray : private SliceArrayBase1D
//{
//public:
//   using size_type = typename DirectBaseT::size_type;
//   using value_type = typename DirectBaseT::value_type;
//   using real_type = typename DirectBaseT::real_type;
//   using base_type = SliceArrayBase1D;
//   using expr_base_type = SliceArrayExpr1D;
//   using expr_type = const RandSliceArray<DirectBaseT>;
//   using slice_type = RandSliceArray<DirectBaseT>;
//
//   explicit inline RandSliceArray(DirectBaseT & A, const Array<long long int> & index_array);
//   RandSliceArray(const RandSliceArray & s);
//
//   RandSliceArray & operator=(const RandSliceArray & s);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator=(const SliceArrayBaseT1D & s);
//   RandSliceArray & operator=(StrictVal<real_type> s);
//   RandSliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);
//
//   // assign either Array, SliceArray, or their expression template
//   template<OneDimBaseType OneDimBaseT> RandSliceArray & Assign(const OneDimBaseT & A) &;
//
//   [[nodiscard]] auto & operator[](size_type i)
//      { return A[index_array[i]]; }
//   [[nodiscard]] const auto & operator[](size_type i) const
//      { return A[index_array[i]]; }
//
//   [[nodiscard]] inline auto sl(size_type first, size_type last);
//   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
//   [[nodiscard]] inline auto sl(Slice slice);
//   [[nodiscard]] inline auto sl(Slice slice) const;
//
//   [[nodiscard]] size_type size() const { return index_array.size(); }
//   [[nodiscard]] bool empty() const { return A.empty(); }
//
//   RandSliceArray & operator+=(StrictVal<real_type> val);
//   RandSliceArray & operator-=(StrictVal<real_type> val);
//   RandSliceArray & operator*=(StrictVal<real_type> val);
//   RandSliceArray & operator/=(StrictVal<real_type> val);
//
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator+=(const SliceArrayBaseT1D & A);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator-=(const SliceArrayBaseT1D & A);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator*=(const SliceArrayBaseT1D & A);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator/=(const SliceArrayBaseT1D & A);
//
//   [[nodiscard]] auto & front() { return (*this)[0]; }
//   [[nodiscard]] auto & back() { return (*this)[index_array.size()-1]; }
//   [[nodiscard]] auto & front() const { return (*this)[0]; }
//   [[nodiscard]] auto & back() const { return (*this)[index_array.size()-1]; }
//
//   [[nodiscard]] auto begin() { return iterator{*this, 0}; }
//   [[nodiscard]] auto end() { return iterator{*this, size()}; }
//   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
//   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
//   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
//   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }
//
//   [[nodiscard]] auto rbegin() { return std::reverse_iterator{end()}; }
//   [[nodiscard]] auto rend() { return std::reverse_iterator{begin()}; }
//   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
//   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
//   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
//   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }
//
//private:
//   typename DirectBaseT::slice_type A;
//   Array<long long int> index_array;
//
//   template<typename F> void apply0(F f);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
//      void apply1(const SliceArrayBaseT1D & A, F f);
//};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
class SliceArray : private SliceArrayBase1D
{
public:
   using size_type = typename DirectBaseT::size_type;
   using value_type = typename DirectBaseT::value_type;
   using real_type = typename DirectBaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = const SliceArray<DirectBaseT>;
   using slice_type = SliceArray<DirectBaseT>;

   explicit inline SliceArray(DirectBaseT & A, Slice slice);
   SliceArray(const SliceArray & s);

   SliceArray & operator=(const SliceArray & s);
   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator=(const SliceArrayBaseT1D & s);
   SliceArray & operator=(StrictVal<real_type> s);
   SliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);

   // assign either Array, SliceArray, or their expression template
   template<OneDimBaseType OneDimBaseT> SliceArray & Assign(const OneDimBaseT & A) &;

   [[nodiscard]] auto & operator[](size_type i)
      { return A[slice.start() + i*slice.stride()]; }
   [[nodiscard]] const auto & operator[](size_type i) const
      { return A[slice.start() + i*slice.stride()]; }

   [[nodiscard]] inline auto sl(size_type first, size_type last);
   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
   [[nodiscard]] inline auto sl(Slice slice);
   [[nodiscard]] inline auto sl(Slice slice) const;

   [[nodiscard]] size_type size() const { return slice.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   SliceArray & operator+=(StrictVal<real_type> val);
   SliceArray & operator-=(StrictVal<real_type> val);
   SliceArray & operator*=(StrictVal<real_type> val);
   SliceArray & operator/=(StrictVal<real_type> val);

   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator+=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator-=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator*=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator/=(const SliceArrayBaseT1D & A);

   [[nodiscard]] auto & front() { return (*this)[0]; }
   [[nodiscard]] auto & back() { return (*this)[slice.size()-1]; }
   [[nodiscard]] auto & front() const { return (*this)[0]; }
   [[nodiscard]] auto & back() const { return (*this)[slice.size()-1]; }

   [[nodiscard]] auto begin() { return iterator{*this, 0}; }
   [[nodiscard]] auto end() { return iterator{*this, size()}; }
   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() { return std::reverse_iterator{end()}; }
   [[nodiscard]] auto rend() { return std::reverse_iterator{begin()}; }
   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename DirectBaseT::slice_type A;
   Slice slice;

   template<typename F> void apply0(F f);
   template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
      void apply1(const SliceArrayBaseT1D & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class ConstSliceArray : private SliceArrayBase1D
{
public:
   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = const ConstSliceArray<BaseT>;

   explicit inline ConstSliceArray(const BaseT & A, Slice slice);
   ConstSliceArray(const ConstSliceArray & cs);
   ConstSliceArray & operator=(const ConstSliceArray &) = delete;

   [[nodiscard]] decltype(auto) operator[](size_type i) const
      { return A[slice.start()+i*slice.stride()]; }

   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
   [[nodiscard]] inline auto sl(Slice slice) const;

   [[nodiscard]] size_type size() const { return slice.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] decltype(auto) front() const { return (*this)[0]; }
   [[nodiscard]] decltype(auto) back() const { return (*this)[slice.size()-1]; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename BaseT::expr_type A;
   Slice slice;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T>::Array() :
   elem{},
   sz{}
{}

template<RealType T>
Array<T>::Array(size_type size) :
   elem{new StrictVal<T>[static_cast<std::size_t>(size)]},
   sz{size}
{ ASSERT_STRICT_DEBUG(sz > -1); }

template<RealType T>
Array<T>::Array(size_type size, StrictVal<T> val) :
   Array(size)
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
}

template<RealType T>
Array<T>::Array(std::initializer_list<StrictVal<T>> list) :
   Array(static_cast<size_type>(list.size()))
{ std::copy(list.begin(), list.end(), begin()); }

template<RealType T> Array<T>::Array(const Array<T> & A) :
   Array(A.size())
{ std::copy(A.begin(), A.end(), begin()); }

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
   if(this != &A) {
      ASSERT_STRICT_DEBUG(sz == A.sz);
      std::copy(A.begin(), A.end(), begin());
   }
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

template<RealType T> template<OneDimBaseType OneDimBaseT>
Array<T> & Array<T>::Assign(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayExprType1D ArrayExprT1D>
Array<T>::Array(const ArrayExprT1D & expr) :
   Array(expr.size())
{ std::copy(expr.begin(), expr.end(), begin()); }

template<RealType T> template<ArrayExprType1D ArrayExprT1D>
Array<T> & Array<T>::operator=(const ArrayExprT1D & expr) &
{
   ASSERT_STRICT_DEBUG(sz == expr.size());
   std::copy(expr.begin(), expr.end(), begin());
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::~Array()
{ delete[] elem; }

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
template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator+=(const ArrayBaseT1D & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size()); // Changed A.sz to A.size().
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator-=(const ArrayBaseT1D & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator*=(const ArrayBaseT1D & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator/=(const ArrayBaseT1D & A) &
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
void Array<T>::resize(size_type size)
{
   Array<T> temp(size);
   std::copy(begin(), begin() + std::min(sz, size), temp.begin());
   swap(temp);
}

template<RealType T>
void Array<T>::resize_and_assign(const Array<T> & A)
{
   resize(A.size());
   *this = A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array<T>::operator[](size_type i)
{
   #ifdef STRICT_DEBUG_ON
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array<T>::operator[](size_type i) const
{
   #ifdef STRICT_DEBUG_ON
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
[[nodiscard]] inline auto Array<T>::sl(size_type first, size_type last)
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return SliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice{first, last-first+1, size_type{1}}};
}

template<RealType T>
[[nodiscard]] inline auto Array<T>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice{first, last-first+1, size_type{1}}};
}

template<RealType T>
[[nodiscard]] inline auto Array<T>::sl(Slice slice)
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return SliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

template<RealType T>
[[nodiscard]] inline auto Array<T>::sl(Slice slice) const
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
void Array<T>::sort_decreasing()
{
   ASSERT_STRICT_DEBUG(!empty());
   std::sort(begin(), end(), [](auto a, auto b) { return a > b; });
}

template<RealType T>
void Array<T>::sort_increasing()
{
   ASSERT_STRICT_DEBUG(!empty());
   std::sort(begin(), end(), [](auto a, auto b) { return a < b; });
}

template<RealType T> template<RealType U>
[[nodiscard]] Array<U> Array<T>::convert() const
{
   Array<U> A(size());
   for(size_type i = 0; i < size(); ++i)
      A[i] = ((*this)[i]).template convert<U>();
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<typename F>
void Array<T>::apply0(F f)
{
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D, typename F>
void Array<T>::apply1(const ArrayBaseT1D & A, F f)
{
   (void)A;
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<OneDimBaseType OneDimBaseT>
[[nodiscard]] auto ConstructArray(const OneDimBaseT & A)
{
   Array<RealTypeOf<OneDimBaseT>> C(A.size());
   C.Assign(A);
   return C;
}

template<IntegerType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_DEBUG(size > 0);
   ASSERT_STRICT_DEBUG(high >= low);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   long int diff_range = (high - low).template convert<long int>() + 1;
   for(auto & x : A)
      x = low + T(std::rand() % diff_range);
   return A;
}

template<FloatingType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_DEBUG(size > 0);
   ASSERT_STRICT_DEBUG(high >= low);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   for(auto & x : A)
      x = low + (high - low) * T(std::rand()) / T(RAND_MAX);
   return A;
}

template<RealType T>
[[nodiscard]] Array<T> array_iota(typename Array<T>::size_type size, StrictVal<T> val)
{
   ASSERT_STRICT_DEBUG(size > 0);
   Array<T> A(size);
   std::iota(A.begin(), A.end(), val);
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
inline SliceArray<DirectBaseT>::SliceArray(DirectBaseT & A, Slice slice) : A{A}, slice{slice}
{ ASSERT_STRICT_DEBUG(slice.valid(A)); }

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT>::SliceArray(const SliceArray<DirectBaseT> & s) : A{s.A}, slice{s.slice}
{}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(const SliceArray<DirectBaseT> & s)
{
   if(this != &s) {
      ASSERT_STRICT_DEBUG(size() == s.size());
      std::copy(s.begin(), s.end(), begin());
   }
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(const SliceArrayBaseT1D & s)
{
   static_assert(SameType<typename SliceArrayBaseT1D::real_type, real_type>);
   ASSERT_STRICT_DEBUG(size() == s.size());
   std::copy(s.begin(), s.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(StrictVal<real_type> val)
{
   std::fill(begin(), end(), val);
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(std::initializer_list<StrictVal<real_type>> list)
{
   ASSERT_STRICT_DEBUG(size() == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT> template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::Assign(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto SliceArray<DirectBaseT>::sl(size_type first, size_type last)
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return SliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto SliceArray<DirectBaseT>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto SliceArray<DirectBaseT>::sl(Slice slice)
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return SliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto SliceArray<DirectBaseT>::sl(Slice slice) const
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator+=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] += val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator-=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] -= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator*=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] *= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator/=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] /= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator+=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] += A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator-=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] -= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator*=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] *= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator/=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] /= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<typename F>
void SliceArray<DirectBaseT>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
void SliceArray<DirectBaseT>::apply1(const SliceArrayBaseT1D & A, F f)
{
   (void)A;
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline ConstSliceArray<BaseT>::ConstSliceArray(const BaseT & A, Slice slice) : A{A}, slice{slice}
{ ASSERT_STRICT_DEBUG(slice.valid(A)); }

template<BaseType BaseT>
ConstSliceArray<BaseT>::ConstSliceArray(const ConstSliceArray<BaseT> & cs) : A{cs.A}, slice{cs.slice}
{}

template<BaseType BaseT>
[[nodiscard]] inline auto ConstSliceArray<BaseT>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<BaseType BaseT>
[[nodiscard]] inline auto ConstSliceArray<BaseT>::sl(Slice slice) const
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, slice};
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

struct UnaryPowInt : private UnaryOperation
{
   UnaryPowInt(StrictVal<int> p) : p{p} {}

   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return pows_int(strict_val, p);
   }
private:
   StrictVal<int> p;
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

template<typename T>
class StandardUnitVector : private ArrayExpr1D
{
public:
   using size_type = long long int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using base_type = ArrayBase1D;
   using expr_base_type = ArrayExpr1D;
   using expr_type = const StandardUnitVector<T>;

   explicit StandardUnitVector(long long int j, long long int size) : j{j}, sz{size}
   { ASSERT_STRICT_DEBUG(j > -1); ASSERT_STRICT_DEBUG(size > j); }

   StandardUnitVector(const StandardUnitVector &) = default;
   // assignment is deleted to stay consistent with other expression templates
   StandardUnitVector & operator=(const StandardUnitVector &) = delete;

   value_type operator[](long long int i) const { return j == i ? T{1} : T{0}; }
   long long int size() const { return sz; }
   bool empty() const { return false; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   const long long int j;
   const long long int sz;
};

template<typename T>
class StandardSliceUnitVector : private SliceArrayExpr1D
{
public:
   using size_type = long long int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = const StandardSliceUnitVector<T>;

   explicit StandardSliceUnitVector(long long int j, long long int size) : j{j}, sz{size}
   { ASSERT_STRICT_DEBUG(j > -1); ASSERT_STRICT_DEBUG(size > j); }

   StandardSliceUnitVector(const StandardSliceUnitVector &) = default;
   // assignment is deleted to stay consistent with other expression templates
   StandardSliceUnitVector & operator=(const StandardSliceUnitVector &) = delete;

   value_type operator[](long long int i) const { return j == i ? T{1} : T{0}; }
   long long int size() const { return sz; }
   bool empty() const { return false; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   const long long int j;
   const long long int sz;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimeBaseT, UnaryOperationType Op>
class UnaryExpr : private OneDimeBaseT::expr_base_type
{
public:
   using size_type = typename OneDimeBaseT::size_type;
   using value_type = typename OneDimeBaseT::value_type;
   using real_type = typename OneDimeBaseT::real_type;
   using base_type = typename OneDimeBaseT::base_type;
   using expr_base_type = typename OneDimeBaseT::expr_base_type;
   using expr_type = const UnaryExpr<OneDimeBaseT, Op>;

   explicit UnaryExpr(const OneDimeBaseT & A, Op op) : A{A}, op{op} { ASSERT_STRICT_DEBUG(!A.empty()); }
   UnaryExpr(const UnaryExpr &) = default;
   UnaryExpr & operator=(const UnaryExpr &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const { return op(A[i]); }
   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
   [[nodiscard]] inline auto sl(Slice sl) const;

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] value_type front() const { return (*this)[0]; }
   [[nodiscard]] value_type back() const { return (*this)[size()-1]; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename OneDimeBaseT::expr_type A;
   Op op;
};

template<OneDimBaseType OneDimeBaseT, UnaryOperationType Op>
[[nodiscard]] inline auto UnaryExpr<OneDimeBaseT, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<OneDimBaseType OneDimeBaseT, UnaryOperationType Op>
[[nodiscard]] inline auto UnaryExpr<OneDimeBaseT, Op>::sl(Slice slice) const
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, BinaryOperationType Op>
class BinExpr : private OneDimBaseT1::expr_base_type
{
public:
   using size_type = typename OneDimBaseT1::size_type;
   using value_type = typename OneDimBaseT1::value_type;
   using real_type = typename OneDimBaseT1::real_type;
   using base_type = typename OneDimBaseT1::base_type;
   using expr_base_type = typename OneDimBaseT1::expr_base_type;
   using expr_type = const BinExpr<OneDimBaseT1, OneDimBaseT2, Op>;

   explicit BinExpr(const OneDimBaseT1 & A, const OneDimBaseT2 & B, Op op) : A{A}, B{B}, op{op} {
      static_assert(SameType<typename OneDimBaseT1::size_type, typename OneDimBaseT2::size_type>);
      static_assert(SameType<typename OneDimBaseT1::value_type, typename OneDimBaseT2::value_type>);
      static_assert(SameType<typename OneDimBaseT1::real_type, typename OneDimBaseT2::real_type>);
      static_assert(SameType<typename OneDimBaseT1::base_type, typename OneDimBaseT2::base_type>);
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(A.size() == B.size());
   }
   BinExpr(const BinExpr &) = default;
   BinExpr & operator=(const BinExpr &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const { return op(A[i], B[i]); }
   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
   [[nodiscard]] inline auto sl(Slice slice) const;

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] value_type front() const { return (*this)[0]; }
   [[nodiscard]] value_type back() const { return (*this)[size()-1]; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename OneDimBaseT1::expr_type A;
   typename OneDimBaseT2::expr_type B;
   Op op;
};

template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, BinaryOperationType Op>
[[nodiscard]] inline auto BinExpr<OneDimBaseT1, OneDimBaseT2, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, BinaryOperationType Op>
[[nodiscard]] inline auto BinExpr<OneDimBaseT1, OneDimBaseT2, Op>::sl(Slice slice) const
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
class BinExprValLeft : private OneDimBaseT1::expr_base_type
{
public:
   using size_type = typename OneDimBaseT1::size_type;
   using value_type = typename OneDimBaseT1::value_type;
   using real_type = typename OneDimBaseT1::real_type;
   using base_type = typename OneDimBaseT1::base_type;
   using expr_base_type = typename OneDimBaseT1::expr_base_type;
   using expr_type = const BinExprValLeft<OneDimBaseT1, T2, Op>;

   explicit BinExprValLeft(const OneDimBaseT1 & B, T2 val, Op op) : B{B}, val{val}, op{op} {
      static_assert(SameType<typename OneDimBaseT1::real_type, T2>);
      ASSERT_STRICT_DEBUG(!B.empty());
   }
   BinExprValLeft(const BinExprValLeft &) = default;
   BinExprValLeft & operator=(const BinExprValLeft &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const { return op(val, B[i]); }
   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
   [[nodiscard]] inline auto sl(Slice slice) const;

   [[nodiscard]] size_type size() const { return B.size(); }
   [[nodiscard]] bool empty() const { return B.empty(); }

   [[nodiscard]] value_type front() const { return (*this)[0]; }
   [[nodiscard]] value_type back() const { return (*this)[size()-1]; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename OneDimBaseT1::expr_type B;
   StrictVal<T2> val;
   Op op;
};

template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
[[nodiscard]] inline auto BinExprValLeft<OneDimBaseT1, T2, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
[[nodiscard]] inline auto BinExprValLeft<OneDimBaseT1, T2, Op>::sl(Slice slice) const
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
class BinExprValRight : private OneDimBaseT1::expr_base_type
{
public:
   using size_type = typename OneDimBaseT1::size_type;
   using value_type = typename OneDimBaseT1::value_type;
   using real_type = typename OneDimBaseT1::real_type;
   using base_type = typename OneDimBaseT1::base_type;
   using expr_base_type = typename OneDimBaseT1::expr_base_type;
   using expr_type = const BinExprValRight<OneDimBaseT1, T2, Op>;

   explicit BinExprValRight(const OneDimBaseT1 & A, T2 val, Op op) : A{A}, val{val}, op{op} {
      static_assert(SameType<typename OneDimBaseT1::real_type, T2>);
      ASSERT_STRICT_DEBUG(!A.empty());
   }
   BinExprValRight(const BinExprValRight &) = default;
   BinExprValRight & operator=(const BinExprValRight &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const { return op(A[i], val); }
   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
   [[nodiscard]] inline auto sl(Slice slice) const;

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] value_type front() const { return (*this)[0]; }
   [[nodiscard]] value_type back() const { return (*this)[size()-1]; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename OneDimBaseT1::expr_type A;
   StrictVal<T2> val;
   Op op;
};

template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
[[nodiscard]] inline auto BinExprValRight<OneDimBaseT1, T2, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::decay_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
[[nodiscard]] inline auto BinExprValRight<OneDimBaseT1, T2, Op>::sl(Slice slice) const
{
   ASSERT_STRICT_DEBUG(slice.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, slice};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator+(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Plus{}); }

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator-(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Minus{}); }

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator*(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Mult{}); }

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator/(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Divide{}); }

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto two_prod(const T1 & A, const T2 & B)
{ return std::pair{BinExpr(A, B, BinaryTwoProdFirst{}), BinExpr(A, B, BinaryTwoProdSecond{})}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Plus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Minus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Mult{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Plus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Minus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Mult{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(U val, const T & B)
{ return BinExprValLeft(B, val, Plus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(U val, const T & B)
{ return BinExprValLeft(B, val, Minus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(U val, const T & B)
{ return BinExprValLeft(B, val, Mult{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(U val, const T & B)
{ return BinExprValLeft(B, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(const T & A, U val)
{ return BinExprValRight(A, val, Plus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(const T & A, U val)
{ return BinExprValRight(A, val, Minus{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(const T & A, U val)
{ return BinExprValRight(A, val, Mult{}); }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(const T & A, U val)
{ return BinExprValRight(A, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T>
[[nodiscard]] auto operator+(const T & A)
{ return UnaryExpr(A, UnaryPlus{}); }

template<OneDimBaseType T>
[[nodiscard]] auto operator-(const T & A)
{ return UnaryExpr(A, UnaryMinus{}); }

template<OneDimBaseType T>
[[nodiscard]] auto abs(const T & A)
{ return UnaryExpr(A, UnaryAbs{}); }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p)
{ return UnaryExpr(A, UnaryPow{p}); }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow_int(const T & A, StrictVal<int> p)
{ return UnaryExpr(A, UnaryPowInt{p}); }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto exp(const T & A)
{ return UnaryExpr(A, UnaryExp{}); }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto log(const T & A)
{ return UnaryExpr(A, UnaryLog{}); }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto sqrt(const T & A)
{ return UnaryExpr(A, UnarySqrt{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] auto e_unit(long long int j, long long int size)
{ return StandardUnitVector<T>(j, size); }

template<RealType T>
[[nodiscard]] auto e_slice_unit(long long int j, long long int size)
{ return StandardSliceUnitVector<T>(j, size); }

}

#endif
