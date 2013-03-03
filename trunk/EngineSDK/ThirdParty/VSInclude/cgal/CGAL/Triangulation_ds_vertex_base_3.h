// Copyright (c) 1999,2000,2001,2002,2003  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_3/include/CGAL/Triangulation_ds_vertex_base_3.h $
// $Id: Triangulation_ds_vertex_base_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>

#ifndef CGAL_TRIANGULATION_DS_VERTEX_BASE_3_H
#define CGAL_TRIANGULATION_DS_VERTEX_BASE_3_H

#include <CGAL/basic.h>
#include <CGAL/Triangulation_short_names_3.h>
#include <CGAL/Dummy_tds_3.h>

CGAL_BEGIN_NAMESPACE

template < typename TDS = void >
class Triangulation_ds_vertex_base_3
{
public:
  typedef TDS                          Triangulation_data_structure;
  typedef typename TDS::Vertex_handle  Vertex_handle;
  typedef typename TDS::Cell_handle    Cell_handle;

  template <typename TDS2>
  struct Rebind_TDS { typedef Triangulation_ds_vertex_base_3<TDS2> Other; };

  Triangulation_ds_vertex_base_3()
    : _c() {}
  
  Triangulation_ds_vertex_base_3(const Cell_handle& c)
    : _c(c) {}

  const Cell_handle& cell() const
  { return _c; }

  void set_cell(const Cell_handle& c)
  { _c = c; }

  // the following trivial is_valid allows
  // the user of derived cell base classes 
  // to add their own purpose checking
  bool is_valid(bool = false, int = 0) const
  { 
    return cell() != Cell_handle();
  }

  // For use by the Compact_container.
  void *   for_compact_container() const
  { return _c.for_compact_container(); }
  void * & for_compact_container()
  { return _c.for_compact_container(); }

private:
  Cell_handle _c;
};

template < class TDS >
inline
std::istream&
operator>>(std::istream &is, Triangulation_ds_vertex_base_3<TDS> &)
  // no combinatorial information.
{
  return is;
}

template < class TDS >
inline
std::ostream&
operator<<(std::ostream &os, const Triangulation_ds_vertex_base_3<TDS> &)
  // no combinatorial information.
{
  return os;
}

// Specialization for void.
template <>
class Triangulation_ds_vertex_base_3<void>
{
public:
  typedef Dummy_tds_3             Triangulation_data_structure;
  typedef Triangulation_data_structure::Vertex_handle   Vertex_handle;
  typedef Triangulation_data_structure::Cell_handle     Cell_handle;
  template <typename TDS2>
  struct Rebind_TDS { typedef Triangulation_ds_vertex_base_3<TDS2> Other; };
};

CGAL_END_NAMESPACE

#endif // CGAL_TRIANGULATION_DS_VERTEX_BASE_3_H
