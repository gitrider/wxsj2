// Copyright (c) 2003,2004,2005  INRIA Sophia-Antipolis (France) and
// Notre Dame University (U.S.A.).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Segment_Delaunay_graph_2/include/CGAL/Segment_Delaunay_graph_2/Predicates_rtH2.h $
// $Id: Predicates_rtH2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>

//=========================================================================
// THIS FILE IS NOT CURRENTLY USED IN THE SEGMENT DELAUNAY GRAPH PACKAGE
//=========================================================================


#ifndef CGAL_SEGMENT_DELAUNAY_GRAPH_2_PREDICATES_RTH2_H
#define CGAL_SEGMENT_DELAUNAY_GRAPH_2_PREDICATES_RTH2_H


#include <CGAL/Segment_Delaunay_graph_2/Predicates_H2.h>
#include <CGAL/predicates/check_filter.h>
#include <CGAL/Segment_Delaunay_graph_2/Kernel_wrapper_2.h>


CGAL_BEGIN_NAMESPACE

template<class RT, class Method_tag>
Comparison_result
sdg_compare_distanceH2(const RT& qhx, const RT& qhy, const RT& qhw,
		       const RT& shx, const RT& shy, const RT& shw,
		       const RT& thx, const RT& thy, const RT& thw,
		       const RT& phx, const RT& phy, const RT& phw, 
		       Method_tag)
{
  // first check if (qx,qy) is inside, the boundary or at the exterior
  // of the band of the segment s

  //********************************************************************
  //********************************************************************

  must_be_filtered(qhx);

  FT dx = sx - tx;
  FT dy = sy - ty;


  FT d1x = qx - sx;
  FT d1y = qy - sy;
  FT d2x = qx - tx;
  FT d2y = qy - ty;

  if ( px == sx && py == sy ) {
    FT o = dx * d1x + dy * d1y;
    if ( o >= FT(0) ) {
      return LARGER;
    }
  }

  if ( px == tx && py == ty ) {
    FT o = dx * d2x + dy * d2y;
    if ( o <= FT(0) ) {
      return LARGER;
    }
  }


  FT d2_from_p = CGAL::square (qx-px) + CGAL::square(qy-py);

  FT dot1 = dx * d1x + dy * d1y;
  if ( dot1 >= FT(0) ) {
    // q is outside (or the boundary of) the band on the side of s.
    FT d2_from_s = CGAL::square(d1x) + CGAL::square(d1y);
    return CGAL::compare(d2_from_s, d2_from_p);
  }

  FT dot2 = dx * d2x + dy * d2y;
  if ( dot2 <= FT(0) ) {
    // q is outside (or the boundary of) the band on the side of t.
    FT d2_from_t = CGAL::square(d2x) + CGAL::square(d2y);
    return CGAL::compare(d2_from_t, d2_from_p);
  }

  // q is strictly in the interior of the band, so I have to compare
  // its distance from the supporting line.
  FT c = sx * ty - sy * tx;
  FT n = CGAL::square(dx) + CGAL::square(dy);
  FT d2_from_l = CGAL::square(qx * dy - qy * dx + c);
  return CGAL::compare(d2_from_l, d2_from_p * n);
}

