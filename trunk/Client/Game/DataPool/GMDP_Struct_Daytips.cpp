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
		DayNominate,			//���Ƽ�	
		MonthNominate,			//���Ƽ�
		ShowAll,				//�Ƿ�ȫ���ɼ�	0Ϊ��1Ϊ�����˿ɼ�
		ShowLevel,				//�ɼ��ȼ�
		ShowCountry,			//�ɼ�����

		ShowGuild,				//�ɼ����
		ShowFamily,				//�ɼ�����	
		MisType,				//��������	
		MisName,				//��������
		MisValidMonth,			//������Ч��	

		WeekOrDay,				//���������ڻ�����������0��ʾ����
		MisValidDate,			//������Чʱ��( ��� WeekOrDay��������������ֶα�ʾ�ڼ��ܣ������������������ʾ���µĵڼ���)
		//MisValidWeek,	
		VailidTimeSlice,		//����ʼ��������ʱ��
		MisPos,					//����ID				

		MisCount,				//ÿ�տ�������
		MisText,				//��������
		NpcName,				//������
		ScendId,				//OBJ���ڳ���ID
		SceneName,	

		PosX,
		PosZ,	

		DoneCountIndex,	    // ���ɴ����±� 
		DoneCircleIndex,  	// ���ɻ����±�
		LastDoneTimeIndex,    // ����һ�����ʱ���±�
		LastAbandonTimeIndex, // ����һ�η���ʱ���±�
	};

	memset(&m_DayTips, 0, sizeof(m_DayTips));

	DBC::DBCFile* pThirdFile = (DBC::DBCFile*)pDataBase;

	INT iTableCount	 =	pThirdFile->GetRecordsNum();

	//�ֶ���
	INT iColumnCount =	pThirdFile->GetFieldsNum();

	Assert(iTableCount>0 && iTableCount<MAX_DAY_TIPS_NUM);
	m_DayTips.m_Count	=	iTableCount;
	Assert(PosZ<iColumnCount);			//�ļ�������Ӧ�������������

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

		//�ɼ��ȼ�, ��Ҫ�����ַ���
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowLevel,MAX_LEVEL_NUM );

		//�ɼ�����
		pSplitString	=	pThirdFile->Search_Posistion(i,ShowCountry)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowCountry,MAX_COUNTRY_NUM );

		//�ɼ����, ��Ҫ�����ַ���
		pSplitString	=	pThirdFile->Search_Posistion(i,ShowGuild)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowGuild,GUILD_FAMILY_SIZE );

		//�ɼ�����
		pSplitString	=	pThirdFile->Search_Posistion(i,ShowFamily)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_ShowFamily,GUILD_FAMILY_SIZE );

		m_DayTips.m_TableExt[i].m_MisType = pThirdFile->Search_Posistion(i,MisType)->iValue;	
		strncpy( m_DayTips.m_TableExt[i].m_MisName, pThirdFile->Search_Posistion(i,MisName)->pString, MAX_MIS_NAME_LEN );

		//������Ч�·�
		pSplitString	=	pThirdFile->Search_Posistion(i,MisValidMonth)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, m_DayTips.m_TableExt[i].m_MisValidMonth,13 );


		m_DayTips.m_TableExt[i].m_WeekOrDay = pThirdFile->Search_Posistion(i,WeekOrDay)->iValue;	

		pSplitString	=	pThirdFile->Search_Posistion(i,MisValidDate)->pString;
		if ( m_DayTips.m_TableExt[i].m_WeekOrDay == 1 )
		{
			//������������
			ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString,
				m_DayTips.m_TableExt[i].m_MisValidDate,32 );
		}
		else
		{
			//������������
			ReadDayTipsValidWeek(&oSplitL1, &oSplitL2, pSplitString,
				m_DayTips.m_TableExt[i].m_MisValidWeek);
		}

		//�õ���ʼ�ͽ�����ʱ��
		pSplitString	=	pThirdFile->Search_Posistion(i,VailidTimeSlice)->pString;
		ReadDayTipsBeginAndEndTime(&oSplitL1, &oSplitL2, pSplitString,
			&m_DayTips.m_TableExt[i].m_VailidTimeSlice,1);

		//���ű�λ��
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

		//��ʼ�����	
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
		//�õ���ǰʱ��
		time_t ttTime;
		time( &ttTime ) ;
		tm *pTime;	
		pTime = localtime( &ttTime ) ;

		const CHAR *szTime = pSplitL1->GetResultLine(id);

		//��ȡ~�����ַ����е�һ�γ��ֵ�λ�� 2030~2230 
		size_t iPos = strcspn(szTime, "~" );
		if (iPos>=strlen(szTime))					//����ʱ
		{				
			pVal->m_Begin.tm_year = pTime->tm_year;	//Ĭ��Ϊ�����
			pVal->m_Begin.tm_hour = 0;
			pVal->m_Begin.tm_min  = 0;
			pVal->m_Begin.tm_sec  = 0;

			pVal->m_End.tm_year = pTime->tm_year;	//Ĭ��Ϊ�����
			pVal->m_End.tm_hour = 24;
			pVal->m_End.tm_min  = 0;
			pVal->m_End.tm_sec  = 0;
			return;
		}

		//�õ���ʼʱ��
		CHAR Begin[8] ;
		strncpy(Begin,szTime,iPos);
		INT Hour	= atoi(Begin) / 100;
		INT Minute	= atoi(Begin) % 100;

		//�������뽫ʱ��תΪtime_t�ģ���ֻ֪�����ڲ�֪�����ڵ�����º���ת��
		pVal->m_Begin.tm_year = pTime->tm_year;	//Ĭ��Ϊ�����
		pVal->m_Begin.tm_hour = Hour;
		pVal->m_Begin.tm_min  = Minute;
		pVal->m_Begin.tm_sec  = 0;

		//�õ�����ʱ��
		CHAR End[8];
		strncpy(End,szTime+iPos+1,strlen(szTime) - iPos - 1);
		Hour	= atoi(End) / 100;
		Minute	= atoi(End) % 100;
		pVal->m_End.tm_year = pTime->tm_year;	//Ĭ��Ϊ�����
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
	INT Field1 = 0;		//��һ���ֶ�
	INT Field2 = 0;		//�ڶ����ֶ�

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

		if ( Field1 == 0 )			//������
		{
			for ( int i = 0; i < MONTH_WEEK_NUM; i++ )
			{
				if ( Field2 == 0 )	//������
				{
					for ( int j=0;j<WEEK_DAY_NUM;j++ )
						pVal[i][j] = 1;
				}
				else
				{
					// ����������գ���Ӣ�ĸ�ʽ Sunday = 0
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
			if ( Field2 == 0 )		//������
			{
				for ( int j=0;j<WEEK_DAY_NUM;j++ )
					pVal[Field1][j] = 1;
			}
			else
			{
				// ����������գ���Ӣ�ĸ�ʽ Sunday = 0
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
