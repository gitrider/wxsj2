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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/Iso_rectangle_2.h $
// $Id: Iso_rectangle_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Andreas Fabri, Herve Bronnimann

#ifndef CGAL_CARTESIAN_ISO_RECTANGLE_2_H
#define CGAL_CARTESIAN_ISO_RECTANGLE_2_H

#include <CGAL/Twotuple.h>

CGAL_BEGIN_NAMESPACE

template <class R_>
class Iso_rectangleC2
{
  typedef typename R_::FT                   FT;
  typedef typename R_::Point_2              Point_2;
  typedef typename R_::Iso_rectangle_2      Iso_rectangle_2;
  typedef typename R_::Aff_transformation_2 Aff_transformation_2;
  typedef typename R_::Construct_point_2    Construct_point_2;

  typedef Twotuple<Point_2>                        Rep;
  typedef typename R_::template Handle<Rep>::type  Base;

  Base base;

public:
  typedef R_                                     R;

  Iso_rectangleC2() {}

  Iso_rectangleC2(const Point_2 &p, const Point_2 &q)
    : base(p,q)
  {}


  const Point_2 & min() const
  {
      return get(base).e0;
  }
  const Point_2 & max() const
  {
      return get(base).e1;
  }

  Iso_rectangle_2 transform(const Aff_transformation_2 &t) const
  {
    // FIXME : We need a precondition like this!!!
    // CGAL_kernel_precondition(t.is_axis_preserving());
    return Iso_rectangle_2(t.transform(min()), t.transform(max()));
  }




};





CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_ISO_RECTANGLE_2_H
