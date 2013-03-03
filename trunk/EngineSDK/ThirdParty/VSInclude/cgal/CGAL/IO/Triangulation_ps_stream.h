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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_2/include/CGAL/IO/Triangulation_ps_stream.h $
// $Id: Triangulation_ps_stream.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Carine Bonetto
//                 Mariette Yvinec

#ifndef CGAL_TRIANGULATION_PS_STREAM_H
#define CGAL_TRIANGULATION_PS_STREAM_H


#ifdef CGAL_TRIANGULATION_2_H
CGAL_BEGIN_NAMESPACE
template <class Gt,class Tds>
PS_Stream& operator << (PS_Stream& ps, const Triangulation_2<Gt,Tds> &t)

{
 t.draw_triangulation(ps);
  return ps;
}
CGAL_END_NAMESPACE
#endif // CGAL_TRIANGULATION_2_H


#ifdef CGAL_DELAUNAY_TRIANGULATION_2_H
CGAL_BEGIN_NAMESPACE
template < class Gt, class Tds >
PS_Stream& operator << (PS_Stream& ps, 
			const Delaunay_triangulation_2<Gt,Tds> &t)
{
 t.draw_triangulation(ps);
 return ps; 
}
CGAL_END_NAMESPACE
#endif // CGAL_DELAUNAY_TRIANGULATION_2_H

#ifdef CGAL_CONSTRAINED_TRIANGULATION_2_H
CGAL_BEGIN_NAMESPACE
template < class Gt, class Tds>
PS_Stream& operator<<(PS_Stream& ps,
		      const Constrained_triangulation_2<Gt,Tds> &t)
{

 t.draw_triangulation(ps);
 return ps;
}
CGAL_END_NAMESPACE
#endif // CGAL_CONSTRAINED_TRIANGULATION_2_H


#ifdef CGAL_REGULAR_TRIANGULATION_2_H
CGAL_BEGIN_NAMESPACE
template < class Gt, class Tds >
PS_Stream& operator << (PS_Stream& ps, 
			Regular_triangulation_2<Gt,Tds> &t)
{
  t.draw_triangulation(ps);
  return ps;
}
CGAL_END_NAMESPACE
#endif // CGAL_REGULAR_TRIANGULATION_2_H

#endif //CGAL_TRIANGULATION_PS_STREAM



	




