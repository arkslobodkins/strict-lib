// Arkadijs Slobodkins, 2023


#pragma once


#include <cmath>
#include <cstdlib>
#include <iostream>
#include <type_traits>

#include "concepts.hpp"
#include "error.hpp"


#if defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#if __has_include(<mathimf.h>)
#include <mathimf.h>
#endif
#endif


namespace slib {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_REMOVE_NODISCARD
#define STRICT_NODISCARD [[nodiscard]]
#else
#define STRICT_NODISCARD
#endif

#define STRICT_NODISCARD_CONSTEXPR_INLINE STRICT_NODISCARD constexpr inline
#define STRICT_NODISCARD_INLINE STRICT_NODISCARD inline
#define STRICT_NODISCARD_CONSTEXPR STRICT_NODISCARD constexpr
#define STRICT_CONSTEXPR_INLINE constexpr inline
#define STRICT_CONSTEXPR constexpr
#define STRICT_INLINE inline


#if __cplusplus >= 202302L
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2023 STRICT_NODISCARD_CONSTEXPR_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2023 STRICT_NODISCARD_CONSTEXPR
#define STRICT_CONSTEXPR_INLINE_2023 STRICT_CONSTEXPR_INLINE
#define STRICT_CONSTEXPR_2023 STRICT_CONSTEXPR

#else
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2023 STRICT_NODISCARD_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2023 STRICT_NODISCARD
#define STRICT_CONSTEXPR_INLINE_2023 STRICT_INLINE
#define STRICT_CONSTEXPR_2023
#endif


#if __cplusplus > 202302L
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2026 STRICT_NODISCARD_CONSTEXPR_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2026 STRICT_NODISCARD_CONSTEXPR
#define STRICT_CONSTEXPR_INLINE_2026 STRICT_CONSTEXPR_INLINE
#define STRICT_CONSTEXPR_2026 STRICT_CONSTEXPR

#else
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2026 STRICT_NODISCARD_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2026 STRICT_NODISCARD
#define STRICT_CONSTEXPR_INLINE_2026 STRICT_INLINE
#define STRICT_CONSTEXPR_2026
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
struct STRICT_NODISCARD alignas(T) Strict {
public:
   using value_type = T;

   // special member functions
   STRICT_NODISCARD_CONSTEXPR explicit Strict() = default;
   STRICT_NODISCARD_CONSTEXPR Strict(const Strict&) = default;
   STRICT_CONSTEXPR Strict& operator=(const Strict&) & = default;

   // other constructors
   STRICT_NODISCARD_CONSTEXPR_INLINE explicit Strict(T x);
   STRICT_CONSTEXPR Strict(auto x) = delete;

   // conversions
   STRICT_NODISCARD_CONSTEXPR_INLINE T val() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE explicit operator T() const;

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator+() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator-() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator~() const;

   STRICT_CONSTEXPR_INLINE Strict& operator++() &;
   STRICT_CONSTEXPR_INLINE Strict& operator--() &;
   STRICT_CONSTEXPR_INLINE Strict operator++(int) &;
   STRICT_CONSTEXPR_INLINE Strict operator--(int) &;

