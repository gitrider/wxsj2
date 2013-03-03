// Copyright (c) 2003-2004  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_23/include/CGAL/Kernel/global_functions_internal_2.h $
// $Id: global_functions_internal_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion
 
#ifndef CGAL_KERNEL_GLOBAL_FUNCTIONS_INTERNAL_2_H
#define CGAL_KERNEL_GLOBAL_FUNCTIONS_INTERNAL_2_H

// Generic functions calling the kernel functor, taking the kernel as
// parameter.

// These functions are not documented for now, but could be as some point.

#include <CGAL/basic.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template < class K >
inline
Angle
angle(const typename CGAL_WRAP(K)::Point_2 &p,
      const typename CGAL_WRAP(K)::Point_2 &q,
      const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.angle_2_object()(p, q, r);
}

template < class K >
inline
bool
are_ordered_along_line(const typename CGAL_WRAP(K)::Point_2 &p,
                       const typename CGAL_WRAP(K)::Point_2 &q,
                       const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.are_ordered_along_line_2_object()(p, q, r);
}

template < class K >
inline
bool
are_strictly_ordered_along_line(const typename CGAL_WRAP(K)::Point_2 &p,
                                const typename CGAL_WRAP(K)::Point_2 &q,
                                const typename CGAL_WRAP(K)::Point_2 &r,
                                const K& k)
{
  return k.are_strictly_ordered_along_line_2_object()(p, q, r);
}

template < class K >
inline
typename K::FT
area(const typename CGAL_WRAP(K)::Point_2 &p,
     const typename CGAL_WRAP(K)::Point_2 &q,
     const typename CGAL_WRAP(K)::Point_2 &r,
     const K& k)
{
  return k.compute_area_2_object()(p, q, r);
}

template <typename K>
inline
typename K::Line_2
bisector(const typename CGAL_WRAP(K)::Point_2 &p,
         const typename CGAL_WRAP(K)::Point_2 &q, const K &k)
{
  return k.construct_bisector_2_object()(p, q);
}

template <typename K>
inline
typename K::Line_2
bisector(const typename CGAL_WRAP(K)::Line_2 &l1,
         const typename CGAL_WRAP(K)::Line_2 &l2, const K &k)
{
  return k.construct_bisector_2_object()(l1, l2);
}

template < class K >
inline
typename K::Point_2
centroid(const typename CGAL_WRAP(K)::Point_2 &p,
         const typename CGAL_WRAP(K)::Point_2 &q,
         const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.construct_centroid_2_object()(p, q, r);
}

template < class K >
inline
typename K::Point_2
centroid(const typename CGAL_WRAP(K)::Point_2 &p,
         const typename CGAL_WRAP(K)::Point_2 &q,
         const typename CGAL_WRAP(K)::Point_2 &r,
         const typename CGAL_WRAP(K)::Point_2 &s, const K& k)
{
  return k.construct_centroid_2_object()(p, q, r, s);
}

template < class K >
inline
typename K::Point_2
centroid(const typename CGAL_WRAP(K)::Triangle_2 &t, const K& k)
{
  return k.construct_centroid_2_object()(t);
}

template < class K >
inline
typename K::Point_2
circumcenter(const typename CGAL_WRAP(K)::Point_2 &p,
             const typename CGAL_WRAP(K)::Point_2 &q,
             const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.construct_circumcenter_2_object()(p, q, r);
}

template < class K >
inline
typename K::Point_2
circumcenter(const typename CGAL_WRAP(K)::Triangle_2 &t, const K& k)
{
  return k.construct_circumcenter_2_object()(t);
}

template < class K >
inline
bool
collinear(const typename CGAL_WRAP(K)::Point_2 &p,
          const typename CGAL_WRAP(K)::Point_2 &q,
          const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.collinear_2_object()(p, q, r);
}

template < class K >
inline
bool
collinear_are_ordered_along_line(const typename CGAL_WRAP(K)::Point_2 &p,
                                 const typename CGAL_WRAP(K)::Point_2 &q,
                                 const typename CGAL_WRAP(K)::Point_2 &r,
                                 const K& k)
{
  return k.collinear_are_ordered_along_line_2_object()(p, q, r);
}

template < class K >
inline
bool
collinear_are_strictly_ordered_along_line(
             const typename CGAL_WRAP(K)::Point_2 &p,
             const typename CGAL_WRAP(K)::Point_2 &q,
             const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.collinear_are_strictly_ordered_along_line_2_object()(p, q, r);
}

