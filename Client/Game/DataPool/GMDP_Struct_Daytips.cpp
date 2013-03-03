//////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GMDP_Struct_Daytips.h"
#include "Utilities\SplitStringLevelOne.h"
#include "Utilities\SplitStringLevelTwo.h"
#include "..\Interface\GMGameInterface_Script.h"


DaytipsDataMgr *DaytipsDataMgr::s_pMe	= NULL;
DaytipsDataMgr::DaytipsDataMgr( VOID )
{
	s_pMe	= this;
}

DaytipsDataMgr::~DaytipsDataMgr( VOID )
{
	s_pMe = NULL;
}


BOOL DaytipsDataMgr::Init( const DBC::DBCFile *pDataBase )
{
	InitDapTipsAttrTable( pDataBase );

	return TRUE;
}

VOID DaytipsDataMgr::InitDapTipsAttrTable( const DBC::DBCFile *pDataBase )
{
	__ENTER_FUNCTION

	enum DatTipsAttrData
	{
		INDEX			=0,		//index
		DayNominate,			//日推荐	
		MonthNominate,			//月推荐
		ShowAll,				//是否全部可见	0为否，1为所有人可见
		ShowLevel,				//可见等级
		ShowCountry,			//可见国家

		ShowGuild,				//可见帮会
		ShowFamily,				//可见家族	
		MisType,				//任务类型	
		MisName,				//任务名称
		MisValidMonth,			//任务有效月	

		WeekOrDay,				//任务以星期或日期来做，0表示星期
		MisValidDate,			//任务有效时段( 如果 WeekOrDay以星期做，则此字段表示第几周，如果以日期来做，表示当月的第几天)
		//MisValidWeek,	
		VailidTimeSlice,		//任务开始到结束的时间
		MisPos,					//任务ID				

		MisCount,				//每日可做次数
		MisText,				//任务描述
		NpcName,				//怪物名
		ScendId,				//OBJ所在场景ID
		SceneName,	

		PosX,
		PosZ,	

		DoneCountIndex,	    // 活动完成次数下标 
		DoneCircleIndex,  	// 活动完成环数下标
		LastDoneTimeIndex,    // 活动最后一次完成时间下标
		LastAbandonTimeIndex, // 活动最后一次放弃时间下标
	};

	memset(&m_DayTips, 0, sizeof(m_DayTips));

	DBC::DBCFile* pThirdFile = (DBC::DBCFile*)pDataBase;

	INT iTableCount	 =	pThirdFile->GetRecordsNum();

	//字段数
	INT iColumnCount =	pThirdFile->GetFieldsNum();

	Assert(iTableCount>0 && iTableCount<MAX_DAY_TIPS_NUM);
	m_DayTips.m_Count	=	iTableCount;
	Assert(PosZ<iColumnCount);			//文件中列数应大于最大索引号

	const CHAR* pSplitString = NULL;

	for(INT i =0;i<iTableCount;i++)
	{
		m_DayTips.m_TableExt[i].m_Index = pThirdFile->Search_Posistion(i,INDEX)->iValue;	
		m_DayTips.m_TableExt[i].m_DayNominate = pThirdFile->Search_Posistion(i,DayNominate)->iValue;	
		m_DayTips.m_TableExt[i].m_MonthNominate = pThirdFile->Search_Posistion(i,MonthNominate)->iValue;
		m_DayTips.m_TableExt[i].m_ShowAll = pThirdFile->Search_Posistion(i,ShowAll)->iValue;	

		SplitStringLevelOne oSplitL1;
		SplitStringLevelTwo oSplitL2;
		pSplitString	=	pThirdFile->Search_Posistion(i,ShowLevel)->pString;

		//可见等级, 需要解析字符串
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowLevel,MAX_LEVEL_NUM );

		//可见国家
		pSplitString	=	pThirdFile->Search_Posistion(i,ShowCountry)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowCountry,MAX_COUNTRY_NUM );

		//可见帮会, 需要解析字符串
		pSplitString	=	pThirdFile->Search_Posistion(i,ShowGuild)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowGuild,GUILD_FAMILY_SIZE );

		//可见家族
		pSplitString	=	pThirdFile->Search_Posistion(i,ShowFamily)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowFamily,GUILD_FAMILY_SIZE );

		m_DayTips.m_TableExt[i].m_MisType = pThirdFile->Search_Posistion(i,MisType)->iValue;	
		strncpy( m_DayTips.m_TableExt[i].m_MisName, pThirdFile->Search_Posistion(i,MisName)->pString, MAX_MIS_NAME_LEN );

		//任务有效月份
		pSplitString	=	pThirdFile->Search_Posistion(i,MisValidMonth)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_MisValidMonth,13 );


		m_DayTips.m_TableExt[i].m_WeekOrDay = pThirdFile->Search_Posistion(i,WeekOrDay)->iValue;	

		pSplitString	=	pThirdFile->Search_Posistion(i,MisValidDate)->pString;
		if ( m_DayTips.m_TableExt[i].m_WeekOrDay == 1 )
		{
			//任务按日期来做
			ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString,
				m_DayTips.m_TableExt[i].m_MisValidDate,32 );
		}
		else
		{
			//任务按星期来做
			ReadDayTipsValidWeek(&oSplitL1, &oSplitL2, pSplitString,
				m_DayTips.m_TableExt[i].m_MisValidWeek);
		}

		//得到开始和结束的时间
		pSplitString	=	pThirdFile->Search_Posistion(i,VailidTimeSlice)->pString;
		ReadDayTipsBeginAndEndTime(&oSplitL1, &oSplitL2, pSplitString,
			&m_DayTips.m_TableExt[i].m_VailidTimeSlice,1);

		//读脚本位置
		m_DayTips.m_TableExt[i].m_MisPos = pThirdFile->Search_Posistion(i,MisPos)->iValue;
		m_DayTips.m_TableExt[i].m_MisCount = pThirdFile->Search_Posistion(i,MisCount)->iValue;
		strncpy( m_DayTips.m_TableExt[i].m_MisText, 
			pThirdFile->Search_Posistion(i,MisText)->pString, MAX_MIS_TEXT_LEN );
		strncpy( m_DayTips.m_TableExt[i].m_NpcName, 
			pThirdFile->Search_Posistion(i,NpcName)->pString, NPC_NAME_LEN );
		m_DayTips.m_TableExt[i].m_ScendId = pThirdFile->Search_Posistion(i,ScendId)->iValue;
		strncpy( m_DayTips.m_TableExt[i].m_SceneName, 
			pThirdFile->Search_Posistion(i,SceneName)->pString, MAX_SCENE_LEN );
		m_DayTips.m_TableExt[i].m_PosX = pThirdFile->Search_Posistion(i,PosX)->iValue;
		m_DayTips.m_TableExt[i].m_PosZ = pThirdFile->Search_Posistion(i,PosZ)->iValue;

		//初始化标记	
		m_DayTips.m_TableExt[i].m_Used	= TRUE;

		m_DayTips.m_TableExt[i].m_DoneCountIndex = pThirdFile->Search_Posistion( i, DoneCountIndex )->iValue;
		m_DayTips.m_TableExt[i].m_DoneCircleIndex = pThirdFile->Search_Posistion( i, DoneCircleIndex )->iValue;		
		m_DayTips.m_TableExt[i].m_LastDoneTimeIndex = pThirdFile->Search_Posistion( i, LastDoneTimeIndex )->iValue;
		m_DayTips.m_TableExt[i].m_LastAbandonTimeIndex = pThirdFile->Search_Posistion( i, LastAbandonTimeIndex )->iValue;

	}//for end

	//Log::SaveLog( SERVER_LOGFILE, "Load DayTips.tab ... OK! " ) ;

}//InitDapTipsAttrTable end


