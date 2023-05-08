// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (__GNUG__) && !defined (__INTEL_COMPILER) && !defined (__clang__) && !defined (__INTEL_LLVM_COMPILER)
   #define STRICT_QUADRUPLE_PRECISION
   #include <quadmath.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace strict_array {

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);
using float32 = float;
using float64 = double;

template<typename T, typename U>
concept BaseOf = std::is_base_of_v<T, U>;

template<typename T, typename U>
concept SameType = std::is_same_v<T, U>;

template<typename T>
concept IntegerType = SameType<T, short> ||
                      SameType<T, int> ||
                      SameType<T, long int> ||
                      SameType<T, long long int>;

template<typename T>
concept StandardFloatingType = SameType<T, float> ||
                               SameType<T, double> ||
                               SameType<T, long double>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUADRUPLE_PRECISION
static_assert(sizeof(__float128) == 16);
using float128 = __float128;
template<typename T> concept QuadType = SameType<T, float128>;

template<typename T> concept FloatingType = StandardFloatingType<T> || QuadType<T>;
#else
template<typename T> concept FloatingType = StandardFloatingType<T>;
#endif
template<typename T> concept RealType = FloatingType<T> || IntegerType<T>;

#ifdef STRICT_QUADRUPLE_PRECISION
template<typename T> concept NotQuadType = RealType<T> && !QuadType<T>;
#else
template<typename T> concept NotQuadType = RealType<T>;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UnaryOperation {};
class BinaryOperation {};
template<typename T> concept UnaryOperationType = BaseOf<UnaryOperation, T>;
template<typename T> concept BinaryOperationType = BaseOf<BinaryOperation, T>;

class Base {};
class Expr : private Base {};
template<typename T> concept BaseType = BaseOf<Base, T>;               // BaseType returns false for references
                                                                       // referenceness is not removed to prevent
                                                                       // a possibility of dangling references
                                                                       // in generic code

template<typename T> concept ExprType = BaseOf<Expr, T>;               // all types of expression templates
template<typename T> concept DirectBaseType = BaseType<T> &&               // Objects of type Array or slices of Array.
   requires(T A) { requires std::is_lvalue_reference_v<decltype(A[0])>; }; // Expression templates are excluded
                                                                           // since they return by value and thus
                                                                           // std::is_lvalue_reference_v evaluates to false

template<typename T> concept IntegerBaseType = BaseType<T> && IntegerType<typename T::real_type>;
template<typename T> concept StandardFloatingBaseType = BaseType<T> && StandardFloatingType<typename T::real_type>;
template<typename T> concept FloatingBaseType = BaseType<T> && FloatingType<typename T::real_type>;

#ifdef STRICT_QUADRUPLE_PRECISION
template<typename T> concept QuadFloatingBaseType = BaseType<T> && QuadType<typename T::real_type>;
template<typename T> concept NotQuadFloatingBaseType = BaseType<T> && !QuadType<typename T::real_type>;
#else
template<typename T> concept NotQuadFloatingBaseType = BaseType<T>;
#endif

// referenceness is removed so that aliases
// can be used for references as well
template<typename T>
requires (BaseType<std::remove_reference_t<T>>)
using RealTypeOf = typename std::remove_reference_t<T>::real_type;

template<typename T>
requires (BaseType<std::remove_reference_t<T>>)
using SizeTypeOf = typename std::remove_reference_t<T>::size_type;

template<typename T>
requires (BaseType<std::remove_reference_t<T>>)
using ValueTypeOf = typename std::remove_reference_t<T>::value_type;

using strict_int = long long int;
static_assert(sizeof(strict_int) >= 8); // ensure 64-bit indexing

class Base1D : protected Base{};
template<typename T> concept OneDimBaseType = BaseOf<Base1D, T>;
template<typename T> concept OneDimFloatingBaseType = OneDimBaseType<T> && FloatingBaseType<T>;

}

#endif

