#ifndef __BASE_DEFINE_H__
#define __BASE_DEFINE_H__

#pragma once
//#define DECL_NAMESPACE(x)	namespace x {
//#define END_NAMESPACE(x)	}

enum E_NODE_NEIGHBOUR_LIE
{
	LOW_LEFTTOP		= 2,
	UP_LEFTTOP		= 3,
	LOW_RIGHTTOP	= 6,
	UP_RIGHTTOP		= 7,
	LOW_LEFTBOTTOM	= 0,
	UP_LEFTBOTTOM	= 1,
	LOW_RIGHTBOTTOM	= 4,
	UP_RIGHTBOTTOM	= 5
};

enum E_NODE_FLAT_NEIGHBOUR
{
	NEIGHBOUR_LEFT = 0,
	NEIGHBOUR_RIGHT = 1,
	NEIGHBOUR_BOTTOM = 2,
	NEIGHBOUR_TOP = 3
};




#endif

