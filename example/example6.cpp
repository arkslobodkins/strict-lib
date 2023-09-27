#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include "../src/strict_lib.hpp"

using namespace strict;

// Solve first order ODE y' = y, y(t0) = y0, for any t0 and y0
// using trapezoidal method. Exact solution is
// y(t) = y0/exp(t0) exp(t)


Array1D<float128> ode_solve(Strict128 h, Strict128 t_init, Strict128 t_final, Strict128 y_init)
{
   assert(h > 0.Q);
   assert(t_final > t_init);

   strict_int nsteps = strict_cast<strict_int>((t_final - t_init) / h);
   Array1D<float128> y(nsteps+1);
   y[0] = y_init;

   // trapezoidal method
   for(strict_int i = 0; i < nsteps; ++i) {
      y[i+1] = (y[i] + y[i] * h/2.Q) / (1.Q - h/2.Q);
   }

   return y;
}


Array1D<float128> ode_exact(Strict128 h, Strict128 t_init, Strict128 t_final, Strict128 y_init)
{
   assert(h > 0.Q);
   assert(t_final > t_init);

   Strict128 c = y_init / exps(t_init);
   strict_int nsteps = strict_cast<strict_int>((t_final - t_init) / h);
   Array1D<float128> y(nsteps+1);

   for(strict_int i = 0; i < nsteps+1; ++i) {
      auto t_cur = t_init + float128(i) * h;
      y[i] = c * exps(t_cur);
   }
   return y;
}


int main()
{
   Strict128 h = pows_int<float128>(2.Q, -24); // floating-point number
   constexpr Strict128 t_init = 0.Q;
   constexpr Strict128 t_final = 1.Q;
   constexpr Strict128 y_init = 1.Q; // y(t_init), t_init is not necessarily 0

   auto y = ode_solve(h, t_init, t_final, y_init);
   auto y_exact = ode_exact(h, t_init, t_final, y_init);
   if(auto rel_error = util::max_rel_error(y, y_exact))
      std::cout << "maximum relative error: " << *rel_error << std::endl;
   else {
      throw std::runtime_error{"relative error could not be computed"};
   }

   return EXIT_SUCCESS;
}

