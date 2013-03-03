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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/Boolean_set_operations_2/Gps_agg_op_visitor.h $
// $Id: Gps_agg_op_visitor.h 28831 2006-02-27 14:28:18Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>
//                 Ron Wein        <wein@post.tau.ac.il>

#ifndef CGAL_GSP_AGG_OP_VISITOR_H
#define CGAL_GSP_AGG_OP_VISITOR_H

#include <CGAL/Unique_hash_map.h> 
#include <CGAL/Sweep_line_2/Arr_construction_visitor.h>

CGAL_BEGIN_NAMESPACE

template<class Traits, class Arrangement_, class Event,class Subcurve>
class Gps_agg_op_base_visitor : 
  public Arr_construction_visitor<Traits,Arrangement_,Event,Subcurve>
{
  protected:

  typedef Arrangement_                                     Arrangement;
  
  typedef Arr_construction_visitor<Traits,
                                   Arrangement,
                                   Event,
                                   Subcurve>               Base;

  typedef typename Base::SL_iterator                       SL_iterator;
  typedef typename Base::Halfedge_handle                   Halfedge_handle;
  typedef typename Base::Vertex_handle                     Vertex_handle;
  typedef typename Base::SubCurveIter                      SubCurveIter;
  typedef typename Base::SubCurveRevIter                   SubCurveRevIter;
  typedef typename Traits::X_monotone_curve_2              X_monotone_curve_2;
  typedef typename Traits::Point_2                         Point_2;
  
  typedef Unique_hash_map<Halfedge_handle, unsigned int>   Edges_hash;

protected:

  Edges_hash*  m_edges_hash; // maps halfedges to their BC (coundary counter)


public:

  Gps_agg_op_base_visitor(Arrangement *arr,
                          Edges_hash* hash): Base(arr),
                                             m_edges_hash(hash)
  {}

  virtual Halfedge_handle insert_in_face_interior(const X_monotone_curve_2& cv,
                                          Subcurve* sc)
  {
    Halfedge_handle he = 
      Base::insert_in_face_interior(cv, sc);
    insert_edge_to_hash(he, cv);
    return (he);
  }

  virtual Halfedge_handle insert_at_vertices(const X_monotone_curve_2& cv,
                                             Halfedge_handle hhandle,
                                             Halfedge_handle prev,
                                             Subcurve* sc,
                                             bool &new_face_created)
  {
    Halfedge_handle res_he =
      Base::insert_at_vertices(cv, hhandle, prev, sc, new_face_created);
    insert_edge_to_hash(res_he, cv);
    return (res_he);
  }

  virtual Halfedge_handle insert_from_right_vertex
                          (const X_monotone_curve_2& cv,
                           Halfedge_handle he,
                           Subcurve* sc)
  {
    Halfedge_handle res_he = 
      Base::insert_from_right_vertex(cv, he, sc);
    insert_edge_to_hash(res_he, cv);
    return (res_he);
  }

  virtual Halfedge_handle insert_from_left_vertex
                          (const X_monotone_curve_2& cv,
                           Halfedge_handle he,
                           Subcurve* sc)
  {
    Halfedge_handle res_he = 
      Base::insert_from_left_vertex(cv, he, sc);
    insert_edge_to_hash(res_he, cv);
    return (res_he);
  }



private:

  void insert_edge_to_hash(Halfedge_handle he, const X_monotone_curve_2& cv)
  {
    Comparison_result he_dir = he->direction();
    Comparison_result cv_dir =
      this->m_arr->get_traits()->compare_endpoints_xy_2_object()(cv);

    if(he_dir == cv_dir)
    {
      (*m_edges_hash)[he] = cv.data().bc();
      (*m_edges_hash)[he->twin()] = cv.data().twin_bc();
    }
    else
    {
      (*m_edges_hash)[he] = cv.data().twin_bc();
      (*m_edges_hash)[he->twin()] = cv.data().bc();
    }
  }
 

};

template <class BaseEvent_>
class Indexed_event : public BaseEvent_
{
private:

