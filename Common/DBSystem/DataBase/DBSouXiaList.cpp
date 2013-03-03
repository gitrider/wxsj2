#include "stdafx.h"
#include "DBSouXiaList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DBManager.h"

DBSouXiaList::DBSouXiaList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	Assert(pInterface);
	mInterface		=	pInterface;
}


BOOL DBSouXiaList::Load()
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

	pQuery->Parse(LoadCharSouXiaList, CHAR_SOUXIATABLE, m_CharGuid, 0, MAX_SOUXIA_CONTAINER);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBSouXiaList::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_SouXiaInfo,
	};

	//INT Result;
	//if(!Delete())
	//	return FALSE;
	//ParseResult(&Result);


	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	//保存Title数据
	do
	{
		for (int i=0; i<MAX_SOUXIA_CONTAINER; ++i)
		{	
			//if ( pCharFullData->m_SouXia.m_SouXiaData[i].m_CurPos >=0 
			//	&& pCharFullData->m_SouXia.m_SouXiaData[i].m_SouXiaID > 0)
			//{
				CHAR	SouXiaData[2048];
				memset(SouXiaData,0,2048);

				Binary2String((CHAR*)(&(pCharFullData->m_SouXia.m_SouXiaData[i])), sizeof(SOUXIA_DATA), SouXiaData);

				DB_QUERY* pQuery = GetInternalQuery();
				if(!pQuery)
				{
					Assert(FALSE);
				}

				pQuery->Clear();

				pQuery->Parse(NewCharSouXia,m_CharGuid,pCharFullData->m_SouXia.m_SouXiaData[i].m_CurPos,SouXiaData,i);

				if(!ODBCBase::Save(pCharFullData))
					return FALSE;
			//}
		}

	}while(0);

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBSouXiaList::Delete()
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

	pQuery->Parse(DeleteCharSouXia,CHAR_SOUXIATABLE,m_CharGuid);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBSouXiaList::ParseResult(VOID* pResult)
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
					DB_SouXiaData = 1,
				};

				//加载技能属性
				//Assert(mResultCount<MAX_CHAR_MISSION_NUM);
				Assert(mInterface);
				INT ErrorCode;
				for(INT i =0; i<MAX_SOUXIA_CONTAINER; ++i)
				{
					if(!mInterface->Fetch())
						break;

					mInterface->GetField(DB_SouXiaData,
						(CHAR*)(&pCharFullData->m_SouXia.m_SouXiaData[i]),
						sizeof(SOUXIA_DATA), ErrorCode) ;
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

GUID_t	DBSouXiaList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBSouXiaList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBSouXiaList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBSouXiaList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}

