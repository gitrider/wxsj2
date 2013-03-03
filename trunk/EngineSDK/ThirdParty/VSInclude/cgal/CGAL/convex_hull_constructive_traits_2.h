// Copyright (c) 1999  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_2/include/CGAL/convex_hull_constructive_traits_2.h $
// $Id: convex_hull_constructive_traits_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra

// This file's name must begin with a lower-case letter for backward 
// compatability.  Unfortunately, you can't have a file that differs only 
// in capitalization on the Windows platforms.

#ifndef CGAL_CONVEX_HULL_CONSTRUCTIVE_TRAITS_2_H 
#define CGAL_CONVEX_HULL_CONSTRUCTIVE_TRAITS_2_H

#include <CGAL/ch_function_objects_2.h>

CGAL_BEGIN_NAMESPACE
template <class K_>
class Convex_hull_constructive_traits_2 : public K_
{
public:
  typedef K_                                  K;
  typedef typename K::Point_2                 Point_2;    
  typedef typename K::Less_xy_2               Less_xy_2;
  typedef typename K::Less_yx_2               Less_yx_2;
  typedef CGALi::r_Less_dist_to_line<K>       Less_signed_distance_to_line_2;
  typedef typename K::Less_rotate_ccw_2       Less_rotate_ccw_2;
  typedef typename K::Left_turn_2             Left_turn_2;
  typedef typename K::Equal_2                 Equal_2;  
  typedef typename K::Segment_2               Segment_2;    
  
  Less_xy_2
  less_xy_2_object() const 
  { return Less_xy_2(); } 

  Less_yx_2
  less_yx_2_object() const 
  { return Less_yx_2(); } 

  Less_signed_distance_to_line_2
  less_signed_distance_to_line_2_object() const
  { return Less_signed_distance_to_line_2(); } 

  Less_rotate_ccw_2
  less_rotate_ccw_2_object() const
  { return Less_rotate_ccw_2(); }

  Left_turn_2
  left_turn_2_object() const
  { return Left_turn_2(); }

  Equal_2
  equal_2_object() const
  { return Equal_2(); }
};


// for backward compatability

template <class K>
class convex_hull_constructive_traits_2 : 
                         public Convex_hull_constructive_traits_2<K>
{
};

CGAL_END_NAMESPACE

#endif // CGAL_CONVEX_HULL_CONSTRUCTIVE_TRAITS_2_H

