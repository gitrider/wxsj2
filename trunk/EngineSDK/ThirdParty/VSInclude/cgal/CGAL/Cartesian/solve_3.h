// Copyright (c) 2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Cartesian_kernel/include/CGAL/Cartesian/solve_3.h $
// $Id: solve_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Andreas Fabri

#ifndef CGAL_CARTESIAN_CARTESIAN_SOLVE_3_H
#define CGAL_CARTESIAN_CARTESIAN_SOLVE_3_H

#include <CGAL/Kernel/solve.h>
#include <CGAL/Cartesian/Vector_3.h>

CGAL_BEGIN_NAMESPACE

template <class R>
void solve (const VectorC3<R> &v0,
            const VectorC3<R> &v1,
            const VectorC3<R> &v2,
            const VectorC3<R> &d,
            typename R::FT &alpha, typename R::FT &beta, typename R::FT &gamma)
{
  solve(v0.x(), v0.y(), v0.z(),
        v1.x(), v1.y(), v1.z(),
        v2.x(), v2.y(), v2.z(),
        d.x(),  d.y(),  d.z(),
        alpha, beta, gamma);
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_CARTESIAN_SOLVE_3_H
