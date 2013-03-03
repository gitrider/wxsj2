

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
//	"��",
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

		// ���浽���ݳ�
		Relation* pRelation = CDataPool::GetMe()->GetRelation();
		if( pRelation == NULL )
		{
			Assert( FALSE );
			return PACKET_EXE_CONTINUE;
		}

		switch( pReturn->m_Type )
		{
		// �����б�
		case RET_RELATIONLIST:	
			{
				GC_RELATIONLIST* pRelationList = &(pReturn->m_RelationList);

				pRelation->SetMood( pRelationList->GetMood() );

				// ����
				for( INT i=0; i<pRelationList->GetFriendCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetFriend(i) );
				}

				// ������
				for( INT i=0; i<pRelationList->GetBlackCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetBlackName(i) );
				}

				// ����
				for( INT i=0; i<pRelationList->GetEnemyCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetEnemyName(i) );
				}


				// ��Ҫ push һ���¼�֪ͨ UI
				pRelation->UpdateUIList();
				return PACKET_EXE_CONTINUE;
			}
			break;

		// ������Ϣ
		case RET_RELATIONINFO:	
			{
				pRelation->UpdateRelationInfo( &pReturn->m_RelationInfo );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// ������ӳ��ˣ�ɾ�����Ѻͺ�����
		case RET_DELFRIENDFORADDENEMY:
			{
				pRelation->RemoveRelation( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// ��Ӻ���
		case RET_ADDFRIEND:		
		// ��ӳ���
		case RET_ADDENEMY:
			{
				RETURN_ADD_RELATION* pAddRelation = &(pReturn->m_NewRelation);
				RELATION_GROUP Drg;


				// ���ݹ�ϵ�ֱ���
				// ����
				if( pAddRelation->GetRelationType() == (UCHAR)RELATION_TYPE_ENEMY )
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_Enemy_List");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pAddRelation->GetRelationData()->GetName());
					//_snprintf(szText, _MAX_PATH, "�㽫%s������ˡ�", pAddRelation->GetRelationData()->GetName());
					
					Drg = RELATION_GROUP_ENEMY;
				}
				// ����
				else if( pAddRelation->GetRelationType() == (UCHAR)RELATION_TYPE_FRIEND )
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_Firend_List");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pAddRelation->GetRelationData()->GetName());
					//_snprintf(szText, _MAX_PATH, "�㽫%s���Ϊ���ѡ�", pAddRelation->GetRelationData()->GetName());
					Drg = (RELATION_GROUP)pAddRelation->GetGroup();
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Err_Relation");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pAddRelation->GetRelationType());
					//_snprintf(szText, _MAX_PATH, "����Ĺ�ϵ���ͣ�%d��", pAddRelation->GetRelationType());

					AssertEx(FALSE, szText);

					return PACKET_EXE_CONTINUE;
				}


				_RELATION* pRelationData;
				pRelationData = pAddRelation->GetRelationData();

				// �������ʱ���ѣ����Ƴ�
			//	pRelation->RemoveRelation( pRelationData->GetGUID() );
			//	pRelation->RemoveRelation( RELATION_GROUP_TEMPFRIEND, pRelationData->GetName() );

				// ������������б������
				if ( pReturn->m_Type == RET_ADDFRIEND )
				{
					// �жϱ��ӵ����Ƿ��ڳ����б��У�����ڣ���ɾ��
					if ( pRelation->IsEnemyName(pAddRelation->GetRelationData()->GetName()) )
					{
						pRelation->RemoveRelation( pAddRelation->GetRelationData()->GetGUID() );
					}
					
				}

				// ���ѳ�Ա����
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
			
		// ��ϵת��
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

		// ɾ������
		case RET_DELFRIEND:
		// ɾ������
		case RET_DELFROMENEMY:
			{
				pRelation->RemoveRelation( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// ֪ͨ�����Ѿ�������
		case RET_ADDFRIENDNOTIFY:
			{
				STRING strTemp = "";
				strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_You_To_Firend_List");
				_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_NotifyFriend.GetName() );

				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );

				CDataPool::GetMe()->SystemTip_AddEventInfo( STT_FRIENDCHAT, szText );
				// ���½�����ʾ
				CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );

				return PACKET_EXE_CONTINUE;
			}
			break;

		// ֪ͨ�Ѿ��ɹ�������ƶ�������������
		// ͨ���������б��ƶ����������ķ�����Ϣ������Ҫ����������Ϊ��������ֻ��GUIDû������
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

		// ֪ͨ�Ѿ��ɹ�������ƶ��������б���
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
		// ֪ͨ����Ƿ�ͬ�ⱻ��Ϊ����
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

		// ֪ͨ��ұ��ܾ���Ϊ����
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

		// ���ߺ����б�
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

		// ��������
		case RET_RELATIONONLINE:
			{
				RETURN_NOTIFY_ONLINE* pRecv = &(pReturn->m_NotifyOnline);
				pRelation->RelationOnline( pRecv->GetTargetName(), pRecv->GetMood() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// ��������
		case RET_RELATIONOFFLINE:
			{
				pRelation->RelationOffLine( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// �µ�����
		case RET_NEWMOOD:
			{
				pRelation->SetMood( pReturn->m_NewMood.GetMood() );
				return PACKET_EXE_CONTINUE;
			}
			break;

		// ���Ӻ��Ѷ�
		case RET_FRIENDADDPOINT:
			{
				INT nPoint = pReturn->m_NotifyFriend.GetGUID();// �����õĴ�guid�Ŀռ������ӵĺ��Ѷ�

				// ������Ǻ��ѣ������κβ���
				if ( !(pRelation->IsFriendName( pReturn->m_NotifyFriend.GetName())) )
					return PACKET_EXE_CONTINUE;

				pRelation->AddFriendPoint( pReturn->m_NotifyFriend.GetName(), nPoint );

				return PACKET_EXE_CONTINUE;
			}
			break;

		// ��������
		//case RET_NEW_GROUP:
		//	{
		//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��������ɹ�����" );

		//		STRING	groupName	= pReturn->m_AddNewGroup.GetGroupName();
		//		INT		groupId		= pReturn->m_AddNewGroup.m_uGroupID;


		//		CDataPool::GetMe()->Relation_SetNewGroup( groupId, groupName.c_str() );

		//		// ���µ�����
		//		CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_GROUP, "", groupName.c_str(), groupId );

		//		return PACKET_EXE_CONTINUE;
		//	}
		//	break;

		// ���벻ƥ��
		case RET_ERR_PASSWDMISMATCH:
			{
				CEventSystem::GetMe()->PushEvent( GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG );	// ��unlock������档
			}
			// break; // ���ﲻ��Ҫ break;

		// Ŀ�겻���ڻ�����
		case RET_ERR_TARGETNOTEXIST:
		// ����������
		case RET_ERR_GROUPISFULL:
		// �Ѿ��Ǻ���
		case RET_ERR_ISFRIEND:
		// �Ѿ������������
		case RET_ERR_ISBLACKNAME:
		// ����ת��
		case RET_ERR_CANNOTTRANSITION:
		// ���Ǻ���
		case RET_ERR_ISNOTFRIEND:
		// ���ں�����
		case RET_ERR_ISNOTINBLACKLIST:
		// ����ż���������
		case RET_ERR_SPOUSETOBLACKLIST:
		// ��ʦ�����������
		case RET_ERR_MASTERTOBLACKLIST:
		// ��ͽ�ܼ��������
		case RET_ERR_PRENTICETOBLACKLIST:
		// ������ֵܼ��������
		case RET_ERR_BROTHERTOBLACKLIST:
		// ɾ����ż
		case RET_ERR_DELSPOUSE:
		// ɾ��ʦ��
		case RET_ERR_DELMASTER:
		// ɾ��ͽ��
		case RET_ERR_DELPRENTICE:
		// ɾ�������ֵ�
		case RET_ERR_DELBROTHER:
		// �ܾ���Ϊ����
		case RET_ERR_CANNOT_ADDFRIEND:
		// �ܾ������κ��ʼ�
		case RET_ERR_CANNOTRECEIVEMAIL:
		// ����İ�����ʼ�
		case RET_ERR_NOTRECVSTRANGEMAIL:
		// �������Ѿ���Ա
		case RET_ERR_FRIENDGROUPISFULL:
		// ���������Ѿ���Ա
		case RET_ERR_BLACKGROUPISFULL:
		// δ֪����
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
