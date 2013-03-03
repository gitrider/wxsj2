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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_point_location/Arr_lm_vertices_generator.h $
// $Id: Arr_lm_vertices_generator.h 30322 2006-04-14 15:07:17Z lsaboret $
// 
//
// Author(s)     : Idit Haran   <haranidi@post.tau.ac.il>
#ifndef CGAL_ARR_LANDMARKS_VERTICES_GENERATOR_H
#define CGAL_ARR_LANDMARKS_VERTICES_GENERATOR_H

/*! \file
* Definition of the Arr_landmarks_vertices_generator<Arrangement> template.
*/

#include <list>
#include <CGAL/Arr_observer.h>
#include <CGAL/Arrangement_2/Arr_traits_adaptor_2.h>
#include <CGAL/Arr_point_location/Arr_lm_nearest_neighbor.h>

//#define CGAL_LM_VERTICES_DEBUG
#ifdef CGAL_LM_VERTICES_DEBUG
	#define CGAL_PRINT_V_DEBUG(expr)   std::cout << expr << std::endl
#else
	#define CGAL_PRINT_V_DEBUG(expr)
#endif

CGAL_BEGIN_NAMESPACE

/*! \class
* This class is related to the Landmarks point locatoion, and given as 
* a parameter (or template parameter) to it. 
* It creates the list of landmarks, and handles them.
* It inherites from Arr_observer and updates this list whenever the 
* arrangement changes.
*/
template <class Arrangement_, 
	  class Nearest_neighbor_ 
    = Arr_landmarks_nearest_neighbor <typename Arrangement_::Traits_2> >
