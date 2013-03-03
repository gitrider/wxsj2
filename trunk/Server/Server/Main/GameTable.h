
#ifndef __GAMETABLE_H__
#define __GAMETABLE_H__

#include "Type.h"
#include "DB_Struct.h"
#include "PetConfig.h"
#include "DataRecords.h"
#include "CampAndStand.h"
#include "TemplateManager.h"
#include "FileDataMgr.h"
#include "GameStruct_Pet.h"
#include "ShopManager.h"
#include "GameStruct_Bus.h"

class AIScript;

//����ϵ����
struct _BASE_VALUE_TBL
{
	//����Ӱ����������*��������
	INT		m_Table[AINFOTYPE_NUMBER][MATTRIBUTE_NUMBER] ;

	_BASE_VALUE_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*AINFOTYPE_NUMBER*MATTRIBUTE_NUMBER ) ;
	};
	INT		Get( uint AttrInfo, uint MenPai ){
		Assert( AttrInfo<AINFOTYPE_NUMBER && MenPai<MATTRIBUTE_NUMBER ) ;
		return m_Table[AttrInfo][MenPai] ;
	};
};
extern _BASE_VALUE_TBL g_BaseValueTbl ;


#define MAX_NPC_DIALOG_LEN 70
#define MAX_MIS_NAME_LEN 24
#define MAX_MIS_TEXT_LEN 24			//��������



/*****************************************************************
ѭ���������
*****************************************************************/
//ѭ����������
struct XUNHUAN_MISSION_ATTR
{
	BOOL	m_Used;					//�Ƿ��ѳ�ʼ��
	INT		m_Group;				//������
	INT		m_SubMissionType;		//����������
	INT		m_SubMissionId;			//������ID
	INT		m_ObjId;				//Ҫɱ�Ĺ��������ʼ�NPC��ID������ƷID
	CHAR	m_ObjName[NPC_NAME_LEN];//Ŀ�Ĺ���/NPC/ITEM��
	INT		m_ObjCount;				//Ҫɱ�Ĺ������� /Ѱ������� ����������Ϊ1
	INT		m_ScendId;				//OBJ���ڳ���ID
	CHAR	m_SceneName[SCENE_NAME_LEN];		
	INT		m_PosX;
	INT		m_PosZ;

	CHAR	m_DialogText[MAX_NPC_DIALOG_LEN];	//NPC�Ի�����
	
};

#define MAX_XUNHUAN_MISSION_GROUP_NUMBER 10		//ѭ�����������10�飬��ҵȼ�����Ӧ����
#define GROUP_IN_SUB_MISSION_NUMBER 	 50		//ÿ���������50��������
#define SUB_MISSION_TYPE_NUMBER			 10		//ÿ���������10�����͵�������

struct _SUB_MISSION_INFO
{
	INT m_SubMissionCnt;						//���ڸ������������
	INT M_SubMissionPos;						//���ڸ����������λ��
};	

//ѭ����������
struct _XUNHUAN_MISSION_ATTR_TBL
{
	XUNHUAN_MISSION_ATTR		m_TableExt[MAX_XUNHUAN_MISSION_GROUP_NUMBER][GROUP_IN_SUB_MISSION_NUMBER] ;
	
	
	_SUB_MISSION_INFO			m_SubMissionInfo[MAX_XUNHUAN_MISSION_GROUP_NUMBER][SUB_MISSION_TYPE_NUMBER];
	uint						m_Count ;
	uint						m_MaxType;

	_XUNHUAN_MISSION_ATTR_TBL()
	{
		memset( m_TableExt, 0, sizeof(XUNHUAN_MISSION_ATTR)*MAX_XUNHUAN_MISSION_GROUP_NUMBER*GROUP_IN_SUB_MISSION_NUMBER ) ;
		memset( m_SubMissionInfo, 0, sizeof(_SUB_MISSION_INFO)*MAX_XUNHUAN_MISSION_GROUP_NUMBER*SUB_MISSION_TYPE_NUMBER ) ;

		m_Count = 0 ;
	};
	
	INT		Count( ){ return m_Count ; } ;

};
extern _XUNHUAN_MISSION_ATTR_TBL g_XunHuanMissionAttrTbl ;



/*****************************************************************
ÿ����ʾ���
*****************************************************************/
const INT MAX_LEVEL_NUM = 100;
const INT MAX_DAY_TIPS_NUM	 = 256;
const INT MONTH_WEEK_NUM		 = 7;	//ÿ�����6��
const INT WEEK_DAY_NUM			 = 7;	//ÿ�����7��


//�������Чʱ��Ƭ
struct VALID_TIME_SLICE
{
	tm		m_Begin;		//��ʼʱ��
	tm		m_End;			//����ʱ��
};


//ÿ����ʾ����
struct DAY_TIPS_ATTR
{
	BOOL	m_Used;						//�Ƿ��ѳ�ʼ��
	INT		m_DayNominate;				//���Ƽ�
	INT		m_MonthNominate;			//���Ƽ�
	INT     m_ShowAll;					//�Ƿ�ȫ���ɼ�	0Ϊ��1Ϊ�����˿ɼ�

	//�ɼ��ȼ�	������0��ʾ���еȼ��Ρ�
	//��������0ʱ���Ὣ��������Ԫ����1,�����о�û��0��.
	//�˺�0��ʾ���ɼ���1��ʾ�ɼ���
	INT     m_ShowLevel[MAX_LEVEL_NUM];	

	//�ɼ�����,	0��ʾ���й��ҡ�
	//����ͬm_ShowLevel
	INT		m_ShowCountry[MAX_COUNTRY_NUM];	

	//�ɼ����		0��ʾ���а��
	//����ͬm_ShowLevel
	INT		m_ShowGuild[GUILD_FAMILY_SIZE];	

	//�ɼ�����		0��ʾ�м���		
	INT		m_ShowFamily[GUILD_FAMILY_SIZE];		

	INT		m_MisType;					//��������		0��ʾѭ������1��ʾ��ʱ�淨��
	CHAR	m_MisName[MAX_MIS_NAME_LEN];//��������

	//������Ч�·�		0��ʾ�����£������ʾ������������·�
	INT		m_MisValidMonth[13];			
	INT		m_WeekOrDay;				//���������ڻ�����������0��ʾ����

	//m_WeekOrDay=1ʱ��������Ч����,0��ʾ����ʱ��Ҫע��ÿ���²��Ƕ���31��
	INT    m_MisValidDate[32];

	//��������Ϊ��λ����
	//��һ���ֶα�ʾ�ڼ��ܣ�Ϊ0��ʾ������
	//�ڶ����ֶα�ʾ�ܼ���Ϊ0��ʾ����
	INT		m_MisValidWeek[MONTH_WEEK_NUM][WEEK_DAY_NUM];	

	//����ʼ��������ʱ��,Ϊ0��ʾ����ʱ
	VALID_TIME_SLICE	m_VailidTimeSlice;

	INT		m_MisPos;					//��ʾ��ȡ�������������ر������ű�λ��,Խ��Խ��һͷ��ˮ
	INT		m_MisCount;					//ÿ�տ���������0��ʾ���޴���
    CHAR	m_MisText[MAX_MIS_TEXT_LEN];//��������
	CHAR	m_NpcName[NPC_NAME_LEN];	//������
	INT		m_ScendId;					//OBJ���ڳ���ID
	CHAR	m_SceneName[SCENE_NAME_LEN];		
	INT		m_PosX;
	INT		m_PosZ;

	// �������������������������ţ���ӦScriptGlobal.lua��MissionData�и������ݵĶ���
	INT		m_DoneCountIndex;	    // ���ɴ����±� 
	INT		m_DoneCircleIndex;  	// ���ɻ����±�
	INT		m_LastDoneTimeIndex;    // ����һ�����ʱ���±�
	INT		m_LastAbandonTimeIndex; // ����һ�η���ʱ���±�

};

//ÿ����ʾ����
struct _DAY_TIPS_ATTR_TBL
{
	DAY_TIPS_ATTR		m_TableExt[MAX_DAY_TIPS_NUM] ;
	
	uint						m_Count ;
	uint						m_MaxType;

	_DAY_TIPS_ATTR_TBL()
	{
		memset( m_TableExt, 0, sizeof(DAY_TIPS_ATTR)*MAX_DAY_TIPS_NUM ) ;
		m_Count = 0 ;
	};
	
	INT		Count( ){ return m_Count ; } ;

};

extern _DAY_TIPS_ATTR_TBL g_DayTipsAttrTbl ;


/*****************************************************************
����ת�ĵ�ͼ��Ϣ
*****************************************************************/
#define MAX_MAP_INFO_NUM 50

