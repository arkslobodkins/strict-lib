// Arkadijs Slobodkins, 2023

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <valarray>

#include "../src/strict_lib.hpp"

using namespace strict;
using namespace strict::util;

template<RealType T>
void assert_array(const std::valarray<T> & v, const Array1D<T> & s)
{
   assert((long long int)v.size() == s.size());
   for(auto i = 0LL; i < s.size(); ++i) {
      assert(within_tol_rel<T>(v[i], s[i]));
   }
}

template<IntegerType T>
void assert_array(const std::valarray<T> & v, const Array1D<T> & s)
{
   assert((long long int)v.size() == s.size());
   for(auto i = 0LL; i < s.size(); ++i) {
      assert(v[i] == s[i]);
   }
}

template<RealType T>
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

   static_assert(std::random_access_iterator<decltype(s1.begin())>);

   v3 += T{2}; s3 += T{2}; assert_array(v3, s3);
   v3 -= T{2}; s3 -= T{2}; assert_array(v3, s3);
   v3 *= T{2}; s3 *= T{2}; assert_array(v3, s3);
   v3 /= T{2}; s3 /= T{2}; assert_array(v3, s3);

   v3 += v2; s3 += s2; assert_array(v3, s3);
   v3 -= v2; s3 -= s2; assert_array(v3, s3);
   v3 *= v2; s3 *= s2; assert_array(v3, s3);
   v3 /= v2; s3 /= s2; assert_array(v3, s3);

   v3 += T{2} + v2; s3 += T{2} + s2; assert_array(v3, s3); v3 -= T{2} - v2; s3 -= T{2} - s2; assert_array(v3, s3);
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

   static_assert(std::random_access_iterator<decltype(s1.begin())>);

   v3 += T{2}; s3 += T{2}; assert_array(v3, s3);
   v3 -= T{2}; s3 -= T{2}; assert_array(v3, s3);
   v3 *= T{2}; s3 *= T{2}; assert_array(v3, s3);
   v3 /= T{2}; s3 /= T{2}; assert_array(v3, s3);
   v3 <<= T{2}; s3 <<= T{2}; assert_array(v3, s3);
   v3 >>= T{2}; s3 >>= T{2}; assert_array(v3, s3);
   v3 %= T{2}; s3 %= T{2}; assert_array(v3, s3);

   s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) { v3[i] = s3[i]; }
   v3 += v2; s3 += s2; assert_array(v3, s3);
   v3 -= v2; s3 -= s2; assert_array(v3, s3);
   v3 *= v2; s3 *= s2; assert_array(v3, s3);
   v3 /= v2; s3 /= s2; assert_array(v3, s3);
   v3 <<= v2; s3 <<= s2; assert_array(v3, s3);
   v3 >>= v2; s3 >>= s2; assert_array(v3, s3);
   v3 %= v2; s3 %= s2; assert_array(v3, s3);

   s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) { v3[i] = s3[i]; }
   v3 += v2 + T{2}; s3 += s2 + T{2}; assert_array(v3, s3);
   v3 -= v2 + T{2}; s3 -= s2 + T{2}; assert_array(v3, s3);
   v3 *= v2 + T{2}; s3 *= s2 + T{2}; assert_array(v3, s3);
   v3 /= v2 + T{2}; s3 /= s2 + T{2}; assert_array(v3, s3);
   v3 <<= v2 + T{2}; s3 <<= s2 + T{2}; assert_array(v3, s3);
   v3 >>= v2 + T{2}; s3 >>= s2 + T{2}; assert_array(v3, s3);
   v3 %= v2 + T{2}; s3 %= s2 + T{2}; assert_array(v3, s3);

   s3 = random<T>(Size{n}, Low<T>{T(5)}, High<T>{T(10)});
   for(auto i = 0LL; i < n; ++i) { v3[i] = s3[i]; }
   v3 += T{2} + v2; s3 += T{2} + s2; assert_array(v3, s3);
   v3 -= T{2} + v2; s3 -= T{2} + s2; assert_array(v3, s3);
   v3 *= T{2} + v2; s3 *= T{2} + s2; assert_array(v3, s3);
   v3 /= T{2} + v2; s3 /= T{2} + s2; assert_array(v3, s3);
   v3 <<= T{2} + v2; s3 <<= T{2} + s2; assert_array(v3, s3);
   v3 >>= T{2} + v2; s3 >>= T{2} + s2; assert_array(v3, s3);
   v3 %= T{2} + v2; s3 %= T{2} + s2; assert_array(v3, s3);

   v3 = v1 + v2; s3 = s1 + s2; assert_array(v3, s3);
   v3 = v1 - v2; s3 = s1 - s2; assert_array(v3, s3);
   v3 = v1 * v2; s3 = s1 * s2; assert_array(v3, s3);
   v3 = v1 / v2; s3 = s1 / s2; assert_array(v3, s3);
   v3 = v1 << v2; s3 = s1 << s2; assert_array(v3, s3);
   v3 = v1 >> v2; s3 = s1 >> s2; assert_array(v3, s3);
   v3 = v1 % v2; s3 = s1 % s2; assert_array(v3, s3);

   v3 = v1 + T{2}; s3 = s1 + T{2}; assert_array(v3, s3);
   v3 = v1 - T{2}; s3 = s1 - T{2}; assert_array(v3, s3);
   v3 = v1 * T{2}; s3 = s1 * T{2}; assert_array(v3, s3);
   v3 = v1 / T{2}; s3 = s1 / T{2}; assert_array(v3, s3);
   v3 = v1 << T{2}; s3 = s1 << T{2}; assert_array(v3, s3);
   v3 = v1 >> T{2}; s3 = s1 >> T{2}; assert_array(v3, s3);
   v3 = v1 % T{2}; s3 = s1 % T{2}; assert_array(v3, s3);

   v3 = T{2} + v2; s3 = T{2} + s2; assert_array(v3, s3);
   v3 = T{2} - v2; s3 = T{2} - s2; assert_array(v3, s3);
   v3 = T{2} * v2; s3 = T{2} * s2; assert_array(v3, s3);
   v3 = T{2} / v2; s3 = T{2} / s2; assert_array(v3, s3);
   v3 = T{2} << v2; s3 = T{2} << s2; assert_array(v3, s3);
   v3 = T{2} >> v2; s3 = T{2} >> s2; assert_array(v3, s3);
   v3 = T{2} % v2; s3 = T{2} % s2; assert_array(v3, s3);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<RealType T>
