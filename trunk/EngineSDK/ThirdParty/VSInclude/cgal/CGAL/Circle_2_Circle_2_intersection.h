
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Intersections_2/include/CGAL/Circle_2_Circle_2_intersection.h $
// $Id: Circle_2_Circle_2_intersection.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Geert-Jan Giezeman


#ifndef CGAL_CIRCLE_2_CIRCLE_2_INTERSECTION_H
#define CGAL_CIRCLE_2_CIRCLE_2_INTERSECTION_H

#include <CGAL/Circle_2.h>
#include <CGAL/Object.h>
#include <CGAL/squared_distance_2_1.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template <class K>
bool
do_intersect(const typename CGAL_WRAP(K)::Circle_2 & circ1, 
	     const typename CGAL_WRAP(K)::Circle_2& circ2,
	     const K&)
{
    typedef typename K::FT FT;
    FT sr1 = circ1.squared_radius();
    FT sr2 = circ2.squared_radius();
    FT squared_dist = squared_distance(circ1.center(), circ2.center());
    FT temp = sr1+sr2-squared_dist;
    return !(FT(4)*sr1*sr2 < temp*temp);
}

} // namespace CGALi


template <class K>
inline
bool
do_intersect(const Circle_2<K> & circ1, 
	     const Circle_2<K> & circ2)
{
  typedef typename K::Do_intersect_2 Do_intersect;
  return Do_intersect()(circ1, circ2);
}


CGAL_END_NAMESPACE

#endif


