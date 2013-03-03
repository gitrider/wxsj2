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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Stream_support/include/CGAL/IO/File_writer_wavefront.h $
// $Id: File_writer_wavefront.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>

#ifndef CGAL_IO_FILE_WRITER_WAVEFRONT_H
#define CGAL_IO_FILE_WRITER_WAVEFRONT_H 1
#ifndef CGAL_IO_BINARY_FILE_IO_H
#include <CGAL/IO/binary_file_io.h>
#endif // CGAL_IO_BINARY_FILE_IO_H
#ifndef CGAL_PROTECT_IOSTREAM
#include <iostream>
#define CGAL_PROTECT_IOSTREAM
#endif
#ifndef CGAL_PROTECT_CSTDDEF
#include <cstddef>
#define CGAL_PROTECT_CSTDDEF
#endif

CGAL_BEGIN_NAMESPACE

class File_writer_wavefront {
    std::ostream*  m_out;
    std::size_t    m_facets;
public:
    std::ostream& out() const { return *m_out; }
    void write_header( std::ostream& out,
                       std::size_t vertices,
                       std::size_t halfedges,
                       std::size_t facets);
    void write_footer() const {
        out() << "\n# End of Wavefront obj format #" << std::endl;
    }
    void write_vertex( const double& x, const double& y, const double& z) {
        out() << "v " << x << ' ' << y << ' ' << z << '\n';
    }
    void write_facet_header() {
        out() << "\n# " << m_facets << " facets\n";
        out() << "# ------------------------------------------\n\n";
    }
    void write_facet_begin( std::size_t)            { out() << "f "; }
    void write_facet_vertex_index( std::size_t idx) { out() << ' ' << idx+1; }
    void write_facet_end()                          { out() << '\n'; }
};

CGAL_END_NAMESPACE
#endif // CGAL_IO_FILE_WRITER_WAVEFRONT_H //
// EOF //
