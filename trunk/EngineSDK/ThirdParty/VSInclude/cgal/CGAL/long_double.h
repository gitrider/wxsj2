// Copyright (c) 2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/long_double.h $
// $Id: long_double.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_LONG_DOUBLE_H
#define CGAL_LONG_DOUBLE_H

#include <CGAL/basic.h>
#include <utility>
#include <cmath>
#ifdef CGAL_CFG_IEEE_754_BUG
#  include <CGAL/IEEE_754_unions.h>
#endif
#ifdef __sgi
#  include <fp_class.h>
#endif

#include <CGAL/Interval_nt.h>

CGAL_BEGIN_NAMESPACE

template<> struct Number_type_traits<long double> {
  typedef Tag_false  Has_gcd;
  typedef Tag_true   Has_division;
  typedef Tag_true   Has_sqrt;

  typedef Tag_false  Has_exact_ring_operations;
  typedef Tag_false  Has_exact_division;
  typedef Tag_false  Has_exact_sqrt;
};

inline
double
to_double(const long double & d)
{ return d; }

inline
std::pair<double,double>
to_interval(const long double & d)
{
  // We hope that the long double -> double conversion
  // follows the current rounding mode.
  Protect_FPU_rounding<true> P(CGAL_FE_UPWARD);
  volatile long double md = -d; // needed otherwise the conversion can get
                                // factorized between d and -d...
  return std::make_pair(- (double) CGAL_IA_FORCE_TO_DOUBLE(md),
                        (double) CGAL_IA_FORCE_TO_DOUBLE(d));
}

inline
long double
sqrt(const long double & d)
{ return std::sqrt(d); }

#ifdef __sgi

inline
bool is_finite(const long double & d)
{
    switch (fp_class_d(d)) {
    case FP_POS_NORM:
    case FP_NEG_NORM:
    case FP_POS_ZERO:
    case FP_NEG_ZERO:
    case FP_POS_DENORM:
    case FP_NEG_DENORM:
        return true;
    case FP_SNAN:
    case FP_QNAN:
    case FP_POS_INF:
    case FP_NEG_INF:
        return false;
    }
    return false; // NOT REACHED
}

inline
bool is_valid(const long double & d)
{
    switch (fp_class_d(d)) {
    case FP_POS_NORM:
    case FP_NEG_NORM:
    case FP_POS_ZERO:
    case FP_NEG_ZERO:
    case FP_POS_INF:
    case FP_NEG_INF:
    case FP_POS_DENORM:
    case FP_NEG_DENORM:
        return true;
    case FP_SNAN:
    case FP_QNAN:
        return false;
    }
    return false; // NOT REACHED
}

#elif defined CGAL_CFG_IEEE_754_BUG

#define CGAL_EXPONENT_DOUBLE_MASK   0x7ff00000
#define CGAL_MANTISSA_DOUBLE_MASK   0x000fffff

inline
bool
is_finite_by_mask_long_double(unsigned int h)
{
  unsigned int e = h & CGAL_EXPONENT_DOUBLE_MASK;
  return ( ( e ^ CGAL_EXPONENT_DOUBLE_MASK ) != 0 );
}

inline
bool
is_nan_by_mask_long_double(unsigned int h, unsigned int l)
{
  if ( is_finite_by_mask_long_double(h) )
      return false;
  return (( h & CGAL_MANTISSA_DOUBLE_MASK ) != 0) || (( l & 0xffffffff ) != 0);
}

inline
bool
is_finite( const long double& dble)
{
  double d = dble;
  IEEE_754_double* p = reinterpret_cast<IEEE_754_double*>(&d);
  return is_finite_by_mask_long_double( p->c.H );
}

inline
bool
is_valid( const long double& dble)
{
  double d = dble;
  IEEE_754_double* p = reinterpret_cast<IEEE_754_double*>(&d);
  return ! ( is_nan_by_mask_long_double( p->c.H, p->c.L ));
}

#else

inline
bool
is_valid(const long double & d)
{ return (d == d); }

inline
bool
is_finite(const long double & d)
{ return (d == d) && (is_valid(d-d)); }

#endif

inline
io_Operator
io_tag(const long double &)
{ return io_Operator(); }

CGAL_END_NAMESPACE

#endif // CGAL_LONG_DOUBLE_H
