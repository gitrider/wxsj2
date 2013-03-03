///////////////////////////////////////////////////////////////////////////////
// 文件名：DataRecords.cpp
// 程序员：高骐
// 功能说明：技能和效果的表记录结构和具体的功能实现
//
// 修改记录：
//祝方亮 根据表格变化修改相应的加载过程
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file DataRecords.cpp
\brief 各种技能相关
*/

#include "stdafx.h"
#include "DataRecords.h"
#include "GameUtil.h"
#include "WXSJ_DBC.h"
#include "SplitStringLevelOne.h"

using namespace DBC;

//class Descriptor_T;
BOOL Descriptor_T::InitFromDB(DBCFile* pDB, INT const nRow, INT& nColumn, BOOL bNeedSplit)
{
	__ENTER_FUNCTION
	enum
	{
		DPT_SkillMaxLevel = 11
	};
	Assert(pDB);
	Assert(0<=nRow);
	m_szDesc = pDB->Search_Posistion(nRow, nColumn++)->pString;
	//m_nValue = pDB->Search_Posistion(nRow, nColumn++)->iValue;

	SplitStringLevelOne oSplitL1;

	INT iSkillMaxLevel = 1;
	if( bNeedSplit )
	{
		iSkillMaxLevel	= pDB->Search_Posistion(nRow, DPT_SkillMaxLevel)->iValue;
	}
	const CHAR* pSplitString = NULL;

	pSplitString = pDB->Search_Posistion(nRow, nColumn++)->pString;

	oSplitL1.Reset();
	oSplitL1.Init( '|', NULL );
	oSplitL1.DoSplit( pSplitString );
	INT iLineCount	= oSplitL1.GetResultLineCount();
	Assert( iSkillMaxLevel>=iLineCount && MAX_SKIL_LEVEL_LIMIT>=iSkillMaxLevel );
	if( iSkillMaxLevel<iLineCount || MAX_SKIL_LEVEL_LIMIT<iSkillMaxLevel )
	{
		return FALSE;
	}
	for( INT i=0; i<iLineCount; ++i )
	{
		m_nValue[i] = atoi(oSplitL1.GetResultLine(i));
	}

	if( m_nValue[0] == INVALID_ID )
	{
		BOOL bSetInvalidID = TRUE;

		//针对着(-1|-1|-1|-1|2564|)满级后有特效的情况
		for( INT i=1; i<MAX_SKIL_LEVEL_LIMIT; ++i )
		{
			if(m_nValue[i] != INVALID_ID && m_nValue[i] != 0)
			{
				bSetInvalidID = FALSE;
				break;
			}
		}		

		if(bSetInvalidID)
		{
			for( INT i=1; i<MAX_SKIL_LEVEL_LIMIT; ++i )
			{
				m_nValue[i] = INVALID_ID;
			}		
		}
	}

	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}
BOOL ConDepTerm_T::InitFromDB(DBCFile* pDB, INT const nRow, INT& nColumn)
{
	__ENTER_FUNCTION
		Assert(pDB);
	Assert(0<=nRow);
	m_szDesc = pDB->Search_Posistion(nRow, nColumn++)->pString;
	m_nType = pDB->Search_Posistion(nRow, nColumn++)->iValue;
	m_nParam0 = pDB->Search_Posistion(nRow, nColumn++)->iValue;
	m_nParam1 = pDB->Search_Posistion(nRow, nColumn++)->iValue;
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}		

