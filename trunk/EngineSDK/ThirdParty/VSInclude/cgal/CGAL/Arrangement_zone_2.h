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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arrangement_zone_2.h $
// $Id: Arrangement_zone_2.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Ron Wein          <wein@post.tau.ac.il>
//                 (based on old version by Eyal Flato)

#ifndef CGAL_ARRANGEMENT_ZONE_2_H
#define CGAL_ARRANGEMENT_ZONE_2_H

/*! \file
 * Defintion of the Arrangement_zone_2 class.
 */

#include <CGAL/Arrangement_2/Arr_traits_adaptor_2.h>
#include <list>
#include <map>
#include <set>

CGAL_BEGIN_NAMESPACE

/*! \class
 * A class for computing the zone of a given $x$-monotone curve in a given
 * arrangement.
 * The arrangement parameter corresponds to the underlying arrangement, and
 * the zone-visitor parameter corresponds to a visitor class which is capable
 * of receiving notifications on the arrangment features the query curve
 * traverses. The visitor has to support the following functions:
 * - init(), for initializing the visitor with a given arrangement.
 * - found_subcurve(), called when a non-intersecting x-monotone curve is
 *                     computed and located in the arrangement.
 * - found_overlap(), called when an x-monotone curve overlaps an existing
 *                    halfedge in the arrangement.
 * Both the second and the third functions return pair<Halfedge_handle, bool>,
 * where the halfedge handle corresponds to the halfedge created or modified
 * by the visitor (if valid), and the Boolean value indicates whether we
 * should halt the zone-computation process.
 */
template <class Arrangement_, class ZoneVisitor_>
class Arrangement_zone_2
{
public:

  typedef Arrangement_                            Arrangement_2;
  typedef typename Arrangement_2::Traits_2        Traits_2;

  typedef ZoneVisitor_                            Visitor;

  typedef typename Arrangement_2::Vertex_handle   Vertex_handle;
  typedef typename Arrangement_2::Halfedge_handle Halfedge_handle;
  typedef typename Arrangement_2::Face_handle     Face_handle;

  typedef std::pair<Halfedge_handle, bool>        Visitor_result;

  typedef typename Traits_2::Point_2              Point_2;
  typedef typename Traits_2::X_monotone_curve_2   X_monotone_curve_2;
                   
protected:

  typedef Arr_traits_adaptor_2<Traits_2>          Traits_adaptor_2;

  // Types used for caching intersection points:
  typedef std::pair<Point_2, unsigned int>        Intersect_point_2;
  typedef std::list<CGAL::Object>                 Intersect_list;
  typedef std::map<const X_monotone_curve_2*,
                   Intersect_list>                Intersect_map;
  typedef typename Intersect_map::iterator        Intersect_map_iterator;

  typedef std::set<const X_monotone_curve_2*>     Curves_set;
  typedef typename Curves_set::iterator           Curves_set_iterator;

  // Data members:
  Arrangement_2&          arr;         // The associated arrangement.
  Traits_adaptor_2       *traits;      // Its associated traits object.
                                       
  Visitor                *visitor;     // The zone visitor.

  Intersect_map           inter_map;   // Stores all computed intersections.

  const Vertex_handle     invalid_v;   // An invalid vertex handle.
  const Halfedge_handle   invalid_he;  // An invalid halfedge handle.

  X_monotone_curve_2  cv;              // The current portion of the
                                       // inserted curve.
  CGAL::Object        obj;             // The location of the left endpoint.
  Point_2             left_pt;         // Its current left endpoint.
  Point_2             right_pt;        // Its right endpoint.

  Vertex_handle       left_v;          // The arrangement vertex associated
                                       // with the current left_pt (if any).
  Halfedge_handle     left_he;         // If left_v is valid, left_he is the
                                       // predecessor for cv around this
                                       // vertex. Otherwise, if it is valid,
                                       // it is the halfedge that contains
                                       // the left endpoint it its interior.

  Vertex_handle       right_v;         // The arrangement vertex associated
                                       // with the current right_pt (if any).
  Halfedge_handle     right_he;        // If right_v is valid, left_he is the
                                       // predecessor for cv around this
                                       // vertex. Otherwise, if it is valid,
                                       // it is the halfedge that contains
                                       // the right endpoint it its interior.

  Point_2             intersect_p;     // The next intersection point.
  unsigned int        ip_mult;         // Its multiplicity
                                       // (0 in case of an overlap).
  bool                found_intersect; // Have we found an intersection
                                       // (or an overlap).
  X_monotone_curve_2  overlap_cv;      // The currently discovered overlap.
  bool                found_overlap;   // Have we found an overlap.
  bool                found_iso_vert;  // Check if an isolated vertex induces
                                       // the next intersection.
  Vertex_handle       intersect_v;     // The vertex that intersects cv.
  Halfedge_handle     intersect_he;    // The halfedge that intersects cv
                                       // (or overlaps it).

  X_monotone_curve_2  sub_cv1;         // Auxiliary variable (for curve split).
  X_monotone_curve_2  sub_cv2;         // Auxiliary variable (for curve split).

public:

  /*!
   * Constructor.
   * \param _arr The arrangement for which we compute the zone.
   * \param _visitor A pointer to a zone-visitor object.
   */
  Arrangement_zone_2 (Arrangement_2& _arr,
		      Visitor *_visitor) :
    arr (_arr),
    visitor (_visitor),
    invalid_v (),
    invalid_he ()
  {
    traits = static_cast<Traits_adaptor_2*> (arr.get_traits());

    CGAL_assertion (visitor != NULL);

    // Initialize the visitor.
    visitor->init (&arr);
  }

