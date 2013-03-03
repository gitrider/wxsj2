
#include "StdAfx.h"
#include "GMActionSystem_Item.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "DataPool\GMDataPool.h"
#include "DBC\GMDataBase.h"
#include "Procedure\GameProcedure.h"
#include "Interface\GMGameInterface.h"
#include "NetWork\NetManager.h"
#include "CGBankAddItem.h"
#include "CGBankRemoveItem.h"
#include "CGBankSwapItem.h"
#include "CGPlayerShopApply.h"
#include "ExchangeMsgDefine.h"
#include "CGExchangeSynchItemII.h"
#include "GIUISystem.h"
#include "Event\GMEventSystem.h"
#include "CGUnEquip.h"									// 使用卸下装备消息
#include "CGPackage_SwapItem.h"
#include "object\Logic\item\Obj_Item_Equip.h"
#include "object\Logic\item\Obj_Item_Gem.h"
#include "object\Logic\item\Obj_Item_Medicine.h"
#include "Object\Logic\Item\Obj_Item_SoulBead.h"
#include "Object\Logic\Item\Obj_Item_Mount.h"          //  [7/21/2010 陈军龙]

#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMDP_CharacterData.h"

#include "CGStallRemoveItem.h"

#include "CGDiscardEquip.h"
#include "CGDiscardItem.h"

#include "Sound\GMSoundSystem.h"

#include "DBC\GMDataBase.h"
#include "GameStruct_Skill.h"

#include "Input\GMInputSystem.h"
#include "CGItemSynch.h"
#include "CGAskItemInfo.h"
#include "CGUseItem.h"

#include "Interface\GMInterface_Script_Talk.h"

#include "GMActionSystem_MouseCmd.h"

#include "TransferItem\GMTransferItemSystem.h"
#include "..\Interface\GMGameInterface_Script_SouXia.h"




CActionItem_Item::CActionItem_Item(INT nID) : CActionItem(nID)
{
}

CActionItem_Item::~CActionItem_Item()
{
}


VOID CActionItem_Item::Update_Item(tObject_Item* pItem)
{
	KLAssert(pItem);

	//引用
	m_idItemImpl = pItem->GetID();
	//名称
	m_strName = pItem->GetName();
	//图标
	if ( pItem->GetIconName() != NULL )
		m_strIconName = pItem->GetIconName();
	//通知UI
	UpdateToRefrence();
}

tObject_Item* CActionItem_Item::GetItemImpl(VOID)
{
	return CObject_Item::FindItem(m_idItemImpl);
}

LPCTSTR CActionItem_Item::GetType_String(VOID)
{
	if(!GetItemImpl()) 
		return "";

	switch(GetItemImpl()->GetTypeOwner())
	{
	case tObject_Item::IO_ITEMBOX:	// 打开的宝箱中
		return NAMETYPE_LOOTITEM;

	case tObject_Item::IO_BOOTH:	// 商人的货架
		return NAMETYPE_BOOTITEM;

	case tObject_Item::IO_MYSELF_BANK:	// 玩家自己身上的银行中
		return NAMETYPE_BANKITEM;

	case tObject_Item::IO_MYSELF_EQUIP:	// 玩家自己身上的装备
		return NAMETYPE_EQUIP;	// 返回物品在装备栏上

	case tObject_Item::IO_MYEXBOX:		// 自己的交易盒
		return NAMETYPE_EXCHANGESELF;

	case tObject_Item::IO_OTHEREXBOX:	// 对方的交易盒
		return NAMETYPE_EXCHANGEOTHER;

	case tObject_Item::IO_MISSIONBOX:	// 任务递交盒
		return NAMETYPE_MISSIONREFER;

	case tObject_Item::IO_MYSTALLBOX:	// 自己的摊位盒
		return NAMETYPE_STALLSELF;

	case tObject_Item::IO_OTSTALLBOX:	// 对方的摊位盒
		return NAMETYPE_STALLOTHER;

	case tObject_Item::IO_QUESTVIRTUALITEM:	// 任务奖励的（虚拟）物品，只用于显示
		return NAMETYPE_VIRTUALITEM;

	case tObject_Item::IO_PLAYEROTHER_EQUIP:	// 其他玩家身上的装备
		return NAMETYPE_OTHEREQUIP;

	case tObject_Item::IO_PS_OTHERBOX:	// 
		return NAMETYPE_PS_OTHER;

	case tObject_Item::IO_PS_SELFBOX:	// 
		return NAMETYPE_PS_SELF;

	case tObject_Item::IO_ITEMDROP:		//掉落的物品
		return NAMETYPE_PS_DROP;

	case tObject_Item::IO_ITEMTRANSFER:		
		return NAMETYPE_TRANSFER_ITEM;
	
	// 20100428 AddCodeBgin
	case tObject_Item::IO_MYSELF_PACKET: // 玩家背包
		return NAMETYPE_PACKAGEITEM;

	case tObject_Item::IO_UNKNOWN: // 物品主未知
		return "";
	case tObject_Item::IO_ITEMIBSHOP: // IBShop
		return NAMETYPE_IB_SHOP;

	case tObject_Item::IO_MYSELF_MOUNT: //玩家的坐骑      [7/20/2010 陈军龙] 
		return NAMETYPE_MOUNTITEM;

	default:
		return "";
	// 20100428 AddCodeEnd		
	};


	// 20100428 DeleteCodeBegin
	// return NAMETYPE_PACKAGEITEM;
	// 20100428 DeleteCodeEnd

}

INT	CActionItem_Item::GetDefineID(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return -1;

	return pItem->GetIdTable();
}

INT CActionItem_Item::GetNum(VOID)
{
	tObject_Item* pItem = GetItemImpl();

	if(pItem) 
		return pItem->GetNumber();
	else
	{
		//AxTrace(0, 1, "InvalidItem:%d", m_idItemImpl);
		return -1;
	}
}
//得到描述信息
LPCTSTR CActionItem_Item::GetDesc(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return NULL;

	LPCTSTR szExtraInfo = pItem->GetExtraDesc();
	return szExtraInfo;

}

INT CActionItem_Item::GetPosIndex(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return -1;

	return pItem->GetPosIndex();
}

VOID CActionItem_Item::DoSubAction(VOID)
{

	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) return;

	if( InjectItemInfo(TRUE) ) return;

	//被包里的物品
	if(	pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
	{
		//按住Shift
		if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
		{
			//判定这个物品是否具备拆分的条件,数量多于一个可以拆分
			if(pItem->GetNumber() > 1)
			{
				//先将可能存在的已经锁定的物品解除锁定
				INT nCon = CDataPool::GetMe()->Split_GetItemConta();
				INT nPos = CDataPool::GetMe()->Split_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				default:
					break;
				}

				//记录拆分物品的位置到数据池
				CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->Split_SetItemPos(pItem->GetPosIndex());
				CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

				//打开拆分对话框
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);		

				// 锁定这个物品
				pItem->SetLock(TRUE);

				//通知背包锁定状态
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			}
			else
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Cannt_chaifen");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不可拆分");
			}
			return;
			
		}

		//先检测“Ctrl”是不是按下状态，如果是，那么先处理打星的操作   
		if( CInputSystem::GetMe()->IsKeyDown(KC_LCONTROL) || CInputSystem::GetMe()->IsKeyDown(KC_RCONTROL) )

		{
			if(pItem->GetItemClass() == ICLASS_EQUIP)
			{
				//先将可能存在的已经锁定的物品解除锁定
				INT nCon = CDataPool::GetMe()->AddStar_GetItemConta();
				INT nPos = CDataPool::GetMe()->AddStar_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家的装备
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nPos));

						if(pItem)
						{
							pItem->SetLock(FALSE);
								//通知装备锁定状态
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
						}
					}
					break;
				default:
					break;
				}

				//记录打星物品的位置到数据池
				CDataPool::GetMe()->AddStar_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->AddStar_SetItemPos(pItem->GetPosIndex());


				//打开打星对话框
				CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING);

				// 锁定这个物品
				pItem->SetLock(TRUE);

				//通知背包锁定状态
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				return;

			}else

			{
				STRING strTemp = "此物品不可以升星";

				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

			}
		}

		//商品界面是开的
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		{
			//在修理状态
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				//判以是不是装备,只有装备可以修理
				if(pItem->GetItemClass() == ICLASS_EQUIP)
				{
					if( pItem->GetItemDur() <	pItem->GetItemMaxDur() ) //测试用的 其实应该为Item->GetItemDur() < pItem->GetItemMaxDur()
					{
						//argo代表不是全修   //0表示修理单一装备
						 
						CDataPool::GetMe()->ComRepair_SetItemConta(1);//1代表背包 0代表装备
						CDataPool::GetMe()->ComRepair_SetItemPos(pItem->GetPosIndex());

					//	CEventSystem::GetMe()->PushEvent(GE_OPEN_REPAIR_SURE,0);  

						CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex());
					}
					else
					{
						//操作失败音效
						//CSoundSystemFMod::_PlayUISoundFunc(47);
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Not_Require_Repair");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					}
				}
				else
				{
					//操作失败音效
					//CSoundSystemFMod::_PlayUISoundFunc(47);

					STRING strTemp = "不是装备不能进行维修";

					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}

				return;
			}
		}

		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
		{
			if(pItem->GetItemClass() == ICLASS_EQUIP)
			{
				// 使用鉴定卷轴 
				UseIdentify();
				return; 
			}
			else
			{
				// 使用鉴定卷轴 
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("UI_OPERATE_IDENTIFY_EQUIP_ONLY");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}
		}
		
	}
	else if( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_EQUIP )		//玩家自己身上的装备
	{
		INT type = pItem->GetItemTableType();
		
		switch( type )
		{
		case EQUIP_SOUXIA:
			{
				CEventSystem::GetMe()->PushEvent(GE_OPEN_SOUXIA,1);
				//测试代码
				//CEventSystem::GetMe()->PushEvent( GE_UPDATE_SOUXIA_TAB, 1 );

			}
			break;
		}
	}
	
}

