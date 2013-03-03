// Copyright (c) 1997-2000  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Nef_3/include/CGAL/Nef_3/SNC_FM_decorator.h $
// $Id: SNC_FM_decorator.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>

#ifndef CGAL_SNC_FM_DECORATOR_H
#define CGAL_SNC_FM_DECORATOR_H

#include <CGAL/Nef_2/geninfo.h>
#include <CGAL/Nef_2/Segment_overlay_traits.h>
#include <CGAL/Nef_3/SNC_decorator.h>
#include <list>
#undef CGAL_NEF_DEBUG
#define CGAL_NEF_DEBUG 31
#include <CGAL/Nef_2/debug.h>

CGAL_BEGIN_NAMESPACE

//--------------------------------------------------------------------------
/* We introduce a template that is a point that refers to a vertex.
   We derive from the point type and add the vertex handle */

template <typename P, typename H>
class Vertex_point : public P {
  H v_;
public:
  Vertex_point() {}
  Vertex_point(const P& p, H v) : P(p)  { v_=v; }
  Vertex_point(const Vertex_point& vp) : P(vp) { v_=vp.v_; }
  Vertex_point& operator=(const Vertex_point& vp)
  { P::operator=(vp); v_=vp.v_; return *this; } 
  H vertex() const { return v_; }
};

template <typename P, typename H>
std::ostream& operator<<(std::ostream& os, const Vertex_point<P,H>& p)
{ os << static_cast<P>(p); return os; }

template <typename P, typename H>
std::ostream& operator<<(std::ostream& os, 
  const std::pair< Vertex_point<P,H>, Vertex_point<P,H> > & s)
{ os << s.first << s.second; return os; }

template <typename P, typename SE>
struct Sort_sedges {
  bool operator()(SE e1, SE e2) {
    P p1[2], p2[2];
    p1[0] = e1->source()->center_vertex()->point();
    p1[1] = e1->twin()->source()->twin()->center_vertex()->point();
    p2[0] = e2->source()->center_vertex()->point();
    p2[1] = e2->twin()->source()->twin()->center_vertex()->point();
    int i1(0), i2(0);
    if(CGAL::lexicographically_xyz_smaller(p1[1],p1[0])) i1 = 1;
    if(CGAL::lexicographically_xyz_smaller(p2[1],p2[0])) i2 = 1;
    if(p1[i1] != p2[i2]) return CGAL::lexicographically_xyz_smaller(p1[i1],p2[i2]);
    if(p1[1-i1] != p2[1-i2]) return CGAL::lexicographically_xyz_smaller(p1[1-i1],p2[1-i2]);
    return i1<i2;
  }
};

//--------------------------------------------------------------------------
/* The following type is an output model for our generic segment
   sweep module |Segment_overlay_traits|. We use that code to 
   sweep all sedges of a plane to finally associate facet cycles
   to facets. Note that we do not use the segment intersection
   functionality of the code as the sedges only touch in endpoints.
   There is room for optimization here. */
//--------------------------------------------------------------------------

template <typename P, typename V, typename E, typename I>
struct Halffacet_output {

Halffacet_output(CGAL::Unique_hash_map<I,E>& F, std::vector<E>& S) 
  : From(F), Support(S) { edge_number=0; Support[0]=E(); }

typedef P         Point;
typedef V         Vertex_handle;
typedef unsigned  Halfedge_handle;

CGAL::Unique_hash_map<I,E>& From;
std::vector<E>& Support;
unsigned edge_number;

Vertex_handle new_vertex(const Point& p) const
{ geninfo<unsigned>::create(p.vertex()->info());
  return p.vertex(); }

Halfedge_handle new_halfedge_pair_at_source(Vertex_handle v) 
{ CGAL_NEF_TRACEN("new_edge "<<&*v<<" "<<edge_number+1);
  return ++edge_number; }

void supporting_segment(Halfedge_handle e, I it)
{ if ( From[it] != E() ) Support[e] = From[it]; }

void halfedge_below(Vertex_handle v, Halfedge_handle e)
{ CGAL_NEF_TRACEN("halfedge_below point "<< v->point() <<": " << e); 
  geninfo<unsigned>::access(v->info()) = e; }

// all empty, no update necessary
void link_as_target_and_append(Vertex_handle v, Halfedge_handle e)
{ /* do nothing */ }

void trivial_segment(Vertex_handle v, I it) const {}
void starting_segment(Vertex_handle v, I it) const {}
void passing_segment(Vertex_handle v, I it) const {}
void ending_segment(Vertex_handle v, I it) const {}

};

