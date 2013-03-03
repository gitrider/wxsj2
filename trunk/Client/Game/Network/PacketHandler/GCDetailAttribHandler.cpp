
#include "StdAfx.h"
#include "Global.h"
#include "GCDetailAttrib.h"
#include "GITimeSystem.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "GameCommand.h"
#include "DataPool\DataPoolCommandDef.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Event\GMEventSystem.h"
#include "Object\ObjectCommandDef.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Surface\Obj_Bullet.h"
#include "Object\Manager\BulletEntityManager.h"
#include "DataPool/GMDP_Struct_QuestLog.h"

using namespace Packets;

uint GCDetailAttribHandler::Execute( GCDetailAttrib* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID());
		if(pObj == NULL)
			return PACKET_EXE_CONTINUE;
        
		if(!g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			return PACKET_EXE_CONTINUE;

		CObject_Character* pCharObj = (CObject_Character*)pObj;
		CCharacterData* pCharacterData = pCharObj->GetCharacterData();

		// 玩家详细的属性刷新
		Flag128 flagDetail = pPacket->GetFlags();

		// 等级
		if( flagDetail.isSetBit(DETAIL_ATTRIB_LEVEL) )
		{
			pCharacterData->Set_Level(pPacket->GetLevel());

			// 根据等级过滤可接任务列表
			CGameProcedure::s_pQuestLogDataMgr->SetFilter(pPacket->GetLevel());
		}

		// 生命值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_HP) )
			pCharacterData->Set_HP(pPacket->GetHP());

		// 魔法值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MP) )
			pCharacterData->Set_MP(pPacket->GetMP());

		//元气值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_PNEUMA) )
			pCharacterData->Set_Penuma(pPacket->GetPneuma());

		// 经验值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_EXP) )
		{
			// 首次属于进入场景赋值
			if( -1 != pCharacterData->Get_Exp() )
			{
				CBulletEntityManager::GetMe()->CreateExpEffect(pPacket->GetExp(), pPacket->GetTargetID(), pPacket->GetObjID() );				
			}
			pCharacterData->Set_Exp(pPacket->GetExp());
		}

		// 金钱
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MONEY) )
		{
			pCharacterData->Set_Money(pPacket->GetMoney());
		}


		//------------------------------------------------

		// 力量
		if( flagDetail.isSetBit( DETAIL_ATTRIB_STR) )
			pCharacterData->Set_STR(pPacket->GetStr());

		// 体制
		if( flagDetail.isSetBit( DETAIL_ATTRIB_CON) )
			pCharacterData->Set_CON(pPacket->GetCon());

		// 智力
		if( flagDetail.isSetBit( DETAIL_ATTRIB_INT) )
			pCharacterData->Set_INT(pPacket->GetInt());

		// 敏捷
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEX) )
			pCharacterData->Set_DEX(pPacket->GetDex());
		
		// 剩余点数
		if( flagDetail.isSetBit( DETAIL_ATTRIB_POINT_REMAIN) )
			pCharacterData->Set_PointRemain(pPacket->GetPoint_Remain());

		// 近程攻击
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_NEAR) )	
			pCharacterData->Set_AttNear(pPacket->GetAtt_Near());
		// 近程防御
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_NEAR) )	
			pCharacterData->Set_DefNear(pPacket->GetDef_Near());

		// 远程攻击
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_FAR) )	
			pCharacterData->Set_AttFar(pPacket->GetAtt_Far());
		// 远程防御
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_FAR) )	
			pCharacterData->Set_DefFar(pPacket->GetDef_Far());

		// 近程内功攻击力
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_MAGIC_NEAR) )
			pCharacterData->Set_AttMagic_Near(pPacket->GetAtt_MagicNear());
		// 近程内功防御力
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_MAGIC_NEAR) )
			pCharacterData->Set_DefMagic_Near(pPacket->GetDef_MagicNear());

		// 远程内功攻击力
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_MAGIC_FAR) )
			pCharacterData->Set_AttMagic_Far(pPacket->GetAtt_MagicFar());
		// 远程内功防御力
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_MAGIC_FAR) )
			pCharacterData->Set_DefMagic_Far(pPacket->GetDef_MagicFar());

		// 最大生命值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXHP) )
			pCharacterData->Set_MaxHP(pPacket->GetMAXHP());

		// 最大魔法值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXMP) )
			pCharacterData->Set_MaxMP(pPacket->GetMAXMP());

		// 最大元气值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXPNEUMA ))
			pCharacterData->Set_MaxPneuma(pPacket->GetMAXPneuma());

		// 最大怒气值
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXRAGE ))
			pCharacterData->Set_MaxRage(pPacket->GetMaxRage());

		

		// HP恢复速度
		if( flagDetail.isSetBit( DETAIL_ATTRIB_HP_RESPEED) )
			pCharacterData->Set_HPRespeed(pPacket->GetHP_ReSpeed());

		// MP恢复速度
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MP_RESPEED) )
			pCharacterData->Set_MPRespeed(pPacket->GetMP_ReSpeed());

		 // 韧性
		if( flagDetail.isSetBit( DETAIL_ATTRIB_TOUGHNESS ))
			pCharacterData->Set_Toughness(pPacket->GetToughness());

		// 命中率
		if( flagDetail.isSetBit( DETAIL_ATTRIB_HIT) )
			pCharacterData->Set_Hit(pPacket->GetHit());
		// 闪避率
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MISS) )
			pCharacterData->Set_Miss(pPacket->GetMiss());
		// 会心率
		if( flagDetail.isSetBit( DETAIL_ATTRIB_CRITRATE) )
			pCharacterData->Set_CritRate(pPacket->GetCritic());

		// 怒气
		if( flagDetail.isSetBit( DETAIL_ATTRIB_RAGE) )
		{
			pCharacterData->Set_Rage(pPacket->GetRage());

			// 怒气更新要通知界面
			CEventSystem::GetMe()->PushEvent(GE_UNIT_RAGE);
		}

		// 连技点
		if( flagDetail.isSetBit( DETAIL_ATTRIB_STRIKE_POINT) )
			pCharacterData->Set_StrikePoint(pPacket->GetStrikePoint());

		// 移动速度
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MOVESPEED) )
			pCharacterData->Set_MoveSpeed(pPacket->GetMoveSpeed());

		// 攻击速度
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKSPEED) )
			pCharacterData->Set_AttackSpeed(pPacket->GetAttSpeed());

		// 金攻击
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_GOLD) )
			pCharacterData->Set_AttGold(pPacket->GetAttGold());
		// 金防御
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_GOLD) )
			pCharacterData->Set_DefGold(pPacket->GetDefGold());

		// 火攻击
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_FIRE) )
			pCharacterData->Set_AttFire(pPacket->GetAttFire());
		// 火防御
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_FIRE) )
			pCharacterData->Set_DefFire(pPacket->GetDefFire());

		// 木攻击
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_WOOD) )
			pCharacterData->Set_AttWood(pPacket->GetAttWood());
		// 木防御
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_WOOD) )
			pCharacterData->Set_DefWood(pPacket->GetDefWood());

		// 水攻击
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_WATER) )
			pCharacterData->Set_AttWater(pPacket->GetAttWater());
		// 水防御
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_WATER) )
			pCharacterData->Set_DefWater(pPacket->GetDefWater());

		// 土攻击
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_SOIL) )
			pCharacterData->Set_AttSoil(pPacket->GetAttSoil());
		// 土防御
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_SOIL) )
			pCharacterData->Set_DefSoil(pPacket->GetDefSoil());


		// 职业
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MEMPAI) )
		{
			if( pObj != CObjectManager::GetMe()->GetMySelf() )
				pCharacterData->Set_MenPai(pPacket->GetMenpai());
		}

		// GUID
		if( flagDetail.isSetBit(DETAIL_ATTRIB_GUID))
			pCharacterData->Set_GUID(pPacket->GetGUID());

		// 帮派
		if( flagDetail.isSetBit(DETAIL_ATTRIB_GUILD) )
			pCharacterData->Set_Guild(pPacket->GetGuild());

		// 阵营
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CAMP))
			pCharacterData->Set_CampData(pPacket->GetCampData());

		if( flagDetail.isSetBit(DETAIL_ATTRIB_DATAID) )
			pCharacterData->Set_RaceID(pPacket->GetDataID());

		// 外形
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MODELID) )
			pCharacterData->Set_ModelID(pPacket->GetModelID());

		// 座骑
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MOUNTID) )	
			pCharacterData->Set_MountID(pPacket->GetMountID());

		// 座骑index   [7/23/2010 陈军龙]
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MOUNTINDEX) )	
			pCharacterData->Set_MountIndex(pPacket->GetMountIndex());

		// 当前的宠物GUID
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID) )
			pCharacterData->Set_CurrentPetGUID(pPacket->GetCurrentPetGUID());

		// 是否限制不能移动
		if( flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE) )
			pCharacterData->SetLimitMove(pPacket->GetLimitMove());

		// 技能限制标记1
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION1) )
			pCharacterData->SetCanActionFlag1(pPacket->GetCanActionFlag1());
		//技能限制标记2
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION2) )
			pCharacterData->SetCanActionFlag2(pPacket->GetCanActionFlag2());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_USE_SKILL)	)	//39
		//	pCharacterData->SetLimitUseSkill(pPacket->GetLimitUseSkill());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE)	)	//40
		//	pCharacterData->SetLimitHandle(pPacket->GetLimitHandle());

		// 活力
		if( flagDetail.isSetBit(DETAIL_ATTRIB_VIGOR) )
			pCharacterData->Set_Vigor(pPacket->GetVigor());
		// 活力上限
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MAX_VIGOR) )
			pCharacterData->Set_MaxVigor(pPacket->GetMaxVigor());

		// 精力
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ENERGY) )
			pCharacterData->Set_Energy(pPacket->GetEnergy());
		// 精力上限
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MAX_ENERGY) )
			pCharacterData->Set_MaxEnergy(pPacket->GetMaxEnergy());

		// 善恶值
		if( flagDetail.isSetBit(DETAIL_ATTRIB_GOODBADVALUE) )
			pCharacterData->Set_GoodBadValue(pPacket->GetGoodBadValue());

