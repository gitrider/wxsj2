

#include "StdAfx.h"
#include "GCRelation.h"

#include "DB_Struct.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"

#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_Struct_Relation.h"

//#include "Object\Manager\ObjectManager.h"
//#include "Object\Logic\Character\Obj_PlayerMySelf.h"
//#include "..\..\DataPool\GMDP_CharacterData.h"


const char* pRelationString[] = {
	"",
	"GCRelationHandler_Info_Target_Not_Exist",				// RET_ERR_TARGETNOTEXIST
	"GCRelationHandler_Info_Group_Is_Full",					// RET_ERR_GROUPISFULL
	"GCRelationHandler_Info_Is_Firend",						// RET_ERR_ISFRIEND
	"GCRelationHandler_Info_Is_BlackName",					// RET_ERR_ISBLACKNAME
	"GCRelationHandler_Info_Can_Not_Transition",			// RET_ERR_CANNOTTRANSITION
	"GCRelationHandler_Info_Is_Not_Firend",					// RET_ERR_ISNOTFRIEND
	"GCRelationHandler_Info_Is_Not_In_BlackList",			// RET_ERR_ISNOTINBLACKLIST
	"GCRelationHandler_Info_Spouse_To_Black__List",			// RET_ERR_SPOUSETOBLACKLIST
	"GCRelationHandler_Info_Master_To_Black_List",			// RET_ERR_MASTERTOBLACKLIST
	"GCRelationHandler_Info_Prentice_To_Black_List",		// RET_ERR_PRENTICETOBLACKLIST
	"GCRelationHandler_Info_Brother_To_Black__List",		// RET_ERR_BROTHERTOBLACKLIST
	"GCRelationHandler_Info_Del_Spouse",					// RET_ERR_DELSPOUSE
	"GCRelationHandler_Info_Del_Master",					// RET_ERR_DELMASTER
	"GCRelationHandler_Info_Del_Prentice",					// RET_ERR_DELPRENTICE
	"GCRelationHandler_Info_Del_Brother",					// RET_ERR_DELBROTHER
	"GCRelationHandler_Info_Password_Err",					// RET_ERR_PASSWDMISMATCH
	"GCRelationHandler_Info_Reject_Add_Firend",				// RET_ERR_CANNOT_ADDFRIEND
	"GCRelationHandler_Info_Reject_Mail",					// RET_ERR_CANNOTRECEIVEMAIL
	"GCRelationHandler_Info_Reject_Mail_From_stranger",		// RET_ERR_NOTRECVSTRANGEMAIL
	"GCRelationHandler_Info_Friend_Group_Is_Full",			// RET_ERR_FRIENDGROUPISFULL
	"GCRelationHandler_Info_Black_Group_Is_Full",			// RET_ERR_BLACKGROUPISFULL
	"GCRelationHandler_Info_Unknow_Err",					// RET_ERR_RELATIONUNKNOWN
//	"。",
};

