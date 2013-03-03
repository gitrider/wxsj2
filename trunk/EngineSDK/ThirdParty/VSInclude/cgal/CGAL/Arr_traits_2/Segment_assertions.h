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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_traits_2/Segment_assertions.h $
// $Id: Segment_assertions.h 29412 2006-03-12 09:38:57Z wein $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_SEGMENT_ASSERTIONS_H
#define CGAL_SEGMENT_ASSERTIONS_H

CGAL_BEGIN_NAMESPACE

template <class Traits_>
class Segment_assertions
{
  typedef Traits_                                  Traits_2;

  typedef typename Traits_2::Point_2               Point_2;
  typedef typename Traits_2::Kernel                Kernel;
  typedef typename Kernel::Line_2                  Line_2;
  typedef typename Traits_2::X_monotone_curve_2    X_monotone_curve_2;

public:

  static bool _assert_is_point_on (const Point_2& pt,
                                   const X_monotone_curve_2& cv,
                                   Tag_true tag)
  {
    Traits_2    traits;
    return (traits.compare_y_at_x_2_object() (pt, cv) == EQUAL);
  }

  static bool _assert_is_point_on (const Point_2& pt,
                                   const X_monotone_curve_2& cv,
                                   Tag_false tag)
  {
    return (true);
  }

  static bool _assert_is_point_on (const Point_2& pt,
                                   const Line_2& l,
                                   Tag_true tag)
  {
    Kernel      kernel;
    return (kernel.has_on_2_object() (l, pt));
  }

  static bool _assert_is_point_on (const Point_2& pt,
                                   const Line_2&  l,
                                   Tag_false tag)
  {
    return (true);
  }
};

CGAL_END_NAMESPACE

#endif
