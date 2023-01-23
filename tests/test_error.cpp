#include <cstdlib>
#include "../strict_array.hpp"

using namespace strict_array;

void assert_real()
{
   static_assert(RealType<short int>);
   static_assert(RealType<int>);
   static_assert(RealType<long int>);
   static_assert(RealType<long long int>);
   static_assert(RealType<float>);
   static_assert(RealType<double>);
   static_assert(RealType<long double>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(RealType<float128>);
   #endif
   static_assert(!RealType<unsigned int>);
   static_assert(!RealType<const int>);
   static_assert(!RealType<int &>);
}

void assert_int()
{
   static_assert(IntegerType<short int>);
   static_assert(IntegerType<int>);
   static_assert(IntegerType<long int>);
   static_assert(IntegerType<long long int>);
   static_assert(!IntegerType<double>);
}

void assert_floating()
{
   static_assert(FloatingType<float>);
   static_assert(FloatingType<double>);
   static_assert(FloatingType<long double>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(FloatingType<float128>);
   #endif
   static_assert(!FloatingType<int>);
}

void assert_standard_floating()
{
   static_assert(StandardFloatingType<float>);
   static_assert(StandardFloatingType<double>);
   static_assert(StandardFloatingType<long double>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!StandardFloatingType<float128>);
   #endif
   static_assert(!StandardFloatingType<int>);
}

void assert_quad_floating()
{
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(QuadType<float128>);
   #endif
   static_assert(!QuadType<float>);
   static_assert(!QuadType<double>);
   static_assert(!QuadType<long double>);
   static_assert(!QuadType<int>);
}

void assert_not_quad_floating()
{
   static_assert(NotQuadType<float>);
   static_assert(NotQuadType<double>);
   static_assert(NotQuadType<long double>);
   static_assert(NotQuadType<int>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!NotQuadType<float128>);
   #endif
}

void assert_int_base()
{
   static_assert(IntegerArrayBaseType<Array<short int>>);
   static_assert(IntegerArrayBaseType<Array<int>>);
   static_assert(IntegerArrayBaseType<Array<long int>>);
   static_assert(IntegerArrayBaseType<Array<long long int>>);
   static_assert(!IntegerArrayBaseType<Array<double>>);
   static_assert(!IntegerArrayBaseType<int>);
}

void assert_floating_base()
{
   static_assert(FloatingArrayBaseType<Array<float>>);
   static_assert(FloatingArrayBaseType<Array<double>>);
   static_assert(FloatingArrayBaseType<Array<long double>>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(FloatingArrayBaseType<Array<float128>>);
   #endif
   static_assert(!FloatingArrayBaseType<Array<int>>);
   static_assert(!FloatingArrayBaseType<int>);
}

void assert_standard_floating_base()
{
   static_assert(!StandardFloatingArrayBaseType<Array<int>>);
   static_assert(StandardFloatingArrayBaseType<Array<float>>);
   static_assert(StandardFloatingArrayBaseType<Array<double>>);
   static_assert(StandardFloatingArrayBaseType<Array<long double>>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!StandardFloatingArrayBaseType<Array<float128>>);
   #endif
   static_assert(!StandardFloatingArrayBaseType<int>);
}

void assert_quad_base()
{
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(QuadFloatingArrayBaseType<Array<float128>>);
   #endif
   static_assert(!QuadFloatingArrayBaseType<Array<int>>);
   static_assert(!QuadFloatingArrayBaseType<Array<float>>);
   static_assert(!QuadFloatingArrayBaseType<Array<double>>);
   static_assert(!QuadFloatingArrayBaseType<Array<long double>>);
   static_assert(!QuadFloatingArrayBaseType<int>);
}

void assert_not_quad_base()
{
   static_assert(NotQuadFloatingArrayBaseType<Array<int>>);
   static_assert(NotQuadFloatingArrayBaseType<Array<float>>);
   static_assert(NotQuadFloatingArrayBaseType<Array<double>>);
   static_assert(NotQuadFloatingArrayBaseType<Array<long double>>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!NotQuadFloatingArrayBaseType<Array<float128>>);
   #endif
   static_assert(!NotQuadFloatingArrayBaseType<int>);
}

int main(int argc, char *argv[])
{
   return EXIT_SUCCESS;
}
