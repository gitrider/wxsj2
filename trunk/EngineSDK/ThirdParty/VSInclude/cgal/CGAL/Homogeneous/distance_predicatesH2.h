// Copyright (c) 1999  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Homogeneous_kernel/include/CGAL/Homogeneous/distance_predicatesH2.h $
// $Id: distance_predicatesH2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra
 

#ifndef CGAL_DISTANCE_PREDICATESH2_H
#define CGAL_DISTANCE_PREDICATESH2_H

#include <CGAL/determinant.h>

CGAL_BEGIN_NAMESPACE

template < class R>
CGAL_KERNEL_MEDIUM_INLINE
bool
has_larger_distance_to_point(const PointH2<R>& p,
                              const PointH2<R>& q,
                              const PointH2<R>& r)
{
  typedef typename R::RT RT;

  const RT phx = p.hx();
  const RT phy = p.hy();
  const RT phw = p.hw();
  const RT qhx = q.hx();
  const RT qhy = q.hy();
  const RT qhw = q.hw();
  const RT rhx = r.hx();
  const RT rhy = r.hy();
  const RT rhw = r.hw();
  const RT RT0 = RT(0);
  const RT RT2 = RT(2);

  RT dosd =   // difference of squared distances

  //            phx * phx   *   qhw * qhw * rhw * rhw
  //   -RT(2) * phx * qhx   *   phw * qhw * rhw * rhw
  //   +        qhx * qhx   *   phw * phw * rhw * rhw
  //
  //   +        phy * phy   *   qhw * qhw * rhw * rhw
  //   -RT(2) * phy * qhy   *   phw * qhw * rhw * rhw
  //   +        qhy * qhy   *   phw * phw * rhw * rhw
  //
  // - (        phx * phx   *   qhw * qhw * rhw * rhw
  //   -RT(2) * phx * rhx   *   phw * qhw * qhw * rhw
  //   +        rhx * rhx   *   phw * phw * qhw * qhw
  //
  //   +        phy * phy   *   qhw * qhw * rhw * rhw
  //   -RT(2) * phy * rhy   *   phw * qhw * qhw * rhw
  //   +        rhy * rhy   *   phw * phw * qhw * qhw

    rhw*rhw * (         phw * ( qhx*qhx + qhy*qhy )
                - RT2 * qhw * ( phx*qhx + phy*qhy )
              )
  - qhw*qhw * (         phw * ( rhx*rhx + rhy*rhy )
                - RT2 * rhw * ( phx*rhx + phy*rhy )
              );


  return ( dosd > RT0 );
}

template < class R>
CGAL_KERNEL_INLINE
Comparison_result
compare_signed_distance_to_line(const LineH2<R>&  l,
                                const PointH2<R>& p,
                                const PointH2<R>& q)
{
  typedef typename R::RT RT;

  const RT la = l.a();
  const RT lb = l.b();
  const RT phx= p.hx();
  const RT phy= p.hy();
  const RT phw= p.hw();
  const RT qhx= q.hx();
  const RT qhy= q.hy();
  const RT qhw= q.hw();
  const RT RT0 = RT(0);

  RT scaled_dist_p_minus_scaled_dist_q =
      la*( phx*qhw - qhx*phw )
    + lb*( phy*qhw - qhy*phw );



  if ( scaled_dist_p_minus_scaled_dist_q < RT0 )
  {
      return SMALLER;
  }
  else
  {
      return ( RT0 < scaled_dist_p_minus_scaled_dist_q ) ?
             LARGER : EQUAL;
  }
}


template < class R>
CGAL_KERNEL_INLINE
bool
has_larger_signed_distance_to_line(const LineH2<R>&  l,
                                    const PointH2<R>& p,
                                    const PointH2<R>& q)
{
  typedef typename R::RT RT;

  const RT la = l.a();
  const RT lb = l.b();
  const RT phx= p.hx();
  const RT phy= p.hy();
  const RT phw= p.hw();
  const RT qhx= q.hx();
  const RT qhy= q.hy();
  const RT qhw= q.hw();
  const RT RT0 = RT(0);

  RT scaled_dist_p_minus_scaled_dist_q =
      la*( phx*qhw - qhx*phw )
    + lb*( phy*qhw - qhy*phw );



  return ( scaled_dist_p_minus_scaled_dist_q > RT0 );
}


