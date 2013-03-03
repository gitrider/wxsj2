
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Event/GMEventSystem.h"

#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool/GMUIDataPool.h"

#include "DataPool\GMDataPool.h"
#include "DataPool\DetailAttrib_Player.h"


namespace SCRIPT_SANDBOX
{

	//===============================================================
	DaytipsData DaytipsData::s_DaytipsData;
	LuaPlus::LuaObject* DaytipsData::s_pMetaTable = NULL;

	DaytipsData::DaytipsData() 
	{ 
	}

	DaytipsData::~DaytipsData()
	{
		ResetDaytipSet();
	}

	VOID DaytipsData::ResetDaytipSet()
	{
		if ( m_fixedMission.size() > 0 )
		{
			m_fixedMission.clear();
		}
	}
	
	INT DaytipsData::FillCycleMissionList(LuaPlus::LuaState* state)
	{
		INT id = 0;
		CCharacterData* pData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();
		for( int i = 0; i < (int)pAttr->m_Count; i++ )
		{
			if( CommonCheck( &pAttr->m_TableExt[i], pData ) )
			{
				// ��UI�����¼�
				CHAR szTemp[32];
				std::vector< STRING > vParam;
				_snprintf(szTemp, 32, "0+%d+0", id);	// 0��ʾѭ������	
				vParam.push_back( szTemp );		
				vParam.push_back( pAttr->m_TableExt[i].m_MisName );
				_snprintf(szTemp, 32, "%d", pAttr->m_TableExt[i].m_MisCount);
				vParam.push_back( szTemp );
				vParam.push_back( pAttr->m_TableExt[i].m_MisText);
				CHAR szSceneNpc[42];
				_snprintf(szSceneNpc, 42, "%s+%s", pAttr->m_TableExt[i].m_SceneName, pAttr->m_TableExt[i].m_NpcName);
				vParam.push_back( szSceneNpc);

				CEventSystem::GetMe()->PushEvent(GE_UPDATE_DAY_TIPS, vParam);

				id++;
			}
		}
	
		return 0;
	}


	//  ���ݵ�ǰʱ�����ÿ�ջ�б�     20100716 BLL
	INT DaytipsData::FillFixedMissionList(LuaPlus::LuaState* state)
	{
		DayTipSet::iterator iter;

		time_t  clientTime;
		time(&clientTime);
		tm* etm = localtime(&clientTime);	

		InitializeMissionList(); 

		for ( iter = m_fixedMission.begin(); iter != m_fixedMission.end(); iter++ )
		{
			_DBC_DAY_TIPS* pTip = iter->m_pDayTipNode;

			// ���ű���Ŷ���

			int value = -1;
			int ret = TimeSegmentValue( &pTip->m_VailidTimeSlice, etm, &value );

			// ��ǰʱ���ڻʱ����
			if( ret == 0 )
			{	
				iter->m_nTimeType = 1;
			}
			// ��ǰʱ���ڻʱ���ǰ 
			else if( ret == 1 )
			{
				iter->m_nTimeType = 2;
			}
			// ��Ѿ�����
			else if ( ret == -1 )
			{
				iter->m_nTimeType = 3;

			}//if( ret == 0 )
			else // ����δ֪
				iter->m_nTimeType = -1;

		}//for ( iter = m_fixedMission.begin(); iter != m_fixedMission.end(); iter++ )

		return 0;
	}