template<class FT, class Method_tag>
Comparison_result
sdg_compare_distanceC2(const FT& qx, const FT& qy,
		       const FT& s1x, const FT& s1y,
		       const FT& t1x, const FT& t1y,
		       const FT& s2x, const FT& s2y,
		       const FT& t2x, const FT& t2y, Method_tag)
{
  // first check if (qx,qy) is inside, the boundary or at the exterior
  // of the band of the segments s1, s2

  must_be_filtered(qx);

  FT d1x = s1x - t1x;
  FT d1y = s1y - t1y;

  FT d2x = s2x - t2x;
  FT d2y = s2y - t2y;

  FT dqs1x = qx - s1x;
  FT dqs1y = qy - s1y;
  FT dqt1x = qx - t1x;
  FT dqt1y = qy - t1y;

  FT dqs2x = qx - s2x;
  FT dqs2y = qy - s2y;
  FT dqt2x = qx - t2x;
  FT dqt2y = qy - t2y;

  FT dot_s1 = d1x * dqs1x + d1y * dqs1y;
  int idx1; // 0 for s1, 1 for interior of 1, 2 for t1;

  if ( qx == s1x && qy == s1y ) {
    idx1 = 0;
  } else if ( qx == t1x && qy == t1y ) {
    idx1 = 2;
  } else if ( dot_s1 >= FT(0) ) {
    // q is outside (or the boundary of) the band of 1 on the side of s1.
    idx1 = 0;
  } else {
    FT dot_t1 = d1x * dqt1x + d1y * dqt1y;
    if ( dot_t1 <= FT(0) ) {
      // q is outside (or the boundary of) the band of 1 on the side of t1.
      idx1 = 2;
    } else {
      idx1 = 1;
    }
  }

  FT dot_s2 = d2x * dqs2x + d2y * dqs2y;
  int idx2; // 0 for s2, 1 for interior of 2, 2 for t2;

  if ( qx == s2x && qy == s2y ) {
    idx2 = 0;
  } else if ( qx == t2x && qy == t2y ) {
    idx2 = 2;
  } else if ( dot_s2 >= FT(0) ) {
    // q is outside (or the boundary of) the band of 2 on the side of s2.
    idx2 = 0;
  } else {
    FT dot_t2 = d2x * dqt2x + d2y * dqt2y;
    if ( dot_t2 <= FT(0) ) {
      // q is outside (or the boundary of) the band of 2 on the side of t2.
      idx2 = 2;
    } else {
      idx2 = 1;
    }
  }

  if ( idx1 == 0 ) {
    FT d2_from_s1 = CGAL::square(dqs1x) + CGAL::square(dqs1y);
    //    if ( qx == s1x && qy == s1y ) { d2_from_s1 = FT(0); }
    if ( idx2 == 0 ) {
      FT d2_from_s2 = CGAL::square(dqs2x) + CGAL::square(dqs2y);
      //      if ( qx == s2x && qy == s2y ) { d2_from_s2 = FT(0); }

      if ( s1x == s2x && s1y == s2y ) { return EQUAL; }

      return CGAL::compare(d2_from_s1, d2_from_s2);
    } else if ( idx2 == 2 ) {

      FT d2_from_t2 = CGAL::square(dqt2x) + CGAL::square(dqt2y);
      //      if ( qx == t2x && qy == t2y ) { d2_from_t2 = FT(0); }

      if ( s1x == t2x && s1y == t2y ) { return EQUAL; }

      return CGAL::compare(d2_from_s1, d2_from_t2);
    } else { // idx2 == 1
      FT c2 = s2x * t2y - s2y * t2x;
      FT n2 = CGAL::square(d2x) + CGAL::square(d2y);
      FT d2_from_l2 = CGAL::square(qx * d2y - qy * d2x + c2);

      return CGAL::compare(d2_from_s1 * n2, d2_from_l2);
    }
  } else if ( idx1 == 2 ) {
     FT d2_from_t1 = CGAL::square(dqt1x) + CGAL::square(dqt1y);
     //     if ( qx == t1x && qy == t1y ) { d2_from_t1 = FT(0); }

     if ( idx2 == 0 ) {
       FT d2_from_s2 = CGAL::square(dqs2x) + CGAL::square(dqs2y);
       //       if ( qx == s2x && qy == s2y ) { d2_from_s2 = FT(0); }

       if ( t1x == s2x && t1y == s2y ) { return EQUAL; }

       return CGAL::compare(d2_from_t1, d2_from_s2);
     } else if ( idx2 == 2 ) {
       FT d2_from_t2 = CGAL::square(dqt2x) + CGAL::square(dqt2y);
       //       if ( qx == t2x && qy == t2y ) { d2_from_t2 = FT(0); }

       if ( t1x == t2x && t1y == t2y ) { return EQUAL; }

       return CGAL::compare(d2_from_t1, d2_from_t2);
    } else { // idx2 == 1
      FT c2 = s2x * t2y - s2y * t2x;
      FT n2 = CGAL::square(d2x) + CGAL::square(d2y);
      FT d2_from_l2 = CGAL::square(qx * d2y - qy * d2x + c2);

      return CGAL::compare(d2_from_t1 * n2, d2_from_l2);
    }
  } else { // idx1 == 1
    FT c1 = s1x * t1y - s1y * t1x;
    FT n1 = CGAL::square(d1x) + CGAL::square(d1y);
    FT d2_from_l1 = CGAL::square(qx * d1y - qy * d1x + c1);
    if ( idx2 == 0 ) {
      FT d2_from_s2 = CGAL::square(dqs2x) + CGAL::square(dqs2y);
      //      if ( qx == s2x && qy == s2y ) { d2_from_s2 = FT(0); }

      return CGAL::compare(d2_from_l1, d2_from_s2 * n1);
    } else if ( idx2 == 2 ) {
      FT d2_from_t2 = CGAL::square(dqt2x) + CGAL::square(dqt2y);
      //      if ( qx == t2x && qy == t2y ) { d2_from_t2 = FT(0); }

      return CGAL::compare(d2_from_l1, d2_from_t2 * n1);
    } else { // idx2 == 1
      FT c2 = s2x * t2y - s2y * t2x;
      FT n2 = CGAL::square(d2x) + CGAL::square(d2y);
      FT d2_from_l2 = CGAL::square(qx * d2y - qy * d2x + c2);

      return CGAL::compare(d2_from_l1 * n2, d2_from_l2 * n1);
    }
  }
}



