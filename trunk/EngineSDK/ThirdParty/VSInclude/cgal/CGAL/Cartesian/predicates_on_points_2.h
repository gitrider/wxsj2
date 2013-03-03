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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/predicates_on_points_2.h $
// $Id: predicates_on_points_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann

#ifndef CGAL_CARTESIAN_PREDICATES_ON_POINTS_2_H
#define CGAL_CARTESIAN_PREDICATES_ON_POINTS_2_H

#include <CGAL/Cartesian/Point_2.h>
#include <CGAL/predicates/kernel_ftC2.h>

CGAL_BEGIN_NAMESPACE

template < class K >
inline
bool
equal_xy(const PointC2<K> &p, const PointC2<K> &q)
{
  return p.x() == q.x() && p.y() == q.y();
}

template < class K >
inline
Comparison_result
compare_deltax_deltay(const PointC2<K>& p,
                      const PointC2<K>& q,
                      const PointC2<K>& r,
                      const PointC2<K>& s)
{
  return compare_deltax_deltayC2(p.x(), q.x(), r.y(), s.y());
}

template < class K >
inline
Comparison_result
compare_yx(const PointC2<K> &p, const PointC2<K> &q)
{
  return compare_lexicographically_xyC2(p.y(), p.x(), q.y(), q.x());
}

template < class K >
inline
Comparison_result
compare_lexicographically_yx(const PointC2<K> &p,
                             const PointC2<K> &q)
{
  return compare_lexicographically_xyC2(p.y(), p.x(), q.y(), q.x());
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_PREDICATES_ON_POINTS_2_H
