#include "DBGuildExtraInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBGuildExtraInfo::DBGuildExtraInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_GuildID		= INVALID_ID;

	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBGuildExtraInfo::Load()
{
__ENTER_FUNCTION

	LONG_DB_QUERY* pQuery = GetLongInterQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_GuildID==INVALID_ID)
	{
		return FALSE;
	}

	pQuery->Parse(LoadWorldGuildExtraInfo,GUILD_TABLE,m_GuildID);

	return ODBCBase::LongLoad();

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBGuildExtraInfo::Save(VOID* pSource)
{
__ENTER_FUNCTION

	INT Result;
	enum 
	{
		DB_GuildID	=	1,
		DB_GuildExtraInfo,
	};

	GUILD_USER_EXTRA_DATA_DB_t* pGuildExtraInfo = static_cast<GUILD_USER_EXTRA_DATA_DB_t*>(pSource);
	Assert(pGuildExtraInfo);
	
	//±£´æExtraInfoÊý¾Ý
	do
	{
		CHAR    GuildExtraInfo[MAX_LONG_COLUMN_BUFFER] = {0};

		Binary2String((CHAR*)(pGuildExtraInfo),
			sizeof(GUILD_USER_EXTRA_DATA_DB_t)*USER_ARRAY_SIZE,
			GuildExtraInfo);

		LONG_DB_QUERY* pQuery = GetLongInterQuery();
		if(!pQuery)
		{
			Assert(FALSE);
		}
		pQuery->Clear();
		pQuery->Parse(SaveWorldGuildExtraInfo,GUILD_TABLE,GuildExtraInfo,m_GuildID);

		if(!ODBCBase::LongSave(&Result))
			return FALSE;

	}while(0);
	
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBGuildExtraInfo::Delete()
{
__ENTER_FUNCTION
	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_GuildID==INVALID_ID)
	{
		return FALSE;
	}

	pQuery->Parse(ClearWorldGuildExtraInfo,GUILD_TABLE,m_GuildID);

	return ODBCBase::Delete();
__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBGuildExtraInfo::ParseResult(VOID* pResult)
{
__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			GUILD_USER_EXTRA_DATA_DB_t* pGuildExtraInfo = static_cast<GUILD_USER_EXTRA_DATA_DB_t*>(pResult);
			Assert(pGuildExtraInfo);

			enum 
			{
				DB_GuildID	=	1,
				DB_GuildExtraInfo,
			};

			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->LongFetch())
					break;

				mInterface->GetLongField(DB_GuildExtraInfo,
					(CHAR*)(pGuildExtraInfo),sizeof(GUILD_USER_EXTRA_DATA_DB_t)*USER_ARRAY_SIZE,ErrorCode);
			}
			
			mInterface->Clear();
		}
		break;
	case DB_DELETE:
		{
	
		}
		break;
	default:
		break;
	}
	
	Assert(mInterface);
	mInterface->Clear();
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

GuildID_t	DBGuildExtraInfo::GetGuildID()
{
	return m_GuildID;
}

VOID	DBGuildExtraInfo::SetGuildID(GuildID_t guildid)
{
	m_GuildID	 = guildid;
}
