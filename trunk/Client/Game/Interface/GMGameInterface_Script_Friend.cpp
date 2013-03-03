
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "network/netmanager.h"
#include "LuaPlus.h"
#include "Event/GMEventSystem.h"
#include "DataPool/GMDataPool.h"
#include "input/gminputsystem.h"
#include "object/Manager/objectmanager.h"
#include "object/Logic/character/obj_playermyself.h"
#include "datapool/gmdp_characterdata.h"
#include "cgmail.h"
#include "gamestruct.h"
#include "datapool/GMDP_Struct_Mail.h"
#include "cgteamapply.h"
#include "GIDbc_struct.h"
#include "dbc\GMDataBase.h"
#include "Procedure/GameProcedure.h"
#include "DataPool/GMDP_Struct_Relation.h"

#include "CGPlayerCallOfResult.h"
#include "CGSetMoodToHead.h"

#include "CGFinger.h"
#include "CGRelation.h"



namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Friend Friend::s_Friend;
	LuaPlus::LuaObject* Friend::s_pMetaTable = NULL;


	INT	Friend::SetCurrentTeam( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		
		if( !args[ 2 ].IsInteger() )
		{
			KLThrow("LUA: Friend SetCurrentTeam Wrong Param1");
			return 0;
		}

		m_nCurTeam = args[ 2 ].GetInteger();
		
		return 0;
	}
	INT	Friend::GetCurrentTeam( LuaPlus::LuaState* state )
	{
		if( m_nCurTeam == RELATION_GROUP_BLACK )
		{
			for( int i = 0; i < RELATION_GROUP_BLACK; i ++ )
			{
				if( CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP(i) ) > LIST_MEMBER_COUNT )//�õ���ǰ�����������������200�����账��
					continue;

				state->PushInteger( i );
				break;
			}
		}
