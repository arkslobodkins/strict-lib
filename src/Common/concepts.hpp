//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once


#include <concepts>
#include <tuple>
#include <type_traits>
#include <utility>

#include "config.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace slib {


#define STRICT_COMPATIBILITY_ERROR_MSG                                        \
   "THE C++ IMPLEMENTATION YOU ARE USING DOES NOT MEET THE SIZE REQUIREMENTS" \
   " OF STRICT-LIB. CONSIDER USING ANOTHER MACHINE/IMPLEMENTATION TO MEET THESE REQUIREMENTS."


static_assert(sizeof(float) == 4, STRICT_COMPATIBILITY_ERROR_MSG);
static_assert(sizeof(double) == 8, STRICT_COMPATIBILITY_ERROR_MSG);
using float32 = float;
using float64 = double;


static_assert(sizeof(int) >= 4, STRICT_COMPATIBILITY_ERROR_MSG);
static_assert(sizeof(long int) >= 8, STRICT_COMPATIBILITY_ERROR_MSG);  // ensure 64-bit indexing


template <typename D, typename B> concept DerivedFrom = std::derived_from<D, B>;
template <typename B, typename D> concept BaseOf = std::is_base_of_v<B, D>;
template <typename T, typename U> concept SameAs = std::same_as<T, U>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> concept SignedInteger = SameAs<T, int> || SameAs<T, long int>;

template <typename T> concept UnsignedInteger = SameAs<T, unsigned int> || SameAs<T, unsigned long int>;

template <typename T> concept Integer = SignedInteger<T> || UnsignedInteger<T>;

template <typename T> concept StandardFloating
    = SameAs<T, float> || SameAs<T, double> || SameAs<T, long double>;


#ifdef STRICT_QUAD_PRECISION
static_assert(sizeof(__float128) == 16, STRICT_COMPATIBILITY_ERROR_MSG);
using float128 = __float128;
template <typename T> concept Quadruple = SameAs<T, float128>;
template <typename T> concept Floating = StandardFloating<T> || Quadruple<T>;
#else
template <typename T> concept Floating = StandardFloating<T>;
#endif


template <typename T> concept Real = Floating<T> || Integer<T>;
template <typename T> concept Boolean = SameAs<T, bool>;
template <typename T> concept Builtin = Boolean<T> || Real<T>;


#ifdef STRICT_QUAD_PRECISION
template <typename T> concept NotQuadruple = Builtin<T> && !Quadruple<T>;
#else
template <typename T> concept NotQuadruple = Builtin<T>;
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
struct Strict;


// aliases for unsigned integers are not introduced
using index_t = Strict<long int>;
using StrictBool = Strict<bool>;
using StrictInt = Strict<int>;
using StrictLong = Strict<long int>;
using Strict32 = Strict<float32>;
using Strict64 = Strict<float64>;
#ifdef STRICT_QUAD_PRECISION
using Strict128 = Strict<float128>;
#endif


