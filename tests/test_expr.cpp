#include <cstdlib>
#include <cassert>
#include <iostream>

#include "../src/strict_lib.hpp"

using namespace strict;
using namespace strict::util;

template<OneDimFloatingBaseType T1, OneDimFloatingBaseType T2>
void assert_array(const T1 & A, const T2 & B)
{
   assert((long long int)A.size() == B.size());
   for(auto i = 0LL; i < A.size(); ++i) {
      assert(within_tol_rel(A[i], B[i]));
   }
}

template<OneDimIntegerBaseType T1, OneDimIntegerBaseType T2>
void assert_array(const T1 & A, const T2 & B)
{
   assert((long long int)A.size() == B.size());
   for(auto i = 0LL; i < A.size(); ++i) {
      assert(A[i] == B[i]);
   }
}


template<RealType T>
void assert_standard_unit_expr()
{
   auto unit_expr1 = e_unit<T>(Index{0}, Size{5});
   assert_array(unit_expr1, Array1D<T>{T(1), T(0), T(0), T(0), T(0)});
   auto unit_expr1_copy = unit_expr1;
   assert_array(unit_expr1, unit_expr1_copy);

//   unit_expr1[-1];
//   unit_expr1[5];

   auto unit_expr2 = e_unit<T>(Index{3}, Size{5});
   assert_array(unit_expr2, Array1D<T>{T(0), T(0), T(0), T(1), T(0)});

   auto unit_expr3 = e_unit<T>(Index{0}, Size{1});
   assert_array(unit_expr3, Array1D<T>{T(1)});
   assert(unit_expr3[last] == T(1));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}


template<RealType T>
void assert_sequence_expr()
{
   auto seq_expr1 = sequence<T>(Size{3}, Start<T>{T(2)}, Incr<T>{T(1)});
   assert_array(seq_expr1, Array1D<T>{T(2), T(3), T(4)});
   auto seq_expr1_copy = seq_expr1;
   assert_array(seq_expr1, seq_expr1_copy);
   assert(seq_expr1[last] == T(4));
//   seq_expr1[3];
//   seq_expr1[-1];

   auto seq_expr2 = sequence<T>(Size{3}, Start<T>{T(2)}, Incr<T>{T(-1)});
   assert_array(seq_expr2, Array1D<T>{T(2), T(1), T(0)});

   auto seq_expr3 = sequence<T>(Size{0}, Start<T>{T(2)}, Incr<T>{T(-1)});
   assert_array(seq_expr3, Array1D<T>{});
   assert(seq_expr3.empty());

   auto seq_expr4 = sequence<T>(Size{1}, Start<T>{T(2)}, Incr<T>{T(-1)});
   assert_array(seq_expr4, Array1D<T>{T(2)});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}


void assert_range_expr()
{
   auto range_expr = range(5);
   auto seq_expr = sequence<strict_int>(Size{5}, Start<strict_int>{0LL}, Incr<strict_int>{1LL});
//   range_expr[-1];
//   range_expr[5];
   assert_array(range_expr, seq_expr);

   std::cout << "passed " << __func__  << std::endl;
}


void assert_convert_expr()
{
   Array1D<double> x{1.1, 1.2, 1.3, 1.4, 1.5};
   auto xc1 = convert_type<float>(x);
   assert_array(xc1, Array1D<float>{1.1F, 1.2F, 1.3F, 1.4F, 1.5F});
   assert(xc1[last] == 1.5F);
//   xc1[-1];
//   xc1[5];

   auto xc2 = convert_type<int>(x);
   assert_array(xc2, Array1D<int>{1, 1, 1, 1, 1});

   auto xc3 = convert_type<double>(Array1D<int>{});
   assert_array(xc3, Array1D<double>{});
   assert(xc3.empty());

   std::cout << "passed " << __func__  << std::endl;
}


template<RealType T>
void assert_reverse_expr()
{
   Array1D<T> x1{T(0), T(1), T(2), T(3), T(4)};
   auto x_reverse1 = reverse(x1);
   assert_array(x_reverse1, Array1D<T>{T(4), T(3), T(2), T(1), T(0)});
   assert(x_reverse1[last] == T(0));
//   x_reverse1[-1];
//   x_reverse1[5];

   Array1D<T> x2{};
   auto x_reverse2 = reverse(x2);
   assert_array(x_reverse2, Array1D<T>{});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}


template<RealType T>
void assert_const_expr()
{
   auto c1 = const1D(Size{5}, CVal<T>(T(2)));
   assert_array(c1, Array1D<T>{T(2), T(2), T(2), T(2), T(2)});
//   c1[-1];
//   c1[5];

   auto c2 = const1D(Size{0}, CVal<T>(T(2)));
   assert_array(c2, Array1D<T>{});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}


template<RealType T>
void assert_merge_expr()
{
   Array1D<T> A1{};
   Array1D<T> A2{T(1), T(2)};
   Array1D<T> A3{};
   Array1D<T> A4{T(3), T(4), T(5)};
   auto M1 = merge(A1, A2, A3, A4);
   assert_array(M1, Array1D<T>{T(1), T(2), T(3), T(4), T(5)});
//   M1[-1];
//   M1[5];
   assert(M1[last] == T(5));
   assert(M1.size() == 5);

   auto M2 = merge(A1, A3);
   assert_array(M2, Array1D<T>{});
   assert(M2.empty());

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}


int main()
{
   assert_standard_unit_expr<int>();
   assert_standard_unit_expr<double>();

   assert_sequence_expr<int>();
   assert_sequence_expr<double>();

   assert_range_expr();
   assert_convert_expr();

   assert_reverse_expr<int>();
   assert_reverse_expr<double>();

   assert_const_expr<int>();
   assert_const_expr<double>();

   assert_merge_expr<int>();
   assert_merge_expr<double>();

   return EXIT_SUCCESS;
}
