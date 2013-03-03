#include "stdafx.h" 
#include "DBCharExtradbVersion.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"

DBCharExtradbVersion::DBCharExtradbVersion(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
	m_curdbVersion		= -1;
	__LEAVE_FUNCTION

}

BOOL DBCharExtradbVersion::Load()
{
	do 
	{
		enum	
		{
			DB_CHARExtracurdbVersion	= 1,
		};

		DB_QUERY* pQuery = GetInternalQuery();
		if(!pQuery)
		{
			Assert(FALSE);
		}
		pQuery->Clear();

		if(m_CharGuid==INVALID_ID)
		{
			return FALSE;
		}
		pQuery->Parse(LoadCharExtradbVersion,CHAR_EXTRATABLE,m_CharGuid);

		if(!ODBCBase::Load())
			return FALSE;

	
		Assert(mInterface);
		if(!mResultCount)	//可能是已经删除的玩家数据
		{
			//很久未登录游戏的旧帐号可能压根就没有这条数据[2008/02/18 YangJun]
			m_curdbVersion		= NOT_FIND_EXTRA_LINE_IN_DB;
			return TRUE;
		}

		if(!mInterface->Fetch())	
			return FALSE;

		INT ErrorCode;

		m_curdbVersion = mInterface->GetInt(DB_CHARExtracurdbVersion,ErrorCode);
		
		mInterface->Clear();
	} 
	while(0);


	return TRUE;
}

BOOL DBCharExtradbVersion::ParseResult(VOID* pResult)
{
	Assert(mInterface);
	mInterface->Clear();
	return TRUE;
}

VOID DBCharExtradbVersion::SetCharGuid(GUID_t guid)
{
	m_CharGuid = guid;
}
GUID_t	DBCharExtradbVersion::GetCharGuid()
{
	return m_CharGuid;
}
INT		DBCharExtradbVersion::GetdbVersion()
{
	return m_curdbVersion;
}

VOID	DBCharExtradbVersion::ForceCheckConnect()
{
	CheckDBConnect("DBCharExtradbVersion");
}