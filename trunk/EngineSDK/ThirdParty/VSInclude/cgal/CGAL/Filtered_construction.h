// Copyright (c) 2003,2004  INRIA Sophia-Antipolis (France) and
// Notre Dame University (U.S.A.).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Segment_Delaunay_graph_2/include/CGAL/Filtered_construction.h $
// $Id: Filtered_construction.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>




#ifndef CGAL_FILTERED_CONSTRUCTION_H
#define CGAL_FILTERED_CONSTRUCTION_H

#include <CGAL/basic.h>
#include <CGAL/Interval_arithmetic.h>

CGAL_BEGIN_NAMESPACE

template <class AC, class EC, class FC, class C2E, class C2F,
	  class E2C, class F2C,	bool Protection = true>
class Filtered_construction
{
private:
  EC Exact_construction;
  FC Filter_construction;
  C2E To_Exact;
  C2F To_Filtered;
  E2C From_Exact;
  F2C From_Filtered;

  typedef typename AC::result_type  AC_result_type;
  typedef typename FC::result_type  FC_result_type;
  typedef typename EC::result_type  EC_result_type;

public:
  typedef AC_result_type           result_type;
  typedef typename AC::Arity       Arity;

public:
  Filtered_construction() {}

  template <class A1>
  result_type
  operator()(const A1 &a1) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      return From_Filtered( Filter_construction(To_Filtered(a1)) );
    }
    catch (Interval_nt_advanced::unsafe_comparison)
    {
      Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
      return From_Exact( Exact_construction(To_Exact(a1)) );
    }
  }
  template <class A1, class A2>
  result_type
  operator()(const A1 &a1, const A2 &a2) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      return From_Filtered( Filter_construction(To_Filtered(a1),
						To_Filtered(a2)) );
    }
    catch (Interval_nt_advanced::unsafe_comparison)
    {
      Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
      return From_Exact( Exact_construction(To_Exact(a1),
					    To_Exact(a2)) );
    }
  }

  template <class A1, class A2, class A3>
  result_type
  operator()(const A1 &a1, const A2 &a2, const A3 &a3) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      return From_Filtered( Filter_construction(To_Filtered(a1),
						To_Filtered(a2),
						To_Filtered(a3)) );
    }
    catch (Interval_nt_advanced::unsafe_comparison)
    {
      Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
      return From_Exact( Exact_construction(To_Exact(a1),
					    To_Exact(a2),
					    To_Exact(a3)) );
    }
  }

};



CGAL_END_NAMESPACE


#endif // CGAL_FILTERED_CONSTRUCTION_H
