// Copyright (c) 2001-2004  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian_converter.h $
// $Id: Cartesian_converter.h 30707 2006-04-21 12:06:39Z afabri $
// 
//
// Author(s)     : Sylvain Pion <Sylvain.Pion@sophia.inria.fr>
//                 Menelaos Karavelas <mkaravel@cse.nd.edu>

#ifndef CGAL_CARTESIAN_CONVERTER_H
#define CGAL_CARTESIAN_CONVERTER_H

// This file contains the definition of a kernel converter, based on Cartesian
// representation.  It should work between *Cartesian<A> and *Cartesian<B>,
// provided you give a NT converter from A to B.
// There's a Homogeneous counterpart.

#include <CGAL/basic.h>
#include <CGAL/NT_converter.h>
#include <CGAL/Enum_converter.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Origin.h>
#include <vector>

CGAL_BEGIN_NAMESPACE

// Guess which compiler needs this work around ?
namespace CGALi {
template < typename K1, typename K2 >
struct Default_converter {
  typedef typename K1::FT FT1;
  typedef typename K2::FT FT2;
  typedef ::CGAL::NT_converter<FT1, FT2> Type;
};
} // namespace CGALi

template < class K1, class K2,
//          class Converter = NT_converter<typename K1::FT, typename K2::FT> >
           class Converter = typename CGALi::Default_converter<K1, K2>::Type >
class Cartesian_converter : public Enum_converter
{
    typedef Enum_converter   Base;

public:
    typedef K1         Source_kernel;
    typedef K2         Target_kernel;
    typedef Converter  Number_type_converter;

#ifdef CGAL_CFG_USING_BASE_MEMBER_BUG
    bool operator()(bool b) const { return Base::operator()(b); }
    Sign operator()(Sign s) const { return Base::operator()(s); }

    Oriented_side operator()(Oriented_side os) const {
      return Base::operator()(os);
    }

    Bounded_side operator()(Bounded_side bs) const {
      return Base::operator()(bs);
    }

    Comparison_result operator()(Comparison_result cr) const {
      return Base::operator()(cr);
    }

    Angle operator()(Angle a) const { return Base::operator()(a); }
#else
    using Base::operator();
#endif

    Cartesian_converter() // To shut up a warning with SunPRO.
	: c(), k() {}

    Origin
    operator()(const Origin& o) const
    {
        return o;
    }

    Null_vector
    operator()(const Null_vector& n) const
    {
        return n;
    }

    Bbox_2
    operator()(const Bbox_2& b) const
    {
        return b;
    }

    Bbox_3
    operator()(const Bbox_3& b) const
    {
        return b;
    }

    typename K2::FT
    operator()(const typename K1::FT &a) const
    {
        return c(a);
    }

    typename K2::Object_2
    operator()(const typename K1::Object_2 &obj) const
    {
      if (const typename K1::Point_2 * ptr = object_cast<typename K1::Point_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Vector_2 * ptr = object_cast<typename K1::Vector_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Direction_2 * ptr = object_cast<typename K1::Direction_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Segment_2 * ptr = object_cast<typename K1::Segment_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Ray_2 * ptr = object_cast<typename K1::Ray_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Line_2 * ptr = object_cast<typename K1::Line_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Triangle_2 * ptr = object_cast<typename K1::Triangle_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Iso_rectangle_2 * ptr = object_cast<typename K1::Iso_rectangle_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Circle_2 * ptr = object_cast<typename K1::Circle_2>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Point_3 * ptr = object_cast<typename K1::Point_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Vector_3 * ptr = object_cast<typename K1::Vector_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Direction_3 * ptr = object_cast<typename K1::Direction_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Segment_3 * ptr = object_cast<typename K1::Segment_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Ray_3 * ptr = object_cast<typename K1::Ray_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Line_3 * ptr = object_cast<typename K1::Line_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Triangle_3 * ptr = object_cast<typename K1::Triangle_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Tetrahedron_3 * ptr = object_cast<typename K1::Tetrahedron_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Iso_cuboid_3 * ptr = object_cast<typename K1::Iso_cuboid_3>(&obj)) {
        return make_object(operator()(*ptr));
      } else if (const typename K1::Sphere_3 * ptr = object_cast<typename K1::Sphere_3>(&obj)) {
        return make_object(operator()(*ptr));
      }else if (const typename K1::Plane_3 * ptr = object_cast<typename K1::Plane_3>(&obj)) {
        return make_object(operator()(*ptr));
      }else if (const std::vector<typename K1::Point_2> * ptr = object_cast<std::vector<typename K1::Point_2> >(&obj)) {
	std::vector<typename K2::Point_2> res((*ptr).size());
	for(unsigned int i=0; i < (*ptr).size(); i++){
	  res[i] = operator()((*ptr)[i]);
	}
	return make_object(res);
      }
      CGAL_assertion_msg(false,"Cartesian_converter is unable to determine what is wrapped in the Object");
      return Object();
	
    }

  std::vector<Object>
  operator()(const std::vector<Object>& v) const
  {
    std::vector<Object> res;
    res.reserve(v.size());
    for(unsigned int i = 0; i < v.size(); i++){
      res[i] = operator()(v[i]);
    }
    return res;
  }


    typename K2::Point_2
    operator()(const typename K1::Point_2 &a) const
    {
        typedef typename K2::Point_2 Point_2;
	return Point_2(c(a.x()), c(a.y()));
    }

