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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/QP_solver/include/CGAL/QP_solver/Initialization.h $
// $Id: Initialization.h 29042 2006-03-03 18:32:37Z gaertner $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

CGAL_BEGIN_NAMESPACE

// creation & initialization
// -------------------------

// creation standard form
template < class Rep_ >
QP_solver<Rep_>::
QP_solver(int n, int m,
	  A_iterator A, B_iterator b, C_iterator c, D_iterator D,
	  Row_type_iterator r,
	  Pricing_strategy *strategy, int verbosity)
  : et0(0), et1(1), et2(2),
    defaultStrategy(0),
    inv_M_B(vout4),
    d(inv_M_B.denominator()),
    m_phase(-1), is_phaseI(false), is_phaseII(false),
    is_RTS_transition(false),
    is_LP(check_tag(Is_linear())), is_QP(!is_LP), // todo kf.: what is
                                                  // this good for?
                                                  // why not use
                                                  // check_tag()
                                                  // everywhere?
    no_ineq(check_tag(Has_equalities_only_and_full_rank())),
    has_ineq(!no_ineq),
    is_in_standard_form(check_tag(Is_in_standard_form())) // same
							  // question
							  // here...
{
  set_verbosity(verbosity); 
  set_pricing_strategy(strategy);
  set(n,m,A,b,c,D,r);
  init();
  solve();
}

// creation upper bounded case
template < class Rep_ >
QP_solver<Rep_>::
QP_solver(int n, int m,
	  A_iterator A, B_iterator b, C_iterator c, D_iterator D,
	  Row_type_iterator r,
	  FL_iterator fl, L_iterator lb, FU_iterator fu, U_iterator ub,
	  Pricing_strategy *strategy, int verbosity)
  : et0(0), et1(1), et2(2),
    defaultStrategy(0),
    inv_M_B(vout4),
    d(inv_M_B.denominator()),
    m_phase(-1), is_phaseI(false), is_phaseII(false),
    is_RTS_transition(false),
    is_LP(check_tag(Is_linear())), is_QP(!is_LP),
    no_ineq(check_tag(Has_equalities_only_and_full_rank())),
    has_ineq(!no_ineq),
    is_in_standard_form(check_tag(Is_in_standard_form()))
{
  // initialization as in the standard-form case:
  set_verbosity(verbosity); 
  set_pricing_strategy(strategy);

  // Note: we first set the bounds and then call set() because set()
  // accesses qp_fl, qp_l, etc.
  set_explicit_bounds(fl, lb, fu, ub);
  set(n,m,A,b,c,D,r);

  // initialize and solve immediately:
  init();
  solve();
}

