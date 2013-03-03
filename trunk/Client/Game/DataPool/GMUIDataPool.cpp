
/**	
 *	
 *	���ܣ�	
 *	
 *	�޸ļ�¼��
 *			080313	ȥ�����������ص�����Ļ������ʾ�Ĵ��� TEAM_ENTER_MEMBER��TEAM_UPDATE_MEMBER��TEAM_HIDE_ALL_PLAYER��TEAM_REFRESH_DATA
 *
 */ 

#include "StdAfx.h"
#include "Object\Logic\Character\AI\GMAI_MySelf.h"
#include "GIGameInterface.h"
#include "GITimeSystem.h"
#include "Object/Logic/Item/Obj_Item.h"
#include "Object/Logic/Item/Obj_Item_Equip.h"
#include "Network/NetManager.h"
#include "CGAskItemInfo.h"
#include "GameCommand.h"
#include "DataPoolCommandDef.h"
#include "GIException.h"
#include "GIVariable.h"
#include "Event\GMEventSystem.h"
#include "ScriptCommandStruct.h"
#include "CGEventRequest.h"
#include "CGMissionRefuse.h"
#include "CGMissionAccept.h"
#include "CGMissionContinue.h"
#include "CGMissionSubmit.h"
#include "CGMissionCheck.h"
#include "CGMissionAbandon.h"
#include "GIUISystem.h"
#include "DataPool\GMDataPool.h"
#include "Procedure\GameProcedure.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "Sound\GMSoundSystem.h"
#include "NetWork\NetManager.h"
#include "GMUIDataPool.h"
#include "Action\GMActionSystem.h"
#include "TeamOrGroup.h"
#include "GIDatabase.h"
#include "World\WorldManager.h"
#include "CGAskTeamMemberInfo.h"

#include "CGTeamKick.h"								// �߳�һ����Ա.
#include "CGTeamDismiss.h"							// ��ɢ������Ϣ.
#include "CGTeamLeave.h"							// �뿪������Ϣ.
#include "CGTeamAppoint.h"							// �����ӳ���Ϣ
#include "Object\Manager\ObjectManager.h"				// �õ����������.
#include "Object\Logic\Character\Obj_PlayerMySelf.h"

#include "CGAskJoinMenpai.h"
#include "CGAskStudyXinfa.h"
#include "GCMissionModify.h"

#include "GMDP_CharacterData.h"
#include "Procedure\GameProcedure.h"
#include "DBC\GMDataBase.h"



KL_IMPLEMENT_DYNAMIC(CUIDataPool, GETCLASS(tUIDataPool));
CUIDataPool* CUIDataPool::s_pMe = NULL;


CUIDataPool::CUIDataPool()
{
	s_pMe = this;

	//---------------------------------------------------------------------------------------------------------------------
	// ���� 
	//

	// ��ǰ�ܾ����������.
	m_iCurRejectTeamIndex = -1;

	m_bOutGhostTimerWorking	= FALSE;
	m_nOutGhostTimer		= 0;
	m_nCurDialogNpcId		= -1;
	m_nCurShopNpcId			= -1;
}

CUIDataPool::~CUIDataPool()
{
	Release();
}


VOID CUIDataPool::Initial( VOID* )
{
	m_pPlayerData		= new CDetailAttrib_Player;
	m_pPlayerData->Init();

	m_pEventList			= new ScriptParam_EventList;
	m_pMissionInfo			= new ScriptParam_MissionInfo;
	m_pMissionDemandInfo	= new ScriptParam_MissionDemandInfo;
	m_pMissionContinueInfo	= new ScriptParam_MissionContinueInfo;
	m_pMissionTips			= new ScriptParam_MissionTips;

}

VOID CUIDataPool::Release( VOID )
{
	m_bOutGhostTimerWorking	= FALSE;
	m_nOutGhostTimer		= 0;
	
	delete m_pMissionTips;			m_pMissionTips = NULL;
	delete m_pMissionContinueInfo;	m_pMissionContinueInfo = NULL;
	delete m_pMissionDemandInfo;	m_pMissionDemandInfo = NULL;
	delete m_pMissionInfo;			m_pMissionInfo = NULL;
	delete m_pEventList;			m_pEventList = NULL;

	if ( m_pPlayerData != NULL )
	{
		m_pPlayerData->Term();
		delete m_pPlayerData;
		m_pPlayerData = NULL;
	}

	ClearTeamInfo();

//	ItemBox_Clear();
//	UserEquip_Clear();
//	UserBag_Clear();
}

VOID CUIDataPool::Tick( VOID )
{
	if ( m_pPlayerData != NULL )
		m_pPlayerData->Tick();

//	BOOL bResult;
//	if ( m_pMySelfAI != NULL && m_idMySelf != INVALID_ID )
//	{
//		//bResult = m_pMySelfAI->Logic();
////		if ( !bResult )
////			return ;
//	}
	if( m_bOutGhostTimerWorking )
	{
		INT nTempOutGhostTimer = m_nOutGhostTimer - (INT)(CGameProcedure::s_pTimeSystem->GetDeltaTime());
		if( nTempOutGhostTimer < 0 )
			nTempOutGhostTimer = 0;

		if( m_nOutGhostTimer != nTempOutGhostTimer )
		{
			// ��ȷ����ļ���
			INT nSOld = m_nOutGhostTimer/1000;
			INT nSNew = nTempOutGhostTimer/1000;
			m_nOutGhostTimer = nTempOutGhostTimer;
		}
	}


	// �µ�һ�쿪ʼʱͬ������ÿ�ջ���� 20100728 BLL 
	TickMySelfDetailAttrib();

	return ;
}

