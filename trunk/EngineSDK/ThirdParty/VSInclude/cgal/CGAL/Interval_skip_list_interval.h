// Copyright (c) 2003 GeometryFactory
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Interval_skip_list/include/CGAL/Interval_skip_list_interval.h $
// $Id: Interval_skip_list_interval.h 28685 2006-02-22 15:22:27Z glisse $
// 
//
// Author(s)     : Andreas Fabri

#ifndef CGAL_INTERVAL_SKIP_LIST_INTERVAL_H
#define CGAL_INTERVAL_SKIP_LIST_INTERVAL_H

#include <cstdlib>
#include <iostream>


namespace CGAL {

  template <class Value_>
  class Interval_skip_list_interval
  {
  public:
    typedef Value_ Value;

  private:
    bool lbound_;
    bool rbound_;
    Value inf_;
    Value sup_;
  public:

    Interval_skip_list_interval(){}
    Interval_skip_list_interval(const Value& inf_, 
				const Value& sup_, 
				bool lb = true,
				bool rb = true);

    const Value& inf() const {return inf_;}

    const Value& sup() const {return sup_;}

    bool inf_closed() const {return lbound_;}

    bool sup_closed() const {return rbound_;}

    bool contains(const Value& V) const;

    // true iff this contains (l,r)
    bool contains_interval(const Value& l, const Value& r) const;  

    bool operator==(const Interval_skip_list_interval& I) const 
    {
      return ( (inf() == I.inf()) && (sup() == I.sup()) && 
	       (inf_closed() == I.inf_closed()) && (sup_closed() == I.sup_closed()) );
    }

    bool operator!=(const Interval_skip_list_interval& I) const 
    {
      return ! (*this == I);
    }
  };



  template <class V>
  std::ostream& operator<<(std::ostream& os, 
			   const Interval_skip_list_interval<V>& i)
  {
    os << (i.inf_closed()?"[":"(") << i.inf() << ", " << i.sup() << (i.sup_closed()?"]":")");
    return os;
  }


  template <class V>
  Interval_skip_list_interval<V>::Interval_skip_list_interval(
							      const Value& i, 
							      const Value& s, 
							      bool lb, bool rb)
    : lbound_(lb), rbound_(rb), inf_(i), sup_(s)
  {
    if ( inf_ > sup_ )
      {
	std::cerr << "Error: " << *this << std::endl;
	std::exit(1);
      }
  }


  template <class V>
  bool
  Interval_skip_list_interval<V>::contains_interval(const Value& i, 
						    const Value& s) const
    // true iff this contains (l,r)
  {
    return( (inf() <= i) && (sup() >= s) );
  }


  template <class V>
  bool
  Interval_skip_list_interval<V>::contains(const Value& v) const
  {
    // return true if this contains V, false otherwise
    if((v > inf()) && (v < sup()))
      return true;
    else if ((v == inf()) && inf_closed())
      return true;
    else if ((v == sup()) && sup_closed())
      return true;
    else
      return false;
  }




} // namespace CGAL

#endif // CGAL_INTERVAL_SKIP_LIST_INTERVAL_H
