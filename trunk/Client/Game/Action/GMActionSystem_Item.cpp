
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
#include "CGUnEquip.h"									// ʹ��ж��װ����Ϣ
#include "CGPackage_SwapItem.h"
#include "object\Logic\item\Obj_Item_Equip.h"
#include "object\Logic\item\Obj_Item_Gem.h"
#include "object\Logic\item\Obj_Item_Medicine.h"
#include "Object\Logic\Item\Obj_Item_SoulBead.h"
#include "Object\Logic\Item\Obj_Item_Mount.h"          //  [7/21/2010 �¾���]

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

	//����
	m_idItemImpl = pItem->GetID();
	//����
	m_strName = pItem->GetName();
	//ͼ��
	if ( pItem->GetIconName() != NULL )
		m_strIconName = pItem->GetIconName();
	//֪ͨUI
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
	case tObject_Item::IO_ITEMBOX:	// �򿪵ı�����
		return NAMETYPE_LOOTITEM;

	case tObject_Item::IO_BOOTH:	// ���˵Ļ���
		return NAMETYPE_BOOTITEM;

	case tObject_Item::IO_MYSELF_BANK:	// ����Լ����ϵ�������
		return NAMETYPE_BANKITEM;

	case tObject_Item::IO_MYSELF_EQUIP:	// ����Լ����ϵ�װ��
		return NAMETYPE_EQUIP;	// ������Ʒ��װ������

	case tObject_Item::IO_MYEXBOX:		// �Լ��Ľ��׺�
		return NAMETYPE_EXCHANGESELF;

	case tObject_Item::IO_OTHEREXBOX:	// �Է��Ľ��׺�
		return NAMETYPE_EXCHANGEOTHER;

	case tObject_Item::IO_MISSIONBOX:	// ����ݽ���
		return NAMETYPE_MISSIONREFER;

	case tObject_Item::IO_MYSTALLBOX:	// �Լ���̯λ��
		return NAMETYPE_STALLSELF;

	case tObject_Item::IO_OTSTALLBOX:	// �Է���̯λ��
		return NAMETYPE_STALLOTHER;

	case tObject_Item::IO_QUESTVIRTUALITEM:	// �������ģ����⣩��Ʒ��ֻ������ʾ
		return NAMETYPE_VIRTUALITEM;

	case tObject_Item::IO_PLAYEROTHER_EQUIP:	// ����������ϵ�װ��
		return NAMETYPE_OTHEREQUIP;

	case tObject_Item::IO_PS_OTHERBOX:	// 
		return NAMETYPE_PS_OTHER;

	case tObject_Item::IO_PS_SELFBOX:	// 
		return NAMETYPE_PS_SELF;

	case tObject_Item::IO_ITEMDROP:		//�������Ʒ
		return NAMETYPE_PS_DROP;

	case tObject_Item::IO_ITEMTRANSFER:		
		return NAMETYPE_TRANSFER_ITEM;
	
	// 20100428 AddCodeBgin
	case tObject_Item::IO_MYSELF_PACKET: // ��ұ���
		return NAMETYPE_PACKAGEITEM;

	case tObject_Item::IO_UNKNOWN: // ��Ʒ��δ֪
		return "";
	case tObject_Item::IO_ITEMIBSHOP: // IBShop
		return NAMETYPE_IB_SHOP;

	case tObject_Item::IO_MYSELF_MOUNT: //��ҵ�����      [7/20/2010 �¾���] 
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
//�õ�������Ϣ
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

	//���������Ʒ
	if(	pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
	{
		//��סShift
		if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
		{
			//�ж������Ʒ�Ƿ�߱���ֵ�����,��������һ�����Բ��
			if(pItem->GetNumber() > 1)
			{
				//�Ƚ����ܴ��ڵ��Ѿ���������Ʒ�������
				INT nCon = CDataPool::GetMe()->Split_GetItemConta();
				INT nPos = CDataPool::GetMe()->Split_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
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

				//��¼�����Ʒ��λ�õ����ݳ�
				CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->Split_SetItemPos(pItem->GetPosIndex());
				CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

				//�򿪲�ֶԻ���
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);		

				// ���������Ʒ
				pItem->SetLock(TRUE);

				//֪ͨ��������״̬
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			}
			else
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Cannt_chaifen");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ���ɲ��");
			}
			return;
			
		}

		//�ȼ�⡰Ctrl���ǲ��ǰ���״̬������ǣ���ô�ȴ�����ǵĲ���   
		if( CInputSystem::GetMe()->IsKeyDown(KC_LCONTROL) || CInputSystem::GetMe()->IsKeyDown(KC_RCONTROL) )

		{
			if(pItem->GetItemClass() == ICLASS_EQUIP)
			{
				//�Ƚ����ܴ��ڵ��Ѿ���������Ʒ�������
				INT nCon = CDataPool::GetMe()->AddStar_GetItemConta();
				INT nPos = CDataPool::GetMe()->AddStar_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_EQUIP:					// ������ҵ�װ��
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nPos));

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

				//��¼������Ʒ��λ�õ����ݳ�
				CDataPool::GetMe()->AddStar_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->AddStar_SetItemPos(pItem->GetPosIndex());


				//�򿪴��ǶԻ���
				CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING);

				// ���������Ʒ
				pItem->SetLock(TRUE);

				//֪ͨ��������״̬
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				return;

			}else

			{
				STRING strTemp = "����Ʒ����������";

				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());

			}
		}

		//��Ʒ�����ǿ���
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		{
			//������״̬
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				//�����ǲ���װ��,ֻ��װ����������
				if(pItem->GetItemClass() == ICLASS_EQUIP)
				{
					if( pItem->GetItemDur() <	pItem->GetItemMaxDur() ) //�����õ� ��ʵӦ��ΪItem->GetItemDur() < pItem->GetItemMaxDur()
					{
						//argo������ȫ��   //0��ʾ����һװ��
						 
						CDataPool::GetMe()->ComRepair_SetItemConta(1);//1������ 0����װ��
						CDataPool::GetMe()->ComRepair_SetItemPos(pItem->GetPosIndex());

					//	CEventSystem::GetMe()->PushEvent(GE_OPEN_REPAIR_SURE,0);  

						CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex());
					}
					else
					{
						//����ʧ����Ч
						//CSoundSystemFMod::_PlayUISoundFunc(47);
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Not_Require_Repair");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					}
				}
				else
				{
					//����ʧ����Ч
					//CSoundSystemFMod::_PlayUISoundFunc(47);

					STRING strTemp = "����װ�����ܽ���ά��";

					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}

				return;
			}
		}

		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
		{
			if(pItem->GetItemClass() == ICLASS_EQUIP)
			{
				// ʹ�ü������� 
				UseIdentify();
				return; 
			}
			else
			{
				// ʹ�ü������� 
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("UI_OPERATE_IDENTIFY_EQUIP_ONLY");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}
		}
		
	}
	else if( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_EQUIP )		//����Լ����ϵ�װ��
	{
		INT type = pItem->GetItemTableType();
		
		switch( type )
		{
		case EQUIP_SOUXIA:
			{
				CEventSystem::GetMe()->PushEvent(GE_OPEN_SOUXIA,1);
				//���Դ���
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

	//�ȼ�⡰Ctrl���ǲ��ǰ���״̬������ǣ���ô�ȴ�����ǵĲ���   
	if( CInputSystem::GetMe()->IsKeyDown(KC_LCONTROL) || CInputSystem::GetMe()->IsKeyDown(KC_RCONTROL) )

	{
		if( /*pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET || */pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_EQUIP)
			// ������ҵı��� CTRL+ �Ҽ� ���� CTRL+ left

		{
			if(pItem->GetItemClass() == ICLASS_EQUIP)
			{
				//�Ƚ����ܴ��ڵ��Ѿ���������Ʒ�������
				INT nCon = CDataPool::GetMe()->AddStar_GetItemConta();
				INT nPos = CDataPool::GetMe()->AddStar_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_EQUIP:					// ������ҵ�װ��
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nPos));

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

				//��¼������Ʒ��λ�õ����ݳ�
				CDataPool::GetMe()->AddStar_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->AddStar_SetItemPos(pItem->GetPosIndex());


				//�򿪴��ǶԻ���
				CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING);

				// ���������Ʒ
				pItem->SetLock(TRUE);

				if ( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
					//֪ͨ��������״̬
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

				if ( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_EQUIP)
					//֪ͨװ������״̬
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


				return;

			}
			else
			{
				STRING strTemp = "����Ʒ����������";
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
	//			// ʹ�ü������� 
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
						// ʹ�ü������� 
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
			//	// ʹ�ü������� 
			//	STRING strTemp = "";
			//	strTemp = NOCOLORMSGFUNC("UI_OPERATE_IDENTIFY_EQUIP_ONLY");
			//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//	return; 
			//}
			break;
		}

	}

	////�ȼ�⡰Shift���ǲ��ǰ���״̬������ǣ���ô�ȴ����ֵĲ���
	//if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
	//{
	//	//�޸ģ�Ŀǰ��������DoActionʵ�ֲ�֣�����ʹ��DoSubAction���в��
	//	if( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_BANK )
	//	{
	//		//�ж������Ʒ�Ƿ�߱���ֵ�����,��������һ�����Բ��
	//		if(pItem->GetNumber() > 1)
	//		{
	//			//�Ƚ����ܴ��ڵ��Ѿ���������Ʒ�������
	//			INT nCon = CDataPool::GetMe()->Split_GetItemConta();
	//			INT nPos = CDataPool::GetMe()->Split_GetItemPos();
	//			switch( nCon ) 
	//			{
	//			case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
	//				{
	//					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

	//					if(pItem)
	//					{
	//						pItem->SetLock(FALSE);
	//						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	//					}
	//				}
	//				break;
	//			case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
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

	//			//��¼�����Ʒ��λ�õ����ݳ�
	//			CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
	//			CDataPool::GetMe()->Split_SetItemPos(pItem->GetPosIndex());
	//			CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

	//			//�򿪲�ֶԻ���
	//			CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);

	//			// ���������Ʒ
	//			pItem->SetLock(TRUE);

	//			//֪ͨ��������״̬
	//			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
	//		}
	//		else
	//		{
	//			STRING strTemp = "";
	//			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Cannt_chaifen");
	//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
	//			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ���ɲ��");
	//		}
	//	}
	//	return;
	//}

	STRING strTemp = "";

	//������Ʒ���ڵĹ���
	switch(pItem->GetTypeOwner())
	{
		//��ҵİ�̯���棨����
	case tObject_Item::IO_MYSTALLBOX:			//�Լ���̯λ��
		{
			//ֱ��֪ͨ���棬�Լ�ѡ�еı��
			CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,this->GetPosIndex());
		}
		break;
		//��ҵİ�̯���棨��
	case tObject_Item::IO_OTSTALLBOX:			//�Է���̯λ��
		{
			//ֱ��֪ͨ���棬�Լ�ѡ�еı��
			CEventSystem::GetMe()->PushEvent(GE_STALL_BUY_SELECT,this->GetPosIndex());
		}
		break;
		//��ҽ�����ĶԻ�����
	case tObject_Item::IO_MISSIONBOX:
		{
			//�������
			if(pItem)
			{
				INT nBagIndex = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(pItem->GetGUID());

				tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(nBagIndex);
				pBagItem->SetLock(FALSE);
			}

			//������BOX�е�Item���һ�ξ�������ʧ
			CDataPool::GetMe()->MissionBox_SetItem(this->GetPosIndex(), NULL);

			CActionSystem::GetMe()->UserMission_Update();
			//���͸�����Ϣ
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
			//���±���
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
		//��ҵ���������������
	case tObject_Item::IO_MYSELF_BANK:
		{
			CGBankRemoveItem msg;
			msg.SetToType(CGBankRemoveItem::BAG_POS);
			msg.SetIndexFrom(this->GetPosIndex());
			msg.SetIndexTo(255);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
		//�������е���Ʒ
	case tObject_Item::IO_ITEMBOX:
		{
			//����
			if(pItem->GetItemClass() == ICLASS_TASKITEM)
			{
				INT idTable = pItem->GetIdTable();
				INT nCount = CDataPool::GetMe()->UserBag_CountItemByIDTable(idTable);

				DBC_DEFINEHANDLE(s_pItem_Task, DBC_ITEM_MEDIC);
				//������¼
				const _DBC_ITEM_TASK* pTask = (const _DBC_ITEM_TASK*)s_pItem_Task->Search_Index_EQU(idTable);
				if( pTask->nMaxHold != INVALID_ID && nCount >= pTask->nMaxHold )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����ʰȡ������Ʒ��");				
					break;
				}
			}
			CGameProcedure::s_pGameInterface->ItemBox_PickItem(pItem);
			break;
		}
		break;

		
		//����Լ����ϵİ���
	case tObject_Item::IO_MYSELF_PACKET:
		{
			DoAction_Packet();
		}
		break;

	case tObject_Item::IO_BOOTH:
		// ���˵Ļ���,���ͽ�������
		{
			// �������ָ��󣬲����ٹ�����Ʒ
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_You_Can_Not_Repair_Item");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����������Ʒ");
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
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
			}
		}
		break;
	case tObject_Item::IO_MYEXBOX:
		// ���׽�����Լ�����Ʒ����ͷ
		{
			//�����϶���Ʒ����Ϣ
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEITEM);
			//msg.SetFromType(EXCHANGE_MSG::);
			msg.SetFromIndex(this->GetPosIndex());
			msg.SetToType(EXCHANGE_MSG::POS_BAG);
			msg.SetToIndex(0);						//�ɷ���������
			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP :
		// ���action item��װ������.
		{
			if(!CDataPool::GetMe()->Booth_IsClose())
			{
				//������״̬
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
				{
					//�������ǲ���װ��,ֻ��װ����������
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						if( pItem->GetItemDur() < pItem->GetItemMaxDur() )
						{
							
							 CDataPool::GetMe()->ComRepair_SetItemConta(0);//1������ 0����װ��
							 CDataPool::GetMe()->ComRepair_SetItemPos(pItem->GetPosIndex());


							// CEventSystem::GetMe()->PushEvent(GE_OPEN_REPAIR_SURE,0); 	//0��ʾ����һװ��

						 CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex(), FALSE);

						}
						else
						{
							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Not_Require_Repair");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ����Ҫ����");
						}
					}else
					{
						STRING strTemp = "����װ�����ܽ���ά��";
						 
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

				// ����ж��װ����Ϣ.
				msg.setEquipPoint(this->GetPosIndex());
				msg.setBagIndex(-1);

				CNetManager::GetMe()->SendPacket( &msg );
			}
		}
		break;
	case tObject_Item::IO_PS_SELFBOX:
		{
			//ѡ�е���Ʒ��Ҫ�ڽ��л�ҳ���������
			//����̵꣨�Լ��ģ�
			// ��¼��ǰѡ�е���Ʒ
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(this->GetPosIndex()%20);

			CEventSystem::GetMe()->PushEvent(GE_PS_SELF_SELECT);
		}
		break;
	case tObject_Item::IO_PS_OTHERBOX:
		{
			//����̵꣨���˵ģ�
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(this->GetPosIndex()%20);
						
			CEventSystem::GetMe()->PushEvent(GE_PS_OTHER_SELECT);
		}
		break;
	case tObject_Item::IO_ITEMIBSHOP:
		// IB �̳� ,���ͽ�������
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
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
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
		//��֧�ִ�������
		BOOL bChatActive = CGameProcedure::s_pUISystem && 
			CGameProcedure::s_pUISystem->HasInputFocus();
		if( !bChatActive ) return FALSE;

		//����Ʒ֧����Ϣ����
		CTransferItemSystem* transfer = CTransferItemSystem::GetMe();
		tTransferItemSystem::Element* pElement = CTransferItemSystem::GetMe()->ConvertFromItem( pItem );
		if( !pElement ) return FALSE;

		//�����Ϣ�ִ���������ַ����Ƿ񳤶ȳ������Ƶ��ؼ��ڲ�ȥ����
		//	����UIϵͳ
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
		//ˢ���������
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

