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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Boolean_set_operations_2/include/CGAL/General_polygon_set_2.h $
// $Id: General_polygon_set_2.h 28830 2006-02-27 14:26:55Z baruchzu $ $Date: 2006-02-27 15:26:55 +0100 (Mon, 27 Feb 2006) $
// 
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_GENERAL_POLYGON_SET_2_H
#define CGAL_GENERAL_POLYGON_SET_2_H

#include <CGAL/Boolean_set_operations_2/Gps_dcel.h>
#include <CGAL/Boolean_set_operations_2/Gps_do_intersect_functor.h>
#include <CGAL/Boolean_set_operations_2/Gps_intersection_functor.h>
#include <CGAL/Boolean_set_operations_2/Gps_join_functor.h>
#include <CGAL/Boolean_set_operations_2/Gps_difference_functor.h>
#include <CGAL/Boolean_set_operations_2/Gps_sym_diff_functor.h>
#include <CGAL/Boolean_set_operations_2/Gps_merge.h>

#include <CGAL/Boolean_set_operations_2/Gps_polygon_simplifier.h>


#include <CGAL/Arr_walk_along_line_point_location.h>

#include <CGAL/Arr_overlay.h>
#include <CGAL/Kernel_traits.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Object.h>
#include <CGAL/enum.h>
#include <CGAL/iterator.h> 
#include <CGAL/Boolean_set_operations_2/Ccb_curve_iterator.h>

CGAL_BEGIN_NAMESPACE

// General_polygon_set_2
template < class Traits_ >
class General_polygon_set_2 
{
public:

  typedef Traits_                                         Traits_2;
  typedef typename Traits_2::Polygon_2                    Polygon_2;
  typedef typename Traits_2::Polygon_with_holes_2         Polygon_with_holes_2;
  typedef Arrangement_2<Traits_2, Gps_dcel<Traits_2> >    Arrangement_2;
  typedef std::size_t                                     Size;

private:
  typedef General_polygon_set_2<Traits_>                  Self;
  typedef typename Traits_2::Point_2                      Point_2;
  typedef typename Traits_2::X_monotone_curve_2           X_monotone_curve_2;
  
  typedef typename Polygon_with_holes_2::Hole_const_iterator  
                                                 GP_Holes_const_iterator;
  typedef typename Traits_2::Curve_const_iterator         Curve_const_iterator;
  typedef typename Traits_2::Compare_endpoints_xy_2       Compare_endpoints_xy_2;
  typedef typename Traits_2::Construct_opposite_2         Construct_opposite_2;

  typedef typename Arrangement_2::Face_const_iterator        Face_const_iterator;
  typedef typename Arrangement_2::Halfedge_const_iterator    Halfedge_const_iterator;
  typedef typename Arrangement_2::Vertex_const_iterator      Vertex_const_iterator;
  typedef typename Arrangement_2::Edge_const_iterator        Edge_const_iterator;
  typedef typename Arrangement_2::Hole_const_iterator       Hole_const_iterator;
  typedef typename Arrangement_2::Ccb_halfedge_const_circulator 
                                                      Ccb_halfedge_const_circulator;
  typedef typename Arrangement_2::Face_iterator              Face_iterator;
  typedef typename Arrangement_2::Halfedge_iterator          Halfedge_iterator;
  typedef typename Arrangement_2::Vertex_iterator            Vertex_iterator;
  typedef typename Arrangement_2::Edge_iterator              Edge_iterator;
  typedef typename Arrangement_2::Hole_iterator             Hole_iterator;
  typedef typename Arrangement_2::Ccb_halfedge_circulator 
                                                      Ccb_halfedge_circulator;
  typedef typename Arrangement_2::Face_handle                Face_handle;
  typedef typename Arrangement_2::Halfedge_handle            Halfedge_handle;
  typedef typename Arrangement_2::Vertex_handle              Vertex_handle;

  typedef typename Arrangement_2::Face_const_handle          Face_const_handle;
  typedef typename Arrangement_2::Halfedge_const_handle      Halfedge_const_handle;
  typedef typename Arrangement_2::Vertex_const_handle        Vertex_const_handle;

  typedef typename Arrangement_2::Halfedge_around_vertex_const_circulator
                                            Halfedge_around_vertex_const_circulator;

