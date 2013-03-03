// Copyright (c) 1999-2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Lazy_exact_nt.h $
// $Id: Lazy_exact_nt.h 30667 2006-04-19 16:56:12Z glisse $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_LAZY_EXACT_NT_H
#define CGAL_LAZY_EXACT_NT_H

#include <CGAL/basic.h>
#include <CGAL/tags.h>
#include <CGAL/number_utils.h>
#include <CGAL/number_utils_classes.h>
#include <CGAL/Number_type_traits.h>
#include <CGAL/Interval_nt.h>
#include <CGAL/Handle.h>
#include <CGAL/Filtered_exact.h> // to get the overloaded predicates.
#include <CGAL/Kernel/mpl.h>
#include <CGAL/NT_converter.h>
#include <CGAL/Binary_operator_result.h>
#include <CGAL/Lazy_exact_nt_fwd.h>

#include <boost/operators.hpp>

#include <CGAL/Root_of_traits.h>

/*
 * This file contains the definition of the number type Lazy_exact_nt<ET>,
 * where ET is an exact number type (must provide the exact operations needed).
 *
 * Lazy_exact_nt<ET> provides a DAG-based lazy evaluation, like LEDA's real,
 * Core's Expr, LEA's lazy rationals...
 *
 * The values are first approximated using Interval_base.
 * The exactness is provided when needed by ET.
 *
 * Lazy_exact_nt<ET> is just a handle to the abstract base class
 * Lazy_exact_rep which has pure virtual methods .approx() and .exact().
 * From this class derives one class per operation, with one constructor.
 *
 * The DAG is managed by :
 * - Handle and Rep.
 * - virtual functions to denote the various operators (instead of an enum).
 *
 * Other packages with vaguely similar design : APU, MetaCGAL, LOOK.
 */

/*
 * TODO :
 * - Generalize it for constructions at the kernel level.
 * - Add mixed operations with ET too ?
 * - Interval refinement functionnality ?
 * - Separate the handle and the representation(s) in 2 files (?)
 *   maybe not a good idea, better if everything related to one operation is
 *   close together.
 * - Add a CT template parameter like Filtered_exact_nt<> ?
 * - Add a string constant to provide an expression string (a la MetaCGAL) ?
 *   // virtual ostream operator<<() const = 0; // or string, like Core ?
 * - Have a template-expression (?) thing that evaluates a temporary element,
 *   and allocates stuff in memory only when really needs to convert to the
 *   NT.  (cf gmp++, and maybe other things, Blitz++, Synaps...)
 */

/*
 * Interface of the rep classes:
 * - .approx()      returns Interval_nt<> (assumes rounding=nearest).
 *                  [ only called from the handle, and declared in the base ]
 * - .exact()       returns ET, if not already done, computes recursively
 *
 * - .rafine_approx()   ??
 */

CGAL_BEGIN_NAMESPACE

#ifdef CGAL_LAZY_KERNEL_DEBUG 
template <class T>
void
print_at(std::ostream& os, const T& at)
{
  os << at;
}

template <class T>
void
print_at(std::ostream& os, const std::vector<T>& at)
{
  os << "std::vector";
}

template <>
void
print_at(std::ostream& os, const Object& o)
{
  os << "CGAL::Object";
}

template <class T1, class T2>
void
print_at(std::ostream& os, const std::pair<T1,T2> & at)
{
  os << "[ " << at.first << " | " << at.second << " ]" << std::endl ;
} 


template <class ET>
class Lazy_exact_nt;

template <typename ET>
inline
void
print_dag(const Lazy_exact_nt<ET>& l, std::ostream& os, int level=0)
{
  l.print_dag(os, level);
}

inline
void
print_dag(double d, std::ostream& os, int level)
{
  for(int i = 0; i < level; i++){
    os << "    ";
  }
  os << d << std::endl;
}


void
msg(std::ostream& os, int level, char* s)
  {
    int i;
    for(i = 0; i < level; i++){
      os << "    ";
    }
    os << s << std::endl;
  }