VOID CActionItem_Item::DoAction(VOID)
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) return;

	if(	InjectItemInfo(FALSE) ) return;

	//先检测“Ctrl”是不是按下状态，如果是，那么先处理打星的操作   
	if( CInputSystem::GetMe()->IsKeyDown(KC_LCONTROL) || CInputSystem::GetMe()->IsKeyDown(KC_RCONTROL) )

	{
		if( /*pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET || */pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_EQUIP)
			// 来自玩家的背包 CTRL+ 右键 身上 CTRL+ left

		{
			if(pItem->GetItemClass() == ICLASS_EQUIP)
			{
				//先将可能存在的已经锁定的物品解除锁定
				INT nCon = CDataPool::GetMe()->AddStar_GetItemConta();
				INT nPos = CDataPool::GetMe()->AddStar_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家的装备
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nPos));

						if(pItem)
						{
							pItem->SetLock(FALSE);
								//通知装备锁定状态
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
						}
					}
					break;
				default:
					break;
				}

				//记录打星物品的位置到数据池
				CDataPool::GetMe()->AddStar_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->AddStar_SetItemPos(pItem->GetPosIndex());


				//打开打星对话框
				CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING);

				// 锁定这个物品
				pItem->SetLock(TRUE);

				if ( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
					//通知背包锁定状态
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

				if ( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_EQUIP)
					//通知装备锁定状态
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


				return;

			}
			else
			{
				STRING strTemp = "此物品不可以升星";
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}

		}
	}

	switch(pItem->GetItemClass())
	{
	//case ICLASS_EQUIP:
	//	{
	//		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
	//		{
	//			// 使用鉴定卷轴 
	//			UseIdentify();
	//			return; 
	//		}
	//		break;
	//	}

	case ICLASS_IDENT:
		{
			int iType		= pItem->GetItemTableType();
			int iQuanlity	= 5;//pItem->GetItemTableQuality();

			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
			{
				//STRING strTemp = "";
				//strTemp = NOCOLORMSGFUNC("CActionItem_Item_Idenitfy_Not");
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				return; 
			}

			if(5 == iQuanlity)
			{
				if((iType >= COMITEM_WPIDENT)&&(iType <= COMITEM_NCIDENT))				
				{
					PrepareUseIdentify();
					return;
				}
				else
				{
					if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
					{
						// 使用鉴定卷轴 
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("UI_OPERATE_IDENTIFY_EQUIP_ONLY");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						return; 
					}
				}
			}
			break;
		}
	//case ICLASS_SOUXIA:
	//	{
	//		CEventSystem::GetMe()->PushEvent(GE_OPEN_SOUXIA);
	//		break;
	//	}
	default:
		{
			//if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
			//{
			//	// 使用鉴定卷轴 
			//	STRING strTemp = "";
			//	strTemp = NOCOLORMSGFUNC("UI_OPERATE_IDENTIFY_EQUIP_ONLY");
			//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//	return; 
			//}
			break;
		}

	}

	////先检测“Shift”是不是按下状态，如果是，那么先处理拆分的操作
	//if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
	//{
	//	//修改：目前银行是是DoAction实现拆分，背包使用DoSubAction进行拆分
	//	if( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_BANK )
	//	{
	//		//判定这个物品是否具备拆分的条件,数量多于一个可以拆分
	//		if(pItem->GetNumber() > 1)
	//		{
	//			//先将可能存在的已经锁定的物品解除锁定
	//			INT nCon = CDataPool::GetMe()->Split_GetItemConta();
	//			INT nPos = CDataPool::GetMe()->Split_GetItemPos();
	//			switch( nCon ) 
	//			{
	//			case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
	//				{
	//					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

	//					if(pItem)
	//					{
	//						pItem->SetLock(FALSE);
	//						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	//					}
	//				}
	//				break;
	//			case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
	//				{
	//					tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);

	//					if(pItem)
	//					{
	//						pItem->SetLock(FALSE);

	//					}
	//				}
	//				break;
	//			default:
	//				break;
	//			}

	//			//记录拆分物品的位置到数据池
	//			CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
	//			CDataPool::GetMe()->Split_SetItemPos(pItem->GetPosIndex());
	//			CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

	//			//打开拆分对话框
	//			CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);

	//			// 锁定这个物品
	//			pItem->SetLock(TRUE);

	//			//通知背包锁定状态
	//			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
	//		}
	//		else
	//		{
	//			STRING strTemp = "";
	//			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Cannt_chaifen");
	//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
	//			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不可拆分");
	//		}
	//	}
	//	return;
	//}

	STRING strTemp = "";

	//根据物品所在的归属
	switch(pItem->GetTypeOwner())
	{
		//玩家的摆摊界面（卖）
	case tObject_Item::IO_MYSTALLBOX:			//自己的摊位盒
		{
			//直接通知界面，自己选中的编号
			CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,this->GetPosIndex());
		}
		break;
		//玩家的摆摊界面（买）
	case tObject_Item::IO_OTSTALLBOX:			//对方的摊位盒
		{
			//直接通知界面，自己选中的编号
			CEventSystem::GetMe()->PushEvent(GE_STALL_BUY_SELECT,this->GetPosIndex());
		}
		break;
		//玩家交任务的对话框中
	case tObject_Item::IO_MISSIONBOX:
		{
			//解除锁定
			if(pItem)
			{
				INT nBagIndex = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(pItem->GetGUID());

				tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(nBagIndex);
				pBagItem->SetLock(FALSE);
			}

			//在任务BOX中的Item点击一次就让它消失
			CDataPool::GetMe()->MissionBox_SetItem(this->GetPosIndex(), NULL);

			CActionSystem::GetMe()->UserMission_Update();
			//发送更新消息
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
			//更新背包
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
		//玩家的银行租赁箱子中
	case tObject_Item::IO_MYSELF_BANK:
		{
			CGBankRemoveItem msg;
			msg.SetToType(CGBankRemoveItem::BAG_POS);
			msg.SetIndexFrom(this->GetPosIndex());
			msg.SetIndexTo(255);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
		//掉落箱中的物品
	case tObject_Item::IO_ITEMBOX:
		{
			//捡起
			if(pItem->GetItemClass() == ICLASS_TASKITEM)
			{
				INT idTable = pItem->GetIdTable();
				INT nCount = CDataPool::GetMe()->UserBag_CountItemByIDTable(idTable);

				DBC_DEFINEHANDLE(s_pItem_Task, DBC_ITEM_MEDIC);
				//搜索纪录
				const _DBC_ITEM_TASK* pTask = (const _DBC_ITEM_TASK*)s_pItem_Task->Search_Index_EQU(idTable);
				if( pTask->nMaxHold != INVALID_ID && nCount >= pTask->nMaxHold )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "不能拾取这样物品。");				
					break;
				}
			}
			CGameProcedure::s_pGameInterface->ItemBox_PickItem(pItem);
			break;
		}
		break;

		
		//玩家自己身上的包中
	case tObject_Item::IO_MYSELF_PACKET:
		{
			DoAction_Packet();
		}
		break;

	case tObject_Item::IO_BOOTH:
		// 商人的货架,发送交易请求
		{
			// 变成修理指针后，不能再购买商品
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_You_Can_Not_Repair_Item");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不能修理此物品");
				break;
			}

			if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= pItem->GetItemPrice())
			{
				CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
			}
			else
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Money_Is_Not_Enough");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不够");
			}
		}
		break;
	case tObject_Item::IO_MYEXBOX:
		// 交易界面的自己的物品筐里头
		{
			//发送拖动物品的消息
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEITEM);
			//msg.SetFromType(EXCHANGE_MSG::);
			msg.SetFromIndex(this->GetPosIndex());
			msg.SetToType(EXCHANGE_MSG::POS_BAG);
			msg.SetToIndex(0);						//由服务器查找
			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP :
		// 如果action item在装备栏上.
		{
			if(!CDataPool::GetMe()->Booth_IsClose())
			{
				//在修理状态
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
				{
					//判以下是不是装备,只有装备可以修理
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						if( pItem->GetItemDur() < pItem->GetItemMaxDur() )
						{
							
							 CDataPool::GetMe()->ComRepair_SetItemConta(0);//1代表背包 0代表装备
							 CDataPool::GetMe()->ComRepair_SetItemPos(pItem->GetPosIndex());


							// CEventSystem::GetMe()->PushEvent(GE_OPEN_REPAIR_SURE,0); 	//0表示修理单一装备

						 CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex(), FALSE);

						}
						else
						{
							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Not_Require_Repair");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不需要修理");
						}
					}else
					{
						STRING strTemp = "不是装备不能进行维修";
						 
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

					}

				}
			}
			else
			{
				CGUnEquip msg;
				CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
				if(pItemEquip->GetItemClass() != ICLASS_EQUIP)
				{
					return;
				}

				// 发送卸下装备消息.
				msg.setEquipPoint(this->GetPosIndex());
				msg.setBagIndex(-1);

				CNetManager::GetMe()->SendPacket( &msg );
			}
		}
		break;
	case tObject_Item::IO_PS_SELFBOX:
		{
			//选中的物品需要在进行换页操作后清除
			//玩家商店（自己的）
			// 记录当前选中的物品
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(this->GetPosIndex()%20);

			CEventSystem::GetMe()->PushEvent(GE_PS_SELF_SELECT);
		}
		break;
	case tObject_Item::IO_PS_OTHERBOX:
		{
			//玩家商店（别人的）
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(this->GetPosIndex()%20);
						
			CEventSystem::GetMe()->PushEvent(GE_PS_OTHER_SELECT);
		}
		break;
	case tObject_Item::IO_ITEMIBSHOP:
		// IB 商城 ,发送交易请求
		{
			if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= pItem->GetItemPrice())
			{
				CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
			}
			else
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Money_Is_Not_Enough");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不够");
			}
		}
		break;
	default:
		break;

	}

}

BOOL CActionItem_Item::InjectItemInfo(BOOL bSubAction)
{
	tObject_Item* pItem = GetItemImpl();
	if( !pItem ) return FALSE;

	if( CInputSystem::GetMe()->IsKeyDown(KC_LCONTROL) || CInputSystem::GetMe()->IsKeyDown(KC_RCONTROL) )
	{
		//仅支持窗口聊天
		BOOL bChatActive = CGameProcedure::s_pUISystem && 
			CGameProcedure::s_pUISystem->HasInputFocus();
		if( !bChatActive ) return FALSE;

		//该物品支持信息传送
		CTransferItemSystem* transfer = CTransferItemSystem::GetMe();
		tTransferItemSystem::Element* pElement = CTransferItemSystem::GetMe()->ConvertFromItem( pItem );
		if( !pElement ) return FALSE;

		//检查消息字串加上这个字符后是否长度超出（移到控件内部去做）
		//	传入UI系统
		if ( !CGameProcedure::s_pUISystem->InjectItemInfo(pElement))
		{
			CTransferItemSystem::GetMe()->DeleteElement( pElement->nID );
			return TRUE;
		} 
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CActionItem_Item::IsValidate()
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return FALSE;

	if( pItem->GetTypeOwner() == tObject_Item::IO_BOOTH )
	{
		return (pItem->GetMax() == 0)? FALSE : TRUE;
	}

	return TRUE;
}

VOID CActionItem_Item::Update(tActionReference* pRef)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return;

	if( pItem->GetTypeOwner() == tObject_Item::IO_BOOTH )
	{
		//刷新最大数量
		INT nMaxNumber = pItem->GetMax();
		if(nMaxNumber > 1)
		{
			pRef->Enable();
			CHAR szTemp[32];
			_snprintf(szTemp, 32, "%d", nMaxNumber);
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, szTemp);
		}
		else if(nMaxNumber == 0)
		{
			pRef->Disable();
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
		}
		else
		{
			pRef->Enable();
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
		}
	}
}

