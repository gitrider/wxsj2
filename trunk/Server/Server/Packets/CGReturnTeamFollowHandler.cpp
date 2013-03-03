/********************************************************************************
 *	�ļ�����	CGReturnTeamFollowHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\CGReturnTeamFollowHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	18:00
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "CGReturnTeamFollow.h"

#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "PacketFactoryManager.h"

#include "GCReturnTeamFollow.h"
#include "GCTeamFollowErr.h"
#include "GCTeamFollowList.h"
#include "GWEnterTeamFollow.h"

uint CGReturnTeamFollowHandler::Execute( CGReturnTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	
	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
	if( pTeamInfo->HasTeam() == FALSE )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	const TEAMMEMBER* pLeaderInfo = pTeamInfo->Leader();
	// ���ﲻ���Լ��Ƿ�ӳ����м��

	if( pLeaderInfo->m_SceneID != pScene->SceneID() )
	{ // ����ͬһ������
		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X is not in the same scene with team leader.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pLeader = pScene->GetHumanManager()->GetHuman( pLeaderInfo->m_ObjID );
	if( pLeader==NULL )
	{ // û���ҵ��ӳ�
		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't find team leader.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	// ����Լ����ڰ�̯�����������
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCTeamFollowErr Msg;
		Msg.SetError( TF_ERROR_STALL_OPEN );

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't follow when open stall.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	if( pPacket->GetReturn() )
	{ // ͬ�����
		// �����ӳ�֮��ľ����Ƿ񳬹� 10 ��
		if( pHuman->IsInValidRadius(pLeader, (FLOAT)g_Config.m_ConfigInfo.m_nAvailableFollowDist) == FALSE )
		{ // �����򷵻ش�����Ϣ
			GCTeamFollowErr Msg;
			Msg.SetError( TF_ERROR_TOO_FAR );

			pGamePlayer->SendPacket( &Msg );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't follow for the distance.", 
				pHuman->GetGUID() );
			return PACKET_EXE_CONTINUE;
		}

		// ���ӳ��Ƿ������Ӹ���״̬
		if( pLeader->__GetTeamFollowFlag() == FALSE )
		{ // �ӳ���������Ӹ���״̬�����ܶӳ��Ѿ�����ȡ��������
			GCTeamFollowErr Msg;
			Msg.SetError( TF_ERROR_NOT_IN_FOLLOW_MODE );

			pGamePlayer->SendPacket( &Msg );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't follow for not in follow mode.", 
				pHuman->GetGUID() );
			return PACKET_EXE_CONTINUE;
		}

		if ( pHuman->__GetTeamFollowFlag() )
		{ // ����Ѿ����ڴ�״̬���򷵻�
			return PACKET_EXE_CONTINUE;
		}

		pHuman->__SetTeamFollowFlag( TRUE );

		_FOLLOWEDMEMBER MyInfo;
		MyInfo.m_GUID = pHuman->GetGUID();
		MyInfo.m_pHuman = pHuman;

		// ���µĸ����б����ɼ���Χ�����пͻ���
		// ֻ�жӳ�������б��������Խ����Ż�
		GCTeamFollowList Msg;
		Msg.SetObjID( pLeader->GetID() );

		INT nMaxFollowedCount = pLeader->__GetFollowedMembersCount();

		for( INT i=0; i<nMaxFollowedCount; ++i )
		{ // ���и����߼����Լ����Լ�����ÿ��������
			Obj_Human* pMember;
			const _FOLLOWEDMEMBER* pFollowedMember;

			pFollowedMember = pLeader->__GetFollowedMember(i);

			pMember = pFollowedMember->m_pHuman;
			if( pMember!=NULL )
			{ // û�ж��߲�����ͬһ����
				pMember->__AddFollowedMember( MyInfo );
			}

			pHuman->__AddFollowedMember( *pFollowedMember );
			Msg.AddFollowMember( pFollowedMember->m_GUID );
		}

		pHuman->__AddFollowedMember( MyInfo ); // �Լ������Լ�
		pHuman->GetHumanAI()->PushCommand_TeamFollow();

		Msg.AddFollowMember( MyInfo.m_GUID );

		//GCNotifyTeamInfo notifyMsg;
		//notifyMsg.SetObjID( pHuman->GetID() );
		//notifyMsg.SetHaveTeamFlag( pTeamInfo->HasTeam() );
		//notifyMsg.SetTeamLeaderFlag( pTeamInfo->IsLeader() );
		//notifyMsg.SetTeamFullFlag( pTeamInfo->IsFull() );
		//notifyMsg.SetTeamFollowFlag( pHuman->__GetTeamFollowFlag() );
		//pScene->BroadCast( &notifyMsg, pHuman );

		pScene->BroadCast( &Msg, pLeader, TRUE ); // ���͸����б�����пͻ��ˣ��Թ�·��ģ��

		GCReturnTeamFollow RetMsg;
		RetMsg.SetReturn( TF_RESULT_ENTER_FOLLOW );
		RetMsg.SetGUID( pHuman->GetGUID() );

		pGamePlayer->SendPacket( &RetMsg ); // �����Լ�
		pLeader->GetPlayer()->SendPacket( &RetMsg ); // �����ӳ�

		// ������Ҫ֪ͨ world GWEnterTeamFollow
		GWEnterTeamFollow* pMsg = (GWEnterTeamFollow*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_ENTER_TEAMFOLLOW);

		pMsg->SetGUID( pHuman->GetGUID() );

		g_pServerManager->SendPacket( pMsg, INVALID_ID );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X success.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}
	else
	{ // ��ͬ�����
		GCReturnTeamFollow Msg;
		Msg.SetReturn( TF_RESULT_REFUSE_FOLLOW );
		Msg.SetGUID( pHuman->GetGUID() );

		pLeader->GetPlayer()->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X refuse to follow.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
