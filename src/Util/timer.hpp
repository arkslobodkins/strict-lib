//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
//
//  This file is part of the strict-lib library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//


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
