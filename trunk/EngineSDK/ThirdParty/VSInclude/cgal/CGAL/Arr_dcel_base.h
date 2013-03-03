// Copyright (c) 2005  Tel-Aviv University (Israel).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_dcel_base.h $
// $Id: Arr_dcel_base.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Ron Wein <wein@post.tau.ac.il>
//                 (based on old version by: Iddo Hanniel and Oren Nechushtan)

#ifndef CGAL_ARR_DCEL_BASE_H
#define CGAL_ARR_DCEL_BASE_H

/*! \file
 * The definition of the base DCEL class and peripheral records.
 */

#include <CGAL/basic.h>
#include <list>
#include <map>
#include <CGAL/N_step_adaptor_derived.h>
#include <CGAL/In_place_list.h>
#include <CGAL/HalfedgeDS_iterator.h>
#include <CGAL/Arrangement_2/Arrangement_2_iterators.h>

CGAL_BEGIN_NAMESPACE

inline void* _clean_pointer (const void* p)
{
  const size_t  mask = ~1;
  const size_t  val = (reinterpret_cast<size_t>(p) & mask);

  return (reinterpret_cast<void*> (val));
}

inline void* _set_lsb (const void* p)
{
  const size_t  mask = 1;
  const size_t  val = (reinterpret_cast<size_t>(p) | mask);

  return (reinterpret_cast<void*> (val));
}

inline bool _is_lsb_set (const void* p)
{
  const size_t  mask = 1;
  const size_t  val = reinterpret_cast<size_t>(p);

  return ((val & mask) != 0); 
}

/*! \class
 * Base vertex class. 
 */
template <class Point_> class Arr_vertex_base 
{
public:

  typedef Point_       Point;

  /*! \struct
   * An auxiliary structure for rebinding the vertex with a new point class.
   */
  template<typename PNT>
  struct rebind
  {
    typedef Arr_vertex_base<PNT>             other;
  };

protected:

  void       *p_inc;  // An incident halfedge pointing at the vertex,
                      // or the isolated vertex information (in case it is
                      // isolated). The LSB of the pointer indicates whether
                      // the vertex is isolated.
  Point      *p_pt;   // The point associated with the vertex.

public:

  /*! Default constructor. */
  Arr_vertex_base() :
    p_inc (NULL),
    p_pt (NULL)
  {}
  
  /*! Destructor. */
  virtual ~Arr_vertex_base() {}

  /*! Get the point (const version). */
  const Point& point() const 
  { 
    return (*p_pt);
  }

  /*! Get the point (non-const version). */
  Point& point() 
  { 
    return (*p_pt);
  }

  /*! Set the point. */
  void set_point (Point *p) 
  {
    p_pt = p;
  }

  /*! Assign from another vertex. */
  virtual void assign (const Arr_vertex_base<Point>& v)
  {
    p_pt = v.p_pt;
  }
};

/*! \class
 * Base halfedge class.
 */
template <class X_monotone_curve_> class Arr_halfedge_base 
{
public:

  typedef X_monotone_curve_  X_monotone_curve;

  /*! \struct
   * An auxiliary structure for rebinding the halfedge with a new curve class.
   */
  template<typename XCV>
  struct rebind
  {
    typedef Arr_halfedge_base<XCV>           other;
  };

protected:

  void       *p_opp;   // The opposite halfedge.
  void       *p_prev;  // The previous halfedge in the component boundary.
  void       *p_next;  // The next halfedge in the component boundary.

  void       *p_v;     // The incident vertex (the target of the halfedge).
                       // The LSB of this pointer is used to store the
                       // direction of the halfedge.
  void       *p_comp;  // The component this halfedge belongs to: the incident
                       // face for outer CCBs and the hole information for
                       // inner CCBs. The LSB of the pointer indicates whether
                       // the halfedge lies on a hole boundary (inner CCB).
  
  X_monotone_curve *p_cv; // The associated x-monotone curve.

public:

  /*! Default constructor */
  Arr_halfedge_base() :
    p_opp (NULL),
    p_prev (NULL),
    p_next (NULL),
    p_v (NULL),
    p_comp (NULL),
    p_cv (NULL)
  {}

  /*! Destructor. */
  virtual ~Arr_halfedge_base()
  {}

  /*! Get the x-monotone curve (const version). */
  const X_monotone_curve& curve() const 
  {
    return (*p_cv);
  }

  /*! Get the x-monotone curve (non-const version). */
  X_monotone_curve& curve () 
  {
    return (*p_cv);
  }

  /*! Set the x-monotone curve. */
  void set_curve (X_monotone_curve* c)
  { 
    p_cv = c;

    // Set the curve for the opposite halfedge as well.
    Arr_halfedge_base<X_monotone_curve>* opp =
      reinterpret_cast<Arr_halfedge_base<X_monotone_curve>* > (p_opp);

    opp->p_cv = c;
  }

  /*! Assign from another halfedge. */
  virtual void assign (const Arr_halfedge_base<X_monotone_curve>& he)
  {
    p_cv = he.p_cv;
  }
};

