/****************************************************************************
 * Core Library Version 1.7, August 2004
 * Copyright (c) 1995-2004 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of CORE (http://cs.nyu.edu/exact/core/); you may
 * redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with CORE.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * File: CORE.h
 * Synopsis:
 *      The main inclusion file for the Core Library system.
 *      All "Core programs" must include this file.
 *
 * Written by 
 *       Chee Yap <yap@cs.nyu.edu>
 *       Chen Li <chenli@cs.nyu.edu>
 *       Zilin Du <zilin@cs.nyu.edu>
 *
 * WWW URL: http://cs.nyu.edu/exact/
 * Email: exact@cs.nyu.edu
 *
 * $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Core/include/CORE/CORE.h $
 * $Id: CORE.h 30951 2006-05-02 20:21:31Z afabri $
 ***************************************************************************/

#ifndef _CORE_CORE_H_
#define _CORE_CORE_H_

#include <CORE/CoreDefs.h>
#include <CORE/Timer.h>

// User can still access machine types:
typedef double machine_double;
typedef long machine_long;

#ifndef CORE_LEVEL
#   define CORE_LEVEL  DEFAULT_CORE_LEVEL
#endif

#if CORE_LEVEL  == 1
#   define Real double
#   define Expr double
#elif CORE_LEVEL  == 2
#   include <CORE/Real.h>
#   undef long
#   undef double
#   define long Real
#   define double Real
#   define Expr Real
#elif CORE_LEVEL  == 3
#   include <CORE/Expr.h>
#   undef long
#   undef double
#   define long Expr
#   define double Expr
#   define Real Expr
#elif CORE_LEVEL == 4
#   include <CORE/Expr.h>
#endif

// automaticall link necessary static library under visual c++
#ifdef CGAL_LINK_AUTOMATICALLY
#ifdef _MSC_VER
  #ifdef _DEBUG
    #pragma comment(lib, "core++Debug.lib")
    #pragma comment(lib, "gmpDebug.lib")
  #else
    #pragma comment(lib, "core++.lib")
    #pragma comment(lib, "gmp.lib")
  #endif
#endif
#endif

// This seems redundant with the previous block, but 
// we don't want to break backward compatibility 
#if ! defined( CGAL_DONT_USE_LINK_PRAGMA) && defined( _MSC_VER )
   #pragma comment(lib, "core++.lib")
    #pragma comment(lib, "gmp.lib")
    #pragma comment(lib, "mpfr.lib")
#endif 

#ifndef CORE_NO_AUTOMATIC_NAMESPACE
using namespace CORE;
#endif

#endif // _CORE_CORE_H_


