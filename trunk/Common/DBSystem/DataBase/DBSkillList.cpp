#include "stdafx.h"
#include "DBSkillList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

static	int		SkillCompare(const void* pVoid1, const void*pVoid2)
{
	if(!pVoid1 || !pVoid2)	return 0;
	const _OWN_SKILL* pSkill1 = (const _OWN_SKILL*)pVoid1;
	const _OWN_SKILL* pSkill2 = (const _OWN_SKILL*)pVoid2;
	if(pSkill1->m_nSkillID == pSkill2->m_nSkillID)	return 0;
	if(pSkill1->m_nSkillID == INVALID_ID)	return 1;
	if(pSkill2->m_nSkillID == INVALID_ID)	return -1;
	if(pSkill1->m_nSkillID > pSkill2->m_nSkillID)	return 1;
	if(pSkill1->m_nSkillID < pSkill2->m_nSkillID)	return -1;

	return 0;
}

DBSkillList::DBSkillList(ODBCInterface* pInterface)
{
	mDBName			=		CHARACTER_DATABASE;
	mResult			=		0;
	mResultCount	=		0;
	m_CharGuid		=		INVALID_ID;
	m_DBVersion		=		0;

	Assert(pInterface);
	mInterface		=		pInterface;
}

BOOL DBSkillList::Load()
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

	pQuery->Parse(LoadCharSkillList,SKILL_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBSkillList::Save(VOID* pSource)
{
	
	__ENTER_FUNCTION
	enum 
	{
		DB_CharGuid	=	1,
		DB_SkillID,
		DB_DBVersion,
	};
	
	INT Result;
	if(!Delete())
		return FALSE;
	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	INT SkillCount = 	pCharFullData->m_Skill.m_Count;

	for(INT i=0;i<SkillCount;i++)
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

		pQuery->Parse(NewCharSkill,
					  SKILL_TABLE,
					  m_CharGuid,
					  pCharFullData->m_Skill.m_aSkill[i].m_nSkillID,
					  pCharFullData->m_Skill.m_aSkill[i].m_eState,
					  pCharFullData->m_Skill.m_aSkill[i].m_nLevel,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;

	}

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBSkillList::Delete()
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

	pQuery->Parse(DeleteCharSkillList,SKILL_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBSkillList::ParseResult(VOID* pResult)
{

	switch(mOPType) 
	{
	case DB_LOAD:
		{
			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);

			enum 
			{
				DB_CharGuid	=	1,
				DB_SkillID,
				DB_SkillState,
				DB_SkillLevel
			};

			//加载技能属性
			Assert(mResultCount<=MAX_CHAR_SKILL_NUM);
			Assert(mInterface);
			INT ErrorCode;
			
			pCharFullData->m_Skill.m_Count	=	0;
			
			_OWN_SKILL	tmpSkill[MAX_CHAR_SKILL_NUM];
			for(INT nIdx=0;MAX_CHAR_SKILL_NUM>nIdx;++nIdx)
			{
				tmpSkill[nIdx].m_nSkillID = INVALID_ID;
			}

			for(INT i =0;i<MAX_CHAR_SKILL_NUM;i++)
			{
				if(!mInterface->Fetch())
					break;
				tmpSkill[i].m_nSkillID	=	mInterface->GetShort(DB_SkillID,ErrorCode);
				tmpSkill[i].m_eState	=	(_OWN_SKILL::_OWN_SKILL_STATE)mInterface->GetShort(DB_SkillState,ErrorCode);
				tmpSkill[i].m_nLevel	=	mInterface->GetShort(DB_SkillLevel,ErrorCode);
			}

			qsort(tmpSkill,MAX_CHAR_SKILL_NUM,sizeof(_OWN_SKILL),SkillCompare);

			for(INT i =0;i<MAX_CHAR_SKILL_NUM;i++)
			{
				if(tmpSkill[i].m_nSkillID == INVALID_ID)
					break;

				pCharFullData->m_Skill.m_aSkill[i].m_nSkillID = tmpSkill[i].m_nSkillID;
				pCharFullData->m_Skill.m_aSkill[i].m_eState = tmpSkill[i].m_eState;
				pCharFullData->m_Skill.m_aSkill[i].m_nLevel = tmpSkill[i].m_nLevel;

				pCharFullData->m_Skill.m_Count++;	
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
}

GUID_t	DBSkillList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBSkillList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBSkillList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBSkillList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}