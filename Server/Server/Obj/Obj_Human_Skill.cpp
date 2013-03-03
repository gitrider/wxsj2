#include "stdafx.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Scene.h"
#include "Config.h"
#include "GameTable.h"
#include "GCCoolDownUpdate.h"
#include "Obj_Character.h"
#include "LogicManager.h"
#include "SkillInfo.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCDetailSkillList.h"
#include "GCDetailXinFaList.h"
#include "Impact_Core.h"
#include "ImpactLogic.h"


using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;
using namespace Packets;

const _SKILL_LIST& Obj_Human::Skill_GetSkillList(VOID)
{
	const _SKILL_LIST* pSkillList = (((HumanDB&)m_DB).GetSkillDB());
	if(NULL==pSkillList)
	{
		//应该永远不会到这里，内存申请在系统初始化是已经校验完成。这里不会出现空指针
		//这里必须终止服务器的运行，立即检查人物的初始化流程,可能是内存分配失败
		AssertEx(FALSE,"[Obj_Human::Skill_GetSkillList]:have not Skill list!!!");
	}
	return *pSkillList;
}

const _XINFA_LIST& Obj_Human::Skill_GetXinFaList(VOID)
{
	const _XINFA_LIST* pXinFaList = (((HumanDB&)m_DB).GetXinFaDB());
	if(NULL==pXinFaList)
	{
		//应该永远不会到这里，内存申请在系统初始化是已经校验完成。这里不会出现空指针
		//这里必须终止服务器的运行，立即检查人物的初始化流程,可能是内存分配失败
		AssertEx(FALSE,"[Obj_Human::Skill_GetXinFaList]:have not Xinfa list!!!");
	}
	return *pXinFaList;
}
_IMPACT_LIST& Obj_Human::Impact_GetImpactList(VOID)
{
	_IMPACT_LIST* pImpactList = (_IMPACT_LIST*)(((HumanDB&)m_DB).GetImpactDB());
	if(NULL==pImpactList)
	{
		//应该永远不会到这里，内存申请在系统初始化是已经校验完成。这里不会出现空指针
		//这里必须终止服务器的运行，立即检查人物的初始化流程,可能是内存分配失败
		AssertEx(FALSE,"[Obj_Human::Impact_GetImpactList]: Have not Impact List!!!");
	}
	return *pImpactList;
}

VOID 	Obj_Human::Impact_InitList(VOID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	memcpy(&rList,GetDB()->GetImpactDB(), sizeof(_IMPACT_LIST));
	BYTE& rCount=rList.m_Count;
	if(MAX_IMPACT_NUM<rCount)
	{
		AssertEx(FALSE,"[Obj_Character::Impact_InitList]: List.count > MAX_IMPACT_NUM! check it now");
		rCount=MAX_IMPACT_NUM;
	}
	Impact_SetSNSeed(rCount);
	
	for(short i=0; rCount>i; ++i)
	{
		rList.m_aImpacts[i].SetSN(i);
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			return;
		}
		pLogic->OnActive(rImp, static_cast<Obj_Character&>(*this));
		if(TRUE == Impact_IsOverTimed(rImp))
		{
			pLogic->MarkModifiedAttrDirty(rImp, static_cast<Obj_Character&>(*this));
		}
	}
	__LEAVE_FUNCTION
}
VOID	Obj_Human::SetCooldown(CooldownID_t const nID, Time_t nCooldown)
{
	__ENTER_FUNCTION
	m_DB.SetCooldown(nID, nCooldown);
	GCCooldownUpdate Msg;
	Msg.CooldownModified(nID, nCooldown);
	Player* pPlayer = GetPlayer();
	if(NULL != pPlayer)
	{
		pPlayer->SendPacket( &Msg ) ;
	}
	__LEAVE_FUNCTION
}
BOOL	Obj_Human::IsCooldowned(CooldownID_t const nID) const
{
	__ENTER_FUNCTION
	return m_DB.IsCooldowned(nID);
	__LEAVE_FUNCTION
	return FALSE;
}
Time_t	Obj_Human::GetCooldownRemained(CooldownID_t const nID) const
{
	__ENTER_FUNCTION
	return m_DB.GetCooldownRemain(nID);
	__LEAVE_FUNCTION
	return 0;
}

BOOL Obj_Human::HeartBeat_Cooldown(UINT uTime)
{
	__ENTER_FUNCTION
	INT nDeltaTime = GetLogicTime();
	m_DB.HeartBeat_Cooldown(nDeltaTime);
	__LEAVE_FUNCTION
	return TRUE;
}