/*!
 * Base face class.
 */
class Arr_face_base
{
public:

  typedef std::list<void*>                  Holes_container;
  typedef Holes_container::iterator         Hole_iterator;
  typedef Holes_container::const_iterator   Hole_const_iterator;

  typedef std::list<void*>                  Isolated_vertices_container;
  typedef Isolated_vertices_container::iterator
                                            Isolated_vertex_iterator;
  typedef Isolated_vertices_container::const_iterator
                                            Isolated_vertex_const_iterator;

protected:

  void           *p_he;        // An incident halfedge along the face boundary.
  Holes_container              holes;      // The holes inside the face.
  Isolated_vertices_container  iso_verts;  // The isolated vertices inside
                                           // the face.

public:

  /*! Default constructor. */
  Arr_face_base() :
    p_he (NULL),
    holes()
  {}

  /*! Destructor. */
  virtual ~Arr_face_base()
  {}

  /*! Assign from another face (does nothing). */
  virtual void assign (const Arr_face_base& )
  {}
};

// Forward declarations:
template <class V, class H, class F> class Arr_vertex;
template <class V, class H, class F> class Arr_halfedge;
template <class V, class H, class F> class Arr_face;
template <class V, class H, class F> class Arr_hole;
template <class V, class H, class F> class Arr_isolated_vertex;

/*! \class
 * The default arrangement DCEL vertex class.
 */
template <class V, class H, class F>
class Arr_vertex : public V,
                   public In_place_list_base<Arr_vertex<V,H,F> >
{
public:

  typedef V                           Base;
  typedef Arr_vertex<V,H,F>           Vertex;
  typedef Arr_halfedge<V,H,F>         Halfedge;
  typedef Arr_isolated_vertex<V,H,F>  Isolated_vertex;

  /*! Default constructor. */
  Arr_vertex() 
  {}

  /*! Check if the vertex is isolated. */
  bool is_isolated () const
  {
    // Note that we use the LSB of the p_inc pointer as a Boolean flag.
    return (_is_lsb_set (this->p_inc));
  }

  /*! Get an incident halfedge (const version). */
  const Halfedge* halfedge () const
  {
    CGAL_precondition (! is_isolated());
    return (reinterpret_cast<const Halfedge*>(this->p_inc));
  }

  /*! Get an incident halfedge (non-const version). */
  Halfedge* halfedge ()
  {
    CGAL_precondition (! is_isolated());
    return (reinterpret_cast<Halfedge*>(this->p_inc));
  }

  /*! Set an incident halfedge (for non-isolated vertices). */
  void set_halfedge (Halfedge* he)
  { 
    // Set the halfedge pointer and reset the LSB.
    this->p_inc = he;
  }

  /*! Get the isolated vertex information (const version). */
  const Isolated_vertex* isolated_vertex () const
  {
    CGAL_precondition (is_isolated());
    return (reinterpret_cast<const Isolated_vertex*>(_clean_pointer 
                                                     (this->p_inc)));
  }

  /*! Get the isolated vertex information (non-const version). */
  Isolated_vertex* isolated_vertex ()
  {
    CGAL_precondition (is_isolated());
    return (reinterpret_cast<Isolated_vertex*>(_clean_pointer (this->p_inc)));
  }

  /*! Set the isolated vertex information. */
  void set_isolated_vertex (Isolated_vertex* iv)
  { 
    // Set the isolated vertex-information pointer and set its LSB.
    this->p_inc = _set_lsb (iv);
  }
};

/*! \class
 * The default arrangement DCEL halfedge class.
 */
