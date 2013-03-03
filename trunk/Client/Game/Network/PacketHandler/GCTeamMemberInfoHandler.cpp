// GCTeamMemberInfoHandler.cpp

#include "StdAfx.h"
#include "GCTeamMemberInfo.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"


uint GCTeamMemberInfoHandler::Execute( GCTeamMemberInfo* pPacket, Player* pPlayer )
{ 
	// 暂时不考虑团队情况
__ENTER_FUNCTION

	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);
		CTeamOrGroup* team = pDataPool->GetTeamOrGroup();
		TeamMemberInfo* pTMInfo = team->GetMember( pPacket->getGUID() );

		if( pTMInfo == NULL )
		{ // 组队切换场景时会遇到这个问题
			// KLAssert(FALSE);
			return PACKET_EXE_CONTINUE;
		}


		// 职业
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_FAMILY) )
		{
			pTMInfo->m_uFamily = pPacket->GetFamily();
		}

		// 等级
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_LEVEL) )
		{
			pTMInfo->m_uLevel = pPacket->GetLevel();
		}

		// 位置
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_POSITION) )
		{
			CGameProcedure::m_bWaitNeedFreshMinimap = true;

			pTMInfo->m_WorldPos.m_fX = pPacket->GetWorldPos()->m_fX;
			pTMInfo->m_WorldPos.m_fZ = pPacket->GetWorldPos()->m_fZ;
		}

		// HP
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HP) )
		{
			pTMInfo->m_nHP = pPacket->GetHP();
		}

		// HP上限
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_HP) )
		{
			pTMInfo->m_dwMaxHP = pPacket->GetMaxHP();
		}

		// MP
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MP) )
		{
			pTMInfo->m_nMP = pPacket->GetMP();
		}

		// MP上限
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_MP) )
		{
			pTMInfo->m_dwMaxMP = pPacket->GetMaxMP();
		}

		// 怒气值
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_ANGER) )
		{
			pTMInfo->m_nAnger = pPacket->GetAnger();
		}

		// 主手武器
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_WEAPON) )
		{
			pTMInfo->m_WeaponID = pPacket->GetWeaponID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_MAINHAND, pTMInfo->m_WeaponID);
		}
		// 副手武器
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_WEAPON) )
		{
			pTMInfo->m_AssiID = pPacket->GetAssihandID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_ASSIHAND, pTMInfo->m_AssiID);
		}

		// 帽子
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_CAP) )
		{
			pTMInfo->m_CapID = pPacket->GetCapID();
			pTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_HEAD, pTMInfo->m_CapID );
		}

		// 衣服
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_ARMOR) )
		{
			pTMInfo->m_ArmourID = pPacket->GetArmourID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_BACK, pTMInfo->m_ArmourID);
		}

		// 肩
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_SCAPULAR) )
		{
			pTMInfo->m_ShoulderID = pPacket->GetScapularID();
			pTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_SHOULDER, pTMInfo->m_ShoulderID );
		}
		
		// 护腕
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_CUFF) )
		{
			pTMInfo->m_CuffID = pPacket->GetCuffID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_WRIST, pTMInfo->m_CuffID);
		}

		// 靴子
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_BOOT) )
		{
			pTMInfo->m_FootID = pPacket->GetFootID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_FEET, pTMInfo->m_FootID);
		}



		// 断线
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_DEADLINK) )
		{
			pTMInfo->m_bDeadLink = pPacket->IsDeadLink();
		}

		// 死亡
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_DEAD) )
		{
			pTMInfo->m_bDead = pPacket->IsDead();
		}



		// 面部模型
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_FACEMESH) )
		{
			pTMInfo->m_uFaceMeshID = pPacket->GetFaceModel();
			if(
			pTMInfo->m_uFaceMeshID < 255)
			{
				// 设置脸形
				pTMInfo->m_UIModel.SetFaceMeshId(pTMInfo->m_uFaceMeshID);
			}
		}

		// 头发模型
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRMESH) )
		{
			pTMInfo->m_uHairMeshID = pPacket->GetHairModel();

			if(pTMInfo->m_uHairMeshID < 255)
			{
				// 设置发型
				pTMInfo->m_UIModel.SetFaceHairId(pTMInfo->m_uHairMeshID);
			}
		}

		// 头发颜色
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRCOLOR) )
		{
			pTMInfo->m_uHairColor = pPacket->GetHairColor();
				
			//if(pTMInfo->m_uHairColor < 255)
			//{
			//	// 设置颜色
			//	pTMInfo->m_UIModel.SetHairColor(pTMInfo->m_uHairColor);
			//}
			//else
			//{
			//	// 设置颜色
			//	pTMInfo->m_UIModel.SetHairColor(0);
			//}//

			// 设置颜色
			pTMInfo->m_UIModel.SetHairColor(pTMInfo->m_uHairColor);
		}


		INT idx = team->GetMemberUIIndex( pTMInfo->m_GUID );
		// 更新队员信息
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);

		for( idx =0; idx<team->GetTeamMemberCount(); ++idx )
		{
			if( team->GetMemberByIndex(idx)->m_GUID == pTMInfo->m_GUID )
			{
				// 刷新队员信息
				CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_MEMBER, idx );
			}
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
