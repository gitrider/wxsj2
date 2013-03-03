
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Global.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "DataPool/GMDP_CharacterData.h"

#include "Network\NetManager.h"
#include "DataPool/GMDataPool.h"
#include "Action/GMActionSystem.h"
#include "Event/GMEventSystem.h"
#include "Procedure\GameProcedure.h"
#include "GIUtil.h"

#include "CGPlayerShopEstablish.h"
#include "CGPlayerShopAcquireItemList.h"
#include "CGItemSynch.h"
#include "CGPlayerShopOnSale.h"
#include "CGPlayerShopMoney.h"
#include "CGPlayerShopBuyItem.h"
#include "CGPlayerShopOpenStall.h"
#include "CGPlayerShopDesc.h"
#include "CGPlayerShopName.h"
#include "CGPlayerShopSaleOut.h"
#include "CGPlayerShopBuyShop.h"
#include "CGPlayerShopPartner.h"

#include "CGPlayerShopAskForRecord.h"
#include "CGPlayerShopSize.h"



namespace SCRIPT_SANDBOX
{
	//===============================================================
	PlayerShop PlayerShop::s_PlayerShop;
	LuaPlus::LuaObject* PlayerShop::s_pMetaTable = NULL;

	// ��һ���µ�
	INT PlayerShop::CreateShop(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::CreateShop[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::CreateShop[3] param parameter error");
		}
		std::string strName = args[2].GetString();
		if(strName == "")
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Need_ShopName");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Բ�����Ҫ�������������֡�");
			return 0;
		}
		if(strName.size()>12)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Name_TooLong");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Բ���������ֹ�����");
			return 0;
		}

		//��֤�̵����ƵĺϷ���
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(strName, FT_NAME))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}
		//��֤�̵����ƵĺϷ���
		if(!KLU_CheckStringValid(strName.c_str()))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}

		CHAR szMsg[MAX_SHOP_NAME];
		sprintf(szMsg,"%s",strName.c_str());
		INT nType = args[3].GetInteger();

		CGPlayerShopEstablish Msg;
		if( nType == 0 )
		{
			//���û��ѡ��
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Choice_ShopType");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ϊ��Ҫ���ĵ�ѡ��һ������");
			return 0;
		}
		else if(nType == 1)
		{
			Msg.SetType(PLAYERSHOP_MSG::TYPE_ITEM);
		}
		else if( nType == 2 )
		{
			Msg.SetType(PLAYERSHOP_MSG::TYPE_PET);
		}
		Msg.SetShopNameSize((BYTE)strlen(szMsg));
		Msg.SetShopName(szMsg);

		CNetManager::GetMe()->SendPacket(&Msg);

		//��Ϣ����֮����֪ͨ������ڹر�
		CEventSystem::GetMe()->PushEvent(GE_PS_CLOSE_CREATESHOP);

		return 0;
	}

	//����̵������
	INT PlayerShop::GetShopNum(LuaPlus::LuaState* state)
	{
		state->PushInteger( CDataPool::GetMe()->PlayerShop_GetShopNum() );
		return 1;
	}

	//
	INT PlayerShop::EnumShop(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumShop[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		state->PushString( CDataPool::GetMe()->PlayerShop_GetShopNameByIndex(nIndex));
		state->PushInteger( (INT)CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nIndex) );
		state->PushInteger( (INT)CDataPool::GetMe()->PlayerShop_GetStallNumOnSale(nIndex) );

		INT nType = CDataPool::GetMe()->PlayerShop_GetTypeByIndex(nIndex);
		switch(nType)
		{
		case CDataPool::TYPE_PLAYER_SHOP_INVALID :		//��Ч��𣬻�û�з����ȥ
			{
				state->PushString("");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_ITEM:				//��Ʒ��
			{
				state->PushString("��Ʒ��");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_GEM:				//��ʯ��
			{
				state->PushString("��ʯ��");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_WEAPON:			//������
			{
				state->PushString("������");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_ARMOR:				//���׵�
			{
				state->PushString("���׵�");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_MATERIAL:			//���ϵ�
			{
				state->PushString("���ϵ�");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_PET:				//�����
			{
				state->PushString("�����");
			}
			break;
		default:
			{
				state->PushString("");
			}
			break;
		}

		return 4;
	}
	// �̵���Ҫ�ıȽ���ϸ��һЩ��Ϣ(ע��ShopList�Ի���ʹ��)
	INT PlayerShop::EnumShopInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("PlayerShop::EnumShopInfo[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("PlayerShop::EnumShopInfo[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		if(nIndex == -1)
		{
			state->PushString("");
			return 1;
		}

		if(str == "since")
		{
			state->PushString(CDataPool::GetMe()->PlayerShop_GetShopFoundedTimeByIndex((UINT)nIndex));
		}
		else if(str == "desc")
		{
			state->PushString(CDataPool::GetMe()->PlayerShop_GetShopDescByIndex((UINT)nIndex));
		}
		else if(str == "ownername")
		{
			state->PushString(CDataPool::GetMe()->PlayerShop_GetOwnerNameByIndex((UINT)nIndex));
		}
		else if(str == "ownerid")
		{
			UINT uID = CDataPool::GetMe()->PlayerShop_GetOwnerGuidByIndex((UINT)nIndex);
			CHAR szTemp[16];
			sprintf(szTemp,"%X",uID);
			state->PushString(szTemp);
		}
		else
		{
			state->PushString("");
		}
		//����ж����Ϣ��ֱ����������ӾͿ�����
		return 1;
	}
	//
	INT PlayerShop::AskOpenShop(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::AskOpenShop[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();
		Assert(nIndex != -1);

		// ��һ��������Ĭ�ϴ򿪵�һҳ��
		_PLAYERSHOP_GUID PlayerShopGUID = CDataPool::GetMe()->PlayerShop_GetIDByIndex((UINT)nIndex);
		CGPlayerShopAcquireItemList msg;
		msg.SetShopID(PlayerShopGUID);
		msg.SetStallIndex(0);
		msg.SetSign(CGPlayerShopAcquireItemList::ASK_OPEN_STALLL);
	
		CNetManager::GetMe()->SendPacket(&msg);

		//���ԭ������
		CDataPool::GetMe()->PlayerShop_CleanObjItem(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanObjItem(FALSE);
		CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);

		return 0;
	}
	//
	INT PlayerShop::EnumItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[3] param parameter error");
		}
		if (!(args[4].IsString()))
		{
			KLThrow("PlayerShop::EnumItem[4] param parameter error");
		}

		INT nConIndex = args[2].GetInteger();
		INT nPosition = args[3].GetInteger();
		std::string str = args[4].GetString();

		tActionItem* pActionItem = NULL;

		if( str == "self" )
		{
			pActionItem = CActionSystem::GetMe()->EnumAction((nConIndex*STALL_BOX_SIZE) + nPosition, NAMETYPE_PS_SELF);
			if(pActionItem)
			{
				if(1 == CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE,nConIndex,nPosition))
				{
					//�ϼ�״̬
					pActionItem->Disable();
				}
				else
				{
					//�¼�״̬
					pActionItem->Enable();
				}
			}
		}
		else if( str == "other" )
		{
			pActionItem = CActionSystem::GetMe()->EnumAction((nConIndex*STALL_BOX_SIZE) + nPosition, NAMETYPE_PS_OTHER);
		}

		if( pActionItem )
		{
			LuaObject objReturn = state->BoxPointer(pActionItem);
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();

			if(((tObject_Item*)pActionItem->GetImpl())->IsLocked())
				state->PushBoolean(true);
			else
				state->PushBoolean(false);
			return 2;
		}

		//�Ƿ�ActionItem
		LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();

		state->PushBoolean(false);
		return 2;
	}
	//
	INT PlayerShop::GetSelectIndex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("PlayerShop::GetSelectIndex[2] param parameter error");
		}

		std::string str = args[2].GetString();

		INT nIndex = 0;

		if( str == "self" )
		{
			nIndex = CDataPool::GetMe()->PlayerShop_GetMySelectPos();
			AxTrace(0,0,"PlayerShop_GetMySelectPos");
		}
		if( str == "other" )
		{
			nIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();
		}

		state->PushInteger(nIndex + 1);
		return 1;
	}
	// ������̵�ȡ����Ʒ���Լ��ı���
	INT PlayerShop::RetackItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("PlayerShop::RetackItem[2] param parameter error");
		}
		std::string str = args[2].GetString();

		//ʹ�õ���DataPool�м�¼����Ʒ
		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();


		if(str == "item")
		{
			tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nConIndex,nPosition);
			//ִ�н���Ʒ���̵�ת�Ƶ�����
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			//��ѯ����Ʒ�ǲ��Ǵ����ϼ�״̬��
			if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Item_OnStall");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ�����ϼ�״̬");
				return 0;
			}

			//tObject_Item* pSelectItem = this->GetItemImpl();
			if(NULL == pSelectItem)
			{
				return 0;
			}

			pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGItemSynch msg;
			msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
			msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
			msg.SetToType(CGItemSynch::POS_BAG);
			msg.SetItemGUID(Guid);

			INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			CGAutoMoveItemFromPlayerShopToBag_t ExtraInfoData;
			ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
			ExtraInfoData.m_uSerial			= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nPosition);
			msg.SetExtraInfoLength(ExtraInfoData.GetSize());
			msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//UINT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(TRUE,nConIndex,nPosition);
			//SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nPosition);
			//if(pPet == NULL)
			//{
			//	return 0;
			//}

			//if(1 == CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
			//{
			//	//��Ʒ�����ﴦ���ϼ�״̬����Ҫ���¼ܲ���ȡ�������Ʒ��
			//	STRING strTemp = "";
			//	strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Pet_OnStall");
			//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ﴦ���ϼ�״̬");
			//	return 0;
			//}

			//CGItemSynch msg;
			//msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
			//msg.SetFromType(CGItemSynch::POS_PLAYERSHOPPET);
			//msg.SetToType(CGItemSynch::POS_PET);
			//msg.SetPetGUID(pPet->m_GUID);

			//CGAutoMovePetFromPlayerShopToBag_t ExtraInfoData;
			//ExtraInfoData.m_ShopGuid	= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			//ExtraInfoData.m_nStallIndex	= nConIndex;
			//ExtraInfoData.m_uSerial		= nSerial;

			//msg.SetExtraInfoLength(ExtraInfoData.GetSize());
			//msg.SetExtraInfoData((BYTE*)&ExtraInfoData);

			//CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}
	//����ϼ�
	INT PlayerShop::InputMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::InputMoney[2] param parameter error");
		}
		std::string str = args[2].GetString();

		INT nCon = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPos = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

		if(str == "ps_upitem" )//|| str == "ps_uppet")
		{
			tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nCon,nPos);
			if(! pSelectItem )
			{
				//û��ѡ�е���Ʒ]
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Select_Item_Need");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ҫ��ѡ��һ����Ʒ");
				return 0;
			}
			//�������Ǯ�ĶԻ���
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,str.c_str());
			return 0;
		}
		else if(str == "ps_uppet")
		{
			//SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nCon, nPos);
			//if( !pPet )
			//{
			//	STRING strTemp = "";
			//	strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Select_Pet_Need");
			//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ҫ��ѡ��һ������");
			//	return 0;
			//}
			////�������Ǯ�ĶԻ���
			//CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,str.c_str());
			return 0;
		}

		//�������Ǯ�ĶԻ���
		CEventSystem::GetMe()->PushEvent(GE_PS_INPUT_MONEY,str.c_str());
		AxTrace(0,0,"C++ str.c_str =%s",str.c_str());
		return 0;
	}
	//�ϼ�
	INT PlayerShop::UpStall(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::UpStall param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::UpStall param parameter error");
		}

		std::string str = args[2].GetString();
		UINT nMoney = static_cast<UINT>(args[3].GetInteger());

		//��Ʒ���۲���Ϊ0
		if(nMoney==0)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Item_NoPrice");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�ϼ�ʧ�ܣ��ϼ���Ʒ�۸���Ϊ0");
			return 0;
		}
		if(nMoney<=1000000000)
		{//С��10w������ϼ�

			//ʹ�õ���DataPool�м�¼����Ʒ
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();
			
			_PLAYERSHOP_GUID	nShopID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);

			if( str == "item")
			{
				UINT nSerial = CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nConIndex,nPosition);
				tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nConIndex,nPosition);
				//ִ�н���Ʒ���̵�ת�Ƶ�����
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				//tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return 0;
				}
				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;


				CGPlayerShopOnSale msg;
				msg.SetIsOnSale(1);//�ϼ�Ϊ1���¼�Ϊ0
				msg.SetObjGUID(Guid);
				msg.SetPrice(nMoney);
				msg.SetSerial(nSerial);
				msg.SetShopID(nShopID);
				msg.SetStallIndex(nConIndex);

				CNetManager::GetMe()->SendPacket(&msg);
			}
			else
			{
				
				/*UINT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(TRUE,nConIndex,nPosition);
				SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nPosition);
				if(pPet == NULL)
				{
					return 0;
				}

				CGPlayerShopOnSale msg;
				msg.SetIsOnSale(1);
				msg.SetPetGuid(pPet->m_GUID);
				msg.SetSerial(nSerial);
				msg.SetPrice(nMoney);
				msg.SetShopID(nShopID);
				msg.SetStallIndex(nConIndex);

				CNetManager::GetMe()->SendPacket(&msg);*/
			}
		}

		return 0;
	}
	//
	INT	PlayerShop::SetCurSelectPage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::SetCurSelectPage[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::SetCurSelectPage[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		if(str == "self")
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nIndex);
		else
			CDataPool::GetMe()->PlayerShop_SetOtSelectConTa(nIndex);

		return 0;
	}
	//
	INT	PlayerShop::GetCurSelectPage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::SetCurSelectPage[2] param parameter error");
		}
		std::string str = args[2].GetString();
		if(str == "self")
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetMySelectConTa());
		else
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetOtSelectConTa());

		return 1;
	}
	//
	INT PlayerShop::DealMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::DealMoney[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::DealMoney[3] param parameter error");
		}
		std::string str = args[2].GetString();
		UINT nMoney = args[3].GetInteger();
		
		if(str == "immitbase")
		{
			// ����һ�γ�ǮҪ10G
			if(nMoney < 100000)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����Ľ�Ǯδ�ﵽ��С����");

				state->PushString("few");
				state->PushInteger(0);
				return 2;
			}
			// ��������1000G
			INT nCurMoney = CDataPool::GetMe()->PlayerShop_GetBaseMoney(TRUE);
			if(nMoney + nCurMoney > 10000000)
			{
				//STRING strTemp = "";
				//strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_ShopBaseMoney");
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				////CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���̱�������Ϊ1000�����������롣");

				state->PushString("more");
				state->PushInteger(10000000 - nCurMoney);
				return 2;
			}
			state->PushString("ok");
			state->PushInteger(0);
			return 2;
		}
		else if(str == "immit")
		{
			// ���ٳ���Ľ�ǮΪ  30�� *��ҵָ��*�ѿ���̨��
			FLOAT nCom = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(TRUE);

			/*_PLAYERSHOP_GUID nMyStallID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nMyStallID)
				{
					nPos = i;
					break;
				}
			}
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/

			if( nMoney < 300000 * nCom * nStallNum )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����Ľ�Ǯδ�ﵽ��С����");

				state->PushString("few");
				state->PushInteger(0);
				return 2;
			}
				
			// Ӯ��������5000G
			INT nCurMoney = CDataPool::GetMe()->PlayerShop_GetProfitMoney(TRUE);
			if(nMoney + nCurMoney > 50000000)
			{
				//STRING strTemp = "";
				//strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_ShopProfitMoney");
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				////CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ӯ���ʽ�����Ϊ5000�����������롣");
				
				state->PushString("more");
				state->PushInteger(50000000 - nCurMoney);
				return 2;
			}
			state->PushString("ok");
			state->PushInteger(0);
			return 2;
		}
		else if(str == "draw")
		{
			//ȡǮ���ȼ����ȡ��Ǯ�ǲ��� <= Ӯ���ʽ�
			if( nMoney > CDataPool::GetMe()->PlayerShop_GetProfitMoney(TRUE) )
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_No_ProfitMoney");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��û����ô���Ӯ���ʽ�");
				return 0;
			}
		}


		return 0;
	}
	// ��������ύ
	INT PlayerShop::ApplyMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::ApplyMoney[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::ApplyMoney[3] param parameter error");
		}
		std::string str = args[2].GetString();
		UINT nMoney = args[3].GetInteger();

		if(str == "immitbase")
		{
			INT nTaxMoney = (INT) (nMoney*0.03);
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "immitbase" ,"immitbase_ok", nMoney, nTaxMoney);
		}
		else if(str == "immit")
		{
			INT nTaxMoney = (INT) (nMoney*0.03);
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "immit" ,"immit_ok", nMoney, nTaxMoney);
		}
		else if(str == "draw")
		{
			INT nTaxMoney = (INT) (nMoney*0.03);
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "draw" ,"draw_ok", nMoney, nTaxMoney);
		}
		else if(str == "immitbase_ok")
		{
			//��������Ǯ������
			INT nSelfMoney = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

			if( (INT) (nMoney * (1+SHOP_TAX)) > nSelfMoney)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_No_Self_Money");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ϵ�Ǯ����");
				return 0;
			}

			CGPlayerShopMoney msg;
			msg.SetOpt(CGPlayerShopMoney::OPT_SAVE_MONEY);
			msg.SetType(CGPlayerShopMoney::TYPE_BASE_MONEY);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetAmount((UINT) nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "immit_ok")
		{
			//��������Ǯ������
			//��������Ǯ������
			INT nSelfMoney = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

			if( (INT) (nMoney * (1+SHOP_TAX)) > nSelfMoney)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_No_Self_Money");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ϵ�Ǯ����");
				return 0;
			}

			CGPlayerShopMoney msg;
			msg.SetOpt(CGPlayerShopMoney::OPT_SAVE_MONEY);
			msg.SetType(CGPlayerShopMoney::TYPE_PROFIT_MONEY);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetAmount((UINT) nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "draw_ok")
		{
			CGPlayerShopMoney msg;
			msg.SetOpt(CGPlayerShopMoney::OPT_GET_MONEY);
			msg.SetType(CGPlayerShopMoney::TYPE_PROFIT_MONEY);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetAmount((UINT) nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}
	//
	INT PlayerShop::InfoMoney(LuaPlus::LuaState* state)
	{

		return 0;
	}
	//
	INT PlayerShop::GetStallNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetStallNum[2] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nStallNum = 0;
		if(str == "self")
		{
			/*_PLAYERSHOP_GUID nMyStallID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nMyStallID)
				{
					nPos = i;
					break;
				}
			}
			
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/
			nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(TRUE);

		}
		else
		{
			/*_PLAYERSHOP_GUID nOtStallID = CDataPool::GetMe()->PlayerShop_GetShopID(FALSE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nOtStallID)
				{
					nPos = i;
					break;
				}
			}
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/
			nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(FALSE);
		}
		state->PushInteger(nStallNum);

		return 1;
	}
	//
	INT PlayerShop::BuyItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::BuyItem[2] param parameter error");
		}
		std::string str = args[2].GetString();

		//ʹ�õ���DataPool�м�¼����Ʒ
		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();
		
		if(str == "item")
		{
			tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(FALSE,nConIndex,nPosition);

			//ִ�н���Ʒ���̵�ת�Ƶ�����
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			if(NULL == pSelectItem)
			{
				return 0;
			}

			pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGPlayerShopBuyItem msg;
			msg.SetObjGUID(Guid);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(FALSE));
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetItemSerial(FALSE,nConIndex,nPosition));
			msg.SetStallIndex(nConIndex);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			/*SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nConIndex, nPosition);
			if(!pPet)
			{
				return 0;
			}
			UINT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(FALSE,nConIndex,nPosition);

			CGPlayerShopBuyItem msg;
			msg.SetPetGuid(pPet->m_GUID);
			msg.SetSerial(nSerial);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(FALSE));
			msg.SetStallIndex(nConIndex);

			CNetManager::GetMe()->SendPacket(&msg);*/
		}

		return 0;
	}
	//
	INT PlayerShop::IsSelectOnSale(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::IsSelectOnSale[2] param parameter error");
		}
		std::string str = args[2].GetString();


		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

        INT nOnSale;
		if(str == "item")
		{
			nOnSale = (INT)CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE,nConIndex,nPosition);
		}
		else
		{
			if (!(args[3].IsInteger()))
			{
				KLThrow("LUA:PlayerShop::IsSelectOnSale[3] param parameter error");
			}
			INT nIndex  = args[3].GetInteger();
			
			nOnSale = (INT)CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE,nConIndex,nIndex);
		}
		state->PushInteger(nOnSale);
		return 1;
	}

	//�¼�
	INT PlayerShop::DownSale(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::DownSale[2] param parameter error");
		}
		std::string str = args[2].GetString();

		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

		if( str == "item" )
		{
			if(nConIndex == -1 || nPosition == -1)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Choice_Item");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ѡ��һ����Ʒ");
				return 0;
			}
			tObject_Item* pItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nConIndex,nPosition);
			if(!pItem)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Choice_Item");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ѡ��һ����Ʒ");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			//tObject_Item* pSelectItem = this->GetItemImpl();
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGPlayerShopOnSale msg;
			msg.SetIsOnSale(0);//�ϼ�Ϊ1���¼�Ϊ0
			msg.SetStallIndex(CDataPool::GetMe()->PlayerShop_GetMySelectConTa());
			msg.SetObjGUID(Guid);
			msg.SetPrice(0);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nConIndex,nPosition));
	
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//if(nConIndex == -1 || nPosition == -1)
			//{
			//	STRING strTemp = "";
			//	strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Choice_Pet");
			//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ѡ��һ������");
			//	return 0;
			//}
			//SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nPosition);
			//if(!pPet)
			//{
			//	STRING strTemp = "";
			//	strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_Choice_Pet");
			//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ѡ��һ������");
			//	return 0;
			//}

			//INT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(TRUE,nConIndex,nPosition);
			//CGPlayerShopOnSale msg;
			//msg.SetIsOnSale(0);//�ϼ�Ϊ1���¼�Ϊ0
			//msg.SetStallIndex(CDataPool::GetMe()->PlayerShop_GetMySelectConTa());
			//msg.SetPetGuid(pPet->m_GUID);
			//msg.SetPrice(0);
			//msg.SetSerial(nSerial);
			//msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));

			//CNetManager::GetMe()->SendPacket(&msg);
		}

		return 0;
	}
	//��յ�ǰ��ѡ��
	INT PlayerShop::ClearSelect(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::ClearSelect[2] param parameter error");
		}
		std::string str = args[2].GetString();
		
		if(str == "self")
		{
			CDataPool::GetMe()->PlayerShop_MyClearSelect();
		}
		else
		{
			CDataPool::GetMe()->PlayerShop_OtClearSelect();
		}
		return 0;
	}
	//��ý�Ǯ
	INT PlayerShop::GetMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetMoney[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetMoney[3] param parameter error");
		}
		std::string str = args[2].GetString();
		std::string strWho = args[3].GetString();

		INT nMoney;
		//����
		if( str == "base" )
		{
			if(strWho == "self")
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetBaseMoney(TRUE);
			}
			else
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetBaseMoney(FALSE);
			}
		}
		//Ӯ����
		else if( str == "profit" )
		{
			if(strWho == "self")
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetProfitMoney(TRUE);
			}
			else
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetProfitMoney(FALSE);
			}
		}
		//������Ҫ��Ǯ
		else if( str == "open" )
		{
			FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			nMoney = (INT)(300000 * fCommercialFactor * 2 * (1 + SHOP_TAX));
		}
		else if( str == "saleout" )
		{
			if(strWho == "self")
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetSaleOutPrice(TRUE);
			}
			else
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetSaleOutPrice(FALSE);
			}
		}
		else if( str == "input_profit" )
		{
			FLOAT fCom = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nStallNum;
			nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(TRUE);
			/*_PLAYERSHOP_GUID nMyStallID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nMyStallID)
				{
					nPos = i;
					break;
				}
			}
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/
			nMoney = (INT)( fCom * 300000 * nStallNum );
		}
		else
		{
			nMoney = 0;
		}

		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ( (nMoney-nCopperCoin)/100 ) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}

		if ( nMoney >= 10000 )
		{
			nGoldCoin = (  ( (nMoney-nCopperCoin)/100 )-nSilverCoin  )/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger( nMoney );
		state->PushInteger( nGoldCoin );
		state->PushInteger( nSilverCoin );
		state->PushInteger( nCopperCoin );

		return 4;
	}
	//�����ҵָ��
	INT PlayerShop::GetCommercialFactor(LuaPlus::LuaState* state)
	{
		FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();

		fCommercialFactor *= 100;
		CHAR szCommercialFactor[64];
		sprintf(szCommercialFactor,"%0.0f��",fCommercialFactor);

		state->PushString(szCommercialFactor);
		return 1;
	}
	//��õ���������ʾ���׵�ʱ����ʾ��
	INT PlayerShop::GetShopInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetShopInfo[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetShopInfo[2] param parameter error");
		}
		std::string str = args[2].GetString();
		std::string strInfo = args[3].GetString();
		if(str == "self")
		{
			if(strInfo == "ownername")				// ��������
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetOwnerName(TRUE));
			}
			else if(strInfo == "ownerid")			// ����ID
			{
				UINT uID = CDataPool::GetMe()->PlayerShop_GetOwnerGuid(TRUE);
				CHAR szTemp[16];
				sprintf(szTemp,"%X",uID);
				state->PushString(szTemp);
			}
			else if(strInfo == "shopname")			// �̵�����
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopName(TRUE));
			}
			else if(strInfo == "desc")				// ����
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopDesc(TRUE));
			}
			else
			{
				state->PushString("");
			}
		}
		else
		{
			if(strInfo == "ownername")				// ��������
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetOwnerName(FALSE));
			}
			else if(strInfo == "ownerid")			// ����ID
			{
				UINT uID = CDataPool::GetMe()->PlayerShop_GetOwnerGuid(FALSE);
				CHAR szTemp[16];
				sprintf(szTemp,"%X",uID);
				state->PushString(szTemp);
			}
			else if(strInfo == "shopname")			// �̵�����
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopName(FALSE));
			}
			else if(strInfo == "desc")				// ����
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopDesc(FALSE));
			}
			else
			{
				state->PushString("");
			}
		}

		return 1;
	}
	//���һ��Stall�Ƿ�Open
	INT PlayerShop::IsOpenStall(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::IsOpenStall[2] param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::IsOpenStall[3] param parameter error");
		}
		std::string  str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		if( str == "self" )
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(TRUE, nIndex));

			//if(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(TRUE, nIndex))
			//{
			//	state->PushBoolean(true);
			//}
			//else
			//{
			//	state->PushBoolean(false);
			//}
		}
		else
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(FALSE, nIndex));

			//if(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(FALSE, nIndex))
			//{
			//	state->PushBoolean(true);
			//}
			//else
			//{
			//	state->PushBoolean(false);
			//}
		}
		return 1;
	}
	//����һ����̨
	INT PlayerShop::OpenStall(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::OpenStall[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::OpenStall[3] param parameter error");
		}
		INT nIndex = args[2].GetInteger();
		INT bOpen = args[3].GetInteger();

		CGPlayerShopOpenStall msg;
		msg.SetOpen((BYTE)bOpen);
		msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
		msg.SetStallIndex(nIndex);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//�����������һ��Stall������
	INT PlayerShop::AskStallData(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::AskStallData[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::AskStallData[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nStallIndex = args[3].GetInteger();
		
		_PLAYERSHOP_GUID ShopId;
		if(str == "self")
		{
			ShopId =  CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
		}
		else
		{
			ShopId =  CDataPool::GetMe()->PlayerShop_GetShopID(FALSE);
		}
		CGPlayerShopAcquireItemList msg;
		msg.SetShopID(ShopId);
		msg.SetStallIndex(nStallIndex);
		msg.SetSign(CGPlayerShopAcquireItemList::ASK_GET_STALL);
	
		CNetManager::GetMe()->SendPacket(&msg);

		//���ԭ������
		CDataPool::GetMe()->PlayerShop_CleanObjItem(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanObjItem(FALSE);
		CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);


		return 0;
	}
	//
	INT PlayerShop::EnumPet(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);
		//if (!(args[2].IsString()))
		//{
		//	KLThrow("LUA:PlayerShop::EnumItem[2] param parameter error");
		//}
		//if (!(args[3].IsInteger()))
		//{
		//	KLThrow("LUA:PlayerShop::EnumItem[3] param parameter error");
		//}
		//if (!(args[4].IsInteger()))
		//{
		//	KLThrow("LUA:PlayerShop::EnumItem[4] param parameter error");
		//}
		//std::string str = args[2].GetString();
		//INT nStallIndex = args[3].GetInteger();
		//INT nIndex = args[4].GetInteger();

		//if(str == "self")
		//{
		//	SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nStallIndex, nIndex);
		//	if(pPet == NULL)
		//	{
		//		state->PushString("");
		//		state->PushInteger(0);
		//		return 2;
		//	}
		//	//��������Ѿ��ϼܣ���ô�ѳ�������ֱ�ɺ�ɫ
		//	BYTE bOnSale = CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nStallIndex, nIndex);

		//	STRING szName = pPet->m_szName;

		//	state->PushString(szName.c_str());
		//	state->PushInteger((INT)bOnSale);
		//	return 2;
		//}
		//else
		//{
		//	SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nStallIndex, nIndex);
		//	if(pPet == NULL)
		//	{
		//		state->PushString("");
		//		return 1;
		//	}
		//	STRING szName = pPet->m_szName;

		//	state->PushString(szName.c_str());

		//	return 1;
		//}
		return 0;

	}
	//���õ�ǰѡ�еĳ���
	INT PlayerShop::SetSelectPet(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::SetSelectPet[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		CDataPool::GetMe()->PlayerShop_SetMySelectPos(nIndex);*/

		return 0;
	}
	//��õ�ǰ�ܿ����̵������
	INT PlayerShop::GetCanOpenShopType(LuaPlus::LuaState* state)
	{
		state->PushInteger(	(INT)CDataPool::GetMe()->PlayerShop_GetType() );

		return 1;
	}
	//��õ�ǰѡ�еĳ���Index
	INT PlayerShop::SetCurSelectPetIndex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::SetCurSelectPetIndex[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::SetCurSelectPetIndex[3] param parameter error");
		}
		if (!(args[4].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::SetCurSelectPetIndex[4] param parameter error");
		}
		std::string str =args[2].GetString();
		INT nConIndex = args[3].GetInteger();
		INT nIndex = args[4].GetInteger();

		if(str == "self")
		{
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(nIndex);
		}
		else	
		{
			CDataPool::GetMe()->PlayerShop_SetOtSelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(nIndex);
		}
		return 0;
	}
	//���ѡ����Ʒ���߳���ļ۸�
	INT PlayerShop::GetObjPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetObjPrice[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetObjPrice[3] param parameter error");
		}
		std::string strWho = args[2].GetString();
		std::string strObj = args[3].GetString();

		if(strWho == "self")
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

			if(nConIndex<0 || nIndex<0)
			{
				state->PushInteger( 0 );
				return 1;
			}
			if(strObj == "item")
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetItemPrice(TRUE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}
			else
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetPetPrice(TRUE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}

		}
		else
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();

			if(strObj == "item")
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetItemPrice(FALSE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}
			else
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetPetPrice(FALSE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}
		}
		return 1;
	}
	//�޸��̵������
	INT PlayerShop::Modify(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::Modify[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("LUA:PlayerShop::Modify[3] param parameter error");
		}
		std::string str = args[2].GetString();
		std::string strDesc = args[3].GetString();


		if(str == "ad_ok")  // �޸Ĺ����
		{
			if(strDesc.size() > 72)
			{
				return 0;
			}
			CGPlayerShopDesc msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetPlayerShopDesc((CHAR *)strDesc.c_str());
			msg.SetPlayerShopDescSize((BYTE)strDesc.size());

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "name_ok")  //�޸ĵ���
		{
			//��֤�̵����ƵĺϷ���
			if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc, FT_NAME))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			//��֤�̵����ƵĺϷ���
			if(!KLU_CheckStringValid(strDesc.c_str()))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			if(strDesc.size() > 12)
			{
				return 0;
			}

			if( strDesc == "" )
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_ShopName_Cannt_Null");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����������Ϊ��");
				return 0;
			}
			CGPlayerShopName msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetPlayerShopNameSize((BYTE)strDesc.size());
			msg.SetPlayerShopName((CHAR *)strDesc.c_str());

			CNetManager::GetMe()->SendPacket(&msg);

			//CGPlayerShopAskForRecord msgtest;
			//msgtest.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			//msgtest.SetType(CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD);
			//msgtest.SetPage(0);
			//CNetManager::GetMe()->SendPacket(&msgtest);

		}
		//�����޸ĵ����ĶԻ���
		else if(str == "ad")
		{
			//��֤�̵����ƵĺϷ���
			if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc, FT_NAME))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}

			FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nNeedMoney = (INT) (fCommercialFactor * 50000);

			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "ad" , strDesc.c_str(), nNeedMoney);
		}
		else if(str == "name")
		{
			//��֤�̵����ƵĺϷ���
			if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc, FT_NAME))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			//��֤�̵����ƵĺϷ���
			if(!KLU_CheckStringValid(strDesc.c_str()))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			if(strDesc.size() > 12)
			{
				return 0;
			}

			if( strDesc == "" )
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMInterface_PlayerShop_Info_ShopName_Cannt_Null");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����������Ϊ��");
				return 0;
			}
			FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nNeedMoney = (INT)(fCommercialFactor * 100000);

			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "name" , strDesc.c_str(), nNeedMoney);
		}
        
		return 0;
	}
	//���ѡ�е���Ʒ�����������
	INT PlayerShop::GetObjName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetObjName[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetObjName[3] param parameter error");
		}
		std::string strWho = args[2].GetString();
		std::string str = args[3].GetString();

		CHAR szTemp[32];

		if(strWho == "self")
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetMySelectPos();
			if(nConIndex == -1 || nIndex == -1)
			{
				state->PushString("");
				state->PushString("");
				return 2;
			}
			if(str == "item")
			{
				tObject_Item* pItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nConIndex, nIndex);
				if(!pItem)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pItem->GetName());
				sprintf(szTemp,"����:%d",pItem->GetNumber());
				state->PushString(szTemp);
			}
			else if(str == "pet")
			{
				/*SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nIndex);
				if(!pPet)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pPet->m_szName.c_str());
				state->PushString("");*/
			}
			else
			{
				state->PushString("");
				state->PushString("");
			}
		}
		else
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();
			if(nConIndex == -1 || nIndex == -1)
			{
				state->PushString("");
				state->PushString("");
				return 2;
			}
			if(str == "item")
			{
				tObject_Item* pItem = CDataPool::GetMe()->PlayerShop_GetItem(FALSE, nConIndex, nIndex);
				if(!pItem)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pItem->GetName());
				sprintf(szTemp,"����:%d",pItem->GetNumber());
				state->PushString(szTemp);
			}
			else if(str == "pet")
			{
				/*SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nConIndex, nIndex);
				if(!pPet)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pPet->m_szName.c_str());
				state->PushString("");*/
			}
			else
			{
				state->PushString("");
				state->PushString("");
			}
		}

		return 2;
	}
	//��ʾ������ϸ��Ϣ
	INT PlayerShop::ViewPetDesc(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::ViewPetDesc[2] param parameter error");
			return 0;
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::ViewPetDesc[3] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		INT nConIndex;

		SDATA_PET* pPet;
		if(str == "self")
		{
			nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nIndex);
		}
		else
		{
			nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
			pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nConIndex, nIndex);
		}
		if( (!pPet) || (INVALID_ID == pPet->m_nIsPresent) )
		{
			KLThrow("LUA:pPet = NULL parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(pPet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);*/
		return 0;
	}
	//�������ѡ��� Position,,�ڽ��в�����ʱ�򣬿���ʹ�ü�����ֵ�ǲ���Ϊ��1 ����־�ǲ��ǽ�����ѡ����Ʒ
	INT PlayerShop::ClearSelectPos(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::ClearSelectPos[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if(str == "self")
		{
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(-1);
		}
		else
		{
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(-1);
		}

		return 0;
	}
	//�̳��̵�
	INT PlayerShop::Transfer(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::Transfer[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if( str == "open" )
		{
			//ִ�д������Ǯ��ĵ���
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"transfershop");
		}
		else if(str == "idok")
		{
			if (!(args[3].IsInteger()))
			{
				KLThrow("LUA:PlayerShop::Transfer[3] param parameter error");
			}
			INT nMoney = args[3].GetInteger();
			//���ȷ���̳���Ĳ���
			CGPlayerShopSaleOut msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetPrice(nMoney);
			msg.SetSaleOut(TRUE);
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetShopSerial(TRUE));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "cancel")
		{
			//���Ҫ�̻ص���
			CGPlayerShopSaleOut msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetSaleOut(FALSE);
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetShopSerial(TRUE));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}
	// �鿴��ǰ���̵��ǲ��Ǵ����̵�״̬
	INT PlayerShop::IsSaleOut(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::IsSaleOut[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			state->PushInteger( (CDataPool::GetMe()->PlayerShop_GetIsSaleOut(TRUE)) ? 1: 0 );
		}
		else 
		{
			state->PushInteger( (CDataPool::GetMe()->PlayerShop_GetIsSaleOut(FALSE)) ? 1: 0 );
		}

		return 1;
	}
	// �����̵�
	INT PlayerShop::BuyShop(LuaPlus::LuaState* state)
	{
		CGPlayerShopBuyShop msg;

		msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetShopSerial(FALSE));
		msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(FALSE));
		
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//�ر�
	INT PlayerShop::CloseShop(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::CloseShop[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(-1);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(-1);
		}
		else
		{
			CDataPool::GetMe()->PlayerShop_SetOtSelectConTa(-1);
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(-1);
		}

		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);
		return 0;
	}

	//�򿪣��رգ�Message
	INT PlayerShop::OpenMessage(LuaPlus::LuaState* state)
	{
		//����
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::OpenMessage[2] param parameter error");
			return 0;
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::OpenMessage[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		if( str == "exchange" )			// ���׼�¼
		{
			CGPlayerShopAskForRecord msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetType(CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD);
			msg.SetPage(nIndex);

			CNetManager::GetMe()->SendPacket(&msg);

		}
		else if( str == "massage" )		// ��Ϣ
		{
			CGPlayerShopAskForRecord msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetType(CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD);
			msg.SetPage(nIndex);

			CNetManager::GetMe()->SendPacket(&msg);

		}

		return 0;
	}
	//���Message������
	INT PlayerShop::GetMessageNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetMessageNum[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "exchange" )
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetExRecNum(TRUE));
		}
		else if(str == "manage")
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetMaRecNum(TRUE));
		}
		else
		{
			state->PushInteger(0);
		}
		return 1;
	}
	//��ǰҳ��Message������
	INT PlayerShop::GetCurPageMessageNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::GetMessageNum[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "exchange" )
		{
			state->PushInteger(CDataPool::GetMe()->GetRecordList()->m_nRecordNum);
		}
		else if(str == "manage")
		{
			state->PushInteger(CDataPool::GetMe()->GetRecordList()->m_nRecordNum);
		}
		else
		{
			state->PushInteger(0);
		}
		return 1;
	}
	//���Message
	INT PlayerShop::EnumMessage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::EnumMessage[2] param parameter error");
			return 0;
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::EnumMessage[3] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		if( str == "exchange" )
		{
			state->PushString(CDataPool::GetMe()->GetRecordList()->m_RecordEntryList[nIndex].szContent.c_str());
		}
		else if(str == "manage")
		{
			state->PushString(CDataPool::GetMe()->GetRecordList()->m_RecordEntryList[nIndex].szContent.c_str());
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}
	//�����ϻ���
	INT PlayerShop::DealFriend(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::DealFriend[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "add" )
		{
			if(!(args[3].IsString()))
			{
				KLThrow("LUA:PlayerShop::DealFriend[3] param parameter error");
				return 0;
			}
			std::string strID = args[3].GetString();

			UINT uObjID = 0;
			sscanf(strID.c_str(), "%X", &uObjID);
			
			CGPlayerShopPartner msg;
			msg.SetGuid( uObjID );
			msg.SetOpt( CGPlayerShopPartner::OPT_ADD_PARTNER );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if( str == "del" )
		{
			if (!(args[3].IsInteger()))
			{
				KLThrow("LUA:PlayerShop::DealFriend[3] param parameter error");
				return 0;
			}
			INT nIndex = args[3].GetInteger();

			CGPlayerShopPartner msg;

			msg.SetGuid(CDataPool::GetMe()->PlayerShop_GetPartnerByIndex((UINT)nIndex)->m_Guid);
			msg.SetOpt( CGPlayerShopPartner::OPT_DEL_PARTNER );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			;
		}

		return 0;
	}
	//��úϻ��˵�����
	INT PlayerShop::GetFriendNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->PlayerShop_GetPartnerNum());
		return 1;
	}
	//��úϻ���
	INT PlayerShop::EnumFriend(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:PlayerShop::EnumFriend[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		state->PushString(CDataPool::GetMe()->PlayerShop_GetPartnerByIndex(nIndex)->m_szName.c_str());

		return 1;
	}
	//���ţ�����������
	INT PlayerShop::ChangeShopNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:PlayerShop::ChangeShopNum[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		
		if( str == "add" )
		{
			INT nMoney = (INT)(300000 * 2 * 1.03 * CDataPool::GetMe()->PlayerShop_GetCommercialFactor());
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "add_stall" ,"add_ok", nMoney);
		}
		else if( str == "del" )
		{
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "del_stall" ,"del_ok");
		}
		else if( str == "add_ok" )
		{
			CGPlayerShopSize msg;
			msg.SetOpt( CGPlayerShopSize::TYPE_EXPAND );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if( str == "del_ok" )
		{
			CGPlayerShopSize msg;
			msg.SetOpt( CGPlayerShopSize::TYPE_SHRINK );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}

		return 0;
	}
	// ��÷��ʵ��̵����Npc�ĵ�ǰ����ID����������CareObject��
	INT PlayerShop::GetNpcId(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->PlayerShop_GetNpcID());

		return 1;
	}
}