template <class V, class H, class F>
class Arr_halfedge : public H,
                     public In_place_list_base<Arr_halfedge<V,H,F> >
{
public:

  typedef H                     Base;
  typedef Arr_vertex<V,H,F>     Vertex;
  typedef Arr_halfedge<V,H,F>   Halfedge;
  typedef Arr_face<V,H,F>       Face;
  typedef Arr_hole<V,H,F>       Hole;

  /*! Default constructor. */
  Arr_halfedge()
  {}

  /*! Get the opposite halfedge (const version). */
  const Halfedge* opposite () const
  { 
    return (reinterpret_cast<const Halfedge*>(this->p_opp));
  }
  
  /*! Get the opposite halfedge (non-const version). */
  Halfedge* opposite ()
  { 
    return (reinterpret_cast<Halfedge*>(this->p_opp));
  }

  /*! Sets the opposite halfedge. */
  void set_opposite (Halfedge* he) 
  { 
    this->p_opp = he;
  }

  /*!
   * Get the direction of the halfedge (the result of the lexicoraphical
   * comparison between the source and the target endpoints).
   * \return SMALLER if the halfedge is directed to the right,
   *         LARGER if the halfedge is directed to the left.
   */
  Comparison_result direction () const
  {
    // Note that we use the LSB of the p_v pointer as a Boolean flag.
    if (_is_lsb_set (this->p_v))
      return (SMALLER);
    else
      return (LARGER);
  }

  /*! Set the direction of the edge (and of its opposite halfedge). */
  void set_direction (Comparison_result dir)
  {
    CGAL_precondition (dir != EQUAL);

    Halfedge*   opp = reinterpret_cast<Halfedge*> (this->p_opp);

    if (dir == SMALLER)
    {
      this->p_v = _set_lsb (this->p_v);
      opp->p_v = _clean_pointer (opp->p_v);
    }
    else
    {
      this->p_v = _clean_pointer (this->p_v);
      opp->p_v = _set_lsb (opp->p_v);
    }
  }

  /*! Get the previous halfedge along the chain (const version). */
  const Halfedge* prev () const
  {
    return (reinterpret_cast<const Halfedge*>(this->p_prev));
  }

  /*! Get the previous halfedge along the chain (const version). */
  Halfedge* prev ()
  {
    return (reinterpret_cast<Halfedge*>(this->p_prev));
  }

  /*! Set the previous halfedge along the chain. */
  void set_prev (Halfedge* he)
  {
    this->p_prev = he;
    he->p_next = this;
  }

  /*! Get the next halfedge along the chain (const version). */
  const Halfedge* next () const
  {
    return (reinterpret_cast<const Halfedge*>(this->p_next));
  }

  /*! Get the next halfedge along the chain (const version). */
  Halfedge* next ()
  {
    return (reinterpret_cast<Halfedge*>(this->p_next));
  }

  /*! Set the next halfedge along the chain. */
  void set_next (Halfedge* he)
  {
    this->p_next = he;
    he->p_prev = this;
  }

  /*! Get the target vertex (const version). */
  const Vertex* vertex () const 
  { 
    return (reinterpret_cast<const Vertex*>(_clean_pointer (this->p_v)));
  }

  /*! Get the target vertex (non-const version). */
  Vertex* vertex ()
  { 
    return (reinterpret_cast<Vertex*>(_clean_pointer (this->p_v)));
  }

  /*! Set the target vertex. */
  void set_vertex (Vertex* v)
  {
    // Set the vertex pointer, preserving the content of the LSB.
    if (_is_lsb_set (this->p_v))
      this->p_v = _set_lsb (v);
    else
      this->p_v = v;
  }

  /*! Check whether the halfedge lies on the boundary of a hole. */
  bool is_on_hole () const
  {
    return (_is_lsb_set (this->p_comp));
  }

  /*! Get an incident face (const version). */
  const Face* face () const
  {
    CGAL_precondition (! is_on_hole());
    return (reinterpret_cast<const Face*>(this->p_comp));
  }

  /*! Get an incident face (non-const version). */
  Face* face ()
  {
    CGAL_precondition (! is_on_hole());
    return (reinterpret_cast<Face*>(this->p_comp));
  }

  /*! Set the incident facee (for halfedges that lie on an outer CCB). */
  void set_face (Face* f)
  { 
    // Set the face pointer and reset the LSB.
    this->p_comp = f;
  }

  /*! Get the incident hole (const version). */
  const Hole* hole () const 
  {     
    CGAL_precondition (is_on_hole());
    return (reinterpret_cast<const Hole*> (_clean_pointer (this->p_comp)));
  }

  /*! Get the incident hole (non-const version). */
  Hole* hole () 
  {
    CGAL_precondition (is_on_hole());
    return (reinterpret_cast<Hole*> (_clean_pointer (this->p_comp)));
  }

  /*! Set the incident hole. */
  void set_hole (Hole *hole)
  { 
    this->p_comp = _set_lsb (hole);
  }
};

/*! \class
 * The default arrangement DCEL face class.
 */