//--------------------------------------------------------------------------
// the following class is the geometry kernel of the generic sweep
// module |Segment_overlay_traits|. 
//--------------------------------------------------------------------------

template <typename Point, typename Plane, typename Handle>
class Halffacet_geometry { 

 public:
  typedef Point Point_3;
  typedef Plane Plane_3;
  typedef Vertex_point<Point,Handle>  Point_2;
  typedef std::pair<Point_2,Point_2>  Segment_2;

 private:
  // the underlying plane:
  Plane_3 h;

  Point_3 above(const Point_3& p) const
  { return p + h.orthogonal_vector(); }

 public:

Halffacet_geometry(const Plane_3& hi) : h(hi) {}

Point_2 source(const Segment_2& s) const  { return s.first; }
Point_2 target(const Segment_2& s) const  { return s.second; }

bool is_degenerate(const Segment_2& s) const
{ return source(s)==target(s); }

Segment_2 construct_segment(const Point_2& p1, const Point_2& p2) const
{ return Segment_2(p1,p2); }

int orientation(const Point_2& p1, const Point_2& p2, const Point_2& p3) const
{ return static_cast<int>(
    CGAL::orientation(p1,p2,p3,above(p1))); }

int orientation(const Segment_2& s, const Point_2& p) const
{ return orientation(source(s),target(s),p); }

int compare_xy(const Point_2& p1, const Point_2& p2) const
{ return static_cast<int>(
    CGAL::compare_xyz(p1,p2)); }

Point_2 intersection(const Segment_2& s1, const Segment_2& s2) const
{ CGAL_assertion(target(s1)==target(s2)); 
  return target(s1); }

bool left_turn(const Point_3& p1, const Point_3& p2, const Point_3& p3) const
{ return CGAL::orientation(p1,p2,p3,above(p1)) == CGAL::POSITIVE; }

}; // Halffacet_geometry


//--------------------------------------------------------------------------
// SNC_FM_decorator
// Note that we interpret sedges as edge uses between vertices.  We
//   overwrite some operations from the base class for that semantics.
//--------------------------------------------------------------------------

template <typename S> class SNC_decorator;

template <typename SNC_structure_>
class SNC_FM_decorator : public SNC_decorator<SNC_structure_> {
public:
  typedef SNC_structure_ SNC_structure;
  typedef SNC_decorator<SNC_structure> Base;

  typedef typename SNC_structure::Vertex_iterator Vertex_iterator; 
  typedef typename SNC_structure::Vertex_handle Vertex_handle;
  typedef typename SNC_structure::Halfedge_iterator Halfedge_iterator; 
  typedef typename SNC_structure::Halfedge_handle Halfedge_handle;
  typedef typename SNC_structure::Halffacet_iterator Halffacet_iterator; 
  typedef typename SNC_structure::Halffacet_handle Halffacet_handle;
  typedef typename SNC_structure::Volume_iterator Volume_iterator;
  typedef typename SNC_structure::Volume_handle Volume_handle;
  typedef typename SNC_structure::SVertex_iterator SVertex_iterator; 
  typedef typename SNC_structure::SVertex_handle SVertex_handle;
  typedef typename SNC_structure::SHalfedge_iterator SHalfedge_iterator; 
  typedef typename SNC_structure::SHalfedge_handle SHalfedge_handle;
  typedef typename SNC_structure::SHalfloop_iterator SHalfloop_iterator; 
  typedef typename SNC_structure::SHalfloop_handle SHalfloop_handle;
  typedef typename SNC_structure::SFace_iterator SFace_iterator; 
  typedef typename SNC_structure::SFace_handle SFace_handle;
  typedef typename SNC_structure::SFace_cycle_iterator SFace_cycle_iterator;
  typedef typename SNC_structure::Halffacet_cycle_iterator Halffacet_cycle_iterator;
  typedef typename SNC_structure::Shell_entry_iterator Shell_entry_iterator;
  typedef typename SNC_structure::Object_handle Object_handle;
  typedef typename SNC_structure::Point_3 Point_3;
  typedef typename SNC_structure::Plane_3 Plane_3;
  typedef typename SNC_structure::Mark Mark;

  typedef typename Base::SHalfedge_around_facet_circulator
    SHalfedge_around_facet_circulator;
#if 0
  typedef typename Base::SHalfedge_around_facet_const_circulator
    SHalfedge_around_facet_const_circulator;
#endif

  typedef typename std::list<Object_handle>::iterator 
    Object_list_iterator;

