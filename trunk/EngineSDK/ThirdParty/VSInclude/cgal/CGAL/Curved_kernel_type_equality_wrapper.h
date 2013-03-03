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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Curved_kernel/include/CGAL/Curved_kernel_type_equality_wrapper.h $
// $Id: Curved_kernel_type_equality_wrapper.h 28743 2006-02-24 09:39:43Z teillaud $
//
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_CURVED_KERNEL_TYPE_EQUALITY_WRAPPER_H
#define CGAL_CURVED_KERNEL_TYPE_EQUALITY_WRAPPER_H

#include <CGAL/user_classes.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Circular_arc_2.h>
#include <CGAL/Circular_arc_point_2.h>
#include <CGAL/Line_arc_2.h>
#include <CGAL/Root_of_2.h>


CGAL_BEGIN_NAMESPACE

// This is a kernel wrapper which provides the type equality between
// Kernel::Point_2 and CGAL::Point_2<Kernel>, by deriving from
// K_base::Point_2 (and similar for the other types).

template < typename K_base, typename Kernel >
struct Curved_kernel_type_equality_wrapper
  : public Type_equality_wrapper<K_base, Kernel>
{
    typedef K_base                                  Kernel_base;
    typedef CGAL::Circular_arc_2<Kernel>               Circular_arc_2;     
    typedef CGAL::Line_arc_2<Kernel>                   Line_arc_2;
    typedef CGAL::Circular_arc_point_2<Kernel>         Circular_arc_point_2;
    //typedef CGAL::Root_of_2<typename Kernel_base::FT>  Root_of_2;
    
	//Something has to be done with these 3, maybe a lazy Algebraic kernel?
	   
    //typedef Polynomial_for_circles_2_2<Kernel>   Polynomial_for_circles_2_2;
    //typedef Polynomial_1_2<Kernel>               Polynomial_1_2;
    //typedef Root_of_2<Kernel>                    Root_of_2;   
};

CGAL_END_NAMESPACE

#endif // CGAL_CURVED_KERNEL_TYPE_EQUALITY_WRAPPER_H
