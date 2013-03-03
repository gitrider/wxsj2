/********************************************************************
	created:	2008/03/12
	created:	12:3:2008   14:48
	filename: 	MapServer\Server\Obj\HumanAssistantClass\HorseAttrCalculater.cpp
	file path:	MapServer\Server\Obj\HumanAssistantClass
	file base:	HorseAttrCalculater
	file ext:	cpp
	author:		Richard
	
	purpose:	实现马的所有属性计算相关的内容
*********************************************************************/

#include "stdafx.h"
#include "HorseAttrCalculater.h"
#include "Obj_Human.h"
#include "ItemOperator.h"
#include "GameTable.h"
#include "GCHorseLevelUp.h"
#include "Scene.h"
#include "GCDetailAttrib_Pet.h"
//4种属性每次升级增长的点数
const INT iIncPoint		= 1;
//每次升级玩家可以分配的点数
const INT iRemainPoint	= 4;
//出战宠物每隔固定时间快乐度减少的点数
const INT iHappinessDecreaseAnHour = 4;
//快乐度减少的时间间隔，单位：秒
const INT iHappinessDecreaseTime = 10;
//快乐度下降到此值以下时经验获得为 50%
const INT HappinessMin  = 60;
//快乐度下降到此值以下时经验获得为 80%
const INT HappinessDecExp = 80;

using namespace Packets;

HorseAttrCalculater::HorseAttrCalculater()
{
	m_iHappinessTimer = iHappinessDecreaseTime * 1000;
}

HorseAttrCalculater::~HorseAttrCalculater()
{

}

VOID HorseAttrCalculater::HeartBeat( UINT uTime /*= 0 */ )
{
	if( !IsActive() )
	{
		return;
	}
	HappinessHeartBeat( uTime );
}

VOID HorseAttrCalculater::OnExpChange( INT iExp )
{
	__ENTER_FUNCTION
	
	if( !IsActive() )
	{
		return;
	}

	// 如果已经到达等级上限则直接返回
	if (GetLevel() == PET_LEVEL_NUM)
	{
		return ;
	}

	if( m_pPetDB->m_byHappiness < HappinessDecExp )
		iExp = (INT)(iExp*0.8f);
	else if( m_pPetDB->m_byHappiness < HappinessMin )
		iExp /= 2;
	// 判断是否 >= 该级别升级的经验
	INT nCurExp = GetExp();
	nCurExp += iExp;
	INT nNeedExp = g_PetLevelUpTbl.GetExp(GetLevel());
	BOOL bLevelup = FALSE;
	while (nCurExp > nNeedExp)
	{
		LevelUp();
		nCurExp -= nNeedExp;
		if (GetLevel() == PET_LEVEL_NUM)
		{
			nCurExp = 0;
			break;
		}
		//领悟技能
		SkillRealization();
		bLevelup = TRUE;
	}
	SetExp(nCurExp);
	if( bLevelup )
	{
		m_pOnwer->RefeshHorseDetailAttrib( m_pPetDB->m_GUID );
	}
	return;
	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::SetGenGu( INT iGenGu )
{
	if( !IsActive() )
	{
		return;
	}
	g_ItemOperator.SetGenGu( m_pOnwer->GetPetContain(), m_nIndex, iGenGu );
	UpdatePerception();
}

VOID HorseAttrCalculater::SetSavvy( INT iSavvy )
{
	if( !IsActive() )
	{
		return;
	}
	g_ItemOperator.SetSavvy( m_pOnwer->GetPetContain(), m_nIndex, iSavvy );
	UpdatePerception();
}

VOID HorseAttrCalculater::UpdateAttackLevel1()
{
	__ENTER_FUNCTION

	g_ItemOperator.SetLvl1Attr
		( 
			m_pOnwer->GetPetContain(), 
			m_nIndex, 
			CATTR_LEVEL1_STR, 
			m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_STR) + iIncPoint
		);

	g_ItemOperator.SetLvl1Attr
		( 
			m_pOnwer->GetPetContain(), 
			m_nIndex, 
			CATTR_LEVEL1_CON, 
			m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_CON) + iIncPoint 
		);

	g_ItemOperator.SetLvl1Attr
		( 
			m_pOnwer->GetPetContain(), 
			m_nIndex, 
			CATTR_LEVEL1_INT, 
			m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_INT) + iIncPoint 
		);

	g_ItemOperator.SetLvl1Attr
		( 
			m_pOnwer->GetPetContain(), 
			m_nIndex, 
			CATTR_LEVEL1_DEX, 
			m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_DEX) + iIncPoint
		);

	g_ItemOperator.SetRemainPoint
		( 
			m_pOnwer->GetPetContain(), 
			m_nIndex, 
			m_pPetDB->m_nRemainPoint+iRemainPoint 
		);

	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::SetHappiness( INT iHappiness )
{
	g_ItemOperator.SetHappiness( m_pOnwer->GetPetContain(), m_nIndex, iHappiness );
}

