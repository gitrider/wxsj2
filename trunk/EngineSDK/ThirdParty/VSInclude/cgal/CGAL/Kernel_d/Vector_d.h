// Copyright (c) 2000,2001  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_d/include/CGAL/Kernel_d/Vector_d.h $
// $Id: Vector_d.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel
#ifndef CGAL_VECTOR_D_H
#define CGAL_VECTOR_D_H

CGAL_BEGIN_NAMESPACE
 
template <class pR>
class Vector_d : public pR::Vector_d_base
{ public:
  typedef typename pR::Vector_d_base Base;
  typedef Vector_d<pR>               Self;
  typedef pR R;
  typedef typename R::RT RT;
  typedef typename R::FT FT;
  typedef typename R::LA LA;
  typedef typename Base::Base_vector Base_vector;

  Vector_d(int d=0) : Base(d) {}
  Vector_d(int d, Null_vector v) : Base(d,v) {}
  Vector_d(int a, int b, int c = 1) : Base(a,b,c) {}
  Vector_d(const RT& a, const RT& b, const RT& c = 1) :
    Base(a,b,c) {}
  Vector_d(int a, int b, int c, int d) : Base(a,b,c,d) {}
  Vector_d(const RT& a, const RT& b, const RT& c, const RT& d) :
    Base(a,b,c,d) {}
  Vector_d(int d, Base_vector, int i) :
    Base(d,Base_vector(),i) {}

  template <class InputIterator>
  Vector_d (int d, InputIterator first, InputIterator last)
    : Base (d, first, last) {}
  template <class InputIterator>
  Vector_d (int d, InputIterator first, InputIterator last, const RT& D)
    : Base (d, first, last, D) {}

  Vector_d(const Self& v) : Base(v) {}
  Vector_d(const Base& v) : Base(v) {}

  Direction_d<R> direction() const { return Base::direction(); }

  FT operator* (const Self& w) const
  { return Base::operator*(w); }
  Self operator+(const Self& w) const
  { return Base::operator+(w); }
  Self operator-(const Self& w) const
  { return Base::operator-(w); }
  Self operator-() const 
  { return Base::operator-(); }

  template <class NT> 
  Self operator/(const NT& n) const { return Base::operator/(n); }

  Self& operator+=(const Self& w) 
  { return static_cast<Self&>(Base::operator+=(w)); }
  Self& operator-=(const Self& w) 
  { return static_cast<Self&>(Base::operator-=(w)); }
  template <class NT> 
  Self& operator*=(const NT& n) 
  { return static_cast<Self&>(Base::operator*=(n)); }
  template <class NT> 
  Self& operator/=(const NT& n) 
  { return static_cast<Self&>(Base::operator/=(n)); }

  bool operator==(const Self& w) const
  { return Base::operator==(w); }
  bool operator!=(const Self& w) const
  { return Base::operator!=(w); }
  
};

template <class R> Point_d<R> 
operator+ (const Origin& o, const Vector_d<R>& v)
{ return Point_d<R>( o + static_cast<typename Vector_d<R>::Base>(v) ); }

template <class NT, class R>
Vector_d<R> operator*(const NT& n, const Vector_d<R>& v) 
{ return Vector_d<R>( n * static_cast<typename Vector_d<R>::Base>(v) ); }

CGAL_END_NAMESPACE
#endif //CGAL_VECTOR_D_H
