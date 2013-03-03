
// GCDetailAttrib_PetHandler.cpp

#include "StdAfx.h"
#include "GCDetailAttrib_Pet.h"
#include "DataPool\GMDP_Struct_Pet.h"
#include "DataPool\GMDataPool.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "../../Event/GMEventSystem.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "../../dbc/GMDataBase.h"



using namespace Packets;


UINT GCDetailAttrib_PetHandler::Execute( GCDetailAttrib_Pet* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{

			Flag64  flagDetail = pPacket->GetFlags();
			BYTE	ExtraInfoLength = pPacket->GetExtraInfoLength();
			BYTE    ExtraInfoData[MAX_EXTRAINFO_LENGTH];

			memset(ExtraInfoData, 0, sizeof(ExtraInfoData));
			memcpy(ExtraInfoData, pPacket->GetExtraInfoData(), ExtraInfoLength);

			// 宠物交易信息
			if(ExtraInfoData[0] == TYPE_EXCHANGE)
			{
			}
			// 其他玩家宠物的察看信息
			else if(ExtraInfoData[0] == TYPE_CONTEX_MENU_OTHER_PET)
			{				
				if (ExtraInfoLength != 1)
				{
					KLAssert(ExtraInfoLength == 1);
					return PACKET_EXE_CONTINUE;
				}

				if (0 == flagDetail.m_uHighFlags && 0 == flagDetail.m_uLowFlags)
					return PACKET_EXE_CONTINUE;

				SDATA_PET* pTargetPet = CDataPool::GetMe()->TargetPet_GetPet();
				if (NULL == pTargetPet)			
					return PACKET_EXE_CONTINUE;			

				pTargetPet->m_GUID = pPacket->GetGUID();

				// 基本数据ID
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )
					pTargetPet->m_nBaseID = pPacket->GetDataID();

				// 名称
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NAME) )
					pTargetPet->m_szName = pPacket->GetName();

				// 昵称
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NICK_NAME) )
					pTargetPet->m_szNickname = pPacket->GetNickName();

				// 性别
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SEX) )
					pTargetPet->m_nGender = pPacket->GetSex();

				// 等级
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LEVEL) )
					pTargetPet->m_nLevel = pPacket->GetLevel();

				// 几代宠
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENERATION) )
					pTargetPet->m_nEraCount = pPacket->GetGeneration();

				// 快乐度
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HAPPINESS) )
					pTargetPet->m_nHappiness = pPacket->GetHappiness();

				// 经验
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_EXP) )			
					pTargetPet->m_nExp = pPacket->GetExp();

				// 力量资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION) )			
					pTargetPet->m_nAttrStrApt = pPacket->GetStrPerception();

				// 体力资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION) )			
					pTargetPet->m_nAttrConApt = pPacket->GetConPerception();

				// 敏捷资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION) )			
					pTargetPet->m_nAttrDexApt = pPacket->GetDexPerception();

				// 智力资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION) )			
					pTargetPet->m_nAttrIntApt = pPacket->GetIntPerception();

				// 力量
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STR) )			
					pTargetPet->m_nAttrStr = pPacket->GetStr();

				// 体力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CON) )			
					pTargetPet->m_nAttrCon = pPacket->GetCon();

				// 敏捷
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEX) )			
					pTargetPet->m_nAttrDex = pPacket->GetDex();

				// 智力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INT) )			
					pTargetPet->m_nAttrInt = pPacket->GetInt();

				// 根骨
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
					pTargetPet->m_nBasic = pPacket->GetGenGu();

				// 潜能
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN) )			
					pTargetPet->m_nPotential = pPacket->GetRemainPoint();

				// 悟性
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SAVVY) )			
					pTargetPet->m_nSavvy = pPacket->GetSavvy();

				// 成长率
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GROWRATE) )			
					pTargetPet->m_nUpRate = pPacket->GetGrowRate();

				
				
				// 性格
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_AI_TYPE) )			
					pTargetPet->m_nAIType = pPacket->GetAIType();
				
				// 配偶的GUID
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID) )			
					pTargetPet->m_SpouseGUID = pPacket->GetSpouseGUID();

				// 血当前值
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HP) )			
					pTargetPet->m_nHP = pPacket->GetHP();
				
				// 血最大值
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HPMAX) )			
					pTargetPet->m_nHPMax = pPacket->GetHPMax();
				
				// 当前寿命
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LIFE) )			
					pTargetPet->m_nLife = pPacket->GetLife();

				// 命中率
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HIT) )			
					pTargetPet->m_nHit = pPacket->GetHit();
				
				// 闪避率
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MISS) )			
					pTargetPet->m_nMiss = pPacket->GetMiss();
				
				// 会心率
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CRITICAL) )			
					pTargetPet->m_nCritical = pPacket->GetCritical();

				//近程外功攻击力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_NEAR) )			
					pTargetPet->m_nAtt_Near = pPacket->GetAtt_Near();
				
				//远程外功攻击力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_FAR) )			
					pTargetPet->m_nAtt_Far = pPacket->GetAtt_Far();

				// 近程内功攻击力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC_NEAR) )			
					pTargetPet->m_nAtt_MagicNear = pPacket->GetAtt_MagicNear();
				
				// 远程内功攻击力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC_FAR) )			
					pTargetPet->m_nAtt_MagicFar = pPacket->GetAtt_MagicFar();

				//近程外功防御力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_NEAR) )			
					pTargetPet->m_nDef_Near = pPacket->GetDef_Near();
				
				//远程外功防御力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_FAR) )			
					pTargetPet->m_nDef_Far = pPacket->GetDef_Far();

				// 近程内功防御力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC_NEAR) )			
					pTargetPet->m_nDef_MagicNear = pPacket->GetDef_MagicNear();
				
				// 远程内功防御力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC_FAR) )			
					pTargetPet->m_nDef_MagicFar = pPacket->GetDef_MagicFar();

				// 技能1
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_0) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(0);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(0,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能2
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_1) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(1);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(1,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能3
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_2) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(2);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(2,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能4
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_3) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(3);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(3,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能5
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_4) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(4);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(4,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能6
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_5) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(5);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(5,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能7
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_6) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(6);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(6,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能8
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_7) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(7);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(7,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能9
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_8) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(8);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(8,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能10
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_9) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(9);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(9,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能11
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_10) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(10);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(10,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能12
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_11) ) 
				{			
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(11);
					if (pOwnerSkill)
						CDataPool::GetMe()->TargetPet_SetSkill(11,pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 模型ID
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MOUNTID) )	
				{
					DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
						const _DBC_CHARACTER_MOUNT *pMount = (const _DBC_CHARACTER_MOUNT*)(s_pCharMountDBC->Search_Index_EQU(pPacket->GetMountID()));
					pTargetPet->m_pModelData = pMount;

					// 创建UI模型
					CDataPool::GetMe()->TargetPet_SetPetFakeModel(pPacket->GetMountID());
				}

				//刷新界面
				CEventSystem::GetMe()->PushEvent(GE_TARGET_MOUNT_ATTR_UPDATE);
			}


			// 自己的宠物信息
			else if(ExtraInfoLength == 0)
			{
				if( 0 == flagDetail.m_uHighFlags && 0 == flagDetail.m_uLowFlags)
					return PACKET_EXE_CONTINUE;

				SDATA_PET* pMyPet = NULL;
				PET_GUID_t guidPet = pPacket->GetGUID();

				INT nPetIndex = -1;

				BOOL bNewPet = FALSE;// 信息显示判断用

				// 查找该宠物
				for (INT i=0; i<HUMAN_PET_MAX_COUNT; ++i)
				{
					SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(i);
					if (pPetData && pPetData->m_GUID == guidPet)
					{
						nPetIndex = i;
						pMyPet = pPetData;

						break;
					}
				}

				// 没有该宠物，创建
				if (!pMyPet)
				{
					for(INT i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
					{
						SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(i);
						if (pPetData && (pPetData->m_GUID.IsNull()))
						{
							nPetIndex = i;
							pMyPet = pPetData;
							bNewPet = TRUE;

							break;
						}
					}
				}

				if (!pMyPet)
					return PACKET_EXE_CONTINUE;


				// 坐骑GUID
				pMyPet->m_GUID = pPacket->GetGUID();

				// 坐骑状态
				if (HorseState_Normal == pPacket->GetHorseState())
				{
					pMyPet->m_bLocked = FALSE;
				}
				else
				{
					pMyPet->m_bLocked = TRUE;
				}

				// 基本数据ID
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )
					pMyPet->m_nBaseID = pPacket->GetDataID();

				// 名称
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NAME) )
					pMyPet->m_szName = pPacket->GetName();

				// 昵称
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NICK_NAME) )
					pMyPet->m_szNickname = pPacket->GetNickName();

				// 性别
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SEX) )
					pMyPet->m_nGender = pPacket->GetSex();


				// 等级
				if (flagDetail.isSetBit(PET_DETAIL_ATTR_LEVEL))
				{
					INT nOldLv = pMyPet->m_nLevel;
					pMyPet->m_nLevel = pPacket->GetLevel();

					// 显示宠物升级提示信息
					if (pMyPet->m_nLevel > nOldLv && FALSE == bNewPet)
					{
						STRING strTemp = COLORMSGFUNC("GCDetailAttrib_Pet_PetLevelUp", pMyPet->m_szName.c_str(), pMyPet->m_nLevel);
						ADDTALKMSG(strTemp);
					}
				}

				// 几代宠
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENERATION) )
					pMyPet->m_nEraCount = pPacket->GetGeneration();

				// 快乐度
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HAPPINESS) )
					pMyPet->m_nHappiness = pPacket->GetHappiness();

				// 经验
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_EXP) )	
				{
					INT nOldExp = pMyPet->m_nExp;
					pMyPet->m_nExp = pPacket->GetExp();

					// 显示宠物获得经验值的提示信息
					if(pMyPet->m_nExp > nOldExp && FALSE == bNewPet)
					{
						STRING strTemp = COLORMSGFUNC("GCDetailAttrib_Pet_PetExpUp", pMyPet->m_szName.c_str(), pMyPet->m_nExp-nOldExp);
						ADDTALKMSG(strTemp);
					}
				}

				// 力量资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION) )			
					pMyPet->m_nAttrStrApt = pPacket->GetStrPerception();

				// 体力资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION) )			
					pMyPet->m_nAttrConApt = pPacket->GetConPerception();

				// 敏捷资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION) )			
					pMyPet->m_nAttrDexApt = pPacket->GetDexPerception();

				// 智力资质
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION) )			
					pMyPet->m_nAttrIntApt = pPacket->GetIntPerception();


				// 力量
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STR) )			
					pMyPet->m_nAttrStr = pPacket->GetStr();

				// 体力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CON) )			
					pMyPet->m_nAttrCon = pPacket->GetCon();

				// 敏捷
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEX) )			
					pMyPet->m_nAttrDex = pPacket->GetDex();

				// 智力
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INT) )			
					pMyPet->m_nAttrInt = pPacket->GetInt();


				// 根骨
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
					pMyPet->m_nBasic = pPacket->GetGenGu();

				// 潜能
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN) )			
					pMyPet->m_nPotential = pPacket->GetRemainPoint();

				// 悟性
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SAVVY) )			
					pMyPet->m_nSavvy = pPacket->GetSavvy();

				// 成长率
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GROWRATE) )			
					pMyPet->m_nUpRate = pPacket->GetGrowRate();


				pMyPet->m_nSkillGrid = 6;

				// 技能1
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_0) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(0);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 0, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能2
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_1) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(1);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 1, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能3
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_2) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(2);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 2, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能4
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_3) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(3);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 3, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能5
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_4) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(4);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 4, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能6
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_5) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(5);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 5, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能7
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_6) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(6);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 6, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能8
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_7) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(7);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 7, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能9
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_8) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(8);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 8, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能10
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_9) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(9);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 9, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能11
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_10) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(10);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 10, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 技能12
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_11) ) 
				{
					const _PET_SKILL* pOwnerSkill = pPacket->GetSkill(11);
					if (pOwnerSkill)
						CDataPool::GetMe()->Pet_SetSkill(nPetIndex, 11, pOwnerSkill->m_nSkillID, pOwnerSkill->m_bCanUse);
				}

				// 骑宠数据
				if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MOUNTID) )
				{
					DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
						const _DBC_CHARACTER_MOUNT *pMount = (const _DBC_CHARACTER_MOUNT*)(s_pCharMountDBC->Search_Index_EQU(pPacket->GetMountID()));
					pMyPet->m_pModelData = pMount;

					// 创建UI模型
					CDataPool::GetMe()->Pet_SetPetFakeModel(nPetIndex, pPacket->GetMountID());
				}

				CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);
			}

		}

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

			return PACKET_EXE_ERROR ;
}
