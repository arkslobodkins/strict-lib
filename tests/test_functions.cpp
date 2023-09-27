#include <cstdlib>
#include <cassert>

#include "../src/strict_lib.hpp"

using namespace strict;
using namespace strict::util;

template<IntegerType T>
void assert_sum()
{
   auto A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});

   assert(sum(A) == T(15));
   assert(sum(T(2)*A) == T(2*15));
   assert(sum(A[seq(0, 2)]) == T(6));
   assert(sum(A[{0, 1, 3}]) == T(7));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_sum()
{
   auto A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});

   assert( within_tol_rel<T>(sum(A), T(15)) );
   assert( within_tol_rel<T>(sum(T(2) * A), T(2*15)) );
   assert( within_tol_rel<T>(sum(A[seq(0, 2)]), T(6)) );
   assert( within_tol_rel<T>(sum(A[{0, 1, 3}]), T(7)) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_stable_sum()
{
   auto A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});

   assert( within_tol_rel<T>(stable_sum(A), T(15)) );
   assert( within_tol_rel<T>(stable_sum(T(2) * A), T(2*15)) );
   assert( within_tol_rel<T>(stable_sum(A[seq(0, 2)]), T(6)) );
   assert( within_tol_rel<T>(stable_sum(A[{0, 1, 3}]), T(7)) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_mean()
{
   auto A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});

   assert( within_tol_rel<T>(mean(A), T(3)) );
   assert( within_tol_rel<T>(mean(T(2) * A), T(6)) );
   assert( within_tol_rel<T>(mean(A[seq(0, 2)]), T(2)) );
   assert( within_tol_rel<T>(mean(A[{0, 1, 3}]), T(7)/T(3)) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_stable_mean()
{
   auto A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});

   assert( within_tol_rel<T>(stable_mean(A), T(3)) );
   assert( within_tol_rel<T>(stable_mean(T(2) * A), T(6)) );
   assert( within_tol_rel<T>(stable_mean(A[seq(0, 2)]), T(2)) );
   assert( within_tol_rel<T>(stable_mean(A[{0, 1, 3}]), T(7)/T(3)) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_min()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(min(A) == T{1});

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert(min(A) == T{-5});

   A = {T{3}, T{2}, T{1}, T{4}, T{5}};
   assert(min(A) == T{1});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_min()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(min(A), T{1}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(min(A), T{-5}) );

   A = {T{3}, T{2}, T{1}, T{4}, T{5}};
   assert( within_tol_rel<T>(min(A), T{1}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_max()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(max(A) == T{5});

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert(max(A) == T{-1});

   A = {T{3}, T{2}, T{5}, T{4}, T{1}};
   assert(max(A) == T{5});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_max()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(max(A), T{5}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(max(A), T{-1}) );

   A = {T{3}, T{2}, T{5}, T{4}, T{1}};
   assert( within_tol_rel<T>(max(A), T{5}) );
   assert( within_tol_rel<T>(max(A*A), T{25}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_min_index()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(min_index(A).first == 0);
   assert(min_index(A).second == T{1});

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert(min_index(A).first == 4);
   assert(min_index(A).second == T{-5});

   A = {T{3}, T{2}, T{1}, T{4}, T{5}};
   assert(min_index(A).first == 2);
   assert(min_index(A).second == T{1});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_min_index()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(min_index(A).first == 0);
   assert( within_tol_rel<T>(min_index(A).second, T{1}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert(min_index(A).first == 4);
   assert( within_tol_rel<T>(min_index(A).second, T{-5}) );

   A = {T{3}, T{2}, T{1}, T{4}, T{5}};
   assert(min_index(A).first == 2);
   assert( within_tol_rel<T>(min_index(A).second, T{1}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_max_index()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(max_index(A).first == 4);
   assert(max_index(A).second == T{5});

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert(max_index(A).first == 0);
   assert(max_index(A).second == T{-1});

   A = {T{3}, T{2}, T{5}, T{4}, T{1}};
   assert(max_index(A).first == 2);
   assert(max_index(A).second == T{5});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_max_index()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(max_index(A).first == 4);
   assert( within_tol_rel<T>(max_index(A).second, T{5}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert(max_index(A).first == 0);
   assert( within_tol_rel<T>(max_index(A).second, T{-1}) );

   A = {T{3}, T{2}, T{5}, T{4}, T{1}};
   assert(max_index(A).first == 2);
   assert( within_tol_rel<T>(max_index(A).second, T{5}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_finite()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(all_finite(A));
   assert(all_finite(T{2} * A));

   A[1] = A[1]/T{0};
   assert(!all_finite(A));
   A[1] = T{0}/T{0};
   assert(!all_finite(A));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_has_inf()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(!has_inf(A));
   assert(!has_inf(T{2} * A));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_has_nan()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(!has_nan(A));
   assert(!has_nan(T{2} * A));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_norm_inf()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(norm_inf(A), T{5}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(norm_inf(A), T{5}) );

   A = sequence(Size{5}, Start<T>{T(5)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(norm_inf(A), T{5}) );

   A = {T{-3}, T{-2}, T{-5}, T{-4}, T{-1}};
   assert( within_tol_rel<T>(norm_inf(A), T{5}) );
   assert( within_tol_rel<T>(norm_inf(A[seq(3, 4)]), T{4}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_norm2()
{
   Array1D<T> A{T{2}, T{4}, T{5}, T{6}};
   assert( within_tol_rel<T>(norm2(A), T{9}) );

   A = {T{-2}, T{-4}, T{-5}, T{-6}};
   assert( within_tol_rel<T>(norm2(A), T{9}) );

   A = {T{2}, T{3}, T{4}, T{6}};
   assert( within_tol_rel<T>(norm2(A[seq(1, 2)]), T{5}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_stable_norm2()
{
   Array1D<T> A{T{2}, T{4}, T{5}, T{6}};
   assert( within_tol_rel<T>(stable_norm2(A), T{9}) );

   A = {T{-2}, T{-4}, T{-5}, T{-6}};
   assert( within_tol_rel<T>(stable_norm2(A), T{9}) );

   A = {T{2}, T{3}, T{4}, T{6}};
   assert( within_tol_rel<T>(stable_norm2(A[seq(1, 2)]), T{5}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_norm2_scaled()
{
   Array1D<T> A{T{2}, T{4}, T{5}, T{6}};
   assert( within_tol_rel<T>(norm2_scaled(A), T{9}/T{2}) );

   A = {T{-2}, T{-4}, T{-5}, T{-6}};
   assert( within_tol_rel<T>(norm2_scaled(A), T{9}/T{2}) );

   A = {T{2}, T{3}, T{4}, T{6}};
   assert( within_tol_rel<T>(norm2_scaled(A[seq(1, 2)]), T{5}/sqrts<T>(T{2})) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_stable_norm2_scaled()
{
   Array1D<T> A{T{2}, T{4}, T{5}, T{6}};
   assert( within_tol_rel<T>(stable_norm2_scaled(A), T{9}/T{2}) );

   A = {T{-2}, T{-4}, T{-5}, T{-6}};
   assert( within_tol_rel<T>(stable_norm2_scaled(A), T{9}/T{2}) );

   A = {T{2}, T{3}, T{4}, T{6}};
   assert( within_tol_rel<T>(stable_norm2_scaled(A[seq(1, 2)]), T{5}/sqrts<T>(T{2})) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_norm1()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(norm1(A), T{15}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(norm1(A), T{15}) );

   A = {T{3}, T{-2}, T{1}, T{-4}, T{5}};
   assert( within_tol_rel<T>(norm1(A), T{15}) );
   assert( within_tol_rel<T>(norm1(A[seq(2, 3)]), T{5}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_stable_norm1()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(stable_norm1(A), T{15}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(stable_norm1(A), T{15}) );

   A = {T{3}, T{-2}, T{1}, T{-4}, T{5}};
   assert( within_tol_rel<T>(stable_norm1(A), T{15}) );
   assert( within_tol_rel<T>(stable_norm1(A[seq(2, 3)]), T{5}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_norm1_scaled()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(norm1_scaled(A), T{15}/T{5}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(norm1_scaled(A), T{15}/T{5}) );

   A = {T{3}, T{-2}, T{1}, T{-4}, T{5}};
   assert( within_tol_rel<T>(norm1_scaled(A), T{15}/T{5}) );
   assert( within_tol_rel<T>(norm1_scaled(A[seq(2, 3)]), T{5}/T{2}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_stable_norm1_scaled()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(stable_norm1_scaled(A), T{15}/T{5}) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(stable_norm1_scaled(A), T{15}/T{5}) );

   A = {T{3}, T{-2}, T{1}, T{-4}, T{5}};
   assert( within_tol_rel<T>(stable_norm1_scaled(A), T{15}/T{5}) );
   assert( within_tol_rel<T>(stable_norm1_scaled(A[seq(2, 3)]), T{5}/T{2}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_norm_lp()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(norm_lp(A, 3), pows<T>(T{225}, T{1}/T{3})) );

   A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(norm_lp(A, 3), pows<T>(T{225}, T{1}/T{3})) );

   assert( within_tol_rel<T>(norm_lp(A[seq(2, 3)], 3), pows<T>(T(91), T{1}/T{3})) );
   assert( within_tol_rel<T>(norm_lp(A[seq(2, 2)], 3), T{3}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_dot_prod()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   Array1D<T> B = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(dot_prod(A, B) == T{sum(A*A)});

   B = sequence(Size{5}, Start<T>{T(5)}, Incr<T>{T(-1)});
   assert(dot_prod(A, B) == T{35});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_dot_prod()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   Array1D<T> B = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(dot_prod(A, B), stable_sum(A*A)) );

   B = sequence(Size{5}, Start<T>{T(5)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(dot_prod(A, B), T{35}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_stable_dot_prod()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   Array1D<T> B = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( within_tol_rel<T>(stable_dot_prod(A, B), T{pows_int(norm2(A), 2)}) );

   B = sequence(Size{5}, Start<T>{T(5)}, Incr<T>{T(-1)});
   assert( within_tol_rel<T>(stable_dot_prod(A, B), T{35}) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_does_contain_zero()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(!has_zero(A));

   A[2] = T(0);
   assert(has_zero(A));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_does_contain_zero()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(!has_zero(A));

   A[2] = T(0);
   assert(has_zero(A));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<RealType T>
void assert_all_positive()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert(all_positive(A));

   A[2] = T(0);
   assert(!all_positive(A));
   A[2] = T(-1);
   assert(!all_positive(A));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<RealType T>
void assert_all_negative()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(-1)}, Incr<T>{T(-1)});
   assert(all_negative(A));

   A[2] = T(0);
   assert(!all_negative(A));
   A[2] = T(1);
   assert(!all_negative(A));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<RealType T>
void assert_any_satisfy()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( any_satisfy(A, [](auto x) { return x > T{3}; }) );
   assert( any_satisfy(A, [](auto x) { return x < T{3}; }) );
   assert( !any_satisfy(A, [](auto x) { return x < T{0}; }) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<RealType T>
void assert_all_satisfy()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   assert( all_satisfy(A, [](auto x) { return x > T{-1}; }) );
   assert( all_satisfy(A, [](auto x) { return x < T{7}; }) );
   assert( !all_satisfy(A, [](auto x) { return x < T{2}; }) );
   assert( !all_satisfy(A, [](auto x) { return x < T{2}; }) );

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_unique_blas_array()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   auto a_ptr1 = unique_blas_array(A);
   for(strict_int i = 0; i < A.size(); ++i) {
      assert(a_ptr1[i] == A[i]);
   }
   auto a_ptr2 = unique_blas_array(A*A);
   for(strict_int i = 0; i < A.size(); ++i) {
      assert(a_ptr2[i] == A[i]*A[i]);
   }

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_unique_blas_array()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   auto a_ptr1 = unique_blas_array(A);
   for(strict_int i = 0; i < A.size(); ++i) {
      assert(within_tol_rel<T>(a_ptr1[i], A[i]) );
   }
   auto a_ptr2 = unique_blas_array(A*A);
   for(strict_int i = 0; i < A.size(); ++i) {
      assert(within_tol_rel<T>(a_ptr2[i], A[i]*A[i]) );
   }

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_within_range()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1)}, Incr<T>{T(1)});
   auto a_sl1 = within_range(A, Low<T>{T{2}}, High<T>{T(4)});
   auto indexes1 = a_sl1.indexes();
   assert(indexes1[0].get() == 1);
   assert(indexes1[1].get() == 2);
   assert(indexes1[2].get() == 3);

   A = {T{3}, T{2}, T{1}, T{4}, T{5}};
   auto a_sl2 = within_range(A, Low<T>{T{2}}, High<T>{T(4)});
   auto indexes2 = a_sl2.indexes();
   assert(indexes2[0].get() == 0);
   assert(indexes2[1].get() == 1);
   assert(indexes2[2].get() == 3);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_within_range()
{
   Array1D<T> A = sequence(Size{5}, Start<T>{T(1.1)}, Incr<T>{T(1)});
   auto a_sl1 = within_range(A, Low<T>{T{2}}, High<T>{T(4)});
   auto indexes1 = a_sl1.indexes();
   assert(indexes1[0].get() == 1);
   assert(indexes1[1].get() == 2);

   A = {T{3}, T{2}, T{1}, T{4}, T{5}};
   auto a_sl2 = within_range(A, Low<T>{T{2}}, High<T>{T(4)});
   auto indexes2 = a_sl2.indexes();
   assert(indexes2[0].get() == 0);
   assert(indexes2[1].get() == 1);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<RealType T>
void assert_within_cond()
{
   Array1D<T> A{T{-5}, T{-3}, T{-1}, T{1}, T{3}, T{5}};
   auto a_sl1 = within_cond(A, [](auto x) { return x > T{2} || x < T{-2}; });
   auto indexes1 = a_sl1.indexes();
   assert(indexes1[0].get() == 0);
   assert(indexes1[1].get() == 1);
   assert(indexes1[2].get() == 4);
   assert(indexes1[3].get() == 5);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_apply()
{
   Array1D<T> A{T{-5}, T{-3}, T{-1}, T{1}, T{3}, T{5}};
   apply(A, [](auto & x) { if(x > T{2}) x *= T{2}; });
   assert(A[0] == T{-5});
   assert(A[1] == T{-3});
   assert(A[2] == T{-1});
   assert(A[3] == T{1});
   assert(A[4] == T{6});
   assert(A[5] == T{10});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_apply()
{
   Array1D<T> A{T{-5}, T{-3}, T{-1}, T{1}, T{3}, T{5}};
   apply(A, [](auto & x) { if(x > T{2}) x *= T{2}; });
   assert(within_tol_rel<T>(A[0], T{-5}));
   assert(within_tol_rel<T>(A[1], T{-3}));
   assert(within_tol_rel<T>(A[2], T{-1}));
   assert(within_tol_rel<T>(A[3], T{1}));
   assert(within_tol_rel<T>(A[4], T{6}));
   assert(within_tol_rel<T>(A[5], T{10}));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<IntegerType T>
void assert_sort()
{
   Array1D<T> A{T{-5}, T{-3}, T{-1}, T{1}, T{3}, T{5}};

   sort_decreasing(A);
   assert(A[0] == T{5});
   assert(A[1] == T{3});
   assert(A[2] == T{1});
   assert(A[3] == T{-1});
   assert(A[4] == T{-3});
   assert(A[5] == T{-5});

   sort_increasing(A);
   assert(A[0] == T{-5});
   assert(A[1] == T{-3});
   assert(A[2] == T{-1});
   assert(A[3] == T{1});
   assert(A[4] == T{3});
   assert(A[5] == T{5});

   sort_decreasing(A[seq(0, 2)]);
   assert(A[0] == T{-1});
   assert(A[1] == T{-3});
   assert(A[2] == T{-5});
   assert(A[3] == T{1});
   assert(A[4] == T{3});
   assert(A[5] == T{5});

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<FloatingType T>
void assert_sort()
{
   Array1D<T> A{T{-5}, T{-3}, T{-1}, T{1}, T{3}, T{5}};

   sort_decreasing(A);
   assert(within_tol_rel<T>(A[0], T{5}));
   assert(within_tol_rel<T>(A[1], T{3}));
   assert(within_tol_rel<T>(A[2], T{1}));
   assert(within_tol_rel<T>(A[3], T{-1}));
   assert(within_tol_rel<T>(A[4], T{-3}));
   assert(within_tol_rel<T>(A[5], T{-5}));

   sort_increasing(A);
   assert(within_tol_rel<T>(A[0], T{-5}));
   assert(within_tol_rel<T>(A[1], T{-3}));
   assert(within_tol_rel<T>(A[2], T{-1}));
   assert(within_tol_rel<T>(A[3], T{1}));
   assert(within_tol_rel<T>(A[4], T{3}));
   assert(within_tol_rel<T>(A[5], T{5}));

   sort_decreasing(A[seq(0, 2)]);
   assert(within_tol_rel<T>(A[0], T{-1}));
   assert(within_tol_rel<T>(A[1], T{-3}));
   assert(within_tol_rel<T>(A[2], T{-5}));
   assert(within_tol_rel<T>(A[3], T{1}));
   assert(within_tol_rel<T>(A[4], T{3}));
   assert(within_tol_rel<T>(A[5], T{5}));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

int main()
{
   assert_sum<short int>();
   assert_sum<int>();
   assert_sum<long int>();
   assert_sum<long long int>();
   assert_sum<float>();
   assert_sum<double>();
   assert_sum<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_sum<float128>();
   #endif

   assert_stable_sum<float>();
   assert_stable_sum<double>();
   assert_stable_sum<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_stable_sum<float128>();
   #endif

   assert_mean<float>();
   assert_mean<double>();
   assert_mean<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_mean<float128>();
   #endif

   assert_stable_mean<float>();
   assert_stable_mean<double>();
   assert_stable_mean<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_stable_mean<float128>();
   #endif

   assert_min<short int>();
   assert_min<int>();
   assert_min<long int>();
   assert_min<long long int>();
   assert_min<float>();
   assert_min<double>();
   assert_min<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_min<float128>();
   #endif

   assert_max<short int>();
   assert_max<int>();
   assert_max<long int>();
   assert_max<long long int>();
   assert_max<float>();
   assert_max<double>();
   assert_max<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_max<float128>();
   #endif

   assert_min_index<short int>();
   assert_min_index<int>();
   assert_min_index<long int>();
   assert_min_index<long long int>();
   assert_min_index<float>();
   assert_min_index<double>();
   assert_min_index<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_min_index<float128>();
   #endif

   assert_max_index<short int>();
   assert_max_index<int>();
   assert_max_index<long int>();
   assert_max_index<long long int>();
   assert_max_index<float>();
   assert_max_index<double>();
   assert_max_index<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_max_index<float128>();
   #endif

   assert_finite<float>();
   assert_finite<double>();
   assert_finite<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_finite<float128>();
   #endif

   assert_has_inf<float>();
   assert_has_inf<double>();
   assert_has_inf<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_has_inf<float128>();
   #endif

   assert_has_nan<float>();
   assert_has_nan<double>();
   assert_has_nan<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_has_nan<float128>();
   #endif

   assert_norm_inf<float>();
   assert_norm_inf<double>();
   assert_norm_inf<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_norm_inf<float128>();
   #endif

   assert_norm2<float>();
   assert_norm2<double>();
   assert_norm2<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_norm2<float128>();
   #endif

   assert_stable_norm2<float>();
   assert_stable_norm2<double>();
   assert_stable_norm2<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_stable_norm2<float128>();
   #endif

   assert_norm2_scaled<float>();
   assert_norm2_scaled<double>();
   assert_norm2_scaled<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_norm2_scaled<float128>();
   #endif

   assert_stable_norm2_scaled<float>();
   assert_stable_norm2_scaled<double>();
   assert_stable_norm2_scaled<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_stable_norm2_scaled<float128>();
   #endif

   assert_norm1<float>();
   assert_norm1<double>();
   assert_norm1<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_norm1<float128>();
   #endif

   assert_stable_norm1<float>();
   assert_stable_norm1<double>();
   assert_stable_norm1<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_stable_norm1<float128>();
   #endif

   assert_norm1_scaled<float>();
   assert_norm1_scaled<double>();
   assert_norm1_scaled<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_norm1_scaled<float128>();
   #endif

   assert_stable_norm1_scaled<float>();
   assert_stable_norm1_scaled<double>();
   assert_stable_norm1_scaled<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_stable_norm1_scaled<float128>();
   #endif

   assert_norm_lp<float>();
   assert_norm_lp<double>();
   assert_norm_lp<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_norm_lp<float128>();
   #endif

   assert_dot_prod<short int>();
   assert_dot_prod<int>();
   assert_dot_prod<long int>();
   assert_dot_prod<long long int>();
   assert_dot_prod<float>();
   assert_dot_prod<double>();
   assert_dot_prod<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_dot_prod<float128>();
   #endif

   assert_stable_dot_prod<float>();
   assert_stable_dot_prod<double>();
   assert_stable_dot_prod<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_stable_dot_prod<float128>();
   #endif

   assert_does_contain_zero<short int>();
   assert_does_contain_zero<int>();
   assert_does_contain_zero<long int>();
   assert_does_contain_zero<long long int>();
   assert_does_contain_zero<float>();
   assert_does_contain_zero<double>();
   assert_does_contain_zero<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_does_contain_zero<float128>();
   #endif

   assert_all_positive<short int>();
   assert_all_positive<int>();
   assert_all_positive<long int>();
   assert_all_positive<long long int>();
   assert_all_positive<float>();
   assert_all_positive<double>();
   assert_all_positive<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_all_positive<float128>();
   #endif

   assert_all_negative<short int>();
   assert_all_negative<int>();
   assert_all_negative<long int>();
   assert_all_negative<long long int>();
   assert_all_negative<float>();
   assert_all_negative<double>();
   assert_all_negative<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_all_negative<float128>();
   #endif

   assert_any_satisfy<short int>();
   assert_any_satisfy<int>();
   assert_any_satisfy<long int>();
   assert_any_satisfy<long long int>();
   assert_any_satisfy<float>();
   assert_any_satisfy<double>();
   assert_any_satisfy<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_any_satisfy<float128>();
   #endif

   assert_all_satisfy<short int>();
   assert_all_satisfy<int>();
   assert_all_satisfy<long int>();
   assert_all_satisfy<long long int>();
   assert_all_satisfy<float>();
   assert_all_satisfy<double>();
   assert_all_satisfy<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_all_satisfy<float128>();
   #endif

   assert_unique_blas_array<short int>();
   assert_unique_blas_array<int>();
   assert_unique_blas_array<long int>();
   assert_unique_blas_array<long long int>();
   assert_unique_blas_array<float>();
   assert_unique_blas_array<double>();
   assert_unique_blas_array<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_unique_blas_array<float128>();
   #endif

   assert_within_range<short int>();
   assert_within_range<int>();
   assert_within_range<long int>();
   assert_within_range<long long int>();
   assert_within_range<float>();
   assert_within_range<double>();
   assert_within_range<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_within_range<float128>();
   #endif

   assert_within_cond<short int>();
   assert_within_cond<int>();
   assert_within_cond<long int>();
   assert_within_cond<long long int>();
   assert_within_cond<float>();
   assert_within_cond<double>();
   assert_within_cond<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_within_cond<float128>();
   #endif

   assert_apply<short int>();
   assert_apply<int>();
   assert_apply<long int>();
   assert_apply<long long int>();
   assert_apply<float>();
   assert_apply<double>();
   assert_apply<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_apply<float128>();
   #endif

   assert_sort<short int>();
   assert_sort<int>();
   assert_sort<long int>();
   assert_sort<long long int>();
   assert_sort<float>();
   assert_sort<double>();
   assert_sort<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_sort<float128>();
   #endif

   return EXIT_SUCCESS;
}
