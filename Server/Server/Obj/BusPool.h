// BusPool.h

#ifndef __BUSPOOL_H__
#define __BUSPOOL_H__

#include "Type.h"
#include "ObjPool.h"
#include "Obj_Bus.h"

typedef ObjPool< Obj_Bus > BusPool;

extern BusPool* g_pBusPool;

#endif // __BUSPOOL_H__
