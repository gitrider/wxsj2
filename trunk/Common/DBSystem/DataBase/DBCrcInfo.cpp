#include "stdafx.h" 
#include "DBCrcInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBCrcInfo::DBCrcInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;

	m_aid			= INVALID_ID;
	m_CharGuid		= INVALID_ID;
	m_Crc32			= 0;
	m_ServerId		= -1;
	m_Change		= FALSE;
}

BOOL DBCrcInfo::Load()
{
__ENTER_FUNCTION
	LONG_DB_QUERY* pQuery = GetLongInterQuery();
	if(!pQuery)
	{
		Assert(FALSE);
	}
	pQuery->Clear();

	if(m_aid==INVALID_ID)
	{
		return FALSE;
	}

	pQuery->Parse(LoadCrcInfo,CRC32_TABLE,m_aid);

	return ODBCBase::LongLoad();

__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCrcInfo::Save(VOID* pSource)
{
__ENTER_FUNCTION

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	//保存CRC校验数据
	do
	{
		//FullData
		CHAR	FullDataInfo[102400];
		memset(FullDataInfo,0,102400);

		Binary2String((CHAR*)(pCharFullData),
			sizeof(FULLUSERDATA),
			FullDataInfo);
		//LogoutTime
		UINT nLogoutTime = pCharFullData->m_Human.m_LastLogoutTime;
		
		//Prepare Query
		LONG_DB_QUERY* pQuery = GetLongInterQuery();
		if(!pQuery)
		{
			Assert(FALSE);
		}
		pQuery->Clear();

		if(m_CharGuid == INVALID_ID)
		{
			return FALSE;
		}

		//Parse Query
		pQuery->Parse(SaveCrcInfo,
			CRC32_TABLE,
			m_CharGuid,
			nLogoutTime,
			m_Crc32,
			FullDataInfo,
			1,
			m_ServerId);

		//Execute Query
		if(!ODBCBase::LongSave(pCharFullData))
			return FALSE;

	}while(0);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCrcInfo::Delete()
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

	if(m_Change)
	{
		pQuery->Parse(ChangeCrcInfo,CRC32_TABLE,m_CharGuid);
	}
	else
	{
		pQuery->Parse(DeleteCrcInfo,CRC32_TABLE,m_CharGuid);
	}

	return ODBCBase::Delete();

__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCrcInfo::ParseResult(VOID* pResult)
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
					DB_LogoutTime,
					DB_Crc32,
					DB_FullData,
					DB_Sign,
					DB_ServerId,
				};

				//加载crc信息
				if(0>mResultCount)
					return FALSE;

				Assert(mInterface);
				INT ErrorCode;

				if(!mInterface->Fetch())
					return FALSE;

				m_CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				m_Crc32		= mInterface->GetUInt(DB_Crc32,ErrorCode);
					mInterface->GetLongField(DB_FullData,
						(CHAR*)(pCharFullData),
						sizeof(FULLUSERDATA),
						ErrorCode);
				m_ServerId	= mInterface->GetInt(DB_ServerId,ErrorCode);

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
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT	DBCrcInfo::GetAid()
{
	return m_aid;
}

VOID	DBCrcInfo::SetAid(UINT aid)
{
	m_aid = aid;
}

GUID_t	DBCrcInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBCrcInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBCrcInfo::GetCrc32()
{
	return m_Crc32;
}

VOID	DBCrcInfo::SetCrc32(UINT crc32)
{
	m_Crc32 = crc32;
}

INT		DBCrcInfo::GetServerId()
{
	return m_ServerId;
}

VOID	DBCrcInfo::SetServerId(INT sid)
{
	m_ServerId = sid;
}

VOID	DBCrcInfo::SetChange(BOOL bc)
{
	m_Change = bc;
}