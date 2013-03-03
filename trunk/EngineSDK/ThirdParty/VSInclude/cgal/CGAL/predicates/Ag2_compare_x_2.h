// Copyright (c) 2003,2004  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Apollonius_graph_2/include/CGAL/predicates/Ag2_compare_x_2.h $
// $Id: Ag2_compare_x_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>



#ifndef CGAL_AG2_COMPARE_X_2_H
#define CGAL_AG2_COMPARE_X_2_H

#include <CGAL/enum.h>

//--------------------------------------------------------------------

CGAL_BEGIN_NAMESPACE

template<class K>
class Ag2_compare_x_2
{
public:
  typedef K                    Kernel;
  typedef typename K::Site_2   Site_2;

  typedef Comparison_result    result_type;
  typedef Arity_tag<2>         Arity;
  typedef Site_2               argument_type;

  inline
  Comparison_result operator()(const Site_2& s1, const Site_2& s2) const
  {
    return CGAL::compare(s1.x(), s2.x());
  }
};

//--------------------------------------------------------------------

CGAL_END_NAMESPACE

#endif // CGAL_AG2_COMPARE_X_2_H
