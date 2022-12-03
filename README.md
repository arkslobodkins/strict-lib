Your math professor is strict and so am I.  
  
Facilities of generic programming allow us to write code with extensive amount of flexibility.  
On the other hand, the exact same facilities allow us to specify how flexible we want to be.  
This could not be more true with concepts of C++20. They are a convenient way to control the generality  
of templates.  
  
Certain fields require stronger type safety and more rigorous error handling than others.  
That is certainly true when it comes to high-performance computing and  
numerics. We want to know exactly what types we are dealing with, how many bytes  
of memory they occupy, and how does that type handle different types of errors.  
  
strict-array is a class that addresses two  
***unconflicting goals: efficiency and type safety***.  
The former is not particularly difficult: performing operations  
by traversing an array linearly is all one can really do for standard  
operations. The only challenge is to write a user-friendly interface, which  
requires expression templates to avoid temporaries. Vectorization and loop  
unrolling is successfully performed by compiler.  
  
Type safety, on the other hand, is more challenging, and can have somewhat different  
meanings depending on the context. Since strict-array is primarily designed for numerical computation,  
it employs the strictest type safety strategies available.  
  
For starters, it offers functionality for the following types:  
short, int, long int, long long int, float, double, long double, and  
float128(GCC only). In addition, it uses long int for indexing rather  
than unsigned long, which allows catching errors associated with negative  
indexing.  
  
Second, all operations must strictly respect the type of an object.  
For example, suppose I have  
  
Array<float>(some_size) x;  
x += 1; //  won't compile, 1 is not of the same type as elements of x  
  
This is a deliberate design. Instead,  
x += 1.F; //  compiles correctly  
  
But why be so strict? Well, here is one reason:  
  
double global_minimum(float parameter) {  
   Array<double> x(some_size);  
   ...  
   x *= parameter;  // smells like a bug  
   ...  
   return x.min();  
}  
  
Our array would not compiler here, since parameter and elements of x are different types.  
Are we really sure that a parameter has to be float, whereas the rest of the function  
consists of doubles? Odds are pretty high that it should be double. We have just  
prevented the loss of precision.

For the second example, consider indexing:
Array<float> x(some_large_size);
for(int i = 0; i < x.size(); ++i)
   x[i] = ...;

Here the size of x is a large value that exceeds maximum range of int.
Eventually i would exceed its range and cause undefined behaviour.
Therefore, indexing x[i] requires i to be of type long int. Thus,
the previous code also results in compilation error and prevents
from the runtime disaster.

for(long int i = 0; i < x.size(); ++i)
   x[i] = ...;  // compiles and works as expected



