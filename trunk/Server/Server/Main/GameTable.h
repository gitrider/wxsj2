
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

//基本系数表
struct _BASE_VALUE_TBL
{
	//属性影响因素数量*门派数量
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
#define MAX_MIS_TEXT_LEN 24			//任务描述



/*****************************************************************
循环任务相关
*****************************************************************/
//循环任务属性
struct XUNHUAN_MISSION_ATTR
{
	BOOL	m_Used;					//是否已初始化
	INT		m_Group;				//任务组
	INT		m_SubMissionType;		//子任务类型
	INT		m_SubMissionId;			//子任务ID
	INT		m_ObjId;				//要杀的怪物或接收邮件NPC的ID，或物品ID
	CHAR	m_ObjName[NPC_NAME_LEN];//目的怪物/NPC/ITEM名
	INT		m_ObjCount;				//要杀的怪物数量 /寻物的数量 若是送信则为1
	INT		m_ScendId;				//OBJ所在场景ID
	CHAR	m_SceneName[SCENE_NAME_LEN];		
	INT		m_PosX;
	INT		m_PosZ;

	CHAR	m_DialogText[MAX_NPC_DIALOG_LEN];	//NPC对话内容
	
};

#define MAX_XUNHUAN_MISSION_GROUP_NUMBER 10		//循环任务最多有10组，玩家等级有相应的组
#define GROUP_IN_SUB_MISSION_NUMBER 	 50		//每组内最多有50个子任务
#define SUB_MISSION_TYPE_NUMBER			 10		//每组内最多有10种类型的子任务

struct _SUB_MISSION_INFO
{
	INT m_SubMissionCnt;						//组内各种子任务计数
	INT M_SubMissionPos;						//组内各种子任务的位置
};	

//循环任务属性
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
每日提示相关
*****************************************************************/
const INT MAX_LEVEL_NUM = 100;
const INT MAX_DAY_TIPS_NUM	 = 256;
const INT MONTH_WEEK_NUM		 = 7;	//每月最多6周
const INT WEEK_DAY_NUM			 = 7;	//每周最多7天


//任务的有效时间片
struct VALID_TIME_SLICE
{
	tm		m_Begin;		//开始时间
	tm		m_End;			//结束时间
};


//每日提示属性
struct DAY_TIPS_ATTR
{
	BOOL	m_Used;						//是否已初始化
	INT		m_DayNominate;				//日推荐
	INT		m_MonthNominate;			//月推荐
	INT     m_ShowAll;					//是否全部可见	0为否，1为所有人可见

	//可见等级	读表中0表示所有等级段。
	//代码中遇0时，会将数组所有元素置1,代码中就没有0了.
	//此后0表示不可见，1表示可见。
	INT     m_ShowLevel[MAX_LEVEL_NUM];	

	//可见国家,	0表示所有国家。
	//其它同m_ShowLevel
	INT		m_ShowCountry[MAX_COUNTRY_NUM];	

	//可见帮会		0表示所有帮会
	//其它同m_ShowLevel
	INT		m_ShowGuild[GUILD_FAMILY_SIZE];	

	//可见家族		0表示有家族		
	INT		m_ShowFamily[GUILD_FAMILY_SIZE];		

	INT		m_MisType;					//任务类型		0表示循环任务，1表示定时玩法。
	CHAR	m_MisName[MAX_MIS_NAME_LEN];//任务名称

	//任务有效月份		0表示不限月，否则表示可做该任务的月份
	INT		m_MisValidMonth[13];			
	INT		m_WeekOrDay;				//任务以星期或日期来做，0表示星期

	//m_WeekOrDay=1时的任务有效日期,0表示不限时。要注意每个月不是都在31天
	INT    m_MisValidDate[32];

	//任务以周为单位来做
	//第一个字段表示第几周，为0表示不限周
	//第二个字段表示周几，为0表示不限
	INT		m_MisValidWeek[MONTH_WEEK_NUM][WEEK_DAY_NUM];	

	//任务开始到结束的时间,为0表示不限时
	VALID_TIME_SLICE	m_VailidTimeSlice;

