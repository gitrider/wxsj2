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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/QP_solver/include/CGAL/QP_solver/QP__partial_base.h $
// $Id: QP__partial_base.h 28526 2006-02-15 11:54:47Z fischerk $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

#ifndef CGAL_QP__PARTIAL_BASE_H
#define CGAL_QP__PARTIAL_BASE_H

// includes
#include <CGAL/QP_pricing_strategy.h>
#include <CGAL/Random.h>
#include <algorithm>
#include <vector>
#include <cmath>

CGAL_BEGIN_NAMESPACE

// ==================
// class declarations
// ==================
template < class Rep_ >
class QP__partial_base;

template < class Solver >
struct transition_sync_functor {

    // Note that we rely here on working_vars being the number of
    // variables without artificials and the solvers in_B variable
    // being up to date. Furthermore the operator() below relies on short
    // circuit evaluation   

    transition_sync_functor( const Solver& s, int w) : amb_solver(s),
        working_vars(w) { } 
         
    bool operator() (int i) {
        return (i < working_vars) && !amb_solver.is_basic(i);
    }
    
private:    
    const Solver& amb_solver;
    int working_vars;
};


// ===============
// class interface
// ===============
template < class Rep_ >
class QP__partial_base : virtual public QP_pricing_strategy<Rep_> {

    // self
    typedef  Rep_                       Rep;
    typedef  QP__partial_base<Rep>     Self;
    typedef  QP_pricing_strategy<Rep>  Base;

    typedef  typename Base::QP_solver   QP_solver;
  protected:

    // types
    typedef  typename QP_solver::Indices               Indices;
    typedef  typename QP_solver::Index_iterator        Index_iterator;
    typedef  typename QP_solver::Index_const_iterator  Index_const_iterator;

    // construction
    QP__partial_base( bool  randomize, Random&  random);

    // initialization
    virtual  void  init( );

    // access
    Index_const_iterator    active_set_begin( ) const { return N.begin();   }
    Index_const_iterator    active_set_end  ( ) const { return N.begin()+s; }

    Index_const_iterator  inactive_set_begin( ) const { return N.begin()+s; }
    Index_const_iterator  inactive_set_end  ( ) const { return N.end  ();   }

    // operations
    void  entering_basis( Index_const_iterator  it);
    void  activating    ( Index_const_iterator& it);

    virtual  void  leaving_basis( int i);
    virtual  void  transition( );

  private:
  
    // data members
    Indices                  N;         // non-basis;
    int                      s;         // size of active set

    bool                     permute;   // flag: permute initial non-basis
    Random&                  rand_src;  // random source
    
    //basic_functor<QP_solver> is_non_basic;
};

// ----------------------------------------------------------------------------

// ===============================
// class implementation (template)
// ===============================

// construction
template < class Rep_ >  inline
QP__partial_base<Rep_>::
QP__partial_base( bool  randomize, Random&  random)
    : permute( randomize), rand_src( random)
{ }

// initialization
template < class Rep_ >
void
QP__partial_base<Rep_>::
init( )
{
    // initialize indices of non-basic variables
    int  w = this->solver().number_of_working_variables();
    int  b = this->solver().number_of_basic_variables();

    if ( ! N.empty()) N.clear();
    N.reserve( w-b);                                        // use 'w' ???
    for ( int i = 0; i < w; ++i) {
	if ( ! this->solver().is_basic( i)) N.push_back( i);
    }
    if ( permute) std::random_shuffle( N.begin(), N.end(), rand_src);

    // initialize size of active set
    int  n = this->solver().number_of_variables();
    int  m = this->solver().number_of_constraints();

    s = std::min( static_cast< unsigned int>( m*std::sqrt( n/2.0)),
		  static_cast< unsigned int>(N.size()));

    //is_non_basic.init(this->solver());
}

// operations
template < class Rep_ >  inline
void
QP__partial_base<Rep_>::
entering_basis( Index_const_iterator it)
{
    CGAL_qpe_precondition( it >= active_set_begin() && it < active_set_end());

    // remove from active set
    --s;
    const_cast< typename Indices::value_type&>( *it) = N[ s];
    N[ s] = N.back();
    N.pop_back();
}

template < class Rep_ >  inline
void
QP__partial_base<Rep_>::
activating( Index_const_iterator& it)
{
    CGAL_qpe_precondition(
	it >= inactive_set_begin() && it < inactive_set_end());

    // 'append' to active set
    std::swap( const_cast< typename Indices::value_type&>( *it), N[ s]);
    it = N.begin()+s;
    ++s;
}

template < class Rep_ >
void
QP__partial_base<Rep_>::
leaving_basis( int i)
{
    // all non-basic variables active?
    if ( s == static_cast< int>( N.size())) {

	// append at the end of all non-basic variables
	N.push_back( i);

    } else {

	// insert at the end of the current active subset
	N.push_back( N[ s]);
	N[ s] = i;
    }
    ++s;
}


template < class Rep_ >
void
QP__partial_base<Rep_>::
transition( )
{
    // Remove from N nonbasic slack and original variables that have become
    // basic during the expelling of artificial variables out of the basis
    // (between phaseI and phaseII), since these formerly nonbasic variables
    // have not entered the basis through pricing the set N has not accordingly
    // been updated.
    // Remove from N the artificial variables as well.
    // Note that we rely on the number of working variables including only
    // original and slack variables, the solvers in_B variable must be 
    // up to date.
    // Furthermore we rely on std::partition not destroying the randomness
    // of the order of the nonbasic variables in N.
    
    int  w = this->solver().number_of_working_variables();    
    transition_sync_functor<QP_solver> is_non_basic(this->solver(), w);
    N.erase( std::partition( N.begin(), N.end(),
			     is_non_basic), N.end());
    
    // initialize size of active set
    int  n = this->solver().number_of_variables();
    int  m = this->solver().number_of_constraints();

    s = std::min( static_cast< unsigned int>( m*std::sqrt( n/2.0)),
		  static_cast< unsigned int>(N.size()));
}

CGAL_END_NAMESPACE

#endif // CGAL_QP__PARTIAL_BASE_H

// ===== EOF ==================================================================
