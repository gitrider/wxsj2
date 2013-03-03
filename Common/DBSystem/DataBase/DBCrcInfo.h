#ifndef _DB_CRCINFO_H_
#define _DB_CRCINFO_H_

#include "Type.h"
#include "ODBCBase.h"

class DBCrcInfo:public ODBCBase
{
	UINT			m_aid;
	GUID_t			m_CharGuid;
	UINT			m_Crc32;
	INT				m_ServerId;
	
	BOOL			m_Change;
public:
	DBCrcInfo(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();

	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetAid(UINT aid);
	UINT			GetAid();

	VOID			SetCharGuid(GUID_t gid);
	GUID_t			GetCharGuid();

	VOID			SetCrc32(UINT crc32);
	UINT			GetCrc32();

	VOID			SetServerId(INT sid);
	INT				GetServerId();

	VOID			SetChange(BOOL bc);
};


#endif