	INT		m_MisPos;					//表示读取此任务次数的相关变量及脚本位置,越看越是一头雾水
	INT		m_MisCount;					//每日可做次数，0表示不限次数
    CHAR	m_MisText[MAX_MIS_TEXT_LEN];//任务描述
	CHAR	m_NpcName[NPC_NAME_LEN];	//怪物名
	INT		m_ScendId;					//OBJ所在场景ID
	CHAR	m_SceneName[SCENE_NAME_LEN];		
	INT		m_PosX;
	INT		m_PosZ;

	// 活动参数保存在任务参数数组的序号，对应ScriptGlobal.lua里MissionData中各个数据的定义
	INT		m_DoneCountIndex;	    // 活动完成次数下标 
	INT		m_DoneCircleIndex;  	// 活动完成环数下标
	INT		m_LastDoneTimeIndex;    // 活动最后一次完成时间下标
	INT		m_LastAbandonTimeIndex; // 活动最后一次放弃时间下标

};

//每日提示属性
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
可跳转的地图信息
*****************************************************************/
#define MAX_MAP_INFO_NUM 50

struct _MAP_INFO_ATTR
{
	INT			m_Index;
	SceneID_t	m_SceneId;
	CHAR		m_SceneName[SCENE_NAME_LEN];
	INT			m_DestPosX;			//可到达的目的点
	INT			m_DestPosZ;
	INT			m_SceneType;
	INT			m_MinUserLevel;		//玩家进入场景的最小等级
	INT			m_MinMapLevel;		//地图上的NPC最小等级
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
系统定时活动相关数据结构定义
*****************************************************************/

//系统定时活动名字上限	
#define TIMER_DOING_NAME_LEN 30

//传递的参数个数
#define PARAMETER_NUM 5

//报道内容长度限制
#define REPORT_TEXT_LEN 256

#define TIMER_DOING_MAP_NUM 10

#define MAX_TIMER_DOING_NUM 50

#define MAX_TIEMR_PARAM_NUM 8

//报道的类型
enum REPORT_TYPE
{
	REPORT_TYPE_INVALID = 0,
	REPORT_TYPE_SLOW,		//慢报
	REPORT_TYPE_QUICK,		//快报
	REPORT_TYPE_EARLIER,	//活动刚开始时的报道
	REPORT_TYPE_MID,		//活动进行过程中的报道
	REPORT_TYPE_LAST,		//活动快结束时的报道
	REPORT_TYPE_NUM,

};

struct _TIMER_DOING_ATTR
{
	INT			m_Index;

	//活动的名字
	CHAR		m_szName[TIMER_DOING_NAME_LEN];

	INT			m_ScriptId;			//对应的脚本ID

	//活动执行类型：0表示仅执行一次，
	//1表示在有效时间段内每次服务器启动都会执行一次
	INT			m_OperateType;		

	//时间控制类型：0表示仅执行一次
	//1表示每天执行一次，2表示每周执行一次，3表示每月执行一次
	INT			m_TimeType;	

	//开始&结束时间点:	总共7位数（XXXXXXX）（3221330）
	//时间控制类型＝0时：(前三位表示一年中的天数1～365，后四位表示一天时间[24小时制])
	//时间控制类型＝1时：(前三位无意义，后四位表示一天时间[24小时制])
	//时间控制类型＝2时：(前二位无意义，第三位表示星期数0=星期天;1=星期一，后四位表示一天时间[24小时制])
	//时间控制类型＝3时：(前一位无意义，第二、三位表示本月的天数(01-31)，后四位表示一天时间[24小时制])
    INT			m_StartTime;
	INT			m_EndTime;

	//传递的参数
	INT			m_Parameter[PARAMETER_NUM];

	//广播类型：
	//"0主界面的聊天窗口内
	//1左下角个人的系统提示内
	//2右下角系统的系统提示内
	//3垂直滚动提示内
	//4水平滚动提示内
	//填写格式：
	//3|2|4|1|0，这表示对应的五个地方都会出现后面的广播
	//3|2|4|，这表示对应的三个地方都会出现后面的广播"
	INT			m_BroadcastType[CHAR_SHOW_POS_NUM];

