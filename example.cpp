// for example, compile using recent version of gcc or recent intel compilers from intel-oneAPI
// g++-12.2 -std=c++2a main.cpp
// dpcpp -std=c++2a main.cpp
// icpx -std=c++2a main.cpp

// to use quadruple precision, for example
// g++-12.2 -std=gnu++2a main.cpp -lquadmath

// to enable debugging and range checking add -DSTRICT_ARRAY_DEBUG_ON
// to enable division by 0 checking add -DSTRICT_ARRAY_DIVISION_ON

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <cstdlib>
#include <iostream>

#include "strict_array.hpp"
using namespace std;
using namespace strict_array;

template<typename F>
void derivative(Array<float64> & A, F f)
{
   constexpr StrictVal<float64> h = 0.000'001;
   constexpr StrictVal<float64> two_h = 2. * h;
   for(decltype(A.size()) i = 0; i < A.size(); ++i)
      A[i] = ( f(A[i] + h) - f(A[i] - h) ) / two_h;
}

template<typename T>
T sign(StrictVal<T> x)
{
   return x > T(0) ? T(1) : T(-1);
}

int main()
{
   Array<float64> A{-1., -2., -3., -4., -5.};
   auto func = [](float64 x) -> StrictVal<float64> { return x * std::exp(x); };
   derivative(A, func);
   cout << 2. * A << endl;
   cout << norm_inf(2. * A) << endl;

   auto n = 100'000LL;
   Array B = array_random<float32>(n, -1.F, 1.F);
   auto half_range = B.within_range(-0.5F, 0.5F);
   for(auto x_ptr : half_range) *x_ptr += 0.5F * sign(*x_ptr);

   return EXIT_SUCCESS;

}

#endif
