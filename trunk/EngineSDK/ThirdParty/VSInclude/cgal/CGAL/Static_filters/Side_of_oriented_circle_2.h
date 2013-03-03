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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Static_filters/Side_of_oriented_circle_2.h $
// $Id: Side_of_oriented_circle_2.h 28889 2006-02-28 13:19:00Z glisse $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_STATIC_FILTERS_SIDE_OF_ORIENTED_CIRCLE_2_H
#define CGAL_STATIC_FILTERS_SIDE_OF_ORIENTED_CIRCLE_2_H

#include <CGAL/Profile_counter.h>
#include <CGAL/Static_filter_error.h>
#include <cmath>

CGAL_BEGIN_NAMESPACE

template < typename K_base >
class SF_Side_of_oriented_circle_2
  : public K_base::Side_of_oriented_circle_2
{
  typedef typename K_base::Point_2                      Point_2;
  typedef typename K_base::Side_of_oriented_circle_2    Base;

public:

  Oriented_side operator()(const Point_2 &p, const Point_2 &q,
	                   const Point_2 &r, const Point_2 &t) const
  {
      CGAL_PROFILER("In_circle_2 calls");

      using std::fabs;

      double px, py, qx, qy, rx, ry, tx, ty;

      if (fit_in_double(p.x(), px) && fit_in_double(p.y(), py) &&
          fit_in_double(q.x(), qx) && fit_in_double(q.y(), qy) &&
          fit_in_double(r.x(), rx) && fit_in_double(r.y(), ry) &&
          fit_in_double(t.x(), tx) && fit_in_double(t.y(), ty))
      {
          CGAL_PROFILER("In_circle_2 semi-static attempts");

          double qpx = qx-px;
          double qpy = qy-py;
          double rpx = rx-px;
          double rpy = ry-py;
          double tpx = tx-px;
          double tpy = ty-py;

	  double tqx = tx-qx;
	  double tqy = ty-qy;
	  double rqx = rx-qx;
	  double rqy = ry-qy;

          double det = det2x2_by_formula(qpx*tpy - qpy*tpx, tpx*tqx + tpy*tqy,
                                         qpx*rpy - qpy*rpx, rpx*rqx + rpy*rqy);

          // We compute the semi-static bound.
          double maxx = fabs(qpx);
          if (maxx < fabs(rpx)) maxx = fabs(rpx);
          if (maxx < fabs(tpx)) maxx = fabs(tpx);
          if (maxx < fabs(tqx)) maxx = fabs(tqx);
          if (maxx < fabs(rqx)) maxx = fabs(rqx);
          double maxy = fabs(qpy);
          if (maxy < fabs(rpy)) maxy = fabs(rpy);
          if (maxy < fabs(tpy)) maxy = fabs(tpy);
          if (maxy < fabs(tqy)) maxy = fabs(tqy);
          if (maxy < fabs(rqy)) maxy = fabs(rqy);

          if (maxx > maxy)  std::swap(maxx, maxy);

          double eps = 8.8878565762001373e-15 * maxx * maxy * (maxy*maxy);

          // Protect against underflow in the computation of eps.
          if (maxx < 1e-73) {
            if (maxx == 0)
              return ON_ORIENTED_BOUNDARY;
          }
          else if (maxy < 1e76) /* sqrt(sqrt(max_double/16 [hadamard])) */ {
            if (det > eps)  return ON_POSITIVE_SIDE;
            if (det < -eps) return ON_NEGATIVE_SIDE;
          }

          CGAL_PROFILER("In_circle_2 semi-static failures");
      }

      return Base::operator()(p, q, r, t);
  }

  // Computes the epsilon for In_circle_2.
  static double compute_epsilon()
  {
    typedef CGAL::Static_filter_error F;
    F t1 = F(1, F::ulp()/2);         // First translation
    F a = t1*t1 - t1*t1;
    F b = t1*t1 + t1*t1;
    F det = det2x2_by_formula(a, b, a, b);
    double err = det.error();
    err += err * 3 * F::ulp(); // Correction due to "eps * maxx * maxy...".

    std::cerr << "*** epsilon for In_circle_2 = " << err << std::endl;
    return err;
  }
};

CGAL_END_NAMESPACE

#endif // CGAL_STATIC_FILTERS_SIDE_OF_ORIENTED_CIRCLE_2_H
