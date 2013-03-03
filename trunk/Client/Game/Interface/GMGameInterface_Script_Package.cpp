
#include "StdAfx.h"
#include "GMGameInterface_Script_Package.h"
#include "GIException.h"
#include "LuaPlus.h"
#include "Action\GMActionSystem.h"
#include "GameDefine.h"
#include "Event/GMEventSystem.h"
#include "Network/NetManager.h"
#include "DataPool/GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\AI\GMAI_MySelf.h"
#include "DataPool\GMDP_CharacterData.h"
#include "procedure/GameProcedure.h"
#include "procedure/GamePro_Main.h"
#include "GMGameInterface.h"
 
#include "CGStallApply.h"
#include "CGStallOpen.h"
#include "CGSplitItem.h"
#include "CGPackUpPacket.h"
#include "CGAskLockObj.h"
#include "CGEquipLevelup.h"
#include "CGShopSpecialRepair.h"
#include "CGEquipRemould.h"
#include "CGEquipDecompound.h"
#include "CGEquipExChangePaiType.h"
#include "CGEquipChangePaiType.h"

#include "DataPool/GMUIDataPool.h"
#include "CGPackage_SwapItem.h"
#include "Object/Logic/Item/Obj_Item_Equip.h"
#include "Game\GIObjectManager.h"


namespace SCRIPT_SANDBOX
{

PlayerPackage PlayerPackage::s_PlayerPackage;
LuaPlus::LuaObject* PlayerPackage::s_pMetaTable = NULL;


INT PlayerPackage::EnumItem(LuaPlus::LuaState* state)
{
	const static STRING s_strBaseName		= "base";		//基本物品  0-19
	const static STRING s_strMaterialName	= "material";	//材料物品 20-39
	const static STRING s_strQuestName		= "quest";		//任务物品 40-59

	LuaStack args(state);

	//检查参数
	if (!(args[2].IsString()))
	{
		KLThrow("LUA: PlayerPackage::EnumItem Wrong Param1");
	}
	if (!(args[3].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EnumItem Wrong Param2");
	}

	STRING strPakageType = args[2].GetString();
	INT nIndex = args[3].GetInteger();
	
	//查询物品
	INT nOffIndex = -1;
	//if(strPakageType == s_strBaseName)
	//{
		nOffIndex = BASE_CONTAINER_OFFSET;
	//}
	/*else if(strPakageType == s_strMaterialName)
	{
		nOffIndex = MAT_CONTAINER_OFFSET;
	}
	else if(strPakageType == s_strQuestName)
	{
		nOffIndex = TASK_CONTAINER_OFFSET;
	}*/

	if(nOffIndex >= 0)
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex + nOffIndex, NAMETYPE_PACKAGEITEM);
		if(pActionItem)
		{
			LuaObject objReturn = state->BoxPointer(pActionItem);
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();

			if(((tObject_Item*)pActionItem->GetImpl())->IsLocked())
				state->PushInteger(1);
			else
				state->PushInteger(0);
			return 2;
		}
	}

	//非法ActionItem
	LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
	objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
	objReturn.PushStack();


	state->PushInteger(0);
	return 2;
}

// 打开摆摊（出售）界面
INT PlayerPackage::OpenStallSaleFrame(LuaPlus::LuaState* state)
{
	// 放在脚本中处理检测
//	BOOL isSetPasswd = CDataPool::GetMe()->Rich_IsSet( MINORPASSWD_INFO::PROTECT_STALL );
//	if( isSetPasswd )
//	{
//		CEventSystem::GetMe()->PushEvent( GE_RICH_CHECK_PASSWORD );
//		return;
//	}


	//判断是否处于骑乘状态或者变身状态，这样的状态不能摆摊
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_ModelID() != -1 )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_BianShen_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "变身状态，不能摆摊");
		return 0;
	}
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID() != -1 )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Ride_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "骑乘状态，不能摆摊");
		return 0;
	}
	//判断用户是不是处于自动跑动状态中，处于这样的状态不能摆摊
	if(	CGameProcedure::s_pProcMain->IsAutoRun() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_AutoRun_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "自动跑动状态，不能摆摊");
		return 0;
	}
	//宠物出战状态，不能摆摊
	if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_StallSale_Info_Pet_Out_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "宠物出战状态，不能摆摊");
		return 0;
	}		
	if( CObjectManager::GetMe()->GetMySelf()->IsMoving())   //2判断都可以
		//	if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE)  == CObject_Character::CHARACTER_LOGIC_MOVE)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_StallSale_Info_run_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "跑步状态，不能摆摊");
		return 0;
	}


	//需要先判定此用户现在是不是处于摆摊状态，
	if( CDataPool::GetMe()->MyStallBox_IsOpen() )
	{
		//当处于摆摊状态时 
		CGStallOpen msg;
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		CNetManager::GetMe()->SendPacket(&msg);
	}
	else
	{
		//组队跟随状态
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if(bTeamFollowMode)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Team_Follow_Can_Not_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"当前是组队跟随状态，不能摆摊");
			return 0;
		}
		// 普通跟随不能进行摆摊
		if(((CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI())->IsFollow())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Follow_Can_Not_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"当前是跟随状态，不能摆摊");
			return 0;
		}

		// 需要先判定是否玩家够30级
		if( 30 > CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Bellow_30_Level_Can_Not_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"需要等级30级你才能摆摊");
			return 0;
		}

		// 当处于非摆摊状态，先提交摆摊请求给服务器
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level()>=30)
		{
			CGStallApply msg;
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Bellow_30_Level_Can_Not_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"30级后才可以摆摊");
		}

	}

	return 0;
}

