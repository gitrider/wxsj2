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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Kernel/Lower_bound_root.h $
// $Id: Lower_bound_root.h 31252 2006-05-22 12:26:07Z drussel $ $Date: 2006-05-22 14:26:07 +0200 (Mon, 22 May 2006) $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_LOWER_BOUND_ROOT_H
#define CGAL_POLYNOMIAL_INTERNAL_LOWER_BOUND_ROOT_H

#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/internal/interval_arithmetic.h>
#include <CGAL/Polynomial/Interval_polynomial.h>
#include <CGAL/Polynomial/polynomial_converters.h>



CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE
template <class Kernel>
struct Lower_bound_root {
  typedef typename Kernel::Function first_argument_type;
  typedef typename first_argument_type::NT NT;
  typedef typename Kernel::Root second_argument_type;
  typedef double result_type;

  double operator()(const first_argument_type &f, const second_argument_type &lb) const {
    Interval_arithmetic_guard gd;
    Polynomial_converter<first_argument_type, Interval_polynomial,  To_interval<NT> > pc;
    Interval_polynomial ip= pc(f);
    Interval_nt iv(CGAL::to_interval(lb));

    if (ip(iv).inf() <=0) {
      return iv.inf();
    }

    if (ip(Interval_nt(iv.inf(), std::numeric_limits<double>::infinity())).inf() >0) {
      return std::numeric_limits<double>::infinity();
    }

    double step= iv.sup()-iv.inf();
    if (step==0) {
      step=.1;
    }

    double last=iv.sup();
    while ( iv.sup() 
	   != std::numeric_limits<double>::infinity()) {
      last=iv.sup();
      iv= std::make_pair(iv.inf(), iv.sup()+ step);
      step*=2;
      if (ip(iv).inf() <=0){
	iv= std::make_pair(iv.inf(), last);
	break;
      }
    }
    CGAL_assertion(ip(Interval_nt(iv.inf(), last)).inf() >0);
    CGAL_assertion(iv.sup() !=  std::numeric_limits<double>::infinity());
    double over= iv.inf()+ (iv.sup()-iv.inf())*2.0;
    
    for (int i=0; i< 4; ++i){
      iv= std::make_pair(iv.inf(), (last+over)/2.0);
      //double mp= (top+iv.sup())/2.0;
      //Interval_nt ivt= std::make_pair(iv.inf(), mp);
      if ( ip(iv).inf() >0) {
	last= iv.sup();
      } else {
	over=iv.sup();
      }
    }

    CGAL_postcondition(ip(Interval_nt(iv.inf(), last)).inf() >0);
    /*std::cout << "Bound is " << (iv.sup()-iv.inf())/2.0 << std::endl;
      std::cout << "Root is " << CGAL::to_interval(cur_).first - CGAL::to_interval(lb).first <<std::endl;*/
    return last;
  }
};
CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
