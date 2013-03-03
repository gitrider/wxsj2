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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/predicates/kernel_ftC2.h $
// $Id: kernel_ftC2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann (Herve.Bronnimann@sophia.inria.fr)
//                 Sylvain Pion

#ifndef CGAL_PREDICATES_KERNEL_FTC2_H
#define CGAL_PREDICATES_KERNEL_FTC2_H

#include <CGAL/number_utils.h>
#include <CGAL/predicates/sign_of_determinant.h>
#include <CGAL/constructions/kernel_ftC2.h>

CGAL_BEGIN_NAMESPACE

template < class FT >
inline
typename Equal_to<FT>::result_type
parallelC2(const FT &l1a, const FT &l1b,
           const FT &l2a, const FT &l2b)
{
    return sign_of_determinant2x2(l1a, l1b, l2a, l2b) == ZERO;
}

template < class FT >
typename Equal_to<FT>::result_type
parallelC2(const FT &s1sx, const FT &s1sy,
           const FT &s1tx, const FT &s1ty,
           const FT &s2sx, const FT &s2sy,
           const FT &s2tx, const FT &s2ty)
{
    return sign_of_determinant2x2(s1tx - s1sx, s1ty - s1sy,
                                  s2tx - s2sx, s2ty - s2sy) == ZERO;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Equal_to<FT>::result_type
equal_lineC2(const FT &l1a, const FT &l1b, const FT &l1c,
             const FT &l2a, const FT &l2b, const FT &l2c)
{
    if (sign_of_determinant2x2(l1a, l1b, l2a, l2b) != ZERO)
        return false; // Not parallel.
    typename Sgn<FT>::result_type s1a = CGAL_NTS sign(l1a);
    if (s1a != ZERO)
        return s1a == CGAL_NTS sign(l2a)
	    && sign_of_determinant2x2(l1a, l1c, l2a, l2c) == ZERO;
    return CGAL_NTS sign(l1b) == CGAL_NTS sign(l2b)
	&& sign_of_determinant2x2(l1b, l1c, l2b, l2c) == ZERO;
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_xC2(const FT &px,
            const FT &la, const FT &lb, const FT &lc,
            const FT &ha, const FT &hb, const FT &hc)
{
  // The abscissa of the intersection point is num/den.
  FT num = det2x2_by_formula( lb, lc, hb, hc);
  FT den = det2x2_by_formula( la, lb, ha, hb);
  typename Sgn<FT>::result_type s = CGAL_NTS sign(den);
  CGAL_kernel_assertion( s != ZERO );
  return enum_cast<Comparison_result>( s * CGAL_NTS compare( px * den, num) );
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_xC2(const FT &la, const FT &lb, const FT &lc,
            const FT &h1a, const FT &h1b, const FT &h1c,
            const FT &h2a, const FT &h2b, const FT &h2c)
{
  /*
  FT num1 = det2x2_by_formula( lb, lc, h1b, h1c);
  FT den1 = det2x2_by_formula( la, lb, h1a, h1b);
  FT num2 = det2x2_by_formula( lb, lc, h2b, h2c);
  FT den2 = det2x2_by_formula( la, lb, h2a, h2b);
  Sign s = Sign (CGAL_NTS sign(den1) * CGAL_NTS sign(den2));
  CGAL_kernel_assertion( s != ZERO );
  return Comparison_result( s * sign_of_determinant2x2(num1, 
                                                       num2, den1, den2));
  */
  FT num1 = det2x2_by_formula( la, lc, h1a, h1c);
  FT num2 = det2x2_by_formula( la, lc, h2a, h2c);
  FT num  = det2x2_by_formula(h1a,h1c,h2a,h2c)*lb
            + det2x2_by_formula(num1,num2,h1b,h2b);
  FT den1 = det2x2_by_formula( la, lb, h1a, h1b);
  FT den2 = det2x2_by_formula( la, lb, h2a, h2b);
  return enum_cast<Comparison_result>( CGAL_NTS sign(lb) *
                                       CGAL_NTS sign(num) *
                                       CGAL_NTS sign(den1) *
                                       CGAL_NTS sign(den2));
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_xC2(const FT &l1a, const FT &l1b, const FT &l1c,
            const FT &h1a, const FT &h1b, const FT &h1c,
            const FT &l2a, const FT &l2b, const FT &l2c,
            const FT &h2a, const FT &h2b, const FT &h2c)
{
  FT num1 = det2x2_by_formula( l1b, l1c, h1b, h1c);
  FT den1 = det2x2_by_formula( l1a, l1b, h1a, h1b);
  FT num2 = det2x2_by_formula( l2b, l2c, h2b, h2c);
  FT den2 = det2x2_by_formula( l2a, l2b, h2a, h2b);
  typename Sgn<FT>::result_type s = enum_cast<Sign>(CGAL_NTS sign(den1) *
                                                    CGAL_NTS sign(den2));
  CGAL_kernel_assertion( s != ZERO );
  return enum_cast<Comparison_result>( s * sign_of_determinant2x2(num1, num2,
			                                          den1, den2));
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_y_at_xC2(const FT &px, const FT &py,
                 const FT &la, const FT &lb, const FT &lc)
{
  typename Sgn<FT>::result_type s = CGAL_NTS sign(lb);
  CGAL_kernel_assertion( s != ZERO );
  return enum_cast<Comparison_result>(s * CGAL_NTS sign(la*px + lb*py + lc));
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_y_at_xC2(const FT &px,
                 const FT &l1a, const FT &l1b, const FT &l1c,
                 const FT &l2a, const FT &l2b, const FT &l2c)
{
  typename Sgn<FT>::result_type s = enum_cast<Sign>(CGAL_NTS sign(l1b) *
                                                    CGAL_NTS sign(l2b));
  CGAL_kernel_assertion( s != ZERO );
  return enum_cast<Comparison_result>( s *
                               sign_of_determinant2x2<FT>(l2a*px+l2c, l2b,
                                                          l1a*px+l1c, l1b));
}

template < class FT >
CGAL_KERNEL_LARGE_INLINE
typename Compare<FT>::result_type
compare_y_at_xC2(const FT &l1a, const FT &l1b, const FT &l1c,
                 const FT &l2a, const FT &l2b, const FT &l2c,
                 const FT &ha,  const FT &hb,  const FT &hc)
{
  typename Sgn<FT>::result_type s = enum_cast<Sign>( CGAL_NTS sign(hb) *
                                    sign_of_determinant2x2(l1a, l1b, l2a, l2b));
  CGAL_kernel_assertion( s != ZERO );
  return enum_cast<Comparison_result>(s * sign_of_determinant3x3(l1a, l1b, l1c,
                                                                 l2a, l2b, l2c,
                                                                 ha,  hb,  hc));
}

template < class FT >
CGAL_KERNEL_LARGE_INLINE
typename Compare<FT>::result_type
compare_y_at_xC2(const FT &l1a, const FT &l1b, const FT &l1c,
                 const FT &l2a, const FT &l2b, const FT &l2c,
                 const FT &h1a, const FT &h1b, const FT &h1c,
                 const FT &h2a, const FT &h2b, const FT &h2c)
{
  // The abscissa of the intersection point is num/den.
  FT num = det2x2_by_formula( l1b, l1c, l2b, l2c);
  FT den = det2x2_by_formula( l1a, l1b, l2a, l2b);
  typename Sgn<FT>::result_type s = enum_cast<Sign>(CGAL_NTS sign(h1b) *
                                                    CGAL_NTS sign(h2b) *
                                                    CGAL_NTS sign(den));
  CGAL_kernel_assertion( s != ZERO );
  return enum_cast<Comparison_result>( s *
	                     sign_of_determinant2x2<FT>(h2a*num+h2c*den, h2b,
                                                        h1a*num+h1c*den, h1b));
}

template < class FT >
/*CGAL_NO_FILTER*/
CGAL_KERNEL_LARGE_INLINE
typename Compare<FT>::result_type
compare_y_at_xC2(const FT &px, const FT &py,
                 const FT &ssx, const FT &ssy,
                 const FT &stx, const FT &sty)
{
    // compares the y-coordinates of p and the vertical projection of p on s.
    // Precondition : p is in the x-range of s.

    CGAL_kernel_precondition(px >= min(ssx, stx) && px <= max(ssx, stx));

    if (ssx < stx)
	return enum_cast<Comparison_result>(orientationC2(px, py, ssx, ssy, stx, sty));
    else if (ssx > stx)
	return enum_cast<Comparison_result>(orientationC2(px, py, stx, sty, ssx, ssy));
    else {
	if (py < min(sty, ssy))
	    return SMALLER;
	if (py > max(sty, ssy))
	    return LARGER;
	return EQUAL;
    }
}

template < class FT >
CGAL_KERNEL_LARGE_INLINE
typename Compare<FT>::result_type
compare_y_at_x_segment_C2(const FT &px,
                          const FT &s1sx, const FT &s1sy,
                          const FT &s1tx, const FT &s1ty,
                          const FT &s2sx, const FT &s2sy,
                          const FT &s2tx, const FT &s2ty)
{
    // compares the y-coordinates of the vertical projections of p on s1 and s2
    // Precondition : p is in the x-range of s1 and s2.
    // - if one or two segments are vertical :
    //   - if the segments intersect, return EQUAL
    //   - if not, return the obvious SMALLER/LARGER.

    CGAL_kernel_precondition(px >= min(s1sx, s1tx) && px <= max(s1sx, s1tx));
    CGAL_kernel_precondition(px >= min(s2sx, s2tx) && px <= max(s2sx, s2tx));

    if (s1sx != s1tx && s2sx != s2tx) {
	FT s1stx = s1sx-s1tx;
	FT s2stx = s2sx-s2tx;

	return enum_cast<Comparison_result>(
	    CGAL_NTS compare(s1sx, s1tx) *
	    CGAL_NTS compare(s2sx, s2tx) *
	    CGAL_NTS compare(-(s1sx-px)*(s1sy-s1ty)*s2stx,
		             (s2sy-s1sy)*s2stx*s1stx
		            -(s2sx-px)*(s2sy-s2ty)*s1stx ));
    }
    else {
	if (s1sx == s1tx) { // s1 is vertical
	    typename Compare<FT>::result_type c1, c2;
	    c1 = compare_y_at_xC2(px, s1sy, s2sx, s2sy, s2tx, s2ty);
	    c2 = compare_y_at_xC2(px, s1ty, s2sx, s2sy, s2tx, s2ty);
	    if (c1 == c2)
		return c1;
	    return EQUAL;
	}
	// s2 is vertical
	typename Compare<FT>::result_type c3, c4;
	c3 = compare_y_at_xC2(px, s2sy, s1sx, s1sy, s1tx, s1ty);
	c4 = compare_y_at_xC2(px, s2ty, s1sx, s1sy, s1tx, s1ty);
	if (c3 == c4)
	    return opposite(c3);
	return EQUAL;
    }
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Equal_to<FT>::result_type
equal_directionC2(const FT &dx1, const FT &dy1,
                  const FT &dx2, const FT &dy2) 
{
  return CGAL_NTS sign(dx1) == CGAL_NTS sign(dx2)
      && CGAL_NTS sign(dy1) == CGAL_NTS sign(dy2)
      && sign_of_determinant2x2(dx1, dy1, dx2, dy2) == ZERO;
}

template < class FT >
/*CGAL_NO_FILTER*/
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_angle_with_x_axisC2(const FT &dx1, const FT &dy1,
                            const FT &dx2, const FT &dy2) 
{
  // angles are in [-pi,pi], and the angle between Ox and d1 is compared
  // with the angle between Ox and d2
  int quadrant_1 = (dx1 >= 0) ? (dy1 >= 0 ? 1 : 4)
                              : (dy1 >= 0 ? 2 : 3);
  int quadrant_2 = (dx2 >= 0) ? (dy2 >= 0 ? 1 : 4)
                              : (dy2 >= 0 ? 2 : 3);
  // We can't use CGAL_NTS compare(quadrant_1,quadrant_2) because in case
  // of tie, we need additional computation
  if (quadrant_1 > quadrant_2)
    return LARGER;
  else if (quadrant_1 < quadrant_2)
    return SMALLER;
  return enum_cast<Comparison_result>(-sign_of_determinant2x2(dx1,dy1,dx2,dy2));
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_slopesC2(const FT &l1a, const FT &l1b, const FT &l2a, const FT &l2b) 
{
   if (CGAL_NTS is_zero(l1a))  // l1 is horizontal
    return CGAL_NTS is_zero(l2b) ? SMALLER
	                : enum_cast<Comparison_result>(CGAL_NTS sign(l2a) *
                                                       CGAL_NTS sign(l2b));
   if (CGAL_NTS is_zero(l2a)) // l2 is horizontal
    return CGAL_NTS is_zero(l1b) ? LARGER
	                : enum_cast<Comparison_result>(-CGAL_NTS sign(l1a) *
                                                        CGAL_NTS sign(l1b));
   if (CGAL_NTS is_zero(l1b)) return CGAL_NTS is_zero(l2b) ? EQUAL : LARGER;
   if (CGAL_NTS is_zero(l2b)) return SMALLER;
   int l1_sign = - CGAL_NTS sign(l1a) * CGAL_NTS sign(l1b);
   int l2_sign = - CGAL_NTS sign(l2a) * CGAL_NTS sign(l2b);

   if (l1_sign < l2_sign) return SMALLER;
   if (l1_sign > l2_sign) return LARGER;

   if (l1_sign > 0)
     return CGAL_NTS compare ( CGAL_NTS abs(l1a * l2b),
			       CGAL_NTS abs(l2a * l1b) );

   return CGAL_NTS compare ( CGAL_NTS abs(l2a * l1b),
			     CGAL_NTS abs(l1a * l2b) );
}

template < class FT >
CGAL_KERNEL_MEDIUM_INLINE
typename Compare<FT>::result_type
compare_slopesC2(const FT &s1_src_x, const FT &s1_src_y, const FT &s1_tgt_x, 
                 const FT &s1_tgt_y, const FT &s2_src_x, const FT &s2_src_y, 
                 const FT &s2_tgt_x, const FT &s2_tgt_y) 
{
   typedef typename Compare<FT>::result_type  Cmp;
   typedef typename Sgn<FT>::result_type      Sg;
   Cmp cmp_y1 = CGAL_NTS compare(s1_src_y, s1_tgt_y);
   if (cmp_y1 == EQUAL) // horizontal
   {
      Cmp cmp_x2 = CGAL_NTS compare(s2_src_x, s2_tgt_x);

      if (cmp_x2 == EQUAL) return SMALLER;
      return enum_cast<Comparison_result>(- CGAL_NTS sign((s2_src_y - s2_tgt_y) *
                                                          (s2_src_x - s2_tgt_x)) );
   }

   Cmp cmp_y2 = CGAL_NTS compare(s2_src_y, s2_tgt_y);
   if (cmp_y2 == EQUAL)
   {
      Cmp cmp_x1 = CGAL_NTS compare(s1_src_x, s1_tgt_x);

      if (cmp_x1 == EQUAL) return LARGER;
      return enum_cast<Comparison_result>( CGAL_NTS sign((s1_src_y - s1_tgt_y) *
                                                         (s1_src_x - s1_tgt_x)) );
   }

   Cmp cmp_x1 = CGAL_NTS compare(s1_src_x, s1_tgt_x);
   Cmp cmp_x2 = CGAL_NTS compare(s2_src_x, s2_tgt_x);

   if (cmp_x1 == EQUAL) return cmp_x2 == EQUAL ? EQUAL : LARGER;

   if (cmp_x2 == EQUAL) return SMALLER;

   FT s1_xdiff = s1_src_x - s1_tgt_x;
   FT s1_ydiff = s1_src_y - s1_tgt_y;
   FT s2_xdiff = s2_src_x - s2_tgt_x;
   FT s2_ydiff = s2_src_y - s2_tgt_y;
   Sg s1_sign = enum_cast<Sign>(CGAL_NTS sign(s1_ydiff) * CGAL_NTS sign(s1_xdiff));
   Sg s2_sign = enum_cast<Sign>(CGAL_NTS sign(s2_ydiff) * CGAL_NTS sign(s2_xdiff));

   if (s1_sign < s2_sign) return SMALLER;
   if (s1_sign > s2_sign) return LARGER;

   if (s1_sign > 0)
     return CGAL_NTS compare( CGAL_NTS abs(s1_ydiff * s2_xdiff),
                              CGAL_NTS abs(s2_ydiff * s1_xdiff));

   return CGAL_NTS compare( CGAL_NTS abs(s2_ydiff * s1_xdiff),
                            CGAL_NTS abs(s1_ydiff * s2_xdiff));
}


template < class FT >
inline
typename Compare<FT>::result_type
compare_deltax_deltayC2(const FT &px, const FT &qx,
                        const FT &ry, const FT &sy)
{
  return CGAL_NTS compare(CGAL_NTS abs(px-qx), CGAL_NTS abs(ry-sy));
}

template < class FT >
/*CGAL_NO_FILTER*/
inline
typename Compare<FT>::result_type
compare_lexicographically_xyC2(const FT &px, const FT &py,
                               const FT &qx, const FT &qy)
{
  typename Compare<FT>::result_type c = CGAL_NTS compare(px,qx);
  return (c != EQUAL) ? c : CGAL_NTS compare(py,qy);
}

template < class FT >
inline
typename Same_uncertainty_nt<Orientation, FT>::type
orientationC2(const FT &px, const FT &py,
              const FT &qx, const FT &qy,
              const FT &rx, const FT &ry)
{
  return enum_cast<Orientation>(sign_of_determinant2x2(qx-px, qy-py,
                                                       rx-px, ry-py));
}

template < class FT >
inline
typename Same_uncertainty_nt<Orientation, FT>::type
orientationC2(const FT &ux, const FT &uy, const FT &vx, const FT &vy)
{
  return enum_cast<Orientation>(sign_of_determinant2x2(ux, uy, vx, vy));
}

template < class FT >
inline
typename Same_uncertainty_nt<Angle, FT>::type
angleC2(const FT &px, const FT &py,
        const FT &qx, const FT &qy,
        const FT &rx, const FT &ry)
{
  return enum_cast<Angle>(CGAL_NTS sign((px-qx)*(rx-qx)+(py-qy)*(ry-qy)));
}

template < class FT >
/*CGAL_NO_FILTER*/
CGAL_KERNEL_MEDIUM_INLINE
typename Equal_to<FT>::result_type
collinear_are_ordered_along_lineC2(const FT &px, const FT &py,
                                   const FT &qx, const FT &qy,
                                   const FT &rx, const FT &ry)
{
  if (px < qx) return !(rx < qx);
  if (qx < px) return !(qx < rx);
  if (py < qy) return !(ry < qy);
  if (qy < py) return !(qy < ry);
  return true; // p==q
}

template < class FT >
/*CGAL_NO_FILTER*/
CGAL_KERNEL_MEDIUM_INLINE
typename Equal_to<FT>::result_type
collinear_are_strictly_ordered_along_lineC2(const FT &px, const FT &py,
                                            const FT &qx, const FT &qy,
                                            const FT &rx, const FT &ry)
{
  if (px < qx) return (qx < rx);
  if (qx < px) return (rx < qx);
  if (py < qy) return (qy < ry);
  if (qy < py) return (ry < qy);
  return false;
}

template < class FT >
CGAL_KERNEL_LARGE_INLINE
typename Same_uncertainty_nt<Oriented_side, FT>::type
side_of_oriented_circleC2(const FT &px, const FT &py,
                          const FT &qx, const FT &qy,
                          const FT &rx, const FT &ry,
                          const FT &tx, const FT &ty)
{
  // Oriented_side(
  //  sign_of_determinant4x4(px, py, px*px + py*py, 1,
  //                         qx, qy, qx*qx + qy*qy, 1,
  //                         rx, ry, rx*rx + ry*ry, 1,
  //                         tx, ty, tx*tx + ty*ty, 1));
  // We first translate so that p is the new origin.
  FT qpx = qx-px;
  FT qpy = qy-py;
  FT rpx = rx-px;
  FT rpy = ry-py;
  FT tpx = tx-px;
  FT tpy = ty-py;
// The usual 3x3 formula can be simplified a little bit to a 2x2.
//         - sign_of_determinant3x3(qpx, qpy, square(qpx) + square(qpy),
//                                  rpx, rpy, square(rpx) + square(rpy),
//                                  tpx, tpy, square(tpx) + square(tpy)));
  return enum_cast<Oriented_side>(sign_of_determinant2x2<FT>(
                             qpx*tpy - qpy*tpx, tpx*(tx-qx) + tpy*(ty-qy),
                             qpx*rpy - qpy*rpx, rpx*(rx-qx) + rpy*(ry-qy)));
}

template < class FT >
CGAL_KERNEL_LARGE_INLINE
typename Same_uncertainty_nt<Bounded_side, FT>::type
side_of_bounded_circleC2(const FT &px, const FT &py,
                         const FT &qx, const FT &qy,
                         const FT &rx, const FT &ry,
                         const FT &tx, const FT &ty)
{
  return enum_cast<Bounded_side>( side_of_oriented_circleC2(px,py,qx,qy,rx,ry,tx,ty)
                                 * orientationC2(px,py,qx,qy,rx,ry) );
}

template < class FT >
CGAL_KERNEL_LARGE_INLINE
typename Same_uncertainty_nt<Bounded_side, FT>::type
side_of_bounded_circleC2(const FT &px, const FT &py,
                         const FT &qx, const FT &qy,
                         const FT &tx, const FT &ty)
{
  // Returns whether T lies inside or outside the circle which diameter is PQ.
  return enum_cast<Bounded_side>(
                      CGAL_NTS compare((tx-px)*(qx-tx), (ty-py)*(ty-qy)) );
}

template < class FT >
inline
typename Compare<FT>::result_type
cmp_dist_to_pointC2(const FT &px, const FT &py,
                    const FT &qx, const FT &qy,
                    const FT &rx, const FT &ry)
{
  return CGAL_NTS compare(squared_distanceC2(px,py,qx,qy),
                          squared_distanceC2(px,py,rx,ry));
}

template < class FT >
/*CGAL_NO_FILTER*/
inline
typename Equal_to<FT>::result_type
has_larger_dist_to_pointC2(const FT &px, const FT &py,
                           const FT &qx, const FT &qy,
                           const FT &rx, const FT &ry)
{
  return cmp_dist_to_pointC2(px,py,qx,qy,rx,ry) == LARGER;
}

template < class FT >
/*CGAL_NO_FILTER*/
inline
typename Equal_to<FT>::result_type
has_smaller_dist_to_pointC2(const FT &px, const FT &py,
                            const FT &qx, const FT &qy,
                            const FT &rx, const FT &ry)
{
  return cmp_dist_to_pointC2(px,py,qx,qy,rx,ry) == SMALLER;
}

template < class FT >
inline
typename Compare<FT>::result_type
cmp_signed_dist_to_directionC2(const FT &la, const FT &lb,
                               const FT &px, const FT &py,
                               const FT &qx, const FT &qy)
{
  return CGAL_NTS compare(scaled_distance_to_directionC2(la,lb,px,py),
                          scaled_distance_to_directionC2(la,lb,qx,qy));
}

template < class FT >
/*CGAL_NO_FILTER*/
inline
typename Equal_to<FT>::result_type
has_larger_signed_dist_to_directionC2(const FT &la, const FT &lb,
                                      const FT &px, const FT &py,
                                      const FT &qx, const FT &qy)
{
  return cmp_signed_dist_to_directionC2(la,lb,px,py,qx,qy) == LARGER;
}

template < class FT >
/*CGAL_NO_FILTER*/
inline
typename Equal_to<FT>::result_type
has_smaller_signed_dist_to_directionC2(const FT &la, const FT &lb,
                                       const FT &px, const FT &py,
                                       const FT &qx, const FT &qy)
{
  return cmp_signed_dist_to_directionC2(la,lb,px,py,qx,qy) == SMALLER;
}

template <class FT>
inline
typename Compare<FT>::result_type
cmp_signed_dist_to_lineC2(const FT &px, const FT &py,
                          const FT &qx, const FT &qy,
                          const FT &rx, const FT &ry,
                          const FT &sx, const FT &sy)
{
  return CGAL_NTS compare(scaled_distance_to_lineC2(px,py,qx,qy,rx,ry),
                          scaled_distance_to_lineC2(px,py,qx,qy,sx,sy));
}

template <class FT>
/*CGAL_NO_FILTER*/
inline
typename Equal_to<FT>::result_type
has_larger_signed_dist_to_lineC2(const FT &px, const FT &py,
                                 const FT &qx, const FT &qy,
                                 const FT &rx, const FT &ry,
                                 const FT &sx, const FT &sy)
{
  return cmp_signed_dist_to_lineC2(px,py,qx,qy,rx,ry,sx,sy) == LARGER;
}

template <class FT>
/*CGAL_NO_FILTER*/
inline
typename Equal_to<FT>::result_type
has_smaller_signed_dist_to_lineC2(const FT &px, const FT &py,
                                  const FT &qx, const FT &qy,
                                  const FT &rx, const FT &ry,
                                  const FT &sx, const FT &sy)
{
  return cmp_signed_dist_to_lineC2(px,py,qx,qy,rx,ry,sx,sy) == SMALLER;
}

template <class FT>
inline
typename Same_uncertainty_nt<Oriented_side, FT>::type
side_of_oriented_lineC2(const FT &a, const FT &b, const FT &c,
                        const FT &x, const FT &y)
{
  return enum_cast<Oriented_side>(CGAL_NTS sign(a*x+b*y+c));
}

CGAL_END_NAMESPACE

#ifdef CGAL_ARITHMETIC_FILTER_H
#include <CGAL/Arithmetic_filter/predicates/kernel_ftC2.h>
#endif

#endif  // CGAL_PREDICATES_KERNEL_FTC2_H
