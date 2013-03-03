// Copyright (c) 2003  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Intersections_3/include/CGAL/Triangle_3_Ray_3_do_intersect.h $
// $Id: Triangle_3_Ray_3_do_intersect.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Philippe Guigue

#ifndef CGAL_TRIANGLE_3_RAY_3_DO_INTERSECT_H
#define CGAL_TRIANGLE_3_RAY_3_DO_INTERSECT_H

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template <class K>
bool do_intersect(const typename CGAL_WRAP(K)::Triangle_3 &t, 
		  const typename CGAL_WRAP(K)::Ray_3 &r,
		  const K & k)
{

  CGAL_kernel_precondition( ! k.is_degenerate_3_object()(t) ) ;
  CGAL_kernel_precondition( ! k.is_degenerate_3_object()(r) ) ;

  typedef typename K::Point_3 Point_3;

  typename K::Construct_vertex_3 vertex_on =
    k.construct_vertex_3_object();
  
  typename K::Orientation_3 orientation = 
    k.orientation_3_object();


  const Point_3 & a = vertex_on(t,0);
  const Point_3 & b = vertex_on(t,1);
  const Point_3 & c = vertex_on(t,2);
  
  typename K::Construct_vector_3  construct_vector =
    k.construct_vector_3_object();

  typename K::Construct_ray_3 construct_ray = 
    k.construct_ray_3_object();

  typename K::Construct_point_on_3 point_on = 
    k.construct_point_on_3_object();

  const Point_3 & p = point_on(r,0);
  const Point_3 & q = point_on(r,1);


  const Orientation ray_direction =
    orientation(a,b,c,point_on(construct_ray(a, construct_vector(r)),1));

    if (ray_direction == COPLANAR ) {
      if (orientation(a,b,c,p) == COPLANAR) 
	return do_intersect_coplanar(t,r,k);
      else return false;
    }
  
  const Orientation abcp = orientation(a,b,c,p);

  switch ( abcp ) {
  case POSITIVE: 
    switch ( ray_direction ) {
    case POSITIVE: 
      // the ray lies in the positive open halfspaces defined by the
      // triangle's supporting plane
      return false;
      
    case NEGATIVE:
      // The ray straddles the triangle's plane
      // p sees the triangle in counterclockwise order
      
      return orientation(p,q,a,b) != POSITIVE
	&& orientation(p,q,b,c) != POSITIVE
	&& orientation(p,q,c,a) != POSITIVE;
      
      // case COPLANAR: should not happen
      
    default: // should not happen.
      CGAL_kernel_assertion(false);
      return false;
    }
    
  case NEGATIVE:
    switch ( ray_direction ) {
    case POSITIVE: 
      // The ray straddles the triangle's plane
      // q sees the triangle in counterclockwise order
      
      return orientation(q,p,a,b) != POSITIVE
	&& orientation(q,p,b,c) != POSITIVE
	&& orientation(q,p,c,a) != POSITIVE;
      
    case NEGATIVE:
      // the ray lies in the negative open halfspaces defined by the
      // triangle's supporting plane
      return false;
      
      // case COPLANAR: should not happen
      
    default: // should not happen.
      CGAL_kernel_assertion(false);
      return false;
    }
    
  case COPLANAR: // p belongs to the triangle's supporting plane
    switch ( ray_direction ) {
    case POSITIVE: 
      // q sees the triangle in counterclockwise order
      return orientation(q,p,a,b) != POSITIVE
	&& orientation(q,p,b,c) != POSITIVE
	&& orientation(q,p,c,a) != POSITIVE;
      
    case NEGATIVE:
      // q sees the triangle in clockwise order
      return orientation(p,q,a,b) != POSITIVE
	&& orientation(p,q,b,c) != POSITIVE
	&& orientation(p,q,c,a) != POSITIVE;
      
      // case COPLANAR: should not happen
      
    default: // should not happen.
      CGAL_kernel_assertion(false);
      return false;
    }
    
  default: // should not happen.
    CGAL_kernel_assertion(false);
    return false;
    
  } 
}


template <class K>
inline
bool do_intersect(const typename CGAL_WRAP(K)::Ray_3 &r,
		  const typename CGAL_WRAP(K)::Triangle_3 &t, 
		  const K & k)
{
  return do_intersect(t,r, k);
}


