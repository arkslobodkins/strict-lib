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
   zd = exp(2. * yd);

   auto seq1 = zd[all];
   auto seq2 = zd[seq(0, zd.size()-1, 2)];   // every second
   auto seq3 = zd[seqN(0, zd.size()/2, 2)];  // every second
   auto seq4 = zd[index_vec{0, 1}];          // first two
   auto seq5 = zd[{0, 1, 2, 3, 4, 5}];       // first five
   zd[seq(1, zd.size()-1, 2)] = 2. * seq2;   // assign 2 * even_elements to odd_elements

   StrictVal<double> s1 = sum(zd);
   StrictVal<double> s2 = stable_sum(zd);
   StrictVal<double> mx = max(zd);
   StrictVal<double> mn = min(zd);

   StrictVal<double> dp1 = dot_prod(xd, yd);
   StrictVal<double> dp2 = dot_prod(2. * xd, -yd);

   StrictVal<double> nt1 = norm2(zd);
   StrictVal<double> nt2 = norm2(zd[seq(0, 4)]);
   StrictVal<double> snt1 = stable_norm2(zd);
   StrictVal<double> snt2 = stable_norm2(zd[seq(0, 4)]);

   bool all_p = all_positive(zd);
   bool all_n = all_negative(zd);

   sort_decreasing(zd);
   sort_increasing(zd[seq(0, 4)]);  // sort first five elements

   auto count = 1LL;
   bool all_sat = all_satisfy(zd, [&count](auto x) { return x > strict_cast<double>(count++); });
   bool any_sat = any_satisfy(zd[seq(0, 10)], [](auto x) { return x > sqrts<double>(10.); });

   std::unique_ptr<double[]> raw_ptr = unique_blas_array(zd);

   // within_unit is std optional of RandSliceArray, which contains all
   // elements that are within the interval [0, 1]
   zd = random(Size{n}, Low{-1.}, High{1.});
   auto within_unit = within_range(zd, 0., 1.);
   if(within_unit) {
      std::vector<strict_int> indexes = within_unit->indexes();
      *within_unit = 0.;
   }

   // gr_zero is std optional of RandSliceArray, which contains all
   // elements that are greater than 0
   auto gr_zero = within_cond(zd, [](auto x) { return x > 0.; });
   if(gr_zero) *gr_zero = 0.;

   // gr_one is std optional of ConstRandSliceArray. Elements cannot be modified
   const auto ud = zd;
   auto gr_one = within_cond(ud, [](auto x) { return x > 0.; });

   zd.resize(3);
   zd = {-1., 0., 1.};
   zd.resize_and_assign(xd);
   zd.resize(3);

   Array<double> td = Construct(zd[seq(0, 2)]); // to construct array from slicearray, Construct is used
   td.Assign(xd[seq(0, 2)]);                    // to assign slicearray to array, Assign is used
   xd[seq(0, 2)].Assign(td);                    // Conversely, ...

   Array<float> zf1 = zd.convert_type<float>();  // to convert the type, convert_type is used

   // currently one of the two ways to construct Array
   // from a slice of array of a different type

   Array<long double> zf2 =
      Construct( zd[seq(0, 2)] ).convert_type<long double>();

   Array<long double> zf3 =
      Construct( zd.convert_type<long double>()[seq(0, 2)] );

   zf3 = e_unit<long double>(0, zf3.size()); // e0 unit vector
   zf3 = Array<long double>(3, 1.L);         // vector of ones
   zf3 = {-5.L, 0.L, 5.L};


   return EXIT_SUCCESS;
}
