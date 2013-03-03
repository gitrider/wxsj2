// Copyright (c) 1997-2000  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Qt_widget/include/CGAL/IO/Qt_widget_get_simple_polygon.h $
// $Id: Qt_widget_get_simple_polygon.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Laurent Rineau && Radu Ursu

#ifndef CGAL_QT_WIDGET_GET_SIMPLE_POLYGON_H
#define CGAL_QT_WIDGET_GET_SIMPLE_POLYGON_H

#include <CGAL/IO/Qt_widget_get_polygon.h>
#include <CGAL/Segment_2_Segment_2_intersection.h>  
#include <list>

#include <qcursor.h>

namespace CGAL {
template <class Polygon>
class Qt_widget_get_simple_polygon : public Qt_widget_get_polygon<Polygon>
{
public:
  typedef Qt_widget_get_polygon<Polygon>  Get_polygon;
  typedef typename Polygon::Point_2       Point_2;
  typedef typename Polygon::Segment_2     Segment_2;
  typedef typename Polygon::Edge_const_iterator  ECI;

  Qt_widget_get_simple_polygon(const QCursor
                               c=QCursor(Qt::crossCursor),QObject*
                               parent = 0, const char* name = 0)
    : Qt_widget_get_polygon<Polygon>(c, parent, name){}
  
protected:

  void mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == Qt::RightButton && this->is_pure(e->state()))
    {
      if (this->active) {
        if(!this->poly.is_simple()) return;
        if(this->poly.is_clockwise_oriented())
          this->poly.reverse_orientation ();
        assert( ! this->poly.is_clockwise_oriented());
      }
    }
    Get_polygon::mousePressEvent(e);
  }; 

private:
  bool is_simple()
  {
    Segment_2 rubber_segment(this->rubber, this->last_of_poly);
    if(this->poly.size() > 1)
    {
      ECI before_last_it = this->poly.edges_end();
      --before_last_it;
      --before_last_it;
      ECI it;
      for(it = this->poly.edges_begin(); it != before_last_it; it++)
      {
        if(do_intersect(*it, rubber_segment))
        return false;
      }
      //if I'm out of this means that all the edges, 
      //didn't intersect the last one
      ++it;
      Object o = intersection(*it, rubber_segment);
      Point_2 p;
      if(assign(p, o))
        return true;
      else
        return false;
    }
    else
      return true;
  }
};

} // namespace CGAL

#endif // CGAL_QT_WIDGET_GET_SIMPLE_POLYGON_H