	//慢报间隔、内容、结束时间
	INT			m_SlowReportInterval;
	CHAR		m_SlowReportText[REPORT_TEXT_LEN];
	INT			m_SlowReportEndTime;

	//快报间隔、内容、结束时间
	INT			m_QuickReportInterval;
	CHAR		m_QuickReportText[REPORT_TEXT_LEN];
	INT			m_QuickReportEndTime;

	//活动刚开始时的报道间隔、内容、结束时间
    INT			m_EarlierReportInterval;
	CHAR		m_EarlierReportText[REPORT_TEXT_LEN];
	INT			m_EarlierReportEndTime;

	//活动进行过程中的报道间隔、内容、结束时间
    INT			m_MidReportInterval;
	CHAR		m_MidReportText[REPORT_TEXT_LEN];
	INT			m_MidReportEndTime;

	//活动快结束时的报道间隔、内容、结束时间
    INT			m_LastReportInterval;
	CHAR		m_LastReportText[REPORT_TEXT_LEN];
	INT			m_LastReportEndTime;

	//m_Ruler＝-1，使用m_TempMapId；m_Ruler＝0，激活所有Map项；
	//m_Ruler≥1，激活m_Ruler个Map项
	INT			m_Ruler;

	//只有当m_Ruler＝-1时，才使用
	INT			m_TempMapId;

	//格式为：MapId|Num，比如  0|20，MapId代表地图ID，Num代表该地图的权重。
	//不使用的Map_n项，填写-1|-1
	//Num即m_MapRatio
	INT			m_MapId[TIMER_DOING_MAP_NUM];
	INT			m_MapRatio[TIMER_DOING_MAP_NUM];

	//等于m_MapRatio数组之合
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
//系统定时活动相关数据结构定义结束

const INT MONSTER_EXT_ATTR_ENUM_SIZE = 100;
struct MONSTER_EXT_ATTR
{
	BOOL	m_Used;					//是否已初始化
	INT		m_Type;					//怪物类型
	CHAR	m_Name[NPC_NAME_LEN];	//怪物类型名
	INT		m_CannotFight;			//不可被攻击
	INT		m_Level;				//等级
	INT		m_RespawnTime;			//重生时间
	INT		m_BaseExp;				//基础经验获得
	INT		m_OutLook;				//休闲间隔时间
	INT		m_BaseAI;				//基础AI ID
	INT		m_ExtAI;				//扩展AI ID
	INT		m_Camp;					//阵营
	FLOAT	m_DropSearchRange;		//掉落分配半径（米）
	INT		m_SearchTeamCount;		//掉落最大有效组
	INT		m_MonsterDropItemSetID; //掉落物品组
	INT		m_nMonsterDropRate;		//掉落概率
	INT		m_MinDamagePercent;		//最小伤血百分比
	INT		m_CanHandle;			//是否可以交互
	INT		m_FriendValue;			//友好值
	INT		m_WalkSpeed;			//步行速度
	INT		m_BossFlag;				//BOSS标记
	INT		m_SnapSkillImmID;		//瞬发技能免疫ID
	INT		m_DurationSkillImmID;	//持续技能免疫ID
	INT		m_AttackAnimTime;		//攻击动作时间
	INT		m_AttackCooldownTime;	//攻击冷却时间
	INT		m_ScriptID;				//脚本调用ID
	INT		m_WeaponID[MONSTER_EXT_ATTR_ENUM_SIZE];		//武器调用ID
	INT		m_HorseID[MONSTER_EXT_ATTR_ENUM_SIZE];		//马调用ID
	INT		m_iWeaponCount;			//可供随机选择的武器种类数量
	INT		m_iHorseCount;			//可供随机选择的马种类数量

	INT		m_aHorseDropID[MaxHorseDropNum];
	INT		m_iHorseDropCount;
	INT		m_Table[CATTR_LEVEL2_NUMBER];
};

//怪物二级属性表
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


//称号信息表
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

//帮会升级信息表
struct _GUILD_LEVEL_INFO_TBL
{
    _GUILD_LEVEL_INFO       m_TableInfo[MAX_GUILD_LEVEL];