   STRICT_CONSTEXPR_INLINE Strict& operator+=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator-=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator*=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator/=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator%=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator<<=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator>>=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator&=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator|=(Strict x) &;
   STRICT_CONSTEXPR_INLINE Strict& operator^=(Strict x) &;

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<bool> sb() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<int> si() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long int> sl() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<unsigned int> sui() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<unsigned long int> sul() const;

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<float> sf() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<double> sd() const;
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long double> sld() const;
#ifdef STRICT_QUAD_PRECISION
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<float128> sq() const;
#endif

// quad precision input requires access to x
#ifdef STRICT_QUAD_PRECISION
   template <Quadruple K>
   friend std::istream& operator>>(std::istream& is, Strict<K>& x);
#endif

private:
   T v_{};
};


template <Builtin T>
static STRICT_CONSTEXPR inline auto Zero = Strict<T>{};


template <Builtin T>
static STRICT_CONSTEXPR inline auto One = Strict<T>{T(1)};


// boolean and unsigned integers are not allowed
template <Real T>
   requires(!UnsignedInteger<T>)
static STRICT_CONSTEXPR inline auto NegOne = Strict<T>{T(-1)};


template <Real T>
static STRICT_CONSTEXPR inline auto Thousand = Strict<T>{T(1'000)};


template <Real T>
static STRICT_CONSTEXPR inline auto Million = Strict<T>{T(1'000'000)};


template <Real T>
static STRICT_CONSTEXPR inline auto Billion = Strict<T>{T(1'000'000'000)};


template <>
struct STRICT_NODISCARD alignas(bool) Strict<bool> {
public:
   using value_type = bool;

   // special member functions
   STRICT_NODISCARD_CONSTEXPR explicit Strict() = default;
   STRICT_NODISCARD_CONSTEXPR Strict(const Strict&) = default;
   STRICT_CONSTEXPR Strict& operator=(const Strict&) & = default;

   // other constructors
   STRICT_NODISCARD_CONSTEXPR_INLINE explicit Strict(bool x) : v_{x} {
   }

   STRICT_CONSTEXPR Strict(auto x) = delete;

   // conversions
   STRICT_NODISCARD_CONSTEXPR_INLINE bool val() const {
      return v_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE explicit operator bool() const {
      return v_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator!() const {
      return Strict{!v_};
   }

private:
   bool v_{};
};


static STRICT_CONSTEXPR inline auto true_sb = StrictBool{true};
static STRICT_CONSTEXPR inline auto false_sb = StrictBool{false};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
STRICT_CONSTEXPR inline Strict<int> operator""_si(unsigned long long x) {
   return Strict{int(x)};
}


STRICT_CONSTEXPR inline Strict<long int> operator""_sl(unsigned long long x) {
   return Strict{(long int)(x)};
}


STRICT_CONSTEXPR inline Strict<unsigned int> operator""_sui(unsigned long long x) {
   return Strict{(unsigned int)x};
}


STRICT_CONSTEXPR inline Strict<unsigned long int> operator""_sul(unsigned long long x) {
   return Strict{(unsigned long int)(x)};
}


STRICT_CONSTEXPR inline Strict<float> operator""_sf(long double x) {
   return Strict{float(x)};
}


STRICT_CONSTEXPR inline Strict<double> operator""_sd(long double x) {
   return Strict{double(x)};
}


STRICT_CONSTEXPR inline Strict<long double> operator""_sl(long double x) {
   return Strict{x};
}


// implementation from mp++
#ifdef STRICT_QUAD_PRECISION
template <char... Chars>
inline Strict<float128> operator""_sq() {
   constexpr char str[] = {Chars..., '\0'};

   // Pre-check for binary/octal literals.
   ASSERT_STRICT_ALWAYS_MSG(!(sizeof...(Chars) >= 2u && str[0] == '0'
                              && (str[1] == 'b' || str[1] == 'B' || (str[1] >= '0' && str[1] <= '7'))),
                            "A real128 cannot be constructed from binary or octal literals");

   float128 x = strtoflt128(str, nullptr);
   return Strict{x};
}
#endif


STRICT_CONSTEXPR inline Strict<int> operator""_SI(unsigned long long x) {
   return Strict{int(x)};
}


STRICT_CONSTEXPR inline Strict<long int> operator""_SL(unsigned long long x) {
   return Strict{(long int)(x)};
}


STRICT_CONSTEXPR inline Strict<unsigned int> operator""_SUI(unsigned long long x) {
   return Strict{(unsigned int)x};
}


STRICT_CONSTEXPR inline Strict<unsigned long int> operator""_SUL(unsigned long long x) {
   return Strict{(unsigned long int)(x)};
}


STRICT_CONSTEXPR inline Strict<float> operator""_SF(long double x) {
   return Strict{float(x)};
}


STRICT_CONSTEXPR inline Strict<double> operator""_SD(long double x) {
   return Strict{double(x)};
}


STRICT_CONSTEXPR inline Strict<long double> operator""_SL(long double x) {
   return Strict{x};
}


// implementation from mp++
#ifdef STRICT_QUAD_PRECISION
template <char... Chars>
inline Strict<float128> operator""_SQ() {
   constexpr char str[] = {Chars..., '\0'};

   // Pre-check for binary/octal literals.
   ASSERT_STRICT_ALWAYS_MSG(!(sizeof...(Chars) >= 2u && str[0] == '0'
                              && (str[1] == 'b' || str[1] == 'B' || (str[1] >= '0' && str[1] <= '7'))),
                            "A real128 cannot be constructed from binary or octal literals");

   float128 x = strtoflt128(str, nullptr);
   return Strict{x};
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE std::size_t to_size_t(Strict<T> x);


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> from_size_t(std::size_t x);


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE T builtin_cast(U x);


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE T builtin_cast(Strict<U> x);


template <Real T, Real U>
STRICT_NODISCARD_CONSTEXPR_INLINE T real_cast(U x);


template <Real T, Real U>
STRICT_NODISCARD_CONSTEXPR_INLINE T real_cast(Strict<U> x);


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(U x);


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(Strict<U> x);


template <Floating T, Integer U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> whole(U x);


template <Floating T, Integer U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> whole(Strict<U> x);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator+(Strict<T> x, Strict<T> y);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator-(Strict<T> x, Strict<T> y);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator*(Strict<T> x, Strict<T> y);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator/(Strict<T> x, Strict<T> y);


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator%(Strict<T> x, Strict<T> y);


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator<<(Strict<T> x, Strict<T> y);


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator>>(Strict<T> x, Strict<T> y);


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator&(Strict<T> x, Strict<T> y);


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator|(Strict<T> x, Strict<T> y);


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator^(Strict<T> x, Strict<T> y);


template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator^(Strict<T> x, Strict<T> y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator&&(Strict<T> x, Strict<T> y);


template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator||(Strict<T> x, Strict<T> y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator==(Strict<T> x, Strict<T> y);


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator!=(Strict<T> x, Strict<T> y);


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator<(Strict<T> x, Strict<T> y);


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator<=(Strict<T> x, Strict<T> y);


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator>(Strict<T> x, Strict<T> y);


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator>=(Strict<T> x, Strict<T> y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> signs(Strict<T> x);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> abss(Strict<T> x);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> mins(Strict<T> x, Strict<T> y);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> maxs(Strict<T> x, Strict<T> y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> invs(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> exps(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> logs(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> log2s(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> log10s(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> sqrts(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> cbrts(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> sins(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> coss(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> tans(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> floors(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> ceils(Strict<T> x);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isinfs(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isnans(Strict<T> x);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isfinites(Strict<T> x);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> squares(Strict<T> x);


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> cubes(Strict<T> x);


// optimized for integers
template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> pows(Strict<T> x, Strict<T> p);


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> pows(Strict<T> x, Strict<T> p);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T>::Strict(T x) : v_{x} {
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE T Strict<T>::val() const {
   return v_;
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T>::operator T() const {
   return v_;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> Strict<T>::operator+() const {
   return *this;
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> Strict<T>::operator-() const {
   static_assert(!UnsignedInteger<T>);
   return Strict{T{-v_}};  // -v_ should not change the type of v_ but T is used to be safe
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> Strict<T>::operator~() const {
   static_assert(Integer<T>);
   return Strict<T>{~v_};
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator++() & {
   ++v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator--() & {
   --v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T> Strict<T>::operator++(int) & {
   Strict old{v_};
   ++v_;
   return old;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T> Strict<T>::operator--(int) & {
   Strict old{v_};
   --v_;
   return old;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator+=(Strict<T> x) & {
   v_ += x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator-=(Strict<T> x) & {
   v_ -= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator*=(Strict<T> x) & {
   v_ *= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator/=(Strict<T> x) & {
   if constexpr(Integer<T>) {
      ASSERT_STRICT_DIVISION_DEBUG(x.v_ != 0);
   }

   v_ /= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator%=(Strict<T> x) & {
   static_assert(Integer<T>);
   ASSERT_STRICT_REMAINDER_DEBUG(x.v_ != 0);
   v_ %= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator<<=(Strict<T> x) & {
   static_assert(Integer<T>);
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_SHIFT_FIRST_DEBUG(v_ > -1);
      ASSERT_STRICT_SHIFT_SECOND_DEBUG(x.v_ > -1);
   }
   v_ <<= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator>>=(Strict<T> x) & {
   static_assert(Integer<T>);
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_SHIFT_FIRST_DEBUG(v_ > -1);
      ASSERT_STRICT_SHIFT_SECOND_DEBUG(x.v_ > -1);
   }
   v_ >>= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator&=(Strict<T> x) & {
   static_assert(Integer<T>);
   v_ &= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator|=(Strict<T> x) & {
   static_assert(Integer<T>);
   v_ |= x.v_;
   return *this;
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE Strict<T>& Strict<T>::operator^=(Strict<T> x) & {
   static_assert(Integer<T>);
   v_ ^= x.v_;
   return *this;
}


template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator^(Strict<T> x, Strict<T> y) {
   return strict_cast<bool>(bool{x} ^ bool{y});
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<bool> Strict<T>::sb() const {
   return strict_cast<bool>(v_);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<int> Strict<T>::si() const {
   return strict_cast<int>(v_);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long int> Strict<T>::sl() const {
   return strict_cast<long int>(v_);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<unsigned int> Strict<T>::sui() const {
   return strict_cast<unsigned int>(v_);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<unsigned long int> Strict<T>::sul() const {
   return strict_cast<unsigned long int>(v_);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<float> Strict<T>::sf() const {
   return strict_cast<float>(v_);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<double> Strict<T>::sd() const {
   return strict_cast<double>(v_);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long double> Strict<T>::sld() const {
   return strict_cast<long double>(v_);
}


#ifdef STRICT_QUAD_PRECISION
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<float128> Strict<T>::sq() const {
   return strict_cast<float128>(v_);
}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE std::size_t to_size_t(Strict<T> x) {
   return static_cast<std::size_t>(x.val());
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> from_size_t(std::size_t x) {
   return Strict<T>{static_cast<T>(x)};
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE T builtin_cast(U x) {
   return static_cast<T>(x);
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE T builtin_cast(Strict<U> x) {
   return static_cast<T>(U{x});
}


template <Real T, Real U>
STRICT_NODISCARD_CONSTEXPR_INLINE T real_cast(U x) {
   return static_cast<T>(x);
}


template <Real T, Real U>
STRICT_NODISCARD_CONSTEXPR_INLINE T real_cast(Strict<U> x) {
   return static_cast<T>(U{x});
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(U x) {
   return Strict{builtin_cast<T, U>(x)};
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(Strict<U> x) {
   return Strict{builtin_cast<T, U>(x)};
}


template <Floating T, Integer U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> whole(U x) {
   return strict_cast<T, U>(x);
}


template <Floating T, Integer U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> whole(Strict<U> x) {
   return strict_cast<T, U>(x);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator+(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} + T{y}};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator-(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} - T{y}};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator*(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} * T{y}};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator/(Strict<T> x, Strict<T> y) {
   if constexpr(Integer<T>) {
      ASSERT_STRICT_DIVISION_DEBUG(T{y} != 0);
   }
   return Strict<T>{T{x} / T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator%(Strict<T> x, Strict<T> y) {
   ASSERT_STRICT_REMAINDER_DEBUG(T{y} != 0);
   return Strict<T>{T{x} % T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator<<(Strict<T> x, Strict<T> y) {
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_SHIFT_FIRST_DEBUG(T{x} > -1);
      ASSERT_STRICT_SHIFT_SECOND_DEBUG(T{y} > -1);
   }
   return Strict<T>{T{x} << T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator>>(Strict<T> x, Strict<T> y) {
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_SHIFT_FIRST_DEBUG(T{x} > -1);
      ASSERT_STRICT_SHIFT_SECOND_DEBUG(T{y} > -1);
   }
   return Strict<T>{T{x} >> T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator&(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} & T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator|(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} | T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator^(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} ^ T{y}};
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator&&(Strict<T> x, Strict<T> y) {
   return StrictBool{bool{x} && bool{y}};
}


template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator||(Strict<T> x, Strict<T> y) {
   return StrictBool{bool{x} || bool{y}};
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator==(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} == T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator!=(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} != T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator<(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} < T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator<=(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} <= T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator>(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} > T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator>=(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} >= T{y}};
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> signs(Strict<T> x) {
   return strict_cast<T>((T(0) < T{x}) - (T{x} < T(0)));
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> abss(Strict<T> x) {
   return x > Zero<T> ? x : -x;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> mins(Strict<T> x, Strict<T> y) {
   return x < y ? x : y;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> maxs(Strict<T> x, Strict<T> y) {
   return x > y ? x : y;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> invs(Strict<T> x) {
   return One<T> / x;
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> exps(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::exp(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{expq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> logs(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::log(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{logq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> log2s(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::log2(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{log2q(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> log10s(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::log10(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{log10q(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> sqrts(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::sqrt(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{sqrtq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> cbrts(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::cbrt(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{cbrtq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> sins(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::sin(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{sinq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> coss(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::cos(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{cosq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> tans(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::tan(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{tanq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> floors(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::floor(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{floorq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> ceils(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::ceil(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{ceilq(T{x})};
   }
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isinfs(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return StrictBool{std::isinf(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      // convert to bool first because int is returned
      return StrictBool{bool(isinfq(T{x}))};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isnans(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
      return StrictBool{std::isnan(T{x})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      // convert to bool first because int is returned
      return StrictBool{bool(isnanq(T{x}))};
   }
#endif
}


// All return statements are converted to bool first because int is returned.
// std::isfinite was not giving the correct result when compiled
// with one of the oneAPI versions without -fp-model=precise.
// The version below works even without -fp-model=precise.
template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isfinites(Strict<T> x) {
   if constexpr(StandardFloating<T>) {
// if C++ 23 is not defined, is_constant_evaluated always evaluates to false
#if __cplusplus >= 202302L
      if(std::is_constant_evaluated()) {
         return StrictBool{std::isfinite(T{x})};
      } else {
#endif

#if defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#if __has_include(<mathimf.h>)
         if constexpr(SameAs<T, float>) {
            return StrictBool{bool(finitef(T{x}))};
         } else if constexpr(SameAs<T, double>) {
            return StrictBool{bool(finite(T{x}))};
         } else {
            return StrictBool{bool(finitel(T{x}))};
         }
#endif
#else
      return StrictBool{std::isfinite(T{x})};
#endif
#if __cplusplus >= 202302L
      }
#endif
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return StrictBool{bool(finiteq(T{x}))};
   }
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> squares(Strict<T> x) {
   return x * x;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> cubes(Strict<T> x) {
   return x * x * x;
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> pows(Strict<T> x, Strict<T> p) {
   ASSERT_STRICT_DEBUG(T{p} >= 0);
   T res{1}, pr{T{p}}, xr{T{x}};
   for(;;) {
      if(pr & 1) {
         res *= xr;
      }
      pr >>= 1;
      if(!pr) {
         break;
      }
      xr *= xr;
   }
   return Strict{res};
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> pows(Strict<T> x, Strict<T> p) {
   if constexpr(StandardFloating<T>) {
      return Strict<T>{std::pow(T{x}, T{p})};
   }
#ifdef STRICT_QUAD_PRECISION
   else {
      return Strict<T>{powq(T{x}, T{p})};
   }
#endif
}


}  // namespace slib
