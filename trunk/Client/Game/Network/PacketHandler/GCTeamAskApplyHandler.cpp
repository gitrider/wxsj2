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
		{ // 没有队伍或者不是队长的时候收到信息表示客户端数据出现问题
			KLAssert( FALSE );
			return PACKET_EXE_CONTINUE;
		}

		TeamCacheInfo* pProposerInfo = new(TeamCacheInfo);

		// 创建一个ui界面模型
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
			pProposerInfo->m_WeaponID = pPacket->GetWeaponID();		// 武器
			pProposerInfo->m_AssihandID = pPacket->GetAssihandID();

			pProposerInfo->m_CapID    = pPacket->GetCapID();		// 帽子
			pProposerInfo->m_ArmourID = pPacket->GetArmourID();		// 衣服
			pProposerInfo->m_ShoulderID = pPacket->GetScapularID();		// 衣服
			pProposerInfo->m_CuffID   = pPacket->GetCuffID();		// 护腕
			pProposerInfo->m_FootID   = pPacket->GetFootID();		// 靴子

			pProposerInfo->m_uFaceMeshID = pPacket->GetFaceModel();	// 面部模型
			pProposerInfo->m_uHairColor = pPacket->GetHairColor();	// 头发颜色
			pProposerInfo->m_uHairMeshID = pPacket->GetHairModel();	// 头发模型

			// 根据性别创建模型
			pProposerInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_MenPai( pProposerInfo->m_uFamily );
			pProposerInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_RaceID( pProposerInfo->m_uDataID );
			// 设置ui模型
			if( 0 != pProposerInfo->m_WeaponID )
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_MAINHAND,	pProposerInfo->m_WeaponID );	// 主手
			if( 0 != pProposerInfo->m_AssihandID )
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_ASSIHAND,	pProposerInfo->m_AssihandID );	// 副手
			// 以头为判断条件
			if( 0 != pProposerInfo->m_CapID )
			{
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_HEAD,		pProposerInfo->m_CapID );	// 头
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_BACK,		pProposerInfo->m_ArmourID );// 身体
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_SHOULDER,	pProposerInfo->m_ShoulderID );// 
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_WRIST,		pProposerInfo->m_CuffID );	// 手臂
				pProposerInfo->m_UIModel.SetUIModelInfo( HEQUIP_FEET,		pProposerInfo->m_FootID );	// 脚
			}

			if(pProposerInfo->m_uFaceMeshID < 255)
			{
				// 设置脸形
				pProposerInfo->m_UIModel.SetFaceMeshId(pProposerInfo->m_uFaceMeshID);
			}
				
			if(pProposerInfo->m_uHairMeshID < 255)
			{
				// 设置发型
				pProposerInfo->m_UIModel.SetFaceHairId(pProposerInfo->m_uHairMeshID);
			}
			
			//if(pProposerInfo->m_uHairColor < 255)
			//{
			//	// 设置颜色
			//	pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);
			//}
			//else
			//{
			//	// 设置颜色
			//	pProposerInfo->m_UIModel.SetHairColor(0);
			//}//

			// 设置颜色
			pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);

		}


		if ( pTeam->AddProposer(pProposerInfo) )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCTeamAskApplyhandler_Info_Someone_Apply_Team");
			ADDNEWDEBUGMSG(strTemp);
			//ADDNEWDEBUGMSG(STRING("有队员申请入队啦。"));
			CSoundSystemFMod::_PlayUISoundFunc(60);
		}

		// 通知界面有人申请加入队伍, 闪烁按钮.
//		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_NOTIFY_APPLY, 1);

		CDataPool::GetMe()->SystemTip_AddEventInfo( STT_TEAMINVITE, "有人申请加入队伍", 1 );
		// 更新界面显示
		CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );

	}



__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
