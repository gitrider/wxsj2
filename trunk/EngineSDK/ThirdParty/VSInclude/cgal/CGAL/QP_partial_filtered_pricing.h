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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/QP_solver/include/CGAL/QP_partial_filtered_pricing.h $
// $Id: QP_partial_filtered_pricing.h 28573 2006-02-16 15:10:36Z fischerk $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

#ifndef CGAL_QP_PARTIAL_FILTERED_PRICING_H
#define CGAL_QP_PARTIAL_FILTERED_PRICING_H

// includes
#include <CGAL/QP_solver/QP__partial_base.h>
#include <CGAL/QP_solver/QP__filtered_base.h>

CGAL_BEGIN_NAMESPACE

// =================
// class declaration
// =================
template < class Rep_, class NT_ = double, class ET2NT_ =
    To_double<typename Rep_::ET> >
class QP_partial_filtered_pricing;

// ===============
// class interface
// ===============
template < class Rep_, class NT_, class ET2NT_ >
class QP_partial_filtered_pricing
    : public QP__partial_base <Rep_>,
      public QP__filtered_base<Rep_,NT_,ET2NT_> {

    // self
    typedef  Rep_                               Rep;
    typedef  QP_pricing_strategy<Rep>          Base;
    typedef  QP__partial_base<Rep>             Partial_base;
    typedef  QP__filtered_base<Rep, NT_, ET2NT_>            Filtered_base;
    typedef  QP_partial_filtered_pricing<Rep, NT_, ET2NT_>  Self;

    // types from the base class
    typedef  typename Base::ET                            ET;
    typedef  typename Partial_base::Index_iterator        Index_iterator;
    typedef  typename Partial_base::Index_const_iterator  Index_const_iterator;
  public:

    // number type
    typedef  NT_                        NT;
    typedef  ET2NT_                     ET2NT;

    // creation
    QP_partial_filtered_pricing( bool     randomize = false,
				  Random&  random    = default_random,
				  ET2NT    et2nt     = ET2NT());

    // operations
    int  pricing(int& direction );

    void  init( );
    void  transition( );
    
    
    // cleanup
    ~QP_partial_filtered_pricing() {};
};

// ----------------------------------------------------------------------------

// =============================
// class implementation (inline)
// =============================

// construction
template < class Rep_, class NT_, class ET2NT_ >  inline
QP_partial_filtered_pricing<Rep_,NT_,ET2NT_>::
QP_partial_filtered_pricing( bool randomize, Random& random, ET2NT et2nt)
    : Base( "partial filtered"),
      Partial_base( randomize, random),
      Filtered_base( et2nt)
{ }
    
// operations
template < class Rep_, class NT_, class ET2NT_ >  inline
void  QP_partial_filtered_pricing<Rep_,NT_,ET2NT_>::
init( )
{
     Partial_base::init();
    Filtered_base::init();
}

template < class Rep_, class NT_, class ET2NT_ >  inline
void  QP_partial_filtered_pricing<Rep_,NT_,ET2NT_>::
transition( )
{
     Partial_base::transition();
    Filtered_base::transition();
}

template < class Rep_, class NT_, class ET2NT_ >
int  QP_partial_filtered_pricing<Rep_,NT_,ET2NT_>::
pricing(int& direction )
{
    // initialize filtered computation
    this->init_NT();

    // loop over all active non-basic variables
    CGAL_qpe_debug {
	this->vout() << "active variables:" << std::endl;
    }

    Index_const_iterator  it, min_it;
    NT                            mu, min_mu = this->nt0;
    for ( it = this->active_set_begin(); it != this->active_set_end(); ++it) {

	// don't price artificial variables
	if (this->solver().is_artificial( *it) ||
	    this->solver().is_basic( *it))  // added by kf
	  continue;

	// compute mu_j
	mu = mu_j_NT( *it);

	CGAL_qpe_debug {
	    this->vout() << "  mu_" << *it << " [NT]: " << mu << std::endl;
	}

	// new minimum?
	if ( mu < min_mu) { min_it = it; min_mu = mu; }
    }

    // exact check of entering variable
    if ( min_mu < this->nt0) {
	if ( mu_j( *min_it) >= this->et0) {

	    // exact check failed!
	    CGAL_qpe_debug {
		this->vout() << "--> exact check of entering variable failed!"
		       << std::endl;
	    }

	    // reject entering variable
	    min_mu = this->nt0;
	}
    } else {
	CGAL_qpe_debug {
	    this->vout() << "--> no entering variable found yet" << std::endl;
	}
    }

    // no entering variable found so far?
    if ( ( min_mu == this->nt0) &&
         ( this->inactive_set_begin() < this->inactive_set_end())) {

	// loop over all inactive non-basic variables
	CGAL_qpe_debug {
	    this->vout() << "inactive variables:" << std::endl;
	}
	Index_const_iterator  active_it;
	for ( it = this->inactive_set_begin(); it != this->inactive_set_end(); ++it) {

	    // don't price artificial variables
	    if (this->solver().is_artificial( *it)) continue;

	    // compute mu_j
	    mu = mu_j_NT( *it);

	    CGAL_qpe_debug {
		this->vout() << "  mu_" << *it << " [NT]: " << mu << std::endl;
	    }

	    // candidate for entering?
	    if ( mu < this->nt0) {

		// make variable active
		active_it = it;
		activating( active_it);

		// new minimum?
		if ( mu < min_mu) { min_it = active_it; min_mu = mu; }
	    }
	}

	// exact check of entering variable
	if ( min_mu < this->nt0) {
	    if ( mu_j( *min_it) >= this->et0) {

		// exact check failed!
		CGAL_qpe_debug {
		    this->vout() << "--> exact check of entering variable failed!"
			   << std::endl;
		}

		// reject entering variable
		min_mu = this->nt0;
	    }
	} else {
	    CGAL_qpe_debug {
		this->vout() << "--> still no entering variable found" << std::endl;
	    }
	}
    }

    // certify non-existance of entering variable, if necessary
    if ( min_mu == this->nt0) {

	// update row and column maxima
	this->update_maxima();

	// loop over all non-basic variables again
	for ( it = this->active_set_begin(); it != this->inactive_set_end(); ++it) {

	    // don't price artificial variables
	    if (this->solver().is_artificial( *it)) continue;

	    // certify 'mu_j >= 0'
	    if ( ! certify_mu_j_NT( *it)) {

		// entering variable missed by inexact arithmetic
		min_it = it;
		break;
	    }
	}
    }
    this->vout() << std::endl;

    // return index of entering variable, if any
    if ( min_mu < this->nt0) {
	int  j = *min_it;
	entering_basis( min_it);
	return j;
    }

    // no entering variable found
    return -1;
}

CGAL_END_NAMESPACE

#endif // CGAL_QP_PARTIAL_FILTERED_PRICING_H

// ===== EOF ==================================================================
