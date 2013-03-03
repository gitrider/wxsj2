// Copyright (c) 1999-2004  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Triangulation_3/include/CGAL/Regular_triangulation_3.h $
// $Id: Regular_triangulation_3.h 29437 2006-03-13 08:11:56Z cdelage $
// 
//
// Author(s)     : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//                 Sylvain Pion <Sylvain.Pion@sophia.inria.fr>
//                 Christophe Delage <Christophe.Delage@sophia.inria.fr>

#ifndef CGAL_REGULAR_TRIANGULATION_3_H
#define CGAL_REGULAR_TRIANGULATION_3_H

#include <CGAL/basic.h>

#include <set>

#include <CGAL/Triangulation_short_names_3.h>
#include <CGAL/Triangulation_3.h>
#include <CGAL/Regular_triangulation_cell_base_3.h>
#include <CGAL/Unique_hash_map.h>

CGAL_BEGIN_NAMESPACE

template < class Gt,
           class Tds = Triangulation_data_structure_3 <
                           Triangulation_vertex_base_3<Gt>,
                           Regular_triangulation_cell_base_3<Gt> > >
class Regular_triangulation_3
  : public Triangulation_3<Gt,Tds>
{
  typedef Regular_triangulation_3<Gt, Tds>      Self;
  typedef Triangulation_3<Gt,Tds>               Tr_Base;
public:
  typedef Tds                                   Triangulation_data_structure;
  typedef Gt                                    Geom_traits;

  typedef typename Tr_Base::Vertex_handle       Vertex_handle;
  typedef typename Tr_Base::Cell_handle         Cell_handle;
  typedef typename Tr_Base::Vertex              Vertex;
  typedef typename Tr_Base::Cell                Cell;
  typedef typename Tr_Base::Facet               Facet;
  typedef typename Tr_Base::Edge                Edge;

  typedef Triple<Vertex_handle,Vertex_handle,Vertex_handle> Vertex_triple;

  typedef typename Tr_Base::Locate_type         Locate_type;
  typedef typename Tr_Base::Cell_iterator       Cell_iterator;
  typedef typename Tr_Base::Facet_iterator      Facet_iterator;
  typedef typename Tr_Base::Edge_iterator       Edge_iterator;
  typedef typename Tr_Base::Facet_circulator    Facet_circulator;

  typedef typename Tr_Base::Finite_vertices_iterator Finite_vertices_iterator;
  typedef typename Tr_Base::Finite_cells_iterator    Finite_cells_iterator;
  typedef typename Tr_Base::Finite_facets_iterator   Finite_facets_iterator;
  typedef typename Tr_Base::Finite_edges_iterator    Finite_edges_iterator;
  typedef typename Tr_Base::All_cells_iterator       All_cells_iterator;

  typedef typename Gt::Weighted_point_3            Weighted_point;
  typedef typename Gt::Bare_point                  Bare_point;
  typedef typename Gt::Segment_3                   Segment;
  typedef typename Gt::Triangle_3                  Triangle;
  typedef typename Gt::Tetrahedron_3               Tetrahedron;

  // types for dual:
  typedef typename Gt::Line_3        Line;
  typedef typename Gt::Ray_3         Ray;
  typedef typename Gt::Plane_3       Plane;
  typedef typename Gt::Object_3      Object;

  //Tag to distinguish Delaunay from Regular triangulations
  typedef Tag_true   Weighted_tag; 

  using Tr_Base::cw;
  using Tr_Base::ccw;
#ifndef CGAL_CFG_USING_BASE_MEMBER_BUG_2
  using Tr_Base::geom_traits;
#endif
  using Tr_Base::number_of_vertices;
  using Tr_Base::dimension;
  using Tr_Base::finite_facets_begin;
  using Tr_Base::finite_facets_end;
  using Tr_Base::finite_vertices_begin;
  using Tr_Base::finite_vertices_end;
  using Tr_Base::finite_cells_begin;
  using Tr_Base::finite_cells_end;
  using Tr_Base::finite_edges_begin;
  using Tr_Base::finite_edges_end;
  using Tr_Base::tds;
  using Tr_Base::infinite_vertex;
  using Tr_Base::next_around_edge;
  using Tr_Base::vertex_triple_index;
  using Tr_Base::mirror_vertex;
  using Tr_Base::mirror_index;
  using Tr_Base::orientation;
  using Tr_Base::coplanar_orientation;

  Regular_triangulation_3(const Gt & gt = Gt())
    : Tr_Base(gt)
  {}

  // copy constructor duplicates vertices and cells
  Regular_triangulation_3(const Regular_triangulation_3 & rt)
      : Tr_Base(rt)
  {
      CGAL_triangulation_postcondition( is_valid() );
  }

  //insertion
  template < typename InputIterator >
  Regular_triangulation_3(InputIterator first, InputIterator last,
                          const Gt & gt = Gt())
      : Tr_Base(gt)
  {
      insert(first, last);
  }

  template < class InputIterator >
  int
  insert(InputIterator first, InputIterator last)
  {
    int n = number_of_vertices();
    while(first != last){
      insert(*first);
      ++first;
    }
    return number_of_vertices() - n;
  }

  Vertex_handle insert(const Weighted_point & p,
	               Cell_handle start = Cell_handle());

  Vertex_handle insert(const Weighted_point & p, Locate_type lt,
	               Cell_handle c, int li, int);

  template <class OutputIteratorBoundaryFacets,
            class OutputIteratorCells,
            class OutputIteratorInternalFacets>
  Triple<OutputIteratorBoundaryFacets,
         OutputIteratorCells,
         OutputIteratorInternalFacets>
  find_conflicts(const Weighted_point &p, Cell_handle c,
	         OutputIteratorBoundaryFacets bfit,
                 OutputIteratorCells cit,
		 OutputIteratorInternalFacets ifit) const
  {
      CGAL_triangulation_precondition(dimension() >= 2);

      std::vector<Cell_handle> cells;
      cells.reserve(32);
      std::vector<Facet> facets;
      facets.reserve(64);

      if (dimension() == 2) {
          Conflict_tester_for_find_conflicts_2 tester(p, this);
	  ifit = find_conflicts_2(c, tester,
                                  make_triple(std::back_inserter(facets),
		                              std::back_inserter(cells),
                                              ifit)).third;
      }
      else {
          Conflict_tester_for_find_conflicts_3 tester(p, this);
	  ifit = find_conflicts_3(c, tester,
                                  make_triple(std::back_inserter(facets),
		                              std::back_inserter(cells),
                                              ifit)).third;
      }

      // Reset the conflict flag on the boundary.
      for(typename std::vector<Facet>::iterator fit=facets.begin();
          fit != facets.end(); ++fit) {
        fit->first->neighbor(fit->second)->set_in_conflict_flag(0);
	*bfit++ = *fit;
      }

      // Reset the conflict flag in the conflict cells.
      for(typename std::vector<Cell_handle>::iterator ccit=cells.begin();
        ccit != cells.end(); ++ccit) {
        (*ccit)->set_in_conflict_flag(0);
	*cit++ = *ccit;
      }
      return make_triple(bfit, cit, ifit);
  }

  template <class OutputIteratorBoundaryFacets, class OutputIteratorCells>
  std::pair<OutputIteratorBoundaryFacets, OutputIteratorCells>
  find_conflicts(const Weighted_point &p, Cell_handle c,
	         OutputIteratorBoundaryFacets bfit,
                 OutputIteratorCells cit) const
  {
      Triple<OutputIteratorBoundaryFacets,
             OutputIteratorCells,
	     Emptyset_iterator> t = find_conflicts(p, c, bfit, cit,
		                                   Emptyset_iterator());
      return std::make_pair(t.first, t.second);
  }

  void remove (Vertex_handle v);

  template < typename InputIterator >
  int remove(InputIterator first, InputIterator beyond)
  {
    int n = number_of_vertices();
    while (first != beyond) {
      remove (*first);
      ++first;
    }
    return n - number_of_vertices();
  }

  Vertex_handle move_point(Vertex_handle v, const Weighted_point & p);

private:
  typedef Facet Edge_2D;
 
  template < class OutputIterator >
  OutputIterator
  make_hole_2D(Vertex_handle v, std::list<Edge_2D> & hole, OutputIterator hidden);
  
  void fill_hole_regular_2D(std::list<Edge_2D> & hole);

  void make_canonical(Vertex_triple& t) const;

  Vertex_triple make_vertex_triple(const Facet& f) const;

#ifndef CGAL_CFG_NET2003_MATCHING_BUG
  void make_hole_3D(Vertex_handle v, 
		    std::map<Vertex_triple,Facet> &outer_map,
		    std::vector<Cell_handle> &hole);
#else
  void make_hole_3D(Vertex_handle v, 
		    std::map<Vertex_triple,Facet> &outer_map,
		    std::vector<Cell_handle> &hole)
  {
    CGAL_triangulation_expensive_precondition( ! test_dim_down(v) );

    incident_cells(v, std::back_inserter(hole));

    for (typename std::vector<Cell_handle>::iterator cit = hole.begin();
	 cit != hole.end(); ++cit) {
      int indv = (*cit)->index(v);
      Cell_handle opp_cit = (*cit)->neighbor( indv );
      Facet f(opp_cit, opp_cit->index(*cit)); 
      Vertex_triple vt = make_vertex_triple(f);
      make_canonical(vt);
      outer_map[vt] = f;
      for (int i=0; i<4; i++)
	if ( i != indv )
	  (*cit)->vertex(i)->set_cell(opp_cit);
    }
  }
#endif

  template < class OutputIterator >
  OutputIterator remove_dim_down(Vertex_handle v, OutputIterator hidden);
  template < class OutputIterator >
  OutputIterator remove_1D(Vertex_handle v, OutputIterator hidden);
  template < class OutputIterator >
  OutputIterator remove_2D(Vertex_handle v, OutputIterator hidden);
  template < class OutputIterator >
  OutputIterator remove_3D(Vertex_handle v, OutputIterator hidden);

protected:

  Oriented_side
  side_of_oriented_power_sphere(const Weighted_point &p0,
                                const Weighted_point &p1,
                                const Weighted_point &p2,
                                const Weighted_point &p3,
                                const Weighted_point &p,
                                bool perturb = false) const;

  Oriented_side
  side_of_oriented_power_circle(const Weighted_point &p0,
                                const Weighted_point &p1,
                                const Weighted_point &p2,
                                const Weighted_point &p,
                                bool perturb = false) const;

  Bounded_side
  side_of_bounded_power_circle(const Weighted_point &p0,
                               const Weighted_point &p1,
                               const Weighted_point &p2,
                               const Weighted_point &p,
                               bool perturb = false) const;

  Bounded_side
  side_of_bounded_power_segment(const Weighted_point &p0,
                                const Weighted_point &p1,
                                const Weighted_point &p,
                                bool perturb = false) const;

  
public:

  // Queries
  Bounded_side
  side_of_power_sphere(Cell_handle c, const Weighted_point &p,
                       bool perturb = false) const;

  Bounded_side
  side_of_power_circle(const Facet & f, const Weighted_point & p,
                       bool perturb = false) const
  {
      return side_of_power_circle(f.first, f.second, p);
  }

  Bounded_side
  side_of_power_circle(Cell_handle c, int i, const Weighted_point &p,
                       bool perturb = false) const;

  Bounded_side
  side_of_power_segment(Cell_handle c, const Weighted_point &p,
                        bool perturb = false) const;

  Vertex_handle
  nearest_power_vertex_in_cell(const Bare_point& p, 
			       const Cell_handle& c)  const;

  Vertex_handle
  nearest_power_vertex(const Bare_point& p, Cell_handle c =
		       Cell_handle()) const;

  bool is_Gabriel(Cell_handle c, int i) const;
  bool is_Gabriel(Cell_handle c, int i, int j) const;
  bool is_Gabriel(const Facet& f)const ;
  bool is_Gabriel(const Edge& e) const;
  bool is_Gabriel(Vertex_handle v) const;


  // Dual functions
  Bare_point dual(Cell_handle c) const;

  Object dual(const Facet & f) const
    { return dual( f.first, f.second ); }

  Object dual(Cell_handle c, int i) const;

  template < class Stream> 		
  Stream& draw_dual(Stream & os)
    {
      for (Finite_facets_iterator fit = finite_facets_begin(),
                                  end = finite_facets_end(); 
           fit != end; ++fit) {
	Object o = dual(*fit);
	if (const Bare_point *p = object_cast<Bare_point>(&o)) os << *p;
	if (const Segment    *s = object_cast<Segment>(&o))    os << *s; 
	if (const Ray        *r = object_cast<Ray>(&o))        os << *r;
      }
      return os;
    }

  bool is_valid(bool verbose = false, int level = 0) const;

private:
  bool
  less_power_distance(const Bare_point &p, 
		      const Weighted_point &q, 
		      const Weighted_point &r)  const 
  {
    return 
      geom_traits().compare_power_distance_3_object()(p, q, r) == SMALLER;
  }

  Bare_point
  construct_weighted_circumcenter(const Weighted_point &p, 
				  const Weighted_point &q, 
				  const Weighted_point &r, 
				  const Weighted_point &s) const
  {
     return geom_traits().construct_weighted_circumcenter_3_object()(p,q,r,s);
  }

  Bare_point
  construct_weighted_circumcenter(const Weighted_point &p, 
				  const Weighted_point &q, 
				  const Weighted_point &r) const
  {
     return geom_traits().construct_weighted_circumcenter_3_object()(p,q,r);
  }

  Line
  construct_perpendicular_line(const Plane &pl, const Bare_point &p) const
  {
      return geom_traits().construct_perpendicular_line_3_object()(pl, p);
  }

  Plane
  construct_plane(const Bare_point &p, const Bare_point &q, const Bare_point &r) const
  {
      return geom_traits().construct_plane_3_object()(p, q, r);
  }

  Ray
  construct_ray(const Bare_point &p, const Line &l) const
  {
      return geom_traits().construct_ray_3_object()(p, l);
  }

  Object
  construct_object(const Bare_point &p) const
  {
      return geom_traits().construct_object_3_object()(p);
  }

  Object
  construct_object(const Segment &s) const
  {
      return geom_traits().construct_object_3_object()(s);
  }

  Object
  construct_object(const Ray &r) const
  {
      return geom_traits().construct_object_3_object()(r);
  }

  Vertex_handle
  nearest_power_vertex(const Bare_point &p, 
		       Vertex_handle v,
		       Vertex_handle w) const
  {
      // In case of equality, v is returned.
      CGAL_triangulation_precondition(v != w);
      if (is_infinite(v))	  return w;
      if (is_infinite(w))	  return v;
      return less_power_distance(p, w->point(), v->point()) ? w : v;
  }

  Oriented_side
  power_test(const Weighted_point &p, const Weighted_point &q) const
  {
      CGAL_triangulation_precondition(this->equal(p, q));
      return geom_traits().power_test_3_object()(p, q);
  }

  Oriented_side
  power_test(const Weighted_point &p, const Weighted_point &q,
	     const Weighted_point &r) const
  {
      CGAL_triangulation_precondition(this->collinear(p, q, r));
      return geom_traits().power_test_3_object()(p, q, r);
  }

  Oriented_side
  power_test(const Weighted_point &p, const Weighted_point &q,
	     const Weighted_point &r, const Weighted_point &s) const
  {
      CGAL_triangulation_precondition(this->coplanar(p, q, r, s));
      return geom_traits().power_test_3_object()(p, q, r, s);
  }

  Oriented_side
  power_test(const Weighted_point &p, const Weighted_point &q,
	     const Weighted_point &r, const Weighted_point &s,
	     const Weighted_point &t) const
  {
      return geom_traits().power_test_3_object()(p, q, r, s, t);
  }

  bool in_conflict_3(const Weighted_point &p, const Cell_handle c) const
  {
      return side_of_power_sphere(c, p, true) == ON_BOUNDED_SIDE;
  }

  bool in_conflict_2(const Weighted_point &p, const Cell_handle c, int i) const
  {
      return side_of_power_circle(c, i, p, true) == ON_BOUNDED_SIDE;
  }

  bool in_conflict_1(const Weighted_point &p, const Cell_handle c) const
  {
      return side_of_power_segment(c, p, true) == ON_BOUNDED_SIDE;
  }

  bool in_conflict_0(const Weighted_point &p, const Cell_handle c) const
  {
      return power_test(c->vertex(0)->point(), p) == ON_POSITIVE_SIDE;
  }

  bool in_conflict(const Weighted_point &p, const Cell_handle c) const
  {
      switch (dimension()) {
          case 0: return in_conflict_0(p, c);
          case 1: return in_conflict_1(p, c);
          case 2: return in_conflict_2(p, c, 3);
          case 3: return in_conflict_3(p, c);
      }
      return true;
  }

  // Conflict_tester_for_find_conflicts_2 and _3 are const
  // while Conflict_tester_2 and _3 are not
  class Conflict_tester_for_find_conflicts_3
  {
      const Weighted_point &p;
      const Self *t;
  public:

      Conflict_tester_for_find_conflicts_3(const Weighted_point &pt, const Self *tr)
	  : p(pt), t(tr) {}

      bool operator()(const Cell_handle c) const
      {
	return t->in_conflict_3(p, c);
      }
  };

  class Conflict_tester_for_find_conflicts_2
  {
      const Weighted_point &p;
      const Self *t;
  public:

      Conflict_tester_for_find_conflicts_2(const Weighted_point &pt, const Self *tr)
	  : p(pt), t(tr) {}

      bool operator()(const Cell_handle c) const
      {
	return t->in_conflict_2(p, c, 3);
      }
  };

  class Conflict_tester_3
  {
      const Weighted_point &p;
      const Self *t;
      mutable std::vector<Vertex_handle> &cv;

  public:

      Conflict_tester_3(const Weighted_point &pt, const Self *tr, 
			std::vector<Vertex_handle> &_cv)
	  : p(pt), t(tr), cv(_cv) {}

      bool operator()(const Cell_handle c) const
      {
	  // We mark the vertices so that we can find the deleted ones easily.
	  if (t->in_conflict_3(p, c))
	  {
	      for (int i=0; i<4; i++)
	      {
		  Vertex_handle v = c->vertex(i);
		  if (v->cell() != Cell_handle())
		  {
		      cv.push_back(v);
		      v->set_cell(Cell_handle());
		  }
	      }
	      return true;
	  }
	  return false;
      }

      std::vector<Vertex_handle> & conflict_vector()
      {
	  return cv;
      }
  };

  class Conflict_tester_2
  {
      const Weighted_point &p;
      const Self *t;
      mutable std::vector<Vertex_handle> &cv;

  public:

      Conflict_tester_2(const Weighted_point &pt, const Self *tr, 
			std::vector<Vertex_handle> &_cv)
	  : p(pt), t(tr), cv(_cv) {}

      bool operator()(const Cell_handle c) const
      {
	  if (t->in_conflict_2(p, c, 3))
	  {
	      for (int i=0; i<3; i++)
	      {
		  Vertex_handle v = c->vertex(i);
		  if (v->cell() != Cell_handle())
		  {
		      cv.push_back(v);
		      v->set_cell(Cell_handle());
		  }
	      }
	      return true;
	  }
	  return false;
      }

      std::vector<Vertex_handle> & conflict_vector()
      {
	  return cv;
      }
  };

  friend class Conflict_tester_for_find_conflicts_3;
  friend class Conflict_tester_for_find_conflicts_2;
  friend class Conflict_tester_3;
  friend class Conflict_tester_2;
};


