// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/internal/Kernel/cartesian_predicates_2.h $
// $Id: cartesian_predicates_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_CARTESIAN_PREDICATES_2_H_
#define CGAL_KINETIC_CARTESIAN_PREDICATES_2_H_
#include <CGAL/Kinetic/basic.h>
#include <CGAL/determinant.h>

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE

template <class KK>
struct Cartesian_orientation_2
{
    Cartesian_orientation_2(){}
  typedef typename KK::Certificate_function result_type;
    typedef typename KK::Point_2 first_argument_type;
    typedef typename KK::Point_2 second_argument_type;
    typedef typename KK::Point_2 third_argument_type;
    result_type operator()(const first_argument_type &a,
        const second_argument_type &b,
        const third_argument_type &c) const
    {
        typedef typename KK::Certificate_function FT;
        FT a00= a.hx();
        FT a01= a.hy();
        FT a10= b.hx();
        FT a11= b.hy();
        FT a20= c.hx();
        FT a21= c.hy();

// First compute the det2x2
        const FT m01 = a00*a11 - a10*a01;
        const FT m02 = a00*a21 - a20*a01;
        const FT m12 = a10*a21 - a20*a11;
// Now compute the minors of rank 3
        const FT m012 = m01 - m02 + m12;
        return m012;
    }
};

template <class KK>
struct Cartesian_side_of_oriented_circle_2
{
    Cartesian_side_of_oriented_circle_2(){}
  typedef typename KK::Certificate_function result_type;
    typedef typename KK::Point_2 first_argument_type;
    typedef typename KK::Point_2 second_argument_type;
    typedef typename KK::Point_2 third_argument_type;
    typedef typename KK::Point_2 fourth_argument_type;
    result_type operator()(const first_argument_type &ap,
        const second_argument_type &bp,
        const third_argument_type &cp,
        const fourth_argument_type &dp) const
    {
        typedef typename KK::Certificate_function RT;

        const RT qhx = ap.hx();
        const RT qhy = ap.hy();
        const RT rhx = bp.hx();
        const RT rhy = bp.hy();
        const RT shx = cp.hx();
        const RT shy = cp.hy();
        const RT thx = dp.hx();
        const RT thy = dp.hy();

// compute sign of      |qx  qy  qx^2+qy^2  1 |   | a b c d |
//                      |      --  r  --      | = | e f g h |
//     determinant      |      --  s  --      | = | i j k l |
//                      |      --  t  --      |   | m n o p |
//           where

        RT a = qhx;
        RT b = qhy;
        RT c = qhx*qhx + qhy*qhy;

        RT e = rhx;
        RT f = rhy;
        RT g = rhx*rhx + rhy*rhy;

        RT i = shx;
        RT j = shy;
        RT k = shx*shx + shy*shy;

        RT m = thx;
        RT n = thy;
        RT o = thx*thx + thy*thy;

        RT det =   a * ( f*(k - o) + j*(o - g) + n*(g - k) )
            - e * ( b*(k - o) + j*(o - c) + n*(c - k) )
            + i * ( b*(g - o) + f*(o - c) + n*(c - g) )
            - m * ( b*(g - k) + f*(k - c) + j*(c - g) );

        return det;
    }
};

template <class KK>
struct Cartesian_less_x_1
{
    Cartesian_less_x_1(){}
    typedef typename KK::Certificate_function result_type;
    typedef typename KK::Point_1 first_argument_type;
    typedef typename KK::Point_1 second_argument_type;
    result_type operator()(const first_argument_type &a,
        const second_argument_type &b) const
    {
        return b.x() - a.x();
    }
};

template <class KK>
struct Cartesian_less_x_2
{
    Cartesian_less_x_2(){}
    typedef typename KK::Certificate_function result_type;
    typedef typename KK::Point_2 first_argument_type;
    typedef typename KK::Point_2 second_argument_type;
    result_type operator()(const first_argument_type &a,
        const second_argument_type &b) const
    {
        return b.x() - a.x();
    }
};

template <class KK>
struct Cartesian_less_y_2
{
    Cartesian_less_y_2(){}
    typedef typename KK::Certificate_function result_type;
    typedef typename KK::Point_2 first_argument_type;
    typedef typename KK::Point_2 second_argument_type;
    result_type operator()(const first_argument_type &a,
        const second_argument_type &b) const
    {
        return b.y() - a.y();
    }
};

CGAL_KINETIC_END_INTERNAL_NAMESPACE
#endif
