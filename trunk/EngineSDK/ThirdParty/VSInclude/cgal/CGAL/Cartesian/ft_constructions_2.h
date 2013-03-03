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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/ft_constructions_2.h $
// $Id: ft_constructions_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann

#ifndef CGAL_CARTESIAN_FT_CONSTRUCTIONS_2_H
#define CGAL_CARTESIAN_FT_CONSTRUCTIONS_2_H

CGAL_BEGIN_NAMESPACE

template < class K >
inline
typename K::FT
squared_distance(const PointC2<K> &p,
                 const PointC2<K> &q)
{
  return squared_distanceC2(p.x(), p.y(), q.x(), q.y());
}

template < class K >
inline
typename K::FT
scaled_distance_to_line(const LineC2<K> &l,
                        const PointC2<K> &p)
{
  return scaled_distance_to_lineC2(l.a(), l.b(), l.c(), p.x(), p.y());
}

template < class K >
inline
typename K::FT
scaled_distance_to_line(const PointC2<K> &p,
                        const PointC2<K> &q,
                        const PointC2<K> &r)
{
  return scaled_distance_to_lineC2(p.x(), p.y(), q.x(), q.y(), r.x(), r.y());
}

template < class K >
inline
typename K::FT
line_y_at_x(const LineC2<K> &l, const typename K::FT &x)
{
  return line_y_at_xC2(l.a(), l.b(), l.c(), x);
}

template < class K >
inline
typename K::FT
line_x_at_y(const LineC2<K> &l, const typename K::FT &y)
{
  return line_y_at_xC2(l.b(), l.a(), l.c(), y);
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_FT_CONSTRUCTIONS_2_H
