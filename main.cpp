#include <cassert>
#include <cstdlib>
#include <iostream>
#include <valarray>
#include "strict_array.hpp"
#include <vector>

using namespace std;
using namespace strict_array;

template<typename T>
void assert_is_same(Array<T> x, valarray<T> y)
{ for(int i = 0; i < x.size(); ++i) assert(x[i] == y[i]); }

template<typename T>
void assert_within_tol(T x, T y)
{

   T max_val = std::max(x, y);
   if(max_val < 1.e-6) return;

   T diff = x - y;
   T abs_diff = diff > T(0) ? diff : -diff;
   assert(abs_diff / max_val < 1.e-6);
}

template<typename T>
void assert_within_tol(Array<T> x, valarray<T> y)
{
   for(int i = 0; i < x.size(); ++i)
   {
      T max_val = std::max(x[i], y[i]);
      if(max_val < 1.e-6) return;

      T diff = x[i] - y[i];
      T abs_diff = diff > T(0) ? diff : -diff;
      assert(abs_diff / max_val < 1.e-6);
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

   xa = val;
   xv = val;
   assert_within_tol(xa, xv);

   xa = ya + val;
   xv = yv + val;
   assert_within_tol(xa, xv);

   xa = ya - val;
   xv = yv - val;
   assert_within_tol(xa, xv);

   xa = ya * val;
   xv = yv * val;
   assert_within_tol(xa, xv);

   xa = ya / val;
   xv = yv / val;
   assert_within_tol(xa, xv);

   xa = val + ya;
   xv = val + yv;
   assert_within_tol(xa, xv);

   xa = val - ya;
   xv = val - yv;
   assert_within_tol(xa, xv);

   xa = val * ya;
   xv = val * yv;
   assert_within_tol(xa, xv);

   xa = val / ya;
   xv = val / yv;
   assert_within_tol(xa, xv);

   za = xa + ya;
   zv = xv + yv;
   assert_within_tol(za, zv);

   za = xa - ya;
   zv = xv - yv;
   assert_within_tol(za, zv);

   za = xa * ya;
   zv = xv * yv;
   assert_within_tol(za, zv);

   za = xa / ya;
   zv = xv / yv;
   assert_within_tol(za, zv);

   za += val;
   zv += val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za -= val;
   zv -= val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za *= val;
   zv *= val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za /= val;
   zv /= val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za += xa;
   zv += xv;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za -= xa;
   zv -= xv;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za *= xa;
   zv *= xv;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za /= xa;
   zv /= xv;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za += xa + ya - val;
   zv += xv + yv - val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za -= xa + ya - val;
   zv -= xv + yv - val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za *= xa + ya - val;
   zv *= xv + yv - val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   za /= xa + ya - val;
   zv /= xv + yv - val;
   assert_within_tol(za, zv);
   for(int i = 0; i < za.size(); ++i) zv[i] = za[i];

   auto sum_a = za.sum();
   auto sum_v = zv.sum();
   assert_within_tol(sum_a, sum_v);

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

   Array l_a = {T(1), T(2), T(3), T(-1), T(-2), T(-3)};
   valarray l_v = {T(1), T(2), T(3), T(-1), T(-2), T(-3)};
   assert_is_same(l_a, l_v);

   za = -za;
   zv = -zv;
   assert_is_same(za, zv);

   za = +za;
   zv = +zv;
   assert_is_same(za, zv);

   za.resize(10);
   zv.resize(10);
   assert(za.size() == zv.size());
}

int main()
{
   assert_all<float32> (10000, -3.2);
   assert_all<float64> (10000, -3.2);
   assert_all<long double> (10000, -3.2);
   #if defined __GNUC__  && !defined __clang__ && !defined __INTEL_LLVM_COMPILER && !defined __INTEL_COMPILER
   assert_all<float128> (10000, -3.2);
   #endif
   return EXIT_SUCCESS;
}
