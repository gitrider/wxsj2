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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arrangement_2/Arr_inc_insertion_zone_visitor.h $
// $Id: Arr_inc_insertion_zone_visitor.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Ron Wein          <wein@post.tau.ac.il>

#ifndef CGAL_ARR_INC_INSERTION_ZONE_VISITOR_H
#define CGAL_ARR_INC_INSERTION_ZONE_VISITOR_H

/*! \file
 * Definition of the Arr_inc_insertion_zone_visitor_2 class.
 */

#include <CGAL/Arr_accessor.h>
//#define ARR_INC_INSERT_DEBUG

CGAL_BEGIN_NAMESPACE

/*! \class
 * A visitor class for Arrangement_zone_2, which performs incremental 
 * insertion of an x-monotone curve into an arrangement.
 * The class shouldbe templated by an Arrangement_2 class.
 */
template <class Arrangement_>
class Arr_inc_insertion_zone_visitor
{
public:

  typedef Arrangement_                                Arrangement_2;
  typedef typename Arrangement_2::Traits_2            Traits_2;

  typedef typename Arrangement_2::Vertex_handle       Vertex_handle;
  typedef typename Arrangement_2::Halfedge_handle     Halfedge_handle;
  typedef typename Arrangement_2::Face_handle         Face_handle;

  typedef typename Arrangement_2::Point_2             Point_2;
  typedef typename Arrangement_2::X_monotone_curve_2  X_monotone_curve_2;

  typedef std::pair<Halfedge_handle, bool>            Result;

private:

  Arrangement_2         *p_arr;         // The arrangement into which we
                                        // insert the curves.
  Traits_2              *traits;        // The arrangement traits.

  const Vertex_handle    invalid_v;     // An invalid vertex handle.
  const Halfedge_handle  invalid_he;    // An invalid halfedge handle.

  X_monotone_curve_2     sub_cv1;       // Auxiliary varibale (for splitting).
  X_monotone_curve_2     sub_cv2;       // Auxiliary varibale (for splitting).

public:

  /*! Constructor. */
  Arr_inc_insertion_zone_visitor () :
    p_arr (NULL),
    traits (NULL),
    invalid_v (),
    invalid_he ()
  {}

  /*! Initialize the visitor with an arrangement object. */
  void init (Arrangement_2 *arr)
  {
    p_arr = arr;
    traits = arr->get_traits();
  }

