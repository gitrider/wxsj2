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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Intersections_2/include/CGAL/Point_2_Line_2_intersection.h $
// $Id: Point_2_Line_2_intersection.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Geert-Jan Giezeman


#ifndef CGAL_POINT_2_LINE_2_INTERSECTION_H
#define CGAL_POINT_2_LINE_2_INTERSECTION_H

#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Object.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template <class K>
inline bool
do_intersect(const typename CGAL_WRAP(K)::Point_2 &pt, 
	     const typename CGAL_WRAP(K)::Line_2 &line,
	     const K&)
{
    return line.has_on(pt);
}

template <class K>
inline bool
do_intersect(const typename CGAL_WRAP(K)::Line_2 &line,
	     const typename CGAL_WRAP(K)::Point_2 &pt, 
	     const K&)
{
    return line.has_on(pt);
}

template <class K>
Object
intersection(const typename CGAL_WRAP(K)::Point_2 &pt, 
	     const typename CGAL_WRAP(K)::Line_2 &line,
	     const K& k)
{
    if (do_intersect(pt,line, k)) {
        return make_object(pt);
    }
    return Object();
}

template <class K>
Object
intersection(const typename CGAL_WRAP(K)::Line_2 &line,
	     const typename CGAL_WRAP(K)::Point_2 &pt, 
	     const K& k)
{
    if (do_intersect(pt,line, k)) {
        return make_object(pt);
    }
    return Object();
}

} // namespace CGALi

template <class K>
inline 
bool
do_intersect(const Line_2<K> &line, 
	     const Point_2<K> &pt)
{
  typedef typename K::Do_intersect_2 Do_intersect;
    return Do_intersect()(pt, line);
}

template <class K>
inline 
bool
do_intersect(const Point_2<K> &pt,
	     const Line_2<K> &line)
{
  typedef typename K::Do_intersect_2 Do_intersect;
    return Do_intersect()(pt, line);
}

template <class K>
inline 
Object
intersection(const Line_2<K> &line, 
	     const Point_2<K> &pt)
{
  typedef typename K::Intersect_2 Intersect;
  return Intersect()(pt, line);
}

template <class K>
inline 
Object
intersection(const Point_2<K> &pt,
	     const Line_2<K> &line)
{
  typedef typename K::Intersect_2 Intersect;
  return Intersect()(pt, line);
}

CGAL_END_NAMESPACE

#endif