// set-up of QP
template < class Rep_ >
void QP_solver<Rep_>::
set(int n, int m,
    typename QP_solver<Rep_>::A_iterator A_it,
    typename QP_solver<Rep_>::B_iterator b_it,
    typename QP_solver<Rep_>::C_iterator c_it,
    typename QP_solver<Rep_>::D_iterator D_it,
    typename QP_solver<Rep_>::Row_type_iterator Row_type_it)
{
  // assertions:
  CGAL_qpe_precondition(n > 0);
  CGAL_qpe_precondition(m >= 0); 

  // store QP
  qp_n = n; qp_m = m;
  qp_A = A_it; qp_b = b_it; qp_c = c_it; qp_D = D_it;
  qp_r = Row_type_it;
  
  // store original variable indices (hack needed to allow
  // access to original variable values through Join_iterator_1
  O.reserve(qp_n);
  for (int i=0; i<qp_n; ++i) O.push_back(i);

  // set up slack variables and auxiliary problem
  // --------------------------------------------

  // clear slack and artificial part of `A', if necessary:
  // Note: currently, this is not needed as set() is only called once.
  #if 0
  if (!slack_A.empty()) slack_A.clear();
  if (!  art_A.empty())   art_A.clear();
  if (!  art_s.empty())   art_s.clear();
  #endif
  
  // reserve memory for slack and artificial part of `A':
  if (has_ineq) {
    const unsigned int art_size = std::count(qp_r, qp_r+qp_m, Rep::EQUAL);
                            // Note: art_size is the number of equalities.
    slack_A.reserve(qp_m - art_size);
    art_A.reserve  (       art_size);
    art_s.insert(art_s.end(), qp_m, A_entry(0));
  } else
    art_A.reserve( qp_m);

  // decide on which bound the variables sit initially:
  if (!check_tag(Is_in_standard_form()))
    init_x_O_v_i();

  set_up_auxiliary_problem();
    
  e = qp_m-slack_A.size(); // number of equalities
  l = std::min(n+e+1, m);  // maximal size of basis in phase I
  
  // diagnostic output:
  CGAL_qpe_debug {
    if (vout.verbose()) {
      if (vout2.verbose()) {
	vout2.out() << "======" << std::endl
		    << "Set-Up" << std::endl
		    << "======" << std::endl;
      }
      vout.out() << "[ " << (is_LP ? "LP" : "QP")
		 << ", " << n << " variables, " << m << " constraints";
      if (vout2.verbose() && (!slack_A.empty())) {
	vout2.out() << " (" << slack_A.size() << " inequalities)";
      }
      vout.out() << " ]" << std::endl;
      if (vout2.verbose()) {
	if (is_QP) {
	  vout2.out() << "flag: D "
		      << (check_tag(Is_symmetric()) ? "" : "not ")
		      << "symmetric" << std::endl;
	}
	if (has_ineq)
	  vout2.out() << "flag: has inequalities or rank not full"
		      << std::endl;
	if (vout4.verbose()) print_program();
      }
    }
  }
  
  // set up pricing strategy:
  if (strategyP != static_cast< Pricing_strategy*>(0))
    strategyP->set(*this, vout2);
  
  // set up basis inverse:
  inv_M_B.set(qp_n, qp_m, e);
  
  // set phase:
  m_phase    = 0;
  is_phaseI  = false;
  is_phaseII = false;
}

template < class Rep_ >
void QP_solver<Rep_>::
set_explicit_bounds(FL_iterator fl,L_iterator lb,FU_iterator fu,U_iterator ub)
{
  qp_fl = fl;
  qp_l = lb;
  qp_fu = fu;
  qp_u = ub;
}

#if 0
// todo: following is an old version that should be replaced by the out#def'ed
// one below.
template < class Rep_ >
void QP_solver<Rep_>::init_x_O_v_i()
{
  // allocate storage:
  x_O_v_i.reserve(qp_n);
  x_O_v_i.resize (qp_n);

  // constants for comparisions:
  const L_entry l0(0);
  const U_entry u0(0);
  
  // our initial solution will have all original variables nonbasic:
  for (int i = 0; i < qp_n; ++i) {
    CGAL_qpe_assertion(qp_l[i]<=qp_u[i] || !*(qp_fl+i) || !*(qp_fu+i));

    if (*(qp_fl+i))                    // finite lower bound?
      if (*(qp_fu+i))                  // finite lower and finite upper bound?
        if (qp_l[i] == qp_u[i])        // fixed variable?
          x_O_v_i[i] = FIXED;
        else                           // finite lower < finite upper?
          if (qp_l[i] <= l0 && u0 <= qp_u[i])
            // todo kf: why don't we prefer zero so that multiplication is
            // cheap? or isn't this the whole point about using ZERO? (same
            // question at other places in this routine...
            if (l0 == qp_l[i])
              x_O_v_i[i] = LOWER;
            else if (u0 == qp_u[i])
              x_O_v_i[i] = UPPER;
            else
              x_O_v_i[i] = ZERO;
          else
            x_O_v_i[i] = LOWER;
      else                             // finite lower and infinite upper bound?
        if (l0 >= qp_l[i])
          if (l0 == qp_l[i])
            x_O_v_i[i] = LOWER;
          else
            x_O_v_i[i] = ZERO;
        else 
          x_O_v_i[i] = LOWER;
    else                               // infinite lower bound?
      if (*(qp_fu+i))                  // infinite lower and finite upper?
        if (u0 <= qp_u[i])
          if (u0 == qp_u[i])
            x_O_v_i[i] = UPPER;
          else
            x_O_v_i[i] = ZERO;
        else
          x_O_v_i[i] = UPPER;
      else                             // infinite lower and infinite upper?
        x_O_v_i[i] = ZERO;
  }
}
#endif

