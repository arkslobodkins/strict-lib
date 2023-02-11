// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <algorithm>
#include <cmath>
#include <ctime>
#include <initializer_list>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_iterator.hpp"
#include "strict_val.hpp"

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array : private ArrayBase
{
public:
   using size_type = long long int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using expr_type = const Array<T> &;
   using base_type = ArrayBase;
   using expr_base_type = ArrayExpr;

   explicit Array();
   explicit Array(size_type size);
   explicit Array(size_type size, StrictVal<T> val);
   Array(std::initializer_list<StrictVal<T>> list);
   Array(const Array & A);
   Array(Array && a) noexcept;

   Array & operator=(StrictVal<T> val) &;
   Array & operator=(std::initializer_list<StrictVal<T>> list) &;
   Array & operator=(const Array & A) &;
   Array & operator=(Array && A) & noexcept;

   // assign either Array, SliceArray, or their expression template
   template<BaseType BType> Array & Assign(const BType & A) &;

   template<ArrayExprType ArrExpr> Array(const ArrExpr & expr);
   template<ArrayExprType ArrExpr> const Array & operator=(const ArrExpr & expr) &;

   ~Array();

   Array & operator+=(StrictVal<T> val);
   Array & operator-=(StrictVal<T> val);
   Array & operator*=(StrictVal<T> val);
   Array & operator/=(StrictVal<T> val);

   template<ArrayBaseType ArrayType> Array & operator+=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator-=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator*=(const ArrayType & A);
   template<ArrayBaseType ArrayType> Array & operator/=(const ArrayType & A);

   void swap(Array & A) noexcept;
   void resize(size_type size);
   void resize_and_assign(const Array & A);

   [[nodiscard]] inline StrictVal<T> & operator[](size_type i);
   [[nodiscard]] inline const StrictVal<T> & operator[](size_type i) const;

   [[nodiscard]] auto sl(size_type first, size_type last);
   [[nodiscard]] auto sl(size_type first, size_type last) const;

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

   [[nodiscard]] std::vector<StrictVal<T>*> within_range(StrictVal<T> low, StrictVal<T> high);
   [[nodiscard]] std::vector<const StrictVal<T>*> within_range(StrictVal<T> low, StrictVal<T> high) const;

   [[nodiscard]] Array sub_array(size_type first, size_type last);
   template<RealType U> [[nodiscard]] Array<U> convert() const; // conversion chosen by the user;

   void sort_increasing();
   void sort_decreasing();

   template<typename F> void apply(F f);

private:
   StrictVal<T>* elem;
   size_type sz;

   template<typename F> void apply0(F f);
   template<ArrayBaseType ArrayType, typename F>
      void apply1(const ArrayType & A, F f);
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

template<ArrayBaseType T> [[nodiscard]] const auto & operator+(const T & A);
template<ArrayBaseType T> [[nodiscard]] auto operator-(const T & A);
template<ArrayBaseType T> [[nodiscard]] auto abs(const T & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high);

template<FloatingType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
std::ostream & operator<<(std::ostream & os, const BaseT & A);

template<BaseType BaseT>
void print(const BaseT & A, const std::string & name);

template<IntegerBaseType IntBaseT>
[[nodiscard]] auto sum(const IntBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto sum(const FloatBaseT & A);

template<BaseType BaseT>
[[nodiscard]] auto max(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] auto min(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] auto min_index(const BaseT & A);  // returns std::pair<size_type, StrictVal<real_type>>

template<BaseType BaseT>
[[nodiscard]] auto max_index(const BaseT & A);  // returns std::pair<size_type, StrictVal<real_type>>

template<FloatingBaseType FloatBaseT>
[[nodiscard]] bool all_finite(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm_inf(const FloatBaseT & A);

template<FloatingBaseType FloatBaseT>
[[nodiscard]] auto norm2(const FloatBaseT & A);

template<BaseType BType1, BaseType BType2>
[[nodiscard]] auto dot_prod(const BType1 & A1, const BType2 & A2);

template<BaseType BaseT>
[[nodiscard]] bool does_contain_zero(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] bool all_positive(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] bool all_negative(const BaseT & A);

template<BaseType BaseT>
[[nodiscard]] auto unique_blas_array(const BaseT & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<IntegerType size_type>
class Slice
{
public:
   explicit Slice(size_type start, size_type size, size_type stride)
      : m_start{start}, m_size{size}, m_stride{stride}
   {
      ASSERT_STRICT_DEBUG(start > -1);
      ASSERT_STRICT_DEBUG(size > 0);
      ASSERT_STRICT_DEBUG(stride > 0);
   }

   [[nodiscard]] size_type start() const { return m_start; }
   [[nodiscard]] size_type size() const { return m_size; }
   [[nodiscard]] size_type stride() const { return m_stride; }

private:
   size_type m_start;
   size_type m_size;
   size_type m_stride;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class SliceArray : private SliceArrayBase
{
public:
   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using expr_type = const SliceArray<BaseT>;
   using base_type = SliceArrayBase;
   using expr_base_type = SliceArrayExpr;

private:
   BaseT* const A;
   Slice<size_type> slice;

public:
   explicit SliceArray(BaseT & A, Slice<size_type> slice);
   SliceArray(const SliceArray & s);

   SliceArray & operator=(const SliceArray & s);
   template<SliceArrayBaseType SType> SliceArray & operator=(const SType & s); // SType is either SliceArray or
                                                                               // expression template of SliceArray.
   SliceArray & operator=(StrictVal<real_type> s);
   SliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);

   [[nodiscard]] auto operator[](size_type i) -> decltype((*A)[i]) { return (*A)[slice.start()+i*slice.stride()]; }
   [[nodiscard]] auto operator[](size_type i) const -> decltype((*A)[i]) { return (*A)[slice.start()+i*slice.stride()]; }
   [[nodiscard]] auto sl(size_type first, size_type last);
   [[nodiscard]] auto sl(size_type first, size_type last) const;
   [[nodiscard]] size_type size() const { return slice.size(); }
   [[nodiscard]] bool empty() const { return A->empty(); }

   SliceArray & operator+=(StrictVal<real_type> val);
   SliceArray & operator-=(StrictVal<real_type> val);
   SliceArray & operator*=(StrictVal<real_type> val);
   SliceArray & operator/=(StrictVal<real_type> val);

   template<SliceArrayBaseType SliceArrayType> SliceArray & operator+=(const SliceArrayType & A);
   template<SliceArrayBaseType SliceArrayType> SliceArray & operator-=(const SliceArrayType & A);
   template<SliceArrayBaseType SliceArrayType> SliceArray & operator*=(const SliceArrayType & A);
   template<SliceArrayBaseType SliceArrayType> SliceArray & operator/=(const SliceArrayType & A);

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
   template<typename F> void apply0(F f);
   template<SliceArrayBaseType SliceArrayType, typename F>
      void apply1(const SliceArrayType & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class ConstSliceArray : private SliceArrayBase
{
public:
   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using expr_type = const ConstSliceArray<BaseT>;
   using base_type = SliceArrayBase;
   using expr_base_type = SliceArrayExpr;

private:
   const BaseT* const A;
   Slice<size_type> slice;

public:
   explicit ConstSliceArray(const BaseT & A, Slice<size_type> slice);
   ConstSliceArray(const ConstSliceArray & cs);
   ConstSliceArray & operator=(const ConstSliceArray &) = delete;

   [[nodiscard]] auto operator[](size_type i) const -> decltype((*A)[i]) { return (*A)[slice.start()+i*slice.stride()]; }
   [[nodiscard]] size_type size() const { return slice.size(); }
   [[nodiscard]] auto sl(size_type first, size_type last) const;
   [[nodiscard]] bool empty() const { return A->empty(); }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SliceArrayBaseType T1, SliceArrayBaseType T2> [[nodiscard]] auto operator+(const T1 & A, const T2 & B);
template<SliceArrayBaseType T1, SliceArrayBaseType T2> [[nodiscard]] auto operator-(const T1 & A, const T2 & B);
template<SliceArrayBaseType T1, SliceArrayBaseType T2> [[nodiscard]] auto operator*(const T1 & A, const T2 & B);
template<SliceArrayBaseType T1, SliceArrayBaseType T2> [[nodiscard]] auto operator/(const T1 & A, const T2 & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator+(StrictVal<U> val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator-(StrictVal<U> val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator*(StrictVal<U> val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator/(StrictVal<U> val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, StrictVal<U> val);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, StrictVal<U> val);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, StrictVal<U> val);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, StrictVal<U> val);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator+(U val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator-(U val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator*(U val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator/(U val, const T & B);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, U val);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, U val);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, U val);
template<SliceArrayBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, U val);

template<SliceArrayBaseType T> [[nodiscard]] const auto & operator+(const T & A);
template<SliceArrayBaseType T> [[nodiscard]] auto operator-(const T & A);
template<SliceArrayBaseType T> [[nodiscard]] auto abs(const T & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {
   template<RealType T>
   std::string smart_spaces(typename Array<T>::size_type max_ind, typename Array<T>::size_type ind)
   {
      using sz_T = typename Array<T>::size_type;
      auto count_digit = [](sz_T number) -> sz_T {
         if(!number) return 1;
         return static_cast<sz_T>(std::log10(number)) + 1;
      };

      sz_T max_digits = count_digit(max_ind);
      sz_T ind_digits = count_digit(ind);
      return std::string(static_cast<std::basic_string<char>::size_type>(1+max_digits-ind_digits), 32);
   }

   template<BaseType BaseT>
   bool valid_index(const BaseT & A, typename BaseT::size_type index)
   {
      if(index < 0 || index > A.size()-1)
         return false;
      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::Array() :
   elem{},
   sz{}
{}

template<RealType T> Array<T>::Array(size_type size) :
   elem{new StrictVal<T>[static_cast<std::size_t>(size)]},
   sz{size}
{
   ASSERT_STRICT_DEBUG(sz > -1);
}

template<RealType T> Array<T>::Array(size_type size, StrictVal<T> val)
   : Array(size)
{
   ASSERT_STRICT_DEBUG(sz > 0);
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
   elem{std::exchange(A.elem, {})},
   sz{std::exchange(A.sz, {})}
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T> & Array<T>::operator=(StrictVal<T> val) &
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

template<RealType T> Array<T> & Array<T>::operator=(const Array<T> & A) &
{
   if(this != &A) {
      ASSERT_STRICT_DEBUG(sz == A.sz);
      std::copy(A.begin(), A.end(), begin());
   }
   return *this;
}

template<RealType T> Array<T> & Array<T>::operator=(Array<T> && A) & noexcept
{
   ASSERT_STRICT_DEBUG(sz == A.sz);
   Array<T> temp(std::move(A));
   swap(temp);
   return *this;
}

template<RealType T> template<BaseType BType>
Array<T> & Array<T>::Assign(const BType & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayExprType ArrExpr>
Array<T>::Array(const ArrExpr & expr)
   : Array(expr.size())
{
   std::copy(expr.begin(), expr.end(), begin());
}

template<RealType T> template<ArrayExprType ArrExpr>
const Array<T> & Array<T>::operator=(const ArrExpr & expr) &
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
Array<T> & Array<T>::operator+=(StrictVal<T> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator-=(StrictVal<T> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator*=(StrictVal<T> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator/=(StrictVal<T> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator+=(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(sz == A.size()); // Changed A.sz to A.size().
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator-=(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(sz == A.size()); // Changed A.sz to A.size().
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator*=(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(sz == A.size()); // Changed A.sz to A.size().
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType ArrayType>
Array<T> & Array<T>::operator/=(const ArrayType & A)
{
   ASSERT_STRICT_DEBUG(sz == A.size()); // Changed A.sz to A.size().
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> void Array<T>::swap(Array<T> & A) noexcept
{
   std::swap(elem, A.elem);
   std::swap(sz, A.sz);
}

template<RealType T> void Array<T>::resize(size_type size)
{
   Array<T> temp(size);
   std::copy(begin(), begin() + std::min(sz, size), temp.begin());
   swap(temp);
}

template<RealType T> void Array<T>::resize_and_assign(const Array<T> & A)
{
   resize(A.size());
   *this = A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
inline StrictVal<T> & Array<T>::operator[](size_type i)
{
   #ifdef STRICT_DEBUG_ON
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
inline const StrictVal<T> & Array<T>::operator[](size_type i) const
{
   #ifdef STRICT_DEBUG_ON
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
auto Array<T>::sl(size_type first, size_type last)
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return SliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice<size_type>{first, last-first+1, size_type{1}}};
}

template<RealType T>
auto Array<T>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice<size_type>{first, last-first+1, size_type{1}}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
std::vector<StrictVal<T>*> Array<T>::within_range(StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_DEBUG(!empty());
   ASSERT_STRICT_DEBUG(high >= low);
   std::vector<StrictVal<T>*> v{};
   for(auto & x : *this)
      if(x >= low && x <= high)
         v.push_back(&x);
   return v;
}

template<RealType T>
std::vector<const StrictVal<T>*> Array<T>::within_range(StrictVal<T> low, StrictVal<T> high) const
{
   ASSERT_STRICT_DEBUG(!empty());
   ASSERT_STRICT_DEBUG(high >= low);
   std::vector<const StrictVal<T>*> v{};
   for(auto & x : *this)
      if(x >= low && x <= high)
         v.push_back(&x);
   return v;
}

template<RealType T> void Array<T>::sort_decreasing()
{
   ASSERT_STRICT_DEBUG(!empty());
   std::sort(begin(), end(), [](auto a, auto b) { return a > b; });
}

template<RealType T> void Array<T>::sort_increasing()
{
   ASSERT_STRICT_DEBUG(!empty());
   std::sort(begin(), end(), [](auto a, auto b) { return a < b; });
}

template<RealType T>
Array<T> Array<T>::sub_array(size_type first, size_type last)
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   Array s(last-first+1);
   for(size_type i = 0; i < s.size(); ++i)
      s.elem[i] = elem[first+i];
   return s;
}

template<RealType T> template<RealType U>
Array<U> Array<T>::convert() const
{
   Array<U> A(size());
   for(size_type i = 0; i < size(); ++i)
      A[i] = ((*this)[i]).template convert<U>();
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<typename F>
void Array<T>::apply(F f)
{
   ASSERT_STRICT_DEBUG(!empty());
   for(size_type i = 0; i < sz; ++i)
      f(elem[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<typename F>
void Array<T>::apply0(F f)
{
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<RealType T> template<ArrayBaseType ArrayType, typename F>
void Array<T>::apply1(const ArrayType & A, F f)
{
   (void)A;
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<IntegerType T>
Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
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
Array<T> array_random(typename Array<T>::size_type size, StrictVal<T> low, StrictVal<T> high)
{
   ASSERT_STRICT_DEBUG(size > 0);
   ASSERT_STRICT_DEBUG(high >= low);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   for(auto & x : A)
      x = low + (high - low) * T(std::rand()) / T(RAND_MAX);
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
SliceArray<BaseT>::SliceArray(BaseT & A, Slice<size_type> slice) : A{&A}, slice{slice}
{
   ASSERT_STRICT_DEBUG(internal::valid_index(A, slice.start()));
   ASSERT_STRICT_DEBUG(internal::valid_index(A, slice.start() + slice.stride() * (slice.size()-1)));
}

template<BaseType BaseT>
SliceArray<BaseT>::SliceArray(const SliceArray<BaseT> & s) : A{s.A}, slice{s.slice}
{}

template<BaseType BaseT>
SliceArray<BaseT> & SliceArray<BaseT>::operator=(const SliceArray<BaseT> & s)
{
   ASSERT_STRICT_DEBUG(size() == s.size());
   std::copy(s.begin(), s.end(), begin());
   return *this;
}

template<BaseType BaseT> template<SliceArrayBaseType SType>
SliceArray<BaseT> & SliceArray<BaseT>::operator=(const SType & s)
{
   static_assert(SameType<typename SType::real_type, real_type>);
   ASSERT_STRICT_DEBUG(size() == s.size());
   std::copy(s.begin(), s.end(), begin());
   return *this;
}

template<BaseType BaseT>
SliceArray<BaseT> & SliceArray<BaseT>::operator=(StrictVal<real_type> val)
{
   std::fill(begin(), end(), val);
   return *this;
}

template<BaseType BaseT>
SliceArray<BaseT> & SliceArray<BaseT>::operator=(std::initializer_list<StrictVal<real_type>> list)
{
   ASSERT_STRICT_DEBUG(size() == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}

template<BaseType BaseT>
auto SliceArray<BaseT>::sl(size_type first, size_type last)
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return SliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<BaseType BaseT>
auto SliceArray<BaseT>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

template<BaseType BaseT>
SliceArray<BaseT> & SliceArray<BaseT>::operator+=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] += val; } );
   return *this;
}

template<BaseType BaseT>
SliceArray<BaseT> & SliceArray<BaseT>::operator-=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] -= val; } );
   return *this;
}

template<BaseType BaseT>
SliceArray<BaseT> & SliceArray<BaseT>::operator*=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] *= val; } );
   return *this;
}

template<BaseType BaseT>
SliceArray<BaseT> & SliceArray<BaseT>::operator/=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] /= val; } );
   return *this;
}

template<BaseType BaseT> template<SliceArrayBaseType SliceArrayType>
SliceArray<BaseT> & SliceArray<BaseT>::operator+=(const SliceArrayType & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] += A[i]; });
   return *this;
}

template<BaseType BaseT> template<SliceArrayBaseType SliceArrayType>
SliceArray<BaseT> & SliceArray<BaseT>::operator-=(const SliceArrayType & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] -= A[i]; });
   return *this;
}

template<BaseType BaseT> template<SliceArrayBaseType SliceArrayType>
SliceArray<BaseT> & SliceArray<BaseT>::operator*=(const SliceArrayType & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] *= A[i]; });
   return *this;
}

template<BaseType BaseT> template<SliceArrayBaseType SliceArrayType>
SliceArray<BaseT> & SliceArray<BaseT>::operator/=(const SliceArrayType & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] /= A[i]; });
   return *this;
}

