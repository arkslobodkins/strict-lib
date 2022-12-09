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

   A = Array<float64>{1., 2., 3., 4., 5.};
   derivative( A, [](float64 x){ return 2. * x * x;} );
   A *= 2.;
   A.remove_element(0L);
   A.sort_increasing();

   std::cout << A << std::endl;;
   std::cout << norm_inf(A) << std::endl;
   std::cout << norm2(A) << std::endl;
   std::cout << dot_prod(A, A) << std::endl;

   return EXIT_SUCCESS;
}
