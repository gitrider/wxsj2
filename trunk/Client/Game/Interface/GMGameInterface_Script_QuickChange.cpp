
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Event/GMEventSystem.h"

#include "NetWork/NetManager.h"
#include "CGEquipSuitExchange.h"

#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool/GMUIDataPool.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	QuickChange QuickChange::s_QuickChange;
	LuaPlus::LuaObject* QuickChange::s_pMetaTable = NULL;

	//一键换装：得到装备套装的名字
	INT QuickChange::Lua_GetSuitName( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !(args[2].IsInteger()) )return 0;

		INT suitId = args[2].GetInteger();

		// 读取装备guid
		CCharacterData* pData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();
		_SUIT_SETTING aSuit = pData->Get_EquipSuit(suitId);

		state->PushString( aSuit.m_SuitName );

		return 1;


	}

	// 一键换装：换装
	INT QuickChange::Lua_ChangeSuit( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;

		int idx = args[ 2 ].GetInteger();

		CGEquipSuitExchange msg;
		msg.setEquipSuitNum(idx);
		CNetManager::GetMe()->SendPacket( &msg );

		return 0;
	}

	// 一键换装：设置当前套装
	INT QuickChange::Lua_SetCurrentSuit( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;

		int idx = args[ 2 ].GetInteger();
		
		((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData()->Set_EquipSuitId(idx);

		return 0;
	}

	// 一键换装：保存命名
	INT QuickChange::Lua_SaveSuitName( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;

		int suitId = args[ 2 ].GetInteger();		// 套装id
		char* name = (char*)args[ 3 ].GetString();	// 套装名字

		CEventSystem::GetMe()->PushEvent( GE_UPDATE_QUICK_CHANGE, name, "name", suitId );

		return 0;
	}

	// 一键换装：保存装备
	INT QuickChange::Lua_SaveSuit( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;
		if( !args[ 3 ].IsInteger() ) return 0;

		int isSend = args[ 2 ].GetInteger(); // 是否发送
		int suitId = args[ 3 ].GetInteger(); // 套装id

		// 填充需要保存的装备guid
		if( isSend == 0 )
		{
			int idx = args[ 4 ].GetInteger();			// 一套中的装备点

			// server guid
			int world = args[ 5 ].GetInteger();			
			int server = args[ 6 ].GetInteger();		
			int serial = args[ 7 ].GetInteger();	

			int index = args[ 8 ].GetInteger();		// 一套中的套装索引

			// 读取装备guid
			CCharacterData* pData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();
			if( index == 0 )
			{
				_SUIT_SETTING aSuit;
				pData->Set_EquipSuit(suitId, aSuit);
			}

			_SUIT_SETTING aSuit = pData->Get_EquipSuit(suitId);
			if( world == -1 )
			{
				return 0;
			}

			// 设置装备guid
			_ITEM_GUID item_guid;
			item_guid.m_World = (BYTE)world;
			item_guid.m_Server = (BYTE)server;
			item_guid.m_Serial = (INT)serial;
			aSuit.m_EquipData[idx] = item_guid;
			pData->Set_EquipSuit(suitId, aSuit);
		}
		// 发送保存装备消息
		else if( isSend == 1 )
		{
			char* name = (char*)args[ 4 ].GetString();	// 套装名字
			CCharacterData* pData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();
			_SUIT_SETTING aSuit = pData->Get_EquipSuit(suitId);
			sprintf( aSuit.m_SuitName, name, sizeof(CHAR)*MAX_SUIT_NAME_SIZE );
			pData->Set_EquipSuit(suitId, aSuit);

			CGameInterface::GetMe()->PacketItem_SaveSuit(suitId);
		}

		return 0;
	}

	// 一键换装：读取装备
	INT QuickChange::Lua_ReadSuit( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;

		int from = args[ 2 ].GetInteger();
		int idx = args[ 3 ].GetInteger();

		// 从10套套装中读取装备
		if( from == 1 )
		{
			int suitid = args[ 4 ].GetInteger();
			_SUIT_SETTING aSuit = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_EquipSuit( suitid );
			_ITEM_GUID itemguid = aSuit.m_EquipData[idx];

			// 该装备点没有装备
			if( itemguid.m_World == 0 && itemguid.m_Server == 0 && itemguid.m_Serial == 0 )
			{
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				return 4;
			}

			// 从背包找
			for ( INT i =0 ;i<  MAX_BAG_SIZE ;++i)
			{
				CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem( i );
				if ( pItem )
				{
					USHORT uWorld, uServer; UINT uSerial;
					pItem->GetGUID(uWorld, uServer, uSerial);
					if (  itemguid.m_World == uWorld && 
						itemguid.m_Server == uServer &&
						itemguid.m_Serial == uSerial )
					{
						USHORT uWorld, uServer; UINT uSerial;
						pItem->GetGUID(uWorld, uServer, uSerial);
						state->PushInteger( pItem->GetID() );
						state->PushInteger( uWorld );
						state->PushInteger( uServer );
						state->PushInteger( uSerial );
						return 4;
					}
				}
			}

			// 从装备找
			tObject_Item* pItemAtUser= CDataPool::GetMe()->UserEquip_GetItem( (HUMAN_EQUIP)idx );
			if( pItemAtUser )
			{
				USHORT uWorld, uServer; UINT uSerial;
				pItemAtUser->GetGUID(uWorld, uServer, uSerial);
				state->PushInteger( pItemAtUser->GetID() );
				state->PushInteger( uWorld );
				state->PushInteger( uServer );
				state->PushInteger( uSerial );
				return 4;
			}
			else
			{
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				return 4;
			}
		}
		// 从装备栏里读取装备
		else
		{
			tObject_Item* pItem= CDataPool::GetMe()->UserEquip_GetItem( (HUMAN_EQUIP)idx );
			if( pItem )
			{
				USHORT uWorld, uServer; UINT uSerial;
				pItem->GetGUID(uWorld, uServer, uSerial);
				state->PushInteger( pItem->GetID() );
				state->PushInteger( uWorld );
				state->PushInteger( uServer );
				state->PushInteger( uSerial );
				return 4;
			}
			else
			{
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				return 4;
			}
		}

		return 0;
	}
};