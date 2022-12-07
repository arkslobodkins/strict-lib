#include <cassert>
#include <cstdlib>
#include <iostream>
#include <valarray>
#include <numeric>

#include "strict_array.hpp"
#include "util.hpp"

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
   z2.remove_element(5L);
   z2.sort_decreasing();
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
   val = norm_inf(-u * v);
   val = norm2(u * v);
   bool b = is_positive(v);
   b = is_nonnegative(v);
   b = does_contain_zero(v - 2.);
   std::cout << u << std::endl;
   auto vec_it = u.within_range(-1., 3.);
   for(auto it : vec_it) std::cout << *it << std::endl;

   Array<float64> y(20L);
   y.resize_and_assign(v);

   Array<short> D(20L, short(1)); // fill with ones
   std::iota(D.begin(), D.end(), short(0)); // fill with 0, 1, ...

   Array<float64> sa(10'000'000L, 1.);
   Array<float64> sb(10'000'000L, 1.);
   Array<float64> sc(10'000'000L, 1.);
   std::valarray<double> va(1., 10'000'000L);
   std::valarray<double> vb(1., 10'000'000L);
   std::valarray<double> vc(1., 10'000'000L);
   TIME(sa += sb + 1.);
   TIME(va += vb + 1.);
   for(auto i = 0L; i < sa.size(); ++i) assert(sa[i] == va[i]);

   TIME(sc = 2.*sa + 2.*sb);
   TIME(vc = 2.*va + 2.*vb);
   for(auto i = 0L; i < sa.size(); ++i) assert(sc[i] == vc[i]);
   return EXIT_SUCCESS;
}
