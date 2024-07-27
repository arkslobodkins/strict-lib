// Arkadijs Slobodkins, 2023


#pragma once


#include <chrono>
#include <iomanip>   // setprecision
#include <iostream>  // cout, scientific(from ios)

#include "../Common/common.hpp"


// removed std::format since Intel and clang compilers do not support it(at this time)
#define STRICT_TIME(a)                                                                                 \
   do {                                                                                                \
      slib::util::timer t;                                                                             \
      a;                                                                                               \
      std::cout << (#a) << " took: " << std::setprecision(4) << std::scientific << t.wall_time().val() \
                << " seconds\n";                                                                       \
   } while(0)


namespace slib::util {


struct timer {
   explicit timer() : start{std::chrono::high_resolution_clock::now()} {
   }

   void restart() {
      start = std::chrono::high_resolution_clock::now();
   }

   Strict64 wall_time() {
      return Strict64{double(std::chrono::duration_cast<std::chrono::nanoseconds>(
                                 std::chrono::high_resolution_clock::now() - start)
                                 .count())
                      / 1.e9};
   }

private:
   std::chrono::system_clock::time_point start;
};


}  // namespace slib::util
