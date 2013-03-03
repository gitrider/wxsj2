
#include "stdafx.h"
#include "WorldTable.h"
#include "Log.h"
#include "Ini.h"
#include "FileDef.h"
#include "WXSJ_DBC.h"

using namespace DBC;
_GUILD_MEMBER_ACCESS	g_GuildMemberAccessTbl ;
_DBC_CITY_DEFINE		g_CityBuildingTbl;
_DEFAULT_CHARDATA_TBL   g_DefaultCharDataTbl ;
_TOP_LIST_ATTR_TBL		g_TopListAttrTbl ;
_GUILD_LEVEL_INFO_TBL   g_GuildLevelInfoTbl;

TableInit				g_TableInit ;

TableInit::TableInit( )
{
}

TableInit::~TableInit( )
{
}

BOOL TableInit::Init( )
{
__ENTER_FUNCTION
	Init_GuildMemberAccess( ) ;
	Init_CityBuilding();
	Init_DefaultChar();
	Init_TopListTable();
    InitGuildLevelInfoTable();
__LEAVE_FUNCTION

	return TRUE ;
}

VOID TableInit::Init_TopListTable()
{
__ENTER_FUNCTION

	enum TopListAttrData
	{	
		TopListID = 0,				//���а�ID
		Name,						//���а������
		UpdateInterval = 4,			//ˢ�µ�ʱ����
		MemberCount ,				//���а���������
		EnterMinKey,				//��ǰ��¼��
	};

	DBCFile DBFile(0);
	BOOL ret = DBFile.OpenFromTXT( FILE_TOP_LIST );
	INT iTableCount = DBFile.GetRecordsNum();

	Assert(iTableCount<=MAX_TOP_LIST);

	for (INT i = 0; i < iTableCount; ++i)
	{
		g_TopListAttrTbl.m_TableExt[i].m_TopListID		= DBFile.Search_Posistion(i, TopListID)->iValue;
		strncpy( g_TopListAttrTbl.m_TableExt[i].m_Name,
			DBFile.Search_Posistion(i,Name)->pString, MAX_TOP_LIST_NAME-1 );

		FLOAT fTime = DBFile.Search_Posistion(i, UpdateInterval)->fValue;
		fTime = fTime * 60 * 60 * 1000;
		g_TopListAttrTbl.m_TableExt[i].m_UpdateInterval		= (UINT)fTime;
		Assert( DBFile.Search_Posistion(i, MemberCount)->iValue <= TOP_USDR_NUM ) ;		
		g_TopListAttrTbl.m_TableExt[i].m_MemberCount	= DBFile.Search_Posistion(i, MemberCount)->iValue;
		g_TopListAttrTbl.m_TableExt[i].m_EnterMinKey	= DBFile.Search_Posistion(i, EnterMinKey)->iValue;

	}

	g_TopListAttrTbl.m_Count = iTableCount;

__LEAVE_FUNCTION
}


