// Copyright (c) 2003  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Apollonius_graph_2/include/CGAL/more_functions_on_signs.h $
// $Id: more_functions_on_signs.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>



#ifndef CGAL_MORE_FUNCTIONS_ON_SIGNS_H
#define CGAL_MORE_FUNCTIONS_ON_SIGNS_H

#include <CGAL/enum.h>

CGAL_BEGIN_NAMESPACE
inline
Sign
sign_of_first_root(const Sign &sb, const Sign &sc)
{
  // this method computes the sign of the second root of the quadratic
  // equation: a x^2 - b x + c == 0. It is assumed that the sign of a
  // is +1.

  if ( sc == NEGATIVE )
    return NEGATIVE;

  if ( sb == POSITIVE )  return sc;
  if ( sb == NEGATIVE )  return NEGATIVE;
  return opposite(sc);
}


inline
Sign
sign_of_second_root(const Sign &sb, const Sign &sc)
{
  // this method computes the sign of the first root of the quadratic
  // equation: a x^2 - b x + c == 0. It is assumed that the sign of a
  // is +1.

  if ( sc == NEGATIVE )
    return POSITIVE;

  if ( sb == POSITIVE )  return POSITIVE;
  if ( sb == NEGATIVE )  return opposite(sc);
  return sc;
}

CGAL_END_NAMESPACE

#endif // CGAL_MORE_FUNCTIONS_ON_SIGNS_H
