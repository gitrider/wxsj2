
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

		// �����ϸ������ˢ��
		Flag128 flagDetail = pPacket->GetFlags();

		// �ȼ�
		if( flagDetail.isSetBit(DETAIL_ATTRIB_LEVEL) )
		{
			pCharacterData->Set_Level(pPacket->GetLevel());

			// ���ݵȼ����˿ɽ������б�
			CGameProcedure::s_pQuestLogDataMgr->SetFilter(pPacket->GetLevel());
		}

		// ����ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_HP) )
			pCharacterData->Set_HP(pPacket->GetHP());

		// ħ��ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MP) )
			pCharacterData->Set_MP(pPacket->GetMP());

		//Ԫ��ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_PNEUMA) )
			pCharacterData->Set_Penuma(pPacket->GetPneuma());

		// ����ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_EXP) )
		{
			// �״����ڽ��볡����ֵ
			if( -1 != pCharacterData->Get_Exp() )
			{
				CBulletEntityManager::GetMe()->CreateExpEffect(pPacket->GetExp(), pPacket->GetTargetID(), pPacket->GetObjID() );				
			}
			pCharacterData->Set_Exp(pPacket->GetExp());
		}

		// ��Ǯ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MONEY) )
		{
			pCharacterData->Set_Money(pPacket->GetMoney());
		}


		//------------------------------------------------

		// ����
		if( flagDetail.isSetBit( DETAIL_ATTRIB_STR) )
			pCharacterData->Set_STR(pPacket->GetStr());

		// ����
		if( flagDetail.isSetBit( DETAIL_ATTRIB_CON) )
			pCharacterData->Set_CON(pPacket->GetCon());

		// ����
		if( flagDetail.isSetBit( DETAIL_ATTRIB_INT) )
			pCharacterData->Set_INT(pPacket->GetInt());

		// ����
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEX) )
			pCharacterData->Set_DEX(pPacket->GetDex());
		
		// ʣ�����
		if( flagDetail.isSetBit( DETAIL_ATTRIB_POINT_REMAIN) )
			pCharacterData->Set_PointRemain(pPacket->GetPoint_Remain());

		// ���̹���
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_NEAR) )	
			pCharacterData->Set_AttNear(pPacket->GetAtt_Near());
		// ���̷���
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_NEAR) )	
			pCharacterData->Set_DefNear(pPacket->GetDef_Near());

		// Զ�̹���
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_FAR) )	
			pCharacterData->Set_AttFar(pPacket->GetAtt_Far());
		// Զ�̷���
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_FAR) )	
			pCharacterData->Set_DefFar(pPacket->GetDef_Far());

		// �����ڹ�������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_MAGIC_NEAR) )
			pCharacterData->Set_AttMagic_Near(pPacket->GetAtt_MagicNear());
		// �����ڹ�������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_MAGIC_NEAR) )
			pCharacterData->Set_DefMagic_Near(pPacket->GetDef_MagicNear());

		// Զ���ڹ�������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_MAGIC_FAR) )
			pCharacterData->Set_AttMagic_Far(pPacket->GetAtt_MagicFar());
		// Զ���ڹ�������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_MAGIC_FAR) )
			pCharacterData->Set_DefMagic_Far(pPacket->GetDef_MagicFar());

		// �������ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXHP) )
			pCharacterData->Set_MaxHP(pPacket->GetMAXHP());

		// ���ħ��ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXMP) )
			pCharacterData->Set_MaxMP(pPacket->GetMAXMP());

		// ���Ԫ��ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXPNEUMA ))
			pCharacterData->Set_MaxPneuma(pPacket->GetMAXPneuma());

		// ���ŭ��ֵ
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MAXRAGE ))
			pCharacterData->Set_MaxRage(pPacket->GetMaxRage());

		

		// HP�ָ��ٶ�
		if( flagDetail.isSetBit( DETAIL_ATTRIB_HP_RESPEED) )
			pCharacterData->Set_HPRespeed(pPacket->GetHP_ReSpeed());

		// MP�ָ��ٶ�
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MP_RESPEED) )
			pCharacterData->Set_MPRespeed(pPacket->GetMP_ReSpeed());

		 // ����
		if( flagDetail.isSetBit( DETAIL_ATTRIB_TOUGHNESS ))
			pCharacterData->Set_Toughness(pPacket->GetToughness());

		// ������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_HIT) )
			pCharacterData->Set_Hit(pPacket->GetHit());
		// ������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MISS) )
			pCharacterData->Set_Miss(pPacket->GetMiss());
		// ������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_CRITRATE) )
			pCharacterData->Set_CritRate(pPacket->GetCritic());

		// ŭ��
		if( flagDetail.isSetBit( DETAIL_ATTRIB_RAGE) )
		{
			pCharacterData->Set_Rage(pPacket->GetRage());

			// ŭ������Ҫ֪ͨ����
			CEventSystem::GetMe()->PushEvent(GE_UNIT_RAGE);
		}

		// ������
		if( flagDetail.isSetBit( DETAIL_ATTRIB_STRIKE_POINT) )
			pCharacterData->Set_StrikePoint(pPacket->GetStrikePoint());

		// �ƶ��ٶ�
		if( flagDetail.isSetBit( DETAIL_ATTRIB_MOVESPEED) )
			pCharacterData->Set_MoveSpeed(pPacket->GetMoveSpeed());

		// �����ٶ�
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKSPEED) )
			pCharacterData->Set_AttackSpeed(pPacket->GetAttSpeed());

		// �𹥻�
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_GOLD) )
			pCharacterData->Set_AttGold(pPacket->GetAttGold());
		// �����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_GOLD) )
			pCharacterData->Set_DefGold(pPacket->GetDefGold());

		// �𹥻�
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_FIRE) )
			pCharacterData->Set_AttFire(pPacket->GetAttFire());
		// �����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_FIRE) )
			pCharacterData->Set_DefFire(pPacket->GetDefFire());

		// ľ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_WOOD) )
			pCharacterData->Set_AttWood(pPacket->GetAttWood());
		// ľ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_WOOD) )
			pCharacterData->Set_DefWood(pPacket->GetDefWood());

		// ˮ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_WATER) )
			pCharacterData->Set_AttWater(pPacket->GetAttWater());
		// ˮ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_WATER) )
			pCharacterData->Set_DefWater(pPacket->GetDefWater());

		// ������
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ATTACK_SOIL) )
			pCharacterData->Set_AttSoil(pPacket->GetAttSoil());
		// ������
		if( flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCE_SOIL) )
			pCharacterData->Set_DefSoil(pPacket->GetDefSoil());


		// ְҵ
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MEMPAI) )
		{
			if( pObj != CObjectManager::GetMe()->GetMySelf() )
				pCharacterData->Set_MenPai(pPacket->GetMenpai());
		}

		// GUID
		if( flagDetail.isSetBit(DETAIL_ATTRIB_GUID))
			pCharacterData->Set_GUID(pPacket->GetGUID());

		// ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_GUILD) )
			pCharacterData->Set_Guild(pPacket->GetGuild());

		// ��Ӫ
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CAMP))
			pCharacterData->Set_CampData(pPacket->GetCampData());

		if( flagDetail.isSetBit(DETAIL_ATTRIB_DATAID) )
			pCharacterData->Set_RaceID(pPacket->GetDataID());

		// ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MODELID) )
			pCharacterData->Set_ModelID(pPacket->GetModelID());

		// ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MOUNTID) )	
			pCharacterData->Set_MountID(pPacket->GetMountID());

		// ����index   [7/23/2010 �¾���]
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MOUNTINDEX) )	
			pCharacterData->Set_MountIndex(pPacket->GetMountIndex());

		// ��ǰ�ĳ���GUID
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID) )
			pCharacterData->Set_CurrentPetGUID(pPacket->GetCurrentPetGUID());

		// �Ƿ����Ʋ����ƶ�
		if( flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE) )
			pCharacterData->SetLimitMove(pPacket->GetLimitMove());

		// �������Ʊ��1
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION1) )
			pCharacterData->SetCanActionFlag1(pPacket->GetCanActionFlag1());
		//�������Ʊ��2
		if( flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION2) )
			pCharacterData->SetCanActionFlag2(pPacket->GetCanActionFlag2());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_USE_SKILL)	)	//39
		//	pCharacterData->SetLimitUseSkill(pPacket->GetLimitUseSkill());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE)	)	//40
		//	pCharacterData->SetLimitHandle(pPacket->GetLimitHandle());

		// ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_VIGOR) )
			pCharacterData->Set_Vigor(pPacket->GetVigor());
		// ��������
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MAX_VIGOR) )
			pCharacterData->Set_MaxVigor(pPacket->GetMaxVigor());

		// ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_ENERGY) )
			pCharacterData->Set_Energy(pPacket->GetEnergy());
		// ��������
		if( flagDetail.isSetBit(DETAIL_ATTRIB_MAX_ENERGY) )
			pCharacterData->Set_MaxEnergy(pPacket->GetMaxEnergy());

		// �ƶ�ֵ
		if( flagDetail.isSetBit(DETAIL_ATTRIB_GOODBADVALUE) )
			pCharacterData->Set_GoodBadValue(pPacket->GetGoodBadValue());

