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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/internal/Kernel/Cartesian_moving_lifted_point_3.h $
// $Id: Cartesian_moving_lifted_point_3.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_CARTESIAN_WEIGHTED_MOVING_LIFTED_3_H_
#define CGAL_KINETIC_CARTESIAN_WEIGHTED_MOVING_LIFTED_3_H_
#include <CGAL/Kinetic/basic.h>
#include <iostream>
#include <CGAL/Kinetic/Kernel/Cartesian_moving_point_3.h>

CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE;

template <class Coordinate_t>
class Cartesian_moving_lifted_point_3
{
    protected:
        typedef Cartesian_moving_lifted_point_3<Coordinate_t> This;
        typedef Cartesian_moving_point_3<Coordinate_t> Point;
    public:

        typedef Point Bare_point;
//typedef Static_point_t Static_point;

//! The cartesian coordinate type
        typedef Coordinate_t Coordinate;

//! What should I do for this
        typedef typename Coordinate::NT NT;

//! initialize it from polys
        Cartesian_moving_lifted_point_3(const Point &pt, const Coordinate &w): point_(pt), lifted_(w) {
        }

//! initialize it from a still point
        template <class Static_point>
            Cartesian_moving_lifted_point_3(const Static_point &pt): point_(pt.point()),
        lifted_(pt.weight()) {
        }

//! null
        Cartesian_moving_lifted_point_3(){}

        const Point &point() const
        {
            return point_;
        }

        const Coordinate &lifted() const
        {
            return lifted_;
        }

        bool is_constant() const
        {
            if (lifted_.degree() >0) return false;
            return point_.is_constant();
        }

        Coordinate weight() const
        {
//if (weight_==Coordinate()){
//	weight_=
            return point().x()*point().x()+ point().y()*point().y() + point().z()*point().z() - lifted_;
//}
//return weight_;
        }

//! Reverse the motion
        template <class CV>
            This negated_time(const CV &cv) const
        {
            This ret(point_.negated_time(cv), cv(lifted_));
            return ret;
        }

        template <class SK>
            struct Static_traits
        {
            typedef typename SK::Weighted_point Static_type;
            static Static_type to_static(const This &o, const typename Coordinate_t::NT &t, const SK&) {
                typename Coordinate_t::NT x=o.point().x()(t),
                    y= o.point().y()(t),
                    z= o.point().z()(t);
                return Static_type(typename SK::Bare_point(x,y,z),
                    CGAL::square(x)
                    + CGAL::square(y)
                    + CGAL::square(z)- o.lifted()(t));
            }
        };
        template <class Converter>
            struct Coordinate_converter
        {
            Coordinate_converter(const Converter &c): c_(c), pc_(c){}
            typedef Cartesian_moving_weighted_point_3<typename Converter::argument_type> argument_type;
            typedef Cartesian_moving_weighted_point_3<typename Converter::result_type> result_type;

            result_type operator()(const argument_type &i) const
            {
                return result_type(pc_(i.point()), c_(i.lifted()));
            }

            Converter c_;
            typename Bare_point::template Coordinate_converter<Converter> pc_;
        };
    protected:
        Point point_;
        Coordinate lifted_;
// Coordinate weight_;
};

template <class Coordinate>
std::ostream &operator<<(std::ostream &out, const Cartesian_moving_lifted_point_3<Coordinate> &point)
{
    out << point.point() << ", " <<  point.lifted();
    return out;
}


CGAL_KINETIC_END_INTERNAL_NAMESPACE;
#endif
