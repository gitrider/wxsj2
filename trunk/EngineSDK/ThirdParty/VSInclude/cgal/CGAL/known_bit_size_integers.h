// Copyright (c) 1999-2005  Utrecht University (The Netherlands),
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Number_types/include/CGAL/known_bit_size_integers.h $
// $Id: known_bit_size_integers.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Lutz Kettner, Sylvain Pion

#ifndef CGAL_KNOWN_BIT_SIZE_INTEGERS_H
#define CGAL_KNOWN_BIT_SIZE_INTEGERS_H

#include <CGAL/basic.h>
#include <boost/mpl/if.hpp>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

namespace mpl = boost::mpl;

template < int size >
struct No_Integer_Type_Of_Size;

// Provides a signed integral type whose sizeof() is s.
// If not possible, gives No_Integer_Type_Of_Size<s>.
template < int s >
struct SizeofSelect
{
    typedef typename mpl::if_c< (sizeof(signed char) == s), signed char,
	      typename mpl::if_c< (sizeof(short) == s), short,
	        typename mpl::if_c< (sizeof(int) == s), int,
	          typename mpl::if_c< (sizeof(long) == s), long,
		    No_Integer_Type_Of_Size<s> >::type >::type >::type >::type  Type;
};

// Same thing for unsigned types.
template < int s >
struct USizeofSelect
{
    typedef typename mpl::if_c< (sizeof(unsigned char) == s), unsigned char,
	      typename mpl::if_c< (sizeof(unsigned short) ==s), unsigned short,
	        typename mpl::if_c< (sizeof(unsigned int) == s), unsigned int,
	          typename mpl::if_c< (sizeof(unsigned long) == s),
		                                              unsigned long,
		    No_Integer_Type_Of_Size<s> >::type >::type >::type >::type  Type;
};

} // namespace CGALi


typedef CGALi::SizeofSelect<1>::Type  Integer8;
typedef CGALi::SizeofSelect<2>::Type  Integer16;
typedef CGALi::SizeofSelect<4>::Type  Integer32;

typedef CGALi::USizeofSelect<1>::Type  UInteger8;
typedef CGALi::USizeofSelect<2>::Type  UInteger16;
typedef CGALi::USizeofSelect<4>::Type  UInteger32;

#if defined __ia64__ || defined __x86_64
    typedef long                     Integer64;
    typedef unsigned long            UInteger64;
#   define CGAL_HAS_INTEGER64
#endif

#if defined __BORLANDC__ || defined _MSC_VER
    typedef __int64                  Integer64;
    typedef unsigned __int64         UInteger64;
#   define CGAL_HAS_INTEGER64
#endif

// 64 integer types are defined for other platforms in CGAL/long_long.h

CGAL_END_NAMESPACE

#endif // CGAL_KNOWN_BIT_SIZE_INTEGERS_H
