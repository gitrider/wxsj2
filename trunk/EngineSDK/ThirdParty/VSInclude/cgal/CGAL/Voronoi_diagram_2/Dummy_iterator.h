// Copyright (c) 2006 Foundation for Research and Technology-Hellas (Greece).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Voronoi_diagram_2/include/CGAL/Voronoi_diagram_2/Dummy_iterator.h $
// $Id: Dummy_iterator.h 29163 2006-03-07 23:41:02Z mkaravel $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@iacm.forth.gr>

#ifndef CGAL_VORONOI_DIAGRAM_2_DUMMY_ITERATOR_H
#define CGAL_VORONOI_DIAGRAM_2_DUMMY_ITERATOR_H 1

#include <CGAL/Voronoi_diagram_2/basic.h>
#include <CGAL/iterator.h>

CGAL_BEGIN_NAMESPACE

CGAL_VORONOI_DIAGRAM_2_BEGIN_NAMESPACE

template<class Value_t>
class Dummy_iterator : public Emptyset_iterator
{
 private:
  typedef Dummy_iterator<Value_t>  Self;

 public:
  typedef Value_t            value_type;
  typedef value_type&        reference;
  typedef value_type*        pointer;
  typedef const value_type&  const_reference;
  typedef const value_type*  const_pointer;
  typedef std::size_t        size_type;
  typedef std::ptrdiff_t     difference_type;

  Dummy_iterator() {}
  Dummy_iterator(const Dummy_iterator&) {}

  template< class T >
  Self& operator=(const T&) { return *this; }

  Self& operator++()        { return *this; }
  Self& operator++(int)     { return *this; }

  Self& operator--()        { return *this; }
  Self& operator--(int)     { return *this; }

  reference operator*()              { return *dummy_pointer(); }
  pointer   operator->()             { return dummy_pointer(); }

  const_reference operator*()  const { return *dummy_pointer(); }
  const_pointer   operator->() const { return dummy_pointer(); }

  bool operator==(const Self&) const { return true; }
  bool operator!=(const Self&) const { return false; }

  bool operator<(const Self& other) const {
    return this < &other;
  }

  static const_reference dummy_reference() {
    static value_type dummy_reference_static;
    return dummy_reference_static;
  }
 private:
  static pointer dummy_pointer() {
    static value_type dummy_pointer_static;
    return &dummy_pointer_static;
  }
};

CGAL_VORONOI_DIAGRAM_2_END_NAMESPACE

CGAL_END_NAMESPACE


#endif // CGAL_VORONOI_DIAGRAM_2_DUMMY_ITERATOR_H