template < class Rep_ >
void QP_solver<Rep_>::
init_x_O_v_i()
{
  // allocate storage:
  x_O_v_i.reserve(qp_n);
  x_O_v_i.resize (qp_n);

  // constants for comparisions:
  const L_entry l0(0);
  const U_entry u0(0);
  
  // our initial solution will have all original variables nonbasic,
  // and so we initialize them to zero (if the bound on the variable
  // allows it), or to the variable's lower or upper bound:
  for (int i = 0; i < qp_n; ++i) {
    CGAL_qpe_assertion(qp_l[i]<=qp_u[i] || !*(qp_fl+i) || !*(qp_fu+i));

    if (*(qp_fl+i))                    // finite lower bound?
      if (*(qp_fu+i))                  // finite lower and finite upper bound?
	if (qp_l[i] == qp_u[i])        // fixed variable?
	  x_O_v_i[i] = FIXED;
	else                           // finite lower and finite upper?
	  if (qp_l[i] <= l0 && u0 <= qp_u[i])
	    x_O_v_i[i] = ZERO;
	  else
	    x_O_v_i[i] = LOWER;
      else                             // finite lower and infinite upper?
	if (qp_l[i] <= l0)
	  x_O_v_i[i] = ZERO;
	else 
	  x_O_v_i[i] = LOWER;
    else                               // infinite lower bound?
      if (*(qp_fu+i))                  // infinite lower and finite upper?
	if (u0 <= qp_u[i])
	  x_O_v_i[i] = ZERO;
	else
	  x_O_v_i[i] = UPPER;
      else                             // infinite lower and infinite upper?
	x_O_v_i[i] = ZERO;
  }
}

#if 0  // (fw) The following is a variant of set_up_auxiliary_problem()
       // for symbolic perturbation for the perturbed case.
