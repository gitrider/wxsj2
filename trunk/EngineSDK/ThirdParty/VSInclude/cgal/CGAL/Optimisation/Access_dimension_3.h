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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Optimisation_basic/include/CGAL/Optimisation/Access_dimension_3.h $
// $Id: Access_dimension_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.ethz.ch>

#ifndef CGAL_OPTIMISATION_ACCESS_DIMENSION_3_H
#define CGAL_OPTIMISATION_ACCESS_DIMENSION_3_H

CGAL_BEGIN_NAMESPACE

// Class declaration
// =================
template < class R_ >
class Access_dimension_3;

// Class interface
// ===============
template < class R_ >
class Access_dimension_3 {
  public:
    // self
    typedef  R_                         R;
    typedef  Access_dimension_3<R>      Self;

    // types
    typedef  typename R::Point_3        Point;

    // unary function class types
    typedef  int                        result_type;
    typedef  Point                      argument_type;

    // creation
    Access_dimension_3( ) { }

    // operations
    int  operator() ( const Point& p) const { return p.dimension(); }
};

CGAL_END_NAMESPACE

#endif // CGAL_OPTIMISATION_ACCESS_DIMENSION_3_H

// ===== EOF ==================================================================
