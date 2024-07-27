//  Copyright (C) 2024 Arkadijs Slobodkins - All Rights Reserved
// License is 3-clause BSD:
// https://github.com/arkslobodkins/strict-lib


#pragma once


#include <iterator>
#include <utility>

#include "Common/common.hpp"


namespace slib {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base>
class IndexFlag {
public:
   STRICT_CONSTEXPR IndexFlag(Base* A) : A_{A} {
   }

   // non-const iterator only needs non-const indexing.
   // returns a reference because non-const iterators only
   // operate on NonConstBaseType bases, which return lvalues
   STRICT_CONSTEXPR ValueTypeOf<Base>& operator()(ImplicitInt i) const {
      return (*A_).index(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).size();
   }

private:
   Base* A_;
};


template <TwoDimNonConstBaseType Base>
class RowFlag {
public:
   STRICT_CONSTEXPR RowFlag(Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).row(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).rows();
   }

private:
   Base* A_;
};


template <TwoDimNonConstBaseType Base>
class ColFlag {
public:
   STRICT_CONSTEXPR ColFlag(Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).col(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).cols();
   }

private:
   Base* A_;
};


template <BaseType Base>
class ConstIndexFlag {
public:
   STRICT_CONSTEXPR ConstIndexFlag(const Base* A) : A_{A} {
   }

   // const iterator only needs const indexing(A_ is a pointer to const)
   // may return either by const reference or by value, as it the case
   // with expression templates
   STRICT_CONSTEXPR decltype(auto) operator()(ImplicitInt i) const {
      return (*A_).index(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).size();
   }

private:
   const Base* A_;
};


template <TwoDimBaseType Base>
class ConstRowFlag {
public:
   STRICT_CONSTEXPR ConstRowFlag(const Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).row(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).rows();
   }

private:
   const Base* A_;
};


template <TwoDimBaseType Base>
class ConstColFlag {
public:
   STRICT_CONSTEXPR ConstColFlag(const Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).col(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).cols();
   }

private:
   const Base* A_;
};


template <typename T, typename Flag>
class IteratorTag {};


// IteratorTag that uses .index() for indexing is a linear iterator
// they can be passed to constructors of one-dimensional Arrays
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class IteratorTag<T, IndexFlag<T>> : private LinearIterator {};


template <typename T>
class IteratorTag<T, ConstIndexFlag<T>> : private LinearIterator {};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base, typename F = IndexFlag<Base>>
class STRICT_NODISCARD Iterator : private IteratorTag<Base, F> {
public:
   // ensure that constant types are passed to ConstIterator
   // even though Iterator would function properly for iterating over rows and columns,
   // it would not work for one-dimensional indexing and potentially cause other issues
   static_assert(!IsConst<Base>);

   // difference_type is long int instead of index_t so that std::random_access_iterator
   // concept is true and for better compatibility with algorithms from the standard library
   // similarly, operators + += - -=, and [] have difference_type as a parameter instead of
   // ImplicitInt for similar reasons
   using difference_type = long int;
   using value_type = RemoveRef<decltype(std::declval<F>()(0))>;
   using pointer = value_type*;
   using reference = value_type&;
   // Contiguous if Base owns the data and iterates over indexes(not rows and not columns)
   using iterator_category
       = std::conditional_t<ReferenceStorable<Base> && LinearIteratorType<IteratorTag<Base, F>>,
                            std::contiguous_iterator_tag, std::random_access_iterator_tag>;

public:
   STRICT_NODISCARD_CONSTEXPR explicit Iterator();
   STRICT_NODISCARD_CONSTEXPR explicit Iterator(Base& A, index_t pos);

   STRICT_CONSTEXPR Iterator& operator++();
   STRICT_CONSTEXPR Iterator& operator--();
   STRICT_CONSTEXPR Iterator operator++(int);
   STRICT_CONSTEXPR Iterator operator--(int);

   STRICT_CONSTEXPR Iterator& operator+=(difference_type incr);
   STRICT_CONSTEXPR Iterator& operator-=(difference_type incr);

