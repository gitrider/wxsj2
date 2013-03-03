// Copyright (c) 2004   INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_3/include/CGAL/Regular_triangulation_filtered_traits_3.h $
// $Id: Regular_triangulation_filtered_traits_3.h 29448 2006-03-13 10:19:25Z afabri $
// 
//
// Author(s)     : Sylvain Pion <Sylvain.Pion@sophia.inria.fr>

#ifndef CGAL_REGULAR_TRIANGULATION_FILTERED_TRAITS_3_H
#define CGAL_REGULAR_TRIANGULATION_FILTERED_TRAITS_3_H

#include <CGAL/basic.h>
#include <CGAL/Regular_triangulation_euclidean_traits_3.h>
#include <CGAL/Filtered_predicate.h>

CGAL_BEGIN_NAMESPACE

// The Weighted_converter is parametrized by a usual kernel converter,
// and adds the conversions for the Weighted_point.
template < typename Converter >
struct Weighted_converter_3
  : public Converter
{
  typedef typename Converter::Source_kernel Source_kernel;
  typedef typename Converter::Target_kernel Target_kernel;

  typedef typename Regular_triangulation_euclidean_traits_base_3<Source_kernel>
                   ::Weighted_point_3  Source_wp;

  typedef typename Regular_triangulation_euclidean_traits_base_3<Target_kernel>
                   ::Weighted_point_3  Target_wp;


#ifndef CGAL_CFG_MATCHING_BUG_6
  using Converter::operator();
#endif

  Target_wp
  operator()(const Source_wp &wp) const
  {
    return Target_wp(Converter::operator()(wp.point()),
                     Converter::operator()(wp.weight()));
  }
};

// The argument is supposed to be a Filtered_kernel like kernel.
template < typename K >
class Regular_triangulation_filtered_traits_3
  : public Regular_triangulation_euclidean_traits_base_3<K>
{
  // Exact traits is based on the exact kernel.
  typedef Regular_triangulation_euclidean_traits_3<typename K::EK>
                                                   Exact_traits;
  // Filtering traits is based on the filtering kernel.
  typedef Regular_triangulation_euclidean_traits_3<typename K::FK>
                                                   Filtering_traits;

  typedef typename K::C2E C2E;
  typedef typename K::C2F C2F;

public:

  typedef Filtered_predicate<
            typename Exact_traits::Power_test_3,
            typename Filtering_traits::Power_test_3,
            Weighted_converter_3<C2E>,
            Weighted_converter_3<C2F> >  Power_test_3;

  typedef Filtered_predicate<
            typename Exact_traits::Compare_power_distance_3,
            typename Filtering_traits::Compare_power_distance_3,
            Weighted_converter_3<C2E>,
            Weighted_converter_3<C2F> >  Compare_power_distance_3;

  typedef Filtered_predicate<
            typename Exact_traits::In_smallest_orthogonal_sphere_3,
            typename Filtering_traits::In_smallest_orthogonal_sphere_3,
            Weighted_converter_3<C2E>,
            Weighted_converter_3<C2F> >  In_smallest_orthogonal_sphere_3;

  typedef Filtered_predicate<
            typename Exact_traits::Side_of_bounded_orthogonal_sphere_3,
            typename Filtering_traits::Side_of_bounded_orthogonal_sphere_3,
            Weighted_converter_3<C2E>,
            Weighted_converter_3<C2F> >  Side_of_bounded_orthogonal_sphere_3;

  typedef Filtered_predicate<
            typename Exact_traits::Does_simplex_intersect_dual_support_3,
            typename Filtering_traits::Does_simplex_intersect_dual_support_3,
            Weighted_converter_3<C2E>,
            Weighted_converter_3<C2F> >  Does_simplex_intersect_dual_support_3;

  Power_test_3 power_test_3_object() const
  { return Power_test_3();}

  Compare_power_distance_3 compare_power_distance_3_object() const
  { return Compare_power_distance_3();}

  In_smallest_orthogonal_sphere_3
  in_smallest_orthogonal_sphere_3_object() const
  { return In_smallest_orthogonal_sphere_3(); }

  Side_of_bounded_orthogonal_sphere_3
  side_of_bounded_orthogonal_sphere_3_object() const
  { return Side_of_bounded_orthogonal_sphere_3(); }

  Does_simplex_intersect_dual_support_3
  does_simplex_intersect_dual_support_3_object() const
  { return Does_simplex_intersect_dual_support_3(); }

  // The following are inherited since they are constructions :
  // Construct_weighted_circumcenter_3
  // Compute_squared_radius_smallest_orthogonal_sphere_3
  // Compute_power_product_3
};

CGAL_END_NAMESPACE

#endif // CGAL_REGULAR_TRIANGULATION_FILTERED_TRAITS_3_H
