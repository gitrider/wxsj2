#include "StdAfx.h"
#include "GITimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\Manager\ObjectManager.h"
#include "..\..\Object\Logic\Object.h"
#include "..\..\Object\Logic\Bus\Obj_Bus.h"
#include "..\..\Object\ObjectDef.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\NetWork\NetManager.h"
#include "..\..\World\WorldManager.h"
#include "GIException.h"
#include "..\..\Global.h"
//#include "TDDEbuger.h"
#include "..\..\Object\GMObjectLoadQueue.h"

#include "GCNewBus.h"

uint GCNewBusHandler :: Execute(GCNewBus* pPacket, Player* pPlayer) 
{
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->GetCurPos()->m_fX, pPacket->GetCurPos()->m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCNewBusHandler");
		}

		//创建玩家
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		CObject_Bus* pBus = (CObject_Bus*)(pObjectManager->FindServerObject((INT)pPacket->GetObjID()));
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans(
			tGfxSystem::AX_PLAN, fVector3(pPacket->GetCurPos()->m_fX, 0.0f, pPacket->GetCurPos()->m_fZ),
			tGfxSystem::AX_GAME, fvGame);

		FLOAT fFaceDir = pPacket->GetDir();

		if(pBus == NULL)
		{
			pBus = (CObject_Bus*)(pObjectManager->CObjectManager::GetMe()->NewObject( "CObject_Bus", (INT)pPacket->GetObjID()));

			SObject_BusInit tBusInit;
			tBusInit.m_fvPos	= fvGame;
			tBusInit.m_fvRot	= fVector3(0.f, fFaceDir, 0.f);
			tBusInit.m_nDataID	= pPacket->GetDataID();
			pBus->Initial(&tBusInit);

			fVector2 mapPos(fvGame.x, fvGame.z);
			pBus->SetMapPosition(mapPos);
		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			if(pBus->isVisible())
			{
				if(fabsf(pBus->GetPosition().x - fvGame.x) + fabsf(pBus->GetPosition().z - fvGame.z) > DEF_CHARACTER_POS_ADJUST_DIST)
				{
					pBus->SetMapPosition(mapPos);
				}
			}
			else
			{
				pBus->SetMapPosition(mapPos);
				pBus->SetFaceDir(fFaceDir);
				//pBus->setVisible(true);
			}
		}

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_BUS_ADD_PASSENGER;
		INT i;
		for(i = 0; i < pPacket->GetPassengerCount(); i++)
		{
			cmdTemp.m_anParam[0]	= i;
			cmdTemp.m_anParam[1]	= pPacket->GetPassengerIDByIndex(i);
			pBus->PushCommand(&cmdTemp );
		}

		//放入Ask队列
		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask((INT)(pPacket->GetObjID()));

		pBus->PushDebugString("GCNewBus");
	}

	return PACKET_EXE_CONTINUE ;
}

