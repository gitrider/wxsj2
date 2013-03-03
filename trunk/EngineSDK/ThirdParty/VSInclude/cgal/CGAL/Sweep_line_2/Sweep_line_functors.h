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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Sweep_line_2/Sweep_line_functors.h $
// $Id: Sweep_line_functors.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Tali Zvi <talizvi@post.tau.ac.il>,
//                 Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_SWEEP_LINE_FUNCTORS_H
#define CGAL_SWEEP_LINE_FUNCTORS_H

#include <CGAL/assertions.h>
#include <CGAL/enum.h>
//#include <CGAL/Sweep_line_2/Sweep_line_event.h>

CGAL_BEGIN_NAMESPACE

template <class SweepLineTraits_2, class Event>
class Event_less_functor 
{
public:
  typedef SweepLineTraits_2           Traits;
  typedef typename Traits::Point_2    Point_2;
  
  Event_less_functor(Traits * t) : m_traits(t)
  {}
  
  Comparison_result operator()(const Event* e1, const Event* e2) const
  {
    return (m_traits->compare_xy_2_object()(e1->get_point(), e2->get_point()));
  }

  Comparison_result operator()(const Point_2& pt, const Event* e) const
  {
    return (m_traits->compare_xy_2_object()(pt, e->get_point()));
  }

private:

  /*! a pointer to a traits object */
  Traits * m_traits;
};


// forward decleration for Sweep_line_event
template <class Traits_, class Subcurve_>
class Sweep_line_event;


template <class SweepLineTraits_2, class Subcurve> 
class Status_line_curve_less_functor 
{
public:
  typedef SweepLineTraits_2 Traits;
  typedef typename Traits::Point_2 Point_2;
  typedef typename Traits::X_monotone_curve_2 X_monotone_curve_2;
  typedef Sweep_line_event<Traits, Subcurve>  Event;
  

  template <class Sweep_event>
  Status_line_curve_less_functor(Traits *t, Sweep_event** e_ptr) : m_traits(t),
                                                            m_curr_event(reinterpret_cast<Event**>(e_ptr))
  {}

  // this comprator should never be called in release mode (only debug mode)
  Comparison_result operator()(const Subcurve * c1, const Subcurve * c2) const
  {
    if(std::find((*m_curr_event)->right_curves_begin(), 
                 (*m_curr_event)->right_curves_end(),
                 c1) != (*m_curr_event)->right_curves_end() &&
       std::find((*m_curr_event)->right_curves_begin(), 
                 (*m_curr_event)->right_curves_end(),
                 c2) != (*m_curr_event)->right_curves_end())

     return m_traits->compare_y_at_x_right_2_object()
       (c1->get_last_curve(), c2->get_last_curve(), (*m_curr_event)->get_point());

    return m_traits->compare_y_at_x_2_object()
      (m_traits->construct_min_vertex_2_object()(c1->get_last_curve()),
       c2->get_last_curve());
  }

  Comparison_result operator()(const Point_2& pt, const Subcurve * c2) const
  {
    return (m_traits->compare_y_at_x_2_object()(pt,c2->get_last_curve()));
  }

  /*! a pointer to a traits object */
  Traits *  m_traits;

  Event**   m_curr_event;  
};


template <class _Subcurve>
class Curves_pair
{
  public:
  Curves_pair(){}

  Curves_pair (_Subcurve* sc1, _Subcurve* sc2)
  {
    //the smallest pointer will be the first 
    if(sc1 < sc2)
      m_curves_pair = std::make_pair(sc1,sc2);
    else
      m_curves_pair = std::make_pair(sc2,sc1);
  }

  _Subcurve* first() const {return m_curves_pair.first;}
  _Subcurve* second() const {return m_curves_pair.second;}

 private:
  std::pair<_Subcurve*, _Subcurve*> m_curves_pair;
};

template <class _Subcurve>
class Curves_pair_less_functor
{
  typedef Curves_pair<_Subcurve> CurvesPair;

public:

  bool operator()(const CurvesPair& pair1, const CurvesPair& pair2) const
  {
    if(pair1.first() < pair2.first())
      return true;
    if(pair1.first() > pair2.first())
      return false;
    if(pair1.second() < pair2.second())
      return true;
    return false;
  }
};

template <class _Subcurve>
class Curves_pair_hash_functor
{
  typedef Curves_pair<_Subcurve> CurvesPair;

public:

  size_t operator() (const CurvesPair& pair) const
  {
    const size_t  half_n_bits = sizeof(size_t) * 8 / 2;
    const size_t  val1 = reinterpret_cast<size_t> (pair.first());
    const size_t  val2 = reinterpret_cast<size_t> (pair.second());

    return (((val1 << half_n_bits) | (val1 >> half_n_bits)) ^ val2);
  }
};

template <class _Subcurve>
class Curves_pair_equal_functor
{
  typedef Curves_pair<_Subcurve> CurvesPair;

public:

  bool operator() (const CurvesPair& pair1, const CurvesPair& pair2) const
  {
    return (pair1.first() == pair2.first() &&
            pair1.second() == pair2.second());
  }
};

template <class Container>
class random_access_input_iterator
{
  public:
  typedef typename Container::value_type  value_type;

  random_access_input_iterator()
  {}

  random_access_input_iterator(Container& _container, unsigned int _index = 0):
      m_container(&_container),
      m_index(_index)
  {}

  value_type& operator*()
  {
    if(m_index >= m_container->capacity())
    {
      m_container->reserve(2 * m_index + 1);
      m_container->resize(m_index+1);
    }
    else
      if(m_index >= m_container->size())
         m_container->resize(m_index+1);
    return (*m_container)[m_index];
  }

  random_access_input_iterator& operator++()
  {
    ++m_index;
    return *this;
  }

  random_access_input_iterator operator++(int)
  {
    random_access_input_iterator temp(*this);
    ++(*this);
    return temp;
  }

  random_access_input_iterator& operator--()
  {
    --m_index;
    return *this;
  }

  bool operator==(const random_access_input_iterator& raii)
  {
    CGAL_precondition(m_container == raii.m_container);
    return (m_index == raii.m_index);
  }

  bool operator!=(const random_access_input_iterator& raii)
  {
    CGAL_precondition(m_container == raii.m_container);
    return !(*this == raii);
  }

  unsigned int operator-(const random_access_input_iterator& raii)
  {
    CGAL_precondition(m_container == raii.m_container);
    return (m_index - raii.m_index);
  }


private:
  Container* m_container;
  unsigned int m_index;
};





CGAL_END_NAMESPACE

#endif
