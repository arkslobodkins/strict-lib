// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <algorithm>         // copy, fill
#include <initializer_list>  // initializer_list

#include "strict_auxiliary.hpp"
#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_slicearray.hpp"
#include "strict_val.hpp"

namespace strict {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, strict_int sz>
class FixedArray1D : private Base1D
{
public:
   using size_type = strict_int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using expr_type = const FixedArray1D<T, sz> &;
   using slice_type = FixedArray1D<T, sz> &;

   explicit constexpr FixedArray1D();
   explicit constexpr FixedArray1D(value_type val);
   constexpr FixedArray1D(std::initializer_list<value_type> list);
   constexpr FixedArray1D(const FixedArray1D & A);
   template<RealType U, strict_int sz2> constexpr FixedArray1D(const FixedArray1D<U, sz2> & A); // fails intentionally

   FixedArray1D & operator=(value_type val) &;
   FixedArray1D & operator=(std::initializer_list<value_type> list) &;
   FixedArray1D & operator=(const FixedArray1D & A) &;
   template<RealType U, strict_int sz2> FixedArray1D & operator=(const FixedArray1D<U, sz2> & A) &; // fails intentionally

   // construct and assign any one-dimensional type of the same real_type
   template<OneDimBaseType OneDimBaseT> FixedArray1D(const OneDimBaseT & A);
   template<OneDimBaseType OneDimBaseT> FixedArray1D & operator=(const OneDimBaseT & A) &;

   ~FixedArray1D() = default;

   FixedArray1D & operator+=(value_type val) &;
   FixedArray1D & operator-=(value_type val) &;
   FixedArray1D & operator*=(value_type val) &;
   FixedArray1D & operator/=(value_type val) &;

   template<IntegerType U = T> FixedArray1D & operator%=(value_type val) &;
   template<IntegerType U = T> FixedArray1D & operator<<=(value_type val) &;
   template<IntegerType U = T> FixedArray1D & operator>>=(value_type val) &;

   template<OneDimBaseType OneDimBaseT> FixedArray1D & operator+=(const OneDimBaseT & A) &;
   template<OneDimBaseType OneDimBaseT> FixedArray1D & operator-=(const OneDimBaseT & A) &;
   template<OneDimBaseType OneDimBaseT> FixedArray1D & operator*=(const OneDimBaseT & A) &;
   template<OneDimBaseType OneDimBaseT> FixedArray1D & operator/=(const OneDimBaseT & A) &;

   template<OneDimIntegerBaseType OneDimIntBaseT> FixedArray1D & operator%=(const OneDimIntBaseT & A) &;
   template<OneDimIntegerBaseType OneDimIntBaseT> FixedArray1D & operator<<=(const OneDimIntBaseT & A) &;
   template<OneDimIntegerBaseType OneDimIntBaseT> FixedArray1D & operator>>=(const OneDimIntBaseT & A) &;

   [[nodiscard]] constexpr inline value_type & operator[](ImplicitInt i);
   [[nodiscard]] constexpr inline const value_type & operator[](ImplicitInt i) const;
   [[nodiscard]] constexpr inline value_type & operator[](internal::Last);
   [[nodiscard]] constexpr inline const value_type & operator[](internal::Last) const;

   [[nodiscard]] constexpr inline size_type size() const noexcept { return sz; }
   [[nodiscard]] constexpr inline bool empty() const noexcept { return sz == 0; }

   STRICT_GENERATE_SLICES1D()
   STRICT_GENERATE_ITERATORS()

private:
   value_type elem[sz];

