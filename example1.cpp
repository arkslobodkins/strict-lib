#include <cstdlib>
#include <iostream>
#include "strict_array.hpp"

int main(int argc, char *argv[])
{
   using std::cout, std::endl;
   using namespace strict_array;

   auto n = 1000LL;
   Array<double> xd = sequence<double>(Size{n}); // 0, 1, 2, ...
   Array<double> yd = random<double>(Size{n});   // random values between 0 and 1

   Array<double> zd = xd + yd;
   zd += 2. * xd + 2. / yd;
   zd = abs(2. - xd);
   zd = exp(2. - xd);

   StrictVal<double> s1 = sum(zd);
   StrictVal<double> s2 = stable_sum(zd);

   auto seq1 = zd[all];
   auto seq2 = zd[seq(0, zd.size()-1, 2)];   // every second
   auto seq3 = zd[seqN(0, zd.size()/2, 2)];  // every second
   auto seq4 = zd[index_vec{0, 1}];          // first two
   auto seq5 = zd[{0, 1, 2, 3, 4, 5}];       // first five
   zd[seq(1, zd.size()-1, 2)] = 2. * seq2;   // assign 2 * even_elements to odd_elements

   sort_decreasing(zd);
   StrictVal<double> dp1 = dot_prod(xd, yd);
   StrictVal<double> dp2 = dot_prod(2. * xd, -yd);
   StrictVal<double> nt1 = norm2(zd);
   StrictVal<double> nt2 = norm2(zd[seq(0, 4)]);

   return EXIT_SUCCESS;
}
