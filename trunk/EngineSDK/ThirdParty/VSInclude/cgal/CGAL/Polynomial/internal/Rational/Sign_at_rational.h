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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Rational/Sign_at_rational.h $
// $Id: Sign_at_rational.h 29334 2006-03-10 00:00:09Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_SIGN_AT_H
#define CGAL_POLYNOMIAL_INTERNAL_SIGN_AT_H

#include <CGAL/Polynomial/basic.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE
template<class Fn>
class Sign_at_rational
{
    protected:
        typedef CGAL_POLYNOMIAL_NS::Sign  Sign;

    public:
        typedef typename Fn::NT   NT;

        typedef Sign                 result_type;
        typedef NT                   argument_type;

        Sign_at_rational(){}
        Sign_at_rational(const Fn& p) : p(p) {}

//! Evaluate the sign of the value of the polynomial at x
        template <class TNT>
            Sign operator()(const TNT& t) const
        {
	  return CGAL_POLYNOMIAL_NS::sign( p(t) );
        }

//! Evaluate the sign of the value of the polynomial
//  at n/d without any divisions
        Sign operator()(const NT &n, const NT& d) const
        {
            Sign s_n = CGAL::sign(n);
            Sign s_d = CGAL::sign(d);

            if ( s_d == CGAL::ZERO ) {
                CGAL_precondition( s_n != CGAL::ZERO );

                Sign s = CGAL::sign( p[p.degree()] );
                if ( CGAL::sign(n) == CGAL::POSITIVE ) {
                    return s;
                }
                else {
                    return opposite(s);
                }
            }

            Fn copy(p);
            if (p.is_zero()) {
                return CGAL::ZERO;
            }
            else if ( p.is_constant() == 0 || s_n == CGAL::ZERO ) {
                return Sign(CGAL::sign( copy[0] ) * s_d);
            }

            NT result = copy[copy.degree()];
            for (int i = copy.degree() - 1; i >= 0; i--) {
                result *= n;
                result += copy[i] * d;
                for (int j = i - 1; j >= 0; j--) {
                    copy.set_coef(j, d * copy[j]);
                }
            }

            return Sign(CGAL::sign(result) * s_d);
        }

    protected:
        Fn p;
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
