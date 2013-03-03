// Copyright (c) 2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/Line_2.h $
// $Id: Line_2.h 29078 2006-03-06 13:08:09Z spion $
// 
//
// Author(s)     : Andreas Fabri, Herve Bronnimann

#ifndef CGAL_CARTESIAN_LINE_2_H
#define CGAL_CARTESIAN_LINE_2_H

#include <CGAL/Threetuple.h>
#include <CGAL/Handle_for.h>
#include <CGAL/predicates/kernel_ftC2.h>

CGAL_BEGIN_NAMESPACE

template < class R_ >
class LineC2
{
  typedef typename R_::FT                   FT;
  typedef typename R_::Point_2              Point_2;
  typedef typename R_::Direction_2          Direction_2;
  typedef typename R_::Vector_2             Vector_2;
  typedef typename R_::Ray_2                Ray_2;
  typedef typename R_::Segment_2            Segment_2;
  typedef typename R_::Line_2               Line_2;


  typedef Threetuple<FT>	                   Rep;
  typedef typename R_::template Handle<Rep>::type  Base;

  Base base;

public:
  typedef R_                                     R;

  LineC2() {}

  LineC2(const FT &a, const FT &b, const FT &c)
    : base(a, b, c) {}
  
  bool            operator==(const LineC2 &l) const;
  bool            operator!=(const LineC2 &l) const;

  const FT & a() const
  {
      return get(base).e0;
  }
  const FT & b() const
  {
      return get(base).e1;
  }
  const FT & c() const
  {
      return get(base).e2;
  }



};

template < class R >
CGAL_KERNEL_INLINE
bool
LineC2<R>::operator==(const LineC2<R> &l) const
{
  if (CGAL::identical(base, l.base))
      return true;
  return equal_lineC2(a(), b(), c(), l.a(), l.b(), l.c());
}

template < class R >
inline
bool
LineC2<R>::operator!=(const LineC2<R> &l) const
{
  return ! (*this == l);
}


CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_LINE_2_H