//--------------------------------------------------------
// 拆分物品界面使用的接口函数
INT PlayerPackage::GetSplitSum(LuaPlus::LuaState* state)
{
	state->PushInteger(CDataPool::GetMe()->Split_GetSum());
	return 1;
}

// 处理点击确定拆分后的处理
INT PlayerPackage::SplitItem(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::SplitItem Wrong Param1");
	}
	INT nNumber = args[2].GetInteger();

	CDataPool::GetMe()->Split_SetNumber(nNumber);


	CGSplitItem msg;
	if( CDataPool::GetMe()->Split_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetContainer(CGSplitItem::BAG_CON);
	}
	else if(CDataPool::GetMe()->Split_GetItemConta() == tObject_Item::IO_MYSELF_BANK )
	{
		msg.SetContainer(CGSplitItem::BANK_CON);
	}
	msg.SetNumber(nNumber);
	msg.SetPosition(CDataPool::GetMe()->Split_GetItemPos());

	CNetManager::GetMe()->SendPacket(&msg);
	return 0;
}

//获得等待拆分物品的名字
INT	PlayerPackage::GetSplitName(LuaPlus::LuaState* state)
{
	INT nCon = CDataPool::GetMe()->Split_GetItemConta();
	INT nPos = CDataPool::GetMe()->Split_GetItemPos();

	if(nCon == -1 || nPos == -1)
	{
		state->PushString("");
		return 1;
	}
	
	tObject_Item* pItem;

	switch(nCon)
	{
	case tObject_Item::IO_MYSELF_PACKET:
		{
			pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:
		{
			pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);
		}
		break;
	default:
		break;
	}

	if(pItem)
		state->PushString(pItem->GetName());
	else
		state->PushString("");

	return 1;
}

// 取消物品拆分
INT PlayerPackage::CancelSplitItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->Split_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Split_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
		}
		break;
	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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

	return 0;
}
// 取消物品打孔
INT PlayerPackage::CancelAddHoleItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->AddHole_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->AddHole_GetItemPos();
    
	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
	 
	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
	CDataPool::GetMe()->AddHole_CleanUP();
	 
	return 0;
}

// 处理点击确定打孔后的处理
INT PlayerPackage::	AddHoleItem(LuaPlus::LuaState* state)
{
	//设置相关npc
	 
	CDataPool::GetMe()->AddHole_SetNpcId(  CUIDataPool::GetMe()->GetCurDialogNpcId());

	if( CDataPool::GetMe()->AddHole_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		CGameInterface::GetMe()->PacketItem_AddHole();
		
	}
	else if(CDataPool::GetMe()->AddHole_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP )
	{

		CGameInterface::GetMe()->HumanEquip_AddHole( );
	}
	 

	return 0;
}

// 处理点击确定绑定后的处理
INT PlayerPackage::	Bind_Equip(LuaPlus::LuaState* state)
{

 
	if( CDataPool::GetMe()->Bind_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		

		CGameInterface::GetMe()->PacketItem_BangDing();


	}
	else if(CDataPool::GetMe()->Bind_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP )
	{

		CGameInterface::GetMe()->HumanEquip_BangDing();
	}

	 

	return 0;
}



// 取消物品绑定
INT PlayerPackage::CancelBindItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->Bind_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Bind_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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

	INT  nITEM =  CDataPool::GetMe()->Bind_GetNeedItemPos();

	tObject_Item* pItemNeed = CDataPool::GetMe()->UserBag_GetItem(nITEM);

	if(pItemNeed)
	{
		pItemNeed->SetLock(FALSE);
	}


	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);




	CDataPool::GetMe()->Bind_CleanUP();
	 
	return 0;
}
	


