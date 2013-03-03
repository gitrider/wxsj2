// Copyright (c) 2005  Tel-Aviv University (Israel).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/Boolean_set_operations_2/Gps_bfs_scanner.h $
// $Id: Gps_bfs_scanner.h 28831 2006-02-27 14:28:18Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_GPS_BFS_SCANNER_H
#define CGAL_GPS_BFS_SCANNER_H

#include <queue>
#include <stack>

CGAL_BEGIN_NAMESPACE

template <class Arrangement_, class Visitor_>
class Gps_bfs_scanner
{
  typedef Arrangement_     Arrangement;

  typedef typename Arrangement::Hole_iterator    Hole_iterator;
  typedef typename Arrangement::Ccb_halfedge_circulator 
                                                  Ccb_halfedge_circulator;
  typedef typename Arrangement::Face_iterator    Face_iterator;
  typedef typename Arrangement::Halfedge_iterator Halfedge_iterator;
  
  typedef Visitor_         Visitor;

protected:
  Visitor*                     m_visitor;
  std::queue<Hole_iterator>   m_holes;
  std::stack<Ccb_halfedge_circulator>  m_ccb_stcak;

public:

  Gps_bfs_scanner(Visitor& v): m_visitor(&v)
  {}

  void scan(Arrangement& arr)
  {
    Face_iterator ubf = arr.unbounded_face();
    ubf->set_visited(true);
    push_to_queue_holes_of_face(ubf);
    
    while(!m_holes.empty())
    {
      Hole_iterator hole = m_holes.front();
      m_holes.pop();
      scan(*hole);
    }
  }

  void scan(Ccb_halfedge_circulator ccb)
  {
    _scan(ccb);
    while(!m_ccb_stcak.empty())
    {
      Ccb_halfedge_circulator curr_ccb = m_ccb_stcak.top();
      m_ccb_stcak.pop();
      _scan(curr_ccb);
    }

  }
  void _scan(Ccb_halfedge_circulator ccb)
  {
    Ccb_halfedge_circulator ccb_circ = ccb;
    Ccb_halfedge_circulator ccb_end  = ccb;
	  Face_iterator new_f;
    do
    {
      Halfedge_iterator he  = ccb_circ;
      new_f = he->twin()->face();
      if(!new_f->visited())
      {
        push_to_queue_holes_of_face(he->twin()->face());
        new_f->set_visited(true);
        m_visitor->flip_face(he->face(), new_f, he); 
        
        //scan(he->twin());
        m_ccb_stcak.push(he->twin());
      }
      ++ccb_circ;
    }
    while(ccb_circ != ccb_end);
  }

  
  void push_to_queue_holes_of_face(Face_iterator f)
  {
    for(Hole_iterator hit = f->holes_begin(); hit!= f->holes_end(); ++hit)
    {
      m_holes.push(hit);
    }
  }
};

CGAL_END_NAMESPACE

#endif
