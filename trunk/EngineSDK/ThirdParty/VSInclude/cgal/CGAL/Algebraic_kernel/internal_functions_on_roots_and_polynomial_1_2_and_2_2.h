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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Algebraic_kernel/include/CGAL/Algebraic_kernel/internal_functions_on_roots_and_polynomial_1_2_and_2_2.h $
// $Id: internal_functions_on_roots_and_polynomial_1_2_and_2_2.h 28942 2006-03-01 09:11:24Z teillaud $
//
// Author(s)     : Monique Teillaud, Sylvain Pion, Julien Hazebrouck

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_ALGEBRAIC_KERNEL_FUNCTIONS_ON_ROOTS_AND_POLYNOMIAL_1_2_AND_2_2_H
#define CGAL_ALGEBRAIC_KERNEL_FUNCTIONS_ON_ROOTS_AND_POLYNOMIAL_1_2_AND_2_2_H


namespace CGAL {
  namespace AlgebraicFunctors {


  template < class AK, class OutputIterator >
  inline 
  OutputIterator
  solve( const typename AK::Polynomial_1_2 & e1,
	 const typename AK::Polynomial_for_circles_2_2 & e2,
	 OutputIterator res )
  {
    typedef typename AK::FT FT;
    typedef typename AK::Root_of_2 Root_of_2;
    typedef typename AK::Root_for_circles_2_2 Root_for_circles_2_2;
    if (e1.a() == 0){//horizontal line
      
      FT a = 1;
      FT b = -2*e2.a();
      FT c = CGAL::square(e2.a()) +
	CGAL::square(e1.c()/e1.b() + e2.b())
	- e2.r_sq();
      FT cond = CGAL::square(b) - 4 *a*c;
    
      if(cond < 0)
	return res;
      if (cond == 0) {
	Root_of_2 x_res = make_root_of_2(a, b, c, true);
	Root_of_2 y_res = Root_of_2(-e1.c()/e1.b()); 
	*res++ = std::make_pair
	  ( Root_for_circles_2_2(x_res, y_res), 2u);
	return res;
      }
      Root_of_2 x_res1 = make_root_of_2(a, b, c, true);
      Root_of_2 x_res2 = make_root_of_2(a, b, c, false);
      Root_of_2 y_res = Root_of_2(-e1.c()/e1.b()); 
      *res++ = std::make_pair
	( Root_for_circles_2_2(x_res1, y_res), 1u);
      *res++ = std::make_pair
	( Root_for_circles_2_2(x_res2,y_res), 1u);
      return res;
    }
    else if(e1.b() == 0){//vertical line
       
      FT a = 1;
      FT b = -2*e2.b();
      FT c = CGAL::square(e2.b()) +
	CGAL::square(e1.c()/e1.a() + e2.a())
	- e2.r_sq();
      FT cond = CGAL::square(b) - 4 *a*c;
    
      if(cond < 0)
	return res;
      if (cond == 0) {
	Root_of_2 y_res = make_root_of_2(a, b, c, true);
	Root_of_2 x_res = Root_of_2(-e1.c()/e1.a()); 
	*res++ = std::make_pair
	  ( Root_for_circles_2_2(x_res, y_res), 2u);
	return res;
      }
      Root_of_2 y_res1 = make_root_of_2(a, b, c, true);
      Root_of_2 y_res2 = make_root_of_2(a, b, c, false);
      Root_of_2 x_res = Root_of_2(-e1.c()/e1.a()); 
      *res++ = std::make_pair
	( Root_for_circles_2_2(x_res, y_res1), 1u);
      *res++ = std::make_pair
	( Root_for_circles_2_2(x_res,y_res2), 1u);
      return res;
    }
    else{
      //general case
      FT a1_sq = CGAL::square(e1.a());
      FT b1_sq = CGAL::square(e1.b());
      FT b2_sq = CGAL::square(e2.b());
      
      FT a = b1_sq + a1_sq;
      FT b = 2*e1.c()*e1.b() + 2*e2.a()*e1.a()*e1.b() - 2*a1_sq*e2.b();
      FT c = CGAL::square(e2.a()*e1.a() + e1.c()) + a1_sq*b2_sq - a1_sq*e2.r_sq();
      
      
      FT cond = CGAL::square(b) - 4 *a*c;
    
      if(cond < 0)
	return res;


      if (cond == 0) {
	Root_of_2 y_res = make_root_of_2(a, b, c, true);
	Root_of_2 x_res = (e1.b()*y_res + e1.c()) / -e1.a();
	*res++ = std::make_pair
	  ( Root_for_circles_2_2(x_res,y_res), 2u);
	return res;
      }
    
      Root_of_2 y_res1 = make_root_of_2(a, b, c, true);
      Root_of_2 x_res1 = (e1.b()*y_res1 + e1.c()) / - e1.a();
      Root_of_2 y_res2 = make_root_of_2(a, b, c, false);
      Root_of_2 x_res2 = (e1.b()*y_res2 + e1.c()) / - e1.a();

      if(x_res2 < x_res1){
	*res++ = std::make_pair
	  ( Root_for_circles_2_2(x_res2, y_res2), 1u);
	*res++ = std::make_pair
	  ( Root_for_circles_2_2(x_res1,y_res1), 1u);
	return res;
      }
      else{
        *res++ = std::make_pair
	  ( Root_for_circles_2_2(x_res1, y_res1), 1u);
	*res++ = std::make_pair
	  ( Root_for_circles_2_2(x_res2,y_res2), 1u);
	return res;
      }     
    }
  }

