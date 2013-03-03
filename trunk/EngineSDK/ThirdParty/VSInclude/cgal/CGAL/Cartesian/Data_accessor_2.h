// Copyright (c) 2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/Data_accessor_2.h $
// $Id: Data_accessor_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Herve Bronnimann

#ifndef CGAL_CARTESIAN_DATA_ACCESSOR_2_H
#define CGAL_CARTESIAN_DATA_ACCESSOR_2_H

CGAL_BEGIN_NAMESPACE

// 2D Cartesian point data accessor
template < class R_ >
class Data_accessorC2
{
public:
    // Min_ellipse_2 wants FT public...
    typedef typename R_::FT                   FT;
    typedef typename R_::Point_2              Point;

    typedef R_                           R;

    FT  get_x( const Point &p) const { return( p.x()); }
    FT  get_y( const Point &p) const { return( p.y()); }

    void get( const Point &p, FT &x, FT &y) const { x=get_x(p); y=get_y(p); }
    void set( Point& p, const FT &x, const FT &y) const { p=Point(x,y); }
};

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_DATA_ACCESSOR_2_H
