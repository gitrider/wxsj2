#include "StdAfx.h"
#include "GITimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "..\..\Object\GMObjectLoadQueue.h"
#include "GCCharBuff.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Object\Logic\Surface\Obj_Bullet.h"
#include "Global.h"
#include "SkillDataMgr.h"
#include "..\..\Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDataPool.h"


namespace Packets
{
	uint GCCharBuffHandler::Execute( GCCharBuff* pPacket, Player* pPlayer )
	{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetReceiverID() ));
			if ( pObj == NULL )
				return PACKET_EXE_CONTINUE;

			////如果需要更新资源，尝试放入缓冲队列
			//if(pObjectManager->GetLoadQueue()->TryAddLoadTask(pObj->GetID(), pPacket))
			//	return PACKET_EXE_NOTREMOVE;			

			SCommand_Object cmdTemp;

			cmdTemp.m_wID			= OC_UPDATE_IMPACT;
			cmdTemp.m_adwParam[0]	= pPacket->GetBuffID();
			cmdTemp.m_adwParam[1]	= pPacket->GetSN();
			cmdTemp.m_anParam[2]	= pPacket->GetLayerCount();
			cmdTemp.m_anParam[3]	= pPacket->GetSenderID();
			cmdTemp.m_abParam[4]	= pPacket->GetEnable();

			// 获取buff的延迟释放时间
			cmdTemp.m_anParam[5]	= pPacket->GetDelayTime();

			// 获取持续buff的倒计时时间，并转换为秒为单位	20100531 BLL
			if ( pPacket->GetContinuance() >= 0 )
			{
				cmdTemp.m_anParam[6] = pPacket->GetContinuance() / 1000;
			}
			else
				cmdTemp.m_anParam[6] = 0;


			pObj->PushCommand(&cmdTemp );

			// 更新团队中玩家的buff ，临时注销掉
			/*CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();
			if ( pTeam != NULL )
			{
				if ( pPacket->GetEnable() )
				{
					pTeam->AddImpact( pPacket->GetReceiverID(), pPacket->GetBuffID() );
				}
				else
				{
					pTeam->RemoveImpact( pPacket->GetReceiverID(), pPacket->GetBuffID() );
				}
			}*/

			pObj->PushDebugString("GCCharBuff");
			pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

			
		}		

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
	}
}
