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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Segment_Delaunay_graph_2/include/CGAL/Segment_Delaunay_graph_2/Predicates_ftC2.h $
// $Id: Predicates_ftC2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>




#ifndef CGAL_SEGMENT_DELAUNAY_GRAPH_2_PREDICATES_FTC2_H
#define CGAL_SEGMENT_DELAUNAY_GRAPH_2_PREDICATES_FTC2_H

#include <CGAL/determinant.h>

#include <CGAL/predicates/check_filter.h>

#include <CGAL/Segment_Delaunay_graph_2/Predicates_C2.h>
#include <CGAL/Segment_Delaunay_graph_2/Kernel_wrapper_2.h>


CGAL_BEGIN_NAMESPACE

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K>
void sdg_predicate_push_back_C2(const typename K::Site_2& t,
				typename K::FT v[], unsigned int& k,
				char site_types[], unsigned int& j,
				const Tag_true&)
{
  unsigned int step(0);

  if ( t.is_point() ) {
    site_types[j] = 'p';
    if ( t.is_input() ) {
      site_types[j+1] = 'e';
      v[k] = t.point().x();
      v[k+1] = t.point().y();
      step = 2;
    } else {
      site_types[j+1] = 'i';
      v[k]   = t.source_of_supporting_site(0).x();
      v[k+1] = t.source_of_supporting_site(0).y();
      v[k+2] = t.target_of_supporting_site(0).x();
      v[k+3] = t.target_of_supporting_site(0).y();
      v[k+4] = t.source_of_supporting_site(1).x();
      v[k+5] = t.source_of_supporting_site(1).y();
      v[k+6] = t.target_of_supporting_site(1).x();
      v[k+7] = t.target_of_supporting_site(1).y();
      step = 8;
    }
  } else {
    site_types[j] = 's';
    if ( t.is_input() ) {
      site_types[j+1] = 'e';
      v[k]   = t.source_of_supporting_site().x();
      v[k+1] = t.source_of_supporting_site().y();
      v[k+2] = t.target_of_supporting_site().x();
      v[k+3] = t.target_of_supporting_site().y();
      step = 4;
    } else {
      v[k]   = t.source_of_supporting_site().x();
      v[k+1] = t.source_of_supporting_site().y();
      v[k+2] = t.target_of_supporting_site().x();
      v[k+3] = t.target_of_supporting_site().y();

      typedef typename K::Point_2   Point_2;

      if ( t.is_input(0) ) {
	const Point_2& cs_p1 = t.source_of_crossing_site(1);
	const Point_2& cs_p2 = t.target_of_crossing_site(1);
	site_types[j+1] = '0';
	v[k+4] = cs_p1.x();
	v[k+5] = cs_p1.y();
	v[k+6] = cs_p2.x();
	v[k+7] = cs_p2.y();
	step = 8;
      } else if ( t.is_input(1) ) {
	const Point_2& cs_p1 = t.source_of_crossing_site(0);
	const Point_2& cs_p2 = t.target_of_crossing_site(0);
	site_types[j+1] = '1';
	v[k+4] = cs_p1.x();
	v[k+5] = cs_p1.y();
	v[k+6] = cs_p2.x();
	v[k+7] = cs_p2.y();
	step = 8;
      } else {
	const Point_2& cs_p1 = t.source_of_crossing_site(0);
	const Point_2& cs_p2 = t.target_of_crossing_site(0);
	const Point_2& cs2_p1 = t.source_of_crossing_site(1);
	const Point_2& cs2_p2 = t.target_of_crossing_site(1);
	site_types[j+1] = 'i';
	v[k+4] = cs_p1.x();
	v[k+5] = cs_p1.y();
	v[k+6] = cs_p2.x();
	v[k+7] = cs_p2.y();
	v[k+8]  = cs2_p1.x();
	v[k+9]  = cs2_p1.y();
	v[k+10] = cs2_p2.x();
	v[k+11] = cs2_p2.y();
	step = 12;
      }
    }
  }

  j += 2;
  k += step;
}

