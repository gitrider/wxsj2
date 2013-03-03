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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/Default_filtering_traits.h $
// $Id: Default_filtering_traits.h 29468 2006-03-13 22:35:27Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_DEFAULT_FILTERING_TRAITS_H
#define CGAL_POLYNOMIAL_DEFAULT_FILTERING_TRAITS_H
#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/Polynomial.h>
#include <CGAL/Polynomial/Interval_polynomial.h>
#include <CGAL/Polynomial/polynomial_converters.h>


CGAL_POLYNOMIAL_BEGIN_NAMESPACE
template <class NT = Default_field_nt>
struct Default_filtering_traits
{
    typedef Polynomial<NT> Exact_function;
    typedef Interval_polynomial Interval_function;
    typedef Polynomial_converter<Exact_function,
        Interval_function,
        To_interval<NT> > Exact_to_interval_converter;
};

CGAL_POLYNOMIAL_END_NAMESPACE

#ifdef CGAL_DEFAULT_FILTERING_DEFAULT_NT
#undef CGAL_DEFAULT_FILTERING_DEFAULT_NT
#endif
#endif
