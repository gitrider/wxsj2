// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Kernel/To_rational.h $
// $Id: To_rational.h 29468 2006-03-13 22:35:27Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_TO_RATIONAL_H
#define CGAL_POLYNOMIAL_INTERNAL_TO_RATIONAL_H

#include <CGAL/Polynomial/basic.h>

#ifdef CGAL_POLYNOMIAL_USE_CORE
#include <CORE/BigRat.h>
#include <CORE/Expr.h>
#include <CGAL/Polynomial/internal/Explicit_root.h>
#endif

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

//! Compute the sign after a root.
/*!
  This has specializations for Explicit_roots.
*/
template <class K>
class To_rational
{
public:
  To_rational(){  }

  typedef typename K::NT result_type;
  typedef typename K::Root argument_type;

  template <class T>
  result_type operator()(const T &v) const
  {
    return v.to_rational();
  }
#ifdef CGAL_POLYNOMIAL_USE_CORE
  CORE::BigRat operator()(const Explicit_root<CORE::Expr> &r) const {
    return r.representation().BigRatValue();
  }
#endif
  double operator()(double v) const
  {
    return v;
  }
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