  /*!
   * Initialize the zone-computation process with a given curve.
   * \param _cv The query curve.
   * \param pl A point-location object associated with the arrangement.
   */
  template <class PointLocation>
  void init (const X_monotone_curve_2& _cv,
	     const PointLocation& pl)
  {
    // Set the curve and its endpoints.
    cv = _cv;
    left_pt = traits->construct_min_vertex_2_object() (cv);
    right_pt = traits->construct_max_vertex_2_object() (cv);

    // Locate the left endpoint.
    obj = pl.locate (left_pt);

    return;
  }

  /*!
   * Initialize the zone-computation process with a given curve.
   * \param _cv The query curve.
   * \param _obj An object that represents the location of the left
   *             endpoint of the curve.
   */
  void init_with_hint (const X_monotone_curve_2& _cv,
                       const Object& _obj)
  {
    // Set the curve and its endpoints.
    cv = _cv;
    left_pt = traits->construct_min_vertex_2_object() (cv);
    right_pt = traits->construct_max_vertex_2_object() (cv);

    // Set the object that represents the location of the left endpoint
    // in the arrangement.
    obj = _obj;

    return;
  }

  /*!
   * Compute the zone of the given curve and issue the apporpriate
   * notifications for the visitor.
   */
  void compute_zone ();

private:

  /*!
   * Find a face containing the query curve cv around the given vertex.
   * In case an overlap occurs, sets intersect_he to be the overlapping edge.
   * \param v The query vertex.
   * \param he Output: The predecessor of cv around the vertex.
   * \return (true) if cv overlaps with the curve associated with he;
   *         (false) if there is no overlap.
   */
  bool _find_prev_around_vertex (Vertex_handle v,
                                 Halfedge_handle& he);

  /*!
   * Direct the halfedge for the location of the given subcurve around a split
   * point that occurs in the interior of a given edge, when the subcurve lies
   * to the right of the split point.
   * In case of overlaps, it sets also found_overlap and intersect_he.
   * \param cv_ins The curve to be inserted, whose left endpoint coincides
   *               with the edge to be split.
   * \param cv_left_pt The left endpoint of cv_ins.
   * \param query_he The edge that intersects cv_ins.
   * \pre The left endpoint of cv_ins lies in the interior of the curve
   *      associated with query_he.
   * \return The halfedge whose incident face contains cv_ins
   *         (either query_he or its twin).
   */
  Halfedge_handle _direct_intersecting_edge_to_right
                                            (const X_monotone_curve_2& cv_ins,
					     const Point_2& cv_left_pt,
					     Halfedge_handle query_he);

  /*!
   * Direct the halfedge for the location of the given subcurve around a split
   * point that occurs in the interior of a given edge, when the subcurve lies
   * to the left of the split point.
   * \param cv_ins The curve to be inserted, whose right endpoint coincides
   *               with the edge to be split.
   * \param query_he The edge that intersects cv_ins.
   * \pre The right endpoint of cv_ins lies in the interior of the curve
   *      associated with query_he.
   * \return The halfedge whose incident face contains cv_ins
   *         (either query_he or its twin).
   */
  Halfedge_handle _direct_intersecting_edge_to_left
                                            (const X_monotone_curve_2& cv_ins,
					     Halfedge_handle query_he);

  /*!
   * Get the next intersection of cv with the given halfedge.
   * \param he A handle to the halfedge.
   * \param skip_first_point Should we skip the first intersection point.
   * \return An object representing the next intersection: Intersect_point_2
   *         in case of a simple intersection point, X_monotone_curve_2 in
   *         case of an overlap, and an empty object if there is no
   *         intersection.
   */
  CGAL::Object _compute_next_intersection (Halfedge_handle he,
                                           bool skip_first_point);

  /*!
   * Remove the next intersection of cv with the given halfedge from the map.
   * \param he A handle to the halfedge.
   * \pre The list of intersections with the curve of he has already been
   *      computed, and it is not empty.
   */
  void _remove_next_intersection (Halfedge_handle he);

  /*!
   * Compute the (lexicographically) leftmost intersection of the query
   * curve with the boundary of a given face in the arrangement.
   * The function computes sets intersect_p, intersect_he (or alternatively
   * overlap_cv and intersect_he) and set the flags found_intersect and
   * found_overlap accordingly.
   * \param face A handle to the face.
   * \param on_boundary Specifies whether the left endpoint of the curve lies
   *                    on the face boundary.
   */
  void _leftmost_intersection_with_face_boundary (Face_handle face,
                                                  bool on_boundary);

  /*!
   * Compute the zone of an x-monotone curve in a given arrangement face.
   * The left endpoint of the curve either lies in the face interior or on
   * the boundary of the face.
   * This function updates cv and its left endpoint and also sets left_v
   * and left_he for the remaining portion of the curve.
   * In case of overlaps, it sets also overlap_cv and intersect_he.
   * \param face The given face.
   * \param on_boundary Specifies whether the left endpoint of the curve lies
   *                    on the face boundary.
   * \pre If on_boundary is (true) then left_he must be valid; if it is (false)
   *      then both left_v anf left_he must be invalid.
   * \return (true) if we are done with the zone-computation process;
   *         (false) if we still have a remaining portion of cv to continue
   *         with.
   */
  bool _zone_in_face (Face_handle face,
		      bool on_boundary);

  /*!
   * Compute the zone of an overlapping subcurve overlap_cv of cv and the
   * curve currently associated with intersect_he.
   * This function updates cv and its left endpoint and also sets left_v
   * and left_he for the remaining portion of the curve.
   * \return (true) if we are done with the zone-computation process;
   *         (false) if we still have a remaining portion of cv to continue
   *         with.
   */
  bool _zone_in_overlap ();
};

CGAL_END_NAMESPACE

// The function definitions can be found under:
#include <CGAL/Arrangement_2/Arrangement_zone_2_functions.h>

#endif
