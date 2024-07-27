//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once


#include <fstream>   // ifstream, ofstream
#include <iostream>  // cout, ifstream, ostream, flush
#include <string>    // string

#include "Common/common.hpp"
#include "derived1D.hpp"


namespace slib {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
std::istream& operator>>(std::istream& is, Array1D<T>& A);


template <Builtin T>
void read_from_file(const std::string& file_path, Array1D<T>& A);


std::ostream& operator<<(std::ostream& os, BaseType auto const& A);


void print(BaseType auto const& A, const std::string& name);


void print(BaseType auto const&... A);


void printn(BaseType auto const& A, const std::string& name);


void printn(BaseType auto const&... A);


void print_to_file(const std::string& file_path, BaseType auto const& A, const std::string& name = "");


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {

struct ArrayFormat {
private:
   bool detailed_ = false;
   enum Style : int { Row, Column } style_ = Column;

public:
   ArrayFormat& detailed(bool d) {
      detailed_ = d;
      return *this;
   }

   ArrayFormat& row_style() {
      style_ = Row;
      return *this;
   }

   ArrayFormat& col_style() {
      style_ = Column;
      return *this;
   }

   ArrayFormat& reset() {
      detailed_ = false;
      style_ = Column;
      return *this;
   }

   friend std::ostream& OstreamBasePrint(std::ostream& os, OneDimBaseType auto const& A,
                                         const std::string& name);

   friend std::ostream& OstreamBasePrint(std::ostream& os, TwoDimBaseType auto const& A,
                                         const std::string& name);
};

}  // namespace internal
static inline internal::ArrayFormat array_format;


namespace internal {


template <Builtin T>
std::istream& IstreamBaseRead(std::istream& is, Array1D<T>& A) {
   T x{};
   index_t count{};

   Array1D<T> tmp(8);
   while(is >> x) {
      tmp.index(count++) = Strict{x};
      if(tmp.size() == count) {
         tmp.resize(2_sl * count);
      }
   }

   if(is.eof()) {
      is.clear();
   } else {
      ASSERT_STRICT_ALWAYS_MSG(false, "invalid input");
   }

   tmp.resize(count);
   A.swap(tmp);
   return is;
}


std::ostream& OstreamBasePrint(std::ostream& os, OneDimBaseType auto const& A, const std::string& name) {
   if(!name.empty()) {
      os << name << ':' << '\n';
   }

   if(array_format.detailed_) {
      if(A.empty()) {
         os << "[]\n";
      }
   }

   if(array_format.style_ == ArrayFormat::Style::Column) {
      if(!array_format.detailed_) {
         for(index_t i = 0_sl; i < A.size(); ++i) {
            os << A.index(i) << '\n';
         }
      } else {
         for(index_t i = 0_sl; i < A.size(); ++i) {
            os << "[" << strict_cast<unsigned long int>(i) << "] =" << smart_spaces(A.size().val(), i.val())
               << A.index(i) << '\n';
         }
      }
   } else {
      if(!array_format.detailed_) {
         for(index_t i = 0_sl; i < A.size(); ++i) {
            os << A.index(i) << "  ";
         }
      } else {
         for(index_t i = 0_sl; i < A.size(); ++i) {
            os << "[" << strict_cast<unsigned long int>(i) << "] =" << " " << A.index(i) << "  ";
         }
      }
      os << '\n';
   }

   os << std::flush;
   return os;
}


std::ostream& OstreamBasePrint(std::ostream& os, TwoDimBaseType auto const& A, const std::string& name) {
   if(!name.empty()) {
      os << name << ":" << '\n';
   }

   if(array_format.detailed_) {
      if(A.empty()) {
         os << "[]\n";
      }
   }

   if(!array_format.detailed_) {
      for(index_t i = 0_sl; i < A.rows(); ++i) {
         for(index_t j = 0_sl; j < A.cols(); ++j) {
            os << A.index(i, j) << ' ' << ' ';
         }
         os << '\n';
      }
   } else {
      for(index_t i = 0_sl; i < A.rows(); ++i) {
         for(index_t j = 0_sl; j < A.cols(); ++j) {
            os << '[' << strict_cast<unsigned long int>(i) << ", " << strict_cast<unsigned long int>(j)
               << "] =" << smart_spaces(A.rows().val(), i.val()) << A.index(i, j) << ' ' << ' ';
         }
         os << '\n';
      }
   }
   os << std::flush;
   return os;
}


template <BaseType Base1, BaseType... BArgs>
void print_helper(const Base1& A1, const BArgs&... AArgs) {
   if constexpr(sizeof...(AArgs) == 0) {
      internal::OstreamBasePrint(std::cout, A1, "");
      return;
   } else {
      internal::OstreamBasePrint(std::cout, A1, "") << std::endl;
      print_helper(AArgs...);
   }
}


}  // namespace internal


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
std::istream& operator>>(std::istream& is, Array1D<T>& A) {
   return internal::IstreamBaseRead(is, A);
}


template <Builtin T>
void read_from_file(const std::string& file_path, Array1D<T>& A) {
   std::ifstream ifs{file_path};
   ASSERT_STRICT_ALWAYS_MSG(ifs, "invalid file path");
   internal::IstreamBaseRead(ifs, A);
   ifs.close();
}


std::ostream& operator<<(std::ostream& os, BaseType auto const& A) {
   return internal::OstreamBasePrint(os, A, "");
}


void print(BaseType auto const& A, const std::string& name) {
   internal::OstreamBasePrint(std::cout, A, name);
}


void print(BaseType auto const&... A) {
   static_assert(sizeof...(A) > 0);
   internal::print_helper(A...);
}


void printn(BaseType auto const& A, const std::string& name) {
   print(A, name);
   std::cout << std::endl;
}


void printn(BaseType auto const&... A) {
   static_assert(sizeof...(A) > 0);
   (..., printn(A, ""));
}


void print_to_file(const std::string& file_path, BaseType auto const& A, const std::string& name) {
   std::ofstream ofs{file_path};
   ASSERT_STRICT_ALWAYS_MSG(ofs, "invalid file path");
   internal::OstreamBasePrint(ofs, A, name);
   ofs.close();
}


}  // namespace slib
