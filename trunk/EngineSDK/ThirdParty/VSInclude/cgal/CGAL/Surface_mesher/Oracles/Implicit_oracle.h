// Copyright (c) 2003-2006  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Surface_mesher/include/CGAL/Surface_mesher/Oracles/Implicit_oracle.h $
// $Id: Implicit_oracle.h 30093 2006-04-06 22:52:38Z lrineau $
//
//
// Author(s)     : Steve OUDOT, Laurent RINEAU

#ifndef CGAL_SURFACE_MESHER_IMPLICIT_ORACLE_H
#define CGAL_SURFACE_MESHER_IMPLICIT_ORACLE_H

#include <CGAL/Surface_mesher/Oracles/Null_oracle_visitor.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/Surface_mesher/Oracles/Sphere_oracle_3.h>

#include <queue>

#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
#  include <string>
#  include <sstream>
#endif

// NB: this oracle requires that the user provide a function that can
// compute the value of the potential in any point of space
namespace CGAL {

  namespace Surface_mesher {

    /**
       Type and functions required in GT:
         - Compute_scalar_product_3             (from rev. 29646)
         - Compute_squared_distance_3
         - Compute_squared_radius_3
         - Construct_center_3
         - Construct_midpoint_3
         - Construct_point_on_3                 (from rev. 29646)
         - Construct_scaled_vector_3
         - Construct_segment_3                  (from rev. 29646)
         - Construct_translated_point_3
         - Construct_vector_3
         - FT
         - Has_on_bounded_side_3                (from rev. 29646)
         - Line_3
         - Point_3
         - Ray_3
         - Segment_3
         - Sphere_3
         - Vector_3                             (from rev. 29646)
         - compute_scalar_product_3_object      (from rev. 29646)
         - compute_squared_distance_3_object
         - compute_squared_radius_3_object
         - construct_center_3_object
         - construct_line_3_object              (no longer, since rev. 29646)
         - construct_midpoint_3_object
         - construct_point_on_3_object          (from rev. 29646)
         - construct_scaled_vector_3_object
         - construct_segment_3_object           (from rev. 29646)
         - construct_translated_point_3_object
         - construct_vector_3_object
         - has_on_bounded_side_3_object         (from rev. 29646)
(Computed by use of:
 perl -ne '/GT\(\)\.([a-zA-Z_0-9]+)/
             && print "$1\n";' {Implicit_oracle.h,Sphere_oracle_3.h} | sort -u
 perl -ne '/GT::([a-zA-Z_0-9]+)/
             && print "$1\n";' {Implicit_oracle.h,Sphere_oracle_3.h} | sort -u
)
    */
  template <
    class GT,
    class Surface,
    class Point_creator = Creator_uniform_3<typename GT::FT,
                                            typename GT::Point_3>,
    class Visitor = Null_oracle_visitor
    >
  class Implicit_surface_oracle
  {
    // private types
    typedef Implicit_surface_oracle<GT, Surface, Point_creator, Visitor> Self;

    typedef Sphere_oracle_3<GT, Point_creator> Sphere_oracle;
    
    typedef typename GT::Point_3 Point;
#ifdef CGAL_DELAUNAY_MESH_3_H
    typedef typename Kernel_traits<Point>::Kernel::Point_3 Kernel_point;
#else
    typedef Point Kernel_point;
#endif

    typedef typename GT::FT FT;
    typedef typename GT::Sphere_3 Sphere_3;

  public:

    // Public types
    typedef GT Geom_traits;
    typedef typename GT::Point_3 Point_3;
    typedef typename GT::Segment_3 Segment_3;
    typedef typename GT::Ray_3 Ray_3;
    typedef typename GT::Line_3 Line_3;

    typedef Surface Surface_3;

  private:
    // Private members
    Visitor visitor; // a visitor that can modify a point, before returning it.

  public:

    // Constructors
    Implicit_surface_oracle (Visitor visitor_ = Visitor() ) :
      visitor(visitor_)
    {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CONSTRUCTORS
      std::cerr << "CONS: Implicit_surface_oracle\n";
#endif
    }

    class Intersect_3 
    {
      Visitor visitor;

    public:
      Intersect_3(Visitor visitor) : visitor(visitor)
      {
      }

      Object operator()(const Surface_3& surface, Segment_3 s) const
      // s is passed by value, because it is clipped below
      {
        typename GT::Construct_point_on_3 point_on =
          GT().construct_point_on_3_object();

        typename Sphere_oracle::Intersect_3 clip =
          Sphere_oracle().intersect_3_object();

        const Sphere_3& sphere = surface.bounding_sphere();

        Point_3 a = point_on(s, 0);
        Point_3 b = point_on(s, 1);

        // if both extremities are on the same side of the surface, return
        // no intersection
        if(surf_equation(surface, a) * surf_equation(surface, b) > 0)
          return Object();

        // Code for surfaces with boundaries
        // First rescale segment s = [a, b]
        if( clip.clip_segment(sphere, a, b) )
          return intersect_clipped_segment(surface,
                                           a,
                                           b,
                                           surface.squared_error_bound());
        // else
        return Object();
      } // end operator()(Surface_3, Segment_3)

      Object operator()(const Surface_3& surface, const Ray_3& r) const {
        typename Sphere_oracle::Intersect_3 clip =
          Sphere_oracle().intersect_3_object();

        const Sphere_3& sphere = surface.bounding_sphere();

        Point_3 a, b;
        if(clip.clip_ray(sphere, r, a, b))
        {
          return intersect_clipped_segment(surface,
                                           a,
                                           b,
                                           surface.squared_error_bound());
        }
        // else
        return Object();
      } // end operator()(Surface_3, Ray_3)

