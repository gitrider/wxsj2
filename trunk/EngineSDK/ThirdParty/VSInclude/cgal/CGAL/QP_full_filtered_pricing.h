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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/QP_solver/include/CGAL/QP_full_filtered_pricing.h $
// $Id: QP_full_filtered_pricing.h 28526 2006-02-15 11:54:47Z fischerk $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

#ifndef CGAL_QP_FULL_FILTERED_PRICING_H
#define CGAL_QP_FULL_FILTERED_PRICING_H

// includes
#include <CGAL/QP_solver/QP__filtered_base.h>

CGAL_BEGIN_NAMESPACE

// =================
// class declaration
// =================
template < class Rep_, class NT_ = double, class ET2NT_ =
    To_double<typename Rep_::ET> >
class QP_full_filtered_pricing;

// ===============
// class interface
// ===============
template < class Rep_, class NT_, class ET2NT_ >
class QP_full_filtered_pricing : public QP__filtered_base<Rep_,NT_,ET2NT_> {

    // self
    typedef  Rep_                            Rep;
    typedef  QP_pricing_strategy<Rep>       Base;
    typedef  QP__filtered_base<Rep, NT_, ET2NT_>         Filtered_base;
    typedef  QP_full_filtered_pricing<Rep, NT_, ET2NT_>  Self;

    // types from the base class
    typedef  typename Base::ET          ET;

  public:

    // number type
    typedef  NT_                        NT;
    typedef  ET2NT_                     ET2NT;

    // creation
    QP_full_filtered_pricing( ET2NT et2nt = ET2NT());

    // operations
    int  pricing(int& direction );
    
    // cleanup
    ~QP_full_filtered_pricing() { };

};

// ----------------------------------------------------------------------------

// =============================
// class implementation (inline)
// =============================

// construction
template < class Rep_, class NT_, class ET2NT_ >  inline
QP_full_filtered_pricing<Rep_,NT_,ET2NT_>::
QP_full_filtered_pricing( ET2NT et2nt)
    : Base( "full filtered"),
      Filtered_base( et2nt)
{ }
    
// operations
template < class Rep_, class NT_, class ET2NT_ >
int  QP_full_filtered_pricing<Rep_,NT_,ET2NT_>::
pricing(int& direction )
{
    // get properties of quadratic program
    int  w = this->solver().number_of_working_variables();

    // initialize filtered computation
    this->init_NT();

    // loop over all non-basic variables
    int  j,  min_j  = -1;
    NT   mu, min_mu = this->nt0;
    for ( j = 0; j < w; ++j) {

	// variable non-basic?
	if ( ! this->solver().is_basic( j)) {
	
	    // don't price artificial variables
	    if (this->solver().is_artificial( j)) continue;


	    // compute mu_j
	    mu = this->mu_j_NT( j);

	    CGAL_qpe_debug {
		this->vout() << "mu_" << j << " [NT]: " << mu << std::endl;
	    }

	    // new minimum?
	    if ( mu < min_mu) { min_j = j; min_mu = mu; }
	}
    }

    // exact check of entering variable
    if ( min_j >= 0) {
	if ( this->mu_j( min_j) >= this->et0) {

	    // exact check failed!
	    CGAL_qpe_debug {
		this->vout() << "--> exact check of entering variable failed!"
		       << std::endl;
	    }
	    
	    min_j  = -1;
	    min_mu = this->nt0;
	}
    }

    // certify non-existance of entering variable, if necessary
    if ( min_j < 0) {

	// update row and column maxima
	this->update_maxima();

	// loop over all non-basic variables again
	for ( j = 0; j < w; ++j) {

	    // variable non-basic?
	    if ( ! this->solver().is_basic( j)) {
	    
	        // don't price artificial variables
	        if (this->solver().is_artificial( j)) continue;


		// certify 'mu_j >= 0'
		if ( ! this->certify_mu_j_NT( j)) {

		    // entering variable missed by inexact arithmetic
		    min_j = j;
		    break;
		}
	    }
	}
    }
    this->vout() << std::endl;

    // return index of entering variable
    return min_j;
}

CGAL_END_NAMESPACE

#endif // CGAL_QP_FULL_FILTERED_PRICING_H

// ===== EOF ==================================================================
