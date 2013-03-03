// Copyright (c) 1997  ETH Zurich (Switzerland).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Polyhedron_IO/include/CGAL/IO/print_VRML_2.h $
// $Id: print_VRML_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>

#ifndef CGAL_IO_PRINT_VRML_2_H
#define CGAL_IO_PRINT_VRML_2_H 1

#include <CGAL/IO/Polyhedron_VRML_2_ostream.h>

CGAL_BEGIN_NAMESPACE

template <class Polyhedron>
void print_polyhedron_VRML_2( std::ostream& out, const Polyhedron& P) {
    VRML_2_ostream os( out);
    os << P;
}

// Deprecated global functions, replaced with functions above

template < class Traits,
           class Items,
#ifndef CGAL_CFG_NO_TMPL_IN_TMPL_PARAM
           template < class T, class I, class A>
#endif
           class HDS, class Alloc>
void
print_VRML_2( std::ostream& out,
              const Polyhedron_3<Traits,Items,HDS,Alloc>& P) {
    VRML_2_ostream os( out);
    os << P;
}

CGAL_END_NAMESPACE
#endif // CGAL_IO_PRINT_VRML_2_H //
// EOF //