  typedef std::pair<Arrangement_2 *, 
                    std::vector<Vertex_handle> *>            Arr_entry;
  typedef Arr_walk_along_line_point_location<Arrangement_2>  Walk_pl;

protected:

  Traits_2*             m_traits;
  bool                  m_traits_owner;

  // the underlying arrangement
  Arrangement_2*        m_arr;


public:

  // default costructor
  General_polygon_set_2() : m_traits(new Traits_2()),
                            m_traits_owner(true),
                            m_arr(new Arrangement_2(m_traits))       
  {}


  // constructor with traits object
  General_polygon_set_2(Traits_2& tr) : m_traits(&tr),
                                        m_traits_owner(false),
                                        m_arr(new Arrangement_2(m_traits)) 
  {}


  General_polygon_set_2(const Self& ps):  m_traits(new Traits_2(*(ps.m_traits))),
                                                           m_traits_owner(true),
                                                           m_arr(new Arrangement_2(*(ps.m_arr)))
  {}

  
  General_polygon_set_2& operator=(const Self& ps)
  {
    if(this == &ps)
      return (*this);

    if(m_traits_owner)
      delete m_traits;
    delete m_arr;
    m_traits = new Traits_2(*(ps.m_traits));
    m_traits_owner = true;
    m_arr = new Arrangement_2(*(ps.m_arr));
    return (*this);
  }


  explicit General_polygon_set_2(const Polygon_2& pgn) : 
    m_traits(new Traits_2()),
    m_traits_owner(true),
    m_arr(new Arrangement_2(m_traits)) 
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _insert(pgn, *m_arr);
  }

  explicit General_polygon_set_2(const Polygon_with_holes_2& pgn_with_holes): 
    m_traits(new Traits_2()),
    m_traits_owner(true),
    m_arr(new Arrangement_2(m_traits))
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn_with_holes));
    _insert(pgn_with_holes, *m_arr);
  }

