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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Static_filters/Coplanar_orientation_3.h $
// $Id: Coplanar_orientation_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_STATIC_FILTERS_COPLANAR_ORIENTATION_3_H
#define CGAL_STATIC_FILTERS_COPLANAR_ORIENTATION_3_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Filtered_exact.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Profile_counter.h>

CGAL_BEGIN_NAMESPACE

// XXX :
// Is this really useful to optimize this predicate ?
// I de-prioritize it for now.


// This one is easy : it's just 3 Orientation_2.

template < typename P3 >
struct Point_23_xy {
    const P3 &p;

    Point_23_xy(const P3& pp) : p(pp) {}

    const FT& x() const { return p.x(); }
    const FT& y() const { return p.y(); }
};

template < typename Kernel >
class SF_Coplanar_orientation_3
  : public Kernel::Coplanar_orientation_3
{
  typedef typename Kernel::Point_3                  Point_3;
  typedef typename Kernel::Coplanar_orientation_3   Base;

public:
  typedef Orientation result_type;

  Orientation operator()(const Point_3 &p, const Point_3 &q, const Point_3 &r) const
  {
      return opti_coplanar_orientationC3(
	    to_double(p.x()), to_double(p.y()), to_double(p.z()),
	    to_double(q.x()), to_double(q.y()), to_double(q.z()),
	    to_double(r.x()), to_double(r.y()), to_double(r.z()));
  }

  Orientation operator()(const Point_3 &p, const Point_3 &q,
                         const Point_3 &r, const Point_3 &s) const
  {
      return opti_coplanar_orientationC3(
	    to_double(p.x()), to_double(p.y()), to_double(p.z()),
	    to_double(q.x()), to_double(q.y()), to_double(q.z()),
	    to_double(r.x()), to_double(r.y()), to_double(r.z()),
	    to_double(s.x()), to_double(s.y()), to_double(s.z()));
  }

private:
  Orientation
  opti_coplanar_orientationC3(double px, double py, double pz,
                              double qx, double qy, double qz,
		              double rx, double ry, double rz) const
  {
      CGAL_PROFILER("Coplanar_orientation_3 #1 calls");

      std::pair<bool, Orientation> oxy_pqr = orient_2d(px,py,qx,qy,rx,ry);
      if (oxy_pqr != COLLINEAR)
          return oxy_pqr;

      CGAL_PROFILER("Coplanar_orientation_3 #1 step2");

      Orientation oyz_pqr = orient_2d(py,pz,qy,qz,ry,rz);
      if (oyz_pqr != COLLINEAR)
          return oyz_pqr;

      CGAL_PROFILER("Coplanar_orientation_3 #1 step3");

      return orient_2d(px,pz,qx,qz,rx,rz);

  }

  Orientation
  opti_coplanar_orientationC3(double px, double py, double pz,
                              double qx, double qy, double qz,
		              double rx, double ry, double rz,
		              double sx, double sy, double sz) const
  {
      CGAL_PROFILER("Coplanar_orientation_3 #2 calls");

      Orientation oxy_pqr = orient_2d(px,py,qx,qy,rx,ry);
      if (oxy_pqr != COLLINEAR)
          return Orientation( oxy_pqr *
		              orient_2d(px,py,qx,qy,sx,sy));

      CGAL_PROFILER("Coplanar_orientation_3 #2 step2");

      Orientation oyz_pqr = orient_2d(py,pz,qy,qz,ry,rz);
      if (oyz_pqr != COLLINEAR)
          return Orientation( oyz_pqr *
		              orient_2d(py,pz,qy,qz,sy,sz));

      CGAL_PROFILER("Coplanar_orientation_3 #2 step3");

      Orientation oxz_pqr = orient_2d(px,pz,qx,qz,rx,rz);
      CGAL_kernel_assertion(oxz_pqr != COLLINEAR);
      return Orientation( oxz_pqr * orient_2d(px,pz,qx,qz,sx,sz));
  }

  // FIXME : Some code duplicated from Orientation_2...
  std::pair<bool, Orientation>
  orient_2d(double px, double py, double qx, double qy, double rx, double ry) const
  {
    /*
    double px = p.x();
    double py = p.y();
    double qx = q.x();
    double qy = q.y();
    double rx = r.x();
    double ry = r.y();
    */

    CGAL_PROFILER("orient2d calls");

    double pqx = qx-px;
    double pqy = qy-py;
    double prx = rx-px;
    double pry = ry-py;

    double det = det2x2_by_formula(pqx, pqy,
                                   prx, pry);

    // Then semi-static filter.
    double maxx = fabs(px);
    if (maxx < fabs(qx)) maxx = fabs(qx);
    if (maxx < fabs(rx)) maxx = fabs(rx);
    double maxy = fabs(py);
    if (maxy < fabs(qy)) maxy = fabs(qy);
    if (maxy < fabs(ry)) maxy = fabs(ry);
    double eps = 3.55271e-15 * maxx * maxy;

    if (det > eps)  return std::make_pair(true, POSITIVE);
    if (det < -eps) return std::make_pair(true, NEGATIVE);

    CGAL_PROFILER("orient2d semi-static failures");

    return std::make_pair(false, ZERO);
  }

};

CGAL_END_NAMESPACE

#endif // CGAL_STATIC_FILTERS_COPLANAR_ORIENTATION_3_H
