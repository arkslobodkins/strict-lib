// Arkadijs Slobodkins, 2023


#pragma once


#include <initializer_list>
#include <utility>
#include <vector>

#include "Common/common.hpp"


namespace slib {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class STRICT_NODISCARD seqN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit seqN(ImplicitInt start, ImplicitInt size, ImplicitInt stride = 1)
       : start_{start.get()},
         size_{size.get()},
         stride_{stride.get()} {
      ASSERT_STRICT_DEBUG(start_ > -1_sl);
      ASSERT_STRICT_DEBUG(size_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit seqN(First first, Size size, Stride stride = Stride{1})
       : seqN{first.get(), size.get(), stride.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t start() const {
      return start_;
   }

   STRICT_NODISCARD_CONSTEXPR index_t size() const {
      return size_;
   }

   STRICT_NODISCARD_CONSTEXPR index_t stride() const {
      return stride_;
   }

   // note: stride is already valid, start and size are "partially" valid
   template <BaseType BaseT>
   STRICT_NODISCARD_CONSTEXPR StrictBool valid(const BaseT& A) const {
      if(size_ != 0_sl) {
         return internal::valid_index(A, start_)
             && internal::valid_index(A, start_ + stride_ * (size_ - 1_sl));
      }
      return true_sb;
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_NODISCARD_CONSTEXPR StrictBool valid_first(const TwoDimBaseT& A) const {
      if(size_ != 0_sl) {
         return internal::valid_row(A, start_) && internal::valid_row(A, start_ + stride_ * (size_ - 1_sl));
      }
      return true_sb;
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_NODISCARD_CONSTEXPR StrictBool valid_second(const TwoDimBaseT& A) const {
      if(size_ != 0_sl) {
         return internal::valid_col(A, start_) && internal::valid_col(A, start_ + stride_ * (size_ - 1_sl));
      }
      return true_sb;
   }

private:
   index_t start_;
   index_t size_;
   index_t stride_;
};


// unlike seqN, seq cannot be empty
class STRICT_NODISCARD seq {
public:
   STRICT_NODISCARD_CONSTEXPR explicit seq(ImplicitInt first, ImplicitInt last, ImplicitInt stride = 1)
       : first_{first.get()},
         last_{last.get()},
         stride_{stride.get()} {
      ASSERT_STRICT_DEBUG(first_ > -1_sl);
      ASSERT_STRICT_DEBUG(last_ > -1_sl);
      ASSERT_STRICT_DEBUG(((stride_ > 0_sl) && (first_ <= last_)) || ((stride_ < 0_sl) && (first_ >= last_))
                          || ((stride_ == 0_sl) && (first_ == last_)));
   }

   STRICT_NODISCARD_CONSTEXPR explicit seq(First first, Endmost last, Stride stride = Stride{1})
       : seq{first.get(), last.get(), stride.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR seqN to_slice() const {
      if(stride_ != 0_sl) {
         return seqN{first_, (last_ - first_) / stride_ + 1_sl, stride_};
      }
      // if stride is 0, avoid division by 0
      // in that case, first must be equal to last
      return seqN{first_, 1_sl, 0_sl};
   }

   STRICT_NODISCARD_CONSTEXPR index_t first() const {
      return first_;
   }

   STRICT_NODISCARD_CONSTEXPR index_t last() const {
      return last_;
   }

   STRICT_NODISCARD_CONSTEXPR index_t stride() const {
      return stride_;
   }

   // note: stride is already valid, first and last are "partially" valid
   template <BaseType BaseT>
   STRICT_NODISCARD_CONSTEXPR StrictBool valid(const BaseT& A) const {
      return internal::valid_index(A, first_) && internal::valid_index(A, last_);
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_NODISCARD_CONSTEXPR StrictBool valid_first(const TwoDimBaseT& A) const {
      return internal::valid_row(A, first_) && internal::valid_row(A, last_);
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_NODISCARD_CONSTEXPR StrictBool valid_second(const TwoDimBaseT& A) const {
      return internal::valid_col(A, first_) && internal::valid_col(A, last_);
   }

private:
   index_t first_;
   index_t last_;
   index_t stride_;
};


namespace place {


class STRICT_NODISCARD skipN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit skipN(ImplicitInt stride) : stride_{stride.get()} {
      // stride cannot be 0, otherwise size of slice cannot be deduced
      ASSERT_STRICT_DEBUG(stride_ > 0_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit skipN(Stride stride) : skipN{stride.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t stride() const {
      return stride_;
   }

private:
   index_t stride_;
};


class STRICT_NODISCARD firstN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit firstN(ImplicitInt n) : n_{n.get()} {
      ASSERT_STRICT_DEBUG(n_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit firstN(Size size) : firstN{size.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return n_;
   }

private:
   index_t n_;
};


class STRICT_NODISCARD lastN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit lastN(ImplicitInt n) : n_{n.get()} {
      ASSERT_STRICT_DEBUG(n_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit lastN(Size size) : lastN{ImplicitInt{size.get()}} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return n_;
   }

private:
   index_t n_;
};


class STRICT_NODISCARD complement {
public:
   STRICT_NODISCARD_CONSTEXPR explicit complement(std::vector<ImplicitInt> v) : v_{std::move(v)} {
   }

   STRICT_NODISCARD_CONSTEXPR explicit complement(std::initializer_list<ImplicitInt> v) : v_{v} {
   }

   STRICT_NODISCARD_CONSTEXPR const std::vector<ImplicitInt>& get() const {
      return v_;
   }

private:
   std::vector<ImplicitInt> v_;
};


}  // namespace place


template <typename T> concept SliceType = SameAs<T, seqN> || SameAs<T, seq> || SameAs<T, internal::All>
                                       || SameAs<T, internal::Even> || SameAs<T, internal::Odd>
                                       || SameAs<T, internal::Reverse> || SameAs<T, place::skipN>
                                       || SameAs<T, place::firstN> || SameAs<T, place::lastN>;


namespace internal {


template <typename T>
class SliceWrapper {
public:
   SliceWrapper(T i) = delete;
};


template <>
class SliceWrapper<seqN> {
   seqN s_;

public:
   STRICT_CONSTEXPR_INLINE SliceWrapper(seqN s) : s_{s} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(s_.valid(A));
      return s_;
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(s_.valid_first(A));
      return s_;
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(s_.valid_second(A));
      return s_;
   }
};


template <>
class SliceWrapper<seq> {
   seq s_;

public:
   STRICT_CONSTEXPR_INLINE SliceWrapper(seq s) : s_{s} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(s_.valid(A));
      return s_.to_slice();
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(s_.valid_first(A));
      return s_.to_slice();
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(s_.valid_second(A));
      return s_.to_slice();
   }
};


template <>
class SliceWrapper<internal::All> {
public:
   STRICT_CONSTEXPR_INLINE SliceWrapper(internal::All) {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      return seqN{0, A.size(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      return seqN{0, A.rows(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      return seqN{0, A.cols(), 1};
   }
};


template <>
class SliceWrapper<internal::Even> {
public:
   STRICT_CONSTEXPR_INLINE SliceWrapper(internal::Even) {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      return seqN{0, (A.size() + 1_sl) / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      return seqN{0, (A.rows() + 1_sl) / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      return seqN{0, (A.cols() + 1_sl) / 2_sl, 2};
   }
};


template <>
class SliceWrapper<internal::Odd> {
public:
   STRICT_CONSTEXPR_INLINE SliceWrapper(internal::Odd) {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      return seqN{1, A.size() / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      return seqN{1, A.rows() / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      return seqN{1, A.cols() / 2_sl, 2};
   }
};


template <>
class SliceWrapper<internal::Reverse> {
public:
   STRICT_CONSTEXPR_INLINE SliceWrapper(internal::Reverse) {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      return seqN{A.size() - 1_sl, A.size(), -1};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      return seqN{A.rows() - 1_sl, A.rows(), -1};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      return seqN{A.cols() - 1_sl, A.cols(), -1};
   }
};


template <>
class SliceWrapper<place::skipN> {
public:
   place::skipN x_;

   STRICT_CONSTEXPR_INLINE SliceWrapper(place::skipN x) : x_{x} {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      return seqN{0, (A.size() + x_.stride() - 1_sl) / x_.stride(), x_.stride()};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      return seqN{0, (A.rows() + x_.stride() - 1_sl) / x_.stride(), x_.stride()};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      return seqN{0, (A.cols() + x_.stride() - 1_sl) / x_.stride(), x_.stride()};
   }
};


template <>
class SliceWrapper<place::firstN> {
public:
   place::firstN x_;

   STRICT_CONSTEXPR_INLINE SliceWrapper(place::firstN x) : x_{x} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.size());
      return seqN{0, x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.rows());
      return seqN{0, x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.cols());
      return seqN{0, x_.get(), 1};
   }
};


template <>
class SliceWrapper<place::lastN> {
public:
   place::lastN x_;

   STRICT_CONSTEXPR_INLINE SliceWrapper(place::lastN x) : x_{x} {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.size());
      return seqN{A.size() - x_.get(), x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.rows());
      return seqN{A.rows() - x_.get(), x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.cols());
      return seqN{A.cols() - x_.get(), x_.get(), 1};
   }
};


STRICT_CONSTEXPR_INLINE seqN slice_helper(OneDimBaseType auto const& A, auto i) {
   return SliceWrapper{i}.get(A);
}


STRICT_CONSTEXPR_INLINE seqN slice_row_helper(TwoDimBaseType auto const& A, auto i) {
   return SliceWrapper{i}.get_row(A);
}


STRICT_CONSTEXPR_INLINE seqN slice_col_helper(TwoDimBaseType auto const& A, auto j) {
   return SliceWrapper{j}.get_col(A);
}


STRICT_CONSTEXPR_INLINE std::pair<seqN, seqN> slice_row_col_helper(TwoDimBaseType auto const& A, auto i,
                                                                   auto j) {
   return {SliceWrapper{i}.get_row(A), SliceWrapper{j}.get_col(A)};
}


}  // namespace internal


}  // namespace slib