private:
  General_polygon_set_2(Arrangement_2* arr) : m_traits(new Traits_2()),
                                              m_traits_owner(true),
                                              m_arr(arr)
   {}

 public:



  //destructor
  virtual ~General_polygon_set_2()
  {
    delete m_arr;

    if(m_traits_owner)
      delete m_traits;
  }

  void simplify(const Polygon_2& pgn, Polygon_with_holes_2& res)
  {
    typedef Gps_polygon_simplifier<Arrangement_2>  Simplifier;

    Arrangement_2*  arr = new Arrangement_2();

    Simplifier simp(*arr, *m_traits);
    simp.simplify(pgn);
    _remove_redundant_edges(arr);
    Self gps(arr);
    gps._reset_faces();
  
    typedef Oneset_iterator<Polygon_with_holes_2>    OutputItr;
    OutputItr oi (res);
    gps.polygons_with_holes(oi);
  }

  // insert a simple polygon
  void insert(const Polygon_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _insert(pgn, *m_arr);
  }

  // insert a polygon with holes
  void insert(const Polygon_with_holes_2& pgn_with_holes)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn_with_holes));
    _insert(pgn_with_holes, *m_arr);
  }


  // insert a range of polygons that can be either simple polygons
  // or polygons with holes
  // precondition: the polygons are disjoint and simple
  template <class PolygonIterator>
  void insert(PolygonIterator pgn_begin, PolygonIterator pgn_end);


  // insert two ranges of : the first one for simple polygons,
  // the second one for polygons with holes
  // precondition: the first range is disjoint simple polygons 
  //               the second range is disjoint polygons with holes
  template <class PolygonIterator, class PolygonWithHolesIterator>
  void insert(PolygonIterator pgn_begin,
              PolygonIterator pgn_end,
              PolygonWithHolesIterator  pgn_with_holes_begin,
              PolygonWithHolesIterator  pgn_with_holes_end);



   // test for intersection with a simple polygon
  bool do_intersect(const Polygon_2 &pgn) const
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    Self other(pgn);
    return (do_intersect(other));
  }

  // test for intersection with a polygon with holes
  bool do_intersect(const Polygon_with_holes_2& pgn_with_holes) const
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn_with_holes));
    Self other(pgn_with_holes);
    return (do_intersect(other));
  }

  //test for intersection with another General_polygon_set_2 object
  bool do_intersect(const Self& other) const
  {
    if(this->is_empty() || other.is_empty())
      return false;

    if(this->is_plane() || other.is_plane())
      return true;
    
    Arrangement_2 res_arr;

    Gps_do_intersect_functor<Arrangement_2>  func;
    overlay(*m_arr, *(other.m_arr), res_arr, func);
    return func.found_reg_intersection();
  }

  template <class InputIterator>
  bool do_intersect(InputIterator begin, InputIterator end)
  {
    Self other(*this);
    other.intersection(begin, end);
    return (other.is_empty());
  }

  template <class InputIterator1, class InputIterator2>
  bool do_intersect(InputIterator1 begin1, InputIterator1 end1,
                    InputIterator2 begin2, InputIterator2 end2)
  {
    Self other(*this);
    other.intersection(begin1, end1, begin2, end2);
    return (other.is_empty());
  }


  // intersection with a simple polygon
  void intersection(const Polygon_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _intersection(pgn);
  }

  // intersection with a polygon with holes
  void intersection(const Polygon_with_holes_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _intersection(pgn);
  }

  //intersection with another General_polygon_set_2 object
  void intersection(const Self& other)
  {
    _intersection(other);
  }

  void intersection(const Self& gps1, const Self& gps2)
  {
    this->clear();
    _intersection(*(gps1.m_arr), *(gps2.m_arr), *(this->m_arr));
  }


  // join with a simple polygon
  void join(const Polygon_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _join(pgn);
  }

  // join with a polygon with holes
  void join(const Polygon_with_holes_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _join(pgn);
  }

  //join with another General_polygon_set_2 object
  void join(const Self& other)
  {
    _join(other);
  }

  void join(const Self& gps1, const Self& gps2)
  {
    this->clear();
    _join(*(gps1.m_arr), *(gps2.m_arr), *(this->m_arr));
  }


   // difference with a simple polygon
  void difference (const Polygon_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _difference(pgn);
  }

  // difference with a polygon with holes
  void difference (const Polygon_with_holes_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _difference(pgn);
  }

  //difference with another General_polygon_set_2 object
  void difference (const Self& other)
  {
    _difference(other);
  }

  void difference(const Self& gps1, const Self& gps2)
  {
    this->clear();
    _difference(*(gps1.m_arr), *(gps2.m_arr), *(this->m_arr));
  }


  // symmetric_difference with a simple polygon
  void symmetric_difference(const Polygon_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _symmetric_difference(pgn);
  }

  // symmetric_difference with a polygon with holes
  void symmetric_difference(const Polygon_with_holes_2& pgn)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    _symmetric_difference(pgn);
  }

  //symmetric_difference with another General_polygon_set_2 object
  void symmetric_difference(const Self& other)
  {
    _symmetric_difference(other);
  }

  void symmetric_difference(const Self& gps1, const Self& gps2)
  {
    this->clear();
    _symmetric_difference(*(gps1.m_arr), *(gps2.m_arr), *(this->m_arr));
  }


  void complement()
  {
    this->_complement(m_arr);
  }

  void complement(const Self& other)
  {
    *(this->m_arr) = *(other.m_arr);
    this->complement();
  }

  // TODO: move to private (or prot. area)
  void _complement(Arrangement_2* arr)
  {
    for(Face_iterator fit = arr->faces_begin();
        fit != arr->faces_end();
        ++fit)
    {
      fit->set_contained(!fit->contained());
    }

    Construct_opposite_2 ctr_opp = m_traits->construct_opposite_2_object();
    for(Edge_iterator eit = arr->edges_begin();
        eit != arr->edges_end();
        ++eit)
    {
      Halfedge_handle he = eit;
      const X_monotone_curve_2& cv = he->curve();
      arr->modify_edge(he, ctr_opp(cv));
    }
  }

  //fix the directions of the curves (given correct marked face)
  // it should be called mostly after  symmetric_difference.
  void _fix_curves_direction(Arrangement_2& arr)
  {
    Compare_endpoints_xy_2 cmp_endpoints =
      m_traits->compare_endpoints_xy_2_object();
    Construct_opposite_2 ctr_opp = m_traits->construct_opposite_2_object();

    for(Edge_iterator eit = arr.edges_begin();
        eit != arr.edges_end();
        ++eit)
    {
      Halfedge_handle he = eit;
      X_monotone_curve_2&  cv = he->curve();
      bool is_cont = he->face()->contained();
      bool has_same_dir = (cmp_endpoints(cv) == he->direction());
      if((is_cont && !has_same_dir) ||
         (!is_cont && has_same_dir))
        arr.modify_edge(he, ctr_opp(cv));
    }
  }

  void fix_curves_direction()
  {
    _fix_curves_direction(*m_arr);
  }
         
  Size number_of_polygons_with_holes() const;

  Traits_2& traits()
  {
    return *m_traits;
  }

  const Traits_2& traits() const
  {
    return *m_traits;
  }

  bool is_empty() const
  {
    return (m_arr->is_empty() && ! m_arr->unbounded_face()->contained());
  }

  bool is_plane() const
  {
    return (m_arr->is_empty() &&  m_arr->unbounded_face()->contained());
  }

  void clear()
  {
    m_arr->clear();
  }

  
  Oriented_side oriented_side(const Point_2& q) const
  {
    Walk_pl pl(*m_arr);

    Object obj = pl.locate(q);
    Face_const_iterator f;
    if(CGAL::assign(f, obj))
    {
      if(f->contained())
        return ON_POSITIVE_SIDE;

      return ON_NEGATIVE_SIDE ;
    }  
    return ON_ORIENTED_BOUNDARY ;
  }

  Oriented_side oriented_side(const Polygon_2& pgn) const
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    Self other(pgn);
    return (oriented_side(other));
  }

  Oriented_side oriented_side(const Polygon_with_holes_2& pgn) const
  {
    CGAL_precondition(m_traits->is_valid_2_object()(pgn));
    Self other(pgn);
    return (oriented_side(other));
  }

  Oriented_side oriented_side(const Self& other) const
  {
     if(this->is_empty() || other.is_empty())
      return ON_NEGATIVE_SIDE;

    if(this->is_plane() || other.is_plane())
      return ON_POSITIVE_SIDE;
    
    Arrangement_2 res_arr;

    Gps_do_intersect_functor<Arrangement_2>  func;
    overlay(*m_arr, *(other.m_arr), res_arr, func);
    if (func.found_reg_intersection())
      return ON_POSITIVE_SIDE;
    
    if(func.found_boundary_intersection())
      return ON_ORIENTED_BOUNDARY;

    return ON_NEGATIVE_SIDE;
  }


  // returns the location of the query point
  bool locate(const Point_2& q, Polygon_with_holes_2& pgn) const;

  //advanced function: get const reference to the arrangement
  const Arrangement_2& arrangement() const
  {
    return *m_arr;
  }

  bool is_valid() const
  {
    if(!CGAL::is_valid(*m_arr))
      return false;

    Compare_endpoints_xy_2 cmp_endpoints =
      m_traits->compare_endpoints_xy_2_object();
    Construct_opposite_2 ctr_opp = m_traits->construct_opposite_2_object();

    for(Edge_const_iterator eci = m_arr->edges_begin();
        eci != m_arr->edges_end();
        ++eci)
    {
      Halfedge_const_handle he = eci;
      if(he->face() == he->twin()->face())
      {
        return false;
      }
      if(he->face()->contained() == he->twin()->face()->contained())
      {
        return false;
      }
      const X_monotone_curve_2&  cv = he->curve();
      bool is_cont = he->face()->contained();
      bool has_same_dir = (cmp_endpoints(cv) == he->direction());
      if((is_cont && !has_same_dir) ||
         (!is_cont && has_same_dir))
        return false;
    }
    return true;
  }


  // get the simple polygons, takes O(n)
  template <class OutputIterator>
  OutputIterator polygons_with_holes(OutputIterator out) const;

 // join a range of polygons
 template <class InputIterator>
 void join(InputIterator begin, InputIterator end, unsigned int k = 5)
{
  typename std::iterator_traits<InputIterator>::value_type pgn;
  this->join(begin, end, pgn, k);
  this->remove_redundant_edges();
  this->_reset_faces();
}