VOID HorseAttrCalculater::UpdateAttackLevel2()
{
	__ENTER_FUNCTION

	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::UpdatePerception()
{
	__ENTER_FUNCTION
	
	INT iTemp;
	iTemp = CalcPerception( m_pPetDB->m_iMindPerception );
	g_ItemOperator.SetMindPer( m_pOnwer->GetPetContain(), m_nIndex, iTemp );
	iTemp = CalcPerception( m_pPetDB->m_iSmartnessPerception );
	g_ItemOperator.SetSmartnessPer( m_pOnwer->GetPetContain(), m_nIndex, iTemp );
	iTemp = CalcPerception( m_pPetDB->m_iStrengthPerception );
	g_ItemOperator.SetStrPer( m_pOnwer->GetPetContain(), m_nIndex, iTemp );
	iTemp = CalcPerception( m_pPetDB->m_iConstitutionPerception );
	g_ItemOperator.SetConstitutionPer( m_pOnwer->GetPetContain(), m_nIndex, iTemp );

	__LEAVE_FUNCTION
}

BOOL HorseAttrCalculater::IsActive()
{
	__ENTER_FUNCTION
	PET_GUID_t guidHorse = m_pOnwer->GetGUIDOfCallUpHorse();
	if( guidHorse.IsNull() )
	{
		return FALSE;
	}
	m_nIndex = m_pOnwer->GetHorseIndexByGUID( guidHorse );

	//TODO: 判断是否达到升级所需的经验值
	m_pPetDB = m_pOnwer->GetHorseDB( m_nIndex );
	if( m_pPetDB == NULL )
	{
		return FALSE;
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID HorseAttrCalculater::LevelUp()
{
	__ENTER_FUNCTION

	//达到升级经验值，可以升级
	g_ItemOperator.SetLevel( m_pOnwer->GetPetContain(), m_nIndex, m_pPetDB->m_nLevel+1 );

	UpdateAttackLevel1();

	SetHappiness( 100 );

	GCHorseLevelUp	msgLevelUp;
	msgLevelUp.SetOnwerObjId(m_pOnwer->GetID());
	msgLevelUp.SetHorseLevel(GetLevel());

	m_pOnwer->getScene()->BroadCast(&msgLevelUp, m_pOnwer, TRUE);
	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::SetExp( INT iExp )
{
	__ENTER_FUNCTION
	g_ItemOperator.SetExp( m_pOnwer->GetPetContain(), m_nIndex, iExp );
	__LEAVE_FUNCTION
}

INT HorseAttrCalculater::GetLevel( VOID )
{
	__ENTER_FUNCTION

	Assert(m_pPetDB);
	return m_pPetDB->m_nLevel;
	__LEAVE_FUNCTION
	return 0;
}

INT HorseAttrCalculater::GetExp()
{
	return m_pPetDB->m_nExp;
}

INT HorseAttrCalculater::CalcPerception( INT iPer )
{
	return (INT)( ( iPer + m_pPetDB->m_nGenGu )*( 1.0f + (FLOAT)(m_pPetDB->m_iSavvy) / 1000.0f ) );
}

VOID HorseAttrCalculater::SkillRealization()
{
	__ENTER_FUNCTION
	if( m_pPetDB->m_nLevel > 60 )
		return;

	INT iSkillSlotIndex = m_pPetDB->m_nLevel % 10;
	//在整10级的时候才能领悟技能
	if( iSkillSlotIndex != 0 )
		return;

	//查找空余技能槽
	INT iSlot=0;
	//判断技能槽是否为领悟技能槽（前6个为领悟技能槽）
	for( ; iSlot<6; ++iSlot )
	{
		if( m_pPetDB->m_SkillList[iSlot].m_nSkillID == INVALID_ID )
		{
			break;
		}
	}
	if( iSlot >= 6 )
		return;

	//判断技能槽是否为领悟技能槽（前6个为领悟技能槽）
	INT iSkillCount = 0;
	for( INT i=0; i<6; ++i )
	{
		if( m_pPetDB->m_SkillList[i].m_nSkillID != INVALID_ID )
		{
			++iSkillCount;
		}
	}
	//获得技能领悟ID
	INT iRealID = g_HorseAttrTbl.m_aHorseTbl[m_pPetDB->m_nDataID].m_iRealizationID;
	if( iSkillCount >= g_HorseSkillTbl.m_aHorseSkillTbl[iRealID].m_iSkillSlotCount )
	{
		return;
	}
	//取得当前找到的技能槽的领悟概率
	INT iRealRate = g_HorseSkillTbl.m_aSkillRealRateForSlot[iSlot];
	//技能槽的领悟概率导致领悟失败
	INT iRand = m_pOnwer->getScene()->GetRand100();
	if( iRand > iRealRate )
	{
		return;
	}
	 
	//当前技能槽可以领悟的技能个数
	INT iRandCount = g_HorseSkillTbl.m_aHorseSkillTbl[iRealID].m_oSkillSlot[iSlot].m_iSkillCount;
	if( iRandCount <= 0 )
	{
		return;
	}
	INT iSkillIndexInSlot = iRand % iRandCount; 
	if( iSkillIndexInSlot >= iRandCount )
	{
		iSkillIndexInSlot -= 1;
	}
	INT iRealizationSkillID = 
		g_HorseSkillTbl.m_aHorseSkillTbl[iRealID].m_oSkillSlot[iSlot].m_aSkillSlot[iSkillIndexInSlot];
	Assert( iRealizationSkillID != INVALID_ID );
	_PET_SKILL oPetSkill;
	oPetSkill.m_nSkillID = iRealizationSkillID;
	oPetSkill.m_bCanUse	 = TRUE;
	g_ItemOperator.SetSkill( m_pOnwer->GetPetContain(), m_nIndex, iSlot, oPetSkill );

	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::RefreshAttr()
{
	__ENTER_FUNCTION
		INT nAttr = 0;

	BOOL bDetailAttrModified		= FALSE;

	GCDetailAttrib_Pet msgDetail;

	msgDetail.SetObjID( -1 );
	msgDetail.SetGUID( m_pPetDB->m_GUID );

	if ( m_AttrBackUp.m_bNameModified )
	{
		bDetailAttrModified	= TRUE;
		msgDetail.SetName( m_pPetDB->m_szName );
		m_AttrBackUp.m_bNameModified = FALSE;
	}

	if(m_AttrBackUp.m_nDataID != m_pPetDB->m_nDataID)
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_nDataID = m_pPetDB->m_nDataID;
		msgDetail.SetDataID( m_pPetDB->m_nDataID );
		_HORSE_ATTR* pPetAttr = g_HorseAttrTbl.GetAttr(m_AttrBackUp.m_nDataID);
		if (!pPetAttr)
			return;

		msgDetail.SetModelID(pPetAttr->m_iBehaveID);
		msgDetail.SetMountID(pPetAttr->m_iMountID);
	}

	if(m_AttrBackUp.m_Level != m_pPetDB->m_nLevel)
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_Level = m_pPetDB->m_nLevel;
		msgDetail.SetLevel( m_pPetDB->m_nLevel );
	}

	if( m_AttrBackUp.m_GUID != m_pPetDB->m_GUID )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_GUID= m_pPetDB->m_GUID;
		msgDetail.SetGUID( m_AttrBackUp.m_GUID );
	}

	if( m_AttrBackUp.m_SpouseGUID != m_pPetDB->m_SpouseGUID )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_SpouseGUID = m_pPetDB->m_SpouseGUID;
		msgDetail.SetSpouseGUID( m_AttrBackUp.m_SpouseGUID );
	}

	if( m_AttrBackUp.m_byGeneration != m_pPetDB->m_byGeneration )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_byGeneration= m_pPetDB->m_byGeneration;
		msgDetail.SetGeneration( m_pPetDB->m_byGeneration );
	}

	if( m_AttrBackUp.m_byHappiness != m_pPetDB->m_byHappiness )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_byHappiness= m_pPetDB->m_byHappiness;
		msgDetail.SetHappiness( m_AttrBackUp.m_byHappiness );
	}

	if( m_AttrBackUp.m_nExp != GetExp() )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_nExp= m_pPetDB->m_nExp;
		msgDetail.SetExp( m_AttrBackUp.m_nExp );
	}

	if( m_AttrBackUp.m_iStrengthPerception != m_pPetDB->m_iStrengthPerception )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iStrengthPerception= m_pPetDB->m_iStrengthPerception;
		msgDetail.SetStrPerception( m_AttrBackUp.m_iStrengthPerception );
	}

	if( m_AttrBackUp.m_iSmartnessPerception != m_pPetDB->m_iSmartnessPerception )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iSmartnessPerception= m_pPetDB->m_iSmartnessPerception;
		msgDetail.SetConPerception( m_AttrBackUp.m_iSmartnessPerception );
	}

	if( m_AttrBackUp.m_iMindPerception != m_pPetDB->m_iMindPerception )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iMindPerception= m_pPetDB->m_iMindPerception;
		msgDetail.SetDexPerception( m_pPetDB->m_iMindPerception );
	}

	if( m_AttrBackUp.m_iConstitutionPerception != m_pPetDB->m_iConstitutionPerception )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iConstitutionPerception= m_pPetDB->m_iConstitutionPerception;
		msgDetail.SetIntPerception( m_AttrBackUp.m_iConstitutionPerception );
	}

	if( m_AttrBackUp.m_iStrength != m_pPetDB->m_BaseAttrLevel1.Get(0) )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iStrength= m_pPetDB->m_BaseAttrLevel1.Get(0);
		msgDetail.SetStr( m_AttrBackUp.m_iStrength );
	}

	if( m_AttrBackUp.m_iSmartness != m_pPetDB->m_BaseAttrLevel1.Get(1) )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iSmartness= m_pPetDB->m_BaseAttrLevel1.Get(1);
		msgDetail.SetCon( m_AttrBackUp.m_iSmartness );
	}

	if( m_AttrBackUp.m_iMind != m_pPetDB->m_BaseAttrLevel1.Get(2) )
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iMind= m_pPetDB->m_BaseAttrLevel1.Get(2);
		msgDetail.SetDex( m_AttrBackUp.m_iMind );
	}

	if( m_AttrBackUp.m_iConstitution != m_pPetDB->m_BaseAttrLevel1.Get(3))
	{
		bDetailAttrModified	= TRUE;
		m_AttrBackUp.m_iConstitution= m_pPetDB->m_BaseAttrLevel1.Get(3);
		msgDetail.SetInt( m_AttrBackUp.m_iConstitution );
	}

	if( m_AttrBackUp.m_GenGu != m_pPetDB->m_nGenGu)
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_GenGu= m_pPetDB->m_nGenGu;
		msgDetail.SetGenGu( m_AttrBackUp.m_GenGu );
	}

	if( m_AttrBackUp.m_nRemainPoint != m_pPetDB->m_nRemainPoint )
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nRemainPoint= m_pPetDB->m_nRemainPoint;
		msgDetail.SetRemainPoint( m_AttrBackUp.m_nRemainPoint );
	}

	if ( bDetailAttrModified )
	{
		if (m_pOnwer->GetObjType() == Obj::OBJ_TYPE_HUMAN )
			m_pOnwer->GetPlayer()->SendPacket( &msgDetail );
	}

	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::InitBackupAttr()
{
	__ENTER_FUNCTION
	m_AttrBackUp.m_bNameModified		= FALSE;
	m_AttrBackUp.m_bTitleModified		= FALSE;
	m_AttrBackUp.m_SpouseGUID			= m_pPetDB->m_SpouseGUID;
	m_AttrBackUp.m_nAIType				= m_pPetDB->m_AIType;
	m_AttrBackUp.m_Level				= m_pPetDB->m_nLevel;

	m_AttrBackUp.m_CampData				= m_pPetDB->m_CampData;
	m_AttrBackUp.m_nOwnerID				= m_pOnwer->GetID();

	m_AttrBackUp.m_GUID					= m_pPetDB->m_GUID;

	m_AttrBackUp.m_byGeneration			= m_pPetDB->m_byGeneration;
	m_AttrBackUp.m_byHappiness			= m_pPetDB->m_byHappiness;

	m_AttrBackUp.m_nExp					= m_pPetDB->m_nExp;

	m_AttrBackUp.m_nDataID				= m_pPetDB->m_nDataID;

	m_AttrBackUp.m_iStrengthPerception		= m_pPetDB->m_iStrengthPerception;
	m_AttrBackUp.m_iSmartnessPerception		= m_pPetDB->m_iSmartnessPerception;
	m_AttrBackUp.m_iMindPerception			= m_pPetDB->m_iMindPerception;
	m_AttrBackUp.m_iConstitutionPerception	= m_pPetDB->m_iConstitutionPerception;

	m_AttrBackUp.m_iStrength				= m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_STR);
	m_AttrBackUp.m_iConstitution			= m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_CON);
	m_AttrBackUp.m_iMind					= m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_INT);
	m_AttrBackUp.m_iSmartness				= m_pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_DEX);

	m_AttrBackUp.m_nRemainPoint				= m_pPetDB->m_nRemainPoint;
	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::ResetHorse()
{
	__ENTER_FUNCTION
	InitBackupAttr();
	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::HappinessHeartBeat( UINT uTime )
{
	__ENTER_FUNCTION
	//计算经历的时间
	if( m_iHappinessTimer > uTime )
	{
		return;
	}
	INT iHappiness = m_pPetDB->m_byHappiness - iHappinessDecreaseAnHour;
	if( iHappiness < 0 )
	{
		iHappiness = 0;
	}
	SetHappiness( iHappiness );
	InitCurHappinessTime();
	RefreshAttr();
	__LEAVE_FUNCTION
}

VOID HorseAttrCalculater::InitCurHappinessTime()
{
	m_iHappinessTimer = iHappinessDecreaseTime * 1000 + g_pTimeManager->CurrentTime();
}

_HORSE_ATTR_BACKUP::_HORSE_ATTR_BACKUP( VOID )
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}

