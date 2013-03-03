// GameStruct_Pet.cpp

#include "stdafx.h"
#include "GameStruct_Pet.h"


////////////////////////////////////////////////
// _PET_DETAIL_ATTRIB 宠物属性
////////////////////////////////////////////////
_PET_DETAIL_ATTRIB::_PET_DETAIL_ATTRIB(VOID)
{
	//m_GUID.Reset();

	m_ObjID			= INVALID_ID;
	m_nDataID		= INVALID_ID;
	memset(m_szName, 0, sizeof(m_szName));
	m_nAIType		= -1;
	//m_SpouseGUID.Reset();
	m_nLevel		= -1;
	m_nExp			= -1;
	m_nHP			= -1;
	m_nHPMax		= -1;

	m_nLife			= -1;
	m_byGeneration	= 0;
	m_byHappiness	= 0;

	m_nAtt_Near	= -1;
	m_nDef_Near	= -1;
	m_nAtt_Far 	= -1;	
	m_nDef_Far 	= -1;

	m_nAtt_MagicNear= -1;
	m_nAtt_MagicFar = -1;

	m_nDef_MagicNear= -1;
	m_nDef_MagicFar = -1;

	m_nHit			= -1;
	m_nMiss			= -1;
	m_nCritical		= -1;

	m_nModelID		= -1;
	m_nMountID		= -1;

	m_iStrengthPerception		= -1;
	m_iSmartnessPerception		= -1;
	m_iMindPerception			= -1;
	//m_SprPerception			= -1;
	m_iConstitutionPerception	= -1;

	m_Str			= -1;
	m_Con			= -1;
	m_Dex			= -1;
	//m_Spr			= -1;
	m_Int			= -1;
	m_GenGu			= -1;

	m_nRemainPoint	= -1;

	INT i;
	for(i = 0; i < MAX_PET_SKILL_COUNT; i++)
	{
		m_aSkill[i].m_nSkillID		= INVALID_ID;
	}
}

BOOL _PET_DETAIL_ATTRIB::Init(const _PET_DB *pPetDB,
							ObjID_t idObj,
							INT nAtt_Near,
							INT nDef_Near,
							INT nAtt_Far,	
							INT nDef_Far,
							INT nAtt_MagicNear,
							INT nAtt_MagicFar,
							INT nDef_MagicNear,
							INT nDef_MagicFar,
							INT nHit,
							INT nMiss,
							INT nCritical,
							INT nModelID,
							INT nMountID)
{
	Assert(pPetDB != NULL && "_PET_DETAIL_ATTRIB::InitFormDB");
	if(pPetDB == NULL)
		return FALSE;

	m_GUID			= pPetDB->m_GUID;

	m_ObjID			= idObj;
	m_nDataID		= pPetDB->m_nDataID;
	strncpy(m_szName, pPetDB->m_szName, sizeof(m_szName) - 1);
	m_nAIType		= pPetDB->m_AIType;
	m_SpouseGUID	= pPetDB->m_SpouseGUID;
	m_nLevel		= pPetDB->m_nLevel;
	m_nExp			= pPetDB->m_nExp;
	//m_nHP			= pPetDB->m_nHP;

	//m_nLife			= pPetDB->m_nLife;
	m_byGeneration	= pPetDB->m_byGeneration;
	m_byHappiness	= pPetDB->m_byHappiness;

	m_nAtt_Near	= nAtt_Near;
	m_nDef_Near	= nDef_Near;
	m_nAtt_Far 	= nAtt_Far;	
	m_nDef_Far 	= nDef_Far;	


	m_nAtt_MagicNear	= nAtt_MagicNear;
	m_nAtt_MagicFar = nAtt_MagicFar;

	m_nDef_MagicNear	= nDef_MagicNear;
	m_nDef_MagicFar	= nDef_MagicFar;

	m_nHit			= nHit;
	m_nMiss			= nMiss;
	m_nCritical		= nCritical;

	m_nModelID		= nModelID;
	m_nMountID		= nMountID;

	m_iStrengthPerception	= pPetDB->m_iStrengthPerception;
	m_iSmartnessPerception	= pPetDB->m_iSmartnessPerception;
	m_iMindPerception	= pPetDB->m_iMindPerception;
	m_iConstitutionPerception	= pPetDB->m_iConstitutionPerception;

	m_Str			= pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR];
	m_Con			= pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON];
	m_Dex			= pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX];
	//m_Spr			= pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_SPR];
	m_Int			= pPetDB->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT];
	m_GenGu			= pPetDB->m_nGenGu;

	m_nRemainPoint	= pPetDB->m_nRemainPoint;

	INT i;
	for(i = 0; i < MAX_PET_SKILL_COUNT; i++)
	{
		m_aSkill[i].m_nSkillID		= pPetDB->m_SkillList[i].m_nSkillID;
	}

	return TRUE;
}

