// Copyright (c) 1997  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Alpha_shapes_2/include/CGAL/Alpha_shape_vertex_base_2.h $
// $Id: Alpha_shape_vertex_base_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Tran Kai Frank DA <Frank.Da@sophia.inria.fr>

#ifndef CGAL_ALPHA_SHAPE_VERTEX_BASE_2_H
#define CGAL_ALPHA_SHAPE_VERTEX_BASE_2_H

#include <utility>
#include <CGAL/Triangulation_vertex_base_2.h>

//-------------------------------------------------------------------
CGAL_BEGIN_NAMESPACE
//-------------------------------------------------------------------

template <class Gt, class Vb = Triangulation_vertex_base_2<Gt> >
class Alpha_shape_vertex_base_2 : public Vb
{
  typedef typename Vb::Triangulation_data_structure  TDS;
public:
  typedef TDS                             Triangulation_data_structure;
  typedef typename TDS::Vertex_handle     Vertex_handle;
  typedef typename TDS::Face_handle       Face_handle;

  typedef typename Gt::FT                Coord_type;
  typedef std::pair< Coord_type, Coord_type >    Interval2;
  typedef typename Vb::Point                   Point;

  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename Vb::template Rebind_TDS<TDS2>::Other    Vb2;
    typedef Alpha_shape_vertex_base_2 <Gt,Vb2>         Other;
  };
private:
  Interval2 I;

public:
  Alpha_shape_vertex_base_2()
    : Vb() 
    {}
  
  Alpha_shape_vertex_base_2(const Point & p)
    : Vb(p) 
    {}
  
  Alpha_shape_vertex_base_2(const Point & p, Face_handle f)
    : Vb(p, f) 
    {}


public:

  inline Interval2 get_range() 
    {
      return I;
    }

  inline void set_range(Interval2 Inter)
    {  
      I = Inter;
    }

};

//-------------------------------------------------------------------
CGAL_END_NAMESPACE
//-------------------------------------------------------------------

#endif //ALPHA_SHAPE_VERTEX_BASE_2_H
