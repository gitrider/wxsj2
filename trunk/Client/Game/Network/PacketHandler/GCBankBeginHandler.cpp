#include"stdafx.h"
#include "..\..\Network\NetManager.h"
#include "GCBankBegin.h"
#include "CGBankAcquireList.h"

#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "GIException.h"

uint GCBankBeginHandler::Execute( GCBankBegin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//������NPC��ID���浽���ݳ�
			ObjID_t ObjServerId = pPacket->GetNPCObjID();
			INT ObjectID = (CObjectManager::GetMe()->FindServerObject(ObjServerId))->GetID();
			CDataPool::GetMe()->UserBank_SetNpcId(ObjectID);

			if( pPacket->IsBankPwd() )
			{
//				if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(FALSE))
				{
					// ������ù����룬 �͵����������롣
					
					CEventSystem::GetMe()->PushEvent( GE_OPEN_BANK_EVENT );	// GE_INPUT_PASSWORD_DLG
				}
			}
			else
			{
				CGBankAcquireList Msg;
				Msg.SetPwdSize( 0 );
				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