void assert_funcs()
{
   auto n = 300LL;
   std::valarray<T> v1(T(1), n);
   Array1D<T> s1(n, T(1));
   assert_array(v1, s1);

   v1 = T(2);
   s1 = T(2);
   assert_array(v1, s1);

   s1 = random<T>(Size{n});
   for(strict_int i = 0; i < n; ++i) {
      v1[i] = s1[i];
   }

   std::valarray<T> v2 = v1;
   Array1D<T> s2 = s1;
   assert_array(v2, s2);

   v2.resize(5);
   s2.resize(5);
   v2 = {T(1), T(2), T(3), T(4), T(5)};
   s2 = {T(1), T(2), T(3), T(4), T(5)};
   assert_array(v2, s2);

   std::valarray<T> v3 = {T(1), T(2), T(3), T(4), T(5)};
   Array1D<T> s3{T(1), T(2), T(3), T(4), T(5)}; // think whether this constructor should be explicit
   assert_array(v3, s3);

   Array1D<T> s4 = std::move(s3);
   assert_array(v3, s4);
   assert(s3.empty());

   s3.resize(s4.size());
   s3 = std::move(s4);
   assert_array(v3, s3);
   assert(s4.empty());

   s2 = s3 + T(1);
   v2 = v3 + T(1);
   s3.swap(s2);
   assert_array(v3, s2);
   assert_array(v2, s3);

   s2 = s3;
   s2.resize(3);
   assert(s2[0] == s3[0]);
   assert(s2[1] == s3[1]);
   assert(s2[2] == s3[2]);

   s2.resize(6);
   assert(s2[0] == s3[0]);
   assert(s2[1] == s3[1]);
   assert(s2[2] == s3[2]);

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

   assert_funcs<float>();
   assert_funcs<double>();
   assert_funcs<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_funcs<float128>();
   #endif
   return EXIT_SUCCESS;
}
