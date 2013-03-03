// Copyright (c) 1999  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Homogeneous_kernel/include/CGAL/Homogeneous/basic_constructionsH3.h $
// $Id: basic_constructionsH3.h 29102 2006-03-06 23:51:27Z spion $
// 
//
// Author(s)     : Stefan Schirra
 

#ifndef CGAL_BASIC_CONSTRUCTIONSH3_H
#define CGAL_BASIC_CONSTRUCTIONSH3_H

CGAL_BEGIN_NAMESPACE

template <class R>
typename R::Point_3
_projection(const typename R::Point_3& p, const PlaneH3<R>& pl)
{
  typedef typename R::RT RT;
  if ( pl.has_on(p) ) return p;

  RT A = pl.a();
  RT B = pl.b();
  RT C = pl.c();
  RT D = pl.d();
  RT phx = p.hx();
  RT phy = p.hy();
  RT phz = p.hz();
  RT phw = p.hw();

  RT num = A * phx  +  B * phy  +  C * phz  +  D * phw;
  RT den = A * A    +  B * B    +  C * C;

  return typename R::Point_3( num * A  -  den * phx,
                              num * B  -  den * phy,
                              num * C  -  den * phz,
                             -den );
}

template <class R>
typename R::Point_3
gp_linear_intersection(const PlaneH3<R> &f,
                       const PlaneH3<R> &g,
                       const PlaneH3<R> &h)
{
  typedef typename R::RT RT;
  return typename R::Point_3(
                  det3x3_by_formula<RT>(-f.d(), f.b(), f.c(),
                                        -g.d(), g.b(), g.c(),
                                        -h.d(), h.b(), h.c()),
                  det3x3_by_formula<RT>( f.a(),-f.d(), f.c(),
                                         g.a(),-g.d(), g.c(),
                                         h.a(),-h.d(), h.c()),
                  det3x3_by_formula<RT>( f.a(), f.b(),-f.d(),
                                         g.a(), g.b(),-g.d(),
                                         h.a(), h.b(),-h.d()),
                  det3x3_by_formula<RT>( f.a(), f.b(), f.c(),
                                         g.a(), g.b(), g.c(),
                                         h.a(), h.b(), h.c()));
}

template <class R>
CGAL_KERNEL_INLINE
typename R::FT
squared_distance( PointH3<R> const& p, PointH3<R> const& q)
{ return (p-q)*(p-q); }

CGAL_END_NAMESPACE

#endif // CGAL_BASIC_CONSTRUCTIONSH3_H
