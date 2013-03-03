/*
 * author:  Bruno Levy, INRIA, project ALICE
 * website: http://www.loria.fr/~levy/software
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Scientific work that use this software can reference the website and
 * the following publication:
 *
 * @INPROCEEDINGS {levy:NMDGP:05,
 *    AUTHOR = Bruno Levy,
 *    TITLE  = Numerical Methods for Digital Geometry Processing,
 *    BOOKTITLE =Israel Korea Bi-National Conference,
 *    YEAR=November 2005,
 *    URL=http://www.loria.fr/~levy/php/article.php?pub=../publications/papers/2005/Numerics
 * }
 *
 *  Laurent Saboret 01/2005-04/2005: Change for CGAL:
 *      - Added OpenNL namespace
 *      - DefaultLinearSolverTraits is now a model of the SparseLinearAlgebraTraits_d concept
 *      - Added SymmetricLinearSolverTraits
 */


#ifndef __OPENNL_LINEAR_SOLVER__
#define __OPENNL_LINEAR_SOLVER__

#include <OpenNL/conjugate_gradient.h>
#include <OpenNL/bicgstab.h>
#include <OpenNL/sparse_matrix.h>
#include <OpenNL/full_vector.h>

#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>

namespace OpenNL {



// Class DefaultLinearSolverTraits
// is a traits class for solving general sparse linear systems.
// By default, it uses OpenNL BICGSTAB general purpose solver.
//
// TODO: Add to OpenNL a BICGSTAB general purpose solver
// with Jacobi preconditioner to increase speed and support larger systems.
//
// Concept: Model of the SparseLinearAlgebraTraits_d concept.

template
<
    class COEFFTYPE,                            // type of matrix and vector coefficients
    class MATRIX = SparseMatrix<COEFFTYPE>,     // model of SparseLinearSolverTraits_d::Matrix
    class VECTOR = FullVector<COEFFTYPE>,       // model of SparseLinearSolverTraits_d::Vector
    class SOLVER = Solver_BICGSTAB<MATRIX,VECTOR>// BICGSTAB general purpose solver
>
class DefaultLinearSolverTraits
{
// Public types
public:
    // For SparseLinearAlgebraTraits_d concept
    typedef COEFFTYPE                       NT;
    typedef MATRIX                          Matrix ;
    typedef VECTOR                          Vector ;

    typedef COEFFTYPE                       CoeffType ;

// Private types
public:
    typedef SOLVER                          Solver ;

// Public operations
public:
    // Default contructor and destructor are fine

    // Solve the sparse linear system "A*X = B"
    // Return true on success. The solution is then (1/D) * X.
    // (modified for SparseLinearAlgebraTraits_d concept)
    //
    // Preconditions:
    // - A.row_dimension()    == B.dimension()
    // - A.column_dimension() == X.dimension()
    bool linear_solver (const Matrix& A, const Vector& B, Vector& X, NT& D)
    {
        Solver solver ;
        D = 1;              // Solver_BICGSTAB does not support homogeneous coordinates
        X = B;              // mandatory
        return solver.solve(A, B, X) ;
    }

    // Indicate if the linear system can be solved and if the matrix conditioning is good.
    // (added for SparseLinearAlgebraTraits_d concept)
    //
    // Preconditions:
    // A.row_dimension() == B.dimension()
    //
    // TODO: Implement DefaultLinearSolverTraits::is_solvable() by solving the system,
    // then checking that | ||A*X||/||B|| - 1 | < epsilon
    bool is_solvable (const Matrix& A, const Vector& B)
    {
        // This feature is not implemented in Solver_BICGSTAB => we do only basic checking
        if (A.row_dimension() != B.dimension())
            return false;

        return true;
    }
} ;

// Class SymmetricLinearSolverTraits
// is a traits class for solving SYMMETRIC DEFINITE POSITIVE sparse linear systems.
// By default, it uses OpenNL Conjugate Gradient solver without preconditioner.
//
// TODO: Add to OpenNL a Conjugate Gradient solver
// with Jacobi preconditioner to increase speed and support larger systems.
//
// Concept: Model of the SparseLinearAlgebraTraits_d concept.

template
<
    class COEFFTYPE,                            // type of matrix and vector coefficients
    class MATRIX = SparseMatrix<COEFFTYPE>,     // model of SparseLinearSolverTraits_d::Matrix
    class VECTOR = FullVector<COEFFTYPE>,       // model of SparseLinearSolverTraits_d::Vector
    class SOLVER = Solver_CG<MATRIX, VECTOR>    // Conjugate Gradient solver
>
class SymmetricLinearSolverTraits
    : public DefaultLinearSolverTraits<COEFFTYPE, MATRIX, VECTOR, SOLVER>
{
};


/*
 * Solves a linear system or minimizes a quadratic form.
 *
 * Requirements for its traits class: must be a model of SparseLinearSolverTraits_d concept
 */
template <class TRAITS>
class LinearSolver
{
protected:
    enum State {
        INITIAL, IN_SYSTEM, IN_ROW, CONSTRUCTED, SOLVED
    } ;

public:
    typedef TRAITS Traits ;
    typedef typename Traits::Matrix Matrix ;
    typedef typename Traits::Vector Vector ;
    typedef typename Traits::NT CoeffType ;

