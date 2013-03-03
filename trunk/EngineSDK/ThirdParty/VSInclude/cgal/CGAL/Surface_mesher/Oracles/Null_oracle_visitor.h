// Copyright (c) 2005  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Surface_mesher/include/CGAL/Surface_mesher/Oracles/Null_oracle_visitor.h $
// $Id: Null_oracle_visitor.h 28605 2006-02-17 17:01:13Z lsaboret $
//
//
// Author(s)     : Laurent Rineau


#ifndef CGAL_SURFACE_MESHER_NULL_ORACLE_VISITOR_H
#define CGAL_SURFACE_MESHER_NULL_ORACLE_VISITOR_H

namespace CGAL {

  namespace Surface_mesher {

  /** \interface OracleVisitor
      \brief Concept of a visitor of oracles.

      \fn void new_point(Point& p)
      Called before a new Point is returned by the oracle.
      \param p The point that has just been computed and will be returned.
  */

  /** Trivial model of the OracleVisitor concept. */
  struct Null_oracle_visitor
  {
    template <class P>
    void new_point(P&) const
    {
    }
  };

  }  // namespace Surface_mesher

} // namespace CGAL


#endif  // CGAL_SURFACE_MESHER_NULL_ORACLE_VISITOR_H
