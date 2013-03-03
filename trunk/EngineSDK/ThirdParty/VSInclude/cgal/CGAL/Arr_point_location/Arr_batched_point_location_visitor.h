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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_point_location/Arr_batched_point_location_visitor.h $
// $Id: Arr_batched_point_location_visitor.h 29926 2006-04-04 12:11:18Z wein $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_ARR_BATCHED_POINT_LOCATION_VISITOR_H
#define CGAL_ARR_BATCHED_POINT_LOCATION_VISITOR_H

CGAL_BEGIN_NAMESPACE


#include <CGAL/Sweep_line_2_empty_visitor.h>
#include <CGAL/Object.h>
#include <utility>


template< class Traits_, class OutputIerator, class Arrangement_ >
class Arr_batched_point_location_visitor : public Empty_visitor< Traits_ >
{
  typedef Arr_batched_point_location_visitor<Traits_,
                                             OutputIerator,
                                             Arrangement_>        Self;   
  typedef Arrangement_                                            Arrangement;
  typedef Traits_                                                 Traits;
  

  typedef Empty_visitor<Traits>                        Base;
  typedef typename Base::Event                         Event;
  typedef typename Base::Subcurve                      Subcurve;
  typedef typename Base::SL_iterator                   SL_iterator;

   
  typedef typename Traits::X_monotone_curve_2            X_monotone_curve_2;
  typedef typename Traits::Point_2                       Point_2;
  typedef typename Traits::Base_Point_2                  Base_Point_2;
  typedef typename Arrangement::Halfedge_const_handle    Halfedge_const_handle;
  typedef typename Arrangement::Vertex_const_handle      Vertex_const_handle;
  typedef std::pair<Base_Point_2,Object>                 PL_Pair;


  public:


  Arr_batched_point_location_visitor(OutputIerator out,
                                     const Arrangement& arr):
                                     m_out(out),
                                     m_arr(arr)
  {}

  

  //after_handle_event
  //(above_on_event is true iff 'above' subcurve is on the event
  bool after_handle_event(Event* event, SL_iterator above, bool above_on_event)
  {
    if(! event->is_query())
      return true;


    // ISOLATED VERTEX
    if(event->is_action())
    {
      Vertex_const_handle vh = event->get_point().get_vertex_handle();
      *m_out = std::make_pair(event->get_point().base_point(), 
                              CGAL::make_object(vh));
      ++m_out;
      return true;
    }

    // VERTEX
    if(event->has_right_curves() || event->has_left_curves())
    {
      if(event->has_right_curves())
      {
        Subcurve* sc = *(event->right_curves_begin());
        Halfedge_const_handle he = sc->get_last_curve().get_halfedge_handle();
        *m_out = std::make_pair(event->get_point().base_point(), 
                                CGAL::make_object(he->target()));
        ++m_out;
        return true;
      }
      Subcurve* sc = *(event->left_curves_begin());
      Halfedge_const_handle he = sc->get_last_curve().get_halfedge_handle();
      *m_out = std::make_pair (event->get_point().base_point(),
                               make_object(he->source()));
      ++m_out;
      return true;
    }

     //UNBOUNDED_FACE
    if(above == this ->status_line_end())
    {
      *m_out = std::make_pair (event->get_point().base_point(),
                               CGAL::make_object(m_arr.unbounded_face()));
      ++m_out;
      return true;
    }

    // EDGE
    if(above_on_event)
    {
      Halfedge_const_handle he = 
        (*above)->get_last_curve().get_halfedge_handle();
      *m_out = std::make_pair (event->get_point().base_point(),
                               make_object(he));
      ++m_out;
      return true;
    }

    //FACE or UNBOUNDED_FACE
    Halfedge_const_handle he = 
      (*above)->get_last_curve().get_halfedge_handle();
    *m_out = std::make_pair (event->get_point().base_point(),
                             CGAL::make_object(he->face()));
    ++m_out;
    return true;
  }

  OutputIerator get_output_iterator()
  {
    return (m_out);
  }

protected:

  OutputIerator      m_out;
  const Arrangement& m_arr;
};



CGAL_END_NAMESPACE

#endif