template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Vertex_handle
Regular_triangulation_3<Gt,Tds>::
nearest_power_vertex_in_cell(const Bare_point& p, 
			     const Cell_handle& c) const
// Returns the finite vertex of the cell c with smaller
// power distance  to p.
{
    CGAL_triangulation_precondition(dimension() >= 1);
    Vertex_handle nearest = nearest_power_vertex(p, 
						 c->vertex(0), 
						 c->vertex(1));
    if (dimension() >= 2) {
	nearest = nearest_power_vertex(p, nearest, c->vertex(2));
        if (dimension() == 3)
	    nearest = nearest_power_vertex(p, nearest, c->vertex(3));
    }
    return nearest;
}


template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Vertex_handle
Regular_triangulation_3<Gt,Tds>::
nearest_power_vertex(const Bare_point& p, Cell_handle start) const
{
    if (number_of_vertices() == 0)  
      return Vertex_handle();

    // Use a brute-force algorithm if dimension < 3.
    if (dimension() < 3) {
	Finite_vertices_iterator vit = finite_vertices_begin();
	Vertex_handle res = vit;
	for (++vit; vit != finite_vertices_end(); ++vit)
	    res = nearest_power_vertex(p, res, vit);
	return res;
    }

    Locate_type lt;
    int li, lj;
    // I put the cast here temporarily 
    // until we solve the traits class pb of regular triangulation
    Cell_handle c = locate(static_cast<Weighted_point>(p), lt, li, lj, start);
  
    // - start with the closest vertex from the located cell.
    // - repeatedly take the nearest of its incident vertices if any
    // - if not, we're done.
    Vertex_handle nearest = nearest_power_vertex_in_cell(p, c);
    std::vector<Vertex_handle> vs;
    vs.reserve(32);
    while (true) {
	Vertex_handle tmp = nearest;
        incident_vertices(nearest, std::back_inserter(vs));
        for (typename std::vector<Vertex_handle>::const_iterator
		vsit = vs.begin(); vsit != vs.end(); ++vsit)
	    tmp = nearest_power_vertex(p, tmp, *vsit);
	if (tmp == nearest)
	    break;
	vs.clear();
	nearest = tmp;
    }
    return nearest;
}