struct _MAP_INFO_ATTR
{
	INT			m_Index;
	SceneID_t	m_SceneId;
	CHAR		m_SceneName[SCENE_NAME_LEN];
	INT			m_DestPosX;			//�ɵ����Ŀ�ĵ�
	INT			m_DestPosZ;
	INT			m_SceneType;
	INT			m_MinUserLevel;		//��ҽ��볡������С�ȼ�
	INT			m_MinMapLevel;		//��ͼ�ϵ�NPC��С�ȼ�
	CHAR		m_LetterIndex;

};


struct _MAP_INFO_ATTR_TAB
{
	_MAP_INFO_ATTR		m_TableExt[MAX_MAP_INFO_NUM] ;
	
	uint						m_Count ;

	_MAP_INFO_ATTR_TAB()
	{
		memset( m_TableExt, 0, sizeof(_MAP_INFO_ATTR)*MAX_MAP_INFO_NUM ) ;
		m_Count = 0 ;
	};
	
	INT		Count( ){ return m_Count ; } ;
};

extern _MAP_INFO_ATTR_TAB g_MapInfoAttrTbl ;


/*****************************************************************
ϵͳ��ʱ�������ݽṹ����
*****************************************************************/

//ϵͳ��ʱ���������	
#define TIMER_DOING_NAME_LEN 30

//���ݵĲ�������
#define PARAMETER_NUM 5

//�������ݳ�������
#define REPORT_TEXT_LEN 256

#define TIMER_DOING_MAP_NUM 10

#define MAX_TIMER_DOING_NUM 50

#define MAX_TIEMR_PARAM_NUM 8

//����������
enum REPORT_TYPE
{
	REPORT_TYPE_INVALID = 0,
	REPORT_TYPE_SLOW,		//����
	REPORT_TYPE_QUICK,		//�챨
	REPORT_TYPE_EARLIER,	//��տ�ʼʱ�ı���
	REPORT_TYPE_MID,		//����й����еı���
	REPORT_TYPE_LAST,		//������ʱ�ı���
	REPORT_TYPE_NUM,

};

struct _TIMER_DOING_ATTR
{
	INT			m_Index;

	//�������
	CHAR		m_szName[TIMER_DOING_NAME_LEN];

	INT			m_ScriptId;			//��Ӧ�Ľű�ID

	//�ִ�����ͣ�0��ʾ��ִ��һ�Σ�
	//1��ʾ����Чʱ�����ÿ�η�������������ִ��һ��
	INT			m_OperateType;		

	//ʱ��������ͣ�0��ʾ��ִ��һ��
	//1��ʾÿ��ִ��һ�Σ�2��ʾÿ��ִ��һ�Σ�3��ʾÿ��ִ��һ��
	INT			m_TimeType;	

	//��ʼ&����ʱ���:	�ܹ�7λ����XXXXXXX����3221330��
	//ʱ��������ͣ�0ʱ��(ǰ��λ��ʾһ���е�����1��365������λ��ʾһ��ʱ��[24Сʱ��])
	//ʱ��������ͣ�1ʱ��(ǰ��λ�����壬����λ��ʾһ��ʱ��[24Сʱ��])
	//ʱ��������ͣ�2ʱ��(ǰ��λ�����壬����λ��ʾ������0=������;1=����һ������λ��ʾһ��ʱ��[24Сʱ��])
	//ʱ��������ͣ�3ʱ��(ǰһλ�����壬�ڶ�����λ��ʾ���µ�����(01-31)������λ��ʾһ��ʱ��[24Сʱ��])
    INT			m_StartTime;
	INT			m_EndTime;

	//���ݵĲ���
	INT			m_Parameter[PARAMETER_NUM];

	//�㲥���ͣ�
	//"0����������촰����
	//1���½Ǹ��˵�ϵͳ��ʾ��
	//2���½�ϵͳ��ϵͳ��ʾ��
	//3��ֱ������ʾ��
	//4ˮƽ������ʾ��
	//��д��ʽ��
	//3|2|4|1|0�����ʾ��Ӧ������ط�������ֺ���Ĺ㲥
	//3|2|4|�����ʾ��Ӧ�������ط�������ֺ���Ĺ㲥"
	INT			m_BroadcastType[CHAR_SHOW_POS_NUM];

	//������������ݡ�����ʱ��
	INT			m_SlowReportInterval;
	CHAR		m_SlowReportText[REPORT_TEXT_LEN];
	INT			m_SlowReportEndTime;

	//�챨��������ݡ�����ʱ��
	INT			m_QuickReportInterval;
	CHAR		m_QuickReportText[REPORT_TEXT_LEN];
	INT			m_QuickReportEndTime;

	//��տ�ʼʱ�ı�����������ݡ�����ʱ��
    INT			m_EarlierReportInterval;
	CHAR		m_EarlierReportText[REPORT_TEXT_LEN];
	INT			m_EarlierReportEndTime;

	//����й����еı�����������ݡ�����ʱ��
    INT			m_MidReportInterval;
	CHAR		m_MidReportText[REPORT_TEXT_LEN];
	INT			m_MidReportEndTime;

	//������ʱ�ı�����������ݡ�����ʱ��
    INT			m_LastReportInterval;
	CHAR		m_LastReportText[REPORT_TEXT_LEN];
	INT			m_LastReportEndTime;

	//m_Ruler��-1��ʹ��m_TempMapId��m_Ruler��0����������Map�
	//m_Ruler��1������m_Ruler��Map��
	INT			m_Ruler;

	//ֻ�е�m_Ruler��-1ʱ����ʹ��
	INT			m_TempMapId;

	//��ʽΪ��MapId|Num������  0|20��MapId�����ͼID��Num����õ�ͼ��Ȩ�ء�
	//��ʹ�õ�Map_n���д-1|-1
	//Num��m_MapRatio
	INT			m_MapId[TIMER_DOING_MAP_NUM];
	INT			m_MapRatio[TIMER_DOING_MAP_NUM];

	//����m_MapRatio����֮��
	INT			m_RatioTotal;

};

struct _TIMER_DOING_ATTR_TAB
{
	_TIMER_DOING_ATTR		m_TableExt[MAX_TIMER_DOING_NUM] ;
	
	uint						m_Count ;

	_TIMER_DOING_ATTR_TAB()
	{
		memset( m_TableExt, 0, sizeof(_TIMER_DOING_ATTR)*MAX_TIMER_DOING_NUM ) ;
		m_Count = 0 ;
	};
	
	INT		Count( ){ return m_Count ; } ;
};

extern _TIMER_DOING_ATTR_TAB g_TiemrDoingAttrTbl ;
//ϵͳ��ʱ�������ݽṹ�������

const INT MONSTER_EXT_ATTR_ENUM_SIZE = 100;
struct MONSTER_EXT_ATTR
{
	BOOL	m_Used;					//�Ƿ��ѳ�ʼ��
	INT		m_Type;					//��������
	CHAR	m_Name[NPC_NAME_LEN];	//����������
	INT		m_CannotFight;			//���ɱ�����
	INT		m_Level;				//�ȼ�
	INT		m_RespawnTime;			//����ʱ��
	INT		m_BaseExp;				//����������
	INT		m_OutLook;				//���м��ʱ��
	INT		m_BaseAI;				//����AI ID
	INT		m_ExtAI;				//��չAI ID
	INT		m_Camp;					//��Ӫ
	FLOAT	m_DropSearchRange;		//�������뾶���ף�
	INT		m_SearchTeamCount;		//���������Ч��
	INT		m_MonsterDropItemSetID; //������Ʒ��
	INT		m_nMonsterDropRate;		//�������
	INT		m_MinDamagePercent;		//��С��Ѫ�ٷֱ�
	INT		m_CanHandle;			//�Ƿ���Խ���
	INT		m_FriendValue;			//�Ѻ�ֵ
	INT		m_WalkSpeed;			//�����ٶ�
	INT		m_BossFlag;				//BOSS���
	INT		m_SnapSkillImmID;		//˲����������ID
	INT		m_DurationSkillImmID;	//������������ID
	INT		m_AttackAnimTime;		//��������ʱ��
	INT		m_AttackCooldownTime;	//������ȴʱ��
	INT		m_ScriptID;				//�ű�����ID
	INT		m_WeaponID[MONSTER_EXT_ATTR_ENUM_SIZE];		//��������ID
	INT		m_HorseID[MONSTER_EXT_ATTR_ENUM_SIZE];		//�����ID
	INT		m_iWeaponCount;			//�ɹ����ѡ���������������
	INT		m_iHorseCount;			//�ɹ����ѡ�������������

	INT		m_aHorseDropID[MaxHorseDropNum];
	INT		m_iHorseDropCount;
	INT		m_Table[CATTR_LEVEL2_NUMBER];
};

//����������Ա�
struct _MONSTER_ATTREX_TBL
{

	MONSTER_EXT_ATTR		m_TableExt[MAXTYPE_NUMBER];
	uint					m_Count;
	uint					m_MaxType;

	_MONSTER_ATTREX_TBL()
	{
		memset(m_TableExt,0,sizeof(MONSTER_EXT_ATTR)*MAXTYPE_NUMBER);
		m_Count = 0 ;
	};
	static INT CompareMonsterAttrExtTB(const VOID* pArg1,const VOID* pArg2)
	{
		__ENTER_FUNCTION

			INT Serial1,Serial2;

		Serial1	=	((MONSTER_EXT_ATTR*)pArg1)->m_Type;
		Serial2	=	((MONSTER_EXT_ATTR*)pArg2)->m_Type;

		if(Serial1>Serial2)
			return 1;
		else if(Serial1<Serial2)
			return -1;
		else
			return 0;
		__LEAVE_FUNCTION	
			return 0 ;
	}

	INT	Get( uint MonsterType, uint AttrEx )
	{
		__ENTER_FUNCTION

		Assert(AttrEx<CATTR_LEVEL2_NUMBER);

		MONSTER_EXT_ATTR* pAttr = GetExtAttr( MonsterType );
		if( NULL != pAttr )
		{
			return pAttr->m_Table[AttrEx];
		}

		__LEAVE_FUNCTION
		return 0;
	};

	MONSTER_EXT_ATTR* GetExtAttr(uint MonsterType)
	{
		__ENTER_FUNCTION

		MONSTER_EXT_ATTR oAttr;
		oAttr.m_Type = MonsterType;
		MONSTER_EXT_ATTR* pResult = (MONSTER_EXT_ATTR*)bsearch(&oAttr,
			m_TableExt,
			m_Count,
			sizeof(MONSTER_EXT_ATTR),
			(INT (*)(const VOID*, const VOID*))CompareMonsterAttrExtTB);

		if( pResult != NULL )
		{
			return pResult;
		}
		__LEAVE_FUNCTION
		return	NULL;
	}
	INT		Count( ){ return m_Count ; } ;
};
extern _MONSTER_ATTREX_TBL g_MonsterAttrExTbl ;


//�ƺ���Ϣ��
struct _TITLE_INFO_TBL
{
    _TITLE::TITLE_INFO		                        m_TableInfo[MAX_TITLE_SIZE];

    _TITLE_INFO_TBL()
    {
        //memset( m_TableInfo, 0, sizeof(_TITLE::TITLE_INFO)*MAX_TITLE_SIZE ) ;
    };
    _TITLE::TITLE_INFO*			Get( UINT uTitileID )
    {
        Assert( uTitileID<MAX_TITLE_SIZE);
        return &m_TableInfo[uTitileID];
    };
};
extern _TITLE_INFO_TBL g_TitleInfoTbl ;

//���������Ϣ��
struct _GUILD_LEVEL_INFO_TBL
{
    _GUILD_LEVEL_INFO       m_TableInfo[MAX_GUILD_LEVEL];

    _GUILD_LEVEL_INFO_TBL()
    {
    };

};
extern _GUILD_LEVEL_INFO_TBL g_GuildLevelInfoTbl ;


//��ϳƺ��䷽��
struct _TITLE_COMBINATION_TBL
{
    _TITLE::TITLE_COMBINATION		                m_TableCombination[MAX_TITLE_SIZE];

    _TITLE_COMBINATION_TBL()
    {
    };

    _TITLE::TITLE_COMBINATION*			Get( UINT uCombinationID )
    {
        Assert( uCombinationID<MAX_TITLE_SIZE);
        return &m_TableCombination[uCombinationID];
    };
};
extern _TITLE_COMBINATION_TBL g_TitleComTbl ;

struct NPC_QUEST
{
	INT		m_Type;
	INT		m_ScriptID;
	INT		m_EventCount;
	INT		m_EventList[MAX_NPC_EVENT];
	

	NPC_QUEST()
	{
		m_Type = 0;
		m_ScriptID = -1;
		m_EventCount = 0;
		for(INT i = 0;i<MAX_NPC_EVENT;i++)
		{
			m_EventList[i] = 0;
		}
	}
};


#define NPC_EVENT_LENGTH 512

struct _NPC_QUEST_TBL
{

	NPC_QUEST			*m_Table;
	INT					m_Count;

	_NPC_QUEST_TBL()
	{
		m_Table	=	0;
		m_Count	=	0;
	}

	NPC_QUEST*	Get(INT NpcType);

};

extern	_NPC_QUEST_TBL g_NpcQuestTbl;

// NPC����˵��������
struct _NPC_PAOPAO_INDEX
{
	INT m_iDataIndex; // ����ID
	INT m_nBaseIndex; // NPCPaoPao��ʼ����ID
	INT m_nIndexStep; // NPCPaoPao����ID��Ӧ����Ч��Χ
};
struct _NPC_PAOPAO_INDEX_TBL
{
	_NPC_PAOPAO_INDEX m_NPCPaoPaoIndex[MAXTYPE_NUMBER];
	INT m_iCount;

	static INT CompareNpcPaoPao(const VOID* pArg1,const VOID* pArg2)
	{
		__ENTER_FUNCTION

			INT Serial1,Serial2;

		Serial1	=	((_NPC_PAOPAO_INDEX*)pArg1)->m_iDataIndex;
		Serial2	=	((_NPC_PAOPAO_INDEX*)pArg2)->m_iDataIndex;

		if(Serial1>Serial2)
			return 1;
		else if(Serial1<Serial2)
			return -1;
		else
			return 0;
		__LEAVE_FUNCTION	
			return 0 ;
	}
	_NPC_PAOPAO_INDEX* GetNpcPaoPaoData( INT iDataIndex )
	{
		__ENTER_FUNCTION
		_NPC_PAOPAO_INDEX oAttr;
		oAttr.m_iDataIndex = iDataIndex;

		_NPC_PAOPAO_INDEX* pResult = (_NPC_PAOPAO_INDEX*)bsearch(&oAttr,
			m_NPCPaoPaoIndex,
			m_iCount,
			sizeof(_NPC_PAOPAO_INDEX),
			(INT (*)(const VOID*, const VOID*))CompareNpcPaoPao);

		if( pResult != NULL )
		{
			return pResult;
		}

		__LEAVE_FUNCTION
		return	NULL;
	}

};
extern _NPC_PAOPAO_INDEX_TBL g_NPCPaoPaoIndexTbl;


//�������Ա�
struct _PET_ATTR_TBL
{
	PET_ATTR					m_TableExt[MAXTYPE_NUMBER];
	uint						m_Count ;
	uint						m_MaxType;

	_PET_ATTR_TBL()
	{
		memset(m_TableExt,0,sizeof(PET_ATTR)*MAXTYPE_NUMBER);
		m_Count = 0 ;
	};

	PET_ATTR	*GetAttr(uint PetType)
	{
		if (PetType<=m_MaxType)
			return	&m_TableExt[PetType];
		return NULL;
	}
	INT		Count( ){ return m_Count ; } ;
};
extern _PET_ATTR_TBL g_PetAttrTbl ;

//�������Ե���䷽����
#define PET_ATTRPOINT_DIS_NUM (1024)
struct _PET_ATTRPOINT_DIS_TBL
{
	struct _PET_ATTRPOINT_DIS
	{
		INT			m_ID;
		INT			m_StrRate;
		INT			m_ConRate;
		//INT			m_SprRate;
		INT			m_DexRate;
		INT			m_IntRate;
	};

	_PET_ATTRPOINT_DIS	m_Table[PET_ATTRPOINT_DIS_NUM];
    INT					m_Count;				//��������

	_PET_ATTRPOINT_DIS_TBL()
	{
		m_Count = 0;
	}

	// nType���������������Եȡ����� nID��������
	INT GetAttrDisRate(INT nType, INT nID)
	{
		Assert(nID < m_Count);
		Assert(nType < CATTR_LEVEL1_NUMBER);
		INT nRet;

		switch(nType)
		{
		case CATTR_LEVEL1_STR:
			nRet = m_Table[nID].m_StrRate;
			break;
		case CATTR_LEVEL1_CON:
			nRet = m_Table[nID].m_ConRate;
			break;
		/*case CATTR_LEVEL1_SPR:
			nRet = m_Table[nID].m_SprRate;
			break;*/
		case CATTR_LEVEL1_DEX:
			nRet = m_Table[nID].m_DexRate;
			break;
		case CATTR_LEVEL1_INT:
			nRet = m_Table[nID].m_IntRate;
			break;
		default:
			break;
		}
		return nRet;		
	}
};
extern _PET_ATTRPOINT_DIS_TBL g_PetAttrPointDisTbl;

