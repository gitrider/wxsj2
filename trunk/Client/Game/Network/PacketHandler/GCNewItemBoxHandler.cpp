#include "StdAfx.h"
#include "GCNewItemBox.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\TripperObj\GMTripperObj_ItemBox.h"
#include "Object\Logic\TripperObj\GMTripperObj_Resource.h"
#include "World\WorldManager.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "GITimeSystem.h"
#include "..\..\Global.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GIException.h"

using namespace Packets;
uint GCNewItemBoxHandler::Execute(GCNewItemBox* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewItemBox::Execute");
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		
		//���λ���Ƿ�Ϸ�
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCNewItemBoxHandler");
		}

		if(ITYPE_DROPBOX == pPacket->getObjType())
		{
			BOOL bMustCreater = TRUE;
			CObject* pObj = (CObject*)(pObjectManager->FindServerObject( pPacket->getMonsterID() ));
			if ( pObj != NULL && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)) )
			{
				CObject_Character *pCharacter		= (CObject_Character*)pObj;
				if(!pCharacter->IsDie())
				{
					ObjID_t		idItemBox		= pPacket->getObjID();
					GUID_t		idOwner			= pPacket->getOwner();
					WORLD_POS	posCreate		= pPacket->getWorldPos();
					pCharacter->AddDropBoxEvent(idItemBox, idOwner, &posCreate);
					bMustCreater = FALSE;
				}
			}

			if(bMustCreater)
			{
				//����ItemBox
				CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", pPacket->getObjID());
				pBox->Initial(NULL);	
				//����λ��
				pBox->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
				//���õ�����Ĺ���
				pBox->SetOwnerGUID(pPacket->getOwner());
			}
		}
		else
		{
			//��������������
			CTripperObject_Resource* pResource = (CTripperObject_Resource*)CObjectManager::GetMe()->NewObject( "CTripperObject_Resource", pPacket->getObjID());
			if(!(pResource->SetResourceID(pPacket->getObjType())))
			{
				//�Ƿ�����ԴID
				CObjectManager::GetMe()->DestroyObject(pResource);
				return PACKET_EXE_CONTINUE;
			}
			pResource->Initial(NULL);	
			//����λ��
			pResource->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
		}
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}