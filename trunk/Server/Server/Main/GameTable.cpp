

#include "stdafx.h"
#include "GameTable.h"
#include "Log.h"
#include "Ini.h"
#include "FileDataMgr.h"

#include "WXSJ_DBC.h"
#include "FileDef.h"
#include "AIScript.h"
#include "State.h"
#include "SplitStringLevelOne.h"
#include "SplitStringLevelTwo.h"
#include "ShopManager.h"
#include "ItemHelper.h"

using namespace DBC;

_BASE_VALUE_TBL				g_BaseValueTbl ;
_MONSTER_ATTREX_TBL			g_MonsterAttrExTbl ;
//_PET_ATTREX_TBL				g_PetAttrExTbl ;
_XUNHUAN_MISSION_ATTR_TBL	g_XunHuanMissionAttrTbl ;
_DAY_TIPS_ATTR_TBL			g_DayTipsAttrTbl ;
_MAP_INFO_ATTR_TAB			g_MapInfoAttrTbl;
_TIMER_DOING_ATTR_TAB		g_TiemrDoingAttrTbl ;
_NPC_QUEST_TBL				g_NpcQuestTbl;
_TITLE_INFO_TBL             g_TitleInfoTbl;
_TITLE_COMBINATION_TBL      g_TitleComTbl;
_GUILD_LEVEL_INFO_TBL       g_GuildLevelInfoTbl;

_NPC_PAOPAO_INDEX_TBL		g_NPCPaoPaoIndexTbl;
// pet
_PET_ATTR_TBL				g_PetAttrTbl ;
_PET_ATTRPOINT_DIS_TBL		g_PetAttrPointDisTbl;
_PET_LEVELUP_TBL			g_PetLevelUpTbl;
_PET_DOMESTICATION_MONEY_TBL		g_PetDomesticationMoneyTbl;
_PET_SKILLDISTR_TBL			g_PetSkillDistrTbl[PET_MAX_NUM];
_PET_SKILLINDEX_TBL			g_PetSkillIndexTbl[PET_SKILL_NUM];
_PET_STUDYSKILL_TBL			g_PetStudySkillTbl;
_PET_APPERCEIVESKILL_TBL	g_PetApperceiveSkillTbl[PET_APPERCEIVESKILLRATE_NUM];
FLOAT						g_PetAIStrategyTble[PET_SKILL_TYPE][PET_AITYPE];

_MONSTER_AI_TBL				g_MonsterAITbl ;
//�����ʼ��ʱ��¼�����ͼ��������Ϣ�ı�
MonsterWeaponAndSkillTable	g_MonsterWeaponAndSkill;
//����ѧϰ�������ı�
Skill_Study_Condition_TABLE g_SkillStudyCondition;
_ATTR_LEVELUP_TBL			g_AttrLevelupTbl ;
_LEVEL_EXP_TBL				g_LevelExpTbl;
_XINFA_UPLEVEL_SPEND_TBL	g_XinfaUpLevelSpendTbl;
_MONSTER_SPEAK_TBL			g_MonsterSpeakTbl ;
_GROW_POINT_INFO_TBL		g_GrowPointInfoTbl;
_PLANTS_INFO_TBL			g_PlantInfoTbl;
_QUESTION_LIB				g_QuestionLib ;
//Skill
XinFaDataMgr_T				g_XinFaDataMgr;
SkillTemplateDataMgr_T		g_SkillTemplateDataMgr;
SkillDataMgr_T				g_SkillDataMgr;
ImpactSEDataMgr_T			g_ImpactSEDataMgr;
ImpactDataMgr_T				g_StandardImpactMgr;
//end Skill
//impact collection
IDCollectionMgr_T			g_IDCollectionMgr;
//camp and stand
CampAndStandDataMgr_T		g_CampAndStandDataMgr;
//Special Obj
SpecialObjDataMgr_T			g_SpecialObjDataMgr;
//end
CFileDataMgr				g_AIFileDataMgr;
AIScript*					g_pAIScriptList[AISCRIPT_NUM];
_IPRANGE					g_IPRange ;

//�������Ǯ�������Ӧ
_LEVEL_MAX_MONEY			g_LevelMoneyTbl;
_CITY_SCENE					g_CitySceneTbl;
_CHAT_CONSUME				g_ChatConsumeTbl ;
_HAIR_STYLE					g_HairStyleTbl;
_GM_TABLE					g_GMTbl ;

//��������Ա�
_HORSE_ATTR_TBL				g_HorseAttrTbl;
//���������
_HORSE_SKILL_TBL			g_HorseSkillTbl;
//����Ʒʹ�ñ�
_HORSE_ITEM_TBL				g_HorseItemTbl;
//������
_HORSE_DROP_TBL				g_HorseDrop;

//��̬�̵�����
_DYSHOP_INFO_TBL            g_DyShopInfoTbl;
_SHOP_INFO_TBL              g_ShopInfoTbl;

//PK ������ñ�
_PKPOINT_INFO_TBL           g_PKPointTbl;
_PKLEVEL_INFO_TBL           g_PKLevelTbl;
_PKPUNISH_INFO_TBL          g_PKPunishTbl;

//by zheng
//��������ر�
OpenBox_ItemInfo_Table g_OpenBox_ItemInfo_Table;

//�;����
_DURSPOIL_VALUE_TBL    g_DurSpoilValueTbl;

_BUS_INFO_TBL				g_BusInfoTbl;

TableInit					g_TableInit ;
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
TableInit::TableInit( )
{
}

TableInit::~TableInit( )
{
	CleanUpAIScrpt();
	CleanUpAIStateList();
}

BOOL TableInit::Init( )
{
	BOOL bRet ;

	InitBaseValueTable( ) ;
	InitMonsterAttrExTable( ) ;
	InitHorseAttrTable();
	InitHorseRealTable();
	InitHorseItemTable();
	InitHorseDropTable();
	InitXunHunMissionAttrTable();
	InitDapTipsAttrTable();
	Init_MapInfoTable();
	Init_TimerDoingTable();
	InitNPCQuestTable();
	InitNPCPaoPaoIndexTable();
	InitPetAttrTable( );
	InitMonsterAITable( ) ;
	InitMonsterSkillTable();
	InitPetConfigTable( );
	InitPetAttrPointDistribute( );
	InitPetLevelUpTable();
	InitPetDomesticationMoney();
	InitPetSkillDistributeTable( );
	InitPetSkillIndexTable( );
	InitPetStudySkillTable( );
	InitPetApperceiveSkillTable( );
	InitPetAIStrategyTable( );
	InitAIScriptFile( );
	InitAIStateList();
    InitTitleInfoTable();
    InitTitleCombinationTable();
	bRet=InitCampAndStandDataMgr( );Assert(bRet);
	bRet=InitXinFaDataMgr( );Assert(bRet);
	bRet=InitSkillTemplateDataMgr( );Assert(bRet);
	bRet=InitSkillDataMgr( );Assert(bRet);
	bRet=InitImpactSEDataMgr( );Assert(bRet);
	bRet=InitSpecialObjDataMgr( );Assert(bRet);
	bRet=InitIdCollectionMgr( );Assert(bRet);
	bRet=InitStandardImpactMgr( );Assert(bRet);
	InitAttrLevelupTable( ) ;
	InitLevelExpTable( );
	InitXinfaUpLevelSpendTable( );
	InitMonsterSpeakTable( );
	InitGrowPointTable( );
	InitQuestionLib( ) ;
	InitIPRange( ) ;
	InitLevelMoneyTable( );
	InitCitySceneTable( );
	InitChatConsume( ) ;
	InitHairStyleTable( );
	InitGMTable( ) ;
	InitSKillStudyTable();
    InitDyShopInfoTable();
    InitShopInfoTable();
    InitGuildLevelInfoTable();
    InitPKPointInfoTable();
    InitPKLevelInfoTable();
    InitPKPunishInfoTable();
	//by zheng
	//���ر�����ر�
	InitOpenBoxInfoTable();
	InitBusInfoTable();
	InitDurSpoilInfoTable();
	return TRUE ;
}

BOOL TableInit::ReloadCombatTables(VOID)
{
	BOOL bRet ;
	bRet=InitCampAndStandDataMgr( );Assert(bRet);
	bRet=InitXinFaDataMgr( );Assert(bRet);
	bRet=InitSkillTemplateDataMgr( );Assert(bRet);
	bRet=InitSkillDataMgr( );Assert(bRet);
	bRet=InitImpactSEDataMgr( );Assert(bRet);
	bRet=InitSpecialObjDataMgr( );Assert(bRet);
	bRet=InitIdCollectionMgr( );Assert(bRet);
	bRet=InitStandardImpactMgr( );Assert(bRet);
	return TRUE ;
}

VOID TableInit::InitBaseValueTable( )
{//��ȡ����ϵ���������

	__ENTER_FUNCTION

	Ini ini( FILE_BASE_VALUE ) ;
	for( INT i=0; i<MATTRIBUTE_NUMBER; i++ )
	{
		for( INT j=0; j<AINFOTYPE_NUMBER; j++ )
		{
			CHAR szSection[256], szName[256] ;
			sprintf( szSection, "MENPAI%d", i ) ;
			sprintf( szName, "AINFOTYPE%d", j ) ;
			g_BaseValueTbl.m_Table[j][i] = ini.ReadInt( szSection, szName ) ;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load BaseValueTable.ini ... OK! " ) ;

	__LEAVE_FUNCTION

	
}

VOID	TableInit::InitLevelExpTable()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PLAYER_EXP_LEVEL );

	INT iTableCount = ThirdFile.GetRecordsNum();
	
	Assert(iTableCount==MAX_PLAYER_EXP_LEVEL);

	for(INT i = 0;	i<MAX_PLAYER_EXP_LEVEL;i++)
	{
		
		g_LevelExpTbl.m_Table[i][0]	=	i;
		g_LevelExpTbl.m_Table[i][1]	=	ThirdFile.Search_Posistion(i,1)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load PlayerExpLevel.txt ... OK! " ) ;
	
	__LEAVE_FUNCTION
}

VOID	TableInit::InitXinfaUpLevelSpendTable()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_XINFA_STUDY_SPEND );


	//����
	INT iTableCount = ThirdFile.GetRecordsNum();
	
	for(INT iXinfa=0; iXinfa<MAX_ALL_XINFA_NUM; iXinfa++)
	{
		for(INT iLevel=0; iLevel<MAX_XINFA_LEVEL_NUM; iLevel++)
		{
			g_XinfaUpLevelSpendTbl.m_Table[iXinfa][iLevel][0] = ThirdFile.Search_Posistion(iLevel+2,0)->iValue;
			g_XinfaUpLevelSpendTbl.m_Table[iXinfa][iLevel][1] = ThirdFile.Search_Posistion(iLevel+2,iXinfa*2 + 1)->iValue;
			g_XinfaUpLevelSpendTbl.m_Table[iXinfa][iLevel][2] = ThirdFile.Search_Posistion(iLevel+2,iXinfa*2 + 2)->iValue;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load XinFaStudySpend_V1.txt ... OK! " ) ;
	
	__LEAVE_FUNCTION
}

VOID	TableInit::InitMonsterSpeakTable()
{
__ENTER_FUNCTION
#define BASENUM (1000)
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_NPC_CHAT );

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT nLastID = 0 ,	
		nNewID  = 0 ,
		nIndex  = 0 ;

	for(INT i = 0;	i<iTableCount; i++)
	{
		nNewID = ThirdFile.Search_Posistion(i,0)->iValue ;
		const CHAR* pString = ThirdFile.Search_Posistion(i,1)->pString ;
		if( !nLastID ) 
		{//�ǵ�һ��̸��
			g_MonsterSpeakTbl.m_TypeCount++ ;
			g_MonsterSpeakTbl.m_NumberOfType[nIndex]++ ;
		}
        else if ( nLastID && nLastID/BASENUM==nNewID/BASENUM )
		{//ͬһ���͵�̸����
			g_MonsterSpeakTbl.m_NumberOfType[nIndex]++ ;
		}
		else
		{//��һ���͵�̸����
			nIndex++ ;
			g_MonsterSpeakTbl.m_TypeCount++ ;
			g_MonsterSpeakTbl.m_NumberOfType[nIndex]++ ;
		}

		g_MonsterSpeakTbl.m_Table[i].nSpeakID = nNewID;
		memset((VOID*)(g_MonsterSpeakTbl.m_Table[i].szSpeakContent),0,MAX_MONSTERSPEAK_CONTENT);
		memcpy((VOID*)(g_MonsterSpeakTbl.m_Table[i].szSpeakContent),pString,sizeof(pString));
		g_MonsterSpeakTbl.m_Table[i].nSpeakID = nNewID ;
		g_MonsterSpeakTbl.m_Count++ ;
		nLastID = nNewID ;
		
	}

	Log::SaveLog( SERVER_LOGFILE, "Load NPCChat.txt ... OK! " ) ;

__LEAVE_FUNCTION

}

uint	TableInit::GetLevelExp(uint iLevel)
{
	
	__ENTER_FUNCTION
	
		return g_LevelExpTbl.m_Table[iLevel-1][1];
	
	__LEAVE_FUNCTION

}

VOID TableInit::InitDapTipsAttrTable()
{
	__ENTER_FUNCTION

	enum DatTipsAttrData
	{
		DayNominate		=	1,	//���Ƽ�	
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
	    MisValidDate,	
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

	memset(&g_DayTipsAttrTbl,0,sizeof(g_DayTipsAttrTbl));

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_DAY_TIPS );
	AssertEx(ret,FILE_DAY_TIPS);

	//��¼��
	INT iTableCount			=	ThirdFile.GetRecordsNum();
	
	//�ֶ���
	INT iColumnCount		=	ThirdFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAX_DAY_TIPS_NUM);
	g_DayTipsAttrTbl.m_Count	=	iTableCount;
	Assert(PosZ<iColumnCount);			//�ļ�������Ӧ�������������

	const CHAR* pSplitString = NULL;

	for(INT i =0;i<iTableCount;i++)
	{
		g_DayTipsAttrTbl.m_TableExt[i].m_DayNominate = ThirdFile.Search_Posistion(i,DayNominate)->iValue;	
		g_DayTipsAttrTbl.m_TableExt[i].m_MonthNominate = ThirdFile.Search_Posistion(i,MonthNominate)->iValue;
		g_DayTipsAttrTbl.m_TableExt[i].m_ShowAll = ThirdFile.Search_Posistion(i,ShowAll)->iValue;	

		SplitStringLevelOne oSplitL1;
		SplitStringLevelTwo oSplitL2;
		pSplitString	=	ThirdFile.Search_Posistion(i,ShowLevel)->pString;
	
		//�ɼ��ȼ�, ��Ҫ�����ַ���
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, g_DayTipsAttrTbl.m_TableExt[i].m_ShowLevel,MAX_LEVEL_NUM );

		//�ɼ�����
		pSplitString	=	ThirdFile.Search_Posistion(i,ShowCountry)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, g_DayTipsAttrTbl.m_TableExt[i].m_ShowCountry,MAX_COUNTRY_NUM );
		
		//�ɼ����, ��Ҫ�����ַ���
		pSplitString	=	ThirdFile.Search_Posistion(i,ShowGuild)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, g_DayTipsAttrTbl.m_TableExt[i].m_ShowGuild,GUILD_FAMILY_SIZE );
		
		//�ɼ�����
		pSplitString	=	ThirdFile.Search_Posistion(i,ShowFamily)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, g_DayTipsAttrTbl.m_TableExt[i].m_ShowFamily,GUILD_FAMILY_SIZE );

		g_DayTipsAttrTbl.m_TableExt[i].m_MisType = ThirdFile.Search_Posistion(i,MisType)->iValue;	
		strncpy( g_DayTipsAttrTbl.m_TableExt[i].m_MisName, ThirdFile.Search_Posistion(i,MisName)->pString, MAX_MIS_NAME_LEN );
			
		//������Ч�·�
		pSplitString	=	ThirdFile.Search_Posistion(i,MisValidMonth)->pString;
		ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString, g_DayTipsAttrTbl.m_TableExt[i].m_MisValidMonth,13 );

		g_DayTipsAttrTbl.m_TableExt[i].m_WeekOrDay = ThirdFile.Search_Posistion(i,WeekOrDay)->iValue;	

		pSplitString	=	ThirdFile.Search_Posistion(i,MisValidDate)->pString;
		if ( g_DayTipsAttrTbl.m_TableExt[i].m_WeekOrDay == 1 )
		{
			//������������
			ReadDayTipsData(&oSplitL1, &oSplitL2, pSplitString,
							g_DayTipsAttrTbl.m_TableExt[i].m_MisValidDate,32 );
		}
		else
		{
			//������������
			ReadDayTipsValidWeek(&oSplitL1, &oSplitL2, pSplitString,
								g_DayTipsAttrTbl.m_TableExt[i].m_MisValidWeek);
		}

		//�õ���ʼ�ͽ�����ʱ��
		pSplitString	=	ThirdFile.Search_Posistion(i,VailidTimeSlice)->pString;
		ReadDayTipsBeginAndEndTime(&oSplitL1, &oSplitL2, pSplitString,
									&g_DayTipsAttrTbl.m_TableExt[i].m_VailidTimeSlice,1);

		//���ű�λ��
		g_DayTipsAttrTbl.m_TableExt[i].m_MisPos = ThirdFile.Search_Posistion(i,MisPos)->iValue;
		g_DayTipsAttrTbl.m_TableExt[i].m_MisCount = ThirdFile.Search_Posistion(i,MisCount)->iValue;
		strncpy( g_DayTipsAttrTbl.m_TableExt[i].m_MisText, 
				ThirdFile.Search_Posistion(i,MisText)->pString, MAX_MIS_TEXT_LEN );
		strncpy( g_DayTipsAttrTbl.m_TableExt[i].m_NpcName, 
				ThirdFile.Search_Posistion(i,NpcName)->pString, NPC_NAME_LEN );
		g_DayTipsAttrTbl.m_TableExt[i].m_ScendId = ThirdFile.Search_Posistion(i,ScendId)->iValue;
		strncpy( g_DayTipsAttrTbl.m_TableExt[i].m_SceneName, 
				ThirdFile.Search_Posistion(i,SceneName)->pString, SCENE_NAME_LEN );
		g_DayTipsAttrTbl.m_TableExt[i].m_PosX = ThirdFile.Search_Posistion(i,PosX)->iValue;
		g_DayTipsAttrTbl.m_TableExt[i].m_PosZ = ThirdFile.Search_Posistion(i,PosZ)->iValue;

		g_DayTipsAttrTbl.m_TableExt[i].m_DoneCountIndex = ThirdFile.Search_Posistion( i, DoneCountIndex )->iValue;
		g_DayTipsAttrTbl.m_TableExt[i].m_DoneCircleIndex = ThirdFile.Search_Posistion( i, DoneCircleIndex )->iValue;		
		g_DayTipsAttrTbl.m_TableExt[i].m_LastDoneTimeIndex = ThirdFile.Search_Posistion( i, LastDoneTimeIndex )->iValue;
		g_DayTipsAttrTbl.m_TableExt[i].m_LastAbandonTimeIndex = ThirdFile.Search_Posistion( i, LastAbandonTimeIndex )->iValue;

		//��ʼ�����	
		g_DayTipsAttrTbl.m_TableExt[i].m_Used	= TRUE;

	}//for end

	Log::SaveLog( SERVER_LOGFILE, "Load DayTips.tab ... OK! " ) ;
	__LEAVE_FUNCTION

}//InitDapTipsAttrTable end