// �϶�����
VOID CActionItem_Item::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	//�϶�����ʾ���ٵĵط�
	if( bDestory )
	{
		DestoryItem(szSourceName);
		return;
	}
	//�϶����հ׵ط�
	if(!szTargetName || szTargetName[0]=='\0' || GetItemImpl()==0)
	{
		return;
	}

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];
	INT nOldTargetId = -1;
	INT nIndex =-1;
	//����ǿ�����������������ط��ϣ������Լ�����
	if(  cSourceName == 'M' && 
		 cTargetType != 'M' )
		return;

	switch(cTargetType)
	{
	case 'S':		//��̯����
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//����ǲ����ɱ��������Ϲ�����(ֻ�ܽ�����ұ����ڵ���Ʒ)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
				{
					//������Ʒ
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�����ϼ�������Ʒ");
					break;
				}
				// �ж���Ʒ�Ƿ��Ѿ���
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
						break;
					}
				}

				// ̯λ���к�����ϼ� 20100706 BLL
				if ( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_StallOpenPublic() == FALSE )
				{
					break;
				}

				// ���϶�����Ʒ��Ϊ��ǰ����Ʒ
				CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
				
				//ֻ���Լ���̯�ŵ����ϼӽ��棬�޸�������Ұ�̯���������Ʒ�ϼӵ��������bug
				if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE )== CObject_Character::CHARACTER_LOGIC_STALL)	
				{
					// ����������ۼ۸�ĶԻ���
					CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "price");
				}
			}
		}
		break;
	case 'Q':		//�����ύ����(MissionReply)
		{
			INT nIndex = szTargetName[1]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ����ʱֻ֧�ֱ�����
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					INT nGUID = this->GetItemImpl()->GetIdTable();
					//��¡��Ʒ
					tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
					((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

					//�����ﱣ��ֱ��֪ͨ������Ӱ�ť����������������Ϣ��
					CDataPool::GetMe()->MissionBox_SetItem(GetPosIndex(), pNewItem);

					// ���������ж�Ӧ�������Ʒ
					tObject_Item* pMissionItem = this->GetItemImpl();
					pMissionItem->SetLock(TRUE);

					CActionSystem::GetMe()->UserMission_Update();
					//���͸�����Ϣ
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			default:
				break;
			}				
		}
		break;
	case 'B':		//����
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//������Ʒ���ܷ�������
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				return;
			}
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nIndex + this->GetCurBankRentBegin());
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//���������ڲ�
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
	case 'F':		//�̵�
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			//������Ʒ���ܷ����̵�
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				return;
			}
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					char strPrice[20] = "";
					sprintf( strPrice, "%d", GetItemPrice() );

					CEventSystem::GetMe()->PushEvent(GE_OPEN_SALE_CONFIRM, GetName(), strPrice, GetPosIndex(), GetNum());
				}
				break;
			}
		}
		break;

	case 'P':		//����
		{
			INT nTargetIndex = atoi(szTargetName+1)-1;
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			
			switch(nTypeOwner)
			{
			// Shop -> Package
			case tObject_Item::IO_BOOTH:			//����NPC�̵�
				{

					char strPrice[20] = "";

					if ( GetPosIndex() >= 200 ) // ������̵���Ļع���Ʒ
					{
						sprintf( strPrice, "%d", GetItemPrice() );
					}
					else // ������̵���ĳ�����Ʒ
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
					//ͬһ��
					INT nSourcePos = this->GetPosIndex();
					if(nSourcePos == nTargetIndex) 
						break;

					//��ͬ��
					CGPackage_SwapItem msg;
					msg.SetPackageIndex1(GetPosIndex());
					msg.SetPackageIndex2(nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			//SelfEquip -> package
			case tObject_Item::IO_MYSELF_EQUIP:
				{ 
					//����ж��װ������Ϣ
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
					
					//��Ҫ���ж������Ʒ�Ƿ������ƶ�������
					//��ѯ�����Ʒ�ǲ��Ǵ����ϼܵ�״̬
					INT nIndex = pSelectItem->GetPosIndex();
					INT nConIndex = nIndex/20;
					INT nPosition = nIndex%20;
					if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
					{
						//�Ѿ��ϼܣ�����ȡ��
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
	case 'E':		//���׽���,ֻ���϶����Լ��ģ��ұߵģ�������ͷ��1~6��
		{
			INT nIndex = szTargetName[1]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch(nTypeOwner)
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					// �ж���Ʒ�Ƿ��Ѿ���
					tObject_Item* pItem = this->GetItemImpl();
					if(pItem)
					{
						if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
						{
							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
							break;
						}
					}

					//��֤�Ƿ���Խ���(��ֵ����Ʒ���л�ȡ)
					if(!this->GetItemImpl()->Rule(tObject_Item::RULE_EXCHANGE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, this->GetItemImpl()->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
						break;
					}

					//��⽻�����ǲ����п�λ
					if( !CDataPool::GetMe()->MyExBox_IsEmpty() )
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Exchange_Box_Is_Full");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�������Ѿ�����");
						break;
					}

					//�����϶���Ʒ����Ϣ
					CGExchangeSynchItemII msg;
					msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
					msg.SetFromType(EXCHANGE_MSG::POS_BAG);
					msg.SetFromIndex(this->GetPosIndex());
					msg.SetToIndex(0);					//�ɷ���������
					
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}
			
		}
		break;
	case 'R':		//���еĽ����ϵ����������
		{
			INT nIndex = szTargetName[1]-'0';

			//Ŀ����������Ѿ����ˣ��͸�����ʾ�����ڷ������Ϣ
			if(FALSE == CDataPool::GetMe()->UserBank_IsEmpty(nIndex))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Target_Box_Is_Full");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"Ŀ�������Ѿ�����");
				break;
			}

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//������Ʒ���ܷ�������
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				break;
			}

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//�������
				{
					CGBankSwapItem msg;
					msg.SetFromType(CGBankSwapItem::BANK_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					msg.SetToType(CGBankSwapItem::BANK_POS);

					//�������ͬһ���������ӣ���ô�Ͳ�����Ϣ��
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
	case 'G':		//��ʯ�ϳ�/��Ƕ����
		{
			INT nIndex = szTargetName[1]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					if(GetItemImpl()->GetItemClass() == ICLASS_GEM)    //remove
					{
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, nIndex, GetItemImpl()->GetPosIndex());
					}
					else
					{
						STRING strTemp = "��Ҫ��ʯ������Ƕ";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					}
				}
				break;

			default:
				break;
			}

		}
		break;
	case 'T':		// ��˳��＼��ѧϰ /ι�� /ѱ�� /��ͯ /�ӳ����� /��������
		{
			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					tObject_Item *pObj = this->GetItemImpl();
					if(pObj && !(pObj->IsLocked()) && pObj->GetItemClass() == ICLASS_COMITEM)
					{
						CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;

						if (pMedicine_Item->GetItemTableType() == COMMON_HORSE_ITEM)
						{
							// ����������Ʒ��
							DBC_DEFINEHANDLE(s_pMountItemDataDBC, DBC_MOUNT_ITEM_DATA);
							const _DBC_MOUNT_ITEM_DATA* pMIDBC = 
								(const _DBC_MOUNT_ITEM_DATA*)(s_pMountItemDataDBC->Search_Index_EQU( pMedicine_Item->GetIdTable() ));
							if (NULL == pMIDBC)
								break;

							switch (szTargetName[1])
							{
							case 'S': // ������ / ����� / ������
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
							case 'B': // ������	
								{
									if (pMIDBC->nType == HORSE_ITEM_BASIC)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_FEED_ITEM_UPDATE, 0, this->GetID());
									}
								}
								break;
							case 'V': // ������
								{
									if (pMIDBC->nType == HORSE_ITEM_SAVVY)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_FEED_ITEM_UPDATE, 1, this->GetID());
									}
								}
								break;
							case 'T': // ѱ������
								{
									if (pMIDBC->nType == HORSE_ITEM_HAPPINESS)
									{
										pMedicine_Item->SetLock(TRUE);
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
										CEventSystem::GetMe()->PushEvent(GE_MOUNT_TRAIN_ITEM_UPDATE, this->GetID());
									}
								}
								break;
							case 'C': // ��ͯ����
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
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���Ǹ�����ļ���");

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


	case  'C': //���
		{

	
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//���Ա���

				{   

				 
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;

					 
					//�ж�����
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
						//֪ͨ��������״̬
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

						 
					}
					else
					{ 
							STRING strTemp = "����Ʒ�޷���ף�ֻ��װ�����Դ��";
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					}
	
				}

			break;

			case tObject_Item::IO_MYSELF_EQUIP:				//�����Լ�װ��װ��
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
					//֪ͨװ������״̬
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
				}

				break;
			}
		}
		break;
 
	case 'D': //��
		{
			INT nIndex = szTargetName[1]-'0';


			if( 1 == nIndex)  // 1��ʾװ��    2��ʾ����
			{
					
				
				
				  INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

					switch( nTypeOwner )
					{
					case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
						{


							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

						
							//�ж�����
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
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


							}else  
							{ 
								STRING strTemp = "ֻ��װ�����ܽ��а�";
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							}

						}

						break;
					case tObject_Item::IO_MYSELF_EQUIP:				//�����Լ�װ��װ��
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
								//֪ͨװ������״̬
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

						}

						break;

					}


				} else if ( 2 == nIndex)
				{

					//����ǲ����ɱ��������Ϲ�����(ֻ�ܽ�����ұ����ڵ���Ʒ)
					if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)

					{

						CObject_Item* pItem = (CObject_Item*)GetItemImpl();
						if(!pItem) return;


						////�򿪴�ױ� Equip_bind.tab  
						//DBC_DEFINEHANDLE(s_Bang_Ding, DBC_EQUIP_BINDING_INFO);
						////������¼
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
							//֪ͨ��������״̬
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


						//}else

						//{
						//	STRING strTemp = "�˵��߲��Ǵ˴ΰ���Ҫ�ĵ���";
						//	CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

						//}

					}
				}
					
		}

		break;


	case 'V':  //װ������
		{
		

			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

				switch( nTypeOwner )
				{
				case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
					{

						CObject_Item* pItem = (CObject_Item*)GetItemImpl();
						if(!pItem) return;

						//�ж�����
						if(pItem->GetItemClass() == ICLASS_EQUIP)
						{

							tObject_Item *pItemOld =  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->SpRepair_GetItemPos());
							if (pItemOld)
							{
								pItemOld->SetLock(false);

							}


							DBC_DEFINEHANDLE(s_SP_Repair, DBC_EQUIP_SP_REPAIR);
							//������¼
							const _DBC_EQUIP_SP_REPAIR* p_SP_Repair = (const _DBC_EQUIP_SP_REPAIR*)s_SP_Repair->Search_Index_EQU((UINT)11000006 );
							if(!p_SP_Repair) return ;

							CDataPool::GetMe()->SpRepair_SetNeedItemId(p_SP_Repair->nNeedItem);
							CDataPool::GetMe()->SpRepair_SetIntensifyInfo(p_SP_Repair->nIntensifyInfo);
						
							CDataPool::GetMe()->SpRepair_SetItemPos(pItem->GetPosIndex());
							CDataPool::GetMe()->SpRepair_SetItemConta(tObject_Item::IO_MYSELF_PACKET);

							CEventSystem::GetMe()->PushEvent(GE_UPDATE_SP_REPAIR, pItem->GetID());
							pItem->SetLock(true);
							//֪ͨ��������״̬
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


						}else  
						{ 
							STRING strTemp = "ֻ��װ�����ܽ�������";
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						}

					}

					break;
				case tObject_Item::IO_MYSELF_EQUIP:				//�����Լ�װ��װ��
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
						//������¼
						const _DBC_EQUIP_SP_REPAIR* p_SP_Repair = (const _DBC_EQUIP_SP_REPAIR*)s_SP_Repair->Search_Index_EQU((UINT)11000006 );
						if(!p_SP_Repair) return ;

						CDataPool::GetMe()->SpRepair_SetNeedItemId(p_SP_Repair->nNeedItem);
						CDataPool::GetMe()->SpRepair_SetIntensifyInfo(p_SP_Repair->nIntensifyInfo);

						CDataPool::GetMe()->SpRepair_SetItemPos(pItem->GetPosIndex());
						CDataPool::GetMe()->SpRepair_SetItemConta(tObject_Item::IO_MYSELF_EQUIP);

						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SP_REPAIR,pItem->GetID());
						pItem->SetLock(true);
							//֪ͨװ������״̬
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

					}

					break;

				}



		}
		break;

	case  'X':   //װ���Ĳ��
		{

			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
				{

					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;

					//�ж�����
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
						//֪ͨ��������״̬
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
 
					}else  

					{ 
						STRING strTemp = "���߲��ܽ��в��";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					}


				}

			break;

			case tObject_Item::IO_MYSELF_EQUIP:				//�����Լ�װ��װ��
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
						//֪ͨװ������״̬
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


				}

				break;

			}


	}
		break;


	case 'W':			//װ��������
		{
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
				{
					CHAR cTargetType2 = szTargetName[1];
					switch( cTargetType2 )
					{
					case '1':		//����װ����Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//�ж�����
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if ( pItem->GetItemTableType() == EQUIP_SOUXIA )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "����¼�����ڴ�����");
									break;
								}

								if ( pItem->GetNeedLevel() < 40 )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���װ���ȼ�̫�ͣ�40�����ϲſ�������");
									break;
								}

								if ( ((CObject_Item_Equip*)pItem)->GetStartLevel()<5 )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���װ���Ǽ�̫�ͣ�5�����ϲſ�������");
									break;
								}

								if( ( (CObject_Item_Equip*)pItem)->GetGemCount() < 1 )
								{
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���װ����û����Ƕ����ʯ��������ʲô���壬����������");
									break;
								}

								INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
								CEventSystem::GetMe()->PushEvent(GE_ADD_LIANHUAEQUIP_ITEM, pos, GetID() );
								
								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ��װ���ſ��԰���");

						}
						break;

					case '2':		//װ���������ߴ�Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//�ж�����
							if ( pItem->GetItemClass() == ICLASS_COMITEM )
							{
								if ( pItem->GetItemTableType() == COMITEM_EQUIP_REFINING )
								{
									INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
									CEventSystem::GetMe()->PushEvent(GE_ADD_LIANHUAEQUIP_DIS, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//֪ͨ��������״̬
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
									break;
								}
							}
							CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ����������ſ��԰���");
							
						}
						break;

					case '3':		//��ħ�����ڵ�װ����Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//�ж�����
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if ( pItem->GetItemTableType() == EQUIP_CAP ||		//ͷ��
									pItem->GetItemTableType() == EQUIP_SHOULDER	||	//����
									pItem->GetItemTableType() == EQUIP_ARMOR	||		//�·�
									pItem->GetItemTableType() == EQUIP_HAND	||			//����
									pItem->GetItemTableType() == EQUIP_BOOT		//Ь
									)	
								{
									INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
									CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_FUMO_ITEM, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//֪ͨ��������״̬
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
								}
								else
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "����װ�������Ը�ħ");		

							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ��װ���ſ��Ը�ħ");
						}
						break;

					case '4':		//��ħ���� ��ħ���� ��Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//�ж�����
							if(pItem->GetItemClass() == ICLASS_SOUL_BEAD )
							{
									INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
									CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_FUMO_DIS, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//֪ͨ��������״̬
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���ǻ��鲻���԰���");
						}
						break;

					case '5':		//��ħ���� װ����Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//�ж�����
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if( ( (CObject_Item_Equip*)pItem )->GetIsFumo() )
								{
									INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
									CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_TUIMO_ITEM, pos, GetID() );

									CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
									pItem->SetLock(true);
									//֪ͨ��������״̬
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
								}
								else
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "û�и�ħ��װ����������ħ");

							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "����Ʒ��������ħ");
						}
						break;

					case '6':		//��ħ���� ��ħʯ���ð�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//�ж�����
							if((pItem->GetItemClass() == ICLASS_COMITEM) && (pItem->GetItemTableType() == COMITEM_EQUIP_CANCEL_MAGIC))
							{
								INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
								CEventSystem::GetMe()->PushEvent(GE_ADD_EQUIP_TUIMO_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "������ħʯ�����԰���");
						}
						break;
					}

				}
				break;
			}
		}
		break;
	
	case 'O':			//����¼ ��ع��ܽ���
		{
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();

			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
				{
					CHAR cTargetType2 = szTargetName[1];
					switch( cTargetType2 )
					{
					case '1':		//����¼���ߴ�Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							//�ж�����
							if(pItem->GetItemClass() == ICLASS_EQUIP )
							{
								if ( pItem->GetItemTableType() == EQUIP_SOUXIA )
								{
									INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
									CHAR cTargetType3 = szTargetName[2];
									BOOL IsBlankDis = false;		//�ڲ������У��ж�����¼�Ƿ�Ϊ�հ�
									switch ( cTargetType3 )
									{
									case '1':		//�ڲ������¼�����з�������¼����
										{
											IsBlankDis = SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsBlankSouXiaItem(pos);
											if( !IsBlankDis )
												CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIADISMANTLE_ITEM, pos, GetID() );
											else
												CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�հ�����¼�����԰���!");
										}
										break;
									case '2':		//����������¼�����з�������¼����
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIADESTORY_ITEM, pos, GetID() );
										}
										break;
									case '3':		//����������¼�����з�������¼����1
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIACOMPLEX_ITEM1, pos, GetID() );
										}
										break;
									case '4':		//����������¼�����з�������¼����2
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIACOMPLEX_ITEM2, pos, GetID() );
										}
										break;
									case '5':		//�ڷ������¼�����з�������¼����
										{
											CEventSystem::GetMe()->PushEvent(GE_ADD_SOUXIASEW_ITEM1, pos, GetID() );
										}
										break;
									}
									if ( !IsBlankDis )
									{
										CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
										pItem->SetLock(true);
										//֪ͨ��������״̬
										CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
									}
									
								}
								else
									CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ������¼���߲ſ��԰���");
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ������¼���߲ſ��԰���");
						}
						break;

					case '2':		//����¼�����ߴ�Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaItem( pItem->GetPosIndex(), 2) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIADISMANTLE_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ������¼�����߲ſ��԰���");
						}
						break;

					case '3':		//����¼�������ߴ�Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaItem( pItem->GetPosIndex(), 3) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIACOMPLEX_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ������¼�������߲ſ��԰���");
						}
						break;

					case '4':		//����¼��ϵ��ߴ�Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaItem( pItem->GetPosIndex(), 4) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIASEW_DIS, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ������¼����ϵ��߲ſ��԰���");
						}
						break;

					case '5':		//����¼��ϵ��ߴ�Ű�ť
						{
							CObject_Item* pItem = (CObject_Item*)GetItemImpl();
							if(!pItem) return;

							if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiadeformityItem( pItem->GetPosIndex()) )
							{
								pItem->SetLock(true);
								INT pos = pItem->GetPosIndex();		//����ڱ����е�λ��
								CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_SOUXIASEW_DISITEM, pos, GetID() );

								CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
								pItem->SetLock(true);
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
							else
								CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ֻ������¼��ҳ�ſ��԰���");
						}
						break;
					}
				}
				break;
			}
		}
		break;
   
		// װ�������͵���
	case  'H':    
		{

			INT nIndex = szTargetName[1]-'0';
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
			if(!pItem) return;


			if( 1 == nIndex)  // 1��ʾװ��    2��ʾ��һ��װ�� 3��ʾ����
			{	

					switch( nTypeOwner )
					{
					  case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
						 {

							//�ж�����
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
								//֪ͨ��������״̬
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

							}
							else
							{
								STRING strTemp = "ֻ������װ���ſ��ԶԵ�����";
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
						 
								//֪ͨװ������״̬
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

						}
						break;

					}

			}

			else if ( 2 == nIndex)

			{

				switch( nTypeOwner )
				{
				case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
					{	
						//�ж�����
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
							//֪ͨ��������״̬
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

						}else
						{
							STRING strTemp = "ֻ������װ���ſ��ԶԵ�����";
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
							//֪ͨװ������״̬
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


					}
					break;

				}

			}

			else if ( 3 == nIndex)

			{
				//����ǲ����ɱ��������Ϲ�����(ֻ�ܽ�����ұ����ڵ���Ʒ)
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
					//֪ͨ��������״̬
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


				}


			}


		}

		break;
		// װ�������͸���
	case  'I':    
		{

			INT nIndex = szTargetName[1]-'0';
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			CObject_Item_Equip* pItem = (CObject_Item_Equip*)GetItemImpl();
			if(!pItem) return;


			if( 1 == nIndex)  // 1��ʾװ��    2 ��ʾ����
			{	

				switch( nTypeOwner )
				{
				case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
					{

						//�ж�����
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
							//֪ͨ��������״̬
							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

						}
						else
						{
							STRING strTemp = "ֻ������װ���ſ��Ը�������";
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
					 
							//֪ͨװ������״̬
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);


					}
					break;

				}

			}


			else if ( 2 == nIndex)

			{
				//����ǲ����ɱ��������Ϲ�����(ֻ�ܽ�����ұ����ڵ���Ʒ)
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
					//֪ͨ��������״̬
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


				}


			}


		}

		break;


	case  'K'://һ����װ
		{
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//���Ա���
				{
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;

					//�ж�����
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{ 
						int iNeedLevel   = 0;
						int iPlayerLevel = 0;

						if(pItem)
						{
							//��֤�Ƿ����ʹ��
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
							//	STRING strTemp = "ְҵ����";
							//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
							//	return;
							//}
	
							string sTarName(szTargetName);
							string sTarIdx = sTarName.substr(1);

							USHORT uWorld, uServer; UINT uSerial;
							pItem->GetGUID(uWorld, uServer, uSerial);

							// ����lua�ű�������guid�ַ���
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
							//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, _T("�ȼ�����") );
						}
					}
					else
					{ 
						STRING strTemp = "����Ʒ�޷�װ��";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					}
				}
				break;
			}
		}
		break;

	case 'Z':		//����װ������   
		{
			INT nIndex = szTargetName[1]-'0';

			//������Դ��ȷ����δ���
			//���������ط���ֱ�Ӹ�����ʾ����װ��
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//���Ա������ͷ���װ����Ϣ
				{
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;
					//�ж�����
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						int iNeedLevel   = 0;
						int iPlayerLevel = 0;

						tObject_Item* pItem = GetItemImpl();
						if(pItem)
						{
							//��֤�Ƿ����ʹ��
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
							//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, _T("�ȼ�����") );
						}
					}
					else
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Can_Not_Equip");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ�޷�װ��");
					}
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:				//�����Լ�װ��װ��
				{
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//�������У�Ҳ����װ����Ϣ
				{
				}
				break;
			case tObject_Item::IO_MYEXBOX:					//���Խ��׶Ի���
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Exchanging");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ���ڽ���");
				}
				break;
			case tObject_Item::IO_BOOTH:					//�������˻���
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Buy_Item_First");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ȹ������Ʒ");
				}
				break;
			case tObject_Item::IO_MYSTALLBOX:				//�����Լ���̯λ
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Selling");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ���ڰ�̯������");
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'A':		//����̵�
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//�����������ǲ����Ѿ�����Ʒ�����������Ʒ�ǲ����Ѿ��ϼ�
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			tObject_Item* pShopItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nConIndex, nIndex);
			if(pShopItem)
			{
				if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nIndex))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "Ŀ��λ�õ���Ʒ�Ѿ��ϼ�");
					return;
				}
			}

			//����ǲ����ɱ��������Ϲ�����(ֻ�ܽ�����ұ����ڵ���Ʒ)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				// �ж���Ʒ�Ƿ��Ѿ���
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
						break;
					}
					//��Ҫ���ж��ǲ����ܹ��������е���Ʒ
					if( GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
					{
						//������Ʒ���ܷ�������
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToPS_SHOP");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܴ�������");
						break;
					}

				}
				//ִ�н���Ʒת�Ƶ��̵�ȥ�Ĳ���
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
			//�ڱ����ڵ���λ��
			else if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_PS_SELFBOX)
			{
				//����ŵ��Լ����ӣ���ô����Ҫ���������Ϣ
				if(this->GetPosIndex() == nIndex)
				{
					return;
				}
				//ִ�н���Ʒת�Ƶ��̵�ȥ�Ĳ���
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
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ��ϼ�");
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
	case 'M':		//���˵�
		{
			//��ѯ��Դ��ֻ����Դ�ڱ���
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if( this->GetItemImpl()->Rule(tObject_Item::RULE_QUICK) )
				{
					INT nIndex = szTargetName[1]-'0';
					nIndex = szTargetName[2]-'0' + nIndex*10 ;
					//��ѯĿ��λ��ԭ��������
					nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
					CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID() );
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Can_Put_In_ShortCut");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ��������������");
				}
			}
		}
		break;

	case 'Y':		//����ͷ��
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//����װ������
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//ҩƿ
			case ICLASS_TASKITEM:	//������Ʒ��
			case ICLASS_STOREMAP:	//�ر�ͼ
				{		//��̯ʱ���������ʹ����Ʒ
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Stalling");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
						break;
					}

					//��֤�Ƿ����ʹ��
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//�����ȴ�Ƿ����
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
	case 'L':		//�Լ�ͷ��
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//����װ������
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//ҩƿ
			case ICLASS_TASKITEM:	//������Ʒ��
			case ICLASS_STOREMAP:	//�ر�ͼ
				{		//��̯ʱ���������ʹ����Ʒ
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Stalling");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
						break;
					}

					//��֤�Ƿ����ʹ��
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//�����ȴ�Ƿ����
					if(!CoolDownIsOver()) 
					{
						CActionSystem::GetMe()->SetSetoutAction(this);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
						break;
					}

					//����Ŀ��ʹ����Ʒ
					CObject* pMainTarget = (CObject*)CObjectManager::GetMe()->GetMainTarget();
					CGameProcedure::s_pGameInterface->PacketItem_UserItem(this, 
						CObjectManager::GetMe()->GetMySelf()->GetServerID(), fVector2(-1, -1));
				}
				break;
			}

		}
		break;
	case 'N':		//���ǽ���
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//����װ������
			switch(pItem->GetItemClass())
			{
			case ICLASS_EQUIP:	//װ��
				{
					//��̯ʱ���������ʹ����Ʒ
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Stalling");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
						break;
					}
					//�Ƚ����ܴ��ڵ��Ѿ���������Ʒ�������
					INT nCon = CDataPool::GetMe()->AddStar_GetItemConta();
					INT nPos = CDataPool::GetMe()->AddStar_GetItemPos();
					switch( nCon ) 
					{
					case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
						{
							tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

							if(pItem)
							{
								pItem->SetLock(FALSE);
								CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							}
						}
						break;
					case tObject_Item::IO_MYSELF_EQUIP:					// ������ҵ�װ��
						{
							tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nPos));

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

					//��¼������Ʒ��λ�õ����ݳ�
					CDataPool::GetMe()->AddStar_SetItemConta(pItem->GetTypeOwner());
					CDataPool::GetMe()->AddStar_SetItemPos(pItem->GetPosIndex());


					//�򿪴��ǶԻ���
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING);

					// ���������Ʒ
					pItem->SetLock(TRUE);

					//֪ͨ��������״̬
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
					return;

				}
				break;
			default:
				{
					STRING strTemp = "����Ʒ����������";

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
	case  'K'://һ����װ
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			if( !pItem ) 
				return;

			string sSrcName(szSourceName);
			string sSrcIdx = sSrcName.substr(1);
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_QUICK_CHANGE, sSrcIdx.c_str(), "-1" );
		}
		break;

	case  'E'://����
		{
			//�����϶���Ʒ����Ϣ
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEITEM);
			//msg.SetFromType(EXCHANGE_MSG::);
			msg.SetFromIndex(this->GetPosIndex());
			msg.SetToType(EXCHANGE_MSG::POS_BAG);
			msg.SetToIndex(0);						//�ɷ���������
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