RC_RESULT CUIDataPool::OnCommand_( const SCommand_DPC *pCmd )
{
	RC_RESULT rcResult = RC_SKIP;

	BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
	switch ( pCmd->m_wID )
	{
	// �ű�ָ��
	case DPC_SCRIPT_COMMAND:
		{
			INT nCmdID	= pCmd->m_anParam[0];
			VOID *pBuf	= pCmd->m_apParam[1];

			switch ( nCmdID )
			{
			// ��һ�ζԻ���ȡ�����б�
			case SCRIPT_COMMAND_EVENT_LIST_RESPONSE:
				*m_pEventList = *((ScriptParam_EventList*)(pBuf));
				OnEventListResponse();
				break;

			// ��������Ϣ
			case SCRIPT_COMMAND_MISSION_RESPONSE:
				*m_pMissionInfo = *((ScriptParam_MissionInfo*)(pBuf));
				OnMissionInfoResponse();
				break;

			// ���������ѯ����
			case SCRIPT_COMMAND_MISSION_REGIE:
				*m_pEventList = *((ScriptParam_EventList*)(pBuf));
				OnMissionRegie();
				break;

			// ����������Ϣ�������������������������Ҫ����Ʒ����������Ʒ��
			case SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE:
				*m_pMissionDemandInfo = *((ScriptParam_MissionDemandInfo*)(pBuf));
				OnMissionDemandInfoResponse();
				break;

			// ���������Ϣ�������˽�����Ϣ���ڵ��continue֮����ȥ��ʾ��
			case SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE:
				*m_pMissionContinueInfo = *((ScriptParam_MissionContinueInfo*)(pBuf));
				OnMissionContinueInfoResponse();
				break;

			// ������������ʾ��Ϣ
			case SCRIPT_COMMAND_MISSION_TIPS:
				*m_pMissionTips = *((ScriptParam_MissionTips*)(pBuf));
				OnMissionTips();
				break;

			default:
				break;
			}
		}
		break;

	// ���������б�
	case DPC_UPDATE_MISSION_LIST://�����ƺ����ٺ�����
		{
			UINT dwObjID				= pCmd->m_adwParam[0];
			UINT dwModifyFlags			= pCmd->m_adwParam[1];
			_OWN_MISSION *paMissionBuf	= (_OWN_MISSION *)(pCmd->m_apParam[2]);

			// 20100510 DeleteCode
			// ����������û�д�����,Ҳû���õ�
			// BOOL bUpdateMissionFlags	= pCmd->m_abParam[3];
			// UINT *padwMissionFlagsBuf	= (UINT *)(pCmd->m_apParam[4]);

			// 20100510 ModifyCodeBegin
			UINT i;
			INT	 nExist;	// ���������������ID�ڵ�ǰ����������ѯ��ķ��ؽ��
			INT  nMyselfID;	// ����Լ��ķ�����ID

			nExist    = UINT_MAX;
			nMyselfID = CObjectManager::GetMe()->GetMySelf()->GetServerID();

			if( dwObjID == nMyselfID )
			{
				CDetailAttrib_Player* playData = const_cast<CDetailAttrib_Player*>(CUIDataPool::GetMe()->GetMySelfDetailAttrib());
				_OWN_MISSION *pMission;

				for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
				{
					if ( dwModifyFlags & (0x00000001 << i) )
					{
						// ��������ʱ��������񲻴�������ӣ��������޸�����
						nExist = playData->GetMissionIndexByID( paMissionBuf[i].m_idMission );

						pMission = &(playData->m_listMission.m_aMission[i]);
						memcpy( (void*)pMission, &paMissionBuf[i], sizeof(_OWN_MISSION) );					


						if ( nExist == UINT_MAX )
						{
							// ����ǵ�һ��������Ϊ�׵�ַ��memcpy���ˣ�������AddMission��ֻ�����������ֵ����
							if ( i == 0 )
							{
								playData->m_listMission.m_Count++;
								continue;
							}
							else
								playData->AddMission( pMission );
						}
						else
						{
							playData->ModifyMission( pMission );
						}

					}//if ( dwModifyFlags & (0x00000001 << i) )

				}//for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
				
			}//if( dwObjID == nMyselfID )
			
			rcResult = RC_OK;

			// 20100510 ModifyCodeEnd
		}
		break;

	// ����һ������
	case DPC_UPDATE_MISSION_ADD:
		{
			_OWN_MISSION *pMission	= (_OWN_MISSION *)(pCmd->m_apParam[0]);
			m_pPlayerData->AddMission( pMission );
			rcResult = RC_OK;
		}
		break;

	// ����һ������Ĳ���
	case DPC_UPDATE_MISSION_MODIFY:
		{
			// �޸�����
			if( pCmd->m_anParam[1] == GCMissionModify::MISSIONMODIFY_MISSION )
			{
				_OWN_MISSION *pMission	= (_OWN_MISSION *)(pCmd->m_apParam[0]);
				m_pPlayerData->ModifyMission( pMission );
			}
			// �޸����������
			else if( pCmd->m_anParam[1] == GCMissionModify::MISSIONMODIFY_MISSIONDATA )
			{
				m_pPlayerData->ModifyMissionData((INT*)pCmd->m_apParam[0]);
			}

			rcResult = RC_OK;
		}
		break;

	// ɾ��һ������
	case DPC_UPDATE_MISSION_REMOVE:
		{
			// ͨ������idɾ��
			MissionID_t idMission = (MissionID_t)(pCmd->m_adwParam[0]);
			m_pPlayerData->RemoveMission( idMission );
			rcResult = RC_OK;
		}
		break;
		//�޸�������ɱ�־
	case DPC_UPDATE_MISSION_HAVEDOWN_FALG:
		{
			MissionID_t nMissionID;
			BOOL bHaveDown;
			nMissionID = pCmd->m_anParam[0];
			bHaveDown = pCmd->m_abParam[1];
			CDetailAttrib_Player* playData = const_cast<CDetailAttrib_Player*>(CUIDataPool::GetMe()->GetMySelfDetailAttrib());
			if(NULL != playData)
			{
				playData->SetMissionComplete(nMissionID, bHaveDown);
			}
			rcResult = RC_OK;
		}
		break;

	// ˢ�¿���ʰȡ��������Ʒ�б�
	case DPC_UPDATE_CAN_PICK_MISSION_ITEM_LIST:
		{
			UINT dwItemCount	= pCmd->m_adwParam[0];
			UINT *paItemList	= (UINT*)(pCmd->m_apParam[1]);
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->UpdateCanPickMissionItemList( dwItemCount, paItemList );
			rcResult = RC_OK;
		}
		break;

	// ����һ������ʰȡ��������Ʒ
	case DPC_ADD_CAN_PICK_MISSION_ITEM:
		{
			UINT dwItemDataID	= pCmd->m_adwParam[0];
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->AddCanPickMissionItem( dwItemDataID );
			rcResult = RC_OK;
		}
		break;

	// ɾ��һ������ʰȡ��������Ʒ
	case DPC_REMOVE_CAN_PICK_MISSION_ITEM:
		{
			UINT dwItemDataID	= pCmd->m_adwParam[0];
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->RemoveCanPickMissionItem( dwItemDataID );
			rcResult = RC_OK;
		}
		break;
/*
	// ��ȴʱ��
	case DPC_UPDATE_COOL_DOWN_LIST:
		{
			UINT dwUpdateNum	= pCmd->m_adwParam[0];
			const UINT *pdwSkillCoolDownList	= (const UINT*)(pCmd->m_apConstParam[1]);
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->UpdateCoolDownList( pdwSkillCoolDownList, dwUpdateNum );
			rcResult = RC_OK;
		}
		break;
*/

	// �����Ϣ����
	case DPC_UPDATE_TEAM_OR_GROUP:
		{
			UINT res = pCmd->m_adwParam[0];
			CTeamOrGroup* pTeamOrGroup = GetTeamOrGroup();
			INT MyServerID = CObjectManager::GetMe()->GetMySelf()->GetServerID();
			STRING strTemp = "";

			// ����õ�������Ϣ�����쳣, ���沢�ҷ���.
			if( NULL == pTeamOrGroup )
			{
				Assert(pTeamOrGroup);
				return RC_ERROR;
			}

			TeamMemberInfo tmInfo;
			
			/*
			[0]	= Return
			[1]	= GUID
			[2]	= TeamID
			[3]	= GUIDEx
			[4]	= SceneID
			*/

			switch( res )
			{
			// �������������Ϣ�Ļظ�
			case TEAM_RESULT_TEAMREFRESH:
			// ��Ա�������
			case TEAM_RESULT_MEMBERENTERTEAM:
				{
					//	MessageBox(NULL, "���¶�Ա���룡", "BEEP(got it))", MB_OK|MB_ICONHAND);
					
					tmInfo.m_GUID		= pCmd->m_adwParam[1];
					tmInfo.m_OjbID		= pCmd->m_adwParam[3];
					tmInfo.m_SceneID	= pCmd->m_adwParam[4];
					tmInfo.m_nPortrait	= pCmd->m_adwParam[5];
					tmInfo.m_uDataID	= pCmd->m_adwParam[6];	// �Ա�
					tmInfo.m_uFamily	= pCmd->m_adwParam[7];	// ְҵ
					
					strncpy(tmInfo.m_szNick, (char*)&pCmd->m_apParam[8], MAX_CHARACTER_NAME);
					
					// ...

					// ��ӳ�Ա
					pTeamOrGroup->AddMember( &tmInfo, pCmd->m_adwParam[2] );

					//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�ж�Ա�������========");
					INT iUiIndex =  pTeamOrGroup->GetMemberUIIndex(tmInfo.m_GUID);
					if( INVALID_ID != iUiIndex )
					{
						// ��ʾ��Ļ��ߵĶ�Ա���
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MEMBER_ENTER, iUiIndex);

						if( pTeamOrGroup->GetLeaderGUID() == CObjectManager::GetMe()->GetMySelf()->GetServerGUID() )
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );
						}
						else
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );
						}

						// ˢ����ߵĶ��鴰�ڽ��档
						 CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
					}

					// ��Ա�������
					if( res == TEAM_RESULT_MEMBERENTERTEAM )
					{
						if( tmInfo.m_OjbID != MyServerID )
						{ // ���˼����˶���
							if(bLogValid)
							{
								strTemp = NOCOLORMSGFUNC("team_join_other", tmInfo.m_szNick);
								ADDNEWDEBUGMSG(strTemp);
								strTemp = COLORMSGFUNC("team_join_other", tmInfo.m_szNick);
								ADDTALKMSG(strTemp);
							}

							// ˢ����ߵĶ��鴰�ڽ��档
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
							CSoundSystemFMod::_PlayUISoundFunc(62);
						}
						else
						{
							// �����˱��˵Ķ���
							if( tmInfo.m_GUID != pTeamOrGroup->GetLeaderGUID() )
							{ 
								char* pLeaderName = pTeamOrGroup->GetMember(pTeamOrGroup->GetLeaderGUID())->m_szNick;

								//msg
								if( bLogValid )
								{
									strTemp = NOCOLORMSGFUNC("team_join_to_other", pLeaderName);
									ADDNEWDEBUGMSG(strTemp);
									strTemp = COLORMSGFUNC("team_join_to_other", pLeaderName);
									ADDTALKMSG(strTemp);
								}
								// ˢ����ߵĶ��鴰�ڽ��档
								CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
								CSoundSystemFMod::_PlayUISoundFunc(62);
							}
							else
							{
								if(bLogValid)
								{
									strTemp = NOCOLORMSGFUNC("team_create_ok");
									ADDNEWDEBUGMSG(strTemp);
								}
								CSoundSystemFMod::_PlayUISoundFunc(62);
				
							}

							pTeamOrGroup->ClearInviteTeam();	// ����������
							pTeamOrGroup->ClearProposer();		// ����������
						}

						if( IsTeamLeader_Me() )
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );
						}
						else
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );
						}

						// ����4��ʾ, ����򿪾�ˢ��, ����رղ�ˢ��.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
						ClearAllInviteTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );

						// ˢ����ߵĶ��鴰�ڽ��档 
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
					}
				}
				break;

			// ��ͨ��Ա�뿪����
			case TEAM_RESULT_MEMBERLEAVETEAM:
				{
					TeamMemberInfo* pTMInfo;
					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leave_other", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leave_other", pTMInfo->m_szNick);
							ADDTALKMSG(strTemp);
						}

						pTeamOrGroup->DelMember( pTMInfo->m_GUID );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
						// ����4��ʾ, ����򿪾�ˢ��, ����رղ�ˢ��.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leave_myself");
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leave_myself");
							ADDTALKMSG(strTemp);
						}

						pTeamOrGroup->DismissTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI );
						// ��������2����.
						//CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
						SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
					}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );
					CSoundSystemFMod::_PlayUISoundFunc(61);
				}
				break ;

			// �ӳ��뿪����
			case TEAM_RESULT_LEADERLEAVETEAM:
				{
					TeamMemberInfo* pTMInfo;
					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leave_other", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leave_other", pTMInfo->m_szNick);
							ADDTALKMSG(strTemp);
						}

						pTeamOrGroup->SetLeaderGUID( pCmd->m_adwParam[3] );
						pTeamOrGroup->DelMember( pTMInfo->m_GUID );
						
						if( IsTeamLeader_Me() )
						{
							// ����Լ���ɶӳ���ˢ�½���.�򿪶ӳ�����.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );

							//  ����4��ʾ, ����򿪾�ˢ��, ����رղ�ˢ��.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
							
						}
						else
						{
							// ����Լ���ɶӳ���ˢ�½���.�򿪶ӳ�����.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );

							// ˢ�¶�Ա��Ϣ.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4);
						}
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leave_myself");
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leave_myself");
							ADDTALKMSG(strTemp);
						}

						pTeamOrGroup->DismissTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI );
						SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();

						// ���������
						pTeamOrGroup->ClearInviteTeam();

						// ���������
						pTeamOrGroup->ClearProposer();
						
					}

					// ˢ����ߵĶ��鴰�ڽ��档 
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );
					CSoundSystemFMod::_PlayUISoundFunc(61);
				}
				break ;

			// �����ɢ
			case TEAM_RESULT_TEAMDISMISS:
				{
					pTeamOrGroup->DismissTeam();

					// ɾ������.
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI);
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );

					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMUIDataPool_Info_Team_Dismiss");
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, "��Ķ����ɢ�ˣ�" );
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, -1);
					
					//msg
					if(bLogValid)
					{
						strTemp = NOCOLORMSGFUNC("team_dismiss_myself");
						ADDNEWDEBUGMSG(strTemp);
						strTemp = COLORMSGFUNC("team_dismiss_myself");
						ADDTALKMSG(strTemp);
					}

					// ���������
					pTeamOrGroup->ClearInviteTeam();

					// ���������
					pTeamOrGroup->ClearProposer();

					CSoundSystemFMod::_PlayUISoundFunc(61);
					SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
				}
				break ;

			// �߳���Ա
			case TEAM_RESULT_TEAMKICK:
				{
					TeamMemberInfo* pTMInfo;
					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leave_force1", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leave_force1", pTMInfo->m_szNick);
							ADDTALKMSG(strTemp);
						}
						pTeamOrGroup->DelMember( pTMInfo->m_GUID );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );

						//  ����4��ʾ, ����򿪾�ˢ��, ����رղ�ˢ��.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leave_force");
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leave_force");
							ADDTALKMSG(strTemp);
						}
						pTeamOrGroup->DismissTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI);
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );
                        CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, -1);
						SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
					}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str());
					CSoundSystemFMod::_PlayUISoundFunc(61);
				}
				break ;

			// �����¶ӳ�
			case TEAM_RESULT_TEAMAPPOINT:
				{
					TeamMemberInfo* pTMInfo;
					INT NewLeaderGUID;

					NewLeaderGUID = pCmd->m_adwParam[3];

					pTeamOrGroup->Appoint( NewLeaderGUID );

					pTMInfo = pTeamOrGroup->GetMember( NewLeaderGUID );

					if ( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leader_change_other", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leader_change_other", pTMInfo->m_szNick);
							ADDTALKMSG(strTemp);
						}
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );
						// ����4��ʾ, ����򿪾�ˢ��, ����رղ�ˢ��.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("team_leader_change_myself");
							ADDNEWDEBUGMSG(strTemp);
							strTemp = COLORMSGFUNC("team_leader_change_myself");
							ADDTALKMSG(strTemp);
						}
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );
						// ����4��ʾ, ����򿪾�ˢ��, ����رղ�ˢ��.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );
					CSoundSystemFMod::_PlayUISoundFunc(61);
				}
				break ;

			// ��ʼ�л�����
			case TEAM_RESULT_STARTCHANGESCENE:
				{ // �����л��������ı�ͷ��״̬
					TeamMemberInfo* pTMInfo;

					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if ( pTMInfo == NULL )
					{ // ����������ܻ�����Ӹ�����л����������г���
						// Assert( FALSE && "Team info is not found when a team member start change scene(ignore this)." );
						return RC_OK;
					}

					pTMInfo->m_SceneID = pCmd->m_adwParam[4];

					if ( MyServerID == pTMInfo->m_OjbID )
					{
						break;
					}

//					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_UPTEDATA_MEMBER_INFO, pTeamOrGroup->GetMemberUIIndex( pTMInfo->m_GUID ) );
				//	CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��ʼ�л�������" );
				}
				break;

			// ���ѽ����³���
			case TEAM_RESULT_ENTERSCENE:
				{ // �ж��ѽ����³��������³�����Ϣ��������Լ���ͬ���������ѯ������Ϣ
					TeamMemberInfo* pTMInfo;

					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if( pTMInfo == NULL )
					{ // ����������ܻ�����Ӹ�����л����������г���
						// Assert( FALSE && "Team info is not found when a team member enter scene(ignore this)." );
						return RC_OK;
					}

					if( MyServerID == pTMInfo->m_OjbID )
					{ // �Լ������³��������ܣ���Ϊ���Զ���ȡ������Ϣ
						break;
					}

					pTMInfo->m_SceneID = pCmd->m_adwParam[4];
					pTMInfo->m_OjbID = pCmd->m_adwParam[3];
					pTMInfo->m_bDeadLink = FALSE; // ���볡����Ȼ�Ƕ���״̬����ȥ�Զ����Ƿ�����������ж�

					//if( CGameProcedure::s_pVariableSystem->GetAs_Int("Scene_ID") == pTMInfo->m_SceneID )
					//{
					CGAskTeamMemberInfo tmMsg;
					tmMsg.SetSceneID( pTMInfo->m_SceneID );
					tmMsg.SetGUID( pTMInfo->m_GUID );
					tmMsg.setObjID( pTMInfo->m_OjbID );

					CNetManager::GetMe()->SendPacket( &tmMsg );
					//}

//					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_UPTEDATA_MEMBER_INFO, pTeamOrGroup->GetMemberUIIndex( pTMInfo->m_GUID ) );

				}
				break;

			default :
				{
					Assert(FALSE) ;
				}
				break ;
			}

			rcResult = RC_OK;
		}
		break;
