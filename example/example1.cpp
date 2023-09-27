#include <cstdlib>
#include <iostream>

#include "../src/strict_lib.hpp"

using namespace strict;

int main()
{
   StrictVal<long int> s1{1L}; // won't compile unless s1 is initialized
                               // with [long int] or [StrictVal<long int>]

   long int v1{1};             // works fine for fundamental types, as long
                               // as there is no narrowing(due to curly braces)

   StrictVal<long int> s2 = s1 + v1; // [StrictVal<long int>] + [long int] produces [StrictVal<long int>]
   s2 = v1 + s1;
   s2 = s1 + s2;
   s2 += 1L;                         // would not compile without [L] suffix
   s2 <<= 1L;
   bool g = s2 > 0L;

   StrictVal<double> s3 = strict_cast<double>(s2); // strict_cast is essentially a static_cast for StrictVals
                                                   // in this example, it returns [StrictVal<double>]
   v1 = real_cast<long int>(s3);                   // real_cast is similar, but returns a regular [long int]


   // functions such as sqrts, sins, logs only accept floating-point
   // types as arguments, and of course [StrictVal<floating-type>]
   s3 = sqrts<double>(4.);
   s3 = sqrts(s3);            // returns [StrictVal<double>]
   s3 = sins(s3);
   s3 = pows_int(s3, 2);      // pows_int can be raised to any integer type([short], [int], [long], [StrictVal<int>], etc.)
   s3 = fast_pows_int(s3, 2); // fast but roundoff error is slightly greater than pows_int function

   bool is_close = util::within_tol_rel(abss(s3), sqrts(s3 * s3));

   StrictVal<strict_int> fact = math::factorial(5);        // strict_int fact = ... would compile as well
   StrictVal<strict_int> binom = math::binom_coeff(10, 5); // very effective at avoiding overflow
   std::cout << binom << std::endl;

   return EXIT_SUCCESS;
}

