// Copyright (c) 2005  INRIA Sophia-Antipolis (France) and
// Notre Dame University (U.S.A.).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Segment_Delaunay_graph_2/include/CGAL/Edge_hash_function.h $
// $Id: Edge_hash_function.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>

#ifndef CGAL_EDGE_HASH_FUNCTION_H
#define CGAL_EDGE_HASH_FUNCTION_H

#include <CGAL/basic.h>
#include <CGAL/Handle_hash_function.h>

CGAL_BEGIN_NAMESPACE


class Edge_hash_function
  : public Handle_hash_function
{
private:
  typedef Handle_hash_function     Base;

public:
  typedef Base::result_type        result_type;

  template<class Edge>
  result_type operator()(const Edge& e) const
  {
    return (Base::operator()(e.first)) << e.second;
  }
};


CGAL_END_NAMESPACE


#endif // CGAL_EDGE_HASH_FUNCTION_H
