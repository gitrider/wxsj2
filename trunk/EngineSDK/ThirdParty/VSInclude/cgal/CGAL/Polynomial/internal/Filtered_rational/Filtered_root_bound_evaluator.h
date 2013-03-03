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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Filtered_rational/Filtered_root_bound_evaluator.h $
// $Id: Filtered_root_bound_evaluator.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_FILTERED_ROOT_BOUND_EVALUATOR_H
#define CGAL_POLYNOMIAL_FILTERED_ROOT_BOUND_EVALUATOR_H

#include <CGAL/Polynomial/basic.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

template<class Kernel, class M_t = CGAL::Field_tag>
class Filtered_root_bound_evaluator
{
    public:
        Filtered_root_bound_evaluator(bool ,
            const Kernel k): rb_(k.interval_traits_object().root_bound_object())  {}
        typedef double result_type;
        typedef typename Kernel::Function argument_type;

        result_type operator()(const argument_type& p) const
        {
            Interval_arithmetic_guard iag;
            return rb_(p.interval_function()).sup();
        }
    protected:
        typename Kernel::Interval_traits::Root_bound rb_;
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif                                            // CGAL_POLYNOMIAL_ROOT_BOUND_EVALUATOR_H
