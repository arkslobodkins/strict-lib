#include <cassert>
#include <cmath>
#include <cstdlib>

#include "../strict_val.hpp"

using namespace strict_array;

template<StandardFloatingType T>
void assert_floats()
{
   T x{T{1}}, y{T{2}}, z{};
   StrictVal<T> xs{T{1}}, ys{T{2}}, zs{};

   z = x + y; zs = xs + ys; assert(z == zs);
   z = x - y; zs = xs - ys; assert(z == zs);
   z = x * y; zs = xs * ys; assert(z == zs);
   z = x / y; zs = xs / ys; assert(z == zs);

   ++z; ++zs; assert(z == zs);
   --z; --zs; assert(z == zs);
   z++; zs++; assert(z == zs);
   z--; zs--; assert(z == zs);
   z = +x; zs = +xs; assert(z == zs);
   z = -x; zs = -xs; assert(z == zs);

   z += T{2}; zs += T{2}; assert(z == zs);
   z -= T{2}; zs -= T{2}; assert(z == zs);
   z *= T{2}; zs *= T{2}; assert(z == zs);
   z /= T{2}; zs /= T{2}; assert(z == zs);

   z = std::abs(T{2}); zs = abss(T{2}); assert(z == zs);
   z = std::min(x, y); zs = mins(xs, ys); assert(z == zs);
   z = std::max(x, y); zs = maxs(xs, ys); assert(z == zs);
   z = std::exp(T{2}); zs = exps(T{2}); assert(z == zs);
   z = std::sqrt(T{2}); zs = sqrts(T{2}); assert(z == zs);
   z = std::sin(T{2}); zs = sins(T{2}); assert(z == zs);
   z = std::cos(T{2}); zs = coss(T{2}); assert(z == zs);
}

int main(int argc, char *argv[])
{
   assert_floats<float>();
   assert_floats<double>();
   assert_floats<long double>();
   return EXIT_SUCCESS;
}
