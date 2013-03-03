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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Curved_kernel/include/CGAL/Curved_kernel/functions_on_circle_2.h $
// $Id: functions_on_circle_2.h 28723 2006-02-23 14:42:53Z spion $
//
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_CURVED_KERNEL_FUNCTIONS_ON_CIRCLE_2_H
#define CGAL_CURVED_KERNEL_FUNCTIONS_ON_CIRCLE_2_H

namespace CGAL {

  // Should we have an iterator based interface, or both ?
  template <class CK>
  typename CK::Circular_arc_point_2
  x_extremal_point(const Circle_2<CK> & c, bool i)
  {
  	return CircularFunctors::x_extremal_point<CK>(c,i);
  }
  
  template <class CK, class OutputIterator>
  OutputIterator
  x_extremal_points(const Circle_2<CK> & c, OutputIterator res)
  {
  	return CircularFunctors::x_extremal_points<CK>(c,res);
  }

  template <class CK>
  typename CK::Circular_arc_point_2
  y_extremal_point(const Circle_2<CK> & c, bool i)
  {
    return CircularFunctors::y_extremal_point<CK>(c,i);
  }

  template <class CK, class OutputIterator>
  OutputIterator
  y_extremal_points(const Circle_2<CK> & c, OutputIterator res)
  {
  	return CircularFunctors::y_extremal_points<CK>(c,res);
  }

} // namespace CGAL

#endif // CGAL_CURVED_KERNEL_FUNCTIONS_ON_CIRCLE_2_H
