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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_2/include/CGAL/Constrained_triangulation_face_base_2.h $
// $Id: Constrained_triangulation_face_base_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Mariette Yvinec

#ifndef CGAL_CONSTRAINED_TRIANGULATION_FACE_BASE_2_H
#define CGAL_CONSTRAINED_TRIANGULATION_FACE_BASE_2_H

#include <CGAL/triangulation_assertions.h>
#include <CGAL/Triangulation_short_names_2.h>
#include <CGAL/Triangulation_ds_face_base_2.h>

CGAL_BEGIN_NAMESPACE 

template <class Gt, class Fb = Triangulation_face_base_2<Gt> >
class Constrained_triangulation_face_base_2
  :  public Fb
{
  typedef Fb                                           Base;
  typedef typename Fb::Triangulation_data_structure    TDS;
public:
  typedef Gt                                   Geom_traits;
  typedef TDS                                  Triangulation_data_structure;
  typedef typename TDS::Vertex_handle          Vertex_handle;
  typedef typename TDS::Face_handle            Face_handle;

  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename Fb::template Rebind_TDS<TDS2>::Other    Fb2;
    typedef Constrained_triangulation_face_base_2<Gt,Fb2>    Other;
  };


protected:
  bool C[3];
 
public:
  Constrained_triangulation_face_base_2()
    : Base()
  {
    set_constraints(false,false,false);
  }

  Constrained_triangulation_face_base_2(Vertex_handle v0, 
					Vertex_handle v1, 
					Vertex_handle v2)
    : Base(v0,v1,v2)
  {
    set_constraints(false,false,false);
  }

  Constrained_triangulation_face_base_2(Vertex_handle v0, 
					Vertex_handle v1, 
					Vertex_handle v2,
					Face_handle n0, 
					Face_handle n1, 
					Face_handle n2)
    : Base(v0,v1,v2,n0,n1,n2)
  {
    set_constraints(false,false,false);
  }


  Constrained_triangulation_face_base_2(Vertex_handle v0, 
					Vertex_handle v1, 
					Vertex_handle v2,
					Face_handle n0, 
					Face_handle n1, 
					Face_handle n2,
					bool c0, 
					bool c1, 
					bool c2 )
    : Base(v0,v1,v2,n0,n1,n2)
  {
    set_constraints(c0,c1,c2);
  }


  bool is_constrained(int i) const ;
  void set_constraints(bool c0, bool c1, bool c2) ;
  void set_constraint(int i, bool b);
  void reorient();
  void ccw_permute();
  void cw_permute();
  
};

template <class Gt, class Fb>
inline void
Constrained_triangulation_face_base_2<Gt,Fb>::
set_constraints(bool c0, bool c1, bool c2)
{
  C[0]=c0;
  C[1]=c1;
  C[2]=c2;
}

template <class Gt, class Fb>
inline void
Constrained_triangulation_face_base_2<Gt,Fb>::
set_constraint(int i, bool b)
{
  CGAL_triangulation_precondition( i == 0 || i == 1 || i == 2);
  C[i] = b;
}
    
template <class Gt, class Fb>
inline bool
Constrained_triangulation_face_base_2<Gt,Fb>::
is_constrained(int i) const
{
  return(C[i]);
}

template <class Gt, class Fb>
inline void
Constrained_triangulation_face_base_2<Gt,Fb>::
reorient()
{
  Base::reorient();
  set_constraints(C[1],C[0],C[2]);
}

template <class Gt, class Fb>
inline void
Constrained_triangulation_face_base_2<Gt,Fb>::
ccw_permute()
{
  Base::ccw_permute();
  set_constraints(C[2],C[0],C[1]);
}

template <class Gt, class Fb>
inline void
Constrained_triangulation_face_base_2<Gt,Fb>::
cw_permute()
{
  Base::cw_permute();
  set_constraints(C[1],C[2],C[0]);
}
  
CGAL_END_NAMESPACE 
  
#endif //CGAL_CONSTRAINED_TRIANGULATION_FACE_BASE_2_H







