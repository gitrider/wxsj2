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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Active_objects_listener_helper.h $
// $Id: Active_objects_listener_helper.h 31630 2006-06-16 13:09:51Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_NOTIFICATION_LH2_HELPERS_H
#define CGAL_KINETIC_NOTIFICATION_LH2_HELPERS_H
#include <CGAL/Kinetic/basic.h>

CGAL_KINETIC_BEGIN_NAMESPACE

//! A helper for KDSs interested in events from MovingObjectTables.
/*!
  The lister helper listens for EDITING notifications and calls the functions
  -new_object
  -change_object
  -delete_object
  with the appropriate object keys from the moving object table.

  This helper should be stored as a member variable in the KDS since
  its pointer to the KDS is not protected.

  See CGAL::Listener for a description of what the
  Moving_object_table_listener template paramenter should provide.
*/
template <class Moving_object_table_listener, class KDS>
class Active_objects_listener_helper: public Moving_object_table_listener
{
  typedef typename Moving_object_table_listener::Notifier_handle::element_type MOT;
  typedef Moving_object_table_listener P;
public:

  //! The constructor
  Active_objects_listener_helper(typename Moving_object_table_listener::Notifier_handle h,
                                 KDS *kds):
    Moving_object_table_listener(h), t_(kds) {
    for (typename Moving_object_table_listener::Notifier::Key_iterator it= P::notifier()->keys_begin();
         it != P::notifier()->keys_end(); ++it) {
      t_->insert(*it);
    }
  }
  Active_objects_listener_helper(){}
  //! Pass EDITING notifications
  /*!  When editing changes to false, call new_object, changed_object,
    deleted_object for each new, changed or deleted object in the
    MovingObjectTable.
  */
  virtual void new_notification(typename Moving_object_table_listener::Notification_type et) {
    if (et== P::IS_EDITING) {
      if (P::notifier()->is_editing()==false) {
	//! Note, this order is important
	for (typename MOT::Inserted_iterator it= P::notifier()->inserted_begin();
	     it != P::notifier()->inserted_end(); ++it) {
	  t_->insert(*it);
	}
	for (typename MOT::Changed_iterator it= P::notifier()->changed_begin();
	     it != P::notifier()->changed_end(); ++it) {
	  t_->set(*it);
	}
	for (typename MOT::Erased_iterator it= P::notifier()->erased_begin();
	     it != P::notifier()->erased_end(); ++it) {
	  t_->erase(*it);
	}
      }
    }
  }

protected:
  KDS *t_;
};

CGAL_KINETIC_END_NAMESPACE
#endif
