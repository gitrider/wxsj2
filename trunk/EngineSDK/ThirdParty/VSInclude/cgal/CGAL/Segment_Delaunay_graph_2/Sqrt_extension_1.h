// Copyright (c) 2003,2004,2005  INRIA Sophia-Antipolis (France) and
// Notre Dame University (U.S.A.).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Segment_Delaunay_graph_2/include/CGAL/Segment_Delaunay_graph_2/Sqrt_extension_1.h $
// $Id: Sqrt_extension_1.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>


#ifndef CGAL_SEGMENT_DELAUNAY_GRAPH_2_SQRT_EXTENSION_1_H
#define CGAL_SEGMENT_DELAUNAY_GRAPH_2_SQRT_EXTENSION_1_H

#include <CGAL/basic.h>
#include <CGAL/enum.h>

#include <iostream>

CGAL_BEGIN_NAMESPACE

template<class NT>
class Sqrt_extension_1;

template<class NT>
Sign sign(const Sqrt_extension_1<NT>&);

template<class NT>
Comparison_result compare(const Sqrt_extension_1<NT>&,
			  const Sqrt_extension_1<NT>&);

template<class NT>
class Sqrt_extension_1
{
private:
  NT x, y, r;

private:
  typedef Sqrt_extension_1<NT>   Self;

public:
  typedef NT                  FT;
  typedef NT                  RT;

  Sqrt_extension_1() : x(0), y(0), r(0) {}
  Sqrt_extension_1(int i) : x(i), y(0), r(0) {}
  Sqrt_extension_1(const NT& a) : x(a), y(0), r(0) {}
  Sqrt_extension_1(const NT& a, const NT& b, const NT& c)
    : x(a), y(b), r(c)
  {
    CGAL_exactness_assertion( !(CGAL::is_negative(r)) );
  }

  Sqrt_extension_1(const Sqrt_extension_1<NT>& other)
    : x(other.x), y(other.y), r(other.r) {}


  NT a() const { return x; }
  NT b() const { return y; }
  NT c() const { return r; }

  Self square() const
  {
    NT xy = x * y;

    return Self(CGAL::square(x) + CGAL::square(y) * r,
		xy + xy,
		r);
  }

  Sign sign() const
  {
    Sign sx = CGAL::sign(x);

    if ( CGAL::sign(r) == ZERO )  { return sx; }

    Sign sy = CGAL::sign(y);

    if ( sx == sy )  { return sx; }
    if ( sx == ZERO )  { return sy; }

    return Sign( sx * CGAL::compare( CGAL::square(x),
				     r * CGAL::square(y) )
		 );
  }

  const Self& operator+() const
  {
    return (*this);
  }

  Self operator-() const
  {
    return Self(-x, -y, r);
  }

  double to_double() const
  {
    // THIS MUST BE CHECK WITH SYLVAIN FOR CORRECTNESS
    double xd = CGAL::to_double(x);
    double yd = CGAL::to_double(y);
    double rd = CGAL::to_double(r);

    return (xd + yd * CGAL::sqrt(rd));
  }

  std::pair<double,double> to_interval() const
  {
    // THIS MUST BE CHECK WITH SYLVAIN FOR CORRECTNESS
    std::pair<double,double> x_ivl = CGAL::to_interval(x);
    std::pair<double,double> y_ivl = CGAL::to_interval(y);
    std::pair<double,double> r_ivl = CGAL::to_interval(r);

    std::pair<double,double> sqrt_r_ivl(CGAL::sqrt(r_ivl.first),
					CGAL::sqrt(r_ivl.second));

    std::pair<double,double>
      ivl(x_ivl.first + y_ivl.first * sqrt_r_ivl.first,
	  x_ivl.second + y_ivl.second * sqrt_r_ivl.second);

    return ivl;
  }
};


// operator *
template<class NT>
inline
Sqrt_extension_1<NT>
operator*(const Sqrt_extension_1<NT>& x, const NT& n)
{
  return Sqrt_extension_1<NT>(x.a() * n, x.b() * n, x.c());
}


template<class NT>
inline
Sqrt_extension_1<NT>
operator*(const NT& n, const Sqrt_extension_1<NT>& x)
{
  return (x * n);
}

template<class NT>
inline
Sqrt_extension_1<NT>
operator*(const Sqrt_extension_1<NT>& x, const Sqrt_extension_1<NT>& y)
{
  CGAL_exactness_precondition( CGAL::compare(x.c(), y.c()) == EQUAL );

  NT a = x.a() * y.a() + x.b() * y.b() * x.c();
  NT b = x.a() * y.b() + x.b() * y.a();

  return Sqrt_extension_1<NT>(a, b, x.c());
}