  template < class AK, class OutputIterator >
  inline 
  OutputIterator
  solve( const typename AK::Polynomial_for_circles_2_2 & e1,
	 const typename AK::Polynomial_1_2 & e2,
	 OutputIterator res )
  {
    return solve<AK> ( e2, e1, res);
  }

  template < class AK, class OutputIterator >
  inline 
  OutputIterator
  solve( const typename AK::Polynomial_1_2 & e1,
	 const typename AK::Polynomial_1_2 & e2,
	 OutputIterator res )
  {
    typedef typename AK::FT FT;
    typedef typename AK::Root_of_2 Root_of_2;
    typedef typename AK::Root_for_circles_2_2 Root_for_circles_2_2;
    //parallele case
    if(e1.a()*e2.b() == e2.a()*e1.b())
      return res;
    //case : e2 horizontal
    if(e2.a() == 0){
      Root_of_2 y = -e2.c()/e2.b();
      Root_of_2 x = -(e1.b()*y + e1.c())/e1.a();
      *res++ = std::make_pair
	  ( Root_for_circles_2_2(x, y), 1u);
    return res;
    }
    //general case
    Root_of_2 y = (e2.a()*e1.c() - e2.c()*e1.a())/(e1.a()*e2.b() - e2.a()*e1.b());
    Root_of_2 x = (e2.b()*y + e2.c())/(-e2.a());
    *res++ = std::make_pair
	  ( Root_for_circles_2_2(x, y), 1u);
    return res;
  }

    template < class AK >
    inline 
    Sign sign_at( const typename AK::Polynomial_1_2 & equation,
		  const typename AK::Root_for_circles_2_2 & r)
    {
      typedef typename AK::Root_of_2 Root_of_2;
      Root_of_2 part_left = r.x()*equation.a();
      Root_of_2 part_right =  -equation.c() - r.y()*equation.b();
      if (part_left == part_right)
	return ZERO;
      return (part_left < part_right) ? NEGATIVE : POSITIVE; 
    }


  
  } // namespace AlgebraicFunctors
} // namespace CGAL

#endif //  CGAL_ALGEBRAIC_KERNEL_FUNCTIONS_ON_ROOTS_AND_POLYNOMIAL1_2_AND_2_2_H