template <class V, class H, class F>
class Arr_face : public F,
                 public In_place_list_base<Arr_face<V,H,F> >
{
public:

  typedef F                     Base;
  typedef Arr_vertex<V,H,F>     Vertex;
  typedef Arr_halfedge<V,H,F>   Halfedge;
  typedef Arr_face<V,H,F>       Face;

  /*! Default constructor. */
  Arr_face()
  {}

  /*! Get an incident halfedge (const version). */
  const Halfedge * halfedge() const
  {
    return (reinterpret_cast<const Halfedge*>(this->p_he));
  }

  /*! Get an incident halfedge (non-const version). */
  Halfedge * halfedge()
  {
    return (reinterpret_cast<Halfedge*>(this->p_he));
  }

  /*! Set an incident halfedge. */
  void set_halfedge (Halfedge* he)
  {
    this->p_he = he;
  }

  // Define the hole iterators:
  typedef I_HalfedgeDS_iterator<
    typename F::Hole_iterator,
    Halfedge*,
    typename F::Hole_iterator::difference_type,
    typename F::Hole_iterator::iterator_category>       Hole_iterator;

  typedef I_HalfedgeDS_const_iterator<
    typename F::Hole_const_iterator,
    typename F::Hole_iterator,
    const Halfedge*,
    typename F::Hole_const_iterator::difference_type,
    typename F::Hole_const_iterator::iterator_category> Hole_const_iterator;

  /*! Get the number of holes inside the face. */
  size_t number_of_holes() const
  {
    return (this->holes.size());
  }

  /*! Add a hole inside the face. */
  Hole_iterator add_hole (Halfedge* h)
  {
    return (this->holes.insert (this->holes.end(), h));
  }

  /*! Erase a hole from the face. */
  void erase_hole (Hole_iterator hit)
  {
    this->holes.erase (hit.current_iterator());
  }

  /*! Get an iterator for the first hole inside the face. */
  Hole_iterator holes_begin()
  {
    return (this->holes.begin());
  }

  /*! Get a past-the-end iterator for the holes inside the face. */
  Hole_iterator holes_end()
  {
    return (this->holes.end());
  }

  /*! Get an const iterator for the first hole inside the face. */
  Hole_const_iterator holes_begin() const
  {
    return (this->holes.begin());
  }

  /*! Get a const past-the-end iterator for the holes inside the face. */
  Hole_const_iterator holes_end() const
  {
    return (this->holes.end());
  }

  // Define the isloated vertices iterators:
  typedef I_Dereference_iterator<
    typename F::Isolated_vertex_iterator,
    Vertex,
    typename F::Isolated_vertex_iterator::difference_type,
    typename F::Isolated_vertex_iterator::iterator_category>
                                              Isolated_vertex_iterator;
  
  typedef I_Dereference_const_iterator<
    typename F::Isolated_vertex_const_iterator,
    typename F::Isolated_vertex_iterator,
    Vertex,
    typename F::Isolated_vertex_iterator::difference_type,
    typename F::Isolated_vertex_iterator::iterator_category>
                                              Isolated_vertex_const_iterator;

  /*! Get the number of isloated vertices inside the face. */
  size_t number_of_isolated_vertices() const
  {
    return (this->iso_verts.size());
  }

  /*! Add an isloated vertex inside the face. */
  Isolated_vertex_iterator add_isolated_vertex (Vertex* v)
  {
    return (this->iso_verts.insert (this->iso_verts.end(), v));
  }

  /*! Erase an isloated vertex from the face. */
  void erase_isolated_vertex (Isolated_vertex_iterator ivit)
  {
    this->iso_verts.erase (ivit.current_iterator());
  }

  /*! Get an iterator for the first isloated vertex inside the face. */
  Isolated_vertex_iterator isolated_vertices_begin()
  {
    return (this->iso_verts.begin());
  }

  /*! Get a past-the-end iterator for the isloated vertices inside the face. */
  Isolated_vertex_iterator isolated_vertices_end()
  {
    return (this->iso_verts.end());
  }

  /*! Get an const iterator for the first isloated vertex inside the face. */
  Isolated_vertex_const_iterator isolated_vertices_begin() const
  {
    return (this->iso_verts.begin());
  }

  /*! Get a const past-the-end iterator for the isloated vertices inside the
   * face. */
  Isolated_vertex_const_iterator isolated_vertices_end() const
  {
    return (this->iso_verts.end());
  }
};

template <class V, class H, class F>
class Arr_hole : public In_place_list_base<Arr_hole<V,H,F> >
{
public:

  typedef Arr_hole<V,H,F>                Self;
  typedef Arr_face<V,H,F>                Face;
  typedef typename Face::Hole_iterator  Hole_iterator;

private:

  Face            *p_f;      // The face the contains the hole in its interior.
  Hole_iterator   hole_it;  // The hole identifier.

public:

  /*! Default constructor. */
  Arr_hole () 
  {}

  /*! Get the incident face (const version). */
  const Face* face () const
  {
    return (p_f);
  }

  /*! Get the incident face (non-const version). */
  Face* face ()
  {
    return (p_f);
  }

  /*! Set the incident face, the one that contains the hole. */
  void set_face (Face* f)
  {
    p_f = f;
    return;
  }

  /*! Get the hole iterator (const version). */
  Hole_iterator iterator () const
  {
    return (hole_it);
  }

