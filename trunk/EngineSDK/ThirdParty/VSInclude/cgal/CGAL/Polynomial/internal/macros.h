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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Polynomial/internal/macros.h $
// $Id: macros.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_POLYNOMIAL_INTERNAL_MACROS_H
#define CGAL_POLYNOMIAL_INTERNAL_MACROS_H

#include <CGAL/Polynomial/internal/config.h>

#ifdef CGAL_POLYNOMIAL_USE_CGAL
/*
  When CGAL is present
*/
#include <CGAL/basic.h>

#define CGAL_POLYNOMIAL_BEGIN_NAMESPACE CGAL_BEGIN_NAMESPACE \
namespace POLYNOMIAL \
{ \

    #define CGAL_POLYNOMIAL_END_NAMESPACE } \
CGAL_END_NAMESPACE

#define CGAL_POLYNOMIAL_NS CGAL::POLYNOMIAL
#define CGAL_Polynomial_assertion(x) CGAL_assertion(x)
#define CGAL_Polynomial_assertion_code(x) CGAL_assertion_code(x)
#define CGAL_Polynomial_precondition(x) CGAL_precondition(x)
#define CGAL_Polynomial_precondition_code(x) CGAL_precondition_code(x)
#define CGAL_Polynomial_postcondition(x) CGAL_postcondition(x)
#ifdef CGAL_POLYNOMIAL_CHECK_EXPENSIVE
#define CGAL_Polynomial_expensive_precondition(x) CGAL_expensive_precondition(x)
#define CGAL_Polynomial_expensive_assertion(x) CGAL_expensive_assertion(x)
#define CGAL_Polynomial_expensive_postcondition(x) CGAL_expensive_postcondition(x)
#else
#define CGAL_Polynomial_expensive_precondition(x)
#define CGAL_Polynomial_expensive_assertion(x)
#define CGAL_Polynomial_expensive_postcondition(x)
#endif
#define CGAL_Polynomial_exactness_assertion(x) CGAL_exactness_assertion(x)
#define CGAL_Polynomial_exactness_postcondition(x) CGAL_exactness_postcondition(x)
#define CGAL_Polynomial_exactness_precondition(x) CGAL_exactness_precondition(x)

#else
/*
  When no CGAL is present
*/

#define CGAL_POLYNOMIAL_BEGIN_NAMESPACE \
namespace Polynomial \
{ \

    #define CGAL_POLYNOMIAL_END_NAMESPACE }

#define POLYNOMIAL_NS Polynomial

#include <assert.h>

#define CGAL_Polynomial_assertion(x) assert(x)
// This does not work
#define CGAL_Polynomial_assertion_code(x) x
#define CGAL_Polynomial_precondition(x) assert(x)
#define CGAL_Polynomial_postcondition(x) assert(x)
#define CGAL_Polynomial_expensive_precondition(x)
#define CGAL_Polynomial_expensive_assertion(x)
#define CGAL_Polynomial_expensive_postcondition(x)
#define CGAL_Polynomial_exactness_postcondition(x)
#define CGAL_Polynomial_exactness_precondition(x)
#endif

#define CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE CGAL_POLYNOMIAL_BEGIN_NAMESPACE \
namespace internal \
{ \

    #define CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE } \
CGAL_POLYNOMIAL_END_NAMESPACE
#endif
