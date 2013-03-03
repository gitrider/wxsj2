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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Min_sphere_d_new/include/CGAL/Min_sphere_d_new.h $
// $Id: Min_sphere_d_new.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Bernd Gaertner, Sven Schoenherr <sven@inf.ethz.ch>

#ifndef CGAL_MIN_SPHERE_D_H
#define CGAL_MIN_SPHERE_D_H

// includes
// --------
#ifndef CGAL_OPTIMISATION_BASIC_H
#  include <CGAL/Optimisation/basic.h>
#endif

#include <CGAL/function_objects.h>
#include <CGAL/QP_solver.h>
#include <CGAL/QP_partial_filtered_pricing.h>
#include <CGAL/QP_partial_exact_pricing.h>

CGAL_BEGIN_NAMESPACE

// Class declarations
// ==================
template < class Traits_ >
class Min_sphere_d;

template < class ET_, class NT_, class Point, class Point_iterator,
           class Access_coord, class Access_dim >
struct QP_rep_min_sphere_d;

template < class NT, class Point, class Point_iterator,
           class Access_coord, class Access_dim >
struct QP_rep_row_of_d;


// Interface to QP-solver
// ======================

// Following class is used to compute the scalar product p_i^p_j
// of two points: you create an instance of class QP_rep_inner_product
// for p_i, say, and then call operator(p_j):
template < class NT, class Point,
           class Access_coord, class Access_dim >
class QP_rep_inner_product
  : public std::unary_function< Point, NT > {
  Point         p_i;
  Access_coord  da_coord;
  Access_dim    da_dim;
public:
  QP_rep_inner_product( ) { }
  QP_rep_inner_product( const Point& p,
			const Access_coord& ac,
			const Access_dim&   ad)
    : p_i( p), da_coord( ac), da_dim( ad) { }

  NT operator( ) ( const Point& p_j) const
  { return std::inner_product( da_coord( p_i),
			       da_coord( p_i)+da_dim( p_i),
			       da_coord( p_j), NT( 0),
			       std::plus<NT>(),
			       std::multiplies<NT>()); }
};

// The following is used to compute a row of the matrix P^T P where P
// is a matrix holding as columns some points p_i.  More precisely,
// the following is a functor that takes an iterator over a set of
// points (think of an iterator over P) and once you call
// operator(p_i), you obtain an iterator over the "row" p_i^Tp_j, p_j
// in P.
template < class NT, class Point, class Point_iterator,
           class Access_coord, class Access_dim >
class QP_rep_row_of_d {
  Point_iterator  pts_it;
  Access_coord    da_coord;
  Access_dim      da_dim;
public:
  typedef  QP_rep_inner_product<
    NT, Point, Access_coord, Access_dim >
  Inner_product;
  typedef  Join_input_iterator_1<
    Point_iterator, Inner_product >
  Row_of_d;

  typedef  Point                  argument_type;
  typedef  Row_of_d               result_type;

  QP_rep_row_of_d( ) { }
  QP_rep_row_of_d( const Point_iterator& it,
		   const Access_coord&   ac,
		   const Access_dim&     ad)
    : pts_it( it), da_coord( ac), da_dim( ad) { }

  Row_of_d  operator( ) ( const Point& p_i) const
  { return Row_of_d( pts_it, Inner_product( p_i, da_coord, da_dim));}
};

// A functor whose operator(int i) provides access to the i-th element
// of a random access iterator.
template < typename RndAccIt, typename ArgType >
class Access_by_index {
public:
  typedef  typename std::iterator_traits<RndAccIt>::value_type result_type;

  Access_by_index(RndAccIt it = RndAccIt()) : a(it) {}

  result_type operator () (ArgType i) const { return a[i]; }

private:
  RndAccIt     a;
};

// The traits class for the QP-solver:
template < class ET_, class NT_, class Point, class Point_iterator,
           class Access_coord, class Access_dim >
struct QP_rep_min_sphere_d {
  // (possibly inexact) number type for the pricing:
  typedef  NT_                    NT;
  
