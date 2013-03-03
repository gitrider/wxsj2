#include "stdafx.h" 
#include "DBFatigueInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBFatigueInfo::DBFatigueInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBFatigueInfo::Load()
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

	pQuery->Parse(LoadCharFatigueInfo,CHAR_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBFatigueInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_TitleInfo,
	};

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	
	//保存PvPInfo数据
	do
	{
		CHAR	FatigueInfo[2048];
		memset(FatigueInfo,0,2048);
	
		//Binary2String((CHAR*)(&pCharFullData->m_Human.m_FatigueInfo),
		//			  sizeof(_FATIGUE_INFO),
		//			  FatigueInfo);
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateCharFatigueInfo,
					  CHAR_TABLE,
					  FatigueInfo,
					  m_CharGuid,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBFatigueInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBFatigueInfo::ParseResult(VOID* pResult)
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
				DB_FatigueInfo,
			};

			//加载技能属性
			//Assert(mResultCount<MAX_CHAR_MISSION_NUM);
			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					break;

				//UINT		CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				//mInterface->GetField(DB_FatigueInfo,
				//	(CHAR*)(&pCharFullData->m_Human.m_FatigueInfo),
				//	sizeof(_FATIGUE_INFO),
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

GUID_t	DBFatigueInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBFatigueInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBFatigueInfo::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBFatigueInfo::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}