//��ѯ�߼�����
STRING CActionItem_Item::GetAttributeValue(LPCTSTR szAttributeName)
{
	tObject_Item* pItem = GetItemImpl();

	if(!pItem) return "";
	else return pItem->GetAttributeValue(szAttributeName);
}


// ��װ��������ж��
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

	// ����ж��װ����Ϣ.
	msg.setEquipPoint((BYTE)(pItemEquip->GetItemType()));
	CNetManager::GetMe()->SendPacket( &msg );
}

// �õ���Ʒ�ĵȼ�
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

// �õ���Ʒ���;�
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



// �õ���Ʒ������;�
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

// �õ���Ʒ���������
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

// �õ���Ʒ�İ���Ϣ 
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

// �õ�װ����2�ΰ���Ϣ 
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
//	�õ�װ����2�ΰ󶨼ӳ�
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

//	�õ�װ����color
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
// �õ���Ʒ����ļ۸�
INT	CActionItem_Item::GetItemBasePrice()
{
	tObject_Item* pItem = GetItemImpl();

	// �����ȡ������Ʒ������۸�����ݲ�ͬ����Ʒ��𣬴���ͬ
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


// �õ���Ʒ����npc�ļ۸�
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

// �õ���Ʒְҵ
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


//�õ�װ������

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



//�õ���Ʒ�Ƿ�Ψһ
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


// �õ���Ʒ��������
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

//�õ�װ���Ŀ�����
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

//�õ�װ������װ��Ϣ
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

//�õ�װ�����ͼ������Ϣ 
 
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

//�õ��������װ��С�������Ϣ 
 
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

//�õ��������װ���󼤻����Ϣ
 
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


//�õ�װ���󼤻����Ϣ
 
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


//�õ�װ��С�������Ϣ
 
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





//�õ�װ������װ��Ϣ
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





//�õ�װ����װ�����������Ϣ

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

//�õ�װ����װ�����������Ϣ

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





//�õ�װ�����Ǽ�

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




// �õ���ɫ���� 
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
// �õ�װ����������
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
// �õ�װ������ 
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

//�õ�װ����������
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

// �õ�action�����õ�item ������(����װ��, ������Ʒ)
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
		("mountitem" == strTypeName)	                       // [7/21/2010 �¾���]
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

// �õ�װ���ϵı�ʯ����
INT CActionItem_Item::GetGemInEquipCount()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	
	if(pItemEquip)
	{
		// �õ��Ѿ���Ƕ��ʯ�ĸ���
		//INT iCount = pItemEquip->GetGemCount();

		// �õ�������Ƕ��ʯ�ĸ���
		INT iCount = pItemEquip->GetGemMaxCount();
		return iCount;
	}
	else
	{
		return 0;
	}
	
}

