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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Algebraic_kernel/include/CGAL/Polynomials_1_2.h $
// $Id: Polynomials_1_2.h 28727 2006-02-23 14:52:22Z spion $
//
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_1_2_H
#define CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_1_2_H

#include <CGAL/enum.h>

CGAL_BEGIN_NAMESPACE

template < typename RT_ >
class Polynomial_1_2
{
  RT_ rep[3]; // stores a, b, c for line ax+by+c=0
  
public:
  
  typedef RT_ RT;
  
  Polynomial_1_2(){}
  
  Polynomial_1_2(const RT & a, const RT & b, const RT & c)
  { 
    rep[0]=a;
    rep[1]=b;
    rep[2]=c;
  }

  const RT & a() const
  { return rep[0]; }

  const RT & b() const
  { return rep[1]; }

  const RT & c() const
  { return rep[2]; }
};

template < typename RT >
bool 
operator == ( const Polynomial_1_2<RT> & p1,
	      const Polynomial_1_2<RT> & p2 )
{
  return( (p1.a() == p2.a()) && 
              (p1.b() == p2.b()) &&
              (p1.c() == p2.c()) );
}
    
CGAL_END_NAMESPACE

#endif //CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_1_2_H