// 拖动结束
VOID CActionItem_Item::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	//拖动到表示销毁的地方
	if( bDestory )
	{
		DestoryItem(szSourceName);
		return;
	}
	//拖动到空白地方
	if(!szTargetName || szTargetName[0]=='\0' || GetItemImpl()==0)
	{
		return;
	}

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];
	INT nOldTargetId = -1;
	INT nIndex =-1;
	//如果是快捷栏，不能往其他地方拖，除了自己本身。
	if(  cSourceName == 'M' && 
		 cTargetType != 'M' )
		return;

	switch(cTargetType)
	{
	case 'S':		//摆摊界面
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//检查是不是由背包界面拖过来的(只能接受玩家背包内的物品)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
				{
					//任务物品
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "不能上架任务物品");
					break;
				}
				// 判定物品是否已经绑定
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
						break;
					}
				}

				// 摊位开市后才能上架 20100706 BLL
				if ( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_StallOpenPublic() == FALSE )
				{
					break;
				}

				// 将拖动的物品设为当前的物品
				CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
				
				//只有自己摆摊才弹出上加界面，修复其他玩家摆摊，买家托物品上加弹出界面的bug
				if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE )== CObject_Character::CHARACTER_LOGIC_STALL)	
				{
					// 弹出输入出售价格的对话框
					CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "price");
				}
			}
		}
		break;
	case 'Q':		//任务提交界面(MissionReply)
		{
			INT nIndex = szTargetName[1]-'0';

			//根据物品的来源决定发送什么消息（暂时只支持背包）
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					INT nGUID = this->GetItemImpl()->GetIdTable();
					//克隆物品
					tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
					((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

					//在这里保存直接通知界面添加按钮，不给服务器发消息，
					CDataPool::GetMe()->MissionBox_SetItem(GetPosIndex(), pNewItem);

					// 锁定背包中对应的这个物品
					tObject_Item* pMissionItem = this->GetItemImpl();
					pMissionItem->SetLock(TRUE);

					CActionSystem::GetMe()->UserMission_Update();
					//发送更新消息
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			default:
				break;
			}				
		}
		break;
	case 'B':		//银行
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//任务物品不能放入银行
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				return;
			}
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nIndex + this->GetCurBankRentBegin());
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//来自银行内部
				{
					CGBankSwapItem msg;
					msg.SetFromType(CGBankSwapItem::BANK_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nIndex + this->GetCurBankRentBegin());
					msg.SetToType(CGBankSwapItem::BANK_POS);
					if( msg.GetIndexFrom() == msg.GetIndexTo() )
						break;
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}

		}
		break;
	case 'F':		//商店
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			//任务物品不能放入商店
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				return;
			}
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					char strPrice[20] = "";
					sprintf( strPrice, "%d", GetItemPrice() );

					CEventSystem::GetMe()->PushEvent(GE_OPEN_SALE_CONFIRM, GetName(), strPrice, GetPosIndex(), GetNum());
				}
				break;
			}
		}
		break;

	case 'P':		//背包
		{
			INT nTargetIndex = atoi(szTargetName+1)-1;
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			
			switch(nTypeOwner)
			{
			// Shop -> Package
			case tObject_Item::IO_BOOTH:			//来自NPC商店
				{

					char strPrice[20] = "";

					if ( GetPosIndex() >= 200 ) // 如果是商店里的回购物品
					{
						sprintf( strPrice, "%d", GetItemPrice() );
					}
					else // 如果是商店里的出售物品
						sprintf( strPrice, "%d", GetItemBasePrice() );


					/*
					int num = GetNum();
					int pos = GetPosIndex();

					char str[20] = "";
					sprintf( str, "%d", GetName() );*/
					
					// 20100406 ModifyCodeBegin
					// CEventSystem::GetMe()->PushEvent(GE_OPEN_BUY_CONFIRM, GetName(), strPrice, GetPosIndex() );
					CEventSystem::GetMe()->PushEvent(GE_OPEN_BUY_CONFIRM, GetName(), strPrice, GetPosIndex(), GetNum() );
					// 20100406 ModifyCodeEnde
				}
				break;

			//Bank -> Package
			case tObject_Item::IO_MYSELF_BANK:
				{
					CGBankRemoveItem msg;
					msg.SetToType(CGBankRemoveItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_PACKET:
				{
					//同一格
					INT nSourcePos = this->GetPosIndex();
					if(nSourcePos == nTargetIndex) 
						break;

					//不同格
					CGPackage_SwapItem msg;
					msg.SetPackageIndex1(GetPosIndex());
					msg.SetPackageIndex2(nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			//SelfEquip -> package
			case tObject_Item::IO_MYSELF_EQUIP:
				{ 
					//发送卸下装备的消息
					CGUnEquip msg;
					msg.setEquipPoint(this->GetPosIndex());
					msg.setBagIndex((BYTE)nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
					break;
				}
			//playerShop -> package
			case tObject_Item::IO_PS_SELFBOX:
				{
					_ITEM_GUID Guid;
					tObject_Item::ITEM_GUID temp;
					temp.m_idUnion = 0;

					tObject_Item* pSelectItem = this->GetItemImpl();
					if(NULL == pSelectItem)
					{
						return;
					}
					
					//需要先判定这个物品是否满足移动的条件
					//查询这个物品是不是处于上架的状态
					INT nIndex = pSelectItem->GetPosIndex();
					INT nConIndex = nIndex/20;
					INT nPosition = nIndex%20;
					if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
					{
						//已经上架，不能取回
						return;
					}

					pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
					Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
					Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
					Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

					CGItemSynch msg;
					msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
					msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
					msg.SetToIndex(nTargetIndex);
					msg.SetToType(CGItemSynch::POS_BAG);
					msg.SetItemGUID(Guid);

					INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
					CGManuMoveItemFromPlayerShopToBag_t ExtraDataInfo;
					ExtraDataInfo.m_ShopGuid	= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
					ExtraDataInfo.m_nStallIndex = (BYTE)nPage;
					ExtraDataInfo.m_uSerial		= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,this->GetPosIndex()%20);
					msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
					msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);
				
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}



		}
		break;
	case 'E':		//交易界面,只能拖动到自己的（右边的）格子里头（1~6）
		{
			INT nIndex = szTargetName[1]-'0';

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch(nTypeOwner)
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					// 判定物品是否已经绑定
					tObject_Item* pItem = this->GetItemImpl();
					if(pItem)
					{
						if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
						{
							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
							break;
						}
					}

					//验证是否可以交易(该值从物品表中获取)
					if(!this->GetItemImpl()->Rule(tObject_Item::RULE_EXCHANGE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, this->GetItemImpl()->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
						break;
					}

					//检测交易栏是不是有空位
					if( !CDataPool::GetMe()->MyExBox_IsEmpty() )
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Exchange_Box_Is_Full");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"交易栏已经满了");
						break;
					}

					//发送拖动物品的消息
					CGExchangeSynchItemII msg;
					msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
					msg.SetFromType(EXCHANGE_MSG::POS_BAG);
					msg.SetFromIndex(this->GetPosIndex());
					msg.SetToIndex(0);					//由服务器查找
					
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}
			
		}
		break;
	case 'R':		//银行的界面上的租赁箱格子
		{
			INT nIndex = szTargetName[1]-'0';

			//目标箱子如果已经满了，就给个提示，不在发这个消息
			if(FALSE == CDataPool::GetMe()->UserBank_IsEmpty(nIndex))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Target_Box_Is_Full");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"目标箱子已经满了");
				break;
			}

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//任务物品不能放入银行
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				break;
			}

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//玩家银行
				{
					CGBankSwapItem msg;
					msg.SetFromType(CGBankSwapItem::BANK_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					msg.SetToType(CGBankSwapItem::BANK_POS);

					//如果来自同一个租赁箱子，那么就不发消息了
					if( CActionSystem::GetMe()->GetCurBankRentBoxIndex() == nIndex )
					{
						break;
					}
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}

		}
		break;
	case 'G':		//宝石合成/镶嵌界面
		{
			INT nIndex = szTargetName[1]-'0';

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					if(GetItemImpl()->GetItemClass() == ICLASS_GEM)    //remove
					{
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, nIndex, GetItemImpl()->GetPosIndex());
					}
					else
					{
						STRING strTemp = "需要宝石进行镶嵌";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					}
				}
				break;

			default:
				break;
			}

		}
		break;
	case 'T':		// 骑乘宠物技能学习 /喂养 /驯养 /还童 /延长寿命 /宠物征友
		{
			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					tObject_Item *pObj = this->GetItemImpl();
					if(pObj && !(pObj->IsLocked()) && pObj->GetItemClass() == ICLASS_COMITEM)
					{
						CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;

						if (pMedicine_Item->GetItemTableType() == COMMON_HORSE_ITEM)
						{
							// 索引坐骑物品表
							DBC_DEFINEHANDLE(s_pMountItemDataDBC, DBC_MOUNT_ITEM_DATA);
							const _DBC_MOUNT_ITEM_DATA* pMIDBC = 
								(const _DBC_MOUNT_ITEM_DATA*)(s_pMountItemDataDBC->Search_Index_EQU( pMedicine_Item->GetIdTable() ));
							if (NULL == pMIDBC)
								break;

							switch (szTargetName[1])
							{
							case 'S': // 技能书 / 打孔书 / 遗忘书
								{
									if (pMIDBC->nType == HORSE_ITEM_SKILL_APPER ||
										pMIDBC->nType == HORSE_ITEM_SKILL_STUDY ||
										pMIDBC->nType == HORSE_ITEM_SKILL_GIRD	||
										pMIDBC->nType == HORSE_ITEM_SKILL_FORGET)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_STUDY_ITEM_UPDATE, this->GetID());
									}									
								}
								break;
							case 'B': // 根骨书	
								{
									if (pMIDBC->nType == HORSE_ITEM_BASIC)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_FEED_ITEM_UPDATE, 0, this->GetID());
									}
								}
								break;
							case 'V': // 悟性书
								{
									if (pMIDBC->nType == HORSE_ITEM_SAVVY)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_FEED_ITEM_UPDATE, 1, this->GetID());
									}
								}
								break;
							case 'T': // 驯养道具
								{
									if (pMIDBC->nType == HORSE_ITEM_HAPPINESS)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_TRAIN_ITEM_UPDATE, this->GetID());
									}
								}
								break;
							case 'C': // 还童道具
								{
									if (pMIDBC->nType == HORSE_ITEM_CHILD)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_CHILD_ITEM_UPDATE, this->GetID());
									}
								}
								break;
							default:
								break;
							}
						}
						else
						{
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "不是该坐骑的技能");

							/*STRING strTemp = COLORMSGFUNC("GMActionSystem_Item_CantUseInPetSkillStudy");
							ADDNEWDEBUGMSG(strTemp);*/
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;


	case  'C': //打孔
		{

	
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自背包

				{   

				 
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;

					 
					//判定类型
					if(pItem->GetItemClass() == ICLASS_EQUIP)

					{
						tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->AddHole_GetItemPos());

						if (pItemOld)
						{
							pItemOld->SetLock(false);
								
						}

						CDataPool::GetMe()->AddHole_SetItemPos(pItem->GetPosIndex());
						CDataPool::GetMe()->AddHole_SetItemConta(tObject_Item::IO_MYSELF_PACKET);

						CEventSystem::GetMe()->PushEvent(GE_UPDATE_ADD_HOLE, pItem->GetID());
						pItem->SetLock(true);
						//通知背包锁定状态
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

						 
					}
					else
					{ 
							STRING strTemp = "此物品无法打孔，只有装备可以打孔";
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					}
	
				}

			break;

			case tObject_Item::IO_MYSELF_EQUIP:				//来自自己装备装备
				{

					
					CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
					if(!pItem) return;

					tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem( HUMAN_EQUIP (CDataPool::GetMe()->AddHole_GetItemPos()));

					if (pItemOld)
					{
						pItemOld->SetLock(false);
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
					}


					CDataPool::GetMe()->AddHole_SetItemPos(pItem->GetPosIndex());
					CDataPool::GetMe()->AddHole_SetItemConta(tObject_Item::IO_MYSELF_EQUIP);
					
                 
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_ADD_HOLE,pItem->GetID());
					pItem->SetLock(true);
					//通知装备锁定状态
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
				}

				break;
			}
		}
		break;
 
	case 'D': //绑定
		{
			INT nIndex = szTargetName[1]-'0';


			if( 1 == nIndex)  // 1表示装备    2表示道具
			{
					
				
				
				  INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

					switch( nTypeOwner )
					{
					case tObject_Item::IO_MYSELF_PACKET:			//来自背包
						{


							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

						
							//判定类型
							if(pItem->GetItemClass() == ICLASS_EQUIP)

							{

								tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->Bind_GetItemPos());

								if (pItemOld)
								{
									pItemOld->SetLock(false);
									
								}

								CDataPool::GetMe()->Bind_SetItemPos(pItem->GetPosIndex());
								CDataPool::GetMe()->Bind_SetItemConta(tObject_Item::IO_MYSELF_PACKET);

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANG_DING, pItem->GetID(), nIndex);
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


							}else  
							{ 
								STRING strTemp = "只有装备才能进行绑定";
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							}

						}

						break;
					case tObject_Item::IO_MYSELF_EQUIP:				//来自自己装备装备
						{
						

							CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
							if(!pItem) return;


							tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->Bind_GetItemPos());

							if (pItemOld)
							{
								pItemOld->SetLock(false);
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

							}


							CDataPool::GetMe()->Bind_SetItemPos(pItem->GetPosIndex());
							CDataPool::GetMe()->Bind_SetItemConta(tObject_Item::IO_MYSELF_EQUIP);

							CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANG_DING,pItem->GetID(),nIndex);
							pItem->SetLock(true);
								//通知装备锁定状态
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

						}

						break;

					}


				} else if ( 2 == nIndex)
				{

					//检查是不是由背包界面拖过来的(只能接受玩家背包内的物品)
					if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)

					{

						CObject_Item* pItem = (CObject_Item*)GetItemImpl();
						if(!pItem) return;


						////打开打孔表 Equip_bind.tab  
						//DBC_DEFINEHANDLE(s_Bang_Ding, DBC_EQUIP_BINDING_INFO);
						////搜索纪录
						//const _DBC_EQUIP_BINDING_INFO* p_Bang_Ding = (const _DBC_EQUIP_BINDING_INFO*)s_Bang_Ding->Search_Index_EQU((UINT) 1 );
						//if(!p_Bang_Ding) return  ;

						//if( pItem->GetIdTable() == p_Bang_Ding->nGemNeed)
						//{

							tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->Bind_GetNeedItemPos());

							if (pItemOld)
							{
								pItemOld->SetLock(false);

							}

						//	CDataPool::GetMe()->Bind_SetNeedItemId(p_Bang_Ding->nGemNeed);
							CDataPool::GetMe()->Bind_SetNeedItemPos(pItem->GetPosIndex());
				
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANG_DING, pItem->GetID(),nIndex);
							pItem->SetLock(true);
							//通知背包锁定状态
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


						//}else

						//{
						//	STRING strTemp = "此道具不是此次绑定需要的道具";
						//	CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

						//}

					}
				}
					
		}

		break;


	case 'V':  //装备特修
		{
		

			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

				switch( nTypeOwner )
				{
				case tObject_Item::IO_MYSELF_PACKET:			//来自背包
					{

						CObject_Item* pItem = (CObject_Item*)GetItemImpl();
						if(!pItem) return;

						//判定类型
						if(pItem->GetItemClass() == ICLASS_EQUIP)
						{

							tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->SpRepair_GetItemPos());
							if (pItemOld)
							{
								pItemOld->SetLock(false);

							}


							DBC_DEFINEHANDLE(s_SP_Repair, DBC_EQUIP_SP_REPAIR);
							//搜索纪录
							const _DBC_EQUIP_SP_REPAIR* p_SP_Repair = (const _DBC_EQUIP_SP_REPAIR*)s_SP_Repair->Search_Index_EQU((UINT)11000006 );
							if(!p_SP_Repair) return ;

							CDataPool::GetMe()->SpRepair_SetNeedItemId(p_SP_Repair->nNeedItem);
							CDataPool::GetMe()->SpRepair_SetIntensifyInfo(p_SP_Repair->nIntensifyInfo);
						
							CDataPool::GetMe()->SpRepair_SetItemPos(pItem->GetPosIndex());
							CDataPool::GetMe()->SpRepair_SetItemConta(tObject_Item::IO_MYSELF_PACKET);

							CEventSystem::GetMe()->PushEvent(GE_UPDATE_SP_REPAIR, pItem->GetID());
							pItem->SetLock(true);
							//通知背包锁定状态
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


						}else  
						{ 
							STRING strTemp = "只有装备才能进行特修";
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						}

					}

					break;
				case tObject_Item::IO_MYSELF_EQUIP:				//来自自己装备装备
					{


						CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
						if(!pItem) return;


						tObject_Item *pItemOld =  CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (CDataPool::GetMe()->SpRepair_GetItemPos()));

						if (pItemOld)
						{
							pItemOld->SetLock(false);
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							 

						}
						CDataPool::GetMe()->SpRepair_CleanUP();

						DBC_DEFINEHANDLE(s_SP_Repair, DBC_EQUIP_SP_REPAIR);
						//搜索纪录
						const _DBC_EQUIP_SP_REPAIR* p_SP_Repair = (const _DBC_EQUIP_SP_REPAIR*)s_SP_Repair->Search_Index_EQU((UINT)11000006 );
						if(!p_SP_Repair) return ;

						CDataPool::GetMe()->SpRepair_SetNeedItemId(p_SP_Repair->nNeedItem);
						CDataPool::GetMe()->SpRepair_SetIntensifyInfo(p_SP_Repair->nIntensifyInfo);

						CDataPool::GetMe()->SpRepair_SetItemPos(pItem->GetPosIndex());
						CDataPool::GetMe()->SpRepair_SetItemConta(tObject_Item::IO_MYSELF_EQUIP);

						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SP_REPAIR,pItem->GetID());
						pItem->SetLock(true);
							//通知装备锁定状态
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

					}

					break;

				}



		}
		break;

	case  'X':   //装备的拆解
		{

			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自背包
				{

					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;

					//判定类型
					if(pItem->GetItemClass() == ICLASS_EQUIP )
					{

						tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->Dismantle_GetItemPos());
						if (pItemOld)
						{
							pItemOld->SetLock(false);
							 

						}
						CDataPool::GetMe()->Dismantle_CleanUP();

						CDataPool::GetMe()->Dismantle_SetItemPos(pItem->GetPosIndex());
						CDataPool::GetMe()->Dismantle_SetItemConta(tObject_Item::IO_MYSELF_PACKET);

						CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
						pItem->SetLock(true);
						//通知背包锁定状态
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
 
					}else  

					{ 
						STRING strTemp = "道具不能进行拆解";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					}


				}

			break;

			case tObject_Item::IO_MYSELF_EQUIP:				//来自自己装备装备
				{
					CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
					if(!pItem) return;


					tObject_Item *pItemOld =  CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (CDataPool::GetMe()->Dismantle_GetItemPos() ));

					if (pItemOld)
					{
						pItemOld->SetLock(false);
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


					}
					CDataPool::GetMe()->Dismantle_CleanUP();

					CDataPool::GetMe()->Dismantle_SetItemPos(pItem->GetPosIndex());
					CDataPool::GetMe()->Dismantle_SetItemConta(tObject_Item::IO_MYSELF_EQUIP);

					CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS,pItem->GetID());
					pItem->SetLock(true);
						//通知装备锁定状态
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


				}

				break;

			}


	}
		break;


	case 'W':			//装备的炼化
		{
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自背包
				{
					CHAR cTargetType2 = szTargetName[1];
					switch( cTargetType2 )
					{
					case '1':		//炼化装备存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//判定类型
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if ( pItem->GetItemTableType() == EQUIP_SOUXIA )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "搜侠录不能在此炼化");
									break;
								}

								if ( pItem->GetNeedLevel() < 40 )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "你的装备等级太低，40级以上才可以炼化");
									break;
								}

								if ( ((CObject_Item_Equip*)pItem)->GetStartLevel()<5 )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "你的装备星级太低，5星以上才可以炼化");
									break;
								}

								if( ( (CObject_Item_Equip*)pItem)->GetGemCount() < 1 )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "你的装备都没有镶嵌过宝石，炼化有什么意义，别来丢人了");
									break;
								}

								INT pos = pItem->GetPosIndex();		//获得在背包中的位置
								CEventSystem::GetMe()->PushEvent(GE_ADD_LIANHUAEQUIP_ITEM, pos, GetID() );
								
								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有装备才可以安放");

						}
						break;

					case '2':		//装备炼化道具存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//判断类型
							if ( pItem->GetItemClass() == ICLASS_COMITEM )
							{
								if ( pItem->GetItemTableType() == COMITEM_EQUIP_REFINING )
								{
									INT pos = pItem->GetPosIndex();		//获得在背包中的位置
									CEventSystem::GetMe()->PushEvent(GE_ADD_LIANHUAEQUIP_DIS, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//通知背包锁定状态
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
									break;
								}
							}
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有炼器神符才可以安放");
							
						}
						break;

					case '3':		//附魔界面内的装备存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//判定类型
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if ( pItem->GetItemTableType() == EQUIP_CAP ||		//头盔
									pItem->GetItemTableType() == EQUIP_SHOULDER	||	//护肩
									pItem->GetItemTableType() == EQUIP_ARMOR	||		//衣服
									pItem->GetItemTableType() == EQUIP_HAND	||			//手套
									pItem->GetItemTableType() == EQUIP_BOOT		//鞋
									)	
								{
									INT pos = pItem->GetPosIndex();		//获得在背包中的位置
									CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_FUMO_ITEM, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//通知背包锁定状态
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
								}
								else
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "此类装备不可以附魔");		

							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有装备才可以附魔");
						}
						break;

					case '4':		//附魔界面 附魔道具 存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//判定类型
							if(pItem->GetItemClass() == ICLASS_SOUL_BEAD )
							{
									INT pos = pItem->GetPosIndex();		//获得在背包中的位置
									CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_FUMO_DIS, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//通知背包锁定状态
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "不是魂珠不可以安放");
						}
						break;

					case '5':		//退魔界面 装备存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//判定类型
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if( ( (CObject_Item_Equip*)pItem )->GetIsFumo() )
								{
									INT pos = pItem->GetPosIndex();		//获得在背包中的位置
									CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_TUIMO_ITEM, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//通知背包锁定状态
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
								}
								else
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "没有附魔的装备不可以退魔");

							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "此物品不可以退魔");
						}
						break;

					case '6':		//退魔界面 退魔石放置按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//判定类型
							if((pItem->GetItemClass() == ICLASS_COMITEM) && (pItem->GetItemTableType() == COMITEM_EQUIP_CANCEL_MAGIC))
							{
								INT pos = pItem->GetPosIndex();		//获得在背包中的位置
								CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_TUIMO_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "不是退魔石不可以安放");
						}
						break;
					}

				}
				break;
			}
		}
		break;
	
	case 'O':			//搜侠录 相关功能界面
		{
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自背包
				{
					CHAR cTargetType2 = szTargetName[1];
					switch( cTargetType2 )
					{
					case '1':		//搜侠录道具存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//判定类型
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if ( pItem->GetItemTableType() == EQUIP_SOUXIA )
								{
									INT pos = pItem->GetPosIndex();		//获得在背包中的位置
									CHAR cTargetType3 = szTargetName[2];
									BOOL IsBlankDis = false;		//在拆解界面中，判断搜侠录是否为空白
									switch ( cTargetType3 )
									{
									case '1':		//在拆解搜侠录界面中放置搜侠录道具
										{
											IsBlankDis = SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsBlankSouXiaItem(pos);
											if( !IsBlankDis )
												CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIADISMANTLE_ITEM, pos, GetID() );
											else
												CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "空白搜侠录不可以安放!");
										}
										break;
									case '2':		//在销毁搜侠录界面中放置搜侠录道具
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIADESTORY_ITEM, pos, GetID() );
										}
										break;
									case '3':		//在炼化搜侠录界面中放置搜侠录道具1
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIACOMPLEX_ITEM1, pos, GetID() );
										}
										break;
									case '4':		//在炼化搜侠录界面中放置搜侠录道具2
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIACOMPLEX_ITEM2, pos, GetID() );
										}
										break;
									case '5':		//在缝合搜侠录界面中放置搜侠录道具
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIASEW_ITEM1, pos, GetID() );
										}
										break;
									}
									if ( !IsBlankDis )
									{
										CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
										pItem->SetLock(true);
										//通知背包锁定状态
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
									}
									
								}
								else
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有搜侠录道具才可以安放");
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有搜侠录道具才可以安放");
						}
						break;

					case '2':		//搜侠录拆解道具存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaItem( pItem->GetPosIndex(), 2) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//获得在背包中的位置
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIADISMANTLE_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有搜侠录拆解道具才可以安放");
						}
						break;

					case '3':		//搜侠录炼化道具存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaItem( pItem->GetPosIndex(), 3) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//获得在背包中的位置
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIACOMPLEX_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有搜侠录炼化道具才可以安放");
						}
						break;

					case '4':		//搜侠录缝合道具存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaItem( pItem->GetPosIndex(), 4) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//获得在背包中的位置
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIASEW_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有搜侠录炼缝合道具才可以安放");
						}
						break;

					case '5':		//搜侠录缝合道具存放按钮
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiadeformityItem( pItem->GetPosIndex()) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//获得在背包中的位置
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIASEW_DISITEM, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "只有搜侠录残页才可以安放");
						}
						break;
					}
				}
				break;
			}
		}
		break;
   
		// 装备的牌型调换
	case  'H':    
		{

			INT nIndex = szTargetName[1]-'0';
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
			if(!pItem) return;


			if( 1 == nIndex)  // 1表示装备    2表示另一个装备 3表示道具
			{	

					switch( nTypeOwner )
					{
					  case tObject_Item::IO_MYSELF_PACKET:			//来自背包
						 {

							//判定类型
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								
								tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->SwapCard_GetItemPos1());

								if (pItemOld)
								{
									pItemOld->SetLock(false);

								}

								CDataPool::GetMe()->SwapCard_SetItemPos1(pItem->GetPosIndex());
								CDataPool::GetMe()->SwapCard_SetItemConta1(tObject_Item::IO_MYSELF_PACKET);

								CEventSystem::GetMe()->PushEvent(GE_EQUIP_SWAPCARD_UPDATA, pItem->GetID(), nIndex);
								pItem->SetLock(true);
								//通知背包锁定状态
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

							}
							else
							{
								STRING strTemp = "只可以是装备才可以对调牌型";
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

							}


						 }
						break;

					case  tObject_Item::IO_MYSELF_EQUIP:
						{

							tObject_Item *pItemOld =  CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (CDataPool::GetMe()->SwapCard_GetItemPos1()));

							if (pItemOld)
							{
								pItemOld->SetLock(false);

							}

							CDataPool::GetMe()->SwapCard_SetItemPos1(pItem->GetPosIndex());
							CDataPool::GetMe()->SwapCard_SetItemConta1(tObject_Item::IO_MYSELF_EQUIP);

							CEventSystem::GetMe()->PushEvent(GE_EQUIP_SWAPCARD_UPDATA, pItem->GetID(), nIndex);
							pItem->SetLock(true);
						 
								//通知装备锁定状态
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

						}
						break;

					}

			}

			else if ( 2 == nIndex)

			{

				switch( nTypeOwner )
				{
				case tObject_Item::IO_MYSELF_PACKET:			//来自背包
					{	
						//判定类型
						if(pItem->GetItemClass() == ICLASS_EQUIP )
						{

							tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->SwapCard_GetItemPos2());

							if (pItemOld)
							{
								pItemOld->SetLock(false);

							}

							CDataPool::GetMe()->SwapCard_SetItemPos2(pItem->GetPosIndex());
							CDataPool::GetMe()->SwapCard_SetItemConta2(tObject_Item::IO_MYSELF_PACKET);

							CEventSystem::GetMe()->PushEvent(GE_EQUIP_SWAPCARD_UPDATA, pItem->GetID(), nIndex);
							pItem->SetLock(true);
							//通知背包锁定状态
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

						}else
						{
							STRING strTemp = "只可以是装备才可以对调牌型";
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

						}

					}
					break;

				case  tObject_Item::IO_MYSELF_EQUIP:
					{

						tObject_Item *pItemOld =  CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(CDataPool::GetMe()->SwapCard_GetItemPos1()));

						if (pItemOld)
						{
							pItemOld->SetLock(false);
							

						}

						CDataPool::GetMe()->SwapCard_SetItemPos2(pItem->GetPosIndex());
						CDataPool::GetMe()->SwapCard_SetItemConta2(tObject_Item::IO_MYSELF_EQUIP);

						CEventSystem::GetMe()->PushEvent(GE_EQUIP_SWAPCARD_UPDATA, pItem->GetID(), nIndex);
						pItem->SetLock(true);
							//通知装备锁定状态
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


					}
					break;

				}

			}

			else if ( 3 == nIndex)

			{
				//检查是不是由背包界面拖过来的(只能接受玩家背包内的物品)
				if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)

				{
					 
					tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->SwapCard_GetNeedItemPos());

						if (pItemOld)
						{
							pItemOld->SetLock(false);

						}

					CDataPool::GetMe()->SwapCard_SetNeedItemId(11000001 );
					CDataPool::GetMe()->SwapCard_SetNeedItemPos(pItem->GetPosIndex());

					CEventSystem::GetMe()->PushEvent(GE_EQUIP_SWAPCARD_UPDATA, pItem->GetID(),nIndex);
					pItem->SetLock(true);
					//通知背包锁定状态
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


				}


			}


		}

		break;
		// 装备的牌型改造
	case  'I':    
		{

			INT nIndex = szTargetName[1]-'0';
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
			if(!pItem) return;


			if( 1 == nIndex)  // 1表示装备    2 表示道具
			{	

				switch( nTypeOwner )
				{
				case tObject_Item::IO_MYSELF_PACKET:			//来自背包
					{

						//判定类型
						if(pItem->GetItemClass() == ICLASS_EQUIP )
						{

							tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->ChangeCard_GetItemPos());

							if (pItemOld)
							{
								pItemOld->SetLock(false);

							}

							CDataPool::GetMe()->ChangeCard_SetItemPos(pItem->GetPosIndex());
							CDataPool::GetMe()->ChangeCard_SetItemConta(tObject_Item::IO_MYSELF_PACKET);

							CEventSystem::GetMe()->PushEvent(GE_EQUIP_CHANGECARD_UPDATA, pItem->GetID(), nIndex);
							pItem->SetLock(true);
							//通知背包锁定状态
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

						}
						else
						{
							STRING strTemp = "只可以是装备才可以改造牌型";
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

						}


					}
					break;

				case  tObject_Item::IO_MYSELF_EQUIP:
					{

						tObject_Item *pItemOld =  CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP(CDataPool::GetMe()->ChangeCard_GetItemPos()));

						if (pItemOld)
						{
							pItemOld->SetLock(false);
							 

						}

						CDataPool::GetMe()->ChangeCard_SetItemPos(pItem->GetPosIndex());
						CDataPool::GetMe()->ChangeCard_SetItemConta(tObject_Item::IO_MYSELF_EQUIP);

						CEventSystem::GetMe()->PushEvent(GE_EQUIP_CHANGECARD_UPDATA, pItem->GetID(), nIndex);
						pItem->SetLock(true);
					 
							//通知装备锁定状态
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


					}
					break;

				}

			}


			else if ( 2 == nIndex)

			{
				//检查是不是由背包界面拖过来的(只能接受玩家背包内的物品)
				if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)

				{

					tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->ChangeCard_GetNeedItemPos());

				    if (pItemOld)
				    {
						pItemOld->SetLock(false);
				    }

					CDataPool::GetMe()->ChangeCard_SetNeedItemId(11000001);
					CDataPool::GetMe()->ChangeCard_SetNeedItemPos(pItem->GetPosIndex());

					CEventSystem::GetMe()->PushEvent(GE_EQUIP_CHANGECARD_UPDATA, pItem->GetID(),nIndex);
					pItem->SetLock(true);
					//通知背包锁定状态
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


				}


			}


		}

		break;


	case  'K'://一键换装
		{
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自背包
				{
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;

					//判定类型
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{ 
						int iNeedLevel   = 0;
						int iPlayerLevel = 0;

						if(pItem)
						{
							//验证是否可以使用
							if(!pItem->Rule(tObject_Item::RULE_USE))
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
								break;
							}

							iNeedLevel = pItem->GetNeedLevel();
						}

						CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						if(pCharData)
						{
							iPlayerLevel = pCharData->Get_Level();
						}

						if(iPlayerLevel >= iNeedLevel)
						{
							//if( GetItemImpl()->GetItemProfession() != pCharData->Get_MenPai() && GetItemImpl()->GetItemProfession() != MATTRIBUTE_WUMENPAI)
							//{
							//	STRING strTemp = "职业不对";
							//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//	return;
							//}
	
							string sTarName(szTargetName);
							string sTarIdx = sTarName.substr(1);

							USHORT uWorld, uServer; UINT uSerial;
							pItem->GetGUID(uWorld, uServer, uSerial);

							// 构造lua脚本解析的guid字符串
							char guid[30];
							sprintf(guid, "%d_%d=%d", uWorld, uServer, uSerial);

							if( szSourceName[0] != szTargetName[0] )
								CEventSystem::GetMe()->PushEvent( GE_UPDATE_QUICK_CHANGE, 
								sTarIdx.c_str(), guid, pItem->GetID(), pItem->GetItemTableType() );						}
						else
						{
							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Level_Not_Enough");
							CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, _T("等级不够") );
						}
					}
					else
					{ 
						STRING strTemp = "此物品无法装备";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					}
				}
				break;
			}
		}
		break;

	case 'Z':		//人物装备界面   
		{
			INT nIndex = szTargetName[1]-'0';

			//根据来源，确定如何处理
			//来自其他地方，直接给予提示不能装备
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自背包，就发送装备消息
				{
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;
					//判定类型
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						int iNeedLevel   = 0;
						int iPlayerLevel = 0;

						tObject_Item* pItem = GetItemImpl();
						if(pItem)
						{
							//验证是否可以使用
							if(!pItem->Rule(tObject_Item::RULE_USE))
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
								break;
							}

							iNeedLevel = pItem->GetNeedLevel();
						}

						CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						if(pCharData)
						{
							iPlayerLevel = pCharData->Get_Level();
						}
						
						if(iPlayerLevel >= iNeedLevel)
						{
							CGameProcedure::s_pGameInterface->PacketItem_UserEquip( pItem->GetPosIndex() );
						}
						else
						{
							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Level_Not_Enough");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, _T("等级不够") );
						}
					}
					else
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Can_Not_Equip");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品无法装备");
					}
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:				//来自自己装备装备
				{
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//来自银行，也发送装备消息
				{
				}
				break;
			case tObject_Item::IO_MYEXBOX:					//来自交易对话框
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Exchanging");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品正在交易");
				}
				break;
			case tObject_Item::IO_BOOTH:					//来自商人货架
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Buy_Item_First");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"请先购买此物品");
				}
				break;
			case tObject_Item::IO_MYSTALLBOX:				//来自自己的摊位
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Selling");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品正在摆摊销售中");
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'A':		//玩家商店
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//检测这个格子是不是已经有物品，并且这个物品是不是已经上架
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			tObject_Item* pShopItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nConIndex, nIndex);
			if(pShopItem)
			{
				if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nIndex))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "目标位置的物品已经上架");
					return;
				}
			}

			//检查是不是由背包界面拖过来的(只能接受玩家背包内的物品)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				// 判定物品是否已经绑定
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
						break;
					}
					//需要先判定是不是能够存入银行的物品
					if( GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
					{
						//任务物品不能放入银行
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToPS_SHOP");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能存入银行");
						break;
					}

				}
				//执行将物品转移到商店去的操作
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return;
				}

				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGItemSynch msg;
				msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
				msg.SetFromType(CGItemSynch::POS_BAG);
				msg.SetToIndex(nIndex);
				msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetItemGUID(Guid);


				INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
				CGManuMoveItemFromBagToPlayerShop_t ExtraInfoData;
				ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
				ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
				ExtraInfoData.m_uSerial			= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nIndex);
				msg.SetExtraInfoLength(ExtraInfoData.GetSize());
				msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			
				CNetManager::GetMe()->SendPacket(&msg);
			}
			//在背包内调整位置
			else if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_PS_SELFBOX)
			{
				//如果放到自己格子，那么不需要发送这个消息
				if(this->GetPosIndex() == nIndex)
				{
					return;
				}
				//执行将物品转移到商店去的操作
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return;
				}

				INT nIndex1 = pSelectItem->GetPosIndex();
				INT nConIndex = nIndex1/20;
				INT nPosition = nIndex1%20;

				if( 1 == CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition) )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经上架");
					return;
				}

				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGItemSynch msg;
				msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
				msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetToIndex(nIndex);
				msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetItemGUID(Guid);

				INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
				CGManuMoveItemFromPlayerShopToPlayerShop_t ExtraInfoData;
				ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
				ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
				ExtraInfoData.m_uSerialSource	= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,this->GetPosIndex()%20);
				ExtraInfoData.m_uSerialDest		= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nIndex);
				msg.SetExtraInfoLength(ExtraInfoData.GetSize());
				msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		break;
	case 'M':		//主菜单
		{
			//查询来源，只能来源于背包
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if( this->GetItemImpl()->Rule(tObject_Item::RULE_QUICK) )
				{
					INT nIndex = szTargetName[1]-'0';
					nIndex = szTargetName[2]-'0' + nIndex*10 ;
					//查询目标位置原来的内容
					nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
					CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID() );
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Can_Put_In_ShortCut");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不能置入快捷栏。");
				}
			}
		}
		break;

	case 'Y':		//对象头像
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//根据装备类型
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//药瓶
			case ICLASS_TASKITEM:	//任务物品类
			case ICLASS_STOREMAP:	//藏宝图
				{		//摆摊时不可以随便使用物品
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Stalling");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
						break;
					}

					//验证是否可以使用
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//检查冷却是否结束
					if(!CoolDownIsOver()) 
					{
						CActionSystem::GetMe()->SetSetoutAction(this);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
						break;
					}

					CObject *pTargetObj = (CObject*)(CObjectManager::GetMe()->GetMainTarget());
					CGameProcedure::s_pGameInterface->PacketItem_UserItem(
						this, pTargetObj? pTargetObj->GetServerID() : -1, fVector2(-1, -1));
				}
				break;
			}

		}
		break;
	case 'L':		//自己头像
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//根据装备类型
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//药瓶
			case ICLASS_TASKITEM:	//任务物品类
			case ICLASS_STOREMAP:	//藏宝图
				{		//摆摊时不可以随便使用物品
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Stalling");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
						break;
					}

					//验证是否可以使用
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//检查冷却是否结束
					if(!CoolDownIsOver()) 
					{
						CActionSystem::GetMe()->SetSetoutAction(this);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
						break;
					}

					//对主目标使用物品
					CObject* pMainTarget = (CObject*)CObjectManager::GetMe()->GetMainTarget();
					CGameProcedure::s_pGameInterface->PacketItem_UserItem(this, 
						CObjectManager::GetMe()->GetMySelf()->GetServerID(), fVector2(-1, -1));
				}
				break;
			}

		}
		break;
	case 'N':		//打星界面
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//根据装备类型
			switch(pItem->GetItemClass())
			{
			case ICLASS_EQUIP:	//装备
				{
					//摆摊时不可以随便使用物品
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Stalling");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
						break;
					}
					//先将可能存在的已经锁定的物品解除锁定
					INT nCon = CDataPool::GetMe()->AddStar_GetItemConta();
					INT nPos = CDataPool::GetMe()->AddStar_GetItemPos();
					switch( nCon ) 
					{
					case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
						{
							tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

							if(pItem)
							{
								pItem->SetLock(FALSE);
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
						}
						break;
					case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家的装备
						{
							tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nPos));

							if(pItem)
							{
								pItem->SetLock(FALSE);
								//通知装备锁定状态
								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
							}
						}
						break;
					default:
						break;
					}

					//记录打星物品的位置到数据池
					CDataPool::GetMe()->AddStar_SetItemConta(pItem->GetTypeOwner());
					CDataPool::GetMe()->AddStar_SetItemPos(pItem->GetPosIndex());


					//打开打星对话框
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING);

					// 锁定这个物品
					pItem->SetLock(TRUE);

					//通知背包锁定状态
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
					return;

				}
				break;
			default:
				{
					STRING strTemp = "此物品不可以升星";

					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}

		}
		break;
	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M':
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, nOldTargetId);
			//AxTrace(0,0,"nIndex",)
		}
		break;
	case  'K'://一键换装
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			if( !pItem ) 
				return;

			string sSrcName(szSourceName);
			string sSrcIdx = sSrcName.substr(1);
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_QUICK_CHANGE, sSrcIdx.c_str(), "-1" );
		}
		break;

	case  'E'://交易
		{
			//发送拖动物品的消息
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEITEM);
			//msg.SetFromType(EXCHANGE_MSG::);
			msg.SetFromIndex(this->GetPosIndex());
			msg.SetToType(EXCHANGE_MSG::POS_BAG);
			msg.SetToIndex(0);						//由服务器查找
			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
	default:
		break;
	}