// operator +
template<class NT>
inline
Sqrt_extension_1<NT>
operator+(const Sqrt_extension_1<NT>& x, const NT& n)
{
  return Sqrt_extension_1<NT>(x.a() + n, x.b(), x.c());
}


template<class NT>
inline
Sqrt_extension_1<NT>
operator+(const NT& n, const Sqrt_extension_1<NT>& x)
{
  return (x + n);
}

template<class NT>
inline
Sqrt_extension_1<NT>
operator+(const Sqrt_extension_1<NT>& x, const Sqrt_extension_1<NT>& y)
{
  CGAL_exactness_precondition( CGAL::compare(x.c(), y.c()) == EQUAL );

  return Sqrt_extension_1<NT>(x.a() + y.a(), x.b() + y.b(), x.c());
}



// operator -
template<class NT>
inline
Sqrt_extension_1<NT>
operator-(const Sqrt_extension_1<NT>& x, const NT& n)
{
  return x + (-n);
}


template<class NT>
inline
Sqrt_extension_1<NT>
operator-(const NT& n, const Sqrt_extension_1<NT>& x)
{
  return -(x - n);
}

template<class NT>
inline
Sqrt_extension_1<NT>
operator-(const Sqrt_extension_1<NT>& x, const Sqrt_extension_1<NT>& y)
{
  return (x + (-y));
}


//=============================================================

template<class NT>
struct Number_type_traits< Sqrt_extension_1<NT> >
{

  static inline
  std::pair<double,double> to_interval(const Sqrt_extension_1<NT>& x) {
    return x.to_interval();
  }

  static inline bool is_positive(const Sqrt_extension_1<NT>& x) {
    return x.sign() == POSITIVE;
  }

  static inline bool is_negative(const Sqrt_extension_1<NT>& x) {
    return x.sign() == NEGATIVE;
  }

  static inline bool is_zero(const Sqrt_extension_1<NT>& x) {
    return x.sign() == ZERO;
  }

  static inline Sign sign(const Sqrt_extension_1<NT>& x) {
    return x.sign();
  }

  static inline Sqrt_extension_1<NT> square(const Sqrt_extension_1<NT>& x) {
    return x.square();
  }

  static inline
  Comparison_result compare(const Sqrt_extension_1<NT>& x,
			    const Sqrt_extension_1<NT>& y) {
    CGAL_exactness_precondition( CGAL::compare(x.c(), y.c()) == EQUAL );

    //  Sign s = CGAL::sign(x - y);
    Sign s = (x - y).sign();

    if ( s == ZERO ) { return EQUAL; }
    return (s == POSITIVE) ? LARGER : SMALLER;
  }

  static inline double to_double(const Sqrt_extension_1<NT>& x) {
    return x.to_double();
  }
};

template<class NT>
inline
std::pair<double,double>
to_interval(const Sqrt_extension_1<NT>& x)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::to_interval(x);
}


template<class NT>
inline
bool
is_positive(const Sqrt_extension_1<NT>& x)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::is_positive(x);
}

template<class NT>
inline
bool
is_negative(const Sqrt_extension_1<NT>& x)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::is_negative(x);
}

template<class NT>
inline
bool
is_zero(const Sqrt_extension_1<NT>& x)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::is_zero(x);
}


template<class NT>
inline
Sign
sign(const Sqrt_extension_1<NT>& x)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::sign(x);
}

template<class NT>
inline
Sqrt_extension_1<NT>
square(const Sqrt_extension_1<NT>& x)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::square(x);
}

template<class NT>
inline
Comparison_result
compare(const Sqrt_extension_1<NT>& x, const Sqrt_extension_1<NT>& y)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::compare(x, y);
}

template<class NT>
inline
double
to_double(const Sqrt_extension_1<NT>& x)
{
  return Number_type_traits< Sqrt_extension_1<NT> >::to_double(x);
}

// operator <<
template<class Stream, class NT>
inline
Stream&
operator<<(Stream& os, const Sqrt_extension_1<NT>& x)
{
  os << "(" << x.a()  << ")+(" << x.b() << ") sqrt{" << x.c() << "}";
  return os;
}



CGAL_END_NAMESPACE



#endif // CGAL_SEGMENT_DELAUNAY_GRAPH_2_SQUARE_ROOT_1_H