  /*! Get the hole iterator (non-const version). */
  Hole_iterator iterator ()
  {
    return (hole_it);
  }

  /*! Set the hole iterator. */
  void set_iterator (Hole_iterator hole)
  {
    hole_it = hole;
    return;
  }

};

template <class V, class H, class F>
class Arr_isolated_vertex : 
public In_place_list_base<Arr_isolated_vertex<V,H,F> >
{
public:

  typedef Arr_isolated_vertex<V,H,F>                Self;
  typedef Arr_face<V,H,F>                           Face;
  typedef typename Face::Isolated_vertex_iterator Isolated_vertex_iterator;

private:

  Face                        *p_f;   // The face containing the hole.
  Isolated_vertex_iterator   iv_it; // The isolated vertex identifier.

public:

  /*! Default constructor. */
  Arr_isolated_vertex () 
  {}

  /*! Get the containing face (const version). */
  const Face* face () const
  {
    return (p_f);
  }

  /*! Get the containing face (non-const version). */
  Face* face ()
  {
    return (p_f);
  }

  /*! Set the incident face, the one that contains the isolated vertex. */
  void set_face (Face* f)
  {
    p_f = f;
    return;
  }

  /*! Get the isolated vertex iterator (const version). */
  Isolated_vertex_iterator iterator () const
  {
    return (iv_it);
  }

  /*! Get the isolated vertex iterator (non-const version). */
  Isolated_vertex_iterator iterator ()
  {
    return (iv_it);
  }

  /*! Set the isolated vertex iterator. */
  void set_iterator (Isolated_vertex_iterator iv)
  {
    iv_it = iv;
    return;
  }
};

/*! \class
 * The arrangement DCEL class.
 */
template <class V, class H, class F,
          class Allocator = CGAL_ALLOCATOR(int) >
class Arr_dcel_base
{
public:

  // Define the vertex, halfedge and face types.
  typedef Arr_dcel_base<V,H,F>        Self;
  typedef Arr_vertex<V,H,F>           Vertex;
  typedef Arr_halfedge<V,H,F>         Halfedge;
  typedef Arr_face<V,H,F>             Face;
  typedef Arr_hole<V,H,F>             Hole;
  typedef Arr_isolated_vertex<V,H,F>  Isolated_vertex;

protected:

  // The vetices, halfedges and faces are stored in three in-place lists.
  typedef In_place_list<Vertex, false>          Vertex_list;
  typedef In_place_list<Halfedge, false>        Halfedge_list;
  typedef In_place_list<Face, false>            Face_list;
  typedef In_place_list<Hole, false>            Hole_list;
  typedef In_place_list<Isolated_vertex, false> Iso_vert_list;

  // Vertex allocator.
  typedef typename Allocator::template rebind<Vertex>    Vertex_alloc_rebind;
  typedef typename Vertex_alloc_rebind::other            Vertex_allocator;

  // Halfedge allocator.
  typedef typename Allocator::template rebind<Halfedge>  Halfedge_alloc_rebind;
  typedef typename Halfedge_alloc_rebind::other          Halfedge_allocator;

  // Face allocator.
  typedef typename Allocator::template rebind<Face>      Face_alloc_rebind;
  typedef typename Face_alloc_rebind::other              Face_allocator;
  
  // Hole allocator.
  typedef typename Allocator::template rebind<Hole>      Hole_alloc_rebind;
  typedef typename Hole_alloc_rebind::other              Hole_allocator;

  // Isolated vertex allocator.
  typedef typename Allocator::template rebind<Isolated_vertex>
                                                         Iso_vert_alloc_rebind;
  typedef typename Iso_vert_alloc_rebind::other          Iso_vert_allocator;

public:

  typedef typename Halfedge_list::size_type             Size;
  typedef typename Halfedge_list::size_type             size_type;
  typedef typename Halfedge_list::difference_type       difference_type;
  typedef typename Halfedge_list::difference_type       Difference;
  typedef std::bidirectional_iterator_tag               iterator_category;

protected:

  Vertex_list         vertices;             // The vertices container.
  Halfedge_list       halfedges;            // The halfedges container.
  Face_list           faces;                // The faces container.
  Hole_list           holes;                // The holes (inner components).
  Iso_vert_list       iso_verts;            // The isolated vertices.

  Vertex_allocator    vertex_alloc;         // An allocator for vertices.
  Halfedge_allocator  halfedge_alloc;       // An allocator for halfedges.
  Face_allocator      face_alloc;           // An allocator for faces.
  Hole_allocator      hole_alloc;           // An allocator for holes.
  Iso_vert_allocator  iso_vert_alloc;       // Allocator for isolated vertices.

public:

