// Copyright (c) 2002,2003  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/gmpxx.h $
// $Id: gmpxx.h 30047 2006-04-06 14:09:35Z spion $
// 
//
// Author(s)     : Sylvain Pion
 
#ifndef CGAL_GMPXX_H
#define CGAL_GMPXX_H

#include <CGAL/basic.h>
#include <CGAL/Number_type_traits.h>
#include <CGAL/Interval_nt.h>
#include <utility>

#include <gmpxx.h>
#include <mpfr.h>

#include <CGAL/Root_of_traits.h>
#include <CGAL/Root_of_2.h>

// This file gathers the necessary adaptors so that the following
// C++ number types that come with GMP can be used by CGAL :
// - mpz_class
// - mpq_class

// - mpf_class support is commented out until to_interval() is implemented.
//   It is probably not very useful with CGAL anyway.

// Note that GMP++ use the expression template mechanism, which makes things
// a little bit complicated in order to make square(x+y) work for example.
// Reading gmpxx.h shows that ::__gmp_expr<T, T> is the mp[zqf]_class proper,
// while ::__gmp_expr<T, U> is the others "expressions".

CGAL_BEGIN_NAMESPACE

template <>
struct Number_type_traits<mpz_class> {
  typedef Tag_false Has_gcd;
  typedef Tag_true  Has_division;
  typedef Tag_true  Has_sqrt;

  typedef Tag_true  Has_exact_ring_operations;
  typedef Tag_false Has_exact_division;
  typedef Tag_false Has_exact_sqrt;
};

template <>
struct Number_type_traits<mpq_class> {
  typedef Tag_false Has_gcd;
  typedef Tag_true  Has_division;
  typedef Tag_false Has_sqrt;

  typedef Tag_true  Has_exact_ring_operations;
  typedef Tag_true  Has_exact_division;
  typedef Tag_false Has_exact_sqrt;
};

template <>
struct Rational_traits<mpq_class> {
  typedef mpz_class RT;
  RT numerator   (const mpq_class & r) const { return r.get_num(); }
  RT denominator (const mpq_class & r) const { return r.get_den(); }

  mpq_class make_rational(const RT & n, const RT & d) const
  { return mpq_class(n, d); } 
  mpq_class make_rational(const mpq_class & n, const mpq_class & d) const
  { return n / d; } 
};

template < typename T, typename U >
inline
::__gmp_expr<T, T>
sqrt(const ::__gmp_expr<T, U> &e)
{
    return ::sqrt(e);
}

template < typename T, typename U >
inline
double
to_double(const ::__gmp_expr<T, U> & e)
{ return ::__gmp_expr<T, T>(e).get_d(); }

template < typename T, typename U >
inline
bool
is_finite(const ::__gmp_expr<T, U> &)
{ return true; }

template < typename T, typename U >
inline
bool
is_valid(const ::__gmp_expr<T, U> &)
{ return true; }

template < typename T, typename U >
inline
io_Operator
io_tag(const ::__gmp_expr<T, U> &)
{ return io_Operator(); }

template < typename T, typename U >
std::pair<double,double>
to_interval (const ::__gmp_expr<T, U> & z)
{
  // Calls the functions below after dealing with the expression template.
  return to_interval(::__gmp_expr<T, T>(z));
}

inline
std::pair<double, double>
to_interval (const mpz_class & z)
{
  mpfr_t x;
  mpfr_init2 (x, 53); /* Assume IEEE-754 */
  mpfr_set_z (x, z.get_mpz_t(), GMP_RNDD);
  double i = mpfr_get_d (x, GMP_RNDD); /* EXACT but can overflow */
  mpfr_set_z (x, z.get_mpz_t(), GMP_RNDU);
  double s = mpfr_get_d (x, GMP_RNDU); /* EXACT but can overflow */
  mpfr_clear (x);
  return std::pair<double, double>(i, s);
}

inline
std::pair<double, double>
to_interval (const mpq_class & q)
{
  mpfr_t x;
  mpfr_init2 (x, 53); /* Assume IEEE-754 */
  mpfr_set_q (x, q.get_mpq_t(), GMP_RNDD);
  double i = mpfr_get_d (x, GMP_RNDD); /* EXACT but can overflow */
  mpfr_set_q (x, q.get_mpq_t(), GMP_RNDU);
  double s = mpfr_get_d (x, GMP_RNDU); /* EXACT but can overflow */
  mpfr_clear (x);
  return std::pair<double, double>(i, s);
}

