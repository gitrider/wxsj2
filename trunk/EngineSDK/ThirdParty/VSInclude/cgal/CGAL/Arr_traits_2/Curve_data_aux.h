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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Arrangement_2/include/CGAL/Arr_traits_2/Curve_data_aux.h $
// $Id: Curve_data_aux.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Ron Wein          <wein@post.tau.ac.il>
//                 Efi Fogel         <efif@post.tau.ac.il>

#ifndef CGAL_CURVE_DATA_AUX_H
#define CGAL_CURVE_DATA_AUX_H

/*! \file
 * Definition of auxiliary classes for the Arr_curve_data_traits_2<> template.
 */

CGAL_BEGIN_NAMESPACE

/*!
 * \struct A simple convertor from one type to another.
 */
template <class TYPE_FROM, class TYPE_TO>
struct _Default_convert_func
{
  TYPE_TO operator() (const TYPE_FROM& obj)
  {
    return (obj);
  }
};

/*! \class
 * A template for extending the base curve type with a data field.
 */
template <class BaseCurveType, class Data>
class _Curve_data_ex : public BaseCurveType
{
private:
  Data    m_data;

public:

  /*! Default constructor. */
  _Curve_data_ex ()
  {}
    
  /*!
   * Construct an extended curve from a base curve.
   * \param cv The base curve.
   */ 
  _Curve_data_ex (const BaseCurveType& cv) :
    BaseCurveType (cv)
  {}

  /*!
   * Construct an extended curve from a base curve and a data object.
   * \param cv The base curve.
   * \param data The data object.
   */ 
  _Curve_data_ex (const BaseCurveType& cv, const Data& data) :
    BaseCurveType (cv),
    m_data (data)
  {}

  /*!
   * Get the data object (const version).
   * \return The data object associated with the curve.
   */
  const Data& data () const
  {
    return (m_data);
  }

  /*!
   * Get the data object (non-const version).
   * \return The data object associated with the curve.
   */
  Data& data ()
  {
    return (m_data);
  }

  /*!
   * Set the curve data.
   * \param data The data object to be associated with the curve.
   */
  void set_data (const Data& data)
  {
    m_data = data;
    return;
  }
};

CGAL_END_NAMESPACE

#endif