// 取消物品打星
INT PlayerPackage::Cancel_AddStart_Equip(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->AddStar_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->AddStar_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
	CDataPool::GetMe()->AddStar_CleanUP(); 

	return 0;
}


// 取消物品特修
INT PlayerPackage::Cancel_SpecialRepair_Equip(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->SpRepair_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->SpRepair_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
	CDataPool::GetMe()->SpRepair_CleanUP(); 

	return 0;
}



// 处理点击确定打星后的处理
INT PlayerPackage::AddStar_Equip(LuaPlus::LuaState* state)  
{
	LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))       // 宝石的数目 
	{
		KLThrow("LUA: PlayerPackage::AddStart_Equip Wrong Param1");
	}

	INT nNumber = args[2].GetInteger();

	tObject_Item* pItemBagGem = NULL;
	//客户端不做预先的判断了， 保留这段，以后需要的时候再打开

	//INT nItemNum  = 0;
	//for ( INT i =0 ;i<  MAX_BAG_SIZE ;++i)
	//{
	//	pItemBagGem =  CDataPool::GetMe()->UserBag_GetItem(  i );
	//	if ( pItemBagGem)
	//	{
	//		if (  pItemBagGem->GetIdTable() == CDataPool::GetMe()->AddStar_GetGemId() )
	//		{

	//			nItemNum += pItemBagGem->GetNumber();
	//		}
	//	}

	//}
	//if(nNumber > nItemNum)

	//{
	//	STRING strTemp = "玩家宝石数量不足";
	//
	//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
	//	 
	//	return 0;
	//}

	CGEquipLevelup msg;
	tObject_Item* pItem = NULL;

	CDataPool::GetMe()->AddStar_SetGemNum(nNumber);


	if( CDataPool::GetMe()->AddStar_GetItemConta()== tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetEquipPosType(ENUM_BAG);  
	    pItem = CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->AddStar_GetItemPos());
		 
		 

	}
	else if(CDataPool::GetMe()->AddStar_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP )
	{
		 msg.SetEquipPosType(ENUM_EQUIP_POINT);  
		 pItem = CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP( CDataPool::GetMe()->AddStar_GetItemPos()));
		
	}
	if(pItem)

	{
		_ITEM_GUID  sDataGuid;
		WORD wServer, wWorld;
		GUID_t dwSerial;
		pItem->GetGUID(wWorld, wServer, dwSerial);

		sDataGuid.m_Serial	= dwSerial;
		sDataGuid.m_Server	= (BYTE)wServer;
		sDataGuid.m_World	= (BYTE)wWorld;

	   
		msg.SetPos( CDataPool::GetMe()->AddStar_GetItemPos());
		msg.setGemID(CDataPool::GetMe()->AddStar_GetGemId());
		msg.SetGemNum(CDataPool::GetMe()->AddStar_GetGemNum());
		msg.setItemID(sDataGuid);

		CNetManager::GetMe()->SendPacket(&msg);

	}

	return 0;
}
//
// 处理点击确定特修后的处理
INT PlayerPackage::SpecialRepair_Equip(LuaPlus::LuaState* state)  
{
	LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))       // 材料的数目 
	{
		KLThrow("LUA: PlayerPackage::AddStart_Equip Wrong Param1");
	}

	//说明已经执行过一次了,目前不需要修理了
	if (CDataPool::GetMe()->SpRepair_GetNeedItemId() <= 0)
	{
		return 0;
	}

	INT nNumber = args[2].GetInteger();

	tObject_Item* pItemBagGem = NULL;

	INT nItemNum  = 0;
	for ( INT i =0 ;i<  MAX_BAG_SIZE ;++i)
	{
		pItemBagGem =  CDataPool::GetMe()->UserBag_GetItem(  i );
		if ( pItemBagGem)
		{
			if (  pItemBagGem->GetIdTable() == CDataPool::GetMe()->SpRepair_GetNeedItemId() )
			{

				nItemNum += pItemBagGem->GetNumber();
			}
		}

	}
	if(nNumber > nItemNum)

	{
		STRING strTemp = "玩家材料数量不足";

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

		return 0;
	}

	CGShopSpecialRepair msg;
	tObject_Item* pItem = NULL;

	CDataPool::GetMe()->SpRepair_SetNeedItemNum(nNumber);


	if( CDataPool::GetMe()->SpRepair_GetItemConta()== tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetOpt(ENUM_BAG);  
		pItem = CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->SpRepair_GetItemPos());



	}
	else if(CDataPool::GetMe()->SpRepair_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP )
	{
		msg.SetOpt(ENUM_EQUIP_POINT);  
		pItem = CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP( CDataPool::GetMe()->SpRepair_GetItemPos()));

	}
	if(pItem)

	{ 
		 
  
		msg.SetBagIndex( CDataPool::GetMe()->SpRepair_GetItemPos());
		 
		msg.SetItemNum(CDataPool::GetMe()->SpRepair_GetNeedItemNum());
		 

		CNetManager::GetMe()->SendPacket(&msg);

	}

	return 0;
}


