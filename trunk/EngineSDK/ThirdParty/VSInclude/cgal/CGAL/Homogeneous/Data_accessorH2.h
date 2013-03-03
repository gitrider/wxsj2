// Copyright (c) 1999,2001  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Homogeneous_kernel/include/CGAL/Homogeneous/Data_accessorH2.h $
// $Id: Data_accessorH2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra
 

#ifndef CGAL_DATA_ACCESSORH2_H
#define CGAL_DATA_ACCESSORH2_H

CGAL_BEGIN_NAMESPACE

template < class R >
class Data_accessorH2
{
public:
    typedef typename R::FT FT;
    typedef typename R::RT RT;
    typedef typename R::Point_2      Point_2;

    RT  get_hx( const Point_2 & p) const { return( p.hx()); }
    RT  get_hy( const Point_2 & p) const { return( p.hy()); }
    RT  get_hw( const Point_2 & p) const { return( p.hw()); }

    void
    get( const Point_2 & p, RT& hx, RT& hy, RT& hw) const
    {
        hx = get_hx( p);
        hy = get_hy( p);
        hw = get_hw( p);
    }

    void
    set( Point_2& p, const RT & hx, const RT & hy, const RT & hw) const
    {
        p = Point_2( hx, hy, hw);
    }
};

CGAL_END_NAMESPACE

#endif // CGAL_DATA_ACCESSORH2_H
