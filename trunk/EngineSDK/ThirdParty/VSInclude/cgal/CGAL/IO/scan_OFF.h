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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Polyhedron_IO/include/CGAL/IO/scan_OFF.h $
// $Id: scan_OFF.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>

#ifndef CGAL_IO_SCAN_OFF_H
#define CGAL_IO_SCAN_OFF_H 1

#include <CGAL/IO/Polyhedron_scan_OFF.h>
#include <CGAL/Polyhedron_3.h>
#include <iostream>

CGAL_BEGIN_NAMESPACE

template < class Traits,
           class Items,
#ifndef CGAL_CFG_NO_TMPL_IN_TMPL_PARAM
           template < class T, class I, class A>
#endif
           class HDS, class Alloc>
void scan_OFF( std::istream& in,
               Polyhedron_3<Traits,Items,HDS,Alloc>& P,
               File_header_OFF& header) {
    // reads a polyhedron from `in' and appends it to P.
    // Returns also the File_header_OFF structure of the object.
    typedef Polyhedron_3<Traits,Items,HDS,Alloc> Polyhedron;
    typedef typename Polyhedron::HalfedgeDS HalfedgeDS;
    typedef Polyhedron_scan_OFF<HalfedgeDS> Scanner;
    Scanner scanner( in, header.verbose());
    P.delegate(scanner);
    header = scanner.header();
}

template < class Traits,
           class Items,
#ifndef CGAL_CFG_NO_TMPL_IN_TMPL_PARAM
           template < class T, class I, class A>
#endif
           class HDS, class Alloc>
void scan_OFF( std::istream& in, Polyhedron_3<Traits,Items,HDS,Alloc>& P,
               bool verbose = false) {
    // reads a polyhedron from `in' and appends it to P.
    typedef Polyhedron_3<Traits,Items,HDS,Alloc> Polyhedron;
    typedef typename Polyhedron::HalfedgeDS HalfedgeDS;
    typedef Polyhedron_scan_OFF<HalfedgeDS> Scanner;
    Scanner scanner( in, verbose);
    P.delegate(scanner);
}


CGAL_END_NAMESPACE
#endif // CGAL_IO_SCAN_OFF_H //
// EOF //