// join range of simple polygons
template <class InputIterator>
inline void join(InputIterator begin,
                 InputIterator end,
                 Polygon_2& pgn,
                 unsigned int k=5)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin, end) + 1);
 
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;
  for(InputIterator itr = begin; itr!=end; ++itr, ++i)
  {
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr, *(arr_vec[i].first));
  }

  Join_merge<Arrangement_2> join_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, join_merge);
  
  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
}


//join range of polygons with holes
template <class InputIterator>
inline void join(InputIterator begin,
                 InputIterator end,
                 Polygon_with_holes_2& pgn,
                 unsigned int k=5)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin, end) + 1);
  arr_vec[0].first = this->m_arr;
 
  unsigned int i = 1;
  for(InputIterator itr = begin; itr!=end; ++itr, ++i)
  {
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr, *(arr_vec[i].first));
  }

  Join_merge<Arrangement_2> join_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, join_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
}

template <class InputIterator1, class InputIterator2>
inline void join(InputIterator1 begin1,
                 InputIterator1 end1,
                 InputIterator2 begin2,
                 InputIterator2 end2,
                 unsigned int k=5)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin1, end1)+
                                  std::distance(begin2, end2)+1);
 
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;

  for(InputIterator1 itr1 = begin1; itr1!=end1; ++itr1, ++i)
  {
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr1, *(arr_vec[i].first));
  }

  for(InputIterator2 itr2 = begin2; itr2!=end2; ++itr2, ++i)
  {
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr2, *(arr_vec[i].first));
  }

  Join_merge<Arrangement_2> join_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, join_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
  this->remove_redundant_edges();
  this->_reset_faces();
}