  typedef Vertex_point<Point_3,Vertex_handle>  Vertex_point;
  typedef std::pair<Vertex_point,Vertex_point> Vertex_segment;
  typedef std::list<Vertex_segment>            Segment_list;
  typedef typename Segment_list::iterator      Segment_iterator;

  typedef CGAL::Halffacet_geometry<Point_3,Plane_3,Vertex_handle> 
    Halffacet_geometry;

protected:
  Halffacet_handle f_;
public:

  SNC_FM_decorator(SNC_structure& W) : Base(W), f_() {}
  SNC_FM_decorator(SNC_structure& W, Halffacet_handle f)
   : Base(W),f_(f) {}


  Halffacet_cycle_iterator facet_cycles_begin() const 
  { return f_->facet_cycles_begin(); }  
  Halffacet_cycle_iterator facet_cycles_end()   const 
  { return f_->facet_cycles_end(); }

  void create_facet_objects(const Plane_3& h,
    Object_list_iterator start, Object_list_iterator end) const;


protected:
//--------------------------------------------------------------------------
/* We provide some information on determine_facet. To understand its
   functionality please refer to the Nef_2 implementation report where
   there is similar function determine_face with more documentation.

   When we call |determine_facet|$(e,\ldots)$ we know that the
   shalfedge |e| is not linked to a face object yet and thus no
   shalfedge in its face cycle is linked. Thus we jump to the minimal
   shalfedge and look down (along the previously used sweep line in
   the plane of the facet). If we see an shalfedge we ask for its
   face. If it does not have one we recurse.  Note that the target
   vertex of the minimal halfedge actually has a view downwards as we
   examine a hole facet cycle. The method |link_as_facet_cycle| does
   the linkage between the facet object and all sedges of the facet
   cycle. Its cost is linear in the size of the facet cycle. Note also
   that we do the linking bottom up along the recursion stack for all
   visited hole (facet) cycles.  Thus we visit each hole facet cycle
   only once as afterwards each edge of the facet cycle is incident to
   a facet. */
//--------------------------------------------------------------------------

  Halffacet_handle determine_facet(SHalfedge_handle e, 
    const std::vector<SHalfedge_handle>& MinimalEdge,
    const CGAL::Unique_hash_map<SHalfedge_handle,int>& FacetCycle,
    const std::vector<SHalfedge_handle>& Edge_of) const
  { CGAL_NEF_TRACEN("  determine_facet "<<debug(e));
    int fc = FacetCycle[e];
    SHalfedge_handle e_min = MinimalEdge[fc];
    SHalfedge_handle e_below = 
      Edge_of[geninfo<unsigned>::access(info(e_min->twin()->source()->twin()->source()))];
    CGAL_assertion( e_below != SHalfedge_handle() );
    CGAL_NEF_TRACEN("  edge below " << debug(e_below));    
    Halffacet_handle f = e_below->facet();
    if ( f != Halffacet_handle() ) return f; // has already a facet 
    // e_below also has no facet
    f = determine_facet(e_below, MinimalEdge, FacetCycle, Edge_of);
    CGAL_NEF_TRACEN("  edge below " << debug(e_below));
    link_as_facet_cycle(e_below,f); 
    link_as_facet_cycle(e_below->twin(),f->twin()); 
    return f;
  }

//--------------------------------------------------------------------------
/* segment(...)
   an shalfedge |e| can be interpreted as an edge-use extending along
   the halfedge that leaves the local graph in |target(e)|. Only
   edge-uses allow us to code the non-unique character of an edge as a
   boundary object of several facets. An edge-use |e| represents the
   edge |target(e)| in the boundary structure of a facet. */
//--------------------------------------------------------------------------

  Vertex_segment segment(SHalfedge_handle e) const
  { Vertex_handle vs = e->source()->source(), 
      vt = e->twin()->source()->twin()->source(); 
    Vertex_point  ps(vs->point(),vs), pt(vt->point(),vt);
    return Vertex_segment(ps,pt); }

  Vertex_segment segment(SHalfloop_handle l) const
  { Vertex_handle v = l->incident_sface()->center_vertex(); 
    Vertex_point  p(v->point(),v);
    return Vertex_segment(p,p); }


}; // SNC_FM_decorator<SNC_structure_>



//--------------------------------------------------------------------------
/* create_facet_objects() 
   In this method we use the visibility along a sweep line in the
   facet plane to create facet objects. |SEdge_below[v]| either
   provides the sedge that is hit first by a vertical ray downwards or
   an uninitialized edge if there is none. */
//--------------------------------------------------------------------------