// �õ���ʯ��ͼ��
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

// �õ�װ���ϵı�ʯ����
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
			strInfo = _T("");//ÿ�ű�ʯ���¼��㸽������
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

// �õ�װ��������������
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

// �õ���ʯ�ȼ�
INT CActionItem_Item::GetGemLevel()
{
	CObject_Item_Gem* pItem = (CObject_Item_Gem*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetGemLevel();
	}

	return -1;
}

// �õ���ʯ������Ϣ
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


//�õ��������������
LPCTSTR CActionItem_Item::GetFecthHeadInfo_TypeDesc( INT Index )
{
	CObject_Item_SoulBead* pItem = (CObject_Item_SoulBead*)GetItemImpl();
	if ( pItem )
	{
		return pItem->GetFecthHeadInfo_TypeDesc(Index);
	}
	return NULL;
}


//�õ����������ֵ
INT CActionItem_Item::GetFecthHeadInfo_Value( INT Index )
{
	CObject_Item_SoulBead* pItem = (CObject_Item_SoulBead*)GetItemImpl();
	if ( pItem )
	{
		return pItem->GetFecthHeadInfo_Value(Index);
	}
	return 0;
}


//�õ��������������
INT	CActionItem_Item::GetFecthHeadInfo_Num()
{
	CObject_Item_SoulBead* pItem = (CObject_Item_SoulBead*)GetItemImpl();
	if( pItem )
		return pItem->GetFecthHeadInfo_Num();
	return 0;
}


