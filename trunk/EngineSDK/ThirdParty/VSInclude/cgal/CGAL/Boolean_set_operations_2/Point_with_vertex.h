// Copyright (c) 2005  Tel-Aviv University (Israel).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/Boolean_set_operations_2/Point_with_vertex.h $
// $Id: Point_with_vertex.h 28831 2006-02-27 14:28:18Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_POINT_WITH_VERTEX_H
#define CGAL_POINT_WITH_VERTEX_H

CGAL_BEGIN_NAMESPACE

template <class Arrangement_>
class Point_with_vertex
{
protected:
  typedef typename Arrangement_::Vertex_handle        Vertex_handle;
  typedef typename Arrangement_::Vertex_const_handle  Vertex_const_handle;
  
public:
  Vertex_handle  m_v;

  Point_with_vertex()
  {};

  Point_with_vertex(Vertex_handle v) : m_v(v)
  {}

  Vertex_handle vertex() const
  {
    return (m_v);
  }

  Vertex_handle vertex()
  {
    return (m_v);
  }

  void set_halfedge(Vertex_handle v)
  {
    m_v = v;
  }
};

CGAL_END_NAMESPACE
#endif