  // stuff required by concept QPSolverTraits:
  typedef  ET_                                                ET;
  typedef  Const_oneset_iterator< Const_oneset_iterator<NT> > A_iterator;
  typedef  Const_oneset_iterator<NT>                          B_iterator;
  typedef  typename std::vector<NT>::const_iterator           C_iterator;
  typedef  Join_input_iterator_1<
    Point_iterator, QP_rep_row_of_d<
    NT, Point, Point_iterator, Access_coord, Access_dim > >   D_iterator;
  
  enum Row_type { LESS_EQUAL = -1, EQUAL, GREATER_EQUAL};
  typedef Const_oneset_iterator<Row_type>    Row_type_iterator;
  
  typedef Const_oneset_iterator<bool> FU_iterator;
  typedef Const_oneset_iterator<bool> FL_iterator;
  typedef Const_oneset_iterator<NT>   U_iterator;
  typedef Const_oneset_iterator<NT>   L_iterator;

  typedef Tag_false                          Is_linear;
  typedef Tag_true                           Is_symmetric;
  typedef Tag_true                           Has_equalities_only_and_full_rank;
  typedef Tag_true                           Is_in_standard_form;
};

// Class interfaces
// ================
template < class Traits_ >
class Min_sphere_d {
public:
  // self
  typedef  Traits_                    Traits;
  typedef  Min_sphere_d<Traits>       Self;

  // types from the traits class
  typedef  typename Traits::Point_d   Point;

  typedef  typename Traits::Rep_tag   Rep_tag;

  typedef  typename Traits::RT        RT;
  typedef  typename Traits::FT        FT;

  typedef  typename Traits::Access_dimension_d
  Access_dimension_d;
  typedef  typename Traits::Access_coordinates_begin_d
  Access_coordinates_begin_d;

  typedef  typename Traits::Construct_point_d
  Construct_point_d;

  typedef  typename Traits::ET        ET;
  typedef  typename Traits::NT        NT;

private:
  // QP solver
  typedef  QP_rep_min_sphere_d<
    ET, NT, Point, typename std::vector<Point>::const_iterator,
    Access_coordinates_begin_d, Access_dimension_d >
  QP_rep;
  typedef  QP_solver< QP_rep >  Solver;
  typedef  typename Solver::Pricing_strategy
  Pricing_strategy;
  // types from the QP solver
  typedef  typename Solver::Basic_variable_index_iterator
  Basic_variable_index_iterator;
    
  // private types
  typedef  std::vector<Point>         Point_vector;
  typedef  std::vector<ET>            ET_vector;
    
  typedef  Access_by_index<typename std::vector<Point>::const_iterator, int>
  Point_by_index;
    
public:
  // public types
  typedef  typename Point_vector::const_iterator
  Point_iterator;
    
  typedef  Join_input_iterator_1<
    Basic_variable_index_iterator, Point_by_index >
  Support_point_iterator;
    
  typedef  typename ET_vector::const_iterator
  Coordinate_iterator;
    

  // creation
  Min_sphere_d( const Traits&  traits  = Traits())
    : tco( traits), d( -1), solver(0), strategy(0) {}
    
  template < class InputIterator >
  Min_sphere_d( InputIterator  first,
		InputIterator  last,
		const Traits&  traits = Traits())
    : tco( traits), solver(0), strategy(0)
  {
    set( first, last);
  }

  ~Min_sphere_d() {
    if (solver)
      delete solver;
    if (strategy)
      delete strategy;
  }
    
  // access to point set
  int  ambient_dimension( ) const { return d; }
    
  int  number_of_points( ) const { return points.size(); }
    
  Point_iterator  points_begin( ) const { return points.begin(); }
  Point_iterator  points_end  ( ) const { return points.end  (); }
    
  // access to support points
  int number_of_support_points( ) const {
    if (is_empty())
      return 0;
    else {
      CGAL_optimisation_assertion_msg(solver != 0,
	    "number_of_support_points: solver is nil");
      return solver->number_of_basic_variables(); 
    }
  }
    
  Support_point_iterator
  support_points_begin() const {
    CGAL_optimisation_assertion_msg(!is_empty(),
		 "support_points_begin: no points inserted");
    return Support_point_iterator(
		 solver->basic_original_variables_index_begin(),
		 Point_by_index( points.begin()));
  }
    