// �õ�ԭ�ϻ�ҩƷ������  //remove
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

//�õ�װ������ͨ�Ļ��Ǹ߼���
INT   CActionItem_Item::GetItemEquipQua()

{
	CObject_Item_Equip *pEquip = (CObject_Item_Equip*)GetItemImpl(); 

	if (pEquip)
	{
		return pEquip->GetItemEquipQua();
	}

	return -1;
}






// �õ�����Ʒ�ڱ��е�����
INT CActionItem_Item::GetMedicineItemTableType()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();

	if(pItem)
	{
		return pItem->GetMedicineItemTableType();
	}

	return -1;
}

//�õ���ȴ״��ID
INT	CActionItem_Item::GetCoolDownID(VOID)
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem && pItem->GetItemClass() == ICLASS_COMITEM)
	{
		//ֻ�б��������Ʒ����CoolDown
		if(pItem->GetTypeOwner() != tObject_Item::IO_MYSELF_PACKET)
			return -1;

		INT nSkillTableID = pItem->GetItemSkillID();

		if(nSkillTableID != -1)
		{
			//���
			DBC_DEFINEHANDLE(s_pSkillTable, DBC_SKILL_DATA);
			//������¼
			const _DBC_SKILL_DATA* pSkill = (const _DBC_SKILL_DATA*)s_pSkillTable->Search_Index_EQU((UINT)nSkillTableID);
			if(!pSkill) return NULL;

			return pSkill->m_nCooldownID;
		}
	}
	return -1;
}

