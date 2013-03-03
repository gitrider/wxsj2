#ifndef _DB_CHARDBVERSION_H_
#define _DB_CHARDBVERSION_H_

#include "Type.h"
#include "ODBCBase.h"

class DBChardbVersion:public ODBCBase
{
	INT				m_curdbVersion;
	INT				m_isValid;
	GUID_t			m_CharGuid;
public:
	DBChardbVersion(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	ParseResult(VOID* pResult);
	VOID			ForceCheckConnect();
public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	INT				GetdbVersion();

	BOOL			IsValid();
};


#endif