//��ʼ������ת�ĵ�ͼ��Ϣ
VOID TableInit::Init_MapInfoTable()
{
__ENTER_FUNCTION

	enum MapInfoAttrData
	{
		Index,
		SceneId,
		SceneName,
		DestPosX,			//�ɵ����Ŀ�ĵ�
		DestPosZ,
		SceneType,
		MinUserLevel,		//��ҽ��볡������С�ȼ�
		MinMapLevel,		//��ͼ�ϵ�NPC��С�ȼ�
		LetterIndex,		//ƴ������
	};

	DBCFile DBFile(0);
	BOOL ret = DBFile.OpenFromTXT( FILE_MAP_INFO_LIST );
	AssertEx(ret,FILE_MAP_INFO_LIST);

	INT iTableCount			=	DBFile.GetRecordsNum();
	INT iColumnCount		=	DBFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAX_MAP_INFO_NUM);
	g_MapInfoAttrTbl.m_Count	=	iTableCount;
	
	//�ļ��е�����Ӧ������������ţ��˶����Ƿ�ֹ������ļ��ı�
	Assert(MinMapLevel<iColumnCount);

	for (INT i = 0; i < iTableCount; ++i)
	{
		g_MapInfoAttrTbl.m_TableExt[i].m_Index	= DBFile.Search_Posistion(i,Index)->iValue;
		g_MapInfoAttrTbl.m_TableExt[i].m_SceneId		= DBFile.Search_Posistion(i,SceneId)->iValue;
		strncpy( g_MapInfoAttrTbl.m_TableExt[i].m_SceneName,
				DBFile.Search_Posistion(i,SceneName)->pString, SCENE_NAME_LEN );
		
		g_MapInfoAttrTbl.m_TableExt[i].m_DestPosX	= DBFile.Search_Posistion(i,DestPosX)->iValue;
		g_MapInfoAttrTbl.m_TableExt[i].m_DestPosZ	= DBFile.Search_Posistion(i,DestPosZ)->iValue;
		g_MapInfoAttrTbl.m_TableExt[i].m_SceneType	= DBFile.Search_Posistion(i,SceneType)->iValue;
		g_MapInfoAttrTbl.m_TableExt[i].m_MinUserLevel	= DBFile.Search_Posistion(i,MinUserLevel)->iValue;
		g_MapInfoAttrTbl.m_TableExt[i].m_MinMapLevel	= DBFile.Search_Posistion(i,MinMapLevel)->iValue;
		memcpy( &g_MapInfoAttrTbl.m_TableExt[i].m_LetterIndex,
				DBFile.Search_Posistion(i,LetterIndex)->pString, 1 );

	}

	g_MapInfoAttrTbl.m_Count = iTableCount;

__LEAVE_FUNCTION

}//Init_MapInfoTable end


//��ʼ��ϵͳ��ʱ���Ϣ
VOID TableInit::Init_TimerDoingTable()
{
__ENTER_FUNCTION

	enum TimerDoingAttrData
	{
		Index  = 0,
		szName,
		ScriptId,			//��Ӧ�Ľű�ID
		OperateType,		
		TimeType,	
    	StartTime,
		EndTime,
		Parameter0,
		Parameter1,
		Parameter2,
		Parameter3,
		Parameter4,
		BroadcastType,
		SlowReportInterval,
		SlowReportText,
		SlowReportEndTime,
		QuickReportInterval,
		QuickReportText,
		QuickReportEndTime,
    	EarlierReportInterval,
		EarlierReportText,
		EarlierReportEndTime,
    	MidReportInterval,
		MidReportText,
		MidReportEndTime,
    	LastReportInterval,
		LastReportText,
		LastReportEndTime,
		Ruler,
		TempMapId,
		Map0,
		Map1,
		Map2,
		Map3,
		Map4,
		Map5,
		Map6,
		Map7,
		Map8,
		Map9,
		//MapRatio[TIMER_DOING_MAP_NUM];
	};

	DBCFile DBFile(0);
	BOOL ret = DBFile.OpenFromTXT( FILE_TIMER_DOING_LIST );

	AssertEx(ret,FILE_TIMER_DOING_LIST);

	INT iTableCount			=	DBFile.GetRecordsNum();
	INT iColumnCount		=	DBFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAX_TIMER_DOING_NUM);

	//strnset((char*)&g_TiemrDoingAttrTbl,0,sizeof(g_TiemrDoingAttrTbl));
	memset(&g_TiemrDoingAttrTbl,0,sizeof(g_TiemrDoingAttrTbl));

	g_TiemrDoingAttrTbl.m_Count	=	iTableCount;
	
	//�ļ��е�����Ӧ������������ţ��˶����Ƿ�ֹ������ļ��ı�
	Assert(Map9<iColumnCount);

	const CHAR* pSplitString = NULL;

	for (INT i = 0; i < iTableCount; ++i)
	{
		g_TiemrDoingAttrTbl.m_TableExt[i].m_Index	= DBFile.Search_Posistion(i,Index)->iValue;
		strncpy( g_TiemrDoingAttrTbl.m_TableExt[i].m_szName,
				DBFile.Search_Posistion(i,szName)->pString, TIMER_DOING_NAME_LEN );
		
		g_TiemrDoingAttrTbl.m_TableExt[i].m_ScriptId	= DBFile.Search_Posistion(i,ScriptId)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_OperateType	= DBFile.Search_Posistion(i,OperateType)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_TimeType	= DBFile.Search_Posistion(i,TimeType)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_StartTime	= DBFile.Search_Posistion(i,StartTime)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_EndTime		= DBFile.Search_Posistion(i,EndTime)->iValue;
	
		INT j=0;
		for (j=0;j<PARAMETER_NUM;j++)
		{	
			g_TiemrDoingAttrTbl.m_TableExt[i].m_Parameter[j] = DBFile.Search_Posistion(i,Parameter0+j)->iValue;
		}

		//�㲥���ͣ���Ҫ����ַ���
		SplitStringLevelOne oSplitL1;
		SplitStringLevelTwo oSplitL2;
		pSplitString	=	DBFile.Search_Posistion(i,BroadcastType)->pString;

		for ( j=0;j<CHAR_SHOW_POS_NUM;j++ )
			g_TiemrDoingAttrTbl.m_TableExt[i].m_BroadcastType[j] = -1;

		ReadTiemrDoingData(&oSplitL1, &oSplitL2, pSplitString, 
						   g_TiemrDoingAttrTbl.m_TableExt[i].m_BroadcastType,CHAR_SHOW_POS_NUM );
	
		g_TiemrDoingAttrTbl.m_TableExt[i].m_SlowReportInterval	= DBFile.Search_Posistion(i,SlowReportInterval)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_SlowReportInterval *= 1000;
		strncpy( g_TiemrDoingAttrTbl.m_TableExt[i].m_SlowReportText,
				DBFile.Search_Posistion(i,SlowReportText)->pString, REPORT_TEXT_LEN );
		g_TiemrDoingAttrTbl.m_TableExt[i].m_SlowReportEndTime	= DBFile.Search_Posistion(i,SlowReportEndTime)->iValue;

		g_TiemrDoingAttrTbl.m_TableExt[i].m_QuickReportInterval	= DBFile.Search_Posistion(i,QuickReportInterval)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_QuickReportInterval *= 1000;
		strncpy( g_TiemrDoingAttrTbl.m_TableExt[i].m_QuickReportText,
				DBFile.Search_Posistion(i,QuickReportText)->pString, REPORT_TEXT_LEN );
		g_TiemrDoingAttrTbl.m_TableExt[i].m_QuickReportEndTime	= DBFile.Search_Posistion(i,QuickReportEndTime)->iValue;

		g_TiemrDoingAttrTbl.m_TableExt[i].m_EarlierReportInterval	= DBFile.Search_Posistion(i,EarlierReportInterval)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_EarlierReportInterval *= 1000;
		strncpy( g_TiemrDoingAttrTbl.m_TableExt[i].m_EarlierReportText,
				DBFile.Search_Posistion(i,EarlierReportText)->pString, REPORT_TEXT_LEN );
		g_TiemrDoingAttrTbl.m_TableExt[i].m_EarlierReportEndTime	= DBFile.Search_Posistion(i,EarlierReportEndTime)->iValue;

		g_TiemrDoingAttrTbl.m_TableExt[i].m_MidReportInterval	= DBFile.Search_Posistion(i,MidReportInterval)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_MidReportInterval *= 1000;
		strncpy( g_TiemrDoingAttrTbl.m_TableExt[i].m_MidReportText,
				DBFile.Search_Posistion(i,MidReportText)->pString, REPORT_TEXT_LEN );
		g_TiemrDoingAttrTbl.m_TableExt[i].m_MidReportEndTime	= DBFile.Search_Posistion(i,MidReportEndTime)->iValue;

		g_TiemrDoingAttrTbl.m_TableExt[i].m_LastReportInterval	= DBFile.Search_Posistion(i,LastReportInterval)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_LastReportInterval *= 1000;	
		strncpy( g_TiemrDoingAttrTbl.m_TableExt[i].m_LastReportText,
				DBFile.Search_Posistion(i,LastReportText)->pString, REPORT_TEXT_LEN );
		g_TiemrDoingAttrTbl.m_TableExt[i].m_LastReportEndTime	= DBFile.Search_Posistion(i,LastReportEndTime)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_Ruler		= DBFile.Search_Posistion(i,Ruler)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_TempMapId	= DBFile.Search_Posistion(i,TempMapId)->iValue;
		g_TiemrDoingAttrTbl.m_TableExt[i].m_RatioTotal = 0;

		//Map��Ϣ����Ҫ����ַ���
		for (j=0;j<TIMER_DOING_MAP_NUM;j++)
		{
			pSplitString	=	DBFile.Search_Posistion(i,Map0+j)->pString;
			ReadTiemrDoingMapInfo(&oSplitL1, &oSplitL2, pSplitString, 
							   &g_TiemrDoingAttrTbl.m_TableExt[i].m_MapId[j],
							   &g_TiemrDoingAttrTbl.m_TableExt[i].m_MapRatio[j],
							   2 );

			g_TiemrDoingAttrTbl.m_TableExt[i].m_RatioTotal += g_TiemrDoingAttrTbl.m_TableExt[i].m_MapRatio[j] ;
		}

		//�������Ӧ��ʱ���¼���m_Ruler�ŵ�ͼ
		if ( g_TiemrDoingAttrTbl.m_TableExt[i].m_Ruler > 0 )
		{
			INT MapId[TIMER_DOING_MAP_NUM] = {0};
			memcpy(MapId,g_TiemrDoingAttrTbl.m_TableExt[i].m_MapId,sizeof(INT) * TIMER_DOING_MAP_NUM);
			
			INT MapRatio[TIMER_DOING_MAP_NUM] = {0};
			memcpy(MapRatio,g_TiemrDoingAttrTbl.m_TableExt[i].m_MapRatio,sizeof(INT) * TIMER_DOING_MAP_NUM);
			
			//���m_MapId
			for (j=0;j<TIMER_DOING_MAP_NUM;j++)
			{
				g_TiemrDoingAttrTbl.m_TableExt[i].m_MapId[j] = INVALID_ID;
			}

			for ( INT cnt=0;cnt<g_TiemrDoingAttrTbl.m_TableExt[i].m_Ruler;cnt++)
			{
				//�ҵ������
				INT Max = MapRatio[0];
				for (j=1;j<TIMER_DOING_MAP_NUM;j++)
				{
					if ( MapRatio[j] > Max )
					{
						Max = MapRatio[j];
					}
				}

				INT nRet = rand() % Max;
				for (j=TIMER_DOING_MAP_NUM-1;j>=0;j--)
				{
					if ( nRet <= MapRatio[j] )
					{
						g_TiemrDoingAttrTbl.m_TableExt[i].m_MapId[cnt] = MapId[j];

						if ( j< TIMER_DOING_MAP_NUM-1 )
						{
							memcpy(&MapId[j],&MapId[j+1],(TIMER_DOING_MAP_NUM-1-j) * sizeof(INT) );
							memcpy(&MapRatio[j],&MapRatio[j+1],(TIMER_DOING_MAP_NUM-1-j) * sizeof(INT));	
						}
						else
						{
							MapId[j] = INVALID_ID;
							MapRatio[j] = INVALID_ID;
						}
						break;					
					}
				}			
			}

		}
	}

	g_TiemrDoingAttrTbl.m_Count = iTableCount;

__LEAVE_FUNCTION

}//Init_TimerDoingTable end


VOID TableInit::InitXunHunMissionAttrTable()
{
	__ENTER_FUNCTION

	enum XunHunMissionAttrData
	{
		MissionGroup		=	1,				//������
		SubMissionType,							//����������
		SubMissionId,							//������ID
		ObjId,									//Ҫɱ�Ĺ��������ʼ�NPC��ID������ƷID
		ObjName,								//����/NPC/ITEM��			
		ObjCount,								//Ҫɱ�Ĺ�������
		ScendId,
		SceneName,
		PosX,
		PosZ,
		DialogText,				//NPC�Ի�����
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_XUNHUAN_MISSION );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAX_XUNHUAN_MISSION_GROUP_NUMBER*GROUP_IN_SUB_MISSION_NUMBER);
	g_XunHuanMissionAttrTbl.m_Count	=	iTableCount;
	
	const CHAR* pSplitString = NULL;

	//�ļ��е�����Ӧ������������ţ��˶����Ƿ�ֹ������ļ��ı�
	Assert(DialogText<iColumnCount);
	INT ArrIndex[MAX_XUNHUAN_MISSION_GROUP_NUMBER];
	memset(ArrIndex,0,sizeof(INT)*MAX_XUNHUAN_MISSION_GROUP_NUMBER);

	for(INT i =0;i<iTableCount;i++)
	{
		INT group = ThirdFile.Search_Posistion(i,MissionGroup)->iValue;

		Assert(group>=0&&group<MAX_XUNHUAN_MISSION_GROUP_NUMBER);
		Assert(ArrIndex[group]<GROUP_IN_SUB_MISSION_NUMBER);
	
		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_Group = group;

		INT type = ThirdFile.Search_Posistion(i,SubMissionType)->iValue;
		INT id = ThirdFile.Search_Posistion(i,SubMissionId)->iValue;
		AssertEx(type>=0&&type<SUB_MISSION_TYPE_NUMBER,"ѭ�����������ͳ�����Χ");

		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_SubMissionType = type;
		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_SubMissionId = id;

		if (g_XunHuanMissionAttrTbl.m_SubMissionInfo[group][type].m_SubMissionCnt == 0)
			g_XunHuanMissionAttrTbl.m_SubMissionInfo[group][type].M_SubMissionPos = id;
		g_XunHuanMissionAttrTbl.m_SubMissionInfo[group][type].m_SubMissionCnt++;

		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_ObjId = ThirdFile.Search_Posistion(i,ObjId)->iValue;
		strncpy( g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_ObjName, ThirdFile.Search_Posistion(i,ObjName)->pString, sizeof( g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_ObjName ) - 1 );	
		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_ObjCount = ThirdFile.Search_Posistion(i,ObjCount)->iValue;
		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_ScendId = ThirdFile.Search_Posistion(i,ScendId)->iValue;
		strncpy( g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_SceneName, ThirdFile.Search_Posistion(i,SceneName)->pString, sizeof( g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_SceneName ) - 1 );	
		
		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_PosX = ThirdFile.Search_Posistion(i,PosX)->iValue;
		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_PosZ = ThirdFile.Search_Posistion(i,PosZ)->iValue;
		strncpy( g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_DialogText, ThirdFile.Search_Posistion(i,DialogText)->pString, sizeof( g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_DialogText ) - 1 );
	
		//��ʼ�����	
		g_XunHuanMissionAttrTbl.m_TableExt[group][ArrIndex[group]].m_Used	= TRUE;

		ArrIndex[group]++;
	
	}

	Log::SaveLog( SERVER_LOGFILE, "Load xunhun_shimen.tab ... OK! " ) ;
	__LEAVE_FUNCTION
}