template < class Rep_ >
void
QP_solver<Rep_>::
set_up_auxiliary_problemI(Tag_true)
{
  // initialize slack and artificial part of `A'
  A_entry  max_lc_A(0);
  B_entry  b0(0), b_max(b0), max_lc_b(0);
  C_entry  c1(1);
  int              i_max = -1;
  int              row_le;
  int              inf_max_le = qp_n + 2;
  
  for (int i = 0; i < qp_m; ++i) {
    
    if (has_ineq && (qp_r[i] != Rep::EQUAL)) {   // slack variable
      row_le = signed_leading_exponent(i);
      if (qp_r[i] == Rep::LESS_EQUAL) {                 // '<='
	if (row_le < 0) {
	  
	  // special entry '< -0'
	  art_s[i] = -c1;
	  if (-row_le < inf_max_le) {
	    i_max = slack_A.size();
	    inf_max_le = -row_le;
	    if (inf_max_le > 1) {
	      max_lc_A = -qp_A[inf_max_le - 1][i];
	    } else {
	      max_lc_b = -qp_b[i];
	    }
	  } else {
	    if (-row_le == inf_max_le) {
	      if (inf_max_le > 1) {
		if (-qp_A[inf_max_le - 1][i] > max_lc_A) {
		  i_max = slack_A.size();
		  max_lc_A = -qp_A[inf_max_le - 1][i];
		}
	      } else {
		if (-qp_b[i] > max_lc_b) {
		  i_max = slack_A.size();
		  max_lc_b = -qp_b[i];
		}
	      }
	    }
	  }
	} 

	// store slack column
	slack_A.push_back(std::make_pair(i, false));

      } else {                                            // '>='
	if (row_le > 0) {

	  // special entry '> +0'
	  art_s[i] = c1;
	  if ( row_le < inf_max_le) {
	    i_max = slack_A.size();
	    inf_max_le = row_le;
	    if (inf_max_le > 1) {
	      max_lc_A = qp_A[inf_max_le - 1][i];
	    } else {
	      max_lc_b = qp_b[i];
	    }
	  } else {
	    if (row_le == inf_max_le) {
	      if (inf_max_le > 1) {
		if (qp_A[inf_max_le - 1][i] > max_lc_A) {
		  i_max = slack_A.size();
		  max_lc_A = qp_A[inf_max_le - 1][i];
		}
	      } else {
		if (qp_b[i] > max_lc_b) {
		  i_max = slack_A.size();
		  max_lc_b = qp_b[i];
		}
	      }
	    }
	  }
	}

	// store slack column
	slack_A.push_back(std::make_pair(i, true));
      }

    } else {                                        // artificial variable

      art_A.push_back(std::make_pair(i, qp_b[i] < b0));

    }
  }
  // special artificial column needed?
  if (i_max >= 0) {
    art_s_i = -i_max;
  } else {
    art_s_i = -1;
    art_s.clear();
  }
}

// This is the currently used variant of set_up_auxiliary_problem for
// symbolic perturbation for the perturbed case.
template < class Rep_ >
void
QP_solver<Rep_>::
set_up_auxiliary_problem(Tag_true)
{
  // initialize slack and artificial part of `A'
  A_entry  max_lc_A(0);
  B_entry  b0(0), b_max(b0), max_lc_b(0);
  C_entry  c1(1);
  int              i_max = -1;
  int              row_le;
  int              inf_max_le = qp_n + 1;

  for (int i = 0; i < qp_m; ++i) {

    if (has_ineq && (qp_r[i] != Rep::EQUAL)) {   // slack variable

      if (qp_r[i] == Rep::LESS_EQUAL) {                 // '<='
	if (qp_b[i] < b0) {

	  // special entry '< -0'
	  art_s[i] = -c1;
	  if (-qp_b[i] > b_max) {
	    i_max = slack_A.size();
	    inf_max_le = 0;
	    max_lc_b = -qp_b[i];
	  }
	} else {
	  if (qp_b[i] == b0) {
	    row_le = signed_leading_exponent(i);
	    if (row_le < 0) {
	      art_s[i] = -c1;
	      if (-row_le < inf_max_le) {
		i_max = slack_A.size();
		inf_max_le = -row_le;
		max_lc_A = qp_A[inf_max_le - 1][i];
	      } else {
		if (-row_le == inf_max_le) {
		  if (-qp_A[inf_max_le - 1][i] > max_lc_A) {
		    i_max = slack_A.size();
		    max_lc_A = -qp_A[inf_max_le - 1][i];
		  }
		}
	      } 
	    }
	  }
	}

	// store slack column
	slack_A.push_back(std::make_pair(i, false));

      } else {                                            // '>='
	if (qp_b[i] > b0) {

	  // special entry '> +0'
	  art_s[i] = c1;
	  if ( qp_b[i] > b_max) {
	    i_max = slack_A.size();
	    inf_max_le = 0;
	    max_lc_b = qp_b[i];
	  }
	} else {
	  if (qp_b[i] == b0) {
	    row_le = signed_leading_exponent(i);
	    if (row_le > 0) {
	      art_s[i] = c1;
	      if (row_le < inf_max_le) {
		i_max = slack_A.size();
		inf_max_le = row_le;
		max_lc_A = qp_A[inf_max_le - 1][i];
	      } else {
		if (row_le == inf_max_le) {
		  if (qp_A[inf_max_le - 1][i] > max_lc_A) {
		    i_max = slack_A.size();
		    max_lc_A = qp_A[inf_max_le - 1][i];
		  }
		}
	      }
	    }
	  }
	}

	// store slack column
	slack_A.push_back(std::make_pair(i, true));
      }

    } else {                                        // artificial variable
      row_le = signed_leading_exponent(i);
      art_A.push_back(std::make_pair(i, ((qp_b[i] < b0) ||
					 (qp_b[i] == b0) && (row_le < 0))));

    }
  }
  // special artificial column needed?
  if (i_max >= 0) {
    art_s_i = -i_max;
  } else {
    art_s_i = -1;
    art_s.clear();
  }
}

