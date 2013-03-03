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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_point_location/Td_traits_functions.h $
// $Id: Td_traits_functions.h 30322 2006-04-14 15:07:17Z lsaboret $
// 
//
// Author(s)     : Oren Nechushtan <theoren@math.tau.ac.il>
#ifndef CGAL_TD_TRAITS_H
#include <CGAL/Arr_point_location/Td_traits.h>
#endif

CGAL_BEGIN_NAMESPACE

template <class Traits,class X_curve_plus>
const typename Td_traits<Traits,X_curve_plus>::Point&
Td_traits<Traits,X_curve_plus>::get_point_at_left_top_infinity(){
  /*
  static typename Td_traits<Traits,X_curve_plus>::Point
    Td_traits<Traits,X_curve_plus>::POINT_AT_LEFT_TOP_INFINITY;
  return Td_traits<Traits,X_curve_plus>::POINT_AT_LEFT_TOP_INFINITY;
  */
  //  static Point POINT_AT_LEFT_TOP_INFINITY;
  if (!POINT_AT_LEFT_TOP_INFINITY)
    POINT_AT_LEFT_TOP_INFINITY = new Point();
  return *POINT_AT_LEFT_TOP_INFINITY;
}

template <class Traits,class X_curve_plus>
const typename Td_traits<Traits,X_curve_plus>::Point&
Td_traits<Traits,X_curve_plus>::get_point_at_right_bottom_infinity(){
  /*
  static typename Td_traits<Traits,X_curve_plus>::Point
    Td_traits<Traits,X_curve_plus>::POINT_AT_RIGHT_BOTTOM_INFINITY;
  return Td_traits<Traits,X_curve_plus>::POINT_AT_RIGHT_BOTTOM_INFINITY;
  */
  //  static Point POINT_AT_RIGHT_BOTTOM_INFINITY;
  if (!POINT_AT_RIGHT_BOTTOM_INFINITY)
    POINT_AT_RIGHT_BOTTOM_INFINITY = new Point();
  return *POINT_AT_RIGHT_BOTTOM_INFINITY;
}

template <class Traits,class X_curve_plus>
const typename Td_traits<Traits,X_curve_plus>::X_curve&
Td_traits<Traits,X_curve_plus>::get_curve_at_infinity(){
  /*
  static typename typename Traits::X_curveTraits::X_curve 
    Td_traits<Traits,X_curve_plus>::CURVE_AT_INFINITY;
  return Td_traits<Traits,X_curve_plus>::CURVE_AT_INFINITY;
  */
  //  static X_curve CURVE_AT_INFINITY;
  if (!CURVE_AT_INFINITY)
    CURVE_AT_INFINITY = new X_curve();
  return *CURVE_AT_INFINITY;
}

template <class Traits,class X_curve_plus>
typename Td_traits<Traits,X_curve_plus>::Point *
Td_traits<Traits,X_curve_plus>::POINT_AT_LEFT_TOP_INFINITY = 0;

template <class Traits,class X_curve_plus>
typename Td_traits<Traits,X_curve_plus>::Point *
Td_traits<Traits,X_curve_plus>::POINT_AT_RIGHT_BOTTOM_INFINITY = 0;

template <class Traits,class X_curve_plus>
typename Td_traits<Traits,X_curve_plus>::X_curve *
Td_traits<Traits,X_curve_plus>::CURVE_AT_INFINITY = 0;

CGAL_END_NAMESPACE
