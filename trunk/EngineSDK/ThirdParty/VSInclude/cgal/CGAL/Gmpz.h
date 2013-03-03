// Copyright (c) 1999,2003,2004  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/Gmpz.h $
// $Id: Gmpz.h 31048 2006-05-05 21:07:49Z spion $
// 
//
// Author(s)     : Andreas Fabri, Stefan Schirra, Sylvain Pion


#ifndef CGAL_GMPZ_H
#define CGAL_GMPZ_H

#include <CGAL/basic.h>
#include <CGAL/Handle_for.h>
#include <CGAL/double.h>
#include <CGAL/Interval_nt.h>

#include <string>
#ifndef CGAL_CFG_NO_LOCALE
#  include <locale>
#else
#  include <cctype>
#endif

#include <gmp.h>
#include <mpfr.h>

#include <boost/operators.hpp>

#include <CGAL/Root_of_traits.h>
#include <CGAL/Root_of_2_fwd.h>

CGAL_BEGIN_NAMESPACE

// TODO : benchmark without ref-counting, and maybe give the possibility
// to select ref-counting or not, then... => template class.

// Wrapper around mpz_t to get the destructor call mpz_clear.
struct Gmpz_rep
{
// FIXME : bug if ~() is called before an mpz_init*() is called.
// not a problem in practice, but not nice.
// maybe the mpz_init_set* functions should move back to Gmpz_rep.
// But then we should use the Storage_traits::construct/get...

  mpz_t mpZ;

  Gmpz_rep() {}
  ~Gmpz_rep() { mpz_clear(mpZ); }

private:
  // Make sure it does not get accidentally copied.
  Gmpz_rep(const Gmpz_rep &);
  Gmpz_rep & operator= (const Gmpz_rep &);
};


class Gmpz
  : Handle_for<Gmpz_rep>,
    boost::ordered_euclidian_ring_operators1< Gmpz
  , boost::ordered_euclidian_ring_operators2< Gmpz, int
    > >
{
  typedef Handle_for<Gmpz_rep> Base;
public:
  typedef Tag_true  Has_gcd;
  typedef Tag_true  Has_division;
  typedef Tag_true  Has_sqrt;

  typedef Tag_true  Has_exact_ring_operations;
  typedef Tag_false Has_exact_division;
  typedef Tag_false Has_exact_sqrt;

  Gmpz()
  { mpz_init(mpz()); }

  Gmpz(const mpz_t z)
  { mpz_init_set(mpz(), z); }

  Gmpz(int i)
  { mpz_init_set_si(mpz(), i); }

  Gmpz(long l)
  { mpz_init_set_si(mpz(), l); }

  Gmpz(unsigned long l)
  { mpz_init_set_ui(mpz(), l); }

  Gmpz(double d)
  { mpz_init_set_d(mpz(), d); }

  Gmpz(const std::string& str, int base = 10)
  { mpz_init_set_str(mpz(), str.c_str(), base); }

  Gmpz operator-() const;

  Gmpz& operator+=(const Gmpz &z);
  Gmpz& operator+=(int i);

  Gmpz& operator-=(const Gmpz &z);
  Gmpz& operator-=(int i);

  Gmpz& operator*=(const Gmpz &z);
  Gmpz& operator*=(int i);

  Gmpz& operator%=(const Gmpz &z);

  Gmpz& operator/=(const Gmpz &z);
  Gmpz& operator/=(int i);

  size_t approximate_decimal_length() const;

  double to_double() const;
  Sign sign() const;

  const mpz_t & mpz() const { return Ptr()->mpZ; }
  mpz_t & mpz() { return ptr()->mpZ; }
};


inline
bool
operator<(const Gmpz &a, const Gmpz &b)
{ return mpz_cmp(a.mpz(), b.mpz()) < 0; }

inline
bool
operator==(const Gmpz &a, const Gmpz &b)
{ return mpz_cmp(a.mpz(), b.mpz()) == 0; }