// 处理点击确定修理
INT PlayerPackage::SureRepair_Equip(LuaPlus::LuaState* state)  

{

	// CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex(), FALSE);
	
	 CGameProcedure::s_pGameInterface->Booth_Repair(0, CDataPool::GetMe()->ComRepair_GetItemPos(), CDataPool::GetMe()->ComRepair_GetItemConta());
	 return 0;

}
INT  PlayerPackage::EquipDismantleSure(LuaPlus::LuaState* state)  

{

	CGEquipDecompound  msg;
	tObject_Item* pItem  = NULL ;

	if( CDataPool::GetMe()->Dismantle_GetItemConta()== tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetEquipPosType(ENUM_BAG);  
		pItem = CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->Dismantle_GetItemPos());


	}
	else if(CDataPool::GetMe()->Dismantle_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP )
	{
		msg.SetEquipPosType(ENUM_EQUIP_POINT);  
		pItem = CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP( CDataPool::GetMe()->Dismantle_GetItemPos()));

	}
	if(pItem)

	{ 

		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;


		msg.SetPos(CDataPool::GetMe()->Dismantle_GetItemPos());
		msg.setItemID(Guid);

		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId());
		CNetManager::GetMe()->SendPacket(&msg);


	}
  return 0;

}

// 处理点击确定改造装备按钮
INT PlayerPackage::ReWorkEquip(LuaPlus::LuaState* state)  

{

	LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))       // 材料的数目 
	{
		KLThrow("LUA: PlayerPackage::ReWorkEquip Wrong Param1");
	}
	if (!(args[3].IsInteger()))       // 材料的数目 
	{
		KLThrow("LUA: PlayerPackage::ReWorkEquip Wrong Param2");
	}
	if (!(args[4].IsInteger()))       // 材料的数目 
	{
		KLThrow("LUA: PlayerPackage::ReWorkEquip Wrong Param3");
	}


	INT Grad[MAX_EQUIP_RWMATL_NUM];

	for ( INT i = 0 ;i< MAX_EQUIP_RWMATL_NUM ;i++)
	{
		Grad[i]     =  args[i+2].GetInteger();
	}


	CGameInterface::GetMe()->EquipRework(Grad );

		return 0;

}
//确定交换装备牌型
INT PlayerPackage::SureSwapCarditem(LuaPlus::LuaState* state)  

{
	CGEquipExChangePaiType  msg;
	tObject_Item* pItem1  = NULL ;
	tObject_Item* pItem2  = NULL ;
	if( CDataPool::GetMe()->SwapCard_GetItemConta1() == tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetEquip1PosType(ENUM_BAG);  
		pItem1 = CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->SwapCard_GetItemPos1());


	}
	else if(CDataPool::GetMe()->SwapCard_GetItemConta1() == tObject_Item::IO_MYSELF_EQUIP )
	{
		msg.SetEquip1PosType(ENUM_EQUIP_POINT);  
		pItem1 = CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP( CDataPool::GetMe()->SwapCard_GetItemPos1()));

	}

	if( CDataPool::GetMe()->SwapCard_GetItemConta2() == tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetEquip2PosType(ENUM_BAG);  
		pItem2 = CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->SwapCard_GetItemPos2());


	}
	else if(CDataPool::GetMe()->SwapCard_GetItemConta2() == tObject_Item::IO_MYSELF_EQUIP )
	{
		msg.SetEquip1PosType(ENUM_EQUIP_POINT);  
		pItem2 = CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP( CDataPool::GetMe()->SwapCard_GetItemPos2()));

	}

     if( !pItem1 || !pItem2 )
	 {

		 STRING strTemp = "缺少交换牌型的相应的装备";

		 CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

        
		 return 0;

	 }
	 if (CDataPool::GetMe()->SwapCard_GetNeedItemPos() == -1)
	 {
		 STRING strTemp = "缺少交换牌型的相应的道具";

		 CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());


		 return 0;
	 }

	if(pItem1 &&  pItem2 )

	{ 

		//装备1的guid 和位置
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem1->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;
		msg.SetPos1(CDataPool::GetMe()->SwapCard_GetItemPos1());
		msg.setItem1ID(Guid);

		//装备1的guid 和位置

		pItem2->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;
		msg.SetPos2(CDataPool::GetMe()->SwapCard_GetItemPos2());
		msg.setItem2ID(Guid);

       //道具的位置和相关npc
		msg.setGemBagPos(CDataPool::GetMe()->SwapCard_GetNeedItemPos());
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId());
		CNetManager::GetMe()->SendPacket(&msg);


	} 
 

	return 0;
}
//确定改造装备牌型
INT PlayerPackage::SureChangeCarditem(LuaPlus::LuaState* state)  