//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, "0");
}
INT CActionItem_Item::GetCurBankRentBegin(VOID)
{
	INT nRentIndex = CActionSystem::GetMe()->GetCurBankRentBoxIndex();
	INT nBeginIndex = -1;
	switch(nRentIndex) 
	{
	case 1:
		{
			nBeginIndex = RENTBOX1_START_INDEX;
		}
		break;
	case 2:
		{
			nBeginIndex = RENTBOX2_START_INDEX;
		}
		break;
	case 3:
		{
			nBeginIndex = RENTBOX3_START_INDEX;
		}
		break;
	case 4:
		{
			nBeginIndex = RENTBOX4_START_INDEX;
		}
		break;
	case 5:
		{
			nBeginIndex = RENTBOX5_START_INDEX;
		}
		break;
	default:
		return -1;
	}

	return nBeginIndex;
}


//查询逻辑属性
STRING CActionItem_Item::GetAttributeValue(LPCTSTR szAttributeName)
{
	tObject_Item* pItem = GetItemImpl();

	if(!pItem) return "";
	else return pItem->GetAttributeValue(szAttributeName);
}


// 把装备从身上卸下
void CActionItem_Item::UnUseEquip()
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) 
	{
		return;
	}

	CGUnEquip msg;
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
	if(pItemEquip->GetItemClass() != ICLASS_EQUIP)
	{
		return;
	}

	// 发送卸下装备消息.
	msg.setEquipPoint((BYTE)(pItemEquip->GetItemType()));
	CNetManager::GetMe()->SendPacket( &msg );
}

