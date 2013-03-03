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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Regular_triangulation_inexact_simulation_traits_3.h $
// $Id: Regular_triangulation_inexact_simulation_traits_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_WEIGHTED_EXACT_SIMULATION_3_H
#define CGAL_KINETIC_WEIGHTED_EXACT_SIMULATION_3_H
#include <CGAL/Kinetic/Regular_triangulation_instantaneous_traits_3.h>
#include <CGAL/Kinetic/Simulation_traits.h>
#include <CGAL/Regular_triangulation_euclidean_traits_3.h>

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE
struct Rist3_types: public Sist_types
{
    typedef CGAL::Regular_triangulation_euclidean_traits_3< Sist_types::Static_kernel> Static_kernel;
    typedef Sist_types::Kinetic_kernel::Weighted_point_3 Active_object;
    typedef Active_objects_vector<Active_object> Active_objects_table;
    typedef Regular_triangulation_instantaneous_traits_3< Active_objects_table,
        Static_kernel> Instantaneous_kernel;
};

CGAL_KINETIC_END_INTERNAL_NAMESPACE

CGAL_KINETIC_BEGIN_NAMESPACE

struct Regular_triangulation_inexact_simulation_traits_3:
public  Simulation_traits<internal::Rist3_types::Static_kernel,
internal::Rist3_types::Instantaneous_kernel,
internal::Rist3_types::Kinetic_kernel,
internal::Rist3_types::Simulator,
internal::Rist3_types::Active_objects_table >
{
    typedef Simulation_traits<internal::Rist3_types::Static_kernel,
        internal::Rist3_types::Instantaneous_kernel,
        internal::Rist3_types::Kinetic_kernel,
        internal::Rist3_types::Simulator,
        internal::Rist3_types::Active_objects_table > P;
    Regular_triangulation_inexact_simulation_traits_3(const P::Time lb=0,
        const P::Time ub=10000): P(lb, ub){}
};
CGAL_KINETIC_END_NAMESPACE
#endif
