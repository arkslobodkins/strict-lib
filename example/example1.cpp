#include <cstdlib>
#include "../src/strict_array.hpp"

using namespace strict_array;

int main()
{
   Array<double> x = random<double>(Size{10}, Low{-1.}, High{1.});
   Array<double> y = random<double>(Size{10}, Low{-1.}, High{1.});
   Array<double> z = x + y;

   auto slice = within_cond(z, [](auto x) { return x > 0.; } );
   if(slice) {
      *slice *= 100.;
   }

   z = abs(z);

   auto all_pos = all_positive(z);
   auto all_neg = all_negative(z);
   auto all_greater = all_satisfy(z, [](auto x) { return x > -0.5; });
   auto any_greater = any_satisfy(z, [](auto x) { return x > -0.5; });

   std::cout << z << std::endl;
   print(z, "z");
   std::cout << std::endl;

   return EXIT_SUCCESS;
}
