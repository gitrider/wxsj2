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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/IO/internal/Qt_widget_2_core.h $
// $Id: Qt_widget_2_core.h 30976 2006-05-03 13:20:30Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_IO_INTERNAL_QT_WIDGET_2_CORE_H
#define CGAL_KINETIC_IO_INTERNAL_QT_WIDGET_2_CORE_H

#include <CGAL/Kinetic/basic.h>
#include <CGAL/IO/Qt_widget.h>
#include <qmainwindow.h>

namespace CGAL
{
  namespace Kinetic
  {
    namespace internal
    {
      class Qt_widget_2_core: public ::CGAL::Qt_widget
      {
	Q_OBJECT
      public:
	class Listener
	{
	public:
	  Listener(Qt_widget_2_core *widget): widget_(widget) {
	    CGAL_precondition(widget!= NULL);
	    widget_->set_listener(this);
	  }
	  virtual ~Listener() {
	    // could check first
	    widget_->set_listener(NULL);
	  }
	  typedef enum {PICTURE_IS_CURRENT}
	  Notification_type;
	  virtual void new_notification(Notification_type) {
	    //CGAL_KINETIC_ERROR( "draw not implemented.\n");
	    std::cerr << "Drawing but nothing is to be drawn.\n";
	  }
	  Qt_widget_2_core *widget(){return widget_;}
	protected:
	  Qt_widget_2_core *widget_;
	};

	Qt_widget_2_core(QMainWindow *parent);

	//! do not call, this is for Qt use.
	void redraw() ;

	bool picture_is_current() const
	{
	  return is_drawn_;
	}
	void set_picture_is_current(bool tf) {
	  if (tf==false) redraw();
	}
      protected:
	Listener *drawable_;
	bool is_drawn_;
      private:
	friend class Listener;
	void set_listener(Listener *d) {
	  //CGAL_precondition(drawable_==NULL);
	  drawable_=d;
	  //redraw(); // this doesn't work since virtual functions can't be called from teh constructor
	}
      };
    }
  }
}
#endif