template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Bare_point
Regular_triangulation_3<Gt,Tds>::
dual(Cell_handle c) const
{
  CGAL_triangulation_precondition(dimension()==3);
  CGAL_triangulation_precondition( ! is_infinite(c) );
  return construct_weighted_circumcenter( c->vertex(0)->point(),
					  c->vertex(1)->point(),
					  c->vertex(2)->point(),
					  c->vertex(3)->point() );
}

template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Object
Regular_triangulation_3<Gt,Tds>::
dual(Cell_handle c, int i) const
{
  CGAL_triangulation_precondition(dimension()>=2);
  CGAL_triangulation_precondition( ! is_infinite(c,i) );

  if ( dimension() == 2 ) {
    CGAL_triangulation_precondition( i == 3 );
    return construct_object( 
       construct_weighted_circumcenter(c->vertex(0)->point(),
				       c->vertex(1)->point(),
				       c->vertex(2)->point()) );
  }

  // dimension() == 3
  Cell_handle n = c->neighbor(i);
  if ( ! is_infinite(c) && ! is_infinite(n) )
    return construct_object(construct_segment( dual(c), dual(n) ));

  // either n or c is infinite
  int in;
  if ( is_infinite(c) ) 
    in = n->index(c);
  else {
    n = c;
    in = i;
  }
  // n now denotes a finite cell, either c or c->neighbor(i)
  unsigned char ind[3] = {(in+1)&3,(in+2)&3,(in+3)&3};
  if ( (in&1) == 1 )
      std::swap(ind[0], ind[1]);
  const Weighted_point& p = n->vertex(ind[0])->point();
  const Weighted_point& q = n->vertex(ind[1])->point();
  const Weighted_point& r = n->vertex(ind[2])->point();
  
  Line l =
    construct_perpendicular_line( construct_plane(p,q,r),
				  construct_weighted_circumcenter(p,q,r) );
  return construct_object(construct_ray( dual(n), l));
}