//		if(flagDetail.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS) )
//			pCharacterData->Set_QuestHistory(pPacket->GetMissionHaveDoneFlags());

		// ����
		if( flagDetail.isSetBit(DETAIL_ATTRIB_COUNTRY) )
			pCharacterData->Set_Country(pPacket->GetCountry());
		
		// ��ģ��
		if( flagDetail.isSetBit(DETAIL_ATTRIB_FACEMODEL) )
			pCharacterData->Set_FaceMesh(pPacket->GetFaceModel());

		// ����ͷ��
	//laim	if( flagDetail.isSetBit(DETAIL_ATTRIB_MODELID) )
	//		pCharacterData->Set_PortraitID(pPacket->GetPortraitID());

		// �������
		if( flagDetail.isSetBit(DETAIL_ATTRIB_S_SKILLPOINT) || flagDetail.isSetBit(DETAIL_ATTRIB_SKILLPOINT_REMAIN) )
		{
			// ϵ����
			if( flagDetail.isSetBit(DETAIL_ATTRIB_S_SKILLPOINT) )
			{
				pCharacterData->Set_SeriesPoint(COMMON_S1, pPacket->GetS_SkillPoint(COMMON_S1));
				pCharacterData->Set_SeriesPoint(COMMON_S2, pPacket->GetS_SkillPoint(COMMON_S2));
				pCharacterData->Set_SeriesPoint(COMMON_S3, pPacket->GetS_SkillPoint(COMMON_S3));
			}

			// ���ܵ���
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