//�����ȴ�Ƿ����
BOOL CActionItem_Item::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//����û����ȴ��Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//ȡ����ȴ������
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
		// ȡ������ 
		CancelIdentify();
		return;
	}

	//�������̵꿪�ţ�
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
	{
		
		if(!(CDataPool::GetMe()->PlayerShop_GetShopType(TRUE) == 1))
		{
			return;
		}

		if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
			return;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		}

		//��Ҫ���ж��ǲ����ܹ��������е���Ʒ
		if( GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//������Ʒ���ܷ�������
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToPS_SHOP");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܴ�������");
			return;
		}

		//ִ�н���Ʒת�Ƶ��̵�ȥ�Ĳ���
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
	//�����̯���濪�ţ������Ʒ����ɵ���̯������(�м���Ҫ����һ��ȷ�ϲ���)
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
	{
		// �ж���Ʒ�Ƿ��Ѿ���
		if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
			return;
		}
		//�ж���Ʒ�ǲ���������Ʒ
		if(GetItemType() == ICLASS_TASKITEM )//this->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItemNot2Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������Ʒ�����ϼ�����");
			return;
		}
		//��ѯ̯λ���ǲ��ǻ��п�λ
		if(CDataPool::GetMe()->MyStallBox_IsHaveEmpty(1) == FALSE)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_StallHaveNotEmpty");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "̯λ���Ѿ�û�пո�");
			return;
		}

		//�ȱ���ѡ�е���Ʒ
		CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
		// ͬʱ��¼Item���ڵ�����
		CDataPool::GetMe()->MyStallBox_SetSelectItemInPos(tObject_Item::IO_MYSELF_PACKET);
		//��һ���������۵ĶԻ���
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"price");

		return;
	}
	//����ύ������Ʒ�Ľ��濪��,���������Ʒ�ɵ��ύ����
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
	{
		//��Ҫ��MissionBOX��Ѱ�ҿյ�λ��
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
			//û�пո���Է�����Ʒ�ˣ�������ʾ���˳�
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Misstion_Item_Is_Full");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ�Ѿ������ˣ�");
			return;
		}

		//��¡��Ʒ
		tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
		((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

		//�����ﱣ��ֱ��֪ͨ������Ӱ�ť����������������Ϣ��
		CDataPool::GetMe()->MissionBox_SetItem(nIndex, pNewItem);

		// ���������ж�Ӧ�������Ʒ

		CActionSystem::GetMe()->UserMission_Update();
		// ���͸�����Ϣ
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
		// ����������Ϣ
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

		return;
	}
	//������н���ʱ�򿪵ģ�������������
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Bank"))
	{
		//��Ҫ���ж��ǲ����ܹ��������е���Ʒ
		if( GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//������Ʒ���ܷ�������
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToBank");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܴ�������");
			return;
		}	
		
		//��Ҫ��⵱ǰ���д򿪵��ǵڼ���������
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

		//�������������ǲ����п�λ
		if( !CDataPool::GetMe()->UserBank_IsEmpty( nRentIndex ) )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Target_Box_Is_Full");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"Ŀ�������Ѿ�����");
			return;
		}

		CNetManager::GetMe()->SendPacket(&msg);

		return;
	}
	// ������׽��濪�ţ������������Ʒ���뽻����
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		// �ж���Ʒ�Ƿ��Ѿ���
		if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Is_Binded");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
			return;
		}
		//�ж��ǲ����ܹ��������е���Ʒ
		if(GetItemType() == ICLASS_TASKITEM )//pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//������Ʒ���ܽ���
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_MissionItem_Cannt_ToExchange");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܽ��н���");
			return;
		}
		//��֤�Ƿ���Խ���
		if(!pItem->Rule(tObject_Item::RULE_EXCHANGE))
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
			return;
		}
		
		//�����϶���Ʒ�����׽������Ϣ
		// ��Ҫ�ȼ���Ƿ��׽�������
		CGExchangeSynchItemII msg;
		msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
		msg.SetFromType(EXCHANGE_MSG::POS_BAG);
		msg.SetFromIndex(this->GetPosIndex());
		msg.SetToIndex(0);						//�ɷ���������
		CNetManager::GetMe()->SendPacket(&msg);

		return;
	}

	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Enchase"))
	{
		INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
		switch( nTypeOwner ) 
		{
		case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
			{
				if(GetItemImpl()->GetItemClass() == ICLASS_GEM)    //remove
				{
					//CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
					//if(pCharData)
					//{
					//	if( GetItemImpl()->GetNeedLevel() <= pCharData->Get_Level() ){}
					//	{
					//		STRING strTemp = "�ȼ�������������Ƕ";
					//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					//		return;
					//	}
					//	if( GetItemImpl()->GetItemProfession() != pCharData->Get_MenPai() )
					//	{
					//		STRING strTemp = "ְҵ���ԣ�������Ƕ";
					//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					//		return;
					//	}
					//}

					CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, "-1", "-1", GetItemImpl()->GetPosIndex());
				}
				else
				{
					STRING strTemp = "��Ҫ��ʯ������Ƕ";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				}
			}
		}

		return;
	}

	//�����Ʒ�����ǿ���,�����������
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
	{
		//�Ƿ�������״̬
		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
		{
			//ȡ������
			CGameProcedure::s_pGameInterface->Skill_CancelAction();
		}
		//ִ������
		else
		{
			////������Ʒ��������
			if( GetItemType() != ICLASS_TASKITEM )//pItem->GetPosIndex() < TASK_CONTAINER_OFFSET )
			{
				CGameProcedure::s_pGameInterface->Booth_SellItem( pItem );
			}
			else
			{
				//������Ʒ����������Ϣ��ʾ
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
		//����װ������
		switch(pItem->GetItemClass())
		{
		case ICLASS_EQUIP:
			//װ��
			{
				//ʹ��
				CGameProcedure::s_pGameInterface->PacketItem_UserEquip( pItem->GetPosIndex() );
			}
			break;

		case ICLASS_COMITEM:	//ҩƿ
		case ICLASS_TASKITEM:	//������Ʒ��
		case ICLASS_STOREMAP:	//�ر�ͼ
		case ICLASS_SOUL_BEAD:	//����
			{
				int iType = pItem->GetItemTableType();

				//if ( iType == COMITEM_SOUXIA )
				//{
				//	CEventSystem::GetMe()->PushEvent(GE_OPEN_SOUXIA);
				//	break;
				//}

				if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaUsbItem(0,pItem) )		//������¼USB���������
				{
					INT type = SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.GetSouXiaUsbType(pItem);
					if( type != -1 )
						CEventSystem::GetMe()->PushEvent( GE_OPEN_SOUXIA, type, 1 );
					break;
				}

				if ( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiadeformityItem(pItem) )	//�Ƿ�������¼��ҳ
				{
					SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.ShowSouXiaDeformity(pItem);
				}

				//��̯ʱ���������ʹ����Ʒ
				if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_You_Stalling");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
					break;
				}

				//��֤�Ƿ����ʹ��
				if(!pItem->Rule(tObject_Item::RULE_USE))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
					break;
				}

				//�����ȴ�Ƿ����
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
			//��ʯ
			{
			}
			break;
		//���� [7/8/2010 �¾���] 
		case ICLASS_MOUNT:
			{
			    CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
				const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_ITEM_MOUNT_ATT );
				const _DBC_ITEM_MOUNT_ATT* pMountAtt = (const _DBC_ITEM_MOUNT_ATT*)pdbcbase->Search_Index_EQU(pItem->GetIdTable());


				if((pCharData != NULL) && (pCharData->Get_Level()< pMountAtt->nNeedLevel))
				{
					// ��ʾ�������
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

//������Ʒ�Ĳ���
VOID CActionItem_Item::DestoryItem(LPCSTR szSourceName)
{
	CHAR cSourceType = szSourceName[0];
	switch(cSourceType)
	{
	case 'M':		//���˵�
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
		// ������Ʒ�������٣���һ����ʾ                  
		if((nTypeOwner == tObject_Item::IO_MYSELF_PACKET) && (GetItemType() == ICLASS_TASKITEM) )// && this->GetPosIndex() >= TASK_CONTAINER_OFFSET)
		{
			// ��Ҫ��ѯ���Ƿ��������װ��
			//CEventSystem::GetMe()->PushEvent(GE_OPEN_CANNT_DISCARD_ITEM,this->GetItemImpl()->GetName());
			// ������Ļ��ʾ

			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Item_Can_Not_Drop");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܶ���");
		}
		else
		{
			// ��ȡ�����ܴ��ڵ�����
			INT nTypeOwner	=	CDataPool::GetMe()->DisCard_GetItemConta();
			INT	nItemPos	=	CDataPool::GetMe()->DisCard_GetItemPos();

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
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
					
			// ��Ҫ��ѯ���Ƿ��������װ��
			CEventSystem::GetMe()->PushEvent(GE_OPEN_DISCARD_ITEM_FRAME,this->GetItemImpl()->GetName());

			// ���������Ʒ
			tObject_Item *pObj = this->GetItemImpl();
			pObj->SetLock(TRUE);

			//֪ͨ��������״̬
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			CDataPool::GetMe()->DisCard_SetItemConta(this->GetItemImpl()->GetTypeOwner());
			CDataPool::GetMe()->DisCard_SetItemPos(this->GetPosIndex());
		}
	}
}

// �õ��䷽��Ҫ�ļ������ơ�
LPCTSTR CActionItem_Item::GetPrescrNeedSkillName()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;


		// �����
		DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
		_DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);

		//�䷽��
		DBC_DEFINEHANDLE(s_pAbility, DBC_LIFEABILITY_DEFINE);
		_DBC_LIFEABILITY_DEFINE* pData = (_DBC_LIFEABILITY_DEFINE*)s_pAbility->Search_Index_EQU(pLifeInfo->nLifeAbility);

		if(pData)
		{
			return pData->szName;
		}
	}

	return _T("");
			
}

