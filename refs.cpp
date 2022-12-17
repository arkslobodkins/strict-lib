#include <cstdlib>
#include <iostream>
#include <type_traits>

template<typename T>
void deduce(T s);

struct F
{

   void f() { deduce(*this); }
};

template<typename T>
void deduce(T s)
{
   std::cout << std::is_same_v<T, F> << std::endl;
}


int main(int argc, char *argv[])
{
   F f;
   f.f();


   return EXIT_SUCCESS;
}