// mixed operators.
inline
bool
operator<(const Gmpz &a, int b)
{ return mpz_cmp_si(a.mpz(), b) < 0; }

inline
bool
operator==(const Gmpz &a, int b)
{ return mpz_cmp_si(a.mpz(), b) == 0; }

inline
bool
operator>(const Gmpz &a, int b)
{ return mpz_cmp_si(a.mpz(), b) > 0; }


inline
Gmpz
Gmpz::operator-() const
{
    Gmpz Res;
    mpz_neg(Res.mpz(), mpz());
    return Res;
}


inline
Gmpz&
Gmpz::operator+=(const Gmpz &z)
{
    Gmpz Res;
    mpz_add(Res.mpz(), mpz(), z.mpz());
    swap(Res);
    return *this;
}

inline
Gmpz&
Gmpz::operator+=(int i)
{
    Gmpz Res;
    if (i >= 0)
        mpz_add_ui(Res.mpz(), mpz(), i);
    else
        mpz_sub_ui(Res.mpz(), mpz(), -i);
    swap(Res);
    return *this;
}

inline
Gmpz&
Gmpz::operator-=(const Gmpz &z)
{
    Gmpz Res;
    mpz_sub(Res.mpz(), mpz(), z.mpz());
    swap(Res);
    return *this;
}

inline
Gmpz&
Gmpz::operator-=(int i)
{
    Gmpz Res;
    if (i >= 0)
        mpz_sub_ui(Res.mpz(), mpz(), i);
    else
        mpz_add_ui(Res.mpz(), mpz(), -i);
    swap(Res);
    return *this;
}

inline
Gmpz&
Gmpz::operator*=(const Gmpz &z)
{
    Gmpz Res;
    mpz_mul(Res.mpz(), mpz(), z.mpz());
    swap(Res);
    return *this;
}

inline
Gmpz&
Gmpz::operator*=(int i)
{
    Gmpz Res;
    mpz_mul_si(Res.mpz(), mpz(), i);
    swap(Res);
    return *this;
}

inline
Gmpz&
Gmpz::operator/=(const Gmpz &z)
{
    CGAL_precondition(z != 0);
    Gmpz Res;
    mpz_tdiv_q(Res.mpz(), mpz(), z.mpz());
    swap(Res);
    return *this;
}

inline
Gmpz&
Gmpz::operator/=(int b)
{
    if (b>0)
    {
        Gmpz Res;
        mpz_tdiv_q_ui(Res.mpz(), mpz(), b);
        swap(Res);
        return *this;
    }
    return *this /= Gmpz(b);
}

inline
Gmpz&
Gmpz::operator%=(const Gmpz &z)
{
    Gmpz Res;
    mpz_tdiv_r(Res.mpz(), mpz(), z.mpz());
    swap(Res);
    return *this;
}


inline
double
Gmpz::to_double() const
{ return mpz_get_d(mpz()); }

inline
io_Operator
io_tag(const Gmpz&)
{ return io_Operator(); }

inline
Sign
Gmpz::sign() const
{ return static_cast<Sign>(mpz_sgn(mpz())); }

inline
double
to_double(const Gmpz &z)
{ return z.to_double(); }

inline
Sign
sign(const Gmpz &z)
{ return z.sign(); }

inline
bool
is_valid(const Gmpz &)
{ return true; }

inline
bool
is_finite(const Gmpz &)
{ return true; }

inline
Gmpz
sqrt(const Gmpz &z)
{
  Gmpz Res;
  mpz_sqrt(Res.mpz(), z.mpz());
  return Res;
}

inline
Gmpz
div(const Gmpz &z1, const Gmpz &z2)
{
  return z1 / z2;
}

inline
Gmpz
gcd(const Gmpz &z1, const Gmpz &z2)
{
  Gmpz Res;
  mpz_gcd(Res.mpz(), z1.mpz(), z2.mpz());
  return Res;
}

