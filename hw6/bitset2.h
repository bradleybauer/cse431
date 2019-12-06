// <bitset> -*- C++ -*-

// Copyright (C) 2001-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 * Copyright (c) 1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file include/bitset
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_BITSET
#define _GLIBCXX_BITSET 1

#pragma GCC system_header

#include <string>
#include <bits/functexcept.h>   // For invalid_argument, out_of_range,
                                // overflow_error
#include <iosfwd>
#include <bits/cxxabi_forced.h>

#if __cplusplus >= 201103L
# include <bits/functional_hash.h>
#endif

#define _GLIBCXX_BITSET_BITS_PER_WORD  (__CHAR_BIT__ * __SIZEOF_LONG__)
#define _GLIBCXX_BITSET_WORDS(__n) \
  ((__n) / _GLIBCXX_BITSET_BITS_PER_WORD + \
   ((__n) % _GLIBCXX_BITSET_BITS_PER_WORD == 0 ? 0 : 1))

#define _GLIBCXX_BITSET_BITS_PER_ULL (__CHAR_BIT__ * __SIZEOF_LONG_LONG__)

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  /**
   *  Base class, general case.  It is a class invariant that _Nw will be
   *  nonnegative.
   *
   *  See documentation for bitset.
  */
  template<size_t _Nw>
    struct _Base_bitset
    {
      typedef unsigned long _WordT;

      /// 0 is the least significant word.
      _WordT 		_M_w[_Nw];

      _GLIBCXX_CONSTEXPR _Base_bitset() _GLIBCXX_NOEXCEPT
      : _M_w() { }

#if __cplusplus >= 201103L
      constexpr _Base_bitset(unsigned long long __val) noexcept
      : _M_w{ _WordT(__val)
#if __SIZEOF_LONG_LONG__ > __SIZEOF_LONG__
	       , _WordT(__val >> _GLIBCXX_BITSET_BITS_PER_WORD)
#endif
      } { }
#else
      _Base_bitset(unsigned long __val)
      : _M_w()
      { _M_w[0] = __val; }
#endif

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichword(size_t __pos) _GLIBCXX_NOEXCEPT
      { return __pos / _GLIBCXX_BITSET_BITS_PER_WORD; }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichbyte(size_t __pos) _GLIBCXX_NOEXCEPT
      { return (__pos % _GLIBCXX_BITSET_BITS_PER_WORD) / __CHAR_BIT__; }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichbit(size_t __pos) _GLIBCXX_NOEXCEPT
      { return __pos % _GLIBCXX_BITSET_BITS_PER_WORD; }

      static _GLIBCXX_CONSTEXPR _WordT
      _S_maskbit(size_t __pos) _GLIBCXX_NOEXCEPT
      { return (static_cast<_WordT>(1)) << _S_whichbit(__pos); }

      _WordT&
      _M_getword(size_t __pos) _GLIBCXX_NOEXCEPT
      { return _M_w[_S_whichword(__pos)]; }

      _GLIBCXX_CONSTEXPR _WordT
      _M_getword(size_t __pos) const _GLIBCXX_NOEXCEPT
      { return _M_w[_S_whichword(__pos)]; }

#if __cplusplus >= 201103L
      const _WordT*
      _M_getdata() const noexcept
      { return _M_w; }
#endif

      _WordT&
      _M_hiword() _GLIBCXX_NOEXCEPT
      { return _M_w[_Nw - 1]; }

      _GLIBCXX_CONSTEXPR _WordT
      _M_hiword() const _GLIBCXX_NOEXCEPT
      { return _M_w[_Nw - 1]; }

      size_t
      _M_do_and_count(const _Base_bitset<_Nw>& __x) const _GLIBCXX_NOEXCEPT
      {
        size_t __result = 0;
        for (size_t __i = 0; __i < _Nw; __i++)
            __result += __builtin_popcountl(_M_w[__i] & __x._M_w[__i]);
        return __result;
      }

      void
      _M_do_and(const _Base_bitset<_Nw>& __x) _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++)
	  _M_w[__i] &= __x._M_w[__i];
      }

      void
      _M_do_or(const _Base_bitset<_Nw>& __x) _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++)
	  _M_w[__i] |= __x._M_w[__i];
      }

      void
      _M_do_xor(const _Base_bitset<_Nw>& __x) _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++)
	  _M_w[__i] ^= __x._M_w[__i];
      }

      void
      _M_do_add(const int& __x) _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++) {
	  if (_M_w[__i] == -1) {
	    _M_w[__i] = 0;
	  } else {
	    _M_w[__i] += 1;
	    break;
	  }
	}
      }

      void
      _M_do_sub(const int& __x) _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++) {
	  if (_M_w[__i] == 0) {
	    _M_w[__i] = -1;
	  } else {
	    _M_w[__i] -= 1;
	    break;
	  }
	}
      }

      size_t
      _M_do_first_shared(const _Base_bitset<_Nw>& __x) const _GLIBCXX_NOEXCEPT
      {
      	for (size_t __i = 0; __i < _Nw; __i++) {
      	  _WordT __and = _M_w[__i] & __x._M_w[__i];
      	  if (__and != static_cast<_WordT>(0))
      	    return (__i * _GLIBCXX_BITSET_BITS_PER_WORD
      		    + __builtin_ctzl(__and));
      	}
      	return _Nw * _GLIBCXX_BITSET_BITS_PER_WORD;
      }


      void
      _M_do_left_shift(size_t __shift) _GLIBCXX_NOEXCEPT;

      void
      _M_do_right_shift(size_t __shift) _GLIBCXX_NOEXCEPT;

      void
      _M_do_flip() _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++)
	  _M_w[__i] = ~_M_w[__i];
      }

      void
      _M_do_set() _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++)
	  _M_w[__i] = ~static_cast<_WordT>(0);
      }

      void
      _M_do_reset() _GLIBCXX_NOEXCEPT
      { __builtin_memset(_M_w, 0, _Nw * sizeof(_WordT)); }

      bool
      _M_is_equal(const _Base_bitset<_Nw>& __x) const _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; ++__i)
	  if (_M_w[__i] != __x._M_w[__i])
	    return false;
	return true;
      }

      template<size_t _Nb>
        bool
        _M_are_all() const _GLIBCXX_NOEXCEPT
        {
	  for (size_t __i = 0; __i < _Nw - 1; __i++)
	    if (_M_w[__i] != ~static_cast<_WordT>(0))
	      return false;
	  return _M_hiword() == (~static_cast<_WordT>(0)
				 >> (_Nw * _GLIBCXX_BITSET_BITS_PER_WORD
				     - _Nb));
	}

      bool
      _M_is_any() const _GLIBCXX_NOEXCEPT
      {
	for (size_t __i = 0; __i < _Nw; __i++)
	  if (_M_w[__i] != static_cast<_WordT>(0))
	    return true;
	return false;
      }

      size_t
      _M_do_count() const _GLIBCXX_NOEXCEPT
      {
	size_t __result = 0;
	for (size_t __i = 0; __i < _Nw; __i++)
	  __result += __builtin_popcountl(_M_w[__i]);
	return __result;
      }

      unsigned long
      _M_do_to_ulong() const;

