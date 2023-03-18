// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <iterator>

#include "strict_concepts.hpp"
#include "strict_iterator.hpp"
#include "strict_val.hpp"

#define STRICT_GENERATE_ITERATORS()                                             \
   [[nodiscard]] auto begin() { return iterator{*this, 0}; }                    \
   [[nodiscard]] auto end() { return iterator{*this, size()}; }                 \
   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }        \
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }     \
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }       \
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }    \
                                                                                \
   [[nodiscard]] auto rbegin() { return std::reverse_iterator{end()}; }         \
   [[nodiscard]] auto rend() { return std::reverse_iterator{begin()}; }         \
   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }  \
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }  \
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; } \
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

#define STRICT_GENERATE_CONST_ITERATORS()                                       \
   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }        \
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }     \
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }       \
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }    \
                                                                                \
   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }  \
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }  \
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; } \
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }


#define STRICT_GENERATE_SMALL_TYPES(SmallObjectName)      \
template<RealType T>                                      \
class SmallObjectName                                     \
{                                                         \
public:                                                   \
   explicit SmallObjectName() = default;                  \
   explicit SmallObjectName(T x) : x{x} {}                \
   explicit SmallObjectName(StrictVal<T> x) : x{x} {}     \
   StrictVal<T> get() const { return x; }                 \
private:                                                  \
   StrictVal<T> x;                                        \
};

#define STRICT_GENERATE_SMALL_INT_TYPES(SmallObjectName)  \
class SmallObjectName                                     \
{                                                         \
public:                                                   \
   explicit SmallObjectName() : x{} {}                    \
   explicit SmallObjectName(strict_int x) : x{x} {}       \
   strict_int get() const { return x; }                   \
private:                                                  \
   strict_int x;                                          \
};

namespace strict_array {

STRICT_GENERATE_SMALL_TYPES(Low)
STRICT_GENERATE_SMALL_TYPES(High)
STRICT_GENERATE_SMALL_TYPES(Start)
STRICT_GENERATE_SMALL_TYPES(End)
STRICT_GENERATE_SMALL_TYPES(Incr)

STRICT_GENERATE_SMALL_INT_TYPES(Size)

class Last {};
static constexpr Last last;

}

#endif
