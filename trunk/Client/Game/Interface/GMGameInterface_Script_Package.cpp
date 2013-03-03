
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
	const static STRING s_strBaseName		= "base";		//������Ʒ  0-19
	const static STRING s_strMaterialName	= "material";	//������Ʒ 20-39
	const static STRING s_strQuestName		= "quest";		//������Ʒ 40-59

	LuaStack args(state);

	//������
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
	
	//��ѯ��Ʒ
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

	//�Ƿ�ActionItem
	LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
	objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
	objReturn.PushStack();


	state->PushInteger(0);
	return 2;
}

// �򿪰�̯�����ۣ�����
INT PlayerPackage::OpenStallSaleFrame(LuaPlus::LuaState* state)
{
	// ���ڽű��д�����
//	BOOL isSetPasswd = CDataPool::GetMe()->Rich_IsSet( MINORPASSWD_INFO::PROTECT_STALL );
//	if( isSetPasswd )
//	{
//		CEventSystem::GetMe()->PushEvent( GE_RICH_CHECK_PASSWORD );
//		return;
//	}


	//�ж��Ƿ������״̬���߱���״̬��������״̬���ܰ�̯
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_ModelID() != -1 )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_BianShen_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����״̬�����ܰ�̯");
		return 0;
	}
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID() != -1 )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Ride_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���״̬�����ܰ�̯");
		return 0;
	}
	//�ж��û��ǲ��Ǵ����Զ��ܶ�״̬�У�����������״̬���ܰ�̯
	if(	CGameProcedure::s_pProcMain->IsAutoRun() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_AutoRun_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�Զ��ܶ�״̬�����ܰ�̯");
		return 0;
	}
	//�����ս״̬�����ܰ�̯
	if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_StallSale_Info_Pet_Out_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�����ս״̬�����ܰ�̯");
		return 0;
	}		
	if( CObjectManager::GetMe()->GetMySelf()->IsMoving())   //2�ж϶�����
		//	if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE)  == CObject_Character::CHARACTER_LOGIC_MOVE)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_StallSale_Info_run_Can_Not_Stall");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�ܲ�״̬�����ܰ�̯");
		return 0;
	}


	//��Ҫ���ж����û������ǲ��Ǵ��ڰ�̯״̬��
	if( CDataPool::GetMe()->MyStallBox_IsOpen() )
	{
		//�����ڰ�̯״̬ʱ 
		CGStallOpen msg;
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		CNetManager::GetMe()->SendPacket(&msg);
	}
	else
	{
		//��Ӹ���״̬
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if(bTeamFollowMode)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Team_Follow_Can_Not_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��ǰ����Ӹ���״̬�����ܰ�̯");
			return 0;
		}
		// ��ͨ���治�ܽ��а�̯
		if(((CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI())->IsFollow())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Follow_Can_Not_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��ǰ�Ǹ���״̬�����ܰ�̯");
			return 0;
		}

		// ��Ҫ���ж��Ƿ���ҹ�30��
		if( 30 > CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Package_Info_Bellow_30_Level_Can_Not_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ҫ�ȼ�30������ܰ�̯");
			return 0;
		}

		// �����ڷǰ�̯״̬�����ύ��̯�����������
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
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"30����ſ��԰�̯");
		}

	}

	return 0;
}

//--------------------------------------------------------
// �����Ʒ����ʹ�õĽӿں���
INT PlayerPackage::GetSplitSum(LuaPlus::LuaState* state)
{
	state->PushInteger(CDataPool::GetMe()->Split_GetSum());
	return 1;
}

// ������ȷ����ֺ�Ĵ���
INT PlayerPackage::SplitItem(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
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

//��õȴ������Ʒ������
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

// ȡ����Ʒ���
INT PlayerPackage::CancelSplitItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->Split_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Split_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
		}
		break;
	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}
// ȡ����Ʒ���
INT PlayerPackage::CancelAddHoleItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->AddHole_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->AddHole_GetItemPos();
    
	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
	 
	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
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

