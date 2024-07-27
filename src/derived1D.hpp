//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once

#include <initializer_list>  // initializer_list
#include <iterator>          // reverse_iterator
#include <type_traits>       // is_lvalue_reference_v
#include <utility>           // move, forward
#include <vector>            // vector

#include "array_base1D.hpp"
#include "fixed_array_base1D.hpp"
#include "iterator.hpp"
#include "slicearray_base1D.hpp"


namespace slib {


template <OneDimBaseType Base>
class Derived1D;


template <typename D> concept ArrayOneDimType = FixedArray1DType<D> || Array1DType<D>;

template <typename D> concept ArrayOneDimRealType = ArrayOneDimType<D> && OneDimRealBaseType<D>;

template <typename D> concept ArrayOneDimTypeRvalue = ArrayOneDimType<D> && !std::is_lvalue_reference_v<D>;

template <typename D> concept ArrayOneDimRealTypeRvalue
    = ArrayOneDimRealType<D> && !std::is_lvalue_reference_v<D>;


template <typename D> concept ArrayOneDimTypeRvalueWith
    = OneDimBaseType<RemoveRef<D>> && ArrayOneDimTypeRvalue<D>;

template <typename D> concept ArrayOneDimRealTypeRvalueWith
    = OneDimRealBaseType<RemoveRef<D>> && ArrayOneDimRealTypeRvalue<D>;

template <typename D> concept ArrayOneDimIntegerTypeRvalueWith
    = OneDimIntegerBaseType<RemoveRef<D>> && ArrayOneDimRealTypeRvalue<D>;

template <typename D> concept ArrayOneDimFloatTypeRvalueWith
    = OneDimFloatingBaseType<RemoveRef<D>> && ArrayOneDimRealTypeRvalue<D>;


template <OneDimBaseType Base>
class STRICT_NODISCARD Derived1D final : public Base {
private:
   using ThisType = Derived1D<Base>;

public:
   using Base::Base;

   using size_type = index_t;
   using typename Base::builtin_type;
   using typename Base::value_type;

   // assignments
   // assignment operators are not constrained because it leads to more obscure errors
   // for expression templates, which have a single deleted assignment
   STRICT_CONSTEXPR Derived1D& operator=(value_type x);
   STRICT_CONSTEXPR Derived1D& operator=(std::initializer_list<value_type> list);
   STRICT_CONSTEXPR Derived1D& operator=(OneDimBaseType auto const& A);

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   template <typename I>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator[](I i);

   template <typename I>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator[](I i) const;

   template <typename I>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(I i);

   template <typename I>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(I i) const;

   template <typename I>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) at(I i);

