// Copyright (c) 1997-2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Qt_widget/include/CGAL/IO/Qt_widget_Min_ellipse_2.h $
// $Id: Qt_widget_Min_ellipse_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Radu Ursu


#ifndef CGAL_QT_WIDGET_MIN_ELLIPSE_2_H
#define CGAL_QT_WIDGET_MIN_ELLIPSE_2_H

#include <CGAL/IO/Qt_widget.h>
#include <CGAL/Min_ellipse_2.h>

namespace CGAL{

template< class Traits_ >
Qt_widget&
operator<<(Qt_widget &ws,
              const CGAL::Min_ellipse_2<Traits_>& min_ellipse)
{
    typedef CGAL::Min_ellipse_2<Traits_>::Point_iterator  Point_iterator;

    Point_iterator  first( min_ellipse.points_begin());
    Point_iterator  last ( min_ellipse.points_end());
    for ( ; first != last; ++first)
        ws << *first;
    return ws;
}

}//end namespace CGAL

#endif
