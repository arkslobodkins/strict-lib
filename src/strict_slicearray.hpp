// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include "strict_auxiliary.hpp"
#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_val.hpp"

#include <algorithm>        // copy
#include <initializer_list> // list
#include <iostream>         // ostream, endl
#include <utility>          // move
#include <vector>           // vector

namespace strict {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class slice
{
public:
   explicit slice(ImplicitInt start, ImplicitInt size, ImplicitInt stride = 1)
      : m_start{start.get()}, m_size{size.get()}, m_stride{stride.get()}
   {
      ASSERT_STRICT_DEBUG(m_start > -1);
      ASSERT_STRICT_DEBUG(m_size > -1);
      ASSERT_STRICT_DEBUG(m_stride > 0);
   }

   [[nodiscard]] strict_int start() const {
      return m_start;
   }

   [[nodiscard]] strict_int size() const {
      return m_size;
   }

   [[nodiscard]] strict_int stride() const {
      return m_stride;
   }

   // note: m_stride is already valid, m_start and m_size are "partially" valid
   template<BaseType BaseT>
   [[nodiscard]] bool valid(const BaseT & A) const {
      if(m_start) {
         return internal::valid_index(A, m_start) &&
            internal::valid_index(A, m_start + m_stride * (m_size-1));
      }
      return true;
   }

private:
   strict_int m_start;
   strict_int m_size;
   strict_int m_stride;
};


using seqN = slice;


// unlike slice, sequence cannot be empty
class seq
{
public:
   explicit seq(ImplicitInt first, ImplicitInt last, ImplicitInt stride = 1)
      : m_first{first.get()}, m_last{last.get()}, m_stride{stride.get()}
   {
      ASSERT_STRICT_DEBUG(m_first > -1);
      ASSERT_STRICT_DEBUG(m_last >= m_first);
      ASSERT_STRICT_DEBUG(m_stride > 0);
   }

   [[nodiscard]] slice to_slice() const {
      return slice{m_first, (m_last-m_first)/m_stride + 1, m_stride};
   }

   [[nodiscard]] strict_int first() const {
      return m_first;
   }

   [[nodiscard]] strict_int last() const {
      return m_last;
   }

   [[nodiscard]] strict_int stride() const {
      return m_stride;
   }

   // note: m_stride is already valid, m_first and m_last are "partially" valid
   template<BaseType BaseT>
   [[nodiscard]] bool valid(const BaseT & A) const {
      return internal::valid_index(A, m_first) &&
             internal::valid_index(A, m_last);
   }

private :
   strict_int m_first;
   strict_int m_last;
   strict_int m_stride;
};


class skipN
{
public:
   skipN(ImplicitInt start, ImplicitInt stride) :
      m_start{start.get()}, m_stride{stride.get()}
   {
      ASSERT_STRICT_DEBUG(m_start > -1);
      ASSERT_STRICT_DEBUG(m_stride > 0);
   }

   skipN(ImplicitInt stride) :
      m_start{0}, m_stride{stride.get()}
   {
      ASSERT_STRICT_DEBUG(m_stride > 0);
   }

   [[nodiscard]] strict_int start() const {
      return m_start;
   }

   [[nodiscard]] strict_int stride() const {
      return m_stride;
   }

private:
   strict_int m_start;
   strict_int m_stride;
};


// forward declarations
// non-constant SliceArrays slice base types that are modifiable,
// i. e. indexing returns references
template<DirectBaseType DirectBaseT>
class SliceArray;

template<DirectBaseType DirectBaseT>
class RandSliceArray;

// constant SliceArrays slice any base types
template<BaseType BaseT>
class ConstSliceArray;

template<BaseType Base>
class RandConstSliceArray;


using index_vec = std::vector<ImplicitInt>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
class SliceArray : private Base1D
{
public:
   STRICT_GENERATE_ITERATORS()

   using size_type = typename DirectBaseT::size_type;
   using value_type = typename DirectBaseT::value_type;
   using real_type = typename DirectBaseT::real_type;
   using expr_type = SliceArray<DirectBaseT>;
   using slice_type = SliceArray<DirectBaseT>;

