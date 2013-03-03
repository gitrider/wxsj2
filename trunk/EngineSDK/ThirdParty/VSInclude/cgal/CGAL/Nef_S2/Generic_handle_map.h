// Copyright (c) 1997-2002  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Nef_S2/include/CGAL/Nef_S2/Generic_handle_map.h $
// $Id: Generic_handle_map.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel  <seel@mpi-sb.mpg.de>

#ifndef CGAL_GENERIC_HANDLE_MAP_H
#define CGAL_GENERIC_HANDLE_MAP_H

#include <CGAL/Unique_hash_map.h>

CGAL_BEGIN_NAMESPACE

struct Void_handle_hash_function {
    std::size_t operator() (void* h) const { 
        return std::size_t(h);
    }
};


template <class I>
class Generic_handle_map : public 
  Unique_hash_map<void*,I,Void_handle_hash_function>
{ typedef Unique_hash_map<void*,I,Void_handle_hash_function> Base;
public:
  Generic_handle_map() : Base() {}
  Generic_handle_map(I i) : Base(i) {}

  template <class H>
  const I& operator[](H h) const 
  { return Base::operator[](&*h); }

  template <class H>
  I& operator[](H h) 
  { return Base::operator[](&*h); }

};

CGAL_END_NAMESPACE
#endif //CGAL_GENERIC_HANDLE_MAP_H
