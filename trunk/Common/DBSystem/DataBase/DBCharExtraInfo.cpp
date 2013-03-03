#include "stdafx.h" 
#include "DBCharExtraInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "TimeManager.h"


DBCharExtraInfo::DBCharExtraInfo(ODBCInterface*	pInterface)
{
__ENTER_FUNCTION
	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	
	m_CharGuid			= INVALID_ID;
	m_DBVersion			= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
__LEAVE_FUNCTION
}

BOOL DBCharExtraInfo::Load()
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

	pQuery->Parse(LoadCharExtra,CHAR_EXTRATABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBCharExtraInfo::Save(VOID* pSource)
{
__ENTER_FUNCTION

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	//±£´æCharExtraÊý¾Ý
	do
	{
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();

		pQuery->Parse(SaveCharExtra,
			//CHAR_EXTRATABLE,
			m_CharGuid,
			m_DBVersion,
			0,//pCharFullData->m_Human.m_nBuyYuanBao,
			0,//pCharFullData->m_Human.m_uKillMonsterCount,
			0,//pCharFullData->m_Human.m_CommitNum,
			0,//pCharFullData->m_Human.m_uSafeMoney,
			0,//pCharFullData->m_Human.m_uUnlockTime,
			0,//pCharFullData->m_Human.m_bSafeBoxStatus,
			0,//pCharFullData->m_Human.m_IPRegion,
			0);//pCharFullData->m_Human.m_yPetNumExtra);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;

	}while(0);

	return TRUE;

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBCharExtraInfo::Delete()
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

	pQuery->Parse(DeleteCharExtra,CHAR_EXTRATABLE,m_CharGuid);

	return ODBCBase::Delete();

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBCharExtraInfo::ParseResult(VOID* pResult)
{
__ENTER_FUNCTION
	BOOL bRet = FALSE;
	switch(mOPType)
	{
	case DB_LOAD:
		{
			enum
			{
				DB_CharGuid =	1,
				DB_BuyYuanBao,
				DB_KillMonsterCount,
				DB_CommitNum,
				DB_SafeBoxMoney,
				DB_SafeBoxUnlockTime,
				DB_SafeBoxStatus,
				DB_IPRegion,
				DB_PetNumExtra,
			};

			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);
			
			Assert(mInterface);
			if(mResultCount==0)
			{
				mInterface->Clear();
				return TRUE;
			}

			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					return FALSE;

				//pCharFullData->m_Human.m_nBuyYuanBao = mInterface->GetInt(DB_BuyYuanBao,ErrorCode);
				//pCharFullData->m_Human.m_uKillMonsterCount = mInterface->GetUInt(DB_KillMonsterCount,ErrorCode);
				//pCharFullData->m_Human.m_CommitNum = (BYTE)(mInterface->GetBYTE(DB_CommitNum,ErrorCode));
				//pCharFullData->m_Human.m_uSafeMoney = mInterface->GetUInt(DB_SafeBoxMoney,ErrorCode);
				//pCharFullData->m_Human.m_uUnlockTime = mInterface->GetUInt(DB_SafeBoxUnlockTime,ErrorCode);
				//pCharFullData->m_Human.m_bSafeBoxStatus = mInterface->GetUInt(DB_SafeBoxStatus,ErrorCode);
				//pCharFullData->m_Human.m_IPRegion = mInterface->GetInt(DB_IPRegion,ErrorCode);
				//pCharFullData->m_Human.m_yPetNumExtra = mInterface->GetBYTE(DB_PetNumExtra,ErrorCode);
			}

			mInterface->Clear();
		}
		break;
	case DB_SAVE:
		{
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
	return bRet;
__LEAVE_FUNCTION
	return FALSE;
}

GUID_t	DBCharExtraInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBCharExtraInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBCharExtraInfo::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBCharExtraInfo::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}