   explicit inline SliceArray(DirectBaseT & A, slice sl);
   SliceArray(const SliceArray & s);
   SliceArray & operator=(const SliceArray & s); // has to work for temporaries

   SliceArray & operator=(StrictVal<real_type> s);
   SliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);

   // assign either Array, SliceArray, or their expression template
   template<OneDimBaseType OneDimBaseT> SliceArray & operator=(const OneDimBaseT & A);

   [[nodiscard]] inline auto & operator[](ImplicitInt i);
   [[nodiscard]] inline const auto & operator[](ImplicitInt i) const;
   [[nodiscard]] inline auto & operator[](internal::Last);
   [[nodiscard]] inline const auto & operator[](internal::Last) const;

   [[nodiscard]] inline size_type size() const noexcept;
   [[nodiscard]] inline bool empty() const noexcept;

   STRICT_GENERATE_SLICES1D()

   SliceArray & operator+=(value_type val);
   SliceArray & operator-=(value_type val);
   SliceArray & operator*=(value_type val);
   SliceArray & operator/=(value_type val);

   template<IntegerType U = real_type> SliceArray & operator%=(value_type val);
   template<IntegerType U = real_type> SliceArray & operator<<=(value_type val);
   template<IntegerType U = real_type> SliceArray & operator>>=(value_type val);

