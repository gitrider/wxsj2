// Copyright (c) 2001  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/NT_converter.h $
// $Id: NT_converter.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_NT_CONVERTER_H
#define CGAL_NT_CONVERTER_H

#include <CGAL/functional_base.h>

CGAL_BEGIN_NAMESPACE

// A number type converter usable as default, using the conversion operator.

template < class NT1, class NT2 >
struct NT_converter
  : public std::unary_function< NT1, NT2 >
{
    NT2
    operator()(const NT1 &a) const
    {
        return NT2(a);
    }
};

// Some specializations for :
// - double to call to_double().
// - Interval_nt<> to call to_interval().
// - NT1 == NT2 to return a reference instead of copying.

template < class NT1 >
struct NT_converter < NT1, NT1 >
  : public std::unary_function< NT1, NT1 >
{
    const NT1 &
    operator()(const NT1 &a) const
    {
        return a;
    }
};

template < class NT1 >
struct NT_converter < NT1, double >
  : public std::unary_function< NT1, double >
{
    double
    operator()(const NT1 &a) const
    {
        return CGAL_NTS to_double(a);
    }
};

template <>
struct NT_converter < double, double >
  : public std::unary_function< double, double >
{
    const double &
    operator()(const double &a) const
    {
        return a;
    }
};

template < class NT1, bool b >
struct NT_converter < NT1, Interval_nt<b> >
  : public std::unary_function< NT1, Interval_nt<b> >
{
    Interval_nt<b>
    operator()(const NT1 &a) const
    {
        return CGAL_NTS to_interval(a);
    }
};

CGAL_END_NAMESPACE

#endif // CGAL_NT_CONVERTER_H
