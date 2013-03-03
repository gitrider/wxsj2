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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_d/include/CGAL/Kernel_d/Point_d.h $
// $Id: Point_d.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel
#ifndef CGAL_POINT_D_H
#define CGAL_POINT_D_H

CGAL_BEGIN_NAMESPACE

template <class pR>
class Point_d : public pR::Point_d_base
{ public:
  typedef typename pR::Point_d_base Base;
  typedef Point_d<pR>               Self;
  typedef pR R;
private:
  typedef typename R::RT RT;
  typedef typename R::FT FT;
  typedef typename R::LA LA;
public:

  Point_d(int d=0) : Base(d) {}
  Point_d(int d, const Origin &o) : Base(d,o) {}

  Point_d(int a, int b, int c = 1) : 
    Base(RT(a),RT(b),RT(c)) {} 
  Point_d(const RT& a, const RT& b, const RT& c = 1) :
    Base(a,b,c) {}  
  Point_d(int a, int b, int c, int d) : 
    Base(RT(a),RT(b),RT(c),RT(d)) {}
  Point_d(const RT& a, const RT& b, const RT& c, const RT& d) :
    Base(a,b,c,d) {}

  template <class InputIterator>
  Point_d (int d, InputIterator first, InputIterator last)
    : Base (d, first, last) {}
  template <class InputIterator>
  Point_d(int d, InputIterator first, InputIterator last, const RT& D)
    : Base (d, first, last, D) {}

  Point_d(const Self &p) : Base(p) {}
  Point_d(const Base& p) : Base(p) {}
 
  Vector_d<R> operator-(const Origin& o) const 
  { return Base::operator-(o); }
  Vector_d<R> operator-(const Self& q) const
  { return Base::operator-(q); }
  Self operator+(const Vector_d<R>& v) const
  { return Base::operator+(v); }
  Self operator-(const Vector_d<R>& v) const
  { return Base::operator-(v); }
  Self& operator+=(const Vector_d<R>& v) 
  { return static_cast<Self&>(Base::operator+=(v)); }
  Self& operator-=(const Vector_d<R>& v)
  { return static_cast<Self&>(Base::operator-=(v)); }
  
};

CGAL_END_NAMESPACE
#endif //CGAL_POINT_D_H
