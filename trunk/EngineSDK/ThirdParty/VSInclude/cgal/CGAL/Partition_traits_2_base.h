// Copyright (c) 2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Partition_2/include/CGAL/Partition_traits_2_base.h $
// $Id: Partition_traits_2_base.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

#ifndef PARTITION_TRAITS_2_BASE_H
#define PARTITION_TRAITS_2_BASE_H

#include <CGAL/Polygon_2.h>
#include <list>

namespace CGAL {

template <class Kernel_>
class Partition_traits_2_base
{
  private:
    typedef Kernel_                                 Kernel;
  public:
    typedef typename Kernel::Point_2                Point_2;
    typedef ::std::list<Point_2>                    Container;
    typedef CGAL::Polygon_2<Kernel, Container>      Polygon_2;
    typedef typename Kernel::Less_yx_2              Less_yx_2;
    typedef typename Kernel::Less_xy_2              Less_xy_2;
    typedef typename Kernel::Left_turn_2             Left_turn_2;
    typedef typename Kernel::Orientation_2          Orientation_2;
    typedef typename Kernel::Compare_y_2            Compare_y_2;
    typedef typename Kernel::Compare_x_2            Compare_x_2;

    Less_yx_2
    less_yx_2_object() const
    { return Less_yx_2(); }

    Less_xy_2
    less_xy_2_object() const
    { return Less_xy_2(); }

    Left_turn_2
    left_turn_2_object() const
    { return Left_turn_2(); }

    Orientation_2
    orientation_2_object() const
    { return Orientation_2(); }

    Compare_y_2
    compare_y_2_object() const
    {  return Compare_y_2(); }

    Compare_x_2
    compare_x_2_object() const
    {  return Compare_x_2(); }

};

}

#endif // PARTITION_TRAITS_2_BASE_H