// intersect range of polygins
template <class InputIterator>
inline void intersection(InputIterator begin, InputIterator end, unsigned int k=5)
{
  typename std::iterator_traits<InputIterator>::value_type pgn;
  this->intersection(begin, end, pgn, k);
  this->remove_redundant_edges();
  this->_reset_faces();
}


// intersect range of simple polygons
template <class InputIterator>
inline void intersection(InputIterator begin,
                         InputIterator end,
                         Polygon_2& pgn,
                         unsigned int k)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin, end) + 1);
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;
 
  for(InputIterator itr = begin; itr!=end; ++itr, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr, *(arr_vec[i].first));
  }

  Intersection_merge<Arrangement_2> intersection_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, intersection_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
}

//intersect range of polygons with holes
template <class InputIterator>
inline void intersection(InputIterator begin,
                         InputIterator end,
                         Polygon_with_holes_2& pgn,
                         unsigned int k)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin, end) + 1);
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;
 
  for(InputIterator itr = begin; itr!=end; ++itr, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr, *(arr_vec[i].first));
  }

  Intersection_merge<Arrangement_2> intersection_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, intersection_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
}


template <class InputIterator1, class InputIterator2>
inline void intersection(InputIterator1 begin1,
                         InputIterator1 end1,
                         InputIterator2 begin2,
                         InputIterator2 end2,
                         unsigned int k=5)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin1, end1)+
                                  std::distance(begin2, end2)+1);
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;
 
  for(InputIterator1 itr1 = begin1; itr1!=end1; ++itr1, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr1));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr1, *(arr_vec[i].first));
  }

  for(InputIterator2 itr2 = begin2; itr2!=end2; ++itr2, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr2));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr2, *(arr_vec[i].first));
  }

  Intersection_merge<Arrangement_2> intersection_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, intersection_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
  this->remove_redundant_edges();
  this->_reset_faces();
}



// symmetric_difference of a range of polygons (similar to xor)
template <class InputIterator>
inline void symmetric_difference(InputIterator begin, InputIterator end,
                                 unsigned int k=5)
{
  typename std::iterator_traits<InputIterator>::value_type pgn;
  this->symmetric_difference(begin, end, pgn, k);
  this->remove_redundant_edges();
  this->_reset_faces();
}


// intersect range of simple polygons
template <class InputIterator>
inline void symmetric_difference(InputIterator begin,
                                 InputIterator end,
                                 Polygon_2& pgn,
                                 unsigned int k=5)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin, end) + 1);
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;
 
  for(InputIterator itr = begin; itr!=end; ++itr, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr, *(arr_vec[i].first));
  }

  Xor_merge<Arrangement_2> xor_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, xor_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
}

//intersect range of polygons with holes
template <class InputIterator>
inline void symmetric_difference(InputIterator begin,
                                 InputIterator end,
                                 Polygon_with_holes_2& pgn,
                                 unsigned int k=5)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin, end) + 1);
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;
 
  for(InputIterator itr = begin; itr!=end; ++itr, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr, *(arr_vec[i].first));
  }

  Xor_merge<Arrangement_2> xor_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, xor_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
}


