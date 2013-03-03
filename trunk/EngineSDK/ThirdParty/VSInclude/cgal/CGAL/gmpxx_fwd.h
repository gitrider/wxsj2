// Copyright (c) 2002,2003,2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/gmpxx_fwd.h $
// $Id: gmpxx_fwd.h 30047 2006-04-06 14:09:35Z spion $
// 
//
// Author(s)     : Sylvain Pion
 
#ifndef CGAL_GMPXX_FWD_H
#define CGAL_GMPXX_FWD_H

#ifdef CGAL_USE_GMPXX

#include <gmpxx.h>
#include <CGAL/Root_of_2_fwd.h>

CGAL_BEGIN_NAMESPACE

template < typename T, typename U >
::__gmp_expr<T, T> sqrt(const ::__gmp_expr<T, U> &);

template < typename T, typename U >
double to_double(const ::__gmp_expr<T, U> &);

template < typename T, typename U >
bool is_finite(const ::__gmp_expr<T, U> &);

template < typename T, typename U >
bool is_valid(const ::__gmp_expr<T, U> &);

template < typename T, typename U >
std::pair<double,double> to_interval (const ::__gmp_expr<T, U> &);

std::pair<double, double> to_interval (const mpz_class &);

std::pair<double, double> to_interval (const mpq_class &);

template < typename T, typename U >
::__gmp_expr<T, T> abs(const ::__gmp_expr<T, U>&);

template < typename T, typename U >
::__gmp_expr<T, T> square(const ::__gmp_expr<T, U>&);


template < typename T, typename U >
Sign sign(const ::__gmp_expr<T, U> &);

template < typename T, typename U1, typename U2 >
Comparison_result
compare(const ::__gmp_expr<T, U1> &, const ::__gmp_expr<T, U2> &);

template < typename T, typename U >
bool is_zero(const ::__gmp_expr<T, U> &);

template < typename T, typename U >
bool is_one(const ::__gmp_expr<T, U> &);

template < typename T, typename U >
bool is_positive(const ::__gmp_expr<T, U> &);

template < typename T, typename U >
bool is_negative(const ::__gmp_expr<T, U> &);

template < typename T, typename U1, typename U2, typename U3 >
typename Root_of_traits< ::__gmp_expr<T, T> >::RootOf_2
make_root_of_2(const ::__gmp_expr< T, U1> & a,
               const ::__gmp_expr< T, U2> & b,
               const ::__gmp_expr< T, U3> & c,
               bool d);

CGAL_END_NAMESPACE

#endif // CGAL_USE_GMPXX

#endif // CGAL_GMPXX_FWD_H