    _GUILD_LEVEL_INFO_TBL()
    {
    };

};
extern _GUILD_LEVEL_INFO_TBL g_GuildLevelInfoTbl ;


//组合称号配方表
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

// NPC泡泡说话索引表
struct _NPC_PAOPAO_INDEX
{
	INT m_iDataIndex; // 怪物ID
	INT m_nBaseIndex; // NPCPaoPao起始索引ID
	INT m_nIndexStep; // NPCPaoPao索引ID对应的有效范围
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


//宠物属性表
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

//宠物属性点分配方案表
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
    INT					m_Count;				//方案个数

	_PET_ATTRPOINT_DIS_TBL()
	{
		m_Count = 0;
	}

	// nType：力量，资质属性等。。。 nID：方案号
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

// 宠物升级表
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

// 宠物升级表
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

// 宠物技能生成表
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

// 宠物技能索引表
#define PET_SKILL_NUM (512)
struct _PET_SKILLINDEX_TBL
{
	INT		m_nIndex;
	INT		m_nSkillID;
};
extern _PET_SKILLINDEX_TBL g_PetSkillIndexTbl[PET_SKILL_NUM];

// 宠物学习技能几率表
struct _PET_STUDYSKILL
{
	INT		m_nID;				// ID
	INT		m_nMaxSpaceCount;	// 总空格数量
	INT		m_nSpaceCount;		// 当前空格数量
	INT		m_nNewPercent;		// 打在新格上的百分比
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

// 宠物领悟技能几率表
#define PET_APPERCEIVESKILLRATE_NUM (24)
struct _PET_APPERCEIVESKILL_TBL
{
	INT		m_nSkillCount;					//宠物技能总数
	INT		m_nLearnedVoluntarySkillCount;	//已有主动技能数
	INT		m_nLearnedPassiveSkillCount;	//已有被动技能数
	INT		m_nApperceiveVoluntarySkillRate;//领悟主动技能概率（单位1/10w）
	INT		m_nApperceivePassiveSkillRate;	//领悟被动技能概率（单位1/10w）

};
extern _PET_APPERCEIVESKILL_TBL g_PetApperceiveSkillTbl[PET_APPERCEIVESKILLRATE_NUM];

#define PET_AITYPE (5)
#define PET_SKILL_TYPE (5)
extern FLOAT g_PetAIStrategyTble[PET_SKILL_TYPE][PET_AITYPE];

const INT MAX_HORSE_TYPE = 256;
//马属性表
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
//马技能表的最大行数限制
const INT MAX_HORSE_SKILL = 256;
struct _HORSE_SKILL_TBL
{
	_HORSE_SKILL m_aHorseSkillTbl[MAX_HORSE_SKILL];
	//实际技能个数
	INT			 m_iCount;
	_HORSE_SKILL_TBL::_HORSE_SKILL_TBL()
	{
		memset( this, 0, sizeof(_HORSE_SKILL_TBL) );
		//每个技能槽的领悟概率
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
	INT m_aSkillRealRateForSlot[MaxSlotNum];//对应每个技能槽
};
extern _HORSE_SKILL_TBL g_HorseSkillTbl;

//马物品的最大数量
const INT MAX_HORSE_ITEM = 256;
struct _HORSE_ITEM_TBL
{
	//物品数组，描述每个物品的属性
	_HORSE_ITEM m_aHorseItem[MAX_HORSE_ITEM];
	//物品的实际数量
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

//shoptab商店表
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

//动态商店分组信息表
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
//PK相关配置
const INT MAX_PK_NUM    =   256;

//PK基数
struct _PKPOINT_INFO_TBL
{
    PK_INFO     m_aPKPointInfo[MAX_PK_NUM];
    INT			m_nCount;
};
extern _PKPOINT_INFO_TBL           g_PKPointTbl;

//等级基数
struct _PKLEVEL_INFO_TBL
{
    PK_INFO     m_aPKPointInfo[MAX_PK_NUM];
    INT			m_nCount;
};
extern _PKLEVEL_INFO_TBL           g_PKLevelTbl;

//惩罚基数
struct _PKPUNISH_INFO_TBL
{
    PK_INFO     m_aPKPointInfo[MAX_PK_NUM];
    INT			m_nCount;
};
extern _PKPUNISH_INFO_TBL           g_PKPunishTbl;


///////////////////////////////////////////////////////////////////////////////////
//怪物AI参数
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

//怪物泡泡说话结构 --已经废弃
struct _MONSTER_SPEAK_TBL
{
	struct MonsterSpeakItem
	{
		INT nSpeakID;
		CHAR szSpeakContent[MAX_MONSTERSPEAK_CONTENT];
	};
	MonsterSpeakItem	m_Table[MAXCHAT_NUMBER] ;	//记录所有的谈话标记，如1001，2050。。。
	INT		m_NumberOfType[MAXCHATTYPE_NUMBER] ;	//每一项只是记录每种谈话类型的个数
    INT 	m_TypeCount ;							//记录实际的谈话类型数
	INT 	m_Count ;								//记录实际的谈话数

