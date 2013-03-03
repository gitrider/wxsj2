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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/plane_constructions_3.h $
// $Id: plane_constructions_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann, Sylvain Pion

#ifndef CGAL_CARTESIAN_PLANE_CONSTRUCTIONS_3_H
#define CGAL_CARTESIAN_PLANE_CONSTRUCTIONS_3_H

#include <CGAL/Cartesian/Point_3.h>
#include <CGAL/constructions/kernel_ftC3.h>

CGAL_BEGIN_NAMESPACE

template <class R>
CGAL_KERNEL_LARGE_INLINE
PlaneC3<R>
plane_from_points(const PointC3<R> &p,
                  const PointC3<R> &q,
		  const PointC3<R> &r)
{
  typename R::FT a, b, c, d;
  plane_from_pointsC3(p.x(), p.y(), p.z(),
                      q.x(), q.y(), q.z(),
                      r.x(), r.y(), r.z(),
                      a, b, c, d);
  return PlaneC3<R>(a, b, c, d);
}

template <class R>
CGAL_KERNEL_LARGE_INLINE
PlaneC3<R>
plane_from_point_direction(const PointC3<R> &p,
                           const DirectionC3<R> &d)
{
  typename R::FT A, B, C, D;
  plane_from_point_directionC3(p.x(), p.y(), p.z(), d.dx(), d.dy(), d.dz(),
                               A, B, C, D);
  return PlaneC3<R>(A, B, C, D);
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_PLANE_CONSTRUCTIONS_3_H
