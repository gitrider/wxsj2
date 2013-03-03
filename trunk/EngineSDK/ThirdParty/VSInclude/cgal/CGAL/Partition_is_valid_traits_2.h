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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Partition_2/include/CGAL/Partition_is_valid_traits_2.h $
// $Id: Partition_is_valid_traits_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

#ifndef CGAL_PARTITION_2_IS_VALID_TRAITS_H
#define CGAL_PARTITION_2_IS_VALID_TRAITS_H

namespace CGAL {

template <class Traits, class PolygonIsValid>
class Partition_is_valid_traits_2 : public Traits
{
public:
   typedef typename Traits::Point_2         Point_2;
   typedef typename Traits::Polygon_2       Polygon_2;
   typedef typename Traits::Less_xy_2       Less_xy_2;
   typedef typename Traits::Left_turn_2      Left_turn_2;
   typedef typename Traits::Orientation_2   Orientation_2;

   typedef PolygonIsValid                   Is_valid;
   
   Is_valid
   is_valid_object(const Traits& traits) const
   {  return Is_valid(traits); }
};

}

#endif // CGAL_PARTITION_2_IS_VALID_TRAITS_H