{
	
	CGEquipChangePaiType  msg;
	tObject_Item* pItem  = NULL ;
 
	if( CDataPool::GetMe()->ChangeCard_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetEquipPosType(ENUM_BAG);  
		pItem = CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->ChangeCard_GetItemPos());


	}
	else if(CDataPool::GetMe()->ChangeCard_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP )
	{
		msg.SetEquipPosType(ENUM_EQUIP_POINT);  
		pItem = CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP( CDataPool::GetMe()->ChangeCard_GetItemPos()));

	}

 
  

	if (CDataPool::GetMe()->ChangeCard_GetNeedItemPos() == -1)
	{
		STRING strTemp = "缺少改造牌型的相应的道具";

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

		return 0;
	}

	if(pItem )

	{ 

		//装备的guid 和位置
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;
		msg.SetPos(CDataPool::GetMe()->ChangeCard_GetItemPos());
		msg.setItemID(Guid);

	 
		//道具的位置和相关npc
		msg.setGemBagPos(CDataPool::GetMe()->ChangeCard_GetNeedItemPos());
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId());
		CNetManager::GetMe()->SendPacket(&msg);


	} else
	{
		STRING strTemp = "请放上需要改造牌型的装备";

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

		return 0;
	}




	return 0;
}


//  处理点击取消拆解装备按钮
INT PlayerPackage::EquipDismantleCannel(LuaPlus::LuaState* state)  

{

	INT nTypeOwner	=	CDataPool::GetMe()->Dismantle_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Dismantle_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
	CDataPool::GetMe()->Rework_CleanUP(); 

	return 0;


}

//  处理点击取消改造装备按钮
INT PlayerPackage::ReWorkEquipCannel(LuaPlus::LuaState* state)  
{
	INT nTypeOwner	=	CDataPool::GetMe()->Rework_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Rework_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
	CDataPool::GetMe()->Rework_CleanUP(); 

	return 0;


}


//取消改造装备牌型
INT PlayerPackage::CancelChangeCarditem(LuaPlus::LuaState* state)  

{


	INT nTypeOwner	=	CDataPool::GetMe()->ChangeCard_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->ChangeCard_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
 
	INT  nITEM =  CDataPool::GetMe()->ChangeCard_GetNeedItemPos();

	tObject_Item* pItemNeed = CDataPool::GetMe()->UserBag_GetItem(nITEM);

	if(pItemNeed)
	{
		pItemNeed->SetLock(FALSE);
	}


	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

	CDataPool::GetMe()->ChangeCard_CleanUP(); 
 return 0;

}


//取消交换装备牌型
INT PlayerPackage::CancelSwapCarditem(LuaPlus::LuaState* state)  
{
	INT nTypeOwner	=	-1;
	INT	nItemPos	=	-1;

	for (INT i = 0 ; i < 2 ;++i)
	{
		if ( 0 == i)
		{
			nTypeOwner	=	CDataPool::GetMe()->SwapCard_GetItemConta1();
			nItemPos	    =	CDataPool::GetMe()->SwapCard_GetItemPos1();
		}else if (  1 == i)
		{
			nTypeOwner	=	CDataPool::GetMe()->SwapCard_GetItemConta2();
			nItemPos	    =	CDataPool::GetMe()->SwapCard_GetItemPos2();
		}
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;

			case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
			
	}

	INT  nITEM =  CDataPool::GetMe()->SwapCard_GetNeedItemPos();

	tObject_Item* pItemNeed = CDataPool::GetMe()->UserBag_GetItem(nITEM);

	if(pItemNeed)
	{
		pItemNeed->SetLock(FALSE);
	}


	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

	CDataPool::GetMe()->SwapCard_CleanUP(); 

	return 0;


}



// 处理点击确定合成装备按钮
INT PlayerPackage::EquipSynthesize_Sure(LuaPlus::LuaState* state)  