//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Sign
sdg_vertex_conflict_ftC2(const typename K::Site_2 t[],
			 unsigned int num_sites, Method_tag mtag)
{
  typedef typename K::FT   FT;
  char site_types[4];

  std::vector<FT> v;

  for (unsigned int i = 0; i < num_sites; i++) {
    if ( t[i].is_point() ) {
      v.push_back( t[i].point().x() );
      v.push_back( t[i].point().y() );
      site_types[i] = 'p';
    } else {
      v.push_back( t[i].source().x() );
      v.push_back( t[i].source().y() );
      v.push_back( t[i].target().x() );
      v.push_back( t[i].target().y() );
      site_types[i] = 's';
    }
  }

  return sdg_vertex_conflict_ftC2(v, site_types, num_sites, mtag);
}
	

template<class K, class Method_tag>
inline
Sign
sdg_vertex_conflict_ftC2(const typename K::Site_2& p,
			 const typename K::Site_2& q,
			 const typename K::Site_2& t,
			 Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, t};
  return
    sdg_vertex_conflict_ftC2<K,Method_tag>(site_vec, 3, mtag);
}


template<class K, class Method_tag>
inline
Sign
sdg_vertex_conflict_ftC2(const typename K::Site_2& p,
			 const typename K::Site_2& q,
			 const typename K::Site_2& r,
			 const typename K::Site_2& t,
			 Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, t};
  return
    sdg_vertex_conflict_ftC2<K,Method_tag>(site_vec, 4, mtag);
}


template<class FT, class Method_tag>
inline
Sign
sdg_vertex_conflict_ftC2(const std::vector<FT>& v,
			 char site_types[], unsigned int num_sites,
			 Method_tag)
{
  CGAL_precondition( num_sites == 3 || num_sites == 4 );

  must_be_filtered(FT());
  
  typedef Simple_cartesian<FT>                 Rep;
  typedef CGAL::Segment_Voronoi_diagram_kernel_wrapper_2<Rep>  Kernel;

  typedef typename Kernel::Point_2             Point_2;
  typedef typename Kernel::Segment_2           Segment_2;
  typedef typename Kernel::Site_2              Site_2;
  typedef Sdg_incircle_2<Kernel,Method_tag>    Incircle;


  Site_2* t = new Site_2[4];

  for (unsigned int i = 0, k = 0; i < num_sites; i++) {
    if ( site_types[i] == 'p' ) {
      Point_2 p(v[k], v[k+1]);
      t[i].set_point( p );
    } else if ( site_types[i] == 's' ) {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      Segment_2 s(p1, p2);
      t[i].set_segment( s );
    } else {
      CGAL_assertion( site_types[i] == 'p' ||
		      site_types[i] == 's' );
    }
    k += ( (site_types[i] == 'p') ? 2 : 4 );
  }

  Sign res(ZERO);

  if ( num_sites == 3 ) {
    res = Incircle()(t[0], t[1], t[2]);
  } else {
    res = Incircle()(t[0], t[1], t[2], t[3]);
  }

  delete[] t;
  return res;
}


