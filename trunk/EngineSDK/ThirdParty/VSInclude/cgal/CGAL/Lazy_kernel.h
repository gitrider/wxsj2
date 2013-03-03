// Copyright (c) 2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_arithmetic/include/CGAL/Lazy_kernel.h $
// $Id: Lazy_kernel.h 30322 2006-04-14 15:07:17Z lsaboret $
// 
//
// Author(s)     : Andreas Fabri, Sylvain Pion

#ifndef CGAL_LAZY_KERNEL_H
#define CGAL_LAZY_KERNEL_H

#include <CGAL/basic.h>
#include <CGAL/Filtered_predicate.h>
#include <CGAL/Cartesian_converter.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Interval_nt.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Lazy.h>
#include <boost/mpl/if.hpp>


CGAL_BEGIN_NAMESPACE


// EK = exact kernel that will be made lazy
// Kernel = lazy kernel
template < typename EK_, typename AK_, typename E2A_, typename Kernel >
class Lazy_kernel_base {
public:
  typedef AK_   AK;
  typedef EK_   EK;
  typedef E2A_  E2A;

    template < typename Kernel2 >
    struct Base { typedef Lazy_kernel_base<EK, AK, E2A, Kernel2>  Type; };

    // What to do with the tag ?
    // Probably this should not exist, should it ?
    // struct filter_tag{};
    // typedef filter_tag                                     Kernel_tag;
   typedef typename EK::Kernel_tag                       Kernel_tag;
    // typedef typename CK::Rep_tag                          Rep_tag;

    // Types
  typedef CGAL::Lazy_exact_nt<typename EK::FT>  FT;
  typedef FT RT;

  typedef CGAL::Object Object_2;
  typedef CGAL::Object Object_3;

  typedef Lazy<typename AK::Point_2, typename EK::Point_2, typename EK::FT, E2A> Point_2;
  typedef Lazy<typename AK::Vector_2, typename EK::Vector_2, typename EK::FT, E2A> Vector_2;
  typedef Lazy<typename AK::Direction_2, typename EK::Direction_2, typename EK::FT, E2A> Direction_2;
  typedef Lazy<typename AK::Segment_2, typename EK::Segment_2, typename EK::FT, E2A> Segment_2;
  typedef Lazy<typename AK::Line_2, typename EK::Line_2, typename EK::FT, E2A> Line_2;
  typedef Lazy<typename AK::Ray_2, typename EK::Ray_2, typename EK::FT, E2A> Ray_2;
  typedef Lazy<typename AK::Triangle_2, typename EK::Triangle_2, typename EK::FT, E2A> Triangle_2;
  typedef Lazy<typename AK::Circle_2, typename EK::Circle_2, typename EK::FT, E2A> Circle_2;
  typedef Lazy<typename AK::Iso_rectangle_2, typename EK::Iso_rectangle_2, typename EK::FT, E2A> Iso_rectangle_2;
  typedef Lazy<typename AK::Aff_transformation_2, typename EK::Aff_transformation_2, typename EK::FT, E2A> Aff_transformation_2;
  typedef Cartesian_coordinate_iterator_2<Kernel> Cartesian_const_iterator_2;
  typedef Lazy<typename AK::Data_accessor_2, typename EK::Data_accessor_2, typename EK::FT, E2A> Data_accessor_2;
  typedef Lazy<typename AK::Conic_2, typename EK::Conic_2, typename EK::FT, E2A> Conic_2;

  typedef Lazy<typename AK::Point_3, typename EK::Point_3, typename EK::FT, E2A> Point_3;
  typedef Lazy<typename AK::Vector_3, typename EK::Vector_3, typename EK::FT, E2A> Vector_3;
  typedef Lazy<typename AK::Direction_3, typename EK::Direction_3, typename EK::FT, E2A> Direction_3;
  typedef Lazy<typename AK::Segment_3, typename EK::Segment_3, typename EK::FT, E2A> Segment_3;
  typedef Lazy<typename AK::Line_3, typename EK::Line_3, typename EK::FT, E2A> Line_3;
  typedef Lazy<typename AK::Ray_3, typename EK::Ray_3, typename EK::FT, E2A> Ray_3;
  typedef Lazy<typename AK::Plane_3, typename EK::Plane_3, typename EK::FT, E2A> Plane_3;
  typedef Lazy<typename AK::Triangle_3, typename EK::Triangle_3, typename EK::FT, E2A> Triangle_3;
  typedef Lazy<typename AK::Tetrahedron_3, typename EK::Tetrahedron_3, typename EK::FT, E2A> Tetrahedron_3;
  typedef Lazy<typename AK::Sphere_3, typename EK::Sphere_3, typename EK::FT, E2A> Sphere_3;
  typedef Lazy<typename AK::Iso_cuboid_3, typename EK::Iso_cuboid_3, typename EK::FT, E2A> Iso_cuboid_3;
  typedef Lazy<typename AK::Aff_transformation_3, typename EK::Aff_transformation_3, typename EK::FT, E2A> Aff_transformation_3;
  typedef Lazy<typename AK::Cartesian_const_iterator_3, typename EK::Cartesian_const_iterator_3, typename EK::FT, E2A> Cartesian_const_iterator_3;