//		if(flagDetail.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS) )
//			pCharacterData->Set_QuestHistory(pPacket->GetMissionHaveDoneFlags());

		// 国家
		if( flagDetail.isSetBit(DETAIL_ATTRIB_COUNTRY) )
			pCharacterData->Set_Country(pPacket->GetCountry());
		
		// 脸模型
		if( flagDetail.isSetBit(DETAIL_ATTRIB_FACEMODEL) )
			pCharacterData->Set_FaceMesh(pPacket->GetFaceModel());

		// 外形头像
	//laim	if( flagDetail.isSetBit(DETAIL_ATTRIB_MODELID) )
	//		pCharacterData->Set_PortraitID(pPacket->GetPortraitID());

		// 技能相关
		if( flagDetail.isSetBit(DETAIL_ATTRIB_S_SKILLPOINT) || flagDetail.isSetBit(DETAIL_ATTRIB_SKILLPOINT_REMAIN) )
		{
			// 系点数
			if( flagDetail.isSetBit(DETAIL_ATTRIB_S_SKILLPOINT) )
			{
				pCharacterData->Set_SeriesPoint(COMMON_S1, pPacket->GetS_SkillPoint(COMMON_S1));
				pCharacterData->Set_SeriesPoint(COMMON_S2, pPacket->GetS_SkillPoint(COMMON_S2));
				pCharacterData->Set_SeriesPoint(COMMON_S3, pPacket->GetS_SkillPoint(COMMON_S3));
			}

			// 技能点数
			if( flagDetail.isSetBit(DETAIL_ATTRIB_SKILLPOINT_REMAIN) )
				pCharacterData->Set_CurrentSkillPoint(pPacket->GetSkillPoint_Remain());

			CEventSystem::GetMe()->PushEvent(GE_SKILL_UPDATE);
		}
		


		pObj->PushDebugString("GCDetailAttrib");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

