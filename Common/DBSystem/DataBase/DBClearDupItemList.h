#ifndef _DB_CLEARDUPITEMLIST_H_
#define _DB_CLEARDUPITEMLIST_H_

#include "Type.h"
#include "ODBCBase.h"

class DBClearDupItemList:public ODBCBase
{
	GUID_t			m_CharGuid;
	UINT			m_DBVersion;
	//DupItemList		m_DupList[DB_BANK_POS_END];		//保存复制物品的信息
public:
	DBClearDupItemList(ODBCInterface* pInterface);

	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	VOID			SetDBVersion(UINT version);
	UINT			GetDBVersion();
};


#endif