template <class InputIterator1, class InputIterator2>
inline void symmetric_difference(InputIterator1 begin1,
                                 InputIterator1 end1,
                                 InputIterator2 begin2,
                                 InputIterator2 end2,
                                 unsigned int k=5)
{
  std::vector<Arr_entry> arr_vec (std::distance(begin1, end1)+
                                  std::distance(begin2, end2)+1);
  arr_vec[0].first = this->m_arr;
  unsigned int i = 1;
 
  for(InputIterator1 itr1 = begin1; itr1!=end1; ++itr1, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr1));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr1, *(arr_vec[i].first));
  }

  for(InputIterator2 itr2 = begin2; itr2!=end2; ++itr2, ++i)
  {
    CGAL_precondition(m_traits->is_valid_2_object()(*itr2));
    arr_vec[i].first = new Arrangement_2(m_traits);
    _insert(*itr2, *(arr_vec[i].first));
  }

  Xor_merge<Arrangement_2> xor_merge;
  _build_sorted_vertices_vectors (arr_vec);
  _divide_and_conquer(0, arr_vec.size()-1, arr_vec, k, xor_merge);

  //the result arrangement is at index 0
  this->m_arr = arr_vec[0].first;
  delete arr_vec[0].second;
  this->remove_redundant_edges();
  this->_reset_faces();
}


static void construct_polygon(Ccb_halfedge_const_circulator ccb,
                              Polygon_2&              pgn,
                              Traits_2*               tr);


bool is_hole_of_face(Face_const_handle f,
                     Halfedge_const_handle he) const;

Ccb_halfedge_const_circulator get_boundary_of_polygon(Face_const_iterator f) const;


void remove_redundant_edges()
{
  this->_remove_redundant_edges(m_arr);
}

void _remove_redundant_edges(Arrangement_2* arr)
{
  for(Edge_iterator itr = arr->edges_begin(); 
      itr != arr->edges_end(); )
  {
    Halfedge_handle he = itr;
    if(he->face()->contained() == he->twin()->face()->contained())
    {
      Edge_iterator next = itr;
      ++next;
      arr->remove_edge(he);
      itr = next;
    }
    else
      ++itr;
  }
}
  
class Less_vertex_handle
{
  typename Traits_2::Compare_xy_2     comp_xy;

public:

  Less_vertex_handle (const typename Traits_2::Compare_xy_2& cmp) :
    comp_xy (cmp)
  {}

  bool operator() (Vertex_handle v1, Vertex_handle v2) const
  {
    return (comp_xy (v1->point(), v2->point()) == SMALLER);
  }
};
  
void _build_sorted_vertices_vectors (std::vector<Arr_entry>& arr_vec)
{
  Less_vertex_handle    comp (m_traits->compare_xy_2_object());
  Arrangement_2        *p_arr;
  Vertex_iterator       vit;
  const unsigned int    n = arr_vec.size();
  unsigned int          i, j;

  for (i = 0; i < n; i++)
  {
    // Allocate a vector of handles to all vertices in the current
    // arrangement.
    p_arr = arr_vec[i].first;
    arr_vec[i].second = new std::vector<Vertex_handle>;
    arr_vec[i].second->resize (p_arr->number_of_vertices());

    for (j = 0, vit = p_arr->vertices_begin();
         vit != p_arr->vertices_end();
         j++, ++vit)
    {
      (*(arr_vec[i].second))[j] = vit;
    }

    // Sort the vector.
    std::sort (arr_vec[i].second->begin(), arr_vec[i].second->end(),
               comp);
  }

  return;
}

template <class Merge>
void _divide_and_conquer (unsigned int lower,
                          unsigned int upper,
                          std::vector<Arr_entry>& arr_vec,
                          unsigned int k,
                          Merge merge_func)
{
  if((upper - lower) < k)
  {
    merge_func(lower, upper, 1, arr_vec);
    return;
  }

  unsigned int sub_size = ((upper - lower + 1) / k);
  
  unsigned int i=0;
  unsigned int curr_lower = lower;

  for(; i<k-1; ++i, curr_lower += sub_size )
  {
    _divide_and_conquer (curr_lower,
                         curr_lower + sub_size-1,
                         arr_vec,
                         k,
                         merge_func);
  }
  _divide_and_conquer (curr_lower, upper,arr_vec, k, merge_func);
  merge_func (lower, curr_lower, sub_size ,arr_vec);

  return;
}