/*
	case DPC_UI_COMMAND:
		{
			INT nUIIndex	= pCmd->m_anParam[0];
			VOID *pBuf	= pCmd->m_apParam[1];
			*pX_PARAM = *((X_PARAM*)(pBuf)); 

			CEventSystem::GetMe()->PushEvent(GE_UI_COMMAND, nUIIndex);
		}
		break;
*/
		default:
			rcResult = RC_SKIP;
			break;
	}

	return rcResult;
}


// ��һ�ζԻ���ȡ�����б�
VOID CUIDataPool::OnEventListResponse( VOID )
{
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pEventList->m_idNPC);

	if( pNPC )
	{
		SetCurDialogNpcId( pNPC->GetServerID() );
	}

	CEventSystem::GetMe()->PushEvent(GE_QUEST_EVENTLIST, pNPC ? pNPC->GetID() : -1);
}

//����������ʹ��б���ѡȡ������
//nExIndex2������Ϣ
VOID CUIDataPool::SendSelectEvent(INT nIndex,INT nExIndex1,INT nExIndex2)
{
	const ScriptEventItem *pItem;

	BYTE i, yNum;
	INT nSelectIndex;
	yNum = 0;
	nSelectIndex = -1;

	for( i = 0; i < m_pEventList->m_yItemCount; i++ )
	{
		if(( m_pEventList->GetItem( i )->m_index == nExIndex2) && (m_pEventList->GetItem( i )->m_idScript == nExIndex1 ))
		{
			pItem = m_pEventList->GetItem( i );

			CGEventRequest msg;
			msg.setStateIndex(pItem->m_state);
			msg.setExIndex( pItem->m_index );
			msg.setScriptID( pItem->m_idScript );
			msg.setNPCID( m_pEventList->m_idNPC );
			CNetManager::GetMe()->SendPacket( &msg );

//			CGameProcedure::s_pGfxSystem->PushDebugString("CGEventRequest");

			break;
		}

		/*if ( m_pEventList->GetItem( i )->m_nType != EVENT_ITEM_TYPE_TEXT )
		{
			if ( (BYTE)nIndex == yNum )
			{
				nSelectIndex = (INT)i;
				break;
			}
			yNum++;
		}*/
	}

	/*if ( nSelectIndex >= 0 )
	{
		pItem = m_pEventList->GetItem( (BYTE)nSelectIndex );

		CGEventRequest msg;
		msg.setExIndex( pItem->m_index );
		msg.setScriptID( pItem->m_idScript );
		msg.setNPCID( m_pEventList->m_idNPC );
		CNetManager::GetMe()->SendPacket( &msg );

		CGameProcedure::s_pGfxSystem->PushDebugString("CGEventRequest");
	}*/
}

