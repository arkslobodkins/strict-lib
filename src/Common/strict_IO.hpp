//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once


#include <cstddef>  // ptrdiff_t
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "auxiliary_types.hpp"
#include "strict_val.hpp"


namespace slib {


template <NotQuadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x);


template <Boolean T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);


template <Integer T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);


template <StandardFloating T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUAD_PRECISION
template <Quadruple T>
std::istream& operator>>(std::istream& is, T& x);


template <Quadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x);


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, T x);


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);
#endif


template <Builtin T>
void print(Strict<T> x, const std::string& name = "");


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {


// scientific, precision, and reset routines are provided so that
// the user can choose the IO format for all floating-point types.
struct StrictFormat {
private:
   static constexpr int float_precision = std::numeric_limits<float>::digits10 + 1;
   static constexpr int double_precision = std::numeric_limits<double>::digits10 + 1;
   static constexpr int long_double_precision = std::numeric_limits<long double>::digits10 + 1;
   static constexpr int quad_precision = 33;

   StrictBool scientific_{true};
   int precision_[4]{float_precision, double_precision, long_double_precision, quad_precision};

public:
   StrictFormat& reset() {
      scientific_ = true_sb;
      precision_[0] = float_precision;
      precision_[1] = double_precision;
      precision_[2] = long_double_precision;
      precision_[3] = quad_precision;
      return *this;
   }

   StrictFormat& scientific(ImplicitBool b) {
      scientific_ = b.get();
      return *this;
   }

   StrictFormat& precision(ImplicitNonNegInt n) {
      precision_[0] = int(n.get().val());
      precision_[1] = int(n.get().val());
      precision_[2] = int(n.get().val());
      precision_[3] = int(n.get().val());
      return *this;
   }


   StrictFormat& precision_float(ImplicitNonNegInt n) {
      precision_[0] = int(n.get().val());
      return *this;
   }

   StrictFormat& precision_double(ImplicitNonNegInt n) {
      precision_[1] = int(n.get().val());
      return *this;
   }

   StrictFormat& precision_long_double(ImplicitNonNegInt n) {
      precision_[2] = int(n.get().val());
      return *this;
   }

#ifdef STRICT_QUAD_PRECISION
   StrictFormat& precision_quad(ImplicitNonNegInt n) {
      precision_[3] = int(n.get().val());
      return *this;
   }
#endif

   // must explicitly qualify namespace
   template <StandardFloating T>
   friend std::ostream& slib::operator<<(std::ostream& os, Strict<T> x);

#ifdef STRICT_QUAD_PRECISION
   template <Quadruple T>
   friend std::ostream& slib::operator<<(std::ostream& os, T x);
#endif
};


}  // namespace internal
static inline internal::StrictFormat format;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <NotQuadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x) {
   T v;
   is >> v;
   ASSERT_STRICT_ALWAYS(is);
   x = Strict{v};
   return is;
}


template <Boolean T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   x ? os << "true" : os << "false";
   return os;
}


template <Integer T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   // sign of unsigned integers is not printed so that
   // one can distinguish signed and unsigned integers
   os << std::showpos << T{x};
   return os;
}


template <StandardFloating T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   os << std::showpos;
   if(format.scientific_) {
      if constexpr(SameAs<T, float>) {
         os << std::scientific << std::showpoint << std::setprecision(format.precision_[0]) << T{x};

      } else if constexpr(SameAs<T, double>) {
         os << std::scientific << std::showpoint << std::setprecision(format.precision_[1]) << T{x};
      } else {
         os << std::scientific << std::showpoint << std::setprecision(format.precision_[2]) << T{x};
      }
   } else {
      if constexpr(SameAs<T, float>) {
         os << std::fixed << std::showpoint << std::setprecision(format.precision_[0]) << T{x};
      } else if constexpr(SameAs<T, double>) {
         os << std::fixed << std::showpoint << std::setprecision(format.precision_[1]) << T{x};
      } else {
         os << std::fixed << std::showpoint << std::setprecision(format.precision_[2]) << T{x};
      }
   }
   return os;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUAD_PRECISION
// implementation from boost/multiprecision/float128.hpp
template <Quadruple T>
std::istream& operator>>(std::istream& is, T& x) {
   std::string str;
   is >> str;
   const char* s = str.c_str();
   char* p_end;
   x = strtoflt128(s, &p_end);
   ASSERT_STRICT_ALWAYS(p_end - s == (std::ptrdiff_t)std::strlen(s));

   return is;
}


template <Quadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x) {
   is >> x.v_;
   return is;
}


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, T x) {
   char buf[128];  // not declared static since it is not thread safe
   if(format.scientific_) {
      std::string frmt = "%+-#*." + std::to_string(format.precision_[3]) + std::string("QE");
      int width = format.precision_[3] + 7;
      quadmath_snprintf(buf, sizeof(buf), frmt.c_str(), width, x);
   } else {
      std::string frmt = "%+-#*." + std::to_string(format.precision_[3]) + std::string("QF");
      int width = format.precision_[3] + 3;
      quadmath_snprintf(buf, sizeof(buf), frmt.c_str(), width, x);
   }
   os << buf;
   return os;
}


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   return os << T{x};
}
#endif


template <Builtin T>
void print(Strict<T> x, const std::string& name) {
   if(!name.empty()) {
      std::cout << name << ":" << '\n';
   }
   std::cout << x << std::endl;
}


}  // namespace slib