uint GCRelationHandler::Execute( GCRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		if( !CGameProcedure::s_pUISystem ) 
			return PACKET_EXE_CONTINUE;


		GC_RELATION* pReturn = pPacket->GetRelation();
		
		CHAR szText[_MAX_PATH];

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GCRelationHandler_Info_Other_Firend_Operate");
		_snprintf(szText, _MAX_PATH, strTemp.c_str());

		// 保存到数据池
		Relation* pRelation = CDataPool::GetMe()->GetRelation();
		if( pRelation == NULL )
		{
			Assert( FALSE );
			return PACKET_EXE_CONTINUE;
		}

		switch( pReturn->m_Type )
		{
		// 好友列表
		case RET_RELATIONLIST:	
			{
				GC_RELATIONLIST* pRelationList = &(pReturn->m_RelationList);

				pRelation->SetMood( pRelationList->GetMood() );

				// 好友
				for( INT i=0; i<pRelationList->GetFriendCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetFriend(i) );
				}

				// 黑名单
				for( INT i=0; i<pRelationList->GetBlackCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetBlackName(i) );
				}

				// 敌人
				for( INT i=0; i<pRelationList->GetEnemyCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetEnemyName(i) );
				}


				// 需要 push 一个事件通知 UI
				pRelation->UpdateUIList();
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 好友信息
		case RET_RELATIONINFO:	
			{
				pRelation->UpdateRelationInfo( &pReturn->m_RelationInfo );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 由于添加仇人，删除好友和黑名单
		case RET_DELFRIENDFORADDENEMY:
			{
				pRelation->RemoveRelation( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 添加好友
		case RET_ADDFRIEND:		
		// 添加仇人
		case RET_ADDENEMY:
			{
				RETURN_ADD_RELATION* pAddRelation = &(pReturn->m_NewRelation);
				RELATION_GROUP Drg;


				// 根据关系分别处理
				// 敌人
				if( pAddRelation->GetRelationType() == (UCHAR)RELATION_TYPE_ENEMY )
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_Enemy_List");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pAddRelation->GetRelationData()->GetName());
					//_snprintf(szText, _MAX_PATH, "你将%s加入敌人。", pAddRelation->GetRelationData()->GetName());
					
					Drg = RELATION_GROUP_ENEMY;
				}
				// 好友
				else if( pAddRelation->GetRelationType() == (UCHAR)RELATION_TYPE_FRIEND )
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_Firend_List");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pAddRelation->GetRelationData()->GetName());
					//_snprintf(szText, _MAX_PATH, "你将%s添加为好友。", pAddRelation->GetRelationData()->GetName());
					Drg = (RELATION_GROUP)pAddRelation->GetGroup();
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Err_Relation");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pAddRelation->GetRelationType());
					//_snprintf(szText, _MAX_PATH, "意外的关系类型：%d。", pAddRelation->GetRelationType());

					AssertEx(FALSE, szText);

					return PACKET_EXE_CONTINUE;
				}


				_RELATION* pRelationData;
				pRelationData = pAddRelation->GetRelationData();

				// 如果是临时好友，则移除
			//	pRelation->RemoveRelation( pRelationData->GetGUID() );
			//	pRelation->RemoveRelation( RELATION_GROUP_TEMPFRIEND, pRelationData->GetName() );

				// 如果是往好友列表里添加
				if ( pReturn->m_Type == RET_ADDFRIEND )
				{
					// 判断被加的人是否在仇人列表中，如果在，则删掉
					if ( pRelation->IsEnemyName(pAddRelation->GetRelationData()->GetName()) )
					{
						pRelation->RemoveRelation( pAddRelation->GetRelationData()->GetGUID() );
					}
					
				}

				// 好友成员数据
				SDATA_RELATION_MEMBER Member;

				Member.m_GUID = pRelationData->GetGUID();
				strncpy( Member.m_szName, pRelationData->GetName(), sizeof(Member.m_szName) - 1 );
				Member.m_RelationType = (RELATION_TYPE)pAddRelation->GetRelationType();
				Member.m_nLevel = pRelationData->GetLevel();
				Member.m_nMenPai = pRelationData->GetMenPai();
				Member.m_nPortrait = pRelationData->GetPortrait();
				Member.m_GuildID = pRelationData->GetGuildID();
				strncpy( Member.m_szGuildName, pRelationData->GetGuildName(), sizeof(Member.m_szGuildName) - 1 );
				Member.m_bOnlineFlag = (BOOL)pRelationData->GetOnlineFlag();
				Member.m_nFriendPoint = (INT)pRelationData->GetPoint();

				if( Member.m_bOnlineFlag )
				{
					strncpy( Member.m_szMood, pRelationData->GetMood(), sizeof(Member.m_szMood) - 1 );
					strncpy( Member.m_szTitle, pRelationData->GetTitle(), sizeof(Member.m_szTitle) - 1 );
					Member.m_SceneID = pRelationData->GetSceneID();
					Member.m_nTeamSize = pRelationData->GetTeamSize();
				}

				pRelation->AddRelation( Drg, &Member );
			}
			break;
			
		// 关系转移
		case RET_TRANSITION:
			{
				RELATION_GUID_UCHAR_UCHAR* pRelationInfo = &(pReturn->m_RelationGUIDUCHARUCHAR);

				RELATION_GROUP Srg;
				INT idx;

				pRelation->GetRelationPosition( pRelationInfo->GetTargetGUID(), Srg, idx );
				pRelation->MoveRelation( Srg, (RELATION_TYPE)pRelationInfo->GetRelationType(),
					(RELATION_GROUP)pRelationInfo->GetGroup(), pRelationInfo->GetTargetGUID() );

				return PACKET_EXE_CONTINUE;
			}
			break;

		// 删除好友
		case RET_DELFRIEND:
		// 删除敌人
		case RET_DELFROMENEMY:
			{
				pRelation->RemoveRelation( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 通知好友已经被加了
		case RET_ADDFRIENDNOTIFY:
			{
				STRING strTemp = "";
				strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_You_To_Firend_List");
				_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_NotifyFriend.GetName() );

				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );

				CDataPool::GetMe()->SystemTip_AddEventInfo( STT_FRIENDCHAT, szText );
				// 更新界面显示
				CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );

				return PACKET_EXE_CONTINUE;
			}
			break;

		// 通知已经成功将玩家移动到黑名单当中
		// 通过在其他列表移动到黑名单的返回消息在这需要单独处理，因为传回来的只有GUID没有名字
		case RET_ASKADDTOBLACKLIST:
			{
				STRING strTemp = "";
				strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_Black_List");
				
				RELATION_GROUP Srg;
				INT idx;
				pRelation->GetRelationPosition( pReturn->m_NotifyFriend.GetGUID(), Srg, idx );
				const SDATA_RELATION_MEMBER *pMember = CDataPool::GetMe()->GetRelation()->GetRelationInfo( ( RELATION_GROUP )Srg, idx );

				_snprintf(szText, _MAX_PATH, strTemp.c_str(), pMember->m_szName );

				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );

				return PACKET_EXE_CONTINUE;
			}
			break;

		// 通知已经成功将玩家移动到好友列表中
		case RET_ASKTHROWTOFRIENDLIST:
			{
				STRING strTemp = "";
				strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Throw_To_Friend_List");

				RELATION_GROUP Srg;
				INT idx;
				pRelation->GetRelationPosition( pReturn->m_NotifyFriend.GetGUID(), Srg, idx );
				const SDATA_RELATION_MEMBER *pMember = CDataPool::GetMe()->GetRelation()->GetRelationInfo( ( RELATION_GROUP )Srg, idx );

				_snprintf(szText, _MAX_PATH, strTemp.c_str(), pMember->m_szName );

				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );

				return PACKET_EXE_CONTINUE;	
			}
			break;
		// 通知玩家是否同意被加为好友
		case RET_ASKADDFRIEND:
			{
				if ( pReturn->m_NotifyFriend.GetGUID() != INVALID_ID)
				{
					CDataPool::GetMe()->Relation_SetInviteName( pReturn->m_NotifyFriend.GetName() );
					CEventSystem::GetMe()->PushEvent( GE_VALIDATE_ADD_FRIEND, pReturn->m_NotifyFriend.GetName() );
				}
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 通知玩家被拒绝加为好友
		case RET_REJECTADDFRIEND:
			{
				STRING strTemp = "";
				if ( strlen(pReturn->m_NotifyFriend.GetName()) > 0 )
				{
					strTemp = NOCOLORMSGFUNC("GCRejectAddFriend", pReturn->m_NotifyFriend.GetName());
					ADDNEWDEBUGMSG(strTemp);
				}

				return PACKET_EXE_CONTINUE;
			}
			break;

		// 在线好友列表
		case RET_ONLINELIST:
			{
				RETURN_ONLINE_LIST* pRecv = &(pReturn->m_RelationOnline);

				for( INT i=0; i<pRecv->GetOnlineCount(); ++i )
				{
					pRelation->UpdateOnlineFriend( pRecv->GetOnlineRelation(i) );
				}

				//CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(TRUE);

				return PACKET_EXE_CONTINUE;
			}
			break;

		// 好友上线
		case RET_RELATIONONLINE:
			{
				RETURN_NOTIFY_ONLINE* pRecv = &(pReturn->m_NotifyOnline);
				pRelation->RelationOnline( pRecv->GetTargetName(), pRecv->GetMood() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 好友下线
		case RET_RELATIONOFFLINE:
			{
				pRelation->RelationOffLine( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 新的心情
		case RET_NEWMOOD:
			{
				pRelation->SetMood( pReturn->m_NewMood.GetMood() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// 增加好友度
		case RET_FRIENDADDPOINT:
			{
				INT nPoint = pReturn->m_NotifyFriend.GetGUID();// 在这用的存guid的空间存的增加的好友度

				// 如果不是好友，不做任何操作
				if ( !(pRelation->IsFriendName( pReturn->m_NotifyFriend.GetName())) )
					return PACKET_EXE_CONTINUE;

				pRelation->AddFriendPoint( pReturn->m_NotifyFriend.GetName(), nPoint );

				return PACKET_EXE_CONTINUE;
			}
			break;

		// 创建新组
		//case RET_NEW_GROUP:
		//	{
		//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "创建新组成功！！" );

		//		STRING	groupName	= pReturn->m_AddNewGroup.GetGroupName();
		//		INT		groupId		= pReturn->m_AddNewGroup.m_uGroupID;


		//		CDataPool::GetMe()->Relation_SetNewGroup( groupId, groupName.c_str() );

		//		// 更新到界面
		//		CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_GROUP, "", groupName.c_str(), groupId );

		//		return PACKET_EXE_CONTINUE;
		//	}
		//	break;

		// 密码不匹配
		case RET_ERR_PASSWDMISMATCH:
			{
				CEventSystem::GetMe()->PushEvent( GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG );	// 打开unlock密码界面。
			}
			// break; // 这里不需要 break;

		// 目标不存在或不在线
		case RET_ERR_TARGETNOTEXIST:
		// 好友组已满
		case RET_ERR_GROUPISFULL:
		// 已经是好友
		case RET_ERR_ISFRIEND:
		// 已经被加入黑名单
		case RET_ERR_ISBLACKNAME:
		// 不能转换
		case RET_ERR_CANNOTTRANSITION:
		// 不是好友
		case RET_ERR_ISNOTFRIEND:
		// 不在黑名单
		case RET_ERR_ISNOTINBLACKLIST:
		// 将配偶加入黑名单
		case RET_ERR_SPOUSETOBLACKLIST:
		// 将师傅加入黑名单
		case RET_ERR_MASTERTOBLACKLIST:
		// 将徒弟加入黑名单
		case RET_ERR_PRENTICETOBLACKLIST:
		// 将结拜兄弟加入黑名单
		case RET_ERR_BROTHERTOBLACKLIST:
		// 删除配偶
		case RET_ERR_DELSPOUSE:
		// 删除师傅
		case RET_ERR_DELMASTER:
		// 删除徒弟
		case RET_ERR_DELPRENTICE:
		// 删除结义兄弟
		case RET_ERR_DELBROTHER:
		// 拒绝加为好友
		case RET_ERR_CANNOT_ADDFRIEND:
		// 拒绝接收任何邮件
		case RET_ERR_CANNOTRECEIVEMAIL:
		// 拒收陌生人邮件
		case RET_ERR_NOTRECVSTRANGEMAIL:
		// 好友组已经满员
		case RET_ERR_FRIENDGROUPISFULL:
		// 黑名单组已经满员
		case RET_ERR_BLACKGROUPISFULL:
		// 未知错误
		case RET_ERR_RELATIONUNKNOWN:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC(pRelationString[ pReturn->m_Type - RET_ERR_START ]);
				strncpy(szText, strTemp.c_str(), MAX_PATH-1);
				//strncpy(szText, pRelationString[ pReturn->m_Type - RET_ERR_START ], MAX_PATH-1);
			}
			break;

		default :
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		// 
		//CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, szText );
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
		//ADDTALKMSG(szText);
		//ADDNEWDEBUGMSG(szText);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
