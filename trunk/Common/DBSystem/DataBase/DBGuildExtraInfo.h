#ifndef _DB_FATIGUEINFO_H_
#define _DB_FATIGUEINFO_H_

#include "Type.h"
#include "ODBCBase.h"

class DBGuildExtraInfo:public ODBCBase
{
	GuildID_t		m_GuildID;
public:
	DBGuildExtraInfo(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();

	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetGuildID(GuildID_t guildid);
	GuildID_t		GetGuildID();
};


#endif