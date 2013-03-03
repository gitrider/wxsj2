#include "StdAfx.h"

#include "GCPlayerShopError.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "Action\GMActionSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"

#include "GIException.h"
UINT GCPlayerShopErrorHandler::Execute( GCPlayerShopError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
				//不用强行取消本次交易的错误
			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_LEVEL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Create_Shop_Need_Level_30");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"30级后才可以建立自己的商店");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NAME_ALREADY_EXISTED:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Shop_Name_Exist");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"对不起，已经有同名店铺存在，请重新取名。");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY_TO_NEW:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Create_Shop_Money_Is_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你的金钱不够，筹些资金再来找我吧。");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_SHOP_IN_POOL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Create_Shop_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过系统店铺上限，不能再开店了");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Cur_Shop_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此柜台已满");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_LOCK:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Item_Is_Locked");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品已锁");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NEED_NEW_COPY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Data_Changed_ReOpen_Shop");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"数据已改变，请重新打开商店");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_pag_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"背包已满");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Moeny_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不够");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_HAVE_ENOUGH_SHOP:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Have_Two_Shop");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你已经拥有两家店铺了，留一点机会给别人吧");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_SELL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Buy_Shop_OK");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"购买商店成功");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_DESC:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_ModifyShopDesc_OK");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"修改商店描述成功");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_NAME:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_ModifyShopName_OK");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"修改商店名成功");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_STILL_STALL_OPEN:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_still_stall_open");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_serial_is_changed");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_BASE_MONEY_TOO_LOW:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_base_money_too_low");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_SALE_OUT:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_not_enough_money_to_saleout");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_BUY_BACK:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_not_enough_money_to_buy_back");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_CHANGE_NAME:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_not_enough_money_to_change_name");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_FULL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_partner_list_full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_ALREADY_IN_LIST:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_partner_already_in_list");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_WORLD:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_partner_not_find_in_world");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_INVALID:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_partner_invalid");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_EMPTY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_partner_list_empty");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_LIST:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_partner_not_find_in_list");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_ADD_TO_FAVORITE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_not_add_favor");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_DEL_FROM_FAVORITE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_not_del_favor");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;

				//必须取消本次交易的错误
			case PLAYERSHOP_MSG::ERR_ILLEGAL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Err_Msg");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"非法消息");
				}
			default:
				{

				}
				break;
			}

			//清空池中的商店数据
			CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
			CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