template<class K>
void sdg_predicate_push_back_C2(const typename K::Site_2& t,
				typename K::FT v[], unsigned int& k,
				char site_types[], unsigned int& j,
				const Tag_false&)
{
  unsigned int step(0);

  if ( t.is_point() ) {
    site_types[j] = 'p';
    site_types[j+1] = 'e';
    v[k]   = t.point().x();
    v[k+1] = t.point().y();
    step = 2;
  } else {
    site_types[j] = 's';
    site_types[j+1] = 'e';
    v[k]   = t.source().x();
    v[k+1] = t.source().y();
    v[k+2] = t.target().x();
    v[k+3] = t.target().y();
    step = 4;
  }

  j += 2;
  k += step;
}

//--------------------------------------------------------------------------

template<class K, class ITag> class Sdg_get_site_C2;

template<class K>
class Sdg_get_site_C2<K,Tag_true>
{
private:
  typedef typename K::Point_2             Point_2;
  typedef typename K::Segment_2           Segment_2;
  typedef typename K::Site_2              Site_2;
  typedef typename K::FT                  FT;

public:
  Site_2 operator()(const FT v[], unsigned int& k,
		    const char site_types[], unsigned int& j)
  {
    Site_2 t;

    unsigned int step(0);

    if ( site_types[j] == 'p' ) {
      if ( site_types[j+1] == 'e' ) {
	Point_2 p(v[k], v[k+1]);
	t = Site_2::construct_site_2(p);
	step = 2;
      } else {
	Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
	t = Site_2::construct_site_2(p1, p2, p3, p4);
	step = 8;
      }
    } else {
      if ( site_types[j+1] == 'e' ) {
	Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	t = Site_2::construct_site_2(p1, p2);
	step = 4;
      } else {
	if ( site_types[j+1] != 'i' ) {
	  Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	  Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
	  t = Site_2::construct_site_2(p1, p2, p3, p4,
				       (site_types[j+1] == '0'));
	  step = 8;
	} else {
	  Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	  Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
	  Point_2 p5(v[k+8], v[k+9]), p6(v[k+10], v[k+11]);
	  t = Site_2::construct_site_2(p1, p2, p3, p4, p5, p6);
	  step = 12;
	}
      }
    }

    j += 2;
    k += step;

    return t;
  }
};

template<class K>
class Sdg_get_site_C2<K,Tag_false>
{
private:
  typedef typename K::Point_2             Point_2;
  typedef typename K::Segment_2           Segment_2;
  typedef typename K::Site_2              Site_2;
  typedef typename K::FT                  FT;

public:
  Site_2 operator()(const FT v[], unsigned int& k,
		    const char site_types[], unsigned int& j)
  {
    Site_2 t;

    unsigned int step(0);

    if ( site_types[j] == 'p' ) {
      Point_2 p(v[k], v[k+1]);
      t = Site_2::construct_site_2(p);
      step = 2;
    } else {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      t = Site_2::construct_site_2(p1, p2);
      step = 4;
    }

    j += 2;
    k += step;

    return t;
  }
};

template<class K>
typename K::Site_2
sdg_get_site_C2(const typename K::FT v[], unsigned int& k,
		const char site_types[], unsigned int& j, const Tag_true&)
{
  typedef typename K::Point_2             Point_2;
  typedef typename K::Segment_2           Segment_2;
  typedef typename K::Site_2              Site_2;

  Site_2 t;

  unsigned int step(0);

  if ( site_types[j] == 'p' ) {
    if ( site_types[j+1] == 'e' ) {
      Point_2 p(v[k], v[k+1]);
      t = Site_2::construct_site_2(p);
      step = 2;
    } else {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
      t = Site_2::construct_site_2(p1, p2, p3, p4);
      step = 8;
    }
  } else {
    if ( site_types[j+1] == 'e' ) {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      t = Site_2::construct_site_2(p1, p2);
      step = 4;
    } else {
      if ( site_types[j+1] != 'i' ) {
	Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
	t = Site_2::construct_site_2(p1, p2, p3, p4, (site_types[j+1] == '0'));
	step = 8;
      } else {
	Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
	Point_2 p5(v[k+8], v[k+9]), p6(v[k+10], v[k+11]);
	t = Site_2::construct_site_2(p1, p2, p3, p4, p5, p6);
	step = 12;
      }
    }
  }

  j += 2;
  k += step;

  return t;
}

