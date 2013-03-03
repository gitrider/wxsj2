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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Derivitive_filter_function_kernel.h $
// $Id: Derivitive_filter_function_kernel.h 28701 2006-02-22 19:40:46Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_NUMERIC_SOLVER_H
#define CGAL_KINETIC_NUMERIC_SOLVER_H
#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/Numeric_root_stack.h>
#include <CGAL/Polynomial/internal/numeric_solvers.h>

CGAL_KINETIC_BEGIN_NAMESPACE

template <class Traits>
struct Derivitive_filter_function_kernel: public Traits
{
  typedef typename Traits::Function Function;
  
  typedef CGAL::POLYNOMIAL::Numeric_root_stack<Traits,
					       CGAL::POLYNOMIAL::internal::Turkowski_cleaned_numeric_solver> Root_stack;
  /*class Root_stack: public CGAL_POLYNOMIAL_NS::internal::Numeric_root_stack_core<Traits, true> {
    typedef CGAL_POLYNOMIAL_NS::internal::Numeric_root_stack_core<Traits, true> Parent;
    public:
    typedef typename Parent::Root Root;
    typedef typename Traits::Function Function;
    Root_stack(const typename Traits::Function &f,
    Root lb, Root ub,
    const Traits&k): Parent(f, lb, ub, k){
    CGAL_KINETIC_LOG(LOG_LOTS, "Solved " << f << " from " << lb << " to " << ub << " to get ");
    for (unsigned int i=0; i< Parent::roots_.size(); ++i){
    CGAL_KINETIC_LOG(LOG_LOTS, Parent::roots_[i] << " ");
    }
    CGAL_KINETIC_LOG(LOG_LOTS, std::endl);
    }

    Root_stack(){};
    };*/

  typedef typename Root_stack::Root Root;

  Derivitive_filter_function_kernel(Traits tr): Traits(tr){}
  Derivitive_filter_function_kernel(){}

  Root_stack root_stack_object(const typename Traits::Function &f,
			       const Root &lb,
			       const Root &ub) const {
    //std::cout << "Solving " << f << " from " << lb << " to " << ub;
    Root_stack rs= Root_stack(f, lb, ub, *this);
    //if (! rs.empty()) std::cout << " got " << rs.top() << std::endl;
    return rs;
  }

  using Traits::Rational_between_roots;
  using Traits::rational_between_roots_object;
  using Traits::Is_rational;
  using Traits::is_rational_object;
  using Traits::To_rational;
  using Traits::to_rational_object;
  using Traits::Root_stack_traits;
  using Traits::root_stack_traits_object;
  using Traits::NT;
  using Traits::differentiate_object;
  using Traits::Differentiate;
  using Traits::Negate_variable;
  using Traits::negate_variable_object;
};

CGAL_KINETIC_END_NAMESPACE
#endif                                            // inclusion guard