      Object operator()(const Surface_3& surface, const Line_3& l) const {
        typename Sphere_oracle::Intersect_3 clip =
          Sphere_oracle().intersect_3_object();

        const Sphere_3& sphere = surface.bounding_sphere();

        Point_3 a, b;
        if(clip.clip_line(sphere, l, a, b))
        {
          return intersect_clipped_segment(surface,
                                           a,
                                           b,
                                           surface.squared_error_bound());
        }
        else
          return Object();
      }; // end operator()(Surface_3, Line_3)

      // debug function
      static std::string debug_point(const Surface_3& surface,
                                     const Point& p)
      {
        std::stringstream s;
        s << p << " (distance=" 
          << CGAL::sqrt(CGAL::squared_distance(p,
                                 surface.bounding_sphere().center()))
          << ", sign=" << surf_equation(surface, p)
          << ")";
        return s.str();
      }

    private:
      // Private functions
      static CGAL::Sign surf_equation (Surface_3 surface,
                                       const Point& p)
      {
        return CGAL::sign(surface(p));
      } // @TODO, @WARNING: we use x(), y() and z()

      Object intersect_clipped_segment(const Surface_3& surface,
                                       Point p1,
                                       Point p2,
                                       const FT& squared_distance_bound) const
      {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
        std::cerr << "clipped_segment\n";
#endif
        typename GT::Compute_squared_distance_3 squared_distance = 
          GT().compute_squared_distance_3_object();
        typename GT::Construct_midpoint_3 midpoint =
          GT().construct_midpoint_3_object();

        Sign sign_at_p1 = surf_equation(surface, p1);
        Sign sign_at_p2 = surf_equation(surface, p2);

        if( sign_at_p1 == ZERO )
        {
          visitor.new_point(p1);
          return make_object(p1);
        }
        if( sign_at_p2 == ZERO )
        {
          visitor.new_point(p2);
          return make_object(p2);
        }

        // if both extremities are on the same side of the surface, return
        // no intersection
        if(sign_at_p1 * sign_at_p2 > 0)
          return Object();

        while(true)
        {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
          std::cerr << debug_point(surface, p1) << ", "
                    << debug_point(surface, p2) << "\n";
#endif
          const Point mid = midpoint(p1, p2);
          const Sign sign_at_mid = surf_equation(surface, mid);

          if ( sign_at_mid == ZERO || 
               squared_distance(p1, p2) < squared_distance_bound )
          // If the two points are close, then we must decide
          {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CLIPPED_SEGMENT
            std::cerr << "=" << debug_point(surface, mid) << "\n";
#endif
            visitor.new_point(mid);
            return make_object(mid);
          }

          // Else we must go on
          if ( sign_at_p1 * sign_at_mid < 0 )
          {
            p2 = mid;
            sign_at_p2 = sign_at_mid;
          }
          else
          {
            p1 = mid;
            sign_at_p1 = sign_at_mid;
          }
        }
      } // end intersect_clipped_segment

    }; // end nested class Intersect_3

    class Construct_initial_points
    {
      Self& oracle;
    public:
      Construct_initial_points(Self& oracle) : oracle(oracle)
      {
      }
      
      // Random points
      template <typename OutputIteratorPoints>
      OutputIteratorPoints operator() (const Surface_3& surface, 
                                       OutputIteratorPoints out, 
                                       int n = 20) // WARNING: why 20?
      {
        const Sphere_3& sphere = surface.bounding_sphere();
        const Point center = 
          GT().construct_center_3_object()(sphere);
        const FT squared_radius = 
          GT().compute_squared_radius_3_object()(sphere);
        const FT radius = CGAL::sqrt(squared_radius);

        typename CGAL::Random_points_on_sphere_3<Point,
          Point_creator> random_point_on_sphere(CGAL::to_double(radius));
        typename GT::Construct_segment_3 segment_3 = 
          GT().construct_segment_3_object();
        typename GT::Construct_vector_3 vector_3 =
          GT().construct_vector_3_object();
        typename GT::Construct_translated_point_3 translate =
          GT().construct_translated_point_3_object();

        while (n>0) 
        {
          const Point p = translate(*random_point_on_sphere++,
                                    vector_3(CGAL::ORIGIN, center));

#ifdef CGAL_SURFACE_MESHER_DEBUG_INITIAL_POINTS
          std::cerr << "test " << Self::Intersect_3::debug_point(surface, center)
                    << ", " << Self::Intersect_3::debug_point(surface, p);
#endif

          Object o = oracle.intersect_3_object()(surface,
                                                 segment_3(center, p));
          Point intersection;
          if (assign(intersection, o)) {
            *out++= intersection;
            --n;
#ifdef CGAL_SURFACE_MESHER_DEBUG_INITIAL_POINTS
            std::cerr << " = "
                      << Self::Intersect_3::debug_point(surface, intersection)
                      << "\n";
#endif
          }
#ifdef CGAL_SURFACE_MESHER_DEBUG_INITIAL_POINTS
          else
            std::cerr << " = null\n";
#endif
        }
        return out;
      }
    }; // end nested class Construct_initial_points

    Construct_initial_points construct_initial_points_object()
    {
      return Construct_initial_points(*this);
    }

    Intersect_3 intersect_3_object()
    {
      return Intersect_3(visitor);
    }

    bool is_in_volume(const Surface_3& surface, const Point& p)
    {
      return Intersect_3::surf_equation(surface, p)<0.;
    }

  };  // end Implicit_surface_oracle

template <typename FT>
FT approximate_sqrt(const FT x) {
  return FT (CGAL_NTS sqrt(CGAL_NTS to_double(x)));
}

  }  // namespace Surface_mesher

} // namespace CGAL


#endif  // CGAL_SURFACE_MESHER_IMPLICIT_ORACLE_H