// 得到物品的等级
INT	CActionItem_Item::GetItemLevel()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetNeedLevel();
	}
	else
	{
		return -1;
	}

}

// 得到物品的耐久
INT	CActionItem_Item::GetItemDur()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemDur();
	}
	else
	{
		return -1;
	}

}



// 得到物品的最大耐久
INT	CActionItem_Item::GetItemMaxDur()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemMaxDur();
	}
	else
	{
		return -1;
	}
}

// 得到物品的修理次数
INT	CActionItem_Item::GetItemRepairCount()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemRepairCount();
	}
	else
	{
		return -1;
	}
}

// 得到物品的绑定信息 
INT	CActionItem_Item::GetItemBindInfo()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemBindInfo();
	}
	else
	{
		return -1;
	}
}

// 得到装备的2次绑定信息 
INT	CActionItem_Item::GetItemSecLevelBindInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetItemSecLevelBindInfo();
	}
	else
	{
		return -1;
	}
}
//	得到装备的2次绑定加成
LPCTSTR	CActionItem_Item::GetItemBindAddInfo()
{

	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetItemBindAddInfo();
	}
	else
	{
		return _T("");
	}


}

//	得到装备的color
LPCTSTR	CActionItem_Item::GetDropVisColor()
{

	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetDropVisColor();
	}
	else
	{
		return _T("");
	}


}