template < typename K >
inline
Comparison_result
compare_angle_with_x_axis(const typename CGAL_WRAP(K)::Direction_2& d1,
                          const typename CGAL_WRAP(K)::Direction_2& d2,
                          const K& k)
{
  return k.compare_angle_with_x_axis_2_object()(d1, d2);
}

template <class K >
inline
Comparison_result
compare_distance_to_point(const typename CGAL_WRAP(K)::Point_2 &p,
                          const typename CGAL_WRAP(K)::Point_2 &q,
                          const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.compare_distance_2_object()(p, q, r);
}

template <class K>
inline
Comparison_result
compare_signed_distance_to_line(const typename CGAL_WRAP(K)::Point_2& p,
				const typename CGAL_WRAP(K)::Point_2& q,
				const typename CGAL_WRAP(K)::Point_2& r,
				const typename CGAL_WRAP(K)::Point_2& s,
				const K& k)
{
  if (k.less_signed_distance_to_line_2_object()(p, q, r, s)) return SMALLER;
  if (k.less_signed_distance_to_line_2_object()(p, q, s, r)) return LARGER;
  return EQUAL;
}

template <class K>
inline
Comparison_result
compare_signed_distance_to_line(const typename CGAL_WRAP(K)::Line_2& l,
				const typename CGAL_WRAP(K)::Point_2& p,
				const typename CGAL_WRAP(K)::Point_2& q,
				const K& k)
{
  if (k.less_signed_distance_to_line_2_object()(l, p, q)) return SMALLER;
  if (k.less_signed_distance_to_line_2_object()(l, q, p)) return LARGER;
  return EQUAL;
}

template < class K >
inline
Comparison_result
compare_slopes(const typename CGAL_WRAP(K)::Line_2 &l1,
               const typename CGAL_WRAP(K)::Line_2 &l2, const K& k)
{
  return k.compare_slope_2_object()(l1, l2);
}

template < class K >
inline
Comparison_result
compare_slopes(const typename CGAL_WRAP(K)::Segment_2 &s1,
               const typename CGAL_WRAP(K)::Segment_2 &s2, const K& k)
{
  return k.compare_slope_2_object()(s1, s2);
}

template < class K >
inline
Comparison_result
compare_x(const typename CGAL_WRAP(K)::Point_2 &p,
          const typename CGAL_WRAP(K)::Point_2 &q, const K& k)
{
  return k.compare_x_2_object()(p, q);
}

template < class K >
inline
Comparison_result
compare_x(const typename CGAL_WRAP(K)::Point_2 &p,
          const typename CGAL_WRAP(K)::Line_2 &l1,
          const typename CGAL_WRAP(K)::Line_2 &l2, const K& k)
{
  return k.compare_x_2_object()(p, l1, l2);
}

template < class K >
inline
Comparison_result
compare_x(const typename CGAL_WRAP(K)::Line_2 &l,
          const typename CGAL_WRAP(K)::Line_2 &h1,
          const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_x_2_object()(l, h1, h2);
}

template < class K >
inline
Comparison_result
compare_x(const typename CGAL_WRAP(K)::Line_2 &l1,
          const typename CGAL_WRAP(K)::Line_2 &h1,
          const typename CGAL_WRAP(K)::Line_2 &l2,
          const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_x_2_object()(l1, h1, l2, h2);
}

template < class K >
inline
Comparison_result
compare_x_at_y(const typename CGAL_WRAP(K)::Point_2& p,
               const typename CGAL_WRAP(K)::Line_2& h, const K& k)
{
  return k.compare_x_at_y_2_object()(p, h);
}

/* Undocumented
template < class K >
inline
Comparison_result
compare_y_at_x(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Segment_2 &s, const K& k)
{
  return k.compare_y_at_x_2_object()(p, s);
}
*/

template < class K >
inline
Comparison_result
compare_x_at_y(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Line_2 &h1,
               const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_x_at_y_2_object()(p, h1, h2);
}

template < class K >
inline
Comparison_result
compare_x_at_y(const typename CGAL_WRAP(K)::Line_2 &l1,
               const typename CGAL_WRAP(K)::Line_2 &l2,
               const typename CGAL_WRAP(K)::Line_2 &h, const K& k)
{
  return k.compare_x_at_y_2_object()(l1, l2, h);
}