{
	INT i =0;

	//for ( i = 0;i< MAX_BAG_SIZE ;++i)
	//{
	//	
	//	if	( ! CDataPool::GetMe()->UserBag_GetItem(i) )

	//		break ;
	//}
 // if ( i == MAX_BAG_SIZE)
 // {

	//  CGameProcedure::s_pGfxSystem->PushDebugString("背包空间不足，至少有一个空格");
	//  STRING strTemp = "背包空间不足，至少有一个空格";
	//  CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
	//	return 0;
 // }
	//

	LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param1");      //配方id
	}
	//检查参数
	if (!(args[3].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param2");      //材料1等级
	}
	//检查参数
	if (!(args[4].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param3");		//材料2等级
	}

	//检查参数
	if (!(args[5].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param4");		//材料3等级

	}

	//检查参数
	if (!(args[6].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param5");		//材料4等级
	}


   INT Grad[MAX_EQUIP_PROMATL_NUM];

   for ( INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM ;i++)
   {
	   Grad[i]     =  args[i+3].GetInteger();
   }

 	CGameInterface::GetMe()->EquipSynthesize(args[2].GetInteger() ,Grad );


	return 0;

}


//整理物品
INT PlayerPackage::PackUpPacket(LuaPlus::LuaState* state)
{
	//if( CGameInterface::GetMe()->IsInHell()) return 0;

	LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::PackUpPacket Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();
	if( nIndex != 0 && nIndex != 1 && nIndex != 2 && nIndex != 3 ) 
	{
		KLThrow("LUA: PlayerPackage::PackUpPacket Wrong Param1" );
	}
	INT nBeginIndex = 0;//nIndex * 20; 
	INT nEndIndex	= 0;//nIndex * 20 + 19;
	if( nIndex == 0 )
	{
		nBeginIndex = BASE_CONTAINER_OFFSET;
		nEndIndex = BASE_CONTAINER_OFFSET + EXTRA_BAG1_OFFSET - 1;
	}
	else if( nIndex == 1 )
	{
		nBeginIndex = EXTRA_BAG1_OFFSET;
		nEndIndex = EXTRA_BAG1_OFFSET + + EXTRA_BAG1_OFFSET - 1;
	}
	else if( nIndex == 2 )
	{
		nBeginIndex = EXTRA_BAG3_OFFSET;
		nEndIndex = EXTRA_BAG3_OFFSET + + EXTRA_BAG1_OFFSET - 1;
	}
	else if( nIndex == 3 )
	{
		nBeginIndex = EXTRA_BAG4_OFFSET;
		nEndIndex = EXTRA_BAG4_OFFSET + + EXTRA_BAG1_OFFSET - 1;
	}

	// 需要检测在这些状态下，不让整理，  摆摊，交易，提交任务，打开玩家商店界面，使用物品
	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊，不能整理背包");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_2");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"交易中不能整理背包");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_3");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"现在不能整理背包");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_4");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"现在不能整理背包");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_5");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}

	// 在这里需要将整个背包先锁定
	//CEventSystem::GetMe()->PushEvent(GE_BEGIN_PACKUP_PACKET);

	// 先Clone背包
	CDataPool::GetMe()->UserBagClone_Clone();


	// 需要一个复制的容器来进行模拟,这个容器记录了物品的指针，所以，不能在过程中调用Item 的 Set函数
	// 使用在客户端模拟玩家移动物品的方式操作
	// 第一步。叠加所有能叠加的物品
	for( INT i=nBeginIndex; i<=nEndIndex; i++ )
	{
		tObject_Item* pItem1	= CDataPool::GetMe()->UserBagClone_GetItem(i)->m_pItem;
		INT nItemNum1	= CDataPool::GetMe()->UserBagClone_GetItem(i)->m_nItemNum;

		if(pItem1)
			if(pItem1->Rule(tObject_Item::RULE_OVERLAY))			// 物品规则可以叠加
				if(!pItem1->IsLocked())						// 没有被锁定
					if(!pItem1->GetByProtect())						// 没有被二级密码保护
					{
						for( INT j=nBeginIndex; j<=nEndIndex; j++ )
						{
							if(i != j)
							{
								pItem1	= CDataPool::GetMe()->UserBagClone_GetItem(i)->m_pItem;
								nItemNum1			= CDataPool::GetMe()->UserBagClone_GetItem(i)->m_nItemNum;

								tObject_Item* pItem2	= CDataPool::GetMe()->UserBagClone_GetItem(j)->m_pItem;
								INT nItemNum2	= CDataPool::GetMe()->UserBagClone_GetItem(j)->m_nItemNum;

								if(pItem1)
									if(pItem2)
										if(!pItem1->GetByProtect())
											if(!pItem1->IsLocked())
												if(!pItem2->GetByProtect())
													if(!pItem2->IsLocked())
														if( pItem1->GetIdTable() == pItem2->GetIdTable() )		// 两个物品可以叠加
														{
															if(nItemNum1 < pItem1->GetLayNum())
															{
																if(nItemNum2 < pItem2->GetLayNum())
																{
																	// 发送移动物品（合并物品）的消息
																	CGPackage_SwapItem msg;
																	msg.SetPackageIndex1(j);
																	msg.SetPackageIndex2(i);

																	CNetManager::GetMe()->SendPacket(&msg);

																	// 模拟结果
																	if(nItemNum1 + nItemNum2 <= pItem2->GetLayNum())
																	{
																		CDataPool::USER_BAG_CLONE		CloneObj;
																		CloneObj.m_nItemNum = nItemNum1 + nItemNum2;
																		CloneObj.m_pItem	= pItem1;
																		CDataPool::GetMe()->UserBagClone_SetItem(i,&CloneObj);

																		CloneObj.m_nItemNum = 0;
																		CloneObj.m_pItem	= NULL;
																		CDataPool::GetMe()->UserBagClone_SetItem(j,&CloneObj);
																	}
																	else
																	{
																		CDataPool::USER_BAG_CLONE		CloneObj;
																		CloneObj.m_nItemNum = pItem1->GetLayNum();
																		CloneObj.m_pItem	= pItem1;
																		CDataPool::GetMe()->UserBagClone_SetItem(i,&CloneObj);

																		CloneObj.m_nItemNum = nItemNum1 + nItemNum2 - pItem2->GetNumber();
																		CloneObj.m_pItem	= pItem2;
																		CDataPool::GetMe()->UserBagClone_SetItem(j,&CloneObj);
																	}
																}
															}
														}
							}
						}
					}
	}
	//叠加完成

	// 开始调整位置，现在已经可以不用关心物品的数量了
	for(INT i=nBeginIndex; i<=nEndIndex; i++)
	{
		tObject_Item*  pItem1 = 0;
		tObject_Item*  pItem2 = 0;

		INT nTempIndex = i;

		for(INT j=i; j<=nEndIndex; j++)
		{
			pItem1	= CDataPool::GetMe()->UserBagClone_GetItem(nTempIndex)->m_pItem;
			pItem2	= CDataPool::GetMe()->UserBagClone_GetItem(j)->m_pItem;

			// 将最高优先级的物品放入这个位置，
			if( i != j )
				if( IsSwapItem(pItem1, pItem2) )
				{
					nTempIndex = j;
				}
		}
		if( nTempIndex != i )
		{
			// 发交换消息
			CGPackage_SwapItem msg;
			msg.SetPackageIndex1(nTempIndex);
			msg.SetPackageIndex2(i);

			CNetManager::GetMe()->SendPacket(&msg);

			// 客户端同步模拟
			tObject_Item*  pItem1 = CDataPool::GetMe()->UserBagClone_GetItem(i)->m_pItem;
			CDataPool::USER_BAG_CLONE		CloneObj;
			CloneObj.m_pItem	= pItem1;
			CDataPool::GetMe()->UserBagClone_SetItem(nTempIndex,&CloneObj);

			CloneObj.m_pItem	= pItem2;
			CDataPool::GetMe()->UserBagClone_SetItem(i,&CloneObj);
		}
	}

	// 现在这个消息的目的是在客户端完成整理的模拟后，发一个结尾性的消息给服务器
	CGPackUpPacket msg;
	msg.SetConIndex(nIndex);

	CNetManager::GetMe()->SendPacket(&msg);

	// 通知整理按钮变灰
	CEventSystem::GetMe()->PushEvent(GE_BEGIN_PACKUP_PACKET);

	return 0;
}


