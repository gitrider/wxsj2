#ifndef _DB_CHARFULLDATA_H_
#define _DB_CHARFULLDATA_H_

#include "Type.h"
#include "ODBCBase.h"

class DBCharFullData:public ODBCBase
{
	GUID_t			m_CharGuid;
	UINT			m_FullDataCRC;
	UINT			m_DebugStep;	//µ÷ÊÔÐÅÏ¢
public:
	DBCharFullData(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	ParseResult(VOID* pResult);

	VOID			SetCRC(UINT uCRC);
	UINT			GetCRC();

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	UINT			GetDebugStep(){return m_DebugStep;}
};

INT		FullDataMagicInt1(FULLUSERDATA* pData);
INT		FullDataMagicInt2(FULLUSERDATA* pData);


#endif