inline
void
print_dag(const Null_vector& nv, std::ostream& os, int level)
{
  for(int i = 0; i < level; i++){
    os << "    ";
  }
  os << "Null_vector" << std::endl;
}

inline
void
print_dag(const Origin& nv, std::ostream& os, int level)
{
  for(int i = 0; i < level; i++){
    os << "    ";
  }
  os << "Origin" << std::endl;
}

#endif

// Abstract base class for lazy numbers and lazy objects
template <typename AT_, typename ET, typename E2A>
struct Lazy_construct_rep : public Rep
{
  typedef AT_ AT;

  AT at;
  mutable ET *et;

  Lazy_construct_rep ()
      : at(), et(NULL) {}

  Lazy_construct_rep (const AT& a)
      : at(a), et(NULL) 
  {}

  Lazy_construct_rep (const AT& a, const ET& e)
      : at(a), et(new ET(e)) 
  {}

private:
  Lazy_construct_rep (const Lazy_construct_rep&) { std::abort(); } // cannot be copied.
public:

  const AT& approx() const
  {
      return at;
  }

  AT& approx()
  {
      return at;
  }

  const ET & exact() const
  {
    if (et==NULL)
      update_exact();
    return *et;
  }
  
  ET & exact()
  {
    if (et==NULL)
      update_exact();
    return *et;
  }

#ifdef CGAL_LAZY_KERNEL_DEBUG
  void print_at_et(std::ostream& os, int level) const
  {
    for(int i = 0; i < level; i++){
      os << "    ";
    }
    os << "Approximation: ";
    CGAL::print_at(os, at);
    os << std::endl;
    if(! is_lazy()){
      for(int i = 0; i < level; i++){
	os << "    ";
      }
      os << "Exact: ";
      CGAL::print_at(os, *et);
      os << std::endl;
    }
  }

  virtual void print_dag(std::ostream& os, int level) const {}
#endif

  bool is_lazy() const { return et == NULL; }
  virtual void update_exact() = 0;
  virtual int depth() const  { return 1; }
  virtual ~Lazy_construct_rep () { delete et; };
};

// Abstract base representation class for lazy numbers
template <typename ET>
struct Lazy_exact_rep : public Lazy_construct_rep<Interval_nt<false>,
                                                  ET, To_interval<ET> >
{
  typedef Lazy_construct_rep<Interval_nt<false>, ET, To_interval<ET> > Base;

  Lazy_exact_rep (const Interval_nt<false> & i)
      : Base(i) {}

#ifdef CGAL_LAZY_KERNEL_DEBUG
  void
  print_dag(std::ostream& os, int level) const
  {
    this->print_at_et(os, level);
  }
#endif  

private:
  Lazy_exact_rep (const Lazy_exact_rep&) { std::abort(); } // cannot be copied.

};

// int constant
template <typename ET>
struct Lazy_exact_Int_Cst : public Lazy_exact_rep<ET>
{
  Lazy_exact_Int_Cst (int i)
      : Lazy_exact_rep<ET>(double(i)) {}

  void update_exact()  { this->et = new ET((int)this->approx().inf()); }

};

// double constant
template <typename ET>
struct Lazy_exact_Cst : public Lazy_exact_rep<ET>
{
  Lazy_exact_Cst (double d)
      : Lazy_exact_rep<ET>(d) {}

  void update_exact()  { this->et = new ET(this->approx().inf()); }
};

// Exact constant
template <typename ET>
struct Lazy_exact_Ex_Cst : public Lazy_exact_rep<ET>
{
  Lazy_exact_Ex_Cst (const ET & e)
      : Lazy_exact_rep<ET>(to_interval(e))
  {
    this->et = new ET(e);
  }

  void update_exact()  { CGAL_assertion(false); }
};

// Construction from a Lazy_exact_nt<ET1> (which keeps the lazyness).
template <typename ET, typename ET1>
class Lazy_lazy_exact_Cst : public Lazy_exact_rep<ET>
{
  Lazy_exact_nt<ET1> l;

public:

  Lazy_lazy_exact_Cst (const Lazy_exact_nt<ET1> &x)
      : Lazy_exact_rep<ET>(x.approx()), l(x) {}

  void update_exact()
  {
    this->et = new ET(l.exact());
    this->approx() = l.approx();
    prune_dag();
  }
  int depth() const { return l.depth() + 1; }
  void prune_dag() { l = Lazy_exact_nt<ET1>::zero(); }
};


// Unary  operations: abs, sqrt, square.
// Binary operations: +, -, *, /, min, max.

// Base unary operation
template <typename ET>
struct Lazy_exact_unary : public Lazy_exact_rep<ET>
{
  Lazy_exact_nt<ET> op1;

  Lazy_exact_unary (const Interval_nt<false> &i, const Lazy_exact_nt<ET> &a)
      : Lazy_exact_rep<ET>(i), op1(a) {}

  int depth() const { return op1.depth() + 1; }
  void prune_dag() { op1 = Lazy_exact_nt<ET>::zero(); }

#ifdef CGAL_LAZY_KERNEL_DEBUG
  void
  print_dag(std::ostream& os, int level) const
  {
    this->print_at_et(os, level);
    if(this->is_lazy()){
      CGAL::msg(os, level, "Unary number operator:");
      CGAL::print_dag(op1, os, level+1);
    }
  }
#endif
};

// Base binary operation
template <typename ET, typename ET1 = ET, typename ET2 = ET>
struct Lazy_exact_binary : public Lazy_exact_rep<ET>
{
  Lazy_exact_nt<ET1> op1;
  Lazy_exact_nt<ET2> op2;

  Lazy_exact_binary (const Interval_nt<false> &i,
		     const Lazy_exact_nt<ET1> &a, const Lazy_exact_nt<ET2> &b)
      : Lazy_exact_rep<ET>(i), op1(a), op2(b) {}

  int depth() const { return std::max(op1.depth(), op2.depth()) + 1; }
  void prune_dag()
  {
    op1 = Lazy_exact_nt<ET1>::zero();
    op2 = Lazy_exact_nt<ET2>::zero();
  }

#ifdef CGAL_LAZY_KERNEL_DEBUG
  void
  print_dag(std::ostream& os, int level) const
  {
    this->print_at_et(os, level);
    if(this->is_lazy()){
      CGAL::msg(os, level, "Binary number operator:");
      CGAL::print_dag(op1, os, level+1);
      CGAL::print_dag(op2, os, level+1);
    }
  }
#endif
};

// Here we could use a template class for all operations (STL provides
// function objects plus, minus, multiplies, divides...).  But it would require
// a template template parameter, and GCC 2.95 seems to crash easily with them.

#ifndef CGAL_CFG_COMMA_BUG
// Macro for unary operations
#define CGAL_LAZY_UNARY_OP(OP, NAME)                                     \
template <typename ET>                                                   \
struct NAME : public Lazy_exact_unary<ET>                                \
{                                                                        \
  typedef typename Lazy_exact_unary<ET>::AT::Protector P;                \
  NAME (const Lazy_exact_nt<ET> &a)                                      \
      : Lazy_exact_unary<ET>((P(), OP(a.approx())), a) {}                \
                                                                         \
  void update_exact()                                                    \
  {                                                                      \
    this->et = new ET(OP(this->op1.exact()));                            \
    if (!this->approx().is_point())                                      \
      this->approx() = CGAL::to_interval(*(this->et));                   \
    this->prune_dag();                                                   \
   }                                                                     \
};
#else
// Macro for unary operations
#define CGAL_LAZY_UNARY_OP(OP, NAME)                                     \
template <typename ET>                                                   \
struct NAME : public Lazy_exact_unary<ET>                                \
{                                                                        \
  typedef typename Lazy_exact_unary<ET>::AT::Protector P;                \
  NAME (const Lazy_exact_nt<ET> &a)                                      \
      : Lazy_exact_unary<ET>(a.approx() /* dummy value */, a)            \
  { P p; this->approx() = OP(a.approx()); }                              \
                                                                         \
  void update_exact()                                                    \
  {                                                                      \
    this->et = new ET(OP(this->op1.exact()));                            \
    if (!this->approx().is_point())                                      \
      this->approx() = CGAL::to_interval(*(this->et));                   \
    this->prune_dag();                                                   \
  }                                                                      \
};
#endif

