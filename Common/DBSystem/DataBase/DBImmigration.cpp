#include "stdafx.h" 
#include "DBImmigration.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "TimeManager.h"


DBImmigration::DBImmigration(ODBCInterface*	pInterface)
{
	__ENTER_FUNCTION

	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	memset(m_Account,0,MAX_ACCOUNT+1);
	memset(m_CharName,0,MAX_CHARACTER_NAME+1);
	m_CreateTime		= 0;
	m_CharGuid			= INVALID_GUID;
	
	Assert(pInterface);
	mInterface		=	pInterface;

	__LEAVE_FUNCTION

}

BOOL DBImmigration::Load()
{
__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_CharGuid==INVALID_GUID)
	{
		return FALSE;
	}

	pQuery->Parse(MigLoadChar,CHAR_TABLE,m_CharGuid);

	return ODBCBase::Load();
__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBImmigration::IsCharNameOK()
{
__ENTER_FUNCTION
	do 
	{
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();

		if(m_CharName[0]=='\0')
		{
			return FALSE;
		}
		pQuery->Parse(MigCheckCharName,CHAR_TABLE,m_CharName);

		if(!ODBCBase::Load())
			return FALSE;

		if(mResultCount != 0)
		{
			Assert(mInterface);
			if(!mInterface->Fetch())	
				return FALSE;

			mInterface->Clear();
			return FALSE;
		}
		mInterface->Clear();
	} 
	while(0);

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBImmigration::UpdateNewCharName()
{
__ENTER_FUNCTION
	do 
	{
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();

		if(m_CharName[0]=='\0')
		{
			return FALSE;
		}

		if(m_CharGuid==INVALID_GUID)
		{
			return FALSE;
		}

		if(!StrSafeCheck(m_CharName,MAX_CHARACTER_NAME))
		{
			return FALSE;
		}

		pQuery->Parse(UpdateCharName,CHAR_TABLE,m_CharName,m_CharGuid);

		if(!ODBCBase::Load())
			return FALSE;

		if(mResultCount == 0)
		{
			Assert(mInterface);
			mInterface->Clear();

			return FALSE;
		}
		mInterface->Clear();
	} 
	while(0);

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBImmigration::AddNew()
{
__ENTER_FUNCTION
	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if( m_Account[0]== '\0' || 
		INVALID_GUID == m_CharGuid ||
		m_CharName[0] == '\0')
	{
		return FALSE;
	}

	if(!StrSafeCheck(m_Account,MAX_ACCOUNT+1))
		return FALSE;

	pQuery->Parse(MigNewChar,
				 CHAR_TABLE,
				 m_Account,
				 m_CharGuid,
				 m_CharName,
				 "",
				 "",
				 0,			  //性别
				 1,           //级别
				 0,			  //enegry
				 0,			  //外观
				 0,			  //场景
				 100,		  //xPos
				 100,		  //zPos
				 9,			  //门派
				 5000,		  //hp
				 5000,		  //mp
				 0,			  //strikerpoint
				 5,			  //str
				 5,			  //con
				 5,			  //dex
				 5,		      //spr
				 5,			  //ipr
				 0,			  //remainpoints
				 0,			  //logouttime
				 0,			  //logintime
				 m_CreateTime,			  //createtime
				 0,			  //dbversion
				 0,			  //haircolor
				 0,			  //hairmodel
				 0,			  //facecolor
				 0,			  //facemodel
				 0,			  //vmoney
				 "",		  //settings
				 31234,			  //isvalid	// 使用这个数值来标识是移民中的数据[4/13/2007]
				 0,			  //exp
				 "",		  //pres
				 "",		  //shopinfo
				 "",		  //carrypet
				 -1,		  //guldid
				 -1,		  //teamid
				 0,			  //headid
				 0,			  //erecover
				 0,			  //vigor
				 0,			  //maxvigor
				 0,			  //vrecover
				 0,			  //energymax
				 0,			  //pwdeltime
				 "",		  //pinfo,
				 0,			  //bkscene,
				 0,			  //bkxpos,
				 0,			  //bkzpos,
				 "",		  //titleinfo
				 0,			  //dietime	
				 0,			  //bankmoney
				 20,		  //bankend
				 "",		  //cooldown
				 0	    	  //defeq
				 );

	return ODBCBase::AddNew();
__LEAVE_FUNCTION	
	return FALSE;
}

BOOL DBImmigration::Save(VOID* pResult)
{
__ENTER_FUNCTION

	//保存移民成功的数据
	do
	{
		if(INVALID_GUID == m_CharGuid)
			return FALSE;

		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();

		pQuery->Parse(MigFinishChar,
			CHAR_TABLE,
			m_CharGuid);

		if(!ODBCBase::Save(pResult))
			return FALSE;

	}while(0);

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBImmigration::ParseResult(VOID* pResult)
{
__ENTER_FUNCTION
	Assert(mInterface);

	BOOL bRet = FALSE;
	switch(mOPType)
	{
	case DB_ADDNEW:
		{
			//记录是否插入成功
			if(GetInternalAffectCount()>0)
			{
				bRet = TRUE;
			}
		}
		break;
	case DB_LOAD:
		{
			enum
			{
				DB_CHAR_ACC_NAME	= 1,
			};

			Assert(mResultCount<=1);
			Assert(mInterface);
			INT	   ErrorCode;

			if(!mInterface->Fetch())
				break;

			mInterface->GetString(DB_CHAR_ACC_NAME,m_Account,MAX_ACCOUNT,ErrorCode);
			bRet = TRUE;
		}
		break;
	case DB_SAVE:
		{
			bRet = TRUE;
		}
		break;
	default:
		break;
	}

	mInterface->Clear();
	return bRet;
__LEAVE_FUNCTION
	return FALSE;
}

const	CHAR*	DBImmigration::GetAccount()	const
{
	return m_Account;
}
VOID	DBImmigration::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(m_Account,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	m_Account[MAX_ACCOUNT] = '\0' ;
}

const	CHAR*	DBImmigration::GetCharName()	const
{
	return m_CharName;
}
VOID	DBImmigration::SetCharName(const CHAR*	pName)
{
	Assert(pName);
	strncpy(m_CharName,pName,MAX_CHARACTER_NAME*sizeof(CHAR));
	m_CharName[MAX_CHARACTER_NAME] = '\0' ;
}

GUID_t	DBImmigration::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBImmigration::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

VOID	DBImmigration::SetCharCreateTime(UINT createTime)
{
	m_CreateTime	=	createTime;
}

UINT	DBImmigration::GetCharCreateTime()
{
	return m_CreateTime;
}