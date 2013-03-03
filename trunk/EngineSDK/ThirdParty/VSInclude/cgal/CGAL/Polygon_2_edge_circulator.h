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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Polygon/include/CGAL/Polygon_2_edge_circulator.h $
// $Id: Polygon_2_edge_circulator.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Wieger Wesselink <wieger@cs.ruu.nl>

#ifndef CGAL_POLYGON_2_EDGE_CIRCULATOR_H
#define CGAL_POLYGON_2_EDGE_CIRCULATOR_H

#include <iterator>
#include <CGAL/circulator.h>
#include <CGAL/Polygon_2_vertex_circulator.h>
#include <CGAL/polygon_assertions.h>

CGAL_BEGIN_NAMESPACE

//-----------------------------------------------------------------------//
//                          Polygon_2_const_edge_circulator
//-----------------------------------------------------------------------//
// Ideally the class Polygon_2_const_edge_circulator would be a nested
// class of Polygon_2, but this leads to compiler problems with
// SGI C++ 4.0 with the iterator_category() function

template <class _Traits, class _Container> class Polygon_2;

template <class _Traits, class _Container>
class Polygon_2_const_edge_circulator {
  public:
    typedef _Traits Traits;
    typedef typename _Traits::Segment_2 Segment_2;
    typedef _Container Container;
//    typedef Bidirectional_const_circulator_from_container<_Container>
    typedef Polygon_circulator<_Container>
            Vertex_const_circulator;

    typedef Segment_2                            value_type;
    typedef typename _Container::difference_type difference_type;
    typedef typename _Container::size_type       size_type;
    typedef Segment_2*                           pointer;
    typedef const Segment_2*                     const_pointer;
    typedef Segment_2&                           reference;
    typedef const Segment_2&                     const_reference;
    typedef Bidirectional_circulator_tag    iterator_category;

  private:
    Vertex_const_circulator first_vertex;

  public:
    Polygon_2_const_edge_circulator() {}

    Polygon_2_const_edge_circulator(Vertex_const_circulator f)
      : first_vertex(f) {}

    bool operator==( CGAL_NULL_TYPE p ) const {
      CGAL_polygon_assertion( p == 0);
      return (first_vertex == 0);
    }

    bool operator!=( CGAL_NULL_TYPE p ) const
    {
      return !(*this == p);
    }

    bool
    operator==(
      const Polygon_2_const_edge_circulator<_Traits, _Container>& x) const
    {
      return first_vertex == x.first_vertex;
    }

    bool
    operator!=(
      const Polygon_2_const_edge_circulator<_Traits, _Container>& x) const
    {
      return !(first_vertex == x.first_vertex);
    }

    Segment_2 operator*() const
    {
      Vertex_const_circulator second_vertex = first_vertex;
      ++second_vertex;
      typename Traits::Construct_segment_2 construct_segment_2 = 
            Traits().construct_segment_2_object();
      return construct_segment_2(*first_vertex, *second_vertex);
    }

    Polygon_2_const_edge_circulator<_Traits, _Container>& operator++()
    {
      ++first_vertex;
      return *this;
    }

    Polygon_2_const_edge_circulator<_Traits, _Container> operator++(int)
    {
      Polygon_2_const_edge_circulator<_Traits, _Container> tmp = *this;
      ++*this;
      return tmp;
    }

    Polygon_2_const_edge_circulator<_Traits, _Container>& operator--()
    {
      --first_vertex;
      return *this;
    }

    Polygon_2_const_edge_circulator<_Traits, _Container> operator--(int)
    {
      Polygon_2_const_edge_circulator<_Traits, _Container> tmp = *this;
      --*this;
      return tmp;
    }

// random access iterator requirements
    Polygon_2_const_edge_circulator<_Traits, _Container>&
    operator+=(difference_type n)
    {
      first_vertex += n;
      return *this;
    }

    Polygon_2_const_edge_circulator<_Traits, _Container>
    operator+(difference_type n)
    {
      return Polygon_2_const_edge_circulator<_Traits, _Container>(
        this->container, first_vertex + n);
    }

    Polygon_2_const_edge_circulator<_Traits, _Container>&
    operator-=(difference_type n)
    {
      return (*this) -= n;
    }

    Polygon_2_const_edge_circulator<_Traits, _Container>
    operator-(difference_type n)
    {
      return Polygon_2_const_edge_circulator<_Traits, _Container>(
        this->container, first_vertex - n);
    }

    difference_type
    operator-(
      const Polygon_2_const_edge_circulator<_Traits, _Container>& a)
    {
      return first_vertex - a.first_vertex;
    }

    Segment_2 operator[](int n)
    {
      return *Polygon_2_const_edge_circulator<_Traits, _Container>(
        this->container, first_vertex+n);
    }

    bool operator<(
      const Polygon_2_const_edge_circulator<_Traits, _Container>& a)
    {
      return first_vertex < a.first_vertex;
    }

    bool operator>(
      const Polygon_2_const_edge_circulator<_Traits, _Container>& a)
    {
      return first_vertex > a.first_vertex;
    }

    bool operator<=(
      const Polygon_2_const_edge_circulator<_Traits, _Container>& a)
    {
      return first_vertex <= a.first_vertex;
    }

    bool operator>=(
      const Polygon_2_const_edge_circulator<_Traits, _Container>& a)
    {
      return first_vertex >= a.first_vertex;
    }

};

/*
template <class _Traits, class _Container>
typename _Container::difference_type
distance_type(
  const Polygon_2_const_edge_circulator<_Traits,_Container>&)
{
  return typename _Container::difference_type();
}

template <class _Traits, class _Container>
typename _Traits::Segment_2*
value_type(const Polygon_2_const_edge_circulator<_Traits,_Container>&)
{
  return (typename _Traits::Segment_2*)(0);
}
*/

//-----------------------------------------------------------------------//
//                          implementation
//-----------------------------------------------------------------------//

//--------------------------------------------------------------------//
// I don't know how to implement the following function:
//
// template <class _Traits, class _Container>
// inline
// Polygon_2_const_edge_circulator<_Traits, _Container>
// operator+(_Container::difference_type n,
//           Polygon_2_const_edge_circulator<_Traits, _Container>& a)
// { return a+n; }
//--------------------------------------------------------------------//

CGAL_END_NAMESPACE

#endif

