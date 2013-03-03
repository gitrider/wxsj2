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

// �Ƿ��ڻʱ����
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


// �Ƿ��ǽ��ջ
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
		// �����������Ч�·�
		if ( dayTipsAttr.m_MisValidMonth[ etm.tm_mon ] == 1 )
		{
			// �������������������
			if ( dayTipsAttr.m_WeekOrDay == 0 )
			{
				int nWeek = GetWeekNO();

				// ��Ч����
				if ( dayTipsAttr.m_MisValidWeek[ nWeek ][ etm.tm_wday ] == 0 )
				{
					return FALSE;
				}
			}
			// �������������������
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

// �����Ǳ��µڼ���    
INT	 DayTips::GetWeekNO()
{
	tm etm = g_pTimeManager->GetFormatTime();

	int weekDay  = etm.tm_wday;	// �������ڼ�
	int monthDay = etm.tm_mday;	// ���ռ���

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

// ��ȡһ���ĿɽӴ��� 
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

// �ж�ʱ���Ƿ���ʱ�����
INT	DayTips::IsInTimeSegment( const VALID_TIME_SLICE pSlice, const tm& pCheck, int* pValue )
{
	int seg1 = pSlice.m_Begin.tm_hour * 60 + pSlice.m_Begin.tm_min;
	int seg2 = pSlice.m_End.tm_hour * 60 + pSlice.m_End.tm_min;
	int check = pCheck.tm_hour * 60 + pCheck.tm_min;

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

	//�Ƿ�ȫ���ɼ�	0Ϊ��1Ϊ�����˿ɼ�
	if( dayTipsAttr.m_ShowAll == 1 )
		return TRUE;

	//�ɼ�����:�ȼ�
	int nLevel = pHuman->GetLevel();
	if( nLevel >= 0 )
	{
		if( dayTipsAttr.m_ShowLevel[nLevel] != 1 )
			return FALSE;
	}

	//�ɼ�����:����
	int nCountry = pHuman->GetCountry();
	if( nCountry >= 0 )
	{
		if( dayTipsAttr.m_ShowCountry[nCountry] != 1 )
			return FALSE;
	}

	//�ɼ�����:���
	int nGuild = pHuman->GetGuildID();
	if( nGuild >= 0 )
	{
		if( dayTipsAttr.m_ShowGuild[nGuild] != 1 )
			return FALSE;
	}

	//�ɼ�����:����
	int nFamily = pHuman->GetMenPai();
	if( nFamily >= 0 )
	{
		if( dayTipsAttr.m_ShowFamily[nFamily] != 1 )
			return FALSE;
	}

	return TRUE;
}

// ����һ����������Ƿ��ǽ���
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

// ��ȡת����Ľ�������
INT DayTips::GetTodayFormatDate()
{
	INT nYear;
	INT nMonth;
	INT nDay;

	nYear  = g_pTimeManager->GetYear();
	nMonth = g_pTimeManager->GetMonth();
	nDay   = g_pTimeManager->GetDay();

	//���ͱ�ʾ20121231
	return nYear * 10000 + nMonth * 100 + nDay;
}

// ��ȡ�������ɴ����±�
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


// ��ȡ�����
INT DayTips::GetMisCount()
{
	return g_DayTipsAttrTbl.Count();
}

// �����±��ȡ���Ӧ������ID
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