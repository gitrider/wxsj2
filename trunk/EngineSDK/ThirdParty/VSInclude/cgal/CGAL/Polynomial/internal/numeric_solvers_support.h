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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/numeric_solvers_support.h $
// $Id: numeric_solvers_support.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_NUMERIC_SOLVERS_SUPPORT_H
#define CGAL_POLYNOMIAL_INTERNAL_NUMERIC_SOLVERS_SUPPORT_H
#include <CGAL/Polynomial/basic.h>
#include <vector>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

template <class NT>
inline bool root_is_good(NT r, NT c, NT lb, NT ub, NT tol=.0000005)
{
  if (std::abs(c) < tol && r > lb && r < ub) {
        return true;
    } else return false;
}


void compute_quadratic_roots(const double *begin, const double *end,
			     double lb, double ub,
			     std::vector<double> &roots);

void compute_quadratic_cleaned_roots(const double *begin, const double *end,
				     double lb, double ub, 
				     std::vector<double> &roots);

void compute_linear_roots(const double *begin, const double *end,
			  double lb, double ub,
			  std::vector<double> &roots);

void compute_linear_cleaned_roots(const double *begin, const double *end,
				  double lb, double ub, 
				  std::vector<double> &roots);



void filter_solver_roots(const double *begin, const double *end,
			 double lb, double ub, double last,
			 std::vector<double> &roots);

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