//整理物品
INT PlayerPackage::OpenPetList(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);

	return 0;
}
//打开锁定界面
INT PlayerPackage::OpenLockFrame(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::OpenLockFrame Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();

	CEventSystem::GetMe()->PushEvent(GE_OPEN_ITEM_COFFER, nIndex);
	return 0;
}
//锁定物品（宠物）
INT PlayerPackage::Lock(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//检查参数
	if (!(args[2].IsString()))
	{
		KLThrow("LUA: PlayerPackage::Lock[2] Wrong Param1");
	}
	if (!(args[3].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::Lock[3] Wrong Param1");
	}

	std::string str = args[2].GetString();
	INT nIndex = args[3].GetInteger();

	if( str == "lock" )
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup())
		{
			//进行物品的加密设置过程,发送锁定物品的消息到服务器
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//检测这个物品是不是已经锁定
			if(pItem->GetByProtect())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品已经加密");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGAskLockObj msg;
			msg.SetLockObj(CGAskLockObj::LOCK_ITEM);
			msg.SetLockType(CGAskLockObj::OPR_LOCK);
			msg.SetItemGUID(Guid);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//什么也不用我做，朗完成 －－Add By hongyu
		}
	}
	// 解除锁定
	else	
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdUnlocked())
		{
			//进行物品的加密设置过程,发送锁定物品的消息到服务器
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//检测这个物品是不是已经锁定
			if( !pItem->GetByProtect() )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品没有加密");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGAskLockObj msg;
			msg.SetLockObj(CGAskLockObj::LOCK_ITEM);
			msg.SetLockType(CGAskLockObj::OPR_UNLOCK);
			msg.SetItemGUID(Guid);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//什么也不用我做，朗完成 －－Add By hongyu
		}
	}
	return 0;
}
//获得物品是否锁定
INT PlayerPackage::IsLock(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::IsLock[2] Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();
	tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);

	if(!pItem)
	{
		state->PushInteger(0);
		return 1;
	}
	if(pItem->GetByProtect())
	{
		state->PushInteger(1);
	}
	else
	{
		state->PushInteger(0);
	}

	return 1;
}