  // Definitions of iterators.
  typedef typename Vertex_list::iterator              Vertex_iterator;
  typedef typename Halfedge_list::iterator            Halfedge_iterator;
  typedef typename Face_list::iterator                Face_iterator;
  typedef CGAL::N_step_adaptor_derived<Halfedge_iterator, 
                                       2>             Edge_iterator;
  
  // Definitions of const iterators.
  typedef typename Vertex_list::const_iterator        Vertex_const_iterator;
  typedef typename Halfedge_list::const_iterator      Halfedge_const_iterator;
  typedef typename Face_list::const_iterator          Face_const_iterator;
  typedef CGAL::N_step_adaptor_derived<Halfedge_const_iterator, 
                                       2>             Edge_const_iterator;

private:

  // Copy Constructor - not supported.
  Arr_dcel_base (const Self& ) ;

  // Assignment operator - not supported.
  Self& operator= (const Self& );

public:

  /// \name Construction and destruction.
  //@{
  /*! Default constructor. */
  Arr_dcel_base ()
  {}
  
  /*! Destructor. */
  ~Arr_dcel_base ()
  {
    delete_all();
  }
  //@}

  /// \name The DCEL size.
  //@{
  /*! Get the number of DCEL vertices. */
  Size size_of_vertices () const
  { 
    return (vertices.size());
  }

  /*! Get the number of DCEL halfedges (twice the number of edges). */
  Size size_of_halfedges () const
  {
    return (halfedges.size());
  }

  /*! Get the number of DCEL faces. */
  Size size_of_faces() const
  {
    return (faces.size());
  }

  /*! Get the number of holes. */
  Size size_of_holes() const
  {
    return (holes.size());
  }

  /*! Get the number of isolated vertices. */
  Size size_of_isolated_vertices () const
  {
    return (iso_verts.size());
  }
  //@}

  /// \name Obtaining iterators.
  //@{
  Vertex_iterator   vertices_begin()  { return vertices.begin(); }
  Vertex_iterator   vertices_end()    { return vertices.end(); }
  Halfedge_iterator halfedges_begin() { return halfedges.begin();}
  Halfedge_iterator halfedges_end()   { return halfedges.end(); }
  Face_iterator     faces_begin()     { return faces.begin(); }
  Face_iterator     faces_end()       { return faces.end(); }
  Edge_iterator     edges_begin()     { return halfedges.begin(); }  
  Edge_iterator     edges_end()       { return halfedges.end(); }  
  //@}

  /// \name Obtaining constant iterators.
  //@{
  Vertex_const_iterator   vertices_begin() const { return vertices.begin(); }
  Vertex_const_iterator   vertices_end() const { return vertices.end(); }
  Halfedge_const_iterator halfedges_begin() const { return halfedges.begin(); }
  Halfedge_const_iterator halfedges_end() const { return halfedges.end(); }
  Face_const_iterator     faces_begin() const { return faces.begin(); }
  Face_const_iterator     faces_end() const { return faces.end(); }
  Edge_const_iterator     edges_begin() const { return halfedges.begin(); }  
  Edge_const_iterator     edges_end() const { return halfedges.end(); }  
  //@}

  // \name Creation of new DCEL features.
  //@{
  /*! Create a new vertex. */
  Vertex* new_vertex()
  {
    Vertex     *v = vertex_alloc.allocate (1);

    vertex_alloc.construct (v, Vertex());
    vertices.push_back (*v);
    return v;
  }
  
  /*! Create a new pair of opposite halfedges. */
  Halfedge* new_edge() 
  {
    // Create two new halfedges.
    Halfedge   *h1 = _new_halfedge ();
    Halfedge   *h2 = _new_halfedge ();

    // Pair them together.
    h1->set_opposite (h2);
    h2->set_opposite (h1);

    return (h1);
  }

  /*! Create a new face. */
  Face* new_face()
  {
    Face       *f = face_alloc.allocate (1);
    
    face_alloc.construct (f, Face());
    faces.push_back (*f);
    return (f);
  }

  /*! Create a new hole. */
  Hole* new_hole ()
  {
    Hole       *h = hole_alloc.allocate (1);
    
    hole_alloc.construct (h, Hole());
    holes.push_back (*h);
    return (h);
  }

  /*! Create a new isolated vertex. */
  Isolated_vertex* new_isolated_vertex ()
  {
    Isolated_vertex  *iv = iso_vert_alloc.allocate (1);
    
    iso_vert_alloc.construct (iv, Isolated_vertex());
    iso_verts.push_back (*iv);
    return (iv);
  }
  //@}

  /// \name Deletion of DCEL features.
  //@{
  /*! Delete an existing vertex. */
  void delete_vertex (Vertex * v)
  {
    vertices.erase (v);
    vertex_alloc.destroy (v);
    vertex_alloc.deallocate (v,1);
  }
  
