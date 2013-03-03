// Copyright (c) 1997  Tel-Aviv University (Israel).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Sweep_line_2/Arr_basic_addition_visitor.h $
// $Id: Arr_basic_addition_visitor.h 28840 2006-02-27 14:36:55Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_ARR_BASIC_ADDITION_VISITOR_H
#define CGAL_ARR_BASIC_ADDITION_VISITOR_H

CGAL_BEGIN_NAMESPACE

template<class Traits, class Arrangement_, class Event,class Subcurve>
class Arr_basic_addition_visitor : 
  public Arr_construction_visitor<Traits,Arrangement_,Event,Subcurve>
{
protected:

  typedef Arrangement_                                     Arrangement;
  
  typedef Arr_construction_visitor<Traits,
                                   Arrangement,
                                   Event,
                                   Subcurve>               Base;

  typedef Arr_basic_addition_visitor<Traits,
                               Arrangement,
                               Event,
                               Subcurve>                   Self;

  typedef typename Base::SL_iterator                       SL_iterator;
  typedef typename Base::Halfedge_handle                   Halfedge_handle;
  typedef typename Base::Vertex_handle                     Vertex_handle;
  typedef typename Base::SubCurveIter                      SubCurveIter;
  typedef typename Base::SubCurveRevIter                   SubCurveRevIter;
  typedef typename Traits::X_monotone_curve_2              X_monotone_curve_2;
  typedef typename Traits::Point_2                         Point_2;
  
  typedef typename Arrangement::Face_handle                Face_handle;
  typedef typename Arrangement::Face_const_handle          Face_const_handle;
  
public:

  Arr_basic_addition_visitor(Arrangement *arr): Base(arr)
  {}

  void before_handle_event(Event* event)
  {
    Base::before_handle_event(event);
    event->get_is_curve_in_arr().resize(event->get_num_right_curves(),false);

    if(!event->has_right_curves())
    {
      // update the event with the highest left halfedge
      for(SubCurveRevIter iter = event->left_curves_rbegin();
          iter != event->left_curves_rend();
          ++iter)
      {
        Halfedge_handle he;
        if((he =(*iter)->get_last_curve().get_halfedge_handle()) !=
         Halfedge_handle(NULL))
        {
          event->set_halfedge_handle(he->twin());
          return;
        }
      }
    }

    if(!event->has_left_curves())
    {
      int i = 0;
      // indicates if there's halfedge to the right of the event
      for(SubCurveRevIter iter = event->right_curves_rbegin();
          iter != event->right_curves_rend();
          ++iter, ++i)
      {
        // update the event with the highest right halfedge
        Halfedge_handle he;
        if((he = (*iter)->get_last_curve().get_halfedge_handle()) !=
           Halfedge_handle(NULL))
        {
          event->get_is_curve_in_arr()[i] = true;
          if(event->get_halfedge_handle() ==
             Halfedge_handle(NULL))
            event->set_halfedge_handle(he);
        }
      }
      return;
    }

    // the event has left and right curves
    int i = 0;
    bool exist_right_halfedge = false; 
    for(SubCurveRevIter iter = event->right_curves_rbegin();
        iter != event->right_curves_rend();
        ++iter, ++i)
    {
      Halfedge_handle he;
      if((he = (*iter)->get_last_curve().get_halfedge_handle()) !=
         Halfedge_handle(NULL))
      {
        exist_right_halfedge = true;
        event->get_is_curve_in_arr()[i] = true;
        if(!is_split_event(*iter, event))
          // halfedge will not be splitted 
          event->set_halfedge_handle(he);
        else
        {
          he = split_edge((*iter)->get_last_curve().get_halfedge_handle(),
                          (*iter),
                           event->get_point());
          
          // 'he' has the same source as the splitted halfedge
          event->set_halfedge_handle(he);
          X_monotone_curve_2& last_curve =
            const_cast<X_monotone_curve_2&>((*iter)->get_last_curve());
          last_curve.set_halfedge_handle(he);
          
          //there cannot be another existing halfedge that need to be splitted
          // because they are disjoint
          return;
        }
      }
    }

    if(exist_right_halfedge)
    {
      return;
    }
    // if we have reached here, there are no halfedges to the right of 
    // the event, but still can be on the left of the event
    for(SubCurveRevIter iter = event->left_curves_rbegin();
        iter != event->left_curves_rend();
        ++iter)
    {
      Halfedge_handle he;
      if((he =(*iter)->get_last_curve().get_halfedge_handle()) !=
        Halfedge_handle(NULL))
      {
        event->set_halfedge_handle(he->twin());
        return;
      }
    }
  }


  void add_subcurve(const X_monotone_curve_2& cv, Subcurve* sc)
  {
    if(cv.get_halfedge_handle() == Halfedge_handle(NULL))
      Base::add_subcurve(cv,sc);
    else
    {
      // sc is an overlap Subcurve of existing edge and new curve,
      // which means that the edeg will have to be modified
      if (sc->get_orig_subcurve1())
      {
        this->m_arr->modify_edge
          (this->current_event()->get_halfedge_handle()->
           next()->twin(),
           cv.base());
      }

      Halfedge_handle next_ccw_he = 
        this->current_event()->get_halfedge_handle()->next()->twin();
                                                                
      this->current_event()->set_halfedge_handle(next_ccw_he);
    }
  }

  bool after_handle_event(Event* event, SL_iterator iter, bool flag)
  {
    return (Base::after_handle_event(event,iter,flag));
  }


  Halfedge_handle _insert_in_face_interior (const X_monotone_curve_2& cv,
                                            Subcurve* sc)
  {
    Halfedge_handle he_above = ray_shoot_up(sc);
    if(he_above == Halfedge_handle(NULL))
      return (this->m_arr->insert_in_face_interior
              (cv.base(), this->m_arr->unbounded_face()));

    return (this->m_arr->insert_in_face_interior 
            (cv.base(), he_above->face()));
  }

  Halfedge_handle insert_at_vertices (const X_monotone_curve_2& cv,
                                      Halfedge_handle hhandle,
                                      Halfedge_handle prev,
                                      Subcurve* sc,
                                      bool &new_face_created)
  {
    return (this->m_arr->insert_at_vertices (cv.base(), hhandle, prev));
  }

  virtual Halfedge_handle split_edge(Halfedge_handle /*he*/,
                                     Subcurve* /*sc*/,
                                     const Point_2& /*pt*/)
  {
    return Halfedge_handle();
  }
  

  // check if the halfedge associated with 'sc' will be splitted at the given
  // return false.
  virtual bool is_split_event(Subcurve* /*sc*/, Event* /*event*/)
  {
    return false;
  }


  virtual Vertex_handle insert_isolated_vertex(const Point_2& pt,
                                               SL_iterator iter)
  {
    Vertex_handle res(NULL);
    //the isolated vertex is already at the arrangement
    if(pt.get_vertex_handle() != Vertex_handle(NULL))
      return res;
    if(iter == this->status_line_end())
    {
      res = this->m_arr->insert_in_face_interior
        (pt.base(), this->m_arr->unbounded_face());
    }
    else
    {
      Halfedge_handle he = ray_shoot_up(*iter);
      if (he == Halfedge_handle(NULL))
      {
        res = this->m_arr->insert_in_face_interior
          (pt.base(), this->m_arr->unbounded_face());
      }
      else
      {
        res = this->m_arr->insert_in_face_interior (pt.base(), he->face());
      }
    }

    return (res);
  }


  Halfedge_handle ray_shoot_up(Subcurve* sc)
  {
    Halfedge_handle he_above;
    for(SL_iterator iter = this -> status_line_position(sc);
        iter != this -> status_line_end();
        ++iter)
    {
      if((*iter)->get_last_curve().get_halfedge_handle() !=
         Halfedge_handle(NULL))
      {
        he_above = (*iter)->get_last_curve().get_halfedge_handle();
        return (he_above);
      }
    }
  
    return (he_above);
  }




   void update_event(){}

   void update_event(Event* e,
                    const Point_2& end_point,
                    const X_monotone_curve_2& cv,
                    bool is_left_end)
  {}

  void update_event(Event* e,
                    Subcurve* sc1,
                    Subcurve* sc2,
                    bool created = false)
  {}

  void update_event(Event* e,
                    Subcurve* sc1)
  {}


  void update_event(Event* e, const Point_2& pt)
  {
    Vertex_handle vh;
    if(e->get_point().get_vertex_handle() == vh)
      e->get_point().set_vertex_handle(pt.get_vertex_handle());
  }

  virtual Halfedge_handle
    insert_in_face_interior(const X_monotone_curve_2& cv,
                            Subcurve* sc)
  {
    Event *lastEvent = reinterpret_cast<Event*>((sc)->get_last_event());
    Vertex_handle last_v = lastEvent->get_point().get_vertex_handle();
    Vertex_handle curr_v =
      this->current_event()->get_point().get_vertex_handle();
    Vertex_handle null_v;

    if(last_v == null_v && curr_v == null_v)
      return (this->_insert_in_face_interior(cv, sc));
    if(last_v == null_v && curr_v != null_v)
    {
      Halfedge_handle he = this->m_arr->insert_from_right_vertex (cv.base(),
                                                                  curr_v);
      return (he->twin());
    }
    if(last_v != null_v && curr_v == null_v)
      return (this->m_arr->insert_from_left_vertex (cv.base(), last_v));

    CGAL_assertion(last_v != null_v && curr_v != null_v);
    return (this->m_arr->insert_at_vertices (cv.base(), last_v, curr_v));

  }


  virtual Halfedge_handle
    insert_from_right_vertex (const X_monotone_curve_2& cv,
                              Halfedge_handle he,
                              Subcurve* sc)
  {
    Event *lastEvent = reinterpret_cast<Event*>((sc)->get_last_event());
    Vertex_handle last_v = lastEvent->get_point().get_vertex_handle();

    if(last_v != Vertex_handle())
      return (this->m_arr->insert_at_vertices (cv.base(), he, last_v));
    
    return (this->m_arr->insert_from_right_vertex (cv.base(), he));
  }

  virtual Halfedge_handle
    insert_from_left_vertex (const X_monotone_curve_2& cv,
                             Halfedge_handle he,
                             Subcurve* sc)
  {
    Vertex_handle curr_v =
      this->current_event()->get_point().get_vertex_handle();

     if(curr_v != Vertex_handle())
       return (this->m_arr->insert_at_vertices (cv.base(), he, curr_v));

    return (this->m_arr->insert_from_left_vertex (cv.base(), he));
  }

protected:

  X_monotone_curve_2   sub_cv1;         // Auxiliary variable (for splitting).
  X_monotone_curve_2   sub_cv2;         // Auxiliary variable (for splitting).

};


CGAL_END_NAMESPACE

#endif
