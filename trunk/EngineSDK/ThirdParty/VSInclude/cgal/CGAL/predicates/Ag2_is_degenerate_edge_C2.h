// Copyright (c) 2003,2004  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Apollonius_graph_2/include/CGAL/predicates/Ag2_is_degenerate_edge_C2.h $
// $Id: Ag2_is_degenerate_edge_C2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>



#ifndef CGAL_AG2_IS_DEGENERATE_EDGE_C2_H
#define CGAL_AG2_IS_DEGENERATE_EDGE_C2_H

#include <CGAL/predicates/Apollonius_graph_predicate_constructions_C2.h>

#include <CGAL/predicates/Ag2_incircle_C2.h>
#include <CGAL/predicates/Ag2_finite_edge_test_C2.h>

CGAL_BEGIN_NAMESPACE


//--------------------------------------------------------------------

template < class K, class MTag >
class Is_degenerate_edge_test
{
public:
  typedef K                                 Kernel;
  typedef MTag                              Method_tag;

  typedef typename K::Site_2                Site_2;
  typedef CGAL::Weighted_point_inverter<K>  Weighted_point_inverter;
  typedef CGAL::Inverted_weighted_point<K>  Inverted_weighted_point;
  typedef CGAL::Bitangent_line<K>           Bitangent_line;
  typedef CGAL::Voronoi_circle<K>           Voronoi_circle;
  typedef typename K::FT                    FT;

  typedef CGAL::Sign_of_distance_from_CCW_circle<K>
                                          Sign_of_distance_from_CCW_circle;
  typedef CGAL::Order_on_finite_bisector<K>       Order_on_finite_bisector;

public:
  typedef Site_2             argument_type;
  typedef bool               result_type;
  typedef Arity_tag<4>       Arity;

  bool operator()(const Site_2& p1, const Site_2& p2,
		  const Site_2& p3, const Site_2& p4) const
  {
    Method_tag tag;

    Weighted_point_inverter inverter(p1);
    Inverted_weighted_point u2 = inverter(p2);
    Inverted_weighted_point u3 = inverter(p3);
    Inverted_weighted_point u4 = inverter(p4);

    Sign s;

    Bitangent_line blinv_23(u2, u3);
    s = Sign_of_distance_from_CCW_circle()(blinv_23, u4, tag);
    if ( s != ZERO ) { return false; }

    Bitangent_line blinv_42(u4, u2);
    s = Sign_of_distance_from_CCW_circle()(blinv_42, u3, tag);
    if ( s != ZERO ) { return false; }

    Voronoi_circle vc_123(blinv_23);
    Voronoi_circle vc_142(blinv_42);
    Comparison_result r =
      Order_on_finite_bisector()(vc_123, vc_142, p1, p2, tag);

    return ( r == EQUAL );
  }
};

//--------------------------------------------------------------------

CGAL_END_NAMESPACE

#endif // CGAL_AG2_IS_DEGENERATE_EDGE_C2_H
