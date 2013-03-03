#include "stdafx.h" 
#include "DBChardbVersion.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"

DBChardbVersion::DBChardbVersion(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
	m_curdbVersion		= -1;
	m_isValid			= -1;
	__LEAVE_FUNCTION

}

BOOL DBChardbVersion::Load()
{
	do 
	{
		enum	
		{
			DB_CHARcurdbVersion	= 1,
			DB_CHARisValid,
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
		pQuery->Parse(LoadChardbVersion,CHAR_TABLE,m_CharGuid);

		if(!ODBCBase::Load())
			return FALSE;

	
		Assert(mInterface);
		if(!mResultCount)	//可能是已经删除的玩家数据
		{
			m_curdbVersion		= -1;
			m_isValid			= -1;
			return TRUE;
		}

		if(!mInterface->Fetch())	
			return FALSE;

		INT ErrorCode;

		m_curdbVersion = mInterface->GetInt(DB_CHARcurdbVersion,ErrorCode);
		m_isValid	   = mInterface->GetInt(DB_CHARisValid,ErrorCode);

		mInterface->Clear();
	} 
	while(0);


	return TRUE;
}

BOOL DBChardbVersion::ParseResult(VOID* pResult)
{
	Assert(mInterface);
	mInterface->Clear();
	return TRUE;
}

VOID DBChardbVersion::SetCharGuid(GUID_t guid)
{
	m_CharGuid = guid;
}
GUID_t	DBChardbVersion::GetCharGuid()
{
	return m_CharGuid;
}
INT		DBChardbVersion::GetdbVersion()
{
	return m_curdbVersion;
}

BOOL	DBChardbVersion::IsValid()
{
	return (m_isValid==1)?TRUE:FALSE;
}

VOID	DBChardbVersion::ForceCheckConnect()
{
	CheckDBConnect("DBChardbVersion");
}