
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Object/Manager/ObjectManager.h"
#include "Event/GMEventSystem.h"
#include "Network/NetManager.h"
#include "DataPool/GMDataPool.h"
#include "Action/GMActionSystem.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "object/Logic/character/obj_playermyself.h"
#include "DataPool\GMDP_CharacterData.h"
#include "CGBankMoney.h"
#include "CGBankRemoveItem.h"
#include "procedure/GameProcedure.h"

#include "CGBankAcquireList.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Bank Bank::s_Bank;
	LuaPlus::LuaObject* Bank::s_pMetaTable = NULL;

	// �õ������Ƿ����
	INT Bank::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_Bank)
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

	// �򿪴�Ǯ���棨�����Ǯ�����Ľ��棩
	INT Bank::OpenSaveFrame(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"save");
		return 0;
	}

	// ��ȡǮ���棨����ȡǮ�����Ľ��棩
	INT Bank::OpenGetFrame(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"get");
		return 0;
	}

	// �������������
	INT Bank::OpenInputFrame(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:OpenInputFrame param parameter error");
		}
		char* password = (char*)args[2].GetString();
		int len = (int)strlen( password );
		CGBankAcquireList Msg;
		Msg.SetPassWord( password );
		Msg.SetPwdSize( len );
		CNetManager::GetMe()->SendPacket(&Msg);

		return 0;
	}

	//��Ǯ��ת��
	INT Bank::GetInputMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:GetInputMoney param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("LUA:GetInputMoney param parameter error");
		}
		if (!(args[4].IsString()))
		{
			KLThrow("LUA:GetInputMoney param parameter error");
		}

		/*
		�����ֵ�Ĵ�С���ܴ���1�ڣ�����Ѿ�������
		*/
		std::string szGold			= args[2].GetString();
		std::string szSilver		= args[3].GetString();
		std::string szCopperCoin	= args[4].GetString();

		if(szGold.size() > 6)
			szGold.resize(6);

		if(szSilver.size() > 2)
			szSilver.resize(2);

		if(szCopperCoin.size() > 2)
			szCopperCoin.resize(2);

		INT nGold			= 0;
		INT nSilver			= 0;
		INT nCopperCoin		= 0;
		bool bAvailability = true;
		INT nMoney = 0;

		sscanf(szGold.c_str(),"%d",&nGold);
		sscanf(szSilver.c_str(),"%d",&nSilver);
		sscanf(szCopperCoin.c_str(),"%d",&nCopperCoin);

		nMoney = nCopperCoin + nSilver*100 + nGold*10000;

		state->PushBoolean(bAvailability);
		state->PushInteger(nMoney);
		return 2;
	}

	//���ʹ�Ǯ��Ϣ
	INT Bank::SaveMoneyToBank(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:SaveMoneyToBank param parameter error");
		}

		UINT	Amount = static_cast<UINT>(args[2].GetInteger());

		if(Amount == 0)
			return 0;

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		if(pCharData)
		{
			if((UINT)(pCharData->Get_Money()) >= Amount)
			{
				CGBankMoney msg;
				msg.SetIsSave(CGBankMoney::SAVE_MONEY);
				msg.SetAmount(Amount);
				CNetManager::GetMe()->SendPacket(&msg);
			}
			else
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Bank_Info_Input_Money_Out_Cur_Money");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ��Ŀ�������Ͻ�Ǯ��Ŀ");
			}
		}
		return 0;
	}
	//����ȡǮ��Ϣ
	INT Bank::GetMoneyFromBank(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMoneyFromBank param parameter error");
		}

		UINT	Amount = static_cast<UINT>(args[2].GetInteger());
		
		if(Amount == 0)
			return 0;

		if((UINT)(CDataPool::GetMe()->UserBank_GetBankMoney()) >= Amount)
		{
			CGBankMoney msg;
			msg.SetIsSave(CGBankMoney::PUTOUT_MONEY);
			msg.SetAmount(Amount);
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Bank_Info_Output_Money_Out_Bank_Money");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"ȡǮ��Ŀ�������������н�Ǯ��Ŀ");
		}
		return 0;
	}
	//�������еĽ�Ǯ��
	INT Bank::GetBankMoney(LuaPlus::LuaState* state)
	{
		INT nMoney = CDataPool::GetMe()->UserBank_GetBankMoney();
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
	//����һ���ƶ�������Ʒ����������Ϣ
	INT Bank::MoveItemToPacket(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:MoveItemToPacket param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		CGBankRemoveItem msg;
		msg.SetToType(CGBankRemoveItem::BAG_POS);
		msg.SetIndexFrom(nIndex);
		msg.SetIndexTo(255);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//��������ӵ�е�������ĸ���
	INT Bank::GetRentBoxNum(LuaPlus::LuaState* state)
	{
		INT nRentBosNum = -1;
		//���ӵ����ֻ
		INT nEndIndex = CDataPool::GetMe()->UserBank_GetBankEndIndex();
		//��ѯ����ӵ�е����������
		if( RENTBOX2_START_INDEX == nEndIndex )
		{
			nRentBosNum = 1;
		}
		else if ( RENTBOX3_START_INDEX == nEndIndex )
		{
			nRentBosNum = 2;
		}
		else if ( RENTBOX4_START_INDEX == nEndIndex )
		{
			nRentBosNum = 3;
		}
		else if ( RENTBOX5_START_INDEX == nEndIndex )
		{
			nRentBosNum = 4;
		}
		else if ( MAX_BANK_SIZE == nEndIndex )
		{
			nRentBosNum = 5;
		}
		state->PushInteger( nRentBosNum );
		return 1;
	}

	//����ָ��������Ŀ�ʼ���͸�����
	INT Bank::GetRentBoxInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetRentBoxInfo param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		INT nGridNum = 0;
		INT nBeginIndex;

		switch( nIndex ) 
		{
		case 1:
			{
				nBeginIndex = 0;
				nGridNum = RENTBOX2_START_INDEX;
			}
			break;
		case 2:
			{
				nBeginIndex = RENTBOX2_START_INDEX;
				nGridNum = RENTBOX3_START_INDEX - RENTBOX2_START_INDEX;
			}
			break;
		case 3:
			{
				nBeginIndex = RENTBOX3_START_INDEX;
				nGridNum = RENTBOX4_START_INDEX - RENTBOX3_START_INDEX;
			}
			break;
		case 4:
			{
				nBeginIndex = RENTBOX4_START_INDEX;
				nGridNum = RENTBOX5_START_INDEX - RENTBOX4_START_INDEX;
			}
			break;
		case 5:
			{
				nBeginIndex = RENTBOX5_START_INDEX;
				nGridNum = MAX_BANK_SIZE - RENTBOX5_START_INDEX;
			}
			break;
		default:
			break;
		}
		state->PushInteger( nBeginIndex );
		state->PushInteger( nGridNum );
		return 2;
	}

	//���õ�ǰ��������
	INT Bank::SetCurRentIndex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetRentBoxInfo param parameter error");
		}
		INT nIndex = args[2].GetInteger();
		
		CActionSystem::GetMe()->SetCurBankRentBoxIndex(nIndex);
		return 0;
	}
	//ת������
	INT Bank::TransformCoin(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:Bank::TransformCoin param parameter error");
		}
		INT nMoney = args[2].GetInteger();

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

		state->PushInteger( nGoldCoin );
		state->PushInteger( nSilverCoin );
		state->PushInteger( nCopperCoin );
		return 3;
	}
	//�ر�����
	INT Bank::Close(LuaPlus::LuaState* state)
	{
		//ֱ�ӷ��͹ر�InputMoney���¼�
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);

		return 0;
	}

	//��ò�����Ʒ
	INT Bank::EnumItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		//������
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Bank::EnumItem Wrong Param1");
		}

		INT nIndex = args[2].GetInteger();
		
		//��ѯ��Ʒ
		INT nOffIndex = -1;

		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_BANKITEM);
		if(pActionItem)
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


};