/*
		else if( m_nCurTeam == RELATION_GROUP_ENEMY )
		{
			for( int i = 0; i < RELATION_GROUP_ENEMY; i ++ )
			{
				if( CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP(i) ) > 20 )
					continue;
				state->PushInteger( i );
				break;
			}
		}
*/
		else
		{
			state->PushInteger( m_nCurTeam );
		}

		return 1;
	}


	//�ر�
	INT Friend::Close(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_FRIEND_INFO, -1 );
		return 0;	
	}

	// �����ѷֵ������
	INT Friend::OpenGrouping( LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		
		//�õ����λ��
		POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
		
		std::vector< STRING > vParam;

		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
		
		vParam.push_back("groupingmenu");
		vParam.push_back("-1");
		vParam.push_back( args[ 2 ].GetString() );
		vParam.push_back( args[ 3 ].GetString() );
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		
		return 0;
	}


	INT Friend::OpenMenu( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		
		// �õ����λ��
		POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
		
		std::vector< STRING > vParam;
		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
		
		vParam.push_back("friendmenu");
		vParam.push_back("-1");
		vParam.push_back( args[ 2 ].GetString() );
		vParam.push_back( args[ 3 ].GetString() );
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);

		CEventSystem::GetMe()->PushEvent( GE_SHOW_CONTEXMENU, vParam );

		return 0;
	}
	
	INT Friend::ShowFriendInputDlg( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		CEventSystem::GetMe()->PushEvent( GE_FRIEND_CREATE_GROUP );

		return 0;
	}

	INT Friend::ShowAddNewFriendDlg( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		CEventSystem::GetMe()->PushEvent( GE_OPEN_ADD_NEW_FRIEND );

		return 0;
	}

	INT Friend::CreateGrouping( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if (!(args[2].IsString()))
		{
			KLThrow("LUA: Friend::CreateGrouping Wrong Param1");
		}

		STRING str = args[2].GetString();

		if( str == "" )
		{
			// ֪ͨ������ʾϵͳ��ʾ��Ϣ, ��������
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�������Ϸ���");
			
			return 0;
		}


		// д�����ݳ���

		// �������������б���
		CGRelation Msg;

		Msg.GetRelation()->CleanUp();
		Msg.GetRelation()->m_Type = REQ_ADD_NEW_GROUP;
		
		REQUEST_ADD_NEW_GROUP* pFriend = &(Msg.GetRelation()->m_AddNewGroup);
		
		pFriend->CleanUp();
		pFriend->SetGroupName( str.c_str() );
		
		CNetManager::GetMe()->SendPacket( &Msg );

		// ֪ͨUI
//		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);

		return 0;
	}

	INT Friend::GetNewGroupInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		//������
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: Friend::GetNewGroupInfo Wrong Param1");
		}

		INT nGroupIndex = args[2].GetInteger();

		STRING strGroupName = CDataPool::GetMe()->Relation_GetNewGroup( nGroupIndex );
		state->PushString( strGroupName.c_str() );

		return 1;
	}


	// ��ѯ����
	INT Friend::EnumName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		//������
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: Friend::EnumFriend[2] Wrong Param1");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("LUA: Friend::EnumFriend[3] Wrong Param1");
		}

		INT nIndex = args[2].GetInteger();
		std::string szType = args[3].GetString();

		_OWN_RELATION Relation;

		if( szType == "friend" )
		{
			CDataPool::GetMe()->Relation_GetFriend( nIndex, &Relation );
			state->PushString(Relation.m_Member.m_szMemberName);
		}
		else if( szType == "black" )
		{
			CDataPool::GetMe()->Relation_GetBlack(nIndex,&Relation);
			state->PushString(Relation.m_Member.m_szMemberName);
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}

	INT Friend::AskTeam(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		
		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Friend:AskTeam Wrong Param1");
			return 0;
		}

		STRING str = args[ 2 ].GetString();

		CGTeamApply msg;

		msg.SetSourGUID( CObjectManager::GetMe()->GetMySelf()->GetServerGUID() );
		msg.SetDestName( str.c_str() );

		CNetManager::GetMe()->SendPacket(&msg);
		
		return 0;
	}

	// �õ��ж�������Ϣ
	INT Friend::GetHistroyNumber( LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		if( !args[3].IsInteger() ) return 0; // �ڼ���ĵڼ�����
		int nNumber = CDataPool::GetMe()->GetRelation()->GetMailCount( ( RELATION_GROUP )args[2].GetInteger(), args[3].GetInteger() );
		state->PushInteger( nNumber );
		return 1; 
	}
	// �õ���ʷ����
	INT Friend::GetHistroyData(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		if( !args[3].IsInteger() ) return 0;	// �ڼ���ĵڼ�����
		if( !args[4].IsInteger() ) return 0;	// �ڼ���ĵڼ�����
		if( !args[5].IsString() ) return 0;		// ��ʲô����
		
		int nIndex = CDataPool::GetMe()->GetRelation()->GetMailIndex(( RELATION_GROUP )args[2].GetInteger(), args[3].GetInteger(), args[ 4 ].GetInteger() );
		const SMail* pMail = CDataPool::GetMe()->GetMailPool()->GetMail( nIndex );
		if( pMail )
		{
			std::string str = args[5].GetString();
			if( str == "CONTEX" )// GUID
			{
				state->PushString( pMail->m_szContex );
			}
			else if( str == "TIME" )
			{
				state->PushString( pMail->m_szCreateTime );
			}
			else if( str == "SENDER" )
			{
				state->PushString( pMail->m_szSourName );
			}
			else if( str == "RECIVER" )
			{
				state->PushString( pMail->m_szDestName );
			}
			else
				state->PushString( "-1" );
		}
		return 1;
	}
	//���˽����һ����ͬ�⡱�͡���ͬ�⡱����
	INT Friend::CallOf(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Friend:CallOf Wrong Param1");
			return 0;
		}

		std::string str = args[2].GetString();

		CGPlayerCallOfResult msg;

		if(str == "ok")
		{
			msg.SetResultCode(CALLOF_RESULT_CODE_ACCEPT);
		}
		else
		{
			msg.SetResultCode(CALLOF_RESULT_CODE_REFUSE);
		}
		
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	// �����ʾ����Ĳ���
	INT Friend::ViewFeel(LuaPlus::LuaState* state)
	{
		INT nView = 1;
		if( _TITLE::MOOD_TITLE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() )
		{
			nView = 0;
		}

		//֪ͨ������
		CGSetMoodToHead msg;
		msg.SetView(nView);

		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	//���������ǲ������Լ���ͷ��
	INT Friend::IsMoodInHead(LuaPlus::LuaState* state)
	{
		if( _TITLE::MOOD_TITLE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}
		return 1;
	}

	// ���������ж��Ƿ��Ǻ���
	INT Friend::IsPlayerIsFriend(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if( !args[2].IsString() ) 
		{
			KLThrow("LUA: Friend::EnumFriend[2] Wrong Param1");
			return 0;
		}

		std::string str = args[2].GetString();

		_OWN_RELATION* pRelation = NULL;
		CDataPool::GetMe()->Relation_GetFriendByName( str.c_str(), pRelation );
		
		if( pRelation )
		{
			state->PushInteger( 1 );
		}
		else
		{
			state->PushInteger( 0 );
		}

		return 1;
	}

	// ���õ�ǰѡ��
	INT Friend::SetCurrentSelect(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		//������
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: Friend::EnumFriend[2] Wrong Param1");
			return 0;
		}

		INT nIndex = args[2].GetInteger();

		return 0;
	}


	// ����������
	INT Friend::OpenSearcher(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

//		args->PushInteger( 0 );
		CEventSystem::GetMe()->PushEvent( GE_OPEN_FRIEND_SEARCH );

		return 0;
	}


