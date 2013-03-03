#ifndef _DB_GLOBALDATA_H_
#define _DB_GLOBALDATA_H_

#include "Type.h"
#include "ODBCBase.h"

class DBGlobalData:public ODBCBase
{
	ID_t			m_ServerID;
public:
	DBGlobalData(ODBCInterface* pInterface);
	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
};


#endif