VOID TableInit::InitHorseAttrTable()
{
	__ENTER_FUNCTION

	enum HorseAttrData
	{
		HorseDataID = 0,						// ����ID	
		HorseName,								// ����
		HorseLevel,								// ��ʼ�ȼ�
		HorseTakeLevel,							// ��Я���ȼ�
		HorseReturnToChildID,					// ��ͯ���ID
		HorseChildID,							// �����ID
		HorseCanMating,							// �Ƿ���Խ���
		HorseIsVariation,						// �Ƿ����
		HorseGeneration,						// ��ֳ����
		HorseAttach,							// ��˵Ĺ���
		HorseLifeNoMaster,						// ����ʱ����������
		HorseStrengthMin,						// ��ʼ��С����
		HorseStrengthMax,						// ��ʼ�������
		HorseSmartnessMin,						// ��ʼ��С����
		HorseSmartnessMax,						// ��ʼ�������
		HorseMindMin,							// ��ʼ��С����
		HorseMindMax,							// ��ʼ�������
		HorseConstitutionMin,					// ��ʼ��С����
		HorseConstitutionMax,					// ��ʼ�������
		HorseStrengthMinPer,					// ��ʼ��С��������
		HorseStrengthMaxPer,					// ��ʼ�����������
		HorseSmartnessMinPer,					// ��ʼ��С��������
		HorseSmartnessMaxPer,					// ��ʼ�����������
		HorseMindMinPer,						// ��ʼ��С��������	
		HorseMindMaxPer,						// ��ʼ�����������
		HorseConstitutionMinPer,				// ��ʼ��С��������	
		HorseConstitutionMaxPer,				// ��ʼ�����������	
		HorseSavvyMin,							// ��ʼ��С����	
		HorseSavvyMax,							// ��ʼ�������	
		HorseGenGuMin,							// ��ʼ��С����	
		HorseGenGuMax,							// ��ʼ������	
		HorsePullulationRateMin,				// ��ʼ��С�ɳ���	
		HorsePullulationRateMax,				// ��ʼ���ɳ���
		HorseMoveSpeed,							// �ƶ��ٶ�
		HorseBehaveID,							// ��Ӧ����ID
		HorseMountID,							// ��������е�ID
		HorseInitSkillCount,					// ���ʼ���ܸ������������ĸ��ӣ�ǰ6��
		HorseInitSkillTbl,						// ���ʼ�����б�
		HorseSkillRealizationID=HorseInitSkillTbl+6,				// ��������ID
		HorseDataCount
	};
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_HORSE_ATTR );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount>0 && iTableCount<MAX_HORSE_TYPE && "../Server/Config/qichengjichu.tab");
	g_HorseAttrTbl.m_iCount	=	iTableCount;

	const CHAR* pSplitString = NULL;
	SplitStringLevelOne oSplitL1;
	SplitStringLevelTwo oSplitL2;


	//�˶����Ƿ�ֹ������ļ��ı�
	Assert(HorseDataCount == iColumnCount);
	
	for( INT i=0; i<iTableCount; ++i )
	{
		//������
		INT iType = ThirdFile.Search_Posistion(i,	HorseDataID)->iValue;

		if( iType>=MAX_HORSE_TYPE )
		{
			AssertEx(FALSE,"�������д����ID�ų�����Χ") ;
			continue ;
		}
		g_HorseAttrTbl.m_aHorseTbl[iType].m_nDataID				= iType;
		strncpy( g_HorseAttrTbl.m_aHorseTbl[iType].m_sName,	ThirdFile.Search_Posistion(i,	HorseName)->pString, sizeof(g_HorseAttrTbl.m_aHorseTbl[iType].m_sName)-1 );
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iLevel				= ThirdFile.Search_Posistion(i,	HorseLevel)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iTakeLevel			= ThirdFile.Search_Posistion(i,	HorseTakeLevel)->iValue;
		pSplitString = ThirdFile.Search_Posistion(i,	HorseReturnToChildID)->pString;
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iReturnToChildIDCount = 
			CommonStringSplit( &oSplitL1, &oSplitL2, pSplitString, MaxReturnToChildIDNum, g_HorseAttrTbl.m_aHorseTbl[iType].m_aReturnToChildID );

		pSplitString = ThirdFile.Search_Posistion(i,	HorseChildID)->pString;
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iChildIDCount = 
			CommonStringSplit( &oSplitL1, &oSplitL2, pSplitString, MaxReturnToChildIDNum, g_HorseAttrTbl.m_aHorseTbl[iType].m_aChildID );

		g_HorseAttrTbl.m_aHorseTbl[iType].m_bCanMating 			= ThirdFile.Search_Posistion(i,	HorseCanMating)->iValue;
		g_HorseAttrTbl.m_aHorseTbl[iType].m_bVariation			= ThirdFile.Search_Posistion(i,	HorseIsVariation)->iValue;
		g_HorseAttrTbl.m_aHorseTbl[iType].m_byGeneration 		= ThirdFile.Search_Posistion(i,	HorseGeneration)->iValue;			
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iAttach 			= ThirdFile.Search_Posistion(i,	HorseAttach)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_LifeNoMaster		= ThirdFile.Search_Posistion(i,	HorseLifeNoMaster)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iStrengthMin 		= ThirdFile.Search_Posistion(i,	HorseStrengthMin)->iValue;			
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iStrengthMax 		= ThirdFile.Search_Posistion(i,	HorseStrengthMax)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iSmartnessMin 		= ThirdFile.Search_Posistion(i,	HorseSmartnessMin)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iSmartnessMax 		= ThirdFile.Search_Posistion(i,	HorseSmartnessMax)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iMindMin 			= ThirdFile.Search_Posistion(i,	HorseMindMin)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iMindMax 			= ThirdFile.Search_Posistion(i,	HorseMindMax)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iConstitutionMin 	= ThirdFile.Search_Posistion(i,	HorseConstitutionMin)->iValue;			
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iConstitutionMax 	= ThirdFile.Search_Posistion(i,	HorseConstitutionMax)->iValue;			
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iStrengthMinPer 	= ThirdFile.Search_Posistion(i,	HorseStrengthMinPer)->iValue;			
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iStrengthMaxPer 	= ThirdFile.Search_Posistion(i,	HorseStrengthMaxPer)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iSmartnessMinPer 	= ThirdFile.Search_Posistion(i,	HorseSmartnessMinPer)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iSmartnessMaxPer 	= ThirdFile.Search_Posistion(i,	HorseSmartnessMaxPer)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iMindMinPer 		= ThirdFile.Search_Posistion(i,	HorseMindMinPer)->iValue;				
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iMindMaxPer			= ThirdFile.Search_Posistion(i,	HorseMindMaxPer)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iConstitutionMinPer = ThirdFile.Search_Posistion(i,	HorseConstitutionMinPer)->iValue;			
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iConstitutionMaxPer = ThirdFile.Search_Posistion(i,	HorseConstitutionMaxPer)->iValue;			
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iSavvyMin 			= ThirdFile.Search_Posistion(i,	HorseSavvyMin)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iSavvyMax 			= ThirdFile.Search_Posistion(i,	HorseSavvyMax)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_GenGuMin 			= ThirdFile.Search_Posistion(i,	HorseGenGuMin)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_GenGuMax 			= ThirdFile.Search_Posistion(i,	HorseGenGuMax)->iValue;					
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iPullulationRateMin	= ThirdFile.Search_Posistion(i,	HorsePullulationRateMin)->iValue; 		
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iPullulationRateMax = ThirdFile.Search_Posistion(i,	HorsePullulationRateMax)->iValue;	
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iMoveSpeed			= ThirdFile.Search_Posistion(i,	HorseMoveSpeed)->iValue;	
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iBehaveID 			= ThirdFile.Search_Posistion(i,	HorseBehaveID)->iValue;	
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iMountID			= ThirdFile.Search_Posistion(i,	HorseMountID)->iValue;	
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iSkillSlotCount		= ThirdFile.Search_Posistion(i,	HorseInitSkillCount)->iValue;
		Assert( g_HorseAttrTbl.m_aHorseTbl[iType].m_iSkillSlotCount<=6 && "InitHorseAttrTable���ʼ���ܸ�����" );
		for( INT iSlot=0; iSlot<g_HorseAttrTbl.m_aHorseTbl[iType].m_iSkillSlotCount; ++iSlot )
		{
			//���ܲ�, ��Ҫ�����ַ���
			pSplitString	=	ThirdFile.Search_Posistion(i,HorseInitSkillTbl+iSlot)->pString;
			ReadHorseSkillSlot(&oSplitL1, &oSplitL2, pSplitString, g_HorseAttrTbl.m_aHorseTbl[iType].m_oSkillSlot[iSlot] );
		}
		g_HorseAttrTbl.m_aHorseTbl[iType].m_iRealizationID = ThirdFile.Search_Posistion(i,	HorseSkillRealizationID)->iValue;
	}
	__LEAVE_FUNCTION
}

INT CompareOfHorseItemTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

		UINT	Serial1,Serial2;

	Serial1	= ((_HORSE_ITEM*)pArg1)->m_iItemID;
	Serial2	= ((_HORSE_ITEM*)pArg2)->m_iItemID;

	if(Serial1>Serial2)
		return 1;
	else if(Serial1<Serial2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
		return 0 ;
}

_HORSE_ITEM* _HORSE_ITEM_TBL::GetHorseItemAttr( INT iHorseItemID )
{
	__ENTER_FUNCTION
	_HORSE_ITEM tb;
	tb.m_iItemID		=	iHorseItemID;

	_HORSE_ITEM* pResult = (_HORSE_ITEM*)bsearch(&tb,
		g_HorseItemTbl.m_aHorseItem,
		g_HorseItemTbl.m_iCount,
		sizeof(_HORSE_ITEM),
		(INT (*)(const VOID*, const VOID*))CompareOfHorseItemTB);
	if(pResult)
	{
		return	pResult;
	}
	else
	{
		return NULL;
	}
	__LEAVE_FUNCTION
	return NULL;
}

//��ʼ������Ʒ��
VOID TableInit::InitHorseItemTable()
{
	__ENTER_FUNCTION

	enum HorseItemData
	{
		HorseItemID				= 0,
		//��������е�����
		HorseItemType			= 2,
		HorseAttrChangePercent,
		HorseAttrChangePoint,
		HorseStudySkillID,
		HorseSuccessRate,
		HorseItemDataCount
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_HORSE_ITEM );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount>0 && iTableCount<MAX_HORSE_ITEM && "../Public/Config/qichengdaoju.tab");
	g_HorseItemTbl.m_iCount	=	iTableCount;

	//�˶����Ƿ�ֹ������ļ��ı�
	Assert(HorseItemDataCount == iColumnCount && "../Public/Config/qichengdaoju.tab�����ı�" );

	for( INT i=0; i<iTableCount; ++i )
	{
		//������
		g_HorseItemTbl.m_aHorseItem[i].m_iItemID			= ThirdFile.Search_Posistion(i,	HorseItemID)->iValue;
		g_HorseItemTbl.m_aHorseItem[i].m_iItemType			= ThirdFile.Search_Posistion(i,	HorseItemType)->iValue;
		g_HorseItemTbl.m_aHorseItem[i].m_iAttrChangePercent	= ThirdFile.Search_Posistion(i,	HorseAttrChangePercent)->iValue;
		g_HorseItemTbl.m_aHorseItem[i].m_iAttrChangePoint	= ThirdFile.Search_Posistion(i,	HorseAttrChangePoint)->iValue;
		g_HorseItemTbl.m_aHorseItem[i].m_iStudySkillID		= ThirdFile.Search_Posistion(i,	HorseStudySkillID)->iValue;
		g_HorseItemTbl.m_aHorseItem[i].m_iSuccessRate		= ThirdFile.Search_Posistion(i,	HorseSuccessRate)->iValue;
	}
	__LEAVE_FUNCTION	
}

VOID TableInit::InitHorseDropTable()
{
	__ENTER_FUNCTION

	enum HorseDropData
	{
		HorseDropID = 0,
		HorseMonsterImportance,
		HorseMaxDropNum,
		HorseDropGroup,
		HorseDropDataCount = HorseDropGroup + 3 * MaxHorseDropGroupNum
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_HORSE_DROP );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount>0 && iTableCount<MAX_HORSE_DROP && "qichengdrop_content.tab");
	g_HorseDrop.m_iCount	=	iTableCount;

	//�˶����Ƿ�ֹ������ļ��ı�
	Assert(HorseDropDataCount == iColumnCount && "../Public/Config/qichengdrop_content.tab�����ı�" );

	const CHAR* pSplitString = NULL;
	SplitStringLevelOne oSplitL1;
	SplitStringLevelTwo oSplitL2;

	for( INT i=0; i<iTableCount; ++i )
	{
		INT iIndex = ThirdFile.Search_Posistion(i,	HorseDropID)->iValue;
		g_HorseDrop.m_aHorse_drop[iIndex].m_iDropID			= iIndex;
		g_HorseDrop.m_aHorse_drop[iIndex].m_iMonsterImportance = ThirdFile.Search_Posistion(i,	HorseMonsterImportance)->iValue;
		g_HorseDrop.m_aHorse_drop[iIndex].m_iMaxDropNum		= ThirdFile.Search_Posistion(i,	HorseMaxDropNum)->iValue;
		for( INT iGroup=0; iGroup<MaxHorseDropGroupNum; ++iGroup)
		{
			g_HorseDrop.m_aHorse_drop[iIndex].m_aDropGroup[iGroup].m_iGroupImportance   = ThirdFile.Search_Posistion(i, HorseDropGroup+iGroup*3)->iValue;
			g_HorseDrop.m_aHorse_drop[iIndex].m_aDropGroup[iGroup].m_iGroupMaxDropTimes = ThirdFile.Search_Posistion(i, HorseDropGroup+iGroup*3+1)->iValue;
			pSplitString = ThirdFile.Search_Posistion(i, HorseDropGroup+iGroup*3+2)->pString;
			g_HorseDrop.m_aHorse_drop[iIndex].m_aDropGroup[iGroup].m_iHorseIDCount =
			CommonStringSplit( &oSplitL1, &oSplitL2, pSplitString, MaxReturnToChildIDNum, g_HorseDrop.m_aHorse_drop[iIndex].m_aDropGroup[iGroup].m_aGroupHorseID );
		}
	}
	__LEAVE_FUNCTION	
}

//��ʼ�����������
VOID TableInit::InitHorseRealTable()
{
	__ENTER_FUNCTION

	enum HorseSkillRealData
	{
		HorseRealDataID = 0,					// ����������ID	
		HorseDataCount,							// ��ǰ����ID�µļ��ܲ۸���
		HorseSkillSlot,							// ���ܲ�
		HorseSkillDataCount=8
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_HORSE_SKILL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount>0 && iTableCount<MAX_HORSE_SKILL && "../Server/Config/qichengSkill.tab");
	g_HorseSkillTbl.m_iCount	=	iTableCount;

	const CHAR* pSplitString = NULL;

	//�˶����Ƿ�ֹ������ļ��ı�
	Assert(HorseSkillDataCount == iColumnCount);

	for( INT i=0; i<iTableCount; ++i )
	{
		//������
		INT iType = ThirdFile.Search_Posistion(i,	HorseRealDataID)->iValue;

		if( iType>=MAX_HORSE_SKILL )
		{
			AssertEx(FALSE,"���ܱ�����д��ID�ų�����Χ") ;
			continue ;
		}
		g_HorseSkillTbl.m_aHorseSkillTbl[iType].m_iSkillRealizationID			= iType;
		g_HorseSkillTbl.m_aHorseSkillTbl[iType].m_iSkillSlotCount 				= ThirdFile.Search_Posistion(i,	HorseDataCount)->iValue;
		Assert( g_HorseSkillTbl.m_aHorseSkillTbl[iType].m_iSkillSlotCount <= MaxSlotNum && "���ܲ�������������" );
		if( g_HorseSkillTbl.m_aHorseSkillTbl[iType].m_iSkillSlotCount > MaxSlotNum )
			g_HorseSkillTbl.m_aHorseSkillTbl[iType].m_iSkillSlotCount = MaxSlotNum ;

		SplitStringLevelOne oSplitL1;
		SplitStringLevelTwo oSplitL2;
		for( INT iSlot=0; iSlot<6; ++iSlot )
		{
			//���ܲ�, ��Ҫ�����ַ���
			pSplitString	=	ThirdFile.Search_Posistion(i,HorseSkillSlot+iSlot)->pString;
			ReadHorseSkillSlot(&oSplitL1, &oSplitL2, pSplitString, g_HorseSkillTbl.m_aHorseSkillTbl[iType].m_oSkillSlot[iSlot] );
		}
	}
	__LEAVE_FUNCTION

}

