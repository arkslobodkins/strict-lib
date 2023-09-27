// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <initializer_list> // initializer_list
#include <iterator>         // reverse_iterator
#include <type_traits>      // remove_reference_t
#include <utility>          // move
#include <vector>           // vector

#include "strict_concepts.hpp"
#include "strict_iterator.hpp"
#include "strict_val.hpp"


// 1. All
// 2. seq
// 3. seqN
// 4. Even
// 5. Odd
// 6. skipN
// 7. vector<ImplicitInt>
// 8. initializer_list<ImplicitInt>

#define STRICT_GENERATE_SLICES1D()                                                                  \
   [[nodiscard]] auto operator[](internal::All) {                                                   \
      return SliceArray<std::remove_reference_t<decltype(*this)>>                                   \
         {*this, slice(0, size())};                                                                 \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](seq s) {                                                           \
      ASSERT_STRICT_DEBUG(!empty());                                                                \
      ASSERT_STRICT_DEBUG(s.valid(*this));                                                          \
      return SliceArray<std::remove_reference_t<decltype(*this)>>                                   \
         {*this, s.to_slice()};                                                                     \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](seqN s) {                                                          \
      ASSERT_STRICT_DEBUG(s.valid(*this));                                                          \
      return SliceArray<std::remove_reference_t<decltype(*this)>>                                   \
         {*this, s};                                                                                \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](internal::Even) {                                                  \
      return operator[](slice(0, (size()+1)/2, 2));                                                 \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](internal::Odd) {                                                   \
      return operator[](slice(1, size()/2, 2));                                                     \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](skipN s) {                                                         \
      return operator[](slice(s.start(), (size()+s.stride()-1-s.start())/s.stride(), s.stride()));  \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](std::vector<ImplicitInt> indexes) {                                \
      return RandSliceArray<std::remove_reference_t<decltype(*this)>>                               \
         {*this, std::move(indexes)};                                                               \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](std::initializer_list<ImplicitInt> indexes) {                      \
      return RandSliceArray<std::remove_reference_t<decltype(*this)>>                               \
         {*this, indexes};                                                                          \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](internal::All) const {                                             \
      return ConstSliceArray<std::remove_reference_t<decltype(*this)>>                              \
         {*this, slice(0, size())};                                                                 \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](seq s) const {                                                     \
      ASSERT_STRICT_DEBUG(!empty());                                                                \
      ASSERT_STRICT_DEBUG(s.valid(*this));                                                          \
      return ConstSliceArray<std::remove_reference_t<decltype(*this)>>                              \
         {*this, s.to_slice()};                                                                     \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](seqN s) const {                                                    \
      ASSERT_STRICT_DEBUG(s.valid(*this));                                                          \
      return ConstSliceArray<std::remove_reference_t<decltype(*this)>>                              \
         {*this, s};                                                                                \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](internal::Even) const {                                            \
      return operator[](slice(0, (size()+1)/2, 2));                                                 \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](internal::Odd) const {                                             \
      return operator[](slice(1, size()/2, 2));                                                     \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](skipN s) const {                                                   \
      return operator[](slice(s.start(), (size()+s.stride()-1-s.start())/s.stride(), s.stride()));  \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](std::vector<ImplicitInt> indexes) const {                          \
      return RandConstSliceArray<std::remove_reference_t<decltype(*this)>>                          \
         {*this, std::move(indexes)};                                                               \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](std::initializer_list<ImplicitInt> indexes) const {                \
      return RandConstSliceArray<std::remove_reference_t<decltype(*this)>>                          \
         {*this, indexes};                                                                          \
     }


#define STRICT_GENERATE_CONST_SLICES1D()                                                            \
   [[nodiscard]] auto operator[](internal::All) const {                                             \
      return ConstSliceArray<std::remove_reference_t<decltype(*this)>>                              \
         {*this, slice(0, size())};                                                                 \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](seq s) const {                                                     \
      ASSERT_STRICT_DEBUG(!empty());                                                                \
      ASSERT_STRICT_DEBUG(s.valid(*this));                                                          \
      return ConstSliceArray<std::remove_reference_t<decltype(*this)>>                              \
         {*this, s.to_slice()};                                                                     \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](seqN s) const {                                                    \
      ASSERT_STRICT_DEBUG(s.valid(*this));                                                          \
      return ConstSliceArray<std::remove_reference_t<decltype(*this)>>                              \
         {*this, s};                                                                                \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](internal::Even) const {                                            \
      return operator[](slice(0, (size()+1)/2, 2));                                                 \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](internal::Odd) const {                                             \
      return operator[](slice(1, size()/2, 2));                                                     \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](skipN s) const {                                                   \
      return operator[](slice(s.start(), (size()+s.stride()-1-s.start())/s.stride(), s.stride()));  \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] inline auto operator[](std::vector<ImplicitInt> indexes) const {                   \
      return RandConstSliceArray<std::remove_reference_t<decltype(*this)>>                          \
         {*this, std::move(indexes)};                                                               \
   }                                                                                                \
                                                                                                    \
   [[nodiscard]] auto operator[](std::initializer_list<ImplicitInt> indexes) const {                \
      return RandConstSliceArray<std::remove_reference_t<decltype(*this)>>                          \
         {*this, indexes};                                                                          \
   }

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


#define STRICT_GENERATE_SMALL_EXPLICIT_TYPES(SmallObjectName)       \
template<RealType T>                                                \
class SmallObjectName                                               \
{                                                                   \
public:                                                             \
   explicit constexpr SmallObjectName() = default;                  \
   explicit constexpr SmallObjectName(T x) : x{x} {}                \
   explicit constexpr SmallObjectName(StrictVal<T> x) : x{x} {}     \
   [[nodiscard]] constexpr StrictVal<T> get() const { return x; }   \
private:                                                            \
   StrictVal<T> x;                                                  \
};


