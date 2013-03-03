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
			//将银行NPC的ID保存到数据池
			ObjID_t ObjServerId = pPacket->GetNPCObjID();
			INT ObjectID = (CObjectManager::GetMe()->FindServerObject(ObjServerId))->GetID();
			CDataPool::GetMe()->UserBank_SetNpcId(ObjectID);

			if( pPacket->IsBankPwd() )
			{
//				if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(FALSE))
				{
					// 如果设置过密码， 就弹出输入密码。
					
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
