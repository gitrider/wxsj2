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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Curved_kernel/include/CGAL/Curved_kernel/internal_functions_on_line_2.h $
// $Id: internal_functions_on_line_2.h 29572 2006-03-17 10:17:29Z afabri $
//
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_CURVED_KERNEL_FUNCTIONS_ON_LINE_2_H
#define CGAL_CURVED_KERNEL_FUNCTIONS_ON_LINE_2_H

namespace CGAL {
namespace LinearFunctors {

  template < class CK >
  typename CK::Polynomial_1_2
  get_equation( const typename CK::Line_2 & L )
  {
    typedef typename CK::RT RT;
 
    return typename CK::Polynomial_1_2(L.a(),L.b(),L.c());
  }
  
  template < class CK >
  typename CK::Line_2  
  construct_line_2 ( const typename CK::Polynomial_1_2 &eq )
  {
    return typename CK::Line_2(eq[2],eq[1],eq[0]); 
  }
  
  template < class CK >
  bool
  has_on(const typename CK::Line_2 & l,
	 const typename CK::Circular_arc_point_2 &p)
  {
    typedef typename CK::Algebraic_kernel            AK;
    typedef typename CK::Polynomial_1_2 Polynomial_1_2;
    Polynomial_1_2 equation = CGAL::LinearFunctors::get_equation<CK>(l);

    return(AK().sign_at_object()(equation,p.coordinates())== ZERO);
  }

  template< class CK, class OutputIterator>
  OutputIterator
  intersect_2( const typename CK::Line_2 & l,
	       const typename CK::Circle_2 & c,
	       OutputIterator res )
  {
    typedef typename CK::Algebraic_kernel            AK;
    typedef typename CK::Polynomial_1_2              Equation_line;
    typedef typename CK::Polynomial_for_circles_2_2  Equation_circle; 
    typedef typename CK::Root_for_circles_2_2        Root_for_circles_2_2;
    
    Equation_line e1 = CGAL::get_equation<CK>(l);
    Equation_circle e2 = CGAL::get_equation<CK>(c);
    
    typedef std::vector< std::pair < Root_for_circles_2_2, unsigned > > 
      solutions_container;
    solutions_container solutions;

    AK().solve_object()(e1, e2, std::back_inserter(solutions)); 
    // to be optimized

    typedef typename CK::Circular_arc_point_2 Circular_arc_point_2;

    for ( typename solutions_container::iterator it = solutions.begin(); 
	  it != solutions.end(); ++it )
      {
	*res++ = make_object
	  (std::make_pair(Circular_arc_point_2(it->first), it->second ));
      }

    return res;
  }

} // namespace LinearFunctors
} // namespace CGAL
#endif // CGAL_CURVED_KERNEL_FUNCTIONS_ON_LINE_2_H
