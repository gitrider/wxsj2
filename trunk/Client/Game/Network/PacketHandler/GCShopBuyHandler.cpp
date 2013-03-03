#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "GCShopBuy.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCShopBuyHandler::Execute( GCShopBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		BYTE		bIsBuyOk	=	pPacket->GetBuyOk(); 
		UINT		ItemIndex	=	pPacket->GetIndex();
		BYTE		ItemNum		=	pPacket->GetNum();

		//已经关上了就算了
		if(CDataPool::GetMe()->Booth_IsClose())
			return PACKET_EXE_CONTINUE ;

		if(bIsBuyOk == GCShopBuy::BUY_MONEY_FAIL)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCShopBuyHandler_Info_Money_Is_Not_Enough");
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足.");
		}
		else if(bIsBuyOk == GCShopBuy::BUY_BAG_FULL)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCShopBuyHandler_Info_Pag_Is_Full");
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"包裹已满");
		}
		else if(bIsBuyOk == GCShopBuy::BUY_NO_MERCH)
		{
			
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCShopBuyHandler_Info_Pag_Shop_Data_Change_Reopen");
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"商品已改变，请重新打开");
		}
		else if(bIsBuyOk == GCShopBuy::BUY_OK)
		{
	        tObject_Item* pItem = CObject_Item::NewItem( ItemIndex );
			CHAR szMsg[256];
			
			STRING strTemp = "";
			strTemp = NOPARAMMSGFUNC("GCShopBuyHandler_Info_Shop_Buy");
			_snprintf(szMsg, 256, strTemp.c_str(),	ItemNum, pItem->GetName());
			//_snprintf(szMsg, 256, "您购买了%d个%s",	ItemNum, pItem->GetName());

			ADDTALKMSG(szMsg);
			CObject_Item::DestroyItem((CObject_Item*)pItem);
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"交易成功");
			CSoundSystemFMod::_PlayUISoundFunc(25+59);
		}

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