BOOL Obj_Human::Skill_HaveSkill( SkillID_t const nID ) const
{
	__ENTER_FUNCTION
	Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	//职业是-1则所有职业都不必学就可以使用
	if( INVALID_ID==pSkillTemplateData->GetMenPai() )
	{
		return TRUE;
	}
	_SKILL_LIST const& rList = ((Obj_Human*)this)->Skill_GetSkillList();
	BYTE const& rCount=rList.m_Count;
	if (MAX_CHAR_SKILL_NUM < rCount)
	{
		Assert(!"Skill_HaveSkill");
		return FALSE;
	}

	for(INT i=0; rCount>i; ++i)
	{
		_OWN_SKILL const& rSkill = rList.m_aSkill[i];
		if(nID==rSkill.m_nSkillID && rSkill.m_nLevel>0 )
		{
			return TRUE;
		}
	}
	__LEAVE_FUNCTION
	return FALSE;
}
const _OWN_SKILL* Obj_Human::Skill_GetSkill(INT const nID)
{
	__ENTER_FUNCTION
	_SKILL_LIST const& rList = ((Obj_Human*)this)->Skill_GetSkillList();
	BYTE const& rCount=rList.m_Count;
	if (MAX_CHAR_SKILL_NUM < rCount)
	{
		Assert(!"Skill_GetSkill");
		return NULL;
	}

	for(INT i=0; rCount>i; ++i)
	{
		_OWN_SKILL const& rSkill = rList.m_aSkill[i];
		if(nID == rSkill.m_nSkillID)
		{
			return &rSkill;
		}
	}
	__LEAVE_FUNCTION
	return NULL;
}

VOID Obj_Human::Skill_SetSkillLevel(SkillID_t const nID, INT const nLevel)
{
	__ENTER_FUNCTION
	_SKILL_LIST const& rList = ((Obj_Human*)this)->Skill_GetSkillList();
	if (MAX_CHAR_SKILL_NUM < rList.m_Count)
	{
		Assert(!"Skill_SetSkillLevel");
		return;
	}
	// 查找当前技能
	for (INT i=0; i<rList.m_Count; ++i)
	{
		if (nID == rList.m_aSkill[i].m_nSkillID)
		{
			m_DB.SetSkillLevel(nID, i, nLevel);
			return;
		}
	}

	// 没有找到，添加新技能
	if (MAX_CHAR_SKILL_NUM > rList.m_Count)
	{
		_OWN_SKILL newSkill;

		newSkill.m_nSkillID = nID;
		newSkill.m_nLevel	= nLevel;

		m_DB.SetSkillID(newSkill, rList.m_Count);
		m_DB.SetSkillCount(rList.m_Count+1);
	}
	__LEAVE_FUNCTION
}