template < class K >
inline
Comparison_result
compare_x_at_y(const typename CGAL_WRAP(K)::Line_2 &l1,
               const typename CGAL_WRAP(K)::Line_2 &l2,
               const typename CGAL_WRAP(K)::Line_2 &h1,
               const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_x_at_y_2_object()(l1, l2, h1, h2);
}

template < class K >
inline
Comparison_result
compare_xy(const typename CGAL_WRAP(K)::Point_2 &p,
           const typename CGAL_WRAP(K)::Point_2 &q, const K& k)
{
  return k.compare_xy_2_object()(p, q);
}

template < class K >
inline
Comparison_result
compare_y(const typename CGAL_WRAP(K)::Point_2 &p,
          const typename CGAL_WRAP(K)::Point_2 &q, const K& k)
{
  return k.compare_y_2_object()(p, q);
}

template < class K >
inline
Comparison_result
compare_y(const typename CGAL_WRAP(K)::Point_2 &p,
          const typename CGAL_WRAP(K)::Line_2 &l1,
          const typename CGAL_WRAP(K)::Line_2 &l2, const K& k)
{
  return k.compare_y_2_object()(p, l1, l2);
}

template < class K >
inline
Comparison_result
compare_y(const typename CGAL_WRAP(K)::Line_2 &l1,
          const typename CGAL_WRAP(K)::Line_2 &l2,
          const typename CGAL_WRAP(K)::Line_2 &h1,
          const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_y_2_object()(l1, l2, h1, h2);
}

template < class K >
inline
Comparison_result
compare_y(const typename CGAL_WRAP(K)::Line_2 &l,
          const typename CGAL_WRAP(K)::Line_2 &h1,
          const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_y_2_object()(l, h1, h2);
}

template < class K >
inline
Comparison_result
compare_y_at_x(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Segment_2 &s, const K& k)
{
  return k.compare_y_at_x_2_object()(p, s);
}

template < class K >
inline
Comparison_result
compare_y_at_x(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Segment_2 &s1,
               const typename CGAL_WRAP(K)::Segment_2 &s2, const K& k)
{
  return k.compare_y_at_x_2_object()(p, s1, s2);
}

template < class K >
inline
Comparison_result
compare_y_at_x(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Line_2 &l, const K& k)
{
  return k.compare_y_at_x_2_object()(p, l);
}

template < class K >
inline
Comparison_result
compare_y_at_x(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Line_2 &h1,
               const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_y_at_x_2_object()(p, h1, h2);
}

template < class K >
inline
Comparison_result
compare_y_at_x(const typename CGAL_WRAP(K)::Line_2 &l1,
               const typename CGAL_WRAP(K)::Line_2 &l2,
               const typename CGAL_WRAP(K)::Line_2 &h, const K& k)
{
  return k.compare_y_at_x_2_object()(l1, l2, h);
}

template < class K >
inline
Comparison_result
compare_y_at_x(const typename CGAL_WRAP(K)::Line_2 &l1,
               const typename CGAL_WRAP(K)::Line_2 &l2,
               const typename CGAL_WRAP(K)::Line_2 &h1,
               const typename CGAL_WRAP(K)::Line_2 &h2, const K& k)
{
  return k.compare_y_at_x_2_object()(l1, l2, h1, h2);
}

template <class K>
inline
bool
has_larger_distance_to_point(const typename CGAL_WRAP(K)::Point_2 &p,
			     const typename CGAL_WRAP(K)::Point_2 &q,
			     const typename CGAL_WRAP(K)::Point_2 &r,
			     const K& k)
{
  return k.less_distance_to_point_2_object()(p, r, q);
}

template <class K>
inline
bool
has_smaller_distance_to_point(const typename CGAL_WRAP(K)::Point_2 &p,
                              const typename CGAL_WRAP(K)::Point_2 &q,
                              const typename CGAL_WRAP(K)::Point_2 &r,
                              const K& k)
{
  return k.less_distance_to_point_2_object()(p, q, r);
}

template <class K>
inline
bool
has_smaller_signed_distance_to_line(const typename CGAL_WRAP(K)::Line_2& l,
				    const typename CGAL_WRAP(K)::Point_2& p,
				    const typename CGAL_WRAP(K)::Point_2& q,
				    const K& k)
{
  return k.less_signed_distance_to_line_2_object()(l, p, q);
}

template <class K>
inline
bool
has_larger_signed_distance_to_line(const typename CGAL_WRAP(K)::Line_2& l,
				   const typename CGAL_WRAP(K)::Point_2& p,
				   const typename CGAL_WRAP(K)::Point_2& q,
				   const K& k)
{
  return k.less_signed_distance_to_line_2_object()(l, q, p);
}

