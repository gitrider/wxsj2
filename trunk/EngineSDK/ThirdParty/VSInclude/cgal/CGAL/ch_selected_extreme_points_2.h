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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_2/include/CGAL/ch_selected_extreme_points_2.h $
// $Id: ch_selected_extreme_points_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra


#ifndef CGAL_CH_SELECTED_EXTREME_POINTS_2_H
#define CGAL_CH_SELECTED_EXTREME_POINTS_2_H

#include <CGAL/basic.h>
#include <iterator>


CGAL_BEGIN_NAMESPACE


// traverses the range [|first|,|last|). After execution, the value of |n| is
// an iterator in the range such that |*n| $\ge_{\rm yx}$ |*it| for
// all iterators |it| in the range. Similarly, for |s|, |w|, and |e| 
// the inequations |*s| $\le_{\rm yx}$ |*it|, |*w| $\le_{\rm xy}$ |*it|,
// and |*e| $\ge_{\rm yx}$ |*it| hold respectively for all iterators 
// |it| in the range.
// {\sc traits}: uses |Traits::Less_xy_2| and |Traits::Less_yx_2|.
template <class ForwardIterator, class Traits>
void
ch_nswe_point( ForwardIterator first, ForwardIterator last,
               ForwardIterator& n,
               ForwardIterator& s,
               ForwardIterator& w,
               ForwardIterator& e,
               const Traits& ch_traits);

template <class ForwardIterator>
inline
void
ch_nswe_point( ForwardIterator first, ForwardIterator last,
               ForwardIterator& n,
               ForwardIterator& s,
               ForwardIterator& w,
               ForwardIterator& e)
{
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    ch_nswe_point(first, last, n, s, w, e, Kernel());
}


// traverses the range [|first|,|last|). After execution, the value of |n| is
// an iterator in the range such that |*n| $\ge_{\rm yx}$ |*it| for
// all iterators |it| in the range. Similarly, for |s| the inequation
// |*s| $\le_{\rm yx}$ |*it| holds for all iterators |it| in the range.
// {\sc traits}: uses function object type |Traits::Less_yx_2|.
template <class ForwardIterator, class Traits>
void
ch_ns_point( ForwardIterator first, ForwardIterator last,
             ForwardIterator& n,
             ForwardIterator& s,
             const Traits& ch_traits );

template <class ForwardIterator>
inline
void
ch_ns_point( ForwardIterator first, ForwardIterator last,
             ForwardIterator& n,
             ForwardIterator& s)
{
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    ch_ns_point(first, last, n, s, Kernel());
}


// traverses the range [|first|,|last|). After execution, the value of |w| is
// an iterator in the range such that |*w| $\le_{\rm xy}$ |*it| for
// all iterators |it| in the range. Similarly, for |e| the inequation
// |*e| $\ge_{\rm yx}$ |*it| holds for all iterators |it| in the range.
// {\sc traits}: uses function object type |Traits::Less_xy_2|.
template <class ForwardIterator, class Traits>
void
ch_we_point( ForwardIterator first, ForwardIterator last,
             ForwardIterator& w,
             ForwardIterator& e,
             const Traits& ch_traits );

template <class ForwardIterator>
inline
void
ch_we_point( ForwardIterator first, ForwardIterator last,
             ForwardIterator& w,
             ForwardIterator& e)
{
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    ch_we_point(first, last, w, e, Kernel());
}


// traverses the range [|first|,|last|). After execution, the value of |n| is
// an iterator in the range such that |*n| $\ge_{\rm yx}$ |*it| for
// all iterators |it| in the range. 
// {\sc traits}: uses |Traits::Less_yx_2|.
template <class ForwardIterator, class Traits>
void
ch_n_point( ForwardIterator first, ForwardIterator last,
            ForwardIterator& n,
            const Traits& ch_traits );

template <class ForwardIterator>
inline
void
ch_n_point( ForwardIterator first, ForwardIterator last, ForwardIterator& n)
{
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    ch_n_point(first, last, n, Kernel());
}


// traverses the range [|first|,|last|). After execution, the value of |s| is
// an iterator in the range such that |*s| $\le_{\rm yx}$ |*it| for
// all iterators |it| in the range. 
// {\sc traits}: uses |Traits::Less_yx_2|.
template <class ForwardIterator, class Traits>
void
ch_s_point( ForwardIterator first, ForwardIterator last,
            ForwardIterator& s,
            const Traits& ch_traits );

template <class ForwardIterator>
inline
void
ch_s_point( ForwardIterator first, ForwardIterator last, ForwardIterator& s)
{
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    ch_s_point(first, last, s, Kernel());
}


// traverses the range [|first|,|last|). After execution, the value of |e| is
// an iterator in the range such that |*e| $\ge_{\rm yx}$ |*it| for
// for all iterators |it| in the range.
// {\sc traits}: uses |Traits::Less_xy_2|.
template <class ForwardIterator, class Traits>
void
ch_e_point( ForwardIterator first, ForwardIterator last,
            ForwardIterator& e,
            const Traits& ch_traits );

template <class ForwardIterator>
inline
void
ch_e_point( ForwardIterator first, ForwardIterator last, ForwardIterator& e)
{
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    ch_e_point(first, last, e, Kernel());
}


// traverses the range [|first|,|last|). After execution, the value of |w| is
// an iterator in the range such that |*w| $\le_{\rm yx}$ |*it| for
// for all iterators |it| in the range.
// {\sc traits}: uses |Traits::Less_yx_2|.
template <class ForwardIterator, class Traits>
void
ch_w_point( ForwardIterator first, ForwardIterator last,
            ForwardIterator& w,
            const Traits& ch_traits );

template <class ForwardIterator>
inline
void
ch_w_point( ForwardIterator first, ForwardIterator last, ForwardIterator& w)
{
    typedef std::iterator_traits<ForwardIterator> ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    ch_w_point(first, last, w, Kernel());
}

CGAL_END_NAMESPACE

#ifdef CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION
#include <CGAL/ch_selected_extreme_points_2.C>
#endif // CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION

#endif // CGAL_CH_SELECTED_EXTREME_POINTS_2_H

