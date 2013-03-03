// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Delaunay_triangulation_visitor_base_3.h $
// $Id: Delaunay_triangulation_visitor_base_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_DELAUNAY_TRIANGULATION_3_WATCHER_BASE_H
#define CGAL_KINETIC_DELAUNAY_TRIANGULATION_3_WATCHER_BASE_H
#include <CGAL/Kinetic/basic.h>

CGAL_KINETIC_BEGIN_NAMESPACE

struct Delaunay_triangulation_visitor_base_3
{
//typedef Tr Triangulation;
    Delaunay_triangulation_visitor_base_3(){}

    template <class Vertex_handle>
    void remove_vertex(Vertex_handle) {
    }

    template <class Vertex_handle>
    void create_vertex(Vertex_handle) {
    }

    template <class Vertex_handle>
    void modify_vertex(Vertex_handle) {
    }

    template <class It>
    void create_cells(It, It) {
    }

    template <class It>
    void remove_cells(It, It) {
    }

    template <class Edge>
    void before_edge_flip(Edge) {

    }
    template <class Edge>
    void after_facet_flip(Edge) {

    }

    template <class Facet>
    void before_facet_flip(Facet) {

    }

    template <class Facet>
    void after_edge_flip(Facet) {
    }
};

CGAL_KINETIC_END_NAMESPACE
#endif