//����������ؽ�������
VOID CUIDataPool::SendAcceptEvent()
{
	CGMissionAccept msg;
	msg.setScriptID( m_pMissionInfo->m_idScript );
	msg.setNPCID( m_pMissionInfo->m_idNPC );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionAccept");
}

//����������ؾܾ�����
VOID CUIDataPool::SendRefuseEvent()
{
	CGMissionRefuse msg;
	msg.setScriptID( m_pMissionInfo->m_idScript );
	msg.setNPCID( m_pMissionInfo->m_idNPC );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionRefuse");
}

//����������ؼ�������
VOID CUIDataPool::SendContinueEvent()
{
	CGMissionContinue msg;
	msg.setScriptID( m_pMissionDemandInfo->m_idScript );
	msg.setNPCID( m_pMissionDemandInfo->m_idNPC );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionContinue");
}

//����������ط�������
VOID CUIDataPool::SendAbandonEvent( INT nIndex )
{
	CGMissionAbandon msg;
	msg.setScriptID((ScriptID_t)nIndex);
	CNetManager::GetMe()->SendPacket( &msg );//

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionAbandon");
}

VOID CUIDataPool::SendMissionItemInfo(INT nPetIndex)
{
	CGMissionCheck msg;

	// ʹ�����ݳ��е� m_MissionItemList �������Լ��ı��
	INT nItemIndex[3];
	for(int i=0;i<3;i++)
	{
		if(CDataPool::GetMe()->MissionBox_GetItem(i))
			nItemIndex[i] = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(CDataPool::GetMe()->MissionBox_GetItem(i)->GetGUID());
		else
			nItemIndex[i] = 255;
	}

	msg.setPetIndex(nPetIndex);
	msg.setItemIndexList((BYTE*)nItemIndex);
	msg.setScriptID( m_pMissionDemandInfo->m_idScript );
	msg.setNPCID( m_pMissionDemandInfo->m_idNPC );

	CNetManager::GetMe()->SendPacket(&msg);
}

