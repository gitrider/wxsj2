// Copyright (c) 1997  Tel-Aviv University (Israel).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Sweep_line_2/Sweep_line_2_debug.h $
// $Id: Sweep_line_2_debug.h 28840 2006-02-27 14:36:55Z baruchzu $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_SWEEP_LINE_2_DEBUG_H
#define CGAL_SWEEP_LINE_2_DEBUG_H


#include <CGAL/Basic_sweep_line_2.h>


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//                         DEBUG UTILITIES                                //
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


template <class Traits_,
          class SweepVisitor,
          class CurveWrap,
          class SweepEvent,
          typename Allocator>
inline void 
Basic_sweep_line_2<Traits_,
                   SweepVisitor,
                   CurveWrap,
                   SweepEvent,
                   Allocator>::
PrintEventQueue()
{
  SL_DEBUG(std::cout << std::endl << "Event queue: " << std::endl;)
  EventQueueIter iter = m_queue->begin();
  while ( iter != m_queue->end() )
  {
    SL_DEBUG(std::cout << "Point (" << iter->first << ")" << std::endl;)
    Event *e = iter->second;
    e->Print();
    ++iter;
  }
  SL_DEBUG(std::cout << "--------------------------------" << std::endl;)
}

template <class Traits_,
          class SweepVisitor,
          class CurveWrap,
          class SweepEvent,
          typename Allocator>
inline void 
Basic_sweep_line_2<Traits_,
                   SweepVisitor,
                   CurveWrap,
                   SweepEvent,
                   Allocator>::
PrintSubCurves()
{
  SL_DEBUG(std::cout << std::endl << "Sub curves: " << std::endl;)
  for(unsigned int i=0 ; i < m_num_of_subCurves ; ++i)
  {
    m_subCurves[i].Print();
  }
}

template < class SweepLineTraits_2,
           class SweepEvent, class CurveWrap, class SweepNotif,
           typename Allocator >
inline void 
Basic_sweep_line_2<SweepLineTraits_2,SweepEvent,CurveWrap,SweepNotif,
                  Allocator>::
PrintStatusLine()
{
  if ( m_statusLine.size() == 0) {
    std::cout << std::endl << "Status line: empty" << std::endl;
    return;
  }
  std::cout << std::endl << "Status line: (" 
            << m_currentEvent->get_point() << ")" << std::endl;
  StatusLineIter iter = m_statusLine.begin();
  while ( iter != m_statusLine.end() )
  {
    (*iter)->Print();
    ++iter;
  }
  std::cout << "Status line - end" << std::endl;
}



#endif
