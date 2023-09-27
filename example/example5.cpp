#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "../src/strict_lib.hpp"

int main()
{
   using namespace strict;
   using std::cout, std::endl;

   Strict64 err{0.}, err_stable{0.};
   Strict64 ave{0.}, ave_stable{0.};
   int its{10'000};
   int m{100};
   for(auto i = 0; i < its; ++i)
   {
      Array1D<float64> X = random(Size{m}, Low{-1.}, High{1.});
      Array1D<float64> C = random(Size{m}, Low{-1.}, High{1.});
      Array1D<int> P = random(Size{m}, Low{0}, High{10});
      auto CQuad = convert_type<float128>(C);
      auto XQuad = convert_type<float128>(X);

      Strict64 p = polynomial(C, X, P);
      Strict64 pstable = stable_polynomial(C, X, P);
      Strict64 pquad = strict_cast<float64>(polynomial(CQuad, XQuad, P));

      auto rel = util::rel_error(p, pquad);
      auto rel_stable = util::rel_error(pstable, pquad);
      if(rel && rel_stable) {
         ave += *rel;
         err = maxs<float64>(err, *rel);
         ave_stable += *rel_stable;
         err_stable = maxs<float64>(err_stable, *rel_stable);
      }
      else {
         throw std::runtime_error{"relative error could not be computed"};
      }
   }

   cout << "maximum relative error using  polynomial: " << err << endl;
   cout << "average relative error using polynomial: " << ave/float64(its) << endl;
   cout << "maximum relative error using stable_polynomial: " << err_stable << endl;
   cout << "average relative error using stable_polynomial: " << ave_stable/float64(its) << endl;

   return EXIT_SUCCESS;
}

