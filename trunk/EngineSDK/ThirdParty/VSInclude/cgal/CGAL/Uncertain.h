// Copyright (c) 2005  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Uncertain.h $
// $Id: Uncertain.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_UNCERTAIN_H
#define CGAL_UNCERTAIN_H

#include <CGAL/basic.h>
#include <stdexcept>
#include <CGAL/enum.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

  // Accessory traits class to provide min and max value of a type.
  // Specialized for bool and CGAL's enums.
  template < typename T >
  struct Minmax_traits;

  template <>
  struct Minmax_traits <bool>
  {
    static const bool min = false;
    static const bool max = true;
  };

  template <>
  struct Minmax_traits <Sign>
  {
    static const Sign min = NEGATIVE;
    static const Sign max = POSITIVE;
  };

  template <>
  struct Minmax_traits <Comparison_result>
  {
    static const Comparison_result min = SMALLER;
    static const Comparison_result max = LARGER;
  };

  template <>
  struct Minmax_traits <Oriented_side>
  {
    static const Oriented_side min = ON_NEGATIVE_SIDE;
    static const Oriented_side max = ON_POSITIVE_SIDE;
  };

  template <>
  struct Minmax_traits <Bounded_side>
  {
    static const Bounded_side min = ON_UNBOUNDED_SIDE;
    static const Bounded_side max = ON_BOUNDED_SIDE;
  };

  template <>
  struct Minmax_traits <Angle>
  {
    static const Angle min = OBTUSE;
    static const Angle max = ACUTE;
  };

} // namespace CGALi


// Encodes an interval [min;max] of values of type T.
// The primary template is supposed to work for enums and bool.

template < typename T >
class Uncertain
{
  T _i, _s;

  static unsigned failures;   // Number of conversion failures.

public:

  typedef T  value_type;

  Uncertain()
    : _i(CGALi::Minmax_traits<T>::min),
      _s(CGALi::Minmax_traits<T>::max) {}

  Uncertain(const T & t)
    : _i(t), _s(t) {}

  Uncertain(const T & i, const T & s)
    : _i(i), _s(s) {}


  const T & inf() const { return _i; }
  const T & sup() const { return _s; }

  // NB : conversion to bool might be too risky
  //      (boost::tribool uses something else).
  operator const T &() const
  {
    if (_i == _s)
      return _i;
    //++failures;
    ++Uncertain<bool>::number_of_failures();  // reasonnable ?
    throw std::range_error("undecidable conversion of Uncertain<T>");
  }

  static unsigned & number_of_failures() { return failures; }

  static Uncertain indeterminate();
};


template < typename T >
unsigned
Uncertain<T>::failures = 0;


// Access functions
// ----------------

template < typename T >
inline
const T & inf(Uncertain<T> const& i)
{
  return i.inf();
}

template < typename T >
inline
const T & sup(Uncertain<T> const& i)
{
  return i.sup();
}


// Basic functions
// ---------------

template < typename T >
inline
Uncertain<T>
Uncertain<T>::indeterminate()
{
  return Uncertain<T>();
}

template < typename T >
inline
bool is_indeterminate(T const& a)
{
  return false;
}

template < typename T >
inline
bool is_indeterminate(Uncertain<T> const& a)
{
  return a.inf() != a.sup();
}

template < typename T >
inline
bool is_singleton(Uncertain<T> const& a)
{
  return a.inf() == a.sup();
}


// Boolean operations for Uncertain<bool>
// --------------------------------------

inline
Uncertain<bool> operator!(Uncertain<bool> const& a)
{
  return Uncertain<bool>(!a.sup(), !a.inf());
}

inline
Uncertain<bool> operator|(Uncertain<bool> const& a, Uncertain<bool> const& b)
{
  return Uncertain<bool>(a.inf() || b.inf(), a.sup() || b.sup());
}

inline
Uncertain<bool> operator|(bool a, Uncertain<bool> const& b)
{
  return Uncertain<bool>(a || b.inf(), a || b.sup());
}

inline
Uncertain<bool> operator|(Uncertain<bool> const& a, bool b)
{
  return Uncertain<bool>(a.inf() || b, a.sup() || b);
}

inline
Uncertain<bool> operator&(Uncertain<bool> const& a, Uncertain<bool> const& b)
{
  return Uncertain<bool>(a.inf() && b.inf(), a.sup() && b.sup());
}

inline
Uncertain<bool> operator&(bool a, Uncertain<bool> const& b)
{
  return Uncertain<bool>(a && b.inf(), a && b.sup());
}

inline
Uncertain<bool> operator&(Uncertain<bool> const& a, bool b)
{
  return Uncertain<bool>(a.inf() && b, a.sup() && b);
}


// Equality operators

template < typename T >
inline
Uncertain<bool> operator==(Uncertain<T> const& a, Uncertain<T> const& b)
{
  if (is_indeterminate(a) || is_indeterminate(b))
    return Uncertain<bool>::indeterminate();
  return a.inf() == b.inf();
}

template < typename T >
inline
Uncertain<bool> operator==(Uncertain<T> const& a, const T & b)
{
  if (is_indeterminate(a))
    return Uncertain<bool>::indeterminate();
  return a.inf() == b;
}

template < typename T >
inline
Uncertain<bool> operator==(const T & a, Uncertain<T> const& b)
{
  if (is_indeterminate(b))
    return Uncertain<bool>::indeterminate();
  return a == b.inf();
}

template < typename T >
inline
Uncertain<bool> operator!=(Uncertain<T> const& a, Uncertain<T> const& b)
{
  return ! (a == b);
}

template < typename T >
inline
Uncertain<bool> operator!=(Uncertain<T> const& a, const T & b)
{
  return ! (a == b);
}

template < typename T >
inline
Uncertain<bool> operator!=(const T & a, Uncertain<T> const& b)
{
  return ! (a == b);
}


// Maker function (a la std::make_pair).

template < typename T >
inline
Uncertain<T> make_uncertain(const T&t)
{
  return Uncertain<T>(t);
}

template < typename T >
inline
const Uncertain<T> & make_uncertain(const Uncertain<T> &t)
{
  return t;
}


// opposite
template < typename T >
inline
Uncertain<T> opposite(const Uncertain<T> &u)
{
  return Uncertain<T>(opposite(u.sup()), opposite(u.inf()));
}


// enum_cast overload

#ifdef CGAL_CFG_MATCHING_BUG_5

template < typename T, typename U >
inline
Uncertain<T> enum_cast_bug(const Uncertain<U>& u, const T*)
{
  return Uncertain<T>(static_cast<const T>(u.inf()),
                      static_cast<const T>(u.sup()));
}

#else

template < typename T, typename U >
inline
Uncertain<T> enum_cast(const Uncertain<U>& u)
{
  return Uncertain<T>(static_cast<T>(u.inf()), static_cast<T>(u.sup()));
}

#endif

CGAL_END_NAMESPACE

#endif // CGAL_UNCERTAIN_H
