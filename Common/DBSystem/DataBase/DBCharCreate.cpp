#include "stdafx.h" 
#include "DBCharCreate.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "TimeManager.h"


DBCharCreate::DBCharCreate(ODBCInterface*	pInterface)
{
	__ENTER_FUNCTION

	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	m_Account[0]		= '\0';
	m_AID				= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
	m_CharGuid			= INVALID_ID;
	m_Country = 0;
	__LEAVE_FUNCTION

}

BOOL DBCharCreate::Load()
{
		return TRUE;
}

BOOL DBCharCreate::AddNew()
{
__ENTER_FUNCTION
	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_Account[0]=='\0')
	{
		return FALSE;
	}

	if(!StrSafeCheck(m_Account,MAX_ACCOUNT+1))
		return FALSE;

	if(!StrSafeCheck(m_CharName,MAX_CHARACTER_NAME))
		return FALSE;

	pQuery->Parse(CreateChar,
				 m_Account,
				 m_CharName,
				 m_Sex,		  //ÐÔ±ð
				 g_pTimeManager->CurrentDate(),	//createtime
				 m_HairColor, //haircolor
				 m_HairModel, //hairmodel
				 m_FaceColor, //facecolor
				 m_FaceModel, //facemodel
				 m_HeadID,	  //headid
				 m_DefEquip,	  //defeq
				 m_Country
				 );

	return ODBCBase::AddNew();
__LEAVE_FUNCTION	
	return FALSE;
}

BOOL DBCharCreate::ParseResult(VOID* pResult)
{
__ENTER_FUNCTION
	BOOL bRet = FALSE;
	switch(mOPType)
	{
	case DB_ADDNEW:
		{
			enum
			{
				DB_CreatResult = 1,
				DB_CharGuid,
			};

			enum
			{
				Duplicate_CharName	= -2,
				Invalid_CharGuid	= -3,
				CreateOk			= 1,
			};

			ASKCREATECHAR_RESULT* pAskResult = static_cast<ASKCREATECHAR_RESULT*>(pResult);
			
			Assert(pAskResult);
			Assert(mInterface);
			if(!mInterface->Fetch())	
				return FALSE;

			INT ErrorCode;
			INT ResultCode = mInterface->GetInt(DB_CreatResult,ErrorCode);
			m_CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);

			switch(ResultCode)
			{
			case Duplicate_CharName:
				*pAskResult = ASKCREATECHAR_SAME_NAME;
				break;
			case CreateOk:
				*pAskResult = ASKCREATECHAR_SUCCESS;
				bRet = TRUE;
				break;
			case Invalid_CharGuid:
			default:
				*pAskResult = ASKCREATECHAR_INTERNAL_ERROR;
				break;
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

const	CHAR*	DBCharCreate::GetAccount()	const
{
	return m_Account;
}
VOID	DBCharCreate::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(m_Account,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	m_Account[MAX_ACCOUNT] = '\0' ;
}

VOID	DBCharCreate::SetCharName(const CHAR* pName)
{
	strncpy(m_CharName,pName,MAX_CHARACTER_NAME);
	m_CharName[MAX_CHARACTER_NAME] = '\0';
}

const CHAR* DBCharCreate::GetCharName()
{
	return m_CharName;
}

VOID DBCharCreate::SetSex(BOOL bSex)
{
	m_Sex = bSex;
}

BOOL DBCharCreate::GetSex()
{
	return m_Sex;
}

VOID	DBCharCreate::SetHairColor(BYTE Color)
{
	m_HairColor	= Color;
}
BYTE	DBCharCreate::GetHairColor()
{
	return m_HairColor;
}

VOID	DBCharCreate::SetFaceColor(BYTE Color)
{
	m_FaceColor	= Color;
}
BYTE	DBCharCreate::GetFaceColor()
{
	return m_FaceColor;
}

VOID	DBCharCreate::SetHairModel(BYTE Model)
{
	m_HairModel = Model;
}
BYTE	DBCharCreate::GetHairModel()
{
	return m_HairModel;
}

VOID	DBCharCreate::SetFaceModel(BYTE Model)
{
	m_FaceModel = Model;
}
BYTE	DBCharCreate::GetFaceModel()
{
	return m_FaceModel;
}

VOID	DBCharCreate::SetHeadID(BYTE Head)
{
	m_HeadID = Head;
}
BYTE	DBCharCreate::GetHeadID()
{
	return m_HeadID;
}

VOID	DBCharCreate::SetDefEquip(INT defEquip)
{
	m_DefEquip = defEquip;
}

INT	DBCharCreate::GetDefEquip()
{
	return m_DefEquip;
}

GUID_t DBCharCreate::GetCharGuid()
{
	return m_CharGuid;
}

VOID			DBCharCreate::SetCountry(INT Country)
{
	m_Country = Country;
}
INT				DBCharCreate::GetCountry()
{
	return m_Country;
}