#if __cplusplus >= 201103L
      unsigned long long
      _M_do_to_ullong() const;
#endif

      // find first "on" bit
      size_t
      _M_do_find_first(size_t) const _GLIBCXX_NOEXCEPT;

      // find first "on" bit
      size_t
      _M_do_find_first(const _Base_bitset<_Nw>&, size_t) const _GLIBCXX_NOEXCEPT;

      // find the next "on" bit that follows "prev"
      size_t
      _M_do_find_next(size_t, size_t) const _GLIBCXX_NOEXCEPT;

      size_t
      _M_do_find_next(const _Base_bitset<_Nw>&, size_t, size_t) const _GLIBCXX_NOEXCEPT;
    };

  // Definitions of non-inline functions from _Base_bitset.
  template<size_t _Nw>
    void
    _Base_bitset<_Nw>::_M_do_left_shift(size_t __shift) _GLIBCXX_NOEXCEPT
    {
      if (__builtin_expect(__shift != 0, 1))
	{
	  const size_t __wshift = __shift / _GLIBCXX_BITSET_BITS_PER_WORD;
	  const size_t __offset = __shift % _GLIBCXX_BITSET_BITS_PER_WORD;

	  if (__offset == 0)
	    for (size_t __n = _Nw - 1; __n >= __wshift; --__n)
	      _M_w[__n] = _M_w[__n - __wshift];
	  else
	    {
	      const size_t __sub_offset = (_GLIBCXX_BITSET_BITS_PER_WORD
					   - __offset);
	      for (size_t __n = _Nw - 1; __n > __wshift; --__n)
		_M_w[__n] = ((_M_w[__n - __wshift] << __offset)
			     | (_M_w[__n - __wshift - 1] >> __sub_offset));
	      _M_w[__wshift] = _M_w[0] << __offset;
	    }

	  std::fill(_M_w + 0, _M_w + __wshift, static_cast<_WordT>(0));
	}
    }

  template<size_t _Nw>
    void
    _Base_bitset<_Nw>::_M_do_right_shift(size_t __shift) _GLIBCXX_NOEXCEPT
    {
      if (__builtin_expect(__shift != 0, 1))
	{
	  const size_t __wshift = __shift / _GLIBCXX_BITSET_BITS_PER_WORD;
	  const size_t __offset = __shift % _GLIBCXX_BITSET_BITS_PER_WORD;
	  const size_t __limit = _Nw - __wshift - 1;

	  if (__offset == 0)
	    for (size_t __n = 0; __n <= __limit; ++__n)
	      _M_w[__n] = _M_w[__n + __wshift];
	  else
	    {
	      const size_t __sub_offset = (_GLIBCXX_BITSET_BITS_PER_WORD
					   - __offset);
	      for (size_t __n = 0; __n < __limit; ++__n)
		_M_w[__n] = ((_M_w[__n + __wshift] >> __offset)
			     | (_M_w[__n + __wshift + 1] << __sub_offset));
	      _M_w[__limit] = _M_w[_Nw-1] >> __offset;
	    }

	  std::fill(_M_w + __limit + 1, _M_w + _Nw, static_cast<_WordT>(0));
	}
    }

  template<size_t _Nw>
    unsigned long
    _Base_bitset<_Nw>::_M_do_to_ulong() const
    {
      for (size_t __i = 1; __i < _Nw; ++__i)
	if (_M_w[__i])
	  __throw_overflow_error(__N("_Base_bitset::_M_do_to_ulong"));
      return _M_w[0];
    }

#if __cplusplus >= 201103L
  template<size_t _Nw>
    unsigned long long
    _Base_bitset<_Nw>::_M_do_to_ullong() const
    {
      const bool __dw = sizeof(unsigned long long) > sizeof(unsigned long);
      for (size_t __i = 1 + __dw; __i < _Nw; ++__i)
	if (_M_w[__i])
	  __throw_overflow_error(__N("_Base_bitset::_M_do_to_ullong"));

      if (__dw)
	return _M_w[0] + (static_cast<unsigned long long>(_M_w[1])
			  << _GLIBCXX_BITSET_BITS_PER_WORD);
      return _M_w[0];
    }
