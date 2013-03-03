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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/Point_2.h $
// $Id: Point_2.h 29078 2006-03-06 13:08:09Z spion $
// 
//
// Author(s)     : Andreas Fabri, Herve Bronnimann

#ifndef CGAL_CARTESIAN_POINT_2_H
#define CGAL_CARTESIAN_POINT_2_H

#include <CGAL/Origin.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Twotuple.h>
#include <CGAL/Handle_for.h>

CGAL_BEGIN_NAMESPACE

template < class R_ >
class PointC2
{
  typedef PointC2<R_>                       Self;
  typedef typename R_::FT                   FT;
  typedef typename R_::Vector_2             Vector_2;
  typedef typename R_::Point_2              Point_2;
  typedef typename R_::Aff_transformation_2 Aff_transformation_2;

  typedef Twotuple<FT>	                           Rep;
  typedef typename R_::template Handle<Rep>::type  Base;

  Base base;

  static FT one;

public:
  typedef const FT* Cartesian_const_iterator;
  
  typedef R_                                     R;

  PointC2() {}

  PointC2(const Origin &)
    : base(FT(0), FT(0)) {}

  PointC2(const FT &x, const FT &y)
    : base(x, y) {}

  PointC2(const FT &hx, const FT &hy, const FT &hw)
  {
    if (hw != FT(1))
      base = Rep(hx/hw, hy/hw);
    else
      base = Rep(hx, hy);
  }

  const FT& x() const
  {
      return get(base).e0;
  }
  
  const FT& y() const
  {
      return get(base).e1;
  }

  const FT& hx() const
  {
      return x();
  }
  const FT& hy() const
  {
      return y();
  }
  const FT& hw() const
  {
      return one;
  }


  Cartesian_const_iterator cartesian_begin() const 
  {
    return & get(base).e0; 
  }

  Cartesian_const_iterator cartesian_end() const 
  {
    const FT* ptr = & get(base).e1;
    ptr++;
    return ptr;
  }

  bool operator==(const PointC2 &p) const
  {
      if (CGAL::identical(base, p.base))
	  return true;
      return equal_xy(*this, p);
  }
  bool operator!=(const PointC2 &p) const
  {
      return !(*this == p);
  }


  Point_2 transform(const Aff_transformation_2 &t) const
  {
    return t.transform(static_cast<const Point_2&>(*this));
  }
};

template <class R >
typename R::FT PointC2<R>::one = 1;




CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_POINT_2_H
