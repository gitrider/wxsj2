// BusManager.h
// 
//////////////////////////////////////////////////////

#ifndef __BUSMANAGER_H__
#define __BUSMANAGER_H__

#include "Type.h"
#include "ObjSingleManager.h"

class Obj_Bus;
class BusManager :	
	public ObjSingleManager
{
public :
	BusManager(VOID);
	virtual ~BusManager(VOID);

	virtual BOOL HeartBeat(UINT uTime = 0);

	Obj_Bus *GetBus(ObjID_t idObj);

	Obj_Bus *GetBusByGUID(GUID_t uGUID);

public :
	BOOL LoadBus(const CHAR *pszFileName);
	BOOL RemoveAllBus(VOID);

protected:
	enum
	{
		MAX_REMOVE_SIZE = 1024
	};
	INT m_nRemoveCount;
	Obj_Bus *m_pNeedRemoveBuss[MAX_REMOVE_SIZE];
};


#endif // __BUSMANAGER_H__
