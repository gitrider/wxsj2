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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/Boolean_set_operations_2/Curve_with_halfedge.h $
// $Id: Curve_with_halfedge.h 28831 2006-02-27 14:28:18Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_CURVE_WITH_HALFEDGE_H
#define CGAL_CURVE_WITH_HALFEDGE_H

CGAL_BEGIN_NAMESPACE

template <class Arrangement_>
class Curve_with_halfedge
{
protected:
  typedef typename Arrangement_::Halfedge_handle        Halfedge_handle;
  typedef typename Arrangement_::Halfedge_const_handle  Halfedge_const_handle;
  
public:
  Halfedge_handle  m_he;

  Curve_with_halfedge()
  {};

  Curve_with_halfedge(Halfedge_handle he) : m_he(he)
  {}

  Halfedge_handle halfedge() const
  {
    return (m_he);
  }

  Halfedge_handle halfedge()
  {
    return (m_he);
  }

  void set_halfedge(Halfedge_handle he)
  {
    m_he = he;
  }
};

CGAL_END_NAMESPACE
#endif