#endif

  template<size_t _Nw>
    size_t
    _Base_bitset<_Nw>::
    _M_do_find_first(size_t __not_found) const _GLIBCXX_NOEXCEPT
    {
      for (size_t __i = 0; __i < _Nw; __i++)
	{
	  _WordT __thisword = _M_w[__i];
	  if (__thisword != static_cast<_WordT>(0))
	    return (__i * _GLIBCXX_BITSET_BITS_PER_WORD
		    + __builtin_ctzl(__thisword));
	}
      // not found, so return an indication of failure.
      return __not_found;
    }

  template<size_t _Nw>
    size_t
    _Base_bitset<_Nw>::
    _M_do_find_first(const _Base_bitset<_Nw>& __other, size_t __not_found) const _GLIBCXX_NOEXCEPT
    {
      for (size_t __i = 0; __i < _Nw; __i++)
	{
	  _WordT __thisword = _M_w[__i] & __other._M_w[__i];
	  if (__thisword != static_cast<_WordT>(0))
	    return (__i * _GLIBCXX_BITSET_BITS_PER_WORD
		    + __builtin_ctzl(__thisword));
	}
      // not found, so return an indication of failure.
      return __not_found;
    }

  template<size_t _Nw>
    size_t
    _Base_bitset<_Nw>::
    _M_do_find_next(size_t __prev, size_t __not_found) const _GLIBCXX_NOEXCEPT
    {
      // make bound inclusive
      ++__prev;

      // check out of bounds
      if (__prev >= _Nw * _GLIBCXX_BITSET_BITS_PER_WORD)
	return __not_found;

      // search first word
      size_t __i = _S_whichword(__prev);
      _WordT __thisword = _M_w[__i];

      // mask off bits below bound
      __thisword &= (~static_cast<_WordT>(0)) << _S_whichbit(__prev);

      if (__thisword != static_cast<_WordT>(0))
	return (__i * _GLIBCXX_BITSET_BITS_PER_WORD
		+ __builtin_ctzl(__thisword));

      // check subsequent words
      __i++;
      for (; __i < _Nw; __i++)
	{
	  __thisword = _M_w[__i];
	  if (__thisword != static_cast<_WordT>(0))
	    return (__i * _GLIBCXX_BITSET_BITS_PER_WORD
		    + __builtin_ctzl(__thisword));
	}
      // not found, so return an indication of failure.
      return __not_found;
    } // end _M_do_find_next

  template<size_t _Nw>
    size_t
    _Base_bitset<_Nw>::
    _M_do_find_next(const _Base_bitset<_Nw>& __other, size_t __prev, size_t __not_found) const _GLIBCXX_NOEXCEPT
    {
      // make bound inclusive
      ++__prev;

      // check out of bounds
      if (__prev >= _Nw * _GLIBCXX_BITSET_BITS_PER_WORD)
	return __not_found;

      // search first word
      size_t __i = _S_whichword(__prev);
      _WordT __thisword = _M_w[__i] & __other._M_w[__i];

      // mask off bits below bound
      __thisword &= (~static_cast<_WordT>(0)) << _S_whichbit(__prev);

      if (__thisword != static_cast<_WordT>(0))
	return (__i * _GLIBCXX_BITSET_BITS_PER_WORD
		+ __builtin_ctzl(__thisword));

      // check subsequent words
      __i++;
      for (; __i < _Nw; __i++)
	{
	  __thisword = _M_w[__i] & __other._M_w[__i];
	  if (__thisword != static_cast<_WordT>(0))
	    return (__i * _GLIBCXX_BITSET_BITS_PER_WORD
		    + __builtin_ctzl(__thisword));
	}
      // not found, so return an indication of failure.
      return __not_found;
    } // end _M_do_find_next

  /**
   *  Base class, specialization for a single word.
   *
   *  See documentation for bitset.
  */
  template<>
    struct _Base_bitset<1>
    {
      typedef unsigned long _WordT;
      _WordT _M_w;

      _GLIBCXX_CONSTEXPR _Base_bitset() _GLIBCXX_NOEXCEPT
      : _M_w(0)
      { }

#if __cplusplus >= 201103L
      constexpr _Base_bitset(unsigned long long __val) noexcept
#else
      _Base_bitset(unsigned long __val)
#endif
      : _M_w(__val)
      { }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichword(size_t __pos) _GLIBCXX_NOEXCEPT
      { return __pos / _GLIBCXX_BITSET_BITS_PER_WORD; }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichbyte(size_t __pos) _GLIBCXX_NOEXCEPT
      { return (__pos % _GLIBCXX_BITSET_BITS_PER_WORD) / __CHAR_BIT__; }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichbit(size_t __pos) _GLIBCXX_NOEXCEPT
      {  return __pos % _GLIBCXX_BITSET_BITS_PER_WORD; }

      static _GLIBCXX_CONSTEXPR _WordT
      _S_maskbit(size_t __pos) _GLIBCXX_NOEXCEPT
      { return (static_cast<_WordT>(1)) << _S_whichbit(__pos); }

      _WordT&
      _M_getword(size_t) _GLIBCXX_NOEXCEPT
      { return _M_w; }

      _GLIBCXX_CONSTEXPR _WordT
      _M_getword(size_t) const _GLIBCXX_NOEXCEPT
      { return _M_w; }

#if __cplusplus >= 201103L
      const _WordT*
      _M_getdata() const noexcept
      { return &_M_w; }
#endif

      _WordT&
      _M_hiword() _GLIBCXX_NOEXCEPT
      { return _M_w; }

      _GLIBCXX_CONSTEXPR _WordT
      _M_hiword() const _GLIBCXX_NOEXCEPT
      { return _M_w; }

      void
      _M_do_and(const _Base_bitset<1>& __x) _GLIBCXX_NOEXCEPT
      { _M_w &= __x._M_w; }

      void
      _M_do_or(const _Base_bitset<1>& __x) _GLIBCXX_NOEXCEPT
      { _M_w |= __x._M_w; }

      void
      _M_do_xor(const _Base_bitset<1>& __x) _GLIBCXX_NOEXCEPT
      { _M_w ^= __x._M_w; }

      void
      _M_do_left_shift(size_t __shift) _GLIBCXX_NOEXCEPT
      { _M_w <<= __shift; }

      void
      _M_do_right_shift(size_t __shift) _GLIBCXX_NOEXCEPT
      { _M_w >>= __shift; }

      void
      _M_do_flip() _GLIBCXX_NOEXCEPT
      { _M_w = ~_M_w; }

      void
      _M_do_set() _GLIBCXX_NOEXCEPT
      { _M_w = ~static_cast<_WordT>(0); }

      void
      _M_do_reset() _GLIBCXX_NOEXCEPT
      { _M_w = 0; }

      bool
      _M_is_equal(const _Base_bitset<1>& __x) const _GLIBCXX_NOEXCEPT
      { return _M_w == __x._M_w; }

      template<size_t _Nb>
        bool
        _M_are_all() const _GLIBCXX_NOEXCEPT
        { return _M_w == (~static_cast<_WordT>(0)
			  >> (_GLIBCXX_BITSET_BITS_PER_WORD - _Nb)); }

      bool
      _M_is_any() const _GLIBCXX_NOEXCEPT
      { return _M_w != 0; }

      size_t
      _M_do_count() const _GLIBCXX_NOEXCEPT
      { return __builtin_popcountl(_M_w); }

      unsigned long
      _M_do_to_ulong() const _GLIBCXX_NOEXCEPT
      { return _M_w; }

#if __cplusplus >= 201103L
      unsigned long long
      _M_do_to_ullong() const noexcept
      { return _M_w; }
#endif

      size_t
      _M_do_find_first(size_t __not_found) const _GLIBCXX_NOEXCEPT
      {
        if (_M_w != 0)
          return __builtin_ctzl(_M_w);
        else
          return __not_found;
      }

      // find the next "on" bit that follows "prev"
      size_t
      _M_do_find_next(size_t __prev, size_t __not_found) const
	_GLIBCXX_NOEXCEPT
      {
	++__prev;
	if (__prev >= ((size_t) _GLIBCXX_BITSET_BITS_PER_WORD))
	  return __not_found;

	_WordT __x = _M_w >> __prev;
	if (__x != 0)
	  return __builtin_ctzl(__x) + __prev;
	else
	  return __not_found;
      }
    };

  /**
   *  Base class, specialization for no storage (zero-length %bitset).
   *
   *  See documentation for bitset.
  */
  template<>
    struct _Base_bitset<0>
    {
      typedef unsigned long _WordT;

      _GLIBCXX_CONSTEXPR _Base_bitset() _GLIBCXX_NOEXCEPT
      { }

#if __cplusplus >= 201103L
      constexpr _Base_bitset(unsigned long long) noexcept
#else
      _Base_bitset(unsigned long)
#endif
      { }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichword(size_t __pos) _GLIBCXX_NOEXCEPT
      { return __pos / _GLIBCXX_BITSET_BITS_PER_WORD; }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichbyte(size_t __pos) _GLIBCXX_NOEXCEPT
      { return (__pos % _GLIBCXX_BITSET_BITS_PER_WORD) / __CHAR_BIT__; }

      static _GLIBCXX_CONSTEXPR size_t
      _S_whichbit(size_t __pos) _GLIBCXX_NOEXCEPT
      {  return __pos % _GLIBCXX_BITSET_BITS_PER_WORD; }

      static _GLIBCXX_CONSTEXPR _WordT
      _S_maskbit(size_t __pos) _GLIBCXX_NOEXCEPT
      { return (static_cast<_WordT>(1)) << _S_whichbit(__pos); }

      // This would normally give access to the data.  The bounds-checking
      // in the bitset class will prevent the user from getting this far,
      // but (1) it must still return an lvalue to compile, and (2) the
      // user might call _Unchecked_set directly, in which case this /needs/
      // to fail.  Let's not penalize zero-length users unless they actually
      // make an unchecked call; all the memory ugliness is therefore
      // localized to this single should-never-get-this-far function.
      _WordT&
      _M_getword(size_t) _GLIBCXX_NOEXCEPT
      {
	__throw_out_of_range(__N("_Base_bitset::_M_getword"));
	return *new _WordT;
      }

      _GLIBCXX_CONSTEXPR _WordT
      _M_getword(size_t) const _GLIBCXX_NOEXCEPT
      { return 0; }

      _GLIBCXX_CONSTEXPR _WordT
      _M_hiword() const _GLIBCXX_NOEXCEPT
      { return 0; }

      void
      _M_do_and(const _Base_bitset<0>&) _GLIBCXX_NOEXCEPT
      { }

      void
      _M_do_or(const _Base_bitset<0>&) _GLIBCXX_NOEXCEPT
      { }

      void
      _M_do_xor(const _Base_bitset<0>&) _GLIBCXX_NOEXCEPT
      { }

      void
      _M_do_left_shift(size_t) _GLIBCXX_NOEXCEPT
      { }

      void
      _M_do_right_shift(size_t) _GLIBCXX_NOEXCEPT
      { }

      void
      _M_do_flip() _GLIBCXX_NOEXCEPT
      { }

      void
      _M_do_set() _GLIBCXX_NOEXCEPT
      { }

      void
      _M_do_reset() _GLIBCXX_NOEXCEPT
      { }

      // Are all empty bitsets equal to each other?  Are they equal to
      // themselves?  How to compare a thing which has no state?  What is
      // the sound of one zero-length bitset clapping?
      bool
      _M_is_equal(const _Base_bitset<0>&) const _GLIBCXX_NOEXCEPT
      { return true; }

      template<size_t _Nb>
        bool
        _M_are_all() const _GLIBCXX_NOEXCEPT
        { return true; }

      bool
      _M_is_any() const _GLIBCXX_NOEXCEPT
      { return false; }

      size_t
      _M_do_count() const _GLIBCXX_NOEXCEPT
      { return 0; }

      unsigned long
      _M_do_to_ulong() const _GLIBCXX_NOEXCEPT
      { return 0; }

#if __cplusplus >= 201103L
      unsigned long long
      _M_do_to_ullong() const noexcept
      { return 0; }
#endif

      // Normally "not found" is the size, but that could also be
      // misinterpreted as an index in this corner case.  Oh well.
      size_t
      _M_do_find_first(size_t) const _GLIBCXX_NOEXCEPT
      { return 0; }

      size_t
      _M_do_find_next(size_t, size_t) const _GLIBCXX_NOEXCEPT
      { return 0; }
    };


  // Helper class to zero out the unused high-order bits in the highest word.
  template<size_t _Extrabits>
    struct _Sanitize
    {
      typedef unsigned long _WordT;

      static void
      _S_do_sanitize(_WordT& __val) _GLIBCXX_NOEXCEPT
      { __val &= ~((~static_cast<_WordT>(0)) << _Extrabits); }
    };

  template<>
    struct _Sanitize<0>
    {
      typedef unsigned long _WordT;

      static void
      _S_do_sanitize(_WordT) _GLIBCXX_NOEXCEPT { }
    };