//������������������
VOID CUIDataPool::SendCompleteEvent( INT nIndex )
{
	CGMissionSubmit msg;
	msg.setScriptID( m_pMissionContinueInfo->m_idScript );
	msg.setNPCID( m_pMissionContinueInfo->m_idNPC );
	//��Ҫ������ƷID
	msg.setSelectRadioID( m_pMissionContinueInfo->GetSelectItemID(nIndex) );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionSubmit");
}

VOID CUIDataPool::OnMissionInfoResponse( VOID )
{
	// ���ԭ������
	QuestReward_Clear();

	// ���ɽ�����Ʒ��Ϣ
	INT nItemCount = m_pMissionInfo->m_yBonusCount;
	for(INT i=0; i<nItemCount; i++)
	{
		SMissionBonus* pRewardItem = &(m_pMissionInfo->m_aBonus[i]);

		if(	pRewardItem->m_nType == MISSION_BONUS_TYPE_ITEM ||
			pRewardItem->m_nType == MISSION_BONUS_TYPE_ITEM_RADIO)
		{
			tObject_Item* pItem = CObject_Item::NewItem(pRewardItem->m_ItemBonus.m_uItemID);
			if(!pItem)
			{
				KLThrow("Invalid reward item id: %d", pRewardItem->m_ItemBonus.m_uItemID);
			}
			//���ӵ����ݳ���
			QuestRewardItem newRewardItem;
			newRewardItem.pItemData = pRewardItem;
			newRewardItem.pItemImpl = pItem;
			newRewardItem.bSelected = FALSE;
			QuestReward_AddItem(newRewardItem);
		}
		else
		{
			//���ӵ����ݳ���
			QuestRewardItem newRewardItem;
			newRewardItem.pItemData = pRewardItem;
			newRewardItem.pItemImpl = 0;
			newRewardItem.bSelected = FALSE;
			QuestReward_AddItem(newRewardItem);
		}
	}

	//֪ͨUI
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pMissionInfo->m_idNPC);
	if(pNPC)
	{
		SetCurDialogNpcId( pNPC->GetServerID() );
	}

	CEventSystem::GetMe()->PushEvent(GE_QUEST_INFO, pNPC ? pNPC->GetID(): -1);
}

VOID CUIDataPool::OnMissionRegie( VOID )
{
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pEventList->m_idNPC);

	CEventSystem::GetMe()->PushEvent(GE_QUEST_REGIE, pNPC ? pNPC->GetID() : -1);
}

VOID CUIDataPool::OnMissionDemandInfoResponse( VOID )
{
	//���ԭ������
	QuestDemand_Clear();
	//����������Ʒ��Ϣ
	INT nItemCount = m_pMissionDemandInfo->m_yDemandCount;
	for(INT i=0; i<nItemCount; i++)
	{
		tObject_Item* pItem = CObject_Item::NewItem(m_pMissionDemandInfo->m_aDemandItem[i].m_uItemID);//->m_itemBonus.m_dwItemID);
		
		//���ӵ����ݳ���
		QuestDemandItem newDemandItem;
		newDemandItem.pDemandItem = &(m_pMissionDemandInfo->m_aDemandItem[i]);
		newDemandItem.pItemImpl = pItem;
		QuestDemand_AddItem(newDemandItem);
	}

	//֪ͨUI
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pMissionDemandInfo->m_idNPC);
	if ( m_pMissionDemandInfo->m_bDone == ScriptParam_MissionDemandInfo::MISSION_DONE )
	{
		//�����Ѿ����
		if(pNPC)
		{
			SetCurDialogNpcId( pNPC->GetServerID() );
		}
		CEventSystem::GetMe()->PushEvent(GE_QUEST_CONTINUE_DONE, pNPC ? pNPC->GetID(): -1);
	}
	else if ( m_pMissionDemandInfo->m_bDone == ScriptParam_MissionDemandInfo::MISSION_NODONE )
	{
		//����û�����
		if(pNPC)
		{
			SetCurDialogNpcId( pNPC->GetServerID() );
		}
		CEventSystem::GetMe()->PushEvent(GE_QUEST_CONTINUE_NOTDONE, pNPC ? pNPC->GetID(): -1);
	}
	else if( m_pMissionDemandInfo->m_bDone == ScriptParam_MissionDemandInfo::MISSION_CHECK )
	{
		//�����ύ����
		if(pNPC)
		{
			SetCurDialogNpcId( pNPC->GetServerID() );
		}
		//��Ҫ�����ж�����Ҫ�ύ��Ʒ
		CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION, pNPC ? pNPC->GetID(): -1);//����¼�ͬʱҲ��򿪱�������
	}
}

