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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_d/include/CGAL/Kernel_d/Direction_d.h $
// $Id: Direction_d.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel
#ifndef CGAL_DIRECTION_D_H
#define CGAL_DIRECTION_D_H

CGAL_BEGIN_NAMESPACE

template <class pR>
class Direction_d : public pR::Direction_d_base
{ public:
  typedef typename pR::Direction_d_base Base;
  typedef Direction_d<pR>               Self;
  typedef pR R;
  typedef typename R::RT RT;
  typedef typename R::FT FT;
  typedef typename R::LA LA;
  typedef typename Base::Base_direction Base_direction;

  Direction_d(int d=0) : Base(d) {}
  Direction_d(int a, int b) : Base(a,b) {}
  Direction_d(const RT& a, const RT& b) : Base(a,b) {}
  Direction_d(int a, int b, int c) : Base(a,b,c) {}
  Direction_d(const RT& a, const RT& b, const RT& c) : Base(a,b,c) {}

  template <class InputIterator>
  Direction_d (int d, InputIterator first, InputIterator last)
    : Base(d, first, last) {}

  Direction_d(const Direction_d<R> &d) : Base(d) {}
  Direction_d(const Vector_d<R> &v) : Base(v) {}
  Direction_d(int d, Base_direction, int i) : 
    Base(d,Base_direction(),i) {}
  Direction_d(const Base& p) : Base(p) {}

  Self operator-() const { return Base::operator-(); }
  Vector_d<R> vector() const { return Base::vector(); }

  bool operator==(const Self& w) const
  { return Base::operator==(w); }
  bool operator!=(const Self& w) const
  { return Base::operator!=(w); }
};

CGAL_END_NAMESPACE
#endif //CGAL_DIRECTION_D_H
