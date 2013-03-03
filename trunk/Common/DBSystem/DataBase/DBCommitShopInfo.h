#ifndef _DB_COMMITSHOPINFO_H_
#define _DB_COMMITSHOPINFO_H_

#include "Type.h"
#include "ODBCBase.h"

class DBCommitShopInfo:public ODBCBase
{
	ID_t			mWorldID;
	ID_t			mServerID;
public:
	DBCommitShopInfo(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetServerID(ID_t	sid);
	ID_t			GetServerID();

	VOID			SetWorldID(ID_t	sid);
	ID_t			GetWorldID();
};


#endif