// ����������
#define PET_LEVEL_NUM (105)
struct _PET_LEVELUP_TBL
{
	INT m_Table[PET_LEVEL_NUM];
	
	_PET_LEVELUP_TBL()
	{
		memset(m_Table, INVALID_ID, PET_LEVEL_NUM);
	}

	INT GetExp(INT nLevel)
	{
		Assert(nLevel <= PET_LEVEL_NUM);
		return m_Table[nLevel];
	}
	INT m_iCount;
};
extern _PET_LEVELUP_TBL g_PetLevelUpTbl;

// ����������
struct _PET_DOMESTICATION_MONEY_TBL
{
	FLOAT m_Table[PET_LEVEL_NUM];
	
	_PET_DOMESTICATION_MONEY_TBL()
	{
		memset(m_Table, 0, PET_LEVEL_NUM);
	}

	FLOAT GetDomesticationMoney(INT nLevel)
	{
		Assert(nLevel <= PET_LEVEL_NUM);
		return m_Table[nLevel];
	}
};
extern _PET_DOMESTICATION_MONEY_TBL g_PetDomesticationMoneyTbl;

// ���＼�����ɱ�
#define PET_MAX_NUM	(1024)
struct _PET_SKILLDISTR_TBL
{
	struct PET_SKILL_MAP
	{
		INT		m_nSkillIndex;
		INT		m_nSkillRange;

		PET_SKILL_MAP( )
		{
			m_nSkillIndex = -1;
			m_nSkillRange = 0;
		}
	};

	PET_SKILL_MAP	m_Table[PET_MAX_SKILL_COUNT - 1];
	INT				m_PetID;

};
extern _PET_SKILLDISTR_TBL g_PetSkillDistrTbl[PET_MAX_NUM];

// ���＼��������
#define PET_SKILL_NUM (512)
struct _PET_SKILLINDEX_TBL
{
	INT		m_nIndex;
	INT		m_nSkillID;
};
extern _PET_SKILLINDEX_TBL g_PetSkillIndexTbl[PET_SKILL_NUM];

// ����ѧϰ���ܼ��ʱ�
struct _PET_STUDYSKILL
{
	INT		m_nID;				// ID
	INT		m_nMaxSpaceCount;	// �ܿո�����
	INT		m_nSpaceCount;		// ��ǰ�ո�����
	INT		m_nNewPercent;		// �����¸��ϵİٷֱ�
};

#define MAKE_PET_STUDYSKILL_ID( x, y )	(x * 10 + y)
#define PET_STUDYSKILLRATE_NUM (56)
struct _PET_STUDYSKILL_TBL
{
	_PET_STUDYSKILL		m_aStudySkill[PET_STUDYSKILLRATE_NUM];

	const _PET_STUDYSKILL *Get( INT nMaxSpaceCount, INT nSpaceCount )const
	{
		INT nID = MAKE_PET_STUDYSKILL_ID( nMaxSpaceCount, nSpaceCount );
		if ( nID < PET_STUDYSKILLRATE_NUM )
		{
			return &m_aStudySkill[nID];
		}
		else
		{
			return NULL;
		}
	}

	_PET_STUDYSKILL_TBL( VOID )
	{
		memset( m_aStudySkill, 0, sizeof( m_aStudySkill ) );
	}
};
extern _PET_STUDYSKILL_TBL g_PetStudySkillTbl;

// ���������ܼ��ʱ�
#define PET_APPERCEIVESKILLRATE_NUM (24)
struct _PET_APPERCEIVESKILL_TBL
{
	INT		m_nSkillCount;					//���＼������
	INT		m_nLearnedVoluntarySkillCount;	//��������������
	INT		m_nLearnedPassiveSkillCount;	//���б���������
	INT		m_nApperceiveVoluntarySkillRate;//�����������ܸ��ʣ���λ1/10w��
	INT		m_nApperceivePassiveSkillRate;	//���򱻶����ܸ��ʣ���λ1/10w��

};
extern _PET_APPERCEIVESKILL_TBL g_PetApperceiveSkillTbl[PET_APPERCEIVESKILLRATE_NUM];

#define PET_AITYPE (5)
#define PET_SKILL_TYPE (5)
extern FLOAT g_PetAIStrategyTble[PET_SKILL_TYPE][PET_AITYPE];

const INT MAX_HORSE_TYPE = 256;
//�����Ա�
struct _HORSE_ATTR_TBL
{
	_HORSE_ATTR m_aHorseTbl[MAX_HORSE_TYPE];
	INT			m_iCount;
	_HORSE_ATTR_TBL()
	{
		memset( this, 0, sizeof(_HORSE_ATTR_TBL) );
	}

	_HORSE_ATTR* GetAttr(INT PetType)
	{
		if (PetType<=MAX_HORSE_TYPE)
			return	&m_aHorseTbl[PetType];
		return NULL;
	}
	INT	GetTypeCount( ){ return m_iCount ; }
};

extern _HORSE_ATTR_TBL g_HorseAttrTbl;
//���ܱ�������������
const INT MAX_HORSE_SKILL = 256;
struct _HORSE_SKILL_TBL
{
	_HORSE_SKILL m_aHorseSkillTbl[MAX_HORSE_SKILL];
	//ʵ�ʼ��ܸ���
	INT			 m_iCount;
	_HORSE_SKILL_TBL::_HORSE_SKILL_TBL()
	{
		memset( this, 0, sizeof(_HORSE_SKILL_TBL) );
		//ÿ�����ܲ۵��������
		m_aSkillRealRateForSlot[0] = 100;
		m_aSkillRealRateForSlot[1] = 80;
		m_aSkillRealRateForSlot[2] = 60;
		m_aSkillRealRateForSlot[3] = 40;
		m_aSkillRealRateForSlot[4] = 20;
		m_aSkillRealRateForSlot[5] = 5;
	}

	_HORSE_SKILL* GetAttr(INT SkillID )
	{
		if (SkillID<=MAX_HORSE_SKILL)
			return	&m_aHorseSkillTbl[SkillID];
		return NULL;
	}
	INT	GetTypeCount( ){ return m_iCount ; }
	INT m_aSkillRealRateForSlot[MaxSlotNum];//��Ӧÿ�����ܲ�
};
extern _HORSE_SKILL_TBL g_HorseSkillTbl;

//����Ʒ���������
const INT MAX_HORSE_ITEM = 256;
struct _HORSE_ITEM_TBL
{
	//��Ʒ���飬����ÿ����Ʒ������
	_HORSE_ITEM m_aHorseItem[MAX_HORSE_ITEM];
	//��Ʒ��ʵ������
	INT			m_iCount;
	_HORSE_ITEM* GetHorseItemAttr( INT iHorseItemID );
	_HORSE_ITEM_TBL()
	{
		memset( this, 0, sizeof(_HORSE_ITEM_TBL) );
	}
};
extern _HORSE_ITEM_TBL g_HorseItemTbl;

const INT MAX_HORSE_DROP = 256;
struct _HORSE_DROP_TBL
{
	_HORSE_DROP m_aHorse_drop[MAX_HORSE_DROP];
	INT			m_iCount;
	_HORSE_DROP_TBL()
	{
		memset( this, 0, sizeof(_HORSE_DROP_TBL) );
	}
};
extern _HORSE_DROP_TBL g_HorseDrop;

//shoptab�̵��
const INT MAX_SHOP_GROUP = 256;
struct _SHOP_INFO_TBL
{
    _SHOP           m_aShop[MAX_SHOP_GROUP];
    INT			    m_iCount;

    _SHOP_INFO_TBL()
    {
        memset( this, 0, sizeof(_SHOP_INFO_TBL) );
    }
};
extern _SHOP_INFO_TBL g_ShopInfoTbl;

//��̬�̵������Ϣ��
const INT MAX_DYSHOP_GROUP = 256;
struct _DYSHOP_INFO_TBL
{
    DYSHOP_GROUP    m_aGroupInfo[MAX_DYSHOP_GROUP];
    INT			    m_iCount;

    _DYSHOP_INFO_TBL()
    {
        memset( this, 0, sizeof(_DYSHOP_INFO_TBL) );
    }
};
extern _DYSHOP_INFO_TBL g_DyShopInfoTbl;

//////////////////////////////////////////////////////////////////////////
//PK�������
const INT MAX_PK_NUM    =   256;

