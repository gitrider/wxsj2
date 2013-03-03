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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Rational/Descartes_root_counter.h $
// $Id: Descartes_root_counter.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_DESCARTES_ROOT_COUNTER_H
#define CGAL_POLYNOMIAL_DESCARTES_ROOT_COUNTER_H

#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/internal/Alternation_counter.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE;

template <class Kernel>
class Descartes_root_counter
{
    public:
        Descartes_root_counter(){}
        Descartes_root_counter(const typename Kernel::Function &f,
            const Kernel &k): map_(k.map_rational_interval_to_positive_object(f)),
        kernel_(k) {

        }

        typedef unsigned int result_type;
        typedef typename Kernel::NT first_argument_type;
        typedef typename Kernel::NT second_argument_type;

//! Note, the result is an upper bound
        template <class NTT>
            result_type operator()(const NTT &lb, const NTT &ub,
            CGAL_POLYNOMIAL_NS::Sign=CGAL_POLYNOMIAL_NS::ZERO,
            CGAL_POLYNOMIAL_NS::Sign=CGAL_POLYNOMIAL_NS::ZERO) const
        {
            typename Kernel::Function mf= map_(lb, ub);

            typename CGAL_POLYNOMIAL_NS::Alternation_counter<first_argument_type> ac;
            for (int i=0; i<= mf.degree(); ++i) {
                ac.push_back(mf[i]);
            }
//std::cout << "Num alternations is " << ac.number_of_alternations() << std::endl;
            return ac.number_of_alternations();
        }
    protected:
        typename Kernel::Map_rational_interval_to_positive map_;
//! What are these?
        Kernel kernel_;
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE;
#endif
