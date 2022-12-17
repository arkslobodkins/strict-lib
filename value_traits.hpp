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
concept SameType = std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value;

template<typename T>
concept IntegerType = SameType<short, T> || SameType<T, int> ||
                      SameType<T, long int> || SameType<T, long long int>;

template<typename T>
concept StandardFloatingType = std::is_floating_point<T>::value;

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
}

#endif

