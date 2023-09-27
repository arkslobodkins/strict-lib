#include <cstdlib>
#include <cassert>
#include <valarray>

#include "../src/strict_lib.hpp"

using namespace strict;
using namespace strict::util;

template<IntegerType T>
void assert_slicearray(const std::valarray<T> & v, const Array1D<T> & s)
{
   assert((long long int)v.size() == s.size());
   for(auto i = 0LL; i < s.size(); ++i) {
      assert(v[i] == s[i]);
   }
}

template<FloatingType T>
void assert_slicearray(const std::valarray<T> & v, const Array1D<T> & s)
{
   assert((long long int)v.size() == s.size());
   for(auto i = 0LL; i < s.size(); ++i) {
      assert(within_tol_rel<T>(v[i], s[i]));
   }
}

template<FloatingType T>
void assert_float_ops()
{
   auto n = 300LL;
   std::valarray<T> v1(n), v2(n), v3(n);
   const Array1D<T> s1 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   const Array1D<T> s2 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   Array1D<T> s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) {
      v1[i] = s1[i];
      v2[i] = s2[i];
      v3[i] = s3[i];
   }

   auto sl = seq(0, n-1);
   v3 += T{2}; s3[sl] += T{2}; assert_slicearray(v3, s3);
   v3 -= T{2}; s3[sl] -= T{2}; assert_slicearray(v3, s3);
   v3 *= T{2}; s3[sl] *= T{2}; assert_slicearray(v3, s3);
   v3 /= T{2}; s3[sl] /= T{2}; assert_slicearray(v3, s3);

   v3 += v2; s3[sl] += s2[sl]; assert_slicearray(v3, s3);
   v3 -= v2; s3[sl] -= s2[sl]; assert_slicearray(v3, s3);
   v3 *= v2; s3[sl] *= s2[sl]; assert_slicearray(v3, s3);
   v3 /= v2; s3[sl] /= s2[sl]; assert_slicearray(v3, s3);

   v3 += T{2} + v2; s3[sl] += T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 -= T{2} + v2; s3[sl] -= T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 *= T{2} + v2; s3[sl] *= T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 /= T{2} + v2; s3[sl] /= T{2} + s2[sl]; assert_slicearray(v3, s3);

   v3 += v2 + T{2}; s3[sl] += s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 -= v2 + T{2}; s3[sl] -= s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 *= v2 + T{2}; s3[sl] *= s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 /= v2 + T{2}; s3[sl] /= s2[sl] + T{2}; assert_slicearray(v3, s3);

   v3 = v1 + v2; s3[sl] = s1[sl] + s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 - v2; s3[sl] = s1[sl] - s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 * v2; s3[sl] = s1[sl] * s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 / v2; s3[sl] = s1[sl] / s2[sl]; assert_slicearray(v3, s3);

   v3 = v1 + T{2}; s3[sl] = s1[sl] + T{2}; assert_slicearray(v3, s3);
   v3 = v1 - T{2}; s3[sl] = s1[sl] - T{2}; assert_slicearray(v3, s3);
   v3 = v1 * T{2}; s3[sl] = s1[sl] * T{2}; assert_slicearray(v3, s3);
   v3 = v1 / T{2}; s3[sl] = s1[sl] / T{2}; assert_slicearray(v3, s3);

   v3 = T{2} + v2; s3[sl] = T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} - v2; s3[sl] = T{2} - s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} * v2; s3[sl] = T{2} * s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} / v2; s3[sl] = T{2} / s2[sl]; assert_slicearray(v3, s3);

   v3 = T{2} + v1 + v2 - T{3};
   s3[sl] = T{2} + s1[sl] + s2[sl] - T{3};
   assert_slicearray(v3, s3);

   v3 = T{2} + v1 - v2 - T{3};
   s3[sl] = T{2} + s1[sl] - s2[sl] - T{3};
   assert_slicearray(v3, s3);

   v3 = T{2} + v1 * v2 - T{3};
   s3[sl] = T{2} + s1[sl] * s2[sl] - T{3};
   assert_slicearray(v3, s3);

   v3 = T{2} + v1 / v2 - T{3};
   s3[sl] = T{2} + s1[sl] / s2[sl] - T{3};
   assert_slicearray(v3, s3);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_int_ops()
{
   auto n = 300LL;
   std::valarray<T> v1(n), v2(n), v3(n);
   const Array1D<T> s1 = random<T>(Size{n}, Low<T>{T(1)}, High<T>{T(3)});
   const Array1D<T> s2 = random<T>(Size{n}, Low<T>{T(1)}, High<T>{T(3)});
   Array1D<T> s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) {
      v1[i] = s1[i];
      v2[i] = s2[i];
      v3[i] = s3[i];
   }

   auto sl = seq(0, n-1);
   v3 += T{2}; s3[sl] += T{2}; assert_slicearray(v3, s3);
   v3 -= T{2}; s3[sl] -= T{2}; assert_slicearray(v3, s3);
   v3 *= T{2}; s3[sl] *= T{2}; assert_slicearray(v3, s3);
   v3 /= T{2}; s3[sl] /= T{2}; assert_slicearray(v3, s3);
   v3 <<= T{2}; s3[sl] <<= T{2}; assert_slicearray(v3, s3);
   v3 >>= T{2}; s3[sl] >>= T{2}; assert_slicearray(v3, s3);
   v3 %= T{2}; s3[sl] %= T{2}; assert_slicearray(v3, s3);

   s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) { v3[i] = s3[i]; }
   v3 += v2; s3[sl] += s2[sl]; assert_slicearray(v3, s3);
   v3 -= v2; s3[sl] -= s2[sl]; assert_slicearray(v3, s3);
   v3 *= v2; s3[sl] *= s2[sl]; assert_slicearray(v3, s3);
   v3 /= v2; s3[sl] /= s2[sl]; assert_slicearray(v3, s3);
   v3 <<= v2; s3[sl] <<= s2[sl]; assert_slicearray(v3, s3);
   v3 >>= v2; s3[sl] >>= s2[sl]; assert_slicearray(v3, s3);
   v3 %= v2; s3[sl] %= s2[sl]; assert_slicearray(v3, s3);

   s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) { v3[i] = s3[i]; }
   v3 += T{2} + v2; s3[sl] += T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 -= T{2} + v2; s3[sl] -= T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 *= T{2} + v2; s3[sl] *= T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 /= T{2} + v2; s3[sl] /= T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 <<= T{2} + v2; s3[sl] <<= T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 >>= T{2} + v2; s3[sl] >>= T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 %= T{2} + v2; s3[sl] %= T{2} + s2[sl]; assert_slicearray(v3, s3);

   s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) { v3[i] = s3[i]; }
   v3 += v2 + T{2}; s3[sl] += s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 -= v2 + T{2}; s3[sl] -= s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 *= v2 + T{2}; s3[sl] *= s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 /= v2 + T{2}; s3[sl] /= s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 <<= v2 + T{2}; s3[sl] <<= s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 >>= v2 + T{2}; s3[sl] >>= s2[sl] + T{2}; assert_slicearray(v3, s3);
   v3 %= v2 + T{2}; s3[sl] %= s2[sl] + T{2}; assert_slicearray(v3, s3);

   v3 = v1 + v2; s3[sl] = s1[sl] + s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 - v2; s3[sl] = s1[sl] - s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 * v2; s3[sl] = s1[sl] * s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 / v2; s3[sl] = s1[sl] / s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 << v2; s3[sl] = s1[sl] << s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 >> v2; s3[sl] = s1[sl] >> s2[sl]; assert_slicearray(v3, s3);
   v3 = v1 % v2; s3[sl] = s1[sl] % s2[sl]; assert_slicearray(v3, s3);

   v3 = v1 + T{2}; s3[sl] = s1[sl] + T{2}; assert_slicearray(v3, s3);
   v3 = v1 - T{2}; s3[sl] = s1[sl] - T{2}; assert_slicearray(v3, s3);
   v3 = v1 * T{2}; s3[sl] = s1[sl] * T{2}; assert_slicearray(v3, s3);
   v3 = v1 / T{2}; s3[sl] = s1[sl] / T{2}; assert_slicearray(v3, s3);
   v3 = v1 << T{2}; s3[sl] = s1[sl] << T{2}; assert_slicearray(v3, s3);
   v3 = v1 >> T{2}; s3[sl] = s1[sl] >> T{2}; assert_slicearray(v3, s3);
   v3 = v1 % T{2}; s3[sl] = s1[sl] % T{2}; assert_slicearray(v3, s3);

   v3 = T{2} + v2; s3[sl] = T{2} + s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} - v2; s3[sl] = T{2} - s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} * v2; s3[sl] = T{2} * s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} / v2; s3[sl] = T{2} / s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} << v2; s3[sl] = T{2} << s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} >> v2; s3[sl] = T{2} >> s2[sl]; assert_slicearray(v3, s3);
   v3 = T{2} % v2; s3[sl] = T{2} % s2[sl]; assert_slicearray(v3, s3);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

int main()
{
   assert_int_ops<short int>();
   assert_int_ops<int>();
   assert_int_ops<long int>();
   assert_int_ops<long long int>();

   assert_float_ops<float>();
   assert_float_ops<double>();
   assert_float_ops<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_float_ops<float128>();
   #endif
   return EXIT_SUCCESS;
}
