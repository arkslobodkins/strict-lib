#include <cstdlib>
#include "../src/strict_lib.hpp"

using namespace strict;

int main()
{
   Array1D<int> r = random(Size{5}, Low{-10}, High{10});
   Array1D<double> x = random(Size{5}, Low{-1.}, High{1.});
   FixedArray1D<double, 5> y = random(Size{5}, Low{-1.}, High{1.});
   Array1D<double> z = x + y;

   z = {-5., -4., -3., -2., -1.};
   z = 2. * abs(x) +  2./sqrt(y);
   z = sequence(Size{5}, Start{-3.8}, Incr{0.2});
   z = e_unit<double>(Index{0}, Size{5});

   z.resize_and_assign(2. * x[seqN(0, 3)]);
   z.swap({1., 2., 3., 4., 5.});
   z.resize(13);
   z = linspace(Size{13}, Start{-3.}, End{3.});

   auto z_ptr = unique_blas_array(z);
   auto z_float = convert_type<float>(z);
   auto m = merge(z, 2.*z, 3.*z);

   bool all_pos = all_positive(z);
   bool all_greater = all_satisfy(z, [](auto x) { return x > -0.5; });
   bool any_greater = any_satisfy(z, [](auto x) { return x > -0.5; });

   std::cout << z << std::endl;
   print(z, "z", skip_line::yes);
   print_to_file("./ex2_results.txt", z, "z");

   return EXIT_SUCCESS;
}