CGAL_LAZY_UNARY_OP(CGAL::opposite,  Lazy_exact_Opp)
CGAL_LAZY_UNARY_OP(CGAL_NTS abs,    Lazy_exact_Abs)
CGAL_LAZY_UNARY_OP(CGAL_NTS square, Lazy_exact_Square)
CGAL_LAZY_UNARY_OP(CGAL::sqrt,      Lazy_exact_Sqrt)

#ifndef CGAL_CFG_COMMA_BUG
// A macro for +, -, * and /
#define CGAL_LAZY_BINARY_OP(OP, NAME)                                    \
template <typename ET, typename ET1 = ET, typename ET2 = ET>             \
struct NAME : public Lazy_exact_binary<ET, ET1, ET2>                     \
{                                                                        \
  typedef typename Lazy_exact_binary<ET,ET1,ET2>::AT::Protector P;	 \
  NAME (const Lazy_exact_nt<ET1> &a, const Lazy_exact_nt<ET2> &b)        \
    : Lazy_exact_binary<ET, ET1, ET2>((P(), a.approx() OP b.approx()), a, b) {} \
                                                                         \
  void update_exact()                                                    \
  {                                                                      \
    this->et = new ET(this->op1.exact() OP this->op2.exact());           \
    if (!this->approx().is_point())                                      \
      this->approx() = CGAL::to_interval(*(this->et));                   \
    this->prune_dag();                                                   \
   }                                                                     \
};
#else
// A macro for +, -, * and /
#define CGAL_LAZY_BINARY_OP(OP, NAME)                                    \
template <typename ET, typename ET1 = ET, typename ET2 = ET>             \
struct NAME : public Lazy_exact_binary<ET, ET1, ET2>                     \
{                                                                        \
  typedef typename Lazy_exact_binary<ET, ET1, ET2>::AT::Protector P;     \
  NAME (const Lazy_exact_nt<ET1> &a, const Lazy_exact_nt<ET2> &b)        \
    : Lazy_exact_binary<ET, ET1, ET2>(a.approx() /* dummy value */, a, b)\
  {P p; this->approx() = a.approx() OP b.approx(); }                     \
                                                                         \
  void update_exact()                                                    \
  {                                                                      \
    this->et = new ET(this->op1.exact() OP this->op2.exact());           \
    if (!this->approx().is_point())                                      \
      this->approx() = CGAL::to_interval(*(this->et));                   \
    this->prune_dag();                                                   \
   }                                                                     \
};
#endif

CGAL_LAZY_BINARY_OP(+, Lazy_exact_Add)
CGAL_LAZY_BINARY_OP(-, Lazy_exact_Sub)
CGAL_LAZY_BINARY_OP(*, Lazy_exact_Mul)
CGAL_LAZY_BINARY_OP(/, Lazy_exact_Div)

// Minimum
template <typename ET>
struct Lazy_exact_Min : public Lazy_exact_binary<ET>
{
  Lazy_exact_Min (const Lazy_exact_nt<ET> &a, const Lazy_exact_nt<ET> &b)
    : Lazy_exact_binary<ET>(min(a.approx(), b.approx()), a, b) {}

  void update_exact()
  {
    this->et = new ET(min(this->op1.exact(), this->op2.exact()));
    if (!this->approx().is_point()) this->approx() = CGAL::to_interval(*(this->et));
    this->prune_dag();
  }
};

