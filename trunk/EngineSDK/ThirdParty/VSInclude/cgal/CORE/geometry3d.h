/******************************************************************
 * Core Library Version 1.7, August 2004
 * Copyright (c) 1995-2002 Exact Computation Project
 * 
 * File: geometry3d.h
 *
 * Written by
 *       Shubin Zhao (shubinz@cs.nyu.edu) (2001)
 *
 * WWW URL: http://cs.nyu.edu/exact/
 * Email: exact@cs.nyu.edu
 *
 * $Id: geometry3d.h 23263 2004-11-14 12:00:19Z efi $
 *****************************************************************/

#ifndef CORE_GEOMETRY3D_H
#define CORE_GEOMETRY3D_H

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include <CORE/linearAlgebra.h>

class Point3d;
class Line3d;
class Segment3d;
class Plane3d;
class Triangle3d;
class Polygon3d;

#include <CORE/geom3d/point3d.h>
#include <CORE/geom3d/line3d.h>
#include <CORE/geom3d/segment3d.h>
#include <CORE/geom3d/plane3d.h>
#include <CORE/geom3d/triangle3d.h>
#include <CORE/geom3d/polygon3d.h>

// automaticall link necessary static library under visual c++
#ifdef _MSC_VER
	#if CORE_LEVEL == 1
		#ifdef _DEBUG
			#pragma comment(lib, "corexDebug_level1.lib")
		#else
			#pragma comment(lib, "corex_level1.lib")
		#endif
	#elif CORE_LEVEL == 2
		#ifdef _DEBUG
			#pragma comment(lib, "corexDebug_level2.lib")
		#else
			#pragma comment(lib, "corex_level2.lib")
		#endif
	#elif CORE_LEVEL == 3
		#ifdef _DEBUG
			#pragma comment(lib, "corexDebug_level3.lib")
		#else
			#pragma comment(lib, "corex_level3.lib")
		#endif
	#endif
#endif

#endif
