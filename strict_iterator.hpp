// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include "strict_concepts.hpp"
#include "strict_error.hpp"

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class iterator
{
public:
   using size_type = typename BaseT::size_type;
   using difference_type = size_type;
   using value_type = typename BaseT::value_type;
   using pointer = value_type*;
   using reference = value_type &;
   using iterator_category = std::random_access_iterator_tag;

   inline iterator();
   inline iterator(BaseT & A, size_type pos);
   inline iterator & operator++();
   inline iterator & operator--();
   inline iterator operator++(int);
   inline iterator operator--(int);

   inline iterator & operator+=(difference_type incr);
   inline iterator & operator-=(difference_type incr);

   [[nodiscard]] inline iterator operator+(difference_type incr) const;
   [[nodiscard]] inline iterator operator-(difference_type incr) const;

   [[nodiscard]] inline difference_type operator-(const iterator & it) const;

   [[nodiscard]] inline reference operator[](difference_type n) const;
   [[nodiscard]] inline reference operator*() const;
   [[nodiscard]] inline pointer operator->() const;

   [[nodiscard]] inline bool operator==(const iterator & it) const;
   [[nodiscard]] inline bool operator!=(const iterator & it) const;
   [[nodiscard]] inline bool operator<(const iterator & it) const;
   [[nodiscard]] inline bool operator<=(const iterator & it) const;
   [[nodiscard]] inline bool operator>(const iterator & it) const;
   [[nodiscard]] inline bool operator>=(const iterator & it) const;

private:
   BaseT* A_ptr;
   size_type pos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
[[nodiscard]] inline iterator<BaseT> operator+(typename iterator<BaseT>::difference_type incr, const iterator<BaseT> & it)
{ return it + incr; }

template<BaseType BaseT>
[[nodiscard]] inline iterator<BaseT> operator-(typename iterator<BaseT>::difference_type incr, const iterator<BaseT> & it)
{ return it + incr; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline iterator<BaseT>::iterator() : A_ptr{nullptr}, pos{0}
{}

template<BaseType BaseT>
inline iterator<BaseT>::iterator(BaseT & A, size_type pos) :
   A_ptr{&A},
   pos{pos}
{
   ASSERT_STRICT_DEBUG(pos > -1 && pos <= (*A_ptr).size());
}

template<BaseType BaseT>
inline iterator<BaseT> & iterator<BaseT>::operator++()
{
   ++pos;
   return *this;
}

template<BaseType BaseT>
inline iterator<BaseT> & iterator<BaseT>::operator--()
{
   --pos;
   return *this;
}

template<BaseType BaseT>
inline iterator<BaseT> iterator<BaseT>::operator++(int)
{
   auto old = *this;
   ++*this;
   return old;
}

template<BaseType BaseT>
inline iterator<BaseT> iterator<BaseT>::operator--(int)
{
   auto old = *this;
   --*this;
   return old;
}

template<BaseType BaseT>
inline iterator<BaseT> & iterator<BaseT>::operator+=(difference_type incr)
{
   pos += incr;
   return *this;
}

template<BaseType BaseT>
inline iterator<BaseT> & iterator<BaseT>::operator-=(difference_type incr)
{
   pos -= incr;
   return *this;
}

template<BaseType BaseT>
inline iterator<BaseT> iterator<BaseT>::operator+(difference_type incr) const
{
   auto new_it = *this;
   new_it += incr;
   return new_it;
}

template<BaseType BaseT>
inline iterator<BaseT> iterator<BaseT>::operator-(difference_type incr) const
{
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}

template<BaseType BaseT>
inline auto iterator<BaseT>::operator-(const iterator & it) const -> difference_type
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos - it.pos;
}

template<BaseType BaseT>
inline auto iterator<BaseT>::operator[](difference_type n) const -> reference
{
   #ifdef STRICT_DEBUG_ON
   if(pos + n < 0 || pos + n > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos + n];
}

template<BaseType BaseT>
inline auto iterator<BaseT>::operator*() const -> reference
{
   #ifdef STRICT_DEBUG_ON
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos];
}

template<BaseType BaseT>
[[nodiscard]] inline auto iterator<BaseT>::operator->() const -> pointer
{
   #ifdef STRICT_DEBUG_ON
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return &((*A_ptr)[pos]);
}

template<BaseType BaseT>
inline bool iterator<BaseT>::operator==(const iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos == it.pos;
}

template<BaseType BaseT>
inline bool iterator<BaseT>::operator!=(const iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos != it.pos;
}

template<BaseType BaseT>
inline bool iterator<BaseT>::operator<(const iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos < it.pos;
}

template<BaseType BaseT>
inline bool iterator<BaseT>::operator<=(const iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos <= it.pos;
}

template<BaseType BaseT>
inline bool iterator<BaseT>::operator>(const iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos > it.pos;
}

