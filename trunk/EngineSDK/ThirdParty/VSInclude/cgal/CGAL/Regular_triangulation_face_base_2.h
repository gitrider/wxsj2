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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_2/include/CGAL/Regular_triangulation_face_base_2.h $
// $Id: Regular_triangulation_face_base_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Frederic Fichel, Mariette Yvinec

#ifndef CGAL_REGULAR_TRIANGULATION_FACE_BASE_2_H
#define CGAL_REGULAR_TRIANGULATION_FACE_BASE_2_H

#include <list>
#include <CGAL/triangulation_assertions.h>
#include <CGAL/Triangulation_short_names_2.h>
#include <CGAL/Triangulation_ds_face_base_2.h>

CGAL_BEGIN_NAMESPACE


template <class Gt, class Fb = Triangulation_face_base_2<Gt> >
class Regular_triangulation_face_base_2
  :  public Fb
{
  typedef Fb                                            Fbase;
  typedef typename Fbase::Triangulation_data_structure  TDS;
public:
  typedef Gt                                   Geom_traits;
  typedef TDS                                  Triangulation_data_structure;
  typedef typename TDS::Vertex_handle          Vertex_handle;
  typedef typename TDS::Face_handle            Face_handle;

  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename Fb::template Rebind_TDS<TDS2>::Other   Fb2;
    typedef Regular_triangulation_face_base_2<Gt,Fb2>             Other;
  };

  typedef std::list<Vertex_handle>             Vertex_list;

protected:
  Vertex_list vlist;

public:
 Regular_triangulation_face_base_2()
   : Fbase(),  vlist()
  {}

  Regular_triangulation_face_base_2(Vertex_handle v0, 
				    Vertex_handle v1, 
				    Vertex_handle v2)
    : Fbase(v0,v1,v2), vlist()
  { }

  Regular_triangulation_face_base_2(Vertex_handle v0, 
				    Vertex_handle v1, 
				    Vertex_handle v2,
				    Face_handle n0, 
				    Face_handle n1, 
				    Face_handle n2)
    : Fbase(v0,v1,v2,n0,n1,n2),  vlist()
  { }

  ~Regular_triangulation_face_base_2()
  { 
    vlist.clear();
  }


  Vertex_list& vertex_list()
  {
    return vlist;
  }


};

CGAL_END_NAMESPACE 

#endif // CGAL_REGULAR_TRIANGULATION_FACE_BASE_2_H