INT TableInit::CommonStringSplit
	(	
	SplitStringLevelOne*	pSplitL1, 
	SplitStringLevelTwo*	pSplitL2, 
	const CHAR*				pSplitString, 
	INT						iMaxNum,
	INT*					pArry
	)
{
	__ENTER_FUNCTION

	pSplitL1->Reset();
	pSplitL2->Reset();
	pSplitL1->Init( '|', pSplitL2 );
	pSplitL2->Init( '~', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iLineCount	= pSplitL2->GetResultLineCount();

	Assert( iMaxNum >= iLineCount );
	for( INT i=0; i<iLineCount; ++i )
	{
		pArry[i] = atoi(pSplitL2->GetResultLine(i));
	}
	return iLineCount;
	__LEAVE_FUNCTION
	return 0;
}


VOID TableInit::ReadHorseSkillSlot
	( 
		SplitStringLevelOne* pSplitL1, 
		SplitStringLevelTwo* pSplitL2, 
		const CHAR* pSplitString, 
		_HORSE_SKILL_SLOT& oSlot
	)
{
	__ENTER_FUNCTION

	pSplitL1->Reset();
	pSplitL2->Reset();
	pSplitL1->Init( '|', pSplitL2 );
	pSplitL2->Init( '~', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iLineCount	= pSplitL2->GetResultLineCount();

	Assert( MaxSlotSkillNum >= iLineCount );
	oSlot.m_iSkillCount = iLineCount;
	for( INT iSkill=0; iSkill<iLineCount; ++iSkill )
	{
		oSlot.m_aSkillSlot[iSkill] = atoi(pSplitL2->GetResultLine(iSkill));
	}
	__LEAVE_FUNCTION

}

//��ȡ�ƺ����ñ�
VOID TableInit::InitTitleInfoTable()
{
    __ENTER_FUNCTION

    enum TitleData
    {
        TitileID                = 0,            //�ƺ�ID
        TitleType,                              //�ƺ�����
        TitleNameMale           = 4,            //���Գƺ�����
        TitleNameFemale,                        //Ů�Գƺ�����
        TitleTime               = 7,            //�ƺ�ʱ��
        TitleBuff               = 9,            //�ƺ�Ч��
        TitleSuitID             = 11,           //��ϳƺ���װID
        TitleDataCount,
    };

    DBCFile ThirdFile(0);
    BOOL ret = ThirdFile.OpenFromTXT( FILE_TITLE_INFO );

    INT iTableCount			=	ThirdFile.GetRecordsNum();
    INT iColumnCount		=	ThirdFile.GetFieldsNum();

    Assert(iTableCount>0 && iTableCount< MAX_TITLE_SIZE);

    for( INT i=0; i<iTableCount; ++i )
    {
        //�ƺ�ID
        INT iID = ThirdFile.Search_Posistion(i,	TitileID)->iValue;

        if( iID >= MAX_TITLE_SIZE )
        {
            AssertEx(FALSE,"�ƺ�ID�����ƺ����ޣ�") ;
            continue ;
        }

        g_TitleInfoTbl.m_TableInfo[iID].m_iTitleID     = iID;      //�ƺ�ID
        g_TitleInfoTbl.m_TableInfo[iID].m_iTitleType   = ThirdFile.Search_Posistion(i,	TitleType)->iValue;
        g_TitleInfoTbl.m_TableInfo[iID].m_uTime        = ThirdFile.Search_Posistion(i,	TitleTime)->iValue;
        g_TitleInfoTbl.m_TableInfo[iID].m_iBuffID      = ThirdFile.Search_Posistion(i,	TitleBuff)->iValue;
        g_TitleInfoTbl.m_TableInfo[iID].m_iSuitID      = ThirdFile.Search_Posistion(i,	TitleSuitID)->iValue;
    }

    __LEAVE_FUNCTION

}

//��ȡ��ϳƺű�
VOID TableInit::InitTitleCombinationTable()
{
    __ENTER_FUNCTION

    enum TitleData
    {
        Group_ID                = 0,            //��ID
        TitileID,                               //��ϳƺ�ID
        ID_1,                                   //�ƺ�ID_1
        ID_2,                                   //�ƺ�ID_2
        ID_3,                                   //�ƺ�ID_3
        ID_4,                                   //�ƺ�ID_4
        ID_5,                                   //�ƺ�ID_5
        ID_6,                                   //�ƺ�ID_6
        ID_7,                                   //�ƺ�ID_7
        ID_8,                                   //�ƺ�ID_8
        ID_9,                                   //�ƺ�ID_9
        ID_10,                                  //�ƺ�ID_10
    };

    DBCFile ThirdFile(0);
    BOOL ret = ThirdFile.OpenFromTXT( FILE_TITLE_COMBINATION );

    INT iTableCount			=	ThirdFile.GetRecordsNum();
    INT iColumnCount		=	ThirdFile.GetFieldsNum();

    Assert(iTableCount>0 && iTableCount< MAX_TITLE_SIZE);

    for( INT i=0; i<iTableCount; ++i )
    {
        //��ID
        INT iID = ThirdFile.Search_Posistion(i,	Group_ID)->iValue;

        if( iID >= MAX_TITLE_SIZE )
        {
            AssertEx(FALSE,"��ID�������ޣ�") ;
            continue ;
        }

        g_TitleComTbl.m_TableCombination[iID].m_iGroupID        = iID;
        g_TitleComTbl.m_TableCombination[iID].m_comTitleID      = ThirdFile.Search_Posistion(i,	TitileID)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[0]   = ThirdFile.Search_Posistion(i,	ID_1)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[1]   = ThirdFile.Search_Posistion(i,	ID_2)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[2]   = ThirdFile.Search_Posistion(i,	ID_3)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[3]   = ThirdFile.Search_Posistion(i,	ID_4)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[4]   = ThirdFile.Search_Posistion(i,	ID_5)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[5]   = ThirdFile.Search_Posistion(i,	ID_6)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[6]   = ThirdFile.Search_Posistion(i,	ID_7)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[7]   = ThirdFile.Search_Posistion(i,	ID_8)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[8]   = ThirdFile.Search_Posistion(i,	ID_9)->iValue;
        g_TitleComTbl.m_TableCombination[iID].m_arPart[9]   = ThirdFile.Search_Posistion(i,	ID_10)->iValue;
    }

    __LEAVE_FUNCTION
}



VOID TableInit::InitMonsterAttrExTable( )
{//��ȡ����ϵ���������
	__ENTER_FUNCTION
	enum MonsterAttrData
	{
		MonsterType								= 0,		//������
		MonsterName,										//����
		MonsterLevel,										//�ȼ�
		MonsterRespawnTime,									//ˢ������
		MonsterMinDamagePercent,							//��С��Ѫ�ٷֱ�
		MonsterExp,											//����������
		Monster_CATTR_LEVEL2_ATTACKRATE_NEAR,				//���̹���
		Monster_CATTR_LEVEL2_ATTACKRATE_FAR,				//Զ�̹���
		Monster_CATTR_LEVEL2_ATTACKRATE_MAGIC_NEAR,			//�����ڹ�����
		Monster_CATTR_LEVEL2_ATTACKRATE_MAGIC_FAR,			//�����⹦����
		Monster_CATTR_LEVEL2_DEFENCE_NEAR,					//���̷���
		Monster_CATTR_LEVEL2_DEFENCE_FAR		= 11,		//Զ�̷���
		Monster_CATTR_LEVEL2_DEFENCE_MAGIC_NEAR,			//�����ڹ�����
		Monster_CATTR_LEVEL2_DEFENCE_MAGIC_FAR,				//�����⹦����
		Monster_CATTR_LEVEL2_MAXHP,							//HP����
		Monster_CATTR_LEVEL2_MAXMP,							//MP����
		Monster_CATTR_LEVEL2_RESTOREHP,						//HP�ظ�
		Monster_CATTR_LEVEL2_RESTOREMP,						//MP�ظ�
		Monster_CATTR_LEVEL2_HIT,							//������
		Monster_CATTR_LEVEL2_MISS,							//����
		Monster_CATTR_LEVEL2_CRITRATE,						//������
		Monster_CATTR_LEVEL2_TOUGHNESS,						//����
		MonsterAttrAttack1,									//���Թ���1
		MonsterAttrAttack2,									//���Թ���2
		MonsterAttrAttack3,									//���Թ���3
		MonsterAttrAttack4,									//���Թ���4
		MonsterAttrAttack5,									//���Թ���5
		MonsterAttrDefence1,								//���Է���1
		MonsterAttrDefence2,								//���Է���2
		MonsterAttrDefence3,								//���Է���3
		MonsterAttrDefence4,								//���Է���4
		MonsterAttrDefence5,								//���Է���5
		MonsterScriptID,									//���ýű�ID
		//����server���账���7��
		MonsterAttackAnimTime					= 40,		//��������ʱ��
		MonsterAttackCooldownTime,							//������ȴʱ��
		MonsterOutLook,										//���м��ʱ��
		Monster_CATTR_LEVEL2_SPEED,							//�ƶ��ٶ�
		MonsterWalkSpeed,									//�����ٶ�
		Monster_CATTR_LEVEL2_ATTACKSPEED,					//�����ٶ�
		//��Ҫ�����ַ���
		MonsterWeaponID,									//��������ID
		//��Ҫ�����ַ���
		MonsterHorseID,										//�����ID
		MonsterBaseAI,										//����AI
		MonsterExtAI,										//��չAI
		MonsterCamp,										//��Ӫ
		MonsterCannotFight,									//���ɱ�����
		MonsterCanHandle,									//�Ƿ���Խ���
		MonsterFriendValue,									//�Ѻ�ֵ
		MonsterBossFlag,									//BOSS���
		MonsterBossSnapImmID,								//˲ʱЧ������ID
		MonsterBossDurationImmID,							//����Ч������ID
		MonsterDropSearchRange,								//�������뾶(��)
		MonsterDropTeamCount,								//���������Ч��
		MonsterDropItemSetID,								//������Ʒ��ID
		MonsterDropRate,									//�������

		MonsterDropHorseID = MonsterDropRate+9,		//�����ID,��Ӧ������ID
	};	
		
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MONSTER_ATTREX );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAXTYPE_NUMBER);
	g_MonsterAttrExTbl.m_Count	=	iTableCount;
	
	const CHAR* pSplitString = NULL;

	INT iValue;
	INT iType;
	//�ļ��е�����Ӧ������������ţ��˶����Ƿ�ֹ������ļ��ı�
	Assert(MonsterDropTeamCount<iColumnCount);

	for(INT i =0;i<iTableCount;i++)
	{
		//������
		iValue		=	ThirdFile.Search_Posistion(i,MonsterType)->iValue;

		iType	    =	iValue;

		if(g_MonsterAttrExTbl.m_MaxType<(uint)iType)
			g_MonsterAttrExTbl.m_MaxType = iType;
		//ȷ�������������������ֲ�
		g_MonsterAttrExTbl.m_TableExt[i].m_Type	=	iType;
		iType = i;
		//����
		strncpy( g_MonsterAttrExTbl.m_TableExt[iType].m_Name, ThirdFile.Search_Posistion(i,MonsterName)->pString, sizeof( g_MonsterAttrExTbl.m_TableExt[i].m_Name ) - 1 );
		//�ȼ�
		iValue		=	ThirdFile.Search_Posistion(i,MonsterLevel)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_Level			=	iValue;
		//ˢ������
		iValue		=	ThirdFile.Search_Posistion(i,MonsterRespawnTime)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_RespawnTime		=	iValue;
		//��С��Ѫ�ٷֱ�
		iValue		=	ThirdFile.Search_Posistion(i,MonsterMinDamagePercent)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_MinDamagePercent	=	iValue;
		//����������
		iValue		=	ThirdFile.Search_Posistion(i,MonsterExp)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_BaseExp			=	iValue;
		//���̹���
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACK_NEAR]		= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKRATE_NEAR)->iValue;
		//Զ�̹���					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACK_FAR]		= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKRATE_FAR)->iValue;

		//�����ڹ�����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACKRATE_MAGIC_NEAR] = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKRATE_MAGIC_NEAR)->iValue;
		//�����⹦����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACKRATE_MAGIC_FAR] = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKRATE_MAGIC_FAR)->iValue;
		
		//���̷���					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_NEAR]	= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCE_NEAR)->iValue;
		//Զ�̷���					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_FAR]		= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCE_FAR)->iValue;

		//�����ڹ�����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_MAGIC_NEAR]	= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCE_MAGIC_NEAR)->iValue;
		//�����ڹ�����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_MAGIC_FAR]	= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCE_MAGIC_FAR)->iValue;

		//HP����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_MAXHP]			= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_MAXHP)->iValue;
		//MP����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_MAXMP]			= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_MAXMP)->iValue;
		//HP�ظ�					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_RESTOREHP]		= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_RESTOREHP)->iValue;
		//MP�ظ�					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_RESTOREMP]		= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_RESTOREMP)->iValue;
		//������					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_HIT]				= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_HIT)->iValue;
		//����						 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_MISS]			= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_MISS)->iValue;
		//������					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_CRITRATE]		= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_CRITRATE)->iValue;
		//����						 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_TOUGHNESS]		= ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_TOUGHNESS)->iValue;
		//�𹥻�					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACK_GOLD]		= ThirdFile.Search_Posistion(i,MonsterAttrAttack1)->iValue;
		//ľ����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACK_WOOD]		= ThirdFile.Search_Posistion(i,MonsterAttrAttack2)->iValue;
		//ˮ����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACK_WATER]	= ThirdFile.Search_Posistion(i,MonsterAttrAttack3)->iValue;
		//�𹥻�					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACK_FIRE]		= ThirdFile.Search_Posistion(i,MonsterAttrAttack4)->iValue;
		//������					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACK_SOIL]		= ThirdFile.Search_Posistion(i,MonsterAttrAttack4)->iValue;
		//�����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_GOLD]		= ThirdFile.Search_Posistion(i,MonsterAttrDefence1)->iValue;
		//ľ����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_WOOD]	= ThirdFile.Search_Posistion(i,MonsterAttrDefence2)->iValue;
		//ˮ����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_WATER]	= ThirdFile.Search_Posistion(i,MonsterAttrDefence3)->iValue;
		//�����					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_FIRE]		= ThirdFile.Search_Posistion(i,MonsterAttrDefence4)->iValue;
		//������					 
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_DEFENCE_SOIL]	= ThirdFile.Search_Posistion(i,MonsterAttrDefence4)->iValue;
		//���ýű�ID
		g_MonsterAttrExTbl.m_TableExt[iType].m_ScriptID			= ThirdFile.Search_Posistion(i,MonsterScriptID)->iValue;
		//��������ʱ��
		iValue		=	ThirdFile.Search_Posistion(i,MonsterAttackAnimTime)->iValue; 
		g_MonsterAttrExTbl.m_TableExt[iType].m_AttackAnimTime		=	iValue;
		//������ȴʱ��
		iValue		=	ThirdFile.Search_Posistion(i,MonsterAttackCooldownTime)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_AttackCooldownTime	=	iValue;
		//���м��ʱ��
		iValue		=	ThirdFile.Search_Posistion(i,MonsterOutLook)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_OutLook				=	iValue;
		//�ƶ��ٶ�
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_SPEED]	    = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_SPEED			)->iValue;
		//�����ٶ�
		iValue		=	ThirdFile.Search_Posistion(i,MonsterWalkSpeed)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_WalkSpeed	=	iValue;
		//�����ٶ�
		g_MonsterAttrExTbl.m_TableExt[iType].m_Table[CATTR_LEVEL2_ATTACKSPEED]  = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKSPEED	)->iValue;

		SplitStringLevelOne oSplitL1;
		SplitStringLevelTwo oSplitL2;
		//��������ID, ��Ҫ�����ַ���
		pSplitString	=	ThirdFile.Search_Posistion(i,MonsterWeaponID)->pString;
		ReadWeaponID(&oSplitL1, &oSplitL2, pSplitString, iType);
		//��ID,��Ҫ�����ַ���
		pSplitString	=	ThirdFile.Search_Posistion(i,MonsterHorseID)->pString;
		
		ReadHorseID(&oSplitL1, &oSplitL2, pSplitString, iType);

		//����AI
		iValue		=	ThirdFile.Search_Posistion(i,MonsterBaseAI)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_BaseAI		=	iValue;
		//��չAI
		iValue		=	ThirdFile.Search_Posistion(i,MonsterExtAI)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_ExtAI		=	iValue;
		//��Ӫ
		iValue		=	ThirdFile.Search_Posistion(i,MonsterCamp)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_Camp			=	iValue;
		//���ɱ�����
		iValue		=	ThirdFile.Search_Posistion(i,MonsterCannotFight)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_CannotFight	=	iValue;
		//�Ƿ���Խ���
		iValue		=	ThirdFile.Search_Posistion(i,MonsterCanHandle)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_CanHandle	=	iValue;
		//�Ѻ�ֵ
		iValue		=	ThirdFile.Search_Posistion(i,MonsterFriendValue)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_FriendValue	=	iValue;
		//BOSS���
		iValue		=	ThirdFile.Search_Posistion(i,MonsterBossFlag)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_BossFlag		=	iValue;
		//˲ʱЧ������ID
		iValue		=	ThirdFile.Search_Posistion(i,MonsterBossSnapImmID)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_SnapSkillImmID		=	iValue;
		//����Ч������ID
		iValue		=	ThirdFile.Search_Posistion(i,MonsterBossDurationImmID)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_DurationSkillImmID	=	iValue;
		//�������뾶(��)
		iValue		=	ThirdFile.Search_Posistion(i,MonsterDropSearchRange)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_DropSearchRange		=	(FLOAT)iValue;
		//���������Ч��
		iValue		=	ThirdFile.Search_Posistion(i,MonsterDropTeamCount)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_SearchTeamCount		=	iValue;
		
		iValue		=	ThirdFile.Search_Posistion(i,MonsterDropItemSetID)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_MonsterDropItemSetID		=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterDropRate)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_nMonsterDropRate		=	iValue;

		pSplitString = ThirdFile.Search_Posistion(i,MonsterDropHorseID)->pString;
		g_MonsterAttrExTbl.m_TableExt[iType].m_iHorseDropCount = CommonStringSplit( &oSplitL1, &oSplitL2, pSplitString, MaxHorseDropNum, g_MonsterAttrExTbl.m_TableExt[iType].m_aHorseDropID );

		//��ʼ�����	
		g_MonsterAttrExTbl.m_TableExt[iType].m_Used	= TRUE;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MonsterAttrExTable.ini ... OK! " ) ;
	
	__LEAVE_FUNCTION
}




