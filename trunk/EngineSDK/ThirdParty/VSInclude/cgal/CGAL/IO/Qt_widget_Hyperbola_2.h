// Copyright (c) 2003,2004  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Apollonius_graph_2/include/CGAL/IO/Qt_widget_Hyperbola_2.h $
// $Id: Qt_widget_Hyperbola_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>

#ifndef CGAL_QT_WIDGET_HYPERBOLA_2_H
#define CGAL_QT_WIDGET_HYPERBOLA_2_H

#if defined CGAL_USE_QT

#include <CGAL/Hyperbola_2.h>
#include <CGAL/Hyperbola_ray_2.h>
#include <CGAL/IO/Qt_widget.h>

CGAL_BEGIN_NAMESPACE

template< class Gt >
inline
Qt_widget& operator<<(Qt_widget& s, const Hyperbola_2< Gt > &H)
{
  H.draw_qt(s);
  return s;
}

template< class Gt >
inline
Qt_widget& operator<<(Qt_widget &s, Hyperbola_ray_2<Gt> &H)
{
  H.draw_qt(s);
  return s;
}

CGAL_END_NAMESPACE

#endif


#endif // CGAL_QT_WIDGET_HYPERBOLA_2_H
