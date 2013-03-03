#ifndef _DB_CHECKCHARNAME_H_
#define _DB_CHECKCHARNAME_H_

#include "Type.h"
#include "ODBCBase.h"

class DBCheckCharName:public ODBCBase
{
	CHAR			m_CharName[MAX_CHARACTER_NAME];
	INT				m_nResult;
public:
	DBCheckCharName(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetCharName(const CHAR* pName);
	const CHAR*		GetCharName();

	INT				GetCheckResult();
};


#endif