// Maximum
template <typename ET>
struct Lazy_exact_Max : public Lazy_exact_binary<ET>
{
  Lazy_exact_Max (const Lazy_exact_nt<ET> &a, const Lazy_exact_nt<ET> &b)
    : Lazy_exact_binary<ET>(max(a.approx(), b.approx()), a, b) {}

  void update_exact()
  {
    this->et = new ET(max(this->op1.exact(), this->op2.exact()));
    if (!this->approx().is_point()) this->approx() = CGAL::to_interval(*(this->et));
    this->prune_dag();
  }
};

#define CGAL_int(T)    typename First_if_different<int,    T>::Type
#define CGAL_double(T) typename First_if_different<double, T>::Type

// The real number type, handle class
template <typename ET>
class Lazy_exact_nt
  : public Handle
  , boost::ordered_euclidian_ring_operators2< Lazy_exact_nt<ET>, int >
  , boost::ordered_euclidian_ring_operators2< Lazy_exact_nt<ET>, double >
{
  typedef Lazy_exact_nt<ET> Self;
  typedef Lazy_construct_rep<Interval_nt<false>, ET, To_interval<ET> > Self_rep;

public :

  typedef typename Number_type_traits<ET>::Has_gcd      Has_gcd;
  typedef typename Number_type_traits<ET>::Has_division Has_division;
  typedef typename Number_type_traits<ET>::Has_sqrt     Has_sqrt;

  typedef typename Number_type_traits<ET>::Has_exact_sqrt Has_exact_sqrt;
  typedef typename Number_type_traits<ET>::Has_exact_division
                                                        Has_exact_division;
  typedef typename Number_type_traits<ET>::Has_exact_ring_operations
                                                     Has_exact_ring_operations;

  Lazy_exact_nt (Self_rep *r)
  { PTR = r; }

  Lazy_exact_nt ()
    : Handle(zero()) {}

  Lazy_exact_nt (const CGAL_int(ET) & i)
  { PTR = new Lazy_exact_Int_Cst<ET>(i); }

  Lazy_exact_nt (const CGAL_double(ET) & d)
  { PTR = new Lazy_exact_Cst<ET>(d); }

  Lazy_exact_nt (const ET & e)
  { PTR = new Lazy_exact_Ex_Cst<ET>(e); }

  template <class ET1>
  Lazy_exact_nt (const Lazy_exact_nt<ET1> &x)
  { PTR = new Lazy_lazy_exact_Cst<ET, ET1>(x); }

  Self operator- () const
  { return new Lazy_exact_Opp<ET>(*this); }

  Self & operator+=(const Self& b)
  { return *this = new Lazy_exact_Add<ET>(*this, b); }

  Self & operator-=(const Self& b)
  { return *this = new Lazy_exact_Sub<ET>(*this, b); }

  Self & operator*=(const Self& b)
  { return *this = new Lazy_exact_Mul<ET>(*this, b); }

  Self & operator/=(const Self& b)
  {
    CGAL_precondition(b != 0);
    return *this = new Lazy_exact_Div<ET>(*this, b);
  }

  // Mixed operators. (could be optimized ?)
  Self & operator+=(CGAL_int(ET) b)
  { return *this = new Lazy_exact_Add<ET>(*this, b); }

  Self & operator-=(CGAL_int(ET) b)
  { return *this = new Lazy_exact_Sub<ET>(*this, b); }

  Self & operator*=(CGAL_int(ET) b)
  { return *this = new Lazy_exact_Mul<ET>(*this, b); }

  Self & operator/=(CGAL_int(ET) b)
  {
    CGAL_precondition(b != 0);
    return *this = new Lazy_exact_Div<ET>(*this, b);
  }

  Self & operator+=(CGAL_double(ET) b)
  { return *this = new Lazy_exact_Add<ET>(*this, b); }

  Self & operator-=(CGAL_double(ET) b)
  { return *this = new Lazy_exact_Sub<ET>(*this, b); }

  Self & operator*=(CGAL_double(ET) b)
  { return *this = new Lazy_exact_Mul<ET>(*this, b); }

  Self & operator/=(CGAL_double(ET) b)
  {
    CGAL_precondition(b != 0);
    return *this = new Lazy_exact_Div<ET>(*this, b);
  }

  // % kills filtering
  Self & operator%=(const Self& b)
  {
    CGAL_precondition(b != 0);
    ET res = exact();
    res %= b.exact();
    return *this = Lazy_exact_nt<ET>(res);
  }

  Self & operator%=(int b)
  {
    CGAL_precondition(b != 0);
    ET res = exact();
    res %= b;
    return *this = Lazy_exact_nt<ET>(res);
  }

  Interval_nt<true> interval() const
  {
    const Interval_nt<false>& i = approx();
    return Interval_nt<true>(i.inf(), i.sup());
  }

  const Interval_nt<false>& approx() const
  { return ptr()->approx(); }

  Interval_nt_advanced approx_adv() const
  { return ptr()->approx(); }

  const ET & exact() const
  { return ptr()->exact(); }

  int depth() const
  { return ptr()->depth(); }

  void
  print_dag(std::ostream& os, int level) const
  {
    ptr()->print_dag(os, level);
  }

  static const double & get_relative_precision_of_to_double()
  {
      return relative_precision_of_to_double;
  }

  static void set_relative_precision_of_to_double(const double & d)
  {
      CGAL_assertion(d > 0 && d < 1);
      relative_precision_of_to_double = d;
  }

  bool identical(const Self& b) const
  {
    return CGAL::identical(static_cast<const Handle &>(*this),
                           static_cast<const Handle &>(b));
  }

  template < typename T >
  bool identical(const T&) const
  { return false; }

  // We have a static variable for optimizing zero and default constructor.
  static const Self & zero()
  {
    static const Self z = new Lazy_exact_Int_Cst<ET>(0);
    return z;
  }

private:
  Self_rep * ptr() const { return (Self_rep*) PTR; }

  static double relative_precision_of_to_double;
};


