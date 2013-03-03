
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Global.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Event/GMEventSystem.h"
#include "Network/NetManager.h"
#include "DataPool/GMDataPool.h"
#include "Object/Logic/character/obj_playermyself.h"
#include "DataPool/GMDP_CharacterData.h"
#include "Procedure\GameProcedure.h"
#include "GIUISystem.h"

#include "CGBankMoney.h"
#include "CGBankRemoveItem.h"

#include "ExchangeMsgDefine.h"
#include "CGExchangeApplyI.h"
#include "CGExchangeReplyI.h"
#include "CGExchangeCancel.h"
#include "CGExchangeSynchLock.h"
#include "CGExchangeOkIII.h"
#include "CGExchangeSynchMoneyII.h"
#include "CGExchangeSynchItemII.h"
#include "CGItemSynch.h"
#include "GIUtil.h"

#include "Object\Logic\Character\AI\GMAI_MySelf.h"
#include "procedure/GameProcedure.h"
#include "procedure/GamePro_Main.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Exchange Exchange::s_Exchange;
	LuaPlus::LuaObject* Exchange::s_pMetaTable = NULL;

	//的到玩家是否存在
	INT Exchange::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_Exchange)
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

	//申请交易
	INT Exchange::SendExchangeApply(LuaPlus::LuaState* state)
	{
		LuaStack args(state);		

		/// ---------------------------------------------------------------------
		std::string strChatObjName = "";

		// 如果是由聊天框的玩家名称右键菜单的交易项单击事件传来的交易玩家名称，且不为空	20100524LL
		if ( args[2].IsString() )
		{
			strChatObjName = args[2].GetString();
			CObject_Character* pCharObj = CObjectManager::GetMe()->FindCharacterByName( strChatObjName );

			if ( pCharObj != NULL )
			{
				CObjectManager::GetMe()->SetMainTarget( pCharObj->GetServerID() );
			}
			else // 找不到对象直接返回，防止当前已有选择目标，通过聊天框选择的交易玩家此时已不在场景或下线时，错给当前选择目标发送交易申请
				return 0;
		}

		/// ---------------------------------------------------------------------
		
		// 判断自己状态可否交易		20100709 BLL

		// 如果自己正在摆摊，不能交易( "你正在摆摊！" )
		if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMAI_Base_Info_You_Stalling" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// 普通跟随状态不能交易( "你正在跟随别人，不能交易" )
		if( ( (CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI() )->IsFollow() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// 组队跟随状态不能交易( "你正在跟随别人，不能交易" )
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if( bTeamFollowMode == TRUE )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		// 普通跑步状态不能交易( "你正在行进，不能交易" )
		if( CObjectManager::GetMe()->GetMySelf()->IsMoving() )   
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange");
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// 自动跑步状态不能交易( "你正在行进，不能交易" )
		if(	CGameProcedure::s_pProcMain->IsAutoRun() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		/// --------------------------------------------------------------------------------------------

		// 判断对方状态可否交易
		if( this == &s_Exchange )
		{
			CObject* pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();

			if( pChar && g_theKernel.IsKindOf( pChar->GetClass(), GETCLASS(CObject_Character) ) )
			{
				// 不能向已经死亡的玩家发送交易申请( "不能向已经死亡的玩家申请交易" )
				if ( ((CObject_Character*)pChar)->CharacterLogic_Get( CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )	
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Can_Exchange_Width_dead_Man" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					return 0;
				}

				// 先判定双方距离是不是合法
				if(pChar)
				{
					const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
					const fVector3 pOtherPos = ((CObject*)pChar)->GetPosition();

					FLOAT fDistance = KLU_GetDist( fVector2( pMePos.x, pMePos.z ), fVector2( pOtherPos.x, pOtherPos.z ) );

					// "与对方距离太远，不能请求交易"
					if( EXCHANGE_MAX_DISTANCE < fDistance )
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Too_Far_To_Exchange" );
						CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
						return 0;
					}
				}

				CGExchangeApplyI msg;
				msg.SetObjID( pChar->GetServerID() );
				CNetManager::GetMe()->SendPacket(&msg);

				// "交易请求已经发送"
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Send_Exchange_Msg" );
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				return 0;
			}
		}

		return 0;
	}

	// 打开交易对话框
	INT Exchange::OpenExchangeFrame(LuaPlus::LuaState* state)
	{
		// 判断自己状态是否合法 20100709 BLL

		// "自己已死亡不能申请交易"
		if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Dead_Can_Not_Exchange");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			return 0;
		}

		// 如果自己正在摆摊，不能交易
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
			return 0;
		}

		/// --------------------------------------------------------------------------------------------

		// 普通跟随状态不能交易( "你正在跟随别人，不能交易" )
		if( ( (CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI() )->IsFollow() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// 组队跟随状态不能交易( "你正在跟随别人，不能交易" )
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if( bTeamFollowMode == TRUE )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Follow_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		// 普通跑步状态不能交易( "你正在行进中，不能交易" )
		if( CObjectManager::GetMe()->GetMySelf()->IsMoving() )   
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange");
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}

		// 自动跑步状态不能交易( "你正在行进中，不能交易" )
		if(	CGameProcedure::s_pProcMain->IsAutoRun() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange" );
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			return 0;
		}	

		/// --------------------------------------------------------------------------------------------

		// 判断对方状态可否交易

		// 从数据池列表中取出一个请求与自己交易的人的ID
		if( CDataPool::GetMe()->IsStillAnyAppInList() )
		{
			INT nUserID = CDataPool::GetMe()->GetFromAppList();

			tObject *pObj = CObjectManager::GetMe()->FindServerObject(nUserID);

			//做一个距离判定，如果距离大于交易允许的最大距离，提示失败
			if( pObj != NULL )
			{
				const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
				const fVector3 pOtherPos = ((CObject*)pObj)->GetPosition();

				FLOAT fDistance = KLU_GetDist( fVector2( pMePos.x, pMePos.z ), fVector2( pOtherPos.x, pOtherPos.z ) );

				if( EXCHANGE_MAX_DISTANCE < fDistance )
				{
					// "与对方距离太远，不能开启交易"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_TooFar_Accept_Exchange" );
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return 0;
				}

				if( ((CObject_Character*)(CObjectManager::GetMe()->FindServerObject(nUserID) ))->CharacterLogic_Get(CObject_Character::LOGIC_BASE) != CObject_Character::CHARACTER_LOGIC_IDLE )
				{
					// "那个玩家正处于特殊状态，不能交易"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Can_Exchange_With_Player_in_Special_Status");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return 0;
				}

				CGExchangeReplyI msg;
				msg.SetObjID(nUserID);
				CNetManager::GetMe()->SendPacket(&msg);
			}
			else
			{					
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Out_Exchange_Range");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过能够交易的范围");

			}//if( pObj != NULL )

		}// if( CDataPool::GetMe()->IsStillAnyAppInList() )

		return 0;
	}

	// 导出自己放入交易的物品个数
	// 自己的（"self"），对方的（"other"）
	INT Exchange::GetItemNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetItemNum param parameter error");
		}

		std::string str	= args[2].GetString();
		if( str == "self" )		//自己
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetItemNum());
			return 1;
		}
		if( str == "other" )	//对方
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetItemNum());
			return 1;
		}
		return 0;
	}

	// 导出Action
	INT Exchange::GetItemAction(LuaPlus::LuaState* state)
	{
		return 1;
	}

	//导出交易的金钱数
	INT Exchange::GetMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetMoney param parameter error");
		}
		INT nMoney;
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		std::string str	= args[2].GetString();
		if( str == "self" )		//自己
			nMoney = CDataPool::GetMe()->MyExBox_GetMoney();
		else if( str == "other" )	//对方
			nMoney = CDataPool::GetMe()->OtExBox_GetMoney();
			

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

	//锁定交易
	INT Exchange::LockExchange(LuaPlus::LuaState* state)
	{
		//发送锁定、取消锁定消息
		CGExchangeSynchLock msg;
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
			//自己处于锁定状态，执行取消锁定的操作
			msg.SetLockMyself(FALSE);
			//更新自己的数据
			CDataPool::GetMe()->MyExBox_SetLock(FALSE);
		}
		else
		{	
			//自己处于尚未锁定状态，执行锁定的操作
			msg.SetLockMyself(TRUE);

			//更新自己的数据
			CDataPool::GetMe()->MyExBox_SetLock(TRUE);

		}

		CNetManager::GetMe()->SendPacket(&msg);

		//更新自己的界面
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}
	
	//导出锁定状态
	INT Exchange::IsLocked(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetMoney param parameter error");
		}

		BOOL bLocked;
		std::string str	= args[2].GetString();
		if( str == "self" )		//自己
			bLocked = CDataPool::GetMe()->MyExBox_GetLock();
		else if( str == "other" )	//对方
			bLocked = CDataPool::GetMe()->OtExBox_GetLock();

		state->PushBoolean(bLocked==TRUE);
		return 1;
	}


	//确定交易
	INT Exchange::AcceptExchange(LuaPlus::LuaState* state)
	{
		//发送交易信息
		CGExchangeOkIII msg;
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}

	// 交易取消
	INT Exchange::ExchangeCancel(LuaPlus::LuaState* state)
	{
		// 发送取消交易消息
		CGExchangeCancel msg;
		CNetManager::GetMe()->SendPacket(&msg);

		// 通知输入金钱对话框关闭
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);

		return 0;
	}

	// 打开宠物界面（暂时用来打开金钱输入框）
	INT Exchange::OpenPetFrame(LuaPlus::LuaState* state)
	{
		// 打开金钱输入框
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"exch");
		return 0;
	}

	// 从输入框获得金钱
	INT Exchange::GetMoneyFromInput(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:Exchange::GetMoney param parameter error");
		}

		UINT nMoney = static_cast<UINT>(args[2].GetInteger());

		//如果此时自己已经处于锁定状态，则这个操作无效
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
				
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_You_have_Locked_Exchange");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你已经锁定了交易");
			return 0;
		}

		//需要先检验是不是有这么多的钱，如果没有，给个最大值
		//同时需要检验不能为负数
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if (nMoney > (UINT)pCharData->Get_Money()) 
		{
				
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_You_Money_Is_Not_Enough");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"身上携带的金钱不足");
			return 0;
		}
		CDataPool::GetMe()->MyExBox_SetMoney(nMoney);
		//发送金钱消息
		CGExchangeSynchMoneyII msg;
		msg.SetMoney(nMoney);
		CNetManager::GetMe()->SendPacket(&msg);


		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}

	//导出交易请求列表，是否为空
	INT Exchange::IsStillAnyAppInList(LuaPlus::LuaState* state)
	{
		state->PushBoolean(CDataPool::GetMe()->IsStillAnyAppInList() == TRUE);
		
		return 1;
	}

	//导出以自己交易的人的名字
	INT Exchange::GetOthersName(LuaPlus::LuaState* state)
	{
		
		INT nObjId = CDataPool::GetMe()->GetExchangObjID();
		CObject_Character* pObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( nObjId ));
		if(pObj)
		{
			state->PushString(pObj->GetCharacterData()->Get_Name());
			return 1;
		}

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Can_Not_Find_Name");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		state->PushString("找不到名字");
		return 1;
	}

	//选择一个宠物列表中的宠物，点击确定后的操作
	INT Exchange::AddPet(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);
		//if (!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA:Exchange:AddPet[2] param parameter error");
		//}
		//INT nPetIndex = args[2].GetInteger();		
		//
		//SDATA_PET* SelectPet = CDataPool::GetMe()->Pet_GetPet(nPetIndex);
		//if( (!SelectPet) || (INVALID_ID == SelectPet->m_nIsPresent) )
		//{
		//	return 0;
		//}

		//// 如果交易界面开着
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
		//{
		//	CGExchangeSynchItemII msg;
		//	msg.SetOpt(EXCHANGE_MSG::OPT_ADDPET);
		//	msg.SetPetGuid(SelectPet->m_GUID);

		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		////如果摆摊界面开着
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
		//{
		//	//查询宠物是不是已经上架
		//	if(CDataPool::GetMe()->MyStallBox_IsPetOnStall(SelectPet->m_GUID))
		//	{
		//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"这个宠物已经上架了");
		//		return 0;
		//	}

		//	//打开上架界面
		//	CDataPool::GetMe()->MyStallBox_SetSelectpetGuid(SelectPet->m_GUID);
		//	CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"st_pet");
		//}
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
		//{
		//	//将宠物放到商店里头去
		//	CGItemSynch msg;
		//	msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
		//	msg.SetFromType(CGItemSynch::POS_PET);
		//	msg.SetToType(CGItemSynch::POS_PLAYERSHOPPET);
		//	msg.SetPetGUID(SelectPet->m_GUID);

		//	CGAutoMovePetFromBagToPlayerShop_t ExtraDataInfo;
		//	ExtraDataInfo.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
		//	ExtraDataInfo.m_nStallIndex		= (BYTE)CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		//	msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
		//	msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);

		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
		//{
		//	CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		//}
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PetSkillStudy"))
		//{
		//	CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		//}

		return 0;
	}
	// 获得宠物的个数
	INT Exchange::GetPetNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::GetPetNum[2] param parameter error");
		}
		std::string str = args[2].GetString();
		if(str == "self")
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetCount());
		}
		else
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetCount());
		}

		return 1;
	}
	//
	INT Exchange::EnumPet(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}

		INT nIndex = args[3].GetInteger();
		std::string str = args[2].GetString();
		if(str == "self")
		{
			SDATA_PET* pPet = CDataPool::GetMe()->MyExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}
		else
		{
			SDATA_PET* pPet = CDataPool::GetMe()->OtExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}*/
		return 1;
	}
	//从交易栏上拿下一个宠物
	INT Exchange::DelSelectPet(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);
		//if (!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA:Exchange::EnumPet[2] param parameter error");
		//}

		//INT nIndex = args[2].GetInteger();

		////发从交易框上拿下宠物的消息
		//CGExchangeSynchItemII msg;
		//msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEPET);
		//msg.SetPetGuid(CDataPool::GetMe()->MyExBox_GetPet(nIndex)->m_GUID);

		//CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//显示宠物详细信息
	INT Exchange::ViewPetDesc(LuaPlus::LuaState* state)//("self",g_nSelectPet);
	{
		/*LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:Exchange::ViewPetDesc[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:Exchange::ViewPetDesc[3] param parameter error");
		}
		
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		SDATA_PET* pPet;
		if(str == "self")
		{
			pPet = CDataPool::GetMe()->MyExBox_GetPet( nIndex );
		}
		else
		{
			pPet = CDataPool::GetMe()->OtExBox_GetPet( nIndex );
		}
		if( (!pPet) || (INVALID_ID == pPet->m_nIsPresent) )
		{
			KLThrow("LUA:Exchange ViewPetDesc parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(pPet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);*/
		return 0;
	}
	
};