  unsigned int    m_index;

public:

  Indexed_event () :
    BaseEvent_(),
    m_index (0)
  {}

  unsigned int index () const
  {
    return (m_index);
  }

  void set_index (unsigned int index)
  {
    m_index = index;
    return;
  }
};

template<class Traits, class Arrangement_, class Event, class Subcurve>
class Gps_agg_op_visitor : 
  public Gps_agg_op_base_visitor<Traits, Arrangement_, Event, Subcurve>
{
protected:

  typedef Arrangement_                                    Arrangement;

  typedef Gps_agg_op_base_visitor<Traits,
                                  Arrangement,
                                  Event,
                                  Subcurve>               Base;

  typedef typename Base::SL_iterator                       SL_iterator;
  typedef typename Base::Halfedge_handle                   Halfedge_handle;
  typedef typename Base::Vertex_handle                     Vertex_handle;
  typedef typename Base::SubCurveIter                      SubCurveIter;
  typedef typename Base::SubCurveRevIter                   SubCurveRevIter;
  typedef typename Traits::X_monotone_curve_2              X_monotone_curve_2;
  typedef typename Traits::Point_2                         Point_2;
  
protected:

  unsigned int m_event_count; // The number of events so far.
  std::vector<Vertex_handle>   *m_vertices_vec;  // The vertices, sorted in
                                                 // ascending order.

public:

  Gps_agg_op_visitor (Arrangement *arr,
                      typename Base::Edges_hash* hash,
                      std::vector<Vertex_handle>* vertices_vec) :
    Base (arr, hash),
    m_event_count (0),
    m_vertices_vec (vertices_vec)
  {}

  void before_handle_event (Event* event)
  {
    event->set_index (m_event_count);
    m_event_count++;

    return;
  }

  virtual Halfedge_handle 
  insert_in_face_interior (const X_monotone_curve_2& cv,
                           Subcurve* sc)
  {
    Halfedge_handle res_he = Base::insert_in_face_interior(cv, sc);

    // We now have a halfedge whose source vertex is associated with the
    // last event and whose target vertex is associated with the current event:
    Event *curr_event = reinterpret_cast<Event*>(this->current_event());
    Event *last_event = reinterpret_cast<Event*>((sc)->get_last_event());

    CGAL_assertion (res_he->direction() == SMALLER);
    _insert_vertex (curr_event, res_he->target());
    _insert_vertex (last_event, res_he->source());

    return (res_he);
  }

  virtual Halfedge_handle
  insert_from_right_vertex (const X_monotone_curve_2& cv,
                            Halfedge_handle he,
                            Subcurve* sc)
  {
    Halfedge_handle  res_he = Base::insert_from_right_vertex (cv, he, sc);

    // We now have a halfedge whose target vertex is associated with the
    // last event (we have already dealt with its source vertex).
    Event *last_event = reinterpret_cast<Event*>((sc)->get_last_event());

    CGAL_assertion (res_he->direction() == LARGER);
    _insert_vertex (last_event, res_he->target());

    return (res_he);
  }

  virtual Halfedge_handle
  insert_from_left_vertex (const X_monotone_curve_2& cv,
                           Halfedge_handle he,
                           Subcurve* sc)
  {
    Halfedge_handle  res_he = Base::insert_from_left_vertex (cv, he, sc);

    // We now have a halfedge whose target vertex is associated with the
    // current event (we have already dealt with its source vertex).
    Event *curr_event = reinterpret_cast<Event*>(this->current_event());

    CGAL_assertion (res_he->direction() == SMALLER);
    _insert_vertex (curr_event, res_he->target());

    return (res_he);
  }

private:

  void _insert_vertex (const Event* event,
                       Vertex_handle v)
  {
    const unsigned int    index = event->index();
    
    if (index >= m_vertices_vec->size())
      m_vertices_vec->resize (2 * (index + 1));

    (*m_vertices_vec)[index] = v;
    return;
  }

};

CGAL_END_NAMESPACE

#endif
