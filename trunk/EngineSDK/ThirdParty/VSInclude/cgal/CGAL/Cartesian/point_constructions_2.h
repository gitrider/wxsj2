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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/point_constructions_2.h $
// $Id: point_constructions_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann

#ifndef CGAL_CARTESIAN_POINT_CONSTRUCTIONS_2_H
#define CGAL_CARTESIAN_POINT_CONSTRUCTIONS_2_H

#include <CGAL/Cartesian/Point_2.h>

CGAL_BEGIN_NAMESPACE

template < class K >
inline
typename K::Point_2
line_get_point(const LineC2<K> &l, int i)
{
  typename K::FT x, y;
  line_get_pointC2(l.a(), l.b(), l.c(), i, x, y);
  return PointC2<K>(x, y);
}

template < class K >
inline
typename K::Point_2
line_project_point(const LineC2<K> &l,
                   const PointC2<K> &p)
{
  typename K::FT x, y;
  line_project_pointC2(l.a(), l.b(), l.c(), p.x(), p.y(), x, y);
  return PointC2<K>(x, y);
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_POINT_CONSTRUCTIONS_2_H
