// Copyright (c) 2005  Tel-Aviv University (Israel).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/Boolean_set_operations_2/Gps_default_traits.h $
// $Id: Gps_default_traits.h 28831 2006-02-27 14:28:18Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_GPS_DEFAULT_TRAITS_H
#define CGAL_GPS_DEFAULT_TRAITS_H

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/General_polygon_with_holes_2.h>
#include <CGAL/General_polygon_2.h>
#include <CGAL/Gps_segment_traits_2.h>
#include <CGAL/Gps_traits_2.h>

CGAL_BEGIN_NAMESPACE

template <class Polygon>
struct Gps_default_traits
{};


template <class Kernel, class Container>
struct Gps_default_traits<CGAL::Polygon_2<Kernel, Container> >
{
  typedef Gps_segment_traits_2<Kernel,
                               Container,
                               Arr_segment_traits_2<Kernel> >    Traits;
};

template <class Kernel, class Container>
struct Gps_default_traits<CGAL::Polygon_with_holes_2<Kernel, Container> >
{
  typedef Gps_segment_traits_2<Kernel,
                               Container,
                               Arr_segment_traits_2<Kernel> >    Traits;
};

template <class Polygon>
struct Gps_default_traits<CGAL::General_polygon_with_holes_2<Polygon> >
{
  typedef typename Gps_default_traits<Polygon>::Traits Traits;
};

template <class Arr_traits>
struct Gps_default_traits<CGAL::General_polygon_2<Arr_traits> >
{
  typedef Gps_traits_2<Arr_traits>    Traits;
};

CGAL_END_NAMESPACE

#endif
