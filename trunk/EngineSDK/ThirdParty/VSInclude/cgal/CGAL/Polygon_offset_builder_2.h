// Copyright (c) 2005, 2006 Fernando Luis Cacciola Carballal. All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Straight_skeleton_2/include/CGAL/Polygon_offset_builder_2.h $
// $Id: Polygon_offset_builder_2.h 31990 2006-06-20 18:56:09Z fcacciola $
//
// Author(s)     : Fernando Cacciola <fernando_cacciola@ciudad.com.ar>
//
#ifndef CGAL_POLYGON_OFFSET_BUILDER_2_H
#define CGAL_POLYGON_OFFSET_BUILDER_2_H 1

#include <vector>
#include <algorithm>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

#include <CGAL/Polygon_offset_builder_traits_2.h>

CGAL_BEGIN_NAMESPACE

template<class Ss_, class Traits_, class Container_>
class Polygon_offset_builder_2
{
public :

  typedef Ss_        Ss ;
  typedef Traits_    Traits ;
  typedef Container_ Container ;

  typedef typename Traits::Kernel K ;
  
  typedef typename Traits::FT      FT ;
  typedef typename Traits::Point_2 Point_2 ;

  typedef boost::shared_ptr<Container> ContainerPtr ;

  Polygon_offset_builder_2( Ss const& aSs, Traits const& aTraits = Traits() )  ;

  template<class OutputIterator>
  OutputIterator construct_offset_contours( FT aTime, OutputIterator aOut ) ;

private:

  typedef typename Ss::Halfedge_const_handle Halfedge_const_handle  ;

  typedef std::vector<Halfedge_const_handle> Halfedge_vector ;

  typedef typename Traits::Segment_2 Segment_2 ;
  typedef typename Traits::Triedge_2 Triedge_2 ;

  bool handled_assigned( Halfedge_const_handle aH ) const
  {
    const Halfedge_const_handle cNull ;
    return aH != cNull ;
  }

  Halfedge_const_handle LocateHook( FT aTime, Halfedge_const_handle aBisector ) ;

  void AddOffsetVertex( FT aTime, Halfedge_const_handle aHook, ContainerPtr aPoly ) ;

  template<class OutputIterator>
  OutputIterator TraceOffsetPolygon( FT aTime, Halfedge_const_handle aHook, OutputIterator aOut ) ;

  Halfedge_const_handle LocateSeed( FT aTime ) ;

  bool IsVisited( Halfedge_const_handle aBisector ) { return mVisitedBisectors[aBisector->id()] != 0 ; }

  void Visit( Halfedge_const_handle aBisector ) { mVisitedBisectors[aBisector->id()] = 1 ; }

  inline Segment_2 CreateEdge ( Halfedge_const_handle aH ) const
  {
    Point_2 s = aH->opposite()->vertex()->point() ;
    Point_2 t = aH->vertex()->point() ;
    return K().construct_segment_2_object()(s,t);
  }

  inline Triedge_2 CreateTriedge ( Halfedge_const_handle aE0
                                 , Halfedge_const_handle aE1
                                 , Halfedge_const_handle aE2
                                 ) const
  {
    return Construct_ss_triedge_2(mTraits)(CreateEdge(aE0),CreateEdge(aE1),CreateEdge(aE2));
  }

  Comparison_result Compare_offset_against_event_time( FT aT, Halfedge_const_handle aBisector, Halfedge_const_handle aNextBisector ) const
  {
    CGAL_assertion(aBisector->is_bisector());
    CGAL_assertion(handle_assigned(aBisector->opposite()));
    CGAL_assertion(aBisector->opposite()->is_bisector());
    CGAL_assertion(aNextBisector->is_bisector());
    CGAL_assertion(handle_assigned(aNextBisector->opposite()));
    CGAL_assertion(aNextBisector->opposite()->is_bisector());

    Halfedge_const_handle lBorderA = aBisector->defining_contour_edge();
    Halfedge_const_handle lBorderB = aBisector->opposite()->defining_contour_edge();
    Halfedge_const_handle lBorderC = aNextBisector->opposite()->defining_contour_edge();

    return Compare_offset_against_event_time_2(mTraits)(aT,CreateTriedge(lBorderA,lBorderB,lBorderC));
  }

  boost::optional<Point_2> Construct_offset_point( FT aT, Halfedge_const_handle aBisector ) const
  {
    CGAL_assertion(aBisector->is_bisector());
    CGAL_assertion(handle_assigned(aBisector->opposite()));
    CGAL_assertion(aBisector->opposite()->is_bisector());

    Halfedge_const_handle lBorderA = aBisector->defining_contour_edge();
    Halfedge_const_handle lBorderB = aBisector->opposite()->defining_contour_edge();

    return Construct_offset_point_2(mTraits)(aT,CreateEdge(lBorderA),CreateEdge(lBorderB));
  }

  void ResetVisitedBisectorsMap();

#ifdef CGAL_POLYGON_OFFSET_ENABLE_SHOW
  template<class Halfedge>
  void DrawBorder ( Halfedge aHalfedge )
  {
    SS_IO_AUX::ScopedSegmentDrawing draw_( aHalfedge->opposite()->vertex()->point()
                                         , aHalfedge->vertex()->point()
                                         , CGAL::RED
                                         , "Border"
                                         ) ;
    draw_.Release();
  }
  template<class Halfedge>
  void DrawBisector ( Halfedge aHalfedge )
  {
    SS_IO_AUX::ScopedSegmentDrawing draw_( aHalfedge->opposite()->vertex()->point()
                                         , aHalfedge->vertex()->point()
                                         , aHalfedge->is_inner_bisector() ? CGAL::BLUE  : CGAL::GREEN
                                         , aHalfedge->is_inner_bisector() ? "IBisector" : "CBisector"
                                         ) ;
    draw_.Release();
  }

  void DrawOffset ( ContainerPtr const& aPoly, Point_2 const& aP )
  {
    if ( aPoly->size() > 0 )
    {
      SS_IO_AUX::ScopedSegmentDrawing draw_( (*aPoly)[aPoly->size()-1], aP, CGAL::BLACK, "Offset" ) ;
      draw_.Release();
    }
    else
    {
      SS_IO_AUX::ScopedPointDrawing draw_( aP, CGAL::BLACK, "Seed" ) ;
      draw_.Release();
    }
  }

#endif

  Traits const&    mTraits ;
  std::vector<int> mVisitedBisectors;
  Halfedge_vector  mBorders ;
};

CGAL_END_NAMESPACE

#include <CGAL/Straight_skeleton_2/Polygon_offset_builder_2_impl.h>

#endif // CGAL_POLYGON_OFFSET_BUILDER_2_H //
// EOF //