#if __cplusplus >= 201103L
  template<size_t _Nb, bool = (_Nb < _GLIBCXX_BITSET_BITS_PER_ULL)>
    struct _Sanitize_val
    {
      static constexpr unsigned long long
      _S_do_sanitize_val(unsigned long long __val)
      { return __val; }
    };

  template<size_t _Nb>
    struct _Sanitize_val<_Nb, true>
    {
      static constexpr unsigned long long
      _S_do_sanitize_val(unsigned long long __val)
      { return __val & ~((~static_cast<unsigned long long>(0)) << _Nb); }
    };
#endif

  /**
   *  @brief The %bitset class represents a @e fixed-size sequence of bits.
   *  @ingroup utilities
   *
   *  (Note that %bitset does @e not meet the formal requirements of a
   *  <a href="tables.html#65">container</a>.  Mainly, it lacks iterators.)
   *
   *  The template argument, @a Nb, may be any non-negative number,
   *  specifying the number of bits (e.g., "0", "12", "1024*1024").
   *
   *  In the general unoptimized case, storage is allocated in word-sized
   *  blocks.  Let B be the number of bits in a word, then (Nb+(B-1))/B
   *  words will be used for storage.  B - Nb%B bits are unused.  (They are
   *  the high-order bits in the highest word.)  It is a class invariant
   *  that those unused bits are always zero.
   *
   *  If you think of %bitset as <em>a simple array of bits</em>, be
   *  aware that your mental picture is reversed: a %bitset behaves
   *  the same way as bits in integers do, with the bit at index 0 in
   *  the <em>least significant / right-hand</em> position, and the bit at
   *  index Nb-1 in the <em>most significant / left-hand</em> position.
   *  Thus, unlike other containers, a %bitset's index <em>counts from
   *  right to left</em>, to put it very loosely.
   *
   *  This behavior is preserved when translating to and from strings.  For
   *  example, the first line of the following program probably prints
   *  <em>b(&apos;a&apos;) is 0001100001</em> on a modern ASCII system.
   *
   *  @code
   *     #include <bitset>
   *     #include <iostream>
   *     #include <sstream>
   *
   *     using namespace std;
   *
   *     int main()
   *     {
   *         long         a = 'a';
   *         bitset<10>   b(a);
   *
   *         cout << "b('a') is " << b << endl;
   *
   *         ostringstream s;
   *         s << b;
   *         string  str = s.str();
   *         cout << "index 3 in the string is " << str[3] << " but\n"
   *              << "index 3 in the bitset is " << b[3] << endl;
   *     }
   *  @endcode
   *
   *  Also see:
   *  https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_containers.html
   *  for a description of extensions.
   *
   *  Most of the actual code isn't contained in %bitset<> itself, but in the
   *  base class _Base_bitset.  The base class works with whole words, not with
   *  individual bits.  This allows us to specialize _Base_bitset for the
   *  important special case where the %bitset is only a single word.
   *
   *  Extra confusion can result due to the fact that the storage for
   *  _Base_bitset @e is a regular array, and is indexed as such.  This is
   *  carefully encapsulated.
  */
  template<size_t _Nb>
    class bitset
    : private _Base_bitset<_GLIBCXX_BITSET_WORDS(_Nb)>
    {
    private:
      typedef _Base_bitset<_GLIBCXX_BITSET_WORDS(_Nb)> _Base;
      typedef unsigned long _WordT;

      template<class _CharT, class _Traits, class _Alloc>
      void
      _M_check_initial_position(const std::basic_string<_CharT, _Traits, _Alloc>& __s,
				size_t __position) const
      {
	if (__position > __s.size())
	  __throw_out_of_range_fmt(__N("bitset::bitset: __position "
				       "(which is %zu) > __s.size() "
				       "(which is %zu)"),
				   __position, __s.size());
      }

      void _M_check(size_t __position, const char *__s) const
      {
	if (__position >= _Nb)
	  __throw_out_of_range_fmt(__N("%s: __position (which is %zu) "
				       ">= _Nb (which is %zu)"),
				   __s, __position, _Nb);
      }

      void
      _M_do_sanitize() _GLIBCXX_NOEXCEPT
      {
	typedef _Sanitize<_Nb % _GLIBCXX_BITSET_BITS_PER_WORD> __sanitize_type;
	__sanitize_type::_S_do_sanitize(this->_M_hiword());
      }

#if __cplusplus >= 201103L
      friend struct std::hash<bitset>;
#endif

    public:
      /**
       *  This encapsulates the concept of a single bit.  An instance of this
       *  class is a proxy for an actual bit; this way the individual bit
       *  operations are done as faster word-size bitwise instructions.
       *
       *  Most users will never need to use this class directly; conversions
       *  to and from bool are automatic and should be transparent.  Overloaded
       *  operators help to preserve the illusion.
       *
       *  (On a typical system, this <em>bit %reference</em> is 64
       *  times the size of an actual bit.  Ha.)
       */
      class reference
      {
	friend class bitset;

	_WordT*	_M_wp;
	size_t 	_M_bpos;

	// left undefined
	reference();

      public:
	reference(bitset& __b, size_t __pos) _GLIBCXX_NOEXCEPT
	{
	  _M_wp = &__b._M_getword(__pos);
	  _M_bpos = _Base::_S_whichbit(__pos);
	}

	~reference() _GLIBCXX_NOEXCEPT
	{ }

	// For b[i] = __x;
	reference&
	operator=(bool __x) _GLIBCXX_NOEXCEPT
	{
	  if (__x)
	    *_M_wp |= _Base::_S_maskbit(_M_bpos);
	  else
	    *_M_wp &= ~_Base::_S_maskbit(_M_bpos);
	  return *this;
	}

	// For b[i] = b[__j];
	reference&
	operator=(const reference& __j) _GLIBCXX_NOEXCEPT
	{
	  if ((*(__j._M_wp) & _Base::_S_maskbit(__j._M_bpos)))
	    *_M_wp |= _Base::_S_maskbit(_M_bpos);
	  else
	    *_M_wp &= ~_Base::_S_maskbit(_M_bpos);
	  return *this;
	}

	// Flips the bit
	bool
	operator~() const _GLIBCXX_NOEXCEPT
	{ return (*(_M_wp) & _Base::_S_maskbit(_M_bpos)) == 0; }

	// For __x = b[i];
	operator bool() const _GLIBCXX_NOEXCEPT
	{ return (*(_M_wp) & _Base::_S_maskbit(_M_bpos)) != 0; }

	// For b[i].flip();
	reference&
	flip() _GLIBCXX_NOEXCEPT
	{
	  *_M_wp ^= _Base::_S_maskbit(_M_bpos);
	  return *this;
	}
      };
      friend class reference;

      // 23.3.5.1 constructors:
      /// All bits set to zero.
      _GLIBCXX_CONSTEXPR bitset() _GLIBCXX_NOEXCEPT
      { }

      /// Initial bits bitwise-copied from a single word (others set to zero).
#if __cplusplus >= 201103L
      constexpr bitset(unsigned long long __val) noexcept
      : _Base(_Sanitize_val<_Nb>::_S_do_sanitize_val(__val)) { }
#else
      bitset(unsigned long __val)
      : _Base(__val)
      { _M_do_sanitize(); }
#endif

      /**
       *  Use a subset of a string.
       *  @param  __s  A string of @a 0 and @a 1 characters.
       *  @param  __position  Index of the first character in @a __s to use;
       *                    defaults to zero.
       *  @throw  std::out_of_range  If @a pos is bigger the size of @a __s.
       *  @throw  std::invalid_argument  If a character appears in the string
       *                                 which is neither @a 0 nor @a 1.
       */
      template<class _CharT, class _Traits, class _Alloc>
	explicit
	bitset(const std::basic_string<_CharT, _Traits, _Alloc>& __s,
	       size_t __position = 0)
	: _Base()
	{
	  _M_check_initial_position(__s, __position);
	  _M_copy_from_string(__s, __position,
			      std::basic_string<_CharT, _Traits, _Alloc>::npos,
			      _CharT('0'), _CharT('1'));
	}

      /**
       *  Use a subset of a string.
       *  @param  __s  A string of @a 0 and @a 1 characters.
       *  @param  __position  Index of the first character in @a __s to use.
       *  @param  __n    The number of characters to copy.
       *  @throw std::out_of_range If @a __position is bigger the size
       *  of @a __s.
       *  @throw  std::invalid_argument  If a character appears in the string
       *                                 which is neither @a 0 nor @a 1.
       */
      template<class _CharT, class _Traits, class _Alloc>
	bitset(const std::basic_string<_CharT, _Traits, _Alloc>& __s,
	       size_t __position, size_t __n)
	: _Base()
	{
	  _M_check_initial_position(__s, __position);
	  _M_copy_from_string(__s, __position, __n, _CharT('0'), _CharT('1'));
	}

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 396. what are characters zero and one.
      template<class _CharT, class _Traits, class _Alloc>
	bitset(const std::basic_string<_CharT, _Traits, _Alloc>& __s,
	       size_t __position, size_t __n,
	       _CharT __zero, _CharT __one = _CharT('1'))
	: _Base()
	{
	  _M_check_initial_position(__s, __position);
	  _M_copy_from_string(__s, __position, __n, __zero, __one);
	}

#if __cplusplus >= 201103L
      /**
       *  Construct from a character %array.
       *  @param  __str  An %array of characters @a zero and @a one.
       *  @param  __n    The number of characters to use.
       *  @param  __zero The character corresponding to the value 0.
       *  @param  __one  The character corresponding to the value 1.
       *  @throw  std::invalid_argument If a character appears in the string
       *                                which is neither @a __zero nor @a __one.
       */
      template<typename _CharT>
        explicit
        bitset(const _CharT* __str,
	       typename std::basic_string<_CharT>::size_type __n
	       = std::basic_string<_CharT>::npos,
	       _CharT __zero = _CharT('0'), _CharT __one = _CharT('1'))
        : _Base()
        {
	  if (!__str)
	    __throw_logic_error(__N("bitset::bitset(const _CharT*, ...)"));

	  if (__n == std::basic_string<_CharT>::npos)
	    __n = std::char_traits<_CharT>::length(__str);
	  _M_copy_from_ptr<_CharT, std::char_traits<_CharT>>(__str, __n, 0,
							     __n, __zero,
							     __one);
	}
#endif

      // 23.3.5.2 bitset operations:
      //@{
      /**
       *  Operations on bitsets.
       *  @param  __rhs  A same-sized bitset.
       *
       *  These should be self-explanatory.
       */
      bitset<_Nb>&
      operator&=(const bitset<_Nb>& __rhs) _GLIBCXX_NOEXCEPT
      {
	this->_M_do_and(__rhs);
	return *this;
      }

      bitset<_Nb>&
      operator|=(const bitset<_Nb>& __rhs) _GLIBCXX_NOEXCEPT
      {
	this->_M_do_or(__rhs);
	return *this;
      }

      bitset<_Nb>&
      operator^=(const bitset<_Nb>& __rhs) _GLIBCXX_NOEXCEPT
      {
	this->_M_do_xor(__rhs);
	return *this;
      }
      //@}

      bitset<_Nb>&
      operator+(const int& __rhs) _GLIBCXX_NOEXCEPT
      {
	this->_M_do_add(__rhs);
	return *this;
      }

      bitset<_Nb>&
      operator-(const int& __rhs) _GLIBCXX_NOEXCEPT
      {
	this->_M_do_sub(__rhs);
	return *this;
      }

      //@{
      /**
       *  Operations on bitsets.
       *  @param  __position  The number of places to shift.
       *
       *  These should be self-explanatory.
       */
      bitset<_Nb>&
      operator<<=(size_t __position) _GLIBCXX_NOEXCEPT
      {
	if (__builtin_expect(__position < _Nb, 1))
	  {
	    this->_M_do_left_shift(__position);
	    this->_M_do_sanitize();
	  }
	else
	  this->_M_do_reset();
	return *this;
      }

      bitset<_Nb>&
      operator>>=(size_t __position) _GLIBCXX_NOEXCEPT
      {
	if (__builtin_expect(__position < _Nb, 1))
	  {
	    this->_M_do_right_shift(__position);
	    this->_M_do_sanitize();
	  }
	else
	  this->_M_do_reset();
	return *this;
      }
      //@}

      //@{
      /**
       *  These versions of single-bit set, reset, flip, and test are
       *  extensions from the SGI version.  They do no range checking.
       *  @ingroup SGIextensions
       */
      bitset<_Nb>&
      _Unchecked_set(size_t __pos) _GLIBCXX_NOEXCEPT
      {
	this->_M_getword(__pos) |= _Base::_S_maskbit(__pos);
	return *this;
      }

      bitset<_Nb>&
      _Unchecked_set(size_t __pos, int __val) _GLIBCXX_NOEXCEPT
      {
	if (__val)
	  this->_M_getword(__pos) |= _Base::_S_maskbit(__pos);
	else
	  this->_M_getword(__pos) &= ~_Base::_S_maskbit(__pos);
	return *this;
      }

      bitset<_Nb>&
      _Unchecked_reset(size_t __pos) _GLIBCXX_NOEXCEPT
      {
	this->_M_getword(__pos) &= ~_Base::_S_maskbit(__pos);
	return *this;
      }

      bitset<_Nb>&
      _Unchecked_flip(size_t __pos) _GLIBCXX_NOEXCEPT
      {
	this->_M_getword(__pos) ^= _Base::_S_maskbit(__pos);
	return *this;
      }

      _GLIBCXX_CONSTEXPR bool
      _Unchecked_test(size_t __pos) const _GLIBCXX_NOEXCEPT
      { return ((this->_M_getword(__pos) & _Base::_S_maskbit(__pos))
		!= static_cast<_WordT>(0)); }
      //@}

      // Set, reset, and flip.
      /**
       *  @brief Sets every bit to true.
       */
      bitset<_Nb>&
      set() _GLIBCXX_NOEXCEPT
      {
	this->_M_do_set();
	this->_M_do_sanitize();
	return *this;
      }

      /**
       *  @brief Sets a given bit to a particular value.
       *  @param  __position  The index of the bit.
       *  @param  __val  Either true or false, defaults to true.
       *  @throw  std::out_of_range  If @a pos is bigger the size of the %set.
       */
      bitset<_Nb>&
      set(size_t __position, bool __val = true)
      {
	this->_M_check(__position, __N("bitset::set"));
	return _Unchecked_set(__position, __val);
      }

      /**
       *  @brief Sets every bit to false.
       */
      bitset<_Nb>&
      reset() _GLIBCXX_NOEXCEPT
      {
	this->_M_do_reset();
	return *this;
      }

      /**
       *  @brief Sets a given bit to false.
       *  @param  __position  The index of the bit.
       *  @throw  std::out_of_range  If @a pos is bigger the size of the %set.
       *
       *  Same as writing @c set(pos,false).
       */
      bitset<_Nb>&
      reset(size_t __position)
      {
	this->_M_check(__position, __N("bitset::reset"));
	return _Unchecked_reset(__position);
      }

      /**
       *  @brief Toggles every bit to its opposite value.
       */
      bitset<_Nb>&
      flip() _GLIBCXX_NOEXCEPT
      {
	this->_M_do_flip();
	this->_M_do_sanitize();
	return *this;
      }

      /**
       *  @brief Toggles a given bit to its opposite value.
       *  @param  __position  The index of the bit.
       *  @throw  std::out_of_range  If @a pos is bigger the size of the %set.
       */
      bitset<_Nb>&
      flip(size_t __position)
      {
	this->_M_check(__position, __N("bitset::flip"));
	return _Unchecked_flip(__position);
      }

      /// See the no-argument flip().
      bitset<_Nb>
      operator~() const _GLIBCXX_NOEXCEPT
      { return bitset<_Nb>(*this).flip(); }

      //@{
      /**
       *  @brief  Array-indexing support.
       *  @param  __position  Index into the %bitset.
       *  @return A bool for a <em>const %bitset</em>.  For non-const
       *           bitsets, an instance of the reference proxy class.
       *  @note  These operators do no range checking and throw no exceptions,
       *         as required by DR 11 to the standard.
       *
       *  _GLIBCXX_RESOLVE_LIB_DEFECTS Note that this implementation already
       *  resolves DR 11 (items 1 and 2), but does not do the range-checking
       *  required by that DR's resolution.  -pme
       *  The DR has since been changed:  range-checking is a precondition
       *  (users' responsibility), and these functions must not throw.  -pme
       */
      reference
      operator[](size_t __position)
      { return reference(*this, __position); }

      _GLIBCXX_CONSTEXPR bool
      operator[](size_t __position) const
      { return _Unchecked_test(__position); }
      //@}

      /**
       *  @brief Returns a numerical interpretation of the %bitset.
       *  @return  The integral equivalent of the bits.
       *  @throw  std::overflow_error  If there are too many bits to be
       *                               represented in an @c unsigned @c long.
       */
      unsigned long
      to_ulong() const
      { return this->_M_do_to_ulong(); }

#if __cplusplus >= 201103L
      unsigned long long
      to_ullong() const
      { return this->_M_do_to_ullong(); }
#endif

      /**
       *  @brief Returns a character interpretation of the %bitset.
       *  @return  The string equivalent of the bits.
       *
       *  Note the ordering of the bits:  decreasing character positions
       *  correspond to increasing bit positions (see the main class notes for
       *  an example).
       */
      template<class _CharT, class _Traits, class _Alloc>
	std::basic_string<_CharT, _Traits, _Alloc>
	to_string() const
	{
	  std::basic_string<_CharT, _Traits, _Alloc> __result;
	  _M_copy_to_string(__result, _CharT('0'), _CharT('1'));
	  return __result;
	}

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 396. what are characters zero and one.
      template<class _CharT, class _Traits, class _Alloc>
	std::basic_string<_CharT, _Traits, _Alloc>
	to_string(_CharT __zero, _CharT __one = _CharT('1')) const
	{
	  std::basic_string<_CharT, _Traits, _Alloc> __result;
	  _M_copy_to_string(__result, __zero, __one);
	  return __result;
	}

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 434. bitset::to_string() hard to use.
      template<class _CharT, class _Traits>
	std::basic_string<_CharT, _Traits, std::allocator<_CharT> >
	to_string() const
	{ return to_string<_CharT, _Traits, std::allocator<_CharT> >(); }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 853. to_string needs updating with zero and one.
      template<class _CharT, class _Traits>
	std::basic_string<_CharT, _Traits, std::allocator<_CharT> >
	to_string(_CharT __zero, _CharT __one = _CharT('1')) const
	{ return to_string<_CharT, _Traits,
	                   std::allocator<_CharT> >(__zero, __one); }

      template<class _CharT>
	std::basic_string<_CharT, std::char_traits<_CharT>,
	                  std::allocator<_CharT> >
	to_string() const
	{
	  return to_string<_CharT, std::char_traits<_CharT>,
	                   std::allocator<_CharT> >();
	}

      template<class _CharT>
	std::basic_string<_CharT, std::char_traits<_CharT>,
	                  std::allocator<_CharT> >
	to_string(_CharT __zero, _CharT __one = _CharT('1')) const
	{
	  return to_string<_CharT, std::char_traits<_CharT>,
	                   std::allocator<_CharT> >(__zero, __one);
	}

      std::basic_string<char, std::char_traits<char>, std::allocator<char> >
      to_string() const
      {
	return to_string<char, std::char_traits<char>,
	                 std::allocator<char> >();
      }

      std::basic_string<char, std::char_traits<char>, std::allocator<char> >
      to_string(char __zero, char __one = '1') const
      {
	return to_string<char, std::char_traits<char>,
	                 std::allocator<char> >(__zero, __one);
      }

      // Helper functions for string operations.
      template<class _CharT, class _Traits>
        void
        _M_copy_from_ptr(const _CharT*, size_t, size_t, size_t,
			 _CharT, _CharT);

      template<class _CharT, class _Traits, class _Alloc>
	void
	_M_copy_from_string(const std::basic_string<_CharT,
			    _Traits, _Alloc>& __s, size_t __pos, size_t __n,
			    _CharT __zero, _CharT __one)
	{ _M_copy_from_ptr<_CharT, _Traits>(__s.data(), __s.size(), __pos, __n,
					    __zero, __one); }

      template<class _CharT, class _Traits, class _Alloc>
	void
        _M_copy_to_string(std::basic_string<_CharT, _Traits, _Alloc>&,
			  _CharT, _CharT) const;

      // NB: Backward compat.
      template<class _CharT, class _Traits, class _Alloc>
	void
	_M_copy_from_string(const std::basic_string<_CharT,
			    _Traits, _Alloc>& __s, size_t __pos, size_t __n)
	{ _M_copy_from_string(__s, __pos, __n, _CharT('0'), _CharT('1')); }

      template<class _CharT, class _Traits, class _Alloc>
	void
        _M_copy_to_string(std::basic_string<_CharT, _Traits,_Alloc>& __s) const
	{ _M_copy_to_string(__s, _CharT('0'), _CharT('1')); }

      /// Returns the number of bits which are set.
      size_t
      count() const _GLIBCXX_NOEXCEPT
      { return this->_M_do_count(); }

      /// Returns the total number of bits.
      _GLIBCXX_CONSTEXPR size_t
      size() const _GLIBCXX_NOEXCEPT
      { return _Nb; }

      //@{
      /// These comparisons for equality/inequality are, well, @e bitwise.
      bool
      operator==(const bitset<_Nb>& __rhs) const _GLIBCXX_NOEXCEPT
      { return this->_M_is_equal(__rhs); }

      bool
      operator!=(const bitset<_Nb>& __rhs) const _GLIBCXX_NOEXCEPT
      { return !this->_M_is_equal(__rhs); }
      //@}

      /**
       *  @brief Tests the value of a bit.
       *  @param  __position  The index of a bit.
       *  @return  The value at @a pos.
       *  @throw  std::out_of_range  If @a pos is bigger the size of the %set.
       */
      bool
      test(size_t __position) const
      {
	this->_M_check(__position, __N("bitset::test"));
	return _Unchecked_test(__position);
      }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR 693. std::bitset::all() missing.
      /**
       *  @brief Tests whether all the bits are on.
       *  @return  True if all the bits are set.
       */
      bool
      all() const _GLIBCXX_NOEXCEPT
      { return this->template _M_are_all<_Nb>(); }

      /**
       *  @brief Tests whether any of the bits are on.
       *  @return  True if at least one bit is set.
       */
      bool
      any() const _GLIBCXX_NOEXCEPT
      { return this->_M_is_any(); }

      /**
       *  @brief Tests whether any of the bits are on.
       *  @return  True if none of the bits are set.
       */
      bool
      none() const _GLIBCXX_NOEXCEPT
      { return !this->_M_is_any(); }


      size_t
      count_shared(const bitset<_Nb>& __rhs) const _GLIBCXX_NOEXCEPT
      {
          return this->_M_do_and_count(__rhs);
      }

      size_t
      first_shared(const bitset<_Nb>& __rhs) const _GLIBCXX_NOEXCEPT
      {
          return this->_M_do_first_shared(__rhs);
      }


      //@{
      /// Self-explanatory.
      bitset<_Nb>
      operator<<(size_t __position) const _GLIBCXX_NOEXCEPT
      { return bitset<_Nb>(*this) <<= __position; }

      bitset<_Nb>
      operator>>(size_t __position) const _GLIBCXX_NOEXCEPT
      { return bitset<_Nb>(*this) >>= __position; }
      //@}

      /**
       *  @brief  Finds the index of the first "on" bit.
       *  @return  The index of the first bit set, or size() if not found.
       *  @ingroup SGIextensions
       *  @sa  _Find_next
       */
      size_t
      _Find_first() const _GLIBCXX_NOEXCEPT
      { return this->_M_do_find_first(_Nb); }

      size_t
      _Find_first_shared(const bitset<_Nb>& __other) const _GLIBCXX_NOEXCEPT
      { return this->_M_do_find_first(__other, _Nb); }

      /**
       *  @brief  Finds the index of the next "on" bit after prev.
       *  @return  The index of the next bit set, or size() if not found.
       *  @param  __prev  Where to start searching.
       *  @ingroup SGIextensions
       *  @sa  _Find_first
       */
      size_t
      _Find_next(size_t __prev) const _GLIBCXX_NOEXCEPT
      { return this->_M_do_find_next(__prev, _Nb); }

      size_t
      _Find_next_shared(const bitset<_Nb>& __other, size_t __prev) const _GLIBCXX_NOEXCEPT
      { return this->_M_do_find_next(__other, __prev, _Nb); }
    };

  // Definitions of non-inline member functions.
  template<size_t _Nb>
    template<class _CharT, class _Traits>
      void
      bitset<_Nb>::
      _M_copy_from_ptr(const _CharT* __s, size_t __len,
		       size_t __pos, size_t __n, _CharT __zero, _CharT __one)
      {
	reset();
	const size_t __nbits = std::min(_Nb, std::min(__n, size_t(__len - __pos)));
	for (size_t __i = __nbits; __i > 0; --__i)
	  {
	    const _CharT __c = __s[__pos + __nbits - __i];
	    if (_Traits::eq(__c, __zero))
	      ;
	    else if (_Traits::eq(__c, __one))
	      _Unchecked_set(__i - 1);
	    else
	      __throw_invalid_argument(__N("bitset::_M_copy_from_ptr"));
	  }
      }

  template<size_t _Nb>
    template<class _CharT, class _Traits, class _Alloc>
      void
      bitset<_Nb>::
      _M_copy_to_string(std::basic_string<_CharT, _Traits, _Alloc>& __s,
			_CharT __zero, _CharT __one) const
      {
	__s.assign(_Nb, __zero);
	for (size_t __i = _Nb; __i > 0; --__i)
	  if (_Unchecked_test(__i - 1))
	    _Traits::assign(__s[_Nb - __i], __one);
      }

  // 23.3.5.3 bitset operations:
  //@{
  /**
   *  @brief  Global bitwise operations on bitsets.
   *  @param  __x  A bitset.
   *  @param  __y  A bitset of the same size as @a __x.
   *  @return  A new bitset.
   *
   *  These should be self-explanatory.
  */
  template<size_t _Nb>
    inline bitset<_Nb>
    operator&(const bitset<_Nb>& __x, const bitset<_Nb>& __y) _GLIBCXX_NOEXCEPT
    {
      bitset<_Nb> __result(__x);
      __result &= __y;
      return __result;
    }

  template<size_t _Nb>
    inline bitset<_Nb>
    operator|(const bitset<_Nb>& __x, const bitset<_Nb>& __y) _GLIBCXX_NOEXCEPT
    {
      bitset<_Nb> __result(__x);
      __result |= __y;
      return __result;
    }

  template <size_t _Nb>
    inline bitset<_Nb>
    operator^(const bitset<_Nb>& __x, const bitset<_Nb>& __y) _GLIBCXX_NOEXCEPT
    {
      bitset<_Nb> __result(__x);
      __result ^= __y;
      return __result;
    }
  //@}

  //@{
  /**
   *  @brief Global I/O operators for bitsets.
   *
   *  Direct I/O between streams and bitsets is supported.  Output is
   *  straightforward.  Input will skip whitespace, only accept @a 0 and @a 1
   *  characters, and will only extract as many digits as the %bitset will
   *  hold.
  */
  template<class _CharT, class _Traits, size_t _Nb>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is, bitset<_Nb>& __x)
    {
      typedef typename _Traits::char_type          char_type;
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      std::basic_string<_CharT, _Traits> __tmp;
      __tmp.reserve(_Nb);

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 303. Bitset input operator underspecified
      const char_type __zero = __is.widen('0');
      const char_type __one = __is.widen('1');

      typename __ios_base::iostate __state = __ios_base::goodbit;
      typename __istream_type::sentry __sentry(__is);
      if (__sentry)
	{
	  __try
	    {
	      for (size_t __i = _Nb; __i > 0; --__i)
		{
		  static typename _Traits::int_type __eof = _Traits::eof();

		  typename _Traits::int_type __c1 = __is.rdbuf()->sbumpc();
		  if (_Traits::eq_int_type(__c1, __eof))
		    {
		      __state |= __ios_base::eofbit;
		      break;
		    }
		  else
		    {
		      const char_type __c2 = _Traits::to_char_type(__c1);
		      if (_Traits::eq(__c2, __zero))
			__tmp.push_back(__zero);
		      else if (_Traits::eq(__c2, __one))
			__tmp.push_back(__one);
		      else if (_Traits::
			       eq_int_type(__is.rdbuf()->sputbackc(__c2),
					   __eof))
			{
			  __state |= __ios_base::failbit;
			  break;
			}
		    }
		}
	    }
	  __catch(__cxxabiv1::__forced_unwind&)
	    {
	      __is._M_setstate(__ios_base::badbit);
	      __throw_exception_again;
	    }
	  __catch(...)
	    { __is._M_setstate(__ios_base::badbit); }
	}

      if (__tmp.empty() && _Nb)
	__state |= __ios_base::failbit;
      else
	__x._M_copy_from_string(__tmp, static_cast<size_t>(0), _Nb,
				__zero, __one);
      if (__state)
	__is.setstate(__state);
      return __is;
    }

  template <class _CharT, class _Traits, size_t _Nb>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const bitset<_Nb>& __x)
    {
      std::basic_string<_CharT, _Traits> __tmp;

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 396. what are characters zero and one.
      const ctype<_CharT>& __ct = use_facet<ctype<_CharT> >(__os.getloc());
      __x._M_copy_to_string(__tmp, __ct.widen('0'), __ct.widen('1'));
      return __os << __tmp;
    }
  //@}

