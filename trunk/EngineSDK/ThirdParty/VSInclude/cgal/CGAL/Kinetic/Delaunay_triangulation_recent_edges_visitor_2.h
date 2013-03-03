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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Delaunay_triangulation_recent_edges_visitor_2.h $
// $Id: Delaunay_triangulation_recent_edges_visitor_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_DELAUNAY_TRIANGULATION_2_RE_WATCHER_BASE_H
#define CGAL_KINETIC_DELAUNAY_TRIANGULATION_2_RE_WATCHER_BASE_H
#include <CGAL/Kinetic/basic.h>
#include <set>

CGAL_KINETIC_BEGIN_NAMESPACE

template <class Triangulation>
struct Delaunay_triangulation_recent_edges_visitor_2
{
  typedef typename Triangulation::Edge Edge;
  typedef typename Triangulation::Vertex_handle VH;
  Delaunay_triangulation_recent_edges_visitor_2(){}

  void remove_vertex(VH) {
    recent_.clear();
  }
  void create_vertex(VH) {
    recent_.clear();
  }

  void modify_vertex(VH vh) {
    recent_.clear();
    typename Triangulation::Edge_circulator ec= vh->incident_edges(), ef=ec;
    if (ec != NULL) {
      do {
	recent_.insert(*ec);
	++ec;
      } while (ec != ef);
    }
  }

  template <class It>
  void create_faces(It, It) {
  }

  template <class It>
  void remove_faces(It, It) {
  }

  void before_flip(Edge) {
    recent_.clear();
  }
  void after_flip(Edge e) {
    recent_.insert(e);
  }

  typedef typename std::set<Edge>::const_iterator iterator;
  iterator begin()  const
  {
    return recent_.begin();
  }
  iterator end()  const
  {
    return recent_.end();
  }

  bool contains(Edge e) const
  {
    return recent_.find(e) != recent_.end();
  }

  std::set<Edge> recent_;
};

CGAL_KINETIC_END_NAMESPACE
#endif