template <class K>
bool do_intersect_coplanar(const typename CGAL_WRAP(K)::Triangle_3 &t, 
			   const typename CGAL_WRAP(K)::Ray_3      &r,
			   const K & k )
{
  
  CGAL_kernel_precondition( ! k.is_degenerate_3_object()(t) ) ;
  CGAL_kernel_precondition( ! k.is_degenerate_3_object()(r) ) ;
  
  typedef typename K::Point_3 Point_3;
  
  typename K::Construct_point_on_3 point_on = 
    k.construct_point_on_3_object();
  
  typename K::Construct_vertex_3 vertex_on =
    k.construct_vertex_3_object();
  
  typename K::Coplanar_orientation_3 coplanar_orientation = 
    k.coplanar_orientation_3_object();

  
  const Point_3 & p = point_on(r,0);
  const Point_3 & q = point_on(r,1);
  
  const Point_3 & A = vertex_on(t,0);
  const Point_3 & B = vertex_on(t,1);
  const Point_3 & C = vertex_on(t,2);
  
  
  const Point_3 * a = &A;
  const Point_3 * b = &B;
  const Point_3 * c = &C;
  // Determine the orientation of the triangle in the common plane

  if (coplanar_orientation(A,B,C) != POSITIVE)
    {
      // The triangle is not counterclockwise oriented
      // swap two vertices.
      b = &C;
      c = &B;
    }

  // Test whether the ray's supporting line intersects the
  // triangle in the common plane
  
  const Orientation pqa = coplanar_orientation(p,q,*a);
  const Orientation pqb = coplanar_orientation(p,q,*b);
  const Orientation pqc = coplanar_orientation(p,q,*c);


  switch ( pqa ) {
  case POSITIVE:
    switch ( pqb ) {
    case POSITIVE:
       if (pqc == POSITIVE) 
	 // the triangle lies in the positive halfspace
	 // defined by the ray's supporting line.
	 return false;
       // c is isolated on the negative side
       return coplanar_orientation(*a,*c,p) != POSITIVE ;
       
    case NEGATIVE:
      if (pqc == POSITIVE) // b is isolated on the negative side
	return coplanar_orientation(*c,*b,p) != POSITIVE ;
      // a is isolated on the positive side
      return coplanar_orientation(*a,*c,p) != POSITIVE ;

    case COLLINEAR:
      if (pqc == POSITIVE) // b is isolated on the negative side
	return coplanar_orientation(*c,*b,p) != POSITIVE ;
      // a is isolated on the positive side
      return  coplanar_orientation(*a,*c,p) != POSITIVE ;
      
    default: // should not happen.
      CGAL_kernel_assertion(false);
      return false;
    }
    
  case NEGATIVE:
    switch ( pqb ) {
    case POSITIVE:
      if (pqc == POSITIVE) 	// a is isolated on the negative side
	return  coplanar_orientation(*b,*a,p) != POSITIVE ;
      // b is isolated on the positive side
      return  coplanar_orientation(*b,*a,p) != POSITIVE ;
      
    case NEGATIVE:
      if (pqc == NEGATIVE) 	
	// the triangle lies in the negative halfspace
	// defined by the ray's supporting line.
	return false;
      // c is isolated on the positive side
      return  coplanar_orientation(*c,*b,p) != POSITIVE ;
    case COLLINEAR:
      if (pqc == NEGATIVE) // b is isolated on the positive side
	return  coplanar_orientation(*b,*a,p) != POSITIVE ;
      // a is isolated on the negative side
      return  coplanar_orientation(*b,*a,p) != POSITIVE ;
      
    default: // should not happen.
      CGAL_kernel_assertion(false);
      return false;
    }
    
  case COLLINEAR:
    switch ( pqb ) {
    case POSITIVE:
      if (pqc == POSITIVE) // a is isolated on the negative side
	return  coplanar_orientation(*b,*a,p) != POSITIVE ;
      // b is isolated on the positive side
      return  coplanar_orientation(*b,*a,p) != POSITIVE ;
    case NEGATIVE:
      if (pqc == NEGATIVE) // a is isolated on the positive side
	return  coplanar_orientation(*a,*c,p) != POSITIVE ;
      // b is isolated on the negative side
      return  coplanar_orientation(*c,*b,p) != POSITIVE ;

    case COLLINEAR:
      if (pqc == POSITIVE) // c is isolated on the positive side
	return  coplanar_orientation(*c,*b,p) != POSITIVE ;
      // c is isolated on the negative side
      return  coplanar_orientation(*a,*c,p) != POSITIVE ;
      // case pqc == COLLINEAR is imposiible

    default: // should not happen.
      CGAL_kernel_assertion(false);
      return false;
    }
    
  default: // should not happen.
    CGAL_kernel_assertion(false);
    return false;
  }
}


} // namespace CGALi


template <class K>
inline bool do_intersect(const Ray_3<K> &r, 
			 const Triangle_3<K> &t)
{
  return typename K::Do_intersect_3()(t,r);
}

template <class K>
inline bool do_intersect(const Triangle_3<K> &t, 
			 const Ray_3<K> &r)
{
  return typename K::Do_intersect_3()(t,r);
}


/*
template <class K>
inline bool do_intersect(const Ray_3<K> &r, 
			 const Triangle_3<K> &t,
			 const K & k )
{
  return CGALi::do_intersect(t,r,k);
}
*/

CGAL_END_NAMESPACE

#endif // CGAL_TRIANGLE_3_RAY_3_DO_INTERSECT_H