// function needed for set up of auxiliary problem for symbolic perturbation
//
// returns signed * (most significant exponent + 1) if <> 0
// 0 otherwise
template < class Rep_ >
int  QP_solver<Rep_>::
signed_leading_exponent(int row)
{
  A_entry  a0(0);
    
  int col = 0;
  while ((col < qp_n) && (qp_A[col][row] == a0)) {
    ++col;
  }
  return ((qp_A[col][row] > a0) ? col + 1 : -(col + 1));
}
#endif // end of alternative implementation

template < class Rep_ >
void QP_solver<Rep_>::
set_up_auxiliary_problem()
{
  ET            b_max(et0);
  const C_entry c1(1);
  int           i_max = -1;

  for (int i = 0; i < qp_m; ++i) {
    // Note: For nonstandard form problems, our initial solution is not the
    // zero vector (but the vector with values original_variable_value(i),
    // 0<=i<qp_n), and therefore, rhs=b-Ax is not simply b as in the standard
    // form case, but Ax_init-b:
    const ET rhs = check_tag(Is_in_standard_form())?
      qp_b[i] : ET(qp_b[i]) - multiply__A_ixO(i);

    if (has_ineq && (qp_r[i] != Rep::EQUAL)) { // inequality constraint, so we
					       // add a slack variable, and (if
					       // needed) a special artificial
      if (qp_r[i] == Rep::LESS_EQUAL) {        // '<='

	// add special artificial ('< -0') in case the inequality is
	// infeasible for our starting point (which is the origin):
	if (rhs < et0) { 
	  art_s[i] = -c1;
	  if (-rhs > b_max) {
	    i_max = slack_A.size();
	    b_max = -rhs;
	  }
	}

	// slack variable:
	slack_A.push_back(std::make_pair(i, false));
      } else {                                 // '>='

	// add special artificial ('> +0') in case the inequality is
	// infeasible for our starting point (which is the origin):
	if (rhs > et0) {
	  art_s[i] = c1;
	  if (rhs > b_max) {
	    i_max = slack_A.size();
	    b_max = rhs;
	  }
	}
	
	// store slack column
	slack_A.push_back(std::make_pair(i, true));
      }
      
    } else                                     // equality constraint, so we
					       // add an artificial variable
      // (Note: if rhs==et0 here then the artificial variable is (at the
      // moment!) not needed. However, we nonetheless add it, for the following
      // reason. If we did and were given an equality problem with the zero
      // vector as the right-hand side then NO artificials would be added at
      // all; so our initial basis would be empty, something we do not want.)
      art_A.push_back(std::make_pair(i, rhs < et0));
  }

  // Note: in order to make our initial starting point (which is the origin) a
  // feasible point of the auxiliary problem, we need to initialize the
  // special artificial value correctly, namely to
  //
  //   max { |b_i| | i is index of an infeasible inequality constraint }. (C1)
  //
  // The index of this "most infeasible" constraint is, at this point of the
  // code, i_max (or i_max is -1 in which case all inequality constraints are
  // feasible and hence no special artififial column is needed at all).

  // prepare initialization of special artificial column:
  // Note: the real work is done in init_basis() below.
  if (i_max >= 0) {
    art_s_i = i_max;                           // Note: the actual
					       // initialization of art_s_i
					       // will be done in init_basis()
					       // below. We misuse art_s_i to
					       // remember i_max.
  } else {                                     // no special art col needed
    art_s_i = -1;
    art_s.clear();
  }
}

