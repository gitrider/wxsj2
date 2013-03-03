// Copyright (c) 1997-2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_d/include/CGAL/Kernel_d/simple_objects.h $
// $Id: simple_objects.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>
#ifndef SIMPLE_OBJECTS_H
#define SIMPLE_OBJECTS_H

CGAL_BEGIN_NAMESPACE

template <class R>
struct Lt_from_compare {
  typedef typename R::Point_d Point_d;
  bool operator()(const Point_d& p1, const Point_d& p2) const
  { typename R::Compare_lexicographically_d cmp;
    return cmp(p1,p2) == SMALLER; }
};

template <class R>
struct Le_from_compare {
  typedef typename R::Point_d Point_d;
  bool operator()(const Point_d& p1, const Point_d& p2) const
  { typename R::Compare_lexicographically_d cmp;
    return cmp(p1,p2) != LARGER; }
};

template <class R>
struct Eq_from_method {
  typedef typename R::Point_d Point_d;
  bool operator()(const Point_d& p1, const Point_d& p2) const
  { return p1 == p2; }
};

CGAL_END_NAMESPACE
#endif //SIMPLE_OBJECTS_H