VOID TableInit::Init_GuildMemberAccess( )
{
__ENTER_FUNCTION

	Ini ini( FILE_GUILD_MEMBER_ACCESS ) ;

	g_GuildMemberAccessTbl.m_MemberTypeCount = ini.ReadInt( "info", "membertypecount" ) ;
	Assert( g_GuildMemberAccessTbl.m_MemberTypeCount<MAX_MEMBER_TYPES ) ;

	for( INT i=0; i<g_GuildMemberAccessTbl.m_MemberTypeCount; i++ )
	{
		for( INT j=0; j<GUILD_AUTHORITY_NUMBER; j++ )
		{
			CHAR szSection[256], szName[256] ;
			sprintf( szSection, "member_type%d", i ) ;
			sprintf( szName, "access_type%d", j ) ;
			g_GuildMemberAccessTbl.m_Table[j][i] = ini.ReadInt( szSection, szName ) ;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load GuildMemberAccess.ini ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID TableInit::Init_CityBuilding( )
{
__ENTER_FUNCTION
	DBCFile DBFile(0);
	BOOL ret = DBFile.OpenFromTXT( FILE_CITY_BUILDING );
	INT iTableCount = DBFile.GetRecordsNum();

	Assert(iTableCount<=MAX_CITY_BUILDING);

	INT nIndex = 0;
	for (INT i = 0; i < iTableCount; ++i)
	{
		nIndex = DBFile.Search_Posistion(i, 0)->iValue;
		g_CityBuildingTbl.m_Table[nIndex].nID			= nIndex;
		g_CityBuildingTbl.m_Table[nIndex].nCityID		= DBFile.Search_Posistion(i, 1)->iValue;
		g_CityBuildingTbl.m_Table[nIndex++].nBuildingID	= DBFile.Search_Posistion(i, 2)->iValue;
	}

	g_CityBuildingTbl.m_Num = iTableCount;

__LEAVE_FUNCTION
}

VOID TableInit::Init_DefaultChar( )
{
	__ENTER_FUNCTION

	enum DefaultRoleTBData
	{	
		TBEnterScence		=0, //��ʼ����ID
		TBBornX             ,   //����X����
		TBBornY             ,   //����Y����
		TBCamp              ,   //��ʼ��Ӫ

		TBIniLevel          ,   //��ʼ�ȼ�	
		TBIniExp            ,   //��ʼ����
		TBIniHP             ,   //��ʼ����
		TBIniRage           ,   //��ʼŭ��

		TBIniPneuma			,	//��ʼԪ��
		TBIniMaxVigor		,	//��ʼ��������
		TBIniSkill1         ,   //��ʼս������1	
		TBIniSkill2         ,   //��ʼս������2	

		TBIniSkill3         ,   //��ʼս������3	
		TBIniSkill4         ,   //��ʼս������4	
		TBIniSkill5         ,   //��ʼս������5	
		TBIniAbility1		,   //��ʼ�����1	

		TBIniAbility2		,   //��ʼ�����2	
		TBIniAbility3		,   //��ʼ�����3	
		TBIniAbility4		,   //��ʼ�����4	
		TBIniAbility5		,   //��ʼ�����5	

		TBMoney				,
		TBIniNum			,
	};

	DBCFile DBFile(0);
	BOOL ret = DBFile.OpenFromTXT( FILE_CHAR_DEFAULT );
	INT iTableCount = DBFile.GetRecordsNum();
	for (INT n=0; n<MAX_COUNTRY_NUM; ++n)
	{
		for(INT i = 0;	i<MAX_PREFESSION_NUM;i++)
		{
			for(INT j = 0;j<TBIniNum; j++)
			{
				INT iValue = DBFile.Search_Posistion(i+n*MAX_PREFESSION_NUM, j+3)->iValue;
				g_DefaultCharDataTbl.Set(n, i,j,iValue);
			}
		}
	}
	

	Log::SaveLog( SERVER_LOGFILE, "Init_DefaultChar()  Ok!") ;

	__LEAVE_FUNCTION
}

//�������������ñ�
VOID TableInit::InitGuildLevelInfoTable()
{
    __ENTER_FUNCTION

        enum GuildLevelData
    {
        LEVELID                 = 0,            //���ȼ�
        MAXFAMILY,                              //��������
        GOLD,                                   //�ü������Ǯ
        CONTRIBUTE,                             //�ü����蹱��
        MAXGUILDUSER,                           //�������Ա����
        MAXFAMILYUSER,                          //�ƽ������������
        GUILDLEVELDATACOUNT,
    };

    DBCFile ThirdFile(0);
    BOOL ret = ThirdFile.OpenFromTXT( FILE_GUILD_LEVEL );
    INT iTableCount = ThirdFile.GetRecordsNum();
    Assert(iTableCount<=MAX_GUILD_LEVEL);

    for (INT i = 0; i < iTableCount; ++i)
    {
        g_GuildLevelInfoTbl.m_TableInfo[i].m_Level         = ThirdFile.Search_Posistion(i, LEVELID)->iValue;
        g_GuildLevelInfoTbl.m_TableInfo[i].m_MaxFamily     = ThirdFile.Search_Posistion(i, MAXFAMILY)->iValue;
        g_GuildLevelInfoTbl.m_TableInfo[i].m_Gold          = ThirdFile.Search_Posistion(i, GOLD)->iValue;
        g_GuildLevelInfoTbl.m_TableInfo[i].m_Contribute    = ThirdFile.Search_Posistion(i, CONTRIBUTE)->iValue;
        g_GuildLevelInfoTbl.m_TableInfo[i].m_MaxGuildUser  = ThirdFile.Search_Posistion(i, MAXGUILDUSER)->iValue;
        g_GuildLevelInfoTbl.m_TableInfo[i].m_MaxGoldFamilyUser = ThirdFile.Search_Posistion(i, MAXFAMILYUSER)->iValue;
    }
    Log::SaveLog( SERVER_LOGFILE, "InitGuildLevelInfoTable() ... OK! " ) ;
    __LEAVE_FUNCTION
}
