// PetPlacard.cpp

#include "stdafx.h"
#include "Config.h"
#include "TimeManager.h"
#include "Obj_Human.h"
#include "GameTable.h"

#include "PetPlacard.h"

////////////////////////////////////////////////
// PetPlacard
////////////////////////////////////////////////
PetPlacard::PetPlacard(VOID)
{
	//m_aItems
}

PetPlacard::~PetPlacard(VOID)
{
}

BOOL PetPlacard::Init(VOID)
{
	return TRUE;
}

VOID PetPlacard::CleanUp(VOID)
{
	INT i;
	for(i = 0; i < PET_PLACARD_ITEM_NUM; i++)
	{
		if(!m_aItems[i].IsInvalid())
		{
			m_aItems[i].CleanUp();
		}
	}
}

BOOL PetPlacard::AddItem(const _PET_PLACARD_ITEM *pItem)
{
	Assert(pItem != NULL && "PetPlacard::AddItem");
	if(pItem != NULL)
	{
		INT i;
		for(i = 0; i < PET_PLACARD_ITEM_NUM; i++)
		{
			if(m_aItems[i].IsInvalid())
			{
				m_aItems[i] = *pItem;
				return TRUE;
			}
		}
	}
	return FALSE;
}

VOID PetPlacard::RemoveItem(INT nIndex)
{
	if(nIndex >= 0 && nIndex < PET_PLACARD_ITEM_NUM)
	{
		m_aItems[nIndex].CleanUp();
	}
}

INT PetPlacard::GetIndexByHumanGUID(GUID_t guidHuman)const
{
	if(guidHuman != INVALID_ID)
	{
		INT i;
		for(i = 0; i < PET_PLACARD_ITEM_NUM; i++)
		{
			if(m_aItems[i].GetHumanGUID() == guidHuman)
			{
				return i;
			}
		}
	}
	return -1;
}

const _PET_PLACARD_ITEM *PetPlacard::GetValidItem(INT nIndex)const
{
	if(nIndex >= 0 && nIndex < PET_PLACARD_ITEM_NUM)
	{
		if(!m_aItems[nIndex].IsInvalid())
			return &m_aItems[nIndex];
	}
	return NULL;
}

const _PET_PLACARD_ITEM *PetPlacard::FirstValidItem(INT *pnIndex)const
{
	INT i;
	for(i = 0; i < PET_PLACARD_ITEM_NUM; i++)
	{
		if(!m_aItems[i].IsInvalid())
		{
			if(pnIndex != NULL)
			{
				*pnIndex = i;
			}
			return &m_aItems[i];
		}
	}
	return NULL;
}

const _PET_PLACARD_ITEM *PetPlacard::PrevValidItem(INT *pnIndex)const
{
	if(pnIndex == NULL)
		return NULL;

	INT i;
	for(i = *pnIndex - 1; i >= 0; i--)
	{
		if(!m_aItems[i].IsInvalid())
		{
			*pnIndex = i;
			return &m_aItems[i];
		}
	}
	return NULL;
}

const _PET_PLACARD_ITEM *PetPlacard::NextValidItem(INT *pnIndex)const
{
	if(pnIndex == NULL)
		return NULL;

	INT i;
	for(i = *pnIndex + 1; i < PET_PLACARD_ITEM_NUM; i++)
	{
		if(!m_aItems[i].IsInvalid())
		{
			*pnIndex = i;
			return &m_aItems[i];
		}
	}
	return NULL;
}

////////////////////////////////////////////////
// PetPlacardSystem
////////////////////////////////////////////////
PetPlacardSystem::PetPlacardSystem(VOID)
{
	//m_PetPlacard
}

PetPlacardSystem::~PetPlacardSystem(VOID)
{
}

BOOL PetPlacardSystem::Init(VOID)
{
	BOOL bResult = m_PetPlacard.Init();
	if(!bResult)
		return bResult;

	return TRUE;
}

VOID PetPlacardSystem::CleanUp(VOID)
{
	m_PetPlacard.CleanUp();
}

BOOL PetPlacardSystem::HeartBeat(UINT uTime)
{
	uint	uCurTime, uPetPlacardTime;
	INT		nIndex;
	const _PET_PLACARD_ITEM *pItem;

	uCurTime		= g_pTimeManager->CurrentTime();
	uPetPlacardTime	= g_Config.m_ConfigInfo.m_PetPlacardTime;

	pItem = m_PetPlacard.FirstValidItem(&nIndex);
	while(pItem != NULL)
	{
		if(pItem->GetCreateTime() + uPetPlacardTime < uCurTime)
		{
			m_PetPlacard.RemoveItem(nIndex);
		}
		pItem = m_PetPlacard.NextValidItem(&nIndex);
	}
	return FALSE;
}

