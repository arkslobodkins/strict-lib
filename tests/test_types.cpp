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
   static_assert(IntegerBaseType<Array<short int>>);
   static_assert(IntegerBaseType<Array<int>>);
   static_assert(IntegerBaseType<Array<long int>>);
   static_assert(IntegerBaseType<Array<long long int>>);
   static_assert(!IntegerBaseType<Array<double>>);
   static_assert(!IntegerBaseType<int>);
}

void assert_floating_base()
{
   static_assert(FloatingBaseType<Array<float>>);
   static_assert(FloatingBaseType<Array<double>>);
   static_assert(FloatingBaseType<Array<long double>>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(FloatingBaseType<Array<float128>>);
   #endif
   static_assert(!FloatingBaseType<Array<int>>);
   static_assert(!FloatingBaseType<int>);
}

void assert_standard_floating_base()
{
   static_assert(!StandardFloatingBaseType<Array<int>>);
   static_assert(StandardFloatingBaseType<Array<float>>);
   static_assert(StandardFloatingBaseType<Array<double>>);
   static_assert(StandardFloatingBaseType<Array<long double>>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!StandardFloatingBaseType<Array<float128>>);
   #endif
   static_assert(!StandardFloatingBaseType<int>);
}

void assert_quad_base()
{
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(QuadFloatingBaseType<Array<float128>>);
   #endif
   static_assert(!QuadFloatingBaseType<Array<int>>);
   static_assert(!QuadFloatingBaseType<Array<float>>);
   static_assert(!QuadFloatingBaseType<Array<double>>);
   static_assert(!QuadFloatingBaseType<Array<long double>>);
   static_assert(!QuadFloatingBaseType<int>);
}

void assert_not_quad_base()
{
   static_assert(NotQuadFloatingBaseType<Array<int>>);
   static_assert(NotQuadFloatingBaseType<Array<float>>);
   static_assert(NotQuadFloatingBaseType<Array<double>>);
   static_assert(NotQuadFloatingBaseType<Array<long double>>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!NotQuadFloatingBaseType<Array<float128>>);
   #endif
   static_assert(!NotQuadFloatingBaseType<int>);
}

int main(int argc, char *argv[])
{
   return EXIT_SUCCESS;
}
