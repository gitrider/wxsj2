// Copyright (c) 2006 Foundation for Research and Technology-Hellas (Greece).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Voronoi_diagram_2/include/CGAL/Voronoi_diagram_2/Segment_Voronoi_diagram_nearest_site_2.h $
// $Id: Segment_Voronoi_diagram_nearest_site_2.h 29163 2006-03-07 23:41:02Z mkaravel $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@iacm.forth.gr>

#ifndef CGAL_VORONOI_DIAGRAM_2_SEGMENT_VORONOI_DIAGRAM_NEAREST_SITE_2_H
#define CGAL_VORONOI_DIAGRAM_2_SEGMENT_VORONOI_DIAGRAM_NEAREST_SITE_2_H 1

#include <CGAL/Voronoi_diagram_2/basic.h>
#include <CGAL/Triangulation_utils_2.h>

#include <boost/variant.hpp>

CGAL_BEGIN_NAMESPACE

CGAL_VORONOI_DIAGRAM_2_BEGIN_NAMESPACE

//=========================================================================
//=========================================================================

template<class DG>
class Segment_Voronoi_diagram_nearest_site_2
{
 public:
  typedef DG                                          Delaunay_graph;
  typedef typename Delaunay_graph::Point_2            Point_2;
  typedef Arity_tag<2>                                Arity;

 private:
  typedef Triangulation_cw_ccw_2                      CW_CCW_2;

  typedef typename Delaunay_graph::Site_2             Site_2;
  typedef typename Delaunay_graph::Vertex_handle      Vertex_handle;
  typedef typename Delaunay_graph::Face_handle        Face_handle;
  typedef typename Delaunay_graph::Face_circulator    Face_circulator;
  typedef typename Delaunay_graph::Edge               Edge;
  typedef typename Delaunay_graph::Geom_traits        Geom_traits;

 public:
  typedef boost::variant<Vertex_handle,Edge,Face_handle>  result_type;