// mark all faces as non-visited
void _reset_faces() const
{
  _reset_faces(m_arr);
}

void _reset_faces(Arrangement_2* arr) const
{
  Face_const_iterator fit = arr->faces_begin();
  for( ; fit != arr->faces_end(); ++fit)
  {
    fit->set_visited(false);
  }
}


void _insert(const Polygon_2& pgn, Arrangement_2& arr);

void _insert(const Polygon_with_holes_2& pgn, Arrangement_2& arr);

template< class PolygonIter >
void _insert(PolygonIter p_begin, PolygonIter  p_end, Polygon_2& pgn);

template< class PolygonIter >
void _insert(PolygonIter p_begin, PolygonIter  p_end, Polygon_with_holes_2& pgn);

template <class OutputIterator>
void _construct_curves(const Polygon_2& pgn, OutputIterator oi);

template <class OutputIterator>
void _construct_curves(const Polygon_with_holes_2& pgn, OutputIterator oi);


bool _is_empty(const Polygon_2& pgn) const
{
  const std::pair<Curve_const_iterator, Curve_const_iterator>& itr_pair = 
    m_traits->construct_curves_2_object()(pgn);
  return (itr_pair.first == itr_pair.second);
}

bool _is_empty(const Polygon_with_holes_2& pgn) const
{
  return (false);
}

bool _is_plane(const Polygon_2& pgn) const
{
  return (false);
}

bool _is_plane(const Polygon_with_holes_2& pgn) const
{
  return (pgn.is_unbounded() && (pgn.holes_begin() == pgn.holes_end()));
}

void _intersection(const Arrangement_2& arr)
{
   Arrangement_2* res_arr = new Arrangement_2(m_traits);
   Gps_intersection_functor<Arrangement_2> func;
   overlay(*m_arr, arr, *res_arr, func);
   delete m_arr; // delete the previous arrangement
    
   m_arr = res_arr;
   remove_redundant_edges();
}

void _intersection(const Arrangement_2& arr1,
                   const Arrangement_2& arr2,
                   Arrangement_2& res) 
{
   Gps_intersection_functor<Arrangement_2> func;
   overlay(arr1, arr2, res, func);
   _remove_redundant_edges(&res);

}

template <class Polygon_>
void _intersection(const Polygon_& pgn)
{
  if(_is_empty(pgn))
    this->clear();
  if(_is_plane(pgn))
    return;
  if(this->is_empty())
    return;
  
  if(this->is_plane())
  {
    Arrangement_2* arr = new Arrangement_2(m_traits);
    _insert(pgn, *arr);
    delete (this->m_arr);
    this->m_arr = arr;
    return;
  }

  Arrangement_2 second_arr;
   _insert(pgn, second_arr);
   _intersection(second_arr);
}

 void _intersection(const Self& other)
  {
    if(other.is_empty())
    {
      m_arr->clear();
      return;
    }
    
    if(other.is_plane())
      return;
    
    if(this->is_empty())
      return;
    
    if(this->is_plane())
    {
      *(this->m_arr) = *(other.m_arr);
      return;
    }
  
    _intersection(*(other.m_arr));
  }

 void _join(const Arrangement_2& arr)
{
   Arrangement_2* res_arr = new Arrangement_2(m_traits);
   Gps_join_functor<Arrangement_2> func;
   overlay(*m_arr, arr, *res_arr, func);
   delete m_arr; // delete the previous arrangement
    
   m_arr = res_arr;
   remove_redundant_edges();
}

void _join(const Arrangement_2& arr1,
           const Arrangement_2& arr2,
           Arrangement_2& res) 
{
   Gps_join_functor<Arrangement_2> func;
   overlay(arr1, arr2, res, func);
   _remove_redundant_edges(&res);

}

template <class Polygon_>
void _join(const Polygon_& pgn)
{
  if(_is_empty(pgn))
    return;
  if(_is_plane(pgn))
  {
    this->clear();
    this->m_arr->unbounded_face()->set_contained(true);
    return;
  }
  if(this->is_empty())
  {
    Arrangement_2* arr = new Arrangement_2(m_traits);
    _insert(pgn, *arr);
    delete (this->m_arr);
    this->m_arr = arr;
    return;
  }
  
  if(this->is_plane())
    return;

  Arrangement_2 second_arr;
   _insert(pgn, second_arr);
   _join(second_arr);
}


