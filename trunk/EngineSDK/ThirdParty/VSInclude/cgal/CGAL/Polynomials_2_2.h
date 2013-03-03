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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Algebraic_kernel/include/CGAL/Polynomials_2_2.h $
// $Id: Polynomials_2_2.h 28727 2006-02-23 14:52:22Z spion $
//
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_2_2_H
#define CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_2_2_H

//////////// FIXME - pb RT (cas general Polynomial_2_2) ou FT (ici)

#include <CGAL/enum.h>

CGAL_BEGIN_NAMESPACE

// polynomials of the form (X-a)^2 + (Y-b)^2 - R^2
template < typename FT_ >
class Polynomial_for_circles_2_2
{
  FT_ rep[3]; // stores a, b, R^2
  
public:
  
  typedef FT_ FT;
  
  Polynomial_for_circles_2_2(){}
  
  Polynomial_for_circles_2_2(const FT & a, const FT & b, const FT & rsq)
  { 
    rep[0]=a;
    rep[1]=b;
    rep[2]=rsq;
  }

  const FT & a() const
  { return rep[0]; }

  const FT & b() const
  { return rep[1]; }
  
  const FT & r_sq() const
  { return rep[2]; }
};

template < typename FT >
bool 
operator == ( const Polynomial_for_circles_2_2<FT> & p1,
	      const Polynomial_for_circles_2_2<FT> & p2 )
{
  return( (p1.a() == p2.a()) && 
              (p1.b() == p2.b()) &&
              (p1.r_sq() == p2.r_sq()) );
}

CGAL_END_NAMESPACE

#endif //CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_2_2_H
