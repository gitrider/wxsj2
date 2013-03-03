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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Qt_widget/include/CGAL/IO/Navigation_layer.h $
// $Id: Navigation_layer.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Radu Ursu

#ifndef CGAL_NAVIGATION_LAYER_H
#define CGAL_NAVIGATION_LAYER_H

#include <CGAL/IO/Qt_widget_layer.h>

class Navigation_layer : public CGAL::Qt_widget_layer {
public:
  Navigation_layer(QObject *parent=0, const char* name=0)
    : Qt_widget_layer(parent, name){}
  void draw(){};
protected:
  void keyPressEvent(QKeyEvent *e){
    const double dx = widget->x_real(10) - widget->x_real(0);
    const double dy = widget->y_real(10) - widget->y_real(0);
    const double ypage = widget->y_max() - widget->y_min();
    if ( e->key() == Qt::Key_Left ){
      widget->move_center(dx, 0);
    } else if ( e->key() == Qt::Key_Right ){
      widget->move_center(-dx, 0);
    } else if ( e->key() == Qt::Key_Down ){
      widget->move_center(0, -dy);
    } else if ( e->key() == Qt::Key_Up ){
      widget->move_center(0, dy);
    } else if ( e->key() == Qt::Key_Prior ){ //PageUp
      widget->move_center(0, -ypage/2);
    } else if ( e->key() == Qt::Key_Next ){ //PageDown
      widget->move_center(0, ypage/2);
    } 
  }
  void activating(){
    widget->setFocusPolicy(QWidget::ClickFocus);
  }
};

#endif