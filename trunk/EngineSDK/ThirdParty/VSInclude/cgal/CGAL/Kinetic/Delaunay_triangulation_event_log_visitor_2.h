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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Delaunay_triangulation_event_log_visitor_2.h $
// $Id: Delaunay_triangulation_event_log_visitor_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_DELAUNAY_TRIANGULATION_2_LOG_WATCHER_BASE_H
#define CGAL_KINETIC_DELAUNAY_TRIANGULATION_2_LOG_WATCHER_BASE_H
#include <CGAL/Kinetic/basic.h>
#include <vector>
#include <sstream>
#include <CGAL/Kinetic/Delaunay_triangulation_visitor_base_2.h>

CGAL_KINETIC_BEGIN_NAMESPACE

struct Delaunay_triangulation_event_log_visitor_2: public Delaunay_triangulation_visitor_base_2
{
    Delaunay_triangulation_event_log_visitor_2(){}

    template <class Edge>
    void before_flip(Edge e) {
        typedef typename Edge::first_type::value_type::Vertex_handle::value_type::Point Point;
        std::ostringstream out;
        Point a= e.first->vertex((e.second+1)%3)->point();
        Point b= e.first->vertex((e.second+2)%3)->point();
        out << "Flipping away edge {" << std::min(a,b) << ", " << std::max(a,b) << "}";
        log_.push_back(out.str());
        CGAL_KINETIC_LOG(LOG_LOTS, "Logging: " << out.str());
    }

    template <class Edge>
    void after_flip(Edge e) {
        typedef typename Edge::first_type::value_type::Vertex_handle::value_type::Point Point;
        std::ostringstream out;
        Point a= e.first->vertex((e.second+1)%3)->point();
        Point b= e.first->vertex((e.second+2)%3)->point();
        out << "Flipping in edge {" << std::min(a,b) << ", " << std::max(a,b) << "}";
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
