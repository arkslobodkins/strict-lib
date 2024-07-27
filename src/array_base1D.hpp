//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once


#include <initializer_list>  // initializer_list
#include <new>               // align_val_t
#include <type_traits>       // is_constant_evaluated
#include <utility>           // move, forward, swap, exchange
#include <vector>            // vector

#include "Common/common.hpp"


namespace slib {


enum AlignmentFlag { Aligned, Unaligned };


template <Builtin T, AlignmentFlag AF>
class ArrayBase1D;


template <typename D> concept Array1DType = OneDimBaseType<D>
                                         && (DerivedFrom<D, ArrayBase1D<BuiltinTypeOf<D>, Aligned>>
                                             || DerivedFrom<D, ArrayBase1D<BuiltinTypeOf<D>, Unaligned>>);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
class STRICT_NODISCARD ArrayBase1D : private ReferenceBase1D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   // constructors
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D();
   STRICT_NODISCARD explicit ArrayBase1D(ImplicitInt n)
      requires(AF == Aligned);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(ImplicitInt n)
      requires(AF == Unaligned);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(Size n);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(ImplicitInt n, value_type x);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(Size n, Value<T> x);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(std::initializer_list<value_type> list);

   template <LinearIteratorType L>
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(L b, L e);

   STRICT_NODISCARD_CONSTEXPR ArrayBase1D(const ArrayBase1D& A);
   STRICT_NODISCARD_CONSTEXPR ArrayBase1D(ArrayBase1D&& A) noexcept;
   STRICT_NODISCARD_CONSTEXPR ArrayBase1D(OneDimBaseType auto const& A);

   // assignments
   STRICT_CONSTEXPR ArrayBase1D& operator=(value_type x);
   STRICT_CONSTEXPR ArrayBase1D& operator=(std::initializer_list<value_type> list);
   STRICT_CONSTEXPR ArrayBase1D& operator=(const ArrayBase1D& A);
   STRICT_CONSTEXPR ArrayBase1D& operator=(ArrayBase1D&& A) noexcept;
   STRICT_CONSTEXPR ArrayBase1D& operator=(OneDimBaseType auto const& A);

   ~ArrayBase1D()
      requires(AF == Aligned);
   STRICT_CONSTEXPR ~ArrayBase1D()
      requires(AF == Unaligned);

   STRICT_CONSTEXPR void swap(ArrayBase1D& A) noexcept;
   STRICT_CONSTEXPR void swap(ArrayBase1D&& A) noexcept;

   STRICT_CONSTEXPR ArrayBase1D& resize(ImplicitInt n);
   STRICT_CONSTEXPR ArrayBase1D& resize_forget(ImplicitInt n);

   STRICT_CONSTEXPR ArrayBase1D& resize_and_assign(OneDimBaseType auto const& A);

   // optimized implementation
   STRICT_CONSTEXPR ArrayBase1D& resize_and_assign(ArrayBase1D&& A);

   STRICT_CONSTEXPR ArrayBase1D& remove(ImplicitInt p, ImplicitInt n = 1);
   STRICT_CONSTEXPR ArrayBase1D& remove(Pos p, Count n = Count{1});
   STRICT_CONSTEXPR ArrayBase1D& remove_front(ImplicitInt n = 1);
   STRICT_CONSTEXPR ArrayBase1D& remove_back(ImplicitInt n = 1);
   STRICT_CONSTEXPR ArrayBase1D& remove(internal::Last lst);
   STRICT_CONSTEXPR ArrayBase1D& remove(const std::vector<ImplicitInt>& indexes);

   STRICT_CONSTEXPR ArrayBase1D& insert(ImplicitInt p, value_type x);
   STRICT_CONSTEXPR ArrayBase1D& insert(Pos p, Value<builtin_type> x);
   STRICT_CONSTEXPR ArrayBase1D& insert_front(value_type x);
   STRICT_CONSTEXPR ArrayBase1D& insert_back(value_type x);

