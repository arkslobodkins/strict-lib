// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/strict_common.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>


// Removed std::format since Intel and clang compilers do not support it(at this time).
#define STRICT_TIME(a)                                                       \
   do {                                                                      \
      spp::Timer t;                                                          \
      a;                                                                     \
      std::ostringstream stream;                                             \
      stream << (#a) << " took: " << std::setprecision(4) << std::scientific \
             << t.wall_time().val() << " seconds\n";                         \
      std::cout << stream.str();                                             \
   } while(0)


namespace spp {


struct Timer {
   explicit Timer() : start_{std::chrono::high_resolution_clock::now()} {
   }

   void restart() {
      start_ = std::chrono::high_resolution_clock::now();
   }

   Strict64 wall_time() const {
      return Strict64{static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                                             std::chrono::high_resolution_clock::now() - start_)
                                             .count())
                      / 1.e9};
   }

private:
   std::chrono::system_clock::time_point start_;
};


} // namespace spp
