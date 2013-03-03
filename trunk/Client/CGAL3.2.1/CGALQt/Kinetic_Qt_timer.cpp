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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/src/CGALQt/Kinetic_Qt_timer.cpp $
// $Id: Kinetic_Qt_timer.cpp,v 1.1 2007/07/09 09:52:11 chaojin Exp $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#include <CGAL/basic.h>

#include <CGAL/Kinetic/basic.h>
#include <CGAL/Kinetic/IO/internal/Qt_timer.h>

#include "Kinetic_Qt_timer.moc"

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE

Qt_timer::Qt_timer(): tick_(0), id_(-1) {
  connect( &timer_, SIGNAL(timeout()),
	   this, SLOT(timerDone()) );

}

void Qt_timer::run(double time_in_seconds) {
  id_=timer_.start(static_cast<int>(time_in_seconds*1000), true);
  //CGAL_postcondition(id_ != -1);
}

void Qt_timer::timerDone() {
  ++tick_;
  cb_->new_notification(Listener::TICKS);
}
CGAL_KINETIC_END_INTERNAL_NAMESPACE