    class Variable {
    public:
        Variable() : x_(0), index_(-1), locked_(false) { }
        double value() const { return x_; }
        void set_value(double x_in) { x_ = x_in ; }
        void lock()   { locked_ = true ; }
        void unlock() { locked_ = false ; }
        bool is_locked() const { return locked_ ; }
        unsigned int index() const {
            assert(index_ != -1) ;
            return (unsigned int)(index_) ;
        }
        void set_index(unsigned int index_in) {
            index_ = index_in ;
        }
    private:
        CoeffType x_ ;
        int index_ ;
        bool locked_ ;
    }  ;


    LinearSolver(unsigned int nb_variables) {
        state_ = INITIAL ;
        least_squares_ = false ;
        nb_variables_ = nb_variables ;
        variable_ = new Variable[nb_variables] ;
        A_ = NULL ;
        x_ = NULL ;
        b_ = NULL ;
    }

    ~LinearSolver() {
        delete[] variable_ ;
        delete A_ ;
        delete x_ ;
        delete b_ ;
    }

    // __________________ Parameters ________________________

    void set_least_squares(bool x) { least_squares_ = x ; }

    // __________________ Access ____________________________

    int nb_variables() const { return nb_variables_ ; }

    Variable& variable(unsigned int idx) {
        assert(idx < nb_variables_) ;
        return variable_[idx] ;
    }

    const Variable& variable(unsigned int idx) const {
        assert(idx < nb_variables_) ;
        return variable_[idx] ;
    }

    // _________________ Construction _______________________

    void begin_system() {
        current_row_ = 0 ;
        transition(INITIAL, IN_SYSTEM) ;
        // Enumerate free variables.
        unsigned int index = 0 ;
        for(int ii=0; ii < nb_variables() ; ii++) {
            Variable& v = variable(ii) ;
            if(!v.is_locked()) {
                v.set_index(index) ;
                index++ ;
            }
        }
        unsigned int n = index ;
        A_ = new Matrix(n) ;
        x_ = new Vector(n) ;
        b_ = new Vector(n) ;
        for(unsigned int i=0; i<n; i++) {
            (*x_)[i] = 0 ;
            (*b_)[i] = 0 ;
        }
        variables_to_vector() ;
    }

    void begin_row() {
        transition(IN_SYSTEM, IN_ROW) ;
        af_.clear() ;
        if_.clear() ;
        al_.clear() ;
        xl_.clear() ;
        bk_ = 0 ;
    }

    void set_right_hand_side(double b) {
        check_state(IN_ROW) ;
        bk_ = b ;
    }

    void add_coefficient(unsigned int iv, double a) {
        check_state(IN_ROW) ;
        Variable& v = variable(iv) ;
        if(v.is_locked()) {
            al_.push_back(a) ;
            xl_.push_back(v.value()) ;
        } else {
            af_.push_back(a) ;
            if_.push_back(v.index()) ;
        }
    }

    void normalize_row(CoeffType weight = 1) {
        check_state(IN_ROW) ;
        CoeffType norm = 0.0 ;
        unsigned int nf = af_.size() ;
        for(unsigned int i=0; i<nf; i++) {
            norm += af_[i] * af_[i] ;
        }
        unsigned int nl = al_.size() ;
        for(unsigned int i=0; i<nl; i++) {
            norm += al_[i] * al_[i] ;
        }
        norm = sqrt(norm) ;
        assert( fabs(norm)>1e-40 );
        scale_row(weight / norm) ;
    }