// 20100408 AddCodeBegin
// 得到物品买入的价格
INT	CActionItem_Item::GetItemBasePrice()
{
	tObject_Item* pItem = GetItemImpl();

	// 如果获取出售商品的买入价格，则根据不同的物品类别，处理不同
	if (    pItem->GetItemClass() == ICLASS_IDENT    || pItem->GetItemClass() == ICLASS_GEM
		||  pItem->GetItemClass() == ICLASS_TASKITEM || pItem->GetItemClass() == ICLASS_COMITEM 
		||  pItem->GetItemClass() == ICLASS_MATERIAL)
	{
		return pItem->GetItemBasePrice();
	}
	else if ( pItem->GetItemClass() == ICLASS_EQUIP )
	{
		return pItem->GetItemPrice();
	}
	else
		return -1;

}
// 20100408 AddCodeEnd


// 得到物品卖给npc的价格
INT	CActionItem_Item::GetItemPrice()
{
	tObject_Item* pItem = GetItemImpl();

	if(pItem)
	{		
		return pItem->GetItemPrice();
	}
	else
	{
		return -1;
	}
	
}

// 得到物品职业
INT	CActionItem_Item::GetItemProfession()

{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemProfession();
	}
	else
	{
		return -1;
	}

}


//得到装备牌型

INT	CActionItem_Item::GetEquipCard()

{
	CObject_Item_Equip* pItem =  (CObject_Item_Equip*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetEquipCard();
	}
	else
	{
		return -1;
	}

}



//得到物品是否唯一
INT	CActionItem_Item::GetItemRuleOnly()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemRuleOnly();
	}
	else
	{
		return -1;
	}

}


// 得到物品的制作人
LPCTSTR	CActionItem_Item::GetManufacturer()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetManufacturer();
	}
	else
	{
		return _T("");
	}
	
}

//得到装备的孔数量
INT		CActionItem_Item::GetEquipHoleNum()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipHoleNum();
	}
	else
	{
		return 0;
	}
}

//得到装备的套装信息
LPCTSTR		CActionItem_Item::GetEquipSetInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipSetInfo();
	}
	else
	{
		return "";
	}
}

//得到装备牌型激活的信息 
 
LPCTSTR		CActionItem_Item::GetEquipSpritePaiInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipSpritePaiInfo();
	}
	else
	{
		return "";
	}
}

//得到其他玩家装备小激活的信息 
 
LPCTSTR		CActionItem_Item::GetOtherEquipSpriteChainInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetOtherEquipSpriteChainInfo();
	}
	else
	{
		return "";
	}
}

//得到其他玩家装备大激活的信息
 
LPCTSTR		CActionItem_Item::GetOtherEquipSpriteAllActiveInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetOtherEquipSpriteAllActiveInfo();
	}
	else
	{
		return "";
	}
}


//得到装备大激活的信息
 
LPCTSTR		CActionItem_Item::GetEquipSpriteAllActiveInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipSpriteAllActiveInfo();
	}
	else
	{
		return "";
	}
}


//得到装备小激活的信息
 
LPCTSTR		CActionItem_Item::GetEquipSpriteChainInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipSpriteChainInfo();
	}
	else
	{
		return "";
	}
}





//得到装备的套装信息
LPCTSTR		CActionItem_Item::GetOtherEquipSetInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetOtherEquipSetInfo();
	}
	else
	{
		return "";
	}
}





//得到装备套装激活的属性信息

LPCTSTR		CActionItem_Item::GetEquipSetActiveAttr()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipSetActiveAttr();
	}
	else
	{
		return "";
	}
}

//得到装备套装激活的属性信息

LPCTSTR		CActionItem_Item::GetOtherEquipSetActiveAttr()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetOtherEquipSetActiveAttr();
	}
	else
	{
		return "";
	}
}





//得到装备的星级

INT		CActionItem_Item::GetStartLevel()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetStartLevel();
	}
	else
	{
		return 0;
	}
}




// 得到白色属性 
LPCTSTR	CActionItem_Item::GetBaseWhiteAttrInfo()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetBaseWhiteAttrInfo();
	}
	else
	{
		return _T("");
	}

	
}
// 得到装备基本属性
LPCTSTR	CActionItem_Item::GetEquipBaseAttrInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipBaseAttrInfo();
	}
	else
	{
		return _T("");
	}

}
// 得到装备属性 
LPCTSTR	CActionItem_Item::GetEquipAddAttrInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipAddAttrInfo();
	}
	else
	{
		return _T("");
	}

	
}

//得到装备技能属性
LPCTSTR CActionItem_Item::GetEquipSkillInfo()
{

	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipSkillInfo();
	}
	else
	{
		return _T("");
	}




}

// 得到action所引用的item 的类型(如是装备, 还是物品)
INT	CActionItem_Item::GetItemType()
{

	STRING strTypeName = GetType_String();

	if(("equip" == strTypeName)			|| 
	    ("packageitem" == strTypeName)	||
		("bankitem" == strTypeName)		||
		("boothitem" == strTypeName)	||
		("lootitem" == strTypeName)		||
		("virtualitem" == strTypeName)  ||
		("ex_self" == strTypeName)		||
		("ex_other" == strTypeName)		||
		("st_self" == strTypeName)		||
		("st_other" == strTypeName)		||
		("ps_self" == strTypeName)		||
		("ps_other" == strTypeName)		||
		("mission" == strTypeName)	    ||
		("transferitem" == strTypeName)	||
		("ib_shopitem" == strTypeName)	||
		("mountitem" == strTypeName)	                       // [7/21/2010 陈军龙]
		)	
	{
		CObject_Item* pItem = (CObject_Item*)GetItemImpl();
		if(pItem)
		{
			return pItem->GetItemClass();
		}
		else
		{
			return -1;
		}
	}

	return -1;
	
}

