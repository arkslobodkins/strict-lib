//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


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

