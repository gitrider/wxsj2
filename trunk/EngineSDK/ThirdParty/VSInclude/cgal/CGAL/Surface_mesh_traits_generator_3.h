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

#ifndef CGAL_SURFACE_MESH_TRAITS_GENERATOR_3_H
#define CGAL_SURFACE_MESH_TRAITS_GENERATOR_3_H

#include <CGAL/Surface_mesher/Oracles/Sphere_oracle_3.h>

namespace CGAL {

/** Defaut traits class.
 *  Partial specialization will be in other headers
*/
template <typename Surface>
struct Surface_mesh_traits_generator_3 
{
  typedef typename Surface::Surface_mesher_traits_3 Type;
  typedef Type type; // for Boost compatiblity (meta-programming)
};

  // specialization for Kernel::Sphere_3
template <typename Kernel>
struct Surface_mesh_traits_generator_3<CGAL::Sphere_3<Kernel> >
{
  typedef Surface_mesher::Sphere_oracle_3<Kernel> Type;
  typedef Type type; // for Boost compatiblity (meta-programming)
};

} // end namespace CGAL

#endif // CGAL_SURFACE_MESH_TRAITS_GENERATOR_3_H
