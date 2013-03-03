// Copyright (c) 1997  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/HalfedgeDS/include/CGAL/HalfedgeDS_default.h $
// $Id: HalfedgeDS_default.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>

#ifndef CGAL_HALFEDGEDS_DEFAULT_H
#define CGAL_HALFEDGEDS_DEFAULT_H 1

#include <CGAL/HalfedgeDS_items_2.h>
#include <CGAL/HalfedgeDS_list.h>
#include <CGAL/memory.h>

CGAL_BEGIN_NAMESPACE

#ifndef CGAL_CFG_NO_TMPL_IN_TMPL_PARAM

template <class Traits_, class HalfedgeDSItems = HalfedgeDS_items_2, 
          class Alloc = CGAL_ALLOCATOR(int)>
class HalfedgeDS_default 
    : public HalfedgeDS_list< Traits_, HalfedgeDSItems, Alloc> {
public:
    typedef Traits_                                          Traits;
    typedef HalfedgeDS_list<Traits_, HalfedgeDSItems, Alloc> DS;
    typedef typename DS::size_type                           size_type;
    HalfedgeDS_default() {}
    HalfedgeDS_default( size_type v, size_type h, size_type f)
        : HalfedgeDS_list< Traits_, HalfedgeDSItems, Alloc>(v,h,f) {}
};
#define CGAL_HALFEDGEDS_DEFAULT  ::CGAL::HalfedgeDS_default

#else //  CGAL_CFG_NO_TMPL_IN_TMPL_PARAM //

struct HalfedgeDS_default {
  template <class Traits_, class HalfedgeDSItems = HalfedgeDS_items_2, 
            class Alloc = CGAL_ALLOCATOR(int)>
  class HDS : public HalfedgeDS_list::HDS<Traits_, HalfedgeDSItems, Alloc> {
  public:
      typedef Traits_                                               Traits;
      typedef HalfedgeDS_list::HDS<Traits_, HalfedgeDSItems, Alloc> DS;
      typedef typename DS::size_type                                size_type;
      HDS() {}
      HDS( size_type v, size_type h, size_type f)
          : HalfedgeDS_list::HDS<Traits_, HalfedgeDSItems, Alloc>(v,h,f) {}
  };
};

#define CGAL_HALFEDGEDS_DEFAULT  ::CGAL::HalfedgeDS_default::HDS

#endif // CGAL_CFG_NO_TMPL_IN_TMPL_PARAM //

CGAL_END_NAMESPACE
#endif // CGAL_HALFEDGEDS_DEFAULT_H //
// EOF //
