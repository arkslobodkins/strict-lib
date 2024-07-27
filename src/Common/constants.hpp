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


#include <limits>

#if __has_include(<numbers>)
#include <numbers>
#endif

#include "strict_val.hpp"


namespace slib { namespace math {


template <Floating T>
constexpr inline Strict<T> epsilon = Strict{std::numeric_limits<T>::epsilon()};
#ifdef STRICT_QUAD_PRECISION
template <>
constexpr inline Strict<float128> epsilon<float128> = Strict{FLT128_EPSILON};
#endif

constexpr static inline auto epsilon_sf = epsilon<float>;
constexpr static inline auto epsilon_sd = epsilon<double>;
constexpr static inline auto epsilon_sld = epsilon<long double>;
#ifdef STRICT_QUAD_PRECISION
constexpr static inline auto epsilon_sq = epsilon<float128>;
#endif


#if __has_include(<numbers>)
template <Floating T>
constexpr inline Strict<T> pi = Strict{std::numbers::pi_v<T>};
#ifdef STRICT_QUAD_PRECISION
template <>
constexpr inline Strict<float128> pi<float128> = Strict{M_PIq};
#endif
constexpr static inline auto pi_sf = pi<float>;
constexpr static inline auto pi_sd = pi<double>;
constexpr static inline auto pi_sld = pi<long double>;


template <Floating T>
constexpr inline Strict<T> e = Strict{std::numbers::e_v<T>};
#ifdef STRICT_QUAD_PRECISION
template <>
constexpr inline Strict<float128> e<float128> = Strict{M_Eq};
#endif
constexpr static inline auto e_sf = e<float>;
constexpr static inline auto e_sd = e<double>;
constexpr static inline auto e_sld = e<long double>;


template <Floating T>
constexpr inline Strict<T> sqrt2 = Strict{std::numbers::sqrt2_v<T>};
#ifdef STRICT_QUAD_PRECISION
template <>
constexpr inline Strict<float128> sqrt2<float128> = Strict{M_SQRT2q};
#endif
constexpr static inline auto sqrt2_sf = sqrt2<float>;
constexpr static inline auto sqrt2_sd = sqrt2<double>;
constexpr static inline auto sqrt2_sld = sqrt2<long double>;


#ifdef STRICT_QUAD_PRECISION
constexpr static inline auto pi_sq = pi<float128>;
constexpr static inline auto e_sq = e<float128>;
constexpr static inline auto sqrt2_sq = sqrt2<float128>;
#endif
#endif


}}  // namespace slib::math