template<BaseType BaseT> template<typename F>
void SliceArray<BaseT>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

template<BaseType BaseT> template<SliceArrayBaseType SliceArrayType, typename F>
void SliceArray<BaseT>::apply1(const SliceArrayType & A, F f)
{
   (void)A;
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
ConstSliceArray<BaseT>::ConstSliceArray(const BaseT & A, Slice<size_type> slice) : A{&A}, slice{slice}
{
   ASSERT_STRICT_DEBUG(internal::valid_index(A, slice.start()));
   ASSERT_STRICT_DEBUG(internal::valid_index(A, slice.start() + slice.stride() * (slice.size()-1)));
}

template<BaseType BaseT>
ConstSliceArray<BaseT>::ConstSliceArray(const ConstSliceArray<BaseT> & cs) : A{cs.A}, slice{cs.slice}
{}

template<BaseType BaseT>
auto ConstSliceArray<BaseT>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType T, UnaryOperationType Op>
class UnaryExpr : private T::base_type, private T::expr_base_type
{
public:
   using size_type = typename T::size_type;
   using value_type = typename T::value_type;
   using real_type = typename T::real_type;
   using base_type = typename T::base_type;
   using expr_base_type = typename T::expr_base_type;
   using expr_type = UnaryExpr<T, Op>;

   explicit UnaryExpr(const T & A, Op op) : A{A}, op{op} { ASSERT_STRICT_DEBUG(!A.empty()); }
   UnaryExpr(const UnaryExpr &) = default;
   UnaryExpr & operator=(const UnaryExpr &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(A[i]); }
   [[nodiscard]] auto sl(size_type first, size_type last) const;
   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename T::expr_type A;
   Op op;
};

template<BaseType T, UnaryOperationType Op>
auto UnaryExpr<T, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType T1, BaseType T2, BinaryOperationType Op>
class BinExpr : private T1::base_type, private T1::expr_base_type
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   using real_type = typename T1::real_type;
   using base_type = typename T1::base_type;
   using expr_base_type = typename T1::expr_base_type;
   using expr_type = BinExpr<T1, T2, Op>;

   explicit BinExpr(const T1 & A, const T2 & B, Op op) : A{A}, B{B}, op{op} {
      static_assert(SameType<typename T1::size_type, typename T2::size_type>);
      static_assert(SameType<typename T1::value_type, typename T2::value_type>);
      static_assert(SameType<typename T1::real_type, typename T2::real_type>);
      static_assert(SameType<typename T1::base_type, typename T2::base_type>);
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(A.size() == B.size());
   }
   BinExpr(const BinExpr &) = default;
   BinExpr & operator=(const BinExpr &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(A[i], B[i]); }
   [[nodiscard]] auto sl(size_type first, size_type last) const;
   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename T1::expr_type A;
   typename T2::expr_type B;
   Op op;
};

