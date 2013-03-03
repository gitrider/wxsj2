// Copyright (c) 2005  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Surface_mesher/include/CGAL/Point_with_surface_index_geom_traits.h $
// $Id: Point_with_surface_index_geom_traits.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Laurent RINEAU

#ifndef CGAL_POINT_WITH_SURFACE_INDEX_GEOM_TRAITS_H
#define CGAL_POINT_WITH_SURFACE_INDEX_GEOM_TRAITS_H

#include <CGAL/Point_with_surface_index.h>

namespace CGAL {

template <class GT>
class Point_with_surface_index_geom_traits : public GT
{
  typedef typename GT::Point_3 Old_point_3;

public:
  typedef Point_with_surface_index<Old_point_3> Point_3;

};  // end Point_with_surface_index_geom_traits

} // end namespace CGAL

#endif // CGAL_POINT_WITH_SURFACE_INDEX_GEOM_TRAITS_H