  result_type operator()(const Delaunay_graph& dg, const Point_2& p) const {
    CGAL_precondition( dg.dimension() >= 0 );

    typename Geom_traits::Oriented_side_of_bisector_2 side_of_bisector =
      dg.geom_traits().oriented_side_of_bisector_2_object();

    typename Geom_traits::Equal_2 is_equal =
      dg.geom_traits().equal_2_object();

    Site_2 sp = Site_2::construct_site_2(p);

    Vertex_handle v = dg.nearest_neighbor(p);
    if ( dg.dimension() == 0 ) {
      return v;
    }

    if ( dg.dimension() == 1 ) {
      Edge e = *dg.finite_edges_begin();
      Vertex_handle v1 = e.first->vertex(CW_CCW_2::ccw(e.second));
      Vertex_handle v2 = e.first->vertex(CW_CCW_2::cw(e.second) );

      Oriented_side os = side_of_bisector(v1->site(), v2->site(), sp);
      
      if ( os == ON_ORIENTED_BOUNDARY ) {
	return e;
      } else {
	return v;
      }
    }

    CGAL_assertion( dg.dimension() == 2 );

    Face_circulator fc_start = dg.incident_faces(v);
    Face_circulator fc = fc_start;

    // first check if the point lies on a Voronoi vertex
    do {
      int index = fc->index(v);
      Vertex_handle v1 = fc->vertex(CW_CCW_2::ccw(index));
      Vertex_handle v2 = fc->vertex(CW_CCW_2::cw(index) );

      Oriented_side os1 = ON_POSITIVE_SIDE, os2 = ON_POSITIVE_SIDE;

      // check if the query point is identical to an endpoint of a
      // segment that has a Voronoi face with zero area.
      if ( !dg.is_infinite(v1) && !dg.is_infinite(v2) ) {
	if ( v->is_point() && is_equal(v->site(), sp) &&
	     v1->is_segment() && v2->is_segment() ) {
	  bool b1 =
	    is_equal(v->site(), v1->site().source_site()) ||
	    is_equal(v->site(), v1->site().target_site());
	  bool b2 =
	    is_equal(v->site(), v2->site().source_site()) ||
	    is_equal(v->site(), v2->site().target_site());

	  if ( b1 && b2 ) { 
	    return Face_handle(fc);
	  }
	}
      }

      // do the generic check now
      if ( !dg.is_infinite(v1) ) {
	os1 = side_of_bisector(v->site(), v1->site(), sp);
      }
      if ( !dg.is_infinite(v2) ) {
	os2 = side_of_bisector(v->site(), v2->site(), sp);
      }

      CGAL_assertion( os1 != ON_NEGATIVE_SIDE );
      CGAL_assertion( os2 != ON_NEGATIVE_SIDE );

      if ( os1 == ON_ORIENTED_BOUNDARY && os2 == ON_ORIENTED_BOUNDARY ) {
	return fc;//Face_handle(fc);
      }

      ++fc;
    } while ( fc != fc_start );

    // now check if the point lies on a Voronoi edge
    fc_start = dg.incident_faces(v);
    fc = fc_start;
    do {
      int index = fc->index(v);
      Vertex_handle v1 = fc->vertex(CW_CCW_2::ccw(index));
      Vertex_handle v2 = fc->vertex(CW_CCW_2::cw(index) );

      Oriented_side os1 = ON_POSITIVE_SIDE, os2 = ON_POSITIVE_SIDE;

      // check if the query point is identical to an endpoint of a
      // segment that has a Voronoi face with zero area.
      if ( !dg.is_infinite(v1) && !dg.is_infinite(v2) ) {
	if ( v->is_point() && is_equal(v->site(), sp) &&
	     v1->is_segment() && v2->is_segment() ) {
	  bool b1 =
	    is_equal(v->site(), v1->site().source_site()) ||
	    is_equal(v->site(), v1->site().target_site());
	  bool b2 =
	    is_equal(v->site(), v2->site().source_site()) ||
	    is_equal(v->site(), v2->site().target_site());

	  CGAL_assertion( !b1 || !b2 );

	  if ( b1 ) {
	    Face_handle f(fc);
	    return Edge(f, CW_CCW_2::cw(index));
	  } else if ( b2 ) {
	    Face_handle f(fc);
	    return Edge(f, CW_CCW_2::ccw(index));
	  }
	}
      }

      // check if the query point is lies on the bisector between a
      // segment and its endpoint
      if ( !dg.is_infinite(v1) ) {
	if ( v->is_point() && is_equal(v->site(), sp) && v1->is_segment() ) {
	  bool b =
	    is_equal(v->site(), v1->site().source_site()) ||
	    is_equal(v->site(), v1->site().target_site());

	  if ( b ) {
	    Face_handle f(fc);
	    return Edge(f, CW_CCW_2::cw(index));
	  }
	}
      }

      if ( !dg.is_infinite(v2) ) {
	if ( v->is_point() && is_equal(v->site(), sp) && v2->is_segment() ) {
	  bool b =
	    is_equal(v->site(), v2->site().source_site()) ||
	    is_equal(v->site(), v2->site().target_site());

	  if ( b ) {
	    Face_handle f(fc);
	    return Edge(f, CW_CCW_2::ccw(index));
	  }
	}
      }

      // do the generic check now
      if ( !dg.is_infinite(v1) ) {
	os1 = side_of_bisector(v->site(), v1->site(), sp);
      }
      if ( !dg.is_infinite(v2) ) {
	os2 = side_of_bisector(v->site(), v2->site(), sp);
      }

      CGAL_assertion( os1 != ON_NEGATIVE_SIDE );
      CGAL_assertion( os2 != ON_NEGATIVE_SIDE );
      CGAL_assertion( os1 != ON_ORIENTED_BOUNDARY ||
		      os2 != ON_ORIENTED_BOUNDARY );

      if ( os1 == ON_ORIENTED_BOUNDARY ) {
	Face_handle f(fc);
	return Edge(f, CW_CCW_2::cw(index));
      } else if ( os2 == ON_ORIENTED_BOUNDARY ) {
	Face_handle f(fc);
	return Edge(f, CW_CCW_2::ccw(index));
      }

      ++fc;
    } while ( fc != fc_start );

    return v;
  }

};


//=========================================================================
//=========================================================================

CGAL_VORONOI_DIAGRAM_2_END_NAMESPACE

CGAL_END_NAMESPACE

#endif // CGAL_VORONOI_DIAGRAM_2_SEGMENT_VORONOI_DIAGRAM_NEAREST_SITE_2_H