VOID CUIDataPool::OnMissionContinueInfoResponse( VOID )
{
	//���ԭ������
	QuestReward_Clear();

	//����������Ʒ��Ϣ
	INT nItemCount = m_pMissionContinueInfo->m_yBonusCount;
	for(INT i=0; i<nItemCount; i++)
	{
		tObject_Item* pItem = CObject_Item::NewItem(m_pMissionContinueInfo->m_aBonus[i].m_ItemBonus.m_uItemID);//->m_itemBonus.m_dwItemID);
		
		//���ӵ����ݳ���
		QuestRewardItem newRewardItem;
		newRewardItem.pItemData = &(m_pMissionContinueInfo->m_aBonus[i]);
		newRewardItem.pItemImpl = pItem;
		newRewardItem.bSelected = FALSE;
		QuestReward_AddItem(newRewardItem);
	}

	//�����������֮��
	CEventSystem::GetMe()->PushEvent(GE_QUEST_AFTER_CONTINUE);
}

VOID CUIDataPool::OnMissionTips( VOID )
{
	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE/*GE_QUEST_TIPS*/,m_pMissionTips->m_strText.m_szString);
}

VOID CUIDataPool::BeginOutGhostTimer( INT nOutGhostTimer )
{
	m_bOutGhostTimerWorking = TRUE;
	m_nOutGhostTimer = nOutGhostTimer;
}

VOID CUIDataPool::EndOutGhostTimer( VOID )
{
	m_bOutGhostTimerWorking = FALSE;
}


//================================================================================
VOID CUIDataPool::QuestReward_Clear(VOID)
{
	for(INT i=0; i<(INT)m_vecQuestRewardItem.size(); i++)
	{
		CObject_Item::DestroyItem((CObject_Item*)(m_vecQuestRewardItem[i].pItemImpl));
		m_vecQuestRewardItem[i].pItemImpl = NULL;
	}
	
	m_vecQuestRewardItem.clear();
}

VOID CUIDataPool::QuestDemand_Clear(VOID)
{
	for(INT i=0; i<(INT)m_vecQuestDemandItem.size(); i++)
	{
		CObject_Item::DestroyItem((CObject_Item*)(m_vecQuestDemandItem[i].pItemImpl));
		m_vecQuestDemandItem[i].pItemImpl = NULL;
	}
	//���֮ǰ������Action
	CActionSystem::GetMe()->CleanInvalidAction();
	m_vecQuestDemandItem.clear();
}

VOID CUIDataPool::QuestDemand_AddItem(const QuestDemandItem& itemDemand)
{
	m_vecQuestDemandItem.push_back(itemDemand);
	CActionSystem::GetMe()->CreateObjectItemByTableId(itemDemand.pItemImpl);
}

VOID CUIDataPool::QuestReward_AddItem(const QuestRewardItem& itemReward)
{
	m_vecQuestRewardItem.push_back(itemReward);
	if ( itemReward.pItemImpl)
	{
		itemReward.pItemImpl->SetTypeOwner(tObject_Item::IO_QUESTVIRTUALITEM);
		CActionSystem::GetMe()->CreateObjectItemByTableId(itemReward.pItemImpl);
	}
}


// �µ�һ��ͬ������ÿ�ջ���� 20100728 BLL 
VOID CUIDataPool::TickMySelfDetailAttrib()
{
	CUIDataPool::GetMe()->GetMySelfDetailAttrib()->RefreshDayTipMis();
}

//================================================================================


//------------------------------------------------------------------------------------------------------------------------------------------------
// �����صĲ���
//
// ���ڶ�����Ϣֻ�뵱ǰ����й�ϵ, ��������Ϸ�ͻ���ֻ����һ��, Ӧ��д�����ݳ���.
//

// ���»�ȡ�����Ա��ϸ��Ϣ
VOID CUIDataPool::ClearTeamInfo(VOID)
{
	if( m_TeamOrGroup.HasTeam() )
	{
		m_TeamOrGroup.DismissTeam();
		m_TeamOrGroup.ClearInviteTeam();
		m_TeamOrGroup.ClearProposer();
	}
}

// ����SOURCE_Id
VOID CUIDataPool::SetSourceGUID(GUID_t Id) 
{
	m_IdSource = Id;
}

	// �õ�SOURCE_Id
GUID_t CUIDataPool::GetSourceGUID()
{
	return m_IdSource;
}

// �õ�������SOURCE_Id
GUID_t CUIDataPool::GetApplySourceGUID(int iApplyIndex)
{
	GUID_t ApplyGUID;
	// ���������õ�ĳ��������
	const TeamCacheInfo*	pApplyInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		pApplyInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex(iApplyIndex);
		if(pApplyInfo)
		{
			ApplyGUID = pApplyInfo->m_GUID;
			return ApplyGUID;
		}

	}
	return INVALID_GUID;
}

	// ����DESTINATION_Id
VOID CUIDataPool::SetDestinationGUID(GUID_t Id) 
{
	m_IdDestination = Id;
}


	// �õ�DESTINATION_Id
GUID_t CUIDataPool::GetDestinationGUID()
{
	return m_IdDestination;
}




// ���ö��������ߵ�Id
VOID CUIDataPool::SetTeamInvitorGUID(GUID_t  Id) 
{
	m_IdSource = Id;
}

// �õ����������ߵ�id
GUID_t CUIDataPool::GetTeamInvitorGUID()
{
	return m_IdSource;
}

// �õ����������ߵ�id
GUID_t CUIDataPool::GetTeamInvitorGUID(int iTeamIndex)
{
	const InviteTeam* pInviteTeam = NULL;
	TeamCacheInfo*    pMemberInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		pInviteTeam = CUIDataPool::GetMe()->GetTeamOrGroup()->GetInviteTeamByIndex(iTeamIndex);
		if(pInviteTeam)
		{
			// ��¼��ǰ��������guid
			GUID_t guid = pInviteTeam->m_InviterGUID;
			return guid;
		}
	}

	return INVALID_GUID;
}

