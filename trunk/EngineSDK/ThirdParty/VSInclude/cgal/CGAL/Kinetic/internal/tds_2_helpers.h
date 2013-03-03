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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/internal/tds_2_helpers.h $
// $Id: tds_2_helpers.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_TRIANGULATION_DATA_STRUCTURE_HELPER_2_H
#define CGAL_TRIANGULATION_DATA_STRUCTURE_HELPER_2_H
#include <CGAL/Kinetic/basic.h>
#include <utility>
CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE

template <class TDS>
struct Triangulation_data_structure_helper_2
{
    typedef typename TDS::Edge Edge;
    typedef typename TDS::Vertex_handle Vertex_handle;
    typedef typename TDS::Face Face;
    typedef typename Face::Edge_label Edge_label;

    static Vertex_handle origin(const Edge &e) {
        int o= e.first->ccw(e.second);
        return e.first->vertex(o);
    }

    static Vertex_handle destination(const Edge &e) {
        int o= e.first->cw(e.second);
        return e.first->vertex(o);
    }

    static Edge_label get_directed_edge_label(const Edge &e) {
        return e.first->get_edge_label(e.second);
    }

    static Edge_label get_undirected_edge_label(const Edge &e) {
        if (get_directed_edge_label(e) != get_directed_edge_label(mirror_edge(e))) {
            std::cerr << get_directed_edge_label(e) << " "
                <<  get_directed_edge_label(mirror_edge(e)) << std::endl;
            CGAL_precondition(get_directed_edge_label(e) == get_directed_edge_label(mirror_edge(e)));
        }
        return e.first->get_edge_label(e.second);
    }

    static Edge mirror_edge(const Edge &e) {
        int i= e.first->mirror_index(e.second);
        return Edge(e.first->neighbor(e.second), i);
    }

    static void set_directed_edge_label(const Edge &e, const Edge_label& l) {
        e.first->set_edge_label(e.second, l);
    }

    static void set_undirected_edge_label(const Edge &e, const Edge_label& l) {
        set_directed_edge_label(e,l);
        set_directed_edge_label(mirror_edge(e),l);
    }

    static Vertex_handle third_vertex(const Edge &e) {
        return e.first->vertex(e.second);
    }

    static Vertex_handle mirror_vertex(const Edge &e) {
        return third_vertex(mirror_edge(e));
    }
};

CGAL_KINETIC_END_INTERNAL_NAMESPACE
#endif
