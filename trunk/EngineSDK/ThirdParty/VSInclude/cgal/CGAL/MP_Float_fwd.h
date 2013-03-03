// Copyright (c) 2001-2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/MP_Float_fwd.h $
// $Id: MP_Float_fwd.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_MP_FLOAT_FWD_H
#define CGAL_MP_FLOAT_FWD_H

#include <CGAL/Quotient_fwd.h>

// Forward declarations

CGAL_BEGIN_NAMESPACE

class MP_Float;

Comparison_result compare(const MP_Float&, const MP_Float&);
Sign sign(const MP_Float &);
MP_Float square(const MP_Float&);
MP_Float approximate_sqrt(const MP_Float&);
double to_double(const MP_Float&);
std::pair<double,double> to_interval(const MP_Float &);
bool is_finite(const MP_Float &);
bool is_valid(const MP_Float &);

double to_double(const Quotient<MP_Float>&);
std::pair<double,double> to_interval(const Quotient<MP_Float>&);

CGAL_END_NAMESPACE

#endif // CGAL_MP_FLOAT_FWD_H
