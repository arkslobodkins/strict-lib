#include <cstdlib>

#include <cstdlib>
#include <cassert>
#include <valarray>

#include "../src/strict_array.hpp"
#include "../src/strict_util.hpp"

using namespace strict_array;

template<FloatingType T>
void assert_inf_norm()
{
   Array<T> A{T{1}, T{2}, T{3}, T{4}, T{5}};
   assert(within_tol_abs<T>(norm_inf(A), T{5}));

   A = {T{-1}, T{-2}, T{-3}, T{-4}, T{-5}};
   assert(within_tol_abs<T>(norm_inf(A), T{5}));

   A = {T{5}, T{4}, T{3}, T{2}, T{1}};
   assert(within_tol_abs<T>(norm_inf(A), T{5}));

   A = {T{-5}, T{-4}, T{-3}, T{-2}, T{-1}};
   assert(within_tol_abs<T>(norm_inf(A), T{5}));

   A = {T{-3}, T{-2}, T{-5}, T{-4}, T{-1}};
   assert(within_tol_abs<T>(norm_inf(A), T{5}));

   Array<T> B{T{2}, T{8}, T{6}, T{4}, T{0}, T{-4}, T{-2}};
   assert(within_tol_abs<T>(norm_inf(B[seq(0, 2)]), T{8}));
   assert(within_tol_abs<T>(norm_inf(B[seq(0, 2)] + T{1}), T{9}));
   assert(within_tol_abs<T>(norm_inf(B[seq(0, 2)] * T{2}), T{16}));
   assert(within_tol_abs<T>(norm_inf(B[seq(0, 2)] + B[seq(1, 3)]), T{14}));
   assert(within_tol_abs<T>(norm_inf(B[seq(0, 4)][seq(3, 4)]), T{4}));
   assert(within_tol_abs<T>(norm_inf(T{4}*B[seq(3, 4)]), T{16}));
   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_max()
{
   Array<T> A{T{1}, T{2}, T{3}, T{4}, T{5}};
   assert(within_tol_abs<T>(max(A), T{5}));

   A = {T{-1}, T{-2}, T{-3}, T{-4}, T{-5}};
   assert(within_tol_abs<T>(max(A), T{-1}));
   assert(within_tol_abs<T>(max(T{-1} * A), T{5}));
   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_max()
{
   Array<T> A{T{1}, T{2}, T{3}, T{4}, T{5}};
   assert(max(A) == T{5});

   A = {T{-1}, T{-2}, T{-3}, T{-4}, T{-5}};
   assert(max(A) == T{-1});
   assert(max(T{-1} * A) == T{5});
   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

int main(int argc, char *argv[])
{
   assert_inf_norm<float>();
   assert_inf_norm<double>();
   assert_inf_norm<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_inf_norm<float128>();
   #endif

   assert_max<int>();
   assert_max<long int>();
   assert_max<float>();
   assert_max<double>();
   assert_max<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_max<float128>();
   #endif

   return EXIT_SUCCESS;
}