  Support_point_iterator
  support_points_end() const {
    CGAL_optimisation_assertion_msg(!is_empty(),
			 "support_points_end: no points inserted");
    return Support_point_iterator(
			 solver->basic_original_variables_index_end(),
			 Point_by_index( points.begin()));
  }
    
  // access to center (rational representation)
  Coordinate_iterator
  center_coordinates_begin( ) const { return center_coords.begin(); }
    
  Coordinate_iterator
  center_coordinates_end  ( ) const { return center_coords.end  (); }
    
  // access to squared radius (rational representation)
  ET  squared_radius_numerator  ( ) const { return sqr_rad_numer; }
  ET  squared_radius_denominator( ) const { return sqr_rad_denom; }
    
  // access to center and squared radius
  // NOTE: an implicit conversion from ET to RT must be available!
  Point
  center( ) const
  { CGAL_optimisation_precondition( ! is_empty());
  return tco.construct_point_d_object()( ambient_dimension(),
					 center_coordinates_begin(),
					 center_coordinates_end()); }
    
  FT
  squared_radius( ) const
  { CGAL_optimisation_precondition( ! is_empty());
  return FT( squared_radius_numerator  ()) /
    FT( squared_radius_denominator()); }
    
  // predicates
  Bounded_side
  bounded_side( const Point& p) const
  { CGAL_optimisation_precondition(is_empty() ||
				   tco.access_dimension_d_object()( p) == d);
    return Bounded_side( CGAL_NTS sign(sqr_rad_numer - sqr_dist( p)));
  }
    
  bool
  has_on_bounded_side( const Point& p) const
  { CGAL_optimisation_precondition(is_empty() ||
				   tco.access_dimension_d_object()( p) == d);
  return ( sqr_dist( p) < sqr_rad_numer); }
    
  bool
  has_on_boundary( const Point& p) const
  { CGAL_optimisation_precondition(is_empty() ||
				   tco.access_dimension_d_object()( p) == d);
  return ( sqr_dist( p) == sqr_rad_numer); }
    
  bool
  has_on_unbounded_side( const Point& p) const
  { CGAL_optimisation_precondition(is_empty() ||
				   tco.access_dimension_d_object()( p) == d);
  return( sqr_dist( p) > sqr_rad_numer); }
    
  bool  is_empty     ( ) const { return number_of_points() == 0; }
  bool  is_degenerate( ) const { return number_of_support_points() < 2; }
    
  // modifiers
  template < class InputIterator >
  void
  set( InputIterator first, InputIterator last)
  { if ( points.size() > 0) points.erase( points.begin(), points.end());
  std::copy( first, last, std::back_inserter( points));
  set_dimension();
  CGAL_optimisation_precondition_msg( check_dimension(),
		      "Not all points have the same dimension.");
  compute_min_sphere(); }
    
  void
  insert( const Point& p)
  { CGAL_optimisation_precondition( is_empty() ||
		    ( tco.access_dimension_d_object()( p) == d));
  points.push_back( p);
  compute_min_sphere(); }
    
  template < class InputIterator >
  void
  insert( InputIterator first, InputIterator last)
  { CGAL_optimisation_precondition_code( int old_n = points.size());
  points.insert( points.end(), first, last);
  set_dimension();
  CGAL_optimisation_precondition_msg( check_dimension( old_n),
		      "Not all points have the same dimension.");
  compute_min_sphere(); }
    
  void
  clear( )
  { points.erase( points.begin(), points.end());
  compute_min_sphere(); }
    
  // validity check
  bool  is_valid( bool verbose = false, int level = 0) const;
    
  // traits class access
  const Traits&  traits( ) const { return tco; }
    

private:
  Traits                   tco;            // traits class object
  
  Point_vector             points;         // input points
  int                      d;              // dimension of input points
  
  ET_vector                center_coords;  // center of small.encl.sphere
  
  ET                       sqr_rad_numer;  // squared radius of
  ET                       sqr_rad_denom;  // smallest enclosing sphere
  
  Solver                   *solver;        // quadratic programming solver
  Pricing_strategy         *strategy;      // ...and its pricing strategy
  
  std::vector<NT>          c_vector;       // vector `c' of QP
  
private:
  template < class NT >
  Pricing_strategy *pricing_strategy( NT) {
    return new QP_partial_filtered_pricing<QP_rep>;
  }
  
