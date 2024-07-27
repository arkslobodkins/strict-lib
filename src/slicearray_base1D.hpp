//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once


#include <initializer_list>
#include <utility>
#include <vector>

#include "slice.hpp"


namespace slib {


// Move assignments are not defined for slice arrays since
// moving members would violate slicing semantics. Further,
// for linear slices none of the members are moveable.


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// note that Base can be either one or two-dimensional
template <NonConstBaseType Base>
class STRICT_NODISCARD SliceArrayBase1D : private CopyBase1D {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit SliceArrayBase1D(Base& A, seqN sl);

   STRICT_NODISCARD_CONSTEXPR SliceArrayBase1D(const SliceArrayBase1D& A) = default;
   STRICT_CONSTEXPR SliceArrayBase1D& operator=(const SliceArrayBase1D& A);
   STRICT_CONSTEXPR ~SliceArrayBase1D() = default;

   STRICT_CONSTEXPR SliceArrayBase1D& operator=(value_type x);
   STRICT_CONSTEXPR SliceArrayBase1D& operator=(std::initializer_list<value_type> list);
   STRICT_CONSTEXPR SliceArrayBase1D& operator=(OneDimBaseType auto const& A);

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& index(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& index(ImplicitInt i) const;

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const;

private:
   typename CopyOrReferenceExpr<Base>::type A_;
   seqN sl_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR SliceArrayBase1D<Base>::SliceArrayBase1D(Base& A, seqN sl) : A_{A},
                                                                                         sl_{sl} {
   ASSERT_STRICT_DEBUG(sl_.valid(A_));
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR SliceArrayBase1D<Base>& SliceArrayBase1D<Base>::operator=(value_type x) {
   internal::fill(x, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR SliceArrayBase1D<Base>& SliceArrayBase1D<Base>::operator=(
    std::initializer_list<value_type> list) {
   ASSERT_STRICT_DEBUG(size() == from_size_t<long int>(list.size()));
   internal::copy(list, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR SliceArrayBase1D<Base>& SliceArrayBase1D<Base>::operator=(const SliceArrayBase1D<Base>& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::copy(A, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR SliceArrayBase1D<Base>& SliceArrayBase1D<Base>::operator=(OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::copy(A, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase1D<Base>::index(ImplicitInt i) -> value_type& {
   auto ind = i.get();
   return A_.index(sl_.start() + ind * sl_.stride());
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase1D<Base>::index(ImplicitInt i) const
    -> const value_type& {
   auto ind = i.get();
   return A_.index(sl_.start() + ind * sl_.stride());
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE index_t SliceArrayBase1D<Base>::size() const {
   return sl_.size();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// note that Base can be either one or two-dimensional
template <BaseType Base>
class STRICT_NODISCARD ConstSliceArrayBase1D : private CopyBase1D, private ConstSliceBase {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit ConstSliceArrayBase1D(const Base& A, seqN sl);

   STRICT_NODISCARD_CONSTEXPR ConstSliceArrayBase1D(const ConstSliceArrayBase1D& A) = default;
   STRICT_CONSTEXPR ConstSliceArrayBase1D& operator=(const ConstSliceArrayBase1D& A) = delete;
   STRICT_CONSTEXPR ~ConstSliceArrayBase1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) index(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const;

private:
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   seqN sl_;
};


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR ConstSliceArrayBase1D<Base>::ConstSliceArrayBase1D(const Base& A, seqN sl)
    : A_{A},
      sl_{sl} {
   ASSERT_STRICT_DEBUG(sl_.valid(A_));
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) ConstSliceArrayBase1D<Base>::index(ImplicitInt i) const {
   auto ind = i.get();
   return A_.index(sl_.start() + ind * sl_.stride());
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE index_t ConstSliceArrayBase1D<Base>::size() const {
   return sl_.size();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// note that Base can be either one or two-dimensional
template <NonConstBaseType Base>
class STRICT_NODISCARD RandSliceArrayBase1D : private CopyBase1D {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit RandSliceArrayBase1D(Base& A, std::vector<ImplicitInt>&& indexes);

   STRICT_NODISCARD_CONSTEXPR RandSliceArrayBase1D(const RandSliceArrayBase1D& A) = default;
   STRICT_NODISCARD_CONSTEXPR RandSliceArrayBase1D(RandSliceArrayBase1D&& A) = default;
   STRICT_CONSTEXPR RandSliceArrayBase1D& operator=(const RandSliceArrayBase1D& A);
   // move assignment does not have proper semantics for
   // slice arrays and therefore falls back to copy assignment
   STRICT_CONSTEXPR ~RandSliceArrayBase1D() = default;

   STRICT_CONSTEXPR RandSliceArrayBase1D& operator=(value_type x);
   STRICT_CONSTEXPR RandSliceArrayBase1D& operator=(std::initializer_list<value_type> list);
   STRICT_CONSTEXPR RandSliceArrayBase1D& operator=(OneDimBaseType auto const& A);

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& index(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& index(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const;
   STRICT_NODISCARD_CONSTEXPR const auto& indexes() const&;
   STRICT_NODISCARD_CONSTEXPR auto indexes() &&;

private:
   typename CopyOrReferenceExpr<Base>::type A_;
   std::vector<ImplicitInt> indexes_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR RandSliceArrayBase1D<Base>::RandSliceArrayBase1D(
    Base& A, std::vector<ImplicitInt>&& indexes)
    : A_{A},
      indexes_{std::move(indexes)} {
   ASSERT_STRICT_DEBUG(internal::valid_slice_vector(A, indexes_));
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR RandSliceArrayBase1D<Base>& RandSliceArrayBase1D<Base>::operator=(value_type x) {
   internal::fill(x, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR RandSliceArrayBase1D<Base>& RandSliceArrayBase1D<Base>::operator=(
    std::initializer_list<value_type> list) {
   ASSERT_STRICT_DEBUG(size() == from_size_t<long int>(list.size()));
   internal::copy(list, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR RandSliceArrayBase1D<Base>& RandSliceArrayBase1D<Base>::operator=(
    const RandSliceArrayBase1D<Base>& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::copy(A, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_CONSTEXPR RandSliceArrayBase1D<Base>& RandSliceArrayBase1D<Base>::operator=(
    OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   internal::copy(A, *this);
   return *this;
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE auto RandSliceArrayBase1D<Base>::index(ImplicitInt i) -> value_type& {
   return A_.index(indexes_[to_size_t(i.get())]);
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE auto RandSliceArrayBase1D<Base>::index(ImplicitInt i) const
    -> const value_type& {
   return A_.index(indexes_[to_size_t(i.get())]);
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE index_t RandSliceArrayBase1D<Base>::size() const {
   return from_size_t<long int>(indexes_.size());
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR const auto& RandSliceArrayBase1D<Base>::indexes() const& {
   return indexes_;
}


template <NonConstBaseType Base>
STRICT_NODISCARD_CONSTEXPR auto RandSliceArrayBase1D<Base>::indexes() && {
   return indexes_;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// note that Base can be either one or two-dimensional
template <BaseType Base>
class STRICT_NODISCARD RandConstSliceArrayBase1D : private CopyBase1D, private ConstSliceBase {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit RandConstSliceArrayBase1D(const Base& A,
                                                                 std::vector<ImplicitInt>&& indexes);

   STRICT_NODISCARD_CONSTEXPR RandConstSliceArrayBase1D(const RandConstSliceArrayBase1D& A) = default;
   STRICT_NODISCARD_CONSTEXPR RandConstSliceArrayBase1D(RandConstSliceArrayBase1D&& A) = default;
   STRICT_CONSTEXPR RandConstSliceArrayBase1D& operator=(const RandConstSliceArrayBase1D& A) = delete;
   STRICT_CONSTEXPR RandConstSliceArrayBase1D& operator=(RandConstSliceArrayBase1D&& A) = delete;
   STRICT_CONSTEXPR ~RandConstSliceArrayBase1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) index(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const;
   STRICT_NODISCARD_CONSTEXPR const auto& indexes() const&;
   STRICT_NODISCARD_CONSTEXPR auto indexes() &&;

private:
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   std::vector<ImplicitInt> indexes_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR RandConstSliceArrayBase1D<Base>::RandConstSliceArrayBase1D(
    const Base& A, std::vector<ImplicitInt>&& indexes)
    : A_{A},
      indexes_{std::move(indexes)} {
   ASSERT_STRICT_DEBUG(internal::valid_slice_vector(A, indexes_));
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR decltype(auto) RandConstSliceArrayBase1D<Base>::index(ImplicitInt i) const {
   return A_.index(indexes_[to_size_t(i.get())]);
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR_INLINE index_t RandConstSliceArrayBase1D<Base>::size() const {
   return from_size_t<long int>(indexes_.size());
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR const auto& RandConstSliceArrayBase1D<Base>::indexes() const& {
   return indexes_;
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR auto RandConstSliceArrayBase1D<Base>::indexes() && {
   return indexes_;
}


}  // namespace slib
