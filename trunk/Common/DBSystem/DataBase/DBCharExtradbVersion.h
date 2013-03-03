#ifndef _DB_CHARDBVERSION_H_
#define _DB_CHARDBVERSION_H_

#include "Type.h"
#include "ODBCBase.h"

#define NOT_FIND_EXTRA_LINE_IN_DB	-2

class DBCharExtradbVersion:public ODBCBase
{
	INT				m_curdbVersion;
	GUID_t			m_CharGuid;
public:
	DBCharExtradbVersion(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	ParseResult(VOID* pResult);
	VOID			ForceCheckConnect();
public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	INT				GetdbVersion();
};


#endif