    typename K2::Vector_2
    operator()(const typename K1::Vector_2 &a) const
    {
        typedef typename K2::Vector_2  Vector_2;
	return Vector_2(c(a.x()), c(a.y()));
    }

    typename K2::Direction_2
    operator()(const typename K1::Direction_2 &a) const
    {
        typedef typename K2::Direction_2  Direction_2;
	return Direction_2(c(a.dx()), c(a.dy()));
    }

    typename K2::Segment_2
    operator()(const typename K1::Segment_2 &a) const
    {
        typedef typename K2::Segment_2  Segment_2;
	return Segment_2(operator()(a.source()), operator()(a.target()));
    }

    typename K2::Line_2
    operator()(const typename K1::Line_2 &a) const
    {
        typedef typename K2::Line_2 Line_2;
	return Line_2(c(a.a()), c(a.b()), c(a.c()));
    }

    typename K2::Ray_2
    operator()(const typename K1::Ray_2 &a) const
    {
        typedef typename K2::Ray_2  Ray_2;
	return Ray_2(operator()(a.source()), operator()(a.second_point()));
    }

    typename K2::Circle_2
    operator()(const typename K1::Circle_2 &a) const
    {
        typedef typename K2::Circle_2  Circle_2;
	return Circle_2(operator()(a.center()),
		        c(a.squared_radius()),
			a.rep().orientation());
    }

    typename K2::Triangle_2
    operator()(const typename K1::Triangle_2 &a) const
    {
        typedef typename K2::Triangle_2  Triangle_2;
	return Triangle_2(operator()(a.vertex(0)),
		          operator()(a.vertex(1)),
		          operator()(a.vertex(2)));
    }

    typename K2::Iso_rectangle_2
    operator()(const typename K1::Iso_rectangle_2 &a) const
    {
        typedef typename K2::Iso_rectangle_2  Iso_rectangle_2;
	return Iso_rectangle_2(operator()(a.min()), operator()(a.max()));
    }


    typename K2::Point_3
    operator()(const typename K1::Point_3 &a) const
    {
        typedef typename K2::Point_3 Point_3;
	return Point_3(c(a.x()), c(a.y()), c(a.z()));
    }

    typename K2::Vector_3
    operator()(const typename K1::Vector_3 &a) const
    {
        typedef typename K2::Vector_3  Vector_3;
	return Vector_3(c(a.x()), c(a.y()), c(a.z()));
    }

    typename K2::Direction_3
    operator()(const typename K1::Direction_3 &a) const
    {
        typedef typename K2::Direction_3  Direction_3;
	return Direction_3(c(a.dx()), c(a.dy()), c(a.dz()));
    }

    typename K2::Segment_3
    operator()(const typename K1::Segment_3 &a) const
    {
        typedef typename K2::Segment_3  Segment_3;
	return Segment_3(operator()(a.source()), operator()(a.target()));
    }

    typename K2::Line_3
    operator()(const typename K1::Line_3 &a) const
    {
        typedef typename K2::Line_3  Line_3;
	return Line_3(operator()(a.point()), operator()(a.direction()));
    }

    typename K2::Ray_3
    operator()(const typename K1::Ray_3 &a) const
    {
        typedef typename K2::Ray_3 Ray_3;
	return Ray_3(operator()(a.source()), operator()(a.second_point()));
    }

    typename K2::Sphere_3
    operator()(const typename K1::Sphere_3 &a) const
    {
        typedef typename K2::Sphere_3  Sphere_3;
	return Sphere_3(operator()(a.center()),
		        c(a.squared_radius()),
			a.orientation());
    }

    typename K2::Triangle_3
    operator()(const typename K1::Triangle_3 &a) const
    {
        typedef typename K2::Triangle_3  Triangle_3;
	return Triangle_3(operator()(a.vertex(0)),
		          operator()(a.vertex(1)),
		          operator()(a.vertex(2)));
    }

    typename K2::Tetrahedron_3
    operator()(const typename K1::Tetrahedron_3 &a) const
    {
        typedef typename K2::Tetrahedron_3  Tetrahedron_3;
	return Tetrahedron_3(operator()(a.vertex(0)),
		             operator()(a.vertex(1)),
		             operator()(a.vertex(2)),
		             operator()(a.vertex(3)));
    }

    typename K2::Plane_3
    operator()(const typename K1::Plane_3 &a) const
    {
        typedef typename K2::Plane_3  Plane_3;
	return Plane_3(c(a.a()), c(a.b()), c(a.c()), c(a.d()));
    }

    typename K2::Iso_cuboid_3
    operator()(const typename K1::Iso_cuboid_3 &a) const
    {
        typedef typename K2::Iso_cuboid_3 Iso_cuboid_3;
	return Iso_cuboid_3(operator()(a.min()), operator()(a.max()));
    }

    std::pair<typename K2::Point_2, typename K2::Point_2>
    operator() (const std::pair<typename K1::Point_2, typename K1::Point_2>& pp) const
    {
      return std::make_pair(operator()(pp.first), operator()(pp.second));
    }

private:
    Converter c;
    K2 k;
};

// Specialization when converting to the same kernel,
// to avoid making copies.
template < class K, class C >
class Cartesian_converter <K, K, C>
{
public:
  typedef K Source_kernel;
  typedef K Target_kernel;
  typedef C Number_type_converter;

  template < typename T >
  const T& operator()(const T&t) const { return t; }
};

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_CONVERTER_H