// �뿪����
VOID CUIDataPool::LeaveTeam()
{
	CGTeamLeave msg;
	//BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
	
	msg.SetGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
	CNetManager::GetMe()->SendPacket(&msg);
	//if(bLogValid)	ADDNEWDEBUGMSG(STRING("�����뿪������Ϣ!"));
}

// ��ɢ����
VOID CUIDataPool::DismissTeam()
{
	CGTeamDismiss msg;
	//BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
	
	msg.SetGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
	CNetManager::GetMe()->SendPacket(&msg);
	// CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"���ͽ�ɢ������Ϣ!");
	//if(bLogValid)	ADDNEWDEBUGMSG(STRING("���ͽ�ɢ������Ϣ!"));
}


// ������Ϣ
VOID CUIDataPool::KickTeamMember()
{

	const TeamMemberInfo* pMemberInfo = NULL;
	BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(m_iSelectedTeamMember);
		if(pMemberInfo)
		{
			if(CObjectManager::GetMe()->GetMySelf()->GetServerGUID() == pMemberInfo->m_GUID)
			{
				return;
			}

			CGTeamKick msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
			CNetManager::GetMe()->SendPacket(&msg);
			//if(bLogValid)	ADDNEWDEBUGMSG(STRING("�����߳�������Ϣ!"));

		}
		else
		{
			if(bLogValid)	
			{
				
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMUIDataPool_Info_Team_Dismiss");
				ADDNEWDEBUGMSG(strTemp);
				//ADDNEWDEBUGMSG(STRING("�Ҳ���Ҫ����Ķ�Ա!"));
			}
		}
	}
	else
	{
		if(bLogValid)	
		{
			
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMUIDataPool_Info_Team_Dismiss");
			ADDNEWDEBUGMSG(strTemp);
			//ADDNEWDEBUGMSG(STRING("�Ҳ���Ҫ����Ķ�Ա!"));
		}
	}

}

// ������Ϣ
VOID CUIDataPool::KickTeamMember(int iIndex)
{

	//const TeamMemberInfo* pMemberInfo = NULL;
	//BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	//if(GetTeamOrGroup())
	//{
	//	pMemberInfo = GetTeamOrGroup()->GetMemberByIndex(iIndex);
	//	if(pMemberInfo)
	//	{
	//		CGTeamKick msg;
	//		msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
	//		msg.SetDestGUID(pMemberInfo->m_GUID);
	//		CNetManager::GetMe()->SendPacket(&msg);
	//		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"�����߳�������Ϣ!");

	//	}
	//	else
	//	{
	//		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ҳ���Ҫɾ���Ķ�Ա!");
	//	}
	//}
	//else
	//{
	//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ҳ���Ҫɾ���Ķ�Ա!");
	//}//

	const TeamMemberInfo* pMemberInfo = NULL;
	BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(iIndex);
		if(pMemberInfo)
		{
			if(CObjectManager::GetMe()->GetMySelf()->GetServerGUID() == pMemberInfo->m_GUID)
			{
				return;
			}

			CGTeamKick msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
			CNetManager::GetMe()->SendPacket(&msg);
			//if(bLogValid)	ADDNEWDEBUGMSG(STRING("�����߳�������Ϣ!"));

		}
		else
		{
			if(bLogValid)	
			{
				
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMUIDataPool_Info_Team_Dismiss");
				ADDNEWDEBUGMSG(strTemp);
				//ADDNEWDEBUGMSG(STRING("�Ҳ���Ҫ����Ķ�Ա!"));
			}
		}
	}
	else
	{
		if(bLogValid)	
		{
				
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMUIDataPool_Info_Team_Dismiss");
			ADDNEWDEBUGMSG(strTemp);
			//ADDNEWDEBUGMSG(STRING("�Ҳ���Ҫ����Ķ�Ա!"));
		}
	}

}



	// �����ӳ�
VOID CUIDataPool::AppointLeader(int iIndex)
{
	const TeamMemberInfo* pMemberInfo = NULL;
	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(iIndex);
		if(pMemberInfo)
		{
			CGTeamAppoint msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
		
			CNetManager::GetMe()->SendPacket(&msg);

			// ���������.
			GetTeamOrGroup()->ClearProposer();
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"���������ӳ���Ϣ!");

			return;
		}
	}
		
	//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"���������ӳ���Ϣʧ��!");
	
}

// �����ӳ�
VOID CUIDataPool::AppointLeader()
{
	const TeamMemberInfo* pMemberInfo = NULL;
	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(m_iSelectedTeamMember);
		if(pMemberInfo)
		{
			CGTeamAppoint msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
		
			CNetManager::GetMe()->SendPacket(&msg);

			// ���������.
			GetTeamOrGroup()->ClearProposer();
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"���������ӳ���Ϣ!");
		}
	}
	else
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMUIDataPool_Info_Team_Dismiss");
		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"���������ӳ���Ϣʧ��!");
	}
}


// ͬ�����һ�������, ������е��������.
VOID CUIDataPool::ClearAllInviteTeam()
{
	if(GetTeamOrGroup())
	{
		GetTeamOrGroup()->ClearInviteTeam();
	}
}

// �ܾ���ǰ�ܾ��Ķ���.
VOID CUIDataPool::EraseCurRejectTeam()
{
	if(GetTeamOrGroup())
	{
		GetTeamOrGroup()->EraseInviteTeam(m_iCurRejectTeamIndex);
	}
}


// ĳ����Ա�Ƿ��Ƕӳ�.
BOOL CUIDataPool::IsTeamLeader(INT iIndex)
{
	
	const TeamMemberInfo* pMemberInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iIndex);

		if(pMemberInfo)
		{
			if(pMemberInfo->m_GUID == CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID())
			{
				return TRUE;
			}

		}
	}

	return FALSE;
}

// �Լ��Ƿ��Ƕӳ�.
BOOL CUIDataPool::IsTeamLeader_Me()
{
	if(CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam())
	{
		if( CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID()
		 == CObjectManager::GetMe()->GetMySelf()->GetServerGUID()
		 )
		{
			return TRUE;
		}
	}

	return FALSE;
}


// ���õ�ǰ���������.
void CUIDataPool::SetCurTeamType(INT iTeamType)
{
	m_iCurTeamType = iTeamType;
}

// �õ���ǰ�Ķ��������.
INT  CUIDataPool::GetCurTeamType()
{

	return m_iCurTeamType;

}

// ��ǰ�Ƿ���һ��������
BOOL CUIDataPool::IsInTeam()
{
	return CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam();
}