// These are necessary due to expression-templates.
template < typename T, typename U >
inline
::__gmp_expr<T, T>
abs(const ::__gmp_expr<T, U>& x) { return ::abs(x); }

template < typename T, typename U >
inline
::__gmp_expr<T, T>
square(const ::__gmp_expr<T, U>& x) { return x*x; }

template < typename T, typename U >
inline
Sign
sign(const ::__gmp_expr<T, U> & e)
{ return (Sign) ::sgn(e); }

template < typename T, typename U1, typename U2 >
inline
Comparison_result
compare(const ::__gmp_expr<T, U1> & e1,
        const ::__gmp_expr<T, U2> & e2)
{
  // cmp returns any int value, not just -1/0/1...
  return (Comparison_result) CGAL_NTS sign(::cmp(e1, e2));
}

template < typename T, typename U >
inline
bool
is_zero(const ::__gmp_expr<T, U> & e)
{ return ::sgn(e) == 0; }

template < typename T, typename U >
inline
bool
is_one(const ::__gmp_expr<T, U> & e)
{ return e == 1; }

template < typename T, typename U >
inline
bool
is_positive(const ::__gmp_expr<T, U> & e)
{ return ::sgn(e) > 0; }

template < typename T, typename U >
inline
bool
is_negative(const ::__gmp_expr<T, U> & e)
{ return ::sgn(e) < 0; }


namespace CGALi {

inline
Root_of_2< ::mpz_class >
make_root_of_2_gmpxx(const ::mpz_class & a,
                     const ::mpz_class & b,
                     const ::mpz_class & c,
                     bool d)
{
  return Root_of_2< ::mpz_class >(a, b, c, d);
}

inline
Root_of_2< ::mpz_class >
make_root_of_2_gmpxx(const ::mpq_class & a,
                     const ::mpq_class & b,
                     const ::mpq_class & c,
                     bool d)
{
    typedef CGAL::Rational_traits< ::mpq_class > Rational;

    Rational r;
    CGAL_assertion( r.denominator(a) > 0 );
    CGAL_assertion( r.denominator(b) > 0 );
    CGAL_assertion( r.denominator(c) > 0 );

/*   const RT lcm = ( r.denominator(a) * r.denominator(b) * r.denominator(c)          )/
               ( gcd( r.denominator(a), gcd(r.denominator(b), r.denominator(c)) ) );

      RT a_ = r.numerator(a) * ( lcm / r.denominator(a) );
      RT b_ = r.numerator(b) * ( lcm / r.denominator(b) );
      RT c_ = r.numerator(c) * ( lcm / r.denominator(c) );
*/
    ::mpz_class a_ = r.numerator(a) * r.denominator(b) * r.denominator(c);
    ::mpz_class b_ = r.numerator(b) * r.denominator(a) * r.denominator(c);
    ::mpz_class c_ = r.numerator(c) * r.denominator(a) * r.denominator(b);

    return Root_of_2< ::mpz_class >(a, b, c, d);
}

} // CGALi

template < typename T, typename U1, typename U2, typename U3 >
inline
typename Root_of_traits< ::__gmp_expr<T, T> >::RootOf_2
make_root_of_2(const ::__gmp_expr< T, U1> & a,
               const ::__gmp_expr< T, U2> & b,
               const ::__gmp_expr< T, U3> & c,
               bool d)
{
  return CGALi::make_root_of_2_gmpxx(a, b, c, d);
}

template < typename T, typename U >
struct Root_of_traits< ::__gmp_expr<T, U> >
{
  typedef ::mpq_class               RootOf_1;
  typedef Root_of_2< ::mpz_class >  RootOf_2;
};

CGAL_END_NAMESPACE

// XXX : These seem necessary.
// I don't know why doing them in namespace CGAL is not enough.
// using CGAL::to_double;
// using CGAL::is_valid;

#endif // CGAL_GMPXX_H
