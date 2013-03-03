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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Cartesian_kinetic_kernel.h $
// $Id: Cartesian_kinetic_kernel.h 31055 2006-05-07 04:31:41Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_KINETIC_KERNEL_H
#define CGAL_KINETIC_KINETIC_KERNEL_H
#include <CGAL/Kinetic/basic.h>
#include <CGAL/Kinetic/internal/Kernel/Cartesian_kinetic_kernel_base.h>

CGAL_KINETIC_BEGIN_NAMESPACE

//! A kinetic kernel using cartesian coordinates
/*!  It takes a PolynomialKernel as a template parameter. The
  PolynomialKernel is used to define the Motion_function and the
  Certificate_function.
*/
template <class Function_kernel_k>
class Cartesian_kinetic_kernel:
  public internal::Cartesian_kinetic_kernel_base<Function_kernel_k,
						 Cartesian_kinetic_kernel<Function_kernel_k> >
{
  typedef internal::Cartesian_kinetic_kernel_base<Function_kernel_k,
						  Cartesian_kinetic_kernel<Function_kernel_k> > P;
public:
  typedef Function_kernel_k Function_kernel;
  Cartesian_kinetic_kernel(Function_kernel_k pk): P(pk){}
  Cartesian_kinetic_kernel(){}
};

CGAL_KINETIC_END_NAMESPACE

//#include <CGAL/Kinetic_internals/kernel_undefs.h>
#endif