   template<OneDimBaseType OneDimBaseT> SliceArray & operator+=(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> SliceArray & operator-=(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> SliceArray & operator*=(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> SliceArray & operator/=(const OneDimBaseT & A);

   template<OneDimIntegerBaseType OneDimIntBaseT> SliceArray & operator%=(const OneDimIntBaseT & A);
   template<OneDimIntegerBaseType OneDimIntBaseT> SliceArray & operator<<=(const OneDimIntBaseT & A);
   template<OneDimIntegerBaseType OneDimIntBaseT> SliceArray & operator>>=(const OneDimIntBaseT & A);

private:
   typename DirectBaseT::slice_type A; // slices are stored by copy, arrays by reference
   slice sl;

   template<typename F> void apply0(F f);
   template<OneDimBaseType OneDimBaseT, typename F>
      void apply1([[maybe_unused]] const OneDimBaseT & A, F f);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
inline SliceArray<DirectBaseT>::SliceArray(DirectBaseT & A, slice sl) : A{A}, sl{sl}
{
   ASSERT_STRICT_DEBUG(sl.valid(A));
}


template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT>::SliceArray(const SliceArray<DirectBaseT> & s) : A{s.A}, sl{s.sl}
{}


template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(const SliceArray<DirectBaseT> & s)
{
   ASSERT_STRICT_DEBUG(size() == s.size());
   std::copy(s.begin(), s.end(), begin());
   return *this;
}


template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!this->empty());
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


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & SliceArray<DirectBaseT>::operator[](ImplicitInt i)
{
   auto ind = i.get();
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, ind)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return A[sl.start() + ind*sl.stride()];
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline const auto & SliceArray<DirectBaseT>::operator[](ImplicitInt i) const
{
   auto ind = i.get();
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, ind)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return A[sl.start() + ind*sl.stride()];
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & SliceArray<DirectBaseT>::operator[](internal::Last last)
{
   return operator[](size()-1-last.get());
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline const auto & SliceArray<DirectBaseT>::operator[](internal::Last last) const
{
   return operator[](size()-1-last.get());
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto SliceArray<DirectBaseT>::size() const noexcept -> size_type
{
   ASSERT_STRICT_DEBUG(sl.size() > -1);
   return sl.size();
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline bool SliceArray<DirectBaseT>::empty() const noexcept
{
   ASSERT_STRICT_DEBUG(size() > -1);
   return !size();
}


template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator+=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] += val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator-=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] -= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator*=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] *= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator/=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] /= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<IntegerType U>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator%=(value_type val)
{
   static_assert(SameType<U, real_type>);
   apply0([&](size_type i) { (*this)[i] %= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<IntegerType U>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator<<=(value_type val)
{
   static_assert(SameType<U, real_type>);
   apply0([&](size_type i) { (*this)[i] <<= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<IntegerType U>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator>>=(value_type val)
{
   static_assert(SameType<U, real_type>);
   apply0([&](size_type i) { (*this)[i] >>= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator+=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] += A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator-=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] -= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator*=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] *= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator/=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] /= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimIntegerBaseType OneDimIntBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator%=(const OneDimIntBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] %= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimIntegerBaseType OneDimIntBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator<<=(const OneDimIntBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] <<= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimIntegerBaseType OneDimIntBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator>>=(const OneDimIntBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] >>= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<typename F>
void SliceArray<DirectBaseT>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i) {
      f(i);
   }
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT, typename F>
void SliceArray<DirectBaseT>::apply1(const OneDimBaseT & A, F f)
{
   (void)A;
   for(size_type i = 0; i < size(); ++i) {
      f(i);
   }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
class RandSliceArray : private Base1D
{
public:
   STRICT_GENERATE_ITERATORS()

   using size_type = typename DirectBaseT::size_type;
   using value_type = typename DirectBaseT::value_type;
   using real_type = typename DirectBaseT::real_type;
   using expr_type = RandSliceArray<DirectBaseT>;
   using slice_type = RandSliceArray<DirectBaseT>;

   explicit inline RandSliceArray(DirectBaseT & A, std::vector<ImplicitInt> && indexes);
   RandSliceArray(const RandSliceArray & rs);
   RandSliceArray(RandSliceArray && rs);
   RandSliceArray & operator=(const RandSliceArray & rs);
   RandSliceArray & operator=(RandSliceArray && rs);

   RandSliceArray & operator=(StrictVal<real_type> val);
   RandSliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);

   // assign either Array, SliceArray, or their expression template
   template<OneDimBaseType OneDimBaseT> RandSliceArray & operator=(const OneDimBaseT & A);

   [[nodiscard]] inline auto & operator[](ImplicitInt i);
   [[nodiscard]] inline const auto & operator[](ImplicitInt i) const;
   [[nodiscard]] inline auto & operator[](internal::Last);
   [[nodiscard]] inline const auto & operator[](internal::Last) const;

   [[nodiscard]] inline size_type size() const noexcept;
   [[nodiscard]] inline bool empty() const noexcept;

   [[nodiscard]] const std::vector<ImplicitInt> & indexes() const &;
   [[nodiscard]] std::vector<ImplicitInt> indexes() &&;

   STRICT_GENERATE_SLICES1D()

   RandSliceArray & operator+=(value_type val);
   RandSliceArray & operator-=(value_type val);
   RandSliceArray & operator*=(value_type val);
   RandSliceArray & operator/=(value_type val);

   template<IntegerType U = real_type> RandSliceArray & operator%=(value_type val);
   template<IntegerType U = real_type> RandSliceArray & operator<<=(value_type val);
   template<IntegerType U = real_type> RandSliceArray & operator>>=(value_type val);

   template<OneDimBaseType OneDimBaseT> RandSliceArray & operator+=(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> RandSliceArray & operator-=(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> RandSliceArray & operator*=(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> RandSliceArray & operator/=(const OneDimBaseT & A);

   template<OneDimIntegerBaseType OneDimIntBaseT> RandSliceArray & operator%=(const OneDimIntBaseT & A);
   template<OneDimIntegerBaseType OneDimIntBaseT> RandSliceArray & operator<<=(const OneDimIntBaseT & A);
   template<OneDimIntegerBaseType OneDimIntBaseT> RandSliceArray & operator>>=(const OneDimIntBaseT & A);

private:
   typename DirectBaseT::slice_type A;
   std::vector<ImplicitInt> m_indexes;

   template<typename F> void apply0(F f);
   template<OneDimBaseType OneDimBaseT, typename F>
      void apply1(const OneDimBaseT & A, F f);
};


std::ostream & operator<<(std::ostream & os, const std::vector<ImplicitInt> & indexes);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType BaseT>
inline RandSliceArray<BaseT>::RandSliceArray(BaseT & A, std::vector<ImplicitInt> && indexes)
   : A{A}, m_indexes{std::move(indexes)}
{
   ASSERT_STRICT_DEBUG(internal::valid_vector(A, m_indexes));
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT>::RandSliceArray(const RandSliceArray & rs) : A{rs.A}, m_indexes{rs.m_indexes}
{}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT>::RandSliceArray(RandSliceArray && rs) : A{rs.A}, m_indexes{std::move(rs.m_indexes)}
{}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(const RandSliceArray<DirectBaseT> & rs)
{
   ASSERT_STRICT_DEBUG(size() == rs.size());
   std::copy(rs.begin(), rs.end(), begin());
   return *this;
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(RandSliceArray<DirectBaseT> && rs)
{
   ASSERT_STRICT_DEBUG(size() == rs.size());
   A = rs.A;
   m_indexes = std::move(rs.m_indexes);
   return *this;
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!this->empty());
   std::fill(begin(), end(), val);
   return *this;
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(std::initializer_list<StrictVal<real_type>> list)
{
   ASSERT_STRICT_DEBUG(size() == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & RandSliceArray<DirectBaseT>::operator[](ImplicitInt i)
{
   auto ind = i.get();
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, ind)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return A[m_indexes[ind]];
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline const auto & RandSliceArray<DirectBaseT>::operator[](ImplicitInt i) const
{
   auto ind = i.get();
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, ind)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return A[m_indexes[ind]];
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & RandSliceArray<DirectBaseT>::operator[](internal::Last last)
{
   return operator[](size()-1-last.get());
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline const auto & RandSliceArray<DirectBaseT>::operator[](internal::Last last) const
{
   return operator[](size()-1-last.get());
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto RandSliceArray<DirectBaseT>::size() const noexcept -> size_type
{
   return static_cast<size_type>(m_indexes.size());
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] inline bool RandSliceArray<DirectBaseT>::empty() const noexcept
{
   ASSERT_STRICT_DEBUG(size() > -1);
   return !size();
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] auto RandSliceArray<DirectBaseT>::indexes() const & -> const std::vector<ImplicitInt> &
{
   return m_indexes;
}


template<DirectBaseType DirectBaseT>
[[nodiscard]] auto RandSliceArray<DirectBaseT>::indexes() && -> std::vector<ImplicitInt>
{
   return std::move(m_indexes);
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator+=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] += val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator-=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] -= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator*=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] *= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator/=(value_type val)
{
   apply0([&](size_type i) { (*this)[i] /= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<IntegerType U>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator%=(value_type val)
{
   static_assert(SameType<U, real_type>);
   apply0([&](size_type i) { (*this)[i] %= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<IntegerType U>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator<<=(value_type val)
{
   static_assert(SameType<U, real_type>);
   apply0([&](size_type i) { (*this)[i] <<= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<IntegerType U>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator>>=(value_type val)
{
   static_assert(SameType<U, real_type>);
   apply0([&](size_type i) { (*this)[i] >>= val; } );
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator+=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] += A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator-=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] -= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator*=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] *= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator/=(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] /= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimIntegerBaseType OneDimIntBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator%=(const OneDimIntBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] %= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimIntegerBaseType OneDimIntBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator<<=(const OneDimIntBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] <<= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<OneDimIntegerBaseType OneDimIntBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator>>=(const OneDimIntBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   apply1(A, [&](size_type i) { (*this)[i] >>= A[i]; });
   return *this;
}


template<DirectBaseType DirectBaseT>
template<typename F>
void RandSliceArray<DirectBaseT>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i) {
      f(i);
   }
}


template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT, typename F>
void RandSliceArray<DirectBaseT>::apply1([[maybe_unused]] const OneDimBaseT & A, F f)
{
   for(size_type i = 0; i < size(); ++i) {
      f(i);
   }
}


std::ostream & operator<<(std::ostream & os, const std::vector<ImplicitInt> & indexes)
{
   for(decltype(indexes.size()) i = 0; i < indexes.size(); ++i) {
      os << "index[" << i << "] = " << indexes[i].get() << std::endl;
   }
   return os;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class ConstSliceArray : private Base1D
{
public:
   STRICT_GENERATE_CONST_ITERATORS()

   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using expr_type = ConstSliceArray<BaseT>;

   explicit inline ConstSliceArray(const BaseT & A, slice sl);
   ConstSliceArray(const ConstSliceArray & cs) = default;
   ConstSliceArray & operator=(const ConstSliceArray &) = delete;

   [[nodiscard]] inline decltype(auto) operator[](ImplicitInt i) const;
   [[nodiscard]] inline decltype(auto) operator[](internal::Last) const;

   [[nodiscard]] inline size_type size() const noexcept;
   [[nodiscard]] inline bool empty() const noexcept;

   STRICT_GENERATE_CONST_SLICES1D()

private:
   typename BaseT::expr_type A;
   slice sl;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline ConstSliceArray<BaseT>::ConstSliceArray(const BaseT & A, slice sl) : A{A}, sl{sl}
{
   ASSERT_STRICT_DEBUG(sl.valid(A));
}


template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) ConstSliceArray<BaseT>::operator[](ImplicitInt i) const
{
   auto ind = i.get();
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, ind)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return A[sl.start() + ind*sl.stride()];
}


template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) ConstSliceArray<BaseT>::operator[](internal::Last last) const
{
   return operator[](size()-1-last.get());
}


template<BaseType BaseT>
[[nodiscard]] inline auto ConstSliceArray<BaseT>::size() const noexcept -> size_type
{
   ASSERT_STRICT_DEBUG(sl.size() > -1);
   return sl.size();
}


template<BaseType BaseT>
[[nodiscard]] inline bool ConstSliceArray<BaseT>::empty() const noexcept
{
   ASSERT_STRICT_DEBUG(size() > -1);
   return !size();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class RandConstSliceArray : private Base1D
{
public:
   STRICT_GENERATE_CONST_ITERATORS()

   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using expr_type = const RandConstSliceArray<BaseT>;

   explicit inline RandConstSliceArray(const BaseT & A, std::vector<ImplicitInt> && indexes);
   RandConstSliceArray(const RandConstSliceArray & rs) = default;
   RandConstSliceArray & operator=(const RandConstSliceArray &) = delete;

   [[nodiscard]] inline decltype(auto) operator[](ImplicitInt i) const;
   [[nodiscard]] inline decltype(auto) operator[](internal::Last) const;
   [[nodiscard]] inline size_type size() const noexcept;
   [[nodiscard]] inline bool empty() const noexcept;

   [[nodiscard]] const std::vector<ImplicitInt> & indexes() const &;
   [[nodiscard]] std::vector<ImplicitInt> indexes() &&;

   STRICT_GENERATE_CONST_SLICES1D()

private:
   typename BaseT::expr_type A;
   std::vector<ImplicitInt> m_indexes;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline RandConstSliceArray<BaseT>::RandConstSliceArray(const BaseT & A, std::vector<ImplicitInt> && indexes)
   : A{A}, m_indexes{std::move(indexes)}
{
   ASSERT_STRICT_DEBUG(internal::valid_vector(A, m_indexes));
}


template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) RandConstSliceArray<BaseT>::operator[](ImplicitInt i) const
{
   auto ind = i.get();
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, ind)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return A[m_indexes[ind]];
}


template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) RandConstSliceArray<BaseT>::operator[](internal::Last last) const
{
   return operator[](size()-1-last.get());
}


template<BaseType BaseT>
[[nodiscard]] inline auto RandConstSliceArray<BaseT>::size() const noexcept -> size_type
{
   return static_cast<size_type>(m_indexes.size());
}


template<BaseType BaseT>
[[nodiscard]] inline bool RandConstSliceArray<BaseT>::empty() const noexcept
{
   ASSERT_STRICT_DEBUG(size() > -1);
   return !size();
}


template<BaseType BaseT>
[[nodiscard]] auto RandConstSliceArray<BaseT>::indexes() const & -> const std::vector<ImplicitInt> &
{
   return m_indexes;
}


template<BaseType BaseT>
[[nodiscard]] auto RandConstSliceArray<BaseT>::indexes() && -> std::vector<ImplicitInt>
{
   return std::move(m_indexes);
}


}

#endif