INT CompareNpcQuestTB(const VOID* pArg1,const VOID* pArg2)
{	
	NPC_QUEST* pTB1	=	(NPC_QUEST*)pArg1;
	NPC_QUEST* pTB2	=	(NPC_QUEST*)pArg2;

	if(pTB1->m_Type>pTB2->m_Type)
		return 1;
	else if(pTB1->m_Type<pTB2->m_Type)
		return -1;
	else
		return 0;

}


NPC_QUEST* _NPC_QUEST_TBL::Get(INT NpcType)
{
	NPC_QUEST	tb;

	tb.m_Type = NpcType;

	NPC_QUEST* pResult = (NPC_QUEST*)bsearch(&tb,
		m_Table,
		m_Count,
		sizeof(NPC_QUEST),
		(INT (*)(const VOID*, const VOID*))CompareNpcQuestTB);

	if(pResult)
		return pResult;
	
	return	0;

}


VOID	TableInit::InitNPCQuestTable()
{
	__ENTER_FUNCTION
	
	
	enum NpcQuestTab
	{
		NpcType				= 0,
		NpcName				= 1,
		NpcDefaultDialog	= 2,
		NpcDialogScript		= 3,
		NpcEventList		= 4
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_NPC_QUEST );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount>0 && iTableCount<MAXTYPE_NUMBER);
	g_NpcQuestTbl.m_Table = new NPC_QUEST[iTableCount];
	g_NpcQuestTbl.m_Count = iTableCount;

	INT		iValue;
	INT		iType;
	CHAR	EventList[NPC_EVENT_LENGTH] = {0};
	CHAR	Event[16] = {0};

	for(INT i=0;i<iTableCount;i++)
	{
		iValue		=	ThirdFile.Search_Posistion(i,NpcType)->iValue;
		iType	    =	iValue;
		if( iType>=MAXTYPE_NUMBER )
		{
			AssertEx(FALSE,"ObjQuest������д��Npc��ų�����Χ") ;
			continue ;
		}
		
		g_NpcQuestTbl.m_Table[i].m_Type = iType;
		
		iValue		=	ThirdFile.Search_Posistion(i,NpcDialogScript)->iValue;
		
		if(iValue>0)
		g_NpcQuestTbl.m_Table[i].m_ScriptID	= iValue;
		
		strncpy(EventList,ThirdFile.Search_Posistion(i,NpcEventList)->pString,NPC_EVENT_LENGTH);
		
		INT iEventListLength = (INT)strlen(EventList);

		AssertEx(iEventListLength<NPC_EVENT_LENGTH,"EventList��������!");
		
		INT EventLength = 0;
		INT EventIndex  = 0;

		for(int EventOffSet = 0;EventOffSet<=iEventListLength&&iEventListLength>0;EventOffSet++)
		{
			if(EventOffSet<iEventListLength)
			{
				if(EventList[EventOffSet]!='|')	
				{
					if(EventLength>=12)
						AssertEx(FALSE,"��ʽ����!");

					Event[EventLength++]  = EventList[EventOffSet];
				}
				else
				{
					g_NpcQuestTbl.m_Table[i].m_EventList[EventIndex++] =  atoi(Event);
					g_NpcQuestTbl.m_Table[i].m_EventCount++;
					EventLength =0;
					memset(&Event,0,sizeof(Event));
				}
			}
			else
			{
				g_NpcQuestTbl.m_Table[i].m_EventList[EventIndex++] =  atoi(Event);
				g_NpcQuestTbl.m_Table[i].m_EventCount++;
			}


		}
		
	}

	Log::SaveLog( SERVER_LOGFILE, "Load ObjQuest.txt ... OK! " ) ;

	__LEAVE_FUNCTION

}