inline
Gmpz
gcd(const Gmpz &z, int i)
{
  if (i > 0)
  {
      Gmpz Res;
      mpz_gcd_ui(Res.mpz(), z.mpz(), i);
      return Res;
  }
  return gcd(z, Gmpz(i));
}

inline
Gmpz
exact_division(const Gmpz &z1, const Gmpz &z2)
{
  Gmpz Res;
  mpz_divexact(Res.mpz(), z1.mpz(), z2.mpz());
#ifdef CGAL_CHECK_POSTCONDITIONS
  mpz_t prod;
  mpz_init(prod);
  mpz_mul(prod, Res.mpz(), z2.mpz());
  CGAL_postcondition_msg(mpz_cmp(prod, z1.mpz()) == 0,
                         "exact_division failed\n");
  mpz_clear( prod);
#endif // CGAL_CHECK_POSTCONDITIONS
  return Res;
}

inline
size_t
Gmpz::approximate_decimal_length() const
{ return mpz_sizeinbase(mpz(),10); }

inline
std::ostream&
operator<<(std::ostream& os, const Gmpz &z)
{
  char *str = new char [mpz_sizeinbase(z.mpz(),10) + 2];
  str = mpz_get_str(str, 10, z.mpz());
  os << str ;
  delete[] str;
  return os;
}


inline
void gmpz_eat_white_space(std::istream &is)
{
  std::istream::int_type c;
  do {
    c= is.peek();
    if (c== std::istream::traits_type::eof()) return;
    else {
      std::istream::char_type cc= c;
      if (
#ifndef CGAL_CFG_NO_LOCALE
	     std::isspace(cc, std::locale::classic() )
#else
	     CGAL_CLIB_STD::isspace(cc)
#endif // CGAL_CFG_NO_LOCALE
	     ) {
      is.get();
      // since peek succeeded, this should too
      CGAL_assertion(!is.fail());
    } else {
      return;
    }
    }
  } while (true);
}


inline 
std::istream &
gmpz_new_read(std::istream &is, Gmpz &z)
{
  bool negative = false;
  const std::istream::char_type zero = '0';
  std::istream::int_type c;
  Gmpz r;
  std::ios::fmtflags old_flags = is.flags();

  is.unsetf(std::ios::skipws);
  gmpz_eat_white_space(is);
   
  c=is.peek();
  if (c=='-'){
    is.get();
    CGAL_assertion(!is.fail());
    negative=true;
    gmpz_eat_white_space(is);
    c=is.peek();
  }
  
  std::istream::char_type cc= c;
  
  if (c== std::istream::traits_type::eof()
      || 
#ifndef CGAL_CFG_NO_LOCALE
      !std::isdigit(cc, std::locale::classic() )
#else
      !std::isdigit(cc)
#endif // CGAL_CFG_NO_LOCALE) {
      ){
    is.setstate(std::ios_base::failbit);
  } else {
    CGAL_assertion(cc==c);
    r= cc-zero;
    is.get();
    CGAL_assertion(!is.fail());
    while (true) {
      c=is.peek();
      if (c== std::istream::traits_type::eof()) {
	break;
      }
      cc=c;
      if  (
#ifndef CGAL_CFG_NO_LOCALE
	   !std::isdigit(cc, std::locale::classic() )
#else
	   !std::isdigit(cc)
#endif // CGAL_CFG_NO_LOCALE
	   ) {
	break;
      }
      is.get();
      CGAL_assertion(!is.fail());
      CGAL_assertion(cc==c);
      r= r*10+(cc-zero);
    }  
  }
   
  is.flags(old_flags);
  if (!is.fail()) {
    if (negative) {
      z=-r;
    } else {
      z=r;
    }
  }
  return is;
}

