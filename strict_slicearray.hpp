// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include "strict_auxiliary.hpp"
#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_val.hpp"

#include <algorithm>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <vector>

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class slice
{
public:
   explicit slice(strict_int start, strict_int size, strict_int stride = 1)
      : m_start{start}, m_size{size}, m_stride{stride}
   {
      ASSERT_STRICT_DEBUG(start > -1);
      ASSERT_STRICT_DEBUG(size > 0);
      ASSERT_STRICT_DEBUG(stride > 0);
   }

   [[nodiscard]] strict_int start() const { return m_start; }
   [[nodiscard]] strict_int size() const { return m_size; }
   [[nodiscard]] strict_int stride() const { return m_stride; }

   // note: m_stride is already valid, m_start and m_size are "partially" valid
   template<BaseType BaseT>
   bool valid(const BaseT & A) const
   {
      return internal::valid_index(A, m_start) &&
         internal::valid_index(A, m_start + m_stride * (m_size-1));
   }

private:
   strict_int m_start;
   strict_int m_size;
   strict_int m_stride;
};

using seqN = slice;

class seq
{
public:
   explicit seq(strict_int first, strict_int last, strict_int stride = 1)
      : m_first{first}, m_last{last}, m_stride{stride}
   {
      ASSERT_STRICT_DEBUG(first > -1);
      ASSERT_STRICT_DEBUG(last >= first);
      ASSERT_STRICT_DEBUG(stride > 0);
   }

   slice to_slice() const { return slice{m_first, (m_last-m_first)/m_stride + 1, m_stride}; }

   [[nodiscard]] strict_int first() const { return m_first; }
   [[nodiscard]] strict_int last() const { return m_last; }
   [[nodiscard]] strict_int stride() const { return m_stride; }

   // note: m_stride is already valid, m_first and m_last are "partially" valid
   template<BaseType BaseT>
   bool valid(const BaseT & A) const
   {
      return internal::valid_index(A, m_first) &&
         internal::valid_index(A, m_last);
   }

private :
   strict_int m_first;
   strict_int m_last;
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

using index_vec = std::vector<strict_int>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
class SliceArray : private SliceArrayBase1D
{
public:
   STRICT_GENERATE_ITERATORS();

   using size_type = typename DirectBaseT::size_type;
   using value_type = typename DirectBaseT::value_type;
   using real_type = typename DirectBaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = SliceArray<DirectBaseT>;
   using slice_type = SliceArray<DirectBaseT>;

   explicit inline SliceArray(DirectBaseT & A, slice sl);
   SliceArray(const SliceArray & s);
   SliceArray & operator=(const SliceArray & s);

   // assign other types of 1-D SliceArray
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      SliceArray & operator=(const SliceArrayBaseT1D & s);

   SliceArray & operator=(StrictVal<real_type> s);
   SliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);

   // assign either Array, SliceArray, or their expression template
   template<OneDimBaseType OneDimBaseT>
      SliceArray & Assign(const OneDimBaseT & A);

   [[nodiscard]] inline auto & operator[](size_type i);
   [[nodiscard]] const inline auto & operator[](size_type i) const;
   [[nodiscard]] inline auto & operator[](internal::Last);
   [[nodiscard]] const inline auto & operator[](internal::Last) const;

   STRICT_GENERATE_SLICES();

   [[nodiscard]] size_type size() const { return sl.size(); }
   [[nodiscard]] bool empty() const { return size() == 0; }

   SliceArray & operator+=(StrictVal<real_type> val);
   SliceArray & operator-=(StrictVal<real_type> val);
   SliceArray & operator*=(StrictVal<real_type> val);
   SliceArray & operator/=(StrictVal<real_type> val);

   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      SliceArray & operator+=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      SliceArray & operator-=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      SliceArray & operator*=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      SliceArray & operator/=(const SliceArrayBaseT1D & A);

private:
   typename DirectBaseT::slice_type A;
   slice sl;

