// Copyright (c) 2005  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Surface_mesher/include/CGAL/Point_with_surface_index.h $
// $Id: Point_with_surface_index.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Laurent RINEAU

#ifndef CGAL_POINT_WITH_SURFACE_INDEX_H
#define CGAL_POINT_WITH_SURFACE_INDEX_H

#include <CGAL/Point_traits.h>

namespace CGAL {

template <class Point>
class Point_with_surface_index : public Point
{
  typedef Point_traits<Point> Point_traits;
  typedef typename Point_traits::Bare_point Bare_point;

public:
  Point_with_surface_index() : Point(), index(0) {}

  Point_with_surface_index(const Point& p) : Point(p), index(0) {}
			   

  Point_with_surface_index(const Point_with_surface_index& pi)
    : Point(pi), index(pi.surface_index()) {}

//   template <typename RT>
//   Point_with_surface_index(const RT& x, const RT& y, const RT& z)
//     : Point(Point_traits().point(Bare_point(x, y, z))), index(0) {}

  int surface_index() const
  {
    return index;
  }

  void set_surface_index(const int i)
  {
    index = i;
  }
private:
  int index;
}; // end class Point_with_surface_index

template <class Point>
std::ostream&
operator<<(std::ostream &os, const Point_with_surface_index<Point>& p)
{
  os << static_cast<const Point&>(p);
  if(is_ascii(os))
    os << ' ';
  return os << p.surface_index();
}

template <class Point>
std::istream&
operator>>(std::istream &is, Point_with_surface_index<Point>& p)
{
  is >>  static_cast<Point&>(p);
  int index;
  is >> index;
  p.set_surface_index(index);
  return is;
}

} // end namespace CGAL

#endif
