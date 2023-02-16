// Arkadijs Slobodkins, 2023

#include <cassert>
#include <cstdlib>
#include <valarray>

#include "../strict_array.hpp"
#include "../strict_util.hpp"

using namespace strict_array;

template<RealType T>
void assert_array(const std::valarray<T> & v, const Array<T> & s)
{
   assert((long long int)v.size() == s.size());
   for(auto i = 0LL; i < s.size(); ++i)
      assert(within_tol_abs<T>(v[i], s[i]));
}

template<StandardFloatingType T>
void assert_float_ops()
{
   auto n = 300LL;
   std::valarray<T> v1(n), v2(n), v3(n);
   Array<T> s1 = array_random<T>(300, T{-1.F}, T{1.F});
   Array<T> s2 = array_random<T>(300, T{0.01F}, T{1.F});
   Array<T> s3(n);
   for(auto i = 0LL; i < n; ++i) v1[i] = s1[i];
   for(auto i = 0LL; i < n; ++i) v2[i] = s2[i];

   v3 += T{2}; s3 += T{2}; assert_array(v3, s3);
   v3 -= T{2}; s3 -= T{2}; assert_array(v3, s3);
   v3 *= T{2}; s3 *= T{2}; assert_array(v3, s3);
   v3 /= T{2}; s3 /= T{2}; assert_array(v3, s3);

   v3 += v2; s3 += s2; assert_array(v3, s3);
   v3 -= v2; s3 -= s2; assert_array(v3, s3);
   v3 *= v2; s3 *= s2; assert_array(v3, s3);
   v3 /= v2; s3 /= s2; assert_array(v3, s3);

   v3 += T{2} + v2; s3 += T{2} + s2; assert_array(v3, s3);
   v3 -= T{2} - v2; s3 -= T{2} - s2; assert_array(v3, s3);
   v3 *= T{2} * v2; s3 *= T{2} * s2; assert_array(v3, s3);
   v3 /= T{2} / v2; s3 /= T{2} / s2; assert_array(v3, s3);

   v3 += v2 + T{2}; s3 += s2 + T{2}; assert_array(v3, s3);
   v3 -= v2 + T{2}; s3 -= s2 + T{2}; assert_array(v3, s3);
   v3 *= v2 + T{2}; s3 *= s2 + T{2}; assert_array(v3, s3);
   v3 /= v2 + T{2}; s3 /= s2 + T{2}; assert_array(v3, s3);

   v3 = v1 + v2; s3 = s1 + s2; assert_array(v3, s3);
   v3 = v1 - v2; s3 = s1 - s2; assert_array(v3, s3);
   v3 = v1 * v2; s3 = s1 * s2; assert_array(v3, s3);
   v3 = v1 / v2; s3 = s1 / s2; assert_array(v3, s3);

   v3 = v1 + T{2}; s3 = s1 + T{2}; assert_array(v3, s3);
   v3 = v1 - T{2}; s3 = s1 - T{2}; assert_array(v3, s3);
   v3 = v1 * T{2}; s3 = s1 * T{2}; assert_array(v3, s3);
   v3 = v1 / T{2}; s3 = s1 / T{2}; assert_array(v3, s3);

   v3 = T{2} + v2; s3 = T{2} + s2; assert_array(v3, s3);
   v3 = T{2} - v2; s3 = T{2} - s2; assert_array(v3, s3);
   v3 = T{2} * v2; s3 = T{2} * s2; assert_array(v3, s3);
   v3 = T{2} / v2; s3 = T{2} / s2; assert_array(v3, s3);

   v3 = T{2} + v1 + v2 - T{3}; s3 = T{2} + s1 + s2 - T{3}; assert_array(v3, s3);
   v3 = T{2} + v1 - v2 - T{3}; s3 = T{2} + s1 - s2 - T{3}; assert_array(v3, s3);
   v3 = T{2} + v1 * v2 - T{3}; s3 = T{2} + s1 * s2 - T{3}; assert_array(v3, s3);
   v3 = T{2} + v1 / v2 - T{3}; s3 = T{2} + s1 / s2 - T{3}; assert_array(v3, s3);

   v3 = v1.max(); s3 = max(s1); assert_array(v3, s3);
   v3 = v1.min(); s3 = min(s1); assert_array(v3, s3);
}

int main(int argc, char *argv[])
{
   assert_float_ops<float>();
   assert_float_ops<double>();
   assert_float_ops<long double>();

   int n = 1.e6;
   auto a = array_random<double>(n);
   auto b = array_random<double>(n);
   auto c = Array<double>(n);
   std::valarray<double> x(n), y(n), z(n);
   for(int i = 0; i < n; ++i) {
      x[i] = a[i];
      y[i] = b[i];
   }

   double m1{}, m2{};
   STRICT_TIME(m1 = sum(2.*a + 3.*b));
   STRICT_TIME(m2 = (2.*x + 3.*y).sum());
   std::cout << m1 << " " << m2 << std::endl;
   auto expr = x + y;
   expr[0] = 0;

   return EXIT_SUCCESS;
}
