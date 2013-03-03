// Copyright (c) 2005  INRIA (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Surface_mesh_parameterization/include/CGAL/parameterize.h $
// $Id: parameterize.h 29081 2006-03-06 13:32:35Z lsaboret $
//
//
// Author(s)     : Laurent Saboret, Pierre Alliez, Bruno Levy

//  We need a doxygen comment about CGAL in order for doxygen to
//  extract the global functions, variables or constants of the namespace
/// @namespace CGAL
/// CGAL is a great stuff!


#ifndef CGAL_PARAMETERIZE_H
#define CGAL_PARAMETERIZE_H

#include <CGAL/Mean_value_coordinates_parameterizer_3.h>

CGAL_BEGIN_NAMESPACE


/// Compute a one-to-one mapping from a 3D triangle surface 'mesh' to a
/// 2D circle, using Floater Mean Value Coordinates algorithm.
/// A one-to-one mapping is guaranteed.
///
/// The mapping is piecewise linear on the input mesh triangles.
/// The result is a (u,v) pair of parameter coordinates for each vertex of the input mesh.
///
/// Preconditions:
/// - 'mesh' must be a surface with one connected component.
/// - 'mesh' must be a triangular mesh.
///
template <class ParameterizationMesh_3>
typename Parameterizer_traits_3<ParameterizationMesh_3>::Error_code
parameterize(ParameterizationMesh_3& mesh)  ///< 3D mesh, model of ParameterizationMesh_3 concept
{
    Mean_value_coordinates_parameterizer_3<ParameterizationMesh_3> parameterizer;
    return parameterizer.parameterize(mesh);
}


/// Compute a one-to-one mapping from a 3D triangle surface 'mesh' to a
/// simple 2D domain.
/// The mapping is piecewise linear on the triangle mesh.
/// The result is a pair (u,v) of parameter coordinates for each vertex of the input mesh.
///
/// One-to-one mapping may be guaranteed or
/// not, depending on the chosen ParametizerTraits_3 algorithm.
///
/// Preconditions:
/// - 'mesh' must be a surface with one connected component.
/// - 'mesh' must be a triangular mesh.
/// - the mesh border must be mapped onto a convex polygon
/// (for fixed border parameterizations).
///
template <class ParameterizationMesh_3, class ParameterizerTraits_3>
typename Parameterizer_traits_3<ParameterizationMesh_3>::Error_code
parameterize(ParameterizationMesh_3& mesh,          ///< 3D mesh, model of ParameterizationMesh_3
             ParameterizerTraits_3 parameterizer)   ///< Parameterization method for 'mesh'
{
    return parameterizer.parameterize(mesh);
}


CGAL_END_NAMESPACE

#endif //CGAL_PARAMETERIZE_H

