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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_overlay_2/Arr_overlay_traits.h $
// $Id: Arr_overlay_traits.h 28836 2006-02-27 14:35:50Z baruchzu $
// 
//
// Author(s)     : Ron Wein <baruchzu@post.tau.ac.il>

#ifndef CGAL_ARR_OVERLAY_TRAITS_H
#define CGAL_ARR_OVERLAY_TRAITS_H

CGAL_BEGIN_NAMESPACE

/*!
 * \class
 * An overlay-traits class for computing the overlay of two arrangement that
 * are templated with the default DCEL classes, namely they store no extra
 * data with their DCEL features. The resulting arrangement is also assumed
 * to be templated with the default DCEL.
 */
template <class ArrangementA, class ArrangementB, class ArrangementR>
class _Arr_default_overlay_traits
{
public:

  typedef typename ArrangementA::Vertex_const_handle    Vertex_handle_A;
  typedef typename ArrangementA::Halfedge_const_handle  Halfedge_handle_A;
  typedef typename ArrangementA::Face_const_handle      Face_handle_A;

  typedef typename ArrangementB::Vertex_const_handle    Vertex_handle_B;
  typedef typename ArrangementB::Halfedge_const_handle  Halfedge_handle_B;
  typedef typename ArrangementB::Face_const_handle      Face_handle_B;
  
  typedef typename ArrangementR::Vertex_handle          Vertex_handle_R;
  typedef typename ArrangementR::Halfedge_handle        Halfedge_handle_R;
  typedef typename ArrangementR::Face_handle            Face_handle_R;

  /*! Destructor. */
  virtual ~_Arr_default_overlay_traits ()
  {}
  
  /*!
   * Create a vertex v that corresponds to the coinciding vertices v1 and v2.
   */
  virtual void create_vertex (Vertex_handle_A /* v1 */,
			      Vertex_handle_B /* v2 */,
			      Vertex_handle_R /* v */) const
  {}

  /*!
   * Create a vertex v that mathces v1, which lies of the edge e2.
   */
  virtual void create_vertex (Vertex_handle_A /* v1 */,
			      Halfedge_handle_B /* e2 */,
			      Vertex_handle_R /* v */) const
  {}

  /*!
   * Create a vertex v that mathces v1, contained in the face f2.
   */
  virtual void create_vertex (Vertex_handle_A /* v1 */,
			      Face_handle_B /* f2 */,
			      Vertex_handle_R /* v */) const
  {}

  /*!
   * Create a vertex v that mathces v2, which lies of the edge e1.
   */
  virtual void create_vertex (Halfedge_handle_A /* e1 */,
			      Vertex_handle_B /* v2 */,
			      Vertex_handle_R /* v */) const
  {}

  /*!
   * Create a vertex v that mathces v2, contained in the face f1.
   */
  virtual void create_vertex (Face_handle_A /* f1 */,
			      Vertex_handle_B /* v2 */,
			      Vertex_handle_R /* v */) const
  {}

  /*!
   * Create a vertex v that mathces the intersection of the edges e1 and e2.
   */
  virtual void create_vertex (Halfedge_handle_A /* e1 */,
			      Halfedge_handle_B /* e2 */,
			      Vertex_handle_R /* v */) const
  {}

  /*!
   * Create an edge e that matches the overlap between e1 and e2.
   */
  virtual void create_edge (Halfedge_handle_A /* e1 */,
			    Halfedge_handle_B /* e2 */,
			    Halfedge_handle_R /* e */) const
  {}

  /*!
   * Create an edge e that matches the edge e1, contained in the face f2.
   */
  virtual void create_edge (Halfedge_handle_A /* e1 */,
			    Face_handle_B /* f2 */,
			    Halfedge_handle_R /* e */) const
  {}

  /*!
   * Create an edge e that matches the edge e2, contained in the face f1.
   */
  virtual void create_edge (Face_handle_A /* f1 */,
			    Halfedge_handle_B /* e2 */,
			    Halfedge_handle_R /* e */) const
  {}

  /*!
   * Create a face f that matches the overlapping region between f1 and f2.
   */
  virtual void create_face (Face_handle_A /* f1 */,
			    Face_handle_B /* f2 */,
			    Face_handle_R /* f */) const
  {}

};

CGAL_END_NAMESPACE

#endif
