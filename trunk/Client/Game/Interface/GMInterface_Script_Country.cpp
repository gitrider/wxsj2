
/**	GMInterface_Script_Country.cpp
 *
 *	���ܣ�	
 *
 *	�޸ļ�¼��	
 *			
 *			
 */

#include "StdAfx.h"
#include "LuaPlus.h"

#include "GIEventDefine.h"
#include "GIUISystem.h"
#include "GIException.h"
#include "GIUtil.h"

#include "Network\NetManager.h"

#include "DataPool/GMDataPool.h"
#include "DataPool/GMDP_CharacterData.h"
#include "DataPool/GMDP_Struct_Country.h"

#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Character/Obj_PlayerNPC.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "Event/GMEventSystem.h"

#include "GMInterface_Script_Country.h"

#include "CGCountryRequestKindom.h"
#include "CGCountryAppoint.h"
#include "CGCountryRemove.h"
#include "CGCountryInfoRequest.h"




namespace SCRIPT_SANDBOX
{
	Country Country::s_Country;
	LuaPlus::LuaObject* Country::s_pMetaTable = NULL;


	Country::Country()
	{
	}

	Country::~Country()
	{
	}


	// ��������
	INT Country::CreateCountryKing( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Country GeneraCountryKing Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Country GeneraCountryKing Wrong Param2");
			return -1;
		}

		STRING szCountryName = args[2].GetString();		// ��������
		STRING szCountryDesc = args[3].GetString();		// ��������


		//================
		// ��������

		// ��ұ�������������������ǰ500��
		{
			// ( "��ĵȼ�����δ��������ǰ500�����޷������Ϊ������" )
		}

		// ��ұ�Ȼ��ĳ�����İ���
		if( INVALID_ID == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// �Ƿ��ǰ���
//			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
			{
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "�㲻�ǰ������޷������Ϊ������" );
			}
		}

		// ��һ����Ŀ����Ϸ��
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() < 1000000 )
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "����Я���Ľ����������100���޷������Ϊ������" );

			state->PushInteger(-1);
			return 1;
		}


		// ���ͨ��
		state->PushInteger( 1 );

		// ��ʾȷ�Ͽ�
		CEventSystem::GetMe()->PushEvent( GE_COUNTRY_KING_CREATE_CONFIRM );	
		
		// ȷ�Ͽ���ʾ����
		{
			// messagebox_self.lua
			// �۳����100��

			// ���͹������桰AAA����ѳ�ΪBBB���ҵĹ�����ϣ�������Գ�Ϊ��������������BBB��������ǿ��
		}

		return 1;
	}

	// ȷ�ϲ�������
	INT Country::CreateCountryKingConfirm( LuaPlus::LuaState* state )
	{

		// ���ʹ���������Ϣ��

		CGCountryRequestKindom msg;

		COUNTRY_CGW_KINDOM_REQUEST data;
		data.m_Guid = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();	// GetCharacterData()->Get_Guild();
		msg.SetGetKindomRequestData( &data );

		CNetManager::GetMe()->SendPacket( &msg );

		// ( "�Ѿ�����ҳ�Ϊ�����������޷����룡���Եȴ�**ʱ������ս������" )

		return 0;

	}

	// ������ҹ�Ա��Ϣ
	INT Country::RequestCountryFunctionary( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Country RequestCountryFunctionary Wrong Param1");
			return -1;
		}

		INT nCountryId = args[2].GetInteger();


		// ����������Ϣ 
		CGCountryInfoRequest msg;
		msg.SetCountryID( nCountryId );
		CNetManager::GetMe()->SendPacket( &msg );

		return 0;
	}

	// ��ȡ���ҹ�Ա��Ϣ
	INT Country::GetCountryFunctionary( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Country GetCountryFunctionary Wrong Param1");
			return -1;
		}

		INT nType = args[2].GetInteger();

		CountryFunctionaryInfo* countryFunction = CDataPool::GetMe()->Country_Get( nType );
		if( !countryFunction )
			return -1;

		STRING strTmp = countryFunction->m_sFunctionaryName;

		state->PushString( strTmp.c_str() );

		return 1;

		switch( nType )
		{
		// Ԫ˧
		case 0:
			{
				strTmp = "Ԫ˧";
				state->PushString( countryFunction->m_sFunctionaryName.c_str() );
				break;
			}
		// ����
		case 1:
			{
				strTmp = "����";
				state->PushString( strTmp.c_str() );
				break;
			}
		// ��ͷ
		case 2:
			{
				state->PushString( strTmp.c_str() );
				break;
			}
		// ��ʹ���
		case 3:
			{
				state->PushString( strTmp.c_str() );
				break;
			}

		default:
			state->PushString( "" );
		}

		return 1;
	}

	// �޸����й��ҹ�Աְλ
	INT	Country::AdjustFunctionaryAuth( LuaPlus::LuaState* state )
	{
		LuaStack args(state);



		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Country GetCountryFunctionary Wrong Param1");
			return -1;
		}
		if(!(args[3].IsInteger()))
		{
			KLThrow("LUA: Country GetCountryFunctionary Wrong Param1");
			return -1;
		}

		INT nIndex = args[2].GetInteger();	// ��ְ����
		INT nType  = args[3].GetInteger();	// ��������
		
		// ��ְ
		switch( nIndex )
		{
		// Ԫ˧
		case 0:
			{
				// ����
				if( nType == 0 )
				{
				}
				// ����
				else if( nType == 1 )
				{
				}

				break;
			};
		// ����
		// ��ͷ
		// ��ʹ���

		};

		return 0;
	}

	// ������Ա
	INT Country::AppointFunctionary( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Country GetCountryFunctionary Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Country AdjustFunctionaryAuth Wrong Param1");
			return -1;
		}

		INT nCountryType = args[2].GetInteger();	// ���ҹ�ְ����
		STRING szName	 = args[3].GetString();		// Ҫ�������������


/*
		// �ж�����Ƿ���Ч���˷���ֻ�������ڹ��������Ѵ��ڵĶ���

		CObject* obj = CObjectManager::GetMe()->FindCharacterByName( szName );
//		(CObject*)CObjectManager::GetMe()->GetMainTarget();

		if( !obj )
		{
			// ( "�����������Ҳ����ڣ�" )
			return 0;
		}
*/

		// ������Ϣ

		CGCountryAppoint msg;

		COUNTRY_CGW_COUNTRY_APPOINT data;
		data.m_KindomGuid	= CObjectManager::GetMe()->GetMySelf()->GetServerGUID();		// GUID_t
		data.m_cNameSize	= (CHAR)strlen( szName.c_str() );
		strncpy( data.m_szTargetName, szName.c_str(), MAX_CHARACTER_NAME * sizeof(CHAR) );
		data.m_szTargetName[MAX_CHARACTER_NAME] = 0 ;
		data.m_nOfficialPosition = nCountryType;
		msg.SetCountryAppointData( &data );

		CNetManager::GetMe()->SendPacket( &msg );

		return 0;
	}

	// ����
	INT Country::DismissFunctionary( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Country GetCountryFunctionary Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Country AdjustFunctionaryAuth Wrong Param1");
			return -1;
		}

		INT nCountryType = args[2].GetInteger();	// ���ҹ�ְ����
		STRING szName	 = args[3].GetString();		// Ҫ������������


		CGCountryRemove msg;

		COUNTRY_CGW_COUNTRY_REMOVE data;
		data.m_KindomGuid	= CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
		data.m_cNameSize	= (CHAR)strlen( szName.c_str() );;
		strncpy( data.m_szTargetName, szName.c_str(), MAX_CHARACTER_NAME * sizeof(CHAR) );
		data.m_szTargetName[MAX_CHARACTER_NAME] = 0 ;
		//data.m_nOfficialPosition = nCountryType;
		msg.SetCountryRemoveData( &data );

		CNetManager::GetMe()->SendPacket( &msg );

		return 0;
	}


	// �����ʵ�
	INT Country::CreateMonarch( LuaPlus::LuaState* state )
	{
		// �ж��Ƿ����
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData() )
		{
			// ( "ֻ�й������ܳ�Ϊ�ʵۡ�" );
			return 0;
		}

		// �жϽ�Ǯ
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() < 1000000 )
		{
			// ( "��û���㹻���ʽ������ʵۡ�" );
			return 0;
		}


		// �ɹ������ʾ
		// ( "AAA����Ŀ��֮�³�Ϊ�ʵۣ�" );

		return 0;
	}


};	// namespace