//物品链接
INT PlayerPackage::ItemLink(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::ItemLink[2] Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();
	tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);

	if(pItem)
	{
		CEventSystem::GetMe()->PushEvent(GE_ITEM_LINK, pItem->GetName());
		return 1;
	}

	return 1;
}


//----------------------------------------------------------------------------
// 非接口函数
//----------------------------------------------------------------------------

// 比较两个物品，看看是不是应该交换位置，如果交换，pItem2就排到前面
BOOL PlayerPackage::IsSwapItem(tObject_Item* pItem1, tObject_Item* pItem2)
{
	// 1、替换空位
	if(pItem1 == NULL)
	{
		if(pItem2)
			return TRUE;
		else
			return FALSE;
	}
	if(pItem2 == NULL)
	{
		return FALSE;
	}

	if(pItem1->IsLocked() || pItem2->IsLocked())
		return FALSE;

	// 2、
	if(pItem1->GetItemClass() != pItem2->GetItemClass())
	{
		// 不同大类的物品
		if(pItem1->GetItemClass() > pItem2->GetItemClass())
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		// 同大类的物品
		switch(pItem1->GetItemClass())
		{
		case ICLASS_EQUIP:			//武器WEAPON、防具DEFENCE、饰物ADORN
			{
				if( ((CObject_Item_Equip*)pItem1)->GetItemType() != ((CObject_Item_Equip*)pItem2)->GetItemType() )
				{
					if( ((CObject_Item_Equip*)pItem1)->GetItemType() > ((CObject_Item_Equip*)pItem2)->GetItemType() )
						return TRUE;
					else
						return FALSE;
				}
				else
				{
					if( ((CObject_Item_Equip*)pItem1)->GetItemType() == HEQUIP_MAINHAND)  // 武器
					{
						if( ((CObject_Item_Equip*)pItem1)->GetItemType() != ((CObject_Item_Equip*)pItem2)->GetItemType() )
						{
							if( ((CObject_Item_Equip*)pItem1)->GetItemType() > ((CObject_Item_Equip*)pItem2)->GetItemType() )
								return TRUE;
							else
								return FALSE;
						}
						else
						{
							//看等级了
							if( ((CObject_Item_Equip*)pItem1)->GetNeedLevel() < ((CObject_Item_Equip*)pItem2)->GetNeedLevel() )
								return TRUE;
							else
								return FALSE;
						}
					}
					else
					{
						//看等级了
						if( ((CObject_Item_Equip*)pItem1)->GetNeedLevel() < ((CObject_Item_Equip*)pItem2)->GetNeedLevel() )
							return TRUE;
						else
							return FALSE;
					}

				}
			}
			break;
		case ICLASS_MATERIAL:		//原料
		case ICLASS_COMITEM	:		//药品
		case ICLASS_TASKITEM:		//任务物品
		case ICLASS_GEM:			//宝石
		case ICLASS_STOREMAP:		//藏宝图
		case ICLASS_TALISMAN:		//法宝---???
		case ICLASS_GUILDITEM:		//帮会物品
		case ICLASS_IDENT:			//鉴定符
			{
				if( pItem1->GetIdTable() != pItem2->GetIdTable() )
				{
					if(pItem1->GetIdTable() < pItem2->GetIdTable())
						return TRUE;
					else
						return FALSE;
				}
				else
				{
					//看等级了
					if( pItem1->GetNeedLevel() < pItem2->GetNeedLevel() )
						return TRUE;
					else
						return FALSE;
				}
			}
			break;
		default :
			break;
		}

	}
	return FALSE;
}


}
