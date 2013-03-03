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
				//����ǿ��ȡ�����ν��׵Ĵ���
			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_LEVEL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Create_Shop_Need_Level_30");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"30����ſ��Խ����Լ����̵�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NAME_ALREADY_EXISTED:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Shop_Name_Exist");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Բ����Ѿ���ͬ�����̴��ڣ�������ȡ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY_TO_NEW:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Create_Shop_Money_Is_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ľ�Ǯ��������Щ�ʽ��������Ұɡ�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_SHOP_IN_POOL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Create_Shop_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ϵͳ�������ޣ������ٿ�����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Cur_Shop_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�˹�̨����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_LOCK:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Item_Is_Locked");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NEED_NEW_COPY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Data_Changed_ReOpen_Shop");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����Ѹı䣬�����´��̵�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_pag_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��������");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Moeny_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_HAVE_ENOUGH_SHOP:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Have_Two_Shop");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���Ѿ�ӵ�����ҵ����ˣ���һ���������˰�");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_SELL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Buy_Shop_OK");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����̵�ɹ�");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_DESC:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_ModifyShopDesc_OK");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޸��̵������ɹ�");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_NAME:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_ModifyShopName_OK");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޸��̵����ɹ�");
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

				//����ȡ�����ν��׵Ĵ���
			case PLAYERSHOP_MSG::ERR_ILLEGAL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCPlayerShopErrorHandler_Info_Err_Msg");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ���Ϣ");
				}
			default:
				{

				}
				break;
			}

			//��ճ��е��̵�����
			CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
			CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