   template<typename F> void apply0(F f);
   template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
      void apply1(const SliceArrayBaseT1D & A, F f);
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
   if(this != &s) {
      ASSERT_STRICT_DEBUG(size() == s.size());
      std::copy(s.begin(), s.end(), begin());
   }
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(const SliceArrayBaseT1D & s)
{
   static_assert(SameType<RealTypeOf<SliceArrayBaseT1D>, real_type>); // compiler message
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

template<DirectBaseType DirectBaseT>
template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::Assign(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & SliceArray<DirectBaseT>::operator[](size_type i)
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return A[sl.start() + i * sl.stride()];
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] const inline auto & SliceArray<DirectBaseT>::operator[](size_type i) const
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return A[sl.start() + i*sl.stride()];
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & SliceArray<DirectBaseT>::operator[](internal::Last)
{ return A[sl.start() + (size()-1)*sl.stride()]; }

template<DirectBaseType DirectBaseT>
[[nodiscard]] const inline auto & SliceArray<DirectBaseT>::operator[](internal::Last) const
{ return A[sl.start() + (size()-1)*sl.stride()]; }

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

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator+=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] += A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator-=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] -= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator*=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] *= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator/=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] /= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<typename F>
void SliceArray<DirectBaseT>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
void SliceArray<DirectBaseT>::apply1(const SliceArrayBaseT1D & A, F f)
{
   (void)A;
   for(size_type i = 0; i < size(); ++i)
      f(i);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
class RandSliceArray : private SliceArrayBase1D
{
public:
   STRICT_GENERATE_ITERATORS();

   using size_type = typename DirectBaseT::size_type;
   using value_type = typename DirectBaseT::value_type;
   using real_type = typename DirectBaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = RandSliceArray<DirectBaseT>;
   using slice_type = RandSliceArray<DirectBaseT>;

   explicit inline RandSliceArray(DirectBaseT & A, std::vector<size_type> && indexes);
   RandSliceArray(const RandSliceArray & rs);
   RandSliceArray & operator=(const RandSliceArray &);

   // assign other types of 1-D SliceArray
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      RandSliceArray & operator=(const SliceArrayBaseT1D & s);

   RandSliceArray & operator=(StrictVal<real_type> s);
   RandSliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);

   // assign either Array, SliceArray, or their expression template
   template<OneDimBaseType OneDimBaseT>
      RandSliceArray & Assign(const OneDimBaseT & A);

   [[nodiscard]] inline auto & operator[](size_type i);
   [[nodiscard]] inline auto & operator[](internal::Last);
   [[nodiscard]] inline const auto & operator[](size_type i) const;
   [[nodiscard]] inline const auto & operator[](internal::Last) const;

   STRICT_GENERATE_SLICES();

   [[nodiscard]] size_type size() const { return m_indexes.size(); }
   [[nodiscard]] bool empty() const { return size() == 0; }

   RandSliceArray & operator+=(StrictVal<real_type> val);
   RandSliceArray & operator-=(StrictVal<real_type> val);
   RandSliceArray & operator*=(StrictVal<real_type> val);
   RandSliceArray & operator/=(StrictVal<real_type> val);

   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      RandSliceArray & operator+=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      RandSliceArray & operator-=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      RandSliceArray & operator*=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      RandSliceArray & operator/=(const SliceArrayBaseT1D & A);

private:
   typename DirectBaseT::slice_type A;
   std::vector<size_type> m_indexes;

   template<typename F> void apply0(F f);
   template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
      void apply1(const SliceArrayBaseT1D & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType BaseT>
inline RandSliceArray<BaseT>::RandSliceArray(BaseT & A, std::vector<size_type> && indexes)
   : A{A}, m_indexes{std::move(indexes)}
{
   ASSERT_STRICT_DEBUG(!m_indexes.empty());
   ASSERT_STRICT_DEBUG(m_indexes.size() <= A.size());
   ASSERT_STRICT_DEBUG(m_indexes[0] > -1);
   #ifndef STRICT_DEBUG_OFF
   for(size_type i = 1; i < m_indexes.size(); ++i)
      assert(m_indexes[i] > m_indexes[i-1]);
   #endif
}

template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT>::RandSliceArray(const RandSliceArray & rs) : A{rs.A}, m_indexes{rs.m_indexes}
{}

template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(const RandSliceArray<DirectBaseT> & s)
{
   if(this != &s) {
      ASSERT_STRICT_DEBUG(size() == s.size());
      std::copy(s.begin(), s.end(), begin());
   }
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(const SliceArrayBaseT1D & s)
{
   static_assert(SameType<RealTypeOf<SliceArrayBaseT1D>, real_type>); // compiler message
   ASSERT_STRICT_DEBUG(size() == s.size());
   std::copy(s.begin(), s.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator=(StrictVal<real_type> val)
{
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
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::Assign(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & RandSliceArray<DirectBaseT>::operator[](size_type i)
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return A[m_indexes[i]];
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto & RandSliceArray<DirectBaseT>::operator[](internal::Last)
{
   return A[m_indexes[size()-1]];
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline const auto & RandSliceArray<DirectBaseT>::operator[](size_type i) const
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return A[m_indexes[i]];
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline const auto & RandSliceArray<DirectBaseT>::operator[](internal::Last) const
{
   return A[m_indexes[size()-1]];
}

template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator+=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] += val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator-=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] -= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator*=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] *= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator/=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] /= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator+=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] += A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator-=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] -= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator*=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] *= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D>
RandSliceArray<DirectBaseT> & RandSliceArray<DirectBaseT>::operator/=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] /= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT>
template<typename F>
void RandSliceArray<DirectBaseT>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

