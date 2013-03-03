// Copyright (c) 1997-2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Nef_3/include/CGAL/IO/Nef_polyhedron_iostream_3.h $
// $Id: Nef_polyhedron_iostream_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Peter Hachenberger <hachenberger@mpi-sb.mpg.de>

#ifndef CGAL_NEF_POLYHEDRON_IOSTREAM_3_H
#define CGAL_NEF_POLYHEDRON_IOSTREAM_3_H

#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Nef_3/SNC_io_parser.h>

CGAL_BEGIN_NAMESPACE

template <typename Kernel, typename Items, typename Mark>
std::ostream& operator<<
 (std::ostream& os, Nef_polyhedron_3<Kernel,Items,Mark>& NP)
{
  typedef typename Nef_polyhedron_3<Kernel,Items, Mark>::SNC_structure SNC_structure;
  CGAL::SNC_io_parser<SNC_structure> O(os, NP.snc(),true);
  O.print();
  return os;
}

template <typename Kernel, typename Items, typename Mark>
std::istream& operator>>
  (std::istream& is, Nef_polyhedron_3<Kernel,Items, Mark>& NP)
{
  typedef typename Nef_polyhedron_3<Kernel,Items,Mark>::SNC_structure SNC_structure;
  CGAL::SNC_io_parser<SNC_structure> I(is, NP.snc());
  I.read();
  NP.pl()->initialize(&NP.snc());
  return is;
}

CGAL_END_NAMESPACE

#endif //CGAL_NEF_POLYHEDRON_IOSTREAM_3_H