template < class R>
CGAL_KERNEL_INLINE
bool
has_smaller_signed_distance_to_line(const LineH2<R>&  l,
                                    const PointH2<R>& p,
                                    const PointH2<R>& q)
{
  typedef typename R::RT RT;

  const RT la = l.a();
  const RT lb = l.b();
  const RT phx= p.hx();
  const RT phy= p.hy();
  const RT phw= p.hw();
  const RT qhx= q.hx();
  const RT qhy= q.hy();
  const RT qhw= q.hw();
  const RT RT0 = RT(0);

  RT scaled_dist_p_minus_scaled_dist_q =
      la*( phx*qhw - qhx*phw )
    + lb*( phy*qhw - qhy*phw );

  return ( scaled_dist_p_minus_scaled_dist_q < RT0 );
}


template < class R>
CGAL_KERNEL_MEDIUM_INLINE
Comparison_result
compare_signed_distance_to_line(const PointH2<R>& p,
                                const PointH2<R>& q,
                                const PointH2<R>& r,
                                const PointH2<R>& s)
{
  typedef typename R::RT RT;

  const RT phx= p.hx();
  const RT phy= p.hy();
  const RT phw= p.hw();
  const RT qhx= q.hx();
  const RT qhy= q.hy();
  const RT qhw= q.hw();
  const RT rhx= r.hx();
  const RT rhy= r.hy();
  const RT rhw= r.hw();
  const RT shx= s.hx();
  const RT shy= s.hy();
  const RT shw= s.hw();
  const RT RT0  = RT(0);

  RT  scaled_dist_r_minus_scaled_dist_s =
         ( rhx*shw - shx*rhw ) * (phy*qhw - qhy*phw)
       - ( rhy*shw - shy*rhw ) * (phx*qhw - qhx*phw);


  if ( scaled_dist_r_minus_scaled_dist_s < RT0 )
  {
      return SMALLER;
  }
  else
  {
      return (scaled_dist_r_minus_scaled_dist_s > RT0 ) ?
             LARGER : EQUAL;
  }
}


template < class R>
CGAL_KERNEL_MEDIUM_INLINE
bool
has_smaller_signed_distance_to_line(const PointH2<R>& p,
                                    const PointH2<R>& q,
                                    const PointH2<R>& r,
                                    const PointH2<R>& s)
{
  typedef typename R::RT RT;

  const RT phx= p.hx();
  const RT phy= p.hy();
  const RT phw= p.hw();
  const RT qhx= q.hx();
  const RT qhy= q.hy();
  const RT qhw= q.hw();
  const RT rhx= r.hx();
  const RT rhy= r.hy();
  const RT rhw= r.hw();
  const RT shx= s.hx();
  const RT shy= s.hy();
  const RT shw= s.hw();
  const RT RT0  = RT(0);

  RT  scaled_dist_r_minus_scaled_dist_s =
         ( rhx*shw - shx*rhw ) * (phy*qhw - qhy*phw)
       - ( rhy*shw - shy*rhw ) * (phx*qhw - qhx*phw);

  return ( scaled_dist_r_minus_scaled_dist_s < RT0 );
}



template < class R>
CGAL_KERNEL_MEDIUM_INLINE
bool
has_larger_signed_distance_to_line(const PointH2<R>& p,
                                    const PointH2<R>& q,
                                    const PointH2<R>& r,
                                    const PointH2<R>& s)
{
  typedef typename R::RT RT;

  const RT phx= p.hx();
  const RT phy= p.hy();
  const RT phw= p.hw();
  const RT qhx= q.hx();
  const RT qhy= q.hy();
  const RT qhw= q.hw();
  const RT rhx= r.hx();
  const RT rhy= r.hy();
  const RT rhw= r.hw();
  const RT shx= s.hx();
  const RT shy= s.hy();
  const RT shw= s.hw();
  const RT RT0  = RT(0);

  RT  scaled_dist_r_minus_scaled_dist_s =
         ( rhx*shw - shx*rhw ) * (phy*qhw - qhy*phw)
       - ( rhy*shw - shy*rhw ) * (phx*qhw - qhx*phw);


  return ( scaled_dist_r_minus_scaled_dist_s > RT0 );
}

CGAL_END_NAMESPACE

#endif //CGAL_DISTANCE_PREDICATESH2_H
