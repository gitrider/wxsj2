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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/Upper_bound_root_stack_Descartes_traits.h $
// $Id: Upper_bound_root_stack_Descartes_traits.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_UPPER_BOUND_ENUMERATOR_DESCARTES_TRAITS_H
#define CGAL_POLYNOMIAL_UPPER_BOUND_ENUMERATOR_DESCARTES_TRAITS_H

#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/internal/Root_stack_traits_base.h>
#include <CGAL/Polynomial/internal/Rational/Descartes_root_counter.h>
#include <CGAL/Polynomial/internal/Rational/Sturm_root_counter.h>
#include <CGAL/Polynomial/internal/Simple_interval_root.h>

CGAL_POLYNOMIAL_BEGIN_NAMESPACE

template <class Poly>
class Upper_bound_root_stack_Descartes_traits: public internal::Root_stack_traits_base<Poly>
{
    typedef internal::Root_stack_traits_base<Poly>  P;
    typedef Upper_bound_root_stack_Descartes_traits<Poly> This;
    public:
        typedef CGAL_POLYNOMIAL_NS::internal::Simple_interval_root<This> Root;

        typedef internal::Descartes_root_counter<This> Root_count;
        Root_count root_count_object(const typename P::Function &f) const
        {
            return Root_count(f, *this);
        }
        typedef internal::Sturm_root_counter<This> Sturm_root_count;
        Sturm_root_count Sturm_root_count_object(const typename P::Function &f) const
        {
            return Sturm_root_count(f, *this);
        }
};

CGAL_POLYNOMIAL_END_NAMESPACE
#endif
