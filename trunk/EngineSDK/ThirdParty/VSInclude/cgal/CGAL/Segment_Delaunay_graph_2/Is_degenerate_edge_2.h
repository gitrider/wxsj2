// Copyright (c) 2003,2004,2005  INRIA Sophia-Antipolis (France) and
// Notre Dame University (U.S.A.).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Segment_Delaunay_graph_2/include/CGAL/Segment_Delaunay_graph_2/Is_degenerate_edge_2.h $
// $Id: Is_degenerate_edge_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>




#ifndef CGAL_SEGMENT_DELAUNAY_GRAPH_2_IS_DEGENERATE_EDGE_2_H
#define CGAL_SEGMENT_DELAUNAY_GRAPH_2_IS_DEGENERATE_EDGE_2_H

#include <CGAL/Segment_Delaunay_graph_2/Are_same_points_C2.h>


CGAL_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------



template<class K, class Method_tag>
class Sdg_is_degenerate_edge_C2
{
public:
  typedef typename K::Site_2      Site_2;

private:
  typedef CGAL::Sdg_voronoi_vertex_2<K,Method_tag>  Voronoi_vertex_2;

  typedef Sdg_are_same_points_C2<K>   Are_same_points_C2;

  bool is_endpoint(const Site_2& p, const Site_2& s) const
  {
    CGAL_precondition( p.is_point() && s.is_segment() );
    Are_same_points_C2 same_points;

    return
      same_points(p, s.source_site()) || same_points(p, s.target_site());
  }

public:
  typedef bool          result_type;
  typedef Site_2        argument_type;
  typedef Arity_tag<4>  Arity;

  bool operator()(const Site_2& p, const Site_2& q,
		  const Site_2& r, const Site_2& s) const
  {
    Voronoi_vertex_2 vpqr(p, q, r);
    if ( vpqr.incircle_no_easy(s) == POSITIVE ) { return false; }

    Voronoi_vertex_2 vqps(q, p, s);
    if ( vqps.incircle_no_easy(r) == POSITIVE ) { return false; }

    return true;
  }

};


//-----------------------------------------------------------------------------

CGAL_END_NAMESPACE

#endif // CGAL_SEGMENT_DELAUNAY_GRAPH_2_IS_DEGENERATE_EDGE_2_H