VOID TableInit::InitPetAttrTable( )
{//��ȡ����ϵ���������

	__ENTER_FUNCTION

	enum PetAttrData
	{
		PetID	 = 0,
		PetTakeLevel,
		PetName,
		PetCamp,
		IsVarPet,		// �Ƿ�Ϊ�������
		IsBabyPet,		// �Ƿ�Ϊ��������
		PetFoodType	,
		PetSkillCount,
		VoluntarySkill,
		PassiveSkill1,
		PassiveSkill2,
		PassiveSkill3,
		PassiveSkill4,
		PetLife	,
        PetStrPerception,
		PetConPerception,
		PetDexPerception,
		//PetSprPerception,
		PetIntPerception,
		PetGrowRate0,
		PetGrowRate1,
		PetGrowRate2,
		PetGrowRate3,
		PetGrowRate4,
		CowardiceRate,
		WarinessRate,	
		LoyalismRate,	
		CanninessRate,
		ValourRate,	
	
		PetAttrEnd,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_ATTR );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAXTYPE_NUMBER);
	g_PetAttrTbl.m_Count = iTableCount;
	
	
	INT iValue	= 0;
	INT iType	= 0;

	for(INT i =0; i < iTableCount; i++)
	{
		iValue		=	ThirdFile.Search_Posistion(i, PetID)->iValue;
		iType	    =	iValue;

		if( iType>=MAXTYPE_NUMBER )
		{
			AssertEx(FALSE,"�����ID������Χֵ") ;
			continue ;
		}

		if(g_PetAttrTbl.m_MaxType < (uint)iType)
			g_PetAttrTbl.m_MaxType = iType;

		// ������
		g_PetAttrTbl.m_TableExt[iType].m_Type	=	iType;
		// ��������
		strncpy( g_PetAttrTbl.m_TableExt[iType].m_Name, ThirdFile.Search_Posistion(i, PetName)->pString, sizeof( g_PetAttrTbl.m_TableExt[i].m_Name ) - 1 );
		// �����Я���ȼ�
		iValue		=	ThirdFile.Search_Posistion(i, PetTakeLevel)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_TakeLevel	=	iValue;
		// �Ƿ�Ϊ�������
		iValue		=	ThirdFile.Search_Posistion(i, IsVarPet)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_bVarPet  =	iValue;
		// �Ƿ�Ϊ��������
		iValue		=	ThirdFile.Search_Posistion(i, IsBabyPet)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_bBabyPet	=	iValue;
		// ����ʳ������
		iValue		=	ThirdFile.Search_Posistion(i, PetFoodType)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_FoodType	=	iValue;
		//// ��׼����
		//iValue		=	ThirdFile.Search_Posistion(i, PetLife)->iValue;
		//g_PetAttrTbl.m_TableExt[iType].m_Life	=	iValue;
		// ���������������
		iValue		=   ThirdFile.Search_Posistion(i, PetSkillCount)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkillCount =	iValue;
		// ��������
		iValue		=   ThirdFile.Search_Posistion(i, VoluntarySkill)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_VoluntarySkill =	iValue;
		// ��������1
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill1)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill1 =	iValue;
		// ��������2
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill2)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill2 =	iValue;
		// ��������3
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill3)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill3 =	iValue;
		// ��������4
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill4)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill4 =	iValue;
		// ��׼��������
		iValue		=	ThirdFile.Search_Posistion(i, PetStrPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_iStrengthPerception	=	iValue;
		// ��׼��������
		iValue		=	ThirdFile.Search_Posistion(i, PetConPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_iSmartnessPerception	=	iValue;
		// ��׼��������
		iValue		=	ThirdFile.Search_Posistion(i, PetDexPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_iMindPerception	=	iValue;

		// ��׼��������
		iValue		=	ThirdFile.Search_Posistion(i, PetIntPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_iConstitutionPerception	=	iValue;
		// �ɳ���0
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate0)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate0	=	iValue;
		// �ɳ���1
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate1)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate1	=	iValue;
		// �ɳ���2
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate2)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate2	=	iValue;
		// �ɳ���3
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate3)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate3	=	iValue;
		// �ɳ���4
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate4)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate4	=	iValue;
		// ��С
		iValue		=	ThirdFile.Search_Posistion(i, CowardiceRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_CowardiceRate	=	iValue;
		// ����
		iValue		=	ThirdFile.Search_Posistion(i, WarinessRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_WarinessRate	=	iValue;
		// �ҳ�
		iValue		=	ThirdFile.Search_Posistion(i, LoyalismRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_LoyalismRate	=	iValue;
		// ����
		iValue		=	ThirdFile.Search_Posistion(i, CanninessRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_CanninessRate	=	iValue;
		// ����
		iValue		=	ThirdFile.Search_Posistion(i, ValourRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_ValourRate	=	iValue;
	
	}

	Log::SaveLog( SERVER_LOGFILE, "Load PetAttrTable.txt ... OK! " ) ;
	
	__LEAVE_FUNCTION
}

VOID TableInit::InitMonsterAITable( )
{
	__ENTER_FUNCTION

	Ini ini( FILE_MONSTER_AI ) ;

	g_MonsterAITbl.m_Count = ini.ReadInt( "INFO", "AINUMBER" ) ;
	Assert( g_MonsterAITbl.m_Count>=0 && g_MonsterAITbl.m_Count<MAXAI_NUMBER ) ;

	for( INT i=0; i<g_MonsterAITbl.m_Count; i++ )
	{
		for( INT j=0; j<AIPARAM_NUMBER; j++ )
		{
			CHAR szSection[256], szName[256] ;
			sprintf( szSection, "AI%d", i ) ;
			sprintf( szName, "AIPARAM%d", j ) ;
			g_MonsterAITbl.m_Table[i][j] = ini.ReadInt( szSection, szName ) ;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MonsterAITable.ini ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID TableInit::InitPetConfigTable( )
{
	__ENTER_FUNCTION

	Ini ini( FILE_PET_CONFIG ) ;

	m_PetConfig.m_BabyPetRate = ini.ReadInt("System", "BabyPetRate");
    m_PetConfig.m_VariancePetRate = ini.ReadInt("System", "VariancePetRate");

	m_PetConfig.m_WilenessPetRate_TakeLevel = ini.ReadInt("System", "WilenessPetRate_TakeLevel");
	m_PetConfig.m_WilenessPetRate_Delta1 = ini.ReadInt("System", "WilenessPetRate_Delta1");
	m_PetConfig.m_WilenessPetRate_Delta2 = ini.ReadInt("System", "WilenessPetRate_Delta2");
	m_PetConfig.m_WilenessPetRate_Delta3 = ini.ReadInt("System", "WilenessPetRate_Delta3");

	m_PetConfig.m_GrowRate0 = ini.ReadInt("System", "GrowRate0");
	m_PetConfig.m_GrowRate1 = ini.ReadInt("System", "GrowRate1");
	m_PetConfig.m_GrowRate2 = ini.ReadInt("System", "GrowRate2");
	m_PetConfig.m_GrowRate3 = ini.ReadInt("System", "GrowRate3");
	m_PetConfig.m_GrowRate4 = ini.ReadInt("System", "GrowRate4");

	CHAR szName[256];
	for (INT i = 0; i < GENGU_NUM; ++i)
	{
		memset(szName, 0, sizeof(szName));
		sprintf(szName, "GenGu%d_Begin", i);
		m_PetConfig.m_vGenGu[i].m_Begin = ini.ReadInt("System", szName);
		
		sprintf(szName, "GenGu%d_End", i);
		m_PetConfig.m_vGenGu[i].m_End = ini.ReadInt("System", szName);

		sprintf(szName, "GenGu%d_Rate", i);
		m_PetConfig.m_vGenGu[i].m_Rate = ini.ReadInt("System", szName);
	}

	for (INT j = 0; j < SPetConfig::VARIANCEPET_LEVEL_NUM; ++j)
	{
		tsnprintf(szName, sizeof(szName), "rateoflevel%dvariancepet", j+1);
		INT nBaseRate = 0;
		if (j > 0)
		{
			nBaseRate = m_PetConfig.m_aRateOfLevelVariancePet[j-1];
		}
		INT nRet = 0;
		ini.ReadIntIfExist("System", szName, nRet);
		m_PetConfig.m_aRateOfLevelVariancePet[j] = nBaseRate + nRet;
	}

	m_PetConfig.m_IntelligenceRange = ini.ReadInt("System", "IntelligenceRange");

	m_PetConfig.m_MoveSpeed = ini.ReadInt("System", "MoveSpeed");
	m_PetConfig.m_AttackSpeed = ini.ReadInt("System", "AttackSpeed");

	// ��ʼHP���﹥��ħ���������ħ�������С����ܡ����ģ��˸���
	m_PetConfig.m_BaseHP = ini.ReadInt("System", "BaseHP");
	//m_PetConfig.m_BasePhyAttack = ini.ReadInt("System", "BasePhyAttack");
	m_PetConfig.m_BaseNearAttack = ini.ReadInt("System", "BaseNearAttack");
	m_PetConfig.m_BaseFarAttack = ini.ReadInt("System", "BaseFarAttack");

	m_PetConfig.m_BaseMgcAttack = ini.ReadInt("System", "BaseMgcAttack");
	//m_PetConfig.m_BasePhyDefence = ini.ReadInt("System", "BasePhyDefence");
	m_PetConfig.m_BaseNearDefence = ini.ReadInt("System", "BaseNearDefence");
	m_PetConfig.m_BaseFarDefence = ini.ReadInt("System", "BaseFarDefence");

	m_PetConfig.m_BaseMgcDefence = ini.ReadInt("System", "BaseMgcDefence");
	m_PetConfig.m_BaseHit = ini.ReadInt("System", "BaseHit");
	m_PetConfig.m_BaseMiss = ini.ReadInt("System", "BaseMiss");
	m_PetConfig.m_BaseCritical = ini.ReadInt("System", "BaseCritical");

	// ���ʶ�HP���������﹥��������ħ�������ʶ������������ħ��
	// ���ݶ����ܣ����ݶԻ���,���ݶ����е�Ӱ��ϵ��
	m_PetConfig.m_Con_HP_Pram = ini.ReadInt("System", "Con_HP_Pram") / 1000.f;
	//m_PetConfig.m_Str_PhyAttack_Pram = ini.ReadInt("System", "Str_PhyAttack_Pram") / 1000.f;
	m_PetConfig.m_Str_NearAttack_Pram = ini.ReadInt("System", "Str_NearAttack_Pram") / 1000.f;

	//m_PetConfig.m_Spr_MgcAttack_Pram = ini.ReadInt("System", "Spr_MgcAttack_Pram") / 1000.f;
	//m_PetConfig.m_Con_PhyDefence_Pram = ini.ReadInt("System", "Con_PhyDefence_Pram") / 1000.f;
	m_PetConfig.m_Int_MgcDefence_Pram = ini.ReadInt("System", "Int_MgcDefence_Pram") / 1000.f;
	m_PetConfig.m_Dex_Miss_Pram = ini.ReadInt("System", "Dex_Miss_Pram") / 1000.f;
	m_PetConfig.m_Dex_Critical_Pram = ini.ReadInt("System", "Dex_Critical_Pram") / 1000.f;
	m_PetConfig.m_Dex_Hit_Pram = ini.ReadInt("System", "Dex_Hit_Pram") / 1000.f;

	// �ȼ���HP���﹥��ħ���������ħ�������ܡ����ġ����е�Ӱ��ϵ��
	m_PetConfig.m_Level_HP_Pram = ini.ReadInt("System", "Level_HP_Pram") / 1000.f;
	//m_PetConfig.m_Level_PhyAttack_Pram = ini.ReadInt("System", "Level_PhyAttack_Pram") / 1000.f;
	m_PetConfig.m_Level_NearAttack_Pram = ini.ReadInt("System", "Level_NearAttack_Pram") / 1000.f;
	m_PetConfig.m_Level_FarAttack_Pram = ini.ReadInt("System", "Level_FarAttack_Pram") / 1000.f;

	m_PetConfig.m_Level_MgcAttack_Pram = ini.ReadInt("System", "Level_MgcAttack_Pram") / 1000.f;
	//m_PetConfig.m_Level_PhyDefence_Pram = ini.ReadInt("System", "Level_PhyDefence_Pram") / 1000.f;
	m_PetConfig.m_Level_NearDefence_Pram = ini.ReadInt("System", "Level_NearDefence_Pram") / 1000.f;
	m_PetConfig.m_Level_FarDefence_Pram = ini.ReadInt("System", "Level_FarDefence_Pram") / 1000.f;

	m_PetConfig.m_Level_MgcDefence_Pram = ini.ReadInt("System", "Level_MgcDefence_Pram") / 1000.f;
	m_PetConfig.m_Level_Miss_Pram = ini.ReadInt("System", "Level_Miss_Pram") / 1000.f;
	m_PetConfig.m_Level_Critical_Pram = ini.ReadInt("System", "Level_Critical_Pram") / 1000.f;
	m_PetConfig.m_Level_Hit_Pram = ini.ReadInt("System", "Level_Hit_Pram") / 1000.f;

	//�������
	m_PetConfig.m_Level_FindingHorseMate = ini.ReadInt("System", "Level_FindingHorseMate");
	m_PetConfig.m_Happiness_FindingHorseMate = ini.ReadInt("System", "Happiness_FindingHorseMate");
	m_PetConfig.m_Money_FindingHorseMate = ini.ReadInt("System", "Money_FindingHorseMate");


	Log::SaveLog( SERVER_LOGFILE, "Load PetConfigTable.ini ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID TableInit::InitPetAttrPointDistribute( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_ATTRPOINTDISTRIBUTE );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iValue;
	for(INT i =0; i < iTableCount; i++)
	{
		// ���Ե���䷽����
		iValue		=	ThirdFile.Search_Posistion(i, 0)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_ID = iValue;
        iValue		=	ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_StrRate = iValue;
		iValue		=	ThirdFile.Search_Posistion(i, 2)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_ConRate = iValue;
		//iValue		=	ThirdFile.Search_Posistion(i, 3)->iValue;
		//g_PetAttrPointDisTbl.m_Table[i].m_SprRate = iValue;
		/*iValue		=	ThirdFile.Search_Posistion(i, 4)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_DexRate = iValue;
		iValue		=	ThirdFile.Search_Posistion(i, 5)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_IntRate = iValue;*/
		iValue		=	ThirdFile.Search_Posistion(i, 3)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_DexRate = iValue;
		iValue		=	ThirdFile.Search_Posistion(i, 4)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_IntRate = iValue;

		++g_PetAttrPointDisTbl.m_Count;
	}

	__LEAVE_FUNCTION
}

VOID TableInit::InitPetLevelUpTable( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_LEVELUP );

	INT iTableCount				=	ThirdFile.GetRecordsNum();
	INT iColumnCount			=	ThirdFile.GetFieldsNum();
	g_PetLevelUpTbl.m_iCount	= iTableCount;

	INT iLevel = 0;
	INT iExp = 0;
	for(INT i =0; i < iTableCount; i++)
	{
		// ����
		iLevel	=	ThirdFile.Search_Posistion(i, 0)->iValue;
		// �ü����������辭��
		iExp	=	ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetLevelUpTbl.m_Table[iLevel] = iExp;
	}

	__LEAVE_FUNCTION

}

VOID TableInit::InitPetDomesticationMoney( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_DOMESTICATION_MONEY );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iLevel = 0;
	FLOAT fDomesticationMoney = 0;
	for(INT i =0; i < iTableCount; i++)
	{
		// ����
		iLevel			=	ThirdFile.Search_Posistion(i, 0)->iValue;
		// �ü����������辭��
		fDomesticationMoney	=	ThirdFile.Search_Posistion(i, 1)->fValue;
		g_PetDomesticationMoneyTbl.m_Table[iLevel] = fDomesticationMoney;
	}

	__LEAVE_FUNCTION

}

VOID TableInit::InitPetSkillDistributeTable( )
{// ��ʼ�����ܷ����

	__ENTER_FUNCTION


	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_SKILLDISTRIBUTE );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iPetID = 0;
	INT iRange = 0;
    for(INT i = 0; i < iTableCount; i++)
	{
		iPetID = ThirdFile.Search_Posistion(i, 0)->iValue;
		g_PetSkillDistrTbl[i].m_PetID = iPetID;
		for (INT j = 2; j < iColumnCount; j++)
		{
			iRange = ThirdFile.Search_Posistion(i, j)->iValue;
			if (iRange > 0)
			{
				for (INT n = 0; n < PET_MAX_SKILL_COUNT; ++n)
				{
					if (g_PetSkillDistrTbl[i].m_Table[n].m_nSkillIndex == -1)
					{
						g_PetSkillDistrTbl[i].m_Table[n].m_nSkillIndex = j - 2;
						g_PetSkillDistrTbl[i].m_Table[n].m_nSkillRange = iRange;
						break;
					}
				}
			}
		}
	}
	__LEAVE_FUNCTION

}

VOID TableInit::InitPetSkillIndexTable( )
{// ��ʼ�������������˱���Ҫ��Ϊ���Ժ�ά�����㣡
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_SKILLINDEXTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iIndex = 0;
	INT idSkill = 0;
	memset(g_PetSkillIndexTbl, 0, sizeof(_PET_SKILLINDEX_TBL) * PET_SKILL_NUM);

	for (INT i = 0; i < iTableCount; i++)
	{
		iIndex = ThirdFile.Search_Posistion(i, 0)->iValue;
		idSkill = ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetSkillIndexTbl[i].m_nIndex = iIndex;
		g_PetSkillIndexTbl[i].m_nSkillID = idSkill;
	}
	__LEAVE_FUNCTION
}

VOID TableInit::InitPetStudySkillTable( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_STUDYSKILLTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT nID;
	INT		nMaxSpaceCount;	// �ܿո�����
	INT		nSpaceCount;		// ��ǰ�ո�����
	for (INT i = 0; i < iTableCount; ++i)
	{
		Assert(i < PET_STUDYSKILLRATE_NUM);

		nMaxSpaceCount	= ThirdFile.Search_Posistion(i, 2)->iValue;
		nSpaceCount		= ThirdFile.Search_Posistion(i, 1)->iValue;
		nID = MAKE_PET_STUDYSKILL_ID( nMaxSpaceCount, nSpaceCount );

		g_PetStudySkillTbl.m_aStudySkill[nID].m_nID					= nID;
		g_PetStudySkillTbl.m_aStudySkill[nID].m_nMaxSpaceCount		= nMaxSpaceCount;
		g_PetStudySkillTbl.m_aStudySkill[nID].m_nSpaceCount			= nSpaceCount;
		g_PetStudySkillTbl.m_aStudySkill[nID].m_nNewPercent			= ThirdFile.Search_Posistion(i, 3)->iValue;;
	}
	__LEAVE_FUNCTION
}

VOID TableInit::InitPetApperceiveSkillTable( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_APPERCEIVESKILLTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	memset(g_PetApperceiveSkillTbl, 0, sizeof(_PET_APPERCEIVESKILL_TBL) * PET_APPERCEIVESKILLRATE_NUM);

	for (INT i = 0; i < iTableCount; ++i)
	{
		Assert(i < PET_APPERCEIVESKILLRATE_NUM);
	
		g_PetApperceiveSkillTbl[i].m_nSkillCount = ThirdFile.Search_Posistion(i, 2)->iValue;
		g_PetApperceiveSkillTbl[i].m_nLearnedVoluntarySkillCount = ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetApperceiveSkillTbl[i].m_nLearnedPassiveSkillCount = ThirdFile.Search_Posistion(i, 3)->iValue;
		g_PetApperceiveSkillTbl[i].m_nApperceiveVoluntarySkillRate = ThirdFile.Search_Posistion(i, 4)->iValue;
		g_PetApperceiveSkillTbl[i].m_nApperceivePassiveSkillRate = ThirdFile.Search_Posistion(i, 5)->iValue;
	}
	__LEAVE_FUNCTION

}

VOID	TableInit::InitPetAIStrategyTable()
{
	__ENTER_FUNCTION

	enum 
	{
		AITypeBegin = 2,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_AISTRATEGYTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	memset(g_PetAIStrategyTble, 0, sizeof(FLOAT) * PET_AITYPE * PET_SKILL_TYPE);
	
	for (INT i = 0; i < iTableCount; ++i)
	{
		Assert(i < PET_SKILL_TYPE);
		for (INT j = AITypeBegin; j < iColumnCount; ++j)
		{	
			Assert(j < PET_AITYPE + AITypeBegin);
			g_PetAIStrategyTble[i][j-AITypeBegin] = ThirdFile.Search_Posistion(i, j)->fValue;
		}
	}
	__LEAVE_FUNCTION
}

INT CompareGrowPointTB(const VOID* pArg1,const VOID* pArg2)
{	
	_GROW_POINT_INFO* pTB1	=	(_GROW_POINT_INFO*)pArg1;
	_GROW_POINT_INFO* pTB2	=	(_GROW_POINT_INFO*)pArg2;

	if(pTB1->m_GrowPointIndex>pTB2->m_GrowPointIndex)
		return 1;
	else if(pTB1->m_GrowPointIndex<pTB2->m_GrowPointIndex)
		return -1;
	else
		return 0;

}


_GROW_POINT_INFO*	_GROW_POINT_INFO_TBL::Get(INT GrowType)
{
	_GROW_POINT_INFO	tb;

	tb.m_GrowPointIndex	=	GrowType;

	_GROW_POINT_INFO* pResult = (_GROW_POINT_INFO*)bsearch(&tb,
		m_Table,
		m_Count,
		sizeof(_GROW_POINT_INFO),
		(INT (*)(const VOID*, const VOID*))CompareGrowPointTB);

	if(pResult)
		return pResult;
	else
	{
		Assert(FALSE);
		Log::SaveLog( SERVER_LOGFILE, "Search _GROW_POINT_INFO  GrowPointType:%d Get Errors", 
			GrowType) ;
	}
	return	0;

}
VOID	TableInit::InitGrowPointTable()
{
	__ENTER_FUNCTION
	
	enum	GrowPointTBLData
	{
		TBIndex				=	0,
		TBName				=	1,
		TBOpenAbility		=	2,
		TBMesh				=	3,
		TBOpenTask			=	4,
		TBTips				=	5,
		TBOpenTime			=	6,
		TBScriptID			=	7,
		TBOpenAnim			=	8
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_GROWPOINT );
	

	INT iTableCount		=		ThirdFile.GetRecordsNum();
	INT iColumnCount	=		ThirdFile.GetFieldsNum();

	Assert(TBOpenAnim<iColumnCount);
	//Assert(iTableCount<MAX_GROW_POINT_TYPE);

	g_GrowPointInfoTbl.m_Count	=	iTableCount;
	g_GrowPointInfoTbl.m_Table	=	new _GROW_POINT_INFO[iTableCount];
	for(INT i = 0 ;i<iTableCount;i++)
	{
		g_GrowPointInfoTbl.m_Table[i].m_GrowPointIndex	=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_OpenTime		=	ThirdFile.Search_Posistion(i,TBOpenTime)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_OpenAnim		=	ThirdFile.Search_Posistion(i,TBOpenAnim)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_OpenAbility		=	ThirdFile.Search_Posistion(i,TBOpenAbility)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_ScriptID		=	ThirdFile.Search_Posistion(i,TBScriptID)->iValue;
		
			
	}
	Log::SaveLog( SERVER_LOGFILE, "Load GrowPoint.txt ... OK! " );

	__LEAVE_FUNCTION
}

VOID TableInit::InitPlantTable()
{
	__ENTER_FUNCTION
	
	enum	PlantTBLData
	{
		TBIndex		=	0,
		TBType		=	1,
		TBName		=	2,
		TBProduct	=	3
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PLANTS );


	INT iTableCount		=		ThirdFile.GetRecordsNum();
	INT iColumnCount	=		ThirdFile.GetFieldsNum();

	Assert(TBProduct<iColumnCount);

	g_PlantInfoTbl.m_Count		=		iTableCount;
	g_PlantInfoTbl.m_Table		=		new _PLANTS_INFO[iTableCount];

	for(INT i = 0;i<iTableCount;i++)
	{
		g_PlantInfoTbl.m_Table[i].m_PlantIndex	=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		g_PlantInfoTbl.m_Table[i].m_PlantType	=	ThirdFile.Search_Posistion(i,TBType)->iValue;
		g_PlantInfoTbl.m_Table[i].m_Product		=	ThirdFile.Search_Posistion(i,TBProduct)->iValue;

	}

	Log::SaveLog( SERVER_LOGFILE, "Load Plant.txt ... OK! " );
	__LEAVE_FUNCTION
}

VOID TableInit::InitAttrLevelupTable( )
{
	__ENTER_FUNCTION
		
	enum	AttrLevelupTBData
	{
		TBLevel					=			0,
		TBDataBegin				=			1
	};

	DBCFile ThirdFile(0);

	BOOL ret = ThirdFile.OpenFromTXT( FILE_ATTR_LEVEL_UP );

	INT		iTableCount		=	ThirdFile.GetRecordsNum();
	INT		iTableColumn	=	ThirdFile.GetFieldsNum();
	INT		iColumn			=	0;
	
	Assert(iTableCount==MAX_TABLE_DEFINE_LEVEL+1);

	for(INT i = 0;	i<MAX_TABLE_DEFINE_LEVEL+1;i++)
	{
		for(INT j = 0;j<MATTRIBUTE_NUMBER;j++)
		{
			for(INT k=0;k<CATTR_LEVEL_NUMBER;k++)
			{
				iColumn		=	TBDataBegin+j*CATTR_LEVEL_NUMBER+k;

				Assert(iColumn<iTableColumn);
				INT iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue;
				g_AttrLevelupTbl.Set(k,j,i,iValue);
			}
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load AttrLevelupTable.ini ... OK! " ) ;
	
	__LEAVE_FUNCTION
}
//skill
BOOL 			TableInit::InitXinFaDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_XinFaDataMgr.InitFromDB( FILE_XINFA ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load XinFa_v1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load XinFa_v1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 			TableInit::InitSkillTemplateDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_SkillTemplateDataMgr.InitFromDB( FILE_SKILL_TEMPLATE ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillTemplate_v1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillTemplate_v1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitSkillDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_SkillDataMgr.InitFromDB( FILE_SKILL_TEMPLATE ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillData_v1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillData_v1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitImpactSEDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_ImpactSEDataMgr.InitFromDB( FILE_IMPACT_SEDATA ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load ImpactSEData_v1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load ImpactSEData_v1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitStandardImpactMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_StandardImpactMgr.InitFromDB( FILE_STANDARD_IMPACT_DATA))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load StandardImpact.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load StandardImpact.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitSpecialObjDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_SpecialObjDataMgr.InitFromDB( FILE_SPECIAL_OBJ_DATA ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SpecialObjData.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SpecialObjData.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
//end skill
VOID	TableInit::InitAIScriptFile()
{
	__ENTER_FUNCTION
	g_AIFileDataMgr.Init( FILE_AI_SCRIPT, FALSE);

	memset((VOID*)(g_pAIScriptList), NULL, AISCRIPT_NUM);
	for (INT i = 0; i < AISCRIPT_NUM; ++i)
	{
		SFileData * pSFileData = g_AIFileDataMgr.GetFileData(i);
		if (!pSFileData)
		{// �Ѿ����꣨����������Ѿ����꣩
			continue ;
		}

		AIScript* pAIScript = new AIScript();
		if (!pAIScript)
		{
			Assert(NULL && "TableInit::InitAIScriptFile...new AIScript Failed...");
			return ;
		}

		if(pSFileData)
		{
			CHAR  szName[_MAX_PATH];
			CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());
			memset(szName,0,_MAX_PATH);
			GET_AI_SCRIPT_FULL_PATH( szName, filename);
		
			pAIScript->ParseScript(szName);
		}

		// ѹ��g_pAIScriptList������
		g_pAIScriptList[i] = pAIScript;
	}

	__LEAVE_FUNCTION
}

VOID TableInit::CleanUpAIScrpt()
{
	for (INT i = 0; i < AISCRIPT_NUM; ++i)
	{
		SAFE_DELETE(g_pAIScriptList[i]);
	}
}

//camp and stand
BOOL TableInit::InitCampAndStandDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_CampAndStandDataMgr.InitFromDB( FILE_CAMP_AND_STAND ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load CampAndStand.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load CampAndStand.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
//end camp and stand
//Impact collection
BOOL TableInit::InitIdCollectionMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_IDCollectionMgr.InitFromDB(FILE_ID_COLLECTION))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load IDCollections.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load IDCollections.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID TableInit::InitQuestionLib( )
{
__ENTER_FUNCTION

	enum	QuestionTBLData
	{
		TBIndex		=	0,
		TBContex	=	1,
		TBOption0	=	2,
		TBOption1	=	3,
		TBOption2	=	4,
		TBOption3	=	5,
		TBOption4	=	6,
		TBOption5	=	7,
		TBKey0		=	8,
		TBKey1		=	9,
		TBKey2		=	10,
		TBKey3		=	11,
		TBKey4		=	12,
		TBKey5		=	13,
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_QUESTION );


	INT iTableCount		=		ThirdFile.GetRecordsNum();
	INT iColumnCount	=		ThirdFile.GetFieldsNum();

	Assert(TBKey5<iColumnCount);
	Assert(iTableCount<=MAX_QUESTIONS) ;

	g_QuestionLib.m_Count		=		iTableCount;

	for(INT i = 0;i<iTableCount;i++)
	{
		g_QuestionLib.m_aQuestions[i].m_nIndex	=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		
		strncpy(g_QuestionLib.m_aQuestions[i].m_szContex,ThirdFile.Search_Posistion(i, TBContex)->pString,QUES_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szContex[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szContex, 0, QUES_CON_SIZE ) ;
		
		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption0,ThirdFile.Search_Posistion(i, TBOption0)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption0[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption0, 0, OPT_CON_SIZE ) ;
		
		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption1,ThirdFile.Search_Posistion(i, TBOption1)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption1[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption1, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption2,ThirdFile.Search_Posistion(i, TBOption2)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption2[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption2, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption3,ThirdFile.Search_Posistion(i, TBOption3)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption3[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption3, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption4,ThirdFile.Search_Posistion(i, TBOption4)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption4[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption4, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption5,ThirdFile.Search_Posistion(i, TBOption5)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption5[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption5, 0, OPT_CON_SIZE ) ;

		g_QuestionLib.m_aQuestions[i].m_Key0	=	ThirdFile.Search_Posistion(i,TBKey0)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key1	=	ThirdFile.Search_Posistion(i,TBKey1)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key2	=	ThirdFile.Search_Posistion(i,TBKey2)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key3	=	ThirdFile.Search_Posistion(i,TBKey3)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key4	=	ThirdFile.Search_Posistion(i,TBKey4)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key5	=	ThirdFile.Search_Posistion(i,TBKey5)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load Questions.txt ... OK! " );


__LEAVE_FUNCTION
}

VOID TableInit::InitIPRange( )
{
	__ENTER_FUNCTION

	Ini ini( FILE_IPRANGE ) ;

	INT i;
	g_IPRange.m_EnableCount = ini.ReadInt( "enableip", "count" ) ;
	for( i=0; i<g_IPRange.m_EnableCount; i++ )
	{
		CHAR szIP[32],szMask[32] ;
		sprintf( szIP, "ip%d", i ) ;
		sprintf( szMask, "mask%d", i ) ;
		ini.ReadText( "enableip", szIP, g_IPRange.m_aaIPEnable[i], IP_SIZE ) ;
		ini.ReadText( "enableip", szMask, g_IPRange.m_aaMaskEnable[i], IP_SIZE ) ;
	}

	g_IPRange.m_DisableCount = ini.ReadInt( "disableip", "count" ) ;
	for( i=0; i<g_IPRange.m_DisableCount; i++ )
	{
		CHAR szIP[32],szMask[32] ;
		sprintf( szIP, "ip%d", i ) ;
		sprintf( szMask, "mask%d", i ) ;
		ini.ReadText( "disableip", szIP, g_IPRange.m_aaIPDisable[i], IP_SIZE ) ;
		ini.ReadText( "disableip", szMask, g_IPRange.m_aaMaskDisable[i], IP_SIZE ) ;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load IPRange.ini ... OK! " ) ;

	__LEAVE_FUNCTION

}

VOID	TableInit::InitLevelMoneyTable()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PLAYER_LEVEL_MONEY );

	INT iTableCount = ThirdFile.GetRecordsNum();

	Assert(iTableCount==MAX_PLAYER_EXP_LEVEL);

	for(INT i = 0;	i<MAX_PLAYER_EXP_LEVEL;i++)
	{
		g_LevelMoneyTbl.m_Table[i][0]	=	i;
		g_LevelMoneyTbl.m_Table[i][1]	=	ThirdFile.Search_Posistion(i,1)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load LvMaxMoney.txt ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID	TableInit::InitCitySceneTable()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_CITY_SCENE );

	INT iTableCount = ThirdFile.GetRecordsNum();

	Assert(iTableCount<=MAX_SCENE);

	for(INT i = 0; i<iTableCount; i++)
	{
		g_CitySceneTbl.m_Table[i].m_PortID	=	ThirdFile.Search_Posistion(i, 0)->iValue;
		strncpy( g_CitySceneTbl.m_Table[i].m_szSceneFile, ThirdFile.Search_Posistion(i,1)->pString, sizeof( g_CitySceneTbl.m_Table[i].m_szSceneFile ) - 1 );
	}

	Log::SaveLog( SERVER_LOGFILE, "Load CityInfo.txt ... OK! " ) ;

	__LEAVE_FUNCTION
}
VOID	TableInit::InitChatConsume( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_CHAT_CONSUME );

	INT iTableCount = ThirdFile.GetRecordsNum();
	g_ChatConsumeTbl.m_Count = iTableCount ;

	Assert(iTableCount<=MAX_CHAT_TYPE_INTABLE);

	for(INT i = 0; i<iTableCount; i++)
	{
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 0 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_DelayTime = ThirdFile.Search_Posistion(i, 2)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[0] = ThirdFile.Search_Posistion(i, 4)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeValue[0] = ThirdFile.Search_Posistion(i, 5)->iValue;
		if( g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[0]!=CHAT_NEED_NONE ) g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 1 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[1] = ThirdFile.Search_Posistion(i, 6)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeValue[1] = ThirdFile.Search_Posistion(i, 7)->iValue;
		if( g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[1]!=CHAT_NEED_NONE ) g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 2 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[2] = ThirdFile.Search_Posistion(i, 8)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeValue[2] = ThirdFile.Search_Posistion(i, 9)->iValue;
		if( g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[2]!=CHAT_NEED_NONE ) g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 3 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_NeedLevel = ThirdFile.Search_Posistion(i, 10)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load ChatConfig.txt ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID	TableInit::InitHairStyleTable( )
{
__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_HAIRSTYLE );
	if( ret != TRUE )
	{
		Assert( FALSE && "Read Hair Style table failed, please check it." );
		return;
	}

	INT iTableCount = ThirdFile.GetRecordsNum();
	g_HairStyleTbl.m_uCount = iTableCount;

	Assert(iTableCount<=MAX_HAIR_STYLE_INTABLE);

	for(INT i = 0; i<iTableCount; i++)
	{
		g_HairStyleTbl.m_HairStyle[i].m_nIndex
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_INDEX)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_nRaceID
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_RACEID)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_uItemSerial
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_ITEMSERIAL)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_nItemCostCount
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_ITEMCOSTCOUNT)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_nUseScope
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_USESCOPE)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load HairStyle.txt ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID	TableInit::InitShopInfoTable()
{
    __ENTER_FUNCTION

    UINT d = sizeof(_ITEM);
    DBCFile ShopFile(0);
    BOOL ret = ShopFile.OpenFromTXT(FILE_SHOP);

    INT iTableCount		= ShopFile.GetRecordsNum();
    INT iTableColumn	= ShopFile.GetFieldsNum();

    g_ShopInfoTbl.m_iCount		=	iTableCount;
    INT itemnum = 0;
    INT i,j,k;
    INT itemTypeSn;
    _ITEM_TYPE	itemType;
    INT			PerItemNum	= 0;
    INT			MaxItemNum	= 0;
    FLOAT		PerRate		= 0.0;

    for(i =0;i<iTableCount;i++)
    {
        g_ShopInfoTbl.m_aShop[i].m_ShopId			= 		ShopFile.Search_Posistion(i,SHOP_ID)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_nRepairLevel	    =		ShopFile.Search_Posistion(i,SHOP_REPAIR_LEVEL)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_nBuyLevel		=		ShopFile.Search_Posistion(i,SHOP_BUY_LEVEL)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_nRepairType	    =		ShopFile.Search_Posistion(i,SHOP_REPAIR_TYPE)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_nBuyType		    =		ShopFile.Search_Posistion(i,SHOP_BUY_TYPE)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_nRepairSpend	    =		ShopFile.Search_Posistion(i,SHOP_REPAIR_SPEND)->fValue;
        g_ShopInfoTbl.m_aShop[i].m_nRepairOkProb	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_OKPROB)->fValue;
        g_ShopInfoTbl.m_aShop[i].m_scale			=		ShopFile.Search_Posistion(i,SHOP_SCALE)->fValue;
        g_ShopInfoTbl.m_aShop[i].m_nRate            =       ShopFile.Search_Posistion(i,SHOP_RATE)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_IsDyShop         =       ShopFile.Search_Posistion(i,SHOP_TYPE)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_refreshTime1	    =		ShopFile.Search_Posistion(i,SHOP_REFRESH_TIME_1)->iValue;
        g_ShopInfoTbl.m_aShop[i].m_refreshTime2	    =		ShopFile.Search_Posistion(i,SHOP_REFRESH_TIME_2)->iValue;
        itemnum						                =		ShopFile.Search_Posistion(i,SHOP_ITEM_NUM)->iValue;

        INT nNum = 0;
        for(k=0; k<itemnum*SHOP_ITEM_PROPERTY_NUM; k++)
        {
            itemTypeSn	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+k)->iValue;
            if(itemTypeSn == 0)
            {
                break;
            }
            nNum ++;
            k = k+2;
        }
        itemnum = nNum;

        g_ShopInfoTbl.m_aShop[i].m_ItemList		=		new _SHOP::_MERCHANDISE_LIST(itemnum);

        for(j = 0; j<itemnum*SHOP_ITEM_PROPERTY_NUM; j++)
        {
            //Type
            itemTypeSn	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+j)->iValue;
            TSerialHelper help(itemTypeSn);
            itemType	= help.GetItemTypeStruct();

            //GroupNum 0~100
            PerItemNum  = ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+(++j))->iValue;
            if(PerItemNum<0) PerItemNum = 1;
            if(PerItemNum>100) PerItemNum = 100;

            //MaxNum -1�������ޣ�>0����������Ʒ���ޣ���������0,<100
            MaxItemNum	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+(++j))->iValue;
            if(MaxItemNum == 0)	MaxItemNum = -1;
            if(PerItemNum>100) PerItemNum = 100;

            //Rate 0.0~1.0
            PerRate = 1.0;

            //ADD TO STRUCTURE
            g_ShopInfoTbl.m_aShop[i].m_ItemList->AddType(itemType, PerItemNum, MaxItemNum, PerRate);
        }
    }

    __LEAVE_FUNCTION
}

VOID    TableInit::InitPKPointInfoTable()
{
__ENTER_FUNCTION

    DBCFile ThirdFile(0);
    BOOL ret = ThirdFile.OpenFromTXT( FILE_PK_POINT );
    if( ret != TRUE )
    {
        Assert( FALSE && "Read PK_Point Info table failed, please check it." );
        return;
    }

    INT iTableCount = ThirdFile.GetRecordsNum();
    g_PKPointTbl.m_nCount = iTableCount;

    Assert(iTableCount<=MAX_PK_NUM);

    INT itemnum = 0; 
    for(INT i = 0; i<iTableCount; i++)
    {
        g_PKPointTbl.m_aPKPointInfo[i].m_nID        =		ThirdFile.Search_Posistion(i,0)->iValue;
        g_PKPointTbl.m_aPKPointInfo[i].m_nMin	    =       ThirdFile.Search_Posistion(i,1)->iValue;
        g_PKPointTbl.m_aPKPointInfo[i].m_nMax	    =       ThirdFile.Search_Posistion(i,2)->iValue;
        g_PKPointTbl.m_aPKPointInfo[i].m_nValue	    =       ThirdFile.Search_Posistion(i,3)->iValue;
    }

    Log::SaveLog( SERVER_LOGFILE, "Load PK_Point.tab ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID	TableInit::InitPKLevelInfoTable()
{
__ENTER_FUNCTION

    DBCFile ThirdFile(0);
    BOOL ret = ThirdFile.OpenFromTXT( FILE_PK_LEVEL );
    if( ret != TRUE )
    {
        Assert( FALSE && "Read PK_Level Info table failed, please check it." );
        return;
    }

    INT iTableCount = ThirdFile.GetRecordsNum();
    g_PKLevelTbl.m_nCount = iTableCount;

    Assert(iTableCount<=MAX_PK_NUM);

    INT itemnum = 0; 
    for(INT i = 0; i<iTableCount; i++)
    {
        g_PKLevelTbl.m_aPKPointInfo[i].m_nID        =		ThirdFile.Search_Posistion(i,0)->iValue;
        g_PKLevelTbl.m_aPKPointInfo[i].m_nMin	    =       ThirdFile.Search_Posistion(i,1)->iValue;
        g_PKLevelTbl.m_aPKPointInfo[i].m_nMax	    =       ThirdFile.Search_Posistion(i,2)->iValue;
        g_PKLevelTbl.m_aPKPointInfo[i].m_nValue	    =       ThirdFile.Search_Posistion(i,3)->iValue;
    }

    Log::SaveLog( SERVER_LOGFILE, "Load PK_Level.tab ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID	TableInit::InitPKPunishInfoTable()
{
__ENTER_FUNCTION

    DBCFile ThirdFile(0);
    BOOL ret = ThirdFile.OpenFromTXT( FILE_PK_PUNISH );
    if( ret != TRUE )
    {
        Assert( FALSE && "Read PK_Punish Info table failed, please check it." );
        return;
    }

    INT iTableCount = ThirdFile.GetRecordsNum();
    g_PKPunishTbl.m_nCount = iTableCount;

    Assert(iTableCount<=MAX_PK_NUM);

    INT itemnum = 0; 
    for(INT i = 0; i<iTableCount; i++)
    {
        g_PKPunishTbl.m_aPKPointInfo[i].m_nID       =		ThirdFile.Search_Posistion(i,0)->iValue;
        g_PKPunishTbl.m_aPKPointInfo[i].m_nMin	    =       ThirdFile.Search_Posistion(i,1)->iValue;
        g_PKPunishTbl.m_aPKPointInfo[i].m_nMax	    =       ThirdFile.Search_Posistion(i,2)->iValue;
        g_PKPunishTbl.m_aPKPointInfo[i].m_nValue	=       ThirdFile.Search_Posistion(i,3)->iValue;
    }

    Log::SaveLog( SERVER_LOGFILE, "Load PK_Punish.tab ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID    TableInit::InitDyShopInfoTable()
{
    __ENTER_FUNCTION

    DBCFile ThirdFile(0);
    BOOL ret = ThirdFile.OpenFromTXT( FILE_DY_SHOP );
    if( ret != TRUE )
    {
        Assert( FALSE && "Read DyShop Info table failed, please check it." );
        return;
    }

    INT iTableCount = ThirdFile.GetRecordsNum();
    g_DyShopInfoTbl.m_iCount = iTableCount;

    Assert(iTableCount<=MAX_DYSHOP_GROUP);

    INT itemnum = 0; 
    for(INT i = 0; i<iTableCount; i++)
    {
        g_DyShopInfoTbl.m_aGroupInfo[i].m_GroupId		=		ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_GROUPID)->iValue;
        g_DyShopInfoTbl.m_aGroupInfo[i].m_GoodsNum	    =       ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_NUM)->iValue;
        itemnum                                 		=		g_DyShopInfoTbl.m_aGroupInfo[i].m_GoodsNum;

        //��������
        INT k = 0, j = 0, num = 0;
        while(k < itemnum*DYSHOP_GROUP::DY_ITEM_PROPERTY_NUM)
        {
            g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_ItemID	     = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;
            g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_ItemNumMin    = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;
            g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_ItemNumMax    = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;

            Assert(g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_ItemNumMin <= g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_ItemNumMax);

            g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_Rate          = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;
            g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_RandPriceType = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;
            g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_PriceNum      = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;

            for (INT j=0; j< 10; ++j)
            {
                g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_PriceType[j]  = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;
                g_DyShopInfoTbl.m_aGroupInfo[i].m_Goods[num].m_Price[j]      = ThirdFile.Search_Posistion(i,DYSHOP_GROUP::DY_BEGIN + k++)->iValue;
            }
            ++num;
         }
    }

    Log::SaveLog( SERVER_LOGFILE, "Load DyShop.tab ... OK! " ) ;


    __LEAVE_FUNCTION
}

VOID	TableInit::InitAIStateList()
{
__ENTER_FUNCTION
	g_StateList.InitStateList();
	Log::SaveLog( SERVER_LOGFILE, "Load InitAIStateList ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID	TableInit::CleanUpAIStateList()
{
__ENTER_FUNCTION
	g_StateList.ReleaseState();
	Log::SaveLog( SERVER_LOGFILE, "Load CleanUpAIStateList ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID TableInit::InitNPCPaoPaoIndexTable()
{
__ENTER_FUNCTION
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_NPCPAOPAOINDEXTBL );

	INT iTableCount = ThirdFile.GetRecordsNum();

	Assert(iTableCount<=MAXTYPE_NUMBER);

	memset((VOID*)&g_NPCPaoPaoIndexTbl,-1, sizeof(_NPC_PAOPAO_INDEX_TBL) );
	g_NPCPaoPaoIndexTbl.m_iCount = iTableCount;
	INT nIndex = 0;
	for (INT i = 0; i < iTableCount; ++i)
	{
		nIndex = ThirdFile.Search_Posistion(i, 0)->iValue;
		g_NPCPaoPaoIndexTbl.m_NPCPaoPaoIndex[i].m_iDataIndex = nIndex;
		g_NPCPaoPaoIndexTbl.m_NPCPaoPaoIndex[i].m_nBaseIndex = ThirdFile.Search_Posistion(i, 2)->iValue;
		g_NPCPaoPaoIndexTbl.m_NPCPaoPaoIndex[i].m_nIndexStep = ThirdFile.Search_Posistion(i, 1)->iValue;
	}
	Log::SaveLog( SERVER_LOGFILE, "Load NPCPaoPaoIndexTable.txt ... OK! " ) ;
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
    Log::SaveLog( SERVER_LOGFILE, "Load GuildLevelInfoTable.txt ... OK! " ) ;
    __LEAVE_FUNCTION
}

VOID TableInit::InitGMTable( )
{
__ENTER_FUNCTION

	Ini ini( FILE_GMLIST ) ;

	_GM_TABLE tmp ;

	tmp.m_uCount = ini.ReadInt( "gm", "count" ) ;
	for( INT i=0; i<tmp.m_uCount; i++ )
	{
		CHAR szguid[64] ;
		sprintf( szguid, "guid%d", i ) ;
		tmp.m_aGUID[i] = (GUID_t)(ini.ReadInt( "gm", szguid )) ;
	}

	g_GMTbl = tmp ;

	Log::SaveLog( SERVER_LOGFILE, "Load GMList.txt ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID TableInit::ReadDayTipsBeginAndEndTime(SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
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
		
		//��ȡ~�����ַ����е�һ�γ��ֵ�λ��
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


VOID TableInit::ReadDayTipsValidWeek( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
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

	for( INT id=0; id<iLineCount; ++id )
	{
		temp = atoi(pSplitL2->GetResultLine(id)) ;
		Field1 = temp / 10;
		Field2 = temp % 10;

		Assert( MONTH_WEEK_NUM >= Field1 && WEEK_DAY_NUM>=Field2 );
		if( MONTH_WEEK_NUM < Field1 || WEEK_DAY_NUM < Field2)
		{
			return;
		}

		if ( Field1==0 )			//������
		{
			for ( int i=0;i<MONTH_WEEK_NUM;i++ )
			{
				if ( Field2==0 )	//������
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

			
			}
		}
		else
		{
			if ( Field2==0 )		//������
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
	
		}

	}//for end

}//ReadDayTipsValidWeek end


VOID TableInit::ReadDayTipsData( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
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


VOID TableInit::ReadTiemrDoingData( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
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
				pVal[id] = 1;
				return;
			}
		}
		else
            pVal[atoi(pSplitL2->GetResultLine(id))] = atoi(pSplitL2->GetResultLine(id));
	}

}

VOID TableInit::ReadTiemrDoingMapInfo( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
								const CHAR* pSplitString, INT* pVal1,INT* pVal2,INT MaxCnt )
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

	//if ( atoi(pSplitL2->GetResultLine(0)) == 0 )
	//{
	//	*pVal1 = INVALID_ID;
	//	*pVal2 = INVALID_ID;
	//}
	//else
	{
		*pVal1 = atoi(pSplitL2->GetResultLine(0));
		*pVal2 = atoi(pSplitL2->GetResultLine(1));
	}

}

VOID TableInit::ReadWeaponID( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, const CHAR* pSplitString, INT iType )
{
	pSplitL1->Reset();
	pSplitL2->Reset();
	pSplitL1->Init( '|', pSplitL2 );
	pSplitL2->Init( '~', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iLineCount	= pSplitL2->GetResultLineCount();

	Assert( MONSTER_EXT_ATTR_ENUM_SIZE >= iLineCount );
	if( MONSTER_EXT_ATTR_ENUM_SIZE < iLineCount )
	{
		return;
	}
	for( INT iWeapon=0; iWeapon<iLineCount; ++iWeapon )
	{
		g_MonsterAttrExTbl.m_TableExt[iType].m_WeaponID[iWeapon] = atoi(pSplitL2->GetResultLine(iWeapon));
	}
	g_MonsterAttrExTbl.m_TableExt[iType].m_iWeaponCount = iLineCount;
}

VOID TableInit::ReadHorseID( SplitStringLevelOne *pSplitL1, SplitStringLevelTwo *pSplitL2, const CHAR* pSplitString, INT iType )
{
	pSplitL1->Reset();
	pSplitL2->Reset();
	pSplitL1->Init( '|', pSplitL2 );
	pSplitL2->Init( '~', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iLineCount	= pSplitL2->GetResultLineCount();
	Assert( MONSTER_EXT_ATTR_ENUM_SIZE >= iLineCount );
	if( MONSTER_EXT_ATTR_ENUM_SIZE < iLineCount )
	{
		return;
	}
	for( INT iHorse=0; iHorse<iLineCount; ++iHorse )
	{
		g_MonsterAttrExTbl.m_TableExt[iType].m_HorseID[iHorse] = atoi(pSplitL2->GetResultLine(iHorse));
	}
	g_MonsterAttrExTbl.m_TableExt[iType].m_iHorseCount = iLineCount;
}

VOID TableInit::InitMonsterSkillTable()
{
	enum MonsterSkillIndex
	{
		MSI_Index		= 0,
		MSI_SkillIndex	= 1,
		MSI_WeaponID	= 3,
		MSI_SkillID		= 4
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MONSTER_SKILL );


	INT iTableCount		=		ThirdFile.GetRecordsNum();
	INT iColumnCount	=		ThirdFile.GetFieldsNum();

	Assert(iTableCount<=MonsterWeaponAndSkillTable::MWAST_MAX_NUM) ;
	if( MonsterWeaponAndSkillTable::MWAST_MAX_NUM < iTableCount )
	{
		return;
	}
	g_MonsterWeaponAndSkill.m_Count	= iTableCount;
	
	SplitStringLevelOne oSplitL1;
	SplitStringLevelTwo oSplitL2;
	const CHAR* pSplitString = NULL;

	for(INT i = 0;i<iTableCount;++i)
	{
		INT iIndex = ThirdFile.Search_Posistion(i,MSI_Index)->iValue;
		g_MonsterWeaponAndSkill.m_aWeaponSkill[iIndex].m_iIndex		= iIndex;
		g_MonsterWeaponAndSkill.m_aWeaponSkill[iIndex].m_iWeaponID	= ThirdFile.Search_Posistion(i,MSI_WeaponID)->iValue;
		pSplitString												= ThirdFile.Search_Posistion(i,MSI_SkillIndex)->pString;
		oSplitL1.Reset();
		oSplitL2.Reset();
		oSplitL1.Init( '|', &oSplitL2 );
		oSplitL2.Init( '~', NULL );
		oSplitL1.DoSplit( pSplitString );
		INT iLineCount	= oSplitL2.GetResultLineCount();
		Assert( MonsterWeaponAndSkillTable::MWAST_MAX_NUM >= iLineCount );
		if( MonsterWeaponAndSkillTable::MWAST_MAX_NUM < iLineCount )
		{
			break;
		}
		for( INT iLine=0; iLine<iLineCount; ++iLine )
		{
			g_MonsterWeaponAndSkill.m_aWeaponSkill[iIndex].m_aSkillLevelIndex[iLine] = atoi(oSplitL2.GetResultLine(iLine)) - 1;
		}
		g_MonsterWeaponAndSkill.m_aWeaponSkill[iIndex].m_iSkillLevelIndexCount = iLineCount;

		for( INT j=0; j<MonsterWeaponAndSkill::MWAS_MAX_LEVEL; ++j )
		{
			g_MonsterWeaponAndSkill.m_aWeaponSkill[iIndex].m_aSkillLevel[j] = ThirdFile.Search_Posistion(i,MSI_SkillID+j)->iValue;
		}
	}
}

VOID TableInit::InitSKillStudyTable()
{
	enum SkillStydyIndex
	{
		SSI_ID	= 0,				
		SSI_PLayerLevelRequire,
		SSI_OccupationRequire,		
		SSI_PlayerPointsRequire,
		SSI_ParentSkillID,	
		SSI_ParentSkillLevelRequire,
		SSI_SpendPoints,	
		SSI_SpendMoney,			
		SSI_SpendExp,
		SSI_SpendItem1ID,
		SSI_SpendItem1Num,
		SSI_SpendItem2ID,
		SSI_SpendItem2Num,
		SSI_SpendItem3ID,
		SSI_SpendItem3Num,
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_SKILL_STUDY );

	if( ret != TRUE )
	{
		Assert( FALSE && "InitSKillStudyTable failed, please check it." );
		return;
	}

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	Assert(iTableCount<=Skill_Study_Condition_TABLE::MAX_Skill_COUNT);

	g_SkillStudyCondition.m_iCount = iTableCount;
	SplitStringLevelOne oSplitL1;
	const CHAR* pSplitString = NULL;

	for(INT i = 0;i<iTableCount;++i)
	{
		INT iIndex = ThirdFile.Search_Posistion(i,SSI_ID)->iValue;
		g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iID		= iIndex;
		pSplitString = ThirdFile.Search_Posistion(i,SSI_PLayerLevelRequire)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iPLayerLevelRequire );

		g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iOccupationRequire = ThirdFile.Search_Posistion(i,SSI_OccupationRequire)->iValue;
		//ReadSkillLearn( &oSplitL1, pSplitString, i, g_SkillStudyCondition.m_aSkillStydy_Condition[i].m_iOccupationRequire );

		pSplitString = ThirdFile.Search_Posistion(i,SSI_PlayerPointsRequire)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iPlayerPointsRequire );

		g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iParentSkillID = ThirdFile.Search_Posistion(i,SSI_ParentSkillID)->iValue;
		//ReadSkillLearn( &oSplitL1, pSplitString, i, g_SkillStudyCondition.m_aSkillStydy_Condition[i].m_iParentSkillID );

		pSplitString = ThirdFile.Search_Posistion(i,SSI_ParentSkillLevelRequire)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iParentSkillLevelRequire );

		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendPoints)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendPoints );

		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendMoney)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendMoney );

		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendExp)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendExp );


		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendItem1ID)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendItem1ID );
		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendItem1Num)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendItem1Num );

		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendItem2ID)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendItem2ID );
		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendItem2Num)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendItem2Num );

		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendItem3ID)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendItem3ID );
		pSplitString = ThirdFile.Search_Posistion(i,SSI_SpendItem3Num)->pString;
		ReadSkillLearn( &oSplitL1, pSplitString, g_SkillStudyCondition.m_aSkillStydy_Condition[iIndex].m_iSpendItem3Num );
	}
}