    void scale_row(CoeffType s) {
        check_state(IN_ROW) ;
        unsigned int nf = af_.size() ;
         for(unsigned int i=0; i<nf; i++) {
             af_[i] *= s ;
         }
         unsigned int nl = al_.size() ;
         for(unsigned int i=0; i<nl; i++) {
             al_[i] *= s ;
         }
         bk_ *= s ;
    }

    void end_row() {
        if(least_squares_) {
            unsigned int nf = af_.size() ;
            unsigned int nl = al_.size() ;
            for(unsigned int i=0; i<nf; i++) {
                for(unsigned int j=0; j<nf; j++) {
                    A_->add_coef(if_[i], if_[j], af_[i] * af_[j]) ;
                }
            }
            CoeffType S = - bk_ ;
            for(unsigned int j=0; j<nl; j++) {
                S += al_[j] * xl_[j] ;
            }
            for(unsigned int i=0; i<nf; i++) {
                (*b_)[if_[i]] -= af_[i] * S ;
            }
        } else {
            unsigned int nf = af_.size() ;
            unsigned int nl = al_.size() ;
            for(unsigned int i=0; i<nf; i++) {
                A_->add_coef(current_row_, if_[i], af_[i]) ;
            }
            (*b_)[current_row_] = bk_ ;
            for(unsigned int i=0; i<nl; i++) {
                (*b_)[current_row_] -= al_[i] * xl_[i] ;
            }
        }
        current_row_++ ;
        transition(IN_ROW, IN_SYSTEM) ;
    }

    void end_system() {
        transition(IN_SYSTEM, CONSTRUCTED) ;
    }

    // ----------------------------- Solver -------------------------------

    // Solves a linear system or minimizes a quadratic form.
    // Return true on success.
    // (modified for SparseLinearAlgebraTraits_d concept)
    bool solve()
    {
        check_state(CONSTRUCTED) ;

        // Solve the sparse linear system "A*X = B". On success, the solution is (1/D) * X.
        Traits solver_traits;
        CoeffType D;
        bool success = solver_traits.linear_solver(*A_, *b_, *x_, D) ;
        assert(D == 1.0);   // WARNING: this library does not support homogeneous coordinates!

        vector_to_variables() ;

        transition(CONSTRUCTED, SOLVED) ;

        delete A_ ; A_ = NULL ;
        delete b_ ; b_ = NULL ;
        delete x_ ; x_ = NULL ;

        return success;
    }

protected:

    // ----------- Converting between user representation and the internal representation -----

    void vector_to_variables() {
        for(int ii=0; ii < nb_variables(); ii++) {
            Variable& v = variable(ii) ;
            if(!v.is_locked()) {
                v.set_value( (*x_)[v.index()] ) ;
            }
        }
    }

    void variables_to_vector() {
        for(int ii=0; ii < nb_variables(); ii++) {
            Variable& v = variable(ii) ;
            if(!v.is_locked()) {
                (*x_)[v.index()] = v.value() ;
            }
        }
    }

    // ----------- Finite state automaton (checks that calling sequence is respected) ---------

    std::string state_to_string(State s) {
            switch(s) {
            case INITIAL:
                return "initial" ;
            case IN_SYSTEM:
                return "in system" ;
            case IN_ROW:
                return "in row" ;
            case CONSTRUCTED:
                return "constructed" ;
            case SOLVED:
                return "solved" ;
            }
            // Should not go there.
            assert(false) ;
            return "undefined" ;
    }

    void check_state(State s) {
            if(state_ != s) {
                std::cerr << "Wrong state, expected: "
                     << state_to_string(s)
                     << " got:"
                     << state_to_string(state_)
                     << std::endl ;
                std::cerr << "exiting ..." << std::endl ;
            }
            assert(state_ == s) ;
    }

    void transition(State from, State to) {
            check_state(from) ;
            state_ = to ;
    }

private:

    // --------------- parameters --------------------------
    bool least_squares_ ;

    // --------------- user representation --------------
    unsigned int nb_variables_ ;
    Variable* variable_ ;

    // --------------- construction -----------------------
    State state_ ;
    unsigned int current_row_ ;
    std::vector<CoeffType> af_ ;
    std::vector<unsigned int> if_ ;
    std::vector<CoeffType> al_ ;
    std::vector<CoeffType> xl_ ;
    double bk_ ;

    // --------------- internal representation ---------
    Matrix* A_ ;
    Vector* x_ ;
    Vector* b_ ;

} ;


}; // namespace OpenNL

#endif