void _join(const Self& other)
{
  if(other.is_empty())
    return;
  if(other.is_plane())
  {
    this->clear();
    this->m_arr->unbounded_face()->set_contained(true);
    return;
  }
  if(this->is_empty())
  {
    *(this->m_arr) = *(other.m_arr);
     return;
  }
  if(this->is_plane())
    return;

   _join(*(other.m_arr));
}

void _difference(const Arrangement_2& arr)
{
   Arrangement_2* res_arr = new Arrangement_2(m_traits);
   Gps_difference_functor<Arrangement_2> func;
   overlay(*m_arr, arr, *res_arr, func);
   delete m_arr; // delete the previous arrangement
    
   m_arr = res_arr;
   remove_redundant_edges();
   fix_curves_direction();
}

void _difference(const Arrangement_2& arr1,
                 const Arrangement_2& arr2,
                 Arrangement_2& res) 
{
   Gps_difference_functor<Arrangement_2> func;
   overlay(arr1, arr2, res, func);
   _remove_redundant_edges(&res);
   _fix_curves_direction(res);

}

template <class Polygon_>
void _difference(const Polygon_& pgn)
{
  if(_is_empty(pgn))
    return;

  if(_is_plane(pgn))
  {
    this->clear();
    return;
  }
  if(this->is_empty())
    return;
  
  if(this->is_plane())
  {
    Arrangement_2* arr = new Arrangement_2(m_traits);
    _insert(pgn, *arr);
    delete (this->m_arr);
    this->m_arr = arr;
    this->complement();
    return;
  }

  Arrangement_2 second_arr;
   _insert(pgn, second_arr);
   _difference(second_arr);
}


void _difference(const Self& other)
{
  if(other.is_empty())
    return;
  if(other.is_plane())
  {
    this->clear();
    return;
  }
  if(this->is_empty())
  {
    return;
  }
  if(this->is_plane())
  {
    *(this->m_arr) = *(other.m_arr);
    this->complement();
    return;
  }

   _difference(*(other.m_arr));
}

void _symmetric_difference(const Arrangement_2& arr)
{
   Arrangement_2* res_arr = new Arrangement_2(m_traits);
   Gps_sym_diff_functor<Arrangement_2> func;
   overlay(*m_arr, arr, *res_arr, func);
   delete m_arr; // delete the previous arrangement
    
   m_arr = res_arr;
   remove_redundant_edges();
   fix_curves_direction();
}

void _symmetric_difference(const Arrangement_2& arr1,
                           const Arrangement_2& arr2,
                           Arrangement_2& res) 
{
   Gps_sym_diff_functor<Arrangement_2> func;
   overlay(arr1, arr2, res, func);
   _remove_redundant_edges(&res);
   _fix_curves_direction(res);
}

template <class Polygon_>
void _symmetric_difference(const Polygon_& pgn)
{
  if(_is_empty(pgn))
    return;

  if(_is_plane(pgn))
  {
    this->complement();
    return;
  }
  if(this->is_empty())
  {
    Arrangement_2* arr = new Arrangement_2(m_traits);
    _insert(pgn, *arr);
    delete (this->m_arr);
    this->m_arr = arr;
    return;
  }
  
  if(this->is_plane())
  {
    Arrangement_2* arr = new Arrangement_2(m_traits);
    _insert(pgn, *arr);
    delete (this->m_arr);
    this->m_arr = arr;
    this->complement();
    return;
  }

  Arrangement_2 second_arr;
   _insert(pgn, second_arr);
   _symmetric_difference(second_arr);
}


void _symmetric_difference(const Self& other)
{
  if(other.is_empty())
    return;

  if(other.is_plane())
  {
    this->complement();
    return;
  }
  if(this->is_empty())
  {
    *(this->m_arr) = *(other.m_arr);
    return;
  }
  
  if(this->is_plane())
  {
    *(this->m_arr) = *(other.m_arr);
    this->complement();
    return;
  }

   _symmetric_difference(*(other.m_arr));
}

};

#include <CGAL/Boolean_set_operations_2/Gps_utils.h>

CGAL_END_NAMESPACE

#endif