  /*! Delete an existing pair of opposite halfedges. */
  void delete_edge (Halfedge * h) 
  {
    Halfedge   *h_opp = h->opposite();

    _delete_halfedge (h);
    _delete_halfedge (h_opp);
  }

  /*! Delete an existing face. */
  void delete_face(Face * f)
  {
    faces.erase (f);
    face_alloc.destroy (f);
    face_alloc.deallocate (f, 1);
  }

  /*! Delete an existing hole. */
  void delete_hole (Hole * h)
  {
    holes.erase (h);
    hole_alloc.destroy (h);
    hole_alloc.deallocate (h, 1);
  }

  /*! Delete an existing isolated vertex. */
  void delete_isolated_vertex (Isolated_vertex * iv)
  {
    iso_verts.erase (iv);
    iso_vert_alloc.destroy (iv);
    iso_vert_alloc.deallocate (iv, 1);
  }
  
  /*! Delete all DCEL features. */
  void delete_all() 
  {
    // Free all vertices.
    Vertex_iterator    vit = vertices.begin(), v_curr;

    while (vit != vertices.end())
    {
      v_curr = vit;
      ++vit;
      delete_vertex (&(*v_curr));
    }

    // Free all halfedges.
    Halfedge_iterator  hit = halfedges.begin(), h_curr;

    while (hit != halfedges.end())
    {
      h_curr = hit;
      ++hit;
      _delete_halfedge (&(*h_curr));
    }

    // Free all faces.
    Face_iterator      fit = faces.begin(), f_curr;

    while (fit != faces.end())
    {
      f_curr = fit;
      ++fit;
      delete_face (&(*f_curr));
    }

    // Free all holes.
    typename Hole_list::iterator   hoit = holes.begin(), ho_curr;

    while (hoit != holes.end())
    {
      ho_curr = hoit;
      ++hoit;
      delete_hole (&(*ho_curr));
    }

    // Free all isolated vertices.
    typename Iso_vert_list::iterator   ivit = iso_verts.begin(), iv_curr;

    while (ivit != iso_verts.end())
    {
      iv_curr = ivit;
      ++ivit;
      delete_isolated_vertex (&(*iv_curr));
    }
  }
  //@}

