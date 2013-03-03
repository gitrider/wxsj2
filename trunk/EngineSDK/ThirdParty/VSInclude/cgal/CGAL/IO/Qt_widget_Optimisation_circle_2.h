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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Qt_widget/include/CGAL/IO/Qt_widget_Optimisation_circle_2.h $
// $Id: Qt_widget_Optimisation_circle_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Radu Ursu

#ifndef CGAL_QT_WIDGET_OPTIMISATION_CIRCLE_2_H
#define CGAL_QT_WIDGET_OPTIMISATION_CIRCLE_2_H

#include <CGAL/IO/Qt_widget.h>

namespace CGAL{
template<class Traits>
Qt_widget&
operator << (Qt_widget &ws,
	     const CGAL::Optimisation_circle_2<Traits>& oc){
  typedef typename Traits::Point_2  Point_2;
  typedef typename Traits::Circle_2 Circle_2;

  double cx( CGAL::to_double( oc.center().x()));
  double cy( CGAL::to_double( oc.center().y()));
  double sr( CGAL::to_double( oc.squared_radius()));

  if( ! CGAL_NTS is_negative(sr))
    ws << Circle_2( Point_2(cx, cy), sr);
  return ws;
}

}//end namespace

#endif