//==============================================================================================
// ���Ѳ������

	FriendSearcher FriendSearcher::s_FriendSearcher;
	LuaPlus::LuaObject* FriendSearcher::s_pMetaTable = NULL;

	// ��պ����б�
	INT FriendSearcher::ClearFingerList( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		return 0;
	}
	// ��ȡҳ��
	INT FriendSearcher::GetFriendPageNumber( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		state->PushInteger( 1 );

		return 1;
	}

	// ��ȡһҳ�еĺ�������
	INT FriendSearcher::GetFriendNumberInPage( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::GetFriendNumberInPage Wrong Param1");
			return 0;
		}

		INT nIndex = args[2].GetInteger();

		INT nCount = CDataPool::GetMe()->Finger_GetPlayerInfoNum();

		state->PushInteger( nCount );

		return 1;
	}
	// ��ȡһҳ��ѡ��ĺ��Ѳ���
	INT FriendSearcher::GetFriendFromPage( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::GetFriendFromPage Wrong Param1");
			return 0;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::GetFriendFromPage Wrong Param2");
			return 0;
		}
		if( !args[4].IsString() ) 
		{
			KLThrow("LUA: FriendSearcher::GetFriendFromPage Wrong Param3");
			return 0;
		}

		INT curPos = args[2].GetInteger();
		INT index  = args[3].GetInteger();
		STRING str = args[4].GetString();

		INT nCount = CDataPool::GetMe()->Finger_GetPlayerInfoNum();

		if( index < 0 || index >= nCount )
			return 0;

		FingerPlayerInfo_t* playerInfo = CDataPool::GetMe()->Finger_GetPlayerInfoByIndex( index );

		// �ж� ��Ч��
		if( playerInfo )
		{
			if( "NAME" == str )
			{
				state->PushString( playerInfo->m_sName.c_str() );
			}
			else if("ID" == str)
			{
				state->PushInteger( playerInfo->m_nGUID );
			}
			else if( "ID_TEXT" == str )
			{
				char szInfo[ 16 ];
				sprintf( szInfo,"%08X", DWORD( playerInfo->m_nGUID ) );
				state->PushString( szInfo );

//				state->PushInteger( playerInfo->m_nGUID );
			}
			else if( "ONLINE" == str )
			{
				state->PushInteger( playerInfo->m_bOnlineFlag );
			}
			else if( "LEVEL" == str )
			{
				state->PushInteger( playerInfo->m_nLevel );
			}
			else if( "SEX" == str )
			{
				state->PushInteger( playerInfo->m_nSex );
			}
			else if( "MENPAI_TEXT" == str )
			{
				state->PushInteger( playerInfo->m_nMenPai );
			}
			else if( "GUID_NAME" == str )
			{
				playerInfo->m_nGuildID;

				state->PushString( "���" );
			}
		}

		return 1;
	}

	// ͨ��id����
	INT FriendSearcher::FriendSearchByID( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"��������ȷ�Ĳ�ѯ����");
			//KLThrow("LUA: FriendSearcher::FriendSearchByID Wrong Param1");
			return 0;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByID Wrong Param2");
			return 0;
		}

		INT temp_id			= args[2].GetInteger();
		BOOL temp_bOnline	= args[3].GetInteger();


		// ���Ͳ�����Ϣ
		CGFinger msg;
		CG_FINGER* pFinger = msg.GetFinger();

		pFinger->CleanUp();
		pFinger->m_Type = FREQ_GUID;
		pFinger->m_FingerByGUID.CleanUp();	// FINGER_BY_GUID
		pFinger->m_FingerByGUID.SetTargetGUID( temp_id );
		pFinger->m_FingerByGUID.SetOnlineFlag( temp_bOnline );

		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	// ͨ����������
	INT FriendSearcher::FriendSearchByName( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsString() ) 
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param1");
			return 0;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param2");
			return 0;
		}
		if( !(args[4].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param3");
			return 0;
		}
		if( !(args[5].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param4");
			return 0;
		}

		STRING temp_name	= args[2].GetString();
		INT temp_bPrecise	= args[3].GetInteger();
		INT temp_bOnline	= args[4].GetInteger();
		INT	temp_nPosition	= args[5].GetInteger();


		// ���ڴ��ڲ��Խ׶Σ���ʱ���������ֵĳ���
		/*if ( (temp_name.size() < 4) || (temp_name.size() > 16) )
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"��������Ƴ��ȴ���");
			return 0;
		}*/

		// ���Ͳ�����Ϣ
		CGFinger msg;
		CG_FINGER* pFinger = msg.GetFinger();

		pFinger->CleanUp();
		pFinger->m_Type = FREQ_NAME;
		pFinger->m_FingerByName.CleanUp();	// FINGER_BY_NAME
		pFinger->m_FingerByName.SetTargetName( temp_name.c_str() );
		pFinger->m_FingerByName.SetOnlineFlag( temp_bOnline );		// �Ƿ�����
		pFinger->m_FingerByName.SetPreciseFlag( temp_bPrecise );	// �Ƿ�ȷ����
		pFinger->m_FingerByName.SetPosition( temp_nPosition );		// ��ǰ����λ��

		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
/*
	INT FriendSearcher::FriendSearchByNameAndId( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsString() ) 
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByNameAndId Wrong Param1");
			return 0;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByNameAndId Wrong Param2");
			return 0;
		}

		STRING temp_name= args[2].GetString();
		INT temp_id		= args[3].GetInteger();

		// ���Ͳ�����Ϣ
		CGFinger msg;
		CG_FINGER* pFinger = msg.GetFinger();

		pFinger->CleanUp();
		pFinger->m_Type = FREQ_NAME_GUID;
		pFinger->m_FingerByName.CleanUp();	// FINGER_BY_NAME
		pFinger->m_FingerByName.SetTargetName( temp_name.c_str() );
		pFinger->m_FingerByName.SetOnlineFlag( 1 );		// �Ƿ�����

		CNetManager::GetMe()->SendPacket(&msg);

		return 0;

	}
*/

	// �߼�����
	// nPosition�� ְҵ�� ��ᣬ �Ա� �ȼ����ޣ� �ȼ�����
	INT FriendSearcher::FriendSearchAdvance( LuaPlus::LuaState* state )
	{
		return 0;


		LuaStack args(state);
		
		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param1");
			return 0;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param2");
			return 0;
		}
		if( !(args[4].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param3");
			return 0;
		}
		if( !(args[5].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param4");
			return 0;
		}
		if( !(args[6].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param5");
			return 0;
		}
		if( !(args[7].IsInteger()) )
		{
			KLThrow("LUA: FriendSearcher::FriendSearchByName Wrong Param6");
			return 0;
		}

		INT nPosition		= args[2].GetInteger();
		INT nMenpai			= args[3].GetInteger();
		INT temp_banghui	= args[4].GetInteger();
		INT temp_nSex		= args[5].GetInteger();
		INT temp_LevelDown	= args[6].GetInteger();
		INT temp_LevelUp	= args[7].GetInteger();


		// ���Ͳ�����Ϣ
		CGFinger msg;
		CG_FINGER* pFinger = msg.GetFinger();

		pFinger->CleanUp();
		pFinger->m_Type = FREQ_ADVANCED;
		pFinger->m_AdvancedFinger.CleanUp();	// FINGER_BY_GUID

		pFinger->m_AdvancedFinger.SetPosition( nPosition );			// ��ǰ����λ��
		pFinger->m_AdvancedFinger.SetMenPai( nMenpai );				// ְҵ
		pFinger->m_AdvancedFinger.SetGuildID( temp_banghui );		// ���ID
		pFinger->m_AdvancedFinger.SetSex( temp_nSex );				// �Ա�
		pFinger->m_AdvancedFinger.SetBottomLevel( temp_LevelDown );	// �ȼ���Χ
		pFinger->m_AdvancedFinger.SetTopLevel( temp_LevelUp );		// �ȼ���Χ

		CNetManager::GetMe()->SendPacket(&msg);
	
		return 0;
	}

};