// ������ȷ����׺�Ĵ���
INT PlayerPackage::	AddHoleItem(LuaPlus::LuaState* state)
{
	//�������npc
	 
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

// ������ȷ���󶨺�Ĵ���
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



// ȡ����Ʒ��
INT PlayerPackage::CancelBindItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->Bind_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Bind_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
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
	


// ȡ����Ʒ����
INT PlayerPackage::Cancel_AddStart_Equip(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->AddStar_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->AddStar_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
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


// ȡ����Ʒ����
INT PlayerPackage::Cancel_SpecialRepair_Equip(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->SpRepair_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->SpRepair_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
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



// ������ȷ�����Ǻ�Ĵ���
INT PlayerPackage::AddStar_Equip(LuaPlus::LuaState* state)  
{
	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))       // ��ʯ����Ŀ 
	{
		KLThrow("LUA: PlayerPackage::AddStart_Equip Wrong Param1");
	}

	INT nNumber = args[2].GetInteger();

	tObject_Item* pItemBagGem = NULL;
	//�ͻ��˲���Ԥ�ȵ��ж��ˣ� ������Σ��Ժ���Ҫ��ʱ���ٴ�

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
	//	STRING strTemp = "��ұ�ʯ��������";
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
// ������ȷ�����޺�Ĵ���
INT PlayerPackage::SpecialRepair_Equip(LuaPlus::LuaState* state)  
{
	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))       // ���ϵ���Ŀ 
	{
		KLThrow("LUA: PlayerPackage::AddStart_Equip Wrong Param1");
	}

	//˵���Ѿ�ִ�й�һ����,Ŀǰ����Ҫ������
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
		STRING strTemp = "��Ҳ�����������";

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


// ������ȷ������
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

// ������ȷ������װ����ť
INT PlayerPackage::ReWorkEquip(LuaPlus::LuaState* state)  

{

	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))       // ���ϵ���Ŀ 
	{
		KLThrow("LUA: PlayerPackage::ReWorkEquip Wrong Param1");
	}
	if (!(args[3].IsInteger()))       // ���ϵ���Ŀ 
	{
		KLThrow("LUA: PlayerPackage::ReWorkEquip Wrong Param2");
	}
	if (!(args[4].IsInteger()))       // ���ϵ���Ŀ 
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
//ȷ������װ������
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

		 STRING strTemp = "ȱ�ٽ������͵���Ӧ��װ��";

		 CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

        
		 return 0;

	 }
	 if (CDataPool::GetMe()->SwapCard_GetNeedItemPos() == -1)
	 {
		 STRING strTemp = "ȱ�ٽ������͵���Ӧ�ĵ���";

		 CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());


		 return 0;
	 }

	if(pItem1 &&  pItem2 )

	{ 

		//װ��1��guid ��λ��
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem1->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;
		msg.SetPos1(CDataPool::GetMe()->SwapCard_GetItemPos1());
		msg.setItem1ID(Guid);

		//װ��1��guid ��λ��

		pItem2->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;
		msg.SetPos2(CDataPool::GetMe()->SwapCard_GetItemPos2());
		msg.setItem2ID(Guid);

       //���ߵ�λ�ú����npc
		msg.setGemBagPos(CDataPool::GetMe()->SwapCard_GetNeedItemPos());
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId());
		CNetManager::GetMe()->SendPacket(&msg);


	} 
 

	return 0;
}
//ȷ������װ������
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
		STRING strTemp = "ȱ�ٸ������͵���Ӧ�ĵ���";

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

		return 0;
	}

	if(pItem )

	{ 

		//װ����guid ��λ��
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;
		msg.SetPos(CDataPool::GetMe()->ChangeCard_GetItemPos());
		msg.setItemID(Guid);

	 
		//���ߵ�λ�ú����npc
		msg.setGemBagPos(CDataPool::GetMe()->ChangeCard_GetNeedItemPos());
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId());
		CNetManager::GetMe()->SendPacket(&msg);


	} else
	{
		STRING strTemp = "�������Ҫ�������͵�װ��";

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

		return 0;
	}




	return 0;
}


//  ������ȡ�����װ����ť
INT PlayerPackage::EquipDismantleCannel(LuaPlus::LuaState* state)  

