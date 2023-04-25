#include <cstdlib>
#include <cassert>
#include <valarray>

#include "../src/strict_array.hpp"
#include "../src/strict_util.hpp"

using namespace strict_array;
using namespace strict_array::util;

template<RealType T, SliceArrayBaseType1D SliceArrayBaseT1D>
void assert_randarray(const std::valarray<T> & v, const SliceArrayBaseT1D & s)
{
   assert((long long int)v.size() == s.size());
   for(auto i = 0LL; i < s.size(); ++i)
      assert(is_within_tol_abs<T>(v[i], s[i]));
}

template<StandardFloatingType T>
void assert_float_ops()
{
   auto n = 300LL;
   Array<T> s1 = random<T>(Size{300}, Low{T(-1.)}, High{T(1.)});
   Array<T> s2 = random<T>(Size{300}, Low{T(0.01)}, High{T(1.)});
   Array<T> s3(n);

   std::valarray<T> v1(n), v2(n), v3(n);
   for(auto i = 0LL; i < n; ++i) v1[i] = s1[i];
   for(auto i = 0LL; i < n; ++i) v2[i] = s2[i];

   index_vec ind(n);
   for(auto i = 0LL; i < n; ++i)
      ind[i] = i;

   auto s3_slice = s3[ind];
   v3 += T{2}; s3[ind] += T{2}; assert_randarray(v3, s3_slice);
   v3 -= T{2}; s3[ind] -= T{2}; assert_randarray(v3, s3_slice);
   v3 *= T{2}; s3[ind] *= T{2}; assert_randarray(v3, s3_slice);
   v3 /= T{2}; s3[ind] /= T{2}; assert_randarray(v3, s3_slice);

   v3 += v2; s3[ind] += s2[ind]; assert_randarray(v3, s3_slice);
   v3 -= v2; s3[ind] -= s2[ind]; assert_randarray(v3, s3_slice);
   v3 *= v2; s3[ind] *= s2[ind]; assert_randarray(v3, s3_slice);
   v3 /= v2; s3[ind] /= s2[ind]; assert_randarray(v3, s3_slice);

   v3 += T{2} + v2; s3[ind] += T{2} + s2[ind]; assert_randarray(v3, s3_slice);
   v3 -= T{2} - v2; s3[ind] -= T{2} - s2[ind]; assert_randarray(v3, s3_slice);
   v3 *= T{2} * v2; s3[ind] *= T{2} * s2[ind]; assert_randarray(v3, s3_slice);
   v3 /= T{2} / v2; s3[ind] /= T{2} / s2[ind]; assert_randarray(v3, s3_slice);

   v3 += v2 + T{2}; s3[ind] += s2[ind] + T{2}; assert_randarray(v3, s3_slice);
   v3 -= v2 + T{2}; s3[ind] -= s2[ind] + T{2}; assert_randarray(v3, s3_slice);
   v3 *= v2 + T{2}; s3[ind] *= s2[ind] + T{2}; assert_randarray(v3, s3_slice);
   v3 /= v2 + T{2}; s3[ind] /= s2[ind] + T{2}; assert_randarray(v3, s3_slice);

   v3 = v1 + v2; s3[ind] = s1[ind] + s2[ind]; assert_randarray(v3, s3_slice);
   v3 = v1 - v2; s3[ind] = s1[ind] - s2[ind]; assert_randarray(v3, s3_slice);
   v3 = v1 * v2; s3[ind] = s1[ind] * s2[ind]; assert_randarray(v3, s3_slice);
   v3 = v1 / v2; s3[ind] = s1[ind] / s2[ind]; assert_randarray(v3, s3_slice);

   v3 = v1 + T{2}; s3[ind] = s1[ind] + T{2}; assert_randarray(v3, s3_slice);
   v3 = v1 - T{2}; s3[ind] = s1[ind] - T{2}; assert_randarray(v3, s3_slice);
   v3 = v1 * T{2}; s3[ind] = s1[ind] * T{2}; assert_randarray(v3, s3_slice);
   v3 = v1 / T{2}; s3[ind] = s1[ind] / T{2}; assert_randarray(v3, s3_slice);

   v3 = T{2} + v2; s3[ind] = T{2} + s2[ind]; assert_randarray(v3, s3_slice);
   v3 = T{2} - v2; s3[ind] = T{2} - s2[ind]; assert_randarray(v3, s3_slice);
   v3 = T{2} * v2; s3[ind] = T{2} * s2[ind]; assert_randarray(v3, s3_slice);
   v3 = T{2} / v2; s3[ind] = T{2} / s2[ind]; assert_randarray(v3, s3_slice);

   v3 = T{2} + v1 + v2 - T{3}; s3[ind] = T{2} + s1[ind] + s2[ind] - T{3}; assert_randarray(v3, s3_slice);
   v3 = T{2} + v1 - v2 - T{3}; s3[ind] = T{2} + s1[ind] - s2[ind] - T{3}; assert_randarray(v3, s3_slice);
   v3 = T{2} + v1 * v2 - T{3}; s3[ind] = T{2} + s1[ind] * s2[ind] - T{3}; assert_randarray(v3, s3_slice);
   v3 = T{2} + v1 / v2 - T{3}; s3[ind] = T{2} + s1[ind] / s2[ind] - T{3}; assert_randarray(v3, s3_slice);
   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

int main(int argc, char *argv[])
{
   assert_float_ops<float>();
   assert_float_ops<double>();
   assert_float_ops<long double>();
   return EXIT_SUCCESS;
}
