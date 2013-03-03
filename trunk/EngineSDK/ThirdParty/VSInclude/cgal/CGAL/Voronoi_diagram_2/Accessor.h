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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Voronoi_diagram_2/include/CGAL/Voronoi_diagram_2/Accessor.h $
// $Id: Accessor.h 29163 2006-03-07 23:41:02Z mkaravel $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@iacm.forth.gr>

#ifndef CGAL_VORONOI_DIAGRAM_2_ACCESSOR_H
#define CGAL_VORONOI_DIAGRAM_2_ACCESSOR_H 1

#include <CGAL/Voronoi_diagram_2/basic.h>

CGAL_BEGIN_NAMESPACE

CGAL_VORONOI_DIAGRAM_2_BEGIN_NAMESPACE

template<class VDA>
class Accessor
{
public:
  // TYPES
  //------
  typedef typename VDA::Non_degenerate_faces_iterator
  Non_degenerate_faces_iterator;

  typedef typename VDA::Non_degenerate_edges_iterator
  Non_degenerate_edges_iterator;

  typedef typename VDA::Valid_edges_iterator
  Valid_edges_iterator;

  typedef typename VDA::Edge_rejector  Edge_rejector;
  typedef typename VDA::Face_rejector  Face_rejector;

  typedef typename VDA::Find_valid_vertex      Find_valid_vertex;

  // CONSTRUCTOR
  //------------
  Accessor(VDA* vda) : vda(vda) {}
  Accessor(const VDA* vda) : vda(const_cast<VDA*>(vda)) {}

  // METHODS
  //--------
  VDA* ptr() { return vda; }
  const VDA* ptr() const { return vda; }

private:
  VDA* vda;
};


CGAL_VORONOI_DIAGRAM_2_END_NAMESPACE

CGAL_END_NAMESPACE


#endif // CGAL_VORONOI_DIAGRAM_2_ACCESSOR_H