namespace Combat_Module
{
	//IDCollection_T
	BOOL IDCollection_T::InitFromDB(DBCFile* pDB, INT const nRow)
	{
		__ENTER_FUNCTION
			Assert(pDB);
		Assert(0<=nRow);
		Assert(MAX_ID > nRow);
		INT column = 0;
		m_nID = pDB->Search_Posistion(nRow, column++)->iValue;
		m_nType = pDB->Search_Posistion(nRow, column++)->iValue;
		m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;
		m_nSize = pDB->Search_Posistion(nRow, column++)->iValue;
		m_nSize>COLLECTION_SIZE?m_nSize=COLLECTION_SIZE:NULL;
		for(INT nIndex=0;COLLECTION_SIZE>nIndex;++nIndex)
		{
			m_aID[nIndex]= pDB->Search_Posistion(nRow, column++)->iValue;
		}
		m_szEndDesc = pDB->Search_Posistion(nRow, column++)->pString;
		m_bInited = TRUE;
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
	}
	INT IDCollection_T::GetIDByIndex(INT nIndex) const
	{
		if(0>nIndex||m_nSize<=nIndex)
		{
			return INVALID_ID;
		}
		return m_aID[nIndex];
	}
	BOOL IDCollection_T::IsThisIDInCollection(ID_t nId) const
	{
		if(FALSE==m_bInited)
		{
			return FALSE;
		}
		for(INT nIndex=0;m_nSize>nIndex;++nIndex)
		{
			if(nId==m_aID[nIndex])
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	namespace Skill_Module
	{
		//class XinFaData_T
		BOOL XinFaData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
				Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 1;
			m_szName = pDB->Search_Posistion(nRow, column++)->pString;
			m_nMenPai = pDB->Search_Posistion(nRow, column++)->iValue;
			m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;
			m_szIcon = pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}
		//SkillTemplate
		//class SkillTemplateData_T
		SkillTemplateData_T::SkillTemplateData_T():
		m_bInited(FALSE), 
			m_nSkillID(INVALID_ID),
			m_nMenPai(INVALID_ID),
			m_szName(NULL),
			m_nXinFaLevelRequirement(0),
			m_nClientOnly1(0),
			m_bMustUseWeapon(FALSE),
			m_nDisableByFlag1(0),
			m_nDisableByFlag2(0),
			m_nDisableByFlag3(0),
			m_nXinFa(INVALID_ID),
			m_nXinFaParam(0),
			m_nRangedSkillFlag(-1),
			m_bForceBreakPreSkill(FALSE),
			m_nSkillType(INVALID_ID),
			m_nCooldownID(INVALID_ID),
			m_nTargetMustInSpecialState(-1),
			m_nClassByUser(INVALID_ID),
			m_nPassiveFlag(INVALID_ID),
			m_nSelectType(INVALID_ID),
			m_nOperateModeForPetSkill(INVALID_ID),
			m_nPetRateOfSkill(INVALID_ID),
			m_nTypeOfPetSkill(INVALID_ID),
			m_nImpactIDOfSkill(INVALID_ID),
			m_nSpeedOfBullet(INVALID_ID),
			m_nPlayActionTime(0),
			m_fOptimalRangeMin(0.0f),
			m_fOptimalRangeMax(0.0f),
			m_nStandFlag(-1),
			m_nTargetLogicByStand(INVALID_ID),
			m_nTargetCheckByObjType(INVALID_ID),
			m_bPartyOnly(FALSE),
			m_nChargesOrInterval(0),
			m_bAutoShot(FALSE),
			m_bUseNormalAttackRate(FALSE),
			m_nActiveTime(-1),
			m_fAngle(0.0f),
			m_bCanInterruptAutoShot(FALSE),
			m_nDelayTime(0),
			m_szDescription(NULL),
			m_bUseBallSkill(FALSE),
			m_iSkillClass(INVALID_ID),
			m_iSkillMaxLevel(INVALID_ID),
			m_bCanInterruptChannel(FALSE),
			m_iChannelBackRate(0),
			m_iChannelBackHappanRate(0)
		{
			for(INT i=0;MAX_XINFA_LEVEL>i;++i)
			{
				m_aSkillInstance[i]			=-1;
			}
			for( INT i=0; i<MAX_SUB_SKILL; ++i )
			{
				m_aSubSkill[i]				= -1;
				m_aSubSkillSuccessRate[i]	= 0;
			}
		}

		BOOL SkillTemplateData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
				Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			enum SkillTemplateDataIndex
			{
				STDI_SkillID		= 0,				//ID，（技能系的ID）
				STDI_MenPai			= 2,				//门派（职业）
				STDI_SkillName,							//技能名字
				STDI_SkillClass		= 10,				//技能系别（前提点数之和需求）
				STDI_SkillMaxLevel,						//等级上限
				STDI_MustUseWeapon	= 28,				//需要拿武器
				STDI_DisableByFlag1,					//受限于控制1
				STDI_DisableByFlag2,					//受限于控制2
				STDI_DisableByFlag3,					//受限于骑乘/变身
				STDI_RangedSkillFlag,					//是否是远程技能
				STDI_ForceBreakPreSkill,				//是否打断上一个正在执行的技能
				STDI_SkillType,							//技能类型
				STDI_CooldownID,						//冷却ID
				STDI_TargetMustInSpecialState = 39,		//所选的目标必须是活的还是死的
				STDI_ClassByUser,						//技能类型按使用者划分
				STDI_PassiveFlag,						//主动技能还是被动技能
				STDI_SelectType,						//鼠标点选操作类型
				DTDI_SelectEffect,					//选择环,不读取
				STDI_OperateModeForPetSkill,			//宠物技能发动类型 0:主人手动点选,1:AI自动执行,2:增强自身属性的被动技能
				STDI_PetRateOfSkill,					//技能发动几率,只对宠物技能有效
				STDI_TypeOfPetSkill,					//宠物技能类型,0:物功,1:法功,2:护主,3:防御,4:复仇;
				STDI_ImpactIDOfSkill,					//宠物技能产生的效果ID
				STDI_SpeedOfBullet=STDI_ImpactIDOfSkill+5,//子弹飞行速度
				STDI_TargetingLogic = 56,				//目标选择逻辑
				STDI_PlayActionTime,					//动作时间(ms)
				STDI_OptimalRangeMin,					//最小射程(m)
				STDI_OptimalRangeMax,					//最大射程(m)
				STDI_StandFlag,							//技能友好度，=0为中性技能，>0为正面技能，<0为 负面技能
				STDI_TargetLogicByStand,				//目标和使用者阵营关系
				STDI_TargetCheckByObjType,				//目标类型
				STDI_PartyOnly,							//是否仅对队友有效。注：队友宠物算作队友。1为只对队友有效，0为无此限制。
				STDI_ChargesOrInterval,					//连续攻击次数或引导的时间间隔(ms)
				STDI_AutoShot,							//是否自动连续释放
				STDI_UseNormalAttackRate = 68,			//角色的攻击速度是否影响技能的冷却时间。
				STDI_ActiveTime,						//激活时间
				STDI_Angle = 71,						//角度参量
				STDI_CanInterruptAutoShot = 74,			//本技能是否打断自动释放技能
				STDI_DelayTime,							//延迟时间（暂时不用）
				STDI_HurtDelayTime,						//伤害计算的通用延迟时间（ms）
				STDI_InterruptMove,						//是否瞬发技能
				STDI_BeatBackDistance,					//击退距离
				STDI_BeatBackRate,						//击退概率
				STDI_BeatFlyDistance,					//击飞距离
				STDI_BeatFlyRate,						//击飞概率
				STDI_UseBallSkill,						//是否是带有球特效的技能
				STDI_SubSkill = 109,					//子技能列表
				STDI_SubSkillSuccessRate,				//子技能成功率列表，和子技能列表在索引上有一一对应的关系
				STDI_IsRunSkill,						//是否为冲锋技能
				STDI_CanInterruptChannel,				//是否可以使聚气和引导回退
				STDI_ChannelBackRate,					//回退的百分比
				STDI_ChannelBackHappanRate				//产生回退的概率
			};
			// Read Skill ID
			m_nSkillID	= pDB->Search_Posistion(nRow, STDI_SkillID)->iValue;
			// 跳过策划管理用的ID
			// 读取门派ID
			m_nMenPai	= pDB->Search_Posistion(nRow, STDI_MenPai)->iValue;
			// Read Skill Name
			m_szName = pDB->Search_Posistion(nRow, STDI_SkillName)->pString;
			// Read Must use weapon flag
			m_bMustUseWeapon = pDB->Search_Posistion(nRow, STDI_MustUseWeapon)->iValue;
			// Read control by flags
			m_nDisableByFlag1 = pDB->Search_Posistion(nRow, STDI_DisableByFlag1)->iValue;
			m_nDisableByFlag2 = pDB->Search_Posistion(nRow, STDI_DisableByFlag2)->iValue;
			m_nDisableByFlag3 = pDB->Search_Posistion(nRow, STDI_DisableByFlag3)->iValue;
			// Read the based XinFa ID
			//m_nXinFa = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the XinFa refix param
			//m_nXinFaParam = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the Flag of Is-ranged-skill
			m_nRangedSkillFlag= pDB->Search_Posistion(nRow, STDI_RangedSkillFlag)->iValue;
			// Read the Flag of Force-Break-Pre-Skill
			m_bForceBreakPreSkill= ((pDB->Search_Posistion(nRow, STDI_ForceBreakPreSkill)->iValue==0)?FALSE:TRUE);
			// Read the skill type id
			m_nSkillType = pDB->Search_Posistion(nRow, STDI_SkillType)->iValue;
			// Read the cooldown ID
			m_nCooldownID = pDB->Search_Posistion(nRow, STDI_CooldownID)->iValue;
			// Read Flag: this skill can use on dead flag or alive flag
			m_nTargetMustInSpecialState = pDB->Search_Posistion(nRow, STDI_TargetMustInSpecialState)->iValue;
			// Read Flag: is a pet skill or human skill or item skill or common skil or ...
			m_nClassByUser = pDB->Search_Posistion(nRow, STDI_ClassByUser)->iValue;
			// Read Flag: is a passive skill
			m_nPassiveFlag = pDB->Search_Posistion(nRow, STDI_PassiveFlag)->iValue;
			// Read Flag: mouse operation type
			m_nSelectType = pDB->Search_Posistion(nRow, STDI_SelectType)->iValue;
			// Read the operat-mode for pet skill;
			m_nOperateModeForPetSkill = pDB->Search_Posistion(nRow, STDI_OperateModeForPetSkill)->iValue;
			// Read the rate of pet skill;
			m_nPetRateOfSkill = pDB->Search_Posistion(nRow, STDI_PetRateOfSkill)->iValue;
			// Read the type of pet skill;
			m_nTypeOfPetSkill = pDB->Search_Posistion(nRow, STDI_TypeOfPetSkill)->iValue;
			// Read the ImpactID of pet skill;
			m_nImpactIDOfSkill = pDB->Search_Posistion(nRow, STDI_ImpactIDOfSkill)->iValue;
			// Read the speed of bullet
			m_nSpeedOfBullet   = pDB->Search_Posistion(nRow, STDI_SpeedOfBullet)->iValue;
			// Read the Targeting logic id
			m_nTargetingLogic = pDB->Search_Posistion(nRow, STDI_TargetingLogic)->iValue;
			// Read the action play time
			m_nPlayActionTime = pDB->Search_Posistion(nRow, STDI_PlayActionTime)->iValue;
			// Read the Range info
			m_fOptimalRangeMin = pDB->Search_Posistion(nRow, STDI_OptimalRangeMin)->fValue;
			m_fOptimalRangeMax = pDB->Search_Posistion(nRow, STDI_OptimalRangeMax)->fValue;
			// Read the Camp and stand info
			m_nStandFlag = pDB->Search_Posistion(nRow, STDI_StandFlag)->iValue;
			// Read the target Check logic id
			m_nTargetLogicByStand = pDB->Search_Posistion(nRow, STDI_TargetLogicByStand)->iValue;
			// Read the target Check by OBJ_TYPE
			m_nTargetCheckByObjType = pDB->Search_Posistion(nRow, STDI_TargetCheckByObjType)->iValue;
			// Read Is this skill only effect in party
			m_bPartyOnly = ((pDB->Search_Posistion(nRow, STDI_PartyOnly)->iValue)==0?FALSE:TRUE);
			// read the Charges of a multi-hits Skill or the interval of a channel skill
			m_nChargesOrInterval= pDB->Search_Posistion(nRow, STDI_ChargesOrInterval)->iValue;
			// read the auto shot flag
			m_bAutoShot = ((pDB->Search_Posistion(nRow, STDI_AutoShot)->iValue==0)?FALSE:TRUE);
			// read the flag: is this skill controlled by attack speed 
			m_bUseNormalAttackRate = ((pDB->Search_Posistion(nRow, STDI_UseNormalAttackRate)->iValue==0)?FALSE:TRUE);
			// read the active time
			m_nActiveTime = pDB->Search_Posistion(nRow, STDI_ActiveTime)->iValue;
			// read effective range Angle
			m_fAngle = pDB->Search_Posistion(nRow, STDI_Angle)->fValue;
			m_fAngle = 2*__PI*m_fAngle/360; //角度转弧度
			// Read Can interrupt auto shot flag
			m_bCanInterruptAutoShot= (TRUE==(pDB->Search_Posistion(nRow, STDI_CanInterruptAutoShot)->iValue));
			// Read Delay Time
			m_nDelayTime = pDB->Search_Posistion(nRow, STDI_DelayTime)->iValue;
			0>m_nDelayTime?m_nDelayTime=0:NULL;
			//// Read Skill instance ids
			//for(INT i=0;MAX_XINFA_LEVEL>i;++i)
			//{
			//	m_aSkillInstance[i] = pDB->Search_Posistion(nRow, column++)->iValue;
			//}
			//// Read skill description
			//m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;

			//伤害通用延迟时间,当玩家使用技能有效时,先向client广播技能使用成功消息,然后经过此延迟
			//时间后再向client广播伤害列表,以及在server端产生伤害效果
			m_nHurtDelayTime	= pDB->Search_Posistion(nRow, STDI_HurtDelayTime)->iValue;
			//是否瞬发技能,在玩家使用技能时,是否中断玩家的走、跑、跳等移动状态
			m_bInterruptMove	= pDB->Search_Posistion(nRow, STDI_InterruptMove)->iValue;
			//击退距离，每个技能会有自己的击退距离
			m_fBeatBackDistance = (FLOAT)pDB->Search_Posistion(nRow, STDI_BeatBackDistance)->iValue;
			//击退概率，每个人都按照概率计算
			m_fBeatBackRate		= (FLOAT)pDB->Search_Posistion(nRow, STDI_BeatBackRate)->iValue;
			//击飞距离
			m_fBeatFlyDistance	= (FLOAT)pDB->Search_Posistion(nRow, STDI_BeatFlyDistance)->iValue;
			//击飞概率，每个人都按照概率计算
			m_fBeatFlyRate		= (FLOAT)pDB->Search_Posistion(nRow, STDI_BeatFlyRate)->iValue;
			//是否为先知类技能
			m_bUseBallSkill		= pDB->Search_Posistion(nRow, STDI_UseBallSkill)->iValue;
			m_iSkillClass		= pDB->Search_Posistion(nRow, STDI_SkillClass)->iValue;
			m_iSkillMaxLevel	= pDB->Search_Posistion(nRow, STDI_SkillMaxLevel)->iValue;

			SplitStringLevelOne oSplitL1;
			const CHAR* pSplitString = NULL;

			pSplitString = pDB->Search_Posistion(nRow, STDI_SubSkill)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, m_aSubSkill);
			
			pSplitString = pDB->Search_Posistion(nRow, STDI_SubSkillSuccessRate)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, m_aSubSkillSuccessRate);