{

	INT nTypeOwner	=	CDataPool::GetMe()->Dismantle_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Dismantle_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
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

//  ������ȡ������װ����ť
INT PlayerPackage::ReWorkEquipCannel(LuaPlus::LuaState* state)  
{
	INT nTypeOwner	=	CDataPool::GetMe()->Rework_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Rework_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
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


//ȡ������װ������
INT PlayerPackage::CancelChangeCarditem(LuaPlus::LuaState* state)  

{


	INT nTypeOwner	=	CDataPool::GetMe()->ChangeCard_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->ChangeCard_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);

				//֪ͨװ������״̬
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


//ȡ������װ������
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
			case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;

			case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);

						//֪ͨװ������״̬
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



// ������ȷ���ϳ�װ����ť
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

	//  CGameProcedure::s_pGfxSystem->PushDebugString("�����ռ䲻�㣬������һ���ո�");
	//  STRING strTemp = "�����ռ䲻�㣬������һ���ո�";
	//  CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
	//	return 0;
 // }
	//

	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param1");      //�䷽id
	}
	//������
	if (!(args[3].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param2");      //����1�ȼ�
	}
	//������
	if (!(args[4].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param3");		//����2�ȼ�
	}

	//������
	if (!(args[5].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param4");		//����3�ȼ�

	}

	//������
	if (!(args[6].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::EquipSynthesize_Sure Wrong Param5");		//����4�ȼ�
	}


   INT Grad[MAX_EQUIP_PROMATL_NUM];

   for ( INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM ;i++)
   {
	   Grad[i]     =  args[i+3].GetInteger();
   }

 	CGameInterface::GetMe()->EquipSynthesize(args[2].GetInteger() ,Grad );


	return 0;

}


//������Ʒ
INT PlayerPackage::PackUpPacket(LuaPlus::LuaState* state)
{
	//if( CGameInterface::GetMe()->IsInHell()) return 0;

	LuaStack args(state);
	//������
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

	// ��Ҫ�������Щ״̬�£���������  ��̯�����ף��ύ���񣬴�����̵���棬ʹ����Ʒ
	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯������������");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_2");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����в���������");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_3");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ڲ���������");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_4");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ڲ���������");
		STRING strTemp = COLORMSGFUNC("GMGameInterface_Script_Package_5");
		ADDNEWDEBUGMSG(strTemp);
		return 0;
	}

	// ��������Ҫ����������������
	//CEventSystem::GetMe()->PushEvent(GE_BEGIN_PACKUP_PACKET);

	// ��Clone����
	CDataPool::GetMe()->UserBagClone_Clone();


	// ��Ҫһ�����Ƶ�����������ģ��,���������¼����Ʒ��ָ�룬���ԣ������ڹ����е���Item �� Set����
	// ʹ���ڿͻ���ģ������ƶ���Ʒ�ķ�ʽ����
	// ��һ�������������ܵ��ӵ���Ʒ
	for( INT i=nBeginIndex; i<=nEndIndex; i++ )
	{
		tObject_Item* pItem1	= CDataPool::GetMe()->UserBagClone_GetItem(i)->m_pItem;
		INT nItemNum1	= CDataPool::GetMe()->UserBagClone_GetItem(i)->m_nItemNum;

		if(pItem1)
			if(pItem1->Rule(tObject_Item::RULE_OVERLAY))			// ��Ʒ������Ե���
				if(!pItem1->IsLocked())						// û�б�����
					if(!pItem1->GetByProtect())						// û�б��������뱣��
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
														if( pItem1->GetIdTable() == pItem2->GetIdTable() )		// ������Ʒ���Ե���
														{
															if(nItemNum1 < pItem1->GetLayNum())
															{
																if(nItemNum2 < pItem2->GetLayNum())
																{
																	// �����ƶ���Ʒ���ϲ���Ʒ������Ϣ
																	CGPackage_SwapItem msg;
																	msg.SetPackageIndex1(j);
																	msg.SetPackageIndex2(i);

																	CNetManager::GetMe()->SendPacket(&msg);

																	// ģ����
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
	//�������

	// ��ʼ����λ�ã������Ѿ����Բ��ù�����Ʒ��������
	for(INT i=nBeginIndex; i<=nEndIndex; i++)
	{
		tObject_Item*  pItem1 = 0;
		tObject_Item*  pItem2 = 0;

		INT nTempIndex = i;

		for(INT j=i; j<=nEndIndex; j++)
		{
			pItem1	= CDataPool::GetMe()->UserBagClone_GetItem(nTempIndex)->m_pItem;
			pItem2	= CDataPool::GetMe()->UserBagClone_GetItem(j)->m_pItem;

			// ��������ȼ�����Ʒ�������λ�ã�
			if( i != j )
				if( IsSwapItem(pItem1, pItem2) )
				{
					nTempIndex = j;
				}
		}
		if( nTempIndex != i )
		{
			// ��������Ϣ
			CGPackage_SwapItem msg;
			msg.SetPackageIndex1(nTempIndex);
			msg.SetPackageIndex2(i);

			CNetManager::GetMe()->SendPacket(&msg);

			// �ͻ���ͬ��ģ��
			tObject_Item*  pItem1 = CDataPool::GetMe()->UserBagClone_GetItem(i)->m_pItem;
			CDataPool::USER_BAG_CLONE		CloneObj;
			CloneObj.m_pItem	= pItem1;
			CDataPool::GetMe()->UserBagClone_SetItem(nTempIndex,&CloneObj);

			CloneObj.m_pItem	= pItem2;
			CDataPool::GetMe()->UserBagClone_SetItem(i,&CloneObj);
		}
	}

	// ���������Ϣ��Ŀ�����ڿͻ�����������ģ��󣬷�һ����β�Ե���Ϣ��������
	CGPackUpPacket msg;
	msg.SetConIndex(nIndex);

	CNetManager::GetMe()->SendPacket(&msg);

	// ֪ͨ����ť���
	CEventSystem::GetMe()->PushEvent(GE_BEGIN_PACKUP_PACKET);

	return 0;
}


//������Ʒ
INT PlayerPackage::OpenPetList(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);

	return 0;
}
//����������
INT PlayerPackage::OpenLockFrame(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: PlayerPackage::OpenLockFrame Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();

	CEventSystem::GetMe()->PushEvent(GE_OPEN_ITEM_COFFER, nIndex);
	return 0;
}
//������Ʒ�����
INT PlayerPackage::Lock(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
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
			//������Ʒ�ļ������ù���,����������Ʒ����Ϣ��������
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//��������Ʒ�ǲ����Ѿ�����
			if(pItem->GetByProtect())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ�Ѿ�����");
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
			//ʲôҲ��������������� ����Add By hongyu
		}
	}
	// �������
	else	
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdUnlocked())
		{
			//������Ʒ�ļ������ù���,����������Ʒ����Ϣ��������
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//��������Ʒ�ǲ����Ѿ�����
			if( !pItem->GetByProtect() )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒû�м���");
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
			//ʲôҲ��������������� ����Add By hongyu
		}
	}
	return 0;
}
//�����Ʒ�Ƿ�����
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

