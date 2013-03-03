// Copyright (c) 1999  Martin-Luther-University Halle-Wittenberg (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Point_set_2/include/CGAL/compare_vertices.h $
// $Id: compare_vertices.h 30322 2006-04-14 15:07:17Z lsaboret $
// 
//
// Author(s)     : Matthias Baesken

#ifndef CGAL_COMPARE_VERTICES_H
#define CGAL_COMPARE_VERTICES_H

CGAL_BEGIN_NAMESPACE

namespace CGALi {

// compare function objects for the priority queues used in nearest neighbor search
template<class VP, class NT,class MAP_TYPE>
class compare_vertices {
 public:
  //std::map<VP,NT,std::less<VP> > *pmap;
  MAP_TYPE* pmap;
  
  compare_vertices(MAP_TYPE *p){ pmap=p; }
  
  bool operator()(VP e1, VP e2)
  // get the priorities from the map and return result of comparison ...
  { NT& v1 = (*pmap)[e1];
    NT& v2 = (*pmap)[e2];
    return (v1 > v2);
  }
};


} // namespace CGALi


CGAL_END_NAMESPACE

#endif // CGAL_COMPARE_VERTICES_H