template<class K>
typename K::Site_2
sdg_get_site_C2(const typename K::FT v[], unsigned int& k,
		const char site_types[], unsigned int& j, const Tag_false&)
{
  typedef typename K::Point_2             Point_2;
  typedef typename K::Segment_2           Segment_2;
  typedef typename K::Site_2              Site_2;

  Site_2 t;

  unsigned int step(0);

  if ( site_types[j] == 'p' ) {
    Point_2 p(v[k], v[k+1]);
    t = Site_2::construct_site_2(p);
    step = 2;
  } else {
    Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
    t = Site_2::construct_site_2(p1, p2);
    step = 4;
  }

  j += 2;
  k += step;

  return t;
}

//--------------------------------------------------------------------------

template<typename Result_t, class Predicate, unsigned int Arity>
struct Sdg_predicate_caller;

template<typename Result_t, class Predicate>
struct Sdg_predicate_caller<Result_t, Predicate, 2>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1]);
    }
};

template<typename Result_t, class Predicate>
struct Sdg_predicate_caller<Result_t, Predicate, 3>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1], t[2]);
    }

  template<class S, typename Data>
  Result_t operator()(const S t[], Data data) const
    {
      return Predicate()(t[0], t[1], t[2], data);
    }
};

template<typename Result_t, class Predicate>
struct Sdg_predicate_caller<Result_t, Predicate, 4>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1], t[2], t[3]);
    }

  template<class S, typename Data>
  Result_t operator()(const S t[], Data data) const
    {
      return Predicate()(t[0], t[1], t[2], t[3], data);
    }
};

template<typename Result_t, class Predicate>
struct Sdg_predicate_caller<Result_t, Predicate, 5>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1], t[2], t[3], t[4]);
    }

  template<class S, typename Data>
  Result_t operator()(const S t[], Data data) const
    {
      return Predicate()(t[0], t[1], t[2], t[3], t[4], data);
    }
};


//--------------------------------------------------------------------------

#if 0
template<template<class Kernel> class Predicate_t,
	 typename Return_t, class FT, class ITag,
	 unsigned int Num_sites>
Return_t
sdg_predicate_ftC2(const FT v[], const char site_types[])
{
  typedef Simple_cartesian<FT>                 Rep;
  typedef
  CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

  typedef typename Kernel::Site_2                   Site_2;
  typedef Predicate_t<Kernel>                       Predicate;

  typedef Sdg_predicate_caller<Return_t, Predicate, Num_sites> Caller;

  must_be_filtered(FT());


  Site_2 t[Num_sites];

  for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
    t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
  }

  Return_t result = Caller()(t);

  return result;
}

template<template<class Kernel, class MTag> class Predicate_t,
	 typename Return_t, class FT, class Method_tag,
	 class ITag, unsigned int Num_sites>
Return_t
sdg_predicate_ftC2(const FT v[], const char site_types[])
{
  typedef Simple_cartesian<FT>                 Rep;
  typedef
  CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

  typedef typename Kernel::Site_2                   Site_2;
  typedef Predicate_t<Kernel,Method_tag>            Predicate;

  typedef Sdg_predicate_caller<Return_t, Predicate, Num_sites> Caller;

  must_be_filtered(FT());


  Site_2 t[Num_sites];

  for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
    t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
  }

  Return_t result = Caller()(t);

  return result;
}

template<template<class Kernel, class MTag> class Predicate_t,
	 typename Return_t, class FT, class Method_tag, 
	 class ITag, typename Data, unsigned int Num_sites>
