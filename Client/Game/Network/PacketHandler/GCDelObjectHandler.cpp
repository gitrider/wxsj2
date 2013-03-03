
#include "StdAfx.h"
#include "GCDelObject.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GITimeSystem.h"
#include "Global.h"
#include "World\WorldManager.h"
#include "GIDBC_Struct.h"
#include "Object\GMObjectDestoryQueue.h"



uint GCDelObjectHandler :: Execute( GCDelObject* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	// 当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		// 判断是否是本场景
		if(CWorldManager::GetMe()->GetActiveDataSceneID() != pPacket->getSceneID())
		{
			AxTrace(0, 2, "DelObject:SceneID Error!(%d != %d)", 
				pPacket->getSceneID(), CWorldManager::GetMe()->GetActiveDataSceneID());
			return PACKET_EXE_CONTINUE;
		}

		// 根据id寻找obj
		INT objId = (INT)pPacket->getObjID();
		CObject_Character* pObj = (CObject_Character*)(pObjectManager->FindServerObject(objId));
		if( pObj == NULL )
		{
			return PACKET_EXE_CONTINUE;
		}

		// 清掉选择的主目标
		if( pObj == CObjectManager::GetMe()->GetMainTarget() )
		{
			CObjectManager::GetMe()->SetMainTarget(-1);
		}

		pObj->Release();
		CObjectManager::GetMe()->DestroyObject( pObj );

		// 之后不能执行任何代码
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

