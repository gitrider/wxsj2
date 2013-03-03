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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/interval_arithmetic.h $
// $Id: interval_arithmetic.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_TOOLS_INTERVAL_ARITHMETIC_H
#define CGAL_POLYNOMIAL_TOOLS_INTERVAL_ARITHMETIC_H
#include <CGAL/Polynomial/basic.h>
#include <utility>

/*!
  \file interval_arithmetic.h

  \todo wrapper support for boost::interval
*/

#ifdef CGAL_POLYNOMIAL_USE_CGAL
#include <CGAL/Interval_nt.h>
#include <CGAL/double.h>
#include <CGAL/int.h>

CGAL_POLYNOMIAL_BEGIN_NAMESPACE
//! The class we use for interval arithmetic.
/*!
  This class is not protected, so the Interval_arithmetic_guard must be used when calculations
  are peformed. This is not checked.
*/
typedef CGAL::Interval_nt_advanced Interval_nt;

//! This class sets the rounding modes for interval arithmetic.
/*!
  Create an instance of this class when you want to perform interval arithmetic.
  When the destructor is called, the mode will be automatically cleaned up.
*/
struct Interval_arithmetic_guard: public CGAL::Protect_FPU_rounding<true>{};
/*struct Interval_arithmetic_guard {
  //! I should check that the rounding is not already upwards.
  Interval_arithmetic_guard() {
    bk_= FPU_get_and_set_cw(CGAL_FE_UPWARD);
  };
  Interval_arithmetic_guard(bool b) {
    if (b){
      bk_= FPU_get_and_set_cw(CGAL_FE_UPWARD);
    } else {
      bk_= CGAL_FE_UPWARD;
    }
}
~Interval_arithmetic_guard(){
FPU_set_cw(bk_);
}
bool enabled() const {
return bk_== CGAL_FE_UPWARD;
}
void set_enabled(bool ft) {
if (ft != enabled()) {
bk_= FPU_get_and_set_cw(bk_);
}
}
protected:
FPU_CW_t bk_;
};*/

template <class NT>
class To_interval: public CGAL::To_interval<NT>
{
    public:
        To_interval(){}
};

/*template <class NT>
std::pair<double,double> to_interval(const NT&a){
  return CGAL::to_interval(a);
}

std::pair<double,double> to_interval(double d){
  return CGAL::to_interval(d);
  }*/

#define CGAL_POLYNOMIAL_TO_INTERVAL(nt) CGAL::to_interval(nt)
/*template <class NT>
std::pair<double, double> to_interval(const NT &nt){
  //bool to_interval_general;
  return CGAL::to_interval(nt);
  }*/
//using CGAL::to_interval;

namespace internal
{
    template <class Traits> class Simple_interval_root;
    template <class R1, class R2> class Lazy_upper_bound_root_stack_root;
    template <class R> class Explicit_root;
}


CGAL_POLYNOMIAL_END_NAMESPACE

CGAL_BEGIN_NAMESPACE

template <class Traits>
std::pair<double,double> to_interval(const typename CGAL_POLYNOMIAL_NS::internal::Simple_interval_root<Traits> &r);
template <class R1, class R2>
std::pair<double,double> to_interval(const typename CGAL_POLYNOMIAL_NS::internal::Lazy_upper_bound_root_stack_root<R1, R2> &lr);
template <class R>
std::pair<double,double> to_interval(const typename CGAL_POLYNOMIAL_NS::internal::Explicit_root<R> &r);

CGAL_END_NAMESPACE

#elif POLYNOMIAL_USE_BOOST_INTERVAL

Not implemented yet.

#else

No interval arithmetic support.
#endif

CGAL_POLYNOMIAL_BEGIN_NAMESPACE

inline Extended_sign extended_sign(const Interval_nt &i)
{
    if (i.inf() == 0 && i.sup() ==0) return EXTENDED_ZERO;
    else if (i.inf() <=0 && i.sup() >=0) {
        return EXTENDED_UNKNOWN;
    }
    else if (i.inf() >0 ) {
        return EXTENDED_POSITIVE;
    }
    else {
        return EXTENDED_NEGATIVE;
    }
}


CGAL_POLYNOMIAL_END_NAMESPACE
#endif
