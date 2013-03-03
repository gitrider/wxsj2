#include "StdAfx.h"
#include "GITimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\GameCommand.h"
#include "..\..\Global.h"
#include "Object\ObjectCommandDef.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"

#include "GCCharImpactListUpdate.h"
#include "..\..\Object\GMObjectLoadQueue.h"


uint GCCharImpactListUpdateHandler::Execute( GCCharImpactListUpdate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetOwnerID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		////如果需要更新资源，尝试放入缓冲队列
		//if(pObjectManager->GetLoadQueue()->TryAddLoadTask(pObj->GetID(), pPacket))
		//	return PACKET_EXE_NOTREMOVE;

		const GCCharImpactListUpdate::ImpactStruct_T *pImpactList = pPacket->GetImpactList();

		if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			CObject_Character *pCharacter = (CObject_Character*)pObj;
			pCharacter->RemoveAllImpact();
		}

		//SimpleImpactList ImpactList;
		SHORT i;

		// 更新效果
		for ( i = 0; i < pPacket->NumOfImpacts(); i++ )
		{
			SCommand_Object cmdTemp;

			memset( (void*)&cmdTemp, 0, sizeof(cmdTemp) );

			cmdTemp.m_wID			= OC_UPDATE_IMPACT;
			cmdTemp.m_adwParam[0]	= pImpactList[i].m_nBuffID;
			cmdTemp.m_adwParam[1]	= pImpactList[i].m_nSN;
			cmdTemp.m_anParam[2]	= pImpactList[i].m_nNumOfLayer;
			cmdTemp.m_anParam[3]	= pPacket->GetOwnerID();
			cmdTemp.m_abParam[4]	= TRUE;
			cmdTemp.m_anParam[5]    = pImpactList[i].m_nDelayTime;

			// 获取持续buff的倒计时时间，并转换为秒为单位	20100531 BLL
			cmdTemp.m_anParam[6]    = ( pImpactList[i].m_nTimeCount ) / 1000;
			
			pObj->PushCommand(&cmdTemp );

			//ImpactList.AddImpact(pImpactList[i].m_nBuffID);
		}			

		// 更新团队中玩家的buff
		/*CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();
		if ( pTeam != NULL )
		{
			pTeam->UpdateImpactsList( pPacket->GetOwnerID(), &ImpactList );
		}*/

		pObj->PushDebugString("GCCharImpactListUpdate");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
