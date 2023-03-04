#include <cstdlib>
#include <cassert>
#include <valarray>

#include "../strict_array.hpp"
#include "../strict_util.hpp"

using namespace strict_array;

template<RealType T, SliceArrayBaseType1D SliceArrayBaseT1D>
void assert_slicearray(const std::valarray<T> & v, const SliceArrayBaseT1D & s)
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

   auto s3_slice = s3[seq(0, n-1)];
   v3 += T{2}; s3[seq(0, n-1)] += T{2}; assert_slicearray(v3, s3_slice);
   v3 -= T{2}; s3[seq(0, n-1)] -= T{2}; assert_slicearray(v3, s3_slice);
   v3 *= T{2}; s3[seq(0, n-1)] *= T{2}; assert_slicearray(v3, s3_slice);
   v3 /= T{2}; s3[seq(0, n-1)] /= T{2}; assert_slicearray(v3, s3_slice);

   v3 += v2; s3[seq(0, n-1)] += s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 -= v2; s3[seq(0, n-1)] -= s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 *= v2; s3[seq(0, n-1)] *= s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 /= v2; s3[seq(0, n-1)] /= s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);

   v3 += T{2} + v2; s3[seq(0, n-1)] += T{2} + s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 -= T{2} - v2; s3[seq(0, n-1)] -= T{2} - s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 *= T{2} * v2; s3[seq(0, n-1)] *= T{2} * s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 /= T{2} / v2; s3[seq(0, n-1)] /= T{2} / s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);

   v3 += v2 + T{2}; s3[seq(0, n-1)] += s2[seq(0, n-1)] + T{2}; assert_slicearray(v3, s3_slice);
   v3 -= v2 + T{2}; s3[seq(0, n-1)] -= s2[seq(0, n-1)] + T{2}; assert_slicearray(v3, s3_slice);
   v3 *= v2 + T{2}; s3[seq(0, n-1)] *= s2[seq(0, n-1)] + T{2}; assert_slicearray(v3, s3_slice);
   v3 /= v2 + T{2}; s3[seq(0, n-1)] /= s2[seq(0, n-1)] + T{2}; assert_slicearray(v3, s3_slice);

   v3 = v1 + v2; s3[seq(0, n-1)] = s1[seq(0, n-1)] + s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 = v1 - v2; s3[seq(0, n-1)] = s1[seq(0, n-1)] - s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 = v1 * v2; s3[seq(0, n-1)] = s1[seq(0, n-1)] * s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 = v1 / v2; s3[seq(0, n-1)] = s1[seq(0, n-1)] / s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);

   v3 = v1 + T{2}; s3[seq(0, n-1)] = s1[seq(0, n-1)] + T{2}; assert_slicearray(v3, s3_slice);
   v3 = v1 - T{2}; s3[seq(0, n-1)] = s1[seq(0, n-1)] - T{2}; assert_slicearray(v3, s3_slice);
   v3 = v1 * T{2}; s3[seq(0, n-1)] = s1[seq(0, n-1)] * T{2}; assert_slicearray(v3, s3_slice);
   v3 = v1 / T{2}; s3[seq(0, n-1)] = s1[seq(0, n-1)] / T{2}; assert_slicearray(v3, s3_slice);

   v3 = T{2} + v2; s3[seq(0, n-1)] = T{2} + s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 = T{2} - v2; s3[seq(0, n-1)] = T{2} - s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 = T{2} * v2; s3[seq(0, n-1)] = T{2} * s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);
   v3 = T{2} / v2; s3[seq(0, n-1)] = T{2} / s2[seq(0, n-1)]; assert_slicearray(v3, s3_slice);

   v3 = T{2} + v1 + v2 - T{3}; s3[seq(0, n-1)] = T{2} + s1[seq(0, n-1)] + s2[seq(0, n-1)] - T{3}; assert_slicearray(v3, s3_slice);
   v3 = T{2} + v1 - v2 - T{3}; s3[seq(0, n-1)] = T{2} + s1[seq(0, n-1)] - s2[seq(0, n-1)] - T{3}; assert_slicearray(v3, s3_slice);
   v3 = T{2} + v1 * v2 - T{3}; s3[seq(0, n-1)] = T{2} + s1[seq(0, n-1)] * s2[seq(0, n-1)] - T{3}; assert_slicearray(v3, s3_slice);
   v3 = T{2} + v1 / v2 - T{3}; s3[seq(0, n-1)] = T{2} + s1[seq(0, n-1)] / s2[seq(0, n-1)] - T{3}; assert_slicearray(v3, s3_slice);
   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

int main(int argc, char *argv[])
{
   assert_float_ops<float>();
   assert_float_ops<double>();
   assert_float_ops<long double>();
   return EXIT_SUCCESS;
}