template <typename ET>
double Lazy_exact_nt<ET>::relative_precision_of_to_double = 0.00001;


template <typename ET1, typename ET2>
bool
operator<(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{
  if (a.identical(b))
    return false;
  Uncertain<bool> res = a.approx() < b.approx();
  if (is_singleton(res))
    return res;
  return a.exact() < b.exact();
}

template <typename ET1, typename ET2>
bool
operator==(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{
  if (a.identical(b))
    return true;
  Uncertain<bool> res = a.approx() == b.approx();
  if (is_singleton(res))
    return res;
  return a.exact() == b.exact();
}

template <typename ET1, typename ET2>
inline
bool
operator>(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{ return b < a; }

template <typename ET1, typename ET2>
inline
bool
operator>=(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{ return ! (a < b); }

template <typename ET1, typename ET2>
inline
bool
operator<=(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{ return b >= a; }

template <typename ET1, typename ET2>
inline
bool
operator!=(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{ return ! (a == b); }


template <typename ET>
inline
Lazy_exact_nt<ET>
operator%(const Lazy_exact_nt<ET>& a, const Lazy_exact_nt<ET>& b)
{
  CGAL_precondition(b != 0);
  return Lazy_exact_nt<ET>(a) %= b;
}



// Mixed operators with int.
template <typename ET>
bool
operator<(const Lazy_exact_nt<ET>& a, int b)
{
  Uncertain<bool> res = a.approx() < b;
  if (is_singleton(res))
    return res;
  return a.exact() < b;
}

template <typename ET>
bool
operator>(const Lazy_exact_nt<ET>& a, int b)
{
  Uncertain<bool> res = b < a.approx();
  if (is_singleton(res))
    return res;
  return b < a.exact();
}

template <typename ET>
bool
operator==(const Lazy_exact_nt<ET>& a, int b)
{
  Uncertain<bool> res = b == a.approx();
  if (is_singleton(res))
    return res;
  return b == a.exact();
}


// Mixed operators
template < typename ET1, typename ET2 >
struct Binary_operator_result < Lazy_exact_nt<ET1>, Lazy_exact_nt<ET2> >
{
  typedef Lazy_exact_nt< typename Binary_operator_result<ET1, ET2>::type > type;
};

template <typename ET1, typename ET2>
Lazy_exact_nt< typename Binary_operator_result<ET1, ET2>::type >
operator+(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{
  return new Lazy_exact_Add<typename Binary_operator_result<ET1, ET2>::type,
                            ET1, ET2>(a, b);
}

template <typename ET1, typename ET2>
Lazy_exact_nt< typename Binary_operator_result<ET1, ET2>::type >
operator-(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{
  return new Lazy_exact_Sub<typename Binary_operator_result<ET1, ET2>::type,
                            ET1, ET2>(a, b);
}

template <typename ET1, typename ET2>
Lazy_exact_nt< typename Binary_operator_result<ET1, ET2>::type >
operator*(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{
  return new Lazy_exact_Mul<typename Binary_operator_result<ET1, ET2>::type,
                            ET1, ET2>(a, b);
}

template <typename ET1, typename ET2>
Lazy_exact_nt< typename Binary_operator_result<ET1, ET2>::type >
operator/(const Lazy_exact_nt<ET1>& a, const Lazy_exact_nt<ET2>& b)
{
  CGAL_precondition(b != 0);
  return new Lazy_exact_Div<typename Binary_operator_result<ET1, ET2>::type,
                            ET1, ET2>(a, b);
}


template <typename ET>
double
to_double(const Lazy_exact_nt<ET> & a)
{
    const Interval_nt<false>& app = a.approx();
    if (app.sup() == app.inf())
	return app.sup();

    // If it's precise enough, then OK.
    if ((app.sup() - app.inf())
	    < Lazy_exact_nt<ET>::get_relative_precision_of_to_double()
	      * std::max(std::fabs(app.inf()), std::fabs(app.sup())) )
        return CGAL::to_double(app);

    // Otherwise we trigger exact computation first,
    // which will refine the approximation.
    a.exact();
    return CGAL::to_double(a.approx());
}

template <typename ET>
inline
std::pair<double,double>
to_interval(const Lazy_exact_nt<ET> & a)
{
    return a.approx().pair();
}

template <typename ET>
inline
Sign
sign(const Lazy_exact_nt<ET> & a)
{
  Uncertain<Sign> res = sign(a.approx());
  if (is_singleton(res))
    return res;
  return CGAL_NTS sign(a.exact());
}

template <typename ET1, typename ET2>
inline
Comparison_result
compare(const Lazy_exact_nt<ET1> & a, const Lazy_exact_nt<ET2> & b)
{
  if (a.identical(b))
    return EQUAL;
  Uncertain<Comparison_result> res = compare(a.approx(), b.approx());
  if (is_singleton(res))
    return res;
  return CGAL_NTS compare(a.exact(), b.exact());
}

template <typename ET>
inline
Lazy_exact_nt<ET>
abs(const Lazy_exact_nt<ET> & a)
{ return new Lazy_exact_Abs<ET>(a); }

template <typename ET>
inline
Lazy_exact_nt<ET>
square(const Lazy_exact_nt<ET> & a)
{ return new Lazy_exact_Square<ET>(a); }

template <typename ET>
inline
Lazy_exact_nt<ET>
sqrt(const Lazy_exact_nt<ET> & a)
{
  CGAL_precondition(a >= 0);
  return new Lazy_exact_Sqrt<ET>(a);
}

template <typename ET>
inline
Lazy_exact_nt<ET>
min(const Lazy_exact_nt<ET> & a, const Lazy_exact_nt<ET> & b)
{ return new Lazy_exact_Min<ET>(a, b); }

template <typename ET>
inline
Lazy_exact_nt<ET>
max(const Lazy_exact_nt<ET> & a, const Lazy_exact_nt<ET> & b)
{ return new Lazy_exact_Max<ET>(a, b); }

// gcd kills filtering.
template <typename ET>
Lazy_exact_nt<ET>
gcd(const Lazy_exact_nt<ET>& a, const Lazy_exact_nt<ET>& b)
{
  return Lazy_exact_nt<ET>(CGAL_NTS gcd(a.exact(), b.exact()));
}

template <typename ET>
std::ostream &
operator<< (std::ostream & os, const Lazy_exact_nt<ET> & a)
{ return os << CGAL::to_double(a); }

template <typename ET>
std::istream &
operator>> (std::istream & is, Lazy_exact_nt<ET> & a)
{
  ET e;
  is >> e;
  a = e;
  return is;
}



template <typename ET>
inline
bool
is_finite(const Lazy_exact_nt<ET> & a)
{
  return is_finite(a.approx()) || is_finite(a.exact());
}

template <typename ET>
inline
bool
is_valid(const Lazy_exact_nt<ET> & a)
{
  return is_valid(a.approx()) || is_valid(a.exact());
}

template <typename ET>
inline
io_Operator
io_tag (const Lazy_exact_nt<ET>&)
{ return io_Operator(); }

template < typename ET >
struct NT_converter < Lazy_exact_nt<ET>, ET >
{
  const ET& operator()(const Lazy_exact_nt<ET> &a) const
  { return a.exact(); }
};

// Returns true if the value is representable by a double and to_double()
// would return it.  False means "don't know".
template < typename ET >
inline bool
fit_in_double(const Lazy_exact_nt<ET>& l, double& r)
{ return fit_in_double(l.approx(), r); }


// We create a type of new node in Lazy_exact_nt's DAG
// for the make_root_of_2() operation.

#if 0 // To be finished
template <typename ET >
struct Lazy_exact_ro2
  : public Lazy_exact_rep< typename Root_of_traits<ET>::RootOf_2 >
{
    typedef typename Root_of_traits<ET>::RootOf_2   RO2;
    typedef Lazy_exact_rep<RO2>                     Base;
    typedef typename Base::AT::Protector            P;


    mutable Lazy_exact_nt<ET> op1, op2, op3;
    bool smaller;

    Lazy_exact_ro2 (const Lazy_exact_nt<ET> &a,
                    const Lazy_exact_nt<ET> &b,
                    const Lazy_exact_nt<ET> &c, bool s)
#ifndef CGAL_CFG_COMMA_BUG
      : Base((P(), make_root_of_2(a.approx(), b.approx(), c.approx(), s))),
        op1(a), op2(b), op3(c), smaller(s) {}
#else
      : Base(a.approx() /* dummy value */, a),
        op1(a), op2(b), op3(c), smaller(s)
  {
    P p;
    this->approx() = make_root_of_2(a.approx(), b.approx(),
                                    c.approx(), s);
  }
#endif

    void update_exact()
    {
        this->et = new RO2(make_root_of_2(op1.exact(), op2.exact(),
                                          op3.exact(), smaller));

        if (!this->approx().is_point())
            this->at = CGAL::to_interval(*(this->et));
        this->prune_dag();

    }

    void prune_dag() const
    {
        op1 = op2 = op3 = Lazy_exact_nt<ET>::zero();
    }
};

template < typename ET >
inline
Lazy_exact_nt< typename Root_of_traits<ET>::RootOf_2 >
make_root_of_2( const Lazy_exact_nt<ET> &a,
                const Lazy_exact_nt<ET> &b,
                const Lazy_exact_nt<ET> &c, bool d)
{
    return new Lazy_exact_ro2<ET>(a, b, c, d);
}

template <typename NT >
struct Root_of_traits< Lazy_exact_nt < NT > >
{
private:
    typedef Root_of_traits<NT> T;
public:
    typedef Lazy_exact_nt< typename T::RootOf_1 > RootOf_1;
    typedef Lazy_exact_nt< typename T::RootOf_2 > RootOf_2;
};

#endif // 0

#undef CGAL_double
#undef CGAL_int

CGAL_END_NAMESPACE

#endif // CGAL_LAZY_EXACT_NT_H
