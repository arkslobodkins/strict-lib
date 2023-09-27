// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include "strict_auxiliary.hpp"
#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_slicearray.hpp"
#include "strict_val.hpp"
#include "strict_array.hpp"

#include <array>
#include <initializer_list>

namespace strict {

template<typename T>
using List2D = std::initializer_list<std::initializer_list<T>>;

template<RealType T>
class Array2D : private Base2D
{
public:
   using size_type = strict_int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using expr_type = const Array2D<T> &;
   using slice_type = Array2D<T> &;

   explicit Array2D();
   explicit Array2D(ImplicitInt dim1, ImplicitInt dim2);
   explicit Array2D(ImplicitInt dim1, ImplicitInt dim2, value_type val);

   Array2D(List2D<value_type> list);
   Array2D(const Array2D & A);
   Array2D(Array2D && a) noexcept;

   Array2D & operator=(value_type val) &;
   Array2D & operator=(List2D<value_type> list) &;
   Array2D & operator=(const Array2D & A) &;
   Array2D & operator=(Array2D && A) & noexcept;

   // construct and assign any one-dimensional type of the same real_type
   template<TwoDimBaseType TwoDimBaseT>
      Array2D(const TwoDimBaseT & A);

   template<TwoDimBaseType TwoDimBaseT>
      Array2D & operator=(const TwoDimBaseT & A) &;

   Array2D & operator+=(value_type val) &;
   Array2D & operator-=(value_type val) &;
   Array2D & operator*=(value_type val) &;
   Array2D & operator/=(value_type val) &;

   template<IntegerType U = T>
      Array2D & operator%=(value_type val) &;
   template<IntegerType U = T>
      Array2D & operator<<=(value_type val) &;
   template<IntegerType U = T>
      Array2D & operator>>=(value_type val) &;

   template<TwoDimBaseType TwoDimBaseT>
      Array2D & operator+=(const TwoDimBaseT & A) &;
   template<TwoDimBaseType TwoDimBaseT>
      Array2D & operator-=(const TwoDimBaseT & A) &;
   template<TwoDimBaseType TwoDimBaseT>
      Array2D & operator*=(const TwoDimBaseT & A) &;
   template<TwoDimBaseType TwoDimBaseT>
      Array2D & operator/=(const TwoDimBaseT & A) &;

   template<TwoDimIntegerBaseType TwoDimIntBaseT>
      Array2D & operator%=(const TwoDimIntBaseT & A) &;
   template<TwoDimIntegerBaseType TwoDimIntBaseT>
      Array2D & operator<<=(const TwoDimIntBaseT & A) &;
   template<TwoDimIntegerBaseType TwoDimIntBaseT>
      Array2D & operator>>=(const TwoDimIntBaseT & A) &;

   void swap(Array2D & A) noexcept;
   void swap(Array2D && A) noexcept; // rvalues can be swapped also

   void resize(ImplicitInt dim1, ImplicitInt dim2);

   template<TwoDimBaseType TwoDimBaseT>
      void resize_and_assign(const TwoDimBaseT & A);

   void resize_and_assign(Array2D && A); // more efficient assign than <resize_and_assign(const &)>

   // remove_row, remove_col, remove_rows, remove_cols

   [[nodiscard]] inline value_type & operator[](ImplicitInt i);
   [[nodiscard]] inline const value_type & operator[](ImplicitInt i) const;
   [[nodiscard]] inline value_type & operator[](internal::Last);
   [[nodiscard]] inline const value_type & operator[](internal::Last) const;

   #if __cplusplus > 202002L // C++23 indexing
   [[nodiscard]] inline value_type & operator[](ImplicitInt i, ImplicitInt j);
   [[nodiscard]] inline const value_type & operator[](ImplicitInt i, ImplicitInt j) const;
   #endif

   [[nodiscard]] inline value_type & operator()(ImplicitInt i, ImplicitInt j);
   [[nodiscard]] inline const value_type & operator()(ImplicitInt i, ImplicitInt j) const;

   [[nodiscard]] inline size_type rows() const;
   [[nodiscard]] inline size_type cols() const;
   [[nodiscard]] inline size_type size() const;
   [[nodiscard]] inline bool empty() const;

   [[nodiscard]] auto view1D() const;

private:
   Array1D<T> elem;
   std::array<strict_int, 2> dims;

   template<typename F> void apply0(F f);
   template<TwoDimBaseType TwoDimBaseT, typename F>
      void apply1([[maybe_unused]] const TwoDimBaseT & A, F f);

