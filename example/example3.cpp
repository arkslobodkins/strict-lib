#include <cstdlib>
#include <numbers>
#include "../src/strict_lib.hpp"

using namespace strict;

int main()
{
   Array1D<double> A = random(Size{10}, Low{-1.}, High{1.});
   A.remove_element(last-1);
   ++A[last];

   A[seq(0, 2)] *= 2.;
   A[seqN(0, 3)] *= 2.;
   A[{0, 1, 2}] *= 2.;

   A[{0, 2, 4}] = A[seq(6, 8)];
   auto n = norm2(A[even]);
   auto M = merge(A, A*2.);
   auto skip4a = A[even][even];
   auto skip4b = A[skipN(4)];

   auto slice_in_range = within_range(A, Low{-0.5}, High{0.5});
   slice_in_range = pow_int(slice_in_range, 2);

   auto slice_cond = within_cond(A, [](auto x) { return x*x > x + 1.; } );
   A.remove_elements(slice_cond.indexes());

   for(auto i : index_range(A))
      A[i] = strict_cast<double>(i) + std::numbers::pi_v<double>;

   return EXIT_SUCCESS;
}