Return_t
sdg_predicate_ftC2(const FT v[], const char site_types[], Data data)
{
  typedef Simple_cartesian<FT>                 Rep;
  typedef
  CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

  typedef typename Kernel::Site_2                   Site_2;
  typedef Predicate_t<Kernel,Method_tag>            Predicate;

  typedef Sdg_predicate_caller<Return_t, Predicate, Num_sites> Caller;

  must_be_filtered(FT());


  Site_2 t[Num_sites];

  for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
    t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
  }

  Return_t result = Caller()(t, data);

  return result;
}
#endif

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


template<class FT, class ITag>
class Sdg_are_same_points_ftC2
{
 public:
  inline
  bool operator()(const FT v[], const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_are_same_points_C2<Kernel>            Predicate;

    must_be_filtered(FT());

    Site_2 t[2];

    for (unsigned int i = 0, k = 0, j = 0; i < 2; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Predicate()(t[0], t[1]);
  }
};

template<class K>
inline
bool sdg_are_same_points_C2(const typename K::Site_2& p,
			    const typename K::Site_2& q)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;

  typedef Sdg_are_same_points_ftC2<FT,ITag>   Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q};

  ITag itag;

  FT v[24];
  char site_types[4];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 2; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 24; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class FT, class ITag>
class Sdg_orientation_ftC2
{
 public:
  inline
  Orientation operator()(const FT v[], const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_orientation_C2<Kernel>                Predicate;

    must_be_filtered(FT());

    Site_2 t[3];

    for (unsigned int i = 0, k = 0, j = 0; i < 3; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Predicate()(t[0], t[1], t[2]);
  }
};


template<class K>
inline
Orientation sdg_orientation_C2(const typename K::Site_2& p,
			       const typename K::Site_2& q,
			       const typename K::Site_2& r)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;

  typedef Sdg_orientation_ftC2<FT,ITag>   Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q, r};

  ITag itag;

  FT v[36];
  char site_types[6];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 3; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 36; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


template<class FT, class MTag, class ITag>
class Sdg_oriented_side_of_bisector_ftC2
{
 public:
  inline
  Oriented_side operator()(const FT v[], const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_oriented_side_of_bisector_C2<Kernel,MTag>  Predicate;

    must_be_filtered(FT());

    Site_2 t[3];

    for (unsigned int i = 0, k = 0, j = 0; i < 3; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Predicate()(t[0], t[1], t[2]);
  }
};


template<class K, class Method_tag>
inline
Oriented_side
sdg_oriented_side_of_bisector_ftC2(const typename K::Site_2& p,
				   const typename K::Site_2& q,
				   const typename K::Site_2& t,
				   const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_oriented_side_of_bisector_ftC2<FT,MTag,ITag>  Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q, t};

  ITag itag;

  FT v[36];
  char site_types[6];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 3; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 36; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


template<class FT, class MTag, class ITag, int Num_sites>
class Sdg_vertex_conflict_ftC2
{
 public:
  inline
  Sign
  operator()(const FT v[], const char site_types[]) const
    //	     const MTag& mtag, const ITag& itag)
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_incircle_2<Kernel,MTag>               Predicate;

    typedef Sdg_predicate_caller<Sign, Predicate, Num_sites> Caller;


    must_be_filtered(FT());

    Site_2 t[Num_sites];

    for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Caller()(t);
  }
};


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Sign
sdg_vertex_conflict_ftC2(const typename K::Site_2& p,
			 const typename K::Site_2& q,
			 const typename K::Site_2& t,
			 const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_vertex_conflict_ftC2<FT,MTag,ITag,3>   Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q, t};

  ITag itag;

  FT v[36];
  char site_types[6];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 3; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 36; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Sign
