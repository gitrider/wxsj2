
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

	// 得到银行是否存在
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

	// 打开存钱界面（输入存钱数量的界面）
	INT Bank::OpenSaveFrame(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"save");
		return 0;
	}

	// 打开取钱界面（输入取钱数量的界面）
	INT Bank::OpenGetFrame(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"get");
		return 0;
	}

	// 打开密码输入界面
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

	//金钱的转换
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
		检查数值的大小不能大于1亿，如果已经输入了
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

	//发送存钱消息
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
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"存钱数目超过身上金钱数目");
			}
		}
		return 0;
	}
	//发送取钱消息
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
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"取钱数目超过银行中现有金钱数目");
		}
		return 0;
	}
	//导出银行的金钱数
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
	//发送一个移动银行物品到背包的消息
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
	//导出现在拥有的租赁箱的个数
	INT Bank::GetRentBoxNum(LuaPlus::LuaState* state)
	{
		INT nRentBosNum = -1;
		//格子的最大只
		INT nEndIndex = CDataPool::GetMe()->UserBank_GetBankEndIndex();
		//查询现在拥有的租赁箱个数
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

	//导出指定租赁箱的开始数和格子数
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

	//设置当前的租赁箱
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
	//转换货币
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
	//关闭银行
	INT Bank::Close(LuaPlus::LuaState* state)
	{
		//直接发送关闭InputMoney的事件
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);

		return 0;
	}

	//获得操作物品
	INT Bank::EnumItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		//检查参数
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Bank::EnumItem Wrong Param1");
		}

		INT nIndex = args[2].GetInteger();
		
		//查询物品
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

		//非法ActionItem
		LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();
		state->PushBoolean(false);

		return 2;
	}


};