template <typename T> concept StrictType = requires(T a) {
   typename T::value_type;
   Strict{a};
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DimBase {};


// BaseType returns false for references; referenceness is not removed to prevent
// a possibility of dangling references and other misuses in generic code
template <typename T> concept BaseType = BaseOf<DimBase, T>;


template <typename T> concept RealBaseType = BaseType<T> && Real<typename T::builtin_type>;
template <typename T> concept IntegerBaseType = BaseType<T> && Integer<typename T::builtin_type>;
template <typename T> concept SignedIntegerBaseType = BaseType<T> && SignedInteger<typename T::builtin_type>;
template <typename T> concept FloatingBaseType = BaseType<T> && Floating<typename T::builtin_type>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ConstSliceBase {};
template <typename T> concept ConstSliceBaseType = BaseOf<ConstSliceBase, T>;


// Objects of type Array or slices of Array with non-constant semantics. Expression templates are excluded
// since they return by value and thus std::is_lvalue_reference_v evaluates to false
template <typename T> concept NonConstBaseType = BaseType<T> && (requires(T A) {
                                                    requires std::is_lvalue_reference_v<decltype(A.index(0))>;
                                                 } && !std::is_const_v<T> && !ConstSliceBaseType<T>);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LinearIterator {};


// second condition is added so that it works for reverse iterators as well
template <typename T> concept LinearIteratorType
    = BaseOf<LinearIterator, T> || BaseOf<LinearIterator, typename T::iterator_type>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// referenceness is removed so that aliases
// can be used for references as well
template <typename T>
   requires(BaseType<std::remove_reference_t<T>>)
using SizeTypeOf = std::remove_reference_t<T>::size_type;


template <typename T>
   requires(BaseType<std::remove_reference_t<T>>)
using ValueTypeOf = std::remove_reference_t<T>::value_type;


template <typename T>
   requires(BaseType<std::remove_reference_t<T>>)
using BuiltinTypeOf = std::remove_reference_t<T>::builtin_type;


template <typename T>
   requires(RealBaseType<std::remove_reference_t<T>>)
using RealTypeOf = std::remove_reference_t<T>::builtin_type;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
using RemoveRef = std::remove_reference_t<T>;


// not all compilers support remove_cvref_t
template <typename T>
using RemoveCVRef = std::remove_cv_t<std::remove_reference_t<T>>;


template <typename T>
using AddConst = std::add_const_t<T>;


template <typename T> concept IsConst = std::is_const_v<T>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OneDimBase : protected DimBase {};
template <typename T> concept OneDimBaseType = BaseOf<OneDimBase, T>;
template <typename T> concept OneDimRealBaseType = OneDimBaseType<T> && RealBaseType<T>;
template <typename T> concept OneDimIntegerBaseType = OneDimBaseType<T> && IntegerBaseType<T>;
template <typename T> concept OneDimSignedIntegerBaseType = OneDimBaseType<T> && SignedIntegerBaseType<T>;
template <typename T> concept OneDimFloatingBaseType = OneDimBaseType<T> && FloatingBaseType<T>;
template <typename T> concept OneDimNonConstBaseType = OneDimBaseType<T> && NonConstBaseType<T>;


class TwoDimBase : protected DimBase {};
template <typename T> concept TwoDimBaseType = BaseOf<TwoDimBase, T>;
template <typename T> concept TwoDimRealBaseType = TwoDimBaseType<T> && RealBaseType<T>;
template <typename T> concept TwoDimIntegerBaseType = TwoDimBaseType<T> && IntegerBaseType<T>;
template <typename T> concept TwoDimSignedIntegerBaseType = TwoDimBaseType<T> && SignedIntegerBaseType<T>;
template <typename T> concept TwoDimFloatingBaseType = TwoDimBaseType<T> && FloatingBaseType<T>;
template <typename T> concept TwoDimNonConstBaseType = TwoDimBaseType<T> && NonConstBaseType<T>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T, typename F> concept UnaryOperation
    = std::invocable<F, ValueTypeOf<T>> && StrictType<decltype(std::declval<F>()(ValueTypeOf<T>{}))>;


template <typename T1, typename T2, typename F> concept BinaryOperation
    = std::invocable<F, ValueTypeOf<T1>, ValueTypeOf<T2>>
   && StrictType<decltype(std::declval<F>()(ValueTypeOf<T1>{}, ValueTypeOf<T2>{}))>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StoreByCopy {};
struct StoreByReference {};


class CopyBase1D : protected OneDimBase, protected StoreByCopy {};
class ReferenceBase1D : protected OneDimBase, protected StoreByReference {};
class CopyBase2D : protected TwoDimBase, protected StoreByCopy {};
class ReferenceBase2D : protected TwoDimBase, protected StoreByReference {};


template <typename T> concept CopyStorable = BaseOf<StoreByCopy, T> && BaseType<T>;
template <typename T> concept ReferenceStorable = BaseOf<StoreByReference, T> && BaseType<T>;


template <typename T>
struct CopyOrReferenceExpr;


template <typename T>
   requires CopyStorable<T>
struct CopyOrReferenceExpr<T> {
   using type = T;
};


template <typename T>
   requires ReferenceStorable<T>
struct CopyOrReferenceExpr<T> {
   using type = T&;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename... Args>
struct LastPackTraits {
   using type = std::tuple_element_t<sizeof...(Args) - 1, std::tuple<Args...>>;
};


template <typename... Args>
using LastPack_t = LastPackTraits<Args...>::type;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct IsStrict {
   static constexpr bool value = StrictType<T>;
};


template <typename... Args> concept AllStrict = std::conjunction_v<IsStrict<Args>...>;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct IsNonConstBase {
   static constexpr bool value = NonConstBaseType<T>;
};


template <typename... Args> concept AllNonConstBases = std::conjunction_v<IsNonConstBase<Args>...>;


}  // namespace slib
