// Copyright (c) 1997  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Stream_support/include/CGAL/IO/generic_copy_OFF.h $
// $Id: generic_copy_OFF.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>

#ifndef CGAL_IO_GENERIC_COPY_OFF_H
#define CGAL_IO_GENERIC_COPY_OFF_H 1
#ifndef CGAL_BASIC_H
#include <CGAL/basic.h>
#endif
#ifndef CGAL_PROTECT_CSTDDEF
#include <cstddef>
#define CGAL_PROTECT_CSTDDEF
#endif
#ifndef CGAL_IO_FILE_HEADER_OFF_H
#include <CGAL/IO/File_header_OFF.h>
#endif // CGAL_IO_FILE_HEADER_OFF_H
#ifndef CGAL_IO_FILE_SCANNER_OFF_H
#include <CGAL/IO/File_scanner_OFF.h>
#endif // CGAL_IO_FILE_SCANNER_OFF_H
#ifndef CGAL_PROTECT_IOSTREAM
#include <iostream>
#define CGAL_PROTECT_IOSTREAM
#endif

CGAL_BEGIN_NAMESPACE

template <class Writer>
void
generic_copy_OFF( File_scanner_OFF& scanner,
                  std::ostream& out,
                  Writer& writer) {
    std::istream& in = scanner.in();
    // scans a polyhedral surface in OFF from `in' and writes it
    // to `out' in the format provided by `writer'.
    if ( ! in) {
        if ( scanner.verbose()) {
            std::cerr << " " << std::endl;
            std::cerr << "generic_copy_OFF(): "
                         "input error: file format is not in OFF."
                      << std::endl;
        }
        return;
    }

    // Print header. Number of halfedges is only trusted if it is
    // a polyhedral surface.
    writer.write_header( out,
                         scanner.size_of_vertices(),
                         scanner.polyhedral_surface() ?
                             scanner.size_of_halfedges() : 0,
                         scanner.size_of_facets());

    // read in all vertices
    double  x,  y,  z;  // Point coordinates.
    int  i;
    for ( i = 0; i < scanner.size_of_vertices(); i++) {
        scanner.scan_vertex( x, y, z);
        writer.write_vertex( x, y, z);
        scanner.skip_to_next_vertex( i);
    }

    // read in all facets
    writer.write_facet_header();
    for ( i = 0; i < scanner.size_of_facets(); i++) {
        if ( ! in)
            return;
        Integer32 no;
        scanner.scan_facet( no, i);
        writer.write_facet_begin( no);
        for ( int j = 0; j < no; j++) {
            Integer32 index;
            scanner.scan_facet_vertex_index( index, i);
            writer.write_facet_vertex_index( index);
        }
        writer.write_facet_end();
        scanner.skip_to_next_facet( i);
    }
    writer.write_footer();
}

template <class Writer>
void
generic_copy_OFF( std::istream& in, std::ostream& out, Writer& writer,
                  bool verbose = false) {
    // scans a polyhedral surface in OFF from `in' and writes it
    // to `out' in the format provided by `writer'.
    File_scanner_OFF scanner( in, verbose);
    generic_copy_OFF( scanner, out, writer);
}

CGAL_END_NAMESPACE
#endif // CGAL_IO_GENERIC_COPY_OFF_H //
// EOF //
