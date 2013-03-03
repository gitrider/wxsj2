// Copyright (c) 1999-2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/Quotient_fwd.h $
// $Id: Quotient_fwd.h 30644 2006-04-18 14:10:04Z afabri $
// 
//
// Author(s)     : Stefan Schirra, Sylvain Pion

#ifndef CGAL_QUOTIENT_FWD_H
#define CGAL_QUOTIENT_FWD_H

CGAL_BEGIN_NAMESPACE

template <class NT> class Quotient;
template <class RT> struct Root_of_traits;

template <class NT>
Quotient<NT> sqrt(const Quotient<NT> &);

template <class NT>
Comparison_result compare(const Quotient<NT>&, const Quotient<NT>&);

template <class NT>
double to_double(const Quotient<NT>&);

template <class NT>
std::pair<double,double> to_interval (const Quotient<NT>&);

template <class NT>
bool is_valid(const Quotient<NT>&);

template <class NT>
bool is_finite(const Quotient<NT>&);

template < class NT >
typename Root_of_traits< NT >::RootOf_2
make_root_of_2(const Quotient< NT > &a, const Quotient< NT > &b,
               const Quotient< NT > &c, bool d);

CGAL_END_NAMESPACE

#endif  // CGAL_QUOTIENT_FWD_H
