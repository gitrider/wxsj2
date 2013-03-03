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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/src/CGALQt/Kinetic_pixmaps.cpp $
// $Id: Kinetic_pixmaps.cpp,v 1.1 2007/07/09 09:52:11 chaojin Exp $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#include <CGAL/basic.h>

#include <CGAL/Kinetic/basic.h>
#include <CGAL/Kinetic/IO/internal/pixmaps.h>

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE
namespace pixmaps {
#include "Kinetic_play.xpm"
#include "Kinetic_pause.xpm"
#include "Kinetic_stop.xpm"
#include "Kinetic_play_to.xpm"
#include "Kinetic_play_through.xpm"
#include "Kinetic_reverse.xpm"
#include "Kinetic_faster.xpm"
#include "Kinetic_slower.xpm"
            }
            const char ** play_xpm= pixmaps::play_xpm;
            const char ** faster_xpm= pixmaps::faster_xpm;
            const char ** play_through_xpm= pixmaps::play_through_xpm;
            const char ** slower_xpm= pixmaps::slower_xpm;
            const char ** pause_xpm= pixmaps::pause_xpm;
            const char ** play_to_xpm= pixmaps::play_to_xpm;
            const char ** reverse_xpm= pixmaps::reverse_xpm;
            const char ** stop_xpm= pixmaps::stop_xpm;

CGAL_KINETIC_END_INTERNAL_NAMESPACE
