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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Rational/Invert_variable.h $
// $Id: Invert_variable.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_KERNEL_INVERT_VARIABLE_H
#define CGAL_POLYNOMIAL_KERNEL_INVERT_VARIABLE_H

#include <CGAL/Polynomial/basic.h>
#include <vector>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE
//------------------------------------------------------------------

template <class Polynomial>
class Invert_variable
{
    public:
        typedef typename Polynomial::NT NT;
        typedef NT           argument_type;
        typedef Polynomial   result_type;

        Invert_variable(){}

        void write(std::ostream &out) const
        {
            out << "invert_var";
        }

        result_type operator()(const Polynomial &f) const
        {
            int deg = f.degree();
            std::vector<NT> coef(deg + 1);

            for (int i = 0; i <= deg; i++) {
                coef[i] = f[deg-i];
            }

            return Polynomial(coef.begin(), coef.end());
        }
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
