// Copyright (c) 1997,2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Stream_support/include/CGAL/IO/Scanner_OFF.h $
// $Id: Scanner_OFF.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>
//                 Ralf Osbild   <osbild@mpi-sb.mpg.de>

#ifndef CGAL_IO_SCANNER_OFF_H
#define CGAL_IO_SCANNER_OFF_H 1

#include <CGAL/basic.h>
#include <iterator>
#include <vector>
#include <utility>
#include <CGAL/IO/File_scanner_OFF.h>

CGAL_BEGIN_NAMESPACE

// The Facet_iterator's value type is vector<Integer32>
// that contains the vertex indices.

template <class Pt>
class I_Scanner_OFF_vertex_iterator
{
public:
    typedef std::input_iterator_tag  iterator_category;
    typedef Pt                       value_type;
    typedef std::ptrdiff_t           difference_type;
    typedef const Pt*                pointer;
    typedef const Pt&                reference;
private:
    File_scanner_OFF*  m_scan;
    std::ptrdiff_t     m_cnt;
    Pt                 m_point;

    void next_vertex() {
        CGAL_assertion( m_scan != NULL);
        if ( m_cnt < m_scan->size_of_vertices()) {
            file_scan_vertex( *m_scan, m_point);
            m_scan->skip_to_next_vertex( m_cnt);
            ++m_cnt;
        } else
            m_cnt = m_scan->size_of_vertices() + 1;
    }
public:
    typedef Pt                                 Point;
    typedef File_scanner_OFF                   Scanner;
    typedef I_Scanner_OFF_vertex_iterator<Pt>  Self;

    I_Scanner_OFF_vertex_iterator( int cnt) : m_scan(0), m_cnt(cnt+1) {}
    I_Scanner_OFF_vertex_iterator( Scanner& s, int cnt)
        : m_scan(&s), m_cnt(cnt)
    {
        next_vertex();
    }
    std::ptrdiff_t  count()           const { return m_cnt; }
    bool   operator==( const Self& i) const { return m_cnt == i.m_cnt; }
    bool   operator!=( const Self& i) const { return m_cnt != i.m_cnt; }
    Self&  operator++() {
        next_vertex();
        return *this;
    }
    Self   operator++(int) {
        Self tmp = *this;
        ++(*this);
        return tmp;
    }
    const Point& operator*()  const {
        CGAL_assertion( m_scan != NULL);
        return m_point;
    }
    const Point* operator->() const { return & operator*(); }
};

template <class Pt, class Nrm>
class I_Scanner_OFF_vertex_and_normals_iterator
{
public:
    typedef Pt                                              Point;
    typedef Nrm                                             Normal;
    typedef File_scanner_OFF                                Scanner;
    typedef I_Scanner_OFF_vertex_and_normals_iterator<Pt,Nrm> Self;

    typedef std::input_iterator_tag                         iterator_category;
    typedef std::pair<Point,Normal>                         value_type;
    typedef std::ptrdiff_t                                  difference_type;
    typedef const value_type*                               pointer;
    typedef const value_type&                               reference;
private:
    File_scanner_OFF*  m_scan;
    std::ptrdiff_t     m_cnt;
    value_type         m_current;
    

    void next() {
        CGAL_assertion( m_scan != NULL);
        if ( m_cnt < m_scan->size_of_vertices()) {
            file_scan_vertex( *m_scan, m_current.first);
            if ( m_scan->has_normals())
                file_scan_normal( *m_scan, m_current.second);
            m_scan->skip_to_next_vertex( m_cnt);
            ++m_cnt;
        } else
            m_cnt = m_scan->size_of_vertices() + 1;
    }
public:

    I_Scanner_OFF_vertex_and_normals_iterator( int cnt)
        : m_scan(0), m_cnt(cnt+1) {}
    I_Scanner_OFF_vertex_and_normals_iterator( Scanner& s, int cnt)
        : m_scan(&s), m_cnt(cnt)
    {
        next();
    }
    std::ptrdiff_t  count()           const { return m_cnt; }
    bool   operator==( const Self& i) const { return m_cnt == i.m_cnt; }
    bool   operator!=( const Self& i) const { return m_cnt != i.m_cnt; }
    Self&  operator++() {
        next();
        return *this;
    }
    Self   operator++(int) {
        Self tmp = *this;
        ++(*this);
        return tmp;
    }
    reference operator*()  const {
        CGAL_assertion( m_scan != NULL);
        return m_current;
    }
    pointer   operator->() const { return & operator*(); }
};

class I_Scanner_OFF_facet_iterator
{
public:
    typedef std::input_iterator_tag  iterator_category;
    typedef std::vector< Integer32>  value_type;
    typedef std::ptrdiff_t           difference_type;
    typedef value_type*              pointer;
    typedef value_type&              reference;
private:
    File_scanner_OFF*  m_scan;
    std::ptrdiff_t     m_cnt;
    value_type         m_indices;

