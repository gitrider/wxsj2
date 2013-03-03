#ifndef _DB_PETITEMINFO_H_
#define _DB_PETITEMINFO_H_

#include "Type.h"
#include "ODBCBase.h"


class DBPetItemInfo:public ODBCBase
{
	ID_t			mServerID;

public:
	DBPetItemInfo(ODBCInterface* pInterface);
	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
};


#endif