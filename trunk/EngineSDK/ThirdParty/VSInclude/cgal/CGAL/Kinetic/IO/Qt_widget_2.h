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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/IO/Qt_widget_2.h $
// $Id: Qt_widget_2.h 30976 2006-05-03 13:20:30Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_QT_SIMULATOR_2_H_
#define CGAL_KINETIC_QT_SIMULATOR_2_H_
#include <CGAL/Kinetic/basic.h>
#include <CGAL/Kinetic/IO/internal/GUI_base.h>
#include <CGAL/Kinetic/IO/internal/Qt_window_2.h>
#include <CGAL/Kinetic/IO/internal/Qt_widget_2_core.h>
#include <CGAL/Kinetic/IO/internal/Qt_timer.h>
#include <CGAL/Kinetic/Ref_counted.h>
#include <CGAL/Kinetic/Multi_listener.h>
#include <set>
#include <qapplication.h>

CGAL_KINETIC_BEGIN_NAMESPACE

//! A GUI for a kinetic simulation in 2D.
/*!
  In order to draw graphics, extend Qt_gui_2::Listener. There is
  one notification type PICTURE_IS_VALID which is created
  whenever the validity of the picture changes, for example if
  the time has changed or the picture needs to be redrawn for
  some other reason. To redraw, use Qt_gui_2::Listener::widget()
  to get a CGAL::Qt_widget and use this widget to draw the
  desired geometry.

  As you might guess, Qt is required.

  An example using this and Qt_moving_points_2 can be found in
  \example 2d_gui.cc.
*/
template <class Simulator_t >
class Qt_widget_2: public Ref_counted<Qt_widget_2<Simulator_t> >
{
public:
  // VC is complaningx
  typedef Qt_widget_2<Simulator_t> This;
  typedef Gui_base<Simulator_t, internal::Qt_timer> Graphical_base;
  typedef typename Simulator_t::Time Time;
  typedef typename internal::Qt_core_listener<Graphical_base> Window_listener;
  typedef internal::Qt_widget_2_core Qt_widget;
  typedef internal::Qt_window_2 Qt_window;
 
   class Base_listener;

  struct Listener_core
  {
    typedef Qt_widget_2<Simulator_t> Container;
    typedef enum {PICTURE_IS_VALID}
      Notification_type;
    typedef typename Container::Handle Notifier_handle;
    Qt_widget* widget() const {return widget_;}
  private:
    friend class Qt_widget_2<Simulator_t>;
    void set_widget(Qt_widget *w){ widget_=w;}
  protected:
    Qt_widget *widget_;
  };

  //! The base class for listeners for events.
  typedef Multi_listener<Listener_core> Listener;
  friend class  Multi_listener<Listener_core>;
  typedef Simulator_t Simulator;
  //typedef Const_ref_counted_pointer<This> Const_point;

  //! construct things
  Qt_widget_2(int argc, char *argv[],
	      typename  Simulator::Handle sh,
	      double xmin=-10,double xmax=10, double ymin=-10, double ymax=10): app_(new QApplication(argc, argv)),
										base_(new Graphical_base(sh)),
										base_l_( base_, this) {

    app_->setMainWidget(new Qt_window(static_cast<int>(std::floor(xmin)),
				      static_cast<int>(std::ceil(xmax)),
				      static_cast<int>(std::floor(ymin)),
				      static_cast<int>(std::ceil(ymax))));
    window()->setCaption("KDS");
    window()->show();
    window_l_ = std::auto_ptr<Window_listener>(new Window_listener(window()->button_handler(), base_));
    widget_l_ = std::auto_ptr<Widget_listener>(new Widget_listener(widget(), this));
  }

  //! start the gui
  int begin_event_loop() {
    widget()->set_picture_is_current(false);
    return app_->exec();
  }

  //! Access a reference counted pointer to the simulator.
  /*!
    I am not sure that I need this method.
    \todo check if these methods are needed.
  */
  const typename Simulator::Const_handle& simulator() const
  {
    bool let_me_know_if_this_is_used;
    return base_->simulator();
  }

  //! Access a reference counted pointer to the simulator.
  typename Simulator::Handle& simulator() {
    return base_->simulator();
  }
  //! Return true if the current image of the scene is valid.
  /*!
    If this is false, then things need to redraw themselves.
  */
  bool picture_is_valid() const
  {
    return widget()->picture_is_current();
  }
  //! Return the current time as a double.
  double current_time() const
  {
    return base_->current_time();
  }
  //! Return the current speed of the simulation.
  double speed() const
  {
    return base_->speed();
  }
  //! Set the current speed of the simulation.
  void set_speed(double s) const
  {
    base_->set_speed(s);
  }

  // these should be protected, but vc seems to have problems
  typedef typename Graphical_base::Listener GBL;
  class Base_listener: public GBL
  {
    typedef Qt_widget_2<Simulator_t> Container;
  public:
    Base_listener(typename Graphical_base::Handle &b, Container *t): GBL(b), t_(t){}

    virtual void new_notification(typename GBL::Notification_type nt) {
      if (nt== GBL::CURRENT_TIME) {
	t_->widget()->set_picture_is_current(false);
      }
    }
  protected:
    Container *t_;
  };

  typedef typename Qt_widget::Listener QTL;
  class Widget_listener: public QTL
  {
    typedef Qt_widget_2<Simulator_t> Container;
  public:
    Widget_listener(Qt_widget *w, Container *t): QTL(w),  t_(t) {
    }
    virtual void new_notification(typename QTL::Notification_type nt) {
      if (nt == QTL::PICTURE_IS_CURRENT) {
	if (!widget()->picture_is_current()) {
	  t_->draw();
	} else {
	  //std::cout << "Not drawing...\n";
	}
      }
    }
    virtual ~Widget_listener() {
    }
  protected:
    Container *t_;
  };

  //! Gui will call output_drawing
  void draw() {
    //std::cout << "GUI: Starting drawing.\n";
    //CGAL_KINETIC_LOG(LOG_LOTS, "GUI: Drawing in gui.\n");
    for (typename std::set<Listener*>::iterator dit= drawables_.begin();
	 dit != drawables_.end(); ++dit) {
      //std::cout << "GUI: Drawing something " << *dit << ".\n";
      (*dit)->new_notification(Listener::PICTURE_IS_VALID);
    }
  }

        

private:
  void new_listener(Listener* d) {
    CGAL_KINETIC_LOG(LOG_SOME, "GUI: Registered a drawable.\n");
    drawables_.insert(d);
    d->set_widget(widget());
    //
    //d->new_notification(PICTURE_IS_VALID);
  }
  void delete_listener(Listener* d) {
    CGAL_KINETIC_LOG(LOG_SOME,"GUI: Unregistered a drawable.\n");
    drawables_.erase(d);
  }

  friend class Widget_listener; friend class Base_listener;

  Qt_window *window() {
    return reinterpret_cast<Qt_window*>(app_->mainWidget());
  }

  const Qt_window *window() const
  {
    return reinterpret_cast<const Qt_window*>(app_->mainWidget());
  }

  Qt_widget* widget() {
    return window()->widget();
  }

  const Qt_widget* widget() const
  {
    return window()->widget();
  }
  This operator=(const This &o) {
    return *this;
  }
  Qt_widget_2(const This &o){}
protected:
  std::auto_ptr<QApplication> app_;
  typename Graphical_base::Handle base_;
  std::set<Listener*> drawables_;
  Base_listener base_l_;
  std::auto_ptr<Window_listener> window_l_;
  std::auto_ptr<Widget_listener> widget_l_;
};

CGAL_KINETIC_END_NAMESPACE
#endif                                            // guard
