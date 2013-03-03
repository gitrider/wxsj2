#include "stdafx.h"
#include "DayTips.h"
#include "Obj_Human.h"

DayTips* DayTips::s_dayTip = NULL;
DayTips::DayTips(void)
{
	s_dayTip = this;
}

DayTips::~DayTips(void)
{
}

// 是否在活动时间内
BOOL DayTips::IsMissionInTime( INT nMissionID )
{
	INT nCount = 0;
	VALID_TIME_SLICE misTime;

	nCount = g_DayTipsAttrTbl.Count();

	for ( INT i = 0; i < nCount; i++ )
	{
		if ( g_DayTipsAttrTbl.m_TableExt[i].m_MisPos == nMissionID )
		{
			if ( IsTodayMission( nMissionID ) == TRUE )
			{
				INT nValue = 0;
				if ( IsInTimeSegment( g_DayTipsAttrTbl.m_TableExt[i].m_VailidTimeSlice, g_pTimeManager->GetFormatTime(), &nValue ) == 0 )
				{
					return TRUE;
				}				
			}

			return FALSE;
		}
	}

	return FALSE;
}


// 是否是今日活动
BOOL DayTips::IsTodayMission( INT nMissionID )
{
	if ( nMissionID <= 0 )
	{
		return FALSE;
	}
	
	DAY_TIPS_ATTR dayTipsAttr;

	BOOL    bFind = FALSE;
	INT		nCount = 0;

	tm etm = g_pTimeManager->GetFormatTime();

	nCount = g_DayTipsAttrTbl.Count();

	for ( INT i = 0; i < nCount; i++ )
	{
		if ( g_DayTipsAttrTbl.m_TableExt[i].m_MisPos == nMissionID )
		{
			dayTipsAttr = g_DayTipsAttrTbl.m_TableExt[i];
			bFind = TRUE;
			break;
		}
	}

	if ( bFind == TRUE )
	{
		// 如果当月是有效月份
		if ( dayTipsAttr.m_MisValidMonth[ etm.tm_mon ] == 1 )
		{
			// 如果任务是以星期来做
			if ( dayTipsAttr.m_WeekOrDay == 0 )
			{
				int nWeek = GetWeekNO();

				// 有效星期
				if ( dayTipsAttr.m_MisValidWeek[ nWeek ][ etm.tm_wday ] == 0 )
				{
					return FALSE;
				}
			}
			// 如果任务是以日期来做
			else
			{
				if ( dayTipsAttr.m_MisValidDate[ etm.tm_mday ] == 0 )
				{
					return FALSE;
				}

			}//if ( dayTipsAttr.m_WeekOrDay == 0 )
		}
		else
		{
			return FALSE;

		}//if ( dayTipsAttr.m_MisValidMonth[ etm.tm_mon ] == 1 )

		return TRUE;
	}

	return FALSE;
}

