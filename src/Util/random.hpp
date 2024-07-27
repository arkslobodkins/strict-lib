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


#include <random>

#include "../Common/common.hpp"
#include "../Expr/array_expr1D.hpp"


namespace slib {


namespace internal {


template <typename T>
struct Generator;


template <Integer T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h) : gen{std::random_device{}()}, dst{l.val(), h.val()} {
   }

   auto random() const {
      return Strict{dst(gen)};
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_int_distribution<T> dst;
};


template <Boolean T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h) : gen{std::random_device{}()}, dst{l.val(), h.val()} {
   }

   auto random() const {
      return dst(gen) % 2 == 0 ? false_sb : true_sb;
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_int_distribution<int> dst;
};


template <StandardFloating T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h) : gen{std::random_device{}()}, dst{l.val(), h.val()} {
   }

   auto random() const {
      return Strict{dst(gen)};
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_real_distribution<T> dst;
};


// generates quadruple precision numbers that are double precision numbers
#ifdef STRICT_QUAD_PRECISION
template <Quadruple T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h) : gen{std::random_device{}()}, dst{l.sd().val(), h.sd().val()} {
   }

   auto random() const {
      return Strict{dst(gen)}.sq();
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_real_distribution<double> dst;
};
#endif


template <Builtin T>
Strict<T> rands(Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   Generator<T> g{low, high};
   return g.random();
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>>
void random(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   Generator<BuiltinTypeOf<Base>> g{low, high};
   for(auto& x : A) {
      x = g.random();
   }
}


template <Builtin T>
auto random(ImplicitInt n, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   Generator<T> g{low, high};
   auto G = [g]([[maybe_unused]] auto x) { return g.random(); };
   return generate1D<decltype(const1D<T>(n, Zero<T>)), decltype(G), true>(const1D<T>(n, Zero<T>), G);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
Strict<T> rands_not0(Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<T> && high == Zero<T>));

   Generator<T> g{low, high};
   while(true) {
      if(auto r = g.random(); r != Zero<T>) {
         return r;
      }
   }
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
void random_not0(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<RealTypeOf<Base>> && high == Zero<RealTypeOf<Base>>));

   Generator<RealTypeOf<Base>> g{low, high};
   for(auto& x : A) {
      while(true) {
         if(auto r = g.random(); r != Zero<RealTypeOf<Base>>) {
            x = r;
            break;
         }
      }
   }
}


template <Real T>
auto random_not0(ImplicitInt n, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<T> && high == Zero<T>));

   Generator<T> g{low, high};
   auto G = [g]([[maybe_unused]] auto x) {
      while(true) {
         if(auto r = g.random(); r != Zero<T>) {
            return r;
         }
      }
   };
   return generate1D<decltype(const1D<T>(n, Zero<T>)), decltype(G), true>(const1D<T>(n, Zero<T>), G);
}


}  // namespace internal


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
Strict<T> rands(Strict<T> low, Strict<T> high) {
   return internal::rands(low, high);
}


template <Real T>
Strict<T> rands(Low<T> low, High<T> high) {
   return internal::rands(low.get(), high.get());
}


template <Builtin T>
Strict<T> rands() {
   return internal::rands(Zero<T>, One<T>);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
Strict<T> rands_not0(Strict<T> low, Strict<T> high) {
   return internal::rands_not0(low, high);
}


template <Real T>
Strict<T> rands_not0(Low<T> low, High<T> high) {
   return internal::rands_not0(low.get(), high.get());
}


template <Real T>
Strict<T> rands_not0() {
   return internal::rands_not0(Zero<T>, One<T>);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
void random(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   internal::random(A, low, high);
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
void random(Base&& A, Low<BuiltinTypeOf<Base>> low, High<BuiltinTypeOf<Base>> high) {
   internal::random(A, low.get(), high.get());
}


template <typename... Base>
   requires AllNonConstBases<RemoveRef<Base>...>
void random(Base&&... A) {
   using T = RealTypeOf<LastPack_t<Base...>>;
   (..., internal::random(A, Zero<T>, One<T>));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
void random_not0(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   internal::random_not0(A, low, high);
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
void random_not0(Base&& A, Low<BuiltinTypeOf<Base>> low, High<BuiltinTypeOf<Base>> high) {
   internal::random_not0(A, low.get(), high.get());
}


template <typename... Base>
   requires AllNonConstBases<RemoveRef<Base>...> && Real<BuiltinTypeOf<LastPack_t<Base...>>>
void random_not0(Base&&... A) {
   using T = RealTypeOf<LastPack_t<Base...>>;
   (..., internal::random_not0(A, Zero<T>, One<T>));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
auto random(ImplicitInt n, Strict<T> low, Strict<T> high) {
   return internal::random<T>(n, low, high);
}


template <Real T>
auto random(Size n, Low<T> low, High<T> high) {
   return random<T>(n.get(), low.get(), high.get());
}


template <Builtin T>
auto random(ImplicitInt n) {
   return internal::random<T>(n, Zero<T>, One<T>);
}


template <Builtin T>
auto random(Size n) {
   return random<T>(n.get());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
auto random_not0(ImplicitInt n, Strict<T> low, Strict<T> high) {
   return internal::random_not0<T>(n, low, high);
}


template <Real T>
auto random_not0(Size n, Low<T> low, High<T> high) {
   return random_not0<T>(n.get(), low.get(), high.get());
}


template <Real T>
auto random_not0(ImplicitInt n) {
   return internal::random_not0<T>(n, Zero<T>, One<T>);
}


template <Real T>
auto random_not0(Size n) {
   return random_not0<T>(n.get());
}


}  // namespace slib