VOID _PET_DETAIL_ATTRIB::CleanUp(VOID)
{
	m_GUID.Reset();

	m_ObjID			= INVALID_ID;
	m_nDataID		= INVALID_ID;
	memset(m_szName, 0, sizeof(m_szName));
	m_nAIType		= -1;
	m_SpouseGUID.Reset();
	m_nLevel		= -1;
	m_nExp			= -1;
	m_nHP			= -1;
	m_nHPMax		= -1;

	m_nLife			= -1;
	m_byGeneration	= 0;
	m_byHappiness	= 0;

	m_nAtt_Near	= -1;
	m_nDef_Near	= -1;
	m_nAtt_Far 	= -1;	
	m_nDef_Far 	= -1;	

	m_nAtt_MagicNear= -1;
	m_nAtt_MagicFar = -1;

	m_nDef_MagicNear= -1;
	m_nDef_MagicFar = -1;

	m_nHit			= -1;
	m_nMiss			= -1;
	m_nCritical		= -1;

	m_nModelID		= -1;
	m_nMountID		= -1;

	m_iStrengthPerception	= -1;
	m_iSmartnessPerception	= -1;
	m_iMindPerception	= -1;
	//m_SprPerception	= -1;
	m_iConstitutionPerception	= -1;

	m_Str			= -1;
	m_Con			= -1;
	m_Dex			= -1;
	//m_Spr			= -1;
	m_Int			= -1;
	m_GenGu			= -1;

	m_nRemainPoint	= -1;

	INT i;
	for(i = 0; i < MAX_PET_SKILL_COUNT; i++)
	{
		m_aSkill[i].m_nSkillID		= INVALID_ID;
	}
}


////////////////////////////////////////////////
// _PET_PLACARD_ITEM
////////////////////////////////////////////////
_PET_PLACARD_ITEM::_PET_PLACARD_ITEM(VOID)
{
	// 创建信息
	m_uCreateTime	= 0;

	// 宠主信息
	m_HumanGUID		= INVALID_GUID;
	memset(m_szHumanName, 0, sizeof(m_szHumanName));
	m_nHumanLevel	= -1;
	memset(m_szHumanGuildName, 0, sizeof(m_szHumanGuildName));
	m_nHumanMenPai	= INVALID_JOB;

	// 宠物信息
	//m_PetAttr;

	// 留言
	memset(m_szMessage, 0, sizeof(m_szMessage));
}

_PET_PLACARD_ITEM::~_PET_PLACARD_ITEM(VOID)
{
}


BOOL _PET_PLACARD_ITEM::Init(GUID_t HumanGUID,
					const CHAR *pszHumanName,
					INT nHumanLevel,
					const CHAR *pszHumanGuildName,
					INT nHumanMenPai,
					const _PET_DETAIL_ATTRIB *pPetAttr,
					const CHAR *pszMessage,
					uint uCreateTime)
{
	// 创建信息
	m_uCreateTime	= uCreateTime;

	// 宠主信息
	m_HumanGUID		= HumanGUID;

	if(pszHumanName != NULL)
		strncpy(m_szHumanName, pszHumanName, sizeof(m_szHumanName));
	else
		strncpy(m_szHumanName, "", sizeof(m_szHumanName));

	m_nHumanLevel	= nHumanLevel;

	if(pszHumanGuildName != NULL)
		strncpy(m_szHumanGuildName, pszHumanGuildName, sizeof(m_szHumanGuildName));
	else
		strncpy(m_szHumanGuildName, "", sizeof(m_szHumanGuildName));

	m_nHumanMenPai	= nHumanMenPai;

	// 宠物信息
	if(pPetAttr != NULL)
		m_PetAttr		= *pPetAttr;
	else
		m_PetAttr.CleanUp();

	// 留言
	if(pszMessage != NULL)
	{
		strncpy(m_szMessage, pszMessage, sizeof(m_szMessage));
	}
	else
	{
		strncpy(m_szMessage, "", sizeof(m_szMessage));
	}
	return TRUE;
}

VOID _PET_PLACARD_ITEM::CleanUp(VOID)
{
	// 创建信息
	m_uCreateTime	= 0;

	// 宠主信息
	m_HumanGUID		= INVALID_GUID;
	memset(m_szHumanName, 0, sizeof(m_szHumanName));
	m_nHumanLevel	= -1;
	memset(m_szHumanGuildName, 0, sizeof(m_szHumanGuildName));
	m_nHumanMenPai	= -1;

	// 宠物信息
	m_PetAttr.CleanUp();

	// 留言
	memset(m_szMessage, 0, sizeof(m_szMessage));
}