   inline bool valid_index(strict_int i, strict_int j) const;
};


template<RealType T>
Array2D<T>::Array2D() :
   elem{},
   dims{0, 0}
{}


template<RealType T>
Array2D<T>::Array2D(ImplicitInt dim1, ImplicitInt dim2) :
   elem(dim1.get() * dim2.get()),
   dims{dim1.get(), dim2.get()}
{
   ASSERT_STRICT_DEBUG(dims[0] > -1);
   ASSERT_STRICT_DEBUG(dims[1] > -1);
   ASSERT_STRICT_DEBUG( !(dims[0] == 0) ^ (dims[1] == 0) );
}


template<RealType T>
Array2D<T>::Array2D(ImplicitInt dim1, ImplicitInt dim2, StrictVal<T> val) :
   elem(dim1.get() * dim2.get(), val),
   dims{dim1.get(), dim2.get()}
{
   ASSERT_STRICT_DEBUG(dims[0] > 0);
   ASSERT_STRICT_DEBUG(dims[1] > 0);
}


template<RealType T>
Array2D<T>::Array2D(List2D<value_type> list) :
   Array2D{}
{
   if(list.size() != 0) {
      // list.begin() can be dereferenced inside <if> since list is not empty
      #ifndef STRICT_DEBUG_ON
      strict_int sz = (*list.begin()).size();
      for(auto lrow : list) {
         ASSERT_STRICT_ALWAYS(lrow.size() == sz);
      }
      ASSERT_STRICT_ALWAYS(sz != 0); // number of columns is not zero to forbid nonzero rows and zero columns
      #endif

      resize( static_cast<size_type>(list.size()), static_cast<size_type>((*list.begin()).size()) );

      strict_int r{};
      for(auto lrow : list) {
         strict_int c{};
         for(auto lcol : lrow) {
            (*this)(r, c++) = lcol;
         }
         r++;
      }
   }
}


template<RealType T>
Array2D<T>::Array2D(const Array2D & A) :
   elem(A.elem),
   dims(A.dims)
{}


template<RealType T>
Array2D<T>::Array2D(Array2D && A) noexcept :
   elem(std::move(A.elem)),
   dims(std::exchange(A.dims, {0, 0}))
{}


template<RealType T>
Array2D<T> & Array2D<T>::operator=(value_type val) &
{
   elem = val;
   return *this;
}


template<RealType T>
Array2D<T> & Array2D<T>::operator=(List2D<value_type> list) &
{
   return *this = std::move(Array2D{list}); // Array2D{list} ensures that list is valid
                                            // and move assignment ensures that list and *this
                                            // have the same number of rows and columns
}


template<RealType T>
Array2D<T> & Array2D<T>::operator=(const Array2D & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   elem = A.elem;
   return *this;
}


template<RealType T>
Array2D<T> & Array2D<T>::operator=(Array2D && A) & noexcept
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   Array2D<T> temp(std::move(A));
   swap(temp);

   return *this;
}


template<RealType T>
Array2D<T> & Array2D<T>::operator+=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}


template<RealType T>
Array2D<T> & Array2D<T>::operator-=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}


template<RealType T>
Array2D<T> & Array2D<T>::operator*=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}


template<RealType T>
Array2D<T> & Array2D<T>::operator/=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}


template<RealType T>
template<IntegerType U>
Array2D<T> & Array2D<T>::operator%=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] %= val; } );
   return *this;
}


template<RealType T>
template<IntegerType U>
Array2D<T> & Array2D<T>::operator<<=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] <<= val; } );
   return *this;
}


template<RealType T>
template<IntegerType U>
Array2D<T> & Array2D<T>::operator>>=(StrictVal<T> val) &
{
   static_assert(SameType<U, T>);
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] >>= val; } );
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
template<TwoDimBaseType TwoDimBaseT>
Array2D<T> & Array2D<T>::operator+=(const TwoDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}


template<RealType T>
template<TwoDimBaseType TwoDimBaseT>
Array2D<T> & Array2D<T>::operator-=(const TwoDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}


template<RealType T>
template<TwoDimBaseType TwoDimBaseT>
Array2D<T> & Array2D<T>::operator*=(const TwoDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}


template<RealType T>
template<TwoDimBaseType TwoDimBaseT>
Array2D<T> & Array2D<T>::operator/=(const TwoDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}


template<RealType T>
template<TwoDimIntegerBaseType TwoDimIntBaseT>
Array2D<T> & Array2D<T>::operator%=(const TwoDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] %= A[i]; });
   return *this;
}


template<RealType T>
template<TwoDimIntegerBaseType TwoDimIntBaseT>
Array2D<T> & Array2D<T>::operator<<=(const TwoDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] <<= A[i]; });
   return *this;
}


template<RealType T>
template<TwoDimIntegerBaseType TwoDimIntBaseT>
Array2D<T> & Array2D<T>::operator>>=(const TwoDimIntBaseT & A) &
{
   ASSERT_STRICT_DEBUG(dims == A.dims);
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] >>= A[i]; });
   return *this;
}


template<RealType T>
void Array2D<T>::swap(Array2D<T> & A) noexcept
{
   elem.swap(A.elem);
   dims.swap(A.dims);
}


template<RealType T>
void Array2D<T>::swap(Array2D<T> && A) noexcept
{
   swap(A);
}


