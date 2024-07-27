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


#include "strict_val.hpp"


// Copy constructor and assignment are compiler generated
#define STRICT_GENERATE_SMALL_TYPES(SmallObjectName, cnd)                             \
   template <Builtin T>                                                               \
   class STRICT_NODISCARD SmallObjectName {                                           \
   public:                                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName() = default;           \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(T x) : x_{x} {         \
      }                                                                               \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(Strict<T> x) : x_{x} { \
      }                                                                               \
                                                                                      \
      STRICT_CONSTEXPR SmallObjectName(auto x) = delete;                              \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR Strict<T> get() const {                              \
         return x_;                                                                   \
      }                                                                               \
                                                                                      \
   private:                                                                           \
      Strict<T> x_;                                                                   \
   };


// Copy constructor and assignment are compiler generated
#define STRICT_GENERATE_SMALL_INT_TYPES(SmallObjectName, cnd, non_neg)                                       \
   class STRICT_NODISCARD SmallObjectName {                                                                  \
   public:                                                                                                   \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName() : x_{} {                                    \
      }                                                                                                      \
                                                                                                             \
      template <SignedInteger T, bool b = non_neg>                                                           \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(T x) : x_{(long int)x} {                      \
         if constexpr(b) {                                                                                   \
            ASSERT_STRICT_DEBUG(x_ > -1_sl);                                                                 \
         }                                                                                                   \
      }                                                                                                      \
                                                                                                             \
      template <SignedInteger T, bool b = non_neg>                                                           \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(Strict<T> x) : x_{strict_cast<long int>(x)} { \
         if constexpr(b) {                                                                                   \
            ASSERT_STRICT_DEBUG(x_ > -1_sl);                                                                 \
         }                                                                                                   \
      }                                                                                                      \
                                                                                                             \
      STRICT_NODISCARD_CONSTEXPR index_t get() const {                                                       \
         return x_;                                                                                          \
      }                                                                                                      \
                                                                                                             \
   private:                                                                                                  \
      index_t x_;                                                                                            \
   };


// Copy constructor and assignment are compiler generated
#define STRICT_GENERATE_SMALL_BOOL_TYPES(SmallObjectName, cnd)                        \
   class STRICT_NODISCARD SmallObjectName {                                           \
   public:                                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName() : x_{} {             \
      }                                                                               \
                                                                                      \
      template <Boolean T>                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(T x) : x_{x} {         \
      }                                                                               \
                                                                                      \
      template <Boolean T>                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(Strict<T> x) : x_{x} { \
      }                                                                               \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR StrictBool get() const {                             \
         return x_;                                                                   \
      }                                                                               \
                                                                                      \
   private:                                                                           \
      StrictBool x_;                                                                  \
   };


namespace slib {


STRICT_GENERATE_SMALL_TYPES(Low, true)
STRICT_GENERATE_SMALL_TYPES(High, true)
STRICT_GENERATE_SMALL_TYPES(Start, true)
STRICT_GENERATE_SMALL_TYPES(End, true)
STRICT_GENERATE_SMALL_TYPES(Incr, true)
STRICT_GENERATE_SMALL_TYPES(Value, true)


STRICT_GENERATE_SMALL_INT_TYPES(First, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Endmost, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Size, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Count, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Pos, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Stride, true, false)
STRICT_GENERATE_SMALL_INT_TYPES(Index, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Rows, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Cols, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(ImplicitInt, false, false)
STRICT_GENERATE_SMALL_INT_TYPES(ImplicitNonNegInt, false, true)
STRICT_GENERATE_SMALL_BOOL_TYPES(ImplicitBool, false)


// Copy constructor and assignment are compiler generated
class STRICT_NODISCARD ImplicitIntStatic {
public:
   STRICT_NODISCARD_CONSTEXPR ImplicitIntStatic() : x_{} {
   }

   template <SignedInteger T>
   STRICT_NODISCARD_CONSTEXPR ImplicitIntStatic(T x) : x_{(long int)x} {
   }

   template <SignedInteger T>
   STRICT_NODISCARD_CONSTEXPR ImplicitIntStatic(Strict<T> x) : x_{builtin_cast<long int>(x)} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return index_t{x_};
   }

   STRICT_NODISCARD_CONSTEXPR operator ImplicitInt() const {
      return x_;
   }

   /* using index_t type instead of long int would make it */
   /* unusable in templates as it contains non-public members */
   long int x_;
};


namespace internal {

class All {};

class Even {};

class Odd {};

class Reverse {};

class Lval {};

class Last {
public:
   STRICT_NODISCARD_CONSTEXPR explicit Last(ImplicitInt i) : i_{i.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return i_;
   }

private:
   index_t i_;
};

}  // namespace internal


namespace place {
static constexpr inline internal::All all;
static constexpr inline internal::Even even;
static constexpr inline internal::Odd odd;
static constexpr inline internal::Reverse reverse;
static constexpr inline internal::Lval lval;
}  // namespace place
static constexpr inline internal::Last last{0};


// note that plus operator is allowed from both sides bot not minus
STRICT_NODISCARD_CONSTEXPR_INLINE static internal::Last operator+(internal::Last lst, ImplicitInt i) {
   return internal::Last{ImplicitInt{lst.get() - i.get()}};
}


STRICT_NODISCARD_CONSTEXPR_INLINE static internal::Last operator+(ImplicitInt i, internal::Last lst) {
   return internal::Last{ImplicitInt{lst.get() - i.get()}};
}


STRICT_NODISCARD_CONSTEXPR_INLINE static internal::Last operator-(internal::Last lst, ImplicitInt i) {
   return internal::Last{ImplicitInt{lst.get() + i.get()}};
}


STRICT_NODISCARD_CONSTEXPR_INLINE static ImplicitInt operator+(ImplicitInt i, ImplicitInt j) {
   return i.get() + j.get();
}


STRICT_NODISCARD_CONSTEXPR_INLINE static ImplicitInt operator-(ImplicitInt i, ImplicitInt j) {
   return i.get() - j.get();
}


STRICT_NODISCARD_CONSTEXPR_INLINE static ImplicitIntStatic operator+(ImplicitIntStatic i,
                                                                     ImplicitIntStatic j) {
   return i.get() + j.get();
}


STRICT_NODISCARD_CONSTEXPR_INLINE static ImplicitIntStatic operator-(ImplicitIntStatic i,
                                                                     ImplicitIntStatic j) {
   return i.get() - j.get();
}


}  // namespace slib
