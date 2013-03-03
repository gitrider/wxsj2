// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/internal/Kernel/Center.h $
// $Id: Center.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_INTERNAL_CENTER_H
#define CGAL_KINETIC_INTERNAL_CENTER_H
#include <CGAL/Kinetic/basic.h>

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE

template <class K>
struct Center
{
    typedef typename K::Point_3 argument_type;
    typedef typename K::Point_3 result_type;

    const result_type& operator()(const argument_type &p) const
    {
        return p;
    }
    const result_type& operator()(const typename K::Weighted_point_3 &wp) const
    {
        return wp.point();
    }
};

CGAL_KINETIC_END_INTERNAL_NAMESPACE
#endif
