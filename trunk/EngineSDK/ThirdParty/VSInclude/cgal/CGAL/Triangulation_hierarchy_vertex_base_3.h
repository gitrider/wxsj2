// Copyright (c) 1998, 2001, 2003  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_3/include/CGAL/Triangulation_hierarchy_vertex_base_3.h $
// $Id: Triangulation_hierarchy_vertex_base_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Olivier Devillers <Olivier.Devillers@sophia.inria.fr>
//                 Sylvain Pion <Sylvain.Pion@sophia.inria.fr>

#ifndef CGAL_TRIANGULATION_HIERARCHY_VERTEX_BASE_3_H
#define CGAL_TRIANGULATION_HIERARCHY_VERTEX_BASE_3_H

#include <CGAL/basic.h>
#include <CGAL/Triangulation_short_names_3.h>

CGAL_BEGIN_NAMESPACE

template < class Vbb >
class Triangulation_hierarchy_vertex_base_3
  : public Vbb
{
  typedef Vbb                                           Base;
  typedef typename Base::Triangulation_data_structure   Tds;
public:
  typedef typename Tds::Vertex_handle                   Vertex_handle;
  typedef typename Tds::Cell_handle                     Cell_handle;
  typedef typename Base::Point                          Point;

  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename Vbb::template Rebind_TDS<TDS2>::Other      Vb2;
    typedef Triangulation_hierarchy_vertex_base_3<Vb2>          Other;
  };

  Triangulation_hierarchy_vertex_base_3()
    : Base(), _up(), _down() {}

  Triangulation_hierarchy_vertex_base_3(const Point & p, Cell_handle f)
    : Base(p,f), _up(), _down() {}

  Triangulation_hierarchy_vertex_base_3(const Point & p)
    : Base(p), _up(), _down() {}

  Vertex_handle up()   const { return _up; }
  Vertex_handle down() const { return _down; }
  void set_up(Vertex_handle u)   { _up=u; }
  void set_down(Vertex_handle d) { _down=d; }

private:
  Vertex_handle _up;    // same vertex one level above
  Vertex_handle _down;  // same vertex one level below
};

CGAL_END_NAMESPACE

#endif // CGAL_TRIANGULATION_HIERARCHY_VERTEX_BASE_3_H