/*inline
std::istream&
read_gmpz(std::istream& is, Gmpz &z) {
  bool negative = false;
  bool good = false;
  const int null = '0';
  char c;
  Gmpz tmp;
  std::ios::fmtflags old_flags = is.flags();

  is.unsetf(std::ios::skipws);
#ifndef CGAL_CFG_NO_LOCALE
  while (is.get(c) && std::isspace(c, std::locale::classic() ))
#else
  while (is.get(c) && CGAL_CLIB_STD::isspace(c))
#endif // CGAL_CFG_NO_LOCALE
  {}

  if (c == '-')
  {
        negative = true;
#ifndef CGAL_CFG_NO_LOCALE
        while (is.get(c) && std::isspace(c, std::locale::classic() ))
#else
        while (is.get(c) && CGAL_CLIB_STD::isspace(c))
#endif // CGAL_CFG_NO_LOCALE
        {}
  }
#ifndef CGAL_CFG_NO_LOCALE
  if (std::isdigit(c, std::locale::classic() ))
#else
  if (std::isdigit(c))
#endif // CGAL_CFG_NO_LOCALE
  {
        good = true;
        tmp = c - null;
#ifndef CGAL_CFG_NO_LOCALE
        while (is.get(c) && std::isdigit(c, std::locale::classic() ))
#else
        while (is.get(c) && std::isdigit(c))
#endif // CGAL_CFG_NO_LOCALE
        {
            tmp = 10*tmp + (c-null);
        }
  }
  if (is)
        is.putback(c);
  if (sign(tmp) != ZERO && negative)
      tmp = -tmp;
  if (good){
      z = tmp;
      }
   else
    is.clear(is.rdstate() | std::ios::failbit);

  is.flags(old_flags);
  return is;
  }*/

inline
std::istream&
operator>>(std::istream& is, Gmpz &z)
{
  return gmpz_new_read(is, z);
}

inline
std::pair<double, double>
to_interval (const Gmpz & z)
{
    mpfr_t x;
    mpfr_init2 (x, 53); /* Assume IEEE-754 */
    mpfr_set_z (x, z.mpz(), GMP_RNDD);
    double i = mpfr_get_d (x, GMP_RNDD); /* EXACT but can overflow */
    mpfr_set_z (x, z.mpz(), GMP_RNDU);
    double s = mpfr_get_d (x, GMP_RNDU); /* EXACT but can overflow */
    mpfr_clear (x);
    return std::pair<double, double>(i, s);
}

CGAL_END_NAMESPACE

#include <CGAL/Quotient.h>

CGAL_BEGIN_NAMESPACE

inline
double to_double(const Quotient<Gmpz>& quot)
{
  mpq_t  mpQ;
  mpq_init(mpQ);
  const Gmpz& n = quot.numerator();
  const Gmpz& d = quot.denominator();
  mpz_set(mpq_numref(mpQ), n.mpz());
  mpz_set(mpq_denref(mpQ), d.mpz());

  mpq_canonicalize(mpQ);

  double ret = mpq_get_d(mpQ);
  mpq_clear(mpQ);
  return ret;
}

CGAL_END_NAMESPACE

#include <CGAL/Root_of_2.h>

CGAL_BEGIN_NAMESPACE

class Gmpq;

template <>
struct Root_of_traits< CGAL::Gmpz >
{
  typedef CGAL::Gmpq               RootOf_1;
  typedef Root_of_2< CGAL::Gmpz >  RootOf_2;
};

inline
Root_of_2<Gmpz>
make_root_of_2(const Gmpz &a, const Gmpz &b, const Gmpz &c, bool smaller)
{
  CGAL_assertion( a != 0 );
  return Root_of_2<Gmpz>(a, b, c, smaller);
}


#if ! defined( CGAL_DONT_USE_LINK_PRAGMA) && defined( _MSC_VER )
    #pragma comment(lib, "gmp.lib")
    #pragma comment(lib, "mpfr.lib")
#endif 

CGAL_END_NAMESPACE

#endif // CGAL_GMPZ_H