BOOL Obj_Human::Skill_ModifySkill(SkillID_t	const nID, INT const nModifyFlag)
{
	__ENTER_FUNCTION
	//
	if(0<nModifyFlag)
	{	// nModifyFlag >0, means Add a new skill
		return Skill_RegisterSkill(nID);
	}
	else if(0>nModifyFlag)
	{	// nModifyFlag<0, means del a known skill
		return Skill_UnregisterSkill(nID);
	}
	// nModify should be zero
	AssertEx(FALSE,"[Obj_Human::Skill_ModifySkill]:Modify flag == 0!!");
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL Obj_Human::Skill_HaveXinFa( SkillID_t const nID ) const
{
	__ENTER_FUNCTION
	return 0<Skill_GetXinFaLevel(nID);
	__LEAVE_FUNCTION
	return FALSE;
}
INT Obj_Human::Skill_GetXinFaLevel(SkillID_t const nID)const
{
	__ENTER_FUNCTION
	_XINFA_LIST const& rList = ((Obj_Human*)this)->Skill_GetXinFaList();
	BYTE const& rCount=rList.m_Count;
	for(INT i=0; rCount>i; ++i)
	{
		_OWN_XINFA const& rXinFa = rList.m_aXinFa[i];
		if(nID == rXinFa.m_nXinFaID)
		{
			return rXinFa.m_nLevel;
		}
	}
	__LEAVE_FUNCTION
	return 0;
}
VOID Obj_Human::Skill_AllXinFaLevel(INT const nLevel)
{
	__ENTER_FUNCTION
	const _XINFA_LIST& rList = Skill_GetXinFaList();;
	const BYTE& rCount=rList.m_Count;
	for(INT i=0; rCount>i; ++i)
	{
		//_OWN_XINFA& rXinFa = rList.m_aXinFa[i];
		//rXinFa.m_nLevel = nLevel;
		m_DB.SetXinfaLevel(nLevel,i);
	}
	
	__LEAVE_FUNCTION
}

BOOL Obj_Human::Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel)
{
	__ENTER_FUNCTION
	const _XINFA_LIST& rList = Skill_GetXinFaList();;
	BYTE rCount=rList.m_Count;
	for(INT i=0; rCount>i; ++i)
	{
		const _OWN_XINFA& rXinFa = rList.m_aXinFa[i];
		if(nID == rXinFa.m_nXinFaID)
		{
			//rXinFa.m_nLevel = nLevel;
			m_DB.SetXinfaLevel(nLevel,i);
			return TRUE;
		}
	}
	if(MAX_CHAR_XINFA_NUM>rCount)
	{
		const _OWN_XINFA& rNewXinFa = rList.m_aXinFa[rCount];
		m_DB.SetXinfaID(nID,rCount);
		m_DB.SetXinfaLevel(nLevel,rCount);
		m_DB.SetXinFaCount(rCount+1);
		return TRUE;
	}
	__LEAVE_FUNCTION
	return FALSE;
}
VOID 	Obj_Human::Skill_OnEquipItem(VOID)
{
	__ENTER_FUNCTION
	const _SKILL_LIST& rList = Skill_GetSkillList();
	BYTE rCount=rList.m_Count;
	for(INT i=0; rCount>i; ++i)
	{
		const _OWN_SKILL& rSkill = rList.m_aSkill[i];
		SkillLogic_T const* pLogic = g_SkillLogicList.GetLogicById(rSkill.m_nSkillID);
		if(NULL != pLogic)
		{
			//refix item attr here
			//pLogic->OnEquipItem(rSkill, *this);
		}
	}
	__LEAVE_FUNCTION
}
VOID	Obj_Human::Skill_InitSkillList()
{
}
VOID	Obj_Human::Skill_InitXinFaList()
{
}
BOOL 	Obj_Human::Skill_RegisterSkill(SkillID_t const nID)
{
	__ENTER_FUNCTION
	//const _SKILL_LIST& rList = Skill_GetSkillList();
	//BYTE rCount=rList.m_Count;
	////验证是否是合法的技能ID
	////...
	//for(INT i=0; rCount>i; ++i)
	//{
	//	const _OWN_SKILL& rSkill = rList.m_aSkill[i];
	//	if(nID == rSkill.m_nSkillID)
	//	{
	//		return TRUE;
	//	}
	//}
	//if(MAX_CHAR_SKILL_NUM>rCount)
	//{
	//	SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	//	if(NULL!=pSkillTemplate)
	//	{
	//		XinFaID_t nXinFa = pSkillTemplate->GetXinFa();
	//		if(0<Skill_GetXinFaLevel(nXinFa) || INVALID_ID == nXinFa)
	//		{
	//			m_DB.SetSkillID(nID,rCount);
	//			m_DB.SetSkillCount(rCount+1);
	//			
	//			GCDetailSkillList	Msg;
	//			Msg.setObjID(GetID());
	//			Msg.setSkillList( (WORD)(rList.m_Count), rList.m_aSkill );
	//			GetPlayer()->SendPacket(&Msg);
	//			return TRUE;
	//		}
	//	}
	//}
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 	Obj_Human::Skill_UnregisterSkill(SkillID_t const nID)
{
	__ENTER_FUNCTION
	//const _SKILL_LIST& rList =	Skill_GetSkillList();
	//BYTE rCount				 =	rList.m_Count;
	//for(INT i=0; rCount>i; ++i)
	//{
	//	const _OWN_SKILL& rSkill = rList.m_aSkill[i];
	//	if(nID == rSkill.m_nSkillID)
	//	{
	//		const _OWN_SKILL rLastSkill = rList.m_aSkill[rCount-1];
	//		m_DB.SetSkillID(rLastSkill.m_nSkillID,i);
	//		m_DB.SetSkillID(INVALID_ID,rCount-1);
	//		m_DB.SetSkillCount(rCount-1);

	//		GCDetailSkillList	Msg;
	//		Msg.setObjID(GetID());
	//		Msg.setSkillList( (WORD)(rList.m_Count), rList.m_aSkill );
	//		GetPlayer()->SendPacket(&Msg);
	//		return TRUE;
	//	}
	//}
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 	Obj_Human::Skill_RegisterXinFa(XinFaID_t const nID)
{
	__ENTER_FUNCTION
	const _XINFA_LIST&	rList	=	Skill_GetXinFaList();
	BYTE				rCount	=	rList.m_Count;
	//验证是否是合法的心法ID
	//...
	for(INT i=0; rCount>i; ++i)
	{
		const _OWN_XINFA& rXinFa = rList.m_aXinFa[i];
		if(nID == rXinFa.m_nXinFaID)
		{
			return TRUE;
		}
	}
	if(MAX_CHAR_SKILL_NUM>rCount)
	{
		//_OWN_XINFA& rNewXinFa = rList.m_aXinFa[rCount];
		m_DB.SetXinfaID(nID,rCount);
		m_DB.SetXinfaLevel(1,rCount);
		m_DB.SetXinFaCount(rCount+1);

		GCDetailXinFaList	Msg;
		Msg.setObjID(GetID());
		Msg.setXinFaList((WORD)(rList.m_Count), rList.m_aXinFa);
		GetPlayer()->SendPacket(&Msg);
		return TRUE;
	}
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 	Obj_Human::Skill_UnregisterXinFa(SkillID_t const nID)
{
	__ENTER_FUNCTION
	const _XINFA_LIST& rList =	Skill_GetXinFaList();;
	BYTE	 rCount			 =	rList.m_Count;
	for(INT i=0; rCount>i; ++i)
	{
		const _OWN_XINFA& rXinFa = rList.m_aXinFa[i];
		if(nID == rXinFa.m_nXinFaID)
		{
			const _OWN_XINFA& rLastXinFa = rList.m_aXinFa[rCount-1];
			m_DB.SetXinfaID(rLastXinFa.m_nXinFaID,i);
			m_DB.SetXinfaLevel(rLastXinFa.m_nLevel,i);
			m_DB.SetXinfaID(INVALID_ID,rCount-1);
			m_DB.SetXinfaLevel(0,rCount-1);
			m_DB.SetXinFaCount(rCount-1);

			GCDetailXinFaList	Msg;
			Msg.setObjID(GetID());
			Msg.setXinFaList((WORD)(rList.m_Count), rList.m_aXinFa);
			GetPlayer()->SendPacket(&Msg);
			return TRUE;
		}
	}
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL Obj_Human::Skill_DepleteItem(VOID)
{
	__ENTER_FUNCTION
	SkillInfo_T& rSkillInfo = GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();

	_ITEM_GUID	itemGuid = rParams.GetDepletedItemGuid();
	
	INT nItemIndex = rParams.GetItemIndexOfDepletedItem(); 
	INT nBagIndex = rParams.GetBagIndexOfDepletedItem();
	if(INVALID_ID == nBagIndex)
	{
		if(FALSE==itemGuid.isNull())
		{
			nBagIndex = HumanItemLogic::GetBagItemPosByGUID(this, itemGuid);
		}
		if(INVALID_ID==nBagIndex)
		{
			nBagIndex = HumanItemLogic::GetBagItemPosByType(this, nItemIndex);
		}
	}
	if(INVALID_ID==nBagIndex)
	{
		return FALSE;
	}
	Item* pItem = HumanItemLogic::GetBagItem(this, nBagIndex);
	if(NULL==pItem)
	{
		return FALSE;
	}
	if(pItem->GetItemTableIndex() != rParams.GetItemIndexOfDepletedItem())
	{
		return FALSE;
	}
	if(FALSE == pItem->IsRuler(IRL_CANUSE))
	{
		//AssertEx(FALSE,"[Obj_Human::Skill_DepleteItem]: Item verify failed!!");
		return FALSE;//Item moved or removed
	}
	if(TRUE == pItem->IsRuler(IRL_CONSUME))
	{
		if(pItem->GetLayedNum()<1)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_ITEM);
			return FALSE;
		}
		ItemContainer*	pContainer = HumanItemLogic::GetBagContainer(this,nBagIndex);
		if(NULL==pContainer)
		{
			AssertEx(FALSE,"[Obj_Human::Skill_DepleteItem]: Can't find the specified container!!");
			return FALSE;
		}
		HumanItemLogic::DecBagItemLayCount(this,nBagIndex,1);
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

// 得到可以由玩家控制的法术的数量
INT Obj_Human::Pet_Skill_GetCount_CortrolByPlayer(PET_GUID_t guidPet)
{
	__ENTER_FUNCTION

	//Item* pPetItem = GetPetItem(guidPet);
	//Assert(pPetItem && "Obj_Human::Pet_Skill_GetCount_CortrolByPlayer");
	//if(pPetItem != NULL)
	//{
	//	const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, PET_SKILL_INDEX_CONTROL_BY_PLAYER );
	//	if ( pPetSkill->m_nSkillID != INVALID_ID )
	//		return 1;
	//	else
	//		return 0;
	//}

	return 0;

	__LEAVE_FUNCTION
	return 0;
}

// 得到可以由玩家控制的法术的最大数量
INT Obj_Human::Pet_Skill_GetMaxCount_CortrolByPlayer(PET_GUID_t guidPet)
{
	return 1;
}

// 得到由AI控制的法术的数量
INT Obj_Human::Pet_Skill_GetCount_CortrolByAI(PET_GUID_t guidPet)
{
	__ENTER_FUNCTION

	//Item* pPetItem = GetPetItem(guidPet);
	//Assert(pPetItem && "Obj_Human::Pet_Skill_GetCount_CortrolByAI");
	//if(pPetItem != NULL)
	//{
	//	INT nMaxAISkillCount = Pet_Skill_GetMaxCount_CortrolByAI(guidPet);
	//	INT nCount = 0;
	//	for (INT i = PET_SKILL_INDEX_CONTROL_BY_AI0; i < PET_SKILL_INDEX_CONTROL_BY_AI0 + nMaxAISkillCount; ++i)
	//	{// 被动法攻，被动蕴含都属于被动技能
	//		const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, i );
	//		if ( pPetSkill->m_nSkillID != INVALID_ID )
	//		{
	//			nCount++;
	//		}
	//	}
	//	return nCount;
	//}

	return 0;

	__LEAVE_FUNCTION
	return 0;
}

// 得到由AI控制的法术的最大数量
INT Obj_Human::Pet_Skill_GetMaxCount_CortrolByAI(PET_GUID_t guidPet)
{
	__ENTER_FUNCTION

	//Item* pPetItem = GetPetItem(guidPet);
	//Assert(pPetItem && "Obj_Human::Pet_Skill_GetMaxCount_CortrolByAI");
	//if(pPetItem != NULL)
	//{
	//	PET_ATTR *pPetAttr = g_PetAttrTbl.GetAttr(pPetItem->GetDataID());
	//	if ( pPetAttr != NULL )
	//	{
	//		return pPetAttr->m_PassiveSkillCount;
	//	}
	//}
	return 0;

	__LEAVE_FUNCTION
	return 0;
}

BOOL Obj_Human::Pet_Skill_Have( PET_GUID_t guidPet, SkillID_t const nID ) 
{
__ENTER_FUNCTION

	//if ( nID == INVALID_ID )
	//{
	//	Assert( FALSE && "Obj_Human::Pet_Skill_Have nID == INVALID_ID" );
	//	return TRUE;
	//}

	//Item* pPetItem = GetPetItem(guidPet);
	//Assert(pPetItem && "Obj_Human::Pet_Skill_Have");
	//if(pPetItem == NULL)
	//{
	//	for (INT i = 0; i < PET_MAX_SKILL_COUNT; ++i)
	//	{
	//		if (pPetItem->GetSkill(i).m_nSkillID == nID)
	//		{
	//			return TRUE;
	//		}
	//	}
	//}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

const _PET_SKILL *Obj_Human::Pet_Skill_Get( PET_GUID_t guidPet, INT nIndex ) 
{
__ENTER_FUNCTION

	//Item* pPetItem = GetPetItem(guidPet);
	//Assert(pPetItem && "Obj_Human::Pet_Skill_Get");
	//if(pPetItem != NULL)
	//{
	//	if ( nIndex >= PET_MAX_SKILL_COUNT )
	//	{
	//		Assert( FALSE && "Obj_Human::Pet_Skill_Get  nIndex < PET_MAX_SKILL_COUNT" );
	//		return NULL;
	//	}

	//	return &(pPetItem->GetSkill(nIndex));
	//}
	//else
	//{
	//	return NULL;
	//}

__LEAVE_FUNCTION

	return NULL;
}

BOOL Obj_Human::Pet_Skill_Modify_Study(PET_GUID_t guidPet, SkillID_t nID, INT *pnOutSkillIndex)
{
__ENTER_FUNCTION

	SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	if ( pSkillTemplateData == NULL )
	{
		return FALSE;
	}

	//验证是否是合法的技能ID
	for(INT i=0; PET_MAX_SKILL_COUNT>i; ++i)
	{
		const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, i );
		if(nID == pPetSkill->m_nSkillID)
		{
			return FALSE;
		}
	}

	// 玩家操作还是AI操作
	BOOL bContrlByPlayer = FALSE;
	switch(pSkillTemplateData->GetOperateModeForPetSkill())
	{
	case PET_SKILL_OPERATE_NEEDOWNER:
		bContrlByPlayer = TRUE;
		break;
	case PET_SKILL_OPERATE_AISTRATEGY:
	case PET_SKILL_OPERATE_INCEACEATTR:
	default:
		bContrlByPlayer = FALSE;
		break;
	}
	if ( bContrlByPlayer )
	{
		if(pnOutSkillIndex != NULL)
		{
			*pnOutSkillIndex = PET_SKILL_INDEX_CONTROL_BY_PLAYER;
		}
		return Pet_Skill_Register( guidPet, nID, PET_SKILL_INDEX_CONTROL_BY_PLAYER );
	}
	else
	{
		// 最大的AI控制的技能个数
		INT nMaxAISkillNum = Pet_Skill_GetMaxCount_CortrolByAI(guidPet);
		// AI控制的技能空位
		INT nNonAISkillNum = 0;
		INT nSkillIndex;
		for ( nSkillIndex = PET_SKILL_INDEX_CONTROL_BY_AI0; nSkillIndex < PET_SKILL_INDEX_CONTROL_BY_AI0 + nMaxAISkillNum; nSkillIndex++ )
		{
			const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, nSkillIndex );
			if(INVALID_ID == pPetSkill->m_nSkillID)
			{
				nNonAISkillNum++;
			}
		}

		const _PET_STUDYSKILL *pStudySkill = g_PetStudySkillTbl.Get( nMaxAISkillNum, nNonAISkillNum );
		if ( pStudySkill != NULL )
		{
			INT nPercent = rand() % 100;
			// 打在新格上
			if ( nPercent < pStudySkill->m_nNewPercent )
			{
				Assert(nNonAISkillNum > 0 && "Obj_Human::Pet_Skill_Modify_Study");
				for ( nSkillIndex = PET_SKILL_INDEX_CONTROL_BY_AI0; nSkillIndex < PET_SKILL_INDEX_CONTROL_BY_AI0 + nMaxAISkillNum; nSkillIndex++ )
				{
					const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, nSkillIndex );
					if( pPetSkill->m_nSkillID == INVALID_ID )
					{
						if(pnOutSkillIndex != NULL)
						{
							*pnOutSkillIndex = nSkillIndex;
						}
						return Pet_Skill_Register( guidPet, nID, nSkillIndex );
					}
				}
			}
			// 替换旧的
			else
			{
				INT nAISkillNum = nMaxAISkillNum - nNonAISkillNum;
				Assert(nAISkillNum > 0 && "Obj_Human::Pet_Skill_Modify_Study");
				if(nAISkillNum > 0)
				{
					INT nReplaceIndex = rand()%nAISkillNum;
					INT nReplaceCount = 0;
					for ( nSkillIndex = PET_SKILL_INDEX_CONTROL_BY_AI0; nSkillIndex < PET_SKILL_INDEX_CONTROL_BY_AI0 + nMaxAISkillNum; nSkillIndex++ )
					{
						const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, nSkillIndex );
						if( pPetSkill->m_nSkillID != INVALID_ID )
						{
							if(nReplaceIndex == nReplaceCount)
							{
								if(pnOutSkillIndex != NULL)
								{
									*pnOutSkillIndex = nSkillIndex;
								}
								return Pet_Skill_Register( guidPet, nID, nSkillIndex );
							}
							nReplaceCount++;
						}
					}
				}
			}
		}
	}
	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Human::Pet_Skill_Modify_Realize(PET_GUID_t guidPet, SkillID_t nID)
{
__ENTER_FUNCTION

	SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	if ( pSkillTemplateData == NULL )
	{
		return FALSE;
	}

	//验证是否是合法的技能ID
	for(INT i=0; PET_MAX_SKILL_COUNT>i; ++i)
	{
		const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, i );
		if( pPetSkill->m_nSkillID == nID )
		{
			return FALSE;
		}
	}

	// 玩家操作还是AI操作
	BOOL bContrlByPlayer = pSkillTemplateData->GetOperateModeForPetSkill();
	if ( bContrlByPlayer )
	{
		const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, PET_SKILL_INDEX_CONTROL_BY_PLAYER );
		if( pPetSkill->m_nSkillID == INVALID_ID )
			return Pet_Skill_Register( guidPet, nID, PET_SKILL_INDEX_CONTROL_BY_PLAYER );
	}
	else
	{
		INT nMaxAISkillNum = Pet_Skill_GetMaxCount_CortrolByAI(guidPet);
		INT nSkillIndex;
		for ( nSkillIndex = PET_SKILL_INDEX_CONTROL_BY_AI0; nSkillIndex < PET_SKILL_INDEX_CONTROL_BY_AI0 + nMaxAISkillNum; nSkillIndex++ )
		{
			const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, nSkillIndex );
			if( pPetSkill->m_nSkillID == INVALID_ID )
			{
				return Pet_Skill_Register( guidPet, nID, nSkillIndex );
			}
		}
	}
	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Human::Pet_Skill_Modify_MenPai(PET_GUID_t guidPet, SkillID_t nID)
{
__ENTER_FUNCTION

	SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	if ( pSkillTemplateData == NULL )
	{
		return FALSE;
	}

	const _PET_SKILL *pPetSkill = Pet_Skill_Get( guidPet, PET_SKILL_INDEX_MENPAI );
	if( pPetSkill->m_nSkillID == nID )
	{
		return FALSE;
	}

	return Pet_Skill_Register( guidPet, nID, PET_SKILL_INDEX_MENPAI );

__LEAVE_FUNCTION

	return FALSE;
}
BOOL 	Obj_Human::Pet_Skill_Register(PET_GUID_t guidPet, SkillID_t const nID, INT nIndex)
{
	__ENTER_FUNCTION

	//Item* pPetItem = GetPetItem(guidPet);
	//Assert(pPetItem && "Obj_Human::Pet_Skill_Register");
	//if(pPetItem != NULL)
	//{
	//	SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	//	if ( pSkillTemplateData == NULL )
	//	{
	//		Assert( FALSE && "Obj_Human::Pet_Skill_Register  pSkillTemplateData == NULL" );
	//		return FALSE;
	//	}

	//	if ( nIndex >= PET_MAX_SKILL_COUNT )
	//	{
	//		Assert( FALSE && "Obj_Human::Pet_Skill_Register  nIndex < PET_MAX_SKILL_COUNT" );
	//		return FALSE;
	//	}

	//	_PET_SKILL skill;
	//	skill.m_nSkillID = nID;
	//	g_ItemOperator.SetSkill(GetPetContain(), GetPetIndexByGUID(guidPet), nIndex, skill);
	//}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 	Obj_Human::Pet_Skill_Unregister(PET_GUID_t guidPet, SkillID_t const nID, INT nIndex)
{
	__ENTER_FUNCTION

	//ItemContainer* pPetContainer = GetPetContain();
	//Item* pPetItem = GetPetItem(guidPet);
	//Assert(pPetItem && "Obj_Human::Pet_Skill_Unregister");
	//if(pPetItem != NULL)
	//{
	//	SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	//	if ( pSkillTemplateData != NULL )
	//	{
	//		Assert( FALSE && "Obj_Human::Pet_Skill_Unregister  pSkillTemplateData == NULL" );
	//		return FALSE;
	//	}

	//	if ( nIndex >= PET_MAX_SKILL_COUNT )
	//	{
	//		Assert( FALSE && "Obj_Human::Pet_Skill_Unregister  nIndex < PET_MAX_SKILL_COUNT" );
	//		return FALSE;
	//	}

	//	_PET_SKILL skill;
	//	g_ItemOperator.SetSkill(pPetContainer, GetPetIndexByGUID(guidPet), nIndex, skill);
	//}

	return TRUE;

	__LEAVE_FUNCTION
	return FALSE;
}

BOOL	Obj_Human::Skill_RefixItemAttr(INT nSlotID, INT nItemType, INT nATTRID, SHORT& nAttrValue)
{
	__ENTER_FUNCTION
	_SKILL_LIST const& rList = ((Obj_Human*)this)->Skill_GetSkillList();
	BYTE const& rCount=rList.m_Count;
	SkillInfo_T PassiveSkill;
	for(INT i=0; rCount>i; ++i)
	{
		_OWN_SKILL const& rSkill = rList.m_aSkill[i];
		if(0 < rSkill.m_nSkillID)
		{
			PassiveSkill.Init();
			if(TRUE==g_SkillCore.InstanceSkill(PassiveSkill, *this, rSkill.m_nSkillID))
			{
				SkillLogic_T const* pLogic = Skill_GetLogic(PassiveSkill);
				if(NULL!=pLogic)
				{
					pLogic->Refix_ItemEffect(PassiveSkill, nSlotID, nItemType, nATTRID, nAttrValue);
				}
			}
		}
		
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}
VOID	Obj_Human::Skill_CleanMenPaiSkillAndXinFa(SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog,INT Menpai)
{
	__ENTER_FUNCTION
	if( Menpai<0 || Menpai>=MATTRIBUTE_NUMBER )
	{
		AssertEx( FALSE, "门派取值范围错误！" ) ;
		return ;
	}
	_OWN_SKILL oSkill;
	oSkill.m_eState		= _OWN_SKILL::OWN_SKILL_DISABLE;
	oSkill.m_nLevel		= 0;
	oSkill.m_nSkillID	= INVALID_ID;

	const _XINFA_LIST& rXinFaList = Skill_GetXinFaList();
	const _SKILL_LIST& rSkillList = Skill_GetSkillList();
	//XinFa
	{
		BYTE nCount = rXinFaList.m_Count;
		if(MAX_CHAR_XINFA_NUM < nCount)
		{
			nCount = MAX_CHAR_XINFA_NUM;
		}
		for(INT nIdx=nCount-1;0<=nIdx;--nIdx)
		{
			ID_t nID = rXinFaList.m_aXinFa[nIdx].m_nXinFaID;
			if(0<=nID)
			{
				Combat_Module::Skill_Module::XinFaData_T const* pXinFaData = g_XinFaDataMgr.GetInstanceByID(nID);
				if(NULL!=pXinFaData)
				{
					if(Menpai != pXinFaData->GetMenPai())
					{
						continue;
					}
				}
			}
			if(0<nCount)
			{
				--nCount;
			}
			pXinFaLog->XinFaID = nID;
			pXinFaLog->Level	=	-1;
			pXinFaLog->Count	=	nCount;
			SaveXinFaLog(pXinFaLog);
			m_DB.SetXinfaID(m_DB.GetXinfaID(nCount), nIdx);
			m_DB.SetXinfaLevel(m_DB.GetXinfaLevel(nCount),nIdx);
			m_DB.SetXinfaID(INVALID_ID,nCount);
			m_DB.SetXinfaLevel(0,nCount);
			m_DB.SetXinFaCount(nCount);
		}
		nCount = m_DB.GetXinFaCount();
		for(INT nIdx=nCount;MAX_CHAR_XINFA_NUM>nIdx;++nIdx)
		{
			m_DB.SetXinfaID(INVALID_ID,nIdx);
			m_DB.SetXinfaLevel(0,nIdx);
		}
	}while(0);
	//Skills
	{
		BYTE nCount = rSkillList.m_Count;
		if(MAX_CHAR_SKILL_NUM<nCount)
		{
			nCount = (BYTE)MAX_CHAR_SKILL_NUM;
		}
		for(INT nIdx=nCount-1;0<=nIdx;--nIdx)
		{
			ID_t nID = rSkillList.m_aSkill[nIdx].m_nSkillID;
			if(0<=nID)
			{
				Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
				if(NULL!=pSkillTemplateData)
				{
					if(Menpai != pSkillTemplateData->GetMenPai())
					{
						continue;
					}
				}
			}
			if(0<nCount)
			{
				--nCount;
			}
			pSkillLog->SkillID = nID;
			pSkillLog->Count	= nCount;
			SaveSkillLog(pSkillLog);

			m_DB.SetSkillID(m_DB.GetSkillID(nCount),nIdx);
			m_DB.SetSkillID(oSkill,nCount);
			m_DB.SetSkillCount(nCount);
		}
		nCount = m_DB.GetSkillCount();
		for(INT nIdx=nCount;MAX_CHAR_SKILL_NUM>nIdx;++nIdx)
		{
			m_DB.SetSkillID(oSkill,nIdx);
		}
	}while(0);

	__LEAVE_FUNCTION
}

VOID	Obj_Human::Skill_ADDAllMenPaiSkillAndXinFa(SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog,INT Menpai)
{
	__ENTER_FUNCTION
	if( Menpai<0 || Menpai>=MATTRIBUTE_NUMBER )
	{
		AssertEx( FALSE, "门派取值范围错误！" ) ;
		return ;
	}
	const _XINFA_LIST& rXinFaList = Skill_GetXinFaList();
	const _SKILL_LIST& rSkillList = Skill_GetSkillList();
	//XinFa
	{
		//rXinFaList.m_Count=0;
		m_DB.SetXinFaCount(0);
		pXinFaLog->Count	= 0 ;
		SaveXinFaLog(pXinFaLog);
		BYTE nCount = rXinFaList.m_Count;
		INT nID = 0;
		INT nScaned = 0;
		while(nID<g_XinFaDataMgr.MAXSIZE && nScaned<g_XinFaDataMgr.Size())
		{
			Combat_Module::Skill_Module::XinFaData_T const* pXinFaData = g_XinFaDataMgr.GetInstanceByID(nID);
			if(NULL!=pXinFaData)
			{
				++nScaned;
				if(Menpai == pXinFaData->GetMenPai())
				{
					m_DB.SetXinfaID(nID,nCount);
					m_DB.SetXinfaLevel(1,nCount);
					m_DB.SetXinFaCount(nCount+1);
					
					pXinFaLog->XinFaID = nID;
					pXinFaLog->Level	=	1;
					pXinFaLog->Count	=	nCount+1;
					SaveXinFaLog(pXinFaLog);

				}
				nCount = m_DB.GetXinFaCount();
				if(nCount>=MAX_CHAR_XINFA_NUM)
				{
					break;
				}
			}
			++nID;
		}
		for(INT nIdx=nCount;MAX_CHAR_XINFA_NUM>nIdx;++nIdx)
		{
			m_DB.SetXinfaID(INVALID_ID,nIdx);
			m_DB.SetXinfaLevel(0,nIdx);
		}
	}while(0);
	//Skills
	{
		BYTE nCount = rSkillList.m_Count;
		INT nID = 0;
		INT nScaned = 0;
		_OWN_SKILL oSkill;
		oSkill.m_eState		= _OWN_SKILL::OWN_SKILL_CAN_USE;
		oSkill.m_nLevel		= 1;

		while(nID<g_SkillTemplateDataMgr.MAXSIZE && nScaned<g_SkillTemplateDataMgr.Size())
		{
			Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
			if(NULL!=pSkillTemplateData)
			{
				++nScaned;
				if(0==pSkillTemplateData->GetClassByUser())
				{
					if(Menpai == pSkillTemplateData->GetMenPai())//||INVALID_ID==pSkillTemplateData->GetMenPai())
					{
						oSkill.m_nSkillID	= nID;
						m_DB.SetSkillID(oSkill,nCount);
						Time_t ActiveTime = 0>pSkillTemplateData->GetActiveTime()?-1:0;
						m_DB.SetSkillCount(nCount+1);
						pSkillLog->SkillID = nID;
						pSkillLog->Count	= nCount+1;
						SaveSkillLog(pSkillLog);
					}
					nCount = m_DB.GetSkillCount();
					if(nCount>=MAX_CHAR_SKILL_NUM)
					{
						break;
					}
				}
			}
			++nID;
		}
		oSkill.m_eState		= _OWN_SKILL::OWN_SKILL_DISABLE;
		oSkill.m_nLevel		= 0;
		oSkill.m_nSkillID	= INVALID_ID;
		for(INT nIdx=nCount;MAX_CHAR_SKILL_NUM>nIdx;++nIdx)
		{
			m_DB.SetSkillID(oSkill,nIdx);
		}
	}while(0);

	__LEAVE_FUNCTION
}

//INT Obj_Human::GetWayPointNum() const
//{
//	return m_iWayPointNum;
//}
//
//void Obj_Human::SetWayPointNum( INT val )
//{
//	m_iWayPointNum = val;
//}
//
//WORLD_POS& Obj_Human::GetWayPoint( INT iIndex )
//{
//	Assert( iIndex<MAX_WAY_POINT_NUM && iIndex>=0);
//	return m_aWayPoint[iIndex];
//}
//
//VOID Obj_Human::SetWayPoint( WORLD_POS& val, INT iIndex )
//{
//	Assert( iIndex<MAX_WAY_POINT_NUM && iIndex>=0);
//	m_aWayPoint[iIndex] = val;
//}