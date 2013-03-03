
#include "stdafx.h"
#include "TimerDoingManager.h"
#include "ScriptDef.h"
#include "GCChat.h"
#include "PacketFactoryManager.h"
#include "ChatPipe.h"
#include "Scene.h"

TimerDoingManager::TimerDoingManager()
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}


TimerDoingManager::~TimerDoingManager()
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}


BOOL TimerDoingManager::Init( )
{
__ENTER_FUNCTION
		
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID TimerDoingManager::CleanUp( )
{
__ENTER_FUNCTION
	
	m_pScene = NULL;
	m_ManagerTimer.CleanUp( ) ;

	for (INT i=0;i<MAX_TIMER_DOING_NUM;i++)
		m_LogicTimer[i].CleanUp( ) ;

	memset((char*)m_IsActive,0,sizeof(BOOL)*MAX_TIMER_DOING_NUM);
	memset((char*)m_IsLifetime,0,sizeof(BOOL)*MAX_TIMER_DOING_NUM);
	memset((char*)m_TimerParam,0,sizeof(INT)*MAX_TIMER_DOING_NUM * MAX_TIEMR_PARAM_NUM);
	memset((char*)m_ReportType,0,sizeof(BYTE)*MAX_TIMER_DOING_NUM);

__LEAVE_FUNCTION
}


//�ж�ϵͳʱ���Ƿ���ڣ���ʼʱ��--����ʱ����Ϊ��Ч
BOOL TimerDoingManager::CheckSystemTimerValidity(INT TblIndex)
{
__ENTER_FUNCTION

	return m_IsLifetime[TblIndex];

__LEAVE_FUNCTION
	return FALSE;
}

//ֹͣ��ʱ���¼�
VOID TimerDoingManager::StopTimerDoing(INT TblIndex)
{
	m_IsLifetime[TblIndex] = FALSE;
}

VOID TimerDoingManager::StartupTimerDoing(INT TblIndex)
{
__ENTER_FUNCTION

	m_IsLifetime[TblIndex] = TRUE;
	m_ReportType[TblIndex] = REPORT_TYPE_SLOW;

	if ( g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Ruler == -1 )
	{
		//��ʹ��MAP�ֻ����һ���ű�
		INT MapId = g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_TempMapId;

		if ( MapId == m_pScene->SceneID() )
		{
			m_pScene->GetLuaInterface()->ExeScript_DDDDDDD( 
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_ScriptId, 
			DEF_EVENT_ENTRY_FUNC_NAME, 
			MapId,
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Index,
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[0],
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[1],				
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[2],		
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[3],				
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[4]		
			);	
		}		
	}
	else if( g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Ruler == 0 )
	{//��������MAP
		for (int i=0;i<TIMER_DOING_MAP_NUM;i++)
		{
			INT MapId = g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_MapId[i];
			Assert(MapId<MAX_SCENE);

			if ( MapId == INVALID_ID)
				continue;
			if (  m_pScene->GetSceneType() != SCENE_TYPE_GAMELOGIC ) 
			{
				continue;
			}

			if ( MapId != m_pScene->SceneID() )
				continue;

			if ( g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_MapRatio[i] > 0 )
			{
				//������Ӧ�ű�
				m_pScene->GetLuaInterface()->ExeScript_DDDDDDD( 
				g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_ScriptId, 
				DEF_EVENT_ENTRY_FUNC_NAME, 
				MapId,
				g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Index,
				g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[0],
				g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[1],				
				g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[2],		
				g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[3],				
				g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[4]			
				) ;

			}
		}
	}
	else if ( g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Ruler > 0 )
	{//����m_Ruler��MAP

		for (int i = 0; i < g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Ruler; i++ )
		{	
			INT MapId = g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_MapId[i];
			Assert(MapId<MAX_SCENE);

			if ( MapId == INVALID_ID )
				continue;

			if ( m_pScene->GetSceneType() != SCENE_TYPE_GAMELOGIC ) 
			{
				continue;
			}

			if ( MapId != m_pScene->SceneID() )
				continue;

			//������Ӧ�ű�
			m_pScene->GetLuaInterface()->ExeScript_DDDDDDD( 
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_ScriptId, 
			DEF_EVENT_ENTRY_FUNC_NAME, 
			MapId,
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Index,
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[0],
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[1],				
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[2],		
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[3],				
			g_TiemrDoingAttrTbl.m_TableExt[TblIndex].m_Parameter[4]			
			) ;
				
		}//for end

	}

	__LEAVE_FUNCTION

}

//�ж϶�ʱ���Ƿ�ﵽ��ʼ�����ʱ��
BOOL TimerDoingManager::IsStartOrEndTime(INT TimeType,INT TblTime)
{
__ENTER_FUNCTION

	INT day  = 0;
	INT week = 0;

	//ȡ����λ�õ�Сʱ�ͷ���
	INT time = TblTime % 10000;
	
	INT	hour = time / 100;			//ȡǰ��λ�õ�Сʱ
	INT mimite = time % 100;		//ȡ���λ�õ�����

	switch(TimeType)
	{
	case 0:	//ÿ��ִ��һ��
		{
			//ȡǰ��λ�õ�����
			day = TblTime / 10000;
			if ( day == g_pTimeManager->GetDayTime() % 1000 )
			{
				if ( g_pTimeManager->GetHour() == hour && 
					g_pTimeManager->GetMinute() == mimite	)
				{
					return TRUE;
				}
			}
		}
		break;
	case 1:	//ÿ��ִ��һ��
		{
			if ( hour == g_pTimeManager->GetHour() && 
				g_pTimeManager->GetMinute() == mimite )
			{ 
				return TRUE;
			}
		}
		break;
	case 2:	//ÿ��ִ��һ��
		{
			//��ȡǰ��λ����ȡǰ��λ�е����һλ���õ����ڼ�
			//0=������;1=����һ
			INT temp = TblTime / 10000;
			week = temp % 10;
			if ( week == g_pTimeManager->GetWeek() )
			{
				if ( g_pTimeManager->GetHour() == hour && 
					g_pTimeManager->GetMinute() == mimite )
				{
					return TRUE;
				}				
			}
		}
		break;	
	case 3:	//ÿ��ִ��һ��
		{
			//��ȡǰ��λ����ȡǰ��λ�еĺ��λ���õ���������(0-31)
			INT temp = TblTime / 10000;
			day = temp % 100;		
			if ( day == g_pTimeManager->GetDay() )
			{
				if ( g_pTimeManager->GetHour() == hour && 
					g_pTimeManager->GetMinute() == mimite )
				{
					return TRUE;
				}					
			}
		}
		break;
	default:
		Assert(FALSE&&"��ʱ���ʱ��������ͳ���");
		break;
	}

	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL TimerDoingManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	if( !m_ManagerTimer.IsSetTimer() )
	{
		//����������ʱ��1000
		m_ManagerTimer.BeginTimer( 1000, uTime ) ;
		return TRUE ;
	}

	if( !m_ManagerTimer.CountingTimer(uTime) )
	{
		//ʱ��û��
		return TRUE ;
	}

	g_pTimeManager->SetTime();

	for (UINT TblCnt=0;TblCnt<g_TiemrDoingAttrTbl.m_Count;TblCnt++)
	{		
		BOOL	IsReport = FALSE;

		if ( m_IsLifetime[TblCnt] == TRUE )
		{
			if( m_LogicTimer[TblCnt].IsSetTimer() )
			{
				if( m_LogicTimer[TblCnt].CountingTimer(uTime) )
				{
					//ʱ�䵽
					IsReport = TRUE;
				}
			}

			//�������ļ�ʱ�������¼�
			switch( m_ReportType[TblCnt] )
			{
			case REPORT_TYPE_SLOW:		//����
				if ( IsReport )
				{
					//�򱾳���������ҹ㲥
					if( m_pScene->GetPlayerManager()->GetCount() > 0 )
					{
						for (INT j=0;j<CHAR_SHOW_POS_NUM;j++)
						{
							if ( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j] != -1 )
							{
								INT len = (INT)strlen(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportText);
								GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
								pMsg->SetChatShowPos(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j]);
								pMsg->SetChatType( CHAT_TYPE_SYSTEM ) ;

								// ���Ͽͻ��˽���ǰ׺������ǰ׺�������ò�ͬ�Ĺ���״̬ 20100712 BLL
								pMsg->SetContexSize( len + 10 );

								char strContext[ REPORT_TEXT_LEN + 10 ];

								memcpy( strContext, "@*;SrvMsg;", 10 );
								memcpy( strContext + 10, g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportText, len );

								pMsg->SetContex( strContext );				

								len = (INT)strlen( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName );
								pMsg->SetSourNameSize( len ) ;
								pMsg->SetSourName( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName ) ;

								m_pScene->GetChatPipe()->SendPacket( pMsg, INVALID_ID, INVALID_ID ) ;	
							}
						}					
					}
				}

				//��ⲥ���¼��Ƿ񵽽���ʱ��
				if ( IsStartOrEndTime(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_TimeType,
					g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportEndTime) 
					== TRUE )
				{
					m_ReportType[TblCnt] = REPORT_TYPE_QUICK;
					m_LogicTimer[TblCnt].BeginTimer(
						g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_QuickReportInterval,
						uTime);
				}
				break;
			case REPORT_TYPE_QUICK:		//�챨
				if ( IsReport )
				{
					//�򱾳���������ҹ㲥
					if( m_pScene->GetPlayerManager()->GetCount() > 0 )
					{
						for (INT j=0;j<CHAR_SHOW_POS_NUM;j++)
						{
							if ( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j] != -1 )
							{
								INT len = (INT)strlen(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_QuickReportText);
								GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
								pMsg->SetChatShowPos(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j]);
								pMsg->SetChatType( CHAT_TYPE_SYSTEM ) ;

								// ���Ͽͻ��˽���ǰ׺������ǰ׺�������ò�ͬ�Ĺ���״̬ 20100712 BLL
								pMsg->SetContexSize( len + 10 );

								char strContext[ REPORT_TEXT_LEN + 10 ];

								memcpy( strContext, "@*;SrvMsg;", 10 );
								memcpy( strContext + 10, g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportText, len );

								len = (INT)strlen( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName );
								pMsg->SetSourNameSize( len ) ;
								pMsg->SetSourName( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName ) ;

								m_pScene->GetChatPipe()->SendPacket( pMsg, INVALID_ID, INVALID_ID ) ;	
							}
						}					
					}	
				}

				//��ⲥ���¼��Ƿ񵽽���ʱ��
				if ( IsStartOrEndTime(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_TimeType,
					g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_QuickReportEndTime) 
					== TRUE )
				{
					m_ReportType[TblCnt] = REPORT_TYPE_EARLIER;
					m_LogicTimer[TblCnt].BeginTimer(
						g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_EarlierReportInterval,
						uTime);
				}
				break;
			case REPORT_TYPE_EARLIER:
				if ( IsReport )
				{
					//�򱾳���������ҹ㲥
					if( m_pScene->GetPlayerManager()->GetCount() > 0 )
					{
						for (INT j=0;j<CHAR_SHOW_POS_NUM;j++)
						{
							if ( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j] != -1 )
							{
								INT len = (INT)strlen(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_EarlierReportText);
								GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
								pMsg->SetChatShowPos(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j]);
								pMsg->SetChatType( CHAT_TYPE_SYSTEM ) ;

								// ���Ͽͻ��˽���ǰ׺������ǰ׺�������ò�ͬ�Ĺ���״̬ 20100712 BLL
								pMsg->SetContexSize( len + 10 );

								char strContext[ REPORT_TEXT_LEN + 10 ];

								memcpy( strContext, "@*;SrvMsg;", 10 );
								memcpy( strContext + 10, g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportText, len );

								len = (INT)strlen( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName );
								pMsg->SetSourNameSize( len ) ;
								pMsg->SetSourName( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName ) ;

								m_pScene->GetChatPipe()->SendPacket( pMsg, INVALID_ID, INVALID_ID ) ;	
							}
						}						
					}				
				}

				//��ⲥ���¼��Ƿ񵽽���ʱ��
				if ( IsStartOrEndTime(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_TimeType,
					g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_EarlierReportEndTime) 
					== TRUE )
				{
					m_ReportType[TblCnt] = REPORT_TYPE_MID;
					m_LogicTimer[TblCnt].BeginTimer(
						g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_MidReportInterval,
						uTime);
				}
				break;
			case REPORT_TYPE_MID:
				if ( IsReport )
				{
					//�򱾳���������ҹ㲥
					if( m_pScene->GetPlayerManager()->GetCount() > 0 )
					{
						for (INT j=0;j<CHAR_SHOW_POS_NUM;j++)
						{
							if ( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j] != -1 )
							{
								INT len = (INT)strlen(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_MidReportText);
								GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
								pMsg->SetChatShowPos(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j]);
								pMsg->SetChatType( CHAT_TYPE_SYSTEM ) ;

								// ���Ͽͻ��˽���ǰ׺������ǰ׺�������ò�ͬ�Ĺ���״̬ 20100712 BLL
								pMsg->SetContexSize( len + 10 );

								char strContext[ REPORT_TEXT_LEN + 10 ];

								memcpy( strContext, "@*;SrvMsg;", 10 );
								memcpy( strContext + 10, g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportText, len );

								len = (INT)strlen( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName );
								pMsg->SetSourNameSize( len ) ;
								pMsg->SetSourName( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName ) ;

								m_pScene->GetChatPipe()->SendPacket( pMsg, INVALID_ID, INVALID_ID ) ;	
							}
						}					
					}				
				}

				//��ⲥ���¼��Ƿ񵽽���ʱ��
				if ( IsStartOrEndTime(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_TimeType,
					g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_MidReportEndTime) 
					== TRUE )
				{
					m_ReportType[TblCnt] = REPORT_TYPE_LAST;
					m_LogicTimer[TblCnt].BeginTimer(
						g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_LastReportInterval,
						uTime);
				}
				break;
			case REPORT_TYPE_LAST:
				if ( IsReport )
				{
					//��Server��������ҹ㲥
					if( m_pScene->GetPlayerManager()->GetCount() > 0 )
					{
						for (INT j=0;j<CHAR_SHOW_POS_NUM;j++)
						{
							if ( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j] != -1 )
							{
								INT len = (INT)strlen(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_LastReportText);
								GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
								pMsg->SetChatShowPos(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_BroadcastType[j]);
								pMsg->SetChatType( CHAT_TYPE_SYSTEM ) ;

								// ���Ͽͻ��˽���ǰ׺������ǰ׺�������ò�ͬ�Ĺ���״̬ 20100712 BLL
								pMsg->SetContexSize( len + 10 );

								char strContext[ REPORT_TEXT_LEN + 10 ];

								memcpy( strContext, "@*;SrvMsg;", 10 );
								memcpy( strContext + 10, g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportText, len );

								len = (INT)strlen( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName );
								pMsg->SetSourNameSize( len ) ;
								pMsg->SetSourName( g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_szName ) ;

								m_pScene->GetChatPipe()->SendPacket( pMsg, INVALID_ID, INVALID_ID ) ;	
							}
						}						
					}

				}

				//��ⲥ���¼��Ƿ񵽽���ʱ��
				if ( IsStartOrEndTime(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_TimeType,
					g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_LastReportEndTime) 
					== TRUE )
				{
					m_ReportType[TblCnt] = REPORT_TYPE_INVALID;
					m_LogicTimer[TblCnt].CleanUp();
				}
				break;
			default:
				break;
			}

			//����������ļ�ʱ���Ƿ񵽴����ʱ��
			if ( IsStartOrEndTime(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_TimeType,
				 g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_EndTime) 
				 == TRUE )
			{
				StopTimerDoing(TblCnt);
			}
		}
		else
		{//�ж�ϵͳʱ���Ƿ�ﵽ��ʼʱ���
			if ( IsStartOrEndTime(g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_TimeType,
				g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_StartTime) 
				 == TRUE )
			{
				StartupTimerDoing(TblCnt);
				m_LogicTimer[TblCnt].BeginTimer(
					g_TiemrDoingAttrTbl.m_TableExt[TblCnt].m_SlowReportInterval,
					uTime);
			}
		}//if ( m_IsLifetime[TblCnt] == TRUE )

	}//for end

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}






