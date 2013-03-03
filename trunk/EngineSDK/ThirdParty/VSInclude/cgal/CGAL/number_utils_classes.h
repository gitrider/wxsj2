// Copyright (c) 1997  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/number_utils_classes.h $
// $Id: number_utils_classes.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Hoffmann <hoffmann@inf.ethz.ch>

// to be included by number_utils.h

#ifndef CGAL_NUMBER_UTILS_CLASSES_H
#define CGAL_NUMBER_UTILS_CLASSES_H 1

#include <CGAL/config.h>

#include <algorithm>
#include <utility>
#include <CGAL/functional_base.h>
#include <CGAL/number_utils_fwd.h>

CGAL_BEGIN_NAMESPACE

template < class NT1, class NT2 = NT1 >
struct Equal_to : public Binary_function< NT1, NT2, bool > {
  bool operator()( const NT1& x, const NT2& y) const
  { return x == y; }
};

template < class NT1, class NT2 = NT1 >
struct Not_equal_to : public Binary_function< NT1, NT2, bool > {
  bool operator()( const NT1& x, const NT2& y) const
  { return x != y; }
};

template < class NT1, class NT2 = NT1 >
struct Greater : public Binary_function< NT1, NT2, bool > {
  bool operator()( const NT1& x, const NT2& y) const
  { return x > y; }
};

template < class NT1, class NT2 = NT1 >
struct Less : public Binary_function< NT1, NT2, bool > {
  bool operator()( const NT1& x, const NT2& y) const
  { return x < y; }
};

template < class NT1, class NT2 = NT1 >
struct Greater_equal : public Binary_function< NT1, NT2, bool > {
  bool operator()( const NT1& x, const NT2& y) const
  { return x >= y; }
};

template < class NT1, class NT2 = NT1 >
struct Less_equal : public Binary_function< NT1, NT2, bool > {
  bool operator()( const NT1& x, const NT2& y) const
  { return x <= y; }
};

template < class NT >
struct Is_zero :public Unary_function< NT, bool > {
  bool operator()( const NT& x) const
  { return CGAL_NTS is_zero( x); }
};

template < class NT >
struct Is_one :public Unary_function< NT, bool > {
  bool operator()( const NT& x) const
  { return CGAL_NTS is_one( x); }
};

template < class NT >
struct Is_negative :public Unary_function< NT, bool > {
  bool operator()( const NT& x) const
  { return CGAL_NTS is_negative( x); }
};

template < class NT >
struct Is_positive :public Unary_function< NT, bool > {
  bool operator()( const NT& x) const
  { return CGAL_NTS is_positive( x); }
};

// Sign would result in a name clash with enum.h
template < class NT >
struct Sgn :public Unary_function< NT, Sign > {
  Sign operator()( const NT& x) const
  { return CGAL_NTS sign( x); }
};

template < class NT >
struct Abs :public Unary_function< NT, NT > {
  NT operator()( const NT& x) const
  { return CGAL_NTS abs( x); }
};

template < class NT, class Compare = std::less< NT > >
struct Min :public Binary_function< NT, NT, NT > {
  Min() {}
  Min(const Compare& c_) : c(c_) {}
  NT operator()( const NT& x, const NT& y) const
  { return min( x, y, c); }
protected:
  Compare c;
};

template < class NT, class Compare = std::less< NT > >
struct Max :public Binary_function< NT, NT, NT > {
  Max() {}
  Max(const Compare& c_) : c(c_) {}
  NT operator()( const NT& x, const NT& y) const
  { return max( x, y, c); }
protected:
  Compare c;
};

template < class NT >
struct Compare
  :public Binary_function< NT, NT, Comparison_result > {
  Comparison_result operator()( const NT& x, const NT& y) const
  { return CGAL_NTS compare( x, y); }
};

template < class NT >
struct Square : public Unary_function< NT, NT > {
  NT operator()( const NT& x) const
  { return CGAL_NTS square( x ); }
};

template < class NT >
struct Sqrt : public Unary_function< NT, NT > {
  NT operator()( const NT& x) const
  { return CGAL_NTS sqrt( x ); }
};

template < class NT >
struct Div : public Binary_function< NT, NT, NT > {
  NT operator()( const NT& x, const NT& y) const
  { return CGAL_NTS div( x, y ); }
};

template < class NT >
struct Gcd : public Binary_function< NT, NT, NT > {
  NT operator()( const NT& x, const NT& y) const
  { return CGAL_NTS gcd( x, y ); }
};

template < class NT >
struct To_double : public Unary_function< NT, double > {
  double operator()( const NT& x) const
  { return CGAL_NTS to_double( x ); }
};

template < class NT >
struct To_interval
  : public Unary_function< NT, std::pair<double, double> >
{
  std::pair<double, double> operator()( const NT& x) const
  { return CGAL_NTS to_interval( x ); }
};

CGAL_END_NAMESPACE

#endif // CGAL_NUMBER_UTILS_CLASSES_H
