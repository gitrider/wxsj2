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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/Gps_circle_segment_traits_2.h $
// $Id: Gps_circle_segment_traits_2.h 28830 2006-02-27 14:26:55Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_GPS_CIRCLE_SEGMENT_TRAITS_2_H
#define CGAL_GPS_CIRCLE_SEGMENT_TRAITS_2_H

#include <CGAL/Gps_traits_2.h>
#include <CGAL/Arr_circle_segment_traits_2.h>

CGAL_BEGIN_NAMESPACE

template <class Kernel_, bool Filer_ = true>
class Gps_circle_segment_traits_2 : 
  public Gps_traits_2<Arr_circle_segment_traits_2<Kernel_, Filer_> >
{
public:
  Gps_circle_segment_traits_2<Kernel_, Filer_>(bool use_cache = false): 
      Gps_traits_2<Arr_circle_segment_traits_2<Kernel_, Filer_> >()
  {
    this->m_use_cache = use_cache;
  }

};

CGAL_END_NAMESPACE

#endif