template<ArrayBaseType T1, ArrayBaseType T2, BinaryOperationType Op>
auto BinExpr<T1, T2, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType T1, RealType T2, BinaryOperationType Op>
class BinExprValLeft : private T1::base_type, private T1::expr_base_type
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   using real_type = typename T1::real_type;
   using base_type = typename T1::base_type;
   using expr_base_type = typename T1::expr_base_type;
   using expr_type = BinExprValLeft<T1, T2, Op>;

   explicit BinExprValLeft(const T1 & B, T2 val, Op op) : B{B}, val{val}, op{op} {
      static_assert(SameType<typename T1::real_type, T2>);
      ASSERT_STRICT_DEBUG(!B.empty());
   }
   BinExprValLeft(const BinExprValLeft &) = default;
   BinExprValLeft & operator=(const BinExprValLeft &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(val, B[i]); }
   [[nodiscard]] auto sl(size_type first, size_type last) const;
   [[nodiscard]] size_type size() const { return B.size(); }
   [[nodiscard]] bool empty() const { return B.empty(); }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename T1::expr_type B;
   StrictVal<T2> val;
   Op op;
};

template<ArrayBaseType T1, RealType T2, BinaryOperationType Op>
auto BinExprValLeft<T1, T2, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType T1, RealType T2, BinaryOperationType Op>
class BinExprValRight : private T1::base_type, private T1::expr_base_type
{
public:
   using size_type = typename T1::size_type;
   using value_type = typename T1::value_type;
   using real_type = typename T1::real_type;
   using base_type = typename T1::base_type;
   using expr_base_type = typename T1::expr_base_type;
   using expr_type = BinExprValRight<T1, T2, Op>;

