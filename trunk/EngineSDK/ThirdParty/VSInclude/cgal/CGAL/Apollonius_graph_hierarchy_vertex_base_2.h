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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Apollonius_graph_2/include/CGAL/Apollonius_graph_hierarchy_vertex_base_2.h $
// $Id: Apollonius_graph_hierarchy_vertex_base_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>


#ifndef CGAL_APOLLONIUS_GRAPH_HIERARCHY_VERTEX_BASE_2_H
#define CGAL_APOLLONIUS_GRAPH_HIERARCHY_VERTEX_BASE_2_H

#include <CGAL/basic.h>

#include <CGAL/Apollonius_graph_short_names_2.h>

CGAL_BEGIN_NAMESPACE

template < class Vbb >
class Apollonius_graph_hierarchy_vertex_base_2
 : public Vbb
{
  typedef Vbb                                                Base;
  typedef typename Base::Apollonius_graph_data_structure_2   Agds;

public:
  typedef typename Base::Site_2             Site_2;
  typedef Agds                              Apollonius_graph_data_structure_2;
  typedef typename Agds::Vertex_handle      Vertex_handle;
  typedef typename Agds::Face_handle        Face_handle;

  template < typename AGDS2 >
  struct Rebind_TDS {
    typedef typename Vbb::template Rebind_TDS<AGDS2>::Other   Vb2;
    typedef Apollonius_graph_hierarchy_vertex_base_2<Vb2>     Other;
  };

  Apollonius_graph_hierarchy_vertex_base_2()
    : Base(), _up(), _down()
    {}
  Apollonius_graph_hierarchy_vertex_base_2(const Site_2& p,
					   Face_handle f)
    : Base(p,f), _up(), _down()
    {}
  Apollonius_graph_hierarchy_vertex_base_2(const Site_2& p)
    : Base(p), _up(), _down()
    {}

  Vertex_handle up() {return _up;}
  Vertex_handle down() {return _down;}
  void set_up(Vertex_handle u) {_up=u;}
  void set_down(Vertex_handle d) {if (this) _down=d;}


 private:
  Vertex_handle  _up;    // same vertex one level above
  Vertex_handle  _down;  // same vertex one level below
};

CGAL_END_NAMESPACE

#endif // CGAL_APOLLONIUS_GRAPH_HIERARCHY_VERTEX_BASE_2_H