VOID DaytipsDataMgr::ReadDayTipsBeginAndEndTime(SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
										   const CHAR* pSplitString, VALID_TIME_SLICE* pVal,INT MaxCnt )
{
	pSplitL1->Init( '|', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iLineCount	= pSplitL1->GetResultLineCount();

	Assert( MaxCnt >= iLineCount );
	if( MaxCnt < iLineCount )
	{
		return;
	}

	for( INT id=0; id<iLineCount; ++id )
	{
		//得到当前时间
		time_t ttTime;
		time( &ttTime ) ;
		tm *pTime;	
		pTime = localtime( &ttTime ) ;

		const CHAR *szTime = pSplitL1->GetResultLine(id);

		//获取~号在字符串中第一次出现的位置 2030~2230 
		size_t iPos = strcspn(szTime, "~" );
		if (iPos>=strlen(szTime))					//不限时
		{				
			pVal->m_Begin.tm_year = pTime->tm_year;	//默认为本年度
			pVal->m_Begin.tm_hour = 0;
			pVal->m_Begin.tm_min  = 0;
			pVal->m_Begin.tm_sec  = 0;

			pVal->m_End.tm_year = pTime->tm_year;	//默认为本年度
			pVal->m_End.tm_hour = 24;
			pVal->m_End.tm_min  = 0;
			pVal->m_End.tm_sec  = 0;
			return;
		}

		//得到开始时间
		CHAR Begin[8] ;
		strncpy(Begin,szTime,iPos);
		INT Hour	= atoi(Begin) / 100;
		INT Minute	= atoi(Begin) % 100;

		//本来是想将时间转为time_t的，但只知道星期不知道日期的情况下很难转换
		pVal->m_Begin.tm_year = pTime->tm_year;	//默认为本年度
		pVal->m_Begin.tm_hour = Hour;
		pVal->m_Begin.tm_min  = Minute;
		pVal->m_Begin.tm_sec  = 0;

		//得到结束时间
		CHAR End[8];
		strncpy(End,szTime+iPos+1,strlen(szTime) - iPos - 1);
		Hour	= atoi(End) / 100;
		Minute	= atoi(End) % 100;
		pVal->m_End.tm_year = pTime->tm_year;	//默认为本年度
		pVal->m_End.tm_hour = Hour;
		pVal->m_End.tm_min  = Minute;
		pVal->m_End.tm_sec  = 0;
	}

}//ReadDayTipsBeginAndEndTime end


VOID DaytipsDataMgr::ReadDayTipsValidWeek( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
									 const CHAR* pSplitString,INT pVal[MONTH_WEEK_NUM][WEEK_DAY_NUM] )
{
	pSplitL1->Init( '|', pSplitL2 );
	pSplitL2->Init( '~', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iLineCount	= pSplitL2->GetResultLineCount();

	INT	temp   = 0;
	INT Field1 = 0;		//第一个字段
	INT Field2 = 0;		//第二个字段

	Assert( 1 >= iLineCount );
	if( 1 < iLineCount )
	{
		return;
	}

	for( INT id = 0; id < iLineCount; ++id )
	{
		temp = atoi(pSplitL2->GetResultLine(id)) ;
		Field1 = temp / 10;
		Field2 = temp % 10;

		Assert( MONTH_WEEK_NUM >= Field1 && WEEK_DAY_NUM>=Field2 );
		if( MONTH_WEEK_NUM < Field1 || WEEK_DAY_NUM < Field2)
		{
			return;
		}

		if ( Field1 == 0 )			//不限周
		{
			for ( int i = 0; i < MONTH_WEEK_NUM; i++ )
			{
				if ( Field2 == 0 )	//不限日
				{
					for ( int j=0;j<WEEK_DAY_NUM;j++ )
						pVal[i][j] = 1;
				}
				else
				{
					// 如果是星期日，则按英文格式 Sunday = 0
					if ( Field2 == 7 )
					{
						Field2 = 0;
					}

					pVal[i][Field2] = 1;
				}

			}//for ( int i = 0; i < MONTH_WEEK_NUM; i++ )
		}
		else
		{
			if ( Field2 == 0 )		//不限日
			{
				for ( int j=0;j<WEEK_DAY_NUM;j++ )
					pVal[Field1][j] = 1;
			}
			else
			{
				// 如果是星期日，则按英文格式 Sunday = 0
				if ( Field2 == 7 )
				{
					Field2 = 0;
				}

				pVal[Field1][Field2] = 1;
			}

		}//if ( Field1 == 0 )	

	}//for( INT id = 0; id < iLineCount; ++id )

}//ReadDayTipsValidWeek end


VOID DaytipsDataMgr::ReadDayTipsData( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
								const CHAR* pSplitString, INT* pVal,INT MaxCnt )
{
	pSplitL1->Init( '|', pSplitL2 );
	pSplitL2->Init( '~', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iLineCount	= pSplitL2->GetResultLineCount();

	Assert( MaxCnt >= iLineCount );
	if( MaxCnt < iLineCount )
	{
		return;
	}

	for( INT id=0; id<iLineCount; ++id )
	{
		if ( atoi(pSplitL2->GetResultLine(0)) == 0 )
		{
			for(int i=0;i<MaxCnt;i++)
			{
				pVal[i] = 1;				
			}

			return;
		}
		else
			pVal[atoi(pSplitL2->GetResultLine(id))] = 1;
	}

}
