// Copyright (c) 2003-2006  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Surface_mesher/include/CGAL/Surface_mesher/Surface_mesher.h $
// $Id: Surface_mesher.h 30667 2006-04-19 16:56:12Z glisse $
//
//
// Author(s) : Steve Oudot,
//             David Rey,
//             Mariette Yvinec,
//             Laurent Rineau,
//             Andreas Fabri



#ifndef CGAL_SURFACE_MESHER_SURFACE_MESHER_H
#define CGAL_SURFACE_MESHER_SURFACE_MESHER_H

#include <CGAL/Mesher_level.h>
#include <CGAL/Mesh_2/Triangulation_mesher_level_traits_3.h>
#include <CGAL/Double_map.h>
#include <list>
#include <string>
#include <sstream>

namespace CGAL {

  namespace Surface_mesher {

  // NB: by convention, the priority queue is sorted with respect to the
  // first element of the list of criteria

  template <
    class C2T3,
    class Surface_,
    class SurfaceMeshTraits,
    class Criteria_
    >
  class Surface_mesher_base
    : public Triangulation_mesher_level_traits_3<typename C2T3::Triangulation>
  {
  public:
    typedef C2T3 Complex_2_in_triangulation_3;
    typedef Surface_ Surface;
    typedef SurfaceMeshTraits Surface_mesh_traits;
    typedef Criteria_ Criteria;

    typedef typename C2T3::Triangulation Tr;
    typedef typename Tr::Point Point;
    typedef typename Tr::Edge Edge;
    typedef typename Tr::Vertex_handle Vertex_handle;
    typedef typename Tr::Cell_handle Cell_handle;

    typedef typename Tr::Geom_traits GT;

    typedef Triangulation_mesher_level_traits_3<Tr> Triangulation_traits;
    typedef typename Triangulation_traits::Zone Zone;

    typedef typename Tr::Finite_facets_iterator Finite_facets_iterator;
    typedef typename Tr::Facet Facet;

    typedef typename Criteria::Quality Quality;

    typedef Double_map<Facet, Quality> Bad_facets;

    // Constructor
    Surface_mesher_base (C2T3& co, 
                         Surface& s, 
                         Surface_mesh_traits mesh_traits,
                         Criteria& c) :
      Triangulation_mesher_level_traits_3<Tr>(co.triangulation()),
      c2t3(co),
      tr(co.triangulation()),
      surf(s),
      meshtraits(mesh_traits),
      criteria(c)
    {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CONSTRUCTORS
      std::cerr << "CONS: Surface_mesher_base\n";
#endif
    }

  protected:
    C2T3& c2t3;
    Tr& tr;     // Associated triangulation reference
    Surface& surf;  // Surface
    Surface_mesh_traits meshtraits; // Surface mesh traits
    Criteria& criteria;  // Meshing criteria
    Bad_facets facets_to_refine;  // Set of facets to refine

  public:

    // Helper functions
    Facet mirror_facet(const Facet& f) const
    {
      return tr.mirror_facet(f);
    }

    static void set_facet_visited(Facet f)
    {
      f.first->set_facet_visited(f.second);
    }

    static void set_facet_surface_center(Facet f, const Point& p)
    {
      f.first->set_facet_surface_center(f.second, p);
    }

    static const Point& get_facet_surface_center(const Facet& f)
    {
      return f.first->get_facet_surface_center(f.second);
    }

    static void reset_visited(Facet f)
    {
      f.first->reset_visited(f.second);
    }

    static bool is_facet_visited(const Facet& f)
    {
      return f.first->is_facet_visited(f.second);
    }

    // Remains unchanged
    Tr& triangulation_ref_impl()
      {
	return tr;
      }

    // Remains unchanged
    const Tr& triangulation_ref_impl() const
      {
	return tr;
      }

    // Initialization function
    void scan_triangulation_impl() {
      Point center;
      // We test only the finite Delaunay facets

#ifdef CGAL_SURFACE_MESHER_VERBOSE
      std::cout << "scanning facets..." << std::endl;
#endif
      for (Finite_facets_iterator fit = tr.finite_facets_begin(); fit !=
	     tr.finite_facets_end(); ++fit) {
	if (tr.dimension() == 3) {
	  Facet other_side = mirror_facet(*fit);

	  set_facet_visited(*fit);
	  set_facet_visited(other_side);
	  //(*fit).first->set_facet_visited((*fit).second);
	  //c->set_facet_visited(other_side.second);

	  if (is_facet_on_surface(*fit, center)) {
	    c2t3.set_in_complex(*fit);
	    set_facet_surface_center((*fit), center);
	    set_facet_surface_center(other_side, center);
	    //(*fit).first->set_facet_on_surface((*fit).second,true);
	    //(*fit).first->set_surface_center_facet((*fit).second,center);

	    //c->set_facet_on_surface(other_side.second,true);
	    //c->set_surface_center_facet(other_side.second,center);

            Quality a_r;
            if (criteria.is_bad (*fit, a_r)) {
               // @TODO, @WARNING: why do we insert the two sides of facets?!
	      facets_to_refine.insert(*fit,a_r);
	      facets_to_refine.insert(other_side,a_r);
	    }
	  }

	  else {
	    c2t3.remove_from_complex(*fit);
	    //(*fit).first->set_facet_on_surface((*fit).second,false);
	    //c->set_facet_on_surface(other_side.second,false);
	  }
	}
	else {
	  CGAL_assertion (tr.dimension() == 2);
	  if (is_facet_on_surface(*fit, center)) {
	    //Cell_handle c;
	    c2t3.set_in_complex(*fit);
	    set_facet_surface_center((*fit),center);
	    //c=(*fit).first;
	    //c->set_facet_on_surface((*fit).second,true);
	    //c->set_facet_surface_center((*fit).second,center);

            Quality a_r;
	    if (criteria.is_bad (*fit, a_r)) {
	      facets_to_refine.insert(*fit,a_r);
	    }
	  }
	  else
	    c2t3.remove_from_complex(*fit);
	  //	    (*fit).first->set_facet_on_surface((*fit).second,false);
	}
      }
    } // scan_triangulation_impl end

    // Tells whether there remain elements to refine
    bool no_longer_element_to_refine_impl() const {
      return facets_to_refine.empty();
    }

    // Returns the next element to refine
    Facet get_next_element_impl() {
      return facets_to_refine.front()->second;
    }

    // deletes the next element from the set of elements to refine
    // NB: it is useless here, since the update of the restricted
    // Delaunay triangulation automatically deletes the next element
    void pop_next_element_impl() {}

    // From the element to refine, gets the point to insert
    Point refinement_point_impl(const Facet& f) const
      {
	CGAL_assertion (c2t3.face_status(f) == C2T3::REGULAR);
	//CGAL_assertion (f.first->is_facet_on_surface (f.second));
	return get_facet_surface_center (f);
	//return f.first->get_facet_surface_center (f.second);
      }

    // Useless here
    void before_conflicts_impl(const Facet&, const Point&
#ifdef CGAL_SURFACE_MESHER_DEBUG_BEFORE_CONFLICTS
			                                s)
    {
      std::cerr << "Refine_facets: before conflicts of " << s << " ";
#else
                                                        )
    {
#endif
    }

  // Useless here
    Mesher_level_conflict_status private_test_point_conflict_impl(const Point&,
								  Zone&)
      {
	return NO_CONFLICT;
      }

    // Useless here
    void after_no_insertion_impl(const Facet&, const Point&, const Zone&)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    // Tests if the point p encroaches one facet of the restricted Delaunay
    // triangulation.

    Mesher_level_conflict_status
    test_point_conflict_from_superior_impl(const Point& p,
					   Zone& zone)
      {
	for (typename Zone::Facets_iterator fit =
	       zone.internal_facets.begin();
	     fit != zone.internal_facets.end();
	     ++fit)
	  if( test_if_facet_is_encroached(*fit, p) )
	    return CONFLICT_BUT_ELEMENT_CAN_BE_RECONSIDERED;

	for (typename Zone::Facets_iterator fit =
	       zone.boundary_facets.begin();
	     fit != zone.boundary_facets.end();
	     ++fit)
	  if( test_if_facet_is_encroached(*fit, p) )
	    return CONFLICT_BUT_ELEMENT_CAN_BE_RECONSIDERED;

	return NO_CONFLICT;
      }

    bool test_if_facet_is_encroached(const Facet f, const Point& p)
    {
      if( tr.is_infinite(f.first) )
	return false;

      if (c2t3.face_status(f) == C2T3::REGULAR)
	{
	  const Cell_handle& c = f.first;
	  const int index = f.second;

	  typename GT::Compute_squared_distance_3 distance =
	    tr.geom_traits().compute_squared_distance_3_object();

// 	  std::cerr << "testing conflict... \n";
	  // test Delaunay surfacique
	  Point center = get_facet_surface_center(f);
	  if( distance(center, p) <
	      distance(center, c->vertex((index+1)&3)->point()) )
	    {
// 	      Quality q = criteria.quality(f);
              Quality q;
              criteria.is_bad(f, q); // to get q (passed as reference)
	      facets_to_refine.insert(f, q);
	      return true;
	    }
	  // test Gabriel
	  center = tr.geom_traits().construct_circumcenter_3_object()
	    (c->vertex((index+1)&3)->point(),
	     c->vertex((index+2)&3)->point(),
	     c->vertex((index+3)&3)->point());
	  if( distance(center, p) <
	      distance(center, c->vertex((index+1)&3)->point()) )
	    {
// 	      Quality q = criteria.quality(f);
              Quality q;
              criteria.is_bad(f, q); // to get q (passed as reference)
	      facets_to_refine.insert(f, q);
	      return true;
	    }
	}
      return false;
    }


  /* returns the conflicts zone */
    Zone conflicts_zone_impl(const Point& p, Facet f) const {
	Zone zone;

       	// TODO may be avoid the locate here
	zone.cell =
	  tr.locate(p, zone.locate_type, zone.i, zone.j, f.first);

        tr.find_conflicts(p, zone.cell,
                          std::back_inserter(zone.boundary_facets),
                          std::back_inserter(zone.cells),
                          std::back_inserter(zone.internal_facets));
	return zone;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Deletes old facets from the restricted Delaunay triangulation

    void before_insertion_impl(const Facet&, const Point& s, Zone& zone) {

      // DEBUG: on ajoute un espace
      //std::cout << " ";

      CGAL_assertion_code(Vertex_handle v);
      CGAL_assertion (!tr.is_vertex(s,v));

      if (tr.dimension() == 3) {
	// On s'occupe des facettes de la zone de conflit
	for (typename Zone::Facets_iterator fit =
	       zone.internal_facets.begin();
	     fit != zone.internal_facets.end();
	     ++fit)
	  handle_facet_inside_conflict_zone (*fit);

	for (typename Zone::Facets_iterator fit =
	       zone.boundary_facets.begin(); fit !=
	       zone.boundary_facets.end(); ++fit)
	  handle_facet_on_boundary_of_conflict_zone (*fit);
      }

      // If dim < 3, then the triangulation has only one facet and the
      // complex has no facet, generically
      else {
	CGAL_assertion (tr.dimension() == 2);
	facets_to_refine.clear();
      }

    }


    ///////////////////////////////////////////////////////////////////////////
    // Adds new facets from the restricted Delaunay triangulation

    void after_insertion_impl(const Vertex_handle& v) {
#ifdef CGAL_SURFACE_MESHER_DEBUG_AFTER_INSERTION
      std::cerr << "Inserted\n";
#endif
      restore_restricted_Delaunay(v);
    }

    void restore_restricted_Delaunay(const Vertex_handle& v)
    {
      Cell_handle cellule;

      // On met a jour les flags des nouvelles facettes (celles a
      // l'interieur du trou et celles sur le bord du trou)

      std::list<Cell_handle> cellules;
      tr.incident_cells (v, std::back_inserter(cellules));

      while(!cellules.empty()) {
	cellule=cellules.front();
	cellules.pop_front();

	// Look at all four facets of the cell, starting with the
	// facet opposite to the new vertex
	int indice = cellule->index (v);
	handle_opposite_facet (Facet (cellule, indice));

	for (int i = 1; i <= 3; ++i)
	  handle_incident_facet (Facet (cellule, (indice+i)&3));
      }


      // DEBUG: on ajoute un espace
      //std::cout << " ";

    } // restore_restricted_Delaunay end




    ///////////////////////////////////////////////////////////////////////////

    // Useless here
    void after_no_insertion_impl(const Facet&, const Point&, Zone&) {}




    ///////////////////////////////////////////////////////////////////////////
    // Private functions
  private:


    ///////////////////////
    // For before_insertion

    // Actions to perform on a facet inside the conflict zone
    void handle_facet_inside_conflict_zone (Facet f) {
      Facet other_side = mirror_facet(f);

      // On enleve la facette de la liste des mauvaises facettes
      facets_to_refine.erase(f);
      facets_to_refine.erase(other_side);

      // Le compteur des visites est remis a zero
       reset_visited(f);
       reset_visited(other_side);

      // On retire la facette du complexe (car on doit etre
      // independant de l'implementation du complexe)
      c2t3.remove_from_complex (f);
    }

    // Action to perform on a facet on the boundary of the conflict zone
    void handle_facet_on_boundary_of_conflict_zone (const Facet& f) {
      // perform the same operations as for an internal facet
      handle_facet_inside_conflict_zone (f);
    }



    ///////////////////////
    // For after_insertion

    // Action to perform on a facet incident to the new vertex
    void handle_incident_facet (const Facet& f) {

      // If the facet is infinite or has been already visited,
      // then there is nothing to do as for it or its edges
      if (tr.is_infinite(f) || is_facet_visited(f))
	return;

      Facet other_side = mirror_facet(f);

      // NB: set_facet_visited() is implementation dependant
      // and each side of the real facet has to be considered
      // separately
      set_facet_visited(f);
      set_facet_visited(other_side);

      // On regarde d'abord si la facette est dans le Delaunay
      // restreint
      Point center;
      if (is_facet_on_surface(f, center)) {

	// NB: set_in_complex() is implementation independant and
	// consider both sides of the facet at once
	c2t3.set_in_complex(f);

	// NB: set_facet_surface_center() is implementation dependant
	// and each side of the real facet has to be considered
	// separately
	set_facet_surface_center(f, center);
	set_facet_surface_center(other_side, center);

	// On regarde alors si la facette est bonne
        Quality a_r;
	if (criteria.is_bad (f, a_r)) {
          // @TODO, @WARNING: same as above, why the two sides?!
	  facets_to_refine.insert (f, a_r);
	  facets_to_refine.insert (other_side, a_r);
	}
      }

      // Else the facet is not a restricted Delaunay facet. However,
      // since it was removed before the insertion of the new vertex,
      // it is not in the complex nor in the list of bad facets, by default
    }

    // Action to perform on a facet opposite to the new vertex
    void handle_opposite_facet (const Facet& f) {
      // perform the same operations as for a facet incident to the
      // new vertex
      handle_incident_facet (f);
    }



    ///////////////////////
    // Predicate to test restricted Delaunay membership


    // Tests whether a given facet is restricted or not
    bool is_facet_on_surface(const Facet& f, Point& center) {
      typedef typename Surface_mesh_traits::Intersect_3 Intersect_3;
      Intersect_3 intersect = meshtraits.intersect_3_object();

      Object dual = tr.dual(f);

      //      typename GT::Segment_3 segment;
      typename GT::Segment_3 segment;
      typename GT::Ray_3 ray;
      typename GT::Line_3 line;

      // If the dual is a segment
      if (assign(segment, dual)) {
	Object intersection = intersect(surf, segment);
	if (assign(center,intersection))
	  return true;
      }

      // If the dual is a ray
      else if(assign(ray, dual)) {
	Object intersection = intersect(surf, ray);
	//std::cerr << "intersection: " << std::endl;
	if (assign(center,intersection))
	  return true;
      }

      // If the dual is a line
      else if(assign(line, dual)) {
	Object intersection = intersect(surf, line);
	if (assign(center,intersection))
	  return true;
      }

      // Else there is a problem with the dual
      else {
        CGAL_assertion(false);
      }

      return false;
    }


  protected:


    // Checks restricted Delaunay triangulation
    void check_restricted_delaunay () {
      for (Finite_facets_iterator fit = tr.finite_facets_begin(); fit !=
	     tr.finite_facets_end(); ++fit) {
	Facet other_side = mirror_facet(*fit);
	CGAL_assertion (c2t3.face_status(*fit) ==
			c2t3.face_status(other_side));
	//CGAL_assertion (fit->first->is_facet_on_surface (fit->second) ==
	//		other_side.first->is_facet_on_surface
	//		(other_side.second));
	bool restr, restr_bis;
	Point center;
	restr = is_facet_on_surface(*fit, center);
	restr_bis = is_facet_on_surface(other_side, center);
	CGAL_assertion (restr == restr_bis);
	CGAL_assertion ((c2t3.face_status(*fit)
			 == C2T3::REGULAR) == restr);
	CGAL_assertion ((c2t3.face_status(other_side)
			 == C2T3::REGULAR) == restr_bis);
	//CGAL_assertion (fit->first->is_facet_on_surface (fit->second) ==
	//		restr);
	//CGAL_assertion (other_side.first->is_facet_on_surface
	//		(other_side.second) == restr_bis);

	if ( (c2t3.face_status(*fit) == C2T3::REGULAR) !=
	    is_facet_on_surface(*fit, center)) {
	  std::cerr << "Error in restricted Delaunay triangulation: ("
		    << (c2t3.face_status(*fit) == C2T3::REGULAR)
		    << "/"
		    << is_facet_on_surface(*fit, center)
		    << ")"
		    << std::endl;
	  std::exit (-1);
	}
      }
    }

    std::string debug_info() const
    {
      std::stringstream s;
      s << facets_to_refine.size();
      return s.str();
    }

  };  // end Surface_mesher_base

    namespace details {

      template <typename Base, typename Self>
      class Mesher_level_generator {
        typedef typename Base::Complex_2_in_triangulation_3 C2T3;
        typedef typename C2T3::Triangulation Triangulation;
        typedef typename Triangulation::Facet Facet;
        typedef Triangulation_mesher_level_traits_3<Triangulation> Tr_m_l_traits_3;
      public:
        typedef Mesher_level <Triangulation,
                              Self,
                              Facet,
                              Null_mesher_level,
                              Tr_m_l_traits_3> Type;
        typedef Type type;
      }; // end class Mesher_level_generator<Base, Self>
      
    } // end namespace details

    enum Debug_flag { DEBUG, NO_DEBUG};
    enum Verbose_flag { VERBOSE, NOT_VERBOSE};

  template <
    typename Base,
    Verbose_flag verbose = NOT_VERBOSE,
    Debug_flag debug = NO_DEBUG
    >
  struct Surface_mesher
    : public Base,
      public details::Mesher_level_generator<Base,
                                             Surface_mesher<Base> >::Type
  {
  public:
    typedef typename Base::Complex_2_in_triangulation_3 C2T3;
    typedef typename C2T3::Triangulation Triangulation;
    typedef Triangulation Tr;
    typedef typename Base::Surface Surface;
    typedef typename Base::Criteria Criteria;
    typedef typename Base::Surface_mesh_traits Surface_mesh_traits;

    typedef Surface_mesher<Base> Self;

    typedef typename details::Mesher_level_generator<Base,
      Surface_mesher<Base> >::Type Mesher_lvl;

    using Mesher_lvl::scan_triangulation;
    using Mesher_lvl::refine;
    using Mesher_lvl::is_algorithm_done;
    using Mesher_lvl::one_step;
    using Base::check_restricted_delaunay;

    typedef C2T3 Complex_2_in_triangulation_3;

  private:
    Null_mesher_level null_mesher_level;
    Null_mesh_visitor null_visitor;
    bool initialized;

  public:
    Surface_mesher(C2T3& c2t3,
                   Surface& surface,
                   Surface_mesh_traits mesh_traits,
                   Criteria& criteria)
      : Base(c2t3, surface, mesh_traits, criteria), 
        Mesher_lvl(null_mesher_level),
        initialized(false)
    {
#ifdef CGAL_SURFACE_MESHER_DEBUG_CONSTRUCTORS
      std::cerr << "CONS: Surface_mesher\n";
#endif
    }

    // Initialization
    void init()
      {
	scan_triangulation();
	initialized = true;
	if (debug == DEBUG)
	  check_restricted_delaunay();
      }

    void refine_mesh () {
      if(!initialized)
	init();

      if (verbose == NOT_VERBOSE)
	refine (null_visitor);
      else {
	std::cerr << "Refining...\n";
	int nbsteps = 0;
	std::cerr << "(" << nbsteps << ","
		  << this->facets_to_refine.size() << ")";
	while (!is_algorithm_done()) {
	  one_step (null_visitor);
	  std::cerr << "\r             \r"
		    << "(" << ++nbsteps << ","
		    << this->debug_info()
		    << ")";
	}
	std::cerr << "\ndone.\n";
      }

      if (debug == DEBUG)
	check_restricted_delaunay();

      initialized = false;
    }

  };  // end Surface_mesher


  }  // end namespace Surface_mesher

}  // end namespace CGAL


#endif // CGAL_SURFACE_MESHER_SURFACE_MESHER_H

