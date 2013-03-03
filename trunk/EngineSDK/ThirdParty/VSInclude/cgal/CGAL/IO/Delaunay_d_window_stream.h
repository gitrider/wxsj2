// Copyright (c) 1997-2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_d/include/CGAL/IO/Delaunay_d_window_stream.h $
// $Id: Delaunay_d_window_stream.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>

#ifndef CGAL_DELAUNAY_D_WINDOW_STREAM_H
#define CGAL_DELAUNAY_D_WINDOW_STREAM_H

#include <CGAL/LEDA_basic.h>
#include <CGAL/Delaunay_d.h>
#include <CGAL/IO/Convex_hull_d_window_stream.h>
#include <CGAL/IO/Window_stream.h>

CGAL_BEGIN_NAMESPACE

/*{\Mtext \headerline{Low Dimensional Output Routines}
include |<CGAL/IO/Delaunay_d_window_stream.h>|
\setopdims{2cm}{1cm}}*/

template <typename R, typename Lifted_R>
void d2_show(const Delaunay_d<R,Lifted_R>& D,
             CGAL::Window_stream& W, 
             typename Delaunay_d<R,Lifted_R>::Delaunay_voronoi_kind k = 
             Delaunay_d<R,Lifted_R>::NEAREST)
/*{\Mfunc draws the underlying simplicial complex |D| into window |W|.\\
\precond |dim == 2|. }*/
{ 
  CGAL_assertion_msg(D.dimension() == 2, "d2_map: dim != 2.");
  Regular_complex_d<R> RC(2);
  D.project(RC, (k == Delaunay_d<R,Lifted_R>::NEAREST ? -1 : +1));
  CGAL::d2_show(RC,W);
}

template <typename R, typename Lifted_R>
void d2_map(const Delaunay_d<R,Lifted_R>& D, 
            CGAL_LEDA_SCOPE::GRAPH< typename Delaunay_d<R,Lifted_R>::Point_d, 
                                    int >& DTG, 
            typename Delaunay_d<R,Lifted_R>::Delaunay_voronoi_kind k = 
            Delaunay_d<R,Lifted_R>::NEAREST)
/*{\Mfunc constructs a LEDA graph representation of the nearest 
(|kind = NEAREST| or the furthest (|kind = FURTHEST|) site
Delaunay triangulation.\\ \precond |dim() == 2|. }*/
{ 
  CGAL_assertion_msg(D.dimension() == 2, "d2_map: dim != 2.");
  Regular_complex_d<R> RC(2);
  D.project(RC, (k == NEAREST ? -1 : +1));
  d2_map(RC,DTG);
}


CGAL_END_NAMESPACE
#endif //CGAL_DELAUNAY_D_WINDOW_STREAM_H


