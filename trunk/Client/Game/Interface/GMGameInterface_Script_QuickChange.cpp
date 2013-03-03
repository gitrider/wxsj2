
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

	//һ����װ���õ�װ����װ������
	INT QuickChange::Lua_GetSuitName( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !(args[2].IsInteger()) )return 0;

		INT suitId = args[2].GetInteger();

		// ��ȡװ��guid
		CCharacterData* pData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();
		_SUIT_SETTING aSuit = pData->Get_EquipSuit(suitId);

		state->PushString( aSuit.m_SuitName );

		return 1;


	}

	// һ����װ����װ
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

	// һ����װ�����õ�ǰ��װ
	INT QuickChange::Lua_SetCurrentSuit( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;

		int idx = args[ 2 ].GetInteger();
		
		((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData()->Set_EquipSuitId(idx);

		return 0;
	}

	// һ����װ����������
	INT QuickChange::Lua_SaveSuitName( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;

		int suitId = args[ 2 ].GetInteger();		// ��װid
		char* name = (char*)args[ 3 ].GetString();	// ��װ����

		CEventSystem::GetMe()->PushEvent( GE_UPDATE_QUICK_CHANGE, name, "name", suitId );

		return 0;
	}

	// һ����װ������װ��
	INT QuickChange::Lua_SaveSuit( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;
		if( !args[ 3 ].IsInteger() ) return 0;

		int isSend = args[ 2 ].GetInteger(); // �Ƿ���
		int suitId = args[ 3 ].GetInteger(); // ��װid

		// �����Ҫ�����װ��guid
		if( isSend == 0 )
		{
			int idx = args[ 4 ].GetInteger();			// һ���е�װ����

			// server guid
			int world = args[ 5 ].GetInteger();			
			int server = args[ 6 ].GetInteger();		
			int serial = args[ 7 ].GetInteger();	

			int index = args[ 8 ].GetInteger();		// һ���е���װ����

			// ��ȡװ��guid
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

			// ����װ��guid
			_ITEM_GUID item_guid;
			item_guid.m_World = (BYTE)world;
			item_guid.m_Server = (BYTE)server;
			item_guid.m_Serial = (INT)serial;
			aSuit.m_EquipData[idx] = item_guid;
			pData->Set_EquipSuit(suitId, aSuit);
		}
		// ���ͱ���װ����Ϣ
		else if( isSend == 1 )
		{
			char* name = (char*)args[ 4 ].GetString();	// ��װ����
			CCharacterData* pData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();
			_SUIT_SETTING aSuit = pData->Get_EquipSuit(suitId);
			sprintf( aSuit.m_SuitName, name, sizeof(CHAR)*MAX_SUIT_NAME_SIZE );
			pData->Set_EquipSuit(suitId, aSuit);

			CGameInterface::GetMe()->PacketItem_SaveSuit(suitId);
		}

		return 0;
	}

	// һ����װ����ȡװ��
	INT QuickChange::Lua_ReadSuit( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsInteger() ) return 0;

		int from = args[ 2 ].GetInteger();
		int idx = args[ 3 ].GetInteger();

		// ��10����װ�ж�ȡװ��
		if( from == 1 )
		{
			int suitid = args[ 4 ].GetInteger();
			_SUIT_SETTING aSuit = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_EquipSuit( suitid );
			_ITEM_GUID itemguid = aSuit.m_EquipData[idx];

			// ��װ����û��װ��
			if( itemguid.m_World == 0 && itemguid.m_Server == 0 && itemguid.m_Serial == 0 )
			{
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				state->PushInteger(-1);
				return 4;
			}

			// �ӱ�����
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

			// ��װ����
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
		// ��װ�������ȡװ��
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