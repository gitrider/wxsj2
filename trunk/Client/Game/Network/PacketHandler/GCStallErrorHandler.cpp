/*
服务器发送给客户端的错误消息
*/

#include "stdafx.h"
#include "GCStallError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"

#include "GIException.h"

uint GCStallErrorHandler::Execute( GCStallError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
				//不用强行取消本次交易的错误
			case STALL_MSG::ERR_OWNER_INVALID:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Target_Not_Exist");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"目标人物不存在");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NEED_NEW_COPY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Info_Changed_ReOpen");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摊位信息已改变,请重新打开");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Room_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"空间不足");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Money_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摊位已满");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_CLOSE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Owner_Not_Stall");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摊主已收摊");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_ALREADY_LOCK:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Can_Not_Sell");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不能出售此物品");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_pet_level_too_high");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
					break;
				}

				//必须取消本次交易的错误
			case STALL_MSG::ERR_NOT_ENOUGH_MONEY_TO_OPEN:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Moeny_Is_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足，无法摆摊");
					return PACKET_EXE_CONTINUE ;
					break;
				}
			case STALL_MSG::ERR_ILLEGAL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Err_Msg");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"非法消息");
					return PACKET_EXE_CONTINUE ;
					break;
				}
			default:
				{
				
				}
				break;
			}
			
			//解除货架上物品的锁定
			//需要解除背包对应物品的锁定(注意，克隆的物品GUID相同，所以可以这样操作)
			for(INT i=0; i<STALL_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyStallBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyStallBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//去除锁定
						pBagItem->SetLock(FALSE);
					}
				}
			}


			//清空目标摊位
			CDataPool::GetMe()->OtStallBox_Clear();
			CDataPool::GetMe()->MyStallBox_Clear();

			//ui刷新
			CActionSystem::GetMe()->UserStallSale_Update();
			CActionSystem::GetMe()->UserStallBuy_Update();
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
