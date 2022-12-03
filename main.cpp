#include <cassert>
#include <cstdlib>
#include <iostream>
#include <valarray>
#include "garray.hpp"

using namespace std;
using namespace garray;

template<typename T>
void assert_is_same(Array<T> x, valarray<T> y)
{ for(int i = 0; i < x.size(); ++i) assert(x[i] == y[i]); }

template<typename T>
void assert_within_tol(Array<T> x, valarray<T> y)
{
   for(int i = 0; i < x.size(); ++i)
   {
      T diff = x[i] - y[i];
      T abs_val = diff > 0 ? diff : -diff;
      assert(abs_val < 1.e-3);
   }
}

template<typename T>
void assert_all(int n, T val)
{
   Array xa = array_random<T>(n, T(0.1), T(0.9));
   Array ya = array_random<T>(n, T(0.1), T(0.9));
   Array za = array_random<T>(n);
   valarray<T> xv(n);
   valarray<T> yv(n);
   valarray<T> zv(n);
   for(int i = 0; i < n; ++i) xv[i] = xa[i];
   for(int i = 0; i < n; ++i) yv[i] = ya[i];

   xa = ya + val;
   xv = yv + val;
   assert_is_same(xa, xv);

   xa = ya - val;
   xv = yv - val;
   assert_is_same(xa, xv);

   xa = ya * val;
   xv = yv * val;
   assert_is_same(xa, xv);

   xa = ya / val;
   xv = yv / val;
   assert_is_same(xa, xv);

   xa = val + ya;
   xv = val + yv;
   assert_is_same(xa, xv);

   xa = val - ya;
   xv = val - yv;
   assert_is_same(xa, xv);

   xa = val * ya;
   xv = val * yv;
   assert_is_same(xa, xv);

   xa = val / ya;
   xv = val / yv;
   assert_is_same(xa, xv);

   za = xa + ya;
   zv = xv + yv;
   assert_is_same(za, zv);

   za = xa - ya;
   zv = xv - yv;
   assert_is_same(za, zv);

   za = xa * ya;
   zv = xv * yv;
   assert_is_same(za, zv);

   za = xa / ya;
   zv = xv / yv;
   assert_is_same(za, zv);

   za += val;
   zv += val;
   assert_is_same(za, zv);

   za -= val;
   zv -= val;
   assert_is_same(za, zv);

   za *= val;
   zv *= val;
   assert_is_same(za, zv);

   za /= val;
   zv /= val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za += xa;
   zv += xv;
   assert_is_same(za, zv);

   za -= xa;
   zv -= xv;
   assert_is_same(za, zv);

   za *= xa;
   zv *= xv;
   assert_is_same(za, zv);

   za /= xa;
   zv /= xv;
   assert_is_same(za, zv);

   za += xa + ya - val;
   zv += xv + yv - val;
   assert_is_same(za, zv);

   za -= xa + ya - val;
   zv -= xv + yv - val;
   assert_is_same(za, zv);

   za *= xa + ya - val;
   zv *= xv + yv - val;
   assert_is_same(za, zv);

   za /= xa + ya - val;
   zv /= xv + yv - val;
   assert_is_same(za, zv);

   auto sum_a = za.sum();
   auto sum_v = zv.sum();
   assert(sum_a == sum_v);

   auto max_a = za.max();
   auto max_v = zv.max();
   assert(max_a == max_v);

   auto min_a = za.min();
   auto min_v = zv.min();
   assert(min_a == min_v);

   auto max_index = za.max_index();
   assert(max_a == max_index.second);
   auto min_index = za.min_index();
   assert(min_a == min_index.second);

   Array l_a = {1., 2., 3., -1., -2., -3.};
   valarray l_v = {1., 2., 3., -1., -2., -3.};
   assert_is_same(l_a, l_v);

   za = -za;
   zv = -zv;
   assert_is_same(za, zv);

   za = +za;
   zv = +zv;
   assert_is_same(za, zv);
}

int main()
{
   assert_all<float32> (10000, -3.2);
   assert_all<float64> (10000, -3.2);
   assert_all<float128> (10000, -3.2);
   assert_all<long double> (10000, -3.2);

   return EXIT_SUCCESS;
}