ORESULT PetPlacardSystem::IssuePlacard(Obj_Human *pIssuer, PET_GUID_t guidPet, const CHAR *pszMessage)
{
	//Assert(pIssuer != NULL && "PetPlacardSystem::IssuePlacard");
	//if(pIssuer == NULL)
	//{
	//	return OR_ERROR;
	//}

	//Assert(!guidPet.IsNull() && "PetPlacardSystem::IssuePlacard");
	//if(guidPet.IsNull())
	//{
	//	return OR_PET_PLACARD_NOT_APPOINT_PET;
	//}

	//Item *pPetItem = pIssuer->GetPetItem(guidPet);
	//Assert(pPetItem != NULL && "PetPlacardSystem::IssuePlacard");
	//if(pPetItem == NULL)
	//{
	//	return OR_PET_PLACARD_NOT_APPOINT_PET;
	//}

	//// �����ж�
	////1������Ϊ�豦�������Ա������������ͣ�
	////if(pPetItem->GetPetType() != _PET_DB_LOAD::PET_TYPE_BABY)
	////{
	////	return OR_PET_PLACARD_NEED_PET_TYPE;
	////}

	////2������Ϊ�ﵽ15������
	//if(pPetItem->GetLevel() < g_Config.m_ConfigInfo.m_PetPlacardNeedLevel)
	//{
	//	return OR_PET_PLACARD_NEED_PET_LEVEL;
	//}

	////3���豦���Ŀ���ֵ����Ϊ100��ֵ��
	//if(pPetItem->GetHappiness() < g_Config.m_ConfigInfo.m_PetPlacardNeedHappiness)
	//{
	//	return OR_PET_PLACARD_NEED_PET_HAPPINESS;
	//}

	////4������ĳ費�ܽ������ѡ�
	////...��������ʲô����һ������˵��ֻ�г豦�������������ǳ豦�������Ǳ���裿

	////5����������N=3000��
	////if(pPetItem->GetLife() < g_Config.m_ConfigInfo.m_PetPlacardNeedLife)
	////{
	////	return OR_PET_PLACARD_NEED_PET_LIFE;
	////}

	//// ����ͬʱ����1�����ϵĳ�����Ϣ
	//if(m_PetPlacard.GetIndexByHumanGUID(pIssuer->GetGUID()) != -1)
	//{
	//	return OR_PET_PLACARD_ONLY_CAN_ISSUE_ONE;
	//}

	//uint uTime;
	//uTime = g_pTimeManager->CurrentTime();

	//_PET_DB dbPet;
	//pPetItem->SaveValueTo(&dbPet);

	//_PET_DETAIL_ATTRIB attrPet;
	////INT nAtt_Physics;
	////INT nAtt_MagicNear;
	////INT nDef_Physics;
	////INT nDef_MagicNear;
	////INT nHit;
	////INT nMiss;
	////INT nCritical;
	//{
	//	INT nBaseParam = 0;
	//	INT nValue = 0;
	//	INT nAttribParam = 0;
	//	FLOAT fLevelRate = 0;
	//	FLOAT fAttribRate = 0.f;
	//	
	//	INT nLevel = pPetItem->GetLevel();
	//	FLOAT fGrowRate = pPetItem->GetGrowRate();

	//	//ħ��	��ʼħ��+����*������ħ����Ӱ��ϵ��*����+�ȼ�*�ȼ���ħ��Ӱ��ϵ��*�ɳ�
	//	//nBaseParam = g_TableInit.m_PetConfig.m_BaseMgcAttack;
	//	////fAttribRate = g_TableInit.m_PetConfig.m_Spr_MgcAttack_Pram;
	//	//fLevelRate = g_TableInit.m_PetConfig.m_Level_MgcAttack_Pram;
	//	////nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_SPR);
	//	//nAtt_MagicNear = (INT)(nBaseParam + /*nAttribParam*fAttribRate + */nLevel*fLevelRate*fGrowRate);
	//	////�﹥	��ʼ�﹥+����*�������﹥Ӱ��ϵ��*����+�ȼ�*�ȼ����﹥Ӱ��ϵ��*�ɳ�
	//	//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyAttack;
	//	//fAttribRate = g_TableInit.m_PetConfig.m_Str_PhyAttack_Pram;
	//	//fLevelRate = g_TableInit.m_PetConfig.m_Level_PhyAttack_Pram;
	//	//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_STR);
	//	//nAtt_Physics = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	//	////ħ��	��ʼħ��+����*������ħ��Ӱ��ϵ��*����+�ȼ�*�ȼ���ħ��Ӱ��ϵ��*�ɳ�
	//	//nBaseParam = g_TableInit.m_PetConfig.m_BaseMgcDefence;
	//	//fAttribRate = g_TableInit.m_PetConfig.m_Int_MgcDefence_Pram;
	//	//fLevelRate = g_TableInit.m_PetConfig.m_Level_MgcDefence_Pram;
	//	//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_INT);
	//	//nDef_Magic = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	//	////���	��ʼ�﹥+����*���ʶ����Ӱ��ϵ��*����+�ȼ�*�ȼ������Ӱ��ϵ��*�ɳ�
	//	//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyDefence;
	//	//fAttribRate = g_TableInit.m_PetConfig.m_Con_PhyDefence_Pram;
	//	//fLevelRate = g_TableInit.m_PetConfig.m_Level_PhyDefence_Pram;
	//	//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_CON);
	//	//nDef_Physics = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	//	////����	��ʼ�﹥+����*���ݶ�����Ӱ��ϵ��*����+�ȼ�*�ȼ�������Ӱ��ϵ��*�ɳ�
	//	//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyAttack;
	//	//fAttribRate = g_TableInit.m_PetConfig.m_Dex_Miss_Pram;
	//	//fLevelRate = g_TableInit.m_PetConfig.m_Level_Miss_Pram;
	//	//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX);
	//	//nHit = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	//	////����	��ʼ�﹥+����*���ݶ�����Ӱ��ϵ��*����+�ȼ�*�ȼ�������Ӱ��ϵ��*�ɳ�
	//	//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyAttack;
	//	//fAttribRate = g_TableInit.m_PetConfig.m_Dex_Hit_Pram;
	//	//fLevelRate = g_TableInit.m_PetConfig.m_Level_Hit_Pram;
	//	//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX);
	//	//nMiss = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	//	////����	��ʼ�﹥+����*���ݶԻ���Ӱ��ϵ��*����+�ȼ�*�ȼ��Ի���Ӱ��ϵ��*�ɳ�
	//	//nBaseParam = g_TableInit.m_PetConfig.m_BaseCritical;
	//	//fAttribRate = g_TableInit.m_PetConfig.m_Dex_Critical_Pram;
	//	//fLevelRate = g_TableInit.m_PetConfig.m_Level_Critical_Pram;
	//	//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX);
	//	//nCritical = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	//}
	////attrPet.Init(&dbPet, INVALID_ID, nAtt_Physics, nAtt_MagicNear, nDef_Physics, nDef_Magic, nHit, nMiss, nCritical, INVALID_ID, INVALID_ID);

	//const CHAR *pszHumanGuildName = NULL;

	//_PET_PLACARD_ITEM itemPlacard;
	//BOOL bResult = itemPlacard.Init(pIssuer->GetGUID(),
	//	pIssuer->GetName(),
	//	pIssuer->GetLevel(),
	//	pszHumanGuildName,
	//	pIssuer->GetMenPai(),
	//	&attrPet,
	//	pszMessage,
	//	uTime);

	//Assert(bResult && "PetPlacardSystem::IssuePlacard   _PET_PLACARD_ITEM::Init");
	//if(!bResult)
	//{
	//	return OR_PET_PLACARD_ISSUE_FAILED;
	//}

	//bResult = m_PetPlacard.AddItem(&itemPlacard);
	//if(!bResult)
	//{
	//	return OR_PET_PLACARD_ISSUE_FAILED;
	//}
	return OR_OK;
}

VOID PetPlacardSystem::CancelPlacard(GUID_t guidHuman)
{
	Assert(guidHuman != INVALID_GUID && "PetPlacardSystem::CancelPlacard");
	if(guidHuman == INVALID_GUID)
	{
		return ;
	}

	INT nIndex = m_PetPlacard.GetIndexByHumanGUID(guidHuman);
	m_PetPlacard.RemoveItem(nIndex);
}
