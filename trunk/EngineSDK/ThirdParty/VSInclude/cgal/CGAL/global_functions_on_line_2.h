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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Curved_kernel/include/CGAL/global_functions_on_line_2.h $
// $Id: global_functions_on_line_2.h 28975 2006-03-01 18:09:26Z teillaud $
//
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_CURVED_KERNEL_GLOBAL_FUNCTIONS_ON_LINE_2_H
#define CGAL_CURVED_KERNEL_GLOBAL_FUNCTIONS_ON_LINE_2_H

namespace CGAL {

template< class CK >
inline
typename CK::Polynomial_1_2
get_equation(const typename CK::Line_2 & l)
{
  return CK().get_equation_object()(l);
}

template< class CK >
inline
typename CK::Line_2
construct_line_2(const typename CK::Polynomial_1_2 & eq)
{
  return CK().construct_line_2_object()(eq);
}

template< class CK, class OutputIterator>
inline
OutputIterator
intersect_2( const typename CK::Line_2 & l,
			   const typename CK::Circle_2 & c,
			   OutputIterator res )
{
  return CK().intersect_2_object()(l,c,res);
}


} // namespace CGAL
#endif // CGAL_CURVED_KERNEL_GLOBAL_FUNCTIONS_ON_LINE_2_H
