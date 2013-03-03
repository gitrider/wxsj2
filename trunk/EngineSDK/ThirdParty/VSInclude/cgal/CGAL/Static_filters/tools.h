// Copyright (c) 2001,2004  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Static_filters/tools.h $
// $Id: tools.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion
 
#ifndef CGAL_STATIC_FILTERS_TOOLS_H
#define CGAL_STATIC_FILTERS_TOOLS_H

#include <CGAL/basic.h>

CGAL_BEGIN_NAMESPACE

// Utility function to check a posteriori that a subtraction was performed
// without rounding error.
inline bool diff_was_exact(double a, double b, double ab)
{
    return ab+b == a && a-ab == b;
}

// Auxiliary function to check if static filters can be applied, that is,
// if to_double() does not add roundoff errors.
// TODO :
// - generalize it to other number types.
// - promote it as a number type requirement ?
// - naming : is_representable_in_double() ?
//            is_representable<T>() for representable in T ?

// Current semantics :  bool fit_in_double(const NT& n, double &)
//
// - returns true means that "n" is exactly representable by a double,
//   _and_ then "returns" it in the reference.
// - it is fine to return false conservatively.

template < typename T >
inline bool fit_in_double(const T&, double&) { return false; }

inline bool fit_in_double(const double& d, double& r) { r = d; return true; }

inline bool fit_in_double(const float& f, double& r) { r = f; return true; }

inline bool fit_in_double(const int& i, double& r) { r = i; return true; }

template < typename ET >
class Lazy_exact_nt;

template < typename ET >
inline bool fit_in_double(const Lazy_exact_nt<ET>&, double&);

CGAL_END_NAMESPACE

#endif // CGAL_STATIC_FILTERS_TOOLS_H
