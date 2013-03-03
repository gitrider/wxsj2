// Copyright (c) 1998-2004  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Distance_3/include/CGAL/squared_distance_3_2.h $
// $Id: squared_distance_3_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Geert-Jan Giezeman, Andreas Fabri


#ifndef CGAL_DISTANCE_3_2_H
#define CGAL_DISTANCE_3_2_H

#include <CGAL/squared_distance_3_0.h>

#include <CGAL/Segment_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Plane_3.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template <class K>
bool
contains_vector(const typename CGAL_WRAP(K)::Plane_3 &pl, 
		const typename CGAL_WRAP(K)::Vector_3 &vec,
		const K&)
{
  typedef typename K::RT RT;
  return pl.a()*vec.hx() + pl.b()*vec.hy() + pl.c() * vec.hz() == RT(0);
}


template <class K>
inline typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Point_3 & pt,
    const typename CGAL_WRAP(K)::Plane_3 & plane,
    const K& k)
{
  typename K::Construct_vector_3 construct_vector;
  typedef typename K::Vector_3 Vector_3;
  Vector_3 diff = construct_vector(plane.point(), pt);
  return squared_distance_to_plane(plane.orthogonal_vector(), diff, k);
}



template <class K>
inline typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Plane_3 & plane,
    const typename CGAL_WRAP(K)::Point_3 & pt,
    const K& k)
{
    return squared_distance(pt, plane, k);
}

template <class K>
typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Line_3 &line,
    const typename CGAL_WRAP(K)::Plane_3 &plane,
    const K& k)
{
    typedef typename K::FT FT;
    if (contains_vector(plane, line.direction().vector(), k))
        return squared_distance(plane, line.point(), k);
    return FT(0);
}


template <class K>
inline typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Plane_3 & p,
    const typename CGAL_WRAP(K)::Line_3 & line,
    const K& k)
{
    return squared_distance(line, p, k);
}

template <class K>
typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Ray_3 &ray,
    const typename CGAL_WRAP(K)::Plane_3 &plane,
    const K& k)
{
    typename K::Construct_vector_3 construct_vector;
    typedef typename K::Point_3 Point_3;
    typedef typename K::Vector_3 Vector_3;
    typedef typename K::RT RT;
    typedef typename K::FT FT;
    const Point_3 &start = ray.start();
    const Point_3 &planepoint = plane.point();
    Vector_3 start_min_pp = construct_vector(planepoint, start);
    Vector_3 end_min_pp = ray.direction().vector();
    const Vector_3 &normal = plane.orthogonal_vector();
    RT sdm_rs2pp = wdot(normal, start_min_pp, k);
    RT sdm_re2pp = wdot(normal, end_min_pp, k);
    switch (CGAL_NTS sign(sdm_rs2pp)) {
    case -1:
        if (sdm_re2pp > RT(0))
            return FT(0);
        return squared_distance_to_plane(normal, start_min_pp, k);
    case 0:
    default:
        return FT(0);
    case 1:
        if (sdm_re2pp < RT(0))
            return FT(0);
        return squared_distance_to_plane(normal, start_min_pp, k);
    }
}


template <class K>
inline typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Plane_3 & plane,
    const typename CGAL_WRAP(K)::Ray_3 & ray,
    const K& k)
{
    return squared_distance(ray, plane, k);
}

template <class K>
typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Segment_3 &seg,
    const typename CGAL_WRAP(K)::Plane_3 &plane,
    const K& k)
{
    typename K::Construct_vector_3 construct_vector;
    typedef typename K::Point_3 Point_3;
    typedef typename K::Vector_3 Vector_3;
    typedef typename K::RT RT;
    typedef typename K::FT FT;
    const Point_3 &start = seg.start();
    const Point_3 &end = seg.end();
    if (start == end)
        return squared_distance(start, plane, k);
    const Point_3 &planepoint = plane.point();
    Vector_3 start_min_pp = construct_vector(planepoint, start);
    Vector_3 end_min_pp = construct_vector(planepoint, end);
    const Vector_3 &normal = plane.orthogonal_vector();
    RT sdm_ss2pp = wdot(normal, start_min_pp, k);
    RT sdm_se2pp = wdot(normal, end_min_pp, k);
    switch (CGAL_NTS sign(sdm_ss2pp)) {
    case -1:
        if (sdm_se2pp >= RT(0))
            return FT(0);
        if (sdm_ss2pp >= sdm_se2pp)
            return squared_distance_to_plane(normal, start_min_pp, k);
        else
            return squared_distance_to_plane(normal, end_min_pp, k);
    case 0:
    default:
        return FT(0);
    case 1:
        if (sdm_se2pp <= RT(0))
            return FT(0);
        if (sdm_ss2pp <= sdm_se2pp)
            return squared_distance_to_plane(normal, start_min_pp, k);
        else
            return squared_distance_to_plane(normal, end_min_pp, k);
    }
}


template <class K>
inline typename K::FT
squared_distance(
    const typename CGAL_WRAP(K)::Plane_3 & plane,
    const typename CGAL_WRAP(K)::Segment_3 & seg,
    const K& k)
{
    return squared_distance(seg, plane, k);
}


} // namespace CGALi


template <class K>
bool
contains_vector(const Plane_3<K> &pl, const Vector_3<K> &vec)
{
  return CGALi::contains_vector(pl,vec, K());
}


template <class K>
inline 
typename K::FT
squared_distance(
    const Point_3<K> & pt,
    const Plane_3<K> & plane)
{
  return CGALi::squared_distance(pt, plane, K());
}



template <class K>
inline 
typename K::FT
squared_distance(
    const Plane_3<K> & plane,
    const Point_3<K> & pt)
{
    return CGALi::squared_distance(pt, plane, K());
}

template <class K>
inline
typename K::FT
squared_distance(
    const Line_3<K> &line,
    const Plane_3<K> &plane)
{
    return CGALi::squared_distance(line, plane, K());
}


template <class K>
inline 
typename K::FT
squared_distance(
    const Plane_3<K> & p,
    const Line_3<K> & line)
{
    return CGALi::squared_distance(line, p, K());
}

template <class K>
inline
typename K::FT
squared_distance(
    const Ray_3<K> &ray,
    const Plane_3<K> &plane)
{
  return CGALi::squared_distance(ray, plane, K());
}



template <class K>
inline 
typename K::FT
squared_distance(
    const Plane_3<K> & plane,
    const Ray_3<K> & ray)
{
    return CGALi::squared_distance(ray, plane, K());
}

template <class K>
inline
typename K::FT
squared_distance(
    const Segment_3<K> &seg,
    const Plane_3<K> &plane)
{
  return CGALi::squared_distance(seg, plane, K());

}


template <class K>
inline 
typename K::FT
squared_distance(
    const Plane_3<K> & plane,
    const Segment_3<K> & seg)
{
    return CGALi::squared_distance(seg, plane, K());
}


CGAL_END_NAMESPACE


#endif