_GLIBCXX_END_NAMESPACE_CONTAINER
} // namespace std

#undef _GLIBCXX_BITSET_WORDS
#undef _GLIBCXX_BITSET_BITS_PER_WORD
#undef _GLIBCXX_BITSET_BITS_PER_ULL

#if __cplusplus >= 201103L

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // DR 1182.
  /// std::hash specialization for bitset.
  template<size_t _Nb>
    struct hash<_GLIBCXX_STD_C::bitset<_Nb>>
    : public __hash_base<size_t, _GLIBCXX_STD_C::bitset<_Nb>>
    {
      size_t
      operator()(const _GLIBCXX_STD_C::bitset<_Nb>& __b) const noexcept
      {
	const size_t __clength = (_Nb + __CHAR_BIT__ - 1) / __CHAR_BIT__;
	return std::_Hash_impl::hash(__b._M_getdata(), __clength);
      }
    };

  template<>
    struct hash<_GLIBCXX_STD_C::bitset<0>>
    : public __hash_base<size_t, _GLIBCXX_STD_C::bitset<0>>
    {
      size_t
      operator()(const _GLIBCXX_STD_C::bitset<0>&) const noexcept
      { return 0; }
    };

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // C++11

#ifdef _GLIBCXX_DEBUG
# include <debug/bitset>
#endif

#ifdef _GLIBCXX_PROFILE
# include <profile/bitset>
#endif

#endif /* _GLIBCXX_BITSET */
