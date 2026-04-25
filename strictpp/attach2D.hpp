// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_traits.hpp"
#include "StrictCommon/strict_common.hpp"
#include "derived1D.hpp"
#include "derived2D.hpp"
#include "slice.hpp"


namespace spp {


namespace detail {


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
class SliceArrayBase2D;


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
class ConstSliceArrayBase2D;


template <Builtin T>
struct STRICT_NODISCARD StrictAttachPtr2D : private CopyBase2D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD StrictAttachPtr2D(T* data, ImplicitInt m, ImplicitInt n)
      : data_{reinterpret_cast<Strict<T>*>(data)},
        m_{m.get()},
        n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE value_type& un(ImplicitInt i) {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE value_type& un(ImplicitInt i, ImplicitInt j) {
      return data_[i.get().val() * n_.val() + j.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const {
      return data_[i.get().val() * n_.val() + j.get().val()];
   }

   STRICT_NODISCARD_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return m_ * n_;
   }

private:
   Strict<T>* data_;
   index_t m_, n_;
};


template <Builtin T>
struct STRICT_NODISCARD ConstStrictAttachPtr2D : private CopyBase2D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD ConstStrictAttachPtr2D(const T* data, ImplicitInt m, ImplicitInt n)
      : data_{reinterpret_cast<const Strict<T>*>(data)},
        m_{m.get()},
        n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const {
      return data_[i.get().val() * n_.val() + j.get().val()];
   }

   STRICT_NODISCARD_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return m_ * n_;
   }

private:
   const Strict<T>* data_;
   index_t m_, n_;
};


// concept is BaseType since both 1-D and 2-D arrays must be handled.
template <BaseType Base>
struct STRICT_NODISCARD StrictConvertSlice2D : private CopyBase2D {
public:
   using value_type = Base::value_type;
   using builtin_type = Base::builtin_type;

   STRICT_NODISCARD_CONSTEXPR StrictConvertSlice2D(Base& A, ImplicitInt m, ImplicitInt n)
      : A_{A},
        m_{m.get()},
        n_{n.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR StrictConvertSlice2D(const StrictConvertSlice2D&) = default;

   // SliceArrayBase2D does not implement assignment based on the assignment of its members,
   // thus the following assignment is not strictly necessary but might be useful for other
   // purposes.
   STRICT_NODISCARD_CONSTEXPR StrictConvertSlice2D& operator=(const StrictConvertSlice2D& S) {
      ASSERT_STRICT_DEBUG(same_size(*this, S));
      A_ = S.A_;
      return *this;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i) {
      return A_.un(i.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i) const {
      return A_.un(i.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i, ImplicitInt j) {
      return A_.un(i.get().val() * n_.val() + j.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const {
      return A_.un(i.get().val() * n_.val() + j.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto size() const {
      return m_ * n_;
   }

private:
   typename CopyOrReferenceExpr<Base>::type A_;
   index_t m_, n_;
};


// concept is BaseType since both 1-D and 2-D arrays must be handled.
template <BaseType Base>
struct STRICT_NODISCARD ConstStrictConvertSlice2D : private CopyBase2D {
public:
   using value_type = Base::value_type;
   using builtin_type = Base::builtin_type;

   STRICT_NODISCARD_CONSTEXPR ConstStrictConvertSlice2D(const Base& A, ImplicitInt m,
                                                           ImplicitInt n)
      : A_{A},
        m_{m.get()},
        n_{n.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR
   ConstStrictConvertSlice2D(const ConstStrictConvertSlice2D&) = default;
   STRICT_NODISCARD_CONSTEXPR ConstStrictConvertSlice2D&
   operator=(const ConstStrictConvertSlice2D&) = delete;

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i) const {
      return A_.un(i.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i, ImplicitInt j) const {
      return A_.un(i.get().val() * n_.val() + j.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto size() const {
      return m_ * n_;
   }

private:
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   index_t m_, n_;
};


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR auto convert2D(Base& A, ImplicitInt m, ImplicitInt n) {
   auto proxy = StrictConvertSlice2D{A, m, n};
   return StrictArrayMutable2D<SliceArrayBase2D<decltype(proxy), seqN, seqN>>{
      proxy, seqN{0, m},
       seqN{0, n}
   };
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR auto const_convert2D(const Base& A, ImplicitInt m, ImplicitInt n) {
   auto proxy = ConstStrictConvertSlice2D{A, m, n};
   return StrictArrayBase2D<ConstSliceArrayBase2D<decltype(proxy), seqN, seqN>>{
      proxy, seqN{0, m},
       seqN{0, n}
   };
}


} // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto StrictArrayBase1D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) & {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   if constexpr(detail::NonConstBaseType<StrictArrayBase1D>) {
      return detail::convert2D(*this, nrows, ncols);
   } else {
      return detail::const_convert2D(*this, nrows, ncols);
   }
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto StrictArrayBase1D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) const& {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   return detail::const_convert2D(*this, nrows, ncols);
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto StrictArrayBase1D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) &&
   requires(!detail::ArrayOneDimType<StrictArrayBase1D>)
{
   return this->view2D(nrows, ncols);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto StrictArrayBase2D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) & {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   if constexpr(detail::NonConstBaseType<StrictArrayBase2D>) {
      return detail::convert2D(*this, nrows, ncols);
   } else {
      return detail::const_convert2D(*this, nrows, ncols);
   }
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto StrictArrayBase2D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) const& {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   return detail::const_convert2D(*this, nrows, ncols);
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto StrictArrayBase2D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) &&
   requires(!detail::ArrayTwoDimType<StrictArrayBase2D>)
{
   return this->view2D(nrows, ncols);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <detail::PointerConvertibleLvalue T>
auto attach2D(T&& data, ImplicitInt m, ImplicitInt n) {
   using namespace detail;
   auto proxy = StrictAttachPtr2D(data, m, n);
   return StrictArrayMutable2D<SliceArrayBase2D<decltype(proxy), seqN, seqN>>{
      proxy, seqN{0, m},
       seqN{0, n}
   };
}


template <detail::PointerConvertibleLvalueConst T>
auto attach2D(T&& data, ImplicitInt m, ImplicitInt n) {
   using namespace detail;
   auto proxy = ConstStrictAttachPtr2D(data, m, n);
   return StrictArrayBase2D<ConstSliceArrayBase2D<decltype(proxy), seqN, seqN>>{
      proxy, seqN{0, m},
       seqN{0, n}
   };
}


} // namespace spp
