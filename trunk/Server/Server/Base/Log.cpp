
#include "stdafx.h"
#include "Log.h"
#include "TimeManager.h"
#include "stdarg.h"

INT g_Command_LogPrint = 1 ;
INT g_Command_LogActive = 1 ;

CHAR* g_pLogFileName[] =
{
	"./Log/login",		//0		LOG_FILE_0
	"./Log/debug",		//1		LOG_FILE_1
	"./Log/error",		//2		LOG_FILE_2
	"./Log/functions",	//3		LOG_FILE_3
	"./Log/world",		//4		LOG_FILE_4
	"./Log/item",		//5		LOG_FILE_5
	"./Log/send",		//6		LOG_FILE_6
	"./Log/money",		//7		LOG_FILE_7
	"./Log/pet",		//8		LOG_FILE_8
	"./Log/skill",		//9		LOG_FILE_9
	"./Log/xinfa",		//10	LOG_FILE_10
	"./Log/ability",	//11	LOG_FILE_11
	"./Log/chat",		//12	LOG_FILE_12
	"./Log/mission",	//13	LOG_FILE_13
	"./Log/DBShare",	//14	LOG_FILE_14
	'\0'
};

MyLock	g_log_lock ;

BOOL	g_Log_AllInOne	=	FALSE;

#ifdef  _LOG_GL
#define _LOG_DEFINEED_
CHAR*	g_ExternLogName = "./Log/Log_GLServer_All";
#elif	_LOG_SV
#define _LOG_DEFINEED_
CHAR*	g_ExternLogName = "./Log/Log_MapServer_All";
#elif   _LOG_SM
#define _LOG_DEFINEED_
CHAR*	g_ExternLogName = "./Log/Log_DBShare_All";
#endif

#ifndef _LOG_DEFINEED_
CHAR*	g_ExternLogName = "./Log/Log_Default_All";
#endif