template <class K>
inline
bool
has_larger_signed_distance_to_line(const typename CGAL_WRAP(K)::Point_2& p,
				   const typename CGAL_WRAP(K)::Point_2& q,
				   const typename CGAL_WRAP(K)::Point_2& r,
				   const typename CGAL_WRAP(K)::Point_2& s,
				   const K& k)
{
  return k.less_signed_distance_to_line_2_object()(p, q, s, r);
}

template <class K>
inline
bool
has_smaller_signed_distance_to_line(const typename CGAL_WRAP(K)::Point_2& p,
                                    const typename CGAL_WRAP(K)::Point_2& q,
                                    const typename CGAL_WRAP(K)::Point_2& r,
                                    const typename CGAL_WRAP(K)::Point_2& s,
				    const K& k)
{
  return k.less_signed_distance_to_line_2_object()(p, q, r, s);
}

template < class K >
inline
bool
left_turn(const typename CGAL_WRAP(K)::Point_2 &p,
          const typename CGAL_WRAP(K)::Point_2 &q,
          const typename CGAL_WRAP(K)::Point_2 &r, const K& k)
{
  return k.left_turn_2_object()(p, q, r);
}

template < class K >
inline
bool
less_x(const typename CGAL_WRAP(K)::Point_2 &p,
       const typename CGAL_WRAP(K)::Point_2 &q, const K& k)
{
  return k.less_x_2_object()(p, q);
}

template < class K >
inline
bool
less_y(const typename CGAL_WRAP(K)::Point_2 &p,
       const typename CGAL_WRAP(K)::Point_2 &q, const K& k)
{
  return k.less_y_2_object()(p, q);
}

template < class K >
inline
bool
lexicographically_xy_larger(const typename CGAL_WRAP(K)::Point_2 &p,
                            const typename CGAL_WRAP(K)::Point_2 &q,
                            const K& k)
{
  return k.compare_xy_2_object()(p, q) == LARGER;
}

template < class K >
inline
bool
lexicographically_xy_larger_or_equal(const typename CGAL_WRAP(K)::Point_2 &p,
                                     const typename CGAL_WRAP(K)::Point_2 &q,
                                     const K& k)
{
  return k.compare_xy_2_object()(p, q) != SMALLER;
}

template < class K >
inline
bool
lexicographically_xy_smaller(const typename CGAL_WRAP(K)::Point_2 &p,
                             const typename CGAL_WRAP(K)::Point_2 &q,
                             const K& k)
{
  return k.less_xy_2_object()(p, q);
}

template < class K >
inline
bool
lexicographically_xy_smaller_or_equal(const typename CGAL_WRAP(K)::Point_2 &p,
                                      const typename CGAL_WRAP(K)::Point_2 &q,
                                      const K& k)
{
  return k.compare_xy_2_object()(p, q) != LARGER;
}

template < class K >
inline
bool
lexicographically_yx_smaller(const typename CGAL_WRAP(K)::Point_2 &p,
                             const typename CGAL_WRAP(K)::Point_2 &q,
                             const K& k)
{
  return k.less_yx_2_object()(p, q);
}

template < class K >
inline
bool
lexicographically_yx_smaller_or_equal(const typename CGAL_WRAP(K)::Point_2 &p,
                                      const typename CGAL_WRAP(K)::Point_2 &q,
                                      const K& k)
{
  return !k.less_yx_2_object()(q, p);
}

// FIXME : Undocumented
template < class K >
inline
bool
lexicographically_yx_larger(const typename CGAL_WRAP(K)::Point_2 &p,
                            const typename CGAL_WRAP(K)::Point_2 &q,
                            const K& k)
{
  return k.less_yx_2_object()(q, p);
}

// FIXME : Undocumented
template < class K >
inline
bool
lexicographically_yx_larger_or_equal(const typename CGAL_WRAP(K)::Point_2 &p,
                                     const typename CGAL_WRAP(K)::Point_2 &q,
                                     const K& k)
{
  return !k.less_yx_2_object()(p, q);
}

template < class K >
inline
typename K::Point_2
midpoint(const typename CGAL_WRAP(K)::Point_2 &p,
         const typename CGAL_WRAP(K)::Point_2 &q, const K &k)
{
  return k.construct_midpoint_2_object()(p, q);
}