   template <typename I>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) at(I i) const;

   STRICT_NODISCARD_CONSTEXPR StrictBool empty() const;
   STRICT_NODISCARD_CONSTEXPR index_t size_m1() const;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_NODISCARD_CONSTEXPR auto begin() &;
   STRICT_NODISCARD_CONSTEXPR auto begin() const&;
   STRICT_NODISCARD_CONSTEXPR auto end() &;
   STRICT_NODISCARD_CONSTEXPR auto end() const&;
   STRICT_NODISCARD_CONSTEXPR auto cbegin() const&;
   STRICT_NODISCARD_CONSTEXPR auto cend() const&;

   STRICT_NODISCARD_CONSTEXPR auto rbegin() &;
   STRICT_NODISCARD_CONSTEXPR auto rbegin() const&;
   STRICT_NODISCARD_CONSTEXPR auto rend() &;
   STRICT_NODISCARD_CONSTEXPR auto rend() const&;
   STRICT_NODISCARD_CONSTEXPR auto crbegin() const&;
   STRICT_NODISCARD_CONSTEXPR auto crend() const&;

   // disallow pointers to temporaries to reduce the risk of dangling iterators
   STRICT_CONSTEXPR auto begin() && = delete;
   STRICT_CONSTEXPR auto begin() const&& = delete;
   STRICT_CONSTEXPR auto end() && = delete;
   STRICT_CONSTEXPR auto end() const&& = delete;
   STRICT_CONSTEXPR auto cbegin() const&& = delete;
   STRICT_CONSTEXPR auto cend() const&& = delete;

   STRICT_CONSTEXPR auto rbegin() && = delete;
   STRICT_CONSTEXPR auto rbegin() const&& = delete;
   STRICT_CONSTEXPR auto rend() && = delete;
   STRICT_CONSTEXPR auto rend() const&& = delete;
   STRICT_CONSTEXPR auto crbegin() const&& = delete;
   STRICT_CONSTEXPR auto crend() const&& = delete;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR Derived1D& operator+=(value_type x)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator-=(value_type x)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator*=(value_type x)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator/=(value_type x)
      requires NonConstBaseType<ThisType>;

   STRICT_CONSTEXPR Derived1D& operator%=(value_type x)
      requires NonConstBaseType<ThisType> && Integer<builtin_type>;
   STRICT_CONSTEXPR Derived1D& operator<<=(value_type x)
      requires NonConstBaseType<ThisType> && Integer<builtin_type>;
   STRICT_CONSTEXPR Derived1D& operator>>=(value_type x)
      requires NonConstBaseType<ThisType> && Integer<builtin_type>;
   STRICT_CONSTEXPR Derived1D& operator&=(value_type x)
      requires NonConstBaseType<ThisType> && Integer<builtin_type>;
   STRICT_CONSTEXPR Derived1D& operator|=(value_type x)
      requires NonConstBaseType<ThisType> && Integer<builtin_type>;
   STRICT_CONSTEXPR Derived1D& operator^=(value_type x)
      requires NonConstBaseType<ThisType> && Integer<builtin_type>;

   STRICT_CONSTEXPR Derived1D& operator+=(OneDimRealBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator-=(OneDimRealBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator*=(OneDimRealBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator/=(OneDimRealBaseType auto const& A)
      requires NonConstBaseType<ThisType>;

   STRICT_CONSTEXPR Derived1D& operator%=(OneDimIntegerBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator<<=(OneDimIntegerBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator>>=(OneDimIntegerBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator&=(OneDimIntegerBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator|=(OneDimIntegerBaseType auto const& A)
      requires NonConstBaseType<ThisType>;
   STRICT_CONSTEXPR Derived1D& operator^=(OneDimIntegerBaseType auto const& A)
      requires NonConstBaseType<ThisType>;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_NODISCARD_CONSTEXPR auto operator()(seqN s) &;

   template <typename S>
   STRICT_NODISCARD_CONSTEXPR auto operator()(S s) &;

   STRICT_NODISCARD_CONSTEXPR auto operator()(std::vector<ImplicitInt> indexes) &;
   STRICT_NODISCARD_CONSTEXPR auto operator()(std::initializer_list<ImplicitInt> indexes) &;
   STRICT_NODISCARD_CONSTEXPR auto operator()(const place::complement& indexes) &;

   STRICT_NODISCARD_CONSTEXPR auto view1D() &;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_NODISCARD_CONSTEXPR auto operator()(seqN s) const&;

   template <typename S>
   STRICT_NODISCARD_CONSTEXPR auto operator()(S s) const&;

   STRICT_NODISCARD_CONSTEXPR auto operator()(std::vector<ImplicitInt> indexes) const&;
   STRICT_NODISCARD_CONSTEXPR auto operator()(std::initializer_list<ImplicitInt> indexes) const&;
   STRICT_NODISCARD_CONSTEXPR auto operator()(const place::complement& indexes) const&;

   STRICT_NODISCARD_CONSTEXPR auto view1D() const&;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // must define for rvalues otherwise const& overload is selected, which returns
   // constant slice array in situations when non-constant slice array is needed
   STRICT_NODISCARD_CONSTEXPR auto operator()(seqN s) &&
      requires(!ArrayOneDimType<ThisType>);

   template <typename S>
   STRICT_NODISCARD_CONSTEXPR auto operator()(S s) &&
      requires(!ArrayOneDimType<ThisType>);

   STRICT_NODISCARD_CONSTEXPR auto operator()(std::vector<ImplicitInt> indexes) &&
      requires(!ArrayOneDimType<ThisType>);
   STRICT_NODISCARD_CONSTEXPR auto operator()(std::initializer_list<ImplicitInt> indexes) &&
      requires(!ArrayOneDimType<ThisType>);
   STRICT_NODISCARD_CONSTEXPR auto operator()(const place::complement& indexes) &&
      requires(!ArrayOneDimType<ThisType>);

   STRICT_NODISCARD_CONSTEXPR auto view1D() &&
      requires(!ArrayOneDimType<ThisType>);

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // disallow slicing of temporaries that own data to reduce the risk of dangling references
   template <typename S>
   STRICT_CONSTEXPR auto operator()(S s) const&&
      requires ArrayOneDimType<ThisType>
   = delete;

   STRICT_CONSTEXPR auto view1D() const&&
      requires ArrayOneDimType<ThisType>
   = delete;

   STRICT_CONSTEXPR Derived1D& lval() & = delete;
   STRICT_CONSTEXPR const Derived1D& lval() const& = delete;
   STRICT_NODISCARD_CONSTEXPR Derived1D& lval() &&;
   STRICT_NODISCARD_CONSTEXPR const Derived1D& lval() const&&;

   STRICT_NODISCARD_CONSTEXPR decltype(auto) eval() const&;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // pass through functions for ArrayBase1D class
   // although not strictly necessary, requires clause ensures it
   STRICT_CONSTEXPR Derived1D& resize(ImplicitInt n)
      requires Array1DType<ThisType>;


   STRICT_CONSTEXPR Derived1D& resize_forget(ImplicitInt n)
      requires Array1DType<ThisType>;


   // forwarding references are not used for resize_and_assign because passing arguments is more subtle
   STRICT_CONSTEXPR Derived1D& resize_and_assign(OneDimBaseType auto const& A)
      requires Array1DType<ThisType>;


   // optimized implementation
   STRICT_CONSTEXPR Derived1D& resize_and_assign(Derived1D&& A)
      requires Array1DType<ThisType>;


   template <typename... Args>
   STRICT_CONSTEXPR Derived1D& remove(Args&&... args)
      requires Array1DType<ThisType>;


   template <typename... Args>
   STRICT_CONSTEXPR Derived1D& remove_front(Args&&... args)
      requires Array1DType<ThisType>;


   template <typename... Args>
   STRICT_CONSTEXPR Derived1D& remove_back(Args&&... args)
      requires Array1DType<ThisType>;


   template <typename... Args>
   STRICT_CONSTEXPR Derived1D& insert(Args&&... args)
      requires Array1DType<ThisType>;


   template <typename... Args>
   STRICT_CONSTEXPR Derived1D& insert_front(Args&&... args)
      requires Array1DType<ThisType>;


   template <typename... Args>
   STRICT_CONSTEXPR Derived1D& insert_back(Args&&... args)
      requires Array1DType<ThisType>;


   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_NODISCARD_CONSTEXPR StrictLong bytes() const
      requires ArrayOneDimType<ThisType>;


   STRICT_NODISCARD_CONSTEXPR Strict64 kbytes() const
      requires ArrayOneDimType<ThisType>;


   STRICT_NODISCARD_CONSTEXPR Strict64 mbytes() const
      requires ArrayOneDimType<ThisType>;


   STRICT_NODISCARD_CONSTEXPR Strict64 gbytes() const
      requires ArrayOneDimType<ThisType>;


   STRICT_NODISCARD_CONSTEXPR static index_t dimension() {
      return 1_sl;
   }

private:
   STRICT_CONSTEXPR Derived1D& lval_impl();
   STRICT_CONSTEXPR const Derived1D& lval_impl() const;
};


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator=(value_type x) {
   Base::operator=(x);
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator=(std::initializer_list<value_type> list) {
   Base::operator=(list);
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator=(OneDimBaseType auto const& A) {
   Base::operator=(A);
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
template <typename I>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) Derived1D<Base>::operator[](I i) {
   ASSERT_STRICT_RANGE_DEBUG(internal::valid_index(*this, internal::index_helper(*this, i)));
   return Base::index(internal::index_helper(*this, i));
}


template <OneDimBaseType Base>
template <typename I>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) Derived1D<Base>::operator[](I i) const {
   ASSERT_STRICT_RANGE_DEBUG(internal::valid_index(*this, internal::index_helper(*this, i)));
   return Base::index(internal::index_helper(*this, i));
}


template <OneDimBaseType Base>
template <typename I>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) Derived1D<Base>::un(I i) {
   return Base::index(internal::index_helper(*this, i));
}


template <OneDimBaseType Base>
template <typename I>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) Derived1D<Base>::un(I i) const {
   return Base::index(internal::index_helper(*this, i));
}


template <OneDimBaseType Base>
template <typename I>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) Derived1D<Base>::at(I i) {
   ASSERT_STRICT_RANGE_ALWAYS(internal::valid_index(*this, internal::index_helper(*this, i)));
   return Base::index(internal::index_helper(*this, i));
}


template <OneDimBaseType Base>
template <typename I>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) Derived1D<Base>::at(I i) const {
   ASSERT_STRICT_RANGE_ALWAYS(internal::valid_index(*this, internal::index_helper(*this, i)));
   return Base::index(internal::index_helper(*this, i));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR StrictBool Derived1D<Base>::empty() const {
   return Base::size() == 0_sl;
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::size_m1() const -> index_t {
   return Base::size() - 1_sl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::begin() & {
   if constexpr(NonConstBaseType<Base>) {
      return Iterator{*this, 0_sl};
   } else {
      return ConstIterator{*this, 0_sl};
   }
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::begin() const& {
   return ConstIterator{*this, 0_sl};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::end() & {
   if constexpr(NonConstBaseType<Base>) {
      return Iterator{*this, Base::size()};
   } else {
      return ConstIterator{*this, Base::size()};
   }
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::end() const& {
   return ConstIterator{*this, Base::size()};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::cbegin() const& {
   return this->begin();
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::cend() const& {
   return this->end();
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::rbegin() & {
   return std::reverse_iterator{this->end()};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::rbegin() const& {
   return std::reverse_iterator{this->end()};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::rend() & {
   return std::reverse_iterator{this->begin()};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::rend() const& {
   return std::reverse_iterator{this->begin()};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::crbegin() const& {
   return std::reverse_iterator{this->rbegin()};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::crend() const& {
   return std::reverse_iterator{this->rend()};
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator+=(value_type x)
   requires NonConstBaseType<ThisType>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) += x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator-=(value_type x)
   requires NonConstBaseType<ThisType>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) -= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator*=(value_type x)
   requires NonConstBaseType<ThisType>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) *= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator/=(value_type x)
   requires NonConstBaseType<ThisType>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) /= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator%=(value_type x)
   requires NonConstBaseType<ThisType> && Integer<builtin_type>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) %= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator<<=(value_type x)
   requires NonConstBaseType<ThisType> && Integer<builtin_type>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) <<= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator>>=(value_type x)
   requires NonConstBaseType<ThisType> && Integer<builtin_type>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) >>= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator&=(value_type x)
   requires NonConstBaseType<ThisType> && Integer<builtin_type>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) &= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator|=(value_type x)
   requires NonConstBaseType<ThisType> && Integer<builtin_type>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) |= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator^=(value_type x)
   requires NonConstBaseType<ThisType> && Integer<builtin_type>
{
   internal::apply0(*this, [x, this](index_t i) { Base::index(i) ^= x; });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator+=(OneDimRealBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) += A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator-=(OneDimRealBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) -= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator*=(OneDimRealBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) *= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator/=(OneDimRealBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) /= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator%=(OneDimIntegerBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) %= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator<<=(OneDimIntegerBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) <<= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator>>=(OneDimIntegerBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) >>= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator&=(OneDimIntegerBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) &= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator|=(OneDimIntegerBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) |= A.index(i); });
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::operator^=(OneDimIntegerBaseType auto const& A)
   requires NonConstBaseType<ThisType>
{
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::apply1(*this, A, [&](index_t i) { Base::index(i) ^= A.index(i); });
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(seqN s) & {
   ASSERT_STRICT_DEBUG(s.valid(*this));
   if constexpr(NonConstBaseType<Base>) {
      return Derived1D<SliceArrayBase1D<ThisType>>{*this, s};
   } else {
      return Derived1D<ConstSliceArrayBase1D<ThisType>>{*this, s};
   }
}


template <OneDimBaseType Base>
template <typename S>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(S s) & {
   return operator()(internal::slice_helper(*this, s));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(std::vector<ImplicitInt> indexes) & {
   if constexpr(NonConstBaseType<Base>) {
      return Derived1D<RandSliceArrayBase1D<ThisType>>{*this, std::move(indexes)};
   } else {
      return Derived1D<RandConstSliceArrayBase1D<ThisType>>{*this, std::move(indexes)};
   }
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(std::initializer_list<ImplicitInt> indexes) & {
   return operator()(std::vector<ImplicitInt>(indexes));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(const place::complement& indexes) & {
   return operator()(internal::complement_index_vector(*this, indexes.get()));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::view1D() & {
   return operator()(place::all);
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(seqN s) const& {
   ASSERT_STRICT_DEBUG(s.valid(*this));
   return Derived1D<ConstSliceArrayBase1D<ThisType>>{*this, s};
}


template <OneDimBaseType Base>
template <typename S>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(S s) const& {
   return operator()(internal::slice_helper(*this, s));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(std::vector<ImplicitInt> indexes) const& {
   return Derived1D<RandConstSliceArrayBase1D<ThisType>>{*this, std::move(indexes)};
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(
    std::initializer_list<ImplicitInt> indexes) const& {
   return operator()(std::vector<ImplicitInt>{indexes});
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(const place::complement& indexes) const& {
   return operator()(complement_index_vector(*this, indexes.get()));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::view1D() const& {
   return operator()(place::all);
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(seqN s) &&
   requires(!ArrayOneDimType<ThisType>)
{
   return operator()(s);
}


template <OneDimBaseType Base>
template <typename S>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(S s) &&
   requires(!ArrayOneDimType<ThisType>)
{
   return operator()(s);
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(std::vector<ImplicitInt> indexes) &&
   requires(!ArrayOneDimType<ThisType>)
{
   return operator()(std::move(indexes));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(std::initializer_list<ImplicitInt> indexes) &&
   requires(!ArrayOneDimType<ThisType>)
{
   return operator()(indexes);
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::operator()(const place::complement& indexes) &&
   requires(!ArrayOneDimType<ThisType>)
{
   return operator()(indexes);
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto Derived1D<Base>::view1D() &&
   requires(!ArrayOneDimType<ThisType>)
{
   return this->view1D();
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR Derived1D<Base>& Derived1D<Base>::lval() && {
   return this->lval_impl();
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR const Derived1D<Base>& Derived1D<Base>::lval() const&& {
   return this->lval_impl();
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR decltype(auto) Derived1D<Base>::eval() const& {
   if constexpr(ArrayOneDimType<Base>) {
      return *this;
   } else {
      return Derived1D<ArrayBase1D<builtin_type, Unaligned>>(*this);
   }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::resize(ImplicitInt n)
   requires Array1DType<ThisType>
{
   Base::resize(n);
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::resize_forget(ImplicitInt n)
   requires Array1DType<ThisType>
{
   Base::resize_forget(n);
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::resize_and_assign(OneDimBaseType auto const& A)
   requires Array1DType<ThisType>
{
   Base::resize_and_assign(A);
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::resize_and_assign(Derived1D<Base>&& A)
   requires Array1DType<ThisType>
{
   Base::resize_and_assign(static_cast<Base&&>(A));
   return *this;
}


template <OneDimBaseType Base>
template <typename... Args>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::remove(Args&&... args)
   requires Array1DType<ThisType>
{
   Base::remove(std::forward<Args>(args)...);
   return *this;
}


template <OneDimBaseType Base>
template <typename... Args>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::remove_front(Args&&... args)
   requires Array1DType<ThisType>
{
   Base::remove_front(std::forward<Args>(args)...);
   return *this;
}


template <OneDimBaseType Base>
template <typename... Args>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::remove_back(Args&&... args)
   requires Array1DType<ThisType>
{
   Base::remove_back(std::forward<Args>(args)...);
   return *this;
}


template <OneDimBaseType Base>
template <typename... Args>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::insert(Args&&... args)
   requires Array1DType<ThisType>
{
   Base::insert(std::forward<Args>(args)...);
   return *this;
}


template <OneDimBaseType Base>
template <typename... Args>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::insert_front(Args&&... args)
   requires Array1DType<ThisType>
{
   Base::insert_front(std::forward<Args>(args)...);
   return *this;
}


template <OneDimBaseType Base>
template <typename... Args>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::insert_back(Args&&... args)
   requires Array1DType<ThisType>
{
   Base::insert_back(std::forward<Args>(args)...);
   return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR StrictLong Derived1D<Base>::bytes() const
   requires ArrayOneDimType<ThisType>
{
   return Base::size() * from_size_t<long int>(sizeof(value_type));
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR Strict64 Derived1D<Base>::kbytes() const
   requires ArrayOneDimType<ThisType>
{
   return this->bytes().sd() / 1024._sd;
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR Strict64 Derived1D<Base>::mbytes() const
   requires ArrayOneDimType<ThisType>
{
   return this->bytes().sd() / squares(1024_sl).sd();
}


template <OneDimBaseType Base>
STRICT_NODISCARD_CONSTEXPR Strict64 Derived1D<Base>::gbytes() const
   requires ArrayOneDimType<ThisType>
{
   return this->bytes().sd() / cubes(1024_sl).sd();
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR Derived1D<Base>& Derived1D<Base>::lval_impl() {
   return *this;
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR const Derived1D<Base>& Derived1D<Base>::lval_impl() const {
   return *this;
}


}  // namespace slib


namespace slib {


template <Builtin T, AlignmentFlag AF = Aligned>
using Array1D = Derived1D<ArrayBase1D<T, AF>>;


template <Builtin T, ImplicitIntStatic sz>
using FixedArray1D = Derived1D<FixedArrayBase1D<T, sz>>;


}  // namespace slib
