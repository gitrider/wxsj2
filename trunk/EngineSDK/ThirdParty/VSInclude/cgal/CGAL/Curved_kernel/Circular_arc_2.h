// Copyright (c) 2003-2006  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Curved_kernel/include/CGAL/Curved_kernel/Circular_arc_2.h $
// $Id: Circular_arc_2.h 30992 2006-05-04 07:53:23Z pmachado $
//
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_CURVED_KERNEL_CIRCULAR_ARC_2_H
#define CGAL_CURVED_KERNEL_CIRCULAR_ARC_2_H

#include <CGAL/global_functions_on_circular_arcs_2.h>
#include <CGAL/Curved_kernel/internal_functions_on_circular_arc_2.h> // temporarily
#include <CGAL/intersections.h>
#include <CGAL/Simple_cartesian.h>

namespace CGAL {
namespace CGALi {

  template <class CK >
  class Circular_arc_2
  {
    typedef typename CK::FT                        FT;
    typedef typename CK::RT                        RT;
    typedef typename CK::Point_2                   Point_2;
    typedef typename CK::Line_2                    Line_2;
    typedef typename CK::Circle_2                  Circle_2;
    typedef typename CK::Circular_arc_point_2      Circular_arc_point_2;
    typedef typename CK::Root_of_2                 Root_of_2;

  public:

    Circular_arc_2() {}

    Circular_arc_2(const Circle_2 &c)
      : _support(c), Cache_minmax('s'), Cache_full('y')
    {

      _begin = _end  = 
	CircularFunctors::x_extremal_point<CK>(supporting_circle(),true); 
      
    }

    Circular_arc_2(const Circle_2 &support,
                   const Line_2 &l1, bool b1,
                   const Line_2 &l2, bool b2) 
      : Cache_minmax('n'), Cache_full('n')
    {
      Point_2 center1 (support.center().x() + l1.a()/2,
                       support.center().y() + l1.b()/2);

      FT sqr1 = support.squared_radius() + l1.c()
	        - CGAL::square(support.center().x())
	        - CGAL::square(support.center().y())
	        + CGAL::square(center1.x())
	        + CGAL::square(center1.y());

      Circle_2 c1 (center1, sqr1);
      
      Point_2 center2 (support.center().x() + l2.a()/2,
                       support.center().y() + l2.b()/2);

      FT sqr2 = support.squared_radius() + l2.c()
	        - CGAL::square(support.center().x())
	        - CGAL::square(support.center().y())
	        + CGAL::square(center2.x())
	        + CGAL::square(center2.y());

      Circle_2 c2 (center2, sqr2);
      
      *this = Circular_arc_2(support, c1, b1, c2, b2);

      CGAL_kernel_assertion(do_intersect(support, c1));
      CGAL_kernel_assertion(do_intersect(support, c2));
    }


    Circular_arc_2(const Circle_2 &c,
		   const Circle_2 &c1, const bool b_1,
		   const Circle_2 &c2, const bool b_2)
      : _support(c) , Cache_minmax('n'), Cache_full('n')
    {
      if (c1 != c2) {
	_begin = CGAL::circle_intersect<CK>(c, c1, b_1);
	_end = CGAL::circle_intersect<CK>(c, c2, b_2);
      }
      else{
	typedef std::vector<CGAL::Object > solutions_container;
	
	solutions_container solutions;
	CGAL::intersect_2<CK>( c, c1, std::back_inserter(solutions) );
	typename solutions_container::iterator it = solutions.begin();

	CGAL_kernel_precondition( it != solutions.end() );
	// the circles intersect
	
	const std::pair<typename CK::Circular_arc_point_2, unsigned> *result;
	result = CGAL::object_cast<
	  std::pair<typename CK::Circular_arc_point_2, unsigned> > (&(*it));
	if ( result->second == 2 ){ // double solution
	  _begin =  result->first;
	  _end = result->first;
	}
	else{
	  if (b_1)
	    _begin = result->first;
	  if (b_2)
	    _end = result->first;
	  if (!(b_1 & b_2)) {
	    ++it;
	    result = CGAL::object_cast<
	      std::pair<typename CK::Circular_arc_point_2, unsigned> >(&(*it));
	    if (!b_1)
	      _begin = result->first;
	    if (!b_2)
	      _end = result->first;
	  }
	}
      }
    }