template <typename SNC_>
void SNC_FM_decorator<SNC_>::
create_facet_objects(const Plane_3& plane_supporting_facet,
  Object_list_iterator start, Object_list_iterator end) const
{ CGAL_NEF_TRACEN(">>>>>create_facet_objects");
  CGAL::Unique_hash_map<SHalfedge_handle,int> FacetCycle(-1);
  std::vector<SHalfedge_handle> MinimalEdge;
  std::list<SHalfedge_handle> SHalfedges; 
  std::list<SHalfloop_handle> SHalfloops; 

  CGAL::Unique_hash_map<Vertex_handle,SHalfedge_handle> SHalfedgeBelow;
  CGAL::Unique_hash_map<Segment_iterator,SHalfedge_handle>  From;

  Segment_list Segments;
  SHalfedge_handle e; SHalfloop_handle l;
  typename std::list<SHalfedge_handle>::iterator eit,epred;
  typename std::list<SHalfloop_handle>::iterator lit;

  // the output decorator for the facet plane sweep
  typedef CGAL::Halffacet_output<Vertex_point, Vertex_handle, 
    SHalfedge_handle, Segment_iterator> Halffacet_output;

  // the sweep traits class instantiated with the input, output and
  // geometry models
  typedef CGAL::Segment_overlay_traits
    <Segment_iterator, Halffacet_output, Halffacet_geometry>
    Halffacet_sweep_traits;
  typedef CGAL::generic_sweep<Halffacet_sweep_traits>   Halffacet_sweep;
  Halffacet_geometry G(plane_supporting_facet);

  /* We first separate sedges and sloops, and fill a list of segments
     to trigger a sweep. */

  for ( ; start != end; ++start ) {
    if ( CGAL::assign(e,*start) ) {
      SHalfedges.push_back(e); 
    } else if ( CGAL::assign(l,*start) ) {
      SHalfloops.push_back(l); 
    } else 
      CGAL_assertion_msg(0,"Damn wrong handle.");
  }

  /* We iterate all shalfedges and assign a number for each facet
     cycle.  After that iteration for an edge |e| the number of its
     facet cycle is |FacetCycle[e]| and for a facet cycle |c| we know
     |MinimalEdge[c]|. */

  int i=0; 
  CGAL_forall_iterators(eit,SHalfedges) { e = *eit;
    if ( FacetCycle[e] >= 0 ) continue; // already assigned
    SHalfedge_around_facet_circulator hfc(e),hend(hfc);
    SHalfedge_handle e_min = e;
    CGAL_NEF_TRACEN("\n  facet cycle numbering "<<i);
    CGAL_For_all(hfc,hend) {
      FacetCycle[hfc]=i; // assign face cycle number
      if ( CGAL::lexicographically_xyz_smaller(hfc->twin()->source()->twin()->source()->point(), 
					       e_min->twin()->source()->twin()->source()->point()))
	e_min = hfc;
      CGAL_NEF_TRACEN(hfc->twin()->source()->twin()->source()->point() << " lex xyz smaller " << 
		      e_min->twin()->source()->twin()->source()->point() << "=" << 
		      CGAL::lexicographically_xyz_smaller(hfc->twin()->source()->twin()->source()->point(), 
							  e_min->twin()->source()->twin()->source()->point()));

    } CGAL_NEF_TRACEN("");
    MinimalEdge.push_back(e_min);
    ++i;
  }

  /* We now know the number of facet cycles |i| and we have a minimal
     edge |e| for each facet cycle. We just check the geometric
     embedding of |e| and |e->next()| to characterize the facet cycle
     (outer or hole). Note that the two edges cannot be collinear due
     to the minimality of |e| (the lexicographic minimality of the
     embedding of its target vertex). Outer facet cycles obtain facet
     objects right away. */

  for (int j=0; j<i; ++j) {
    SHalfedge_handle e = MinimalEdge[j];
    CGAL_NEF_TRACEN("  facet cycle "<<j<<" minimal halfedge "<<debug(e));
    Point_3 p1 = e->source()->source()->point(), 
      p2 = e->twin()->source()->twin()->source()->point(), 
      p3 = e->next()->twin()->source()->twin()->source()->point();
    if ( G.left_turn(p1,p2,p3) ) { 
      Halffacet_handle f = this->sncp()->new_halffacet_pair(plane_supporting_facet);
      link_as_facet_cycle(e,f); link_as_facet_cycle(e->twin(),f->twin()); 
      f->mark() = f->twin()->mark() = e->mark();
      CGAL_NEF_TRACEN("  creating new facet object "<<&*f<<" bd "<<&*e);
    }
  }

  /* Now the only shalfedges not linked are those on hole facet
     cycles.  We use a recursive scheme to find the bounding cycle
     providing the facet object and finally iterate over all isolated
     vertices to link them accordingly to their containing facet
     object. Note that in that final iteration all shalfedges already
     have facet links. Thus that ensures termination. The recursive
     operation $|determine_facet|(e,\ldots)$ returns the facet
     containing the hole cycle of |e|. As a postcondition of this code
     part we have all shalfedges and isolated shalfloops linked to
     facet objects, and all facet objects know their bounding facet
     cycles. */
 
  bool do_sweep = false;
  if(SHalfloops.size() > 0)
    do_sweep = true;

  CGAL_forall_iterators(eit,SHalfedges) { 
    if ( (*eit)->facet() == Halffacet_handle() ) {
      do_sweep = true;
      break;
    }
  }
  
#ifndef CGAL_NEF3_PLANE_SWEEP_OPTIMIZATION_OFF
  if(!do_sweep) return; 
#endif

#ifdef CGAL_NEF3_TIMER_PLANE_SWEEPS
  number_of_plane_sweeps++;
  timer_plane_sweeps.start();
#endif

  //  Note that we only allow those edges that are
  //  directed from lexicographically smaller to larger vertices. 
  //  Insertion of SHalfedges into Segments is shifted below in order
  //  to guarantee that there are no gaps in the overlay.

  SHalfedges.sort(Sort_sedges<Point_3,SHalfedge_handle>());
  for(eit = SHalfedges.begin();eit != SHalfedges.end();) {
    CGAL_NEF_TRACEN("  appending edge "<< debug(*eit));
    Segments.push_front(segment(*eit)); 
    From[Segments.begin()] = *eit;
    epred=eit;
    ++eit;
    if(eit != SHalfedges.end()) 
      CGAL_NEF_TRACEN("test " << std::endl << "  " << debug(*epred) 
	     << std::endl << "  " << debug(*eit));
    if(eit != SHalfedges.end() && 
       (*epred)->source()->source()->point()==(*eit)->twin()->source()->twin()->source()->point() &&  
       (*eit)->source()->source()->point()== (*epred)->twin()->source()->twin()->source()->point())
      ++eit;
  }

  CGAL_forall_iterators(lit,SHalfloops) {
    CGAL_NEF_TRACEN("  appending loop " << (*lit)->incident_sface()->center_vertex()->point()); 
    Segments.push_back(segment(*lit));
  }
    
  std::vector<SHalfedge_handle> Edge_of(Segments.size()+1);
  Halffacet_output O(From,Edge_of);
  Halffacet_sweep FS(typename Halffacet_sweep::INPUT(
    Segments.begin(),Segments.end()), O, G); FS.sweep();

#ifdef CGAL_NEF3_TIMER_PLANE_SWEEPS
  timer_plane_sweeps.stop();
#endif

  CGAL_forall_iterators(eit,SHalfedges) { e=*eit;
  if ( e->facet() != Halffacet_handle() ) continue;
    CGAL_NEF_TRACEN("  linking hole "<<debug(e));
    Halffacet_handle f = determine_facet(e,MinimalEdge,FacetCycle,Edge_of);
    link_as_facet_cycle(e,f); link_as_facet_cycle(e->twin(),f->twin());
  }

  CGAL_forall_iterators(lit,SHalfloops) { l=*lit;
    SHalfedge_handle e_below = 
      Edge_of[geninfo<unsigned>::access(info(l->incident_sface()->center_vertex()))];
    
    CGAL_assertion( e_below != SHalfedge_handle() );
    CGAL_NEF_TRACEN("link sloop at vertex "<< l->incident_sface()->center_vertex()->point());
    CGAL_NEF_TRACEN("e_below "  << debug(e_below));
    CGAL_NEF_TRACEN("next    "  << debug(e_below->next()));
    CGAL_NEF_TRACEN("next    "  << debug(e_below->next()->next()));
    CGAL_NEF_TRACEN("next    "  << debug(e_below->next()->next()->next()));
    CGAL_NEF_TRACEN("next    "  << debug(e_below->next()->next()->next()->next()));
    link_as_interior_loop(l,e_below->facet());
    link_as_interior_loop(l->twin(),e_below->facet()->twin());
  }
  
  CGAL_NEF_TRACEN("exit FM");
}

CGAL_END_NAMESPACE
#endif //CGAL_SNC_FM_DECORATOR_H


