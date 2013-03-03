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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Homogeneous_kernel/include/CGAL/Homogeneous/Iso_rectangleH2.h $
// $Id: Iso_rectangleH2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra
 
#ifndef CGAL_ISO_RECTANGLEH2_H
#define CGAL_ISO_RECTANGLEH2_H

#include <CGAL/Twotuple.h>

CGAL_BEGIN_NAMESPACE

template <class R_>
class Iso_rectangleH2
{
  typedef typename R_::FT                   FT;
  typedef typename R_::RT                   RT;
  typedef typename R_::Point_2              Point_2;
  typedef typename R_::Aff_transformation_2 Aff_transformation_2;
  typedef typename R_::Iso_rectangle_2      Iso_rectangle_2;

  typedef Twotuple<Point_2>                        Rep;
  typedef typename R_::template Handle<Rep>::type  Base;

  Base base;

public:
  typedef R_                                    R;
  typedef typename Point_2::Rep::Cartesian_coordinate_type Cartesian_coordinate_type;
  typedef typename Point_2::Rep::Homogeneous_coordinate_type Homogeneous_coordinate_type;

  Iso_rectangleH2() {}

  Iso_rectangleH2(const Point_2& p, const Point_2& q)
    : base(p,q)
  {}

  const Point_2 & min() const;
  const Point_2 & max() const;
  
  Iso_rectangle_2
            transform(const Aff_transformation_2& t) const
  {
    // FIXME : We need a precondition like this!!!
    // CGAL_kernel_precondition(t.is_axis_preserving());
    return Iso_rectangle_2(t.transform(min()), t.transform(max()));
  }

  Bounded_side bounded_side(const Point_2& p) const;
};



template < class R >
inline
const typename Iso_rectangleH2<R>::Point_2 &
Iso_rectangleH2<R>::min() const
{ return get(base).e0; }

template < class R >
inline
const typename Iso_rectangleH2<R>::Point_2 &
Iso_rectangleH2<R>::max() const
{ return get(base).e1; }

template < class R >
CGAL_KERNEL_INLINE
Bounded_side
Iso_rectangleH2<R>::
bounded_side(const typename Iso_rectangleH2<R>::Point_2& p) const
{
  Oriented_side wrt_min = _where_wrt_L_wedge(min(),p);
  Oriented_side wrt_max = _where_wrt_L_wedge(p,max());
  if (( wrt_min == ON_NEGATIVE_SIDE )||( wrt_max == ON_NEGATIVE_SIDE))
  {
      return ON_UNBOUNDED_SIDE;
  }
  if (  ( wrt_min == ON_ORIENTED_BOUNDARY )
      ||( wrt_max == ON_ORIENTED_BOUNDARY ) )
  {
      return ON_BOUNDARY;
  }
  return ON_BOUNDED_SIDE;
}

#ifndef CGAL_NO_OSTREAM_INSERT_ISO_RECTANGLEH2
template < class R >
std::ostream& operator<<(std::ostream& os, const Iso_rectangleH2<R>& r)
{
  switch(os.iword(IO::mode))
  {
    case IO::ASCII :
        return os << r.min() << ' ' << r.max();
    case IO::BINARY :
        return os << r.min() << r.max();
    default:
        return os << "Iso_rectangleH2(" << r.min() << ", " << r.max() << ")";
  }
}
#endif // CGAL_NO_OSTREAM_INSERT_ISO_RECTANGLEH2

#ifndef CGAL_NO_ISTREAM_EXTRACT_ISO_RECTANGLEH2
template < class R >
std::istream& operator>>(std::istream& is, Iso_rectangleH2<R>& r)
{
  typename R::Point_2 p, q;
  is >> p >> q;
  r = Iso_rectangleH2<R>(p, q);
  return is;
}
#endif // CGAL_NO_ISTREAM_EXTRACT_ISO_RECTANGLEH2

CGAL_END_NAMESPACE

#endif // CGAL_ISO_RECTANGLEH2_H
