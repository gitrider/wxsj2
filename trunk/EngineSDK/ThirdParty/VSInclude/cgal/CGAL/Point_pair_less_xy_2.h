// Copyright (c) 2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Partition_2/include/CGAL/Point_pair_less_xy_2.h $
// $Id: Point_pair_less_xy_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

#ifndef CGAL_POINT_PAIR_LESS_XY_2_H
#define CGAL_POINT_PAIR_LESS_XY_2_H

#include <utility>

namespace CGAL {

//
// Given two pairs of points determine their lexicographical order by first
// comparing the first points lexicographically and then the second points if
// the first are equal
//
template <class Traits>
class Point_pair_less_xy_2
{
   typedef typename Traits::Point_2           Point_2;
   typedef std::pair<Point_2, Point_2>        Point_pair;
   typedef typename Traits::Less_xy_2         Less_xy_2;

   public:
     Point_pair_less_xy_2() : _less_xy_2(Traits().less_xy_2_object())
     { }
     

     bool 
     operator()(const Point_pair& p, const Point_pair& q) const
     { 
        if (_less_xy_2(p.first, q.first))
            return true;
        else if (_less_xy_2(q.first, p.first))
            return false;
        else if (_less_xy_2(p.second, q.second))
            return true;
        else
            return false;
     }

   private:
      Less_xy_2 _less_xy_2;
};

}

#endif // CGAL_POINT_PAIR_LESS_XY_H

