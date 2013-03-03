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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Filtered_rational/Filtered_rational_sign_above_rational.h $
// $Id: Filtered_rational_sign_above_rational.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_FILTERNAL_SIGN_ABOVE_RATIONAL_H
#define CGAL_POLYNOMIAL_INTERNAL_FILTERED_SIGN_ABOVE_RATIONAL_H

#include <CGAL/Polynomial/basic.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

do not use

template <class Kernel>
class Filtered_sign_above_rational
{
    public:
        Filtered_sign_above_rational(){}
        Filtered_sign_above_rational(const typename Kernel::Function &p, Kernel k= Kernel()):p_(p), k_(k){}
        typedef typename Kernel::NT argument_type;
//  typedef typename POLYNOMIAL_NS::Sign result_type;
// g++ 3.4 does not like the above declaration
        typedef int result_type;
        result_type operator()(const argument_type &nt) const
        {
//CGAL_exactness_precondition(k.sign_at_object(p)(nt)==CGAL::ZERO);
            POLYNOMIAL_NS::Sign sn= k_.sign_at_object(p_)(nt);
            if (sn != POLYNOMIAL_NS::ZERO) return sn;
            typename Kernel::Differentiate d= k_.differentiate_object();
            typename Kernel::Function pcur= d(p_);
            do {
                POLYNOMIAL_NS::Sign sn= k_.sign_at_object(pcur)(nt);
                if (sn != POLYNOMIAL_NS::ZERO) return sn;
                else {
                    pcur=d(pcur);
                }
            } while (!pcur.is_zero());
            return POLYNOMIAL_NS::ZERO;
        }
    protected:
        typename Kernel::Function p_;
        Kernel k_;
};
CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
