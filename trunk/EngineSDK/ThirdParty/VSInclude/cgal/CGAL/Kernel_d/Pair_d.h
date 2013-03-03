// Copyright (c) 1997-2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_d/include/CGAL/Kernel_d/Pair_d.h $
// $Id: Pair_d.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>
#ifndef CGAL_PAIR_D_H
#define CGAL_PAIR_D_H

#include <CGAL/basic.h>
#include <CGAL/Handle_for.h>

CGAL_BEGIN_NAMESPACE

template <class R> class Segment_d;
template <class R> class Ray_d;
template <class R> class Line_d;

template <class R> 
class Pair_d 
{
  typedef Pair_d<R> Self;
  typedef typename R::Point_d Point_d;
  typedef typename R::Vector_d Vector_d;
  typedef typename R::Direction_d Direction_d;
  Point_d _p[2];

  friend class Line_d<R>; 
  friend class Ray_d<R>; 
  friend class Segment_d<R>; 
   
/* Any line object in $d$ - space is defined by two points |_p1| and |_p2|
respectively. There exists an orientation from _p1 to _p2. */

public: 
Pair_d(int d = 0) { _p[0]=_p[1]=Point_d(d); }

Pair_d(const Point_d& p, const Point_d& q)
{ CGAL_assertion_msg((p.dimension() == q.dimension()), 
  "Pair_d::constructor: source and target must have the same dimension."); 
  _p[0]=p; _p[1]=q;
}

bool is_degenerate() const
{ return (_p[0] == _p[1]); }

Vector_d vector() const 
{ return (_p[1] - _p[0]); }

Direction_d direction() const
{ return vector().direction(); }

void read(std::istream& is)
{ 
  switch( is.iword(CGAL::IO::mode) ) {
    case CGAL::IO::ASCII :
      is >> _p[0] >> _p[1]; break;
    case CGAL::IO::BINARY :
      CGAL::read(is, _p[0]); CGAL::read(is, _p[1]); break;
    default:
    CGAL_assertion_msg(0,"\nStream must be in ascii or binary mode\n"); 
  }
}

void print(std::ostream& os, const char* _name) const
{ 
  switch( os.iword(CGAL::IO::mode) ) {
    case CGAL::IO::ASCII :
      os << _p[0] << " " <<  _p[1]; break;
    case CGAL::IO::BINARY :
      CGAL::write(os, _p[0]); CGAL::write(os, _p[1]); break;
    default :
      os << _name << "(" << _p[0] << ", " << _p[1] << ")"; break;
  }
}

}; // Pair_d<R>

CGAL_END_NAMESPACE
#endif //CGAL_PAIR_D_H

