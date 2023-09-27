#include <cstdlib>
#include "../src/strict_lib.hpp"

using namespace strict;

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

   std::cout << "passed " << __func__ << std::endl;
}

void assert_int()
{
   static_assert(IntegerType<short int>);
   static_assert(IntegerType<int>);
   static_assert(IntegerType<long int>);
   static_assert(IntegerType<long long int>);

   static_assert(!IntegerType<double>);

   std::cout << "passed " << __func__ << std::endl;
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

   std::cout << "passed " << __func__ << std::endl;
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

   std::cout << "passed " << __func__ << std::endl;
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

   std::cout << "passed " << __func__ << std::endl;
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

   std::cout << "passed " << __func__ << std::endl;
}

void assert_int_base()
{
   static_assert(IntegerBaseType<Array1D<short int>>);
   static_assert(IntegerBaseType<Array1D<int>>);
   static_assert(IntegerBaseType<Array1D<long int>>);
   static_assert(IntegerBaseType<Array1D<long long int>>);

   static_assert(!IntegerBaseType<int>);
   static_assert(!IntegerBaseType<Array1D<double>>);

   static_assert(IntegerBaseType<const Array1D<int>>);
   static_assert(!IntegerBaseType<Array1D<int>&>);

   std::cout << "passed " << __func__ << std::endl;
}

void assert_floating_base()
{
   static_assert(FloatingBaseType<Array1D<float>>);
   static_assert(FloatingBaseType<Array1D<double>>);
   static_assert(FloatingBaseType<Array1D<long double>>);
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(FloatingBaseType<Array1D<float128>>);
   #endif

   static_assert(!FloatingBaseType<int>);
   static_assert(!FloatingBaseType<Array1D<int>>);

   static_assert(FloatingBaseType<const Array1D<float>>);
   static_assert(!FloatingBaseType<Array1D<float> &>);

   std::cout << "passed " << __func__ << std::endl;
}

void assert_standard_floating_base()
{
   static_assert(!StandardFloatingBaseType<Array1D<int>>);
   static_assert(StandardFloatingBaseType<Array1D<float>>);
   static_assert(StandardFloatingBaseType<Array1D<double>>);
   static_assert(StandardFloatingBaseType<Array1D<long double>>);

   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!StandardFloatingBaseType<Array1D<float128>>);
   #endif
   static_assert(!StandardFloatingBaseType<int>);

   static_assert(StandardFloatingBaseType<const Array1D<float>>);
   static_assert(!StandardFloatingBaseType<Array1D<float> &>);

   std::cout << "passed " << __func__ << std::endl;
}

void assert_quad_base()
{
   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(QuadFloatingBaseType<Array1D<float128>>);
   #endif

   static_assert(!QuadFloatingBaseType<Array1D<int>>);
   static_assert(!QuadFloatingBaseType<Array1D<float>>);
   static_assert(!QuadFloatingBaseType<Array1D<double>>);
   static_assert(!QuadFloatingBaseType<Array1D<long double>>);
   static_assert(!QuadFloatingBaseType<int>);

   std::cout << "passed " << __func__ << std::endl;
}

void assert_not_quad_base()
{
   static_assert(NotQuadFloatingBaseType<Array1D<int>>);
   static_assert(NotQuadFloatingBaseType<Array1D<float>>);
   static_assert(NotQuadFloatingBaseType<Array1D<double>>);
   static_assert(NotQuadFloatingBaseType<Array1D<long double>>);

   #ifdef STRICT_QUADRUPLE_PRECISION
   static_assert(!NotQuadFloatingBaseType<Array1D<float128>>);
   #endif
   static_assert(!NotQuadFloatingBaseType<int>);

   std::cout << "passed " << __func__ << std::endl;
}

void assert_direct_base_type()
{
   static_assert(DirectBaseType<Array1D<double>>);
   static_assert(DirectBaseType<SliceArray<Array1D<double>>>);
   static_assert(DirectBaseType<ConstSliceArray<Array1D<double>>>);
   static_assert(DirectBaseType<SliceArray<SliceArray<Array1D<double>>>>);
   static_assert(DirectBaseType<ConstSliceArray<SliceArray<Array1D<double>>>>);
   static_assert(DirectBaseType<ConstSliceArray<ConstSliceArray<Array1D<double>>>>);

   Array1D<double> A(1), B(1);
   static_assert(!DirectBaseType<decltype(1. + A)>);
   static_assert(!DirectBaseType<decltype(1. * A)>);
   static_assert(!DirectBaseType<decltype(A + 1.)>);
   static_assert(!DirectBaseType<decltype(A * 1.)>);
   static_assert(!DirectBaseType<decltype(A + B)>);
   static_assert(!DirectBaseType<decltype(A * B)>);

   static_assert(!DirectBaseType<decltype((-A)[seq(0, 0)])>);
   static_assert(!DirectBaseType<decltype((A + B)[seq(0, 0)])>);
   static_assert(!DirectBaseType<decltype((A * B)[seq(0, 0)])>);
   static_assert(!DirectBaseType<decltype(A[seq(0, 0)] * B[seq(0, 0)])>);

   std::cout << "passed " << __func__ << std::endl;
}

int main(int argc, char *argv[])
{
   assert_real();
   assert_int();
   assert_floating();
   assert_standard_floating();
   assert_quad_floating();
   assert_not_quad_floating();

   assert_int_base();
   assert_floating_base();
   assert_standard_floating_base();
   assert_quad_base();
   assert_not_quad_base();

   assert_direct_base_type();
   return EXIT_SUCCESS;
}
