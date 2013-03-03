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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/src/CGALQt/Kinetic_Qt_core.cpp $
// $Id: Kinetic_Qt_core.cpp,v 1.1 2007/07/09 09:52:11 chaojin Exp $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#include <CGAL/basic.h>

#include <CGAL/Kinetic/IO/internal/Qt_core.h>

#include "Kinetic_Qt_core.moc"

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE

Qt_core::Qt_core() {
  playable_=NULL;
  mode_=STOP;
}

void Qt_core::play_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Play button pushed.\n");
  mode_=RUN;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR("...but no handler was registered.\n");
  }
};

void Qt_core::pause_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Pause button pushed.\n");
  mode_=PAUSE;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR("...but no handler was registered.\n");
  }
}

void Qt_core::stop_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Stop button pushed.\n");
  mode_=STOP;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR( "...but no handler was registered.\n");
  }
}

void Qt_core::play_to_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Play_to button pushed.\n");
  mode_=RUN_TO;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR( "...but no handler was registered.\n");
  }
}

void Qt_core::play_through_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Play through button pushed.\n");
  mode_= RUN_THROUGH;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR( "...but no handler was registered.\n");
  }
}
void Qt_core::reverse_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Reverse button pushed.\n");
  mode_=REVERSE;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR("...but no handler was registered.\n");
  }
}
void Qt_core::faster_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Faster button pushed.\n");
  mode_=FASTER;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR( "...but no handler was registered.\n");
  }
}
void Qt_core::slower_button() {
  CGAL_KINETIC_LOG(LOG_SOME, "Slower button pushed.\n");
  mode_=SLOWER;
  if (playable_ != NULL) {
    playable_->new_notification(Listener::LAST_BUTTON_PRESSED);
  }
  else {
    CGAL_KINETIC_ERROR("...but no handler was registered.\n");
  }
}
CGAL_KINETIC_END_INTERNAL_NAMESPACE