    // We don't touch the predicates.
#define CGAL_Kernel_pred(P, Pf)  \
    typedef Filtered_predicate<typename EK::P, typename AK::P, \
	                     Exact_converter, Approx_converter> P; \
    P Pf() const { return P(); }


    // We change the constructions.
#ifdef CGAL_INTERSECT_WITH_ITERATORS_2
#define CGAL_Kernel_cons(C, Cf) \
    typedef typename boost::mpl::if_<boost::is_same<typename AK::C, typename AK::Intersect_with_iterators_2>, \
                                     Lazy_intersect_with_iterators<Kernel,typename AK::C, typename EK::C>, \
                                     typename boost::mpl::if_<boost::is_same<typename AK::C::result_type, Bbox_2>, \
                                     Lazy_construction_bbox<Kernel,typename AK::C, typename EK::C>, \
                                     typename boost::mpl::if_<boost::is_same<typename AK::C::result_type, typename AK::FT>,\
                                                              Lazy_construction_nt<Kernel,typename AK::C, typename EK::C>,\
                                                              typename boost::mpl::if_<boost::is_same<typename AK::C::result_type, Object >,\
                                                                                       Lazy_construction_object<Kernel,typename AK::C, typename EK::C>,\
                                                                                       Lazy_construction<Kernel,typename AK::C, typename EK::C> >::type >::type > ::type > ::type C; \
    C Cf() const { return C(); }

  CGAL_Kernel_cons(Intersect_with_iterators_2,
		   intersect_with_iterators_2_object)
#else 
#define CGAL_Kernel_cons(C, Cf) \
    typedef typename boost::mpl::if_<boost::is_same<typename AK::C, typename AK::Construct_cartesian_const_iterator_2>, \
                                     Lazy_cartesian_const_iterator_2<Kernel,typename AK::C, typename EK::C>, \
                                     typename boost::mpl::if_<boost::is_same<typename AK::C::result_type, Bbox_2>, \
                                     Lazy_construction_bbox<Kernel,typename AK::C, typename EK::C>, \
                                     typename boost::mpl::if_<boost::is_same<typename AK::C::result_type, typename AK::FT>,\
                                                              Lazy_construction_nt<Kernel,typename AK::C, typename EK::C>,\
                                                              typename boost::mpl::if_<boost::is_same<typename AK::C::result_type, Object >,\
                                                                                       Lazy_construction_object<Kernel,typename AK::C, typename EK::C>,\
                                                                                       Lazy_construction<Kernel,typename AK::C, typename EK::C> >::type >::type >::type > ::type C; \
    C Cf() const { return C(); }

#endif //CGAL_INTERSECT_WITH_ITERATORS_2


#include <CGAL/Kernel/interface_macros.h>

};

template <class EK, class AK, class E2A>
struct Lazy_kernel_adaptor
  : public Lazy_kernel_base< EK, AK, E2A, Lazy_kernel_adaptor<EK,AK, E2A> >
{};

template <class EK, class AK, class E2A>
struct Lazy_kernel_without_type_equality
  : public Lazy_kernel_base< EK, AK, E2A, Lazy_kernel_without_type_equality<EK,AK, E2A> >
{};

template <class EK, class AK = Simple_cartesian<Interval_nt_advanced>,
                    class E2A = Cartesian_converter<EK, AK,
                                To_interval<typename EK::RT> > >
struct Lazy_kernel
  : public Type_equality_wrapper< 
             Lazy_kernel_base< EK, AK, E2A, Lazy_kernel<EK, AK, E2A> >,
             Lazy_kernel<EK, AK, E2A> >
{};

CGAL_END_NAMESPACE

#endif // CGAL_LAZY_KERNEL_H
