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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Qt_widget/include/CGAL/IO/Custom_zoom_layer.h $
// $Id: Custom_zoom_layer.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Radu Ursu

#ifndef CGAL_CUSTOM_ZOOM_LAYER_H
#define CGAL_CUSTOM_ZOOM_LAYER_H

#include <CGAL/IO/Qt_widget_zoomrect.h>

class Custom_zoom_layer : public CGAL::Qt_widget_zoomrect{
public:
  Custom_zoom_layer(QObject* parent = 0, const char* name = 0)
    : Qt_widget_zoomrect(parent, name){}
  
protected:
  void keyPressEvent(QKeyEvent *e){
    if ( e->key() == Qt::Key_Plus ){
      widget->zoom(2);
    } else if ( e->key() == Qt::Key_Minus){
      widget->zoom(0.5);
    }
  }
  void mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == Qt::LeftButton 
      && (e->state() & Qt::ControlButton))
    {
      if (!on_first){
        first_x = e->x();
        first_y = e->y();
        on_first = true;
      }
    }
  }
  void mouseReleaseEvent(QMouseEvent *e)
  {
    if(e->button() == Qt::LeftButton
      && (e->state() & Qt::ControlButton))
    {
      if((e->x() != first_x) && (e->y() != first_y)) {
        double x, y, xfirst2, yfirst2;
        widget->x_real(e->x(), x); widget->y_real(e->y(), y);
        widget->x_real(first_x, xfirst2); widget->y_real(first_y, yfirst2);
        double	xmin, xmax, ymin, ymax;
        if(x < xfirst2) {xmin = x; xmax = xfirst2;}
        else {xmin = xfirst2; xmax = x;};
        if(y < yfirst2) {ymin = y; ymax = yfirst2;}
        else {ymin = yfirst2; ymax = y;};
        widget->set_window(xmin, xmax, ymin, ymax);        
        on_first = false;
      }
    }
  }
  void activating(){
    widget->setFocusPolicy(QWidget::ClickFocus);
    oldcursor = widget->cursor();
    widget->setCursor(crossCursor);
  }
  QCursor oldcursor;
};

#endif