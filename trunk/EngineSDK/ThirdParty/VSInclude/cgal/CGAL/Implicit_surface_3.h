// Copyright (c) 2006  INRIA Sophia-Antipolis (France).
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
// $Source: 
// $Revision: 30642 $ $Date: 2006-04-18 14:42:52 +0200 (Tue, 18 Apr 2006) $
// $Name:  $
//
// Author(s)     : Laurent RINEAU

#ifndef CGAL_IMPLICIT_SURFACE_3_H
#define CGAL_IMPLICIT_SURFACE_3_H

#include <CGAL/make_surface_mesh.h>
#include <CGAL/Surface_mesher/Oracles/Implicit_oracle.h>

#include <functional>

namespace CGAL {

  template<
    typename GT,
    typename Function
    >
  class Implicit_surface_3 
  {
  public:
    typedef GT Geom_traits;
    typedef typename Geom_traits::Sphere_3 Sphere_3;
    typedef typename Geom_traits::FT FT;
    typedef typename Geom_traits::Point_3 Point;

    Implicit_surface_3(Function f,
		       const Sphere_3 bounding_sphere,
		       const FT error_bound = FT(1e-3))
      : func(f),
	sphere(bounding_sphere)
    {
      squared_error = error_bound * error_bound;
      squared_error = squared_error * 
        GT().compute_squared_radius_3_object()(bounding_sphere);
    }

    FT operator()(Point p)
    {
      return func(p);
    }

    const FT& squared_error_bound() const
    {
      return squared_error;
    }

    const Sphere_3& bounding_sphere() const
    {
      return sphere;
    }

  private:
    Function func;
    Sphere_3 sphere;
    FT squared_error;
  }; // end Implicit_surface_3


  template <typename GT, typename Function>
  Implicit_surface_3<GT, Function>
  make_implicit_surface_3(GT, Function f,
			  typename GT::Sphere_3 sphere,
			  typename GT::FT error_bound)
  {
    typedef Implicit_surface_3<GT, Function> surface;
    return surface(f, sphere, error_bound);
  }

  template <typename GT, typename Function>
  struct Surface_mesh_traits_generator_3<Implicit_surface_3<GT, Function> >
  {
    typedef Implicit_surface_3<GT, Function> Surface_type;
    typedef typename Surface_mesher::Implicit_surface_oracle<GT,
							     Surface_type> Type;
    typedef Type type; // Boost meta-programming compatibility
  };

  // non documented class
  template <typename FT, typename Point>
  class Implicit_function_wrapper : public std::unary_function<Point, FT> 
  {
    typedef FT (*Implicit_function)(FT, FT, FT);

    Implicit_function function;

  public:
    Implicit_function_wrapper(Implicit_function f) : function(f) {}

    FT operator()(Point p)
    {
      return function(p.x(), p.y(), p.z());
    }
  };

} // end namespace CGAL

#endif // CGAL_IMPLICIT_SURFACE_3_H