   explicit BinExprValRight(const T1 & A, T2 val, Op op) : A{A}, val{val}, op{op} {
      static_assert(SameType<typename T1::real_type, T2>);
      ASSERT_STRICT_DEBUG(!A.empty());
   }
   BinExprValRight(const BinExprValRight &) = default;
   BinExprValRight & operator=(const BinExprValRight &) = delete;

   [[nodiscard]] const value_type operator[](size_type i) const { return op(A[i], val); }
   [[nodiscard]] auto sl(size_type first, size_type last) const;
   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename T1::expr_type A;
   StrictVal<T2> val;
   Op op;
};

template<ArrayBaseType T1, RealType T2, BinaryOperationType Op>
auto BinExprValRight<T1, T2, Op>::sl(size_type first, size_type last) const
{
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, first));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, last));
   ASSERT_STRICT_DEBUG(last >= first);
   return ConstSliceArray<std::remove_cvref_t<decltype(*this)>>
      {*this, Slice(first, last-first+1, size_type{1})};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SliceArrayBaseType T1, SliceArrayBaseType T2>
auto operator+(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Plus{}); }

template<SliceArrayBaseType T1, SliceArrayBaseType T2>
auto operator-(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Minus{}); }

template<SliceArrayBaseType T1, SliceArrayBaseType T2>
auto operator*(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Mult{}); }