// 得到装备上的宝石个数
INT CActionItem_Item::GetGemInEquipCount()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	
	if(pItemEquip)
	{
		// 得到已经镶嵌宝石的个数
		//INT iCount = pItemEquip->GetGemCount();

		// 得到可以镶嵌宝石的个数
		INT iCount = pItemEquip->GetGemMaxCount();
		return iCount;
	}
	else
	{
		return 0;
	}
	
}

// 得到宝石的图标
VOID CActionItem_Item::GetGemIcon(INT iGemIndex, STRING& strIconName)
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	
	if(pItemEquip)
	{
		
		pItemEquip->GetGemIcon(iGemIndex, strIconName);
	}
	else
	{
		strIconName = _T("");
	}

	return;
}

// 得到装备上的宝石描述
VOID CActionItem_Item::GetGemInEquipDesc(STRING& strDesc)
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	STRING strInfo = _T("");
	strDesc = _T("");
	
	if(pItemEquip)
	{
		int iCount = pItemEquip->GetGemCount();
		for(int i = 0; i < iCount; i++)
		{
			strInfo = _T("");//每颗宝石重新计算附加属性
			pItemEquip->GetGemExtAttr(i, strInfo);
			strDesc += strInfo;
		}
	}
	else
	{
		strDesc = _T("");
	}

	return;
}

// 得到装备的最大修理次数
//INT CActionItem_Item::GetItemMaxRepair()
//{
//	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
//	if(pItemEquip)
//	{
//		pItemEquip->GetMaxRepair();
//	}
//	
//	return -1;
//}

// 得到宝石等级
INT CActionItem_Item::GetGemLevel()
{
	CObject_Item_Gem* pItem = (CObject_Item_Gem*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetGemLevel();
	}

	return -1;
}

// 得到宝石属性信息
LPCTSTR	CActionItem_Item::GetGemArribInfo()
{
	static TCHAR szInfo[32] = {0};

	CObject_Item_Gem* pItem = (CObject_Item_Gem*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetGemArribInfo();
	}

	return szInfo;
}


//得到魂珠的属性类型
LPCTSTR CActionItem_Item::GetFecthHeadInfo_TypeDesc( INT Index )
{
	CObject_Item_SoulBead* pItem = (CObject_Item_SoulBead*)GetItemImpl();
	if ( pItem )
	{
		return pItem->GetFecthHeadInfo_TypeDesc(Index);
	}
	return NULL;
}


//得到魂珠的属性值
INT CActionItem_Item::GetFecthHeadInfo_Value( INT Index )
{
	CObject_Item_SoulBead* pItem = (CObject_Item_SoulBead*)GetItemImpl();
	if ( pItem )
	{
		return pItem->GetFecthHeadInfo_Value(Index);
	}
	return 0;
}


//得到魂珠的属性数量
INT	CActionItem_Item::GetFecthHeadInfo_Num()
{
	CObject_Item_SoulBead* pItem = (CObject_Item_SoulBead*)GetItemImpl();
	if( pItem )
		return pItem->GetFecthHeadInfo_Num();
	return 0;
}


// 得到原料或药品的质量  //remove
//INT CActionItem_Item::GetItemQuality()
//{
//	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
//
//	if(pItem)
//	{
//		return pItem->GetQuantity();
//	}
//
//	return -1;
//}

//得到装备是普通的还是高级的
INT   CActionItem_Item::GetItemEquipQua()

{
	CObject_Item_Equip *pEquip = (CObject_Item_Equip*)GetItemImpl(); 

	if (pEquip)
	{
		return pEquip->GetItemEquipQua();
	}

	return -1;
}






// 得到消耗品在表中的类型
INT CActionItem_Item::GetMedicineItemTableType()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();

	if(pItem)
	{
		return pItem->GetMedicineItemTableType();
	}

	return -1;
}

//得到冷却状组ID
INT	CActionItem_Item::GetCoolDownID(VOID)
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem && pItem->GetItemClass() == ICLASS_COMITEM)
	{
		//只有背包里的物品会走CoolDown
		if(pItem->GetTypeOwner() != tObject_Item::IO_MYSELF_PACKET)
			return -1;

		INT nSkillTableID = pItem->GetItemSkillID();

		if(nSkillTableID != -1)
		{
			//查表
			DBC_DEFINEHANDLE(s_pSkillTable, DBC_SKILL_DATA);
			//搜索纪录
			const _DBC_SKILL_DATA* pSkill = (const _DBC_SKILL_DATA*)s_pSkillTable->Search_Index_EQU((UINT)nSkillTableID);
			if(!pSkill) return NULL;

			return pSkill->m_nCooldownID;
		}
	}
	return -1;
}

//检查冷却是否结束
BOOL CActionItem_Item::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//对于没有冷却的Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//取得冷却组数据
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

	if( pCoolDownGroup && pCoolDownGroup->nTime <= 0 && CDataPool::GetMe()->CommonCoolDown_Get() <= 0 ) 
		return TRUE;
	else 
		return FALSE;
}

//DoAction_Packet
VOID CActionItem_Item::DoAction_Packet(VOID)
{
	CObject_Item* pItem = (CObject_Item*)this->GetItemImpl();
	if(!pItem) return;

	BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
	{
		// 取消鉴定 
		CancelIdentify();
		return;
	}

	//如果玩家商店开着，
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
	{
		
		if(!(CDataPool::GetMe()->PlayerShop_GetShopType(TRUE) == 1))
		{
			return;
		}

		if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
			return;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		}

		//需要先判定是不是能够存入银行的物品
		if( GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//任务物品不能放入银行
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToPS_SHOP");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能存入银行");
			return;
		}

		//执行将物品转移到商店去的操作
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;

		tObject_Item* pSelectItem = this->GetItemImpl();
		if(NULL == pSelectItem)
		{
			return;
		}
		pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

		CGItemSynch msg;
		msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
		msg.SetFromType(CGItemSynch::POS_BAG);
		msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
		msg.SetItemGUID(Guid);

		CGAutoMoveItemFromBagToPlayerShop_t ExtraDataInfo;
		ExtraDataInfo.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
		ExtraDataInfo.m_nStallIndex		= (BYTE)CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
		msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);
	
		CNetManager::GetMe()->SendPacket(&msg);

		return;
	}
	//如果摆摊界面开着，点击物品代表飞到摆摊界面中(中间需要经过一个确认步骤)
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
	{
		// 判定物品是否已经绑定
		if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
			return;
		}
		//判断物品是不是任务物品
		if(GetItemType() == ICLASS_TASKITEM )//this->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItemNot2Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "任务物品不能上架销售");
			return;
		}
		//查询摊位上是不是还有空位
		if(CDataPool::GetMe()->MyStallBox_IsHaveEmpty(1) == FALSE)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_StallHaveNotEmpty");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "摊位上已经没有空格");
			return;
		}

		//先保存选中的物品
		CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
		// 同时记录Item所在的容器
		CDataPool::GetMe()->MyStallBox_SetSelectItemInPos(tObject_Item::IO_MYSELF_PACKET);
		//打开一个输入卖价的对话框
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"price");

		return;
	}
	//如果提交任务物品的界面开着,点击代表物品飞到提交栏中
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
	{
		//需要在MissionBOX中寻找空的位置
		INT nIndex = -1;
		for(int i=0;i<3;i++)
		{
			if( CDataPool::GetMe()->MissionBox_GetItem(i) == NULL )
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex == -1)
		{
			//没有空格可以放入物品了，给个提示就退出
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Misstion_Item_Is_Full");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品已经放满了！");
			return;
		}

		//克隆物品
		tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
		((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

		//在这里保存直接通知界面添加按钮，不给服务器发消息，
		CDataPool::GetMe()->MissionBox_SetItem(nIndex, pNewItem);

		// 锁定背包中对应的这个物品

		CActionSystem::GetMe()->UserMission_Update();
		// 发送更新消息
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
		// 背包更新消息
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

		return;
	}
	//如果银行界面时打开的，点击代表存银行
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Bank"))
	{
		//需要先判定是不是能够存入银行的物品
		if( GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//任务物品不能放入银行
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能存入银行");
			return;
		}	
		
		//需要检测当前银行打开的是第几个租赁栏
		INT nRentIndex = CActionSystem::GetMe()->GetCurBankRentBoxIndex();

		CGBankAddItem msg;
		msg.SetFromType(CGBankAddItem::BAG_POS);
		msg.SetIndexFrom(this->GetPosIndex());

		if(nRentIndex == 1)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX1);
		else if(nRentIndex == 2)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX2);
		else if(nRentIndex == 3)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX3);
		else if(nRentIndex == 4)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX4);
		else if(nRentIndex == 5)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX5);

		//检测这个租赁箱是不是有空位
		if( !CDataPool::GetMe()->UserBank_IsEmpty( nRentIndex ) )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Target_Box_Is_Full");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"目标箱子已经满了");
			return;
		}

		CNetManager::GetMe()->SendPacket(&msg);

		return;
	}
	// 如果交易界面开着，点击代表将此物品放入交易栏
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		// 判定物品是否已经绑定
		if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
			return;
		}
		//判定是不是能够存入银行的物品
		if(GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//任务物品不能交易
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToExchange");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能进行交易");
			return;
		}
		//验证是否可以交易
		if(!pItem->Rule(tObject_Item::RULE_EXCHANGE))
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
			return;
		}
		
		//发送拖动物品到交易界面的消息
		// 需要先检测是否交易界面已满
		CGExchangeSynchItemII msg;
		msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
		msg.SetFromType(EXCHANGE_MSG::POS_BAG);
		msg.SetFromIndex(this->GetPosIndex());
		msg.SetToIndex(0);						//由服务器查找
		CNetManager::GetMe()->SendPacket(&msg);

		return;
	}

	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Enchase"))
	{
		INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
		switch( nTypeOwner ) 
		{
		case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
			{
				if(GetItemImpl()->GetItemClass() == ICLASS_GEM)    //remove
				{
					//CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
					//if(pCharData)
					//{
					//	if( GetItemImpl()->GetNeedLevel() <= pCharData->Get_Level() ){}
					//	{
					//		STRING strTemp = "等级不够，不能镶嵌";
					//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					//		return;
					//	}
					//	if( GetItemImpl()->GetItemProfession() != pCharData->Get_MenPai() )
					//	{
					//		STRING strTemp = "职业不对，不能镶嵌";
					//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					//		return;
					//	}
					//}

					CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, "-1", "-1", GetItemImpl()->GetPosIndex());
				}
				else
				{
					STRING strTemp = "需要宝石进行镶嵌";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				}
			}
		}

		return;
	}

	//如果商品界面是开的,点击代表卖出
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
	{
		//是否在修理状态
		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
		{
			//取消修理
			CGameProcedure::s_pGameInterface->Skill_CancelAction();
		}
		//执行销售
		else
		{
			////任务物品不可以卖
			if( GetItemType() != ICLASS_TASKITEM )//pItem->GetPosIndex() < TASK_CONTAINER_OFFSET )
			{
				CGameProcedure::s_pGameInterface->Booth_SellItem( pItem );
			}
			else
			{
				//任务物品不能卖的信息提示
				if(bLog)
				{
					STRING strTemp = NOCOLORMSGFUNC("stall_sold_failure");
					ADDNEWDEBUGMSG(strTemp);
				}
			}
		}

		return;
	}
	else
	{
		//根据装备类型
		switch(pItem->GetItemClass())
		{
		case ICLASS_EQUIP:
			//装备
			{
				//使用
				CGameProcedure::s_pGameInterface->PacketItem_UserEquip( pItem->GetPosIndex() );
			}
			break;

		case ICLASS_COMITEM:	//药瓶
		case ICLASS_TASKITEM:	//任务物品类
		case ICLASS_STOREMAP:	//藏宝图
		case ICLASS_SOUL_BEAD:	//魂珠
			{
				int iType = pItem->GetItemTableType();

				//if ( iType == COMITEM_SOUXIA )
				//{
				//	CEventSystem::GetMe()->PushEvent(GE_OPEN_SOUXIA);
				//	break;
				//}

				if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaUsbItem(0,pItem) )		//是搜侠录USB记忆棒道具
				{
					INT type = SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.GetSouXiaUsbType(pItem);
					if( type != -1 )
						CEventSystem::GetMe()->PushEvent( GE_OPEN_SOUXIA, type, 1 );
					break;
				}

				if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiadeformityItem(pItem) )	//是否是搜侠录残页
				{
					SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.ShowSouXiaDeformity(pItem);
				}

				//摆摊时不可以随便使用物品
				if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_You_Stalling");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
					break;
				}

				//验证是否可以使用
				if(!pItem->Rule(tObject_Item::RULE_USE))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
					break;
				}

				//检查冷却是否结束
				if(!CoolDownIsOver()) 
				{
					CActionSystem::GetMe()->SetSetoutAction(this);
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
					break;
				}
				CObject* pMainTarget = (CObject*)CObjectManager::GetMe()->GetMainTarget();

				CGameProcedure::s_pGameInterface->PacketItem_UserItem(this, 
					pMainTarget ? pMainTarget->GetServerID() : -1, fVector2(-1, -1));
			}
			break;
		case ICLASS_GEM:
			//宝石
			{
			}
			break;
		//坐骑 [7/8/2010 陈军龙] 
		case ICLASS_MOUNT:
			{
			    CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
				const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_ITEM_MOUNT_ATT );
				const _DBC_ITEM_MOUNT_ATT* pMountAtt = (const _DBC_ITEM_MOUNT_ATT*)pdbcbase->Search_Index_EQU(pItem->GetIdTable());


				if((pCharData != NULL) && (pCharData->Get_Level()< pMountAtt->nNeedLevel))
				{
					// 显示到聊天框
					if(bLog)
					{
						STRING szMsg = COLORMSGFUNC("Mount_Equip_Fail_For_Grade_Low", pMountAtt->szName);;
						ADDTALKMSG(szMsg);
					}

					return;
				}

			    CGameProcedure::s_pGameInterface->PacketItem_EquipMount( pItem->GetPosIndex() );
			}
			break;
		}
	}
}

