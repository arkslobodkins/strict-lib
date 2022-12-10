// for example, compile using recent version of gcc or recent intel compilers from intel-oneAPI
// g++-12.2 -std=c++2a main.cpp
// dpcpp -std=c++2a main.cpp
// icpx -std=c++2a main.cpp

// to use quadruple precision, for example
// g++-12.2 -std=gnu++2a main.cpp -lquadmath

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

int main()
{
   Array<float64> A{1., 2., 3., 4., 5.};
   derivative( A, [](float64 x){ return x * std::exp(x);} );

   A = Array<float64>{5., 4., 3., 2., 1.};
   derivative( A, [](float64 x){ return 2. * x * x;} );
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
   #endif

   return EXIT_SUCCESS;
}