sdg_vertex_conflict_ftC2(const typename K::Site_2& p,
			 const typename K::Site_2& q,
			 const typename K::Site_2& r,
			 const typename K::Site_2& t,
			 const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typename K::Site_2 tt[] = {p, q, r, t};

  ITag itag;

  FT v[48];
  char site_types[8];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 4; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 48; ii++) {
    v[ii] = FT(0);
  }

  typedef Sdg_vertex_conflict_ftC2<FT,MTag,ITag,4> VC;
  return VC()(v, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


template<class FT, class MTag, class ITag, int Num_sites>
class Sdg_finite_edge_conflict_ftC2
{
 public:
  inline
  bool operator()(const FT v[], Sign sgn, const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_finite_edge_interior_2<Kernel,MTag>   Predicate;

    typedef Sdg_predicate_caller<bool, Predicate, Num_sites> Caller;


    must_be_filtered(FT());

    Site_2 t[Num_sites];

    for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Caller()(t, sgn);
  }
};


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
sdg_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_finite_edge_conflict_ftC2<FT,MTag,ITag,3>  Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q, t};

  ITag itag;

  FT v[36];
  char site_types[6];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 3; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 36; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, sgn, site_types);
}

//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
sdg_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& r,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_finite_edge_conflict_ftC2<FT,MTag,ITag,4>  Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q, r, t};

  ITag itag;

  FT v[48];
  char site_types[8];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 4; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 48; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, sgn, site_types);
}


//--------------------------------------------------------------------------

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
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_finite_edge_conflict_ftC2<FT,MTag,ITag,5>  Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q, r, s, t};

  ITag itag;

  FT v[60];
  char site_types[10];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 5; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 60; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, sgn, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


template<class FT, class MTag, class ITag>
class Sdg_infinite_edge_conflict_ftC2
{
 public:

