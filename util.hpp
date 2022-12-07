#pragma once

#if __cplusplus < 201103L
#error requires c++11 or higher
#else

#include <chrono>

namespace util {

struct timer
{
   timer() { start = std::chrono::high_resolution_clock::now(); }
   void restart() { start = std::chrono::high_resolution_clock::now(); }

   double wall_time() {
      return double(std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start).count()) / 1.e9;
   }

private:
   std::chrono::system_clock::time_point start;
};

}

#define TIME(a)                                               \
do {                                                          \
   util::timer t;                                             \
   a;                                                         \
   std::printf("%s took: %.4e seconds\n", #a, t.wall_time()); \
   } while (0)

#endif