template < class Gt, class Tds >
Oriented_side
Regular_triangulation_3<Gt,Tds>::
side_of_oriented_power_sphere(const Weighted_point &p0,
                              const Weighted_point &p1,
                              const Weighted_point &p2,
                              const Weighted_point &p3,
                              const Weighted_point &p, bool perturb) const
{
    CGAL_triangulation_precondition( orientation(p0, p1, p2, p3) == POSITIVE );

    Oriented_side os = power_test(p0, p1, p2, p3, p);

    if (os != ON_ORIENTED_BOUNDARY || !perturb)
        return os;

    // We are now in a degenerate case => we do a symbolic perturbation.

    // We sort the points lexicographically.
    const Weighted_point * points[5] = {&p0, &p1, &p2, &p3, &p};
    std::sort(points, points + 5,
              compare_to_less(compose(geom_traits().compare_xyz_3_object(),
				      Dereference<Weighted_point>(),
				      Dereference<Weighted_point>())));

    // We successively look whether the leading monomial, then 2nd monomial
    // of the determinant has non null coefficient.
    for (int i=4; i>1; --i) {
        if (points[i] == &p)
            return ON_NEGATIVE_SIDE; // since p0 p1 p2 p3 are non coplanar
                                     // and positively oriented
	Orientation o;
        if (points[i] == &p3 && (o = orientation(p0,p1,p2,p)) != COPLANAR )
            return Oriented_side(o);
        if (points[i] == &p2 && (o = orientation(p0,p1,p,p3)) != COPLANAR )
            return Oriented_side(o);
        if (points[i] == &p1 && (o = orientation(p0,p,p2,p3)) != COPLANAR )
            return Oriented_side(o);
        if (points[i] == &p0 && (o = orientation(p,p1,p2,p3)) != COPLANAR )
            return Oriented_side(o);
    }

    CGAL_triangulation_assertion(false);
    return ON_NEGATIVE_SIDE;
}


template < class Gt, class Tds >
Bounded_side
Regular_triangulation_3<Gt,Tds>::
side_of_power_sphere(Cell_handle c, const Weighted_point &p,
                     bool perturb) const
{
  CGAL_triangulation_precondition( dimension() == 3 );
  int i3;
  if ( ! c->has_vertex( infinite_vertex(), i3 ) ) {
    return Bounded_side( side_of_oriented_power_sphere(c->vertex(0)->point(),
                                                       c->vertex(1)->point(),
                                                       c->vertex(2)->point(),
                                                       c->vertex(3)->point(),
                                                       p, perturb) );
  }
  // else infinite cell :
  int i0,i1,i2;
  if ( (i3%2) == 1 ) {
    i0 = (i3+1)&3;
    i1 = (i3+2)&3;
    i2 = (i3+3)&3;
  }
  else {
    i0 = (i3+2)&3;
    i1 = (i3+1)&3;
    i2 = (i3+3)&3;
  }

  // general case
  Orientation o = orientation(c->vertex(i0)->point(),
		              c->vertex(i1)->point(),
		              c->vertex(i2)->point(), p);
  if (o != ZERO)
    return Bounded_side(o);

  // else p coplanar with i0,i1,i2
  return side_of_bounded_power_circle(c->vertex(i0)->point(),
                                      c->vertex(i1)->point(),
                                      c->vertex(i2)->point(),
                                      p, perturb);
}


template < class Gt, class Tds >
Bounded_side
Regular_triangulation_3<Gt,Tds>::
side_of_bounded_power_circle(const Weighted_point &p0,
                             const Weighted_point &p1,
                             const Weighted_point &p2,
                             const Weighted_point &p, bool perturb) const
{
    CGAL_triangulation_precondition(coplanar_orientation(p0, p1, p2) != 0);
    if (coplanar_orientation(p0, p1, p2) == POSITIVE)
        return Bounded_side (side_of_oriented_power_circle(p0, p1, p2, p, perturb));
    // Wrong because the low level power test already does a coplanar orientation
    // test.
    // return Bounded_side (- side_of_oriented_power_circle (p0, p2, p1, p,
    // perturb));
    return Bounded_side (side_of_oriented_power_circle(p0, p2, p1, p, perturb));
}


template < class Gt, class Tds >
Oriented_side
Regular_triangulation_3<Gt,Tds>::
side_of_oriented_power_circle(const Weighted_point &p0,
                              const Weighted_point &p1,
                              const Weighted_point &p2,
                              const Weighted_point &p, bool perturb) const
{
    CGAL_triangulation_precondition( coplanar_orientation(p0, p1, p2) == POSITIVE );

    Oriented_side os = power_test(p0, p1, p2, p);

    if (os != ON_ORIENTED_BOUNDARY || !perturb)
        return os;

    // We are now in a degenerate case => we do a symbolic perturbation.

    // We sort the points lexicographically.
    const Weighted_point * points[4] = {&p0, &p1, &p2, &p};
    std::sort(points, points + 4,
              compare_to_less(compose(geom_traits().compare_xyz_3_object(),
				      Dereference<Weighted_point>(),
				      Dereference<Weighted_point>())));

    // We successively look whether the leading monomial, then 2nd monomial
    // of the determinant has non null coefficient.
    // 2 iterations are enough (cf paper)
    for (int i=3; i>1; --i) {
        if (points[i] == &p)
            return ON_NEGATIVE_SIDE; // since p0 p1 p2 are non collinear
                                     // and positively oriented
	Orientation o;
        if (points[i] == &p2 && (o = coplanar_orientation(p0,p1,p)) != COPLANAR )
            return Oriented_side(o);
        if (points[i] == &p1 && (o = coplanar_orientation(p0,p,p2)) != COPLANAR )
            return Oriented_side(o);
        if (points[i] == &p0 && (o = coplanar_orientation(p,p1,p2)) != COPLANAR )
            return Oriented_side(o);
    }

    CGAL_triangulation_assertion(false);
    return ON_NEGATIVE_SIDE;
}


template < class Gt, class Tds >
Bounded_side
Regular_triangulation_3<Gt,Tds>::
side_of_power_circle(Cell_handle c, int i, const Weighted_point &p,
		     bool perturb) const
{
  CGAL_triangulation_precondition( dimension() >= 2 );
  int i3 = 5;
  if ( dimension() == 2 ) {
    CGAL_triangulation_precondition( i == 3 );
    // the triangulation is supposed to be valid, ie the facet
    // with vertices 0 1 2 in this order is positively oriented
    if ( ! c->has_vertex( infinite_vertex(), i3 ) )
      return Bounded_side( side_of_oriented_power_circle(c->vertex(0)->point(),
                                                         c->vertex(1)->point(),
                                                         c->vertex(2)->point(),
                                                         p, perturb) );
    // else infinite facet
    // v1, v2 finite vertices of the facet such that v1,v2,infinite
    // is positively oriented
    Vertex_handle v1 = c->vertex( ccw(i3) ),
                  v2 = c->vertex( cw(i3) );
    CGAL_triangulation_assertion(coplanar_orientation(v1->point(), v2->point(),
                                 mirror_vertex(c, i3)->point()) == NEGATIVE);
    Orientation o = coplanar_orientation(v1->point(), v2->point(), p);
    if ( o != ZERO )
	return Bounded_side( o );
    // case when p collinear with v1v2
    return side_of_bounded_power_segment(v1->point(),
                                         v2->point(),
                                         p, perturb);
  }// dim 2

  // else dimension == 3
  CGAL_triangulation_precondition( (i >= 0) && (i < 4) );
  if ( ( ! c->has_vertex(infinite_vertex(),i3) ) || ( i3 != i ) ) {
    // finite facet
    // initialization of i0 i1 i2, vertices of the facet positively
    // oriented (if the triangulation is valid)
    int i0 = (i>0) ? 0 : 1;
    int i1 = (i>1) ? 1 : 2;
    int i2 = (i>2) ? 2 : 3;
    CGAL_triangulation_precondition(this->coplanar(c->vertex(i0)->point(),
						   c->vertex(i1)->point(),
						   c->vertex(i2)->point(), p));
    return side_of_bounded_power_circle(c->vertex(i0)->point(),
                                        c->vertex(i1)->point(),
                                        c->vertex(i2)->point(),
                                        p, perturb);
  }
  //else infinite facet
  // v1, v2 finite vertices of the facet such that v1,v2,infinite
  // is positively oriented
  Vertex_handle v1 = c->vertex( next_around_edge(i3,i) ),
                v2 = c->vertex( next_around_edge(i,i3) );
  Orientation o = (Orientation)
                  (coplanar_orientation( v1->point(), v2->point(),
					c->vertex(i)->point()) *
                  coplanar_orientation( v1->point(), v2->point(), p));
  // then the code is duplicated from 2d case
  if ( o != ZERO )
      return Bounded_side( -o );
  // because p is in f iff
  // it is not on the same side of v1v2 as c->vertex(i)
  // case when p collinear with v1v2 :
  return side_of_bounded_power_segment(v1->point(),
                                       v2->point(),
                                       p, perturb);
}