//PK����
struct _PKPOINT_INFO_TBL
{
    PK_INFO     m_aPKPointInfo[MAX_PK_NUM];
    INT			m_nCount;
};
extern _PKPOINT_INFO_TBL           g_PKPointTbl;

//�ȼ�����
struct _PKLEVEL_INFO_TBL
{
    PK_INFO     m_aPKPointInfo[MAX_PK_NUM];
    INT			m_nCount;
};
extern _PKLEVEL_INFO_TBL           g_PKLevelTbl;

//�ͷ�����
struct _PKPUNISH_INFO_TBL
{
    PK_INFO     m_aPKPointInfo[MAX_PK_NUM];
    INT			m_nCount;
};
extern _PKPUNISH_INFO_TBL           g_PKPunishTbl;


///////////////////////////////////////////////////////////////////////////////////
//����AI����
struct _MONSTER_AI_TBL
{
	INT		m_Table[MAXAI_NUMBER][AIPARAM_NUMBER] ;
	INT		m_Count ;

	_MONSTER_AI_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*MAXAI_NUMBER*AIPARAM_NUMBER ) ;
		m_Count = 0 ;
	};
	INT		Get( uint AIType, uint AIParam )
	{
		Assert( AIType<(uint)m_Count && AIParam<AIPARAM_NUMBER ) ;
		return m_Table[AIType][AIParam] ;
	};
	INT		Count( ){ return m_Count ; } ;
};
extern _MONSTER_AI_TBL g_MonsterAITbl ;

//��������˵���ṹ --�Ѿ�����
struct _MONSTER_SPEAK_TBL
{
	struct MonsterSpeakItem
	{
		INT nSpeakID;
		CHAR szSpeakContent[MAX_MONSTERSPEAK_CONTENT];
	};
	MonsterSpeakItem	m_Table[MAXCHAT_NUMBER] ;	//��¼���е�̸����ǣ���1001��2050������
	INT		m_NumberOfType[MAXCHATTYPE_NUMBER] ;	//ÿһ��ֻ�Ǽ�¼ÿ��̸�����͵ĸ���
    INT 	m_TypeCount ;							//��¼ʵ�ʵ�̸��������
	INT 	m_Count ;								//��¼ʵ�ʵ�̸����

	_MONSTER_SPEAK_TBL( )
	{
		memset( m_NumberOfType, 0, sizeof(INT)*MAXCHATTYPE_NUMBER ) ;
		memset( m_Table, 0, sizeof(MonsterSpeakItem)*MAXCHAT_NUMBER ) ;
		m_TypeCount = 0 ;
		m_Count = 0 ;

	};
	CHAR*	Get( uint ChatType ,INT Index = -1 )
	{//������������ѡȡ��¼��Ҳ����ѡȡָ���ļ�¼
		if( -1 != Index && 0 < Index && m_Count > Index )
			return m_Table[Index].szSpeakContent;

		Assert( ChatType<(uint)m_TypeCount && ChatType>-1) ;
		if ( ChatType<(uint)m_TypeCount && ChatType>-1 )
			return "error";

		INT Offset=0 ;
		for(uint i=0; i<ChatType; i++)
		{
			Offset += m_NumberOfType[i] ;
		}
		INT index = rand() % (m_NumberOfType[ChatType]+1) + Offset ;
		Assert(index<m_Count) ;
		
		return m_Table[index].szSpeakContent ;
	};
	INT		Count( ){ return m_Count ; } ;
	INT		TypeCount( ){ return m_TypeCount ; } ;
};
extern _MONSTER_SPEAK_TBL g_MonsterSpeakTbl ;

//һ������ֵ������
struct _ATTR_LEVELUP_TBL
{
	INT		m_Table[CATTR_LEVEL_NUMBER][MATTRIBUTE_NUMBER][MAX_TABLE_DEFINE_LEVEL+1] ;

	_ATTR_LEVELUP_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*CATTR_LEVEL_NUMBER*MATTRIBUTE_NUMBER*MAX_TABLE_DEFINE_LEVEL ) ;
	}
	INT		Get( uint Attr, uint MenPai ,uint iLevel){
		Assert( Attr<CATTR_LEVEL_NUMBER && MenPai<MATTRIBUTE_NUMBER && iLevel<=MAX_TABLE_DEFINE_LEVEL ) ;
		return m_Table[Attr][MenPai][iLevel] ;
	};
	VOID	Set( uint Attr, uint MenPai ,uint iLevel,INT iValue){
		Assert( Attr<CATTR_LEVEL_NUMBER && MenPai<MATTRIBUTE_NUMBER && iLevel<=MAX_TABLE_DEFINE_LEVEL ) ;
		m_Table[Attr][MenPai][iLevel] = iValue;	
	}
};
extern _ATTR_LEVELUP_TBL g_AttrLevelupTbl ;

//����ֵ��
struct _LEVEL_EXP_TBL {

	UINT		m_Table[MAX_PLAYER_EXP_LEVEL][2];

	_LEVEL_EXP_TBL()
	{
		memset(m_Table,0,sizeof(UINT)*MAX_PLAYER_EXP_LEVEL*2);
	}

	INT Get(uint Level){
		Assert(Level<MAX_PLAYER_EXP_LEVEL);
		Assert(Level>=0);
		return m_Table[Level][1];
	}

};
extern	_LEVEL_EXP_TBL	g_LevelExpTbl;

//�ķ�ѧϰ���ı�
struct _XINFA_UPLEVEL_SPEND_TBL {
	INT		m_Table[MAX_ALL_XINFA_NUM][MAX_XINFA_LEVEL_NUM][3];

	_XINFA_UPLEVEL_SPEND_TBL()
	{
		memset(m_Table,0,sizeof(UINT)*MAX_ALL_XINFA_NUM*MAX_XINFA_LEVEL_NUM*3);
	}
	INT GetMoney(UINT XinfaId, UINT Level)
	{
		Assert(XinfaId<MAX_ALL_XINFA_NUM);
		Assert(XinfaId>=0);
		Assert(Level<MAX_PLAYER_EXP_LEVEL);
		Assert(Level>=0);
		return m_Table[XinfaId][Level-1][1];
	}

	INT GetExp(UINT XinfaId, UINT Level)
	{
		Assert(XinfaId<MAX_ALL_XINFA_NUM);
		Assert(XinfaId>=0);
		Assert(Level<MAX_PLAYER_EXP_LEVEL);
		Assert(Level>=0);
		return m_Table[XinfaId][Level-1][2];
	}
};
extern	_XINFA_UPLEVEL_SPEND_TBL	g_XinfaUpLevelSpendTbl;

struct _EXP_ATT_TBL {

	FLOAT	 m_Table[MAX_100_PLUS_DELTA_LEVEL];

	_EXP_ATT_TBL()
	{
		memset(m_Table,0,sizeof(FLOAT)*MAX_100_PLUS_DELTA_LEVEL);
		
	}

	FLOAT Get(INT DeltaLevel)
	{
		Assert(DeltaLevel<=100);
		Assert(DeltaLevel>=-100);

		return m_Table[DeltaLevel+100];
	}
};

struct _GROW_POINT_INFO
{
	INT				 m_GrowPointIndex;
	INT				 m_OpenTime;
	WORD			 m_OpenAbility;
	ScriptID_t		 m_ScriptID;
	INT				 m_OpenAnim;
	
};


struct _GROW_POINT_INFO_TBL
{

	_GROW_POINT_INFO	*m_Table;

	INT					m_Count;

	_GROW_POINT_INFO_TBL()
	{
		m_Table	=	0;
		m_Count	=	0;
	}

	_GROW_POINT_INFO*	Get(INT GrowType);
	
};

extern	_GROW_POINT_INFO_TBL g_GrowPointInfoTbl;


struct _PLANTS_INFO 
{
	INT			 m_PlantIndex;	
	INT			 m_PlantType;
	INT			 m_Product;

};

struct _PLANTS_INFO_TBL
{

	_PLANTS_INFO*		m_Table;
	INT					m_Count;

	_PLANTS_INFO_TBL()
	{
		m_Table	=	0;
		m_Count	=	0;
	}

};

extern	_PLANTS_INFO_TBL g_PlantInfoTbl;

//���
#define QUES_CON_SIZE 256
#define OPT_CON_SIZE 64
struct _QUESTION_DATA
{
	INT					m_nIndex ;
	CHAR				m_szContex[QUES_CON_SIZE] ;
	CHAR				m_szOption0[OPT_CON_SIZE] ;
	CHAR				m_szOption1[OPT_CON_SIZE] ;
	CHAR				m_szOption2[OPT_CON_SIZE] ;
	CHAR				m_szOption3[OPT_CON_SIZE] ;
	CHAR				m_szOption4[OPT_CON_SIZE] ;
	CHAR				m_szOption5[OPT_CON_SIZE] ;
	BYTE				m_Key0 ;
	BYTE				m_Key1 ;
	BYTE				m_Key2 ;
	BYTE				m_Key3 ;
	BYTE				m_Key4 ;
	BYTE				m_Key5 ;

