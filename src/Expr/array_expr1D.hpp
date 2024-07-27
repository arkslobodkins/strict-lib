//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
//
//  This file is part of the strict-lib library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//


#pragma once


#include <utility>

#include "../derived1D.hpp"
#include "exclude_last.hpp"
#include "functors.hpp"


namespace slib {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// special operations
template <OneDimBaseType Base, OneDimBaseType... BArgs>
STRICT_CONSTEXPR auto merge(const Base& A, const BArgs&... AArgs);


template <OneDimBaseType Base1, OneDimBaseType Base2>
STRICT_CONSTEXPR auto merge(const Base1& A1, const Base2& A2);


template <OneDimBaseType Base, StrictType S, StrictType... SArgs>
STRICT_CONSTEXPR auto merge(const Base& A, S x, SArgs... xargs);


template <OneDimBaseType Base, StrictType S>
STRICT_CONSTEXPR auto merge(const Base& A, S x);


// cannot separate values from OneDimBaseType, so one must use Other&...
template <StrictType S1, StrictType S2, typename... Other>
   requires OneDimBaseType<LastPack_t<Other...>> && AllStrictExceptLast<Other...>
STRICT_CONSTEXPR auto merge(S1 x, S2 y, const Other&... A);


template <StrictType S, OneDimBaseType Base>
STRICT_CONSTEXPR auto merge(S x, const Base& A);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct IsArrayOneDimTypeRvalueWith {
   static constexpr bool value = ArrayOneDimTypeRvalueWith<T>;
};


template <typename... Args> concept OneOfArrayOneDimTypeRvalueWith
    = std::disjunction_v<IsArrayOneDimTypeRvalueWith<Args>...>;


template <typename... Args>
   requires OneOfArrayOneDimTypeRvalueWith<Args...>
STRICT_CONSTEXPR auto merge(Args&&... args) = delete;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, ImplicitInt p, ImplicitInt n = 1);


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, Pos p, Count n = Count{1});


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deleted overloads
template <typename Base>
   requires ArrayOneDimTypeRvalueWith<Base>
STRICT_CONSTEXPR auto exclude(Base&& A, ImplicitInt p, ImplicitInt n = 1) = delete;


template <typename Base>
   requires ArrayOneDimTypeRvalueWith<Base>
STRICT_CONSTEXPR auto exclude(Base&& A, Pos p, Count n = Count{1}) = delete;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_CONSTEXPR auto sequence(ImplicitInt size, Strict<T> start = Strict<T>{},
                               Strict<T> incr = Strict{T(1)});


template <Real T>
STRICT_CONSTEXPR auto sequence(Size size, Start<T> start = Start<T>{}, Incr<T> incr = Incr{T(1)});


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_CONSTEXPR auto linspace(ImplicitInt size, Strict<T> start, Strict<T> end);


template <Floating T>
STRICT_CONSTEXPR auto linspace(Size size, Start<T> start, End<T> end);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// not a template, so it must be inlined
STRICT_CONSTEXPR_INLINE auto irange(ImplicitInt n);


// overloads for irange do not need to be deleted if A is rvalue
// of ArrayType because it only stores information about its size
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange(const Base& A);


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange_m1(const Base& A);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates standard unit vector on the fly.
// Involves redundant work if assigned to Arrays that only contain zeros.
// empty objects are not allowed(must be of at least size 1)
template <Real T>
STRICT_CONSTEXPR auto e_unit(ImplicitInt unit_index, ImplicitInt size);


template <Real T>
STRICT_CONSTEXPR auto e_unit(Index unit_index, Size size);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_CONSTEXPR auto const1D(ImplicitInt size, Strict<T> c);


template <Builtin T>
STRICT_CONSTEXPR auto const1D(Size size, Value<T> c);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// unary operations
template <OneDimBaseType Base, typename F, bool copy_delete = false>
   requires UnaryOperation<Base, F>
