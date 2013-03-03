
#include "stdafx.h"
#include "GCExchangeError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "GIException.h"


uint GCExchangeErrorHandler::Execute( GCExchangeError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
			//不用强行取消本次交易的错误
			case EXCHANGE_MSG::ERR_ALREADY_LOCKED:
				{
					// "你已锁定交易，请重新操作"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_You_Are_Locked");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_EXROOM:
				{

					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Over_Exchange_Count");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过可交易物品的数量上限，请重新操作");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			//必须取消本次交易的错误
			case EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Doing_Exchange");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"自己正在交易中");
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Other_Doing_Exchange");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"对方正在交易中");
					break;
				}
			case EXCHANGE_MSG::ERR_DROP:
				{
					//STRING strTemp = "";
					//strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Other_Cancel_Exchange");
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"对方取消交易");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER:
				{
					// "对方空间不足"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Pag_Size_Not_Enough_Other");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF:
				{
					// "你的背包空间不够"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Pag_Size_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER:
				{
					// "对方金钱不足，请重新操作"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Money_Is_Not_Enough_Other");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足，请重新操作");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF:
				{
					// "你的金钱不足，请重新操作"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Money_Is_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_TOO_FAR:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Over_Exchange_Range");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过能够交易的范围");
					break;
				}

			case EXCHANGE_MSG::ERR_REFUSE_TRADE:
				{
					// "对方拒绝交易"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Refuse_To_Trade");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}

			case EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					// "骑乘级别过高,无法交易"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Pet_Level_Too_High");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			
			case EXCHANGE_MSG::ERR_SELF_DEAD_OR_OFFLINE:  // "自己已死亡不能申请交易" 
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Dead_Can_Not_Exchange");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				break;
			}
			case EXCHANGE_MSG::ERR_TARGET_DEAD_OR_OFFLINE: // "对方已下线或死亡，不能继续交易" 20100524LL
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_ExchangeObj_OffLine");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_IN_PROTECT_TIME: // "上线保护时间内不能交易"  20100524LL
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Exchange_In_ProtectTime");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_SELF_OPEN_STALL: // "你正在摆摊！"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMAI_Base_Info_You_Stalling" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_OPEN_STALL:  // "对方正在摆摊，不能交易"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_OtherStall" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_SELF_USE_SKILL:  // "你正在使用技能，不能交易"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_UseSkill" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_USE_SKILL:  // "对方正在使用技能，不能交易"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_Other_UseSkill" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_SELF_MOVE:  // "你正在行进中，不能交易"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_MOVE:  // "对方正在行进中，不能交易"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_Other_Run_CanNotExchange" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_ITEM_LOCKED:  // "此物品已经绑定"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMActionSystem_Item_Info_Item_Is_Binded" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}				
			case EXCHANGE_MSG::ERR_ILLEGAL:
			default:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Err_Status");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"非法状态，交易取消");
					break;
				}
			}

			//需要解除背包对应物品的锁定(注意，克隆的物品GUID相同，所以可以这样操作)
			for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyExBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyExBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//去除锁定
						pBagItem->SetLock(FALSE);
					}
				}
			}

			//一切清空
			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear();

			//更新Action
			CActionSystem::GetMe()->UserExchangeOther_Update();
			CActionSystem::GetMe()->UserExchangeSelf_Update();


			//更新背包物品（如果可能还需要更新装备栏物品，这个要看装备栏上的东西是不是可以用来交易）
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			//关闭交易界面
			CEventSystem::GetMe()->PushEvent(GE_SUCCEED_EXCHANGE_CLOSE);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
