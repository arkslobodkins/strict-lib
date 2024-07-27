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


#include <tuple>
#include <type_traits>

#include "../Common/concepts.hpp"


namespace slib {


namespace internal {


///////////////////////////////////////////////////////////////////////////////////////
template <typename Tuple, int... I>
constexpr auto FirstNTupleValue(Tuple&& tuple, std::integer_sequence<int, I...>) {
   return std::forward_as_tuple(std::get<I>(tuple)...);
}


template <typename... Args>
constexpr auto ExcludeLastTupleValue(Args&&... args) {
   return FirstNTupleValue(std::forward_as_tuple(args...),
                           std::make_integer_sequence<int, sizeof...(Args) - 1>{});
}


///////////////////////////////////////////////////////////////////////////////////////
template <typename... Args>
struct ExcludeLastTuple {
   using type = std::invoke_result_t<decltype(ExcludeLastTupleValue<Args...>), Args...>;
};


template <typename... Args>
using ExcludeLastTuple_t = typename ExcludeLastTuple<Args...>::type;


template <typename... Args>
consteval bool AllTupleStrict(std::tuple<Args...>) {
   return AllStrict<Args...>;
}


}  // namespace internal


template <typename... Args> concept AllStrictExceptLast
    = internal::AllTupleStrict(internal::ExcludeLastTuple_t<Args...>{});


}  // namespace slib