template<DirectBaseType DirectBaseT>
template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
void RandSliceArray<DirectBaseT>::apply1(const SliceArrayBaseT1D & A, F f)
{
   (void)A;
   for(size_type i = 0; i < size(); ++i)
      f(i);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class ConstSliceArray : private SliceArrayBase1D
{
public:
   STRICT_GENERATE_CONST_ITERATORS();

   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = ConstSliceArray<BaseT>;

   explicit inline ConstSliceArray(const BaseT & A, slice sl);
   ConstSliceArray(const ConstSliceArray & cs) = default;
   ConstSliceArray & operator=(const ConstSliceArray &) = delete;

   [[nodiscard]] inline decltype(auto) operator[](size_type i) const;
   [[nodiscard]] inline decltype(auto) operator[](internal::Last) const;
   STRICT_GENERATE_CONST_SLICES();

   [[nodiscard]] size_type size() const { return sl.size(); }
   [[nodiscard]] bool empty() const { return size() == 0; }

private:
   typename BaseT::expr_type A;
   slice sl;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline ConstSliceArray<BaseT>::ConstSliceArray(const BaseT & A, slice sl) : A{A}, sl{sl}
{ ASSERT_STRICT_DEBUG(sl.valid(A)); }

template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) ConstSliceArray<BaseT>::operator[](size_type i) const
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return A[sl.start() + i*sl.stride()];
}

template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) ConstSliceArray<BaseT>::operator[](internal::Last) const
{
   return A[sl.start() + (size()-1)*sl.stride()];
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class RandConstSliceArray : private SliceArrayBase1D
{
public:
   STRICT_GENERATE_CONST_ITERATORS();

   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = const RandConstSliceArray<BaseT>;

   explicit inline RandConstSliceArray(const BaseT & A, std::vector<size_type> && indexes);
   RandConstSliceArray(const RandConstSliceArray & rs) = default;
   RandConstSliceArray & operator=(const RandConstSliceArray &) = delete;

   [[nodiscard]] inline decltype(auto) operator[](size_type i) const;
   [[nodiscard]] inline decltype(auto) operator[](internal::Last) const;

   STRICT_GENERATE_CONST_SLICES();

   [[nodiscard]] size_type size() const { return m_indexes.size(); }
   [[nodiscard]] bool empty() const { return size() == 0; }

private:
   typename BaseT::expr_type A;
   std::vector<size_type> m_indexes;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline RandConstSliceArray<BaseT>::RandConstSliceArray(const BaseT & A, std::vector<size_type> && indexes)
   : A{A}, m_indexes{std::move(indexes)}
{
   ASSERT_STRICT_DEBUG(!m_indexes.empty());
   ASSERT_STRICT_DEBUG(m_indexes.size() <= A.size());
   ASSERT_STRICT_DEBUG(m_indexes[0] > -1);
   #ifndef STRICT_DEBUG_OFF
   for(size_type i = 1; i < m_indexes.size(); ++i)
      assert(m_indexes[i] > m_indexes[i-1]);
   #endif
}

template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) RandConstSliceArray<BaseT>::operator[](size_type i) const
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return A[m_indexes[i]];
}

template<BaseType BaseT>
[[nodiscard]] inline decltype(auto) RandConstSliceArray<BaseT>::operator[](internal::Last) const
{
   return A[m_indexes[size()-1]];
}

}

#endif
