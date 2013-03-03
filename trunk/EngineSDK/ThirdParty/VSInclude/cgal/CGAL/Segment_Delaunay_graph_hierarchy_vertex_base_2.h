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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Segment_Delaunay_graph_2/include/CGAL/Segment_Delaunay_graph_hierarchy_vertex_base_2.h $
// $Id: Segment_Delaunay_graph_hierarchy_vertex_base_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>


#ifndef CGAL_SEGMENT_DELAUNAY_GRAPH_HIERARCHY_VERTEX_BASE_2_H
#define CGAL_SEGMENT_DELAUNAY_GRAPH_HIERARCHY_VERTEX_BASE_2_H

#include <CGAL/Segment_Delaunay_graph_2/basic.h>

CGAL_BEGIN_NAMESPACE


template <class Vbb>
class Segment_Delaunay_graph_hierarchy_vertex_base_2
 : public Vbb
{
public:
  typedef Vbb V_Base;
  typedef typename V_Base::Data_structure     DS;

  typedef typename V_Base::Site_2             Site_2;
  typedef typename V_Base::Storage_site_2     Storage_site_2;

  typedef DS                                  Data_structure;

  typedef typename DS::Vertex_handle         Vertex_handle;
  typedef typename DS::Face_handle           Face_handle;

  template < typename DS2 >
  struct Rebind_TDS {
    typedef typename Vbb::template Rebind_TDS<DS2>::Other         Vb2;
    typedef Segment_Delaunay_graph_hierarchy_vertex_base_2<Vb2>   Other;
  };

  Segment_Delaunay_graph_hierarchy_vertex_base_2()
    : V_Base(), up_( Vertex_handle() ), down_( Vertex_handle() ) {}

  Segment_Delaunay_graph_hierarchy_vertex_base_2(const Storage_site_2& ss,
						 Face_handle f)
    : V_Base(ss,f), up_( Vertex_handle() ), down_( Vertex_handle() ) {}

public:  // for use in hierarchy only
  Vertex_handle up()   { return up_; }
  Vertex_handle down() { return down_; }
  void set_up(Vertex_handle u)   { up_ = u; }
  void set_down(Vertex_handle d) { down_ = d; }

private:
  Vertex_handle up_;    // same vertex one level above
  Vertex_handle down_;  // same vertex one level below
};


CGAL_END_NAMESPACE


#endif // CGAL_SEGMENT_DELAUNAY_GRAPH_HIERARCHY_VERTEX_BASE_2_H