   STRICT_CONSTEXPR ArrayBase1D& insert(ImplicitInt p, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR ArrayBase1D& insert(Pos p, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR ArrayBase1D& insert_front(OneDimBaseType auto const& A);
   STRICT_CONSTEXPR ArrayBase1D& insert_back(OneDimBaseType auto const& A);

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& index(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& index(ImplicitInt i) const;

   STRICT_NODISCARD_CONSTEXPR value_type* data();
   STRICT_NODISCARD_CONSTEXPR const value_type* data() const;

   // converting to built-in types requires reinterpret_cast,
   // which cannot be done at compile time(not constexpr)
   STRICT_NODISCARD builtin_type* blas_data();
   STRICT_NODISCARD const builtin_type* blas_data() const;

private:
   value_type* data_;
   index_t n_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D() : data_{nullptr},
                                                               n_{} {
}


// align to 512 byte boundary for AVX-512
template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD ArrayBase1D<T, AF>::ArrayBase1D(ImplicitInt n)
   requires(AF == Aligned)
    : data_{nullptr},
      n_{n.get()} {
   ASSERT_STRICT_DEBUG(n_ > -1_sl);
   if(n_ != 0_sl) {
      data_ = new(std::align_val_t{512}) value_type[to_size_t(n_)];
   }
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(ImplicitInt n)
   requires(AF == Unaligned)
    : data_{nullptr},
      n_{n.get()} {
   ASSERT_STRICT_DEBUG(n_ > -1_sl);
   if(n_ != 0_sl) {
      data_ = new value_type[to_size_t(n_)];
   }
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(Size n) : ArrayBase1D(n.get()) {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(ImplicitInt n, Strict<T> x) : ArrayBase1D(n) {
   internal::fill(x, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(Size n, Value<T> x)
    : ArrayBase1D(n.get(), x.get()) {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(std::initializer_list<Strict<T>> list)
    : ArrayBase1D(from_size_t<long int>(list.size())) {
   internal::copy(list, *this);
}


// passes absolute value of e - b so that assertion e >= b can provide more detail
template <Builtin T, AlignmentFlag AF>
template <LinearIteratorType L>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(L b, L e) : ArrayBase1D(abss(Strict{e - b})) {
   ASSERT_STRICT_DEBUG(e >= b);
   internal::copy(b, e, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(const ArrayBase1D<T, AF>& A)
    : ArrayBase1D(A.size()) {
   internal::copy(A, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(ArrayBase1D<T, AF>&& A) noexcept
    : data_{std::exchange(A.data_, nullptr)},
      n_{std::exchange(A.n_, 0_sl)} {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(OneDimBaseType auto const& A)
    : ArrayBase1D(A.size()) {
   internal::copy(A, *this);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(Strict<T> x) {
   internal::fill(x, *this);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(std::initializer_list<Strict<T>> list) {
   ASSERT_STRICT_DEBUG(this->size() == from_size_t<long int>(list.size()));
   internal::copy(list, *this);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(const ArrayBase1D<T, AF>& A) {
   if(this != &A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      internal::copy(A, *this);
   }
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(ArrayBase1D<T, AF>&& A) noexcept {
   if(this != &A) {
      NORMAL_ASSERT_STRICT_DEBUG(same_size(*this, A));
      this->swap(A);
      A.swap(ArrayBase1D<T, AF>{});
   }
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::copy(A, *this);
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
ArrayBase1D<T, AF>::~ArrayBase1D()
   requires(AF == Aligned)
{
   operator delete[](data_, std::align_val_t{512});
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>::~ArrayBase1D()
   requires(AF == Unaligned)
{
   delete[] data_;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR void ArrayBase1D<T, AF>::swap(ArrayBase1D& A) noexcept {
   std::swap(data_, A.data_);
   std::swap(n_, A.n_);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR void ArrayBase1D<T, AF>::swap(ArrayBase1D&& A) noexcept {
   this->swap(A);
}


// implements strong exception guarantee
// preserves values of the remaining elements
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::resize(ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > -1_sl);

   if(auto n_new = n.get(); n_new != n_) {
      ArrayBase1D<T, AF> tmp(n_new);
      internal::copyn(*this, tmp, mins(n_, n_new));
      this->swap(tmp);
   }
   return *this;
}


// implements strong exception guarantee
// does not preserve values of the remaining elements
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::resize_forget(ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > -1_sl);

   if(auto n_new = n.get(); n_new != n_) {
      ArrayBase1D<T, AF> tmp(n_new);
      this->swap(tmp);
   }
   return *this;
}


// implements strong exception guarantee
// assignment should never throw, so strong exception
// guarantee for assignment is not necessary
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::resize_and_assign(OneDimBaseType auto const& A) {
   this->resize_forget(A.size());
   return *this = A;
}


// Similarly, move assignment does not throw
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::resize_and_assign(ArrayBase1D<T, AF>&& A) {
   this->resize_forget(A.size());
   return *this = std::move(A);
}


// implements strong exception guarantee
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::remove(ImplicitInt p, ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > 0_sl);
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, p.get()));
   ASSERT_STRICT_DEBUG(internal::valid_index(*this, p.get() + n.get() - 1_sl));

   ArrayBase1D<T, AF> tmp(this->size() - n.get());
   for(index_t j = 0_sl; j < p.get(); ++j) {
      tmp.index(j) = (*this).index(j);
   }
   for(index_t j = p.get(); j < this->size() - n.get(); ++j) {
      tmp.index(j) = (*this).index(j + n.get());
   }

   this->swap(tmp);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::remove(Pos p, Count n) {
   this->remove(p.get(), n.get());
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::remove_front(ImplicitInt n) {
   this->remove(0, n.get());
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::remove_back(ImplicitInt n) {
   this->remove(this->size() - n.get(), n.get());
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::remove(internal::Last lst) {
   this->remove(this->size() - 1_sl - lst.get(), 1);
   return *this;
}


// implements strong exception guarantee
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::remove(const std::vector<ImplicitInt>& indexes) {
   if(!indexes.empty()) {
      auto ci = internal::complement_index_vector(*this, indexes);
      ArrayBase1D<T, AF> tmp(from_size_t<long int>(ci.size()));

      for(index_t i = 0_sl; i < tmp.size(); ++i) {
         tmp.index(i) = (*this).index(ci[to_size_t(i)]);
      }
      this->swap(tmp);
   }

   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert(ImplicitInt p, value_type x) {
   ASSERT_STRICT_DEBUG(p.get() >= 0_sl && p.get() <= this->size());
   ArrayBase1D<T, AF> tmp(this->size() + 1_sl);

   for(index_t i = 0_sl; i < p.get(); ++i) {
      tmp.index(i) = (*this).index(i);
   }
   tmp.index(p.get()) = x;

   for(index_t i = p.get() + 1_sl; i < tmp.size(); ++i) {
      tmp.index(i) = (*this).index(i - 1_sl);
   }

   this->swap(tmp);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert(Pos p, Value<builtin_type> x) {
   return this->insert(p.get(), x.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert_front(value_type x) {
   return this->insert(0, x);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert_back(value_type x) {
   return this->insert(this->size(), x);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert(ImplicitInt p, OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(p.get() >= 0_sl && p.get() <= this->size());
   ArrayBase1D<T, AF> tmp(this->size() + A.size());

   for(index_t i = 0_sl; i < p.get(); ++i) {
      tmp.index(i) = (*this).index(i);
   }

   for(index_t i = 0_sl; i < A.size(); ++i) {
      tmp.index(i + p.get()) = A.index(i);
   }

   for(index_t i = 0_sl; i < tmp.size() - p.get() - A.size(); ++i) {
      tmp.index(i + p.get() + A.size()) = (*this).index(i + p.get());
   }

   this->swap(tmp);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert(Pos p, OneDimBaseType auto const& A) {
   return this->insert(p.get(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert_front(OneDimBaseType auto const& A) {
   return this->insert(0, A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::insert_back(OneDimBaseType auto const& A) {
   return this->insert(this->size(), A);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE index_t ArrayBase1D<T, AF>::size() const {
   return n_;
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T>& ArrayBase1D<T, AF>::index(ImplicitInt i) {
   return data_[i.get().val()];
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE const Strict<T>& ArrayBase1D<T, AF>::index(ImplicitInt i) const {
   return data_[i.get().val()];
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto ArrayBase1D<T, AF>::data() -> value_type* {
   return this->size() != 0_sl ? data_ : nullptr;
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto ArrayBase1D<T, AF>::data() const -> const value_type* {
   return this->size() != 0_sl ? data_ : nullptr;
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD auto ArrayBase1D<T, AF>::blas_data() -> builtin_type* {
   return reinterpret_cast<T*>(this->size() != 0_sl ? data_ : nullptr);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD auto ArrayBase1D<T, AF>::blas_data() const -> const builtin_type* {
   return reinterpret_cast<const T*>(this->size() != 0_sl ? data_ : nullptr);
}


}  // namespace slib
