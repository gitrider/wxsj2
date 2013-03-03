#include "stdafx.h"
#include "DBPvPInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBPvPInfo::DBPvPInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBPvPInfo::Load()
{
	__ENTER_FUNCTION

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

	pQuery->Parse(LoadCharPvPInfo,CHAR_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBPvPInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	
	//保存PvPInfo数据
	do
	{
		CHAR	PvPInfo[2048];
		memset(PvPInfo,0,2048);
	
		//Binary2String((CHAR*)(&pCharFullData->m_Human.m_PvpInfo),
		//			  sizeof(_PVPInfo),
		//			  PvPInfo);
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateCharPvPInfo,
					  CHAR_TABLE,
					  PvPInfo,
					  m_CharGuid,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPvPInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPvPInfo::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);

			enum 
			{
				DB_CharGuid	=	1,
				DB_PvPInfo,
			};

			//加载技能属性
			//Assert(mResultCount<MAX_CHAR_MISSION_NUM);
			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT		CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				//mInterface->GetField(DB_PvPInfo,
				//	(CHAR*)(&pCharFullData->m_Human.m_PvpInfo),
				//	sizeof(_PVPInfo),
				//	ErrorCode) ;
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

GUID_t	DBPvPInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBPvPInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBPvPInfo::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBPvPInfo::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}