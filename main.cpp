// for example, compile using recent version of gcc or recent intel compilers from intel-oneAPI
// g++-12.2 -std=c++2a main.cpp
// dpcpp -std=c++2a main.cpp
// icpx -std=c++2a main.cpp

// to use quadruple precision, for example
// g++-12.2 -std=gnu++2a main.cpp -lquadmath

// to enable debugging and range checking add -DSTRICT_ARRAY_DEBUG_ON
// to enable division by 0 checking add -DSTRICT_ARRAY_DIVISION_ON

#include <cstdlib>
#include <iostream>

#include "strict_array.hpp"
using namespace strict_array;

template<typename F>
void derivative(Array<float64> & A, F f)
{
   constexpr float64 h = 0.000'001;
   constexpr float64 two_h = 2. * h;
   for(auto i = 0L; i < A.size(); ++i)
      A[i] = ( f(A[i] + h) - f(A[i] - h) ) / two_h;
}

#include <vector>
int main()
{
   Array<float64> A{1., 2., 3., 4., 5.};
   derivative( A, [](auto x){ return x * std::exp(float64(x));} );

   A = Array<float64>{5., 4., 3., 2., 1.};
   derivative( A, [](auto x){ return 2. * x * x;} );
   A *= 2.;
   A.remove_element(0L);

   std::cout << A << std::endl;;
   std::cout << norm_inf(A) << std::endl;
   std::cout << norm2(A) << std::endl;
   std::cout << dot_prod(A, A) << std::endl << std::endl;

// for(auto i = 0L; i < A.size(); ++i) A[i] *= 2;     // does not compile, rhs is not of type double
// for(auto i = 0; i < A.size(); ++i)  A[i] *= 2.;    // does not compile, i must be of type long int
   for(auto i = 0L; i < A.size(); ++i) A[i] *= 2.;    // compiles, rhs is of type double and i is long int
   for(auto & x : A) x *= 2;                          // compiles even though rhs is int, iterators are raw pointers(for now at least)

   #ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
   Array<float128> B = array_random(10L, -1.Q, 1.Q);
   B.sort_increasing();
   std::cout << B << std::endl;;
   B.resize(3L) =  {1.Q, 2.Q, 3.Q};
   B[0L] = (B[1L] + 2.Q) / 3.Q;        // all arguments on the rhs must be of type float128
   B[0L] = (2. + 3.Q) / B[1L];         // 2. here compiles because of the order of operations
   float128 var = (B[1L] + 2.Q) / 3.Q; // can only be converted to float128
   bool b = B[0L] > 1.Q;               // must compare to float128
// b = B[0L] > 1.;                     // won't compile
   #endif

// Consider using StrictVal as illustrated below

   float factor = 8.25;
   Array<double> X(3L);
   A[0L] = 1. + factor; // factor should probably be float, won't be noticed

// StrictVal<float> strict_factor{factor};
// A[0L] = 1. + strict_factor; // would not compile, operations must be performed on identical types

   StrictVal<double> strict_factor{8.25};
   A[0L] = 1. + strict_factor; // compiles and works fine

   A[0L] = A[1L]/0.; // throws exception in division mode

   return EXIT_SUCCESS;
}