VOID TableInit::ReadSkillLearn( SplitStringLevelOne *pSplitL1, const CHAR* pSplitString, INT* pArry )
{
	pSplitL1->Reset();
	pSplitL1->Init( '|', NULL );
	pSplitL1->DoSplit( pSplitString );
	INT iCount = pSplitL1->GetResultLineCount();
	Assert( Skill_Study_Condition::MAX_SKILL_LEVEL >= iCount );
	if( Skill_Study_Condition::MAX_SKILL_LEVEL < iCount )
	{
		return;
	}
	for( INT j=0; j<iCount; ++j )
	{
		pArry[j] = atoi( pSplitL1->GetResultLine(j) );
	}
	//��������ʡ�����ظ������ݣ�Ҫ�ڴ��õ�һ��ֵ��䱻ʡ�Ե�ֵ
	if( iCount < Skill_Study_Condition::MAX_SKILL_LEVEL )
	{
		for( INT i=iCount; i<Skill_Study_Condition::MAX_SKILL_LEVEL; ++i )
		{
			pArry[i] = pArry[0];
		}
	}
}

//by zheng
VOID TableInit::InitOpenBoxInfoTable()
{

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_OPENBOX_ITEM );

	if( ret != TRUE )
	{
		Assert( FALSE && "InitOpenBoxInfoTable failed, please check it." );
		return;
	}

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	Assert( iTableCount<=OpenBox_ItemInfo_Table::ITEM_KIND_MAX_NUM && iTableCount>= 0 );
	Assert( iTableColumn<=OpenBox_ItemInfo_Table::COLUMN_MAX_NUM && iTableColumn >=0 );

	g_OpenBox_ItemInfo_Table.m_KindRealNum = iTableCount;

	for(INT i = 0;i<iTableCount;++i)
	{
		int nIndex = 0;

		g_OpenBox_ItemInfo_Table.m_ItemInfoRow[i].m_Index = ThirdFile.Search_Posistion(i,nIndex++)->iValue;

		for ( INT j=0; j<OpenBox_ItemInfo_Table::ItemInfoRow::ITEM_INFO_MAX_NUM; ++j )
		{
			if ( ThirdFile.Search_Posistion(i,nIndex)->iValue > 0 )
			{
			g_OpenBox_ItemInfo_Table.m_ItemInfoRow[i].m_ItemInfo[j].m_ItemID
				= ThirdFile.Search_Posistion(i,nIndex++)->iValue;

			g_OpenBox_ItemInfo_Table.m_ItemInfoRow[i].m_ItemInfo[j].m_Rate1
				= ThirdFile.Search_Posistion(i,nIndex++)->iValue;

			g_OpenBox_ItemInfo_Table.m_ItemInfoRow[i].m_ItemInfo[j].m_Rate2
				= ThirdFile.Search_Posistion(i,nIndex++)->iValue;

			g_OpenBox_ItemInfo_Table.m_ItemInfoRow[i].m_RealNum++;

			}
			else
			{
				break;
			}

		}

	}

}

