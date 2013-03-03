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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Stream_support/include/CGAL/IO/Ostream_iterator.h $
// $Id: Ostream_iterator.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@inf.ethz.ch>

#ifndef CGAL_IO_OSTREAM_ITERATOR_H
#define CGAL_IO_OSTREAM_ITERATOR_H

#include <CGAL/circulator.h>

CGAL_BEGIN_NAMESPACE

// This proxy is for the Ostream_iterator.
template <class T, class Stream>
class Ostream_proxy {
    Stream& stream;
public:
    Ostream_proxy( Stream& s) : stream(s) {}
    Ostream_proxy<T,Stream>&  operator=( const T& t) {
        stream << t;
        return *this;
    }
};

template <class T, class Stream>
class Ostream_iterator {
    Stream& stream;
public:
    typedef  T                         value_type;
    typedef  T&                        reference;
    typedef  const T&                  const_reference;
    typedef  T*                        pointer;
    typedef  const T*                  const_pointer;
    typedef  std::ptrdiff_t            difference_type;
    typedef  std::output_iterator_tag  iterator_category;

    Ostream_iterator( Stream& s) : stream(s) {}
    Ostream_iterator<T,Stream>& operator++()      { return *this;}
    Ostream_iterator<T,Stream>  operator++(int)   { return *this;}
    Ostream_proxy<T,Stream>     operator*() const {
        return Ostream_proxy<T,Stream>(stream);
    }
};

CGAL_END_NAMESPACE

#endif // CGAL_IO_OSTREAM_ITERATOR_H
