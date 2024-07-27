// Arkadijs Slobodkins, 2023


#pragma once


#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

#include "auxiliary_types.hpp"  // ImplicitInt
#include "concepts.hpp"


namespace slib {


template <Builtin T>
using List2D = std::initializer_list<std::initializer_list<Strict<T>>>;


template <BaseType Base, Builtin U>
STRICT_CONSTEXPR_INLINE auto value_type_cast(Strict<U> x) {
   return strict_cast<BuiltinTypeOf<Base>>(x);
}


template <typename T> concept IndexType = SignedInteger<T> || SameAs<T, StrictInt> || SameAs<T, StrictLong>
                                       || SameAs<T, ImplicitInt> || SameAs<T, internal::Last>;


namespace internal {


template <typename T>
class IndexWrapper {
public:
   IndexWrapper(T i) = delete;
};


template <typename T>
   requires SignedInteger<T> || SameAs<T, StrictInt> || SameAs<T, StrictLong> || SameAs<T, ImplicitInt>
class IndexWrapper<T> {
   ImplicitInt i_;

public:
   STRICT_CONSTEXPR_INLINE IndexWrapper(T i) : i_{i} {
   }

   STRICT_CONSTEXPR_INLINE index_t get([[maybe_unused]] OneDimBaseType auto const& A) const {
      return i_.get();
   }

   STRICT_CONSTEXPR_INLINE index_t get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return i_.get();
   }

   STRICT_CONSTEXPR_INLINE index_t get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return i_.get();
   }
};


template <typename T>
   requires SameAs<T, Last>
class IndexWrapper<T> {
   index_t i_;

public:
   STRICT_CONSTEXPR_INLINE IndexWrapper(T lst) : i_{lst.get()} {
   }

   STRICT_CONSTEXPR_INLINE index_t get(OneDimBaseType auto const& A) const {
      return A.size() - i_ - 1_sl;
   }

   STRICT_CONSTEXPR_INLINE index_t get_row(TwoDimBaseType auto const& A) const {
      return A.rows() - i_ - 1_sl;
   }

   STRICT_CONSTEXPR_INLINE index_t get_col(TwoDimBaseType auto const& A) const {
      return A.cols() - i_ - 1_sl;
   }
};


STRICT_CONSTEXPR_INLINE index_t index_helper(OneDimBaseType auto const& A, auto i) {
   return IndexWrapper{i}.get(A);
}


STRICT_CONSTEXPR_INLINE index_t index_row_helper(TwoDimBaseType auto const& A, auto i) {
   return IndexWrapper{i}.get_row(A);
}


STRICT_CONSTEXPR_INLINE index_t index_col_helper(TwoDimBaseType auto const& A, auto j) {
   return IndexWrapper{j}.get_col(A);
}


STRICT_CONSTEXPR_INLINE std::pair<index_t, index_t> index_row_col_helper(TwoDimBaseType auto const& A, auto i,
                                                                         auto j) {
   return {IndexWrapper{i}.get_row(A), IndexWrapper{j}.get_col(A)};
}


STRICT_CONSTEXPR StrictBool valid_index(BaseType auto const& A, index_t i) {
   return i > -1_sl && i < A.size();
}


STRICT_CONSTEXPR StrictBool valid_row(TwoDimBaseType auto const& A, index_t i) {
   return i > -1_sl && i < A.rows();
}


STRICT_CONSTEXPR StrictBool valid_col(TwoDimBaseType auto const& A, index_t j) {
   return j > -1_sl && j < A.cols();
}


STRICT_CONSTEXPR StrictBool valid_index(TwoDimBaseType auto const& A, index_t i, index_t j) {
   return valid_row(A, i) && valid_col(A, j);
}


STRICT_CONSTEXPR StrictBool semi_valid_row_col_sizes(index_t m, index_t n) {
   return !((m == 0_sl) ^ (n == 0_sl));
}


template <Builtin T>
STRICT_CONSTEXPR StrictBool valid_list2D(List2D<T> list) {
   if(list.size() != 0) {
      const auto nrows = list.size();
      const auto ncols = (*list.begin()).size();

      // allow empty initializer_list
      if(nrows == 1 && ncols == 0) {
         return true_sb;
      }

      // number of columns is not zero to forbid nonzero rows and zero columns
      if(ncols == 0) {
         return false_sb;
      }

      for(auto lrow : list) {
         if(lrow.size() != ncols) {
            return false_sb;
         }
      }
   }
   return true_sb;
}


template <Builtin T>
STRICT_CONSTEXPR auto list2D_row_col_sizes(List2D<T> list) {
   if(list.size() != 0) {
      const auto nrows = list.size();
      const auto ncols = (*list.begin()).size();

      // allow empty initializer_list
      if(nrows == 1 && ncols == 0) {
         return std::pair{0_sl, 0_sl};
      }

      return std::pair{from_size_t<long int>(nrows), from_size_t<long int>(ncols)};
   } else {
      return std::pair{0_sl, 0_sl};
   }
}


STRICT_CONSTEXPR StrictBool valid_slice_vector(BaseType auto const& A,
                                               const std::vector<ImplicitInt>& indexes) {
   return Strict{std::ranges::all_of(indexes, [&A](auto i) { return bool{valid_index(A, i.get())}; })};
}