template < class Gt, class Tds >
Bounded_side
Regular_triangulation_3<Gt,Tds>::
side_of_bounded_power_segment(const Weighted_point &p0,
                              const Weighted_point &p1,
                              const Weighted_point &p, bool perturb) const
{
    Oriented_side os = power_test(p0, p1, p);

    if (os != ON_ORIENTED_BOUNDARY || !perturb)
        return Bounded_side(os);

    // We are now in a degenerate case => we do a symbolic perturbation.

    switch (this->collinear_position(p0, p, p1)) {
        case Tr_Base::BEFORE: case Tr_Base::AFTER: 
            return ON_UNBOUNDED_SIDE;
        case Tr_Base::MIDDLE:
            return ON_BOUNDED_SIDE;
        default:
            ;
    }

    CGAL_triangulation_assertion(false);
    return ON_UNBOUNDED_SIDE;
}

template < class Gt, class Tds >
Bounded_side
Regular_triangulation_3<Gt,Tds>::
side_of_power_segment(Cell_handle c, const Weighted_point &p,
                      bool perturb) const
{
  CGAL_triangulation_precondition( dimension() == 1 );
  if ( ! is_infinite(c,0,1) )
    return side_of_bounded_power_segment(c->vertex(0)->point(),
                                         c->vertex(1)->point(),
                                         p, perturb);
  Locate_type lt; int i;
  Bounded_side soe = side_of_edge( p, c, lt, i );
  if (soe != ON_BOUNDARY)
    return soe;
  // Either we compare weights, or we use the finite neighboring edge
  Cell_handle finite_neighbor = c->neighbor(c->index(infinite_vertex()));
  CGAL_triangulation_assertion(!is_infinite(finite_neighbor,0,1));
  return side_of_bounded_power_segment(finite_neighbor->vertex(0)->point(),
                                       finite_neighbor->vertex(1)->point(),
                                       p, perturb);
}

template < class Gt, class Tds >
bool
Regular_triangulation_3<Gt,Tds>::
is_Gabriel(const Facet& f) const
{
  return is_Gabriel(f.first, f.second);
}

template < class Gt, class Tds >
bool
Regular_triangulation_3<Gt,Tds>::
is_Gabriel(Cell_handle c, int i) const
{
  CGAL_triangulation_precondition(dimension() == 3 && !is_infinite(c,i));
  typename Geom_traits::Side_of_bounded_orthogonal_sphere_3
    side_of_bounded_orthogonal_sphere = 
    geom_traits().side_of_bounded_orthogonal_sphere_3_object();

  if ((!is_infinite(c->vertex(i))) &&
      side_of_bounded_orthogonal_sphere(
	 c->vertex(vertex_triple_index(i,0))->point(),
	 c->vertex(vertex_triple_index(i,1))->point(),
	 c->vertex(vertex_triple_index(i,2))->point(),
	 c->vertex(i)->point()) == ON_BOUNDED_SIDE ) return false;

  Cell_handle neighbor = c->neighbor(i);
  int in = neighbor->index(c);

  if ((!is_infinite(neighbor->vertex(in))) &&
      side_of_bounded_orthogonal_sphere(
	 c->vertex(vertex_triple_index(i,0))->point(),
	 c->vertex(vertex_triple_index(i,1))->point(),
	 c->vertex(vertex_triple_index(i,2))->point(),	
	 neighbor->vertex(in)->point()) == ON_BOUNDED_SIDE ) return false;
 
  return true;
}

  
template < class Gt, class Tds >
bool
Regular_triangulation_3<Gt,Tds>::
is_Gabriel(const Edge& e) const
{
  return is_Gabriel(e.first, e.second, e.third);
}

template < class Gt, class Tds >
bool
Regular_triangulation_3<Gt,Tds>::
is_Gabriel(Cell_handle c, int i, int j) const
{
  CGAL_triangulation_precondition(dimension() == 3 && !is_infinite(c,i,j));
  typename Geom_traits::Side_of_bounded_orthogonal_sphere_3
    side_of_bounded_orthogonal_sphere = 
    geom_traits().side_of_bounded_orthogonal_sphere_3_object();

  Facet_circulator fcirc = incident_facets(c,i,j),
                   fdone(fcirc);
  Vertex_handle v1 = c->vertex(i);
  Vertex_handle v2 = c->vertex(j);
  do {
      // test whether the vertex of cc opposite to *fcirc
      // is inside the sphere defined by the edge e = (s, i,j)
      Cell_handle cc = (*fcirc).first;
      int ii = (*fcirc).second;
      if (!is_infinite(cc->vertex(ii)) &&
	  side_of_bounded_orthogonal_sphere( v1->point(), 
					 v2->point(),
					 cc->vertex(ii)->point())  
	  == ON_BOUNDED_SIDE ) return false;
  } while(++fcirc != fdone);
  return true;
}

template < class Gt, class Tds >
bool
Regular_triangulation_3<Gt,Tds>::
is_Gabriel(Vertex_handle v) const
{
  return nearest_power_vertex( v->point().point(), v->cell()) == v;
}

template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Vertex_handle
Regular_triangulation_3<Gt,Tds>::
insert(const Weighted_point & p, Cell_handle start)
{
    Locate_type lt;
    int li, lj;
    Cell_handle c = locate(p, lt, li, lj, start);
    return insert(p, lt, c, li, lj);
}