VOID TableInit::InitDurSpoilInfoTable()
{
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_DUR_SPOIL );

	if( ret != TRUE )
	{
		Assert( FALSE && "InitDurSpoilInfoTable failed, please check it." );
		return;
	}

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	for(INT i = 0;i<iTableCount;++i)
	{
		int nIndex = 0;
		INT EquipPos = ThirdFile.Search_Posistion(i,nIndex++)->iValue;
		Assert(EquipPos>-1);
		Assert(EquipPos<EQUIP_TYPE_NUM);

		g_DurSpoilValueTbl.m_Table[EquipPos].m_nDamageMode = ThirdFile.Search_Posistion(i,nIndex++)->iValue;
		g_DurSpoilValueTbl.m_Table[EquipPos].m_nValue      = ThirdFile.Search_Posistion(i,nIndex)->iValue;

	}
}

VOID TableInit::InitBusInfoTable()
{
	__ENTER_FUNCTION

		DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_BUSINFOTABLE );

	INT iTableCount = ThirdFile.GetRecordsNum();
	Assert(iTableCount<=MAX_BUS_INFO_COUNT);

	INT nPreTotalRate = 0;
	for (INT i = 0; i < iTableCount; ++i)
	{
		INT nDataID = ThirdFile.Search_Posistion(i, 0)->iValue;
		if(nDataID < MAX_BUS_INFO_COUNT)
		{
			g_BusInfoTbl.m_aBusInfo[nDataID].m_nDataID				= nDataID;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_pszName				= NULL;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_nScriptID			= ThirdFile.Search_Posistion(i, 2)->iValue;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_nType				= ThirdFile.Search_Posistion(i, 3)->iValue;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_nStopTime			= ThirdFile.Search_Posistion(i, 4)->iValue;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_nModelID				= ThirdFile.Search_Posistion(i, 5)->iValue;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_fModelHeight			= ThirdFile.Search_Posistion(i, 6)->fValue;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_nMaxPassengerCount	= ThirdFile.Search_Posistion(i, 7)->iValue;
			g_BusInfoTbl.m_aBusInfo[nDataID].m_fMoveSpeed			= ThirdFile.Search_Posistion(i, 8)->fValue;
			g_BusInfoTbl.m_abUsed[nDataID]							= TRUE;
			g_BusInfoTbl.m_nCount++;
		}
	}	

	DiskLog( SERVER_LOGFILE, "Load BusInfoTbl.txt ... OK! " ) ;

	__LEAVE_FUNCTION
}
