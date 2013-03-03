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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Rational/Sign_below_rational.h $
// $Id: Sign_below_rational.h 29769 2006-03-26 12:21:16Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_SIGN_BELOW_RATIONAL_H
#define CGAL_POLYNOMIAL_INTERNAL_SIGN_BELOW_RATIONAL_H

#include <CGAL/Polynomial/basic.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

template <class Kernel>
CGAL_POLYNOMIAL_NS::Sign sign_below(const typename Kernel::Function &p,
const typename Kernel::NT &nt, const Kernel &k)
{
//CGAL_exactness_precondition(k.sign_at_object(p)(nt)==CGAL::ZERO);
    CGAL_POLYNOMIAL_NS::Sign sn= k.sign_at_object(p)(nt);
    if (sn != CGAL_POLYNOMIAL_NS::ZERO) return sn;
    typename Kernel::Function pcur=k.differentiate_object()(p);
    int count=1;
    do {
        CGAL_POLYNOMIAL_NS::Sign sn= k.sign_at_object(pcur)(nt);
        if (sn != CGAL_POLYNOMIAL_NS::ZERO) {
            if ((count %2==1 && sn== CGAL_POLYNOMIAL_NS::POSITIVE)
            || (count%2==0 && sn==CGAL_POLYNOMIAL_NS::NEGATIVE)) {
                return CGAL_POLYNOMIAL_NS::NEGATIVE;
            }
            else {
                return CGAL_POLYNOMIAL_NS::POSITIVE;
            }
        }
	pcur= k.differentiate_object()(pcur);
	if (pcur.degree() <0) return  CGAL_POLYNOMIAL_NS::ZERO;
    } while (1);
    //return CGAL_POLYNOMIAL_NS::ZERO;
}


template <class Kernel>
class Sign_below_rational
{
    public:
        Sign_below_rational(){}
        Sign_below_rational(const typename Kernel::Function &p, Kernel k= Kernel()):p_(p), k_(k){}
        typedef typename Kernel::NT argument_type;
//  typedef typename POLYNOMIAL_NS::Sign result_type;
// g++ 3.4 does not like the above typedef
        typedef CGAL_POLYNOMIAL_NS::Sign result_type;
        result_type operator()(const argument_type &n) const
        {
            return sign_below(p_,n, k_);
        }
    protected:
        typename Kernel::Function p_;
        Kernel k_;
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