   template<typename F> void apply0(F f);
   template<OneDimBaseType OneDimBaseT, typename F>
      void apply1([[maybe_unused]] const OneDimBaseT & A, F f);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, strict_int sz>
constexpr FixedArray1D<T, sz>::FixedArray1D()
{}


template<RealType T, strict_int sz>
constexpr FixedArray1D<T, sz>::FixedArray1D(value_type val)
{
   static_assert(sz > 0);
   std::fill(&elem[0], &elem[sz], val);
}


// handles empty initializer list case as well
template<RealType T, strict_int sz>
constexpr FixedArray1D<T, sz>::FixedArray1D(std::initializer_list<value_type> list)
{
   ASSERT_STRICT_DEBUG(sz == static_cast<size_type>(list.size()));
   if constexpr(sz > 0)
      std::copy(list.begin(), list.end(), &elem[0]);
}


template<RealType T, strict_int sz>
constexpr FixedArray1D<T, sz>::FixedArray1D(const FixedArray1D<T, sz> & fa)
{
   if constexpr(sz > 0)
      std::copy(&fa.elem[0], &fa.elem[sz], &elem[0]);
}


template<RealType T, strict_int sz>
template<RealType U, strict_int sz2>
constexpr FixedArray1D<T, sz>::FixedArray1D(const FixedArray1D<U, sz2> & fa)
{
   static_assert(SameType<T, U>);
   static_assert(false, "CANNOT ASSIGN ARRAY OF DIFFERENT SIZE");
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, strict_int sz>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator=(value_type val) &
{
   static_assert(sz > 0);
   std::fill(begin(), end(), val);
   return *this;
}


// handles empty initializer list case as well
template<RealType T, strict_int sz>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator=(std::initializer_list<value_type> list) &
{
   ASSERT_STRICT_DEBUG(sz == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}


template<RealType T, strict_int sz>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator=(const FixedArray1D<T, sz> & fa) &
{
   std::copy(fa.begin(), fa.end(), begin());
   return *this;
}


template<RealType T, strict_int sz>
template<RealType U, strict_int sz2>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator=(const FixedArray1D<U, sz2> & fa) &
{
   static_assert(SameType<T, U>);
   static_assert(false, "CANNOT ASSIGN ARRAY OF DIFFERENT SIZE");
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, strict_int sz>
template<OneDimBaseType OneDimBaseT>
FixedArray1D<T, sz>::FixedArray1D(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   std::copy(A.begin(), A.end(), begin());
}


template<RealType T, strict_int sz>
template<OneDimBaseType OneDimBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, strict_int sz>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator+=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}


template<RealType T, strict_int sz>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator-=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}


template<RealType T, strict_int sz>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator*=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}


template<RealType T, strict_int sz>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator/=(StrictVal<T> val) &
{
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}


template<RealType T, strict_int sz>
template<IntegerType U>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator%=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   apply0([&](size_type i) { elem[i] %= val; } );
   return *this;
}


template<RealType T, strict_int sz>
template<IntegerType U>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator<<=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   apply0([&](size_type i) { elem[i] <<= val; } );
   return *this;
}


template<RealType T, strict_int sz>
template<IntegerType U>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator>>=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   apply0([&](size_type i) { elem[i] >>= val; } );
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, strict_int sz>
template<OneDimBaseType OneDimBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator+=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}


template<RealType T, strict_int sz>
template<OneDimBaseType OneDimBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator-=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}


template<RealType T, strict_int sz>
template<OneDimBaseType OneDimBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator*=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}


template<RealType T, strict_int sz>
template<OneDimBaseType OneDimBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator/=(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}


template<RealType T, strict_int sz>
template<OneDimIntegerBaseType OneDimIntBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator%=(const OneDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] %= A[i]; });
   return *this;
}


template<RealType T, strict_int sz>
template<OneDimIntegerBaseType OneDimIntBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator<<=(const OneDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] <<= A[i]; });
   return *this;
}


template<RealType T, strict_int sz>
template<OneDimIntegerBaseType OneDimIntBaseT>
FixedArray1D<T, sz> & FixedArray1D<T, sz>::operator>>=(const OneDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   apply1(A, [&](size_type i) { elem[i] >>= A[i]; });
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// keep constexpr qualifier, since temporaries bind to non-const  overload
template<RealType T, strict_int sz>
[[nodiscard]] constexpr inline StrictVal<T> & FixedArray1D<T, sz>::operator[](ImplicitInt i)
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i.get())) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[i.get()];
}


template<RealType T, strict_int sz>
[[nodiscard]] constexpr inline const StrictVal<T> & FixedArray1D<T, sz>::operator[](ImplicitInt i) const
{
   #ifndef STRICT_DEBUG_OFF
   if(!internal::valid_index(*this, i.get())) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[i.get()];
}


template<RealType T, strict_int sz>
[[nodiscard]] constexpr inline StrictVal<T> & FixedArray1D<T, sz>::operator[](internal::Last last)
{
   return operator[](sz-1-last.get());
}


template<RealType T, strict_int sz>
[[nodiscard]] constexpr inline const StrictVal<T> & FixedArray1D<T, sz>::operator[](internal::Last last) const
{
   return operator[](sz-1-last.get());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T, strict_int sz>
template<typename F>
void FixedArray1D<T, sz>::apply0(F f)
{
   for(size_type i = 0; i < sz; ++i) {
      f(i);
   }
}


template<RealType T, strict_int sz>
template<OneDimBaseType OneDimBaseT, typename F>
void FixedArray1D<T, sz>::apply1([[maybe_unused]] const OneDimBaseT & A, F f)
{
   for(size_type i = 0; i < sz; ++i) {
      f(i);
   }
}


}


#endif
