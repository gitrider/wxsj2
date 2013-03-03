// Copyright (c) 2003-2004  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Mesh_2/include/CGAL/Delaunay_mesh_criteria_2.h $
// $Id: Delaunay_mesh_criteria_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Laurent RINEAU

#ifndef CGAL_DELAUNAY_MESH_CRITERIA_2_H
#define CGAL_DELAUNAY_MESH_CRITERIA_2_H

#include <CGAL/Mesh_2/Face_badness.h>

namespace CGAL {

template <class Tr>
class Delaunay_mesh_criteria_2
{
  double B;
public:
  typedef typename Tr::Face_handle Face_handle;

  Delaunay_mesh_criteria_2(const double bound = 0.125) : B(bound) {};

  typedef double Quality;

  inline
  double bound() const { return B; };

  inline 
  void set_bound(const double bound) { B = bound; };

  class Is_bad
  {
  protected:
    const double B;
  public:
    typedef typename Tr::Point Point_2;
      
    Is_bad(const double bound) : B(bound) {};
      
    Mesh_2::Face_badness operator()(const Quality q) const
    {
      if( q < B )
	return Mesh_2::BAD;
      else
	return Mesh_2::NOT_BAD;
    }

    Mesh_2::Face_badness operator()(const Face_handle& fh,
				    Quality& q) const
    {
      typedef typename Tr::Geom_traits Geom_traits;
      typedef typename Geom_traits::Compute_area_2 Compute_area_2;
      typedef typename Geom_traits::Compute_squared_distance_2
	Compute_squared_distance_2;
      typedef typename Geom_traits::Construct_triangle_2
	Construct_triangle_2;
      typedef typename Geom_traits::FT FT;

      Geom_traits traits;

      Compute_area_2 area_2 = 
        traits.compute_area_2_object();
      Compute_squared_distance_2 squared_distance = 
	traits.compute_squared_distance_2_object();
      Construct_triangle_2 triangle =
        traits.construct_triangle_2_object();

      const Point_2& pa = fh->vertex(0)->point();
      const Point_2& pb = fh->vertex(1)->point();
      const Point_2& pc = fh->vertex(2)->point();

      double area = 2*CGAL::to_double(area_2(pa, pb, pc));
      area=area*area; // area = 4 * area(triangle)

      double a = CGAL::to_double(squared_distance(pb, pc));
      double b = CGAL::to_double(squared_distance(pc, pa));
      double c = CGAL::to_double(squared_distance(pa, pb));

      if(a<b)
	if(a<c)
	  q = area/(b*c);
	else
	  q = area/(a*b);
      else
	if(b<c)
	  q = area/(a*c);
	else
	  q = area/(a*b);

      return operator()(q);
    };
  };

  Is_bad is_bad_object() const
    { return Is_bad(B); }
};

}; // end namespace CGAL

#endif
