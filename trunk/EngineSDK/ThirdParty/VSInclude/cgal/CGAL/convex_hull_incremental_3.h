// Copyright (c) 2001  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_3/include/CGAL/convex_hull_incremental_3.h $
// $Id: convex_hull_incremental_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra <Stefan.Schirra@mpi-sb.mpg.de>
//               : Michael Seel <Michael.Seel@mpi-sb.mpg.de>

#ifndef CGAL_CONVEX_HULL_INCREMENTAL_3_H
#define CGAL_CONVEX_HULL_INCREMENTAL_3_H

#include <CGAL/Convex_hull_d.h>
#include <CGAL/Convex_hull_d_traits_3.h>
#include <CGAL/Convex_hull_d_to_polyhedron_3.h>
#include <CGAL/ch_assertions.h>

CGAL_BEGIN_NAMESPACE

template <class InputIterator, class Polyhedron>
void
convex_hull_incremental_3(InputIterator first, InputIterator beyond, 
                          Polyhedron& P, bool test_correctness = false)
{
  typedef typename Polyhedron::Traits       PolyTraits;
  typedef typename PolyTraits::Kernel       K;
  typedef Convex_hull_d_traits_3<K>         ChullTraits;
  typedef Convex_hull_d< ChullTraits >      ChullType;

  ChullType CH(3);
  for ( ; first != beyond ; ++first)  CH.insert(*first);
  if ( test_correctness ) CGAL_ch_assertion(CH.is_valid());
  CGAL::convex_hull_d_to_polyhedron_3(CH,P);
}

CGAL_END_NAMESPACE

#endif // CGAL_CONVEX_HULL_INCREMENTAL_3_H