STRICT_CONSTEXPR StrictBool valid_complement_index_vector(BaseType auto const& A,
                                                          const std::vector<ImplicitInt>& indexes) {
   if(!indexes.empty()) {
      if(auto lst = indexes.size() - 1;
         !valid_index(A, indexes[0].get()) || !valid_index(A, indexes[lst].get())) {
         return false_sb;
      }
   }
   // check if indexes are in strictly increasing order
   return Strict{std::ranges::is_sorted(indexes, [](auto x, auto y) { return bool{x.get() <= y.get()}; })};
}


STRICT_CONSTEXPR std::vector<ImplicitInt> complement_index_vector(BaseType auto const& A,
                                                                  const std::vector<ImplicitInt>& indexes) {
   ASSERT_STRICT_DEBUG(valid_complement_index_vector(A, indexes));

   std::vector<ImplicitInt> cmpl;
   cmpl.reserve(to_size_t(A.size()) - indexes.size());
   for(index_t i = 0_sl, cnt = 0_sl; i < A.size(); ++i) {
      // no UB since the order of evaluation is left to right
      if(bool{to_size_t(cnt) < indexes.size()} && bool{i == indexes[to_size_t(cnt)].get()}) {
         ++cnt;
      } else {
         cmpl.emplace_back(i);
      }
   }
   return cmpl;
}


STRICT_INLINE std::string smart_spaces(long int maxi, long int i) {
   auto count_digit = [](long int number) -> long int {
      if(number == 0) {
         return 1;
      }
      return static_cast<long int>(std::log10(number)) + 1;
   };

   long int max_digits = count_digit(maxi);
   long int ind_digits = count_digit(i);
   return std::string(static_cast<std::string::size_type>(1 + max_digits - ind_digits), 32);
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR_INLINE void apply0(Base& A, F f) {
   for(index_t i = 0_sl; i < A.size(); ++i) {
      f(i);
   }
}


template <BaseType Base1, BaseType Base2, typename F>
STRICT_CONSTEXPR_INLINE void apply1(Base1& A1, [[maybe_unused]] const Base2& A2, F f) {
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      f(i);
   }
}


template <typename It, BaseType Base>
STRICT_CONSTEXPR_INLINE void copy(It b, It e, Base& A) {
   for(index_t count = 0_sl; b != e; ++b) {
      A.index(count++) = *b;
   }
}


template <BaseType Base>
STRICT_CONSTEXPR_INLINE void copy(std::initializer_list<ValueTypeOf<Base>> L, Base& A) {
   copy(L.begin(), L.end(), A);
}


template <Builtin T, TwoDimBaseType Base>
STRICT_CONSTEXPR_INLINE void copy(List2D<T> L, Base& A) {
   StrictInt r{};
   for(auto lrow : L) {
      StrictInt c{};
      for(auto lcol : lrow) {
         A.index(r, c++) = lcol;
      }
      ++r;
   }
}


template <BaseType Base1, BaseType Base2>
STRICT_CONSTEXPR_INLINE void copy(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2) {
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      A2.index(i) = A1.index(i);
   }
}


template <BaseType Base1, BaseType Base2>
STRICT_CONSTEXPR_INLINE void copyn(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2, index_t n) {
   for(index_t i = 0_sl; i < n; ++i) {
      A2.index(i) = A1.index(i);
   }
}


template <BaseType Base>
STRICT_CONSTEXPR_INLINE void fill(ValueTypeOf<Base> val, Base& A) {
   for(index_t i = 0_sl; i < A.size(); ++i) {
      A.index(i) = val;
   }
}


}  // namespace internal


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base1, OneDimBaseType Base2, OneDimBaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR StrictBool same_size(const Base1& A1, const Base2& A2, const BArgs&... AArgs) {
   if constexpr(sizeof...(BArgs) > 0) {
      return A1.size() == A2.size() && same_size(A2, AArgs...);
   }
   return A1.size() == A2.size();
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2, TwoDimBaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR StrictBool same_size(const Base1& A1, const Base2& A2, const BArgs&... AArgs) {
   StrictBool b = A1.rows() == A2.rows() && A1.cols() == A2.cols();
   if constexpr(sizeof...(BArgs) > 0) {
      return b && same_size(A2, AArgs...);
   }
   return b;
}


template <BaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR StrictBool same_dimension() {
   auto last_dim = LastPack_t<BArgs...>::dimension();
   return (... && (last_dim == BArgs::dimension()));
}


template <BaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR bool same_dimension_b() {
   return bool{same_dimension<BArgs...>()};
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base1, BaseType Base2>
   requires (same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool operator==(const Base1& A1, const Base2& A2) {
   if(!same_size(A1, A2)) {
      return false_sb;
   }
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      if(A1.index(i) != A2.index(i)) {
         return false_sb;
      }
   }
   return true_sb;
}


template <BaseType Base1, BaseType Base2>
   requires (same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool operator!=(const Base1& A1, const Base2& A2) {
   return !(A1 == A2);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
STRICT_INLINE std::ostream& operator<<(std::ostream& os, const std::vector<ImplicitInt>& indexes) {
   for(typename std::vector<ImplicitInt>::size_type i = 0; i < indexes.size(); ++i) {
      os << "index[" << i
         << "] =" << internal::smart_spaces(static_cast<long int>(indexes.size()), static_cast<long int>(i))
         << indexes[i].get().val() << std::endl;
   }
   return os;
}


}  // namespace slib
