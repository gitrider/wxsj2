// Copyright (c) 1997-2004  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Installation/include/CGAL/config.h $
// $Id: config.h 30927 2006-05-02 18:14:47Z spion $
// 
//
// Author(s)     : Wieger Wesselink 
//                 Michael Hoffmann <hoffmann@inf.ethz.ch>
//                 Sylvain Pion

#ifndef CGAL_CONFIG_H
#define CGAL_CONFIG_H

#include <CGAL/version.h>

//----------------------------------------------------------------------//
//             include platform specific workaround flags (CGAL_CFG_...)
//----------------------------------------------------------------------//

#include <CGAL/compiler_config.h>

//----------------------------------------------------------------------//
//             do some post processing for the flags
//----------------------------------------------------------------------//

#ifdef CGAL_CFG_NO_STL
#  error "This compiler does not have a working STL"
#endif

#define CGAL_BEGIN_NAMESPACE namespace CGAL {
#define CGAL_END_NAMESPACE }

#ifdef CGAL_CFG_NO_STDC_NAMESPACE
#  define CGAL_CLIB_STD
#else
#  define CGAL_CLIB_STD std
#endif

#ifndef CGAL_CFG_NO_LONG_LONG
#  define CGAL_USE_LONG_LONG
#endif

#ifdef CGAL_CFG_NO_LONG_DOUBLE_IO
#include <iostream>
namespace std {
  template < typename _CharT, typename _Traits >
  inline basic_ostream<_CharT, _Traits> &
  operator<<(basic_ostream<_CharT, _Traits> & os, const long double &ld)
  {
      return os << (double) ld;
  }
}
#endif

// FIXME: what is the problem with Sun 5.5? MATCHING_BUG_4 is not
// triggered, but there are runtime errors, e.g., with Distance_3,
// that do not appear when using the wrapper...
#if defined(CGAL_CFG_MATCHING_BUG_4)
 // || (defined(__sun) && defined(__SUNPRO_CC))
namespace CGAL {
    template < typename T >
    struct Self { typedef T Type; };
}
#  define CGAL_WRAP(K) CGAL::Self<K>::Type
#else
#  define CGAL_WRAP(K) K
#endif


#ifndef CGAL_CFG_TYPENAME_BEFORE_DEFAULT_ARGUMENT_BUG
#  define CGAL_TYPENAME_DEFAULT_ARG typename
#else
#  define CGAL_TYPENAME_DEFAULT_ARG
#endif


#ifndef CGAL_CFG_SUNPRO_RWSTD
#  define CGAL_reverse_iterator(T) std::reverse_iterator< T >
#else
#  define CGAL_reverse_iterator(T) std::reverse_iterator< T , \
                                   typename T::iterator_category , \
                                   typename T::value_type , \
                                   typename T::reference , \
                                   typename T::pointer , \
                                   typename T::difference_type >
#endif


// Big endian or little endian machine.
// ====================================
#ifdef CGAL_CFG_NO_BIG_ENDIAN
#  define CGAL_LITTLE_ENDIAN
#else
#  define CGAL_BIG_ENDIAN
#endif

#ifndef CGAL_USE_LEDA
#  define CGAL_USE_CGAL_WINDOW
#endif

// Symbolic constants to tailor inlining. Inlining Policy.
// =======================================================
#ifndef CGAL_MEDIUM_INLINE
#  define CGAL_MEDIUM_INLINE inline
#endif

#ifndef CGAL_LARGE_INLINE
#  define CGAL_LARGE_INLINE
#endif

#ifndef CGAL_HUGE_INLINE
#  define CGAL_HUGE_INLINE
#endif

//----------------------------------------------------------------------//
//             include separate workaround files
//----------------------------------------------------------------------//

#ifdef _MSC_VER
#  include <CGAL/MSVC_standard_header_fixes.h>
#elif defined(__BORLANDC__) && __BORLANDC__ > 0x520
#  include <CGAL/Borland_fixes.h>
#elif defined(__sun) && defined(__SUNPRO_CC)
#  include <CGAL/Sun_fixes.h>
#endif

//--------------------------------------------------------------------//
// This addresses a bug in VC++ 7.0 that (re)defines min(a, b)
// and max(a, b) in windows.h and windef.h 
//-------------------------------------------------------------------//

#ifdef _MSC_VER
#  define NOMINMAX 1
#endif

//-------------------------------------------------------------------//
// When the global min and max are no longer defined (as macros) 
// because of NOMINMAX flag definition, we define our own global 
// min/max functions to make the Microsoft headers compile. (afxtempl.h)
// Users that does not want the global min/max 
// should define CGAL_NOMINMAX
//-------------------------------------------------------------------//
#include <algorithm>
#if defined NOMINMAX && !defined CGAL_NOMINMAX
using std::min;
using std::max;
#endif

//-------------------------------------------------------------------//
// Is Geomview usable ?
#if !defined(__BORLANDC__) && !defined(_MSC_VER) && !defined(__MINGW32__)
#  define CGAL_USE_GEOMVIEW
#endif


//-------------------------------------------------------------------//
// Compilers provide different macros to access the current function name
#ifdef _MSC_VER
#  define CGAL_PRETTY_FUNCTION __FUNCSIG__
#elif defined __GNUG__
#  define CGAL_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else 
#  define CGAL_PRETTY_FUNCTION __func__
// with sunpro, this requires -features=extensions
#endif

#endif // CGAL_CONFIG_H