//销毁物品的操作
VOID CActionItem_Item::DestoryItem(LPCSTR szSourceName)
{
	CHAR cSourceType = szSourceName[0];
	switch(cSourceType)
	{
	case 'M':		//主菜单
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, -1 );
			return;
		}
		break;
	default:
		break;
	}
	INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
	if( nTypeOwner == tObject_Item::IO_MYSELF_PACKET || 
		//nTypeOwner == tObject_Item::IO_MYSELF_EQUIP  ||
		nTypeOwner == tObject_Item::IO_MYSELF_BANK)
	{
		// 任务物品不能销毁，给一个提示                  
		if((nTypeOwner == tObject_Item::IO_MYSELF_PACKET) && (GetItemType() == ICLASS_TASKITEM) )// && this->GetPosIndex() >= TASK_CONTAINER_OFFSET)
		{
			// 需要先询问是否销毁这个装备
			//CEventSystem::GetMe()->PushEvent(GE_OPEN_CANNT_DISCARD_ITEM,this->GetItemImpl()->GetName());
			// 给出屏幕提示

			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Can_Not_Drop");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能丢弃");
		}
		else
		{
			// 先取消可能存在的锁定
			INT nTypeOwner	=	CDataPool::GetMe()->DisCard_GetItemConta();
			INT	nItemPos	=	CDataPool::GetMe()->DisCard_GetItemPos();

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			default:
				break;
			}
					
			// 需要先询问是否销毁这个装备
			CEventSystem::GetMe()->PushEvent(GE_OPEN_DISCARD_ITEM_FRAME,this->GetItemImpl()->GetName());

			// 锁定这个物品
			tObject_Item *pObj = this->GetItemImpl();
			pObj->SetLock(TRUE);

			//通知背包锁定状态
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			CDataPool::GetMe()->DisCard_SetItemConta(this->GetItemImpl()->GetTypeOwner());
			CDataPool::GetMe()->DisCard_SetItemPos(this->GetPosIndex());
		}
	}
}

// 得到配方需要的技能名称。
LPCTSTR CActionItem_Item::GetPrescrNeedSkillName()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;


		// 生活技能
		DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
		_DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);

		//配方表
		DBC_DEFINEHANDLE(s_pAbility, DBC_LIFEABILITY_DEFINE);
		_DBC_LIFEABILITY_DEFINE* pData = (_DBC_LIFEABILITY_DEFINE*)s_pAbility->Search_Index_EQU(pLifeInfo->nLifeAbility);

		if(pData)
		{
			return pData->szName;
		}
	}

	return _T("");
			
}

// 得到配方需要的技能等级。
int CActionItem_Item::GetPrescrNeedSkillLevel()
{

	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;

//		int iNeedSkillLevel = pItem->m_theBaseDef->nLevelRequire;
		DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
		const _DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);

		return pLifeInfo->nLifeAbility_Level;
	//	return iNeedSkillLevel;
	}

	return 0;
}

// 得到配方需要技能的当前等级。
int CActionItem_Item::GetPrescrNeedSkillCurLevel()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(pPrescr)
		{
			return INVALID_ID;
		}
		else
		{
			DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
			const _DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);
			
			const SCLIENT_LIFEABILITY* pLifeAbility = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(pLifeInfo->nLifeAbility);

			if(!pLifeAbility)
				return 0;

			return pLifeAbility->m_nLevel;
			

		}
	}

	return 0;
}

// 发送请求详细数据消息
void CActionItem_Item::SendAskItemInfoMsg()
{
	if(GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
	{
		CObject_Item* pItem = (CObject_Item*)GetItemImpl();

		if((pItem)&&(pItem->IsItemHasDetailInfo()))
		{
			int iIndex = GetItemImpl()->GetPosIndex();
			CGAskItemInfo msg;
			msg.setID(iIndex);
			CNetManager::GetMe()->SendPacket(&msg);
		}
	}
}

// 得到装备的鉴定信息
INT CActionItem_Item::GetEquipIdentify()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipAttrib();
	}

	return 0;
	
}

// 使用鉴定卷轴 
VOID CActionItem_Item::UseIdentify()
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(ICLASS_EQUIP != pItem->GetItemClass())
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("CActionItem_Item_Idenitfy_Equip");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return;
	}

	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip) //remove
	{
		//if(CObject_Item_Equip::BLUE_EQUIP == pItemEquip->GetEquipQuantity())
		{
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
			{
				if(CObject_Item_Equip::EQUIP_ATTRIB_IDENTIFY == pItemEquip->GetEquipAttrib())
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("UI_IPERATE_EQUIP_HAVE_IDENTIFY");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				else
				{
					if(pItemEquip->GetNeedLevel() > (CDataPool::GetMe()->m_iIdentifyLevel + 9))
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("UI_OPERATE_CAN_NOT_IDENTIFY_HEIGHT_LEVEL");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						return;
					}//

					// 使用卷轴的背包索引位置
					CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->m_iIdentifyInBagIndex);

					int iIdentifyType = pItem->GetItemTableType();
					switch(iIdentifyType)
					{
					case CObject_Item_Medicine::IDEN_EQUIP:
						{

							if(!pItemEquip->IsWeapon())
							{
								// 只能鉴定武器
								STRING strTemp = "";
								strTemp = NOCOLORMSGFUNC("CActionItem_Item_Idenitfy_Equip");
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
								return;
							}

							break;
						}
					case CObject_Item_Medicine::IDEN_DEFINCE:
						{

							if(!pItemEquip->IsDefence())
							{
								// 只能鉴定防俱
								STRING strTemp = "";
								strTemp = NOCOLORMSGFUNC("CActionItem_Item_Idenitfy_Define");
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
								return;
							}
							break;
						}
					case CObject_Item_Medicine::IDEN_APPEND:
						{

							if(!pItemEquip->IsAppend())
							{
								// 只能鉴定装饰品。
								STRING strTemp = "";
								strTemp = NOCOLORMSGFUNC("CActionItem_Item_Identify_Append");
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
								return;
							}
							break;
						}

					default:
						{
							return;
							break;
						}

					}

					// 发送鉴定消息
					CGUseItem msg;
					msg.SetBagIndex( CDataPool::GetMe()->m_iIdentifyInBagIndex );
					msg.SetTargetItem(pItemEquip->GetPosIndex());
					CNetManager::GetMe()->SendPacket(&msg);
				}
			}
		}
		//else
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("UI_OPERATE_IDENTIFY_BLUE_EQUIP_ONLY");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//}
	}
	
}



	// 取消鉴定状态 
VOID CActionItem_Item::CancelIdentify()
{
	// 解锁action button
	CDataPool::GetMe()->Identify_UnLock();
}

// 准备使用鉴定卷轴 
VOID CActionItem_Item::PrepareUseIdentify()
{
	if(CActionSystem::GetMe()->GetDefaultAction()==CActionItem_MouseCmd_Identify::GetMe())
	{
		//已经进入鉴定状态
		return;
	}

	// 使用卷轴的背包索引位置
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		if(ICLASS_IDENT == pItem->GetItemClass())
		{
			int iType = pItem->GetItemTableType();
			if((iType >= COMITEM_WPIDENT)&&(iType <= COMITEM_NCIDENT))
			{
				CDataPool::GetMe()->m_iIdentifyInBagIndex = pItem->GetPosIndex();
				CDataPool::GetMe()->m_iIdentifyLevel	  = pItem->GetNeedLevel();

				GetItemImpl()->SetLock(TRUE);
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

				// 记录锁定位置。
				CDataPool::GetMe()->Identify_Lock(pItem->GetPosIndex());
			}
		}
	}
}

	
// 得到类型描述
LPCTSTR	CActionItem_Item::GetTypeDesc()
{
	// 使用卷轴的背包索引位置
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemTableTypeDesc();
	}

	return "";
}

// 得到坐骑增加速度   [7/21/2010 陈军龙]
INT	CActionItem_Item::GetMountAddtionSpeed()
{
	CObject_Item_Mount* pItemMount = (CObject_Item_Mount*)GetItemImpl();
	if(pItemMount)
	{
		return pItemMount->GetMountAddtionSpeed();
	}
	else
	{
		return 0;
	}

}

// 得到坐骑存在时限   [7/21/2010 陈军龙]
INT	CActionItem_Item::GetMountLife()
{
	CObject_Item_Mount* pItemMount = (CObject_Item_Mount*)GetItemImpl();
	if(pItemMount)
	{
		return pItemMount->GetMountLife();
	}
	else
	{
		return 0;
	}

}

// 得到坐骑获取时间   [7/21/2010 陈军龙]
INT	CActionItem_Item::GetMountObtainDate()
{
	CObject_Item_Mount* pItemMount = (CObject_Item_Mount*)GetItemImpl();
	if(pItemMount)
	{
		return pItemMount->GetMountObtainDate();
	}
	else
	{
		return 0;
	}

}

// 得到坐骑剩余使用时间   [7/21/2010 陈军龙]
INT	CActionItem_Item::GetMountRemainTime()
{
	CObject_Item_Mount* pItemMount = (CObject_Item_Mount*)GetItemImpl();
	if(pItemMount)
	{
		return pItemMount->GetMountRemainTime();
	}
	else
	{
		return 0;
	}

}

// 得到坐骑当前是否已绑定   [7/21/2010 陈军龙]
BOOL  CActionItem_Item::GetMountIsBinded()
{
	CObject_Item_Mount* pItemMount = (CObject_Item_Mount*)GetItemImpl();
	if(pItemMount)
	{
		return pItemMount->GetMountIsBinded();
	}
	else
	{
		return 0;
	}

}

// 得到坐骑绑定类型  [7/21/2010 陈军龙]
BOOL  CActionItem_Item::GetMountBindType()
{
	CObject_Item_Mount* pItemMount = (CObject_Item_Mount*)GetItemImpl();
	if(pItemMount)
	{
		return pItemMount->GetMountBindType();
	}
	else
	{
		return 0;
	}

}