  /*!
   * Assign our DCEL the contents of another DCEL.
   * \param dcel The DCEL to be copied.
   * \param uf A pointer to the unbounded face in this DCEL.
   * \return A pointer to the unbounded face in the copied DCEL.
   */
  Face* assign (const Self& dcel, const Face *uf)
  {
    // Clear the current contents of the DCEL.
    delete_all();

    // Create duplicated of the DCEL features and map the features of the
    // given DCEL to their corresponding duplicates.
    typedef std::map<const Vertex*, Vertex*>     Vertex_map;
    typedef std::map<const Halfedge*, Halfedge*> Halfedge_map;
    typedef std::map<const Face*, Face*>         Face_map;
    typedef std::map<const Hole*, Hole*>         Hole_map;
    typedef std::map<const Isolated_vertex*,
                     Isolated_vertex*>           Iso_vert_map;

    Vertex_map                v_map;
    Vertex_const_iterator     vit;
    Vertex                   *dup_v;

    for (vit = dcel.vertices_begin(); vit != dcel.vertices_end(); ++vit)
    {
      dup_v = new_vertex();
      dup_v->assign (*vit);
      v_map.insert (typename Vertex_map::value_type (&(*vit), dup_v));
    }

    Halfedge_map              he_map;
    Halfedge_const_iterator   hit;
    Halfedge                 *dup_h;

    for (hit = dcel.halfedges_begin(); hit != dcel.halfedges_end(); ++hit)
    {
      dup_h = _new_halfedge();
      dup_h->assign (*hit);
      he_map.insert (typename Halfedge_map::value_type(&(*hit), dup_h));
    }

    Face_map                  f_map;
    Face_const_iterator       fit;
    Face                     *dup_f;

    for (fit = dcel.faces_begin(); fit != dcel.faces_end(); ++fit)
    {
      dup_f = new_face();
      dup_f->assign (*fit);
      f_map.insert (typename Face_map::value_type(&(*fit), dup_f));
    }

    Hole_map                            ho_map;
    typename Hole_list::const_iterator  hoit;
    Hole                               *dup_ho;

    for (hoit = dcel.holes.begin(); hoit != dcel.holes.end(); ++hoit)
    {
      dup_ho = new_hole();
      ho_map.insert (typename Hole_map::value_type(&(*hoit), dup_ho));
    }

    Iso_vert_map                            iv_map;
    typename Iso_vert_list::const_iterator  ivit;
    Isolated_vertex                        *dup_iv;

    for (ivit = dcel.iso_verts.begin(); ivit != dcel.iso_verts.end(); ++ivit)
    {
      dup_iv = new_isolated_vertex();
      iv_map.insert (typename Iso_vert_map::value_type(&(*ivit), dup_iv));
    }

    // Update the vertex records.
    const Vertex             *v;
    const Halfedge           *h;
    const Face               *f;
    const Hole               *ho;
    const Isolated_vertex    *iv;
    
    for (vit = dcel.vertices_begin(); vit != dcel.vertices_end(); ++vit)
    {
      v = &(*vit);
      dup_v = (v_map.find (v))->second;

      if (v->is_isolated())
      {
        // Isolated vertex - set its information.
        iv = v->isolated_vertex();
        dup_iv = (iv_map.find (iv))->second;

        dup_v->set_isolated_vertex (dup_iv);
      }
      else
      {
        // Regular vertex - set its incident halfedge.
        h = v->halfedge();
        dup_h = (he_map.find (h))->second;

        dup_v->set_halfedge (dup_h);
      }
    }

    // Update the halfedge records.
    const Halfedge           *opp, *prev, *next;
    Halfedge                 *dup_opp, *dup_prev, *dup_next;

    for (hit = dcel.halfedges_begin(); hit != dcel.halfedges_end(); ++hit)
    {
      h = &(*hit);
      v = h->vertex();
      opp = h->opposite();
      prev = h->prev();
      next = h->next();

      dup_h = (he_map.find (h))->second;
      dup_v = (v_map.find (v))->second;
      dup_opp = (he_map.find (opp))->second;
      dup_prev = (he_map.find (prev))->second;
      dup_next = (he_map.find (next))->second;

      dup_h->set_vertex (dup_v);
      dup_h->set_opposite (dup_opp);
      dup_h->set_prev (dup_prev);
      dup_h->set_next (dup_next);
      dup_h->set_direction (h->direction());

      if (h->is_on_hole())
      {
        // The halfedge lies on a hole (inner CCB) - set its hole record.
        ho = h->hole();
        dup_ho = (ho_map.find (ho))->second;
        dup_h->set_hole (dup_ho);
      }
      else
      {
        // The halfedge lies on an outer CCB - set its incident face.
        f = h->face();
        dup_f = (f_map.find (f))->second;
        dup_h->set_face (dup_f);
      }
    }

    // Update the face records, along with the hole and isolated vertex
    // records.
    typename Face::Hole_const_iterator              holes_it;
    typename Face::Isolated_vertex_const_iterator  iso_verts_it;
    const Halfedge                      *hole;
    const Vertex                        *iso_vert;
    Halfedge                            *dup_hole;
    Vertex                              *dup_iso_vert;

    for (fit = dcel.faces_begin(); fit != dcel.faces_end(); ++fit)
    {
      f = &(*fit);
      h = f->halfedge();

      // Set the pointer to the outer boundary edge (may be NULL in case that
      // the current face f is the unbounded face).
      dup_f = (f_map.find (f))->second;
      if (h != NULL)
        dup_h = (he_map.find (h))->second;
      else
        dup_h = NULL;

      dup_f->set_halfedge (dup_h);

      // Assign the holes.
      for (holes_it = f->holes_begin();
           holes_it != f->holes_end(); ++holes_it)
      {
        hole = *holes_it;

        dup_hole = (he_map.find (hole))->second;
        dup_ho = dup_hole->hole();

        dup_ho->set_face (dup_f);
        dup_ho->set_iterator (dup_f->add_hole (dup_hole));
      }

      // Assign the isolated vertices.
      for (iso_verts_it = f->isolated_vertices_begin();
           iso_verts_it != f->isolated_vertices_end(); ++iso_verts_it)
      {
        iso_vert = &(*iso_verts_it);

        dup_iso_vert = (v_map.find (iso_vert))->second;
        dup_iv = dup_iso_vert->isolated_vertex();

        dup_iv->set_face (dup_f);
        dup_iv->set_iterator (dup_f->add_isolated_vertex (dup_iso_vert));
      }
    }

    // Return the unbounded face in the copied DCEL.
    return ((f_map.find (uf))->second);
  }

protected:

  /*! Create a new halfedge. */
  Halfedge * _new_halfedge ()
  {
    Halfedge   *h = halfedge_alloc.allocate (1);

    halfedge_alloc.construct (h, Halfedge());
    halfedges.push_back (*h);
    return (h);
  }

  /*! Delete an existing halfedge. */
  void _delete_halfedge (Halfedge* h)
  {
    halfedges.erase (h);
    halfedge_alloc.destroy (h);
    halfedge_alloc.deallocate (h, 1);
  }
};

CGAL_END_NAMESPACE

#endif 
