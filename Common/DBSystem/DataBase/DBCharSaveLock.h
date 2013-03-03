#ifndef _DB_CHARSAVELOCK_H_
#define _DB_CHARSAVELOCK_H_

#include "Type.h"
#include "ODBCBase.h"

enum DBCharLock
{
	DBCL_CHAR_NOT_FOUND	=	1,
	DBCL_OTHER_SAVE		=	2,
	DBCL_SAVE_FREE		=	3,
	DBCL_NOT_GET		=	4
};

class DBCharSaveLock:public ODBCBase
{
	INT				m_IsValidTime;
	GUID_t			m_CharGuid;
public:
	DBCharSaveLock(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	ParseResult(VOID* pResult);
	virtual BOOL	Save(VOID* pSource);

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	INT				GetIsValidTime();

};


#endif