	INT DaytipsData::RecommendMission(LuaPlus::LuaState* state)
	{
		time_t  clientTime;
		time(&clientTime);
		tm* etm = localtime(&clientTime);	
		int wday = etm->tm_wday;
		int mday = etm->tm_mday;

		// ��λ�ȡ�������ڵ����Ǳ��µĵڼ��ܣ�������������ϱ��µ�һ��ȱ�ٵ���������7
		etm->tm_mday = 1;
		time_t needTime = mktime( etm );
		tm* needTm = localtime(&needTime);

		// ȱ�ٵ�����
		int lackWeek = (needTm->tm_wday == 0) ? 7 : needTm->tm_wday;
		// ���µĵڼ���
		int howWeek = int( ( mday + (lackWeek - 1) ) / 7 );	

		CCharacterData* pData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();
		for( int i = 0; i < (int)pAttr->m_Count; i++ )
		{
			if( CommonCheck( &pAttr->m_TableExt[i], pData ) )
			{
				// ���Ƽ�
				if( pAttr->m_TableExt[i].m_MonthNominate != 1 )
					continue;

				// m_WeekOrDay=0��������������
				if( pAttr->m_TableExt[i].m_WeekOrDay == 0 )
				{
					if( pAttr->m_TableExt[i].m_MisValidWeek[howWeek][wday] != 1 )
						continue;
				}
				// m_WeekOrDay=1��������������
				else if( pAttr->m_TableExt[i].m_WeekOrDay == 1 )
				{
					if( pAttr->m_TableExt[i].m_MisValidDate[mday] != 1 )
						continue;
				}

				// ��UI�����¼�
				CHAR szTemp[32];
				std::vector< STRING > vParam;
				_snprintf(szTemp, 32, "%d", wday + howWeek * 7);
				vParam.push_back( szTemp );
				vParam.push_back( pAttr->m_TableExt[i].m_MisName );
				vParam.push_back( pAttr->m_TableExt[i].m_MisText );
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_CALENDAR, vParam);
			}
		}
		