	_MONSTER_SPEAK_TBL( )
	{
		memset( m_NumberOfType, 0, sizeof(INT)*MAXCHATTYPE_NUMBER ) ;
		memset( m_Table, 0, sizeof(MonsterSpeakItem)*MAXCHAT_NUMBER ) ;
		m_TypeCount = 0 ;
		m_Count = 0 ;

	};
	CHAR*	Get( uint ChatType ,INT Index = -1 )
	{//即可以以类型选取记录，也可以选取指定的记录
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

//一级属性值升级表
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

//经验值表
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

//心法学习消耗表
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

//题库
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
	INT			m_PortID;					//入口场景ID
	CHAR		m_szSceneFile[MAX_FILE_PATH];	//scene文件名
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
	INT				m_ConsumeCount ;	//消耗类型数量
	INT				m_ConsumeType[MAX_CHAT_CONSUME_TYPE] ;//消耗种类
	INT				m_ConsumeValue[MAX_CHAT_CONSUME_TYPE] ;//消耗数量
	INT				m_NeedLevel ; //需要等级

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
		m_nUseScope		= 0;	// 0 为不可用， 1 只有创建人物时可选用， 2 只有游戏中可以选用， 3 创建人物时以及游戏中都可以选用
	}
};

//怪物初始化武器和技能随机所对应的表格
//
//路径为Server\Config\mon_wqskl.xls
//
//与其他表格的对应索引关系为：
//			
//Public\Config\mon_attr_ex.tab中的武器调用ID
//
//此结构对应表格中的一行 
struct MonsterWeaponAndSkill
{
	enum{ MWAS_MAX_LEVEL = 9 };

	//mon_attr_ex.tab中的武器调用ID对应的索引号
	INT m_iIndex;
	//手持主武器ID
	INT m_iWeaponID;
	//对技能列索引的数组，最终选中技能列的哪一列在此数组中随机
	INT m_aSkillLevelIndex[MWAS_MAX_LEVEL];
	//m_aSkillLevelIndex中有效数据的个数
	INT m_iSkillLevelIndexCount;
	//技能ID数组，在m_aSkillLevelIndex中随机得到的数据会作为在此数组中的下标
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


//技能养成（学习和升级）条件判断表格对应的数据结构
struct Skill_Study_Condition 
{
	enum
	{
		MAX_SKILL_LEVEL = 9
	};
	INT m_iID;											//技能ID
	INT m_iPLayerLevelRequire[MAX_SKILL_LEVEL];			//升级的玩家等级需求
	INT m_iOccupationRequire;							//升级的玩家职业需求
	INT m_iPlayerPointsRequire[MAX_SKILL_LEVEL];		//技能所属系消耗点数总合需求
	INT m_iParentSkillID;								//父技能ID
	INT m_iParentSkillLevelRequire[MAX_SKILL_LEVEL];	//父技能等级需求
	INT m_iSpendPoints[MAX_SKILL_LEVEL];				//升级的玩家需要消耗的点数
	INT m_iSpendMoney[MAX_SKILL_LEVEL];					//升级的玩家需要消耗的金钱
	INT m_iSpendExp[MAX_SKILL_LEVEL];					//升级的玩家需要消耗的经验值
	INT m_iSpendItem1ID[MAX_SKILL_LEVEL];				//升级需要消耗的物品ID1
	INT m_iSpendItem2ID[MAX_SKILL_LEVEL];				//升级需要消耗的物品ID2
	INT m_iSpendItem3ID[MAX_SKILL_LEVEL];				//升级需要消耗的物品ID3
	INT m_iSpendItem1Num[MAX_SKILL_LEVEL];				//升级需要消耗的物品1数量
	INT m_iSpendItem2Num[MAX_SKILL_LEVEL];				//升级需要消耗的物品2数量
	INT m_iSpendItem3Num[MAX_SKILL_LEVEL];				//升级需要消耗的物品3数量

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
		ITEM_KIND_MAX_NUM = 120,		//宝箱物品表中物品类型数量，暂定为20种箱子
		COLUMN_MAX_NUM = 91,			//宝箱物品表中最大列数

	};

