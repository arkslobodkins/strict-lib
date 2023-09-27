#include <cstdlib>
#include <iostream>
#include "../src/strict_lib.hpp"

using namespace strict;

int main()
{
   Array2D<double> A(11, 6);
   Array2D<double> B(11, 6);
   for(strict_int i{}; i < A.rows(); ++i) {
      for(strict_int j{}; j < A.cols(); ++j) {
         A(i, j) = strict_cast<double>(i + j + 1);
         B(i, j) = strict_cast<double>(i + j + 1) + 1.e-15;
      }
   }

   return EXIT_SUCCESS;
}
