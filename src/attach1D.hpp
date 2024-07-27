//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


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

