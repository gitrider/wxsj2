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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Insert_event.h $
// $Id: Insert_event.h 29334 2006-03-10 00:00:09Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_INSERT_EVENT_H_
#define CGAL_KINETIC_INSERT_EVENT_H_
#include <CGAL/Kinetic/basic.h>
#include <CGAL/Kinetic/internal/To_static.h>

CGAL_KINETIC_BEGIN_NAMESPACE;

//! An event to insert a single object into a MovingObjectTable

template <class MOT>
class Insert_event
{
  typedef typename MOT::Handle Pointer;
  typedef typename MOT::Data Object;
public:
  //! Construct it with the time, the object and where to put it
  Insert_event(const Object &obj,
	       Pointer mot):mot_(mot),
			    obj_(obj){}
  template <class T>
  void process(const T&) {
    CGAL_KINETIC_LOG(LOG_SOME, "Inserting object.\n");
    mot_->insert(obj_);
  }

  void write(std::ostream &out) const
  {
    out << " I" << obj_ ;
  }
protected:
  Pointer mot_;
  Object obj_;
};

template <class MH>
std::ostream &operator<<(std::ostream &out, const Insert_event< MH> &moi)
{
  moi.write(out);
  return out;
}


CGAL_KINETIC_END_NAMESPACE;
#endif
