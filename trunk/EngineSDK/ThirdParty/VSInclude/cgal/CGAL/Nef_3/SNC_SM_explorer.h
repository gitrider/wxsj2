// Copyright (c) 1997-2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Nef_3/include/CGAL/Nef_3/SNC_SM_explorer.h $
// $Id: SNC_SM_explorer.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Peter Hachenberger <hachenberger@mpi-sb.mpg.de>

#ifndef CGAL_SNC_SM_EXPLORER_H
#define CGAL_SNC_SM_EXPLORER_H

#include <CGAL/basic.h>
#include <CGAL/Nef_S2/SM_const_decorator.h>

CGAL_BEGIN_NAMESPACE

template <typename SMCDEC>
class SNC_SM_explorer : public SMCDEC {
  typedef SMCDEC                            Base;
  typedef SNC_SM_explorer<SMCDEC>           Self;
  //  typedef typename Base::Kernel    Kernel;
  //  typedef typename Kernel::Point_3 Point_3;

 public:
  SNC_SM_explorer(const Base& E) : Base(E) {}
  Self& operator=(const Self& E) {
    Base::operator=(E); 
    return *this;
  }
};

CGAL_END_NAMESPACE

#endif  // CGAL_SNC_SM_EXPLORER_H
