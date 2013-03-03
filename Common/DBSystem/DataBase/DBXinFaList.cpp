#include "stdafx.h"
#include "DBXinFaList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

static	int		XinFaCompare(const void* pVoid1, const void*pVoid2)
{
	if(!pVoid1 || !pVoid2)	return 0;
	const _OWN_XINFA* pXinFa1 = (const _OWN_XINFA*)pVoid1;
	const _OWN_XINFA* pXinFa2 = (const _OWN_XINFA*)pVoid2;
	if(pXinFa1->m_nXinFaID == pXinFa2->m_nXinFaID)	return 0;
	if(pXinFa1->m_nXinFaID == INVALID_ID)	return 1;
	if(pXinFa2->m_nXinFaID == INVALID_ID)	return -1;
	if(pXinFa1->m_nXinFaID > pXinFa2->m_nXinFaID)	return 1;
	if(pXinFa1->m_nXinFaID < pXinFa2->m_nXinFaID)	return -1;
	
	return 0;
}

DBXinFaList::DBXinFaList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBXinFaList::Load()
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

	pQuery->Parse(LoadCharXinFaList,XINFA_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBXinFaList::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_XinFaID,
		DB_XinFaLvl,
	};

	INT Result;
	if(!Delete())
		return FALSE;
	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	INT XinFaCount = 	pCharFullData->m_XinFa.m_Count;

	for(INT i =0;i<XinFaCount;i++)
	{
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

		pQuery->Parse(NewCharXinFa,
					  XINFA_TABLE,
					  m_CharGuid,
					  pCharFullData->m_XinFa.m_aXinFa[i].m_nXinFaID,
					  pCharFullData->m_XinFa.m_aXinFa[i].m_nLevel,
					  m_DBVersion
					 );

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;
	}
	
	return TRUE;

	__LEAVE_FUNCTION	

		return FALSE;
}

BOOL DBXinFaList::Delete()
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

	pQuery->Parse(DeleteCharXinFaList,XINFA_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBXinFaList::ParseResult(VOID* pResult)
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
				DB_XinFaID,
				DB_XinFaLvl,
			};

			//加载技能属性
			
			Assert(mResultCount<=MAX_CHAR_XINFA_NUM);
			Assert(mInterface);

			INT ErrorCode;
			pCharFullData->m_XinFa.m_Count	= 0;

			_OWN_XINFA tmpXinFa[MAX_CHAR_XINFA_NUM];
			for(INT nIdx=0;MAX_CHAR_XINFA_NUM>nIdx;++nIdx)
			{
				tmpXinFa[nIdx].m_nXinFaID = INVALID_ID;
				tmpXinFa[nIdx].m_nLevel = 0;
			}

			for(INT k =0;k<MAX_CHAR_XINFA_NUM;k++)
			{
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);
				SHORT	XinFaID  = mInterface->GetShort(DB_XinFaID,ErrorCode);
				UINT	XinFaLvl = mInterface->GetBYTE(DB_XinFaLvl,ErrorCode);
				tmpXinFa[k].m_nXinFaID	=	XinFaID;
				tmpXinFa[k].m_nLevel	=	XinFaLvl;
			}
			
			qsort(tmpXinFa,MAX_CHAR_XINFA_NUM,sizeof(_OWN_XINFA),XinFaCompare);

			for(INT i =0;i<MAX_CHAR_XINFA_NUM;i++)
			{
				if(tmpXinFa[i].m_nXinFaID == INVALID_ID)
					break;

				pCharFullData->m_XinFa.m_aXinFa[i].m_nXinFaID	=	tmpXinFa[i].m_nXinFaID;
				pCharFullData->m_XinFa.m_aXinFa[i].m_nLevel		=	tmpXinFa[i].m_nLevel;

				pCharFullData->m_XinFa.m_Count++;
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

GUID_t	DBXinFaList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBXinFaList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}
UINT	DBXinFaList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBXinFaList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}