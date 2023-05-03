// Arkadijs Slobodkins, 2023

// For example, compile using a fairly recent version of gcc, clang or recent intel compiler from intel-oneAPI
// g++-12.2 -std=c++20 example.cpp
// dpcpp -std=c++20 example.cpp
// icpx -std=c++20 example.cpp

// To use quadruple precision, for example:
// g++-12.2 -std=gnu++20 example.cpp -lquadmath
// To enable debugging and range checking add -DSTRICT_DEBUG_ON

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "../src/strict_array.hpp"
#include "../src/strict_util.hpp"

using namespace strict_array;

template<typename F>
void derivative(Array<float64> & A, F f)
{
   constexpr Strict64 h = 0.000'001;
   constexpr Strict64 two_h = 2. * h;
   for(auto & x : A)
      x = ( f(x+h) - f(x-h) ) / two_h;
}

template<RealType T>
StrictVal<T> sign(StrictVal<T> x)
{
   return x > T{0} ? T{1} : T{-1};
}

int main()
{
   using std::cout, std::endl;

   // 1. Compute derivative of x * exp(x) and multiply by 2.
   // Expression 2. * A is evaluated on the fly twice(once when
   // printing and once when computing infinity norm).

   Array<float64> A{-1., -2., -3., -4., -5.};
   auto func = [](auto x) { return x * exps(x); };
   derivative(A, func);
   cout << 2. * A;
   cout << "infinity norm = " << norm_inf(2. * A) << endl << endl;

   // 2. Find all values in [-0.5, 0.5] and map them to
   // [-1, -0.5] and [0.5, 1], depending on the sign.
   // Note that if any other suffix or no suffix were
   // used instead of "F", the code would not compile,
   // because elements of Array are of type StrictVal<float32>,
   // i.e. StrictVal<float>.

   auto n = 100'000LL;
   Array<float32> B = random(Size{n}, Low{-1.F}, High{1.F});
   auto half_range = within_range(B, -0.5F, 0.5F);

   if(half_range)
      for(auto & x_ptr : *half_range)
         x_ptr += 0.5F * sign(x_ptr);
   for(auto x : B)
      assert(abss(x) >= 0.5F && abss(x) <= 1.F); // test mapping

   // 3. Fill C with 0, 1, 2, ..., then multiply C by 2
   // (which gives expression template) and
   // then use constant iterator of expression template to
   // sum every second element of 0, 2, 4, .., i.e.
   // sum of 0, 4, 8 ...

   Array<int> C = sequence<int>(Size{9});
   StrictVal<int> s{};
   auto expr = 2 * C;
   for(auto it = expr.begin(); it < expr.end(); it += 2)
      s += *it;
   cout << "sum = " << s << endl;                       // 0 + 4 + 8 + 12 + 16 is hopefully 40
   cout << "total sum = " << sum(expr) << endl << endl; // 0 + 2 + 4 ... is hopefully 72

   // 4. If for any reason there is a need to convert value to a different type,
   // real_cast() function can be used. Conversion is needed because both prod
   // and sqt in the example below are not float32.

   Array<float32> D{1.F, 2.F, 3.F, 4.F, 5.F};
   apply(D, [](auto & x) {x = x*x;});
   float64 prod = real_cast<float64>(dot_prod(D, D));
   for(auto it = D.begin(); it != D.end(); ++it) {
      float64 sqt = sqrt(real_cast<float64>(*it));
      cout << "square root of " << *it << " = " << sqt << endl;
   }
   cout << endl;

   // 5. Get raw array of zeros and ones. The type of the elements
   // of the raw array is float32, rather than Strictval<float32>.

   const Array<float32> E(5000);
   std::unique_ptr<float32[]> E1_ptr = unique_blas_array(E);
   std::unique_ptr<float32[]> E2_ptr = unique_blas_array(E + 1.F);
   float32* e1 = E1_ptr.get();
   float32* e2 = E2_ptr.get();

   // 6. Quadruple precision is supported on GCC. GNU extensions
   // must be enabled via -std=gnu++20.

   #ifdef STRICT_QUADRUPLE_PRECISION
   const Array<float128> F1 = sequence<float128>(Size{5});
   const Array<float128> F2 = sequence<float128>(Size{5});
   cout << F1 + F2 << endl;
   #endif

   // 7. StrictVal<T> was designed so that
   // StrictVal<T> and T can be used interchangeably.
   // Therefore, algorithms based on iterators are
   // compatible as well(as long as they are of the same type T).

   float64 g_raw[5]{};
   Array<float64> G{1., 2., 3., 4., 5.};
   std::copy(G.begin(), G.end(), g_raw);
   g_raw[1]++; g_raw[3]++;
   std::copy(g_raw, g_raw+5, G.begin());
   cout << G << endl;

   // 8. SliceArray class provides slicing for Array,
   // expression templates of Array, SliceArray itself,
   // and expression templates of SliceArray.

   Array<int> H = random(Size{9}, Low{0}, High{5});
   const Array<int> J = random(Size{9}, Low{0}, High{5});
   H[seq(0, 2)] = 2 * H[seq(3, 5)] + 10 * H[seq(6, 8)];

   auto first_five = H[seq(0, 4)];        // first_five refers to the first 5 entries of H
   first_five = 1;                        // set first five entries to 1
   first_five = {1, 2, 3, 4, 5};          // set them to 1, 2, 3, 4, 5
   for(auto & x : first_five) x = x*x*x;  // raise them to the 3rd power

   cout << 100 * H[seq(0, 2)] << endl;               // multiply SliceArray by 100
   cout << (100 * H)[seq(0, 2)] << endl;             // slice the Array expression template produced by 100 * H
   cout << 100 * H[seq(0, 4)][seq(0, 2)] << endl;    // slice the slice and multiply by 100
   cout << (100 * H[seq(0, 4)])[seq(0, 2)] << endl;  // slice the SliceArray expression template produced by 100 * H[seq(0, 4)]

   // 9. Just like for expression templates of Array, non-member functions
   // can be used for SliceArray and expression templates of SliceArray.

   Array<float64> K = sequence(Size{6}, Start{1.});
   auto m = max(K[seq(0, 3)]);
   auto n1 = norm2(K[seq(0, 2)]);
   auto n2 = norm2(K[seq(0, 2)] + K[seq(3, 5)]);
   auto d = dot_prod(K[seq(0, 2)], K[seq(3, 5)] + 1.);
   auto u_ptr = unique_blas_array(K[seq(0, 3)] + 1.);
   bool k2_finite = all_finite(K * 2.);
   bool pos = all_positive(K[seq(0, 2)]);
   bool neg = all_negative(-K[seq(0, 2)]);
   auto m_index = max_index(-1. * K[seq(1, 3)]); // produces expression template containing -2, -3, -4
                                                 // 0 is the index of max value, -2 is the max value
   Array<float64> L{-1., -2., -3., -4., -5., -6., -7.};
   K.Assign(L[seq(1, 6)]); // If type is not derived from ArrayBase,
                           // Assign routine can be used to assign
                           // other types of the same size.

   // 10. Non-contiguous slices.

   Array<float64> M = sequence(Size{6}, Start{1.});
   auto even_elem = M[seq(0, M.size()-1, 2)];
   even_elem = -1.;                              // set every even element of M to -1
   even_elem = 100. * M[seq(1, M.size()-1, 2)];  // even entries are set to 100 times odd entries
   even_elem = {1., 3., 5.};
   auto ns = norm2(even_elem);
   auto ms = max(M[seq(2, M.size()-1, 2)]);     // maximum of 2nd, 4th, and 6th elements
   for(auto & x : even_elem)                    // iterate over even elements and apply exponential function
      x = exps(x);
   even_elem = exp(even_elem);                  // or use exp function over the entire slice

   // 11. Other operations

   Array<float64> N = 10. * e_unit<float64>(0, 10000);                   // e_i unit vector is an expression template as well
   N.resize_and_assign(e_unit<float64>(0, 10) + e_unit<float64>(1, 10)); // N is resized to 10 and assigned to e_1 + e_2

   N[seq(0, 8, 2)] = e_slice_unit<float64>(3, 5);
   N = sequence(Size{N.size()}, Start{-5.});
   apply_if( N, [](auto & x) { x *= x; },                          // square entries that are greater than zero
               [](auto x) { return x > 0.; } );

   auto above_0 = within_cond( N, [](auto x) { return x > 0.; } ); // returns std::optional containing RandSliceArray
                                                                   // useful when more complicated action must be performed
                                                                   // on elements than passing a lambda to apply_if
   bool all_greater_1 =
      all_satisfy( N, [](auto x) { return x > 1.; } );

   bool all_greater_1_expr =
      all_satisfy( 2. + abs(N), [](auto x) { return x > 1.; } );

   bool any_greater_1 =
      any_satisfy( N, [](auto x) { return x > 1.; } );

   return EXIT_SUCCESS;
}

#endif
