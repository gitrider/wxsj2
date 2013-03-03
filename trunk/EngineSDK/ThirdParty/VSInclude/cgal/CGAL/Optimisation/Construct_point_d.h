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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Optimisation_basic/include/CGAL/Optimisation/Construct_point_d.h $
// $Id: Construct_point_d.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.ethz.ch>

#ifndef CGAL_OPTIMISATION_CONSTRUCT_POINT_D_H
#define CGAL_OPTIMISATION_CONSTRUCT_POINT_D_H

// includes
#  include <CGAL/Kernel_d/Interface_classes.h>
#  include <CGAL/Kernel_d/Point_d.h>

CGAL_BEGIN_NAMESPACE

// Class declaration
// =================
template < class K >
class _Construct_point_d;

// Class interface
// ===============
template < class K_ >
class _Construct_point_d {
  public:
    // self
    typedef  K_                         K;
    typedef  _Construct_point_d<K>      Self;

    // types
    typedef  typename K::Point_d        Point;

    // creation
    _Construct_point_d( ) { }

    // operations
    template < class InputIterator >
    Point
    operator() ( int d, InputIterator first, InputIterator last) const
    {
        // d-dim kernel has no functor to construct a point, use point's
        // constructor
	return Point( d, first, last);
    }
};

CGAL_END_NAMESPACE

#endif // CGAL_OPTIMISATION_CONSTRUCT_POINT_D_H

// ===== EOF ==================================================================
