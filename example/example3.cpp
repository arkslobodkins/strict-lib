#include <cstdlib>
#include "../src/strict_array.hpp"

using namespace strict_array;

// Solve first order ODE y' = y, y(t0) = y0, for any t0 and y0
// using trapezoidal method. Exact solution is
// y(t) = y0/exp(t0) exp(t)

Array<float128> ode_solve(Strict128 h, Strict128 t_init, Strict128 t_final, Strict128 y_init)
{
   strict_int nsteps = strict_cast<strict_int>((t_final - t_init) / h);
   Array<float128> y(nsteps+1);
   y[0] = y_init;

   // trapezoidal method
   for(strict_int i = 0; i < nsteps; ++i) {
      y[i+1] = (y[i] + y[i] * h/2.Q) / (1.Q - h/2.Q);
   }

   return y;
}

Array<float128> ode_exact(Strict128 h, Strict128 t_init, Strict128 t_final, Strict128 y_init)
{
   strict_int nsteps = strict_cast<strict_int>((t_final - t_init) / h);
   Array<float128> y(nsteps+1);

   Strict128 c = y_init / exps(t_init);
   for(strict_int i = 0; i < nsteps+1; ++i) {
      Strict128 ti = t_init + strict_cast<float128>(i) * h;
      y[i] = c * exps(ti);
   }

   return y;
}

int main()
{
   auto y = ode_solve(powq(2.Q, -10), 0.Q, 1.Q, 1.Q);
   auto y_exact = ode_exact(powq(2.Q, -10), 0.Q, 1.Q, 1.Q);
   std::cout << max(abs(y - y_exact)) << std::endl;

   return EXIT_SUCCESS;
}