   STRICT_NODISCARD_CONSTEXPR Iterator operator+(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR Iterator operator-(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR difference_type operator-(const Iterator& it) const;

   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator[](difference_type n) const;
   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator*() const;
   // only works for iterators over indexes, since rows and
   // columns are returned by value and appear as rvalues
   STRICT_NODISCARD_CONSTEXPR pointer operator->() const;

   // bool is used rather than StrictBool so that std::random_access_iterator concept is true
   // and for better compatibility with algorithms from the standard library
   STRICT_NODISCARD_CONSTEXPR bool operator==(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator!=(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<=(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>=(const Iterator& it) const;

   STRICT_NODISCARD_CONSTEXPR StrictBool points_somewhere() const;

private:
   Base* A_ptr_;
   index_t pos_;
   F f_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> operator+(long int incr, const Iterator<Base, F>& it) {
   ASSERT_STRICT_DEBUG(it.points_somewhere());
   return it + incr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F>::Iterator() : A_ptr_{nullptr},
                                                           pos_{-1_sl},
                                                           f_{nullptr} {
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F>::Iterator(Base& A, index_t pos) : A_ptr_{&A},
                                                                               pos_{pos},
                                                                               f_{&A} {
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator++() {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ++pos_;
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator--() {
   ASSERT_STRICT_DEBUG(points_somewhere());
   --pos_;
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator++(int) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto old = *this;
   ++*this;
   return old;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator--(int) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto old = *this;
   --*this;
   return old;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator+=(difference_type incr) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   pos_ += Strict{incr};
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator-=(difference_type incr) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   pos_ -= Strict{incr};
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator+(difference_type incr) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto new_it = *this;
   new_it += incr;
   return new_it;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator-(difference_type incr) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto Iterator<Base, F>::operator-(const Iterator& it) const -> difference_type {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return (pos_ - it.pos_).val();
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) Iterator<Base, F>::operator[](difference_type n) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ + Strict{n} > -1_sl && pos_ + Strict{n} < f_.size());
   return f_(pos_ + Strict{n});
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) Iterator<Base, F>::operator*() const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return f_(pos_);
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto Iterator<Base, F>::operator->() const -> pointer {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return &f_(pos_);
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator==(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ == it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator!=(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ != it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator<(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ < it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator<=(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ <= it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator>(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ > it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator>=(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ >= it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR StrictBool Iterator<Base, F>::points_somewhere() const {
   return StrictBool{A_ptr_ != nullptr};
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename F = ConstIndexFlag<Base>>
class STRICT_NODISCARD ConstIterator : private IteratorTag<Base, F> {
public:
   // difference_type is long int instead of index_t so that std::random_access_iterator
   // concept is true and for better compatibility with algorithms from the standard library
   // similarly, operators + += - -=, and [] have difference_type as a parameter instead of
   // ImplicitInt for similar reasons
   using difference_type = long int;
   using value_type = RemoveRef<decltype(std::declval<F>()(0))>;
   using pointer = value_type*;
   using const_pointer = const value_type*;
   using reference = value_type&;
   using const_reference = const value_type&;
   // Contiguous if Base owns the data and iterates over indexes(not rows and not columns)
   using iterator_category
       = std::conditional_t<ReferenceStorable<Base> && LinearIteratorType<IteratorTag<Base, F>>,
                            std::contiguous_iterator_tag, std::random_access_iterator_tag>;

   STRICT_NODISCARD_CONSTEXPR explicit ConstIterator();
   STRICT_NODISCARD_CONSTEXPR explicit ConstIterator(const Base& A, index_t pos);

   STRICT_CONSTEXPR ConstIterator& operator++();
   STRICT_CONSTEXPR ConstIterator& operator--();
   STRICT_CONSTEXPR ConstIterator operator++(int);
   STRICT_CONSTEXPR ConstIterator operator--(int);

   STRICT_CONSTEXPR ConstIterator& operator+=(difference_type incr);
   STRICT_CONSTEXPR ConstIterator& operator-=(difference_type incr);

   STRICT_NODISCARD_CONSTEXPR ConstIterator operator+(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR ConstIterator operator-(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR difference_type operator-(const ConstIterator& it) const;

   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator[](difference_type n) const;
   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator*() const;
   // only works for iterators over indexes, since rows and
   // columns are returned by value and appear as rvalues
   STRICT_NODISCARD_CONSTEXPR const_pointer operator->() const;

   // bool is used rather than StrictBool so that std::random_access_iterator concept is true
   // and for better compatibility with algorithms from the standard library
   STRICT_NODISCARD_CONSTEXPR bool operator==(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator!=(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<=(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>=(const ConstIterator& it) const;

   STRICT_NODISCARD_CONSTEXPR StrictBool points_somewhere() const;

private:
   const Base* A_ptr_;
   index_t pos_;
   F f_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> operator+(long int incr, const ConstIterator<Base, F>& it) {
   ASSERT_STRICT_DEBUG(it.points_somewhere());
   return it + incr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F>::ConstIterator()
    : A_ptr_{nullptr},
      pos_{-1_sl},
      f_{nullptr} {
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F>::ConstIterator(const Base& A, index_t pos)
    : A_ptr_{&A},
      pos_{pos},
      f_{&A} {
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator++() {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ++pos_;
   return *this;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator--() {
   ASSERT_STRICT_DEBUG(points_somewhere());
   --pos_;
   return *this;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator++(int) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto old = *this;
   ++*this;
   return old;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator--(int) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto old = *this;
   --*this;
   return old;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator+=(difference_type incr) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   pos_ += Strict{incr};
   return *this;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator-=(difference_type incr) {
   ASSERT_STRICT_DEBUG(points_somewhere());
   pos_ -= Strict{incr};
   return *this;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator+(
    difference_type incr) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto new_it = *this;
   new_it += incr;
   return new_it;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator-(
    difference_type incr) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto ConstIterator<Base, F>::operator-(const ConstIterator& it) const
    -> difference_type {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return (pos_ - it.pos_).val();
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) ConstIterator<Base, F>::operator[](difference_type n) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ + Strict{n} > -1_sl && pos_ + Strict{n} < f_.size());
   return f_(pos_ + Strict{n});
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) ConstIterator<Base, F>::operator*() const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return f_(pos_);
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto ConstIterator<Base, F>::operator->() const -> const_pointer {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return &(f_(pos_));
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator==(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ == it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator!=(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ != it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator<(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ < it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator<=(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ <= it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator>(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ > it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator>=(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ >= it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR StrictBool ConstIterator<Base, F>::points_somewhere() const {
   return StrictBool{A_ptr_ != nullptr};
}


template <TwoDimBaseType Base>
class STRICT_NODISCARD row_it {
public:
   STRICT_NODISCARD_CONSTEXPR row_it(Base& A) : A_{A} {
   }

   STRICT_NODISCARD_CONSTEXPR auto begin() {
      if constexpr(NonConstBaseType<Base>) {
         return Iterator<Base, RowFlag<Base>>{A_, 0_sl};
      } else {
         return ConstIterator<Base, ConstRowFlag<Base>>{A_, 0_sl};
      }
   }

   STRICT_NODISCARD_CONSTEXPR auto begin() const {
      return ConstIterator<Base, ConstRowFlag<Base>>{A_, 0_sl};
   }

   STRICT_NODISCARD_CONSTEXPR auto end() {
      if constexpr(NonConstBaseType<Base>) {
         return Iterator<Base, RowFlag<Base>>{A_, A_.rows()};
      } else {
         return ConstIterator<Base, ConstRowFlag<Base>>{A_, A_.rows()};
      }
   }

   STRICT_NODISCARD_CONSTEXPR auto end() const {
      return ConstIterator<Base, ConstRowFlag<Base>>{A_, A_.rows()};
   }

   STRICT_NODISCARD_CONSTEXPR auto cbegin() const {
      return this->begin();
   }

   STRICT_NODISCARD_CONSTEXPR auto cend() const {
      return this->end();
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() const {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() const {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crbegin() const {
      return std::reverse_iterator{this->rbegin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crend() const {
      return std::reverse_iterator{this->rend()};
   }

private:
   Base& A_;
};


template <TwoDimBaseType Base>
class STRICT_NODISCARD col_it {
public:
   STRICT_NODISCARD_CONSTEXPR col_it(Base& A) : A_{A} {
   }

   STRICT_NODISCARD_CONSTEXPR auto begin() {
      if constexpr(NonConstBaseType<Base>) {
         return Iterator<Base, ColFlag<Base>>{A_, 0_sl};
      } else {
         return ConstIterator<Base, ConstColFlag<Base>>{A_, 0_sl};
      }
   }

   STRICT_NODISCARD_CONSTEXPR auto begin() const {
      return ConstIterator<Base, ConstColFlag<Base>>{A_, 0_sl};
   }

   STRICT_NODISCARD_CONSTEXPR auto end() {
      if constexpr(NonConstBaseType<Base>) {
         return Iterator<Base, ColFlag<Base>>{A_, A_.cols()};
      } else {
         return ConstIterator<Base, ConstColFlag<Base>>{A_, A_.cols()};
      }
   }

   STRICT_NODISCARD_CONSTEXPR auto end() const {
      return ConstIterator<Base, ConstColFlag<Base>>{A_, A_.cols()};
   }

   STRICT_NODISCARD_CONSTEXPR auto cbegin() const {
      return this->begin();
   }

   STRICT_NODISCARD_CONSTEXPR auto cend() const {
      return this->end();
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() const {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() const {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crbegin() const {
      return std::reverse_iterator{this->rbegin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crend() const {
      return std::reverse_iterator{this->rend()};
   }

private:
   Base& A_;
};


}  // namespace slib