  /*!
   * Handle the a subcurve located in the interior of a given face.
   * \param cv The subcurve.
   * \param face The face containing cv's interior.
   * \param left_v The vertex that corresponds to the left endpoint of cv
   *               (or an invalid handle if no such arrangement vertex exists).
   * \param left_he The halfedge that contains the left endpoint of cv
   *               (or an invalid handle if no such halfedge exists).
   * \param right_v The vertex that corresponds to the right endpoint of cv
   *               (or an invalid handle if no such arrangement vertex exists).
   * \param right_he The halfedge that contains the right endpoint of cv
   *                 (or an invalid handle if no such halfedge exists).
   * \return A handle to the halfedge obtained from the insertion of the
   *         subcurve into the arrangement.
   */
  Result found_subcurve (const X_monotone_curve_2& cv,
                         Face_handle face,
                         Vertex_handle left_v, Halfedge_handle left_he,
                         Vertex_handle right_v, Halfedge_handle right_he)
  {
#ifdef ARR_INC_INSERT_DEBUG
    std::cout << "Found subcurve: " << cv << std::endl;
    if (left_v != invalid_v)
      std::cout << "           v1 = " << left_v->point() << std::endl;
    if (left_he != invalid_he)
      std::cout << "           e1 = " << left_he->source()->point()
                << "  -->  " << left_he->target()->point() << std::endl;

    if (right_v != invalid_v)
      std::cout << "           v2 = " << right_v->point() << std::endl;
    if (right_he != invalid_he)
      std::cout << "           e2 = " << right_he->source()->point()
                << "  -->  " << right_he->target()->point() << std::endl;
#endif

    // Create an arrangement accessor.
    Arr_accessor<Arrangement_2>    arr_access (*p_arr);

    // Check if the left and the right endpoints of cv should be associated
    // with arrangement vertices.
    const bool       vertex_for_left =
      (left_v != invalid_v) || (left_he != invalid_he);
    const bool       vertex_for_right =
      (right_v != invalid_v) || (right_he != invalid_he);

    // Found the previous halfedges for the left and right endpoints (if any).
    Halfedge_handle  prev_he_left;
    Halfedge_handle  prev_he_right;

    if (vertex_for_left)
    {
      // If we are given the previous halfedge, use it. Otherwise, we are given
      // the vertex and we should locate cv around it.
      if (left_he != invalid_he)
        prev_he_left = left_he;
      else if (! left_v->is_isolated())
        prev_he_left = arr_access.locate_around_vertex (left_v, cv);

      // In case the vertex does not exist, split left_he at cv's left endpoint
      // and create the vertex.
      if (left_v == invalid_v)
      {
        _split_edge (left_he,
                     traits->construct_min_vertex_2_object() (cv),
                     arr_access);

        // Check if we have just split the halfedge that right_he refers to,
	// and if this halfedge is directed from left to right.
        // If so, right_he's target is now the new vertex, and we have to
        // proceed to the next halfedge (which is going to be split).
        if (right_he == left_he && left_he->direction() == SMALLER)
          right_he = right_he->next();
      }
    }

    if (vertex_for_right)
    {
      // If we are given the previous halfedge, use it. Otherwise, we are given
      // the vertex and we should locate cv around it.
      if (right_he != invalid_he)
        prev_he_right = right_he;
      else if (! right_v->is_isolated())
        prev_he_right = arr_access.locate_around_vertex (right_v, cv);

      // In case the vertex does not exist, split right_he at cv's right
      // endpoint and create the vertex.
      if (right_v == invalid_v)
      {
        _split_edge (right_he,
                     traits->construct_max_vertex_2_object() (cv),
                     arr_access);

        // Check if we have just split the halfedge that left_he refers to.
        // If so, prev_he_right's target is now the new vertex, and we have to
        // proceed to the next halfedge (whose target is right_v).
        if (right_he == prev_he_left)
	{
          prev_he_left = prev_he_left->next();
	}
      }
    }

    // Insert the curve into the arrangement.
    Halfedge_handle   inserted_he;

    if (! vertex_for_left)
    {
      // The left endpoint should not be associated with a vertex:
      if (! vertex_for_right)
      {
        // We should insert the curve in the interior of the face.
        inserted_he = p_arr->insert_in_face_interior (cv,
                                                      face);
      }
      else
      {
        // The right endpoint is associated with an arrangement vertex.
        // If possible, use the previous halfedge for the right vertex.
        if (prev_he_right != invalid_he)
        {
          inserted_he = p_arr->insert_from_right_vertex (cv,
                                                         prev_he_right);
        }
        else
        {
          inserted_he = p_arr->insert_from_right_vertex (cv,
                                                         right_v);
        }

        // The returned halfedge is directed to the newly created vertex
        // (the left one), so we take its twin.
        inserted_he = inserted_he->twin();
      }
    }
    else
    {
      // The left endpoint should be associated with a vertex:
      if (! vertex_for_right)
      {
        // If possible, use the previous halfedge for the left vertex.
        if (prev_he_left != invalid_he)
        {
          inserted_he = p_arr->insert_from_left_vertex (cv,
                                                        prev_he_left);
        }
        else
        {
          inserted_he = p_arr->insert_from_left_vertex (cv,
                                                        left_v);
        }
      }
      else
      {
        // Both endpoints are associated with arrangement vertices.
        // If possible, use the previous halfedges for both end vertices.
        if (prev_he_left != invalid_he && prev_he_right != invalid_he)
        {
          inserted_he = p_arr->insert_at_vertices (cv,
                                                   prev_he_left,
                                                   prev_he_right);
        }
        else if (prev_he_left != invalid_he)
        {
          inserted_he = p_arr->insert_at_vertices (cv,
                                                   prev_he_left,
                                                   right_v);
        }
        else if (prev_he_right != invalid_he)
        {
          inserted_he = p_arr->insert_at_vertices (cv,
                                                   prev_he_right,
                                                   left_v);

          // The returned halfedge is currently directed toward the left vertex
          // (instead of the right one), so we take its twin.
          inserted_he = inserted_he->twin();
        }
        else
        {
          inserted_he = p_arr->insert_at_vertices (cv,
                                                   left_v,
                                                   right_v);
        }
      }
    }

    // Return the inserted halfedge, and indicate we should not halt the
    // zone-computation process.
    Result    res = Result (inserted_he, false);

    return (res);
  }

