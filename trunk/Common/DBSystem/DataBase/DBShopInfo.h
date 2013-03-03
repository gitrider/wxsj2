#ifndef _DB_SHOPINFO_H_
#define _DB_SHOPINFO_H_

#include "Type.h"
#include "ODBCBase.h"


class DBShopInfo:public ODBCBase
{
	ID_t			mServerID;

public:
	DBShopInfo(ODBCInterface* pInterface);
	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	
	VOID			SetServerID(ID_t	sid);
	ID_t			GetServerID();
};


#endif