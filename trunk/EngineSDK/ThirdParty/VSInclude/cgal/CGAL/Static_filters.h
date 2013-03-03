// Copyright (c) 2001,2004  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Static_filters.h $
// $Id: Static_filters.h 28618 2006-02-19 10:55:56Z spion $
// 
//
// Author(s)     : Sylvain Pion
 
#ifndef CGAL_STATIC_FILTERS_H
#define CGAL_STATIC_FILTERS_H

// This kernel wrapper gathers optimized predicates written by hand, using
// a few steps of filtering.  It should work if the initial traits has
// cartesian coordinates which fit exactly in doubles.
//
// Purely static filters code has been removed, since it requires additional
// logic and is not plug'n play (requires users providing bounds).
// If it should be provided again, it should probably be separate.

#include <CGAL/basic.h>

#include <CGAL/Kernel/function_objects.h>
#include <CGAL/Cartesian/function_objects.h>

#include <CGAL/Static_filters/tools.h>
#include <CGAL/Static_filters/Orientation_2.h>
#include <CGAL/Static_filters/Orientation_3.h>
#include <CGAL/Static_filters/Compare_y_at_x_2.h>
#include <CGAL/Static_filters/Side_of_oriented_circle_2.h>
#include <CGAL/Static_filters/Side_of_oriented_sphere_3.h>

// #include <CGAL/Static_filters/Coplanar_orientation_3.h>
// #include <CGAL/Static_filters/Coplanar_side_of_bounded_circle_3.h>

// TODO :
// - add more predicates :
//   - all lexicographical comparisons
//   - left_turn (via generic adapter to orientation)
//   - power_tests
//   - others ?
// - benchmark on more algorithms.
// - improve fit_in_double() for other NTs (MP_Float, Lazy). cf tools.h.
// - try to automatize : have a struct a la Static_filter_error, with one part
//   which is runtime, and the other which can be constant-propagated by the
//   compiler.  g++ 4.0 should be able to cprop the second part...


CGAL_BEGIN_NAMESPACE

// The K_base argument is supposed to provide exact primitives.
template < typename K_base >
class Static_filters : public K_base
{
  typedef Static_filters<K_base>                    Self;

public:

  typedef SF_Compare_y_at_x_2<K_base,Self>          Compare_y_at_x_2;
  typedef SF_Orientation_2<K_base>                  Orientation_2;
  typedef SF_Orientation_3<K_base>                  Orientation_3;
  typedef SF_Side_of_oriented_circle_2<K_base>      Side_of_oriented_circle_2;
  typedef SF_Side_of_oriented_sphere_3<K_base>      Side_of_oriented_sphere_3;

  // The following do not require filtering as they only do
  // comparisons.  We must be careful that *all* their function
  // operators must not do any comparisons.
  // In case we would like to avoid filtering only some of
  // the function operators, we need to make a new functors.
  typedef CommonKernelFunctors::Left_turn_2<Self>   Left_turn_2;

  typedef CartesianKernelFunctors::Less_x_2<Self>   Less_x_2;
  typedef CartesianKernelFunctors::Less_y_2<Self>   Less_y_2;
  typedef CartesianKernelFunctors::Less_xy_2<Self>  Less_xy_2;
  typedef CartesianKernelFunctors::Less_yx_2<Self>  Less_yx_2;

  typedef CartesianKernelFunctors::Less_x_3<Self>   Less_x_3;
  typedef CartesianKernelFunctors::Less_y_3<Self>   Less_y_3;
  typedef CartesianKernelFunctors::Less_z_3<Self>   Less_z_3;
  typedef CartesianKernelFunctors::Less_xy_3<Self>  Less_xy_3;
  typedef CartesianKernelFunctors::Less_xyz_3<Self> Less_xyz_3;

  typedef CartesianKernelFunctors::Compare_xy_2<Self>  Compare_xy_2;
  typedef CartesianKernelFunctors::Compare_x_3<Self>   Compare_x_3;
  typedef CartesianKernelFunctors::Compare_y_3<Self>   Compare_y_3;
  typedef CartesianKernelFunctors::Compare_z_3<Self>   Compare_z_3;
  typedef CartesianKernelFunctors::Compare_xy_3<Self>  Compare_xy_3;
  typedef CartesianKernelFunctors::Compare_xyz_3<Self> Compare_xyz_3;

  Compare_xy_2
  compare_xy_2_object() const
  { return Compare_xy_2(); }

  Compare_x_3
  compare_x_3_object() const
  { return Compare_x_3(); }

  Compare_y_3
  compare_y_3_object() const
  { return Compare_y_3(); }

  Compare_z_3
  compare_z_3_object() const
  { return Compare_z_3(); }

  Compare_xy_3
  compare_xy_3_object() const
  { return Compare_xy_3(); }

  Compare_xyz_3
  compare_xyz_3_object() const
  { return Compare_xyz_3(); }

  Left_turn_2
  left_turn_2_object() const
  { return Left_turn_2(); }

  Less_x_2
  less_x_2_object() const
  { return Less_x_2(); }

  Less_y_2
  less_y_2_object() const
  { return Less_y_2(); }

  Less_xy_2
  less_xy_2_object() const
  { return Less_xy_2(); }

  Less_yx_2
  less_yx_2_object() const
  { return Less_yx_2(); }

  Less_x_3
  less_x_3_object() const
  { return Less_x_3(); }

  Less_y_3
  less_y_3_object() const
  { return Less_y_3(); }

  Less_z_3
  less_z_3_object() const
  { return Less_z_3(); }

  Less_xy_3
  less_xy_3_object() const
  { return Less_xy_3(); }

  Less_xyz_3
  less_xyz_3_object() const
  { return Less_xyz_3(); }

  Compare_y_at_x_2
  compare_y_at_x_2_object() const
  { return Compare_y_at_x_2(); }

  Orientation_2
  orientation_2_object() const
  { return Orientation_2(); }

  Orientation_3
  orientation_3_object() const
  { return Orientation_3(); }

  Side_of_oriented_circle_2
  side_of_oriented_circle_2_object() const
  { return Side_of_oriented_circle_2(); }

  Side_of_oriented_sphere_3
  side_of_oriented_sphere_3_object() const
  { return Side_of_oriented_sphere_3(); }

  // The two following are for degenerate cases, so I'll update them later.
  //
  // typedef SF_Coplanar_orientation_3<Point_3, Orientation_2>
  //                                                   Coplanar_orientation_3;
  // typedef SF_Side_of_bounded_circle_3<Point_3>
  //                                         Coplanar_side_of_bounded_circle_3;

  // Coplanar_orientation_3
  // coplanar_orientation_3_object() const
  // { return Coplanar_orientation_3(); }

  // Coplanar_side_of_bounded_circle_3
  // coplanar_side_of_bounded_circle_3_object() const
  // { return Coplanar_side_of_bounded_circle_3(); }
};

CGAL_END_NAMESPACE

#endif // CGAL_STATIC_FILTERS_H
