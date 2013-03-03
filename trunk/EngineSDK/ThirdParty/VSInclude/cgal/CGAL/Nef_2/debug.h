// Copyright (c) 1999  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Nef_2/include/CGAL/Nef_2/debug.h $
// $Id: debug.h 28576 2006-02-16 15:53:42Z hachenb $
// 
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>

#ifndef CGAL_DEBUG_H
#define CGAL_DEBUG_H

#include <iostream>

#ifdef NDEBUG
#undef CGAL_NEF_DEBUG
#define CGAL_NEF_DEBUG 1
#endif

#ifndef CGAL_NEF_DEBUG
#define CGAL_NEF_DEBUG 1
#endif

#ifndef NDEBUG
  static int debugthread=1;
#endif

#undef CGAL_NEF_TRACE
#undef CGAL_NEF_TRACEN
#undef CGAL_NEF_TRACEV
#undef CGAL_NEF_CTRACE
#undef CGAL_NEF_CTRACEN
#undef CGAL_NEF_SETDTHREAD

#ifndef NDEBUG
#define CGAL_NEF_SETDTHREAD(l) debugthread=l
#else
#define CGAL_NEF_SETDTHREAD(l)
#endif

#ifndef NDEBUG
#define CGAL_NEF_TRACE(t) if((debugthread%CGAL_NEF_DEBUG)==0) \
 std::cerr<<" "<<t; \
 std::cerr.flush()
#else
#define CGAL_NEF_TRACE(t) 
#endif

#ifndef NDEBUG
#define CGAL_NEF_TRACEV(t) if((debugthread%CGAL_NEF_DEBUG)==0) \
 std::cerr<<" "<<#t<<" = "<<(t)<<std::endl; \
 std::cerr.flush()
#else
#define CGAL_NEF_TRACEV(t) 
#endif

#ifndef NDEBUG
#define CGAL_NEF_TRACEN(t) if((debugthread%CGAL_NEF_DEBUG)==0) \
 std::cerr<< " "<<t<<std::endl; \
 std::cerr.flush()
#else
#define CGAL_NEF_TRACEN(t)
#endif

#ifndef NDEBUG
#define CGAL_NEF_CTRACE(b,t) if(b) std::cerr<<" "<<t; else std::cerr<<" 0"
#else
#define CGAL_NEF_CTRACE(b,t) 
#endif

#ifndef NDEBUG
#define CGAL_NEF_CTRACEN(b,t) if(b) std::cerr<<" "<<t<<"\n"; else std::cerr<<" 0\n"
#else
#define CGAL_NEF_CTRACEN(b,t) 
#endif

#endif // CGAL_DEBUG_H
