
/**	GMInterface_Script_Country.cpp
 *
 *	功能：	
 *
 *	修改记录：	
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


	// 产生国王
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

		STRING szCountryName = args[2].GetString();		// 国家名称
		STRING szCountryDesc = args[3].GetString();		// 国家描述


		//================
		// 产生条件

		// 玩家必须在整个世界中排名前500名
		{
			// ( "你的等级排名未进入世界前500名，无法申请成为国王！" )
		}

		// 玩家必然是某个帮会的帮主
		if( INVALID_ID == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// 是否是帮主
//			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
			{
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "你不是帮主，无法申请成为国王！" );
			}
		}

		// 有一定数目的游戏币
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() < 1000000 )
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "你所携带的金币数量不足100金，无法申请成为国王！" );

			state->PushInteger(-1);
			return 1;
		}


		// 检测通过
		state->PushInteger( 1 );

		// 显示确认框
		CEventSystem::GetMe()->PushEvent( GE_COUNTRY_KING_CREATE_CONFIRM );	
		
		// 确认框显示内容
		{
			// messagebox_self.lua
			// 扣除金币100金

			// 发送滚动公告“AAA玩家已成为BBB国家的国王，希望他可以成为万世明君，带领BBB国民走向强大”
		}

		return 1;
	}

	// 确认产生国王
	INT Country::CreateCountryKingConfirm( LuaPlus::LuaState* state )
	{

		// 发送创建国王信息包

		CGCountryRequestKindom msg;

		COUNTRY_CGW_KINDOM_REQUEST data;
		data.m_Guid = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();	// GetCharacterData()->Get_Guild();
		msg.SetGetKindomRequestData( &data );

		CNetManager::GetMe()->SendPacket( &msg );

		// ( "已经有玩家成为国王，你已无法申请！可以等待**时报名挑战国王！" )

		return 0;

	}

	// 请求国家官员信息
	INT Country::RequestCountryFunctionary( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Country RequestCountryFunctionary Wrong Param1");
			return -1;
		}

		INT nCountryId = args[2].GetInteger();


		// 发送请求消息 
		CGCountryInfoRequest msg;
		msg.SetCountryID( nCountryId );
		CNetManager::GetMe()->SendPacket( &msg );

		return 0;
	}

	// 获取国家官员信息
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
		// 元帅
		case 0:
			{
				strTmp = "元帅";
				state->PushString( countryFunction->m_sFunctionaryName.c_str() );
				break;
			}
		// 宰相
		case 1:
			{
				strTmp = "宰相";
				state->PushString( strTmp.c_str() );
				break;
			}
		// 捕头
		case 2:
			{
				state->PushString( strTmp.c_str() );
				break;
			}
		// 御使大夫
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

	// 修改现有国家官员职位
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

		INT nIndex = args[2].GetInteger();	// 官职索引
		INT nType  = args[3].GetInteger();	// 任免类型
		
		// 官职
		switch( nIndex )
		{
		// 元帅
		case 0:
			{
				// 罢免
				if( nType == 0 )
				{
				}
				// 任命
				else if( nType == 1 )
				{
				}

				break;
			};
		// 宰相
		// 捕头
		// 御使大夫

		};

		return 0;
	}

	// 任命官员
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

		INT nCountryType = args[2].GetInteger();	// 国家官职类型
		STRING szName	 = args[3].GetString();		// 要任命的玩家名字


/*
		// 判断玩家是否有效（此方法只适用于在管理器中已存在的对象）

		CObject* obj = CObjectManager::GetMe()->FindCharacterByName( szName );
//		(CObject*)CObjectManager::GetMe()->GetMainTarget();

		if( !obj )
		{
			// ( "你所输入的玩家不存在！" )
			return 0;
		}
*/

		// 发送消息

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

	// 罢免
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

		INT nCountryType = args[2].GetInteger();	// 国家官职类型
		STRING szName	 = args[3].GetString();		// 要罢免的玩家名字


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


	// 产生皇帝
	INT Country::CreateMonarch( LuaPlus::LuaState* state )
	{
		// 判断是否国王
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData() )
		{
			// ( "只有国王才能成为皇帝。" );
			return 0;
		}

		// 判断金钱
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() < 1000000 )
		{
			// ( "你没有足够的资金争当皇帝。" );
			return 0;
		}


		// 成功后的提示
		// ( "AAA在众目睽睽之下成为皇帝！" );

		return 0;
	}


};	// namespace