#include "stdafx.h"
#include "DBDeleteCharOp.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBItemList.h"
#include "DBSkillList.h"
#include "DBXinFaList.h"
#include "DBAbilityList.h"
#include "DBTaskList.h"
#include "DBRelationList.h"
#include "DBImpactList.h"
#include "DBPetList.h"
#include "DBManager.h"
#include "DBSouXiaList.h"

#define MAX_DELAY_TIME			14	//�ӳ�ɾ��ʱ��

DBDeleteCharOp::DBDeleteCharOp(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	m_Account[0]		= '\0';
	m_AID				= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
	m_CharGuid		= INVALID_ID;

	__LEAVE_FUNCTION

}

BOOL DBDeleteCharOp::Load()
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

	pQuery->Parse(ClearDeltime,CHAR_TABLE,0/*uCrc*/,m_CharGuid);

	return ODBCBase::Load();

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBDeleteCharOp::AddNew()
{
	return TRUE;
}

BOOL DBDeleteCharOp::Delete()
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

	INT	iTime = g_pTimeManager->GetDayTime()+MAX_DELAY_TIME;
	//UINT uCrc = ConvertUChar(iTime,FALSE);
	
	pQuery->Parse(DeleteBaseChar,
				  "", //charname
				   m_Account,
				   m_CharGuid ,
				   iTime,0/*uCrc*/);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;

	
}

BOOL DBDeleteCharOp::ParseResult(VOID* pResult)
{	
	__ENTER_FUNCTION

	switch(mOPType) 
	{
	case DB_DELETE:
		{
			ASKDELETECHAR_RESULT* pAskResult = static_cast<ASKDELETECHAR_RESULT*>(pResult);
			Assert(pAskResult);
			
			//����ɾ�����
			enum	
			{
				DB_NextStep	= 1,
				DB_dbVersion,
			};

			Assert(mResultCount<=1);
			Assert(mInterface);
			if(!mInterface->Fetch())	
				return FALSE;

			INT ErrorCode;
			INT iNext = 0;
			INT iDBVersion = -1;

			iNext = mInterface->GetInt(DB_NextStep,ErrorCode);
			iDBVersion = mInterface->GetInt(DB_dbVersion,ErrorCode);

			mInterface->Clear();

			//���ݽ����������ɾ��
			if(iNext > 0 && iDBVersion >= 0)
			{
				*pAskResult = ASKDELETECHAR_SUCCESS;
			}
			else
			{
				*pAskResult = ASKDELETECHAR_INTERNAL_ERROR;
			}

			if(*pAskResult == ASKDELETECHAR_SUCCESS	)
			{
				//ɾ��װ��
				DBItemList	ItemListObject(mInterface);
				ItemListObject.SetCharGuid(m_CharGuid);
				ItemListObject.SetDBVersion(iDBVersion);
				if(!ItemListObject.Delete())
					return FALSE;
				ItemListObject.ParseResult(pAskResult);

				//ɾ������
				DBSkillList	SkillListObject(mInterface);
				SkillListObject.SetCharGuid(m_CharGuid);
				SkillListObject.SetDBVersion(iDBVersion);
				if(!SkillListObject.Delete())
					return FALSE;
				SkillListObject.ParseResult(pAskResult);

				//ɾ���ķ�
				DBXinFaList	XinFaListObject(mInterface);
				XinFaListObject.SetCharGuid(m_CharGuid);
				XinFaListObject.SetDBVersion(iDBVersion);
				if(!XinFaListObject.Delete())
					return FALSE;
				XinFaListObject.ParseResult(pAskResult);

				//ɾ�������
				DBAbilityList AbilityListObject(mInterface);
				AbilityListObject.SetCharGuid(m_CharGuid);
				AbilityListObject.SetDBVersion(iDBVersion);
				if(!AbilityListObject.Delete())
					return FALSE;
				AbilityListObject.ParseResult(pAskResult);

				//ɾ��������Ϣ
				DBTaskList	TaskListObject(mInterface);
				TaskListObject.SetCharGuid(m_CharGuid);
				TaskListObject.SetDBVersion(iDBVersion);
				if(!TaskListObject.Delete())
					return FALSE;
				TaskListObject.ParseResult(pAskResult);

				//ɾ����ϵ�б�
				DBRelationList	RelationListObject(mInterface);
				RelationListObject.SetCharGuid(m_CharGuid);
				RelationListObject.SetDBVersion(iDBVersion);
				if(!RelationListObject.Delete())
					return FALSE;
				RelationListObject.ParseResult(pAskResult);

				//ɾ��Impact �б�
				DBImpactList	ImpactListObject(mInterface);
				ImpactListObject.SetCharGuid(m_CharGuid);
				ImpactListObject.SetDBVersion(iDBVersion);
				if(!ImpactListObject.Delete())
					return FALSE;
				ImpactListObject.ParseResult(pAskResult);

				////ɾ�������б�
				//DBPetList	PetListObject(mInterface);
				//PetListObject.SetCharGuid(m_CharGuid);
				//PetListObject.SetDBVersion(iDBVersion);
				//if(!PetListObject.Delete())
				//	return FALSE;
				//PetListObject.ParseResult(pAskResult);

				////ɾ����ɫ��չ��Ϣ
				//DBCharExtraInfo	ExtraObject(mInterface);
				//ExtraObject.SetCharGuid(m_CharGuid);
				//ExtraObject.SetDBVersion(iDBVersion);
				//if(!ExtraObject.Delete())
				//	return FALSE;
				//ExtraObject.ParseResult(pAskResult);

				//ɾ��Impact �б�
				DBSouXiaList	SouXiaListObject(mInterface);
				SouXiaListObject.SetCharGuid(m_CharGuid);
				SouXiaListObject.SetDBVersion(iDBVersion);
				if(!SouXiaListObject.Delete())
					return FALSE;
				SouXiaListObject.ParseResult(pAskResult);
			}
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

const	CHAR*	DBDeleteCharOp::GetAccount()	const
{
	return m_Account;
}
VOID	DBDeleteCharOp::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(m_Account,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	m_Account[MAX_ACCOUNT] = '\0' ;
}
GUID_t	DBDeleteCharOp::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBDeleteCharOp::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

