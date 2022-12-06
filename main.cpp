#include <cstdlib>
#include <iostream>
#include <valarray>
#include "strict_array.hpp"

using namespace strict_array;

int main()
{
   // some examples
   Array<float32> y1 = array_random(10L, 1.F, 2.F);
   Array<float32> y2 = array_random(10L, -2.F, -1.F);
   y2 -= 1.F;
   y2 -= y1 - 2.F;
   std::cout << y2 << std::endl;

   Array<float128> z1 = array_random(10L, 1.Q, 2.Q);
   Array<float128> z2 = array_random(10L, -2.Q, -1.Q);
   z2 *= -1.Q;
   z2 *= z1 * 2.Q;
   std::cout << z2 << std::endl;

   Array<int> u1 = array_random(10L, 1, 2);
   Array<int> u2 = array_random(10L, -2, -1);
   u1 /= -1;
   u2 /= u1 * 2;
   std::cout << u2 << std::endl;

   Array<float64> v{1., 2., 3., 4., 5., -1., 8., 0.};
   std::cout << max(v) << std::endl;
   std::cout << min(v) << std::endl;
   std::cout << min(2.*v) << std::endl;
   std::cout << dot_prod(2.*v, v) << std::endl;
   std::cout << is_positive(v) << std::endl;
   std::cout << is_nonnegative(v) << std::endl;
   std::cout << does_contain_zero(v) << std::endl;
   std::cout << sum(2.*v) << std::endl;

   std::cout << std::endl;
   Array<float64> y(20L);
   y.resize_and_assign(v);
   std::cout << y << std::endl;

   return EXIT_SUCCESS;
}