// initialization (phase I)
template < class Rep_ >
void QP_solver<Rep_>::
init()
{
  CGAL_qpe_debug {
    vout2 << std::endl
	  << "==============" << std::endl
	  << "Initialization" << std::endl
	  << "==============" << std::endl;
              
  }

  // set status:
  m_phase    = 1;
  m_status   = UPDATE;
  m_pivots   = 0;
  is_phaseI  = true;
  is_phaseII = false;

  // initial basis and basis inverse
  init_basis();
    
  // initialize additional data members
  init_additional_data_members();
        
  // initial solution
  init_solution();

  // initialize pricing strategy
  CGAL_qpe_precondition(strategyP != static_cast< Pricing_strategy*>(0));
  strategyP->init(0);

  // basic feasible solution already available?
  if (art_basic == 0) {

    // transition to phase II
    CGAL_qpe_debug {
      if (vout2.verbose()) {
	vout2.out() << std::endl
		    << "no artificial variables at all "
		    << "--> skip phase I"
		    << std::endl;
      }
    }
    transition();
  }
}

// Set up the initial basis and basis inverse.
template < class Rep_ >
void QP_solver<Rep_>::
init_basis()
{
  int s_i = -1;
  const int s = slack_A.size();
  
  // has special artificial column?
  if (!art_s.empty()) {

    // Note: we maintain the information about the special artificial column in
    // the variable art_s_i and the vector s_art; in addition, however, we also
    // add a special "fake" column to art_A. This "fake" column has (in
    // constrast to the special artificial column) only one nonzero entry,
    // namely a +-1 for the most infeasible row (see (C1) above).

    // add "fake" column to art_A:
    s_i = art_s_i;                    // index of most infeasible row, see (C1)
    art_s_i = qp_n+s+art_A.size();    // number of special artificial var
    art_A.push_back(std::make_pair(s_i, !slack_A[s_i].second)); // todo kf: I
								// do not
								// understand
								// this
  }
  
  // initialize indices of basic variables:
  if (!in_B.empty()) in_B.clear();
  in_B.reserve(qp_n+s+art_A.size());
  in_B.insert(in_B.end(), qp_n, -1);  // no original variable is basic
  
  init_basis__slack_variables(s_i, Has_equalities_only_and_full_rank());
  
  if (!B_O.empty()) B_O.clear();
  B_O.reserve(qp_n);                  // all artificial variables are basic
  for (int i = 0; i < (int)art_A.size(); ++i) {
    B_O .push_back(qp_n+s+i);
    in_B.push_back(i);
  }
  art_basic = art_A.size();
  
  // initialize indices of 'basic' and 'nonbasic' constraints:
  if (!C.empty()) C.clear();
  init_basis__constraints(s_i, Has_equalities_only_and_full_rank());
  
  // diagnostic output:
  CGAL_qpe_debug {
    if (vout.verbose()) print_basis();
  }
  
  // initialize basis inverse (explain: 'art_s' not needed here (todo kf: don't
  // understand this note)):
  inv_M_B.init(art_A.size(), art_A.begin());
}

template < class Rep_ >  inline                                 // no ineq.
void QP_solver<Rep_>::
init_basis__slack_variables( int, Tag_true)
{
    // nop
}

