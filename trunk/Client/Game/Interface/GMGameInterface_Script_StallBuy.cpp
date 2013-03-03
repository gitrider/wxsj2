
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "GIUtil.h"
#include "LuaPlus.h"
#include "Global.h"
#include "Procedure/GameProcedure.h"
#include "GIUISystem.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Event/GMEventSystem.h"
#include "Network/NetManager.h"
#include "DataPool/GMDataPool.h"
#include "object/Logic/character/obj_playermyself.h"
#include "DataPool/GMDP_CharacterData.h"
#include "Action/GMActionSystem.h"

#include "CGStallOpen.h"
#include "CGStallBuy.h"
#include "CGBBSApply.h"

#include "CGStallOpenPublic.h"	// ̯λ����	20100706 BLL

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	StallBuy StallBuy::s_StallBuy;
	LuaPlus::LuaObject* StallBuy::s_pMetaTable = NULL;

	//�ĵ�����Ƿ����
	INT StallBuy::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_StallBuy)
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}
	//��������˰��
	INT StallBuy::GetTradeTax(LuaPlus::LuaState* state)
	{
		state->PushInteger( CDataPool::GetMe()->OtStallBox_GetTradeTax() );
		return 1;
	}
	//�򿪱��˵�̯λ
	INT StallBuy::OpenStall(LuaPlus::LuaState* state)
	{
		if(this == &s_StallBuy)
		{
			CObject* pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
			if(pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_Character)))
			{
				//�Ȳ�ѯ�����ɫ�Ƿ��ڰ�̯״̬
				if(((CObject_Character*)pChar)->GetCharacterData()->Get_IsInStall())
				{
					//���ж������ǲ��ǺϷ�
					const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
					const fVector3 pOtherPos = ((CObject*)pChar)->GetPosition();

					FLOAT fDistance = 
						KLU_GetDist(fVector2(pMePos.x, pMePos.z),
									fVector2(pOtherPos.x, pOtherPos.z));

					if( EXCHANGE_MAX_DISTANCE < fDistance )
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_StallBuy_Info_Too_Far_To_Open_Stall");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����̫Զ�����ܴ����̯λ");
						return 0;
					}

					//���ʹ�̯λ����Ϣ
					CGStallOpen OpenStallMsg;
					OpenStallMsg.SetObjID(pChar->GetServerID());
					CNetManager::GetMe()->SendPacket(&OpenStallMsg);

					//ͬʱ��������StallMessage����Ϣ
					CGBBSApply BbsMsg;
					BbsMsg.SetObjID(pChar->GetServerID());
					CNetManager::GetMe()->SendPacket(&BbsMsg);
				}
				return 0;
			}
		}
		return 0;
	}
	//���ָ��λ�õĶ���
	INT StallBuy::GetPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:StallBuy::GetPrice[2] param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:StallBuy::GetPrice[3] param parameter error");
		}
		std::string str = args[2].GetString();
        INT nIndex = args[3].GetInteger();
		//state->PushInteger(CDataPool::GetMe()->MyStallBox_GetItemPrice(nIndex));
		INT nMoney;
		if( str == "item" )
		{
			nMoney = CDataPool::GetMe()->OtStallBox_GetItemPrice(nIndex);
		}
		else
		{
			nMoney = CDataPool::GetMe()->OtStallBox_GetPetPrice(nIndex);
		}

		state->PushInteger( nMoney );
		return 1;
	}
	//������Ʒ
	INT StallBuy::BuyItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:StallBuy::BuyItem param parameter error");
		}
 		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:StallBuy::BuyItem param parameter error");
		}
		INT nActionID = args[2].GetInteger();
		INT nIndex = args[3].GetInteger();
		//state->PushInteger(CDataPool::GetMe()->MyStallBox_GetItemPrice(nIndex));

		// ʹ��IDȥ�����Ʒָ��
		CActionItem * pActionItem = (CActionItem *)CActionSystem::GetMe()->GetAction(nActionID);

		if(!pActionItem)
			return 0;
		tObject_Item* pItem = (tObject_Item*)pActionItem->GetImpl();

		// ������Ϣ���߷�����
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;

		CGStallBuy msg;
		//tObject_Item* p= CDataPool::GetMe()->MyStallBox_GetCurSelectItem();
		pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);

		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

		msg.SetObjID(CDataPool::GetMe()->OtStallBox_GetObjID());
		msg.SetObjGUID(Guid);
		msg.SetSerial(CDataPool::GetMe()->OtStallBox_GetItemSerial(nIndex));

		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	//��̯λ��Ϣ
	INT StallBuy::OpenMessageBuy(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallMessage"))
		{
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
			return 0;
		}

		//����������Ϣ����Ϣ
		CGBBSApply msg;
		msg.SetObjID(CDataPool::GetMe()->OtStallBox_GetObjID());

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}

	//���̯λ��
	INT StallBuy::GetStallName(LuaPlus::LuaState* state)
	{
		LPCTSTR pszStallName = CDataPool::GetMe()->OtStallBox_GetStallName();
		state->PushString(pszStallName);
		return 1;
	}

	//��ù����
	INT StallBuy::GetAdvertise(LuaPlus::LuaState* state)
	{
		LPCTSTR pszStallName = CDataPool::GetMe()->OtStallMsg_GetTitle();
		state->PushString(pszStallName);
		return 1;
	}
	//�ر�
	INT StallBuy::CloseStallMessage(LuaPlus::LuaState* state)
	{
		//�ر�StallMessage
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);
		return 0;
	}
	//����̯����GUID
	INT StallBuy::GetGuid(LuaPlus::LuaState* state)
	{
		UINT unNum = CDataPool::GetMe()->OtStallBox_GetOwnerGUID();

		CHAR str[32];
		sprintf(str,"%X",unNum);

		state->PushString(str);

		return 1;
	}
	//����̯��������
	INT StallBuy::GetStallerName(LuaPlus::LuaState* state)
	{
		state->PushString(CDataPool::GetMe()->OtStallBox_GetOwnerName());
		return 1;
	}
	// ��ó���ĸ���
	INT StallBuy::GetPetNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->OtStallBox_GetCount());
		return 1;
	}
	// ��ó���
	INT StallBuy::EnumPet(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:StallSale::EnumPet[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->OtStallBox_GetPet(nIndex);
		if(pPet)
		{
		
			STRING szName = pPet->m_szName;

			state->PushString(szName.c_str());

		}else 

		{	
			state->PushString("");

		}

		return 1;
	}
	// ���ȱʡҳ
	INT StallBuy::GetDefaultPage(LuaPlus::LuaState* state)
	{
		INT nPage = CDataPool::GetMe()->OtStallBox_GetDefaultPage();
		
		state->PushInteger(nPage);
		return 1;
	}
	//�������
	INT StallBuy::BuyPet(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:StallBuy::BuyItem param parameter error");
		}

		INT nIndex = args[2].GetInteger();

		if( NULL == CDataPool::GetMe()->OtStallBox_GetPet(nIndex) )
		{
			return 0;
		}
		
		CGStallBuy msg;
		msg.SetPetGUID(CDataPool::GetMe()->OtStallBox_GetPet(nIndex)->m_GUID);
		msg.SetSerial(CDataPool::GetMe()->OtStallBox_GetPetSerial(nIndex));
		msg.SetObjID(CDataPool::GetMe()->OtStallBox_GetObjID());
		
		CNetManager::GetMe()->SendPacket(&msg);*/
		return 0;
	}

}