class Arr_landmarks_vertices_generator 
  : public Arr_observer <Arrangement_>
{
public:

  typedef Arrangement_                                  Arrangement_2;
  typedef typename Arrangement_2::Traits_2		Traits_2;

  typedef Arr_landmarks_vertices_generator<Arrangement_2,
					   Nearest_neighbor_>  Self;
  
  typedef typename Arrangement_2::Vertex_const_handle   Vertex_const_handle;
  typedef typename Arrangement_2::Halfedge_const_handle Halfedge_const_handle;
  typedef typename Arrangement_2::Face_const_handle     Face_const_handle;
  typedef typename Arrangement_2::Vertex_handle         Vertex_handle;
  typedef typename Arrangement_2::Halfedge_handle       Halfedge_handle;
  typedef typename Arrangement_2::Face_handle		Face_handle;
  typedef typename Arrangement_2::Vertex_const_iterator Vertex_const_iterator;
  
  typedef typename Traits_2::Point_2			Point_2;
  typedef typename Traits_2::X_monotone_curve_2	        X_monotone_curve_2;

  typedef Nearest_neighbor_				Nearest_neighbor;
  typedef typename Nearest_neighbor_::NN_Point_2	NN_Point_2;
  typedef std::list<NN_Point_2>                         NN_Point_list;
  typedef typename NN_Point_list::iterator		NN_Point_list_iterator;
  
protected:

  typedef Arr_traits_basic_adaptor_2<Traits_2>  Traits_adaptor_2;

  // Data members:
  const Traits_adaptor_2  *traits;  // Its associated traits object.
  Nearest_neighbor	   nn;      // The associated nearest neighbor object.
  bool                     ignore_notifications;	
  bool                     updated;
  int	                   num_small_not_updated_changes;
  int	                   num_landmarks;

private:

  /*! Copy constructor - not supported. */
  Arr_landmarks_vertices_generator (const Self& );

  /*! Assignment operator - not supported. */
  Self& operator= (const Self& );

public: 

  /*! Constructor. */
  Arr_landmarks_vertices_generator (const Arrangement_2& arr) : 
    Arr_observer<Arrangement_2> (const_cast<Arrangement_2 &>(arr)), 
    ignore_notifications (false), 
    updated (false), 
    num_small_not_updated_changes(0), 
    num_landmarks(0)
  {
    CGAL_PRINT_V_DEBUG("Arr_landmarks_vertices_generator constructor"); 
    traits = static_cast<const Traits_adaptor_2*> (arr.get_traits());
    build_landmarks_set();
  }
  
  /*!
   * Creates the landmarks set (choosing the landmarks) , 
   * and saving them in the nearest neighbor search structure.
   * This is a pure virtual function (must be implemented in 
   * the class that derives from this one)
   */
  void build_landmarks_set ()
  {
    CGAL_PRINT_V_DEBUG("build_landmarks_set."); 
    NN_Point_list      plist; 

    //Go over planar map, and insert all vertices as landmarks
    Vertex_const_iterator   vit;
    Arrangement_2 *arr = this->arrangement();

    for (vit=arr->vertices_begin(); vit != arr->vertices_end(); vit++)
    {
      //get point from vertex
      Point_2 p = vit->point() ;
      Vertex_const_handle vh = vit;
      Object obj = CGAL::make_object (vh);
      NN_Point_2 np (p, obj); 
      
      //insert point into list
      plist.push_back(np); 
      
      //CGAL_PRINT_V_DEBUG("landmark = "<< p); 
    } 

    //the search structure is now updated
    nn.clean();
    nn.init(plist.begin(), plist.end());
    
    num_small_not_updated_changes = 0;
    updated = true;
  }

  /*!
   * clear the tree
   */
  void clear_landmarks_set ()
  {
    CGAL_PRINT_V_DEBUG("clear_landmarks_set.");
    
    nn.clean();

    num_landmarks = 0;
    num_small_not_updated_changes = 0;
    updated = false;		  
  }

  /*!
   * get the nearest neighbor (landmark) to the given point
   */
  Point_2 get_closest_landmark (Point_2 p, Object &obj)
  {
    CGAL_PRINT_V_DEBUG("get_closest_landmark.");

    CGAL_assertion(updated);
    return nn.find_nearest_neighbor(p, obj);
  }
  
  //Observer functions that are relevant to overload
  //-------------------------------------------------
  
  /*!
   * Notification after the arrangement has been assigned with another
   * arrangement.
   * \param u A handle to the unbounded face.
   */
  virtual void after_assign ()
  { 
    clear_landmarks_set();
    build_landmarks_set();
    ignore_notifications = false;
  }
  
  /*! 
   * Notification before the observer is attached to an arrangement.
   * \param arr The arrangement we are about to attach the observer to.
   */
  virtual void before_attach (const Arrangement_2& arr)
  {
    clear_landmarks_set();
    traits = static_cast<const Traits_adaptor_2*> (arr.get_traits());
    ignore_notifications = false;
  }
  
  /*!
   * Notification after the observer has been attached to an arrangement.
   */
  virtual void after_attach ()
  {
    build_landmarks_set();
  }

  /*! 
   * Notification before the observer is detached from the arrangement.
   */
  virtual void before_detach ()
  {
    clear_landmarks_set();
  }

  /*!
   * Notification after the arrangement is cleared.
   * \param u A handle to the unbounded face.
   */
  virtual void after_clear (Face_handle /* u */)
  { 
    clear_landmarks_set();
    build_landmarks_set();
  }

  /*! Notification before a global operation modifies the arrangement. */
  virtual void before_global_change ()
  { 
    clear_landmarks_set();
    ignore_notifications = true;
  }

  /*! Notification after a global operation is completed. */
  virtual void after_global_change ()
  {
    build_landmarks_set();
    ignore_notifications = false;
  }

  /*!
   * Notification after the creation of a new vertex.
   * \param v A handle to the created vertex.
   */
  virtual void after_create_vertex (Vertex_handle /* v */)
  {
    if (! ignore_notifications)
    {
      CGAL_PRINT_V_DEBUG("Arr_landmarks_vertices_generator::after_create_vertex");
      _small_change();
    }
  }
  
  /*!
   * Notificaion before the removal of a vertex.
   * \param v A handle to the vertex to be deleted.
   */
  virtual void after_remove_vertex ()
  {
    if (! ignore_notifications)
    {
      clear_landmarks_set();
      build_landmarks_set();
    }
  }

protected:
  /*!
   * 
   */
  void _small_change ()
  {
    CGAL_PRINT_V_DEBUG("small change. num_small_not_updated_changes = " 
      << num_small_not_updated_changes);

    double nl = static_cast<double> (num_landmarks);
    const int sqrt_num_landmarks = 
      static_cast<int> (std::sqrt (nl) + 0.5);

    num_small_not_updated_changes++;
    if ((num_landmarks < 10) ||
      (num_small_not_updated_changes >=  sqrt_num_landmarks))
    {
      CGAL_PRINT_V_DEBUG("updating ...");
      clear_landmarks_set();
      build_landmarks_set();
    }
  }

};

CGAL_END_NAMESPACE

#endif
