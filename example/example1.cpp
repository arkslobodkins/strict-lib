#include <cassert>
#include <cstdlib>
#include <strict_lib.hpp>


using namespace slib;


template <typename... Args>
constexpr OneDimBaseType auto Add(Args&&... args) {
   assert(same_size(args...));
   return (... + std::forward<Args>(args));
}


template <typename... Args>
constexpr OneDimBaseType auto MergeAbs(Args&&... args) {
   return abs(merge(std::forward<Args>(args)...));
}


// example 1 introduces 1-d array classes and performs some operations on them
int main() {
   Array1D<double> x = random(Size{5}, Low{-1.}, High{1.});
   FixedArray1D<double, 5> y = random(5, -1._sd, 1._sd);

   auto R1 = Add(x, y, x + y);
   auto R2 = MergeAbs(x, y, x + y);

   // Add(x, y, Array1D<double>(x + y)); // won't compile, dangling temporary!!!
   // MergeAbs(x, y, Array1D<double>(x + y)); // won't compile, dangling temporary!!!

   auto s1 = sum(Add(x, y, Array1D<double>(x + y).lval()));       // ok, can be used as lvalue
   auto s2 = sum(MergeAbs(x, y, Array1D<double>(x + y).lval()));  // ok, can be used as lvalue

   y = merge(exclude(y, 0), 100._sd);  // rotate elements to the left and set last element to 100

   // suppress unused variable warnings
   []<typename... Args>([[maybe_unused]] Args&&...) {}(R1, R2, s1, s2);

   return EXIT_SUCCESS;
}
