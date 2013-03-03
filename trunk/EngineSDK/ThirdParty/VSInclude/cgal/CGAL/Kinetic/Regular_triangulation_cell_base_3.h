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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Regular_triangulation_cell_base_3.h $
// $Id: Regular_triangulation_cell_base_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_KINETIC_REGULAR_CELL_BASE_3_H
#define CGAL_KINETIC_KINETIC_REGULAR_CELL_BASE_3_H

#include <CGAL/Kinetic/basic.h>
#include <CGAL/Kinetic/Delaunay_triangulation_cell_base_3.h>

CGAL_KINETIC_BEGIN_NAMESPACE
//! A class to track labels of edges of faces in a triangulation
template <class SimulationTraits, class Cell_base= CGAL::Triangulation_cell_base_3<typename SimulationTraits::Instantaneous_kernel> >
class Regular_triangulation_cell_base_3: public Delaunay_triangulation_cell_base_3<SimulationTraits, Cell_base>
{
    private:
        typedef typename Cell_base::Triangulation_data_structure   TDS;
    public:
        typedef TDS                            Triangulation_data_structure;
        typedef typename TDS::Cell_handle     Cell_handle;
        typedef typename TDS::Vertex_handle   Vertex_handle;
        typedef typename Cell_base::Geom_traits Traits;

        typedef typename SimulationTraits::Simulator::Event_key Edge_label;
        typedef Edge_label Facet_label;
        Regular_triangulation_cell_base_3(): Cell_base() {
        }

        Regular_triangulation_cell_base_3(Vertex_handle v0, Vertex_handle v1,
        Vertex_handle v2, Vertex_handle v3): Cell_base(v0, v1, v2, v3) {
        }

        Regular_triangulation_cell_base_3(Vertex_handle v0, Vertex_handle v1,
            Vertex_handle v2, Vertex_handle v3,
            Cell_handle f0, Cell_handle f1,
        Cell_handle f2, Cell_handle f3): Cell_base(v0,v1,v2, v3, f0,f1,f2, f3) {
        }

        template < typename TDS3 >
            struct Rebind_TDS
        {
            typedef typename Cell_base::template Rebind_TDS<TDS3>::Other Cb3;
            typedef Delaunay_triangulation_cell_base_3<SimulationTraits, Cb3>  Other;
        };
};

CGAL_KINETIC_END_NAMESPACE
#endif
