#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (__GNUC__) && !defined (__clang__) && !defined (__INTEL_LLVM_COMPILER) && !defined (__INTEL_COMPILER)
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
concept IntegerType = SameType<short, T> || SameType<T, int> ||
                      SameType<T, long int> || SameType<T, long long int>;

template<typename T>
concept StandardFloatingType = std::is_floating_point_v<T>;

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
class ArrayBase{};
class ArrayExpr{};
class UnaryOperation{};
class BinaryOperation{};

template<typename T> concept ArrayBaseType = BaseOf<ArrayBase, T>;
template<typename T> concept ArrayExprType = BaseOf<ArrayExpr, T>;
template<typename T> concept UnaryOperationType = BaseOf<UnaryOperation, T>;
template<typename T> concept BinaryOperationType = BaseOf<BinaryOperation, T>;

// Forward declarations(expression templates)
template<ArrayBaseType T1, UnaryOperationType Op> class UnaryExpr;
template<ArrayBaseType T1, ArrayBaseType T2, BinaryOperationType Op> class BinExpr;
template<ArrayBaseType T1, RealType T2, BinaryOperationType Op> class BinExprValLeft;
template<ArrayBaseType T1, RealType T2, BinaryOperationType Op> class BinExprValRight;

template<typename T> concept IntegerArrayBaseType = ArrayBaseType<T> && IntegerType<typename T::real_type>;
template<typename T> concept StandardFloatingArrayBaseType = ArrayBaseType<T> && StandardFloatingType<typename T::real_type>;
template<typename T> concept FloatingArrayBaseType = ArrayBaseType<T> && FloatingType<typename T::real_type>;
#ifdef STRICT_QUADRUPLE_PRECISION
template<typename T> concept QuadFloatingArrayBaseType = ArrayBaseType<T> && QuadType<typename T::real_type>;
#endif

}

#endif