	struct ItemInfoRow
	{
		enum
		{
			ITEM_INFO_MAX_NUM = 30,		//每行所包含的最大的物品数量
		};
		//宝箱物品表数据结构
		struct ItemInfo
		{
			INT		m_ItemID;			//物品的id
			INT		m_Rate1;			//出现的概率
			INT		m_Rate2;			//周围物品随机出的概率

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
		INT						m_RealNum;							//ItemInfo的实际数量
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

	INT		m_KindRealNum;										    //宝箱物品表中物品类型实际数量

public:
	OpenBox_ItemInfo_Table()
		: m_KindRealNum(0)
	{
	}

};

extern OpenBox_ItemInfo_Table g_OpenBox_ItemInfo_Table;

//装备损伤表
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
//Bus数据表
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
	// 读取怪物初始化时的技能相关信息
	// 对应server/config/mon_wqskl.tab
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
	// 读取技能养成表格，对应表格为public\SkillStudy.tab
	//************************************
	VOID			InitSKillStudyTable();

	//************************************
	// Method:    InitHorseAttrTable
	// FullName:  TableInit::InitHorseAttrTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// 加载马基础属性表
	//************************************
	VOID			InitHorseAttrTable();
	//************************************
	// Method:    InitHorseRealTable
	// FullName:  TableInit::InitHorseRealTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// 加载马技能领悟表
	//************************************
	VOID			InitHorseRealTable();

	//************************************
	// Method:    InitHorseItemTable
	// FullName:  TableInit::InitHorseItemTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// 马物品表初始化
	//************************************
	VOID			InitHorseItemTable();

	//************************************
	// Method:    InitHorseDropTable
	// FullName:  TableInit::InitHorseDropTable
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	// 马掉落表初始化，指怪物掉落马，不是马掉落物品
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
	// 马技能领悟表格解析
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
	// 马技能学习表格解析
	//************************************
	VOID ReadSkillLearn( SplitStringLevelOne *pSplitL1, const CHAR* pSplitString, INT* pArry );
	//************************************
	// Method:    CommonStringSplit
	// FullName:  TableInit::ReadHorseSkillSlot
	// Access:    protected 
	// Returns:   实际解析出的数量
	// Qualifier:
	// Parameter: SplitStringLevelOne * pSplitL1 
	// Parameter: SplitStringLevelTwo * pSplitL2
	// Parameter: const CHAR * pSplitString
	// Parameter: INT iMaxNum	最大允许拆分数量
	// Parameter: INT * pArry	保存结果的数组
	// 通用解析
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

    // 加载全部商店信息
    VOID			InitShopInfoTable();
    // 加载动态商店信息
    VOID			InitDyShopInfoTable();
    VOID            InitGuildLevelInfoTable();

    // Load PK相关信息
    VOID			InitPKPointInfoTable();
    VOID			InitPKLevelInfoTable();
    VOID			InitPKPunishInfoTable();

	//加载开宝箱相关信息
	VOID			InitOpenBoxInfoTable();

	VOID			InitDurSpoilInfoTable();
	VOID			InitBusInfoTable();
//辅助方法
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
	// Parameter: INT MaxCnt:	最大值  
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
	// Parameter: INT MaxCnt:	最大值  
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
	// Parameter: INT MaxCnt:	最大值  
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
