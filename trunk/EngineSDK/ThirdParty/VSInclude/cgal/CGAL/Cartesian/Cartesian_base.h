// Copyright (c) 2000-2004  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/Cartesian_base.h $
// $Id: Cartesian_base.h 31661 2006-06-16 21:12:43Z spion $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_CARTESIAN_BASE_H
#define CGAL_CARTESIAN_BASE_H

#define CGAL_REP_CLASS_DEFINED

#include <CGAL/basic.h>
#include <CGAL/basic_classes.h>
#include <CGAL/Kernel/global_functions.h>

#include <CGAL/Cartesian/Point_2.h>
#include <CGAL/Cartesian/Vector_2.h>
#include <CGAL/Cartesian/Direction_2.h>
#include <CGAL/Cartesian/Line_2.h>
#include <CGAL/Cartesian/Ray_2.h>
#include <CGAL/Cartesian/Segment_2.h>
#include <CGAL/Cartesian/Triangle_2.h>
#include <CGAL/Cartesian/Circle_2.h>
#include <CGAL/Cartesian/Iso_rectangle_2.h>
#include <CGAL/Cartesian/Aff_transformation_2.h>
#include <CGAL/Cartesian/Data_accessor_2.h>
#include <CGAL/ConicCPA2.h>

#include <CGAL/Cartesian/predicates_on_points_2.h>
#include <CGAL/Cartesian/predicates_on_directions_2.h>
#include <CGAL/Cartesian/basic_constructions_2.h>

#include <CGAL/Cartesian/Point_3.h>
#include <CGAL/Cartesian/Vector_3.h>
#include <CGAL/Cartesian/Direction_3.h>
#include <CGAL/Cartesian/Line_3.h>
#include <CGAL/Cartesian/Plane_3.h>
#include <CGAL/Cartesian/Ray_3.h>
#include <CGAL/Cartesian/Segment_3.h>
#include <CGAL/Cartesian/Triangle_3.h>
#include <CGAL/Cartesian/Tetrahedron_3.h>
#include <CGAL/Cartesian/Iso_cuboid_3.h>
#include <CGAL/Cartesian/Sphere_3.h>
#include <CGAL/Cartesian/Aff_transformation_3.h>

#include <CGAL/Cartesian/predicates_on_points_3.h>
#include <CGAL/Cartesian/predicates_on_planes_3.h>
#include <CGAL/Cartesian/basic_constructions_3.h>

#include <CGAL/representation_tags.h>
#include <CGAL/Cartesian/function_objects.h>
#include <CGAL/Uncertain.h>

CGAL_BEGIN_NAMESPACE

template < typename K_, typename FT_>
struct Cartesian_base
{
    typedef K_                                          Kernel;
    typedef FT_                                         FT;
    typedef Cartesian_base<K_,FT_>                      Self;
    typedef Cartesian_tag                               Rep_tag;
    typedef Cartesian_tag                               Kernel_tag;

    typedef CGAL::Object                                Object_2;
    typedef CGAL::Object                                Object_3;

    // These are currently undocumented.
    // Should they be part of the Kernel interface ?
    typedef typename Same_uncertainty_nt<bool, FT>::type
                                                        Bool_type;
    typedef typename Same_uncertainty_nt<CGAL::Sign, FT>::type
                                                        Sign;
    typedef typename Same_uncertainty_nt<CGAL::Comparison_result, FT>::type
                                                        Comparison_result;
    typedef typename Same_uncertainty_nt<CGAL::Orientation, FT>::type
                                                        Orientation;
    typedef typename Same_uncertainty_nt<CGAL::Oriented_side, FT>::type
                                                        Oriented_side;
    typedef typename Same_uncertainty_nt<CGAL::Bounded_side, FT>::type
                                                        Bounded_side;
    typedef typename Same_uncertainty_nt<CGAL::Angle, FT>::type
                                                        Angle;

    typedef PointC2<Kernel>                             Point_2;
    typedef VectorC2<Kernel>                            Vector_2;
    typedef DirectionC2<Kernel>                         Direction_2;
    typedef SegmentC2<Kernel>                           Segment_2;
    typedef LineC2<Kernel>                              Line_2;
    typedef RayC2<Kernel>                               Ray_2;
    typedef TriangleC2<Kernel>                          Triangle_2;
    typedef CircleC2<Kernel>                            Circle_2;
    typedef Iso_rectangleC2<Kernel>                     Iso_rectangle_2;
    typedef Aff_transformationC2<Kernel>                Aff_transformation_2;

    typedef PointC3<Kernel>                             Point_3;
    typedef VectorC3<Kernel>                            Vector_3;
    typedef DirectionC3<Kernel>                         Direction_3;
    typedef LineC3<Kernel>                              Line_3;
    typedef PlaneC3<Kernel>                             Plane_3;
    typedef RayC3<Kernel>                               Ray_3;
    typedef SegmentC3<Kernel>                           Segment_3;
    typedef TriangleC3<Kernel>                          Triangle_3;
    typedef TetrahedronC3<Kernel>                       Tetrahedron_3;
    typedef Iso_cuboidC3<Kernel>                        Iso_cuboid_3;
    typedef SphereC3<Kernel>                            Sphere_3;
    typedef Aff_transformationC3<Kernel>                Aff_transformation_3;

    typedef const FT_*                                  Cartesian_const_iterator_2;
    typedef const FT_*                                  Cartesian_const_iterator_3;

    // Undocumented stuff.
    typedef Data_accessorC2<Kernel>                     Data_accessor_2;
    typedef ConicCPA2<Point_2,Data_accessor_2>          Conic_2;
};

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_BASE_H
