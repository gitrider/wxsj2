// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/CORE_Expr_root_stack.h $
// $Id: CORE_Expr_root_stack.h 29703 2006-03-22 18:00:35Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_CORE_SOLVER_H
#define CGAL_POLYNOMIAL_CORE_SOLVER_H
#include <CGAL/Polynomial/basic.h>
#include <CGAL/CORE_Expr.h>
#include <CGAL/Polynomial/internal/Explicit_root.h>
#include <CGAL/Polynomial/internal/CORE_polynomial.h>
#include <CGAL/Polynomial/internal/Root_stack_traits_base.h>
#include <CORE/BigInt.h>

#include <iostream>

/*CGAL_BEGIN_NAMESPACE
double to_double(const CORE::BigInt &bi)
{
  return bi.doubleValue();
  }


  CGAL_END_NAMESPACE*/

CGAL_POLYNOMIAL_BEGIN_NAMESPACE


class CORE_Expr_root_stack
{
protected:
  typedef CORE_Expr_root_stack This;
  //typedef CORE::Poly<CORE::BigInt> BIP;
public:

  typedef internal::CORE_polynomial Function;
  typedef Function::NT Coef;
  typedef CORE::Sturm<Coef> CORE_Sturm;
  

  struct Traits: public internal::Root_stack_traits_base<Function> {
    
  };

  typedef internal::Explicit_root<CORE::Expr> Root;

  CORE_Expr_root_stack(const Function &f,
		       const Root &lb,
		       const Root &ub,
		       const Traits &tr): f_(f), ub_(ub), cur_(Root::infinity_rep()), tr_(tr){
    initialize(lb);
  }

  CORE_Expr_root_stack():  num_roots_(0){}

  const Root& top() const
  {
    return cur_;
  }
  void pop() {
    --num_roots_;
    CGAL_precondition(num_roots_>=0);
    if (num_roots_==0) {
      no_roots();
    }
    else {
      make_root();
      enforce_upper_bound();
    }
  }

  bool empty() const
  {
    return num_roots_==0;
  }

protected:
  Function f_;
  CORE_Sturm sturm_;
  Root  ub_;
  Root cur_;
  int num_roots_;
  CORE::BigFloat bflb_, bfub_;
  Traits tr_;

  void initialize(const Root& lb) {
    if (f_.degree()<0) {
      no_roots();
      return;
    } else {
      //std::cout << f_ << std::endl;
      //std::cout << f_.core_polynomial() << std::endl;
      sturm_= CORE_Sturm(f_.core_polynomial()/*, false*/); //BigInt to BigRat
      

      CORE::BigFloat bflb, bfub;
      
      if (lb == -std::numeric_limits<Root>::infinity()){
	bflb_= -f_.core_polynomial().CauchyUpperBound();
      } else {
	bflb_= bf_lower_bound(lb.representation());
      }

      if (ub_ == std::numeric_limits<Root>::infinity()){
	bfub_=  f_.core_polynomial().CauchyUpperBound();
      } else {
	bfub_= bf_upper_bound(ub_.representation());
      }
      if (bflb_ > bfub_) {
	no_roots();
      } else {
	//std::cout << f_ << ": " << bflb_ << " " << bfub_ << std::endl;
	num_roots_= sturm_.numberOfRoots(bflb_, bfub_);
	//std::cout << "nr= " << num_roots_ << std::endl;
	//CORE::Expr testr;
	++num_roots_;
	do {
	  --num_roots_;
	  if ( num_roots_ == 0) {
	    no_roots();
	    return;
	  }
	  make_root();
	  
	} while (cur_ <= lb);
	//make_cur_root(testr);
      }
      //std::cout << "There are " << num_roots_ << " roots.\n";
      enforce_upper_bound();
    }
  }

  void enforce_upper_bound() {
    if (cur_ < ub_) return;
    else {
      //std::cout << "Rejected " << cur_ << std::endl;
      no_roots();
    }
  }

  void make_root() {
    CGAL_precondition(num_roots_!=0);
    //std::cout << bflb_ << " " << bfub_ << std::endl;
    CORE::BFInterval bfi= sturm_.isolateRoot(1, bflb_, bfub_);
    //int nr= sturm_.numberOfRoots(bfi.first, bfi.second);
    int nr=1;
    if (CGAL::sign(f_.core_polynomial().eval(bfi.first)) 
	== CGAL::sign(f_.core_polynomial().eval(bfi.second))) ++nr;
    //std::cout << nr << " " << bfi.first << " " << bfi.second <<  std::endl;
    bflb_= bfi.second;
    CORE::Expr e(f_.core_polynomial(), bfi);
    cur_ =Root(e/*/f_.scale()*/, nr);
    //std::cout << "root= " << cur_ <<  " " << e << std::endl;
  }

  void no_roots() {
    ub_= CORE::Expr(0);
    cur_= infinity<Root>();
    num_roots_=0;
  }

  /*void initialize_counters(const Root &lb) {
    std::cout << "Computing strum of " << poly_ << "..." << std::flush;
    CORE_Sturm sturm(poly_);
    std::cout << "done." << std::endl;
    num_roots_=0;
    CGAL_assertion(-ub_ != infinity<Root>());
    num_roots_= sturm.numberOfRoots();
    if (lb== -infinity<Root>() && ub_== infinity<Root>()) {
      counter_=0;
    }
    else if (ub_ == infinity<Root>()) {
      std::cout << bf_lower_bound(lb.representation()) << std::endl;
      //num_roots_= sturm.numberOfRootsAbove(bf_lower_bound(lb.representation()));
      counter_ = sturm.numberOfRootsBelow(bf_lower_bound(lb.representation()));
    }
    else if (lb == infinity<Root>()) {
      //num_roots_= sturm.numberOfRootsBelow(bf_upper_bound(ub_.representation()));
      counter_ = 0;
    }
    else {
      counter_= sturm.numberOfRootsBelow(bf_lower_bound(lb.representation()));
      
    }
    }*/

  //! There are probably better ways of doing this
  Coef bf_lower_bound(const CORE::Expr &rt) const
  {
    machine_double lb, ub;
    rt.doubleInterval(lb, ub);
    return Coef(lb);
  }

  //! There are probably better ways of doing this
  Coef bf_upper_bound(const CORE::Expr &rt) const
  {
    machine_double lb, ub;
    rt.doubleInterval(lb, ub);
    return Coef(ub);
  }
};

CGAL_POLYNOMIAL_END_NAMESPACE;
#endif
