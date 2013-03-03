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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/ft_constructions_3.h $
// $Id: ft_constructions_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann

#ifndef CGAL_CARTESIAN_FT_CONSTRUCTIONS_3_H
#define CGAL_CARTESIAN_FT_CONSTRUCTIONS_3_H

#include <CGAL/Cartesian/Point_3.h>
#include <CGAL/Cartesian/Vector_3.h>
#include <CGAL/Cartesian/Plane_3.h>
#include <CGAL/constructions/kernel_ftC3.h>

CGAL_BEGIN_NAMESPACE

template < class K >
inline
typename K::FT
squared_distance(const PointC3<K> &p,
                 const PointC3<K> &q)
{
  return squared_distanceC3(p.x(), p.y(), p.z(), q.x(), q.y(), q.z());
}

template < class K >
inline
typename K::FT
scaled_distance_to_plane(const PlaneC3<K> &h,
                         const PointC3<K> &p)
{
  return scaled_distance_to_planeC3(h.a(), h.b(), h.c(), h.d(),
                                    p.x(), p.y(), p.z());
}

template < class K >
inline
typename K::FT
scaled_distance_to_plane(const PointC3<K> &hp,
                         const PointC3<K> &hq,
                         const PointC3<K> &hr,
                         const PointC3<K> &p)
{
  return scaled_distance_to_planeC3(hp.x(), hp.y(), hp.z(),
                                    hq.x(), hq.y(), hq.z(),
                                    hr.x(), hr.y(), hr.z(),
                                    p.x(), p.y(), p.z());
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_FT_CONSTRUCTIONS_3_H
