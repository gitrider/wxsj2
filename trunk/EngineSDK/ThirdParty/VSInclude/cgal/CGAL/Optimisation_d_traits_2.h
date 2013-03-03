// Copyright (c) 1997-2001  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Optimisation_basic/include/CGAL/Optimisation_d_traits_2.h $
// $Id: Optimisation_d_traits_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.ethz.ch>

#ifndef CGAL_OPTIMISATION_D_TRAITS_2_H
#define CGAL_OPTIMISATION_D_TRAITS_2_H

// includes
#ifndef CGAL_OPTIMISATION_ACCESS_DIMENSION_2_H
#  include <CGAL/Optimisation/Access_dimension_2.h>
#endif
#ifndef CGAL_OPTIMISATION_ACCESS_COORDINATES_BEGIN_2_H
#  include <CGAL/Optimisation/Access_coordinates_begin_2.h>
#endif
#ifndef CGAL_OPTIMISATION_CONSTRUCT_POINT_2_H
#  include <CGAL/Optimisation/Construct_point_2.h>
#endif

CGAL_BEGIN_NAMESPACE

// Class declaration
// =================
template < class K_, class ET_ = typename K_::RT,
                     class NT_ = typename K_::RT >
class Optimisation_d_traits_2;

// Class interface
// ===============
template < class K_, class ET_, class NT_>
class Optimisation_d_traits_2 {
  public:
    // self
    typedef  K_                         K;
    typedef  ET_                        ET;
    typedef  NT_                        NT;
    typedef  Optimisation_d_traits_2<K,ET,NT>
                                        Self;

    // types
    typedef  typename K::Point_2        Point_d;

    typedef  typename K::Rep_tag        Rep_tag;

    typedef  typename K::RT             RT;
    typedef  typename K::FT             FT;

    typedef  CGAL::Access_dimension_2<K>      Access_dimension_d;
    typedef  CGAL::Access_coordinates_begin_2<K>
                                        Access_coordinates_begin_d;

    // this does not (yet) work:
    // typedef  typename K::Construct_point_2    Construct_point_d;
    typedef  _Construct_point_2<K>       Construct_point_d;

    // creation
    Optimisation_d_traits_2( ) { }
    Optimisation_d_traits_2( const Optimisation_d_traits_2<K_,ET_,NT_>&) {}

    // operations
    Access_dimension_d
    access_dimension_d_object( ) const
        { return Access_dimension_d(); }

    Access_coordinates_begin_d
    access_coordinates_begin_d_object( ) const
        { return Access_coordinates_begin_d(); }

    Construct_point_d
    construct_point_d_object( ) const
        { return Construct_point_d(); }
};

CGAL_END_NAMESPACE

#endif // CGAL_OPTIMISATION_D_TRAITS_2_H

// ===== EOF ==================================================================
