// Copyright (c) 2002  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_23/include/CGAL/Is_a_predicate.h $
// $Id: Is_a_predicate.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_IS_A_PREDICATE_H
#define CGAL_IS_A_PREDICATE_H

// How to determine if a kernel functor is a predicate or a construction.

#include <CGAL/basic.h>
#include <CGAL/enum.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

// By default it's a construction
template <typename Return_type>
struct Return_type_of_predicate {
    typedef CGAL::Tag_false type;
};

// Specializations for predicates
template <>
struct Return_type_of_predicate<bool> {
    typedef CGAL::Tag_true type;
};

template <>
struct Return_type_of_predicate<CGAL::Sign> {
    typedef CGAL::Tag_true type;
};

template <>
struct Return_type_of_predicate<CGAL::Orientation> {
    typedef CGAL::Tag_true type;
};

template <>
struct Return_type_of_predicate<CGAL::Oriented_side> {
    typedef CGAL::Tag_true type;
};

template <>
struct Return_type_of_predicate<CGAL::Bounded_side> {
    typedef CGAL::Tag_true type;
};

template <>
struct Return_type_of_predicate<CGAL::Comparison_result> {
    typedef CGAL::Tag_true type;
};

template <>
struct Return_type_of_predicate<CGAL::Angle> {
    typedef CGAL::Tag_true type;
};

} // namespace CGALi

template <typename Functor>
struct Is_a_predicate {
  typedef typename CGALi::Return_type_of_predicate<
                   typename Functor::result_type>::type type;
};

CGAL_END_NAMESPACE

#endif // CGAL_IS_A_PREDICATE_H
