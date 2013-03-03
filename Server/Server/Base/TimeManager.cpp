#include "stdafx.h"
#include "TimeManager.h"

TimeManager*	g_pTimeManager=NULL ;

UINT g_FileNameFix_Last = 0 ;
int g_FileNameFix = 0;
TimeManager::TimeManager( )
{
	__ENTER_FUNCTION

		m_CurrentTime = 0 ;

	__LEAVE_FUNCTION
}

TimeManager::~TimeManager( )
{
	__ENTER_FUNCTION


		__LEAVE_FUNCTION
}

BOOL TimeManager::Init( )
{
	__ENTER_FUNCTION

#if defined(__WINDOWS__)
		m_StartTime = GetTickCount() ;
	m_CurrentTime = GetTickCount() ;
#elif defined(__LINUX__)
		m_StartTime		= 0;
	m_CurrentTime	= 0;
	gettimeofday(&_tstart, &tz);
#endif
	SetTime( ) ;

	g_FileNameFix = Time2Day( ) ;
	g_FileNameFix_Last = CurrentDate( ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

uint TimeManager::CurrentTime( )
{
	__ENTER_FUNCTION

#if defined(__WINDOWS__)
		m_CurrentTime = GetTickCount() ;
#elif defined(__LINUX__)
		gettimeofday(&_tend,&tz);			//获取UNIX时间
	double t1, t2;
	t1 =  (double)_tstart.tv_sec*1000 + (double)_tstart.tv_usec/1000;
	t2 =  (double)_tend.tv_sec*1000 + (double)_tend.tv_usec/1000;
	m_CurrentTime = (UINT)(t2-t1);
#endif

	return m_CurrentTime ;


	__LEAVE_FUNCTION

		return 0 ;
}

UINT	TimeManager::CurrentDate()
{
	__ENTER_FUNCTION

		SetTime( ) ;
	UINT Date;
	ConvertTU(&m_TM,Date);

	return Date;

	__LEAVE_FUNCTION

		return 0;
}


VOID TimeManager::SetTime( )
{
	__ENTER_FUNCTION

		time( &m_SetTime ) ;
	tm* ptm = localtime( &m_SetTime ) ;
	m_TM = *ptm ;

	__LEAVE_FUNCTION
}

// 得到标准时间
time_t TimeManager::GetANSITime( )
{
	__ENTER_FUNCTION

		SetTime();

	__LEAVE_FUNCTION

		return m_SetTime;
}

UINT TimeManager::Time2DWORD( )
{
	__ENTER_FUNCTION

		SetTime( ) ;

	UINT uRet=0 ;

	uRet += GetYear( ) ;
	uRet -= 2000 ;
	uRet =uRet*100 ;

	uRet += GetMonth( )+1 ;
	uRet =uRet*100 ;

	uRet += GetDay( ) ;
	uRet =uRet*100 ;

	uRet += GetHour( ) ;
	uRet =uRet*100 ;

	uRet += GetMinute( ) ;

	return uRet ;

	__LEAVE_FUNCTION

		return 0 ;
}

VOID TimeManager::DWORD2Time( UINT Date, tm* TM )
{
	__ENTER_FUNCTION

		Assert( TM ) ;
	memset( TM, 0, sizeof(tm) ) ;

	TM->tm_year	= ( Date / 100000000 ) + 2000 - 1900 ;
	TM->tm_mon	= ( Date % 100000000 ) / 1000000 - 1 ;
	TM->tm_mday	= ( Date % 1000000 ) / 10000 ;
	TM->tm_hour	= ( Date % 10000 ) / 100 ;
	TM->tm_min	= Date % 100 ;

	__LEAVE_FUNCTION
}

//取得两个DWORDTime的时间差（单位：分钟）, Ret = Date2-Data1
UINT TimeManager::DiffDWORDTime( UINT Date1, UINT Date2 )
{
	__ENTER_FUNCTION

		tm S_D1, S_D2 ;
	DWORD2Time( Date1, &S_D1 ) ;
	DWORD2Time( Date2, &S_D2 ) ;
	time_t t1,t2 ;
	t1 = mktime(&S_D1) ;
	t2 = mktime(&S_D2) ;
	UINT dif = ( UINT )( abs( ( INT )( difftime(t2,t1)/60 ) ) ) ;
	return dif ;

	__LEAVE_FUNCTION

		return 0 ;
}

INT	TimeManager::IsDiffDay(time_t uPrevANSITime, time_t uNextANSITime)
{
	__ENTER_FUNCTION

		tm tPrevTm = *localtime((time_t*)&uPrevANSITime);
	tm tCurTm = *localtime((time_t*)&uNextANSITime);
	return tCurTm.tm_yday - tPrevTm.tm_yday;

	__LEAVE_FUNCTION

		return 0 ;
}

INT TimeManager::DiffDayCount(time_t uPrevANSITime, time_t uNextANSITime)
{
	__ENTER_FUNCTION

		//tm tPrevTm = *localtime((time_t*)&uPrevANSITime);
		//tm tCurTm = *localtime((time_t*)&uNextANSITime);
		//return tCurTm.tm_yday - tPrevTm.tm_yday;

		INT dif = ( INT )( difftime(uNextANSITime,uPrevANSITime)/(24*60*60) ); 
	return dif;

	//time_t uDaySeconds = 60 * 60 * 24;
	//time_t uPrevDay = uPrevANSITime / uDaySeconds;
	//time_t uNextDay = uNextANSITime / uDaySeconds;
	//return ((uPrevDay > uNextDay)?((INT)(uPrevDay - uNextDay)):((INT)(uNextDay - uPrevDay)));

	__LEAVE_FUNCTION

		return 0 ;
}

//例如：20070403表示2007年4月3日
UINT TimeManager::Time2Day( )
{
	__ENTER_FUNCTION

		UINT uRet=0 ;

	uRet += GetYear( ) ;

	uRet =uRet*100 ;
	uRet += GetMonth( )+1 ;

	uRet =uRet*100 ;
	uRet += GetDay( ) ;

	return uRet ;

	__LEAVE_FUNCTION

		return 0 ;
}

UINT TimeManager::DiffTime( UINT Date1, UINT Date2 )
{
	__ENTER_FUNCTION

		tm S_D1, S_D2 ;
	ConvertUT( Date1, &S_D1 ) ;
	ConvertUT( Date2, &S_D2 ) ;
	time_t t1,t2 ;
	t1 = mktime(&S_D1) ;
	t2 = mktime(&S_D2) ;
	UINT dif = ( UINT )( abs( ( INT )( difftime(t2,t1) ) ) * 1000 ) ;
	return dif ;

	__LEAVE_FUNCTION

		return 0 ;
}

VOID TimeManager::ConvertUT( UINT Date, tm* TM )
{
	__ENTER_FUNCTION

		Assert(TM) ;
	memset( TM, 0, sizeof(tm) ) ;
	TM->tm_year = ((Date>>26)&0xf)+100 ;
	TM->tm_mon  = (Date>>22)&0xf ;
	TM->tm_mday = (Date>>17)&0x1f ;
	TM->tm_hour = (Date>>12)&0x1f ;
	TM->tm_min  = (Date>>6) &0x3f ;
	TM->tm_sec  = (Date)    &0x3f ;

	__LEAVE_FUNCTION
}

VOID TimeManager::ConvertTU( tm* TM, UINT& Date )
{
	__ENTER_FUNCTION

		Assert( TM ) ;
	Date = 0 ;
	Date += (TM->tm_year%10) & 0xf ;
	Date = (Date<<4) ;
	Date += TM->tm_mon & 0xf ;
	Date = (Date<<5) ;
	Date += TM->tm_mday & 0x1f ;
	Date = (Date<<5) ;
	Date += TM->tm_hour & 0x1f ;
	Date = (Date<<6) ;
	Date += TM->tm_min & 0x3f ;
	Date = (Date<<6) ;
	Date += TM->tm_sec & 0x3f ;

	__LEAVE_FUNCTION
}

UINT TimeManager::GetDayTime( )
{
	__ENTER_FUNCTION

		time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	UINT uRet=0 ;

	uRet  = (ptm->tm_year-100)*1000 ;
	uRet += ptm->tm_yday ;

	return uRet ;

	__LEAVE_FUNCTION

		return 0 ;
}

UINT TimeManager::GetHourTime( )
{
	__ENTER_FUNCTION

		time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	UINT uRet=0 ;
	if( ptm->tm_year + 1900 == 2008 )
	{
		uRet= 365;
	}
	uRet += ptm->tm_yday ;
	uRet *= 100 ;
	uRet += ptm->tm_hour*4 ;
	uRet += (UINT)(ptm->tm_min/15) ;

	return uRet ;

	__LEAVE_FUNCTION

		return 0 ;
}

//取得以周为单位的时间值, 千位数代表年份，其他三位代表时间（周数）
UINT TimeManager::GetWeekTime( )
{
	__ENTER_FUNCTION

		time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	UINT uRet=0 ;

	uRet  = (ptm->tm_year-100)*1000 ;

	if(ptm->tm_yday <= ptm->tm_wday)
		return uRet;

	INT it = ptm->tm_yday - ptm->tm_wday;
	uRet += static_cast<UINT>(ceil((double)(it/7)));

	return uRet ;

	__LEAVE_FUNCTION

		return 0 ;
}

// 得到tm结构的时间  20100723 BLL
tm  TimeManager::GetFormatTime( )
{
	__ENTER_FUNCTION

		SetTime();

	__LEAVE_FUNCTION

	return m_TM;

}