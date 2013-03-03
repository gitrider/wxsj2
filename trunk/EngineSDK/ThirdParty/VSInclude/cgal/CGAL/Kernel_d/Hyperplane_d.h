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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_d/include/CGAL/Kernel_d/Hyperplane_d.h $
// $Id: Hyperplane_d.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel
#ifndef CGAL_HYPERPLANE_D_H
#define CGAL_HYPERPLANE_D_H

CGAL_BEGIN_NAMESPACE

template <class pR>
class Hyperplane_d : public pR::Hyperplane_d_base
{ public:
  typedef typename pR::Hyperplane_d_base Base;
  typedef Hyperplane_d<pR>               Self;
  typedef pR R;
  typedef typename R::RT RT;
  typedef typename R::FT FT;
  typedef typename R::LA LA;

  Hyperplane_d(int d=0) : Base(d) {}
  Hyperplane_d(int a, int b, int c) : Base(a,b,c) {}
  Hyperplane_d(const RT& a, const RT& b, const RT& c) : 
    Base(a,b,c) {}
  Hyperplane_d(int a, int b, int c, int d) : Base(a,b,c,d) {}
  Hyperplane_d(const RT& a, const RT& b, const RT& c, const RT& d) : 
    Base(a,b,c,d) {}

  Hyperplane_d(const Point_d<R>& p, const Direction_d<R>& dir) :
    Base(p,dir) {}

  Hyperplane_d(const Hyperplane_d<R> &h) : Base(h) {}
  Hyperplane_d(const Base& p) : Base(p) {}

  template <class InputIterator>
  Hyperplane_d(int d, InputIterator first, InputIterator last)
    : Base (d, first, last) {}

  template <class InputIterator>
  Hyperplane_d(int d, InputIterator first, InputIterator last,
               const RT& D)
    : Base (d, first, last, D) {}

  template <class ForwardIterator>
  Hyperplane_d(ForwardIterator first, ForwardIterator last, 
               const Point_d<R>& o, Oriented_side side = Oriented_side(0)) :
    Base(first,last,o,side) {}

  Vector_d<R> orthogonal_vector() const 
  { return Base::orthogonal_vector(); }
  Direction_d<R> orthogonal_direction() const 
  { return Base::orthogonal_direction(); }

  bool operator==(const Self& w) const
  { return Base::operator==(w); }
  bool operator!=(const Self& w) const
  { return Base::operator!=(w); }
};

CGAL_END_NAMESPACE
#endif //CGAL_HYPERPLANE_D_H