  Pricing_strategy *pricing_strategy( ET) {
    return new QP_partial_exact_pricing<QP_rep>;
  }
    
  // squared distance to center
  ET
  sqr_dist( const Point& p) const
  { return std::inner_product(center_coords.begin(), center_coords.end()-1,
      tco.access_coordinates_begin_d_object()( p), ET( 0),
	      std::plus<ET>(),
	      compose1_2(
			 compose2_1( std::multiplies<ET>(),
				     Identity<ET>(), Identity<ET>()),
			 compose2_2( std::minus<ET>(),
				     Identity<ET>(),
				     std::bind2nd( std::multiplies<ET>(),
						   center_coords.back())))); }

  // set dimension of input points
  void
  set_dimension( )
  { d = ( points.size() == 0 ? -1 :
	  tco.access_dimension_d_object()( points[ 0])); }
    
  // check dimension of input points
  bool
  check_dimension( unsigned int  offset = 0)
  { return ( std::find_if( points.begin()+offset, points.end(),
	   compose1_1( std::bind2nd(
				    std::not_equal_to<int>(), d),
		       tco.access_dimension_d_object()))
	     == points.end()); }
    
  // compute smallest enclosing sphere
  void
  compute_min_sphere( )
  {
    if ( is_empty()) {
      center_coords.resize( 1);
      sqr_rad_numer = -ET( 1);
      return;
    }
    
    // set up and solve QP
    c_vector.resize( points.size());
    int i;
    for ( i = 0; i < number_of_points(); ++i) {
      c_vector[ i] = -std::inner_product(
		 tco.access_coordinates_begin_d_object()( points[ i]),
		 tco.access_coordinates_begin_d_object()( points[ i])+d,
		 tco.access_coordinates_begin_d_object()( points[ i]), NT( 0),
		 std::plus<NT>(), std::multiplies<NT>());
    }
    typedef  typename QP_rep::A_iterator A_it;
    typedef  typename QP_rep::B_iterator B_it;
    typedef  typename QP_rep::D_iterator D_it;
    typedef  typename QP_rep::Row_type_iterator Row_it;
    B_it  const_one( 1);

    strategy = pricing_strategy(NT());
    solver = new Solver(points.size(), 1,
			A_it( const_one),
			const_one, c_vector.begin(),
			D_it( points.begin(),
			      QP_rep_row_of_d< NT,
			      Point,
			      Point_iterator,
			      Access_coordinates_begin_d,
			      Access_dimension_d >(
				   points.begin(),
				   tco.access_coordinates_begin_d_object(),
				   tco.access_dimension_d_object())),
			Row_it(QP_rep::EQUAL),
			strategy);
    
    // compute center and squared radius
    center_coords.resize( ambient_dimension()+1);
    std::fill( center_coords.begin(), center_coords.end(), ET( 0));
    for ( i = 0; i < solver->number_of_basic_variables(); ++i) {
      ET  value = solver->basic_original_variables_numerator_begin()[ i];
      int index = solver->basic_original_variables_index_begin()[ i];
      for ( int j = 0; j < d; ++j)
	center_coords[ j] += value
	  * tco.access_coordinates_begin_d_object()( points[ index])[ j];
    }
    center_coords[ d] = solver->variables_common_denominator();
    sqr_rad_numer     = -solver->solution_numerator();
    sqr_rad_denom     = center_coords[ d] * center_coords[ d];
  }
    
};

// Function declarations
// =====================
// I/O operators
template < class Traits_ >
std::ostream&
operator << ( std::ostream& os, const Min_sphere_d<Traits_>& min_sphere);

template < class Traits_ >
std::istream&
operator >> ( std::istream& is,       Min_sphere_d<Traits_>& min_sphere);

// ============================================================================

// Class implementation
// ====================

