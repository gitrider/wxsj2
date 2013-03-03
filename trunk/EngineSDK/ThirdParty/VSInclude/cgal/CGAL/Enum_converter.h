// Copyright (c) 2003  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kernel_23/include/CGAL/Enum_converter.h $
// $Id: Enum_converter.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>

#ifndef CGAL_ENUM_CONVERTER_H
#define CGAL_ENUM_CONVERTER_H

#include <CGAL/basic.h>
#include <CGAL/enum.h>

CGAL_BEGIN_NAMESPACE

struct Enum_converter
{
  bool              operator()(bool b) const { return b; }

  Sign              operator()(Sign s) const { return s; }

  Oriented_side     operator()(Oriented_side os) const { return os; }

  Bounded_side      operator()(Bounded_side bs) const { return bs; }

  Comparison_result operator()(Comparison_result cr) const {
    return cr;
  }

  Angle operator()(Angle a) const { return a; }
};


CGAL_END_NAMESPACE


#endif // CGAL_ENUM_CONVERTER_H
