#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include "strict_error.hpp"
#include "strict_concepts.hpp"

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
class iterator
{
public:
   using size_type = typename ArrayType::size_type;
   using difference_type = size_type;
   using value_type = typename ArrayType::value_type;
   using pointer = value_type*;
   using reference = value_type &;
   using iterator_category = std::random_access_iterator_tag;

   inline iterator();
   inline iterator(ArrayType & A, size_type pos);
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
   ArrayType* A_ptr;
   size_type pos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
[[nodiscard]] inline iterator<ArrayType> operator+(typename iterator<ArrayType>::difference_type incr, const iterator<ArrayType> & it)
{ return it + incr; }

template<ArrayBaseType ArrayType>
[[nodiscard]] inline iterator<ArrayType> operator-(typename iterator<ArrayType>::difference_type incr, const iterator<ArrayType> & it)
{ return it + incr; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
inline iterator<ArrayType>::iterator() : A_ptr{nullptr}, pos{0}
{}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType>::iterator(ArrayType & A, size_type pos) :
   A_ptr{&A},
   pos{pos}
{
   ASSERT_STRICT_DEBUG(pos > -1 && pos <= (*A_ptr).size());
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> & iterator<ArrayType>::operator++()
{
   ++pos;
   return *this;
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> & iterator<ArrayType>::operator--()
{
   --pos;
   return *this;
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> iterator<ArrayType>::operator++(int)
{
   auto old = *this;
   ++*this;
   return old;
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> iterator<ArrayType>::operator--(int)
{
   auto old = *this;
   --*this;
   return old;
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> & iterator<ArrayType>::operator+=(difference_type incr)
{
   pos += incr;
   return *this;
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> & iterator<ArrayType>::operator-=(difference_type incr)
{
   pos -= incr;
   return *this;
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> iterator<ArrayType>::operator+(difference_type incr) const
{
   auto new_it = *this;
   new_it += incr;
   return new_it;
}

template<ArrayBaseType ArrayType>
inline iterator<ArrayType> iterator<ArrayType>::operator-(difference_type incr) const
{
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}

template<ArrayBaseType ArrayType>
inline auto iterator<ArrayType>::operator-(const iterator & it) const -> difference_type
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos - it.pos;
}

template<ArrayBaseType ArrayType>
inline auto iterator<ArrayType>::operator[](difference_type n) const -> reference
{
   #ifdef STRICT_DEBUG_ON
   if(pos + n < 0 || pos + n > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos + n];
}

template<ArrayBaseType ArrayType>
inline auto iterator<ArrayType>::operator*() const -> reference
{
   #ifdef STRICT_DEBUG_ON
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos];
}

template<ArrayBaseType ArrayType>
[[nodiscard]] inline auto iterator<ArrayType>::operator->() const -> pointer
{
   return &((*A_ptr)[pos]);
}

template<ArrayBaseType ArrayType>
inline bool iterator<ArrayType>::operator==(const iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos == it.pos;
}

template<ArrayBaseType ArrayType>
inline bool iterator<ArrayType>::operator!=(const iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos != it.pos;
}

template<ArrayBaseType ArrayType>
inline bool iterator<ArrayType>::operator<(const iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos < it.pos;
}

template<ArrayBaseType ArrayType>
inline bool iterator<ArrayType>::operator<=(const iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos <= it.pos;
}

template<ArrayBaseType ArrayType>
inline bool iterator<ArrayType>::operator>(const iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos > it.pos;
}

template<ArrayBaseType ArrayType>
inline bool iterator<ArrayType>::operator>=(const iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos >= it.pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
class const_iterator
{
public:
   using size_type = typename ArrayType::size_type;
   using difference_type = size_type;
   using value_type = typename ArrayType::value_type;
   using pointer = value_type*;
   using reference = value_type &;
   using iterator_category = std::random_access_iterator_tag;

   inline const_iterator();
   inline const_iterator(const ArrayType & A, size_type pos);
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
   const ArrayType* A_ptr;
   size_type pos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
[[nodiscard]] inline const_iterator<ArrayType> operator+(typename const_iterator<ArrayType>::difference_type incr, const const_iterator<ArrayType> & it)
{ return it + incr; }

template<ArrayBaseType ArrayType>
[[nodiscard]] inline const_iterator<ArrayType> operator-(typename const_iterator<ArrayType>::difference_type incr, const const_iterator<ArrayType> & it)
{ return it + incr; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType>::const_iterator() : A_ptr{nullptr}, pos{0}
{}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType>::const_iterator(const ArrayType & A, size_type pos) :
   A_ptr{&A},
   pos{pos}
{
   ASSERT_STRICT_DEBUG(pos > -1 && pos <= (*A_ptr).size());
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> & const_iterator<ArrayType>::operator++()
{
   ++pos;
   return *this;
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> & const_iterator<ArrayType>::operator--()
{
   --pos;
   return *this;
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> const_iterator<ArrayType>::operator++(int)
{
   auto old = *this;
   ++*this;
   return old;
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> const_iterator<ArrayType>::operator--(int)
{
   auto old = *this;
   --*this;
   return old;
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> & const_iterator<ArrayType>::operator+=(difference_type incr)
{
   pos += incr;
   return *this;
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> & const_iterator<ArrayType>::operator-=(difference_type incr)
{
   pos -= incr;
   return *this;
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> const_iterator<ArrayType>::operator+(difference_type incr) const
{
   auto new_it = *this;
   new_it += incr;
   return new_it;
}

template<ArrayBaseType ArrayType>
inline const_iterator<ArrayType> const_iterator<ArrayType>::operator-(difference_type incr) const
{
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}

template<ArrayBaseType ArrayType>
inline auto const_iterator<ArrayType>::operator-(const const_iterator & it) const -> difference_type
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos - it.pos;
}

template<ArrayBaseType ArrayType>
inline decltype(auto) const_iterator<ArrayType>::operator[](difference_type n) const
{
   #ifdef STRICT_DEBUG_ON
   if(pos + n < 0 || pos + n > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos + n];
}

template<ArrayBaseType ArrayType>
inline decltype(auto) const_iterator<ArrayType>::operator*() const
{
   #ifdef STRICT_DEBUG_ON
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos];
}

template<ArrayBaseType ArrayType>
[[nodiscard]] inline auto const_iterator<ArrayType>::operator->() const -> pointer
{
   return &((*A_ptr)[pos]);
}

template<ArrayBaseType ArrayType>
inline bool const_iterator<ArrayType>::operator==(const const_iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos == it.pos;
}

template<ArrayBaseType ArrayType>
inline bool const_iterator<ArrayType>::operator!=(const const_iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos != it.pos;
}

template<ArrayBaseType ArrayType>
inline bool const_iterator<ArrayType>::operator<(const const_iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos < it.pos;
}

template<ArrayBaseType ArrayType>
inline bool const_iterator<ArrayType>::operator<=(const const_iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos <= it.pos;
}

template<ArrayBaseType ArrayType>
inline bool const_iterator<ArrayType>::operator>(const const_iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos > it.pos;
}

template<ArrayBaseType ArrayType>
inline bool const_iterator<ArrayType>::operator>=(const const_iterator<ArrayType> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos >= it.pos;
}

}

#endif