template < class K >
inline
typename K::Point_2
max_vertex(const typename CGAL_WRAP(K)::Iso_rectangle_2 &ir, const K &k)
{
  return k.construct_max_vertex_2_object()(ir);
}

template < class K >
inline
typename K::Point_2
min_vertex(const typename CGAL_WRAP(K)::Iso_rectangle_2 &ir, const K &k)
{
  return k.construct_min_vertex_2_object()(ir);
}

template <typename K>
inline
Orientation
orientation(const typename CGAL_WRAP(K)::Point_2 &p,
            const typename CGAL_WRAP(K)::Point_2 &q,
            const typename CGAL_WRAP(K)::Point_2 &r, const K &k)
{
  return k.orientation_2_object()(p, q, r);
}

template <typename K>
inline
Orientation
orientation(const typename CGAL_WRAP(K)::Vector_2 &u,
            const typename CGAL_WRAP(K)::Vector_2 &v, const K &k)
{
  return k.orientation_2_object()(u, v);
}

template <typename K>
inline
bool
parallel(const typename CGAL_WRAP(K)::Line_2 &l1,
         const typename CGAL_WRAP(K)::Line_2 &l2, const K &k)
{
  return k.are_parallel_2_object()(l1, l2);
}

template <typename K>
inline
bool
parallel(const typename CGAL_WRAP(K)::Ray_2 &r1,
         const typename CGAL_WRAP(K)::Ray_2 &r2, const K &k)
{
  return k.are_parallel_2_object()(r1, r2);
}

template <typename K>
inline
bool
parallel(const typename CGAL_WRAP(K)::Segment_2 &s1,
         const typename CGAL_WRAP(K)::Segment_2 &s2, const K &k)
{
  return k.are_parallel_2_object()(s1, s2);
}

template <typename K>
inline
bool
right_turn(const typename CGAL_WRAP(K)::Point_2 &p,
           const typename CGAL_WRAP(K)::Point_2 &q,
           const typename CGAL_WRAP(K)::Point_2 &r, const K &k)
{
  return CGALi::orientation(p, q, r, k) == RIGHT_TURN;
}

template <class K>
inline
Bounded_side
side_of_bounded_circle(const typename CGAL_WRAP(K)::Point_2 &p,
                       const typename CGAL_WRAP(K)::Point_2 &q,
                       const typename CGAL_WRAP(K)::Point_2 &r,
                       const typename CGAL_WRAP(K)::Point_2 &t, const K &k)
{
  return k.side_of_bounded_circle_2_object()(p, q, r, t);
}

template <class K>
inline
Bounded_side
side_of_bounded_circle(const typename CGAL_WRAP(K)::Point_2 &p,
                       const typename CGAL_WRAP(K)::Point_2 &q,
                       const typename CGAL_WRAP(K)::Point_2 &r, const K &k)
{
  return k.side_of_bounded_circle_2_object()(p, q, r);
}

template <class K>
inline
Oriented_side
side_of_oriented_circle(const typename CGAL_WRAP(K)::Point_2 &p,
                        const typename CGAL_WRAP(K)::Point_2 &q,
                        const typename CGAL_WRAP(K)::Point_2 &r,
                        const typename CGAL_WRAP(K)::Point_2 &t, const K &k)
{
  return k.side_of_oriented_circle_2_object()(p, q, r, t);
}

template < class K >
inline
typename K::FT
squared_radius(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Point_2 &q, const K &k)
{
  return k.compute_squared_radius_2_object()(p, q);
}

template < class K >
CGAL_KERNEL_INLINE
typename K::FT
squared_radius(const typename CGAL_WRAP(K)::Point_2 &p,
               const typename CGAL_WRAP(K)::Point_2 &q,
               const typename CGAL_WRAP(K)::Point_2 &r, const K &k)
{
  return k.compute_squared_radius_2_object()(p, q, r);
}

template < class K >
inline
bool
x_equal(const typename CGAL_WRAP(K)::Point_2 &p,
        const typename CGAL_WRAP(K)::Point_2 &q, const K &k)
{
  return k.equal_x_2_object()(p, q);
}

template < class K >
inline
bool
y_equal(const typename CGAL_WRAP(K)::Point_2 &p,
        const typename CGAL_WRAP(K)::Point_2 &q, const K &k)
{
  return k.equal_y_2_object()(p, q);
}

} // namespace CGALi

CGAL_END_NAMESPACE

#endif  // CGAL_KERNEL_GLOBAL_FUNCTIONS_INTERNAL_2_H
