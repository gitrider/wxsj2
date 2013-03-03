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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Rational/Are_negations.h $
// $Id: Are_negations.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_ARE_NEGATIONS_H
#define CGAL_POLYNOMIAL_INTERNAL_ARE_NEGATIONS_H

#include <CGAL/Polynomial/basic.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

//------------------------------------------------------------------
template <class P>
class Are_negations
{
    public:
        typedef typename P::NT NT;
        typedef P first_argument_type;
        typedef P second_argument_type;
        typedef bool result_type;
        Are_negations(){}

        result_type operator()(const first_argument_type &f0,
            const second_argument_type &f1) const
        {
            if (f0.degree() != f1.degree()) return false;
            else {
                for (int i=0; i<= f0.degree(); ++i) {
                    if (f0[i] != -f1[i]) return false;
                }
                return true;
            }
        }
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
