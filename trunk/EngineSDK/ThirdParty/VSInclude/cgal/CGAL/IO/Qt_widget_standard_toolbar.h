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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Qt_widget/include/CGAL/IO/Qt_widget_standard_toolbar.h $
// $Id: Qt_widget_standard_toolbar.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Radu Ursu


#ifndef CGAL_QT_WIDGET_STANDARD_TOOLBAR_H
#define CGAL_QT_WIDGET_STANDARD_TOOLBAR_H

#include <CGAL/basic.h>

#include <qtoolbar.h>
#include <qbuttongroup.h>
#include <qtoolbutton.h>

namespace CGAL {

class Qt_widget;
class Qt_widget_history;

class Qt_widget_standard_toolbar : public QToolBar
{
	Q_OBJECT
public:
  Qt_widget_standard_toolbar(Qt_widget *w,
			     QMainWindow *parent = 0,
			     const char* name = 0);

  Qt_widget_standard_toolbar(Qt_widget *w,
			     QMainWindow *mw,
			     QWidget* parent,
			     bool newLine = true,
			     const char* name = 0);

  ~Qt_widget_standard_toolbar() { delete button_group; }

  // CGAL-2.4 compatibility
  QToolBar*	toolbar(){return this;}

public slots:
  void back();
  void forward();
  void clear_history();

private slots:
  void zoomin();
  void zoomout();
  void group_clicked(int i);

private:
  void fill_toolbar(QMainWindow *mw);

private:
  Qt_widget          *widget;
  Qt_widget_history  *history;
  QButtonGroup*      button_group;
  // this group has no parent and is destroyed manually in the
  // destructor

  QToolButton* nolayerBt;
};//end class

};//end namespace

#endif //CGAL_QT_WIDGET_STANDARD_TOOLBAR_H