  /*!
   * Handle the a subcurve that overlaps a given edge.
   * \param cv The overlapping subcurve.
   * \param he The overlapped halfedge (directed from left to right).
   * \param left_v The vertex that corresponds to the left endpoint of cv
   *               (or an invalid handle if no such arrangement vertex exists).
   * \param right_v The vertex that corresponds to the right endpoint of cv
   *               (or an invalid handle if no such arrangement vertex exists).
   * \return A handle to the halfedge obtained from the insertion of the
   *         overlapping subcurve into the arrangement.
   */
  Result found_overlap (const X_monotone_curve_2& cv,
                        Halfedge_handle he,
                        Vertex_handle left_v, Vertex_handle right_v)
  {
#ifdef ARR_INC_INSERT_DEBUG

    std::cout << "Found overlap: " << cv
              << "  with: " << he->curve() << std::endl;

    if (left_v != invalid_v)
      std::cout << "           v1 = " << left_v->point() << std::endl;

    if (right_v != invalid_v)
      std::cout << "           v2 = " << right_v->point() << std::endl;
#endif

    // Modify (perhaps split) the overlapping arrangement edge.
    Halfedge_handle   updated_he;

    if (left_v == invalid_v)
    {
      // Split the curve associated with he at the left endpoint of cv.
      traits->split_2_object() (he->curve(),
                                traits->construct_min_vertex_2_object() (cv),
                                sub_cv1, sub_cv2);

      if (right_v == invalid_v)
      {
        // The overlapping curve is contained strictly in the interior of he:
        // Split he as an intermediate step.
        updated_he = p_arr->split_edge (he,
                                        sub_cv1, sub_cv2);
        updated_he = updated_he->next();

        // Split the left subcurve at the right endpoint of cv.
        traits->split_2_object() (updated_he->curve(),
                                  traits->construct_max_vertex_2_object() (cv),
                                  sub_cv1, sub_cv2);

        // Split updated_he once again, so that the left portion corresponds
        // to the overlapping curve and the right portion corresponds to
        // sub_cv2.
        updated_he = p_arr->split_edge (updated_he,
                                        cv, sub_cv2);
      }
      else
      {
        // Split he, such that the left portion corresponds to sub_cv1 and the
        // right portion corresponds to the overlapping curve.
        updated_he = p_arr->split_edge (he,
                                        sub_cv1, cv);
        updated_he = updated_he->next();
      }
    }
    else
    {
      if (right_v == invalid_v)
      {
        // Split the curve associated with he at the right endpoint of cv.
        traits->split_2_object() (he->curve(),
                                  traits->construct_max_vertex_2_object() (cv),
                                  sub_cv1, sub_cv2);

        // Split he, such that the left portion corresponds to the overlapping
        // curve and the right portion corresponds to sub_cv2.
        updated_he = p_arr->split_edge (he,
                                        cv, sub_cv2);
      }
      else
      {
        // The entire edge is overlapped: Modify the curve associated with cv
        // to be the overlapping curve.
        updated_he = p_arr->modify_edge (he,
                                         cv);
      }
    }

    // Return the updated halfedge, and indicate we should not halt the
    // zone-computation process.
    Result    res = Result (updated_he, false);

    return (res);
  }

private:

  /*!
   * Split an arrangement edge.
   * \param he The edge to split (one of the twin halfedges).
   * \param p The split point.
   * \param arr_access An arrangement accessor.
   */
  void _split_edge (Halfedge_handle he,
                    const Point_2& p,
                    Arr_accessor<Arrangement_2>& arr_access)
  {
    // Split the curve at the split point.
    traits->split_2_object() (he->curve(),
                              p,
                              sub_cv1, sub_cv2);

    // Determine the order we send the split curves to the split_edge function,
    // depending whether the left point of sub_cv1 equals he's source (if not,
    // it equals its target).
    if (traits->equal_2_object()
        (he->source()->point(),
         traits->construct_min_vertex_2_object() (sub_cv1)))
    {
      arr_access.split_edge_ex (he,
                                p,
                                sub_cv1, sub_cv2);
    }
    else
    {
      arr_access.split_edge_ex (he,
                                p,
                                sub_cv2, sub_cv1);
    }

    return;
  }
};

CGAL_END_NAMESPACE

#endif