	_QUESTION_DATA( )
	{
		m_nIndex = INVALID_INDEX ;
		memset( m_szContex, 0, QUES_CON_SIZE ) ;
		memset( m_szOption0, 0, OPT_CON_SIZE ) ;
		memset( m_szOption1, 0, OPT_CON_SIZE ) ;
		memset( m_szOption2, 0, OPT_CON_SIZE ) ;
		memset( m_szOption3, 0, OPT_CON_SIZE ) ;
		memset( m_szOption4, 0, OPT_CON_SIZE ) ;
		memset( m_szOption5, 0, OPT_CON_SIZE ) ;
		m_Key0 = INVALID_INDEX ;
		m_Key1 = INVALID_INDEX ;
		m_Key2 = INVALID_INDEX ;
		m_Key3 = INVALID_INDEX ;
		m_Key4 = INVALID_INDEX ;
		m_Key5 = INVALID_INDEX ;
	};
};

#define MAX_QUESTIONS 4096
struct _QUESTION_LIB
{
	_QUESTION_DATA		m_aQuestions[MAX_QUESTIONS] ;
	INT					m_Count ;

	_QUESTION_LIB( )
	{
		m_Count = 0 ;
	};
};

extern _QUESTION_LIB g_QuestionLib ;

#define MAX_IP_RANGE 256
struct _IPRANGE
{
	INT			m_EnableCount ;
	CHAR		m_aaIPEnable[MAX_IP_RANGE][IP_SIZE] ;
	CHAR		m_aaMaskEnable[MAX_IP_RANGE][IP_SIZE] ;
	INT			m_DisableCount ;
	CHAR		m_aaIPDisable[MAX_IP_RANGE][IP_SIZE] ;
	CHAR		m_aaMaskDisable[MAX_IP_RANGE][IP_SIZE] ;

	_IPRANGE( )
	{
		m_EnableCount = 0 ;
	}
};

extern _IPRANGE g_IPRange ;

struct _LEVEL_MAX_MONEY
{
	UINT	m_Table[MAX_PLAYER_EXP_LEVEL][2];

	_LEVEL_MAX_MONEY()
	{
		memset(m_Table,0,sizeof(UINT)*MAX_PLAYER_EXP_LEVEL*2);
	}

	INT Get(uint Level){
		Assert(Level<=MAX_PLAYER_EXP_LEVEL);
		Assert(Level>=0);
		return m_Table[Level-1][1];
	}
};
extern _LEVEL_MAX_MONEY g_LevelMoneyTbl;

struct 	CITY_SCENE_ENTRY
{
	INT			m_PortID;					//��ڳ���ID
	CHAR		m_szSceneFile[MAX_FILE_PATH];	//scene�ļ���
};

struct _CITY_SCENE
{
	CITY_SCENE_ENTRY	m_Table[MAX_SCENE];

	_CITY_SCENE()
	{
		memset(m_Table,0,sizeof(CITY_SCENE_ENTRY)*MAX_SCENE);
	}

	CHAR* GetszFileNameByPort(INT PortID)
	{
		Assert(PortID<MAX_SCENE);
		Assert(PortID>=0);
		for(INT i =0;i<MAX_SCENE;i++)
		{
			if(m_Table[i].m_PortID == PortID)
			{
				return (CHAR*)m_Table[i].m_szSceneFile;
			}
		}
		return NULL;
	}
};
extern _CITY_SCENE g_CitySceneTbl;

#define MAX_CHAT_CONSUME_TYPE 3
struct _CHAT_CONSUME_TYPE
{
	UINT			m_DelayTime ;
	INT				m_ConsumeCount ;	//������������
	INT				m_ConsumeType[MAX_CHAT_CONSUME_TYPE] ;//��������
	INT				m_ConsumeValue[MAX_CHAT_CONSUME_TYPE] ;//��������
	INT				m_NeedLevel ; //��Ҫ�ȼ�

	_CHAT_CONSUME_TYPE( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_DelayTime = 1 ;
		m_ConsumeCount = 0 ;
		for( INT i=0; i<MAX_CHAT_CONSUME_TYPE; i++ )
		{
			m_ConsumeType[i] = CHAT_NEED_NONE ;
			m_ConsumeValue[i] = 0 ;
		}
		m_NeedLevel = 0 ;
	};
};

#define MAX_CHAT_TYPE_INTABLE 18
struct _CHAT_CONSUME
{
	UINT				m_Count ;
	_CHAT_CONSUME_TYPE	m_ChatComsumeType[MAX_CHAT_TYPE_INTABLE] ;
	
	_CHAT_CONSUME( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		m_Count = 0 ;
	}
};

extern _CHAT_CONSUME g_ChatConsumeTbl ;

struct _HAIR_STYLE_TYPE_
{
	enum
	{
		HAIRSTYLE_NONE				= -1,
		HAIRSTYLE_INDEX,
		HAIRSTYLE_RACEID			= 1,
		HAIRSTYLE_ITEMSERIAL		= HAIRSTYLE_RACEID + 3,
		HAIRSTYLE_ITEMCOSTCOUNT		= HAIRSTYLE_ITEMSERIAL+2,
		HAIRSTYLE_USESCOPE,
	};

	INT					m_nIndex;
	INT					m_nRaceID;
	UINT				m_uItemSerial;
	INT					m_nItemCostCount;
	INT					m_nUseScope;

	_HAIR_STYLE_TYPE_( )
	{
		CleanUp( );
	}

	VOID				CleanUp( )
	{
		m_nIndex		= -1;
		m_nRaceID		= 0;
		m_uItemSerial	= 0;
		m_nItemCostCount= 0;
		m_nUseScope		= 0;	// 0 Ϊ�����ã� 1 ֻ�д�������ʱ��ѡ�ã� 2 ֻ����Ϸ�п���ѡ�ã� 3 ��������ʱ�Լ���Ϸ�ж�����ѡ��
	}
};

//�����ʼ�������ͼ����������Ӧ�ı��
//
//·��ΪServer\Config\mon_wqskl.xls
//
//���������Ķ�Ӧ������ϵΪ��
//			
//Public\Config\mon_attr_ex.tab�е���������ID
//
//�˽ṹ��Ӧ����е�һ�� 
struct MonsterWeaponAndSkill
{
	enum{ MWAS_MAX_LEVEL = 9 };

	//mon_attr_ex.tab�е���������ID��Ӧ��������
	INT m_iIndex;
	//�ֳ�������ID
	INT m_iWeaponID;
	//�Լ��������������飬����ѡ�м����е���һ���ڴ����������
	INT m_aSkillLevelIndex[MWAS_MAX_LEVEL];
	//m_aSkillLevelIndex����Ч���ݵĸ���
	INT m_iSkillLevelIndexCount;
	//����ID���飬��m_aSkillLevelIndex������õ������ݻ���Ϊ�ڴ������е��±�
	INT m_aSkillLevel[MWAS_MAX_LEVEL];
};

struct MonsterWeaponAndSkillTable
{
	enum{ MWAST_MAX_NUM = 500 };
	INT m_Count;
	MonsterWeaponAndSkill m_aWeaponSkill[MWAST_MAX_NUM];
	MonsterWeaponAndSkillTable()
	{
		memset( this, 0, sizeof(MonsterWeaponAndSkillTable) );
	}
};
extern MonsterWeaponAndSkillTable g_MonsterWeaponAndSkill;

#define MAX_HAIR_STYLE_INTABLE 256
struct _HAIR_STYLE
{
	UINT				m_uCount;
	_HAIR_STYLE_TYPE_	m_HairStyle[MAX_HAIR_STYLE_INTABLE];

	_HAIR_STYLE( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		m_uCount = 0 ;
	}
};

extern _HAIR_STYLE g_HairStyleTbl;

#define MAX_GM_SIZE 256
struct _GM_TABLE
{
	INT					m_uCount ;
	GUID_t				m_aGUID[MAX_GM_SIZE] ;

	_GM_TABLE( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_uCount = 0 ;
	}
};

extern _GM_TABLE g_GMTbl;


