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
	// ��ʱ�������Ŷ����
__ENTER_FUNCTION

	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);
		CTeamOrGroup* team = pDataPool->GetTeamOrGroup();
		TeamMemberInfo* pTMInfo = team->GetMember( pPacket->getGUID() );

		if( pTMInfo == NULL )
		{ // ����л�����ʱ�������������
			// KLAssert(FALSE);
			return PACKET_EXE_CONTINUE;
		}


		// ְҵ
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_FAMILY) )
		{
			pTMInfo->m_uFamily = pPacket->GetFamily();
		}

		// �ȼ�
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_LEVEL) )
		{
			pTMInfo->m_uLevel = pPacket->GetLevel();
		}

		// λ��
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

		// HP����
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_HP) )
		{
			pTMInfo->m_dwMaxHP = pPacket->GetMaxHP();
		}

		// MP
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MP) )
		{
			pTMInfo->m_nMP = pPacket->GetMP();
		}

		// MP����
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_MP) )
		{
			pTMInfo->m_dwMaxMP = pPacket->GetMaxMP();
		}

		// ŭ��ֵ
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_ANGER) )
		{
			pTMInfo->m_nAnger = pPacket->GetAnger();
		}

		// ��������
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_WEAPON) )
		{
			pTMInfo->m_WeaponID = pPacket->GetWeaponID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_MAINHAND, pTMInfo->m_WeaponID);
		}
		// ��������
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_WEAPON) )
		{
			pTMInfo->m_AssiID = pPacket->GetAssihandID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_ASSIHAND, pTMInfo->m_AssiID);
		}

		// ñ��
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_CAP) )
		{
			pTMInfo->m_CapID = pPacket->GetCapID();
			pTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_HEAD, pTMInfo->m_CapID );
		}

		// �·�
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_ARMOR) )
		{
			pTMInfo->m_ArmourID = pPacket->GetArmourID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_BACK, pTMInfo->m_ArmourID);
		}

		// ��
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_SCAPULAR) )
		{
			pTMInfo->m_ShoulderID = pPacket->GetScapularID();
			pTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_SHOULDER, pTMInfo->m_ShoulderID );
		}
		
		// ����
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_CUFF) )
		{
			pTMInfo->m_CuffID = pPacket->GetCuffID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_WRIST, pTMInfo->m_CuffID);
		}

		// ѥ��
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_BOOT) )
		{
			pTMInfo->m_FootID = pPacket->GetFootID();
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_FEET, pTMInfo->m_FootID);
		}



		// ����
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_DEADLINK) )
		{
			pTMInfo->m_bDeadLink = pPacket->IsDeadLink();
		}

		// ����
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_DEAD) )
		{
			pTMInfo->m_bDead = pPacket->IsDead();
		}



		// �沿ģ��
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_FACEMESH) )
		{
			pTMInfo->m_uFaceMeshID = pPacket->GetFaceModel();
			if(
			pTMInfo->m_uFaceMeshID < 255)
			{
				// ��������
				pTMInfo->m_UIModel.SetFaceMeshId(pTMInfo->m_uFaceMeshID);
			}
		}

		// ͷ��ģ��
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRMESH) )
		{
			pTMInfo->m_uHairMeshID = pPacket->GetHairModel();

			if(pTMInfo->m_uHairMeshID < 255)
			{
				// ���÷���
				pTMInfo->m_UIModel.SetFaceHairId(pTMInfo->m_uHairMeshID);
			}
		}

		// ͷ����ɫ
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRCOLOR) )
		{
			pTMInfo->m_uHairColor = pPacket->GetHairColor();
				
			//if(pTMInfo->m_uHairColor < 255)
			//{
			//	// ������ɫ
			//	pTMInfo->m_UIModel.SetHairColor(pTMInfo->m_uHairColor);
			//}
			//else
			//{
			//	// ������ɫ
			//	pTMInfo->m_UIModel.SetHairColor(0);
			//}//

			// ������ɫ
			pTMInfo->m_UIModel.SetHairColor(pTMInfo->m_uHairColor);
		}


		INT idx = team->GetMemberUIIndex( pTMInfo->m_GUID );
		// ���¶�Ա��Ϣ
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);

		for( idx =0; idx<team->GetTeamMemberCount(); ++idx )
		{
			if( team->GetMemberByIndex(idx)->m_GUID == pTMInfo->m_GUID )
			{
				// ˢ�¶�Ա��Ϣ
				CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_MEMBER, idx );
			}
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
