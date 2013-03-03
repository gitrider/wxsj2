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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Voronoi_diagram_2/include/CGAL/Voronoi_diagram_2/Degeneracy_tester_binders.h $
// $Id: Degeneracy_tester_binders.h 29163 2006-03-07 23:41:02Z mkaravel $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@iacm.forth.gr>

#ifndef CGAL_VORONOI_DIAGRAM_2_DEGENERACY_TESTER_BINDERS_H
#define CGAL_VORONOI_DIAGRAM_2_DEGENERACY_TESTER_BINDERS_H 1

#include <CGAL/Voronoi_diagram_2/basic.h>

CGAL_BEGIN_NAMESPACE

CGAL_VORONOI_DIAGRAM_2_BEGIN_NAMESPACE

//=======================================================================

template<class VDA>
class Edge_rejector_binder
{
private:
  typedef typename VDA::Accessor::Edge_rejector  ER;

public:
  typedef typename ER::result_type               result_type;
  typedef Arity_tag<1>                           Arity;

  Edge_rejector_binder(const VDA* vda = NULL) : vda_(vda) {}

  template<class A>
  bool operator()(const A& a) const {
    CGAL_precondition( vda_ != NULL );
    return vda_->edge_rejector()(vda_->dual(), a);
  }

private:
  const VDA* vda_;
};

//=======================================================================

template<class VDA>
class Face_rejector_binder
{
private:
  typedef typename VDA::Accessor::Face_rejector  FR;

public:
  typedef typename FR::result_type               result_type;
  typedef Arity_tag<1>                           Arity;

  Face_rejector_binder(const VDA* vda = NULL) : vda_(vda) {}

  template<class A>
  bool operator()(const A& a) const {
    CGAL_precondition( vda_ != NULL );
    return vda_->face_rejector()(vda_->dual(), a);
  }

private:
  const VDA* vda_;
};

//=======================================================================

CGAL_VORONOI_DIAGRAM_2_END_NAMESPACE

CGAL_END_NAMESPACE

#endif // CGAL_VORONOI_DIAGRAM_2_DEGENERACY_TESTER_BINDERS_H