//日志写入磁盘,速度慢
VOID DiskLog(CHAR* filename, CHAR const* msg, ...)
{
	__ENTER_FUNCTION

		if( g_Command_LogActive != 1 )
		{
			return ;
		}
		if( filename==NULL || filename[0]==0 )
		{
			return ;
		}

		CHAR szBuff[LOG_BUFF_TEMP];
		memset(szBuff, 0, LOG_BUFF_TEMP) ;

		va_list argptr;
		_MY_TRY
		{
			va_start(argptr, msg);
			tvsnprintf(szBuff,LOG_BUFF_TEMP-LOG_NAME_TEMP-1,msg,argptr);
			va_end(argptr);

			if( g_pTimeManager )
			{
				g_pTimeManager->SetTime() ;

				CHAR szTime[LOG_NAME_TEMP] ;
				memset(szTime, 0, LOG_NAME_TEMP) ;

				tsnprintf( szTime, LOG_NAME_TEMP-1, 
					" (%d)(T0=%d-%d-%d_%d:%d:%d T1=%.4f)\n", 
					MyGetCurrentThreadID(), 
					g_pTimeManager->GetYear(),
					g_pTimeManager->GetMonth()+1,
					g_pTimeManager->GetDay(),
					g_pTimeManager->GetHour(),
					g_pTimeManager->GetMinute(),
					g_pTimeManager->GetSecond(),
					(FLOAT)(g_pTimeManager->RunTime())/1000.0 ) ;

				strncat( szBuff, szTime, LOG_NAME_TEMP-1 ) ;
			}
		}
		_MY_CATCH
		{
			if( g_Command_LogPrint ) printf("ERROR: SaveLog unknown error!\n");
			return ;
		}

		CHAR szFileName[_MAX_PATH] ;
		_MY_TRY
		{
			memset( szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
			sprintf( szFileName, "%s_%.4d-%.2d-%.2d.%u.log", 
				filename, 
				g_FileNameFix/10000,
				(g_FileNameFix%10000)/100,
				g_FileNameFix%100,
				g_FileNameFix_Last ) ;
		}
		_MY_CATCH
		{
		}

		g_log_lock.Lock() ;
		_MY_TRY
		{
			FILE* f = fopen( szFileName, "a+" ) ;
			if( f )
			{
				_MY_TRY
				{
					fwrite( szBuff, 1, strlen(szBuff), f ) ;
				}
				_MY_CATCH
				{
				}
				fclose(f) ;
			}
		}
		_MY_CATCH
		{
		}
		g_log_lock.Unlock() ;

		if( TRUE==g_Command_LogPrint )
		{
			printf("%s", szBuff);
		}

		__LEAVE_FUNCTION
}

Log* g_pLog=NULL ;

Log::Log( )
{
__ENTER_FUNCTION

	for( INT i=0; i<LOG_FILE_NUMBER; i++ )
	{
		m_LogCache[i] = NULL ;
		m_LogPos[i] = 0 ;
	}
	m_CacheSize = 0 ;
	m_DayTime= 0 ;

__LEAVE_FUNCTION
}

Log::~Log( )
{
__ENTER_FUNCTION

	for( INT i=0; i<LOG_FILE_NUMBER; i++ )
	{
		SAFE_DELETE_ARRAY(m_LogCache[i])  ;
	}
	m_CacheSize = 0 ;

__LEAVE_FUNCTION
}

BOOL Log::Init( INT cachesize )
{
__ENTER_FUNCTION

	m_CacheSize = cachesize ;
	for( INT i=0; i<LOG_FILE_NUMBER; i++ )
	{
		m_LogCache[i] = new CHAR[m_CacheSize] ;
		if( m_LogCache[i] == NULL )
		{
			return FALSE ;
		}
		m_LogPos[i] = 0 ;
	}
	if( g_pTimeManager )
		m_DayTime = g_pTimeManager->GetDayTime() ;
	else
		m_DayTime = 6000 ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID Log::FastSaveLog( INT logid, CHAR* msg, ... )
{
__ENTER_FUNCTION

	if( logid<0 || logid >=LOG_FILE_NUMBER )
		return ;

	CHAR buffer[2048];
	va_list argptr;

	try
	{
		va_start(argptr, msg);
		vsprintf(buffer,msg,argptr);
		va_end(argptr);

		CHAR szTime[256]={0} ;
		if( g_pTimeManager )
		{
			g_pTimeManager->SetTime();
			sprintf( szTime, " (%d)[T0=%d-%d-%d_%d:%d:%d][T1=%.4f]\r\n", 
				MyGetCurrentThreadID(),
				g_pTimeManager->GetYear(),
				g_pTimeManager->GetMonth(),
				g_pTimeManager->GetDay(),
				g_pTimeManager->GetHour(),
				g_pTimeManager->GetMinute(),
				g_pTimeManager->GetSecond(),
				(FLOAT)(g_pTimeManager->RunTime())/1000.0 ) ;
			strcat( buffer, szTime ) ;
		}
	}
	catch(...)
	{
		Assert(FALSE) ;
		return ;
	}

	INT iLen = (INT)strlen(buffer) ;
	if( iLen<=0 )
		return ;

	if(g_Log_AllInOne)
	{
		SaveLog(g_ExternLogName,buffer);
		return;
	}

	m_LogLock[logid].Lock() ;
	try
	{
		memcpy( m_LogCache[logid]+m_LogPos[logid], buffer, iLen ) ;
		printf( buffer ) ;
	}
	catch(...)
	{
	}

	m_LogPos[logid] += iLen ;
	m_LogLock[logid].Unlock() ;

	

	if( m_LogPos[logid] > (DEFAULT_LOG_CACHE_SIZE*2)/3 )
	{
		FlushLog( logid ) ;
	}
	return ;

__LEAVE_FUNCTION

	return ;
}

VOID Log::FlushLog( INT logid )
{
__ENTER_FUNCTION

	CHAR szName[_MAX_PATH] ;
	GetLogName( logid, szName ) ;

	m_LogLock[logid].Lock() ;
	try
	{
		FILE* f = fopen( szName, "ab" ) ;
		fwrite( m_LogCache[logid], 1, m_LogPos[logid], f ) ;
		fclose(f) ;
		m_LogPos[logid] = 0 ;
	}
	catch(...)
	{
	}
	m_LogLock[logid].Unlock() ;
	return ;

__LEAVE_FUNCTION
}

VOID Log::FlushLog_All( )
{
__ENTER_FUNCTION

	for( INT i=0; i<LOG_FILE_NUMBER; i++ )
	{
		FlushLog(i) ;
	}

__LEAVE_FUNCTION
}


VOID Log::GetLogName( INT logid, CHAR* szName )
{
__ENTER_FUNCTION
	if(g_pTimeManager)
	{
		sprintf( szName, "%s_%d_%d_%d.log", 
			g_pLogFileName[logid],
			g_pTimeManager->GetYear(),
			g_pTimeManager->GetMonth(),
			g_pTimeManager->GetDay()) ;
	}
	else
	{
		sprintf( szName, "%s_%d.log", g_pLogFileName[logid], m_DayTime ) ;
	}

__LEAVE_FUNCTION
}

VOID Log::SaveLog( CHAR* filename, CHAR* msg, ... )
{
__ENTER_FUNCTION
	
	
	if(g_Log_AllInOne)
	{
		g_log_lock.Lock( ) ;

		CHAR buffer[2048];
		memset( buffer, 0, 2048 ) ;

		va_list argptr;

		_MY_TRY
		{
			va_start(argptr, msg);
			vsprintf(buffer,msg,argptr);
			va_end(argptr);

			CHAR szTime[256]={0} ;
			if( g_pTimeManager )
			{
				g_pTimeManager->SetTime();
				sprintf( szTime, " (%d)[T0=%d-%d-%d_%d:%d:%d][T1=%.4f]\r\n", 
					MyGetCurrentThreadID(),
					g_pTimeManager->GetYear(),
					g_pTimeManager->GetMonth(),
					g_pTimeManager->GetDay(),
					g_pTimeManager->GetHour(),
					g_pTimeManager->GetMinute(),
					g_pTimeManager->GetSecond(),
					(FLOAT)(g_pTimeManager->RunTime())/1000.0 ) ;
				strcat( buffer, szTime ) ;
			}

			CHAR szFileName[256] ={0};
			sprintf(szFileName,"%s_%d_%d_%d.log",
				g_ExternLogName,
				g_pTimeManager->GetYear(),
				g_pTimeManager->GetMonth(),
				g_pTimeManager->GetDay());
			FILE* f = fopen( szFileName, "ab" ) ;
			fwrite( buffer, 1, strlen(buffer), f ) ;
			fclose(f) ;

			printf( buffer ) ;
		}
		_MY_CATCH
		{
			printf("some log error here\r\n");
		}

		g_log_lock.Unlock( ) ;
		return ;

	}
	else
	{
		g_log_lock.Lock( ) ;

		CHAR buffer[2048];
		memset( buffer, 0, 2048 ) ;

		va_list argptr;

		_MY_TRY
		{
			va_start(argptr, msg);
			vsprintf(buffer,msg,argptr);
			va_end(argptr);

			CHAR szTime[256]={0} ;
			if( g_pTimeManager )
			{
				g_pTimeManager->SetTime();
				sprintf( szTime, " (%d)[T0=%d-%d-%d_%d:%d:%d][T1=%.4f]\r\n", 
					MyGetCurrentThreadID(),
					g_pTimeManager->GetYear(),
					g_pTimeManager->GetMonth(),
					g_pTimeManager->GetDay(),
					g_pTimeManager->GetHour(),
					g_pTimeManager->GetMinute(),
					g_pTimeManager->GetSecond(),
					(FLOAT)(g_pTimeManager->RunTime())/1000.0 ) ;
				strcat( buffer, szTime ) ;
			}

			CHAR szFileName[256] ={0};
			sprintf(szFileName,"%s_%d_%d_%d.log",
					filename,
					g_pTimeManager->GetYear(),
					g_pTimeManager->GetMonth(),
					g_pTimeManager->GetDay());
			FILE* f = fopen( szFileName, "ab" ) ;
			fwrite( buffer, 1, strlen(buffer), f ) ;
			fclose(f) ;

			printf( buffer ) ;
		}
		_MY_CATCH
		{
			printf("some log error here\r\n");
		}

		g_log_lock.Unlock( ) ;
		return ;
	}

__LEAVE_FUNCTION

	g_log_lock.Unlock( ) ;
	return ;
}

VOID Log::RemoveLog( CHAR* filename )
{
__ENTER_FUNCTION

	g_log_lock.Lock( ) ;
	//w将文件截断为0
	FILE* f = fopen( filename, "w" ) ;
	fclose(f) ;

	g_log_lock.Unlock( ) ;
	return ;

__LEAVE_FUNCTION

	g_log_lock.Unlock( ) ;
	return ;
}

/////////////////////////////////////////////////////////////////////////////////
//生成序列号
//	pSerial		用来保存生成好的序列号(20位数)
//	nWorldId	世界号(3位数)
//	nServerId	服务器号(1位数)
VOID GetSerialKey(CHAR* pSerial,INT nWorldId, INT nServerId)
{
	static INT s_Step = 100;
	__ENTER_FUNCTION
		Assert(pSerial);
	//根据世界号、服务器号、当前时间生成
	tsnprintf( pSerial, MAX_PRIZE_SERIAL_LENGTH+1, 
		"%.3d%.1d%.4d%.2d%.2d%.2d%.3d%.3d", 
		nWorldId,
		nServerId,
		g_pTimeManager->GetDayTime(),
		g_pTimeManager->GetHour(),
		g_pTimeManager->GetMinute(),
		g_pTimeManager->GetSecond(),
		g_pTimeManager->CurrentTime()%1000,
		s_Step);

	s_Step++;
	if(s_Step >= 1000)	s_Step = 100;

	//不足20位的补足20位
	//for(INT i = (INT)(strlen(pSerial)); i < MAX_PRIZE_SERIAL_LENGTH; ++i)
	//{
	//	pSerial[i] = '0';
	//}
	__LEAVE_FUNCTION
}