  inline
  bool operator()(const FT v[], Sign sgn, const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                    Site_2;
    typedef Sdg_infinite_edge_interior_2<Kernel,MTag>  Predicate;

    must_be_filtered(FT());

    Site_2 t[4];

    for (unsigned int i = 0, k = 0, j = 0; i < 4; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Predicate()(t[0], t[1], t[2], t[3], sgn);
  }
};


template<class K, class Method_tag>
inline
bool
sdg_infinite_edge_conflict_ftC2(const typename K::Site_2& q,
				const typename K::Site_2& r,
				const typename K::Site_2& s,
				const typename K::Site_2& t,
				Sign sgn, const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_infinite_edge_conflict_ftC2<FT,MTag,ITag>  Predicate_ftC2;

  typename K::Site_2 tt[] = {q, r, s, t};

  ITag itag;

  FT v[48];
  char site_types[8];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 4; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 48; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, sgn, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class FT, class MTag, class ITag>
class Sdg_is_degenerate_edge_ftC2
{
 public:
  inline
  bool operator()(const FT v[], const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_is_degenerate_edge_C2<Kernel,MTag>    Predicate;

    must_be_filtered(FT());

    Site_2 t[4];

    for (unsigned int i = 0, k = 0, j = 0; i < 4; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Predicate()(t[0], t[1], t[2], t[3]);
  }
};

template<class K, class Method_tag>
inline
bool
sdg_is_degenerate_edge_ftC2(const typename K::Site_2& p,
			    const typename K::Site_2& q,
			    const typename K::Site_2& r,
			    const typename K::Site_2& t,
			    const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_is_degenerate_edge_ftC2<FT,MTag,ITag>   Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q, r, t};

  ITag itag;

  FT v[48];
  char site_types[8];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 4; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 48; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class FT, class MTag, class ITag>
class Sdg_arrangement_type_ftC2
{
 public:
  typedef Simple_cartesian<FT>                                Rep;
  typedef Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>   Kernel;
  typedef Sdg_arrangement_type_C2<Kernel>                     Predicate;
  typedef typename Predicate::result_type                     result_type;
  typedef typename Kernel::Site_2                             Site_2;

  inline result_type
  operator()(const FT v[], const char site_types[]) const
  {
    must_be_filtered(FT());

    Site_2 t[2];

    for (unsigned int i = 0, k = 0, j = 0; i < 2; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Predicate()(t[0], t[1]);
  }
};


template<class K, class Method_tag>
inline
typename
Sdg_arrangement_type_ftC2<typename K::FT,Method_tag,
			  typename K::Intersections_tag>::result_type
sdg_arrangement_type_C2(const typename K::Site_2& p,
			const typename K::Site_2& q,
			const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_arrangement_type_ftC2<FT,MTag,ITag>   Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q};

  ITag itag;

  FT v[24];
  char site_types[4];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 2; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 24; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class FT, class ITag>
class Sdg_are_parallel_ftC2
{
 public:
  inline
  bool operator()(const FT v[], const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_are_parallel_C2<Kernel>               Predicate;

    must_be_filtered(FT());

    Site_2 t[2];

    for (unsigned int i = 0, k = 0, j = 0; i < 2; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Predicate()(t[0], t[1]);
  }
};


template<class K>
inline
bool
sdg_are_parallel_C2(const typename K::Site_2& p,
		    const typename K::Site_2& q)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;

  typedef Sdg_are_parallel_ftC2<FT,ITag>   Predicate_ftC2;

  typename K::Site_2 tt[] = {p, q};

  ITag itag;

  FT v[24];
  char site_types[4];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 2; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 24; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class FT, class MTag, class ITag, int Num_sites>
class Sdg_oriented_side_ftC2
{
 public:
  inline
  Oriented_side operator()(const FT v[], const char site_types[]) const
  {
    typedef Simple_cartesian<FT>                 Rep;
    typedef
      CGAL::Segment_Delaunay_graph_kernel_wrapper_2<Rep,ITag>  Kernel;

    typedef typename Kernel::Site_2                   Site_2;
    typedef Sdg_oriented_side_C2<Kernel,MTag>         Predicate;

    typedef
      Sdg_predicate_caller<Oriented_side, Predicate, Num_sites> Caller;

    must_be_filtered(FT());

    Site_2 t[Num_sites];

    for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
      t[i] = sdg_get_site_C2<Kernel>(v, k, site_types, j, ITag());
    }

    return Caller()(t);
  }
};

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Oriented_side
sdg_oriented_side_ftC2(const typename K::Site_2& q,
		       const typename K::Site_2& s,
		       const typename K::Site_2& p,
		       const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_oriented_side_ftC2<FT,MTag,ITag,3>   Predicate_ftC2;

  typename K::Site_2 tt[] = {q, s, p};

  ITag itag;

  FT v[36];
  char site_types[6];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 3; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 36; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Oriented_side
sdg_oriented_side_ftC2(const typename K::Site_2& s1,
		       const typename K::Site_2& s2,
		       const typename K::Site_2& s3,
		       const typename K::Site_2& s,
		       const typename K::Site_2& p,
		       const Method_tag& mtag)
{
  typedef typename K::FT                  FT;
  typedef typename K::Intersections_tag   ITag;
  typedef Method_tag                      MTag;

  typedef Sdg_oriented_side_ftC2<FT,MTag,ITag,5>   Predicate_ftC2;

  typename K::Site_2 tt[] = {s1, s2, s3, s, p};

  ITag itag;

  FT v[60];
  char site_types[10];

  unsigned int i, j, k;
  for (i = 0, k = 0, j = 0; i < 5; i++) {
    sdg_predicate_push_back_C2<K>(tt[i], v, k, site_types, j, itag);
  }
  for (unsigned int ii = k; ii < 60; ii++) {
    v[ii] = FT(0);
  }

  return Predicate_ftC2()(v, site_types);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


CGAL_END_NAMESPACE


#ifdef CGAL_ARITHMETIC_FILTER_H
#ifndef CGAL_ARITHMETIC_FILTER_SDG_PREDICATES_FTC2_H
#include <CGAL/Arithmetic_filter/predicates/sdg_predicates_ftC2.h>
#endif // CGAL_ARITHMETIC_FILTER_SDG_PREDICATES_FTC2_H
#endif

#endif // CGAL_SEGMENT_DELAUNAY_GRAPH_2_PREDICATES_FTC2_H

