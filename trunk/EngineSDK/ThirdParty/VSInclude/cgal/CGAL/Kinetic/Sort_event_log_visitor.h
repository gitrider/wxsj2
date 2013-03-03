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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Sort_event_log_visitor.h $
// $Id: Sort_event_log_visitor.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_DELAUNAY_SORT_WATCHER_EVENT_LOG_H
#define CGAL_KINETIC_DELAUNAY_SORT_WATCHER_EVENT_LOG_H
#include <CGAL/Kinetic/basic.h>
#include <string>
#include <sstream>
#include <vector>

CGAL_KINETIC_BEGIN_NAMESPACE

struct Sort_event_log_visitor
{
    Sort_event_log_visitor(){}
   
    template <class Vertex_handle>
    void remove_vertex(Vertex_handle a) {
      typedef typename std::iterator_traits<Vertex_handle>::value_type Key;
      std::ostringstream out;
      out << "Removing vertex " << *a;
      log_.push_back(out.str());
      CGAL_KINETIC_LOG(LOG_LOTS, "Logging: " << out.str());
    }

    template <class Vertex_handle>
    void create_vertex(Vertex_handle a) {
      typedef typename std::iterator_traits<Vertex_handle>::value_type Key;
      std::ostringstream out;
      out << "Creating vertex " << *a;
      log_.push_back(out.str());
      CGAL_KINETIC_LOG(LOG_LOTS, "Logging: " << out.str());
    }

    template <class Vertex_handle>
    void modify_vertex(Vertex_handle a) {
      typedef typename std::iterator_traits<Vertex_handle>::value_type Key;
      std::ostringstream out;
      out << "Changing vertex " << *a;
      log_.push_back(out.str());
      CGAL_KINETIC_LOG(LOG_LOTS, "Logging: " << out.str());
    }

    template <class Vertex_handle>
    void before_swap(Vertex_handle a,Vertex_handle b) {
      typedef typename std::iterator_traits<Vertex_handle>::value_type Key;
      std::ostringstream out;
      out << "Before swap of " << *a << " and " << *b;
      log_.push_back(out.str());
      CGAL_KINETIC_LOG(LOG_LOTS, "Logging: " << out.str());

    }
    template <class Vertex_handle>
    void after_swap(Vertex_handle a, Vertex_handle b) {
      typedef typename std::iterator_traits<Vertex_handle>::value_type Key;
      std::ostringstream out; 
      out << "After swap of " << *a << " and " << *b;
      log_.push_back(out.str());
      CGAL_KINETIC_LOG(LOG_LOTS, "Logging: " << out.str());

    }

  typedef std::vector<std::string>::const_iterator Event_iterator;
  Event_iterator events_begin()  const
  {
    return log_.begin();
  }
  Event_iterator events_end()  const
  {
    return log_.end();
  }

  size_t size() const
  {
    return log_.size();
  }
  
  std::vector<std::string> log_;
};

CGAL_KINETIC_END_NAMESPACE
#endif
