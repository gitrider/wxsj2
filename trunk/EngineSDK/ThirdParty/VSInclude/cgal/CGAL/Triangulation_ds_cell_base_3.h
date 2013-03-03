// Copyright (c) 1999-2005  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_3/include/CGAL/Triangulation_ds_cell_base_3.h $
// $Id: Triangulation_ds_cell_base_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//                 Sylvain Pion <Sylvain.Pion@sophia.inria.fr>

// cell of a triangulation data structure of any dimension <=3

#ifndef CGAL_TRIANGULATION_DS_CELL_BASE_3_H
#define CGAL_TRIANGULATION_DS_CELL_BASE_3_H

#include <CGAL/basic.h>
#include <CGAL/triangulation_assertions.h>
#include <CGAL/Triangulation_short_names_3.h>
#include <CGAL/Dummy_tds_3.h>

CGAL_BEGIN_NAMESPACE

template < typename TDS = void >
class Triangulation_ds_cell_base_3
{
public:
  typedef TDS                          Triangulation_data_structure;
  typedef typename TDS::Vertex_handle  Vertex_handle;
  typedef typename TDS::Cell_handle    Cell_handle;
  typedef typename TDS::Vertex         Vertex;
  typedef typename TDS::Cell           Cell;

  template <typename TDS2>
  struct Rebind_TDS { typedef Triangulation_ds_cell_base_3<TDS2> Other; };

  Triangulation_ds_cell_base_3()
  {
    set_vertices();
    set_neighbors();
    set_in_conflict_flag(0);
  }

  Triangulation_ds_cell_base_3(const Vertex_handle& v0, const Vertex_handle& v1,
                               const Vertex_handle& v2, const Vertex_handle& v3)
  {
    set_vertices(v0, v1, v2, v3);
    set_neighbors();
    set_in_conflict_flag(0);
  }

  Triangulation_ds_cell_base_3(const Vertex_handle& v0, const Vertex_handle& v1,
                               const Vertex_handle& v2, const Vertex_handle& v3,
                               const Cell_handle&   n0, const Cell_handle&   n1,
                               const Cell_handle&   n2, const Cell_handle&   n3)
  {
    set_vertices(v0, v1, v2, v3);
    set_neighbors(n0, n1, n2, n3);
    set_in_conflict_flag(0);
  }

  // ACCESS FUNCTIONS

  const Vertex_handle& vertex(int i) const
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3 );
    return V[i];
  }

  bool has_vertex(const Vertex_handle& v) const
  {
    return (V[0] == v) || (V[1] == v) || (V[2]== v) || (V[3]== v);
  }

  bool has_vertex(const Vertex_handle& v, int & i) const
    {
      if (v == V[0]) { i = 0; return true; }
      if (v == V[1]) { i = 1; return true; }
      if (v == V[2]) { i = 2; return true; }
      if (v == V[3]) { i = 3; return true; }
      return false;
    }

  int index(const Vertex_handle& v) const
  {
    if (v == V[0]) { return 0; }
    if (v == V[1]) { return 1; }
    if (v == V[2]) { return 2; }
    CGAL_triangulation_assertion( v == V[3] );
    return 3;
  }

  const Cell_handle& neighbor(int i) const
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3);
    return N[i];
  }

  bool has_neighbor(const Cell_handle& n) const
  {
    return (N[0] == n) || (N[1] == n) || (N[2] == n) || (N[3] == n);
  }

  bool has_neighbor(const Cell_handle& n, int & i) const
  {
    if(n == N[0]){ i = 0; return true; }
    if(n == N[1]){ i = 1; return true; }
    if(n == N[2]){ i = 2; return true; }
    if(n == N[3]){ i = 3; return true; }
    return false;
  }

  int index(const Cell_handle& n) const
  {
    if (n == N[0]) return 0;
    if (n == N[1]) return 1;
    if (n == N[2]) return 2;
    CGAL_triangulation_assertion( n == N[3] );
    return 3;
  }

  // SETTING

  void set_vertex(int i, const Vertex_handle& v)
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3);
    V[i] = v;
  }

  void set_neighbor(int i, const Cell_handle& n)
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3);
    N[i] = n;
  }

  void set_vertices()
  {
    V[0] = V[1] = V[2] = V[3] = Vertex_handle();
  }

  void set_vertices(const Vertex_handle& v0, const Vertex_handle& v1,
                    const Vertex_handle& v2, const Vertex_handle& v3)
  {
    V[0] = v0;
    V[1] = v1;
    V[2] = v2;
    V[3] = v3;
  }

  void set_neighbors()
  {
    N[0] = N[1] = N[2] = N[3] = Cell_handle();
  }

  void set_neighbors(const Cell_handle& n0, const Cell_handle& n1,
                     const Cell_handle& n2, const Cell_handle& n3)
  {
    N[0] = n0;
    N[1] = n1;
    N[2] = n2;
    N[3] = n3;
  }

  // CHECKING

  // the following trivial is_valid allows
  // the user of derived cell base classes
  // to add their own purpose checking
  bool is_valid(bool = false, int = 0) const
  { return true; }

#ifndef CGAL_NO_DEPRECATED_CODE
  // Obsolete, kept for backward compatibility.
  // This should emit a warning.
  int mirror_index(int i) const
  {
      bool WARNING_THIS_FUNCTION_IS_OBSOLETE;
      CGAL_triangulation_precondition ( i>=0 && i<4 );
      Cell_handle ni = neighbor(i);
      if (&*ni->neighbor(0) == this) return 0;
      if (&*ni->neighbor(1) == this) return 1;
      if (&*ni->neighbor(2) == this) return 2;
      CGAL_triangulation_assertion(&*ni->neighbor(3) == this);
      return 3;
  }

  // Obsolete as above.
  Vertex_handle mirror_vertex(int i) const
  {
      bool WARNING_THIS_FUNCTION_IS_OBSOLETE;
      return neighbor(i)->vertex(mirror_index(i));
  }
#endif

  // For use by Compact_container.
  void * for_compact_container() const { return N[0].for_compact_container(); }
  void * & for_compact_container()     { return N[0].for_compact_container(); }

  // Conflict flag access functions.
  // This should become a property map or something at some point.
  void set_in_conflict_flag(unsigned char f) { _in_conflict_flag = f; }
  unsigned char get_in_conflict_flag() const { return _in_conflict_flag; }

private:

  Cell_handle   N[4];
  Vertex_handle V[4];
  unsigned char _in_conflict_flag;
};

template < class TDS >
inline
std::istream&
operator>>(std::istream &is, Triangulation_ds_cell_base_3<TDS> &)
  // non combinatorial information. Default = nothing
{
  return is;
}

template < class TDS >
inline
std::ostream&
operator<<(std::ostream &os, const Triangulation_ds_cell_base_3<TDS> &)
  // non combinatorial information. Default = nothing
{
  return os;
}

// Specialization for void.
template <>
class Triangulation_ds_cell_base_3<void>
{
public:
  typedef Dummy_tds_3                   Triangulation_data_structure;
  typedef Triangulation_data_structure::Vertex_handle   Vertex_handle;
  typedef Triangulation_data_structure::Cell_handle     Cell_handle;
  template <typename TDS2>
  struct Rebind_TDS { typedef Triangulation_ds_cell_base_3<TDS2> Other; };
};

CGAL_END_NAMESPACE

#endif // CGAL_TRIANGULATION_DS_CELL_BASE_3_H
