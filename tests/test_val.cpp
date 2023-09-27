// Arkadijs Slobodkins, 2023

#include <cassert>
#include <cmath>
#include <cstdlib>

#include "../src/strict_lib.hpp"

using namespace strict;

template<IntegerType T>
void assert_ints()
{
   T x(3), y(2), z(1);
   StrictVal<T> xs{T(3)}, ys{T(2)}, zs{T(1)};

   z = -x; zs = -xs; assert(z == zs);
   z = +x; zs = +xs; assert(z == zs);
   ++z; ++zs; assert(z == zs);
   --z; --zs; assert(z == zs);
   z++; zs++; assert(z == zs);
   z--; zs--; assert(z == zs);

   z += x; zs += x; assert(z == zs);
   z -= x; zs -= x; assert(z == zs);
   z *= x; zs *= x; assert(z == zs);
   z /= x; zs /= x; assert(z == zs);
   z <<= x; zs <<= x; assert(z == zs);
   z >>= x; zs >>= x; assert(z == zs);
   z %= x; zs %= x; assert(z == zs);

   z += xs; zs += xs; assert(z == zs);
   z -= xs; zs -= xs; assert(z == zs);
   z *= xs; zs *= xs; assert(z == zs);
   z /= xs; zs /= xs; assert(z == zs);
   z <<= xs; zs <<= xs; assert(z == zs);
   z >>= xs; zs >>= xs; assert(z == zs);
   z %= xs; zs %= xs; assert(z == zs);

   z = x + y; zs = xs + ys; assert(z == zs);
   z = x - y; zs = xs - ys; assert(z == zs);
   z = x * y; zs = xs * ys; assert(z == zs);
   z = x / y; zs = xs / ys; assert(z == zs);
   z = x << y; zs = xs << ys; assert(z == zs);
   z = x >> y; zs = xs >> ys; assert(z == zs);
   z = x % y; zs = xs % ys; assert(z == zs);

   z = x + y; zs = xs + y; assert(z == zs);
   z = x - y; zs = xs - y; assert(z == zs);
   z = x * y; zs = xs * y; assert(z == zs);
   z = x / y; zs = xs / y; assert(z == zs);
   z = x << y; zs = xs << y; assert(z == zs);
   z = x >> y; zs = xs >> y; assert(z == zs);
   z = x % y; zs = xs % y; assert(z == zs);

   z = x + y; zs = x + ys; assert(z == zs);
   z = x - y; zs = x - ys; assert(z == zs);
   z = x * y; zs = x * ys; assert(z == zs);
   z = x / y; zs = x / ys; assert(z == zs);
   z = x << y; zs = x << ys; assert(z == zs);
   z = x >> y; zs = x >> ys; assert(z == zs);
   z = x % y; zs = x % ys; assert(z == zs);

   z = std::abs(x); zs = abss(xs); assert(z == zs);
   z = std::min(x, y); zs = mins(xs, ys); assert(z == zs);
   z = std::max(x, y); zs = maxs(xs, ys); assert(z == zs);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

template<StandardFloatingType T>
void assert_floats()
{
   T x(3), y(2), z(1);
   StrictVal<T> xs{T(3)}, ys{T(2)}, zs{T(1)};

   ++z; ++zs; assert(z == zs);
   --z; --zs; assert(z == zs);
   z++; zs++; assert(z == zs);
   z--; zs--; assert(z == zs);
   z = +x; zs = +xs; assert(z == zs);

   z += x; zs += x; assert(z == zs);
   z -= x; zs -= x; assert(z == zs);
   z *= x; zs *= x; assert(z == zs);
   z /= x; zs /= x; assert(z == zs);

   z += xs; zs += xs; assert(z == zs);
   z -= xs; zs -= xs; assert(z == zs);
   z *= xs; zs *= xs; assert(z == zs);
   z /= xs; zs /= xs; assert(z == zs);

   z = x + y; zs = xs + ys; assert(z == zs);
   z = x - y; zs = xs - ys; assert(z == zs);
   z = x * y; zs = xs * ys; assert(z == zs);
   z = x / y; zs = xs / ys; assert(z == zs);

   z = x + y; zs = xs + y; assert(z == zs);
   z = x - y; zs = xs - y; assert(z == zs);
   z = x * y; zs = xs * y; assert(z == zs);
   z = x / y; zs = xs / y; assert(z == zs);

   z = x + y; zs = x + y; assert(z == zs);
   z = x - y; zs = x - y; assert(z == zs);
   z = x * y; zs = x * y; assert(z == zs);
   z = x / y; zs = x / y; assert(z == zs);

   z = std::abs(x); zs = abss(xs); assert(z == zs);
   z = std::min(x, y); zs = mins(xs, ys); assert(z == zs);
   z = std::max(x, y); zs = maxs(xs, ys); assert(z == zs);

   z = std::exp(x); zs = exps(xs); assert(z == zs);
   z = std::log(x); zs = logs(xs); assert(z == zs);
   z = std::sqrt(x); zs = sqrts(xs); assert(z == zs);
   z = std::sin(x); zs = sins(xs); assert(z == zs);
   z = std::cos(x); zs = coss(xs); assert(z == zs);
   z = std::pow(x, y); zs = pows(xs, ys); assert(z == zs);
   z = std::pow(x, T(5)); zs = pows_int(xs, 5); assert(z == zs);
   z = std::pow(x, T(5)); zs = fast_pows_int(xs, 5); assert(util::within_tol_rel<T>(T{z}, zs));
   z = std::pow(x, T(-5)); zs = fast_pows_int(xs, -5); assert(util::within_tol_rel<T>(T{z}, zs));
   z = std::fma(x, y, T(2)); zs = fmas<T>(xs, ys, T(2)); assert(z == zs);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

#ifdef STRICT_QUADRUPLE_PRECISION
template<QuadType T>
void assert_floats()
{
   T x(3), y(2), z(1);
   StrictVal<T> xs{T(3)}, ys{T(2)}, zs{T(1)};

   ++z; ++zs; assert(z == zs);
   --z; --zs; assert(z == zs);
   z++; zs++; assert(z == zs);
   z--; zs--; assert(z == zs);
   z = +x; zs = +xs; assert(z == zs);
   z = -x; zs = -xs; assert(z == zs);

   z += x; zs += x; assert(z == zs);
   z -= x; zs -= x; assert(z == zs);
   z *= x; zs *= x; assert(z == zs);
   z /= x; zs /= x; assert(z == zs);

   z += xs; zs += xs; assert(z == zs);
   z -= xs; zs -= xs; assert(z == zs);
   z *= xs; zs *= xs; assert(z == zs);
   z /= xs; zs /= xs; assert(z == zs);

   z = x + y; zs = xs + ys; assert(z == zs);
   z = x - y; zs = xs - ys; assert(z == zs);
   z = x * y; zs = xs * ys; assert(z == zs);
   z = x / y; zs = xs / ys; assert(z == zs);

   z = x + y; zs = xs + y; assert(z == zs);
   z = x - y; zs = xs - y; assert(z == zs);
   z = x * y; zs = xs * y; assert(z == zs);
   z = x / y; zs = xs / y; assert(z == zs);

   z = x + y; zs = x + y; assert(z == zs);
   z = x - y; zs = x - y; assert(z == zs);
   z = x * y; zs = x * y; assert(z == zs);
   z = x / y; zs = x / y; assert(z == zs);

   z = fabsq(x); zs = abss(xs); assert(z == zs);
   z = fminq(x, y); zs = mins(xs, ys); assert(z == zs);
   z = fmaxq(x, y); zs = maxs(xs, ys); assert(z == zs);

   z = expq(x); zs = exps(xs); assert(z == zs);
   z = logq(x); zs = logs(xs); assert(z == zs);
   z = sqrtq(x); zs = sqrts(xs); assert(z == zs);
   z = sinq(x); zs = sins(xs); assert(z == zs);
   z = cosq(x); zs = coss(xs); assert(z == zs);
   z = powq(x, y); zs = pows(xs, ys); assert(z == zs);
   z = powq(x, 5); zs = pows_int(xs, 5); assert(z == zs);
   z = powq(x, 5); zs = fast_pows_int(xs, 5); assert(util::within_tol_rel<T>(T{z}, zs));
   z = powq(x, -5); zs = fast_pows_int(xs, -5); assert(util::within_tol_rel<T>(T{z}, zs));
   z = fmaq(x, y, 2); zs = fmas<T>(xs, ys, T(2)); assert(z == zs);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}
#endif

#if defined (__GNUG__) && !defined (__INTEL_COMPILER) && !defined (__clang__) && !defined (__INTEL_LLVM_COMPILER)
template<StandardFloatingType T>
void assert_finite()
{
   StrictVal<T> f = T(5.L);
   StrictVal<T> i = T(5.L/0.L);
   StrictVal<T> n = T(0.L/0.L);

   assert(isfinites<T>(f));
   assert(!isfinites<T>(i));
   assert(!isfinites<T>(n));

   assert(!isinfs<T>(f));
   assert(!isinfs<T>(n));
   assert(isinfs<T>(i));

   assert(!isnans<T>(f));
   assert(!isnans<T>(i));
   assert(isnans<T>(n));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}
#endif

#if defined (__GNUG__) && !defined (__INTEL_COMPILER) && !defined (__clang__) && !defined (__INTEL_LLVM_COMPILER)
template<QuadType T>
void assert_finite()
{
   StrictVal<T> f = T(5.Q);
   StrictVal<T> i = T(5.Q/0.Q);
   StrictVal<T> n = T(0.Q/0.Q);

   assert(isfinites<T>(f));
   assert(!isfinites<T>(i));
   assert(!isfinites<T>(n));

   assert(!isinfs<T>(f));
   assert(!isinfs<T>(n));
   assert(isinfs<T>(i));

   assert(!isnans<T>(f));
   assert(!isnans<T>(i));
   assert(isnans<T>(n));

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}
#endif

template<RealType T>
void assert_compare()
{
   T x(3), y(2), z(1);
   StrictVal<T> xs{T(3)}, ys{T(2)}, zs{T(1)};

   assert(x == xs);
   assert(xs == x);

   assert(x != ys);
   assert(ys != x);

   assert(x > ys);
   assert(ys < x);

   assert(y < xs);
   assert(xs > y);

   assert(x >= ys);
   assert(ys <= x);

   assert(y <= xs);
   assert(xs >= y);

   std::cout << "passed " << __func__ << " for type " << typeid(T).name() << std::endl;
}

int main(int argc, char *argv[])
{
   assert_ints<short int>();
   assert_ints<int>();
   assert_ints<long int>();
   assert_ints<long long int>();

   assert_floats<float>();
   assert_floats<double>();
   assert_floats<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_floats<float128>();
   #endif

   #if defined (__GNUG__) && !defined (__INTEL_COMPILER) && !defined (__clang__) && !defined (__INTEL_LLVM_COMPILER)
   assert_finite<float>();
   assert_finite<double>();
   assert_finite<long double>();
   assert_finite<float128>();
   #endif

   assert_compare<short int>();
   assert_compare<int>();
   assert_compare<long int>();
   assert_compare<long long int>();

   assert_compare<float>();
   assert_compare<double>();
   assert_compare<long double>();
   #ifdef STRICT_QUADRUPLE_PRECISION
   assert_compare<float128>();
   #endif

   return EXIT_SUCCESS;
}
