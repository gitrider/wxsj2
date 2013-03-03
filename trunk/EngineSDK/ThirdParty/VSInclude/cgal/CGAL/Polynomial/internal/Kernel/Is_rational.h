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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Kernel/Is_rational.h $
// $Id: Is_rational.h 29334 2006-03-10 00:00:09Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_IS_RATIONAL_H
#define CGAL_POLYNOMIAL_INTERNAL_IS_RATIONAL_H

#include <CGAL/Polynomial/basic.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

//! Compute the sign after a root.
/*!
  This has specializations for Explicit_roots.
*/
template <class K>
class Is_rational
{
    public:
        Is_rational(){  }

        typedef bool result_type;
        typedef typename K::Root argument_type;

        template <class T>
            result_type operator()(const T &v) const
        {
	  return v.is_rational();
        }

        bool operator()(double) const
        {
            return true;
        }
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