//�������ɣ�ѧϰ�������������жϱ���Ӧ�����ݽṹ
struct Skill_Study_Condition 
{
	enum
	{
		MAX_SKILL_LEVEL = 9
	};
	INT m_iID;											//����ID
	INT m_iPLayerLevelRequire[MAX_SKILL_LEVEL];			//��������ҵȼ�����
	INT m_iOccupationRequire;							//���������ְҵ����
	INT m_iPlayerPointsRequire[MAX_SKILL_LEVEL];		//��������ϵ���ĵ����ܺ�����
	INT m_iParentSkillID;								//������ID
	INT m_iParentSkillLevelRequire[MAX_SKILL_LEVEL];	//�����ܵȼ�����
	INT m_iSpendPoints[MAX_SKILL_LEVEL];				//�����������Ҫ���ĵĵ���
	INT m_iSpendMoney[MAX_SKILL_LEVEL];					//�����������Ҫ���ĵĽ�Ǯ
	INT m_iSpendExp[MAX_SKILL_LEVEL];					//�����������Ҫ���ĵľ���ֵ
	INT m_iSpendItem1ID[MAX_SKILL_LEVEL];				//������Ҫ���ĵ���ƷID1
	INT m_iSpendItem2ID[MAX_SKILL_LEVEL];				//������Ҫ���ĵ���ƷID2
	INT m_iSpendItem3ID[MAX_SKILL_LEVEL];				//������Ҫ���ĵ���ƷID3
	INT m_iSpendItem1Num[MAX_SKILL_LEVEL];				//������Ҫ���ĵ���Ʒ1����
	INT m_iSpendItem2Num[MAX_SKILL_LEVEL];				//������Ҫ���ĵ���Ʒ2����
	INT m_iSpendItem3Num[MAX_SKILL_LEVEL];				//������Ҫ���ĵ���Ʒ3����

	Skill_Study_Condition()
	{
		memset( this, 0, sizeof(Skill_Study_Condition) );
		m_iID = INVALID_ID;
	}
};

struct Skill_Study_Condition_TABLE
{
	enum
	{
		MAX_Skill_COUNT = SkillTemplateData_T::MAX_ID
	};
	Skill_Study_Condition	m_aSkillStydy_Condition[MAX_Skill_COUNT];
	INT						m_iCount;
	Skill_Study_Condition_TABLE()
	{
		m_iCount = 0;
	}
};

extern Skill_Study_Condition_TABLE g_SkillStudyCondition;

//by zheng
struct OpenBox_ItemInfo_Table
{
	enum
	{
		ITEM_KIND_MAX_NUM = 120,		//������Ʒ������Ʒ�����������ݶ�Ϊ20������
		COLUMN_MAX_NUM = 91,			//������Ʒ�����������

	};

	struct ItemInfoRow
	{
		enum
		{
			ITEM_INFO_MAX_NUM = 30,		//ÿ����������������Ʒ����
		};
		//������Ʒ�����ݽṹ
		struct ItemInfo
		{
			INT		m_ItemID;			//��Ʒ��id
			INT		m_Rate1;			//���ֵĸ���
			INT		m_Rate2;			//��Χ��Ʒ������ĸ���

		public:
			ItemInfo()
				: m_ItemID(0), m_Rate1(0), m_Rate2(0)
			{
			}

			ItemInfo& operator = ( const ItemInfo & oItemInfo )
			{
				m_ItemID = oItemInfo.m_ItemID;
				m_Rate1 = oItemInfo.m_Rate1;
				m_Rate2 = oItemInfo.m_Rate2;

				return *this;
			}

		};

		ItemInfo			    m_ItemInfo[ITEM_INFO_MAX_NUM];
		INT						m_RealNum;							//ItemInfo��ʵ������
		INT						m_Index;
	public:
		ItemInfoRow()
			: m_RealNum(0), m_Index(0)
		{
		}

		ItemInfoRow& operator = ( const ItemInfoRow & oItemInfoRow )
		{
			m_RealNum = oItemInfoRow.m_RealNum;

			if ( m_RealNum>0 && m_RealNum<=ITEM_INFO_MAX_NUM )
			{
				for ( int i=0; i<m_RealNum; ++i )
				{
					m_ItemInfo[i] = oItemInfoRow.m_ItemInfo[i];
				}
			}

			return *this;
		}

	};

	ItemInfoRow m_ItemInfoRow[ITEM_KIND_MAX_NUM];

	INT		m_KindRealNum;										    //������Ʒ������Ʒ����ʵ������

public:
	OpenBox_ItemInfo_Table()
		: m_KindRealNum(0)
	{
	}

};

extern OpenBox_ItemInfo_Table g_OpenBox_ItemInfo_Table;

//װ�����˱�
struct _DURSPOIL_VALUE_TBL
{
	struct SPOIL_VALUE 
	{
		INT      m_nDamageMode;
		UINT	 m_nValue;
	};
	
	SPOIL_VALUE		m_Table[EQUIP_TYPE_NUM];

	_DURSPOIL_VALUE_TBL( )
	{
		for (INT i=0; i<EQUIP_TYPE_NUM; ++i)
		{
			m_Table[i].m_nDamageMode = -1;
			m_Table[i].m_nValue      = -1;
		}
	};
	SPOIL_VALUE&	Get( BYTE equipType )
	{
		Assert(equipType<EQUIP_TYPE_NUM);
		return m_Table[equipType] ;
	};
};
extern _DURSPOIL_VALUE_TBL g_DurSpoilValueTbl ;

///////////////////////////////////////////////////////////////////////////////////
//Bus���ݱ�
#define MAX_BUS_INFO_COUNT		(2046)
struct _BUS_INFO_TBL
{
	INT			m_nCount;
	_BUS_INFO	m_aBusInfo[MAX_BUS_INFO_COUNT];
	BOOL		m_abUsed[MAX_BUS_INFO_COUNT];

	_BUS_INFO_TBL(VOID)
	{
		m_nCount = 0 ;
		memset(m_abUsed, 0, sizeof(m_abUsed));
		INT i;
		for(i = 0; i < MAX_BUS_INFO_COUNT; i++)
		{
			m_aBusInfo[i].CleanUp();
		}
	}

	INT GetCount(VOID)const
	{
		return m_nCount;
	}

	const _BUS_INFO *GetValidItem(INT nDataID)const
	{
		if(nDataID >= 0 && nDataID < MAX_BUS_INFO_COUNT)
		{
			if(m_abUsed[nDataID])
			{
				return &m_aBusInfo[nDataID];
			}
		}
		return NULL;
	}
};
extern _BUS_INFO_TBL g_BusInfoTbl;


//Skill and Combat
using namespace Combat_Module::Skill_Module;
typedef class TemplateManager_T<XinFaData_T> XinFaDataMgr_T;
extern XinFaDataMgr_T g_XinFaDataMgr;
typedef class TemplateManager_T<SkillTemplateData_T> SkillTemplateDataMgr_T;
extern SkillTemplateDataMgr_T g_SkillTemplateDataMgr;
typedef class TemplateManager_T<SkillInstanceData_T> SkillDataMgr_T;
extern SkillDataMgr_T g_SkillDataMgr;
typedef class TemplateManager_T<ImpactSEData_T> ImpactSEDataMgr_T;
extern ImpactSEDataMgr_T g_ImpactSEDataMgr;
//end Skill and combat
//Impact
using namespace Combat_Module::Impact_Module;
typedef class TemplateManager_T<ImpactData_T> ImpactDataMgr_T;
extern ImpactDataMgr_T g_StandardImpactMgr;
//ID collection
typedef class TemplateManager_T<IDCollection_T> IDCollectionMgr_T;
extern IDCollectionMgr_T g_IDCollectionMgr;
//AI 
#define AISCRIPT_NUM (1024)
extern CFileDataMgr g_AIFileDataMgr;
extern AIScript* g_pAIScriptList[AISCRIPT_NUM];
// camp and stand
//using namespace Combat_Module::Camp_Stand;
//typedef class TemplateManager_T<CampStandRecord_T> CampAndStandDataMgr_T;
extern CampAndStandDataMgr_T g_CampAndStandDataMgr;
// Special Obj
using namespace Combat_Module::Special_Obj_Module;
typedef class TemplateManager_T<SpecialObjData_T> SpecialObjDataMgr_T;
extern SpecialObjDataMgr_T g_SpecialObjDataMgr;
// end
class SplitStringLevelOne;
class SplitStringLevelTwo;
class TableInit
{
public :
	TableInit() ;
	~TableInit() ;

	BOOL			Init( ) ;
	BOOL			ReloadCombatTables(VOID);
	VOID			InitGMTable( ) ;

protected :
	VOID			InitBaseValueTable( ) ;
	VOID			InitMonsterAttrExTable( ) ;
	VOID			InitXunHunMissionAttrTable();
	VOID			InitDapTipsAttrTable();
	VOID			Init_MapInfoTable();
	VOID			Init_TimerDoingTable();

