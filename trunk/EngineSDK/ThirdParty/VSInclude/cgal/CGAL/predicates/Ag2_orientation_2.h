// Copyright (c) 2003,2004  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Apollonius_graph_2/include/CGAL/predicates/Ag2_orientation_2.h $
// $Id: Ag2_orientation_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>



#ifndef CGAL_AG2_ORIENTATION_2_H
#define CGAL_AG2_ORIENTATION_2_H

#include <CGAL/enum.h>
#include <CGAL/Number_type_traits.h>
#include <CGAL/predicates/Apollonius_graph_predicate_constructions_C2.h>

//--------------------------------------------------------------------

CGAL_BEGIN_NAMESPACE

template<class K, class MTag>
class Ag2_orientation_2
{
public:
  typedef K                    Kernel;
  typedef MTag                 Method_tag;
  typedef typename K::Site_2   Site_2;
  typedef typename K::Point_2  Point_2;

  typedef Orientation          result_type;
  typedef Arity_tag<3>         Arity;
  typedef Site_2               argument_type;

private:
  typedef CGAL::Weighted_point_inverter<K>  Weighted_point_inverter;
  typedef CGAL::Inverted_weighted_point<K>  Inverted_weighted_point;
  typedef CGAL::Voronoi_circle<K>           Voronoi_circle;
  typedef CGAL::Bitangent_line<K>           Bitangent_line;
  typedef typename Bitangent_line::FT       FT;

private:
  Orientation vv_orientation(const Voronoi_circle& vc, const Point_2& sp1,
			     const Point_2& p1,	const Point_2& p2,
			     const Sqrt_field_tag&) const
  {
    FT a = vc.a1() + vc.a2() * CGAL::sqrt(vc.delta());
    FT b = vc.b1() + vc.b2() * CGAL::sqrt(vc.delta());
    FT det1 = a * (p2.y() - p1.y()) - b * (p2.x() - p1.x());
    FT c = vc.c1() + vc.c2() * CGAL::sqrt(vc.delta());
    FT det2 = det2x2_by_formula(p1.x() - sp1.x(), p1.y() - sp1.y(),
				p2.x() - sp1.x(), p2.y() - sp1.y());
    return CGAL::sign(det1 + FT(2) * c * det2);
  }

  Orientation vv_orientation(const Voronoi_circle vc, const Point_2& sp1,
			     const Point_2& p1, const Point_2& p2,
			     const Ring_tag&) const
  {
    FT dx = p2.x() - p1.x();
    FT dy = p2.y() - p1.y();
    FT det1 = det2x2_by_formula(p1.x() - sp1.x(), p1.y() - sp1.y(),
				p2.x() - sp1.x(), p2.y() - sp1.y());
    FT A = vc.a1() * dy - vc.b1() * dx + FT(2) * vc.c1() * det1;
    FT B = vc.a2() * dy - vc.b2() * dx + FT(2) * vc.c2() * det1;
    return sign_a_plus_b_x_sqrt_c(A, B, vc.delta());
  }

public:
  inline
  Orientation operator()(const Site_2& s1, const Site_2& s2,
			 const Site_2& s3) const
  {
    return Kernel().orientation_2_object()(s1.point(), s2.point(),
					   s3.point());
  }

  Orientation operator()(const Site_2& s1, const Site_2& s2,
			 const Site_2& s3, const Site_2& p1,
			 const Site_2& p2) const
  {
    // computes the operation of the Voronoi vertex of s1, s2, s3 and
    // the points p1 and p2
    Weighted_point_inverter inverter(s1);
    Inverted_weighted_point u2 = inverter(s2);
    Inverted_weighted_point u3 = inverter(s3);
    Bitangent_line blinv_23(u2, u3);
    Voronoi_circle vc(blinv_23);
    return
      vv_orientation(vc, s1.point(), p1.point(), p2.point(), Method_tag());
  }
};

//--------------------------------------------------------------------

CGAL_END_NAMESPACE

#endif // CGAL_AG2_ORIENTATION_2_H