//��Ʒ����
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
// �ǽӿں���
//----------------------------------------------------------------------------

// �Ƚ�������Ʒ�������ǲ���Ӧ�ý���λ�ã����������pItem2���ŵ�ǰ��
BOOL PlayerPackage::IsSwapItem(tObject_Item* pItem1, tObject_Item* pItem2)
{
	// 1���滻��λ
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

	// 2��
	if(pItem1->GetItemClass() != pItem2->GetItemClass())
	{
		// ��ͬ�������Ʒ
		if(pItem1->GetItemClass() > pItem2->GetItemClass())
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		// ͬ�������Ʒ
		switch(pItem1->GetItemClass())
		{
		case ICLASS_EQUIP:			//����WEAPON������DEFENCE������ADORN
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
					if( ((CObject_Item_Equip*)pItem1)->GetItemType() == HEQUIP_MAINHAND)  // ����
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
							//���ȼ���
							if( ((CObject_Item_Equip*)pItem1)->GetNeedLevel() < ((CObject_Item_Equip*)pItem2)->GetNeedLevel() )
								return TRUE;
							else
								return FALSE;
						}
					}
					else
					{
						//���ȼ���
						if( ((CObject_Item_Equip*)pItem1)->GetNeedLevel() < ((CObject_Item_Equip*)pItem2)->GetNeedLevel() )
							return TRUE;
						else
							return FALSE;
					}

				}
			}
			break;
		case ICLASS_MATERIAL:		//ԭ��
		case ICLASS_COMITEM	:		//ҩƷ
		case ICLASS_TASKITEM:		//������Ʒ
		case ICLASS_GEM:			//��ʯ
		case ICLASS_STOREMAP:		//�ر�ͼ
		case ICLASS_TALISMAN:		//����---???
		case ICLASS_GUILDITEM:		//�����Ʒ
		case ICLASS_IDENT:			//������
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
					//���ȼ���
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