    VOID            InitTitleInfoTable();
    VOID            InitTitleCombinationTable();
	//************************************
	// Method:    InitMonsterSkillTable
	// FullName:  TableInit::InitMonsterSkillTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// ��ȡ�����ʼ��ʱ�ļ��������Ϣ
	// ��Ӧserver/config/mon_wqskl.tab
	// FILE_MONSTER_SKILL
	//************************************
	VOID			InitMonsterSkillTable();

	VOID			InitMonsterAITable( ) ;
	VOID			InitNPCPaoPaoIndexTable();
	VOID			InitNPCQuestTable();
	
	// pet
	VOID			InitPetConfigTable( ) ;
	VOID			InitPetAttrTable( );
	VOID			InitPetAttrPointDistribute( );
	VOID			InitPetLevelUpTable( );
	VOID			InitPetSkillDistributeTable( );
	VOID			InitPetSkillIndexTable( );
	VOID			InitPetStudySkillTable( );
	VOID			InitPetApperceiveSkillTable( );
	VOID			InitPetDomesticationMoney( );
	VOID			InitPetTrainCharge( );
	VOID			InitPetAIStrategyTable( );
	//skill
	BOOL 			InitXinFaDataMgr();
	BOOL 			InitSkillTemplateDataMgr();
	BOOL			InitSkillDataMgr();
	BOOL			InitImpactSEDataMgr();
	BOOL			InitStandardImpactMgr();
	//************************************
	// Method:    InitSKillStudyTable
	// FullName:  TableInit::InitSKillStudyTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// ��ȡ�������ɱ�񣬶�Ӧ���Ϊpublic\SkillStudy.tab
	//************************************
	VOID			InitSKillStudyTable();

	//************************************
	// Method:    InitHorseAttrTable
	// FullName:  TableInit::InitHorseAttrTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// ������������Ա�
	//************************************
	VOID			InitHorseAttrTable();
	//************************************
	// Method:    InitHorseRealTable
	// FullName:  TableInit::InitHorseRealTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// �������������
	//************************************
	VOID			InitHorseRealTable();

	//************************************
	// Method:    InitHorseItemTable
	// FullName:  TableInit::InitHorseItemTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// ����Ʒ���ʼ��
	//************************************
	VOID			InitHorseItemTable();

	//************************************
	// Method:    InitHorseDropTable
	// FullName:  TableInit::InitHorseDropTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// �������ʼ����ָ��������������������Ʒ
	//************************************
	VOID			InitHorseDropTable();

	//************************************
	// Method:    ReadHorseSkillSlot
	// FullName:  TableInit::ReadHorseSkillSlot
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne * pSplitL1
	// Parameter: SplitStringLevelTwo * pSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: _HORSE_SKILL_SLOT & oSlot
	// �������������
	//************************************
	VOID ReadHorseSkillSlot
		(	
		SplitStringLevelOne* pSplitL1, 
		SplitStringLevelTwo* pSplitL2, 
		const CHAR* pSplitString, 
		_HORSE_SKILL_SLOT& oSlot
		);

	//************************************
	// Method:    ReadSkillLearn
	// FullName:  TableInit::ReadSkillLearn
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne * pSplitL1
	// Parameter: const CHAR * pSplitString
	// Parameter: INT * pArry
	// ����ѧϰ������
	//************************************
	VOID ReadSkillLearn( SplitStringLevelOne *pSplitL1, const CHAR* pSplitString, INT* pArry );
	//************************************
	// Method:    CommonStringSplit
	// FullName:  TableInit::ReadHorseSkillSlot
	// Access:    protected 
	// Returns:   ʵ�ʽ�����������
	// Qualifier:
	// Parameter: SplitStringLevelOne * pSplitL1 
	// Parameter: SplitStringLevelTwo * pSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT iMaxNum	�������������
	// Parameter: INT * pArry	������������
	// ͨ�ý���
	//************************************
	INT CommonStringSplit
		(	
		SplitStringLevelOne*	pSplitL1, 
		SplitStringLevelTwo*	pSplitL2, 
		const CHAR*				pSplitString, 
		INT						iMaxNum,
		INT*					pArry
		);

	//end skill
	//IdCollectionMgr
	BOOL			InitIdCollectionMgr();
	//camp and stand
	BOOL			InitCampAndStandDataMgr();
	//Special Obj
	BOOL			InitSpecialObjDataMgr();
	//end
	VOID			InitAttrLevelupTable( ) ;
	VOID			InitLevelExpTable();
	VOID			InitXinfaUpLevelSpendTable();
	VOID			InitMonsterSpeakTable();
	VOID			InitGrowPointTable();
	VOID			InitPlantTable();
	uint			GetLevelExp(uint iLevel);

	VOID			InitAIScriptFile();
	VOID			CleanUpAIScrpt();
	VOID			InitQuestionLib() ;
	VOID			InitIPRange( ) ;
	VOID			InitLevelMoneyTable();
	VOID			InitCitySceneTable();
	VOID			InitChatConsume( ) ;
	// data of AI
	VOID			InitAIStateList();
	VOID			CleanUpAIStateList();

	VOID			InitHairStyleTable( );

    // ����ȫ���̵���Ϣ
    VOID			InitShopInfoTable();
    // ���ض�̬�̵���Ϣ
    VOID			InitDyShopInfoTable();
    VOID            InitGuildLevelInfoTable();

    // Load PK�����Ϣ
    VOID			InitPKPointInfoTable();
    VOID			InitPKLevelInfoTable();
    VOID			InitPKPunishInfoTable();

	//���ؿ����������Ϣ
	VOID			InitOpenBoxInfoTable();

	VOID			InitDurSpoilInfoTable();
	VOID			InitBusInfoTable();
//��������
private:
	// Method:    ReadDayTipsBeginAndEndTime
	// FullName:  TableInit::ReadDayTipsBeginAndEndTime
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne & oSplitL1
	// Parameter: SplitStringLevelTwo & oSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: VALID_TIME_SLICE* pVal
	VOID ReadDayTipsBeginAndEndTime(SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
									const CHAR* pSplitString, VALID_TIME_SLICE* pVal,INT MaxCnt);

	// Method:    ReadDayTipsValidWeek
	// FullName:  TableInit::ReadDayTipsValidWeek
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne & oSplitL1
	// Parameter: SplitStringLevelTwo & oSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT* pVal
	//************************************
	VOID ReadDayTipsValidWeek( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, 
								const CHAR* pSplitString, INT pVal[MONTH_WEEK_NUM][WEEK_DAY_NUM] );

	//************************************
	// Method:    ReadDayTipsData
	// FullName:  TableInit::ReadDayTipsData
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne & oSplitL1
	// Parameter: SplitStringLevelTwo & oSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT pVal
	// Parameter: INT MaxCnt:	���ֵ  
	//************************************
	VOID ReadDayTipsData( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, const CHAR* pSplitString, INT* pVal,INT MaxCnt );

	//************************************
	// Method:    ReadTiemrDoingData
	// FullName:  TableInit::ReadTiemrDoingData
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne & oSplitL1
	// Parameter: SplitStringLevelTwo & oSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT pVal
	// Parameter: INT MaxCnt:	���ֵ  
	//************************************
	VOID ReadTiemrDoingData( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, const CHAR* pSplitString, INT* pVal,INT MaxCnt );

	//************************************
	// Method:    ReadTiemrDoingData
	// FullName:  TableInit::ReadTiemrDoingData
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne & oSplitL1
	// Parameter: SplitStringLevelTwo & oSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT pVal
	// Parameter: INT MaxCnt:	���ֵ  
	//************************************
	VOID ReadTiemrDoingMapInfo( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, const CHAR* pSplitString,INT* pVal1,INT* pVal2,INT MaxCnt );

	//************************************
	// Method:    ReadHorseID
	// FullName:  TableInit::ReadHorseID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne & oSplitL1
	// Parameter: SplitStringLevelTwo & oSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT iType
	//************************************
	VOID ReadHorseID( SplitStringLevelOne *pSplitL1, SplitStringLevelTwo *pSplitL2, const CHAR* pSplitString, INT iType );
	//************************************
	// Method:    ReadWeaponID
	// FullName:  TableInit::ReadWeaponID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: SplitStringLevelOne & oSplitL1
	// Parameter: SplitStringLevelTwo & oSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT iType
	//************************************
	VOID ReadWeaponID( SplitStringLevelOne* pSplitL1, SplitStringLevelTwo* pSplitL2, const CHAR* pSplitString, INT iType );

public:
	SPetConfig		m_PetConfig;

};
extern TableInit g_TableInit ;


#endif
