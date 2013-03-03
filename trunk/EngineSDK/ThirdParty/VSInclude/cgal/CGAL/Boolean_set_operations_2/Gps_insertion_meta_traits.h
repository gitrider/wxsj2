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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/Boolean_set_operations_2/Gps_insertion_meta_traits.h $
// $Id: Gps_insertion_meta_traits.h 28831 2006-02-27 14:28:18Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_GPS_INSERTION_META_TRAITS_H
#define CGAL_GPS_INSERTION_META_TRAITS_H

#include <CGAL/Boolean_set_operations_2/Gps_traits_decorator.h>
#include <CGAL/Boolean_set_operations_2/Curve_with_halfedge.h>
#include <CGAL/Boolean_set_operations_2/Point_with_vertex.h>

CGAL_BEGIN_NAMESPACE

template <class Arrangement_>
class Gps_insertion_meta_traits : 
  public Gps_traits_decorator<typename Arrangement_::Traits_2,
                              Curve_with_halfedge<Arrangement_>,
                              Point_with_vertex<Arrangement_> >
{
public:
  typedef typename Arrangement_::Traits_2                       Base_traits;
  typedef Gps_traits_decorator<Base_traits,
                               Curve_with_halfedge<Arrangement_>,
                               Point_with_vertex<Arrangement_> >       Base;
  typedef typename Base::Point_2                                Point_2; 
  typedef typename Base::X_monotone_curve_2                     X_monotone_curve_2; 
  typedef typename Base::Curve_data                             Curve_data;
  typedef typename Base::Point_data                             Point_data;
  typedef typename Base_traits::Construct_min_vertex_2          Base_Construct_min_vertex_2;
  typedef typename Base_traits::Construct_max_vertex_2          Base_Construct_max_vertex_2;
  typedef typename Base_traits::Compare_xy_2                    Base_Compare_xy_2;

public:
  Gps_insertion_meta_traits() : Base()
  {}

  Gps_insertion_meta_traits(Base_traits& base_traits) : Base(base_traits)
  {}

  ~Gps_insertion_meta_traits()
  {}

  class Construct_min_vertex_2
  {
  protected:

    Base_Construct_min_vertex_2 m_base;

  public:
    Construct_min_vertex_2(const Base_Construct_min_vertex_2& base) :m_base(base)
    {}

    Point_2 operator() (const X_monotone_curve_2& cv) const
    {
      Point_data pt_info(cv.data().halfedge()->source());
      return Point_2(m_base(cv.base()), pt_info);
    }
  };

  /*! Get a Construct_min_vertex_2 functor object. */
  Construct_min_vertex_2 construct_min_vertex_2_object () const
  {
    return Construct_min_vertex_2(this->m_base_tr->construct_min_vertex_2_object());
  }


  class Construct_max_vertex_2
  {
  protected:

    Base_Construct_max_vertex_2 m_base;

  public:
    Construct_max_vertex_2(const Base_Construct_max_vertex_2& base) :m_base(base)
    {}

    Point_2 operator() (const X_monotone_curve_2& cv) const
    {
      Point_data pt_info(cv.data().halfedge()->target());
      return Point_2(m_base(cv.base()), pt_info);
    }
  };

  /*! Get a Construct_max_vertex_2 functor object. */
  Construct_max_vertex_2 construct_max_vertex_2_object () const
  {
    return Construct_max_vertex_2(this->m_base_tr->construct_max_vertex_2_object());
  }

  class Compare_xy_2
  {
  protected:

    Base_Compare_xy_2 m_base;

  public:
    Compare_xy_2(const Base_Compare_xy_2& base) :m_base(base)
    {}

    Comparison_result operator() (const Point_2& p1, const Point_2& p2) const
    {
      if(p1.data().vertex() == p2.data().vertex())
        return (EQUAL);
      return (m_base(p1.base(), p2.base()));
    }
  };

  /*! Get a Compare_xy_2 functor object. */
  Compare_xy_2 compare_xy_2_object () const
  {
    return Compare_xy_2(m_base_tr->compare_xy_2_object());
  }

};

CGAL_END_NAMESPACE

#endif