// ��Ա���Լ��Ƿ���ͬһ��������
BOOL CUIDataPool::IsInScene(INT iIndex)
{
	return CUIDataPool::GetMe()->GetTeamOrGroup()->IsInScene(iIndex);
}

// �ж�һ��������Լ��Ƿ���ͬһ�������С�
BOOL CUIDataPool::IsTeamMember(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


// ͨ��guid�õ���������. 
LPCTSTR CUIDataPool::GetTeamNameByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_szNick;
				}
			}
		}
	}
	return NULL;
}

	// ͨ��guid�õ�����HP. 
int CUIDataPool::GetTeamHPByGUID(GUID_t Id)
{

	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_nHP;
				}
			}
		}
	}

	return 0;
}

	// ͨ��guid�õ�����MP. 
int CUIDataPool::GetTeamMPByGUID(GUID_t Id)
{

	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_nMP;
				}
			}
		}
	}
	return 0;
}

	// ͨ��guid�õ�����Range. 
int CUIDataPool::GetTeamRangeByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_nAnger;
				}
			}
		}
	}
	return 0;
}

// ͨ��guid�õ�����Level. 
int CUIDataPool::GetTeamLevelByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_uLevel;
				}
			}
		}
	}

	return 0;
}


	// ͨ��guid�õ�����ͷ��. 
LPCTSTR CUIDataPool::GetTeamIconByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)	// char_face.tab
					//�����ﶨ���
					const _DBC_CHAR_FACE* pFaceInfo = NULL;
					pFaceInfo = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU(pTeamMemberInfo->m_nPortrait));

					// �õ�ͷ������
					if(pFaceInfo)
					{
						return pFaceInfo->pImageSetName;
					}
					else
					{
						return NULL;
					}

				}
			}
		}
	}


	return NULL;
}

// ͨ��guid�õ�����HP. 
float CUIDataPool::GetTeamHPPercentByGUID(GUID_t Id)
{
	
	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					if(pTeamMemberInfo->m_dwMaxHP)
					{
						return (1.0f * pTeamMemberInfo->m_nHP) / ((float)(pTeamMemberInfo->m_dwMaxHP));
					}
				}
			}
		}
	}

	return 0;
}

// ͨ��guid�õ�����MP. 
float CUIDataPool::GetTeamMPPercentByGUID(GUID_t Id)
{

	CTeamOrGroup* pTeamGroup = NULL;

	// �õ�������Ϣ
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// û�ж���
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// �õ������ж�Ա�ĸ���
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// �õ���Ա����Ϣ
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					if(pTeamMemberInfo->m_dwMaxMP)
					{
						return (1.0f * pTeamMemberInfo->m_nMP) / ((float)(pTeamMemberInfo->m_dwMaxMP));
					}
				}
			}
		}
	}

	return 0;
}


// ���ö�Ա�����ߺͱ�������guid
INT CUIDataPool::SetMemberInviteGUID(GUID_t SId, GUID_t DId)
{
	INT iPos = -1; 
	for(int i = 0; i < MAX_INVITE_COUNT; i++)
	{
		if(!m_MemberInviteInfo[i].bHave)
		{
			m_MemberInviteInfo[i].bHave = TRUE;
			m_MemberInviteInfo[i].SourceId = SId;
			m_MemberInviteInfo[i].DestinId = DId;
			iPos = i;
			break;
		}
	}

	return iPos;
}


//	�õ���Ա�����ߺͱ�������guid�� ��ɾ��
BOOL CUIDataPool::GetMemberInviteGUID(int iPos, GUID_t& SId, GUID_t& DId, BOOL bDel)
{
	if((iPos < 0)||(iPos >= MAX_INVITE_COUNT))
	{
		return FALSE;
	}

	SId = m_MemberInviteInfo[iPos].SourceId;
	DId = m_MemberInviteInfo[iPos].DestinId;

	if(bDel)
	{
		m_MemberInviteInfo[iPos].bHave = FALSE;
	}

	return TRUE;


}

// 20100510 AddCodeBegin
// ��ǰNpc�����Ƿ��пɽ�����(����������ǰ�������Ѿ����)
BOOL CUIDataPool::IsNpcMissionCanAccept( LPCTSTR szNpcName )
{
	const CDetailAttrib_Player* pAttrib = CUIDataPool::GetMe()->GetMySelfDetailAttrib();
	std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();

	for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++ )
	{
		if( strcmp( szNpcName, (*iter)->szNPCName ) == 0 ) 
		{
			_DBC_QUEST_LOG* pLog = (*iter);

			// �Ƿ��Ѿ�����
			if( pAttrib->IsMissionHaveDone(pLog->nMissionID) )
				continue;

			// �����Ƿ����������
			INT nIndex = pAttrib->GetMissionIndexByID(pLog->nMissionID);
			if( nIndex != UINT_MAX )
				continue;

			//
			// ǰ�������Ƿ����
			//

			if( pLog->nPrevMission1 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission1) )
					continue;
			}

			if( pLog->nPrevMission2 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission2) )
					continue;
			}

			if( pLog->nPrevMission3 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission3) )
					continue;
			}

			if( pLog->nPrevMission4 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission4) )
					continue;
			}

			if( pLog->nPrevMission5 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission5) )
					continue;
			}

			return TRUE;

		}//if( szNpcName == iter->szNPCName) 	

	}//for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++ )

	return FALSE;
}

// 20100510 AddCodeEnd






//
// �����صĲ���
//---------------------------------------------------------------------------------------------------------------------------------------

/*

//================================================================================
// �ķ���ʦ���ϵ��ķ�����
VOID CUIDataPool::SendStudyXinfaEvent(INT nXinfaID)
{
	//���ɣ��ķ�ID�����ڵļ���
	CGAskStudyXinfa::_STUDYXINFA_INFO	StudyXinfaInfo;

	//StudyXinfaInfo.m_idMenpai	= 0;
	StudyXinfaInfo.m_idXinfa	= nXinfaID;
	StudyXinfaInfo.m_NowLevel	= CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(nXinfaID)->m_nLevel;

	CGAskStudyXinfa msg;
	msg.SetUplevelInfoInfo(&StudyXinfaInfo);

	CNetManager::GetMe()->SendPacket( &msg );
	CGameProcedure::s_pGfxSystem->PushDebugString("CGAskStudyXinfa");
}
*/
//VOID StudyXinfa_SetStudyResult(GCStudyXinfa::_STUDERESULT_INFO studyResultInfo)
//{
//	//studyResultInfo.m_dwSpareExperience ;
//	//studyResultInfo.m_dwSpareMoney ;
//	//studyResultInfo.m_idXinfa ;
//	//studyResultInfo.m_StudedLevel ;
//}

