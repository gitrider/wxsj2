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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_2/include/CGAL/ch_bykat.h $
// $Id: ch_bykat.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra


#ifndef CGAL_CH_BYKAT_H
#define CGAL_CH_BYKAT_H

#include <CGAL/basic.h>
#include <iterator>

CGAL_BEGIN_NAMESPACE

// same as |convex_hull_2(first,last,result)|. {\sc traits}: 
// uses |Traits::Point_2|, |Traits::Less_signed_distance_to_line_2|,
// |Traits::Left_turn_2|,, |Traits::Equal_2| and |Traits::Less_xy_2|.
template <class InputIterator, class OutputIterator, class Traits>
OutputIterator
ch_bykat(InputIterator first, InputIterator last, 
         OutputIterator  result,
         const Traits& ch_traits);

template <class InputIterator, class OutputIterator>
inline
OutputIterator
ch_bykat(InputIterator first, InputIterator last, OutputIterator  result)
{
    typedef std::iterator_traits<InputIterator>   ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    return ch_bykat( first, last, result, Kernel());
}


template <class InputIterator, class OutputIterator, class Traits>
OutputIterator
ch_bykat_with_threshold(InputIterator first, InputIterator last, 
                             OutputIterator  result,
                             const Traits& ch_traits);

template <class InputIterator, class OutputIterator>
inline
OutputIterator
ch_bykat_with_threshold(InputIterator first, InputIterator last, 
                        OutputIterator  result)
{
    typedef std::iterator_traits<InputIterator>   ITraits;
    typedef typename ITraits::value_type          value_type;
    typedef CGAL::Kernel_traits<value_type>       KTraits;
    typedef typename KTraits::Kernel              Kernel;
    return ch_bykat_with_threshold( first, last, result, Kernel());
}

CGAL_END_NAMESPACE

#ifdef CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION
#include <CGAL/ch_bykat.C>
#endif // CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION

#endif // CGAL_CH_BYKAT_H

