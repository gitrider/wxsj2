#include "DBCharSaveLock.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"

DBCharSaveLock::DBCharSaveLock(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
	m_IsValidTime		= -1;

	__LEAVE_FUNCTION

}

BOOL DBCharSaveLock::Load()
{
	do 
	{
		enum	
		{
			DB_CHARIsValidSaveTime	= 1,
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
		pQuery->Parse(FetchSaveTime,m_CharGuid);

		if(!ODBCBase::Load())
			return FALSE;

	
		Assert(mInterface);
		if(!mInterface->Fetch())	
			return FALSE;

		INT ErrorCode;

		m_IsValidTime = mInterface->GetInt(DB_CHARIsValidSaveTime,ErrorCode);

		mInterface->Clear();
	} 
	while(0);


	return TRUE;
}


BOOL DBCharSaveLock::Save(VOID* pSource)
{

	enum	
	{
		DB_CHARIsValidSaveTime	= 1,
	};


	//保存角色存盘完毕信息
	do
	{
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		pQuery->Parse(UpdateSaveTime,m_CharGuid);

		if(!ODBCBase::Save(pSource))
			return FALSE;

	}while(0);

	return TRUE;
}

BOOL DBCharSaveLock::ParseResult(VOID* pResult)
{
	Assert(mInterface);
	mInterface->Clear();
	return TRUE;
}

VOID DBCharSaveLock::SetCharGuid(GUID_t guid)
{
	m_CharGuid = guid;
}
GUID_t	DBCharSaveLock::GetCharGuid()
{
	return m_CharGuid;
}
INT		DBCharSaveLock::GetIsValidTime()
{
	return m_IsValidTime;
}