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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Surface_mesher/include/CGAL/Surface_mesh_complex_2_in_triangulation_3.h $
// $Id: Surface_mesh_complex_2_in_triangulation_3.h 30642 2006-04-18 12:42:52Z lsaboret $
//
//
// Author(s)     : Laurent Rineau

#ifndef CGAL_SURFACE_MESH_COMPLEX_2_IN_TRIANGULATION_3_H
#define CGAL_SURFACE_MESH_COMPLEX_2_IN_TRIANGULATION_3_H

#include <CGAL/Complex_2_in_triangulation_3.h>

namespace CGAL {

/**  Simple forward class for CGAL::Complex_2_in_triangulation_3<Tr>.
 *
 *   CGAL::Surface_mesher_complex_2_in_triangulation_3<Tr> is an alias
 *   for CGAL::Complex_2_in_triangulation_3<Tr>.
 */
template <class Tr>
class Surface_mesh_complex_2_in_triangulation_3 :
    public Complex_2_in_triangulation_3<Tr>
{
public:
  Surface_mesh_complex_2_in_triangulation_3 (Tr& tr) 
    : Complex_2_in_triangulation_3<Tr>(tr)
  {
  }
}; // end Surface_mesh_complex_2_in_triangulation_3

} // end namespace CGAL

#endif // CGAL_SURFACE_MESH_COMPLEX_2_IN_TRIANGULATION_3_H