STRICT_CONSTEXPR auto generate1D(const Base& A, F f);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator~(const Base& A);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto abs(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto exp(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log2(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log10(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sqrt(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cbrt(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sin(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cos(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto tan(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow(const Base& A, ValueTypeOf<Base> p);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow_int(const Base& A, ImplicitInt p);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto fast_pow_int(const Base& A, ImplicitInt p);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto inv(const Base& A);


template <Builtin T, OneDimBaseType Base>
STRICT_CONSTEXPR auto array_cast(const Base& A);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deleted overloads
template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator+(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator-(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator~(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto abs(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto exp(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto log(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto log2(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto log10(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto sqrt(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto cbrt(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto sin(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto cos(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto tan(Base&& A) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto pow(Base&& A, ValueTypeOf<Base> p) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR_2026 auto pow_int(Base&& A, ImplicitInt p) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR auto fast_pow_int(Base&& A, ImplicitInt p) = delete;


template <typename Base>
   requires ArrayOneDimFloatTypeRvalueWith<Base>
STRICT_CONSTEXPR auto inv(Base&& A) = delete;


template <Builtin T, typename Base>
   requires ArrayOneDimTypeRvalueWith<Base>
STRICT_CONSTEXPR auto array_cast(Base&& A) = delete;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// binary operations
template <OneDimBaseType Base1, OneDimBaseType Base2, typename F, bool copy_delete = false>
   requires BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate1D(const Base1& A1, const Base2& A2, F f);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2);


template <OneDimFloatingBaseType Base1, OneDimFloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deleted overloads
template <typename Base1, typename Base2>
   requires(OneDimRealBaseType<RemoveRef<Base1>> && OneDimRealBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimRealTypeRvalueWith<Base1> || ArrayOneDimRealTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator+(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimRealBaseType<RemoveRef<Base1>> && OneDimRealBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimRealTypeRvalueWith<Base1> || ArrayOneDimRealTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator-(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimRealBaseType<RemoveRef<Base1>> && OneDimRealBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimRealTypeRvalueWith<Base1> || ArrayOneDimRealTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator*(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimRealBaseType<RemoveRef<Base1>> && OneDimRealBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimRealTypeRvalueWith<Base1> || ArrayOneDimRealTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator/(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimIntegerBaseType<RemoveRef<Base1>> && OneDimIntegerBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimIntegerTypeRvalueWith<Base1> || ArrayOneDimIntegerTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator%(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimIntegerBaseType<RemoveRef<Base1>> && OneDimIntegerBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimIntegerTypeRvalueWith<Base1> || ArrayOneDimIntegerTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator<<(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimIntegerBaseType<RemoveRef<Base1>> && OneDimIntegerBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimIntegerTypeRvalueWith<Base1> || ArrayOneDimIntegerTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator>>(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimIntegerBaseType<RemoveRef<Base1>> && OneDimIntegerBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimIntegerTypeRvalueWith<Base1> || ArrayOneDimIntegerTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator&(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimIntegerBaseType<RemoveRef<Base1>> && OneDimIntegerBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimIntegerTypeRvalueWith<Base1> || ArrayOneDimIntegerTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator|(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimIntegerBaseType<RemoveRef<Base1>> && OneDimIntegerBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimIntegerTypeRvalueWith<Base1> || ArrayOneDimIntegerTypeRvalueWith<Base2>)
STRICT_CONSTEXPR auto operator^(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(OneDimFloatingBaseType<RemoveRef<Base1>> && OneDimFloatingBaseType<RemoveRef<Base2>>)
            && (ArrayOneDimFloatTypeRvalueWith<Base1> || ArrayOneDimFloatTypeRvalueWith<Base2>)
auto two_prod(Base1&& A1, Base2&& A2) = delete;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A2);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A2);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A2);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A2);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deleted overloads
template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, Base&& A2) = delete;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deleted overloads
template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator+(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator-(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator*(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimRealTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator/(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator%(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator<<(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator>>(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator&(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator|(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires ArrayOneDimIntegerTypeRvalueWith<Base>
STRICT_CONSTEXPR auto operator^(Base&& A, ValueTypeOf<Base> x) = delete;


namespace internal {
// workaround before CWG2518/P2593R1
template <typename>
constexpr bool static_false = false;

}  // namespace internal


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base, typename Op, bool copy_delete = false>
   requires UnaryOperation<Base, Op>
class STRICT_NODISCARD UnaryExpr1D : private CopyBase1D {
public:
   // value_type is not always the same as ValueTypeOf<Base>. For example,
   // when converting array to a different type
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit UnaryExpr1D(const Base& A, Op op) : A_{A}, op_{op} {
   }

   STRICT_NODISCARD_CONSTEXPR UnaryExpr1D(const UnaryExpr1D& E) : A_{E.A_}, op_{E.op_} {
      if constexpr(copy_delete) {
         static_assert(internal::static_false<decltype(*this)>,
                       "Copying this expression template is not allowed for additional safety");
      }
   }

   STRICT_CONSTEXPR UnaryExpr1D& operator=(const UnaryExpr1D&) = delete;
   STRICT_CONSTEXPR ~UnaryExpr1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type index(ImplicitInt i) const {
      return op_(A_.index(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A_.size();
   }

private:
   // slice arrays are stored by copy, arrays by reference
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   Op op_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base1, OneDimBaseType Base2, typename Op, bool copy_delete = false>
   requires BinaryOperation<Base1, Base2, Op>
class STRICT_NODISCARD BinaryExpr1D : private CopyBase1D {
public:
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base1>{}, ValueTypeOf<Base2>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit BinaryExpr1D(const Base1& A1, const Base2& A2, Op op)
       : A1_{A1},
         A2_{A2},
         op_{op} {
      ASSERT_STRICT_DEBUG(same_size(A1_, A2_));
   }

   STRICT_NODISCARD_CONSTEXPR BinaryExpr1D(const BinaryExpr1D& E) : A1_{E.A1_}, A2_{E.A2_}, op_{E.op_} {
      if constexpr(copy_delete) {
         static_assert(internal::static_false<decltype(*this)>,
                       "Copying this expression template is not allowed for additional safety");
      }
   }

   STRICT_CONSTEXPR BinaryExpr1D& operator=(const BinaryExpr1D&) = delete;
   STRICT_CONSTEXPR ~BinaryExpr1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type index(ImplicitInt i) const {
      return op_(A1_.index(i), A2_.index(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A1_.size();
   }

private:
   // slice arrays are stored by copy, arrays by reference
   typename CopyOrReferenceExpr<AddConst<Base1>>::type A1_;
   typename CopyOrReferenceExpr<AddConst<Base2>>::type A2_;
   Op op_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
class STRICT_NODISCARD SequenceExpr1D : private CopyBase1D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   // incr can be 0 or negative
   STRICT_NODISCARD_CONSTEXPR explicit SequenceExpr1D(value_type start, index_t size, value_type incr)
       : start_{start},
         size_{size},
         incr_{incr} {
      ASSERT_STRICT_DEBUG(size_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR SequenceExpr1D(const SequenceExpr1D&) = default;
   STRICT_CONSTEXPR SequenceExpr1D& operator=(const SequenceExpr1D&) = delete;
   STRICT_CONSTEXPR ~SequenceExpr1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type index(ImplicitInt i) const {
      return start_ + incr_ * strict_cast<builtin_type>(i.get());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return size_;
   }

private:
   value_type start_;
   index_t size_;
   value_type incr_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base, typename F, bool copy_delete>
   requires UnaryOperation<Base, F>
STRICT_CONSTEXPR auto generate1D(const Base& A, F f) {
   return Derived1D<UnaryExpr1D<Base, F, copy_delete>>{A, f};
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A) {
   return generate1D(A, UnaryPlus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A) {
   return generate1D(A, UnaryMinus{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator~(const Base& A) {
   return generate1D(A, UnaryNot{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto abs(const Base& A) {
   return generate1D(A, UnaryAbs{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto exp(const Base& A) {
   return generate1D(A, UnaryExp{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log(const Base& A) {
   return generate1D(A, UnaryLog{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log2(const Base& A) {
   return generate1D(A, UnaryLog2{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log10(const Base& A) {
   return generate1D(A, UnaryLog10{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sqrt(const Base& A) {
   return generate1D(A, UnarySqrt{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cbrt(const Base& A) {
   return generate1D(A, UnaryCbrt{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sin(const Base& A) {
   return generate1D(A, UnarySin{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cos(const Base& A) {
   return generate1D(A, UnaryCos{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto tan(const Base& A) {
   return generate1D(A, UnaryTan{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow(const Base& A, ValueTypeOf<Base> p) {
   return generate1D(A, UnaryPow{p});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow_int(const Base& A, ImplicitInt p) {
   return generate1D(A, UnaryPow{strict_cast<BuiltinTypeOf<Base>>(p.get())});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto fast_pow_int(const Base& A, ImplicitInt p) {
   return generate1D(A, UnaryFastPowInt{p.get()});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto inv(const Base& A) {
   return generate1D(A, UnaryInv{});
}


template <Builtin T, OneDimBaseType Base>
STRICT_CONSTEXPR auto array_cast(const Base& A) {
   return generate1D(A, UnaryCast<T>{});
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base1, OneDimBaseType Base2, typename F, bool copy_delete>
   requires BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate1D(const Base1& A1, const Base2& A2, F f) {
   return Derived1D<BinaryExpr1D<Base1, Base2, F, copy_delete>>(A1, A2, f);
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryPlus{});
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryMinus{});
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryMult{});
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryDivide{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryModulo{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryRightShift{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryLeftShift{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryBitwiseAnd{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryBitwiseOr{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, BinaryBitwiseXor{});
}


template <OneDimFloatingBaseType Base1, OneDimFloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2) {
   return std::pair{generate1D(A1, A2, BinaryTwoProdFirst{}), generate1D(A1, A2, BinaryTwoProdSecond{})};
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryPlus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryMinus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryMult{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryDivide{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryModulo{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryRightShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryLeftShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryBitwiseAnd{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryBitwiseOr{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, BinaryBitwiseXor{});
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryPlus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryMinus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryMult{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryDivide{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryModulo{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryRightShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryLeftShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryBitwiseAnd{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryBitwiseOr{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), BinaryBitwiseXor{});
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base, OneDimBaseType... BArgs>
STRICT_CONSTEXPR auto merge(const Base& A, const BArgs&... AArgs) {
   auto AE = generate1D(A, [](auto x) { return x; });
   auto ArgsE = merge(AArgs...);
   auto op = [AE, ArgsE](auto i) { return i < AE.size() ? AE.index(i) : ArgsE.index(i - AE.size()); };
   return generate1D(irange(AE.size() + ArgsE.size()), op);
}


template <OneDimBaseType Base1, OneDimBaseType Base2>
STRICT_CONSTEXPR auto merge(const Base1& A1, const Base2& A2) {
   auto A1E = generate1D(A1, [](auto x) { return x; });
   auto A2E = generate1D(A2, [](auto x) { return x; });
   auto op = [A1E, A2E](auto i) { return i < A1E.size() ? A1E.index(i) : A2E.index(i - A1E.size()); };
   return generate1D(irange(A1.size() + A2.size()), op);
}


template <OneDimBaseType Base, StrictType S, StrictType... SArgs>
STRICT_CONSTEXPR auto merge(const Base& A, S x, SArgs... xargs) {
   return merge(merge(A, x), xargs...);
}


template <OneDimBaseType Base, StrictType S>
STRICT_CONSTEXPR auto merge(const Base& A, S x) {
   auto AE = generate1D(A, [](auto x) { return x; });
   auto op = [AE, x](auto i) { return i == AE.size() ? x : AE.index(i); };
   return generate1D(irange(AE.size() + 1_sl), op);
}


template <StrictType S1, StrictType S2, typename... Other>
   requires OneDimBaseType<LastPack_t<Other...>> && AllStrictExceptLast<Other...>
STRICT_CONSTEXPR auto merge(S1 x, S2 y, const Other&... A) {
   return merge(x, merge(y, A...));
}


template <StrictType S, OneDimBaseType Base>
STRICT_CONSTEXPR auto merge(S x, const Base& A) {
   auto AE = generate1D(A, [](auto x) { return x; });
   auto op = [AE, x](auto i) { return i == 0_sl ? x : AE.index(i - 1_sl); };
   return generate1D(irange(AE.size() + 1_sl), op);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, ImplicitInt p, ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > 0_sl);
   ASSERT_STRICT_DEBUG(internal::valid_index(A, p.get()));
   ASSERT_STRICT_DEBUG(internal::valid_index(A, p.get() + n.get() - 1_sl));
   auto AE = generate1D(A, [](auto x) { return x; });
   return generate1D(irange(A.size() - n.get()),
                     [p, n, AE](auto j) { return j < p.get() ? AE.index(j) : AE.index(j + n.get()); });
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, Pos p, Count n) {
   return exclude(A, p.get(), n.get());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_CONSTEXPR auto sequence(ImplicitInt size, Strict<T> start, Strict<T> incr) {
   return Derived1D<SequenceExpr1D<T>>{start, size.get(), incr};
}


template <Real T>
STRICT_CONSTEXPR auto sequence(Size size, Start<T> start, Incr<T> incr) {
   return sequence<T>(size.get(), start.get(), incr.get());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_CONSTEXPR auto linspace(ImplicitInt size, Strict<T> start, Strict<T> end) {
   ASSERT_STRICT_DEBUG(size.get() > 0_sl);
   auto sz = size.get();
   return Derived1D<SequenceExpr1D<T>>{start, sz, (end - start) / strict_cast<T>(sz - 1_sl)};
}


template <Floating T>
STRICT_CONSTEXPR auto linspace(Size size, Start<T> start, End<T> end) {
   return linspace<T>(size.get(), start.get(), end.get());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
STRICT_CONSTEXPR_INLINE auto irange(ImplicitInt n) {
   return Derived1D<SequenceExpr1D<long int>>{0_sl, n.get(), 1_sl};
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange(const Base& A) {
   return irange(A.size());
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange_m1(const Base& A) {
   return irange(A.size_m1());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_CONSTEXPR auto e_unit(ImplicitInt unit_index, ImplicitInt size) {
   ASSERT_STRICT_DEBUG(unit_index.get() > -1_sl);
   ASSERT_STRICT_DEBUG(size.get() > unit_index.get());
   return generate1D(irange(size), [unit_index](auto i) { return i == unit_index.get() ? One<T> : Zero<T>; });
}


template <Real T>
STRICT_CONSTEXPR auto e_unit(Index unit_index, Size size) {
   return e_unit<T>(unit_index.get(), size.get());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_CONSTEXPR auto const1D(ImplicitInt size, Strict<T> c) {
   ASSERT_STRICT_DEBUG(size.get() > -1_sl);
   return generate1D(irange(size), [c]([[maybe_unused]] auto i) { return c; });
}


template <Builtin T>
STRICT_CONSTEXPR auto const1D(Size size, Value<T> c) {
   return const1D<T>(size.get(), c.get());
}


}  // namespace slib
