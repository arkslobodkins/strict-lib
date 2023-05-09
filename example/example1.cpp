#include <cstdlib>
#include "../src/strict_array.hpp"

using namespace strict_array;

int main()
{
   Array<int> r = random<int>(Size{10}, Low{-10}, High{10});
   Array<double> x = random<double>(Size{10}, Low{-1.}, High{1.});
   Array<double> y = random<double>(Size{10}, Low{-1.}, High{1.});
   Array<double> z = x + y;

   auto slice_cond = within_cond(z, [](auto x) { return x > 0.; } );
   if(slice_cond) {
      *slice_cond *= 100.;
   }

   z[seqN(0, 5)] = 5. * x[seqN(0, 5)] + 10. * y[seqN(0, 5)];
   z[{0, 5, 9}] = 10.;
   z[{0, 5, 9}] = {0., 10., 18.};
   auto slice_even = z[seq(0, z.size()-1, 2)];
   slice_even = sqrt(abs(slice_even));

   z = e_unit<double>(Index{0}, Size{10});
   z = sequence<double>(Size{10}, Start{-10.}, Incr{1.});
   z = {-5., -4., -3., -2., -1., 0., 1., 2., 3., 4.};
   z += abs(z);

   z.resize_and_assign(2. * x[seqN(0, 5)]);
   z.resize(9);
   z = linspace<double>(Size{9}, Start{-2.}, End{2.});

   Array<float> z_float = z.convert_type<float>();
   std::unique_ptr<double[]> z_ptr = unique_blas_array(z);

   bool all_pos = all_positive(z);
   bool all_neg = all_negative(z);
   bool all_greater = all_satisfy(z, [](auto x) { return x > -0.5; });
   bool any_greater = any_satisfy(z, [](auto x) { return x > -0.5; });

   std::cout << z << std::endl;
   print(z, "z", skip_line::yes);

   return EXIT_SUCCESS;
}
