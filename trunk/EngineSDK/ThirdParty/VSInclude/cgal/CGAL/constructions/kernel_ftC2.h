// Copyright (c) 2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/constructions/kernel_ftC2.h $
// $Id: kernel_ftC2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sven Schoenherr, Herve Bronnimann, Sylvain Pion

#ifndef CGAL_CONSTRUCTIONS_KERNEL_FTC2_H
#define CGAL_CONSTRUCTIONS_KERNEL_FTC2_H

#include <CGAL/determinant.h>

CGAL_BEGIN_NAMESPACE

template < class FT >
CGAL_KERNEL_INLINE
void
midpointC2( const FT &px, const FT &py,
            const FT &qx, const FT &qy,
            FT &x, FT &y )
{
  x = (px+qx) / 2;
  y = (py+qy) / 2;
}

template < class FT >
CGAL_KERNEL_LARGE_INLINE
void
circumcenter_translateC2(const FT &dqx, const FT &dqy,
                         const FT &drx, const FT &dry,
                               FT &dcx,       FT &dcy)
{
  // Given 3 points P, Q, R, this function takes as input:
  // qx-px, qy-py, rx-px, ry-py.  And returns cx-px, cy-py,
  // where (cx, cy) are the coordinates of the circumcenter C.

  // What we do is intersect the bisectors.
  FT r2 = CGAL_NTS square(drx) + CGAL_NTS square(dry);
  FT q2 = CGAL_NTS square(dqx) + CGAL_NTS square(dqy);
  FT den = 2 * det2x2_by_formula(dqx, dqy, drx, dry);

  // The 3 points aren't collinear.
  // Hopefully, this is already checked at the upper level.
  CGAL_kernel_assertion ( ! CGAL_NTS is_zero(den) );

  // One possible optimization here is to precompute 1/den, to avoid one
  // division.  However, we loose precision, and it's maybe not worth it (?).
  dcx =   det2x2_by_formula (dry, dqy, r2, q2) / den;
  dcy = - det2x2_by_formula (drx, dqx, r2, q2) / den;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
void
circumcenterC2( const FT &px, const FT &py,
                const FT &qx, const FT &qy,
                const FT &rx, const FT &ry,
                FT &x, FT &y )
{
  circumcenter_translateC2<FT>(qx-px, qy-py, rx-px, ry-py, x, y);
  x += px;
  y += py;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
void
centroidC2( const FT &px, const FT &py,
            const FT &qx, const FT &qy,
            const FT &rx, const FT &ry,
            FT &x, FT &y)
{
   x = (px + qx + rx) / 3;
   y = (py + qy + ry) / 3;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
void
centroidC2( const FT &px, const FT &py,
            const FT &qx, const FT &qy,
            const FT &rx, const FT &ry,
            const FT &sx, const FT &sy,
            FT &x, FT &y)
{
   x = (px + qx + rx + sx) / 4;
   y = (py + qy + ry + sy) / 4;
}

template < class FT >
inline
void
line_from_pointsC2(const FT &px, const FT &py,
                   const FT &qx, const FT &qy,
                   FT &a, FT &b, FT &c) 
{
  // The horizontal and vertical line get a special treatment
  // in order to make the intersection code robust for doubles 
  if(py == qy){
    a = 0 ;
    if(qx > px){
      b = 1;
      c = -py;
    } else if(qx == px){
      b = 0;
      c = 0;
    }else{
      b = -1;
      c = py;
    }
  } else if(qx == px){
    b = 0;
    if(qy > py){
      a = -1;
      c = px;
    } else if (qy == py){
      a = 0;
      c = 0;
    } else {
      a = 1;
      c = -px;
    }
  } else {
    a = py - qy;
    b = qx - px;
    c = -px*a - py*b;
  }
}

template < class FT >
inline
void
line_from_point_directionC2(const FT &px, const FT &py,
                            const FT &dx, const FT &dy,
                            FT &a, FT &b, FT &c) 
{
  a = - dy;
  b = dx;
  c = px*dy - py*dx;
}

template < class FT >
CGAL_KERNEL_INLINE
void
bisector_of_pointsC2(const FT &px, const FT &py,
		     const FT &qx, const FT &qy,
		     FT &a, FT &b, FT& c )
{
  a = 2 * (px - qx);
  b = 2 * (py - qy);
  c = CGAL_NTS square(qx) + CGAL_NTS square(qy) -
      CGAL_NTS square(px) - CGAL_NTS square(py);
}

template < class FT >
CGAL_KERNEL_INLINE
void
bisector_of_linesC2(const FT &pa, const FT &pb, const FT &pc,
		    const FT &qa, const FT &qb, const FT &qc,
		    FT &a, FT &b, FT &c)
{
  // We normalize the equations of the 2 lines, and we then add them.
  FT n1 = CGAL_NTS sqrt(CGAL_NTS square(pa) + CGAL_NTS square(pb));
  FT n2 = CGAL_NTS sqrt(CGAL_NTS square(qa) + CGAL_NTS square(qb));
  a = n2 * pa + n1 * qa;
  b = n2 * pb + n1 * qb;
  c = n2 * pc + n1 * qc;

  // Care must be taken for the case when this produces a degenerate line.
  if (a == 0 && b == 0) {
    a = n2 * pa - n1 * qa;
    b = n2 * pb - n1 * qb;
    c = n2 * pc - n1 * qc;
  }
}

template < class FT >
inline
FT
line_y_at_xC2(const FT &a, const FT &b, const FT &c, const FT &x)
{
  return (-a*x-c) / b;
}

template < class FT > 
inline
void
line_get_pointC2(const FT &a, const FT &b, const FT &c, int i,
                 FT &x, FT &y)
{
  if (CGAL_NTS is_zero(b))
    {
      x = (-b-c)/a + i * b;
      y = 1 - i * a;
    }
  else
    {
      x = 1 + i * b;
      y = -(a+c)/b - i * a;
    }
}

template < class FT > 
inline
void
perpendicular_through_pointC2(const FT &la, const FT &lb,
		              const FT &px, const FT &py,
			      FT &a, FT &b, FT &c)
{
  a = -lb;
  b = la;
  c = lb * px - la * py;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
void
line_project_pointC2(const FT &la, const FT &lb, const FT &lc,
		     const FT &px, const FT &py,
		     FT &x, FT &y)
{
#if 1 // FIXME
  // Original old version
  if (CGAL_NTS is_zero(la)) // horizontal line
  {
    x = px;
    y = -lc/lb;
  }
  else if (CGAL_NTS is_zero(lb)) // vertical line
  {
    x = -lc/la;
    y = py;
  }
  else
  {
    FT ab = la/lb, ba = lb/la, ca = lc/la;
    y = ( -px + ab*py - ca ) / ( ba + ab );
    x = -ba * y - ca;
  }
#else
  // New version, with more multiplications, but less divisions and tests.
  // Let's compare the results of the 2, benchmark them, as well as check
  // the precision with the intervals.
  FT a2 = CGAL_NTS square(la);
  FT b2 = CGAL_NTS square(lb);
  FT d = a2 + b2;
  x = (la * (lb * py - lc) - px * b2) / d;
  y = (lb * (lc - la * px) + py * a2) / d;
#endif
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
FT
squared_radiusC2(const FT &px, const FT &py,
                 const FT &qx, const FT &qy,
                 const FT &rx, const FT &ry,
                 FT &x, FT &y )
{
  circumcenter_translateC2(qx-px, qy-py, rx-px, ry-py, x, y);
  FT r2 = CGAL_NTS square(x) + CGAL_NTS square(y);
  x += px;
  y += py;
  return r2;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
FT
squared_radiusC2(const FT &px, const FT &py,
                 const FT &qx, const FT &qy,
                 const FT &rx, const FT &ry)
{
  FT x, y;
  circumcenter_translateC2<FT>(qx-px, qy-py, rx-px, ry-py, x, y);
  return CGAL_NTS square(x) + CGAL_NTS square(y);
}

template < class FT >
inline
FT
squared_distanceC2( const FT &px, const FT &py,
                    const FT &qx, const FT &qy)
{
  return CGAL_NTS square(px-qx) + CGAL_NTS square(py-qy);
}

template < class FT >
inline
FT
squared_radiusC2(const FT &px, const FT &py,
                 const FT &qx, const FT &qy)
{
  return squared_distanceC2(px, py,qx, qy) / 4;
}

template < class FT >
CGAL_KERNEL_INLINE
FT
scaled_distance_to_lineC2( const FT &la, const FT &lb, const FT &lc,
                           const FT &px, const FT &py)
{
  // for comparisons, use distance_to_directionsC2 instead
  // since lc is irrelevant
  return la*px + lb*py + lc;
}

template < class FT >
CGAL_KERNEL_INLINE
FT
scaled_distance_to_directionC2( const FT &la, const FT &lb,
                                const FT &px, const FT &py)
{
  // scalar product with direction
  return la*px + lb*py;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
FT
scaled_distance_to_lineC2( const FT &px, const FT &py,
                           const FT &qx, const FT &qy,
                           const FT &rx, const FT &ry)
{
  return det2x2_by_formula<FT>(px-rx, py-ry, qx-rx, qy-ry);
}

CGAL_END_NAMESPACE

#endif // CGAL_CONSTRUCTIONS_KERNEL_FTC2_H