// �õ��䷽��Ҫ�ļ��ܵȼ���
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

// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
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

// ����������ϸ������Ϣ
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

// �õ�װ���ļ�����Ϣ
INT CActionItem_Item::GetEquipIdentify()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipAttrib();
	}

	return 0;
	
}

// ʹ�ü������� 
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

					// ʹ�þ���ı�������λ��
					CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->m_iIdentifyInBagIndex);

					int iIdentifyType = pItem->GetItemTableType();
					switch(iIdentifyType)
					{
					case CObject_Item_Medicine::IDEN_EQUIP:
						{

							if(!pItemEquip->IsWeapon())
							{
								// ֻ�ܼ�������
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
								// ֻ�ܼ�������
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
								// ֻ�ܼ���װ��Ʒ��
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

					// ���ͼ�����Ϣ
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



	// ȡ������״̬ 
VOID CActionItem_Item::CancelIdentify()
{
	// ����action button
	CDataPool::GetMe()->Identify_UnLock();
}

// ׼��ʹ�ü������� 
VOID CActionItem_Item::PrepareUseIdentify()
{
	if(CActionSystem::GetMe()->GetDefaultAction()==CActionItem_MouseCmd_Identify::GetMe())
	{
		//�Ѿ��������״̬
		return;
	}

	// ʹ�þ���ı�������λ��
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

				// ��¼����λ�á�
				CDataPool::GetMe()->Identify_Lock(pItem->GetPosIndex());
			}
		}
	}
}

	
// �õ���������
LPCTSTR	CActionItem_Item::GetTypeDesc()
{
	// ʹ�þ���ı�������λ��
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemTableTypeDesc();
	}

	return "";
}

// �õ����������ٶ�   [7/21/2010 �¾���]
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

// �õ��������ʱ��   [7/21/2010 �¾���]
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

// �õ������ȡʱ��   [7/21/2010 �¾���]
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

// �õ�����ʣ��ʹ��ʱ��   [7/21/2010 �¾���]
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

// �õ����ﵱǰ�Ƿ��Ѱ�   [7/21/2010 �¾���]
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

// �õ����������  [7/21/2010 �¾���]
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