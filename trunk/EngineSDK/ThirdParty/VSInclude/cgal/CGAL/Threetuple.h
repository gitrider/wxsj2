// Copyright (c) 1999,2001  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/STL_Extension/include/CGAL/Threetuple.h $
// $Id: Threetuple.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Andreas Fabri
 
#ifndef CGAL_THREETUPLE_H
#define CGAL_THREETUPLE_H

CGAL_BEGIN_NAMESPACE

template < class T >
struct Threetuple
{
  typedef T value_type;

  T  e0, e1, e2;

  Threetuple()
  {}

  Threetuple(const T & a0, const T & a1, const T & a2)
    : e0(a0), e1(a1), e2(a2)
  {}
};

CGAL_END_NAMESPACE

#endif // CGAL_THREETUPLE_H