			m_iIsRunSkill	= pDB->Search_Posistion(nRow, STDI_IsRunSkill)->iValue;

			m_bCanInterruptChannel = pDB->Search_Posistion(nRow, STDI_CanInterruptChannel)->iValue;
			m_iChannelBackRate = pDB->Search_Posistion(nRow, STDI_ChannelBackRate)->iValue;
			m_iChannelBackHappanRate = pDB->Search_Posistion(nRow, STDI_ChannelBackHappanRate)->iValue;
			
			// the end of one record
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}


		VOID SkillTemplateData_T::ReadSplitStringToTable( SplitStringLevelOne *pSplitL1, const CHAR* pSplitString, INT* pArry )
		{
			pSplitL1->Reset();
			pSplitL1->Init( '|', NULL );
			pSplitL1->DoSplit( pSplitString );
			INT iLineCount	= pSplitL1->GetResultLineCount();
			Assert( MAX_SUB_SKILL>=iLineCount && "子技能列表超出最大子技能数限制");
			if( MAX_SUB_SKILL<iLineCount )
			{
				iLineCount = MAX_SUB_SKILL;
			}
			for( INT i=0; i<iLineCount; ++i )
			{
				pArry[i] = atoi(pSplitL1->GetResultLine(i));
			}
		}
		//SkillInstance
		//class SkillInstanceData_T
		SkillInstanceData_T::SkillInstanceData_T() :
			m_bInited(FALSE),
			m_szDesc(NULL),
			m_nLogicID(-1),
			m_nSkillLevel(0)
		{
			memset( m_nCooldownTime, 0, sizeof(m_nCooldownTime) );
			memset( m_nChargeTime, 0, sizeof(m_nChargeTime) );
			memset( m_nChannelTime, 0, sizeof(m_nChannelTime) );
			memset( m_nAccuracy, 0, sizeof(m_nAccuracy) );
			memset( m_nCriticalRate, 0, sizeof(m_nCriticalRate) );
			for(INT i = 0; i < MAX_SKIL_LEVEL_LIMIT; i++) m_fRadius[i] = 0.0;
			memset( m_nMaxTargetNumber, 0, sizeof(m_nMaxTargetNumber) );
			
		}

		BOOL SkillInstanceData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
				Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			enum
			{
				SID_InstanceID		= 0,		//技能ID
				SID_Desc			= 4,		//效果的简要逻辑说明
				SID_LogicID			= 9,		//技能逻辑ID
				SDI_SkillMaxLevel	= 11,		//等级上限
				SID_CooldownTime	= 12,		//冷却时间
				SID_ChargeTime,					//聚气时间(ms)
				SID_ChannelTime,				//引导时间(ms)
				SID_ConDepTerms,				//条件或消耗参数说明
				SID_SkillLevel		= 27,		//目标级别
				STDI_Accuracy		= 66,		//命中率
				STDI_CriticalRate,				//会心率
				STDI_Radius 		= 70,		//范围半径
				STDI_MaxTargetNumber = 72,		//范围搜索的目标个数
				SID_Descriptors		= 85,
				SID_DescDetail		= 5			//技能描述（FOR Interface）

			};
			m_nInstanceID = pDB->Search_Posistion(nRow, SID_InstanceID)->iValue;
			//m_nGUID = pDB->Search_Posistion(nRow, column++)->iValue;
			
			m_nLogicID = pDB->Search_Posistion(nRow, SID_LogicID)->iValue;
			m_szDesc = pDB->Search_Posistion(nRow, SID_Desc)->pString;
			
			INT iSkillMaxLevel	= pDB->Search_Posistion(nRow, SDI_SkillMaxLevel)->iValue;
			SplitStringLevelOne oSplitL1;
			const CHAR* pSplitString = NULL;

			//m_nCooldownTime = pDB->Search_Posistion(nRow, SID_CooldownTime)->iValue;
			pSplitString = pDB->Search_Posistion(nRow, SID_CooldownTime)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, iSkillMaxLevel, m_nCooldownTime );
			//m_nChargeTime = pDB->Search_Posistion(nRow, SID_ChargeTime)->iValue;
			pSplitString = pDB->Search_Posistion(nRow, SID_ChargeTime)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, iSkillMaxLevel, m_nChargeTime );
			//m_nChannelTime = pDB->Search_Posistion(nRow, SID_ChannelTime)->iValue;
			pSplitString = pDB->Search_Posistion(nRow, SID_ChannelTime)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, iSkillMaxLevel, m_nChannelTime );

			INT column = SID_ConDepTerms;
			for(INT i=0;CONDITION_AND_DEPLETE_TERM_NUMBER>i;++i)
			{
				if(FALSE==m_aConDepTerms[i].InitFromDB(pDB, nRow, column))
				{
					AssertEx(NULL,"[SkillInstanceData_T::InitFromDB]:Load Skill Data Failed!");
				}
			}
			// read skill level
			m_nSkillLevel = pDB->Search_Posistion(nRow, SID_SkillLevel)->iValue;
			// read the Hit(Accuracy) rate info
			pSplitString = pDB->Search_Posistion(nRow, STDI_Accuracy)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, iSkillMaxLevel, m_nAccuracy);
			// read the Critical(double damage) rate info
			pSplitString = pDB->Search_Posistion(nRow, STDI_CriticalRate)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, iSkillMaxLevel, m_nCriticalRate);
			// read effective range radius
			pSplitString = pDB->Search_Posistion(nRow, STDI_Radius)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, iSkillMaxLevel, m_fRadius);
			// read max number of the target being effected
			pSplitString = pDB->Search_Posistion(nRow, STDI_MaxTargetNumber)->pString;
			ReadSplitStringToTable(&oSplitL1, pSplitString, iSkillMaxLevel, m_nMaxTargetNumber);
			// skip reserved field
			column = SID_Descriptors; 
			for(INT i=0;DESCRIPTOR_NUMBER>i;++i)
			{
				if(FALSE==m_aDescriptors[i].InitFromDB(pDB, nRow, column, TRUE))
				{
					AssertEx(NULL,"[SkillInstanceData_T::InitFromDB]:Load Skill Data Failed!");
				}
			}
			m_szDescDetail= pDB->Search_Posistion(nRow, SID_DescDetail)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}

		VOID SkillInstanceData_T::ReadSplitStringToTable( SplitStringLevelOne *pSplitL1, const CHAR* pSplitString, INT iMaxSkillLevel, INT* pArry )
		{
			pSplitL1->Reset();
			pSplitL1->Init( '|', NULL );
			pSplitL1->DoSplit( pSplitString );
			INT iLineCount	= pSplitL1->GetResultLineCount();
			Assert( iMaxSkillLevel>=iLineCount && MAX_SKIL_LEVEL_LIMIT>=iMaxSkillLevel );
			if( iMaxSkillLevel<iLineCount || MAX_SKIL_LEVEL_LIMIT<iMaxSkillLevel )
			{
				return;
			}
			for( INT i=0; i<iLineCount; ++i )
			{
				pArry[i] = atoi(pSplitL1->GetResultLine(i));
			}
		}

		VOID SkillInstanceData_T::ReadSplitStringToTable( SplitStringLevelOne *pSplitL1, const CHAR* pSplitString, INT iMaxSkillLevel, FLOAT* pArry )
		{
			pSplitL1->Reset();
			pSplitL1->Init( '|', NULL );
			pSplitL1->DoSplit( pSplitString );
			INT iLineCount	= pSplitL1->GetResultLineCount();
			Assert( iMaxSkillLevel>=iLineCount && MAX_SKIL_LEVEL_LIMIT>=iMaxSkillLevel );
			if( iMaxSkillLevel<iLineCount || MAX_SKIL_LEVEL_LIMIT<iMaxSkillLevel )
			{
				return;
			}
			for( INT i=0; i<iLineCount; ++i )
			{
				pArry[i] = atof(pSplitL1->GetResultLine(i));
			}
		}

		//class ImpactSEData_T
		BOOL ImpactSEData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
				Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			m_nID = pDB->Search_Posistion(nRow, column++)->iValue;			
			m_nMutexID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nPriParam= pDB->Search_Posistion(nRow, column++)->iValue;
			++column; //Skip Icon
			++column; //Skip 激活特效
			++column; //Skip 激活音效
			++column; //Skip 激活特效绑定点
			++column; //Skip 持续特效
			++column; //Skip 持续音效
			++column; //Skip 持续特效绑定点	
			m_bStillOnWhenOwnerDead = (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			m_bCanBeDispeled = (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			m_bHostileFlag = (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			m_bCanBeManualCancel= (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			++column; //Skipp 预留域
			++column; //Skipp 预留域
			m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}		
		//class SkillObjData_T
		BOOL SkillObjData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
				Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 1;
			m_nDoLogic = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nAction = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nSpecialEffect = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nSEBind = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nModel = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nIcon = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nFriendliness = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nTargetTypeRequirement = pDB->Search_Posistion(nRow, column++)->iValue;
			m_bPartyOnly = ((pDB->Search_Posistion(nRow, column++)->iValue)==0?FALSE:TRUE);
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}
	};
	namespace Impact_Module
	{
		//class ImpactData_T
		BOOL ImpactData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
				Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			// Load Data index
			m_nDataIndex 					= 	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Logic ID
			m_nLogicID 						= 	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Impact Name
			m_szName 						= 	pDB->Search_Posistion(nRow, column++)->pString;
			// Load Logic description
			m_szLogicDesc					= 	pDB->Search_Posistion(nRow, column++)->pString;
			// 是否是持续性效果
			m_bIsOverTimed 					=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load Impact ID
			m_nImpactID						=	pDB->Search_Posistion(nRow, column++)->iValue;	
			// Load Mutex ID of OverTimed Impact
			m_nMutexID 						= 	pDB->Search_Posistion(nRow, column++)->iValue;			
			// Load level of OverTimed Impact
			m_nLevel						= 	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Impact Stand Flag: -1 for Hostility,0 for Neutrolity, 1 for Amity
			m_nStandFlag					=	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Remain on Corpse flag
			m_bRemainOnCorpse				=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Can be dispeled by skill or item function 
			m_bCanBeDispeled				=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Can be Cancel by mouse click
			m_bCanBeCanceled				=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of need unit to keep channeling 
			m_bNeedChannelSupport			=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of this impact will ignore any impact fliters or refixer;
			m_bIgnoreFliter					=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Fading out when unit on damage;
			m_bFadeOutWhenUnitOnDamage		=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Fading out when unit starting an action;
			m_bFadeOutWhenUnitStartActions	=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Fading out when unit offline;
			m_bFadeOutWhenUnitOffline		=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of keeping timeing when unit offline
			m_bStillTimingWhenUnitOffline	=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load continuance of this impact remaining on unit
			m_nContinuance					=	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load interval of this impact between two activating
			m_nInterval						=	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load flag of the impact mutex process with caster id or not.
			m_bMutexByCaster				=	(0<pDB->Search_Posistion(nRow, column++)->iValue?TRUE:FALSE);
			// skip reversed columns
			column += 4;
			// Load Descriptors
			for(INT nIdx=0;NUMBER_OF_PARAMS>nIdx;++nIdx)
			{
				if(FALSE==m_aParams[nIdx].InitFromDB(pDB, nRow, column))
				{
					Assert(NULL == "[ImpactData_T::InitFromDB]:Load Descriptor Data Failed!");
				}
			}
			// Load Internal Description
			m_szDescription 				= pDB->Search_Posistion(nRow, column++)->pString;
			m_uBallCount	= pDB->Search_Posistion(nRow, column++)->iValue;
			m_uBallLife		= pDB->Search_Posistion(nRow, column++)->iValue;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		};
	};
	namespace Special_Obj_Module
	{
		//class SpecialObjData_T
		SpecialObjData_T::SpecialObjData_T() :
	m_bInited(FALSE),
		m_nDataID(INVALID_ID),
		m_szName(NULL),
		m_nLogicID(INVALID_ID),
		m_nTrapUsedFlags(0),
		m_nActiveTimes(0),
		m_nContinuance(0),
		m_nInterval(0),
		m_fTriggerRadius(0.0),
		m_fEffectRadius(0.0),
		m_nEffectedObjNumber(0),
		m_szDescription(NULL)
	{
	}
	BOOL SpecialObjData_T::InitFromDB(DBCFile* pDB, INT const nRow)
	{
		__ENTER_FUNCTION
			Assert(pDB);
		Assert(0<=nRow);
		Assert(MAX_ID > nRow);
		INT column = 0;
		m_nDataID = pDB->Search_Posistion(nRow, column++)->iValue;
		m_szName = pDB->Search_Posistion(nRow, column++)->pString;
		m_nClass = pDB->Search_Posistion(nRow, column++)->iValue;
		++column; //Skip Tooltip
		m_nLogicID = pDB->Search_Posistion(nRow, column++)->iValue;
		m_nStealthLevel = pDB->Search_Posistion(nRow, column++)->iValue; 
		m_nTrapUsedFlags = pDB->Search_Posistion(nRow, column++)->iValue;
		m_nActiveTimes = pDB->Search_Posistion(nRow, column++)->iValue;
		column = column + 10; //Skip client data area

		m_nContinuance = pDB->Search_Posistion(nRow, column++)->iValue;
		m_nInterval = pDB->Search_Posistion(nRow, column++)->iValue;

		m_fTriggerRadius = pDB->Search_Posistion(nRow, column++)->fValue;
		m_fEffectRadius = pDB->Search_Posistion(nRow, column++)->fValue;
		m_nEffectedObjNumber = pDB->Search_Posistion(nRow, column++)->iValue;
		//Skip reserved fields
		column = column + 5;

		for(INT i=0;DESCRIPTOR_NUMBER>i;++i)
		{
			if(FALSE==m_aDescriptors[i].InitFromDB(pDB, nRow, column))
			{
				AssertEx(NULL, "[SpecialObjData_T::InitFromDB]:Load Descriptional Data Failed!");
			}
		}
		m_szDescription= pDB->Search_Posistion(nRow, column++)->pString;
		m_bInited = TRUE;
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
	}
	}
};

