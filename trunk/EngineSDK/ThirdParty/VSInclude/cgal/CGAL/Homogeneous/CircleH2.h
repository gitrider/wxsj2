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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Homogeneous_kernel/include/CGAL/Homogeneous/CircleH2.h $
// $Id: CircleH2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sven Schoenherr
//                 Stefan Schirra
 

#ifndef CGAL_CIRCLEH2_H
#define CGAL_CIRCLEH2_H

#include <CGAL/utility.h>
#include <CGAL/Interval_arithmetic.h>

CGAL_BEGIN_NAMESPACE

template <class R_>
class CircleH2
{
    typedef typename R_::FT                   FT;
    typedef typename R_::RT                   RT;
    typedef typename R_::Point_2              Point_2;
    typedef typename R_::Aff_transformation_2 Aff_transformation_2;

    typedef Triple<Point_2, FT, Orientation>         Rep;
    typedef typename R_::template Handle<Rep>::type  Base;

    Base base;

public:
    typedef R_                                    R;

    CircleH2() {}

    CircleH2(const Point_2& p, const Point_2& q, const Point_2& r)
    {
      Orientation o = CGAL::orientation( p, q, r);
      CGAL_kernel_precondition( o != COLLINEAR);

      Point_2    cp   = circumcenter( p, q, r);
      FT         sq_r = squared_distance( p, cp);

      base = Rep(cp, sq_r, o);
    }

    CircleH2(const Point_2& p, const Point_2& q, const Orientation& o)
    {
      CGAL_kernel_precondition( o != COLLINEAR);

      if ( p != q)
      {
         Point_2    cp   = midpoint( p, q);
         FT         sq_r = squared_distance( cp, p);
         base = Rep(cp, sq_r, o);
      }
      else
         base = Rep(p, FT( 0), o);
    }

    CircleH2(const Point_2& cp, const FT& squared_radius,
             const Orientation& o)
    {
      CGAL_precondition( ( ! CGAL_NTS is_negative( squared_radius)) &&
                         ( o != COLLINEAR ) );
      base = Rep(cp, squared_radius, o);
    }

    Bbox_2
    bbox() const;

    CircleH2<R>
    orthogonal_transform(const Aff_transformation_2&) const;

    const Point_2 &
    center() const;

    Orientation
    orientation() const;

    const FT &
    squared_radius() const;

    CircleH2<R>
    opposite() const;

    Oriented_side
    oriented_side(const Point_2& ) const;

    Bounded_side
    bounded_side(const Point_2& ) const;

    bool  operator==( const CircleH2<R>& ) const;
    bool  operator!=( const CircleH2<R>& ) const;
    bool  has_on_positive_side(const Point_2& ) const;
    bool  has_on_negative_side(const Point_2& ) const;
    bool  has_on_boundary( const Point_2& ) const;
    bool  has_on_bounded_side( const Point_2& ) const;
    bool  has_on_unbounded_side(const Point_2&) const;
    bool  is_degenerate() const;

    // bool  oriented_equal( const CircleH2<R>& ) const;
    // bool  unoriented_equal( const CircleH2<R>& ) const;
};

template <class R>
inline
const typename CircleH2<R>::Point_2 &
CircleH2<R>::center() const
{ return get(base).first; }

template <class R>
inline
const typename CircleH2<R>::FT &
CircleH2<R>::squared_radius() const
{ return get(base).second; }

template <class R>
CGAL_KERNEL_INLINE
CircleH2<R>
CircleH2<R>::opposite() const
{
  return CircleH2<R>( center(),
                          squared_radius(),
                          CGAL::opposite( orientation() ) );
}

template <class R>
inline
Orientation
CircleH2<R>::orientation() const
{ return get(base).third; }

template <class R>
CGAL_KERNEL_INLINE
Oriented_side
CircleH2<R>::oriented_side( const typename CircleH2<R>::Point_2& p) const
{
  FT sq_dist = squared_distance( p, center() );
  FT sq_rad  = squared_radius();
  Comparison_result vgl = CGAL_NTS compare( sq_dist, sq_rad );
  Oriented_side rel_pos = (vgl == LARGER ) ?
                                   ON_NEGATIVE_SIDE :
                                   ( (vgl == SMALLER ) ?
                                          ON_POSITIVE_SIDE :
                                          ON_ORIENTED_BOUNDARY);
  if (orientation() == POSITIVE)
  { return rel_pos; }
  else       // NEGATIVE
  { return CGAL::opposite( rel_pos ); }
}

template <class R>
CGAL_KERNEL_INLINE
bool
CircleH2<R>::has_on_positive_side(const typename CircleH2<R>::Point_2& p) const
{
  if ( orientation() == POSITIVE )
  { return (has_on_bounded_side(p) ); }
  else
  { return (has_on_unbounded_side(p) ); }
}

template <class R>
CGAL_KERNEL_INLINE
bool
CircleH2<R>::has_on_boundary(const typename CircleH2<R>::Point_2& p) const
{
  FT sq_dist = squared_distance( p, center() );
  FT sq_rad  = squared_radius();
  return ( sq_dist == sq_rad );
}

