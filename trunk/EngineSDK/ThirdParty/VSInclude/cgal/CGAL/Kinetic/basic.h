// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/basic.h $
// $Id: basic.h 29568 2006-03-16 22:06:48Z spion $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_BASIC_H
#define CGAL_KINETIC_BASIC_H

/*!
  \file CGAL/Kinetic/basic.h The header which defines the standard things for the KDS namespace and module.

  \todo cgal documentation

  \namespace CGAL The CGAL project namespace

  \namespace CGAL::KDS The namespace for classes which involve the Kinetic Data Structures framework.

*/

#include <CGAL/basic.h>

#include <CGAL/Kinetic/internal/config.h>

#ifdef CGAL_CHECK_EXPENSIVE
#ifndef CGAL_KINETIC_CHECK_EXPENSIVE
#define CGAL_KINETIC_CHECK_EXPENSIVE
#endif
#endif

#ifdef CGAL_CHECK_EXACTNESS
#ifndef CGAL_KINETIC_CHECK_EXACTNESS
#define CGAL_KINETIC_CHECK_EXACTNESS
#endif
#endif



#define CGAL_KINETIC_BEGIN_NAMESPACE CGAL_BEGIN_NAMESPACE \
namespace Kinetic \
{ \

    #define CGAL_KINETIC_END_NAMESPACE } \
CGAL_END_NAMESPACE

#define CGAL_KINETIC_BEGIN_INTERNAL_NAMESPACE CGAL_KINETIC_BEGIN_NAMESPACE \
namespace internal \
{ \

    #define CGAL_KINETIC_END_INTERNAL_NAMESPACE } \
CGAL_KINETIC_END_NAMESPACE


#ifdef CGAL_USE_GMP
#include <CGAL/Gmpq.h>
CGAL_KINETIC_BEGIN_NAMESPACE
typedef CGAL::Gmpq Default_field_nt;
CGAL_KINETIC_END_NAMESPACE
#else
#include <CGAL/MP_Float.h>
CGAL_KINETIC_BEGIN_NAMESPACE
typedef CGAL::MP_Float Default_field_nt;
CGAL_KINETIC_END_NAMESPACE
#endif

CGAL_KINETIC_BEGIN_NAMESPACE
//! The types of logs available.
typedef enum {LOG_NONE=0, LOG_SOME=2, LOG_LOTS=3}
Log_level;

CGAL_KINETIC_END_NAMESPACE

#include <CGAL/Kinetic/internal/Log.h>

#define CGAL_KINETIC_LOG(level, expr) if (CGAL::Kinetic::internal::Logs::get().is_output(level))\
{ CGAL::Kinetic::internal::Logs::get().stream(level) << expr;};
#define CGAL_KINETIC_LOG_WRITE(level, expr) if (CGAL::Kinetic::internal::Logs::get().is_output(level))\
{std::ostream &LOG_STREAM= CGAL::Kinetic::internal::Logs::get().stream(level); expr;}
#define CGAL_KINETIC_ERROR(expr) std::cerr << expr;
#define CGAL_KINETIC_SET_LOG_LEVEL(level) CGAL::Kinetic::internal::Logs::get().set_level(level);
#endif