// validity check
template < class Traits_ >
bool
Min_sphere_d<Traits_>::
is_valid( bool verbose, int level) const
{
  using namespace std;

  Verbose_ostream verr( verbose);
  verr << "CGAL::Min_sphere_d<Traits>::" << endl;
  verr << "is_valid( true, " << level << "):" << endl;
  const int nrOfPts = number_of_points();
  verr << "  |P| = " << nrOfPts
       << ", |S| = " << number_of_support_points() << endl;

  // containment check (a)
  // ---------------------
  verr << "  (a) containment check..." << flush;
    
  Point_iterator  point_it = points_begin();
  for ( ; point_it != points_end(); ++point_it) {
    if ( has_on_unbounded_side( *point_it))
      return _optimisation_is_valid_fail( verr,
		"sphere does not contain all points");
  }
    
  verr << "passed." << endl;

  // support set check (b)
  // ---------------------
  verr << "  (b) support set check..." << flush;
    
  // all support points on boundary?
  if (nrOfPts > 0) {
    Support_point_iterator  support_point_it = support_points_begin();
    for ( ; support_point_it != support_points_end(); ++support_point_it) {
      if ( ! has_on_boundary( *support_point_it))
	return _optimisation_is_valid_fail( verr,
		   "sphere does not have all support points on its boundary");
    }
    
    // center strictly in convex hull of support points?
    typename Solver::Basic_variable_numerator_iterator
      num_it = solver->basic_original_variables_numerator_begin();
    for ( ; num_it != solver->basic_original_variables_numerator_end();
	  ++num_it) {
      if ( ! (    CGAL_NTS is_positive( *num_it)
		  && *num_it <= solver->variables_common_denominator()))
	return _optimisation_is_valid_fail( verr,
	   "center does not lie strictly in convex hull of support points");
    }
  }
  verr << "passed." << endl;

  verr << "  object is valid!" << endl;
  return( true);
}

// output operator
template < class Traits_ >
std::ostream&
operator << ( std::ostream& os,
              const Min_sphere_d<Traits_>& min_sphere)
{
  using namespace std;

  typedef  typename Min_sphere_d<Traits_>::Point  Point;
  typedef  ostream_iterator<Point>       Os_it;
  typedef  typename Traits_::ET          ET;
  typedef  ostream_iterator<ET>          Et_it;

  switch ( get_mode( os)) {

  case IO::PRETTY:
    os << "CGAL::Min_sphere_d( |P| = " << min_sphere.number_of_points()
       << ", |S| = " << min_sphere.number_of_support_points() << endl;
    os << "  P = {" << endl;
    os << "    ";
    copy( min_sphere.points_begin(), min_sphere.points_end(),
	  Os_it( os, ",\n    "));
    os << "}" << endl;
    os << "  S = {" << endl;
    os << "    ";
    copy( min_sphere.support_points_begin(),
	  min_sphere.support_points_end(),
	  Os_it( os, ",\n    "));
    os << "}" << endl;
    os << "  center = ( ";
    copy( min_sphere.center_coordinates_begin(),
	  min_sphere.center_coordinates_end(),
	  Et_it( os, " "));
    os << ")" << endl;
    os << "  squared radius = "
       << min_sphere.squared_radius_numerator() << " / "
       << min_sphere.squared_radius_denominator() << endl;
    os << ")" << endl;
    break;

  case IO::ASCII:
    copy( min_sphere.points_begin(), min_sphere.points_end(),
	  Os_it( os, "\n"));
    break;

  case IO::BINARY:
    copy( min_sphere.points_begin(), min_sphere.points_end(),
	  Os_it( os));
    break;

  default:
    CGAL_optimisation_assertion_msg( false,
				     "CGAL::get_mode( os) invalid!");
    break; }

  return( os);
}

// input operator
template < class Traits_ >
std::istream&
operator >> ( std::istream& is, Min_sphere_d<Traits_>& min_sphere)
{
  using namespace std;

  switch ( get_mode( is)) {

  case IO::PRETTY:
    cerr << endl;
    cerr << "Stream must be in ascii or binary mode" << endl;
    break;

  case IO::ASCII:
  case IO::BINARY:
    typedef  typename Min_sphere_d<Traits_>::Point  Point;
    typedef  istream_iterator<Point>             Is_it;
    min_sphere.set( Is_it( is), Is_it());
    break;

  default:
    CGAL_optimisation_assertion_msg( false, "IO::mode invalid!");
    break; }

  return( is);
}

CGAL_END_NAMESPACE

#endif // CGAL_MIN_SPHERE_D_H

// ===== EOF ==================================================================
