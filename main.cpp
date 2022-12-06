#include <cstdlib>
#include <iostream>
#include <valarray>
#include <numeric>
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

   #ifdef STRICT_ARRAY_QUADRUPLE_PRECISION
   Array<float128> z1 = array_random(10L, 1.Q, 2.Q);
   Array<float128> z2 = array_random(10L, -2.Q, -1.Q);
   z2 *= -1.Q;
   z2 *= z1 * 2.Q;
   std::cout << z2 << std::endl;
   #endif

   Array<int> u1 = array_random(10L, 1, 2);
   Array<int> u2 = array_random(10L, -2, -1);
   u1 /= -1;
   u2 /= u1 * 2;
   std::cout << u2 << std::endl;

   Array<float64> v{1., 2., 3., 4., 5., -1., 8., 0.};
   Array<float64> u{-1., -2., 3., 4., 5., -1., 8., 0.};
   Array<float64> abs_u = abs(u);
   float64 val = max(v);
   val = min(u + v);
   val = dot_prod(2.*u, v);
   val = sum(-u + 2.*v);
   val = norm_inf(-u*v);
   val = norm2(u*v);
   bool b = is_positive(v);
   b = is_nonnegative(v);
   b = does_contain_zero(v - 2.);

   Array<float64> y(20L);
   y.resize_and_assign(v);

   Array<short> D(20L, short(1)); // fill with ones
   std::iota(D.begin(), D.end(), short(0)); // fill with 0, 1, ...

   return EXIT_SUCCESS;
}