template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Vertex_handle
Regular_triangulation_3<Gt,Tds>::
insert(const Weighted_point & p, Locate_type lt, Cell_handle c, int li, int)
{
    if (lt == Tr_Base::OUTSIDE_AFFINE_HULL)
        return Tr_Base::insert_outside_affine_hull (p);

    Vertex_handle v;

    // Previously, when an already present point was inserted a second time, it
    // was discarded and its Vertex_handle was returned, we keep this semantic.
    // We need the special case lt == VERTEX because we don't know how the
    // power_test will be perturbed in dimension > 0.
    if (lt == Tr_Base::VERTEX && power_test (c->vertex(li)->point(), p) == 0)
        goto replace_vertex;

    // If the new point is not in conflict with its cell, it is hidden.
    if (! in_conflict (p, c)) {
        c->hide_point (p);
        return Vertex_handle();
    }

    if (dimension() == 0) {
replace_vertex:
        v = c->vertex(li);
        c->hide_point(v->point());
        v->set_point(p); 
        return v;
    }

    // Ok, we really insert the point now.
    // First, find the conflict region.
    std::vector<Cell_handle> cells;
    std::vector<Vertex_handle> vertices;
    Facet facet;
    Cell_handle bound[2];
    // corresponding index: bound[j]->neighbor(1-j) is in conflict.

    switch (dimension()) {
        case 1:
            // We get all cells in conflict,
            // and remember the 2 external boundaries.
            cells.push_back(c);

            for (int j = 0; j<2; ++j) {
                Cell_handle n = c->neighbor(j);
                while ( in_conflict_1( p, n) ) {
                    cells.push_back(n);
                    vertices.push_back(n->vertex(j));
                    n = n->neighbor(j);
                }
                bound[j] = n;
            }
            break;

        case 2:
        {
            cells.reserve(32);
            Conflict_tester_2 tester (p, this, vertices);
            find_conflicts_2 (c, tester, make_triple(Oneset_iterator<Facet>(facet),
                                                     std::back_inserter(cells),
                                                     Emptyset_iterator()));
            break;
        }
        case 3:
        {
            cells.reserve(32);
            Conflict_tester_3 tester (p, this, vertices);
            find_conflicts_3 (c, tester, make_triple(Oneset_iterator<Facet>(facet),
                                                     std::back_inserter(cells),
                                                     Emptyset_iterator()));
            break;
        }
        default:
            CGAL_triangulation_assertion (false);
    }

    // Remember the points that are hidden by the conflicting cells,
    // as they will be deleted during the insertion.

    std::vector<Weighted_point> hidden;

    for (typename std::vector<Cell_handle>::iterator
         ci = cells.begin(); ci != cells.end(); ++ci)
        std::copy ((*ci)->hidden_points_begin(),
                   (*ci)->hidden_points_end(),
                   std::back_inserter(hidden));

    // Insertion.

    if (dimension() == 1) {
        tds().delete_cells(cells.begin(), cells.end());

        for (typename std::vector<Vertex_handle>::iterator
             vi = vertices.begin(); vi != vertices.end(); ++vi)
        {
            hidden.push_back ((*vi)->point());
            tds().delete_vertex(*vi);
        }

        // We preserve the order (like the orientation in 2D-3D).
        v = tds().create_vertex();
        Cell_handle c0 = tds().create_face(v, bound[0]->vertex(0), Vertex_handle());
        Cell_handle c1 = tds().create_face(bound[1]->vertex(1), v, Vertex_handle());
        tds().set_adjacency(c0, 1, c1, 0);
        tds().set_adjacency(bound[0], 1, c0, 0);
        tds().set_adjacency(c1, 1, bound[1], 0);
        bound[0]->vertex(0)->set_cell(bound[0]);
        bound[1]->vertex(1)->set_cell(bound[1]);
        v->set_cell(c0);

    } else { // Same code for dim 2 and 3.
        v = tds()._insert_in_hole(cells.begin(), cells.end(),
                                  facet.first, facet.second);
        for (typename std::vector<Vertex_handle>::iterator
             vi = vertices.begin(); vi != vertices.end(); ++vi)
        {
            if ((*vi)->cell() != Cell_handle()) continue;
            hidden.push_back ((*vi)->point());
            tds().delete_vertex (*vi);
        }
    }

    v->set_point (p);

    // Store the hidden points in their new cells.

    for (typename std::vector<Weighted_point>::iterator
         hp = hidden.begin(); hp != hidden.end(); ++hp)
    {
        Cell_handle hc = locate (*hp, v->cell());
        hc->hide_point (*hp);
    }

    return v;
}

template < class Gt, class Tds >
template < class OutputIterator >
OutputIterator
Regular_triangulation_3<Gt,Tds>::
remove_dim_down(Vertex_handle v, OutputIterator hidden)
{
    CGAL_triangulation_precondition (dimension() >= 0);

    // Collect all the hidden points.
    for (All_cells_iterator ci = tds().raw_cells_begin();
            ci != tds().raw_cells_end(); ++ci)
        std::copy (ci->hidden_points_begin(), ci->hidden_points_end(), hidden);

    tds().remove_decrease_dimension(v, infinite_vertex());

    // Now try to see if we need to re-orient.
    if (dimension() == 2) {
        Facet f = *finite_facets_begin();
        if (coplanar_orientation(f.first->vertex(0)->point(),
                                 f.first->vertex(1)->point(),
                                 f.first->vertex(2)->point()) == NEGATIVE)
            tds().reorient();
    }

    return hidden;
}

template < class Gt, class Tds >
template < class OutputIterator >
OutputIterator
Regular_triangulation_3<Gt,Tds>::
remove_1D(Vertex_handle v, OutputIterator hidden)
{
    CGAL_triangulation_precondition (dimension() == 1);

    Cell_handle c1 = v->cell();
    Cell_handle c2 = c1->neighbor(c1->index(v) == 0 ? 1 : 0);
    std::copy (c1->hidden_points_begin(), c1->hidden_points_end(), hidden);
    std::copy (c2->hidden_points_begin(), c2->hidden_points_end(), hidden);

    tds().remove_from_maximal_dimension_simplex (v);

    return hidden;
}

// The following functions (fill_hole_regular_2D, make_hole_2D, make_canonical,
// make_vertex_triple, make_hole_3D, remove_3D) are an almost verbatim copy of
// their counterpart in Delaunay_triangulation_3. In a perfect world most of
// this code would be in Triangulation_3 and Triangulation_data_structure_3.
template <class Gt, class Tds >
void
Regular_triangulation_3<Gt, Tds>::
fill_hole_regular_2D(std::list<Edge_2D> & first_hole)
{
    typedef std::list<Edge_2D> Hole;

    std::vector<Hole> hole_list;

    Cell_handle  f, ff, fn;
    int i, ii, in;

    hole_list.push_back(first_hole);

    while( ! hole_list.empty()) {
        Hole hole = hole_list.back();
        hole_list.pop_back();

        // if the hole has only three edges, create the triangle
        if (hole.size() == 3) {
            typename Hole::iterator hit = hole.begin();
            f = (*hit).first;        i = (*hit).second;
            ff = (* ++hit).first;    ii = (*hit).second;
            fn = (* ++hit).first;    in = (*hit).second;
            tds().create_face(f, i, ff, ii, fn, in);
            continue;
        }

        // else find an edge with two finite vertices
        // on the hole boundary
        // and the new triangle adjacent to that edge
        //  cut the hole and push it back

        // first, ensure that a neighboring face
        // whose vertices on the hole boundary are finite
        // is the first of the hole
        while (1) {
            ff = (hole.front()).first;
            ii = (hole.front()).second;
            if ( is_infinite(ff->vertex(cw(ii))) ||
                    is_infinite(ff->vertex(ccw(ii)))) {
                hole.push_back(hole.front());
                hole.pop_front();
            }
            else
                break;
        }

        // take the first neighboring face and pop it;
        ff = (hole.front()).first;
        ii = (hole.front()).second;
        hole.pop_front();

        Vertex_handle v0 = ff->vertex(cw(ii));
        Vertex_handle v1 = ff->vertex(ccw(ii));
        Vertex_handle v2 = infinite_vertex();
        const Weighted_point &p0 = v0->point();
        const Weighted_point &p1 = v1->point();
        const Weighted_point *p2 = NULL; // Initialize to NULL to avoid warning.

        typename Hole::iterator hdone = hole.end();
        typename Hole::iterator hit = hole.begin();
        typename Hole::iterator cut_after(hit);

        // if tested vertex is c with respect to the vertex opposite
        // to NULL neighbor,
        // stop at the before last face;
        hdone--;
        for (; hit != hdone; ++hit) {
            fn = hit->first;
            in = hit->second;
            Vertex_handle vv = fn->vertex(ccw(in));
            if (is_infinite(vv)) {
                if (is_infinite(v2))
                    cut_after = hit;
            }
            else {     // vv is a finite vertex
                const Weighted_point &p = vv->point();
                if (coplanar_orientation(p0, p1, p) == COUNTERCLOCKWISE) {
                    if (is_infinite(v2) ||
                            side_of_bounded_power_circle(p0, p1, *p2, p, true)
                            == ON_BOUNDED_SIDE) {
                        v2 = vv;
                        p2 = &p;
                        cut_after = hit;
                    }
                }
            }
        }

        // create new triangle and update adjacency relations
        Cell_handle newf;

        //update the hole and push back in the Hole_List stack
        // if v2 belongs to the neighbor following or preceding *f
        // the hole remain a single hole
        // otherwise it is split in two holes

        fn = (hole.front()).first;
        in = (hole.front()).second;
        if (fn->has_vertex(v2, i) && i == ccw(in)) {
            newf = tds().create_face(ff, ii, fn, in);
            hole.pop_front();
            hole.push_front(Edge_2D(newf, 1));
            hole_list.push_back(hole);
        } else {
            fn = (hole.back()).first;
            in = (hole.back()).second;
            if (fn->has_vertex(v2, i) && i == cw(in)) {
                newf = tds().create_face(fn, in, ff, ii);
                hole.pop_back();
                hole.push_back(Edge_2D(newf, 1));
                hole_list.push_back(hole);
            }
            else{
                // split the hole in two holes
                newf = tds().create_face(ff, ii, v2);
                Hole new_hole;
                ++cut_after;
                while( hole.begin() != cut_after )
                {
                    new_hole.push_back(hole.front());
                    hole.pop_front();
                }

                hole.push_front(Edge_2D(newf, 1));
                new_hole.push_front(Edge_2D(newf, 0));
                hole_list.push_back(hole);
                hole_list.push_back(new_hole);
            }
        }
    }
}