		return 0;
	}

	INT DaytipsData::GetWeek(LuaPlus::LuaState* state)
	{
		time_t  clientTime;
		time(&clientTime);

		tm* etm = localtime(&clientTime);
		etm->tm_mday = 1;
		time_t needTime = mktime( etm );
		tm* needTm = localtime(&needTime);

		// ���µĵ�һ�������ڼ� needTm->tm_wday
		state->PushInteger( needTm->tm_wday );

		int days = 0; 
		switch( etm->tm_mon )
		{
		case 0:		// 1�´�
		case 2:		// 3�´�
		case 4:		// 5�´�
		case 6:		// 7�´�
		case 7:		// 8�´�
		case 9:		// 10�´�
		case 11:	// 12�´�
			days = 31;
			break;

		case 1:		// 2��С
			{
				days = 28;
				int year = etm->tm_year + 1900;
				if( year % 400 == 0 || (year % 4 == 0 && year % 100 != 0) )
					days++;	// ���� 29��
			}
			break;		

		case 3:		// 4��С
		case 5:		// 6��С
		case 8:		// 9��С
		case 10:	// 11��С	
			days = 30;
			break;
		}
		
		// �����ж����� days
		state->PushInteger( days );

		return 2;
	}

	// ����һʱ����ʱ��εĹ�ϵ
	INT	DaytipsData::TimeSegmentValue(const VALID_TIME_SLICE* pSlice, const tm* pCheck, int* pValue)
	{
		int seg1 = pSlice->m_Begin.tm_hour * 60 + pSlice->m_Begin.tm_min;
		int seg2 = pSlice->m_End.tm_hour * 60 + pSlice->m_End.tm_min;
		int check = pCheck->tm_hour * 60 + pCheck->tm_min;

		if( seg1 <= check && check < seg2 )
		{
			*pValue = 0;
			return 0;	// ��������
		}
	
		if( seg1 > check )
		{
			*pValue = seg1 - check;
			return 1;	// ������ǰ
		}

		if( seg2 < check )
		{
			*pValue = check - seg2;
			return -1;	// �������
		}

		return 2;		// δ֪
	}

	// �����������
	BOOL DaytipsData::CommonCheck(const _DBC_DAY_TIPS* pAttr, const CCharacterData* pData)
	{
		//// ѭ������
		//if( pAttr->m_MisType != 0 )
		//	return FALSE;

		//�Ƿ�ȫ���ɼ�	0Ϊ��1Ϊ�����˿ɼ�
		if( pAttr->m_ShowAll == 1 )
			return TRUE;

		//�ɼ�����:�ȼ�
		int nLevel = pData->Get_Level();
		if( nLevel >= 0 )
		{
			if( pAttr->m_ShowLevel[nLevel] != 1 )
				return FALSE;
		}

		//�ɼ�����:����
		int nCountry = pData->Get_Country();
		if( nCountry >= 0 )
		{
			if( pAttr->m_ShowCountry[nCountry] != 1 )
				return FALSE;
		}

		//�ɼ�����:���
		int nGuild = pData->Get_Guild();
		if( nGuild >= 0 )
		{
			if( pAttr->m_ShowGuild[nGuild] != 1 )
				return FALSE;
		}

		//�ɼ�����:����
		int nFamily = pData->Get_MenPai();
		if( nFamily >= 0 )
		{
			if( pAttr->m_ShowFamily[nFamily] != 1 )
				return FALSE;
		}

		return TRUE;
	}

	// �Ƿ��ǽ��տɼ��    20100716 BLL
	BOOL DaytipsData::IsTodayMission( const _DBC_DAY_TIPS* pDayTip )
	{
		time_t  clientTime;
		time( &clientTime );

		tm* etm = localtime( &clientTime );

		// �����������Ч�·�
		if ( pDayTip->m_MisValidMonth[ etm->tm_mon ] == 1 )
		{
			// �������������������
			if ( pDayTip->m_WeekOrDay == 0 )
			{
				int nWeek = GetWeekNO();

				// ��Ч����
				if ( pDayTip->m_MisValidWeek[ nWeek ][ etm->tm_wday ] == 0 )
				{
					return FALSE;
				}
			}
			// �������������������
			else
			{
				if ( pDayTip->m_MisValidDate[ etm->tm_mday ] == 0 )
				{
					return FALSE;
				}

			}//if ( pDayTip->m_WeekOrDay == 0 )
		}
		else
		{
			return FALSE;

		}//if ( pDayTip->m_MisValidMonth[ etm->tm_mon ] == 1 )

		return TRUE;
	}

	// �����Ǳ��µڼ���    20100716 BLL
	INT	 DaytipsData::GetWeekNO()
	{
		time_t  clientTime;
		time( &clientTime );

		tm* etm = localtime( &clientTime );

		int weekDay  = etm->tm_wday;	// �������ڼ�
		int monthDay = etm->tm_mday;	// ���ռ���

		int nDayBefore;		// ����֮ǰ������

		if ( weekDay == 0 ) // ������
		{
			nDayBefore = monthDay - 7;
		}
		else
			nDayBefore = monthDay - weekDay;

		int nRemainder = nDayBefore % 7; 

		if ( nRemainder == 0 ) // �������֮ǰ��������
		{
			return nDayBefore / 7 + 1; // ֮ǰ�������ܼ��ϱ���
		}
		else
		{
			return nDayBefore / 7 + 2; // ֮ǰ�������ܼ��ϱ��ܼ���һ��
		}
	}

	// ��ȡ�Ѳμ�һ���Ĵ���    20100728 BLL
	INT  DaytipsData::GetMissionDoneCount( INT nMissionID )
	{	
		INT nCount = 0;
		nCount = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMissionDoneCount( nMissionID );
		return nCount;
	}

	// ��ȡһ�����¼    20100716 BLL
	INT DaytipsData::GetMissionByIndex(LuaPlus::LuaState* state )	 
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow( "LUA:DaytipsData::GetMissionByIndex param[2] error" );
		}

		INT nIndex = args[2].GetInteger();

		if ( nIndex < 0 || nIndex > m_fixedMission.size() || m_fixedMission.size() == 0 )
		{
			state->PushInteger( -1 );
			return 1;
		}

		BOOL bFilter = FALSE;	// �Ƿ���˵���Ҳ����������ļ�¼
		INT  nCount  = 0;		// ��ȡ�ڼ������������ļ�¼

		if ( !args[3].IsBoolean() )
		{
			KLThrow( "LUA:DaytipsData::GetMissionByIndex param[3] error" );
		}
	
		bFilter = args[3].GetBoolean();		

		DayTipSet::iterator iter;

		CCharacterData* pData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		for ( iter = m_fixedMission.begin(); iter != m_fixedMission.end(); iter++ )
		{
			if ( bFilter == TRUE )	
			{
				// ��鹫������
				if( CommonCheck( iter->m_pDayTipNode, pData ) == TRUE )
				{
					nCount++;
				}
			}
			else
			{			
				nCount++;

			}//if ( bFilter == TRUE )	

			if ( nCount == nIndex )
			{
				state->PushInteger( iter->m_pDayTipNode->m_MisType ); // �����
				state->PushString( iter->m_pDayTipNode->m_MisName );  // �����

				// �ʱ��
				CHAR szTemp[32];

				tm tmBegin = iter->m_pDayTipNode->m_VailidTimeSlice.m_Begin;
				tm tmEnd   = iter->m_pDayTipNode->m_VailidTimeSlice.m_End;

				_snprintf(szTemp, 32, "%0.2d:%0.2d-%0.2d:%0.2d", tmBegin.tm_hour, tmBegin.tm_min, tmEnd.tm_hour, tmEnd.tm_min );
				state->PushString( szTemp );				

				state->PushInteger( iter->m_pDayTipNode->m_MisCount );	// �ɲμӴ���

				// �ѲμӴ���
				INT		nDoneCount = 0;
				nDoneCount = GetMissionDoneCount( iter->m_pDayTipNode->m_MisPos );
				state->PushInteger( nDoneCount );

				state->PushString( iter->m_pDayTipNode->m_MisText );	// �����
				state->PushString( iter->m_pDayTipNode->m_NpcName );	// ����NPC
				state->PushInteger( iter->m_pDayTipNode->m_PosX );		// NPC����
				state->PushInteger( iter->m_pDayTipNode->m_PosZ );		// NPC����

				state->PushString( iter->m_pDayTipNode->m_SceneName );	// ���񳡾�
				state->PushInteger( iter->m_pDayTipNode->m_ScendId );	// ���񳡾�ID

				state->PushInteger( iter->m_nTimeType );				// �״̬

				return 12;

			}//if ( nCount == nIndex )
		
		}//for ( iter = m_fixedMission.begin(); iter != m_fixedMission.end(); iter++ )

		state->PushInteger( -1 );
		return 1;
	}

	// ��ȡ���ջ����   20100716 BLL
	INT DaytipsData::GetMissionCount( LuaPlus::LuaState* state )	
	{
		LuaStack args(state);

		if ( !args[2].IsBoolean() )
		{
			KLThrow( "LUA:DaytipsData::GetMissionCount param[2] error" );
		}

		BOOL bFilter = args[2].GetBoolean();

		INT nCount = 0;

		if ( bFilter == TRUE )
		{
			DayTipSet::iterator iter;

			CCharacterData* pData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

			for ( iter = m_fixedMission.begin(); iter != m_fixedMission.end(); iter++ )
			{	

				// ��鹫������
				if( CommonCheck( iter->m_pDayTipNode, pData ) == TRUE )
				{
					nCount++;
				}
			}
		}	
		else
			nCount = m_fixedMission.size();		

		state->PushInteger( nCount );

		return 1;
	}

	// ���ؽ��ջ�б�	 20100719 BLL
	VOID   DaytipsData::InitializeMissionList()
	{
		time_t  clientTime;
		time(&clientTime);
		tm* etm = localtime(&clientTime);	

		_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();

		DayTipsNode dayNode;

		ResetDaytipSet();		

		// 
		// ��ʼȡ�����ջ����
		// 

		if ( pAttr != NULL )
		{
			for( int i = 0; i < (int)pAttr->m_Count; i++ )
			{
				_DBC_DAY_TIPS* pTip = &pAttr->m_TableExt[i];

				// �ҵ�����������յ�
				if ( IsTodayMission( pTip ) == TRUE )
				{	
					dayNode.m_pDayTipNode = pTip;

					m_fixedMission.insert( dayNode );

				}//if ( IsTodayMission( pTip ) == TRUE )

			}//for( int i = 0; i < (int)pAttr->m_Count; i++ )
		}		
	}

	// �Ƿ��ǻ���� 20100723 BLL
	VOID   DaytipsData::IsDayTipsMission( INT nMisID, BOOL& IsDayTipMission, BOOL& bTodayCanSee )
	{
		_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();

		for( int i = 0; i < (int)pAttr->m_Count; i++ )
		{
			_DBC_DAY_TIPS* pTip = &pAttr->m_TableExt[i];

			if ( pTip->m_MisPos == nMisID )
			{
				IsDayTipMission = TRUE;
				bTodayCanSee = IsTodayMission( pTip );

				return;
			}
		}

		IsDayTipMission = FALSE;
		bTodayCanSee    = FALSE;
	}

	// ��ȡһ���ĿɽӴ���    20100728 BLL
	INT	    DaytipsData::GetMissionCanDoCount( INT nMissionID )
	{
		_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();

		for( int i = 0; i < (int)pAttr->m_Count; i++ )
		{
			_DBC_DAY_TIPS* pTip = &pAttr->m_TableExt[i];

			if ( pTip->m_MisPos == nMissionID )
			{
				return pTip->m_MisCount;
			}
		}

		return 0;
	}
};