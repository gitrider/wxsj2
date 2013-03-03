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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/QP_solver/include/CGAL/QP_solver/QP__filtered_base.h $
// $Id: QP__filtered_base.h 28526 2006-02-15 11:54:47Z fischerk $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

#ifndef CGAL_QP__FILTERED_BASE_H
#define CGAL_QP__FILTERED_BASE_H

// includes
#include <CGAL/QP_pricing_strategy.h>
#include <CGAL/QP_solver.h>
#include <cmath>

CGAL_BEGIN_NAMESPACE

// ==================
// class declarations
// ==================
template < class Rep_, class NT_ = double, class ET2NT_ =
    To_double<typename Rep_::ET> >
class QP__filtered_base;

// ===============
// class interface
// ===============
template < class Rep_, class NT_, class ET2NT_ >
class QP__filtered_base : virtual public QP_pricing_strategy<Rep_> {

    // self
    typedef  Rep_                       Rep;
    typedef  QP__filtered_base<Rep>    Self;
    typedef  QP_pricing_strategy<Rep>  Base;

  public:

    // number type
    typedef  NT_                        NT;
    typedef  typename Base::ET          ET;
    typedef  ET2NT_                     ET2NT;

  protected:

    // construction
    QP__filtered_base( ET2NT et2nt = ET2NT());
    
    // initialization
    virtual  void  set ( );
    virtual  void  init( );

    // access
    const ET2NT&  et2nt_object( ) const { return et2nt_obj; }

    // operations
    void  init_NT( );
    NT    mu_j_NT( int j) const;

    void  update_maxima( );
    bool  certify_mu_j_NT( int j) const;

    virtual  void  transition( );

    // constants (protected)
    const NT                 nt0, nt1;  // small constants of NT

  private:

    // private member functions
    void  set( int l, Tag_true  is_linear);
    void  set( int l, Tag_false is_linear);

    void  init( Tag_true  is_linear);
    void  init( Tag_false is_linear);

    void  init_NT( Tag_true  is_linear);
    void  init_NT( Tag_false is_linear);

    void  update_maxima( Tag_true  is_linear);
    void  update_maxima( Tag_false is_linear);

    void  transition( int n, Tag_true  is_linear);
    void  transition( int n, Tag_false is_linear);

    // types
    typedef  typename Rep::Is_linear    Is_linear;

    typedef  typename Rep::A_iterator   A_iterator;
    typedef  typename Rep::C_iterator   C_iterator;   
    typedef  typename std::iterator_traits
        <typename Rep::D_iterator>::value_type
                                        D_row_iterator;
    typedef  typename Rep::Row_type_iterator
                                        R_iterator;
					
    typedef typename Base::QP_solver::C_auxiliary_iterator C_auxiliary_iterator;

    typedef  typename Base::QP_solver::Basic_variable_index_iterator
                                        Basic_variable_index_iterator;
    typedef  typename Base::QP_solver::Basic_constraint_index_iterator
                                        Basic_constraint_index_iterator;

    typedef  std::vector<NT>            Values_NT;
    typedef  typename Values_NT::const_iterator  Values_NT_iterator;

    // data members
    ET2NT                    et2nt_obj; // conversion from ET to NT

    Values_NT                lambda_NT; // NT version of lambda (from KKT)
    Values_NT                x_B_O_NT;  // NT version of basic vars (orig.)
    NT                       d_NT;      // NT version of common denominator

    NT                       row_max_c; // row maximum of vector 'c'
    Values_NT                row_max_A; // row maxima of matrix 'A'
    Values_NT                row_max_D; // row maxima of matrix 'D'
    Values_NT                col_max;   // column maxima of 'c', 'A', and 'D'

    std::vector<bool>        handled_A; // flags: rows of A already handled
    std::vector<bool>        handled_D; // flags: rows of D already handled

    NT                       bound1;    // first bound for certification
    NT                       bound2_wq; // common part of second bounds
};

// ----------------------------------------------------------------------------

// =============================
// class implementation (inline)
// =============================

// construction
template < class Rep_, class NT_, class ET2NT_ >  inline
QP__filtered_base<Rep_,NT_,ET2NT_>::
QP__filtered_base( ET2NT et2nt)
    : nt0( 0), nt1( 1), et2nt_obj( et2nt)
{ }

// set-up
template < class Rep_, class NT_, class ET2NT_ > inline         // QP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
set( int l, Tag_false)
{
    x_B_O_NT.resize( l, nt0);
}

template < class Rep_, class NT_, class ET2NT_ > inline         // LP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
set( int l, Tag_true)
{
     x_B_O_NT.resize( l, nt0);   
}

// initialization
template < class Rep_, class NT_, class ET2NT_ >  inline        // QP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
init( Tag_false)
{
    if ( ! row_max_D.empty()) row_max_D.clear();
    if ( ! handled_D.empty()) handled_D.clear();
}

template < class Rep_, class NT_, class ET2NT_ >  inline        // LP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
init( Tag_true)
{
    // nop
}

// operations
template < class Rep_, class NT_, class ET2NT_ >  inline        // QP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
init_NT( Tag_false)
{
    if ( this->solver().number_of_basic_original_variables()
	 > static_cast< int>( x_B_O_NT.size())) x_B_O_NT.push_back( nt0);

    std::transform( this->solver().basic_original_variables_numerator_begin(),
		    this->solver().basic_original_variables_numerator_end(),
		    x_B_O_NT.begin(), et2nt_obj);
}

template < class Rep_, class NT_, class ET2NT_ >  inline        // LP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
init_NT( Tag_true)
{
    // nop
}


template < class Rep_, class NT_, class ET2NT_ >  inline
NT_  QP__filtered_base<Rep_,NT_,ET2NT_>::
mu_j_NT( int j) const
{
    return this->solver().mu_j( j, lambda_NT.begin(), x_B_O_NT.begin(), d_NT);
}
/*
template < class Rep_, class NT_, class ET2NT_ >  inline        // LP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
update_maxima( Tag_true)
{
    // nop
}
*/

// transition
template < class Rep_, class NT_, class ET2NT_ >  inline        // QP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
transition( int n, Tag_false)
{
    handled_D.insert( handled_D.end(), n, false);
    row_max_D.insert( row_max_D.end(), n, nt0);
}

template < class Rep_, class NT_, class ET2NT_ >  inline        // LP case
void  QP__filtered_base<Rep_,NT_,ET2NT_>::
transition( int, Tag_true)
{
    // nop
}

CGAL_END_NAMESPACE

#ifdef CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION
#  include <CGAL/QP_solver/QP__filtered_base.C>
#endif

#endif // CGAL_QP__FILTERED_BASE_H

// ===== EOF ==================================================================
