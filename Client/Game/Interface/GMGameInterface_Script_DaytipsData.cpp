
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
				// 给UI发送事件
				CHAR szTemp[32];
				std::vector< STRING > vParam;
				_snprintf(szTemp, 32, "0+%d+0", id);	// 0表示循环任务	
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


	//  根据当前时间更新每日活动列表     20100716 BLL
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

			// 分门别类放队列

			int value = -1;
			int ret = TimeSegmentValue( &pTip->m_VailidTimeSlice, etm, &value );

			// 当前时间在活动时间里
			if( ret == 0 )
			{	
				iter->m_nTimeType = 1;
			}
			// 当前时间在活动时间段前 
			else if( ret == 1 )
			{
				iter->m_nTimeType = 2;
			}
			// 活动已经结束
			else if ( ret == -1 )
			{
				iter->m_nTimeType = 3;

			}//if( ret == 0 )
			else // 类型未知
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

		// 如何获取今天所在的周是本月的第几周：今天的日数加上本月第一周缺少的天数除以7
		etm->tm_mday = 1;
		time_t needTime = mktime( etm );
		tm* needTm = localtime(&needTime);

		// 缺少的天数
		int lackWeek = (needTm->tm_wday == 0) ? 7 : needTm->tm_wday;
		// 本月的第几周
		int howWeek = int( ( mday + (lackWeek - 1) ) / 7 );	

		CCharacterData* pData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();
		for( int i = 0; i < (int)pAttr->m_Count; i++ )
		{
			if( CommonCheck( &pAttr->m_TableExt[i], pData ) )
			{
				// 月推荐
				if( pAttr->m_TableExt[i].m_MonthNominate != 1 )
					continue;

				// m_WeekOrDay=0任务以星期来做
				if( pAttr->m_TableExt[i].m_WeekOrDay == 0 )
				{
					if( pAttr->m_TableExt[i].m_MisValidWeek[howWeek][wday] != 1 )
						continue;
				}
				// m_WeekOrDay=1任务以日期来做
				else if( pAttr->m_TableExt[i].m_WeekOrDay == 1 )
				{
					if( pAttr->m_TableExt[i].m_MisValidDate[mday] != 1 )
						continue;
				}

				// 给UI发送事件
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

		// 当月的第一天是星期几 needTm->tm_wday
		state->PushInteger( needTm->tm_wday );

		int days = 0; 
		switch( etm->tm_mon )
		{
		case 0:		// 1月大
		case 2:		// 3月大
		case 4:		// 5月大
		case 6:		// 7月大
		case 7:		// 8月大
		case 9:		// 10月大
		case 11:	// 12月大
			days = 31;
			break;

		case 1:		// 2月小
			{
				days = 28;
				int year = etm->tm_year + 1900;
				if( year % 400 == 0 || (year % 4 == 0 && year % 100 != 0) )
					days++;	// 闰年 29天
			}
			break;		

		case 3:		// 4月小
		case 5:		// 6月小
		case 8:		// 9月小
		case 10:	// 11月小	
			days = 30;
			break;
		}
		
		// 当月有多少天 days
		state->PushInteger( days );

		return 2;
	}

	// 计算一时间点和时间段的关系
	INT	DaytipsData::TimeSegmentValue(const VALID_TIME_SLICE* pSlice, const tm* pCheck, int* pValue)
	{
		int seg1 = pSlice->m_Begin.tm_hour * 60 + pSlice->m_Begin.tm_min;
		int seg2 = pSlice->m_End.tm_hour * 60 + pSlice->m_End.tm_min;
		int check = pCheck->tm_hour * 60 + pCheck->tm_min;

		if( seg1 <= check && check < seg2 )
		{
			*pValue = 0;
			return 0;	// 在区间内
		}
	
		if( seg1 > check )
		{
			*pValue = seg1 - check;
			return 1;	// 在区间前
		}

		if( seg2 < check )
		{
			*pValue = check - seg2;
			return -1;	// 在区间后
		}

		return 2;		// 未知
	}

	// 公共条件检测
	BOOL DaytipsData::CommonCheck(const _DBC_DAY_TIPS* pAttr, const CCharacterData* pData)
	{
		//// 循环任务
		//if( pAttr->m_MisType != 0 )
		//	return FALSE;

		//是否全部可见	0为否，1为所有人可见
		if( pAttr->m_ShowAll == 1 )
			return TRUE;

		//可见条件:等级
		int nLevel = pData->Get_Level();
		if( nLevel >= 0 )
		{
			if( pAttr->m_ShowLevel[nLevel] != 1 )
				return FALSE;
		}

		//可见条件:国家
		int nCountry = pData->Get_Country();
		if( nCountry >= 0 )
		{
			if( pAttr->m_ShowCountry[nCountry] != 1 )
				return FALSE;
		}

		//可见条件:帮会
		int nGuild = pData->Get_Guild();
		if( nGuild >= 0 )
		{
			if( pAttr->m_ShowGuild[nGuild] != 1 )
				return FALSE;
		}

		//可见条件:家族
		int nFamily = pData->Get_MenPai();
		if( nFamily >= 0 )
		{
			if( pAttr->m_ShowFamily[nFamily] != 1 )
				return FALSE;
		}

		return TRUE;
	}

	// 是否是今日可见活动    20100716 BLL
	BOOL DaytipsData::IsTodayMission( const _DBC_DAY_TIPS* pDayTip )
	{
		time_t  clientTime;
		time( &clientTime );

		tm* etm = localtime( &clientTime );

		// 如果当月是有效月份
		if ( pDayTip->m_MisValidMonth[ etm->tm_mon ] == 1 )
		{
			// 如果任务是以星期来做
			if ( pDayTip->m_WeekOrDay == 0 )
			{
				int nWeek = GetWeekNO();

				// 有效星期
				if ( pDayTip->m_MisValidWeek[ nWeek ][ etm->tm_wday ] == 0 )
				{
					return FALSE;
				}
			}
			// 如果任务是以日期来做
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

	// 本周是本月第几周    20100716 BLL
	INT	 DaytipsData::GetWeekNO()
	{
		time_t  clientTime;
		time( &clientTime );

		tm* etm = localtime( &clientTime );

		int weekDay  = etm->tm_wday;	// 今日星期几
		int monthDay = etm->tm_mday;	// 今日几号

		int nDayBefore;		// 本周之前的天数

		if ( weekDay == 0 ) // 星期日
		{
			nDayBefore = monthDay - 7;
		}
		else
			nDayBefore = monthDay - weekDay;

		int nRemainder = nDayBefore % 7; 

		if ( nRemainder == 0 ) // 如果本周之前是整数周
		{
			return nDayBefore / 7 + 1; // 之前的整数周加上本周
		}
		else
		{
			return nDayBefore / 7 + 2; // 之前的整数周加上本周及第一周
		}
	}

	// 获取已参加一项活动的次数    20100728 BLL
	INT  DaytipsData::GetMissionDoneCount( INT nMissionID )
	{	
		INT nCount = 0;
		nCount = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMissionDoneCount( nMissionID );
		return nCount;
	}

	// 获取一条活动记录    20100716 BLL
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

		BOOL bFilter = FALSE;	// 是否过滤掉玩家不满足条件的记录
		INT  nCount  = 0;		// 获取第几个满足条件的记录

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
				// 检查公共条件
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
				state->PushInteger( iter->m_pDayTipNode->m_MisType ); // 活动类型
				state->PushString( iter->m_pDayTipNode->m_MisName );  // 活动名称

				// 活动时间
				CHAR szTemp[32];

				tm tmBegin = iter->m_pDayTipNode->m_VailidTimeSlice.m_Begin;
				tm tmEnd   = iter->m_pDayTipNode->m_VailidTimeSlice.m_End;

				_snprintf(szTemp, 32, "%0.2d:%0.2d-%0.2d:%0.2d", tmBegin.tm_hour, tmBegin.tm_min, tmEnd.tm_hour, tmEnd.tm_min );
				state->PushString( szTemp );				

				state->PushInteger( iter->m_pDayTipNode->m_MisCount );	// 可参加次数

				// 已参加次数
				INT		nDoneCount = 0;
				nDoneCount = GetMissionDoneCount( iter->m_pDayTipNode->m_MisPos );
				state->PushInteger( nDoneCount );

				state->PushString( iter->m_pDayTipNode->m_MisText );	// 活动介绍
				state->PushString( iter->m_pDayTipNode->m_NpcName );	// 任务NPC
				state->PushInteger( iter->m_pDayTipNode->m_PosX );		// NPC坐标
				state->PushInteger( iter->m_pDayTipNode->m_PosZ );		// NPC坐标

				state->PushString( iter->m_pDayTipNode->m_SceneName );	// 任务场景
				state->PushInteger( iter->m_pDayTipNode->m_ScendId );	// 任务场景ID

				state->PushInteger( iter->m_nTimeType );				// 活动状态

				return 12;

			}//if ( nCount == nIndex )
		
		}//for ( iter = m_fixedMission.begin(); iter != m_fixedMission.end(); iter++ )

		state->PushInteger( -1 );
		return 1;
	}

	// 获取今日活动个数   20100716 BLL
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

				// 检查公共条件
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

	// 加载今日活动列表	 20100719 BLL
	VOID   DaytipsData::InitializeMissionList()
	{
		time_t  clientTime;
		time(&clientTime);
		tm* etm = localtime(&clientTime);	

		_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();

		DayTipsNode dayNode;

		ResetDaytipSet();		

		// 
		// 开始取出今日活动数据
		// 

		if ( pAttr != NULL )
		{
			for( int i = 0; i < (int)pAttr->m_Count; i++ )
			{
				_DBC_DAY_TIPS* pTip = &pAttr->m_TableExt[i];

				// 找到日期满足今日的
				if ( IsTodayMission( pTip ) == TRUE )
				{	
					dayNode.m_pDayTipNode = pTip;

					m_fixedMission.insert( dayNode );

				}//if ( IsTodayMission( pTip ) == TRUE )

			}//for( int i = 0; i < (int)pAttr->m_Count; i++ )
		}		
	}

	// 是否是活动任务 20100723 BLL
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

	// 获取一项活动的可接次数    20100728 BLL
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