//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
sdg_finite_edge_conflict_ftC2(const typename K::Site_2 t[],
			      Sign sgn, unsigned int num_sites,
			      Method_tag mtag)
{
  typedef typename K::FT   FT;
  char site_types[5];

  std::vector<FT> v;

  for (unsigned int i = 0; i < num_sites; i++) {
    if ( t[i].is_point() ) {
      v.push_back( t[i].point().x() );
      v.push_back( t[i].point().y() );
      site_types[i] = 'p';
    } else {
      v.push_back( t[i].source().x() );
      v.push_back( t[i].source().y() );
      v.push_back( t[i].target().x() );
      v.push_back( t[i].target().y() );
      site_types[i] = 's';
    }
  }

  return sdg_finite_edge_conflict_ftC2(v, sgn, site_types,
				       num_sites, mtag);
}
	

template<class K, class Method_tag>
inline
bool
sdg_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, t};
  return
    sdg_finite_edge_conflict_ftC2<K,Method_tag>(site_vec, sgn, 3, mtag);
}


template<class K, class Method_tag>
inline
bool
sdg_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& r,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, t};
  return
    sdg_finite_edge_conflict_ftC2<K,Method_tag>(site_vec, sgn, 4, mtag);
}





template<class K, class Method_tag>
inline
bool
sdg_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& r,
			      const typename K::Site_2& s,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, s, t};
  return
    sdg_finite_edge_conflict_ftC2<K,Method_tag>(site_vec, sgn, 5, mtag);
}

template<class FT, class Method_tag>
inline
bool
sdg_finite_edge_conflict_ftC2(const std::vector<FT>& v, Sign sgn,
			      char site_types[], unsigned int num_sites,
			      Method_tag)
{
  CGAL_precondition( num_sites >= 3 || num_sites <= 5 );

  must_be_filtered(FT());
  
  typedef Simple_cartesian<FT>                           Rep;
  typedef Segment_Voronoi_diagram_kernel_wrapper_2<Rep>  Kernel;

  typedef typename Kernel::Point_2             Point_2;
  typedef typename Kernel::Segment_2           Segment_2;
  typedef typename Kernel::Site_2              Site_2;
  typedef Sdg_finite_edge_interior_2<Kernel,Method_tag>  Edge_interior_test;


  Site_2* t = new Site_2[5];

  for (unsigned int i = 0, k = 0; i < num_sites; i++) {
    if ( site_types[i] == 'p' ) {
      Point_2 p(v[k], v[k+1]);
      t[i].set_point( p );
    } else if ( site_types[i] == 's' ) {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      Segment_2 s(p1, p2);
      t[i].set_segment( s );
    } else {
      CGAL_assertion( site_types[i] == 'p' ||
		      site_types[i] == 's' );
    }
    k += ( (site_types[i] == 'p') ? 2 : 4 );
  }

  bool res(false);

  if ( num_sites == 3 ) {
    res = Edge_interior_test()(t[0], t[1], t[2], sgn);
  } else if ( num_sites == 4 ) {
    res = Edge_interior_test()(t[0], t[1], t[2], t[3], sgn);
  } else {
    res = Edge_interior_test()(t[0], t[1], t[2], t[3], t[4], sgn);
  }

  delete[] t;
  return res;
}

//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
sdg_infinite_edge_conflict_ftC2(const typename K::Site_2 t[],
				Sign sgn, unsigned int num_sites,
				Method_tag mtag)
{
  typedef typename K::FT   FT;
  char site_types[4];

  std::vector<FT> v;

  for (unsigned int i = 0; i < num_sites; i++) {
    if ( t[i].is_point() ) {
      v.push_back( t[i].point().x() );
      v.push_back( t[i].point().y() );
      site_types[i] = 'p';
    } else {
      v.push_back( t[i].source().x() );
      v.push_back( t[i].source().y() );
      v.push_back( t[i].target().x() );
      v.push_back( t[i].target().y() );
      site_types[i] = 's';
    }
  }

  return sdg_infinite_edge_conflict_ftC2(v, sgn, site_types,
					 num_sites, mtag);
}
	

template<class K, class Method_tag>
inline
bool
sdg_infinite_edge_conflict_ftC2(const typename K::Site_2& q,
				const typename K::Site_2& r,
				const typename K::Site_2& s,
				const typename K::Site_2& t,
				Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {q, r, s, t};
  return
    sdg_infinite_edge_conflict_ftC2<K,Method_tag>(site_vec, sgn, 4, mtag);
}


