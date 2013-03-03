// Copyright (c) 1999  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_2/include/CGAL/ch_jarvis.h $
// $Id: ch_jarvis.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra


#ifndef CGAL_CH_JARVIS_H
#define CGAL_CH_JARVIS_H

#include <CGAL/basic.h>
#include <iterator>


CGAL_BEGIN_NAMESPACE

// generates the counterclockwise ordered subsequence of
// extreme points between |start_p| and |stop_p| of the points in the
// range [|first|,|last|), starting at position result with point |start_p|.
// The last point generated is the point preceding |stop_p| in the
// counterclockwise order of extreme points.
// {\it Precondition:} |start_p| and |stop_p| are extreme points with respect
// to the points in the range [|first|,|last|) and |stop_p| is an element of
// range [|first|,|last|).
// {\sc traits}: uses |Traits::Point_2| $\equiv$ |Point|, |Traits::Equal_2|
// and |Traits::Less_rotate_ccw_2|.
template <class ForwardIterator, class OutputIterator, 
          class Point, class Traits>
OutputIterator
ch_jarvis_march(ForwardIterator first, ForwardIterator last,
                const Point& start_p, const Point& stop_p,
                OutputIterator  result,
                const Traits& ch_traits);

template <class ForwardIterator, class OutputIterator, class Point>
inline
OutputIterator
ch_jarvis_march(ForwardIterator first, ForwardIterator last,
                const Point& start_p,
                const Point& stop_p,
                OutputIterator  result )
{
    typedef CGAL::Kernel_traits<Point>  KTraits;
    typedef typename KTraits::Kernel    Kernel;
    return ch_jarvis_march( first, last, start_p, stop_p, result, Kernel());
}


// same as |convex_hull_2(first,last,result)|.
// {\sc traits}: uses |Traits::Point_2|, |Traits::Less_rotate_ccw_2|,
// |Traits::Equal_2| and |Traits::Less_xy_2|.
template <class ForwardIterator, class OutputIterator, class Traits>
OutputIterator
ch_jarvis(ForwardIterator first, ForwardIterator last, 
               OutputIterator  result,
               const Traits& ch_traits);
template <class ForwardIterator, class OutputIterator>
inline
OutputIterator
ch_jarvis(ForwardIterator first, ForwardIterator last, OutputIterator  result)
{ 
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    return ch_jarvis( first, last, result, Kernel()); 
}



CGAL_END_NAMESPACE

#ifdef CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION
#include <CGAL/ch_jarvis.C>
#endif // CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION

#endif // CGAL_CH_JARVIS_H







