// Copyright (c) 1999, 2002, 2003  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/STL_Extension/include/CGAL/memory.h $
// $Id: memory.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel, Sylvain Pion

#ifndef CGAL_MEMORY_H
#define CGAL_MEMORY_H

#include <memory>

// CGAL_ALLOCATOR(t) defines the default allocator used by CGAL.
// CGAL_MEMORY(t) overloads the new and delete operators for a given class.

// When LEDA is there, the user could define these macros as
// leda_allocator< T >  and  LEDA_MEMORY(T) for example.

// For debugging with GCC, the following allocator can be useful :
// std::__allocator<T, std::__debug_alloc<std::__malloc_alloc_template<0> > >

#ifndef CGAL_ALLOCATOR
#  define CGAL_ALLOCATOR(T) std::allocator< T >
#endif

#ifndef CGAL_MEMORY
#  define CGAL_MEMORY(T)
#endif

#endif // CGAL_MEMORY_H