template<class FT, class Method_tag>
inline
bool
sdg_infinite_edge_conflict_ftC2(const std::vector<FT>& v, Sign sgn,
				char site_types[], unsigned int num_sites,
				Method_tag)
{
  CGAL_precondition( num_sites == 4 );

  must_be_filtered(FT());
  
  typedef Simple_cartesian<FT>                           Rep;
  typedef Segment_Voronoi_diagram_kernel_wrapper_2<Rep>  Kernel;

  typedef typename Kernel::Point_2                       Point_2;
  typedef typename Kernel::Segment_2                     Segment_2;
  typedef typename Kernel::Site_2                        Site_2;
  typedef
    Sdg_infinite_edge_interior_2<Kernel,Method_tag>  Edge_interior_test;


  Site_2* t = new Site_2[num_sites];

  for (unsigned int i = 0, k = 0; i < num_sites; i++) {
    if ( site_types[i] == 'p' ) {
      Point_2 p(v[k], v[k+1]);
      t[i].set_point( p );
    } else if ( site_types[i] == 's' ) {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      Segment_2 s(p1, p2);
      t[i].set_segment( s );
    } else {
      CGAL_assertion( site_types[i] == 'p' ||
		      site_types[i] == 's' );
    }
    k += ( (site_types[i] == 'p') ? 2 : 4 );
  }


  bool res = Edge_interior_test()(t[0], t[1], t[2], t[3], sgn);

  delete[] t;
  return res;
}

//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
sdg_is_degenerate_edge_ftC2(const typename K::Site_2 t[],
			    unsigned int num_sites, Method_tag mtag)
{
  typedef typename K::FT   FT;
  char site_types[4];

  std::vector<FT> v;

  for (unsigned int i = 0; i < num_sites; i++) {
    if ( t[i].is_point() ) {
      v.push_back( t[i].point().x() );
      v.push_back( t[i].point().y() );
      site_types[i] = 'p';
    } else {
      v.push_back( t[i].source().x() );
      v.push_back( t[i].source().y() );
      v.push_back( t[i].target().x() );
      v.push_back( t[i].target().y() );
      site_types[i] = 's';
    }
  }

  return sdg_is_degenerate_edge_ftC2(v, site_types,
				     num_sites, mtag);
}
	

template<class K, class Method_tag>
inline
bool
sdg_is_degenerate_edge_ftC2(const typename K::Site_2& p,
			    const typename K::Site_2& q,
			    const typename K::Site_2& r,
			    const typename K::Site_2& t,
			    Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, t};
  return sdg_is_degenerate_edge_ftC2<K,Method_tag>(site_vec, 4, mtag);
}


template<class FT, class Method_tag>
inline
bool
sdg_is_degenerate_edge_ftC2(const std::vector<FT>& v,
			    char site_types[], unsigned int num_sites,
			    Method_tag)
{
  CGAL_precondition( num_sites == 4 );

  must_be_filtered(FT());
  
  typedef Simple_cartesian<FT>                           Rep;
  typedef Segment_Voronoi_diagram_kernel_wrapper_2<Rep>  Kernel;

  typedef typename Kernel::Point_2             Point_2;
  typedef typename Kernel::Segment_2           Segment_2;
  typedef typename Kernel::Site_2              Site_2;
  typedef
    Sdg_is_degenerate_edge_test_2<Kernel,Method_tag> Is_degenerate_edge;


  Site_2* t = new Site_2[4];

  for (unsigned int i = 0, k = 0; i < num_sites; i++) {
    if ( site_types[i] == 'p' ) {
      Point_2 p(v[k], v[k+1]);
      t[i].set_point( p );
    } else if ( site_types[i] == 's' ) {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      Segment_2 s(p1, p2);
      t[i].set_segment( s );
    } else {
      CGAL_assertion( site_types[i] == 'p' ||
		      site_types[i] == 's' );
    }
    k += ( (site_types[i] == 'p') ? 2 : 4 );
  }

  bool res = Is_degenerate_edge()(t[0], t[1], t[2], t[3]);

  delete[] t;
  return res;
}

//--------------------------------------------------------------------------

CGAL_END_NAMESPACE


#ifdef CGAL_ARITHMETIC_FILTER_H
#ifndef CGAL_ARITHMETIC_FILTER_SDG_PREDICATES_RTH2_H
//#include <CGAL/Arithmetic_filter/predicates/sdg_predicates_rtH2.h>
#endif // CGAL_ARITHMETIC_FILTER_SDG_PREDICATES_RTH2_H
#endif

#endif // CGAL_SEGMENT_DELAUNAY_GRAPH_2_PREDICATES_RTH2_H