// 本周是本月第几周    
INT	 DayTips::GetWeekNO()
{
	tm etm = g_pTimeManager->GetFormatTime();

	int weekDay  = etm.tm_wday;	// 今日星期几
	int monthDay = etm.tm_mday;	// 今日几号

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

// 获取一项活动的可接次数 
INT  DayTips::GetMissionCount( INT nMisID )
{
	INT nCount = 0;

	nCount = g_DayTipsAttrTbl.Count();

	for ( INT i = 0; i < nCount; i++ )
	{
		if ( g_DayTipsAttrTbl.m_TableExt[i].m_MisPos == nMisID )
		{
			return g_DayTipsAttrTbl.m_TableExt[i].m_MisCount;
		}
	}

	return 0;
}

// 判断时间是否在时间段内
INT	DayTips::IsInTimeSegment( const VALID_TIME_SLICE pSlice, const tm& pCheck, int* pValue )
{
	int seg1 = pSlice.m_Begin.tm_hour * 60 + pSlice.m_Begin.tm_min;
	int seg2 = pSlice.m_End.tm_hour * 60 + pSlice.m_End.tm_min;
	int check = pCheck.tm_hour * 60 + pCheck.tm_min;

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
BOOL DayTips::CommonCheck( INT nMissionID, Obj_Human* pHuman )
{
	DAY_TIPS_ATTR dayTipsAttr;

	INT nCount = 0;

	nCount = g_DayTipsAttrTbl.Count();

	for ( INT i = 0; i < nCount; i++ )
	{
		if ( g_DayTipsAttrTbl.m_TableExt[i].m_MisPos == nMissionID )
		{
			dayTipsAttr =  g_DayTipsAttrTbl.m_TableExt[i];
		}
	}

	//是否全部可见	0为否，1为所有人可见
	if( dayTipsAttr.m_ShowAll == 1 )
		return TRUE;

	//可见条件:等级
	int nLevel = pHuman->GetLevel();
	if( nLevel >= 0 )
	{
		if( dayTipsAttr.m_ShowLevel[nLevel] != 1 )
			return FALSE;
	}

	//可见条件:国家
	int nCountry = pHuman->GetCountry();
	if( nCountry >= 0 )
	{
		if( dayTipsAttr.m_ShowCountry[nCountry] != 1 )
			return FALSE;
	}

	//可见条件:帮会
	int nGuild = pHuman->GetGuildID();
	if( nGuild >= 0 )
	{
		if( dayTipsAttr.m_ShowGuild[nGuild] != 1 )
			return FALSE;
	}

	//可见条件:家族
	int nFamily = pHuman->GetMenPai();
	if( nFamily >= 0 )
	{
		if( dayTipsAttr.m_ShowFamily[nFamily] != 1 )
			return FALSE;
	}

	return TRUE;
}

// 活动最后一次完成日期是否是今天
INT  DayTips::LastIsTodayRecord( INT time )
{
	INT timeToday;

	timeToday = GetTodayFormatDate();

	if ( timeToday == time )
	{
		return 1;
	}
	else
		return 0;
}

// 获取转换后的今日日期
INT DayTips::GetTodayFormatDate()
{
	INT nYear;
	INT nMonth;
	INT nDay;

	nYear  = g_pTimeManager->GetYear();
	nMonth = g_pTimeManager->GetMonth();
	nDay   = g_pTimeManager->GetDay();

	//整型表示20121231
	return nYear * 10000 + nMonth * 100 + nDay;
}

// 获取活动参数完成次数下标
INT DayTips::GetMisDataCountIndex( INT nMissionID )
{
	INT nCount = 0;

	nCount = g_DayTipsAttrTbl.Count();

	for ( INT i = 0; i < nCount; i++ )
	{
		if ( g_DayTipsAttrTbl.m_TableExt[i].m_MisPos == nMissionID )
		{
			return g_DayTipsAttrTbl.m_TableExt[i].m_DoneCountIndex;
		}
	}

	return -1;
}

INT DayTips::GetMisDataDateIndex( INT nMissionID )
{
	INT nCount = 0;

	nCount = g_DayTipsAttrTbl.Count();

	for ( INT i = 0; i < nCount; i++ )
	{
		if ( g_DayTipsAttrTbl.m_TableExt[i].m_MisPos == nMissionID )
		{
			return g_DayTipsAttrTbl.m_TableExt[i].m_LastDoneTimeIndex;
		}
	}

	return -1;
}


// 获取活动个数
INT DayTips::GetMisCount()
{
	return g_DayTipsAttrTbl.Count();
}

// 根据下标获取活动对应的任务ID
INT DayTips::GetMisIDByIndex( INT nIndex )
{
	INT nCount = 0;

	nCount = g_DayTipsAttrTbl.Count();

	for ( INT i = 0; i < nCount; i++ )
	{
		if ( i == nIndex )
		{
			return g_DayTipsAttrTbl.m_TableExt[i].m_MisPos;
		}
	}

	return -1;
}