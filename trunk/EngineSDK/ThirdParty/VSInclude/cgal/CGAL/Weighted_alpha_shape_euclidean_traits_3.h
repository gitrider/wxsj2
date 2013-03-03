// Copyright (c) 1997  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Alpha_shapes_3/include/CGAL/Weighted_alpha_shape_euclidean_traits_3.h $
// $Id: Weighted_alpha_shape_euclidean_traits_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Tran Kai Frank DA <Frank.Da@sophia.inria.fr>

#ifndef CGAL_WEIGHTED_ALPHA_SHAPE_EUCLIDEAN_TRAITS_3_H
#define CGAL_WEIGHTED_ALPHA_SHAPE_EUCLIDEAN_TRAITS_3_H 

#include <CGAL/Regular_triangulation_euclidean_traits_3.h>

CGAL_BEGIN_NAMESPACE


   
//------------------ Traits class -------------------------------------

template <class K>
class Weighted_alpha_shape_euclidean_traits_3 : public 
Regular_triangulation_euclidean_traits_3<K>
{
public:
  typedef Regular_triangulation_euclidean_traits_3<K> Base;
  typedef typename Base::Side_of_bounded_orthogonal_sphere_3 
                                       Side_of_bounded_sphere_3;

 class  Compute_squared_radius_3 {
    typedef typename Base::Weighted_point   Weighted_point_3;
    typedef typename Base::FT         FT;
    typedef typename Base::Compute_squared_radius_smallest_orthogonal_sphere_3
                                       Compute_squared_radius_base;
  public:
    FT operator() (Weighted_point_3 p, 
		   Weighted_point_3 q , 
		   Weighted_point_3 r, 
		   Weighted_point_3 s) {
      return Compute_squared_radius_base()(p,q,r,s); }

    FT operator() (Weighted_point_3 p, 
		   Weighted_point_3 q , 
		   Weighted_point_3 r) {
      return Compute_squared_radius_base()(p,q,r); }

    FT operator() (Weighted_point_3 p, 
		   Weighted_point_3 q ) {
      return Compute_squared_radius_base()(p,q); }

    FT operator() (Weighted_point_3 p) {
      return -p.weight();}
  };
 
  

  //---------------------------------------------------------------------

  Compute_squared_radius_3 
  compute_squared_radius_3_object() const
    {
      return Compute_squared_radius_3();
    }
  //---------------------------------------------------------------------

  Side_of_bounded_sphere_3 
  side_of_bounded_sphere_3_object() const
    {
      return Side_of_bounded_sphere_3();
    }
};

CGAL_END_NAMESPACE

#endif //CGAL_WEIGHTED_ALPHA_SHAPE_EUCLIDEAN_TRAITS_3_H 