    void next_facet() {
        CGAL_assertion( m_scan != NULL);
        if ( m_cnt < m_scan->size_of_facets()) {
            m_indices.erase( m_indices.begin(), m_indices.end());
            Integer32 no;
            m_scan->scan_facet( no, m_cnt);
            m_indices.reserve( no);
            Integer32 index = -1;
            for ( Integer32 i = 0; i < no; ++i) {
                m_scan->scan_facet_vertex_index( index, m_cnt);
                m_indices.push_back( index);
            }
            m_scan->skip_to_next_facet( m_cnt);
            ++ m_cnt;
        } else
            m_cnt = m_scan->size_of_facets() + 1;
    }
public:
    value_type::size_type size_of_indices () const // RO
       { return m_indices.size(); }
    typedef value_type::size_type	  indices_size_type; // RO
public:
    typedef File_scanner_OFF              Scanner;
    typedef I_Scanner_OFF_facet_iterator  Self;
    typedef value_type::iterator          iterator;

    I_Scanner_OFF_facet_iterator( int cnt) : m_scan(0), m_cnt(cnt+1) {}
    I_Scanner_OFF_facet_iterator( Scanner& s, int cnt)
        : m_scan(&s), m_cnt(cnt)
    {
        next_facet();
    }
    std::ptrdiff_t  count()          const { return m_cnt; }
    bool  operator==( const Self& i) const { return m_cnt == i.m_cnt; }
    bool  operator!=( const Self& i) const { return m_cnt != i.m_cnt; }
    Self& operator++() {
        next_facet();
        return *this;
    }
    Self  operator++(int) {
        Self tmp = *this;
        ++(*this);
        return tmp;
    }
    value_type&       operator*()        {
        CGAL_assertion( m_scan != NULL);
        return m_indices;
    }
    const value_type& operator*()  const {
        CGAL_assertion( m_scan != NULL);
        return m_indices;
    }
    value_type*       operator->()       { return & operator*(); }
    const value_type* operator->() const { return & operator*(); }
};


// The distance function is implemented to work in
// constant time for both iterators.

template <class Pt, class Distance> inline
void distance( const I_Scanner_OFF_vertex_iterator<Pt>& first,
               const I_Scanner_OFF_vertex_iterator<Pt>& last,
               Distance& n) {
    n = Distance( last.count() - first.count());
}
template <class Distance> inline
void distance( const I_Scanner_OFF_facet_iterator& first,
               const I_Scanner_OFF_facet_iterator& last,
               Distance& n) {
    n = Distance( last.count() - first.count());
}
template <class Pt> inline
std::ptrdiff_t distance( const I_Scanner_OFF_vertex_iterator<Pt>& first,
                         const I_Scanner_OFF_vertex_iterator<Pt>& last) {
    return last.count() - first.count();
}

inline
std::ptrdiff_t  distance( const I_Scanner_OFF_facet_iterator& first,
                          const I_Scanner_OFF_facet_iterator& last) {
    return last.count() - first.count();
}


template <class Kernel>
class Scanner_OFF {
    File_scanner_OFF  m_scan;
public:
    typedef typename Kernel::Point_3               Point;
    typedef Point                                  Pt;
    typedef typename Kernel::Vector_3              Normal;
    typedef Scanner_OFF<Kernel>                    Self;
    typedef I_Scanner_OFF_vertex_iterator<Pt>      Vertex_iterator;
    typedef I_Scanner_OFF_vertex_and_normals_iterator<Pt,Normal>
                                                   Vertex_and_normals_iterator;
    typedef I_Scanner_OFF_facet_iterator           Facet_iterator;
    typedef I_Scanner_OFF_facet_iterator::iterator Index_iterator;

    Scanner_OFF( std::istream& in, bool verbose = false)
        : m_scan( in, verbose) {}
    Scanner_OFF( std::istream& in, const File_header_OFF& header)
        : m_scan( in, header) {}

    int  size_of_vertices()   const { return m_scan.size_of_vertices(); }
    int  size_of_halfedges()  const { return m_scan.size_of_halfedges();}
    int  size_of_facets()     const { return m_scan.size_of_facets();   }

    bool verbose()            const { return m_scan.verbose();          }
    bool skel()               const { return m_scan.skel();             }
    bool off()                const { return m_scan.off();              }
    bool binary()             const { return m_scan.binary();           }
    bool ascii()              const { return m_scan.ascii();            }

    bool has_colors()         const { return m_scan.has_colors();       }
    bool has_normals()        const { return m_scan.has_normals();      }

    File_header_OFF& header()       { return m_scan;                    }
    const File_header_OFF&
         header()             const { return m_scan;                    }

    Vertex_iterator vertices_begin(){ return Vertex_iterator( m_scan,0);}
    Vertex_iterator vertices_end()  {
        return Vertex_iterator( size_of_vertices());
    }
    Facet_iterator facets_begin()   { return Facet_iterator( m_scan,0); }
    Facet_iterator facets_end()     {
        return Facet_iterator( size_of_facets());
    }
    Vertex_and_normals_iterator vertices_and_normals_begin(){
        return Vertex_and_normals_iterator( m_scan,0);
    }
    Vertex_and_normals_iterator vertices_and_normals_end()  {
        return Vertex_and_normals_iterator( size_of_vertices());
    }
};

CGAL_END_NAMESPACE
#endif // CGAL_IO_SCANNER_OFF_H //
// EOF //
