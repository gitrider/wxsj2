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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_overlay_2/Overlay_subcurve.h $
// $Id: Overlay_subcurve.h 28836 2006-02-27 14:35:50Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_OVERLAY_SUBCURVE_H
#define CGAL_OVERLAY_SUBCURVE_H

#include <CGAL/Sweep_line_2/Arr_construction_curve.h>

CGAL_BEGIN_NAMESPACE

template<class _Traits>
class Overlay_subcurve : public Arr_construction_curve<_Traits>
{
public:
  typedef _Traits                                        Traits;
  typedef typename Traits::Point_2                       Point_2;
  typedef typename Traits::X_monotone_curve_2            X_monotone_curve_2;

  typedef Arr_construction_curve<Traits>                 Base;
  typedef Overlay_subcurve<Traits>                       Self;

  typedef typename Base::StatusLineIter                  StatusLineIter;

  typedef typename Traits::Color                         Color;
  typedef typename Traits::Halfedge_handle_red           Halfedge_handle_red;
  typedef typename Traits::Halfedge_handle_blue          Halfedge_handle_blue;

  typedef Sweep_line_event<Traits, Self>                 Event;



  using Base::m_left_event;
  using Base::m_right_event;
  using Base::m_lastCurve;
  using Base::m_lastEvent;

protected:

  /*! A Subcure from a different color that lies above */
  Self*        m_above;
  
  
public:

  Overlay_subcurve() : Base(),
                       m_above(NULL)
  {}

  Overlay_subcurve(const X_monotone_curve_2 &curve) : Base(curve),
                                                      m_above(NULL)
  {}

  void set_above(Self* sc)
  {
    m_above = sc;
  }

  Self* get_above() const
  {
    return m_above;
  }

  Color get_color() const
  {
    return m_lastCurve.get_color();
  }

  bool has_same_color(const Self* sc) const
  {
    return (m_lastCurve.get_color() == sc->get_color());
  }

  Halfedge_handle_red get_red_halfedge_handle() const
  {
    return m_lastCurve.get_red_halfedge_handle();
  }

  Halfedge_handle_blue get_blue_halfedge_handle() const
  {
    return m_lastCurve.get_blue_halfedge_handle();
  }

};
  
CGAL_END_NAMESPACE

#endif