template < class Rep_ >                                        // has ineq.
void QP_solver<Rep_>::
init_basis__slack_variables(int s_i, Tag_false)  // Note: s_i is the index of
						 // the most infeasible row,
						 // see (C1).
{
  const int s = slack_A.size();
  
  // reserve memory:
  if (!B_S.empty()) B_S.clear();
  B_S.reserve(s);
  
  // all slack variables are basic, except the slack variable corresponding to
  // special artificial variable (which is nonbasic): (todo kf: I do not
  // understand this)
  for (int i = 0; i < s; ++i)
    if (i != s_i) {
      in_B.push_back(B_S.size());
      B_S .push_back(i+qp_n);     
    } else
      in_B.push_back(-1);
}

template < class Rep_ >  inline                                 // no ineq.
void QP_solver<Rep_>::
init_basis__constraints( int, Tag_true)
{
  // reserve memory:
  C.reserve(qp_m);

  // As there are no inequalities, C consists of all inequality constraints
  // only, so we add them all:
  for (int i = 0; i < qp_m; ++i)
    C.push_back(i);
}

template < class Rep_ >                                        // has ineq.
void QP_solver<Rep_>::
init_basis__constraints(int s_i, Tag_false)  // Note: s_i is the index of the
					     // most infeasible row, see (C1).
{
  int i, j;

  // reserve memory:
  if (!in_C.empty()) in_C.clear();
  if (! S_B.empty())  S_B.clear();
  C.reserve(l);
  S_B.reserve(slack_A.size());
  
  // store constraints' indices:
  in_C.insert(in_C.end(), qp_m, -1);
  if (s_i >= 0) s_i = slack_A[s_i].first;
  for (i = 0, j = 0; i < qp_m; ++i)
    if (qp_r[i] == Rep::EQUAL) {             // equal. constraints are basic
      C.push_back(i);
      in_C[i] = j;
      ++j;
    } else                                   // ineq. constrai. are nonbasic
      if (i != s_i)
	S_B.push_back(i);
  if (s_i >= 0) {                            // special ineq. con. is basic
                                             // todo kf: do not really underst.
    C.push_back(s_i);
    in_C[s_i] = j;
  }
}

// Initialize r_C.
template < class Rep_ >                 // Standard form
void  QP_solver<Rep_>::
init_r_C(Tag_true)
{
}

// Initialize r_C.
template < class Rep_ >                 // Upper bounded
void  QP_solver<Rep_>::
init_r_C(Tag_false)
{
  r_C.resize(C.size());
  multiply__A_CxN_O(r_C.begin());  
}

// Initialize r_S_B.
template < class Rep_ >                 // Standard form
void  QP_solver<Rep_>::
init_r_S_B(Tag_true)
{
}

// Initialize r_S_B.
template < class Rep_ >                 // Upper bounded
void  QP_solver<Rep_>::
init_r_S_B(Tag_false)
{
  r_S_B.resize(S_B.size());
  multiply__A_S_BxN_O(r_S_B.begin()); 
}

template < class Rep_ >  inline                                 // no ineq.
void  QP_solver<Rep_>::
init_solution__b_C(Tag_true)
{
  b_C.reserve(qp_m);
  std::copy(qp_b, qp_b+qp_m, std::back_inserter(b_C));
}

template < class Rep_ >  inline                                 // has ineq.
void  QP_solver<Rep_>::
init_solution__b_C(Tag_false)
{ 
  b_C.insert(b_C.end(), l, et0);
  B_by_index_accessor  b_accessor(qp_b); // todo kf: is there some boost
					 // replacement for this accessor?
  std::copy(B_by_index_iterator(C.begin(), b_accessor),
	    B_by_index_iterator(C.end  (), b_accessor),
	    b_C.begin());
}

