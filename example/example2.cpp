#include <cassert>
#include <cstdlib>
#include "../src/strict_array.hpp"

using namespace strict_array;

// Solve first order ODE y' = y, y(t0) = y0, for any t0 and y0
// using trapezoidal method. Exact solution is
// y(t) = y0/exp(t0) exp(t)

Array<float128> ode_solve(Strict128 h, Strict128 t_init, Strict128 t_final, Strict128 y_init)
{
   assert(h > 0.Q);
   assert(t_final > t_init);

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
   assert(h > 0.Q);
   assert(t_final > t_init);

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
   Strict128 h = pows_int<float128>(2.Q, -24); // floating-point number
   Strict128 t_init = 0.Q;
   Strict128 t_final = 1.Q;
   Strict128 y_init = 1.Q; // y(t_init), t_init is not necessarily 0

   auto y = ode_solve(h, t_init, t_final, y_init);
   auto y_exact = ode_exact(h, t_init, t_final, y_init);
   Strict128 rel_error = norm_inf((y - y_exact)/y_exact);

   std::cout << "maximum relative error: " << rel_error << std::endl;

   return EXIT_SUCCESS;
}
