#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <strict_lib.hpp>


// example 0 demonstrates how Strict arithmetic types can help
// to prevent errors that arise when using built-in arithmetic types
int main() {
   using namespace std;
   using namespace slib;


   // 1.
   long double ld_bad{2.2};    // loses precision, likely a bug
   long double ld_good{2.2L};  // ok, retains all digits
   cout << scientific << setprecision(18);
   cout << "long double initialized with 2.2:   " << ld_bad << '\n';
   cout << "long double initialized with 2.2L:  " << ld_good << "\n\n";

   Strict<long double> sl{2.2L};
   // Strict<long double> s3{2.2}; // won't compile!

   // 2.
   double A[]{0.1, 0.2, 0.3, 0.4, 0.5};
   cout << "result of accumulate when passing 0(deduces int):      "
      << accumulate(begin(A), end(A), 0)
        << "\n";  // accumulates in int, likely a bug

   cout << "result of accumulate when passing 0.(deduces double):  "
      << accumulate(begin(A), end(A), 0.)
        << "\n\n";  // ok, accumulates in double

   Strict64 B[]{0.1_sd, 0.2_sd, 0.3_sd, 0.4_sd, 0.5_sd};
   (void)accumulate(begin(B), end(B), 0._sd);  // as expected
   // accumulate(begin(B), end(B), 0_si);   // won't compile!!!

   return EXIT_SUCCESS;
}