// initial solution
template < class Rep_ >
void
QP_solver<Rep_>::
init_solution()
{
  // initialize exact version of `qp_b' restricted to basic constraints C
  // (implicit conversion to ET):
  if (!b_C.empty()) b_C.clear();
  init_solution__b_C(Has_equalities_only_and_full_rank());

  // initialize exact version of `aux_c' and 'minus_c_B', the
  // latter restricted to basic variables B_O:
  if (!minus_c_B.empty()) minus_c_B.clear();
  minus_c_B.insert(minus_c_B.end(), l, -et1);   // todo: what is minus_c_B?
  if (art_s_i > 0)
    minus_c_B[art_A.size()-1] *= ET(qp_n+qp_m); // Note: the idea here is to
						// give more weight to the
						// special artifical variable
						// so that it gets removed very
						// early, - todo kf: why?

  // ...and now aux_c: as we want to make all artificial variables (including
  // the special one) zero, we weigh these variables with >= 1 in the objective
  // function (and leave the other entries in the objective function at zero):
  aux_c.reserve(art_A.size());
  aux_c.insert(aux_c.end(), art_A.size(), 0);
  for (int col=qp_n+slack_A.size(); col<number_of_working_variables(); ++col)
    if (col==art_s_i)                           // special artificial?
      aux_c[col-qp_n-slack_A.size()]=  qp_n+qp_m;
    else                                        // normal artificial
      aux_c[col-qp_n-slack_A.size()]= 1;

  // allocate memory for current solution:
  if (!lambda.empty()) lambda.clear();
  if (!x_B_O .empty()) x_B_O .clear();
  if (!x_B_S .empty()) x_B_S .clear();
  lambda.insert(lambda.end(), l, et0);
  x_B_O .insert(x_B_O .end(), l, et0);
  x_B_S .insert(x_B_S .end(), slack_A.size(), et0);

  #if 0 // todo kf: I guess the following can be removed...
  //TESTING the updates of r_C, r_S_B, r_B_O, w
  //    ratio_test_bound_index = LOWER;
  //direction = 1;
  #endif

  // The following sets the pricing direction to "up" (meaning that
  // the priced variable will be increased and not decreased); the
  // statement is completely useless except that it causes debugging
  // output to be consistent in case we are running in standard form.
  // (If we are in standard form, the variable 'direction' is never
  // touched; otherwise, it will be set to the correct value during
  // each pricing step.)
  direction = 1;
    
  // initialization of vectors r_C, r_S_B:
  init_r_C(Is_in_standard_form());
  init_r_S_B(Is_in_standard_form());

  // compute initial solution:
  compute_solution(Is_in_standard_form());

  // diagnostic output:
  CGAL_qpe_debug {
    if (vout.verbose()) print_solution();
  }
}

// Initialize additional data members.
template < class Rep_ >
void
QP_solver<Rep_>::
init_additional_data_members()
{
  // todo kf: do we really have to insert et0, or would it suffice to just
  // resize() in the following statements?

  if (!A_Cj.empty()) A_Cj.clear();
  A_Cj.insert(A_Cj.end(), l, et0);
  if (!two_D_Bj.empty()) two_D_Bj.clear();
  two_D_Bj.insert(two_D_Bj.end(), l, et0);
  
  if (!q_lambda.empty()) q_lambda.clear();
  q_lambda.insert(q_lambda.end(), l, et0);
  if (!q_x_O.empty()) q_x_O.clear();
  q_x_O.insert(q_x_O.end(), l, et0);
  if (!q_x_S.empty()) q_x_S.clear();
  q_x_S.insert(q_x_S.end(), slack_A.size(), et0);
  
  if (!tmp_l.empty()) tmp_l.clear();
  tmp_l.insert(tmp_l.end(), l, et0);
  if (!tmp_l_2.empty()) tmp_l_2.clear();
  tmp_l_2.insert(tmp_l_2.end(), l, et0);
  if (!tmp_x.empty()) tmp_x.clear();
  tmp_x.insert(tmp_x.end(), l, et0);
  if (!tmp_x_2.empty()) tmp_x_2.clear();
  tmp_x_2.insert(tmp_x_2.end(), l, et0);
}

CGAL_END_NAMESPACE

// ===== EOF ==================================================================
