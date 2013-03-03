// Copyright (c) 2000-2004  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Simple_cartesian.h $
// $Id: Simple_cartesian.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann, Sylvain Pion

#ifndef CGAL_SIMPLE_CARTESIAN_H
#define CGAL_SIMPLE_CARTESIAN_H

#include <CGAL/Cartesian/Cartesian_base.h>
#include <CGAL/Handle_for.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>

CGAL_BEGIN_NAMESPACE

template < typename FT_, typename Kernel_ >
struct Cartesian_base_no_ref_count
  : public Cartesian_base< Kernel_, FT_ >
{
    typedef FT_                                           RT;
    typedef FT_                                           FT;

    // The mecanism that allows to specify reference-counting or not.
    template < typename T >
    struct Handle { typedef T   type; };

    template < typename Kernel2 >
    struct Base { typedef Cartesian_base_no_ref_count<FT_, Kernel2>  Type; };

    typedef Kernel_ K;
#define CGAL_Kernel_pred(Y,Z) typedef CartesianKernelFunctors::Y<K> Y; \
                              Y Z() const { return Y(); }
#define CGAL_Kernel_cons(Y,Z) CGAL_Kernel_pred(Y,Z)

#include <CGAL/Kernel/interface_macros.h>
};

template < typename FT_ >
struct Simple_cartesian
  : public Type_equality_wrapper<
                Cartesian_base_no_ref_count<FT_, Simple_cartesian<FT_> >,
                Simple_cartesian<FT_> >
{};

CGAL_END_NAMESPACE

#endif // CGAL_SIMPLE_CARTESIAN_H
