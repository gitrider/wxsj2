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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Kernel/Isolating_interval.h $
// $Id: Isolating_interval.h 30642 2006-04-18 12:42:52Z lsaboret $ $Date: 2006-04-18 14:42:52 +0200 (Tue, 18 Apr 2006) $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_TO_ISOLATING_INTERVAL_H
#define CGAL_POLYNOMIAL_INTERNAL_TO_ISOLATING_INTERVAL_H

#include <CGAL/Polynomial/basic.h>

#ifdef CGAL_POLYNOMIAL_USE_CORE
#include <CGAL/Polynomial/internal/Explicit_root.h>
#include <CORE/BigRat.h>
#include <CORE/Expr.h>
#endif

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

//! Compute the sign after a root.
/*!
  This has specializations for Explicit_roots.
*/
template <class K>
class To_isolating_interval
{
public:
  To_isolating_interval(){  }

  typedef typename std::pair<typename K::NT, typename K::NT> result_type;
  typedef typename K::Root argument_type;

  template <class T>
  const result_type& operator()(const T &v) const
  {
    return v.isolating_interval();
  }

#ifdef CGAL_POLYNOMIAL_USE_CORE
  result_type operator()(const Explicit_root<CORE::Expr> &r) const {
    double a,b;
    r.representation().doubleInterval(a,b); // do something
    return std::make_pair(typename K::NT(a), typename K::NT(b));
  }
#endif
  std::pair<double, double> operator()(double v) const
  {
    return std::make_pair(v,v);
  }
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
