#ifndef _DB_CITYINFO_H_
#define _DB_CITYINFO_H_

#include "Type.h"
#include "ODBCBase.h"


class DBCityInfo:public ODBCBase
{
public:
	DBCityInfo(ODBCInterface* pInterface);
	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
};


#endif