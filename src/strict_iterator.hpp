// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <iterator>
#include <type_traits>

#include "strict_concepts.hpp"
#include "strict_error.hpp"

namespace strict_array {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
class iterator
{
public:
   using size_type = SizeTypeOf<DirectBaseT>;
   using difference_type = size_type;
   using value_type = ValueTypeOf<DirectBaseT>;
   using pointer = value_type*;
   using reference = value_type &;
   using iterator_category = std::random_access_iterator_tag;

   inline explicit iterator();
   inline explicit iterator(DirectBaseT & A, size_type pos);
   inline iterator & operator++();
   inline iterator & operator--();
   inline iterator operator++(int);
   inline iterator operator--(int);

   inline iterator & operator+=(difference_type incr);
   inline iterator & operator-=(difference_type incr);

   [[nodiscard]] inline iterator operator+(difference_type incr) const;
   [[nodiscard]] inline iterator operator-(difference_type incr) const;

   [[nodiscard]] inline difference_type operator-(const iterator & it) const;

   // * and [] operators return reference rather than decltype(auto), since
   // non-const iterators apply only to types that point to the actual memory,
   // not expression templates or slices of expression templates.
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
   DirectBaseT* A_ptr;
   size_type pos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
[[nodiscard]] inline iterator<DirectBaseT> operator+(typename iterator<DirectBaseT>::difference_type incr, const iterator<DirectBaseT> & it)
{ return it + incr; }

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline iterator<DirectBaseT> operator-(typename iterator<DirectBaseT>::difference_type incr, const iterator<DirectBaseT> & it)
{ return it + incr; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT>::iterator() : A_ptr{nullptr}, pos{0}
{}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT>::iterator(DirectBaseT & A, size_type pos) :
   A_ptr{&A},
   pos{pos}
{
   ASSERT_STRICT_DEBUG(pos > -1 && pos <= (*A_ptr).size()); // iterator to one past the last is allowed
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> & iterator<DirectBaseT>::operator++()
{
   ++pos;
   return *this;
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> & iterator<DirectBaseT>::operator--()
{
   --pos;
   return *this;
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> iterator<DirectBaseT>::operator++(int)
{
   auto old = *this;
   ++*this;
   return old;
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> iterator<DirectBaseT>::operator--(int)
{
   auto old = *this;
   --*this;
   return old;
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> & iterator<DirectBaseT>::operator+=(difference_type incr)
{
   pos += incr;
   return *this;
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> & iterator<DirectBaseT>::operator-=(difference_type incr)
{
   pos -= incr;
   return *this;
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> iterator<DirectBaseT>::operator+(difference_type incr) const
{
   auto new_it = *this;
   new_it += incr;
   return new_it;
}

template<DirectBaseType DirectBaseT>
inline iterator<DirectBaseT> iterator<DirectBaseT>::operator-(difference_type incr) const
{
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}

template<DirectBaseType DirectBaseT>
inline auto iterator<DirectBaseT>::operator-(const iterator & it) const -> difference_type
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos - it.pos;
}

template<DirectBaseType DirectBaseT>
inline auto iterator<DirectBaseT>::operator[](difference_type n) const -> reference
{
   #ifndef STRICT_DEBUG_OFF
   if(pos + n < 0 || pos + n > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos + n];
}

template<DirectBaseType DirectBaseT>
inline auto iterator<DirectBaseT>::operator*() const -> reference
{
   #ifndef STRICT_DEBUG_OFF
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos];
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto iterator<DirectBaseT>::operator->() const -> pointer
{
   #ifndef STRICT_DEBUG_OFF
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return &((*A_ptr)[pos]);
}

template<DirectBaseType DirectBaseT>
inline bool iterator<DirectBaseT>::operator==(const iterator<DirectBaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos == it.pos;
}

template<DirectBaseType DirectBaseT>
inline bool iterator<DirectBaseT>::operator!=(const iterator<DirectBaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos != it.pos;
}

template<DirectBaseType DirectBaseT>
inline bool iterator<DirectBaseT>::operator<(const iterator<DirectBaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos < it.pos;
}

template<DirectBaseType DirectBaseT>
inline bool iterator<DirectBaseT>::operator<=(const iterator<DirectBaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos <= it.pos;
}

template<DirectBaseType DirectBaseT>
inline bool iterator<DirectBaseT>::operator>(const iterator<DirectBaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos > it.pos;
}

template<DirectBaseType DirectBaseT>
inline bool iterator<DirectBaseT>::operator>=(const iterator<DirectBaseT> & it) const
{
   ASSERT_STRICT_DEBUG(A_ptr == it.A_ptr);
   return pos >= it.pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class const_iterator
{
public:
   using size_type = SizeTypeOf<BaseT>;
   using difference_type = size_type;
   using value_type = ValueTypeOf<BaseT>;
   using pointer = value_type*;
   using const_pointer = const value_type*;
   using reference = value_type &;
   using const_reference = const value_type &;
   using iterator_category = std::random_access_iterator_tag;

   inline explicit const_iterator();
   inline explicit const_iterator(const BaseT & A, size_type pos);
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
   [[nodiscard]] inline const_pointer operator->() const;

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
   ASSERT_STRICT_DEBUG(pos > -1 && pos <= (*A_ptr).size()); // iterator to one past the last is allowed
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
   #ifndef STRICT_DEBUG_OFF
   if(pos + n < 0 || pos + n > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos + n];
}

template<BaseType BaseT>
inline decltype(auto) const_iterator<BaseT>::operator*() const
{
   #ifndef STRICT_DEBUG_OFF
   if(pos < 0 || pos > (*A_ptr).size()-1) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return (*A_ptr)[pos];
}

template<BaseType BaseT>
[[nodiscard]] inline auto const_iterator<BaseT>::operator->() const -> const_pointer
{
   static_assert(std::is_lvalue_reference_v<decltype((*A_ptr)[0])>,
         "CANNOT ASSIGN ADDRESS OF ELEMENTS OF EXPRESSION TEMPLATES OR SLICES OF EXPRESSION TEMPLATES TO A POINTER");
   #ifndef STRICT_DEBUG_OFF
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