template < class Gt, class Tds >
template < class OutputIterator >
OutputIterator
Regular_triangulation_3<Gt, Tds>::
make_hole_2D(Vertex_handle v, std::list<Edge_2D> & hole, OutputIterator hidden)
{
    std::vector<Cell_handle> to_delete;

    typename Tds::Face_circulator fc = tds().incident_faces(v);
    typename Tds::Face_circulator done(fc);

    // We prepare for deleting all interior cells.
    // We ->set_cell() pointers to cells outside the hole.
    // We push the Edges_2D of the boundary (seen from outside) in "hole".
    do {
        Cell_handle f = fc;
        int i = f->index(v);
        Cell_handle fn = f->neighbor(i);
        int in = fn->index(f);

        f->vertex(cw(i))->set_cell(fn);
        fn->set_neighbor(in, Cell_handle());

        hole.push_back(Edge_2D(fn, in));
        std::copy (f->hidden_points_begin(), f->hidden_points_end(), hidden);
        to_delete.push_back(f);

        ++fc;
    } while (fc != done);

    tds().delete_cells(to_delete.begin(), to_delete.end());
    return hidden;
}

template < class Gt, class Tds >
template < class OutputIterator >
OutputIterator
Regular_triangulation_3<Gt,Tds>::
remove_2D(Vertex_handle v, OutputIterator hidden)
{
    CGAL_triangulation_precondition(dimension() == 2);
    std::list<Edge_2D> hole;
    make_hole_2D(v, hole, hidden);
    fill_hole_regular_2D(hole);
    tds().delete_vertex(v);
    return hidden;
}

#ifndef CGAL_CFG_NET2003_MATCHING_BUG
template < class Gt, class Tds >
void
Regular_triangulation_3<Gt,Tds>::
make_hole_3D (Vertex_handle v, 
	      std::map<Vertex_triple,Facet>& outer_map,
	      std::vector<Cell_handle> & hole)
{
  CGAL_triangulation_expensive_precondition( ! test_dim_down(v) );

  incident_cells(v, std::back_inserter(hole));

  for (typename std::vector<Cell_handle>::iterator cit = hole.begin();
       cit != hole.end(); ++cit) {
    int indv = (*cit)->index(v);
    Cell_handle opp_cit = (*cit)->neighbor( indv );
    Facet f(opp_cit, opp_cit->index(*cit)); 
    Vertex_triple vt = make_vertex_triple(f);
    make_canonical(vt);
    outer_map[vt] = f;
    for (int i=0; i<4; i++)
      if ( i != indv )
	(*cit)->vertex(i)->set_cell(opp_cit);
  }
}
#endif

template < class Gt, class Tds >
void
Regular_triangulation_3<Gt,Tds>::
make_canonical(Vertex_triple& t) const
{
  int i = (&*(t.first) < &*(t.second))? 0 : 1;
  if(i==0) {
    i = (&*(t.first) < &*(t.third))? 0 : 2;
  } else {
    i = (&*(t.second) < &*(t.third))? 1 : 2;
  }
  Vertex_handle tmp; 
  switch(i){
  case 0: return;
  case 1:
    tmp = t.first;
    t.first = t.second;
    t.second = t.third;
    t.third = tmp;
    return;
  default:
    tmp = t.first;
    t.first = t.third;
    t.third = t.second;
    t.second = tmp;
  }
}

template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Vertex_triple
Regular_triangulation_3<Gt,Tds>::
make_vertex_triple(const Facet& f) const
{
  // static const int vertex_triple_index[4][3] = { {1, 3, 2}, {0, 2, 3},
//                                                  {0, 3, 1}, {0, 1, 2} };
  Cell_handle ch = f.first;
  int i = f.second;
  
  return Vertex_triple(ch->vertex(vertex_triple_index(i,0)),
		       ch->vertex(vertex_triple_index(i,1)),
		       ch->vertex(vertex_triple_index(i,2))); 
}

template < class Gt, class Tds >
template < class OutputIterator >
OutputIterator
Regular_triangulation_3<Gt,Tds>::
remove_3D(Vertex_handle v, OutputIterator hidden)
{
  std::vector<Cell_handle> hole;
  hole.reserve(64);

  // Construct the set of vertex triples on the boundary
  // with the facet just behind
  typedef std::map<Vertex_triple,Facet> Vertex_triple_Facet_map;
  Vertex_triple_Facet_map outer_map;
  Vertex_triple_Facet_map inner_map;

  make_hole_3D (v, outer_map, hole);
  
  // Output the hidden points.
  for (typename std::vector<Cell_handle>::iterator
       hi = hole.begin(), hend = hole.end(); hi != hend; ++hi) 
  {
    std::copy ((*hi)->hidden_points_begin(), (*hi)->hidden_points_end(), hidden);
  }


  bool inf = false;
  unsigned int i;
  // collect all vertices on the boundary
  std::vector<Vertex_handle> vertices;
  vertices.reserve(64);

  incident_vertices(v, std::back_inserter(vertices));
  
  // create a Regular triangulation of the points on the boundary
  // and make a map from the vertices in aux towards the vertices in *this
  Self aux;

  Unique_hash_map<Vertex_handle,Vertex_handle> vmap;

  Cell_handle ch = Cell_handle();
  for(i=0; i < vertices.size(); i++){
    if(! is_infinite(vertices[i])){
      Vertex_handle vh = aux.insert(vertices[i]->point(), ch);
      ch = vh->cell();
      vmap[vh] = vertices[i];
    }else {
      inf = true;
    }
  }

  if(aux.dimension()==2){
    Vertex_handle fake_inf = aux.insert(v->point());
    vmap[fake_inf] = infinite_vertex();
  } else {
    vmap[aux.infinite_vertex()] = infinite_vertex();
  }

  CGAL_triangulation_assertion(aux.dimension() == 3);

  // Construct the set of vertex triples of aux
  // We reorient the vertex triple so that it matches those from outer_map
  // Also note that we use the vertices of *this, not of aux
  
  if(inf){
    for(All_cells_iterator it = aux.all_cells_begin();
	it != aux.all_cells_end();
	++it){
      for(i=0; i < 4; i++){
	Facet f = std::pair<Cell_handle,int>(it,i);
	Vertex_triple vt_aux = make_vertex_triple(f);
	Vertex_triple vt(vmap[vt_aux.first],vmap[vt_aux.third],vmap[vt_aux.second]);
	make_canonical(vt);
	inner_map[vt]= f;
      }
    }
  } else {
      for(Finite_cells_iterator it = aux.finite_cells_begin();
	it != aux.finite_cells_end();
	++it){
      for(i=0; i < 4; i++){
	Facet f = std::pair<Cell_handle,int>(it,i);
	Vertex_triple vt_aux = make_vertex_triple(f);
	Vertex_triple vt(vmap[vt_aux.first],vmap[vt_aux.third],vmap[vt_aux.second]);
	make_canonical(vt);
	inner_map[vt]= f;
      }
    }
  }
  // Grow inside the hole, by extending the surface
  while(! outer_map.empty()){
    typename Vertex_triple_Facet_map::iterator oit = outer_map.begin();
    while(is_infinite(oit->first.first) ||
	  is_infinite(oit->first.second) ||
	  is_infinite(oit->first.third)){
      ++oit;
      // otherwise the lookup in the inner_map fails
      // because the infinite vertices are different
    }
    typename Vertex_triple_Facet_map::value_type o_vt_f_pair = *oit;
    Cell_handle o_ch = o_vt_f_pair.second.first;
    unsigned int o_i = o_vt_f_pair.second.second;

    typename Vertex_triple_Facet_map::iterator iit =
             inner_map.find(o_vt_f_pair.first);
    CGAL_triangulation_assertion(iit != inner_map.end());
    typename Vertex_triple_Facet_map::value_type i_vt_f_pair = *iit;
    Cell_handle i_ch = i_vt_f_pair.second.first;
    unsigned int i_i = i_vt_f_pair.second.second;
    
    // create a new cell and glue it to the outer surface
    Cell_handle new_ch = tds().create_cell();
    new_ch->set_vertices(vmap[i_ch->vertex(0)], vmap[i_ch->vertex(1)],
			 vmap[i_ch->vertex(2)], vmap[i_ch->vertex(3)]);
    
    o_ch->set_neighbor(o_i,new_ch);
    new_ch->set_neighbor(i_i, o_ch);

    // for the other faces check, if they can also be glued
    for(i = 0; i < 4; i++){
      if(i != i_i){
	Facet f = std::pair<Cell_handle,int>(new_ch,i);
	Vertex_triple vt = make_vertex_triple(f);
	make_canonical(vt);
	std::swap(vt.second,vt.third);
	typename Vertex_triple_Facet_map::iterator oit2 = outer_map.find(vt);
	if(oit2 == outer_map.end()){
	  std::swap(vt.second,vt.third);
	  outer_map[vt]= f;
	} else {
	  // glue the faces
	  typename Vertex_triple_Facet_map::value_type o_vt_f_pair2 = *oit2;
	  Cell_handle o_ch2 = o_vt_f_pair2.second.first;
	  int o_i2 = o_vt_f_pair2.second.second;
	  o_ch2->set_neighbor(o_i2,new_ch);
	  new_ch->set_neighbor(i, o_ch2);
	  outer_map.erase(oit2);
	}
      }
    }
    outer_map.erase(oit);
  }

  tds().delete_vertex(v);
  tds().delete_cells(hole.begin(), hole.end());

  return hidden;
}

