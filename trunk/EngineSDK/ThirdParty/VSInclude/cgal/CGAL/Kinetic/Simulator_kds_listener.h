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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Simulator_kds_listener.h $
// $Id: Simulator_kds_listener.h 29334 2006-03-10 00:00:09Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_NOTIFICATION_SK_HELPERS_H
#define CGAL_KINETIC_NOTIFICATION_SK_HELPERS_H
#include <CGAL/Kinetic/basic.h>

CGAL_KINETIC_BEGIN_NAMESPACE

//! This is a helper for KDSs which want to audit themselves when appropriate..
/*!  This listener object listens for
  Simulator_listener::HAS_EVENT_FREE_TIME notificatations and calls
  the audit method of the KDS. This object should be stored as a
  member variable in the KDS, since the pointer it contains to the KDS
  is not protected.

  See CGAL::Listener for a description of what the Simulator_listener
  template paramenter should provide.
*/
template <class Simulator_listener, class KDS>
class Simulator_kds_listener: public Simulator_listener
{
  typedef Simulator_listener P;
public:
  //! The only constructor
  Simulator_kds_listener(typename P::Notifier_handle sim,
			 KDS *kds): P(sim), t_(kds) {
    CGAL_precondition(kds != NULL);
  }
  Simulator_kds_listener(){}
  //! Pass HAS_AUDIT_TIME notifications via a call to the audit() function
  void new_notification(typename P::Notification_type t) {
    if (t== P::HAS_AUDIT_TIME) {
      if (P::notifier()->has_audit_time()) {
	t_->audit();
      }
    }
  }
protected:
  KDS *t_;
};

CGAL_KINETIC_END_NAMESPACE
#endif
