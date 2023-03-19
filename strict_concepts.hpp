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
concept IntegerType = SameType<T, short> || SameType<T, int> ||
                      SameType<T, long int> || SameType<T, long long int>;

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
template<typename T> concept BaseType = BaseOf<Base, T>;
template<typename T> concept ExprType = BaseOf<Expr, T>;               // all types of expression templates
template<typename T> concept DirectBaseType = BaseType<T> &&           // Objects of type Array or slices of Array.
   requires(T A) { requires std::is_lvalue_reference_v<decltype(A[0])>; };

template<typename T> concept IntegerBaseType = BaseType<T> && IntegerType<typename T::real_type>;
template<typename T> concept StandardFloatingBaseType = BaseType<T> && StandardFloatingType<typename T::real_type>;
template<typename T> concept FloatingBaseType = BaseType<T> && FloatingType<typename T::real_type>;

#ifdef STRICT_QUADRUPLE_PRECISION
template<typename T> concept QuadFloatingBaseType = BaseType<T> && QuadType<typename T::real_type>;
template<typename T> concept NotQuadFloatingBaseType = BaseType<T> && !QuadType<typename T::real_type>;
#else
template<typename T> concept NotQuadFloatingBaseType = BaseType<T>;
#endif

template<BaseType T>
using RealTypeOf = typename T::real_type;

template<BaseType T>
using SizeTypeOf = typename T::size_type;

template<BaseType T>
using ValueTypeOf = typename std::remove_reference_t<T>::value_type;

using strict_int = long long int;

class SliceArrayBase1D : protected Base {};
class SliceArrayExpr1D : protected Expr, protected SliceArrayBase1D {};
template<typename T> concept SliceArrayBaseType1D = BaseOf<SliceArrayBase1D, T>;
template<typename T> concept SliceArrayExprType1D = BaseOf<SliceArrayExpr1D, T>;

class ArrayBase1D : protected Base {};
class ArrayExpr1D : protected Expr, protected ArrayBase1D {};
template<typename T> concept ArrayBaseType1D = BaseOf<ArrayBase1D, T>;
template<typename T> concept ArrayExprType1D = BaseOf<ArrayExpr1D, T>;

template<typename T> concept OneDimBaseType = ArrayBaseType1D<T> || SliceArrayBaseType1D<T>;
template<typename T> concept OneDimFloatingBaseType = OneDimBaseType<T> && FloatingBaseType<T>;

namespace internal {
   template<BaseType BaseT>
   bool valid_index(const BaseT & A, SizeTypeOf<BaseT> index) {
      return index > -1 && index < A.size();
   }
}

}

#endif

