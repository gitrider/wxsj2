/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   10:08
	filename: 	MapServer\Server\Obj\Horse\HorsePool.h
	file path:	MapServer\Server\Obj\Horse
	file base:	HorsePool
	file ext:	h
	author:		Richard
	
	purpose:	马专用内存池
*********************************************************************/

#include "Type.h"
#include "ObjPool.h"
#include "Obj_Horse.h"

#ifndef HorsePool_h__
#define HorsePool_h__


typedef ObjPool< Obj_Horse >	HorsePool;

extern HorsePool	*g_pHorsePool;

#endif // HorsePool_h__


