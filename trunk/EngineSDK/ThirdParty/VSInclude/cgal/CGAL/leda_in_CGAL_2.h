// Copyright (c) 1999  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/LEDA/include/CGAL/leda_in_CGAL_2.h $
// $Id: leda_in_CGAL_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra


#ifndef LEDA_IN_CGAL_H
#define LEDA_IN_CGAL_H

#include <CGAL/basic.h>
#include <CGAL/enum.h>
#include <CGAL/LEDA_basic.h>
#include <LEDA/point.h>
#include <LEDA/segment.h>
#include <LEDA/line.h>

CGAL_BEGIN_NAMESPACE

inline
bool
left_turn( const leda_point & p, 
          const leda_point & q, 
          const leda_point & r)
{ return  CGAL_LEDA_SCOPE::left_turn(p,q,r); }

inline
bool
right_turn( const leda_point & p, 
           const leda_point & q, 
           const leda_point & r)
{ return  CGAL_LEDA_SCOPE::right_turn(p,q,r); }

inline
Orientation
orientation( const leda_point & p, 
             const leda_point & q, 
             const leda_point & r)
{ return (Orientation)CGAL_LEDA_SCOPE::orientation(p,q,r); }

inline
bool
lexicographically_xy_smaller( const leda_point & p, 
                              const leda_point & q)
{ return ( leda_point::cmp_xy(p,q)  <  0 ); }

inline
bool
lexicographically_yx_smaller( const leda_point & p, 
                              const leda_point & q)
{ return ( leda_point::cmp_yx(p,q)  <  0 ); }

inline
bool
lexicographically_xy_larger( const leda_point & p, 
                             const leda_point & q)
{ return ( leda_point::cmp_xy(p,q)  >  0 ); }

inline
bool
lexicographically_yx_larger( const leda_point & p, 
                             const leda_point & q)
{ return ( leda_point::cmp_yx(p,q)  >  0 ); }

inline
bool
collinear_are_ordered_along_line( const leda_point & p, 
                                  const leda_point & q, 
                                  const leda_point & r)
{ 
  return 
  (  (( leda_point::cmp_xy(p,q)<=0 )&&( leda_point::cmp_xy(q,r)<=0 )) 
   ||(( leda_point::cmp_xy(r,q)<=0 )&&( leda_point::cmp_xy(q,p)<=0 )) );
}

inline
Comparison_result 
cmp_signed_dist_to_line( const leda_point & p, const leda_point & q,
                         const leda_point & r, const leda_point & s )
{
#if ( __LEDA__ >= 360 )
  return (Comparison_result)CGAL_LEDA_SCOPE::cmp_signed_dist(p,q,r,s);
#else
  leda_line  l(p,q);
  int  r_or = CGAL_LEDA_SCOPE::orientation( l, r );
  int  s_or = CGAL_LEDA_SCOPE::orientation( l, s );
  if ( r_or != s_or )
  {
      return (Comparison_result)( r_or < s_or );
  }
  else
  {
     return 
      (Comparison_result)(r_or *( CGAL::sign(l.sqr_dist(r) - l.sqr_dist(s) )));
  }
#endif  // __LEDA__ >= 360
}

CGAL_END_NAMESPACE

#endif // LEDA_IN_CGAL_H
