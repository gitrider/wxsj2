#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"

#include "GCTeamAskApply.h"



#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"

#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCTeamAskApplyHandler::Execute( GCTeamAskApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
	//	CUIDataPool::GetMe()->SetSourceGUID( pPacket->GetSourGUID() );
	//	CUIDataPool::GetMe()->SetDestinationGUID( pPacket->GetDestGUID() );
	//	CEventSystem::GetMe()->PushEvent(GE_TEAM_APPLY, pPacket->GetSourName());

		CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();

		if ( pTeam->HasTeam() == FALSE
		 ||  pTeam->GetLeaderGUID() != CObjectManager::GetMe()->GetMySelf()->GetServerGUID()
		 )
		{ // û�ж�����߲��Ƕӳ���ʱ���յ���Ϣ��ʾ�ͻ������ݳ�������
			KLAssert( FALSE );
			return PACKET_EXE_CONTINUE;
		}

		TeamCacheInfo* pProposerInfo = new(TeamCacheInfo);

		// ����һ��ui����ģ��
		pProposerInfo->m_UIModel.CreateNewUIModel();

		
		pProposerInfo->m_GUID = pPacket->GetSourGUID();
		strncpy(pProposerInfo->m_szNick, pPacket->GetSourName(), MAX_CHARACTER_NAME-1);
		pProposerInfo->m_szNick[MAX_CHARACTER_NAME-1] = '\0';
		pProposerInfo->m_uFamily     = pPacket->GetFamily();
		pProposerInfo->m_Scene       = pPacket->GetScene();
		pProposerInfo->m_uLevel      = pPacket->GetLevel();
		pProposerInfo->m_uDataID	 = pPacket->GetDataID();
		pProposerInfo->m_uDetailFlag = pPacket->GetDetailFlag();

		if ( pProposerInfo->m_uDetailFlag>0 )
		{
			pProposerInfo->m_WeaponID = pPacket->GetWeaponID();		// ����
			pProposerInfo->m_AssihandID = pPacket->GetAssihandID();

			pProposerInfo->m_CapID    = pPacket->GetCapID();		// ñ��
			pProposerInfo->m_ArmourID = pPacket->GetArmourID();		// �·�
			pProposerInfo->m_ShoulderID = pPacket->GetScapularID();		// �·�
			pProposerInfo->m_CuffID   = pPacket->GetCuffID();		// ����
			pProposerInfo->m_FootID   = pPacket->GetFootID();		// ѥ��

			pProposerInfo->m_uFaceMeshID = pPacket->GetFaceModel();	// �沿ģ��
			pProposerInfo->m_uHairColor = pPacket->GetHairColor();	// ͷ����ɫ
			pProposerInfo->m_uHairMeshID = pPacket->GetHairModel();	// ͷ��ģ��

			// �����Ա𴴽�ģ��
			pProposerInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_MenPai( pProposerInfo->m_uFamily );
			pProposerInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_RaceID( pProposerInfo->m_uDataID );
			// ����uiģ��
			if( 0 != pProposerInfo->m_WeaponID )
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_MAINHAND,	pProposerInfo->m_WeaponID );	// ����
			if( 0 != pProposerInfo->m_AssihandID )
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_ASSIHAND,	pProposerInfo->m_AssihandID );	// ����
			// ��ͷΪ�ж�����
			if( 0 != pProposerInfo->m_CapID )
			{
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_HEAD,		pProposerInfo->m_CapID );	// ͷ
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_BACK,		pProposerInfo->m_ArmourID );// ����
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_SHOULDER,	pProposerInfo->m_ShoulderID );// 
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_WRIST,		pProposerInfo->m_CuffID );	// �ֱ�
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_FEET,		pProposerInfo->m_FootID );	// ��
			}

			if(pProposerInfo->m_uFaceMeshID < 255)
			{
				// ��������
				pProposerInfo->m_UIModel.SetFaceMeshId(pProposerInfo->m_uFaceMeshID);
			}
				
			if(pProposerInfo->m_uHairMeshID < 255)
			{
				// ���÷���
				pProposerInfo->m_UIModel.SetFaceHairId(pProposerInfo->m_uHairMeshID);
			}
			
			//if(pProposerInfo->m_uHairColor < 255)
			//{
			//	// ������ɫ
			//	pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);
			//}
			//else
			//{
			//	// ������ɫ
			//	pProposerInfo->m_UIModel.SetHairColor(0);
			//}//

			// ������ɫ
			pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);

		}


		if ( pTeam->AddProposer(pProposerInfo) )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCTeamAskApplyhandler_Info_Someone_Apply_Team");
			ADDNEWDEBUGMSG(strTemp);
			//ADDNEWDEBUGMSG(STRING("�ж�Ա�����������"));
			CSoundSystemFMod::_PlayUISoundFunc(60);
		}

		// ֪ͨ������������������, ��˸��ť.
//		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_NOTIFY_APPLY, 1);

		CDataPool::GetMe()->SystemTip_AddEventInfo( STT_TEAMINVITE, "��������������", 1 );
		// ���½�����ʾ
		CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );

	}



__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
