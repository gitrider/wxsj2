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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/internal/Kernel/Certificate.h $
// $Id: Certificate.h 29334 2006-03-10 00:00:09Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_IO_KERNEL_CERTIFICATE_H
#define CGAL_KINETIC_IO_KERNEL_CERTIFICATE_H
#include <CGAL/Kinetic/basic.h>
#include <limits>

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE

template <class Function_kernel_t>
class Certificate {
public:
  typedef typename Function_kernel_t::Function Function;
  typedef typename Function_kernel_t::Root Time;
  Certificate(const Function &f, const Function_kernel_t& fk, const Time &b, const Time &e): rs_(fk.root_stack_object(f, b, e)) {
    /*typename Function_kernel_t::Lower_bound_root lbr= fk.lower_bound_root_object();
      estimate_= lbr(f, b);*/
  }
  Certificate(){}

  const Time &failure_time() const {
    static Time inf= std::numeric_limits<Time>::infinity();
    if (rs_.empty()) return inf;
    else return rs_.top();
  }
  void pop_failure_time() {
    if (!rs_.empty()) rs_.pop();
  }

  const typename Function_kernel_t::Root_stack& root_stack() const {
    return rs_;
  }
  double lower_bound() const {
    return estimate_;
  }
private:
  typename Function_kernel_t::Root_stack rs_;
  double estimate_;
};



CGAL_KINETIC_END_INTERNAL_NAMESPACE


#endif
