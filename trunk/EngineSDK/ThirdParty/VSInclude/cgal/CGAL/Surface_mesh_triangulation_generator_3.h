// Copyright (c) 2006  INRIA Sophia-Antipolis (France).
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
// $Source: 
// $Revision: 30642 $ $Date: 2006-04-18 14:42:52 +0200 (Tue, 18 Apr 2006) $
// $Name:  $
//
// Author(s)     : Laurent Rineau

#ifndef CGAL_SURFACE_MESH_TRIANGULATION_GENERATOR_3_H
#define CGAL_SURFACE_MESH_TRIANGULATION_GENERATOR_3_H

#ifndef CGAL_SURFACE_MESHER_TEST
#  warning This header is not documented.
#endif

namespace CGAL {

template <class Kernel>
class Surface_mesh_triangulation_generator_3
{
  typedef CGAL::Surface_mesh_vertex_base_3<Kernel> Vb;
  typedef CGAL::Surface_mesh_cell_base_3<Kernel> Cb;
  typedef CGAL::Triangulation_data_structure_3<Vb, Cb> Tds;
public:
  typedef CGAL::Delaunay_triangulation_3<Kernel, Tds> Type;
  typedef Type type; // Boost meta-programming compatibility
};

} // end namespace CGAL

#endif // CGAL_SURFACE_MESH_TRIANGULATION_GENERATOR_3_H