#define STRICT_GENERATE_SMALL_INT_EXPLICIT_TYPES(SmallObjectName) \
class SmallObjectName                                             \
{                                                                 \
public:                                                           \
   explicit constexpr SmallObjectName() : x{} {}                  \
                                                                  \
   template<IntegerType T>                                        \
   explicit constexpr SmallObjectName(StrictVal<T> x) :           \
      x{real_cast<strict_int>(x)} {}                              \
                                                                  \
   template<IntegerType T>                                        \
   explicit constexpr SmallObjectName(T x) : x{x} {}              \
                                                                  \
   [[nodiscard]] constexpr strict_int get() const { return x; }   \
private:                                                          \
   strict_int x;                                                  \
};


#define STRICT_GENERATE_SMALL_INT_IMPLICIT_TYPES(SmallObjectName)  \
class SmallObjectName                                              \
{                                                                  \
public:                                                            \
   constexpr SmallObjectName() : x{} {}                            \
   template<IntegerType T>                                         \
   constexpr SmallObjectName(StrictVal<T> x) :                     \
      x{real_cast<strict_int>(x)} {}                               \
                                                                   \
   template<IntegerType T>                                         \
   constexpr SmallObjectName(T x) : x{x} {}                        \
                                                                   \
   [[nodiscard]] constexpr strict_int get() const { return x; }    \
private:                                                           \
   strict_int x;                                                   \
};


namespace strict {


STRICT_GENERATE_SMALL_EXPLICIT_TYPES(Low)
STRICT_GENERATE_SMALL_EXPLICIT_TYPES(High)
STRICT_GENERATE_SMALL_EXPLICIT_TYPES(Start)
STRICT_GENERATE_SMALL_EXPLICIT_TYPES(End)
STRICT_GENERATE_SMALL_EXPLICIT_TYPES(Incr)
STRICT_GENERATE_SMALL_EXPLICIT_TYPES(CVal)


STRICT_GENERATE_SMALL_INT_EXPLICIT_TYPES(Size)
STRICT_GENERATE_SMALL_INT_EXPLICIT_TYPES(Index)
STRICT_GENERATE_SMALL_INT_IMPLICIT_TYPES(ImplicitInt)


namespace internal {
   class All {};
   class Even {};
   class Odd {};

   class Last
   {
   public:
      constexpr explicit Last(ImplicitInt i) :
         i{i.get()}
      {}

   [[nodiscard]] constexpr strict_int get() const {
      return i;
   }

   private:
      strict_int i;
   };
}


inline constexpr internal::All all;
inline constexpr internal::Even even;
inline constexpr internal::Odd odd;
inline constexpr internal::Last last{0};


// note that plus operator is allowed from both sides bot not minus
constexpr internal::Last operator+(internal::Last last, ImplicitInt i)
{
   return internal::Last{last.get() - i.get()};
}


constexpr internal::Last operator+(ImplicitInt i, internal::Last last)
{
   return internal::Last{last.get() - i.get()};
}


constexpr internal::Last operator-(internal::Last last, ImplicitInt i)
{
   return internal::Last{last.get() + i.get()};
}


namespace internal {
   template<BaseType BaseT>
   constexpr bool valid_index(const BaseT & A, strict_int index) {
      return index > -1 && index < A.size();
   }

   template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2>
   constexpr bool same_size(const OneDimBaseT1 & A, const OneDimBaseT2 & B) {
      return A.size() == B.size();
   }

   template<BaseType BaseT>
   bool valid_vector(const BaseT & A, std::vector<ImplicitInt> v)
   {
      if(!v.empty()) {
         strict_int sz = static_cast<strict_int>(v.size());

         if(sz > A.size()) return false;
         if(v[0].get() < 0) return false;
         for(strict_int i = 1; i < sz; ++i)
            if(v[i-1].get() >= v[i].get())
               return false;

         if(!internal::valid_index(A, v[v.size()-1].get()))
            return false;
      }
      return true;
   }

   std::vector<ImplicitInt> complement_set(const std::vector<ImplicitInt> & s, ImplicitInt sz_imp) {
      auto sz = sz_imp.get();
      ASSERT_STRICT_DEBUG(sz > -1);

      if(s.empty()) {
         std::vector<ImplicitInt> cmpl(sz);
         for(strict_int i{}; i < sz; ++i) {
            cmpl[i] = i;
         }
         return cmpl;
      }

      ASSERT_STRICT_DEBUG(sz >= s.size());             // complement is no greater than the total size
      ASSERT_STRICT_DEBUG(s[0].get() > -1);            // at this point indexes are of at least size 1 and first element is valid
      #ifndef STRICT_DEBUG_OFF
      for(strict_int i = 1; i < strict_int(s.size()); ++i) {
         ASSERT_STRICT_ALWAYS(s[i].get() > s[i-1].get());
      }
      #endif
      ASSERT_STRICT_DEBUG(s[s.size()-1].get() < sz);   // last element is valid as well

      std::vector<ImplicitInt> cmpl{};
      cmpl.reserve(sz);
      for(strict_int i{}, cnt{}; i < sz; ++i) {
         if(cnt < s.size() && s[cnt].get() == i) {
            ++cnt;
         }
         else {
            cmpl.push_back(i);
         }
      }
      return cmpl;
   }

}


}

#endif
