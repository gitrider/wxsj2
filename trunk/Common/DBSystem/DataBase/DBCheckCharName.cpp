#include "stdafx.h" 
#include "DBCheckCharName.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "TimeManager.h"


DBCheckCharName::DBCheckCharName(ODBCInterface*	pInterface)
{
__ENTER_FUNCTION
	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	
	m_CharName[0]		= 0;
	m_nResult			= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
__LEAVE_FUNCTION
}

BOOL DBCheckCharName::Load()
{
__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(0 == m_CharName[0])
	{
		return FALSE;
	}

	pQuery->Parse(CheckCharName,m_CharName);

	return ODBCBase::Load();

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBCheckCharName::ParseResult(VOID* pResult)
{
__ENTER_FUNCTION
	BOOL bRet = FALSE;
	switch(mOPType)
	{
	case DB_LOAD:
		{
			enum
			{
				DB_CheckResult =	1,
			};
			
			ASKCREATECHAR_RESULT* pAskResult = static_cast<ASKCREATECHAR_RESULT*>(pResult);

			Assert(mInterface);
			Assert(pAskResult);

			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					return FALSE;

				m_nResult = mInterface->GetInt(DB_CheckResult,ErrorCode);
				if(0 == m_nResult)	*pAskResult = ASKCREATECHAR_SAME_NAME;
				if(0 < m_nResult)	bRet=TRUE;
			}

			mInterface->Clear();
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

VOID	DBCheckCharName::SetCharName(const CHAR* pName)
{
	strncpy(m_CharName,pName,MAX_CHARACTER_NAME);
	m_CharName[MAX_CHARACTER_NAME] = '\0';
}

const CHAR* DBCheckCharName::GetCharName()
{
	return m_CharName;
}

INT	DBCheckCharName::GetCheckResult()
{
	return m_nResult;
}