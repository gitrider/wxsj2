// Copyright (c) 1997  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Intersections_3/include/CGAL/bbox_intersection_3.h $
// $Id: bbox_intersection_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Geert-Jan Giezeman <geert@cs.uu.nl>


#ifndef CGAL_BBOX_INTERSECTION_3_H
#define CGAL_BBOX_INTERSECTION_3_H

#include <CGAL/Bbox_3.h>
#include <CGAL/Object.h>

CGAL_BEGIN_NAMESPACE

Object
intersection_bl(const Bbox_3 &box,
                double lx1, double ly1, double lz1,
                double lx2, double ly2, double lz2,
                bool min_infinite, bool max_infinite);

CGAL_END_NAMESPACE

#endif // CGAL_BBOX_INTERSECTION_3_H