template<RealType T>
void Array2D<T>::resize(ImplicitInt dim1, ImplicitInt dim2)
{
   ASSERT_STRICT_DEBUG(dim1.get() > -1);
   ASSERT_STRICT_DEBUG(dim2.get() > -1);
   ASSERT_STRICT_DEBUG( !(dim1.get() == 0) ^ (dim2.get() == 0) );

   auto dim1_new = dim1.get();
   auto dim2_new = dim2.get();
   if( std::array<strict_int, 2>{dim1_new, dim2_new} != dims ) {
      Array2D<T> temp(dim1_new, dim2_new);
      for(auto i : range(std::min<strict_int>(dims[0], dim1_new))) {
         for(auto j : range(std::min<strict_int>(dims[1], dim2_new))) {
            temp(i, j) = (*this)(i, j);
         }
      }
      swap(temp);
   }
}


template<RealType T>
template<TwoDimBaseType TwoDimBaseT>
void Array2D<T>::resize_and_assign(const TwoDimBaseT & A)
{
   resize(A.rows(), A.cols());
   *this = A;
}


template<RealType T>
void Array2D<T>::resize_and_assign(Array2D && A)
{
   resize(A.rows(), A.cols());
   *this = std::move(A);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array2D<T>::operator[](ImplicitInt i)
{
   return elem[i.get()];
}


template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array2D<T>::operator[](ImplicitInt i) const
{
   return elem[i.get()];
}


template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array2D<T>::operator[](internal::Last last)
{
   return elem[last];
}


template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array2D<T>::operator[](internal::Last last) const
{
   return elem[last];
}


template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array2D<T>::operator()(ImplicitInt i, ImplicitInt j)
{
   ASSERT_STRICT_DEBUG(!empty());
   auto ii = i.get();
   auto jj = j.get();
   #ifndef STRICT_DEBUG_OFF
   if(!valid_index(ii, jj)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[ii * dims[1] + jj];
}


template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array2D<T>::operator()(ImplicitInt i, ImplicitInt j) const
{
   ASSERT_STRICT_DEBUG(!empty());
   auto ii = i.get();
   auto jj = j.get();
   #ifndef STRICT_DEBUG_OFF
   if(!valid_index(ii, jj)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[ii * dims[1] + jj];
}


#if __cplusplus > 202002L
template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array2D<T>::operator[](ImplicitInt i, ImplicitInt j)
{
   ASSERT_STRICT_DEBUG(!empty());
   auto ii = i.get();
   auto jj = j.get();
   #ifndef STRICT_DEBUG_OFF
   if(!valid_index(ii, jj)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[ii * dims[1] + jj];
}


template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array2D<T>::operator[](ImplicitInt i, ImplicitInt j) const
{
   ASSERT_STRICT_DEBUG(!empty());
   auto ii = i.get();
   auto jj = j.get();
   #ifndef STRICT_DEBUG_OFF
   if(!valid_index(ii, jj)) {
      STRICT_THROW_OUT_OF_RANGE();
   }
   #endif
   return elem[ii * dims[1] + jj];
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] inline auto Array2D<T>::rows() const -> size_type
{
   // additional safety check
   ASSERT_STRICT_DEBUG(dims[0] > -1);
   return dims[0];
}


template<RealType T>
[[nodiscard]] inline auto Array2D<T>::cols() const -> size_type
{
   // additional safety check
   ASSERT_STRICT_DEBUG(dims[1] > -1);
   return dims[1];
}


template<RealType T>
[[nodiscard]] inline auto Array2D<T>::size() const -> size_type
{
   // additional safety checks
   ASSERT_STRICT_DEBUG(dims[0] > -1);
   ASSERT_STRICT_DEBUG(dims[1] > -1);
   ASSERT_STRICT_DEBUG( !(dims[0] == 0) ^ (dims[1] == 0) );
   return elem.size();
}


template<RealType T>
[[nodiscard]] inline bool Array2D<T>::empty() const
{
   // additional safety checks
   ASSERT_STRICT_DEBUG( !(dims[0] == 0) ^ (dims[1] == 0) );
   ASSERT_STRICT_DEBUG( !(elem.empty() ^ (dims[0] == 0 && dims[1] == 0)) );
   return elem.empty();
}


template<RealType T>
[[nodiscard]] auto Array2D<T>::view1D() const
{
   return CopyExpr(elem);
}


template<RealType T>
template<typename F> void Array2D<T>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i) {
      f(i);
   }
}


template<RealType T>
template<TwoDimBaseType TwoDimBaseT, typename F>
void Array2D<T>::apply1([[maybe_unused]] const TwoDimBaseT & A, F f)
{
   for(size_type i = 0; i < size(); ++i) {
      f(i);
   }
}


template<RealType T>
inline bool Array2D<T>::valid_index(strict_int i, strict_int j) const
{
   return (i > -1) && (i < dims[0]) && (j > -1) && (j < dims[1]);
}


}


#endif
