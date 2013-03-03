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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Static_filters/Side_of_oriented_sphere_3.h $
// $Id: Side_of_oriented_sphere_3.h 28924 2006-02-28 16:32:59Z glisse $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_STATIC_FILTERS_SIDE_OF_ORIENTED_SPHERE_3_H
#define CGAL_STATIC_FILTERS_SIDE_OF_ORIENTED_SPHERE_3_H

#include <CGAL/Profile_counter.h>
#include <CGAL/Static_filter_error.h>

CGAL_BEGIN_NAMESPACE

template < typename K_base >
class SF_Side_of_oriented_sphere_3
  : public K_base::Side_of_oriented_sphere_3
{
  typedef typename K_base::Point_3                      Point_3;
  typedef typename K_base::Side_of_oriented_sphere_3    Base;

public:

  Oriented_side
  operator()(const Point_3 &p, const Point_3 &q, const Point_3 &r,
             const Point_3 &s, const Point_3 &t) const
  {
      CGAL_PROFILER("In_sphere_3 calls");

      using std::fabs;

      double px, py, pz, qx, qy, qz, rx, ry, rz, sx, sy, sz, tx, ty, tz;

      if (fit_in_double(p.x(), px) && fit_in_double(p.y(), py) &&
          fit_in_double(p.z(), pz) &&
          fit_in_double(q.x(), qx) && fit_in_double(q.y(), qy) &&
          fit_in_double(q.z(), qz) &&
          fit_in_double(r.x(), rx) && fit_in_double(r.y(), ry) &&
          fit_in_double(r.z(), rz) &&
          fit_in_double(s.x(), sx) && fit_in_double(s.y(), sy) &&
          fit_in_double(s.z(), sz) &&
          fit_in_double(t.x(), tx) && fit_in_double(t.y(), ty) &&
          fit_in_double(t.z(), tz))
      {
          CGAL_PROFILER("In_sphere_3 semi-static attempts");

          double ptx = px - tx;
          double pty = py - ty;
          double ptz = pz - tz;
          double pt2 = CGAL_NTS square(ptx) + CGAL_NTS square(pty)
	             + CGAL_NTS square(ptz);
          double qtx = qx - tx;
          double qty = qy - ty;
          double qtz = qz - tz;
          double qt2 = CGAL_NTS square(qtx) + CGAL_NTS square(qty)
	             + CGAL_NTS square(qtz);
          double rtx = rx - tx;
          double rty = ry - ty;
          double rtz = rz - tz;
          double rt2 = CGAL_NTS square(rtx) + CGAL_NTS square(rty)
	             + CGAL_NTS square(rtz);
          double stx = sx - tx;
          double sty = sy - ty;
          double stz = sz - tz;
          double st2 = CGAL_NTS square(stx) + CGAL_NTS square(sty)
	             + CGAL_NTS square(stz);

          // Compute the semi-static bound.
          double maxx = fabs(ptx);
          if (maxx < fabs(qtx)) maxx = fabs(qtx);
          if (maxx < fabs(rtx)) maxx = fabs(rtx);
          if (maxx < fabs(stx)) maxx = fabs(stx);
          double maxy = fabs(pty);
          if (maxy < fabs(qty)) maxy = fabs(qty);
          if (maxy < fabs(rty)) maxy = fabs(rty);
          if (maxy < fabs(sty)) maxy = fabs(sty);
          double maxz = fabs(ptz);
          if (maxz < fabs(qtz)) maxz = fabs(qtz);
          if (maxz < fabs(rtz)) maxz = fabs(rtz);
          if (maxz < fabs(stz)) maxz = fabs(stz);

          // Sort maxx < maxy < maxz.
          if (maxx > maxz)
              std::swap(maxx, maxz);
          if (maxy > maxz)
              std::swap(maxy, maxz);
          else if (maxy < maxx)
              std::swap(maxx, maxy);

          double eps = 1.2466136531027298e-13 * maxx * maxy * maxz
                     * (maxz * maxz);

          double det = det4x4_by_formula(ptx,pty,ptz,pt2,
                                         rtx,rty,rtz,rt2,
                                         qtx,qty,qtz,qt2,
                                         stx,sty,stz,st2);

          // Protect against underflow in the computation of eps.
          if (maxx < 1e-58) /* sqrt^5(min_double/eps) */ {
            if (maxx == 0)
              return ON_ORIENTED_BOUNDARY;
          }
          // Protect against overflow in the computation of det.
          else if (maxz < 1e61) /* sqrt^5(max_double/4 [hadamard]) */ {
            if (det > eps)  return ON_POSITIVE_SIDE;
            if (det < -eps) return ON_NEGATIVE_SIDE;
          }

          CGAL_PROFILER("In_sphere_3 semi-static failures");
      }
      return Base::operator()(p, q, r, s, t);
  }

  // Computes the epsilon for In_sphere_3.
  static double compute_epsilon()
  {
    typedef CGAL::Static_filter_error F;
    F t1 = F(1,F::ulp()/2);         // First translation
    F sq = t1*t1+t1*t1+t1*t1; // squares
    F det = det4x4_by_formula(t1, t1, t1, sq,
                              t1, t1, t1, sq,
                              t1, t1, t1, sq,
                              t1, t1, t1, sq); // Full det
    double err = det.error();
    err += err * 3 * F::ulp(); // Correction due to "eps * maxx * ...".

    std::cerr << "*** epsilon for In_sphere_3 = " << err << std::endl;
    return err;
  }
};

CGAL_END_NAMESPACE

#endif // CGAL_STATIC_FILTERS_SIDE_OF_ORIENTED_SPHERE_3_H