VOID _HORSE_ATTR_BACKUP::CleanUp( VOID )
{
	__ENTER_FUNCTION

	m_bNameModified				= FALSE;
	m_bTitleModified			= FALSE;
	m_CampData.CleanUp();
	m_nOwnerID					= INVALID_ID;
	m_SpouseGUID.Reset();
	m_nAIType					= -1;
	m_Level						= -1;

	m_nDataID					= INVALID_ID;
	m_nModelID					= INVALID_ID;
	m_nMountID					= INVALID_ID;

	m_GUID.Reset();
	m_byGeneration				= 0;
	m_byHappiness				= 0;

	m_nAtt_Near					= -1;
	m_nAtt_Far					= -1;

	m_nAtt_MagicNear			= -1;
	m_nAtt_MagicFar 			= -1;

	m_nDef_Near					= -1;
	m_nDef_Far					= -1;

	m_nDef_MagicNear			= -1;
	m_nDef_MagicFar				= -1;

	m_nHit						= -1;
	m_nMiss						= -1;

	m_nExp						= -1;

	m_iStrengthPerception		= -1;
	m_iSmartnessPerception		= -1;
	m_iMindPerception			= -1;
	m_iConstitutionPerception	= -1;
	m_GenGu						= -1;
	m_iSavvy					= 0;
	m_iPullulationRate			= 0;

	m_iStrength					= 0;	
	m_iSmartness				= 0;	
	m_iMind						= 0;		
	m_iConstitution				= 0;

	m_nRemainPoint				= -1;
	__LEAVE_FUNCTION
}