template <class R>
CGAL_KERNEL_INLINE
bool
CircleH2<R>::has_on_negative_side( const typename CircleH2<R>::Point_2&p) const
{
  if ( orientation() == NEGATIVE )
  {
      return (has_on_bounded_side(p) );
  }
  else
  {
      return (has_on_unbounded_side(p) );
  }
}

template <class R>
CGAL_KERNEL_INLINE
Bounded_side
CircleH2<R>::bounded_side(const typename CircleH2<R>::Point_2& p) const
{
  FT sq_dist = squared_distance( p, center() );
  FT sq_rad  = squared_radius();
  Comparison_result vgl = CGAL_NTS compare( sq_dist, sq_rad );
  return  (vgl == LARGER ) ? ON_UNBOUNDED_SIDE :
                                   ( (vgl == SMALLER ) ?
                                          ON_BOUNDED_SIDE :
                                          ON_BOUNDARY);
}

template <class R>
CGAL_KERNEL_INLINE
bool
CircleH2<R>::has_on_bounded_side(const typename CircleH2<R>::Point_2& p) const
{
  FT sq_dist = squared_distance( p, center() );
  FT sq_rad  = squared_radius();
  return ( sq_dist < sq_rad );
}

template <class R>
CGAL_KERNEL_INLINE
bool
CircleH2<R>::has_on_unbounded_side(const typename CircleH2<R>::Point_2&p) const
{
  FT sq_dist = squared_distance( p, center() );
  FT sq_rad  = squared_radius();
  return ( sq_rad < sq_dist );
}

template <class R>
inline
bool
CircleH2<R>::is_degenerate() const
{ return ( squared_radius() == FT(0) ); }

template <class R>
CGAL_KERNEL_MEDIUM_INLINE
Bbox_2
CircleH2<R>::bbox() const
{
  Bbox_2 b = center().bbox();

  Interval_nt<> x (b.xmin(), b.xmax());
  Interval_nt<> y (b.ymin(), b.ymax());

  Interval_nt<> sqr = CGAL_NTS to_interval(squared_radius());
  Interval_nt<> r = CGAL::sqrt(sqr);
  Interval_nt<> minx = x-r;
  Interval_nt<> maxx = x+r;
  Interval_nt<> miny = y-r;
  Interval_nt<> maxy = y+r;

  return Bbox_2(minx.inf(), miny.inf(), maxx.sup(), maxy.sup());
}

template <class R>
CGAL_KERNEL_INLINE
CircleH2<R>
CircleH2<R>::
orthogonal_transform(const typename CircleH2<R>::Aff_transformation_2& t) const
{
  typename R::Vector_2 vec( RT(1), RT(0) );   // unit vector
  vec = vec.transform(t);                     // transformed
  FT  sq_scale = FT( vec*vec );               // squared scaling factor

  if ( t.is_even() )
  {
      return CircleH2<R>(t.transform(center() ),
                             sq_scale * squared_radius(),
                             orientation() );
  }
  else
  {
      return CircleH2<R>(t.transform(center() ),
                             sq_scale * squared_radius(),
                             CGAL::opposite( orientation()) );
  }
}

template <class R>
CGAL_KERNEL_INLINE
bool
CircleH2<R>::operator==(const CircleH2<R>& c) const
{
  return  ( center() == c.center() )
        &&( squared_radius() == c.squared_radius() )
        &&( orientation() == c.orientation() );
}

template <class R>
inline
bool
CircleH2<R>::operator!=(const CircleH2<R>& c) const
{ return !(*this == c); }

#ifndef CGAL_NO_OSTREAM_INSERT_CIRCLEH2
template < class R >
std::ostream &operator<<(std::ostream &os, const CircleH2<R> &c)
{
  switch(os.iword(IO::mode))
  {
    case IO::ASCII :
        os << c.center() << ' ' << c.squared_radius() << ' '
           << static_cast<int>(c.orientation());
        break;
    case IO::BINARY :
        os << c.center();
        write(os, c.squared_radius());
        write(os, static_cast<int>(c.orientation()));
        break;
    default:
        os << "CircleH2(" << c.center() <<  ", " << c.squared_radius() ;
        if (c.orientation() == CLOCKWISE) {
            os << ", clockwise)";
        } else if (c.orientation() == COUNTERCLOCKWISE) {
            os << ", counterclockwise)";
        } else {
            os << ", collinear)";
        }
        break;
  }
  return os;
}
#endif // CGAL_NO_OSTREAM_INSERT_CIRCLEH2

#ifndef CGAL_NO_ISTREAM_EXTRACT_CIRCLEH2
template < class R >
std::istream& operator>>(std::istream &is, CircleH2<R> &c)
{
  typename R::Point_2 center;
  typename R::FT squared_radius;
  int o;
  switch(is.iword(IO::mode))
  {
    case IO::ASCII :
        is >> center >> squared_radius >> o;
        break;
    case IO::BINARY :
        is >> center;
        read(is, squared_radius);
        is >> o;
        break;
    default:
        std::cerr << "" << std::endl;
        std::cerr << "Stream must be in ascii or binary mode" << std::endl;
        break;
  }
  c = CircleH2<R>(center, squared_radius, static_cast<Orientation>(o));
  return is;
}
#endif // CGAL_NO_ISTREAM_EXTRACT_CIRCLEH2

CGAL_END_NAMESPACE

#endif // CGAL_CIRCLEH2_H
