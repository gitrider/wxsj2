// Copyright (c) 2003-2006  INRIA Sophia-Antipolis (France).
// All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Curved_kernel/include/CGAL/Exact_circular_kernel.h $
// $Id: Exact_circular_kernel.h 30672 2006-04-20 07:16:56Z afabri $
//
// Author(s)     : Monique Teillaud, Sylvain Pion


// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_EXACT_CIRCULAR_KERNEL_H
#define CGAL_EXACT_CIRCULAR_KERNEL_H

#include <CGAL/Cartesian.h>
#include <CGAL/Algebraic_kernel_2_2.h>
#include <CGAL/Circular_kernel.h>

#ifdef CGAL_USE_GMP
#  include <CGAL/Gmpq.h>
#else
#  include <CGAL/MP_Float.h>
#  include <CGAL/Quotient.h>
#endif

#include <CGAL/Lazy_curved_kernel.h>

CGAL_BEGIN_NAMESPACE

#ifdef CGAL_USE_GMP
  typedef CGAL::Gmpq                                           NT1;
#else
  typedef CGAL::Quotient<CGAL::MP_Float>                       NT1;
#endif

  typedef CGAL::Cartesian<NT1>                                 Linear_k1;
  typedef CGAL::Algebraic_kernel_for_circles_2_2<NT1>          Algebraic_k1;
  typedef CGAL::Circular_kernel_2<Linear_k1,Algebraic_k1>      CK1;

  typedef CGAL::Interval_nt_advanced                           NT2;
  typedef CGAL::Cartesian<NT2>                                 Linear_k2;
  typedef CGAL::Algebraic_kernel_for_circles_2_2<NT2>          Algebraic_k2;
  typedef CGAL::Circular_kernel_2<Linear_k2,Algebraic_k2>      CK2;

  typedef CGAL::Lazy_curved_kernel<CK1,CK2>        Exact_circular_kernel_2;

CGAL_END_NAMESPACE

#endif // CGAL_EXACT_CIRCULAR_KERNEL_H
