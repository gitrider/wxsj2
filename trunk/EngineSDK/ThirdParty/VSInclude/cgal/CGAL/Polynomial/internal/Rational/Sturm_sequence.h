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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/Rational/Sturm_sequence.h $
// $Id: Sturm_sequence.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_STURM_SEQUENCE_H
#define CGAL_POLYNOMIAL_INTERNAL_STURM_SEQUENCE_H

#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/internal/Sign_variations_counter.h>
#include <CGAL/Polynomial/internal/Rational/Sturm_sequence_base.h>
/*!
  \file Sturm_sequence.h A non-filtered Sturm sequence class.
*/

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

template<class Kernel_t>
class Sturm_sequence
: public Sturm_sequence_base<Kernel_t>
{
    protected:
        typedef Sturm_sequence_base<Kernel_t>    Base;

    public:
        typedef typename Base::Kernel            Kernel;

        typedef typename Base::Polynomial        Polynomial;
        typedef typename Base::NT                NT;

    protected:
        void compute_sequence(const Polynomial& p, const Polynomial& q) {
// I HAVE TO FIX THE BUG THAT EXISTS HERE; THE FOLLOWING CODE MAY
// NOT WORK CORRECTLY IF p IS THE ZERO POLYNOMIAL AND q IS NOT
// IN GENERAL I HAVE TO CONSIDER ALL THE LIMITING CASES
            if ( p.degree() >= 0 ) {
                add( p );
                this->size_++;
            }

            if ( q.degree() == -1 ) { return; }

            this->add( q );
            this->size_++;

            if ( p.degree() < q.degree() ) {
                Polynomial r = -p;
                this->add( r );
                this->size_++;
            }

            Polynomial r;

            while ( true ) {
                r = -this->k_.remainder_object()(this->seq_[this->size_ - 2],
                    this->seq_[this->size_ - 1]);
                if ( r.is_zero() ) { break; }

// THE FOLLOWING HACK HAS BEEN DONE SO THAT MP_Float HOPEFULLY
// DOES NOT RUN OUT OF EXPONENT BITS WHEN THE STURM SEQUENCE IS
// COMPUTED
                this->normalize(r, NT());

                this->add( r );
                this->size_++;
            }

        }

    public:
        Sturm_sequence() : Base() {}

        Sturm_sequence(const Polynomial& p, const Polynomial& q, const Kernel &k)
        : Base(p, q, k) {
            compute_sequence(p, q);
        }
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif                                            // CGAL_POLYNOMIAL_INTERNAL_STURM_SEQUENCE_H