template<BaseType BaseT>
inline bool iterator<BaseT>::operator>=(const iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos >= it.pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class const_iterator
{
public:
   using size_type = typename BaseT::size_type;
   using difference_type = size_type;
   using value_type = typename BaseT::value_type;
   using pointer = const value_type*;
   using reference = const value_type &;
   using iterator_category = std::random_access_iterator_tag;

   inline const_iterator();
   inline const_iterator(const BaseT & A, size_type pos);
   inline const_iterator & operator++();
   inline const_iterator & operator--();
   inline const_iterator operator++(int);
   inline const_iterator operator--(int);

   inline const_iterator & operator+=(difference_type incr);
   inline const_iterator & operator-=(difference_type incr);

   [[nodiscard]] inline const_iterator operator+(difference_type incr) const;
   [[nodiscard]] inline const_iterator operator-(difference_type incr) const;
   [[nodiscard]] inline difference_type operator-(const const_iterator & it) const;

   [[nodiscard]] inline decltype(auto) operator[](difference_type n) const;
   [[nodiscard]] inline decltype(auto) operator*() const;
   [[nodiscard]] inline pointer operator->() const;

   [[nodiscard]] inline bool operator==(const const_iterator & it) const;
   [[nodiscard]] inline bool operator!=(const const_iterator & it) const;
   [[nodiscard]] inline bool operator<(const const_iterator & it) const;
   [[nodiscard]] inline bool operator<=(const const_iterator & it) const;
   [[nodiscard]] inline bool operator>(const const_iterator & it) const;
   [[nodiscard]] inline bool operator>=(const const_iterator & it) const;

private:
   const BaseT* A_ptr;
   size_type pos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
[[nodiscard]] inline const_iterator<BaseT> operator+(typename const_iterator<BaseT>::difference_type incr, const const_iterator<BaseT> & it)
{ return it + incr; }

template<BaseType BaseT>
[[nodiscard]] inline const_iterator<BaseT> operator-(typename const_iterator<BaseT>::difference_type incr, const const_iterator<BaseT> & it)
{ return it + incr; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline const_iterator<BaseT>::const_iterator() : A_ptr{nullptr}, pos{0}
{}

template<BaseType BaseT>
inline const_iterator<BaseT>::const_iterator(const BaseT & A, size_type pos) :
   A_ptr{&A},
   pos{pos}
{
   ASSERT_STRICT_DEBUG(pos > -1 && pos <= (*A_ptr).size());
}

template<BaseType BaseT>
inline const_iterator<BaseT> & const_iterator<BaseT>::operator++()
{
   ++pos;
   return *this;
}

template<BaseType BaseT>
inline const_iterator<BaseT> & const_iterator<BaseT>::operator--()
{
   --pos;
   return *this;
}

template<BaseType BaseT>
inline const_iterator<BaseT> const_iterator<BaseT>::operator++(int)
{
   auto old = *this;
   ++*this;
   return old;
}

template<BaseType BaseT>
inline const_iterator<BaseT> const_iterator<BaseT>::operator--(int)
{
   auto old = *this;
   --*this;
   return old;
}

template<BaseType BaseT>
inline const_iterator<BaseT> & const_iterator<BaseT>::operator+=(difference_type incr)
{
   pos += incr;
   return *this;
}

template<BaseType BaseT>
inline const_iterator<BaseT> & const_iterator<BaseT>::operator-=(difference_type incr)
{
   pos -= incr;
   return *this;
}

template<BaseType BaseT>
inline const_iterator<BaseT> const_iterator<BaseT>::operator+(difference_type incr) const
{
   auto new_it = *this;
   new_it += incr;
   return new_it;
}

template<BaseType BaseT>
inline const_iterator<BaseT> const_iterator<BaseT>::operator-(difference_type incr) const
{
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}

template<BaseType BaseT>
inline auto const_iterator<BaseT>::operator-(const const_iterator & it) const -> difference_type
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos - it.pos;
}

template<BaseType BaseT>
inline decltype(auto) const_iterator<BaseT>::operator[](difference_type n) const
{
   #ifdef STRICT_DEBUG_ON
   if(pos + n < 0 || pos + n > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos + n];
}

template<BaseType BaseT>
inline decltype(auto) const_iterator<BaseT>::operator*() const
{
   #ifdef STRICT_DEBUG_ON
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos];
}

template<BaseType BaseT>
[[nodiscard]] inline auto const_iterator<BaseT>::operator->() const -> pointer
{
   #ifdef STRICT_DEBUG_ON
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return &((*A_ptr)[pos]);
}

template<BaseType BaseT>
inline bool const_iterator<BaseT>::operator==(const const_iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos == it.pos;
}

template<BaseType BaseT>
inline bool const_iterator<BaseT>::operator!=(const const_iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos != it.pos;
}

template<BaseType BaseT>
inline bool const_iterator<BaseT>::operator<(const const_iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos < it.pos;
}

template<BaseType BaseT>
inline bool const_iterator<BaseT>::operator<=(const const_iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos <= it.pos;
}

template<BaseType BaseT>
inline bool const_iterator<BaseT>::operator>(const const_iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos > it.pos;
}

template<BaseType BaseT>
inline bool const_iterator<BaseT>::operator>=(const const_iterator<BaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos >= it.pos;
}

}

#endif
