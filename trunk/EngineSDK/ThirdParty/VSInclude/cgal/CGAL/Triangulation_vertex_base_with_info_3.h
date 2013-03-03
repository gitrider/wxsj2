// Copyright (c) 2003  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_3/include/CGAL/Triangulation_vertex_base_with_info_3.h $
// $Id: Triangulation_vertex_base_with_info_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_TRIANGULATION_VERTEX_BASE_WITH_INFO_3_H
#define CGAL_TRIANGULATION_VERTEX_BASE_WITH_INFO_3_H

#include <CGAL/Triangulation_vertex_base_3.h>

CGAL_BEGIN_NAMESPACE

template < typename Info_, typename GT,
           typename Vb = Triangulation_vertex_base_3<GT> >
class Triangulation_vertex_base_with_info_3
  : public Vb
{
  Info_ _info;
public:

  typedef typename Vb::Cell_handle                   Cell_handle;
  typedef typename Vb::Point                         Point;
  typedef Info_                                      Info;

  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename Vb::template Rebind_TDS<TDS2>::Other          Vb2;
    typedef Triangulation_vertex_base_with_info_3<Info, GT, Vb2>   Other;
  };

  Triangulation_vertex_base_with_info_3()
    : Vb() {}

  Triangulation_vertex_base_with_info_3(const Point & p)
    : Vb(p) {}

  Triangulation_vertex_base_with_info_3(const Point & p, Cell_handle c)
    : Vb(p, c) {}

  Triangulation_vertex_base_with_info_3(Cell_handle c)
    : Vb(c) {}

  const Info& info() const { return _info; }
  Info&       info()       { return _info; }
};

CGAL_END_NAMESPACE

#endif // CGAL_TRIANGULATION_VERTEX_BASE_WITH_INFO_3_H
