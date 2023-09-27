#include <cstdlib>
#include <iostream>

#include "../src/strict_lib.hpp"

int main()
{
   using namespace strict;
   using std::cout, std::endl;

   // 1.
   strict_int n = 100'000;
   Array1D<float64> A = random(Size{n}, Low{-2.71}, High{2.71});
   Array1D<float128> B = convert_type<float128>(A);

   cout << "product using double precision:        " << prod(A) << endl;
   cout << "stable product using double precision: " << stable_prod(A) << endl;
   cout << "product using quadruple precision:     " << prod(B) << endl << endl;

   // 2.
   n = 1'000'000;
   auto A_init = random(Size{n}, Low{0.}, High{10.});
   A.resize_and_assign(std::move(A_init));
   A[odd] = -A[even] + 1.e-16;
   B.resize_and_assign( convert_type<float128>(A) );

   cout << "sum using double precision:        " << sum(A) << endl;
   cout << "stable sum using double precision: " << stable_sum(A) << endl;
   cout << "sum using quadruple precision:     " << sum(B) << endl << endl;

   // 3.
   auto D1 = random(Size{n}, Low{0.}, High{10.});
   auto D2 = random(Size{n}, Low{0.}, High{10.});
   D1[odd] = D1[even] + 1.e-8;
   D2[odd] = -D2[even] + 1.e-8;
   auto DQ1 = convert_type<float128>(D1);
   auto DQ2 = convert_type<float128>(D2);

   cout << "dot product using double precision:        " << dot_prod(D1, D2) << endl;
   cout << "stable dot product using double precision: " << stable_dot_prod(D1, D2) << endl;
   cout << "dot product using quadruple precision:     " << dot_prod(DQ1, DQ2) << endl;

   return EXIT_SUCCESS;
}

