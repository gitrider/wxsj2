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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Voronoi_diagram_2/include/CGAL/Voronoi_diagram_2/Default_site_removers.h $
// $Id: Default_site_removers.h 29163 2006-03-07 23:41:02Z mkaravel $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@iacm.forth.gr>

#ifndef CGAL_VORONOI_DIAGRAM_2_SITE_REMOVERS_H
#define CGAL_VORONOI_DIAGRAM_2_SITE_REMOVERS_H 1

#include <CGAL/Voronoi_diagram_2/basic.h>
#include <CGAL/Voronoi_diagram_2/Adaptation_traits_functors.h>

CGAL_BEGIN_NAMESPACE

CGAL_VORONOI_DIAGRAM_2_BEGIN_NAMESPACE

//===========================================================================
//===========================================================================

template<class DG>
struct Default_site_remover
{
  typedef DG                                        Delaunay_graph;
  typedef typename Delaunay_graph::Vertex_handle    Vertex_handle;
  typedef void                                      result_type;
  typedef Arity_tag<2>                              Arity;

  Default_site_remover() {}

  result_type operator()(Delaunay_graph& dg, const Vertex_handle& v) const {
    // this should return a boolean that indicates whether the removal
    // was successful.
    dg.remove(v);
  }
};

//===========================================================================

template<class AT, class SR>
class Default_caching_site_remover
{
private:
  typedef AT  Adaptation_traits;
  typedef SR  Site_remover;

  typedef Triangulation_cw_ccw_2                    CW_CCW_2;

public:
  typedef typename Adaptation_traits::Delaunay_graph   Delaunay_graph;
  typedef typename Site_remover::Vertex_handle         Vertex_handle;
  typedef typename Site_remover::result_type           result_type;
  typedef Arity_tag<2>                                 Arity;

public:
  Default_caching_site_remover(const Adaptation_traits* at = NULL) : at_(at) {}

  result_type operator()(Delaunay_graph& dg, const Vertex_handle& v) const
  {
    typedef typename Delaunay_graph::Edge               Edge;
    typedef typename Delaunay_graph::Face_handle        Face_handle;
    typedef typename Delaunay_graph::Face_circulator    Face_circulator;
    typedef typename Delaunay_graph::Vertex_circulator  Vertex_circulator;

    if ( dg.dimension() != 2 ) { dg.remove(v); }

    Face_circulator fc_start = dg.incident_faces(v);
    Face_circulator fc = fc_start;
    do {
      int id = fc->index(v);
      Edge e_opp(fc, id);
      Edge e_other(fc, CW_CCW_2::ccw(id));
      at_->edge_rejector_object().erase(e_opp);
      at_->edge_rejector_object().erase(e_other);
    } while ( ++fc != fc_start );

    Vertex_circulator vc_start = dg.incident_vertices(v);
    Vertex_circulator vc = vc_start;
    do {
      at_->face_rejector_object().erase(vc);
    } while ( ++fc != fc_start );

    Site_remover()(dg, v);
  }

private:
  const Adaptation_traits* at_;
};

//===========================================================================

template<class AT>
struct Default_caching_site_remover<AT,Null_functor>
{
  Default_caching_site_remover() {}
  template<typename T> Default_caching_site_remover(T t) {}
};

//===========================================================================
//===========================================================================

CGAL_VORONOI_DIAGRAM_2_END_NAMESPACE

CGAL_END_NAMESPACE

#endif // CGAL_VORONOI_DIAGRAM_2_SITE_REMOVERS_H