    // IS THIS CONSTRUCTOR USED ?
    // constructs a circular arc that is the arc included in A
    // having same (b) endpoint as A (true == _begin, false == _end)
    // but whose (!b) endpoint is the intersection of A with ccut given 
    // by b_cut
    Circular_arc_2(const Circular_arc_2 &A, const bool b,
		   const Circle_2 &ccut, const bool b_cut)
      : _support(A.supporting_circle()), Cache_minmax('n'), Cache_full('n')
    {
      CGAL_kernel_precondition(A.is_x_monotone());
      CGAL_kernel_precondition(do_intersect(A.supporting_circle(), ccut));
      
      Circular_arc_point_2 new_p =
	CGAL::circle_intersect<CK>(A.supporting_circle(), ccut, b_cut);
      
      // CGAL_kernel_assertion(point_in_range(A, new_p));
      CGAL_kernel_assertion
	(A.on_upper_part() == (CGAL::compare(new_p.y(), A.center().y()) >= 0));

      if (b) {
	_begin  = A._begin;
	_end    = new_p;
      }
      else {
	_begin  = new_p;
	_end    = A._end;
      }
    }

    // Constructs an arc supported by Circle_2(begin, middle, end),
    // with _begin == begin, _end == end.
    // (middle is not necessarily on the arc)
    Circular_arc_2(const Point_2 &begin,
                   const Point_2 &middle,
                   const Point_2 &end)
      : Cache_minmax('n'), Cache_full('n')
    {
      CGAL_kernel_precondition(!CGAL::collinear(begin, middle, end));
      
      Circle_2 c  (begin, middle, end);
      Line_2   l1 (begin, middle);
      Line_2   l2 (middle, end);
      *this = Circular_arc_2(c, 
			     l1, compare_xy(begin, middle) < 0,
			     l2, compare_xy(end,   middle) < 0);
    }

    Circular_arc_2(const Circle_2 &support,
		   const Circular_arc_point_2 &source,
		   const Circular_arc_point_2 &target)
      : _begin(source), _end(target), _support(support), 
	Cache_minmax('n'), Cache_full('n')
    {
      // We cannot enable these preconditions for now, since the 
      // Lazy_curved_kernel
      // calls it on the Interval kernel without try/catch protection
      // through the Curved_kernel_converter.
      // CGAL_kernel_exactness_precondition(CK().has_on_2_object()(support, source));
      // CGAL_kernel_exactness_precondition(CK().has_on_2_object()(support, target));
    }
    
    Circular_arc_2(const Point_2 &begin,
                   const Point_2 &end,
		   const FT &bulge)
      : _begin(begin), _end(end), Cache_minmax('n'), Cache_full('n')
    {
      const FT sqr_bulge = CGAL::square(bulge);
      const FT common = (FT(1) - sqr_bulge) / (FT(4)*bulge);
      const FT x_coord = (begin.x() + end.x())/FT(2)
	                 + common*(begin.y() - end.y());
      const FT y_coord = (begin.y() + end.y())/FT(2)
                          + common*(end.x() - begin.x());
      
      const FT sqr_rad = squared_distance(begin, end) 
	                 * (FT(1)/sqr_bulge + FT(2) + sqr_bulge) / FT(16); 

      _support = Circle_2(Point_2(x_coord, y_coord), sqr_rad);
    }
    
  private:
    // The arc goes from _begin to _end in the positive order
    // If _begin == _end, then it's the full circle
    Circular_arc_point_2  _begin, _end;
    Circle_2 _support;

  public:
    mutable char Cache_minmax;
    mutable char Cache_full; 
    // to remember if the arc was constructed from a full circle
    
    const Circular_arc_point_2 & left() const
    {
      CGAL_kernel_precondition(is_x_monotone());
      CGAL_kernel_precondition(on_upper_part() ? compare_xy(_end,_begin)<0
			       : compare_xy(_begin,_end)<0);
      if (Cache_minmax == 's')
	return _end;
      if (Cache_minmax == 't')
	return _begin;
      if (on_upper_part()) {
	Cache_minmax = 's';
	return _end;
      }
      Cache_minmax = 't';
      return  _begin;
    }

    const Circular_arc_point_2 & right() const
    {
      CGAL_kernel_precondition(is_x_monotone());
      CGAL_kernel_precondition(on_upper_part() ? compare_xy(_end,_begin)<0
			       : compare_xy(_begin,_end)<0);
      if (Cache_minmax == 's')
	return _begin;
      if (Cache_minmax == 't')
	return _end;
      if (on_upper_part()) {
	Cache_minmax = 's';
	return _begin;
      }
      Cache_minmax = 't';
      return  _end;
    }

