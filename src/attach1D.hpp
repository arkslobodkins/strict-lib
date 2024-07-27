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


#include "derived1D.hpp"


namespace slib {


template <Builtin T>
struct STRICT_NODISCARD strict_attach_ptr1D : private CopyBase1D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD strict_attach_ptr1D(T* data, ImplicitInt n)
       : data_{reinterpret_cast<Strict<T>*>(data)},
         n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE value_type& index(ImplicitInt i) {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& index(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return n_;
   }

private:
   Strict<T>* data_;
   index_t n_;
};


template <Builtin T>
struct STRICT_NODISCARD const_strict_attach_ptr1D : private CopyBase1D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD const_strict_attach_ptr1D(const T* data, ImplicitInt n)
       : data_{reinterpret_cast<const Strict<T>*>(data)},
         n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE const value_type& index(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return n_;
   }

private:
   const Strict<T>* data_;
   index_t n_;
};


template <Builtin T>
STRICT_NODISCARD auto attach1D(T* data, ImplicitInt n) {
   auto proxy = strict_attach_ptr1D(data, n);
   return Derived1D<SliceArrayBase1D<decltype(proxy)>>{proxy, seqN{0, n}};
}


template <Builtin T>
STRICT_NODISCARD auto attach1D(const T* data, ImplicitInt n) {
   auto proxy = const_strict_attach_ptr1D(data, n);
   return Derived1D<ConstSliceArrayBase1D<decltype(proxy)>>{proxy, seqN{0, n}};
}


}  // namespace slib