template < class Gt, class Tds >
void
Regular_triangulation_3<Gt,Tds>::
remove(Vertex_handle v)
{
    CGAL_triangulation_precondition( v != Vertex_handle());
    CGAL_triangulation_precondition( !is_infinite(v));
    CGAL_triangulation_expensive_precondition( tds().is_vertex(v) );

    // The removal of v may un-hide some points,
    // remove_*D() functions output them.
    std::vector<Weighted_point> hidden;

    Cell_handle c;
    if (dimension() > 0)
        c = v->cell()->neighbor(v->cell()->index(v));

    if (test_dim_down (v)) remove_dim_down (v, std::back_inserter(hidden));
    else switch (dimension())
    {
    case 1: remove_1D (v, std::back_inserter(hidden)); break;
    case 2: remove_2D (v, std::back_inserter(hidden)); break;
    case 3: remove_3D (v, std::back_inserter(hidden)); break;
    default:
        CGAL_triangulation_assertion (false);
    }

    // Re-insert the points that v was hiding.
    for (typename std::vector<Weighted_point>::iterator
         hi = hidden.begin(); hi != hidden.end(); ++hi)
    {
        Vertex_handle hv = insert (*hi, c);
        if (hv != Vertex_handle()) c = hv->cell();
    }

    CGAL_triangulation_expensive_postcondition (is_valid());
}

// Again, verbatim copy from Delaunay.
template < class Gt, class Tds >
typename Regular_triangulation_3<Gt,Tds>::Vertex_handle
Regular_triangulation_3<Gt,Tds>::
move_point(Vertex_handle v, const Weighted_point & p)
{
    CGAL_triangulation_precondition(! is_infinite(v));
    CGAL_triangulation_expensive_precondition(is_vertex(v));
 
    // Dummy implementation for a start.
 
    // Remember an incident vertex to restart
    // the point location after the removal.
    Cell_handle c = v->cell();
    Vertex_handle old_neighbor = c->vertex(c->index(v) == 0 ? 1 : 0);
    CGAL_triangulation_assertion(old_neighbor != v);
 
    remove(v);
 
    if (dimension() <= 0)
        return insert(p);
    return insert(p, old_neighbor->cell());
}

template < class Gt, class Tds >
bool
Regular_triangulation_3<Gt,Tds>::
is_valid(bool verbose, int level) const
{
  if ( ! Tr_Base::is_valid(verbose,level) ) {
    if (verbose)
	std::cerr << "invalid base triangulation" << std::endl;
    CGAL_triangulation_assertion(false);
    return false;
  }

  switch ( dimension() ) {
  case 3:
    {
      Finite_cells_iterator it;
      for ( it = finite_cells_begin(); it != finite_cells_end(); ++it ) {
	is_valid_finite(it, verbose, level);
	for (int i=0; i<4; i++ ) {
	  if ( !is_infinite
	       (it->neighbor(i)->vertex(it->neighbor(i)->index(it))) ) {
	    if ( side_of_power_sphere
		 (it,
		  it->neighbor(i)->vertex(it->neighbor(i)->index(it))->point())
		  == ON_BOUNDED_SIDE ) {
	      if (verbose)
		std::cerr << "non-empty sphere " << std::endl;
	      CGAL_triangulation_assertion(false);
	      return false;
	    }
	  }
	}
      }
      break;
    }
  case 2:
    {
      Finite_facets_iterator it;
      for ( it = finite_facets_begin(); it != finite_facets_end(); ++it ) {
	is_valid_finite((*it).first, verbose, level);
	for (int i=0; i<3; i++ ) {
	  if( !is_infinite
	      ((*it).first->neighbor(i)->vertex( (((*it).first)->neighbor(i))
						 ->index((*it).first))) ) {
	    if ( side_of_power_circle
		 ( (*it).first, 3,
		   (*it).first->neighbor(i)->
		   vertex( (((*it).first)->neighbor(i))
			   ->index((*it).first) )->point() )
		 == ON_BOUNDED_SIDE ) {
	      if (verbose)
		std::cerr << "non-empty circle " << std::endl;
	      CGAL_triangulation_assertion(false);
	      return false;
	    }
	  }
	}
      }
      break;
    }
  case 1:
    {
      Finite_edges_iterator it;
      for ( it = finite_edges_begin(); it != finite_edges_end(); ++it ) {
	is_valid_finite((*it).first, verbose, level);
	for (int i=0; i<2; i++ ) {
	  if( !is_infinite
	      ((*it).first->neighbor(i)->vertex( (((*it).first)->neighbor(i))
						 ->index((*it).first))) ) {
	    if ( side_of_power_segment
		 ( (*it).first,
		   (*it).first->neighbor(i)->
		   vertex( (((*it).first)->neighbor(i))
			   ->index((*it).first) )->point() )
		 == ON_BOUNDED_SIDE ) {
	      if (verbose)
		std::cerr << "non-empty edge " << std::endl;
	      CGAL_triangulation_assertion(false);
	      return false;
	    }
	  }
	}
      }
      break;
    }
  }
  if (verbose)
      std::cerr << "valid Regular triangulation" << std::endl;
  return true;
}

CGAL_END_NAMESPACE

#endif // CGAL_REGULAR_TRIANGULATION_3_H
