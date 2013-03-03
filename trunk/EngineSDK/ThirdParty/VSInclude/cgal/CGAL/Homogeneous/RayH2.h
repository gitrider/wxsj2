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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Homogeneous_kernel/include/CGAL/Homogeneous/RayH2.h $
// $Id: RayH2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra
 

#ifndef CGAL_RAYH2_H
#define CGAL_RAYH2_H

#include <CGAL/Twotuple.h>

CGAL_BEGIN_NAMESPACE

template < class R_ >
class RayH2
{
    typedef typename R_::FT                   FT;
    typedef typename R_::RT                   RT;
    typedef typename R_::Point_2              Point_2;
    typedef typename R_::Direction_2          Direction_2;
    typedef typename R_::Line_2               Line_2;
    typedef typename R_::Vector_2             Vector_2;
    typedef typename R_::Aff_transformation_2 Aff_transformation_2;

    typedef Twotuple<Point_2>                        Rep;
    typedef typename R_::template Handle<Rep>::type  Base;

    Base base;

public:
    typedef R_                                             R;

    RayH2() {}

    RayH2( const Point_2& sp, const Point_2& secondp)
      : base(sp, secondp) {}

    RayH2( const Point_2& sp, const Direction_2& d)
      : base(sp, sp + d.to_vector()) {}

    RayH2( const Point_2& sp, const Vector_2& v)
      : base(sp, sp + v) {}

    RayH2( const Point_2& sp, const Line_2& l)
      : base(sp, sp + l.to_vector()) {}

    bool    operator==(const RayH2<R>& r) const;
    bool    operator!=(const RayH2<R>& r) const;

    const Point_2 & start() const;
    const Point_2 & source() const;
    const Point_2 & second_point() const;
    Point_2     point(int i) const;
    Direction_2    direction() const;
    Vector_2       to_vector() const;
    Line_2         supporting_line() const;
    RayH2<R>       opposite() const;

    bool    is_horizontal() const;
    bool    is_vertical() const;
    bool    has_on(const Point_2& p) const;
    bool    collinear_has_on(const Point_2& p) const;
    bool    is_degenerate() const;

    RayH2<R> transform( const Aff_transformation_2 & t) const;
};


template < class R >
inline
const typename RayH2<R>::Point_2 &
RayH2<R>::source() const
{ return get(base).e0; }

template < class R >
inline
const typename RayH2<R>::Point_2 &
RayH2<R>::start() const
{ return source(); }

template < class R >
CGAL_KERNEL_INLINE
typename RayH2<R>::Vector_2
RayH2<R>::to_vector() const
{
  CGAL_kernel_precondition( !is_degenerate() );
  return second_point() - start();
}

template < class R >
CGAL_KERNEL_INLINE
typename RayH2<R>::Direction_2
RayH2<R>::direction() const
{
  CGAL_kernel_precondition( !is_degenerate() );
  return Direction_2( second_point() - start() );
}

template < class R >
inline
const typename RayH2<R>::Point_2 &
RayH2<R>::second_point() const
{
  CGAL_kernel_precondition( !is_degenerate() );
  return get(base).e1;
}

template < class R >
CGAL_KERNEL_INLINE
typename RayH2<R>::Point_2
RayH2<R>::point(int i) const
{
  CGAL_kernel_precondition( !is_degenerate() );
  CGAL_kernel_precondition( i>= 0 );
  Vector_2 v = direction().to_vector();
  return start() + RT(i) * v;
}

template < class R >
inline
typename RayH2<R>::Line_2
RayH2<R>::supporting_line() const
{
  CGAL_kernel_precondition( !is_degenerate() );
  return Line_2(*this);
}

template < class R >
inline
RayH2<R>
RayH2<R>::opposite() const
{ return RayH2<R>( start(), - direction() ); }

template < class R >
CGAL_KERNEL_INLINE
RayH2<R>
RayH2<R>::
transform(const typename RayH2<R>::Aff_transformation_2 & t) const
{
  return RayH2<R>(t.transform(start()), t.transform(second_point()) );
}

#ifndef CGAL_NO_OSTREAM_INSERT_RAYH2
template < class R >
std::ostream &
operator<<(std::ostream &os, const RayH2<R> &r)
{
  switch(os.iword(IO::mode))
  {
    case IO::ASCII :
        return os << r.source() << ' ' << r.second_point();
    case IO::BINARY :
        return os << r.source() << r.second_point();
    default:
       return os << "RayC2(" << r.source() <<  ", " << r.second_point() << ")";
  }
}
#endif // CGAL_NO_OSTREAM_INSERT_RAYH2

#ifndef CGAL_NO_ISTREAM_EXTRACT_RAYH2
template < class R >
std::istream &
operator>>(std::istream &is, RayH2<R> &r)
{
  typename R::Point_2 p, q;
  is >> p >> q;
  r = RayH2<R>(p, q);
  return is;
}
#endif // CGAL_NO_ISTREAM_EXTRACT_RAYH2

template < class R >
CGAL_KERNEL_INLINE
bool
RayH2<R>::is_horizontal() const
{
  return start().hy()*second_point().hw() == second_point().hy()*start().hw();
}

template < class R >
CGAL_KERNEL_INLINE
bool
RayH2<R>::is_vertical() const
{
  return start().hx()*second_point().hw() == second_point().hx()*start().hw();
}

template < class R >
CGAL_KERNEL_INLINE
bool
RayH2<R>::has_on(const typename RayH2<R>::Point_2& p) const
{
  return p == start() || Direction_2(p - start()) == direction();
}

template < class R >
CGAL_KERNEL_INLINE
bool
RayH2<R>::is_degenerate() const
{ return start() == get(base).e1; }

template < class R >
inline
bool
RayH2<R>::collinear_has_on(const typename RayH2<R>::Point_2& p) const
{ return has_on(p); }

template < class R >
CGAL_KERNEL_INLINE
bool
RayH2<R>::operator==(const RayH2<R>& r) const
{ return ( (start() == r.start() )&&( direction() == r.direction() ) ); }

template < class R >
inline
bool
RayH2<R>::operator!=( const RayH2<R>& r) const
{ return !(*this == r); }

CGAL_END_NAMESPACE

#endif // CGAL_RAYH2_H