    const Circular_arc_point_2 & source() const
    {
      return _begin;
    }
    
    const Circular_arc_point_2 & target() const
    {
      return _end;
    }
    
    inline const bool is_full() const {
      return Cache_full == 'y';
    }

    bool is_x_monotone() const
    {
      if (Cache_full == 'y' )
	return false;
      
      int cmp_begin = CGAL::compare(_begin.y(), center().y());
      int cmp_end   = CGAL::compare(_end.y(),   center().y());

      // XXX : be careful, this may be surprising if the return value
      // is not -1/1 but some random int...

      if (cmp_begin == opposite(cmp_end) && cmp_begin != 0)
        return false;

      int cmp_x = compare_x(_begin, _end);
      
      // Is the arc on the upper part ?
      if (cmp_begin > 0 || cmp_end > 0)
        return cmp_x > 0;

      // Is the arc on the lower part ?
      if (cmp_begin < 0 || cmp_end < 0)
        return cmp_x < 0;

      // There remains the case :
      CGAL_kernel_assertion(cmp_begin == 0 && cmp_end == 0);

      return cmp_x != 0; // full circle or half circle.
    }
    
    bool is_y_monotone() const
    {
      int cmp_begin = CGAL::compare(_begin.x(), center().x());
      int cmp_end   = CGAL::compare(_end.x(),   center().x());
      
      // XXX : be careful, this may be surprising if the return value
      // is not -1/1 but some random int...
      if (cmp_begin == opposite(cmp_end) && cmp_begin != 0)
        return false;
      
      int cmp_y = compare_y(_begin, _end);
      
      // Is the arc on the right part ?
      if (cmp_begin > 0 || cmp_end > 0)
        return cmp_y < 0;

      // Is the arc on the left part ?
      if (cmp_begin < 0 || cmp_end < 0)
        return cmp_y > 0;
      
      // There remains the case :
      assert(cmp_begin == 0 && cmp_end == 0);

      return cmp_y != 0; // full circle or half circle.
    }

    bool on_upper_part() const
    // check whether the endpoints are above or below the center
    { 
      CGAL_kernel_precondition(is_x_monotone());
      
      int begin_y = 
	CGAL::compare(_begin.y(), supporting_circle().center().y());
      int end_y   = 
	CGAL::compare(_end.y(),   supporting_circle().center().y());

      if (begin_y == 0 && end_y == 0)
        return compare_x(_begin, _end) > 0;

      return begin_y > 0 || end_y > 0;
    }

    const Circle_2 & supporting_circle() const           
    {
       return _support;
    }

    const Point_2 & center() const           
    {
       return supporting_circle().center();
    }

    const FT & squared_radius() const           
    {
       return supporting_circle().squared_radius();
    }

    Bbox_2 bbox() const
    {
      return CGAL::CircularFunctors::circular_arc_bbox<CK>(*this);
    }    
    
  };

  template < typename CK >
  std::ostream &
  operator<<(std::ostream & os, const Circular_arc_2<CK> &a)
  {
    // The output format is :
    // - supporting circle
    // - circle c1
    // - bool b1
    // - circle c2
    // - bool b2
    return os << a.supporting_circle() << " "
	      << a.source() << " "
	      << a.target() << " ";
  }

  template < typename CK >
  std::istream &
  operator>>(std::istream & is, Circular_arc_2<CK> &a)
  {
    typename CK::Circle_2 s;
    typename CK::Circular_arc_point_2 p1;
    typename CK::Circular_arc_point_2 p2;
    is >> s >> p1 >> p2 ;
    if (is)
      a = Circular_arc_2<CK>(s, p1, p2);
    return is;
  }

  template < typename CK >
  std::ostream &
  print(std::ostream & os, const Circular_arc_2<CK> &a)
  {
    return os << "Circular_arc_2( " << std::endl
              << "left : " << a.left() << " , " << std::endl
              << "right : " << a.right() << " , " << std::endl
	      << "upper part : " << a.on_upper_part() << std::endl
              << "  [[ approximate circle is (x,y,r) : "
              << CGAL::to_double(a.supporting_circle().center().x()) << "  "
              << CGAL::to_double(a.supporting_circle().center().y()) << "  "
              << std::sqrt(CGAL::to_double(a.supporting_circle()
                                            .squared_radius()))
              << " ]]" << std::endl;
  }

} // namespace CGALi
} // namespace CGAL

#endif // CGAL_CURVED_KERNEL_CIRCULAR_ARC_2_H
