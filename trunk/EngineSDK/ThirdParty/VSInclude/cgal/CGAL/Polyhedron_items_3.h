// Copyright (c) 1997  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/HalfedgeDS/include/CGAL/Polyhedron_items_3.h $
// $Id: Polyhedron_items_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>

#ifndef CGAL_POLYHEDRON_ITEMS_3_H
#define CGAL_POLYHEDRON_ITEMS_3_H 1
#ifndef CGAL_HALFEDGEDS_VERTEX_BASE_H
#include <CGAL/HalfedgeDS_vertex_base.h>
#endif
#ifndef CGAL_HALFEDGEDS_HALFEDGE_BASE_H
#include <CGAL/HalfedgeDS_halfedge_base.h>
#endif
#ifndef CGAL_HALFEDGEDS_FACE_BASE_H
#include <CGAL/HalfedgeDS_face_base.h>
#endif

CGAL_BEGIN_NAMESPACE

class Polyhedron_items_3 {
public:
    template < class Refs, class Traits>
    struct Vertex_wrapper {
        typedef typename Traits::Point_3 Point;
        typedef HalfedgeDS_vertex_base< Refs, Tag_true, Point> Vertex;
    };
    template < class Refs, class Traits>
    struct Halfedge_wrapper {
        typedef HalfedgeDS_halfedge_base< Refs>                Halfedge;
    };
    template < class Refs, class Traits>
    struct Face_wrapper {
        typedef typename Traits::Plane_3 Plane;
        typedef HalfedgeDS_face_base< Refs, Tag_true, Plane>   Face;
    };
};

CGAL_END_NAMESPACE
#endif // CGAL_POLYHEDRON_ITEMS_3_H //
// EOF //