template<SliceArrayBaseType T1, SliceArrayBaseType T2>
auto operator/(const T1 & A, const T2 & B)
{ return BinExpr(A, B, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SliceArrayBaseType T, RealType U>
auto operator+(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Plus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator-(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Minus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator*(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Mult{}); }

template<SliceArrayBaseType T, RealType U>
auto operator/(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SliceArrayBaseType T, RealType U>
auto operator+(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Plus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator-(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Minus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator*(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Mult{}); }

template<SliceArrayBaseType T, RealType U>
auto operator/(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SliceArrayBaseType T, RealType U>
auto operator+(U val, const T & B)
{ return BinExprValLeft(B, val, Plus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator-(U val, const T & B)
{ return BinExprValLeft(B, val, Minus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator*(U val, const T & B)
{ return BinExprValLeft(B, val, Mult{}); }

template<SliceArrayBaseType T, RealType U>
auto operator/(U val, const T & B)
{ return BinExprValLeft(B, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SliceArrayBaseType T, RealType U>
auto operator+(const T & A, U val)
{ return BinExprValRight(A, val, Plus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator-(const T & A, U val)
{ return BinExprValRight(A, val, Minus{}); }

template<SliceArrayBaseType T, RealType U>
auto operator*(const T & A, U val)
{ return BinExprValRight(A, val, Mult{}); }

template<SliceArrayBaseType T, RealType U>
auto operator/(const T & A, U val)
{ return BinExprValRight(A, val, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<SliceArrayBaseType T>
const auto & operator+(const T & A)
{ return A; }

template<SliceArrayBaseType T>
auto operator-(const T & A)
{ return UnaryExpr(A, UnaryMinus{}); }

template<SliceArrayBaseType T>
auto abs(const T & A)
{ return UnaryExpr(A, UnaryAbs{}); }

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
{ return BinExprValLeft(B, U{val}, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(StrictVal<U> val, const T & B)
{ return BinExprValLeft(B, U{val}, Divide{}); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType T, RealType U>
auto operator+(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Plus{}); }

template<ArrayBaseType T, RealType U>
auto operator-(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Minus{}); }

template<ArrayBaseType T, RealType U>
auto operator*(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Mult{}); }

template<ArrayBaseType T, RealType U>
auto operator/(const T & A, StrictVal<U> val)
{ return BinExprValRight(A, U{val}, Divide{}); }

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
template<ArrayBaseType T>
const auto & operator+(const T & A)
{ return A; }

template<ArrayBaseType T>
auto operator-(const T & A)
{ return UnaryExpr(A, UnaryMinus{}); }

template<ArrayBaseType T>
auto abs(const T & A)
{ return UnaryExpr(A, UnaryAbs{}); }

template<BaseType BaseT>
std::ostream & operator<<(std::ostream & os, const BaseT & A)
{
   using T = typename BaseT::real_type;
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      os << "[" << i << "] ="
         << internal::smart_spaces<T>(A.size(), i) << A[i] << std::endl;
   }
   return os;
}

template<BaseType BaseT>
void print(const BaseT & A, const std::string & name)
{
   using T = typename BaseT::real_type;
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      std::cout << name << "[" << i << "] ="
         << internal::smart_spaces<T>(A.size(), i) << A[i] << std::endl;
   }
}

template<FloatingBaseType FloatBaseT>
auto sum(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using rv_T = typename FloatBaseT::real_type;

   volatile rv_T sum{};
   volatile rv_T c{};
   for(decltype(A.size()) i = 0; i < A.size(); ++i) {
      volatile rv_T y = A[i] - c;
      volatile rv_T t = sum + y;
      c = (t - sum) - y;
      sum = t;
   }
   return StrictVal<rv_T>{sum};
}

template<IntegerBaseType IntBaseT>
auto sum(const IntBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto sum = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      sum += A[i];
   return sum;
}

template<BaseType BaseT>
auto max(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto max_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      max_elem = maxs(A[i], max_elem);
   return max_elem;
}

template<BaseType BaseT>
auto min(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto min_elem = A[0];
   for(decltype(A.size()) i = 1; i < A.size(); ++i)
      min_elem = mins(A[i], min_elem);
   return min_elem;
}

template<BaseType BaseT>
auto max_index(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using sz_T = typename BaseT::size_type;
   using sv_T = typename BaseT::value_type;

   std::pair<sz_T, sv_T> max = {0, A[0]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] > max.second)
         max = {i, A[i]};
   return max;
}

template<BaseType BaseT>
auto min_index(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using sz_T = typename BaseT::size_type;
   using sv_T = typename BaseT::value_type;

   std::pair<sz_T, sv_T> min = {0, A[0]};
   for(sz_T i = 1; i < A.size(); ++i)
      if(A[i] < min.second)
         min = {i, A[i]};
   return min;
}

template<FloatingBaseType FloatBaseT>
bool all_finite(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   for(auto x : A)
      if(!isfinites(x)) return false;
   return true;
}

template<FloatingBaseType FloatBaseT>
auto norm_inf(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   auto max_abs = abss(A[0]);
   for(decltype(A.size()) i = 1; i < A.size(); ++i) {
      auto abs_i = abss(A[i]);
      max_abs  = maxs(abs_i, max_abs);
   }
   return max_abs;
}

template<FloatingBaseType FloatBaseT>
auto norm2(const FloatBaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   return sqrts(dot_prod(A, A));
}

template<BaseType BType1, BaseType BType2>
auto dot_prod(const BType1 & A1, const BType2 & A2)
{
   static_assert(SameType<typename BType1::base_type, typename BType2::base_type>);
   ASSERT_STRICT_DEBUG(A1.size() == A2.size());
   ASSERT_STRICT_DEBUG(!A1.empty());
   return sum(A1 * A2);
}

template<BaseType BaseT>
bool does_contain_zero(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using T = typename BaseT::real_type;
   for(auto x : A)
      if(x == T{0}) return true;
   return false;
}

template<BaseType BaseT>
bool all_positive(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using T = typename BaseT::real_type;
   for(auto x : A)
      if(x <= T{0}) return false;
   return true;
}

template<BaseType BaseT>
bool all_negative(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using T = typename BaseT::real_type;
   for(auto x : A)
      if(x >= T{0}) return false;
   return true;
}

template<BaseType BaseT>
[[nodiscard]] auto unique_blas_array(const BaseT & A)
{
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = typename BaseT::real_type;
   auto blas_array = std::make_unique<real_type[]>(static_cast<std::size_t>(A.size()));
   std::copy(A.begin(), A.end(), blas_array.get());
   return blas_array;
}

}

#endif
