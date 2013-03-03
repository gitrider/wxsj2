// Copyright (c) 2002  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_2/include/CGAL/ch_function_objects_2.h $
// $Id: ch_function_objects_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Susan Hert

#ifndef CGAL_CH_FUNCTION_OBJECTS_2_H
#define CGAL_CH_FUNCTION_OBJECTS_2_H

#include <CGAL/functional_base.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template <class R>
class r_Less_dist_to_line
{
public:
  typedef bool    result_type;
  typedef  Arity_tag< 4 >   Arity;

  typedef typename R::Point_2  Point;
  typedef typename R::Line_2   Line;

        r_Less_dist_to_line() : line_constructed( false )
        { }

  bool  operator()(const Point& a, const Point& b,
                   const Point& c, const Point& d) const
        {
          if (!line_constructed)
          {
             line_constructed = true;
             l_ab = Line(a,b);
          }
          Comparison_result res = compare_signed_distance_to_line(l_ab, c, d);
          if ( res == SMALLER )
          {
              return true;
          }
          else if ( res == EQUAL )
          {
              return lexicographically_xy_smaller( c, d );
          }
          else
          {
              return false;
          }
        }

private:
  mutable bool line_constructed;
  mutable Line l_ab;
};

} // namespace CGALi

CGAL_END_NAMESPACE

#endif // CGAL_CH_FUNCTION_OBJECTS_2_H	

