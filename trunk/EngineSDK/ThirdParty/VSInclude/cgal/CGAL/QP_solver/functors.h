// Copyright (c) 1997-2001  ETH Zurich (Switzerland).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/QP_solver/include/CGAL/QP_solver/functors.h $
// $Id: functors.h 29764 2006-03-25 12:23:10Z spion $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

#ifndef CGAL_QP_SOLVER_FUNCTORS_H
#define CGAL_QP_SOLVER_FUNCTORS_H

// includes
#ifndef CGAL_QP_SOLVER_BASIC_H
#  include <CGAL/QP_solver/basic.h>
#endif
#ifndef CGAL_FUNCTION_OBJECTS_H
#  include <CGAL/function_objects.h>
#endif

#ifndef CGAL_PROTECT_FUNCTIONAL
#  define CGAL_PROTECT_FUNCTIONAL
#  include <functional>
#endif
#ifndef CGAL_PROTECT_ITERATOR
#  define CGAL_PROTECT_ITERATOR
#  include <iterator>
#endif

CGAL_BEGIN_NAMESPACE

// ==================
// class declarations
// ==================
template < class VectorIt,
           bool check_lower = false, bool check_upper = false >
class QP_vector_accessor;

template < class MatrixIt,
           bool check_1st_lower = false, bool check_1st_upper = false,
           bool check_2nd_lower = false, bool check_2nd_upper = false >
class QP_matrix_accessor;

template < class MatrixIt, class IsSymmetric, typename ET >
class QP_matrix_pairwise_accessor;


template < class RndAccIt >
class Value_by_basic_index;


// =====================
// class implementations
// =====================

// -------------------
// QP_vector_accessor
// -------------------
template < class VectorIt, bool check_lower, bool check_upper >
class QP_vector_accessor : public std::unary_function<
    int, typename std::iterator_traits<VectorIt>::value_type > {

  public:
    typedef typename 
        std::unary_function<
           int, 
           typename std::iterator_traits<VectorIt>::value_type >::result_type
    result_type;
    QP_vector_accessor( VectorIt it, int lower = 0, int upper = 0)
        : z( 0), v( it), l(lower), u(upper)
    {}

    result_type  operator ( ) ( int i) const
    {
	if ( check_lower && i <  l) return z;
	if ( check_upper && i >= u) return z;
	return v[ i];
    }

  private:
    const result_type  z;
    VectorIt           v;
    int                l;
    int                u;
};

// ----------------------------------------------------------------------------

// -------------------
// QP_matrix_accessor
// -------------------
template < class MatrixIt, bool check_1st_lower, bool check_1st_upper,
                           bool check_2nd_lower, bool check_2nd_upper >
class QP_matrix_accessor {

  public:    
    typedef CGAL::Arity_tag<2> Arity;
    typedef int argument1_type;
    typedef int argument2_type;
    typedef typename std::iterator_traits<
            typename std::iterator_traits<MatrixIt>::value_type>::value_type  
    result_type;

    QP_matrix_accessor( MatrixIt it, int lower_1 = 0, int upper_1 = 0,
			              int lower_2 = 0, int upper_2 = 0)
	: z( 0), m( it)
    {
	if ( check_1st_lower) l1 = lower_1;
	if ( check_1st_upper) u1 = upper_1;
	if ( check_2nd_lower) l2 = lower_2;
	if ( check_2nd_upper) u2 = upper_2;
    }

    result_type  operator () ( int r, int c) const
    {
	if ( check_1st_lower && ( r <  l1)) return z;
	if ( check_1st_upper && ( r >= u1)) return z;
	if ( check_2nd_lower && ( c <  l2)) return z;
	if ( check_2nd_upper && ( c >= u2)) return z;
	return m[ r][ c];
    }

  private:
    const result_type  z;
    MatrixIt           m;
    int   l1, u1, l2, u2;
};		 		 

// ----------------------------------------------------------------------------

// ----------------------------
// QP_matrix_pairwise_accessor
// ----------------------------
template < class MatrixIt,class IsSymmetric, typename ResultType >
class QP_matrix_pairwise_accessor {
  typedef  typename std::iterator_traits<MatrixIt>::value_type  VectorIt;
  
public:
  typedef int        argument_type;
  typedef ResultType result_type;
  
  // The following default constructor is needed to make it possible
  // to use QP_matrix_pairwise_accessor with CGAL's Join_input_iterator_1
  // (more precisely: once Join_input_iterator_1 should not use an internal
  // mutable variable 'val' anymore, you can remove the following default
  // constructor).
  QP_matrix_pairwise_accessor() {}
  
  QP_matrix_pairwise_accessor( MatrixIt it, int row)
  {
    if ( check_tag( IsSymmetric())) {               // store i-th row
      v = it[ row];
      m = it;                                       // (See (*) below.)
    } else {                                        // matrix and index
      m = it;
      v = it[0];                                    // (See (*) below.)
      r = row;
    }
    // (*) These two statements are not needed, semantically.  If we
    // drop them, however, the if-clause will default-construct the
    // iterator 'm' and the else-clause will default-construct the
    // iterator 'v'.  In any case we end up with a singular iterator,
    // and as instances of this class are copied around, this violates
    // the requirement that singular iterators must not be copied.
    // (In other words: dropping these statements will cause assertion
    // violations when compiled with GCC using -D_GLIBCXX_DEBUG.)
  }
  
  ResultType operator () ( int c) const
  {
    return entry_pair( c, IsSymmetric());
  }
  
  ResultType entry_pair( int c, Tag_true ) const           // symmetric
  { return ResultType(v[ c]) * 2; }
  
  ResultType entry_pair( int c, Tag_false) const           // not symmetric
  { 
    // TEMPORARILY:
    typedef typename std::iterator_traits<
      typename std::iterator_traits<MatrixIt>::value_type>::value_type IT;
    return ResultType(m[ r][ c]) + ResultType(m[ c][ r]); 
  }
  
private:
  MatrixIt           m;
  VectorIt           v;
  int                r;
};


// ----------------------------------------------------------------------------

// --------------------
// Value_by_basic_index
// --------------------
template < class RndAccIt >
class Value_by_basic_index : public std::unary_function<
    int, typename std::iterator_traits<RndAccIt>::value_type > {

  public:
    typedef typename
    std::unary_function<
      int, typename std::iterator_traits
         <RndAccIt>::value_type >::result_type
    result_type;

    Value_by_basic_index( RndAccIt x_B_O_it, int n_original)
	: o( x_B_O_it), s( x_B_O_it),
	  l( n_original), u( n_original+0),
	  z( 0)
	{ }

    Value_by_basic_index( RndAccIt x_B_O_it, int n_original,
			  RndAccIt x_B_S_it, int n_slack = 0)
	: o( x_B_O_it), s( x_B_S_it),
	  l( n_original), u( n_original+n_slack),
	  z( 0)
	{ }

    result_type  operator () ( int i) const
        {
	    if ( i < 0) return z;
	    if ( i >= l && i < u) return s[ i];
	    return o[ i];
	}

  private:
    RndAccIt     o, s;
    int          l, u;
    result_type  z;
};

CGAL_END_NAMESPACE

#endif // CGAL_QP_SOLVER_FUNCTORS_H

// ===== EOF ==================================================================
