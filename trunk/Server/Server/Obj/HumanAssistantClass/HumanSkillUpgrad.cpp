/********************************************************************
	created:	2008/01/11
	created:	11:1:2008   15:45
	filename: 	HumanAssistantClass\HumanSkillUpgrad.cpp
	file path:	HumanAssistantClass
	file base:	HumanSkillUpgrad
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "HumanSkillUpgrad.h"
#include "Obj_Human.h"
#include "DataRecords.h"
#include "GameTable.h"
#include "HumanItemLogic.h"
#include "LogDefine.h"
#include "GCSkillUpgrade.h"
#include "Player.h"
#include "Impact_Core.h"

HumanSkillUpgrade::HumanSkillUpgrade()
{
	Cleanup();
}

HumanSkillUpgrade::~HumanSkillUpgrade()
{

}

VOID HumanSkillUpgrade::Cleanup( VOID )
{
	__ENTER_FUNCTION
	m_pHuman	= NULL;
	m_iSkillID	= INVALID_ID;
	m_iLevel	= INVALID_ID;

	__LEAVE_FUNCTION
}

BOOL HumanSkillUpgrade::HumanSkillLevelUp( const Obj_Human* pHuman, SkillID_t iSkillID, INT iLevel )
{
	__ENTER_FUNCTION;

	InitAttr(pHuman, iSkillID, iLevel);
	if( !CheckCondition() )
	{
		SendFailedSkillLevelupMsg();
		return FALSE;
	}

	DepleteHumanAttr();
	UpgradeLevel();
	SendSuccessSkillLevelupMsg();

	StartPassiveSkill(iSkillID, pHuman);
	return TRUE;

	__LEAVE_FUNCTION
}


BOOL HumanSkillUpgrade::CheckCondition()
{
	__ENTER_FUNCTION

	HumanDB* pDB = m_pHuman->GetDB();
	_OWN_SKILL& oOwnSkill = pDB->GetSkillID( (BYTE)m_iSkillID );

	if( oOwnSkill.m_nLevel >= m_iLevel )
		return FALSE;

	if( CheckSkillStudyCondition() && 
		IsEnoughHumanLevel() &&				// 角色等级条件
		IsOccupationMatch() &&				// 职业条件
		IsSkillClassSpendEnoughPosints() && // 系别点数花费条件
		IsEnoughParentSkillLevel() &&		// 父技能条件
		IsEnoughSkillPoints() &&			// 技能点数条件
		IsEnoughMoney() &&					// 金钱条件
		IsEnoughExp() &&					// 经验条件
		IsHadEnoughItem1() &&				// 物品1条件
		IsHadEnoughItem2() &&				// 物品2条件
		IsHadEnoughItem3()					// 物品3条件
		)
	{
		return TRUE;
	}
	return FALSE;
	__LEAVE_FUNCTION

	return FALSE;
}
VOID HumanSkillUpgrade::DepleteHumanAttr()
{
	__ENTER_FUNCTION
	DepleteSkillPoints();
	DepleteMoney();
	DepleteExp();
	DepleteItem1();
	DepleteItem2();
	DepleteItem3();
	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::UpgradeLevel()
{
	__ENTER_FUNCTION

	m_pHuman->Skill_SetSkillLevel(m_iSkillID, m_iLevel);

	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::SendSuccessSkillLevelupMsg()
{
	GCSkillUpgrade msg;
	msg.SetSkillID( m_iSkillID );
	msg.SetLevel( m_iLevel );
	msg.SetSuccessful( TRUE );

	Player* pPlayer = m_pHuman->GetPlayer();
	if( pPlayer != NULL )
	{
		pPlayer->SendPacket( &msg );
	}
}

VOID HumanSkillUpgrade::SendFailedSkillLevelupMsg()
{
	GCSkillUpgrade msg;
	msg.SetSkillID( m_iSkillID );
	msg.SetLevel( -1 );
	msg.SetSuccessful( FALSE );
	Player* pPlayer = m_pHuman->GetPlayer();
	if( pPlayer != NULL )
	{
		pPlayer->SendPacket( &msg );
	}
}

//玩家等级判断
BOOL HumanSkillUpgrade::IsEnoughHumanLevel()
{
	//Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(m_iSkillID);
	__ENTER_FUNCTION;

	INT iHumanLevel = m_pHuman->GetLevel();

	if( iHumanLevel >= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iPLayerLevelRequire[m_iLevel] )
	{
		return TRUE;
	}

	return FALSE;

	__LEAVE_FUNCTION
	return FALSE;
}


BOOL HumanSkillUpgrade::IsEnoughSkillPoints()
{
	__ENTER_FUNCTION;

	INT iSkillPoints = m_pHuman->Get_RemainSkillPoints();
	if( iSkillPoints >= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendPoints[m_iLevel] )
	{
		return TRUE;
	}
	return FALSE;
	__LEAVE_FUNCTION

	return FALSE;
}

//玩家职业判断
BOOL HumanSkillUpgrade::IsOccupationMatch()
{
	__ENTER_FUNCTION;

	if( g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iOccupationRequire == INVALID_ID )
	{
		return TRUE;
	}
	INT iMenPai = m_pHuman->GetMenPai();
	if( iMenPai == g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iOccupationRequire )
	{
		return TRUE;
	}
	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}

//父技能等级判断
BOOL HumanSkillUpgrade::IsEnoughParentSkillLevel()
{
	__ENTER_FUNCTION;

	INT iParentSkillID = g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iParentSkillID;
	if( iParentSkillID == INVALID_ID )
	{
		return TRUE;
	}
	if (m_pHuman && m_pHuman->Skill_HaveSkill(iParentSkillID))
	{
		const _OWN_SKILL* parentSkill = m_pHuman->Skill_GetSkill(iParentSkillID);
		INT iParentSkillLevelRequire	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iParentSkillLevelRequire[m_iLevel];
		if (parentSkill->m_nLevel >= iParentSkillLevelRequire)
		{
			return TRUE;
		}
	}

	return FALSE;
	
	__LEAVE_FUNCTION
	return FALSE;
}

//金钱判断
BOOL HumanSkillUpgrade::IsEnoughMoney()
{
	__ENTER_FUNCTION;
	INT iMoney			= m_pHuman->GetMoney();
	INT iMoneyRequire	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendMoney[m_iLevel];

	if( iMoney >= iMoneyRequire )
	{
		return TRUE;
	}

	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}

//经验值判断
BOOL HumanSkillUpgrade::IsEnoughExp()
{
	__ENTER_FUNCTION;
	INT iExp = m_pHuman->GetExp();
	INT iExpRequire = g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendExp[m_iLevel];

	if( iExp >= iExpRequire )
	{
		return TRUE;
	}

	return FALSE;
	__LEAVE_FUNCTION
	return TRUE;
}

BOOL HumanSkillUpgrade::CheckSkillStudyCondition()
{
	__ENTER_FUNCTION
	Assert( m_iSkillID <= g_SkillStudyCondition.m_iCount );
	Assert( m_iLevel <= Skill_Study_Condition::MAX_SKILL_LEVEL );
	if( m_iSkillID > g_SkillStudyCondition.m_iCount )
	{
		return FALSE;
	}

	if( m_iLevel > Skill_Study_Condition::MAX_SKILL_LEVEL )
	{
		return FALSE;
	}

	return TRUE;
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL HumanSkillUpgrade::IsSkillClassSpendEnoughPosints()
{
	__ENTER_FUNCTION

	Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(m_iSkillID);
	Assert( pSkillTemplate!=NULL );
	if( pSkillTemplate == NULL )
	{
		return FALSE;
	}
	INT iSkillClass = pSkillTemplate->GetSkillClass();
	
	//如果是无效ID,则认为不需要判断此条件
	if(INVALID_ID == iSkillClass)
	{
		return TRUE;
	}

	HumanDB* pDB = m_pHuman->GetDB();
	
	INT iSkillClassSpendEnoughPosints = pDB->Get_S_SkillPoints( iSkillClass );
	INT iSkillClassSpendEnoughPosintsRequire = g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iPlayerPointsRequire[m_iLevel];
	if( iSkillClassSpendEnoughPosints >= iSkillClassSpendEnoughPosintsRequire )
	{
		return TRUE;
	}
	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}


BOOL HumanSkillUpgrade::IsHadEnoughItem1()
{
	__ENTER_FUNCTION

	INT iItemID			= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem1ID[m_iLevel];
	if(( iItemID == INVALID_ID )||( iItemID == 0 ))
	{
		return TRUE;
	}
	INT iHadItemCount	= HumanItemLogic::CalcBagItemCount( m_pHuman, iItemID );
	INT iRequireItemNum	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem1Num[m_iLevel];
	if( iHadItemCount >= iRequireItemNum )
	{
		return TRUE;
	}
	return FALSE;

	__LEAVE_FUNCTION
	return FALSE;
}

BOOL HumanSkillUpgrade::IsHadEnoughItem2()
{
	__ENTER_FUNCTION

	INT iItemID			= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem2ID[m_iLevel];
	if(( iItemID == INVALID_ID )||( iItemID == 0 ))
	{
		return TRUE;
	}
	INT iHadItemCount	= HumanItemLogic::CalcBagItemCount( m_pHuman, iItemID );
	INT iRequireItemNum	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem2Num[m_iLevel];
	if( iHadItemCount >= iRequireItemNum )
	{
		return TRUE;
	}
	return FALSE;

	__LEAVE_FUNCTION
	return FALSE;
}

BOOL HumanSkillUpgrade::IsHadEnoughItem3()
{
	__ENTER_FUNCTION

	INT iItemID			= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem3ID[m_iLevel];
	if(( iItemID == INVALID_ID )||( iItemID == 0 ))
	{
		return TRUE;
	}
	INT iHadItemCount	= HumanItemLogic::CalcBagItemCount( m_pHuman, iItemID );
	INT iRequireItemNum	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem3Num[m_iLevel];
	if( iHadItemCount >= iRequireItemNum )
	{
		return TRUE;
	}
	return FALSE;

	__LEAVE_FUNCTION
	return FALSE;
}

VOID HumanSkillUpgrade::InitAttr( const Obj_Human* pHuman, SkillID_t iSkillID, INT iLevel )
{
	__ENTER_FUNCTION
	m_pHuman	= const_cast<Obj_Human*>(pHuman);
	m_iSkillID	= iSkillID;
	m_iLevel	= iLevel;
	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::DepleteSkillPoints()
{
	__ENTER_FUNCTION
	INT iSkillPoints	= m_pHuman->Get_RemainSkillPoints();
	INT iSpend			= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendPoints[m_iLevel];

	if( iSpend > iSkillPoints)
		iSpend = iSkillPoints;

	m_pHuman->Set_RemainSkillPoints(iSkillPoints - iSpend);

	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::DepleteMoney()
{
	__ENTER_FUNCTION
	INT iMoney			= m_pHuman->GetMoney();
	INT iMoneyRequire	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendMoney[m_iLevel];

	if( iMoney < iMoneyRequire )
		iMoneyRequire = iMoney;

	m_pHuman->SetMoney( iMoney - iMoneyRequire );

	m_pHuman->SaveMoneyLogByHuman( MONEY_LEARN_SKILL_COST, iMoneyRequire );

	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::DepleteExp()
{
	__ENTER_FUNCTION
	INT iExp = m_pHuman->GetExp();
	INT iExpRequire = g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendExp[m_iLevel];

	if( iExp < iExpRequire )
		iExpRequire = iExp;

	m_pHuman->SetExp( iExp - iExpRequire );
	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::DepleteItem1()
{
	__ENTER_FUNCTION
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType	= ITEM_SKILL_STUDY;

	INT iItemID			= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem1ID[m_iLevel];
	if(( iItemID == INVALID_ID )||( iItemID == 0 ))
	{
		return;
	}
	INT iHadItemCount	= HumanItemLogic::CalcBagItemCount( m_pHuman, iItemID );
	INT iRequireItemNum	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem1Num[m_iLevel];

	HumanItemLogic::EraseBagItem( &ItemLogParam, m_pHuman, iItemID, iHadItemCount-iRequireItemNum );
	SaveItemLog( &ItemLogParam );
	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::DepleteItem2()
{
	__ENTER_FUNCTION
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType	= ITEM_USE;

	INT iItemID			= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem2ID[m_iLevel];
	if(( iItemID == INVALID_ID )||( iItemID == 0 ))
	{
		return;
	}
	INT iHadItemCount	= HumanItemLogic::CalcBagItemCount( m_pHuman, iItemID );
	INT iRequireItemNum	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem2Num[m_iLevel];

	HumanItemLogic::EraseBagItem( &ItemLogParam, m_pHuman, iItemID, iHadItemCount-iRequireItemNum );
	SaveItemLog( &ItemLogParam );

	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::DepleteItem3()
{
	__ENTER_FUNCTION
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType	= ITEM_USE;

	INT iItemID			= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem3ID[m_iLevel];
	if(( iItemID == INVALID_ID )||( iItemID == 0 ))
	{
		return;
	}
	INT iHadItemCount	= HumanItemLogic::CalcBagItemCount( m_pHuman, iItemID );
	INT iRequireItemNum	= g_SkillStudyCondition.m_aSkillStydy_Condition[m_iSkillID].m_iSpendItem3Num[m_iLevel];

	HumanItemLogic::EraseBagItem( &ItemLogParam, m_pHuman, iItemID, iHadItemCount-iRequireItemNum );
	SaveItemLog( &ItemLogParam );

	__LEAVE_FUNCTION
}

VOID HumanSkillUpgrade::FindSkillIndex()
{
	
}

VOID HumanSkillUpgrade::StartPassiveSkill( SkillID_t iSkillID, const Obj_Human* pHuman )
{
	const SkillTemplateData_T* pTemplate = GetSkillTemplateByID( iSkillID );
	if( pTemplate->GetPassiveFlag() <= 0 )
	{
		return;
	}
	const SkillInstanceData_T* pInstance = GetSkillInstanceByID( iSkillID );
	if( pInstance == NULL )
	{
		return;
	}

	Obj_Human* pTemp = const_cast<Obj_Human*>(pHuman);

	for(INT nIdx=0; Skill_Module::ImpactsToTarget_T::IMPACT_NUMBER>nIdx; ++nIdx)
	{
		const Descriptor_T* pDescriptor =  pInstance->GetDescriptorByIndex( nIdx * 2);
		if( pDescriptor == NULL )
		{
			return;
		}
		INT iDataIndex = pDescriptor->GetValueBySkillLevel( m_iLevel-1 );
		if(iDataIndex > 0)
		{
			g_ImpactCore.SendImpactToUnit( *pTemp, iDataIndex, pHuman->GetID(), iSkillID );			
		}
	}
}
