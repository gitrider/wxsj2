// GameStruct_Skill.h

#ifndef __GAMESTRUCT_SKILL_H__
#define __GAMESTRUCT_SKILL_H__

#include "Type.h"
#include "GameDefine2.h"
#include "GameStruct_Impact.h"
/////////////////////////////////////////////////////////////////////////////////
// 客户端与服务器共用的DBC结构

// 技能伤害结构
struct _DBC_DIRECT_IMPACT
{
	UINT				m_uID;					// ID
	CHAR				*m_pszEffect;			// 特效ID
	CHAR				*m_pszSound;			// 音效ID
	CHAR				*m_pszEffectLocator;	// 特效绑定点
	CHAR				*m_pszReserved1;		// 预留1
	CHAR				*m_pszReserved2;		// 预留2
	CHAR				*m_pszInfo;				// 效果描述
};
typedef _DBC_DIRECT_IMPACT	SDirectImpact;
typedef _DBC_DIRECT_IMPACT	_DIRECT_IMPACT;


// 技能BUFF结构
struct _DBC_BUFF_IMPACT
{
	UINT				m_uID;						// ID
	UINT				m_uMutexID;					// 互斥标记
	INT					m_nPri;						// 优先级参数
	UINT				m_uMaxLayerNum;				// 最大累积层数
	LPCSTR				m_szIconName;				// 图标的文件名
	LPCSTR				m_lpszEffect_Active;		// 激活特效ID
	LPCSTR				m_lpszSound_Active;			// 激活特效的音效ID
	LPCSTR				m_lpszBind_Active;			// 激活特效的绑定点
	LPCSTR				m_lpszEffect_Continuous;	// 持续特效ID
	LPCSTR				m_lpszSound_Continuous;		// 持续特效的音效ID
	LPCSTR				m_lpszBind_Continuous;		// 持续特效的绑定点
	BOOL				m_bStillOnWhenOwnerDead;	// 主人死后是否保留
	BOOL				m_bCanBeDispeled;			// 是否可以被驱散
	BOOL				m_bHostileFlag;				// 是否是负面效果
	BOOL				m_bCanBeManualCancel;		// 是否可以被手动取消
	BOOL				m_bRotateEffect;			// 是否让特效做旋转处理
	BOOL				m_bLineEffect;				// 是否为线性特效
	CHAR				*m_pszCreatorLocator;		// 线性特效的目标绑定点
	CHAR				*m_pszInfo;					// 效果描述
};
typedef _DBC_BUFF_IMPACT SBuffImpact;
typedef _DBC_BUFF_IMPACT _BUFF_IMPACT;

// 子弹轨迹类型
enum ENUM_BULLET_CONTRAIL_TYPE
{
	BULLET_CONTRAIL_TYPE_INVALID = -1,
	BULLET_CONTRAIL_TYPE_BEELINE,			// 直线
	BULLET_CONTRAIL_TYPE_PARABOLA,			// 抛物线
	BULLET_CONTRAIL_TYPE_NONE,				// 无轨迹，直接爆炸
	BULLET_CONTRAIL_TYPE_ROTATE,			// 旋转
	BULLET_CONTRAIL_TYPE_EXP,

	BULLET_CONTRAIL_TYPE_NUMBERS
};

// 子弹
struct _DBC_BULLET_DATA
{
	INT				m_nID;					// ID
	CHAR const* 	m_szDesc;				// 说明和描述
	CHAR const* 	szKeyword;				// 关键字
	CHAR const* 	m_szBulletSystemName;	// 子弹模版名
	CHAR const* 	m_szReserved1;			// 预留1
	CHAR const* 	m_szReserved2;			// 预留2

	INT				m_bIsLogicBullet;		// 是否使用客户端逻辑子弹
	INT				m_nContrailType;		// 轨迹类型 ENUM_BULLET_CONTRAIL_TYPE
	INT				m_nContrailParam;		// 轨迹参数(对抛物线为上升的最大高度)
	CHAR const*		m_szFlyEffect;			// 飞行特效
	CHAR const*		m_szFlySound;			// 飞行音效
	FLOAT			m_fSpeed;				// 速度(m/s)
	FLOAT			m_fAcceleration;		// 加速度
	CHAR const*		m_szHitEffect;			// 击中特效
	CHAR const* 	m_szHitSound;			// 击中音效
	CHAR const*		m_szHitEffectLocator;	// 击中特效的绑定点	
	
	
};
typedef _DBC_BULLET_DATA SSkillObjData;
typedef _DBC_BULLET_DATA _SKILLOBJ_DATA;

enum ENUM_SKILL_TYPE
{
	SKILL_TYPE_INVALID	= -1,
	SKILL_TYPE_GATHER,
	SKILL_TYPE_LEAD,
	SKILL_TYPE_LAUNCH,
	SKILL_TYPE_PASSIVE,

	SKILL_TYPE_NUMBERS
};

enum ENUM_TRANSFER_SKILL
{
	TRANSFER_INVALID	= -1,
	TRANSFER_TWINKLING,			// 瞬移
	TRANSFER_TANTIVY,			// 冲锋
};

enum ENUM_SELECT_TYPE
{
	SELECT_TYPE_INVALID	= -1,
	SELECT_TYPE_NONE,			// 无需选择
	SELECT_TYPE_CHARACTER,		// 角色
	SELECT_TYPE_POS,			// 位置
	SELECT_TYPE_DIR,			// 方向
	SELECT_TYPE_SELF,			// 对自己进行操作
	SELECT_TYPE_HUMAN_GUID,		// 玩家
	SELECT_TYPE_NUMBERS
};
enum ENUM_TARGET_LOGIC
{
	TARGET_LOGIC_INVALID	= -1,
	TARGET_SELF,					// 只对自己有效
	TARGET_MY_PET,					// 只对自己的宠物有效
	TARGET_MY_SHADOW_GUARD,			// 只对自己的分身有效
	TARGET_MY_MASTER,				// 只对自己的主人有效，宠物专用
	TARGET_AE_AROUND_SELF,			// 以自己为中心，范围有效
	TARGET_SPECIFIC_UNIT,			// 瞄准的对象有效
	TARGET_AE_AROUND_UNIT,			// 以瞄准的对象为中心，范围有效
	TARGET_AE_AROUND_POSITION,		// 以瞄准的位置点为中心，范围有效
	TARGET_POS,						// 以瞄准位置有效

	TARGET_LOGIC_NUMBERS			// 逻辑总数
};

enum ENUM_BEHAVIOR_TYPE
{
	BEHAVIOR_TYPE_HOSTILITY = -1, //敌对行为
	BEHAVIOR_TYPE_NEUTRALITY = 0, //中立行为
	BEHAVIOR_TYPE_AMITY = 1, //友好行为
};
// 招式类型
enum ENUM_SKILL_ACTION_TYPE
{
	SKILL_ACTION_TYPE_INVALID	= -1,
	SKILL_ACTION_TYPE_NONE,					// 普通招式(无序随机)
	SKILL_ACTION_TYPE_CONCATENATION_EX,		// 连招(1,2,2,2,2,3)
	SKILL_ACTION_TYPE_CONCATENATION,		// 连招(1,2,3,1,2,3)

	SKILL_ACTION_TYPE_NUMBERS
};

#define MAX_SKILL_FRIEND_TMPACT		(2)
#define MAX_SKILL_ENEMY_TMPACT		(2)

// 技能的学习条件 -- 对应表
struct _DBC_SKILL_STUDY_DATA
{
	INT				m_nSkillID;					// 技能ID
	const CHAR*		m_szLevelQua;				// 级别 需求
	INT				m_nVocationQua;				// 职业 需求
	const CHAR*		m_szSeriesSkillPointQua;	// 该系别技能的总共花费 需求
	INT				m_nFatherSkillIDQua;		// 父技能 需求
	const CHAR*		m_szFatherSkillLevelQua;	// 父技能的级别 需求
	const CHAR*		m_szSkillPointQua;			// 拥有的技能点数 需求
	const CHAR*		m_szMoneyQua;				// 金钱 需求
	const CHAR*		m_szExpQua;					// 经验 需求
	const CHAR*		m_szItem1Qua;				// 物品1
	const CHAR*		m_szItem1NumQua;			// 物品1 数量
	const CHAR*		m_szItem2Qua;				// 物品2
	const CHAR*		m_szItem2NumQua;			// 物品2 数量
	const CHAR*		m_szItem3Qua;				// 物品3 
	const CHAR*		m_szItem3NumQua;			// 物品3 数量
};


// 技能 只是供客户端使用的技能模板结构	skl_template.tab
struct _DBC_SKILL_DATA
{
	INT				m_nID;								// ID
	INT				m_nIDForManagement;					// 策划内部使用的管理性ID
	INT				m_nMenPai;							// 门派ID
	const CHAR	*	m_lpszName;							// 名称
	const CHAR	*	m_lpszSkillExplain;					// 技能描述
	const CHAR	*	m_lpszSkillExplainForTip;			// 技能描述
	const CHAR	*	m_lpszSkillExplainForInterface;		// 技能描述
	const CHAR	*	m_lpszIconName;						// Icon名称

	INT				m_nSkillActionType;					// 招式类型ENUM_SKILL_ACTION_TYPE
	INT				m_nSkillLogicID;					// 技能逻辑ID
	INT				m_nSkillSeries;						// 技能系别
	INT				m_nMaxLevel;						// 等级上限
	const CHAR	*	m_lpsznCoolDownTime;					// 冷却时间
	const CHAR	*	m_lpszJiQiTime;						// 聚气时间
	const CHAR	*	m_lpszDirectTime;						// 引导时间
	const CHAR	*	m_lpszConditionExplain1;			// 条件或消耗参数说明1
	INT				m_ConditionLogic1;					// 条件和消耗逻辑1
	INT				m_nCondition11;						// 条件或消耗参数值11
	INT				m_nCondition12;						// 条件或消耗参数值12
	const CHAR	*	m_lpszConditionExplain2;			// 条件或消耗参数说明2
	INT				m_ConditionLogic2;					// 条件和消耗逻辑2
	INT				m_nCondition21;						// 条件或消耗参数值21
	INT				m_nCondition22;						// 条件或消耗参数值22
	const CHAR	*	m_lpszConditionExplain3;			// 条件或消耗参数说明3
	INT				m_ConditionLogic3;					// 条件和消耗逻辑3
	INT				m_nCondition31;						// 条件或消耗参数值31
	INT				m_nCondition32;						// 条件或消耗参数值32
	INT				m_nTargetLevel;						// 目标级别

	INT			m_bMustUseWeapon; 					// 是否是必须使用武器的技能
	INT				m_nDisableByFlag1; 					// 受限于限制标记1, 用于昏迷,魅惑等
	INT				m_nDisableByFlag2; 					// 受限于限制标记2, 用于沉默类似
	INT				m_nDisableByFlag3; 					// 受限于限制标记3, 用于变身骑乘
	INT				m_nRangeSkillFlag;					// 是否是远程技能
	INT			m_bBreakPreSkill;					// 是否中断自己当前使用的技能
	INT				m_nType;							// 技能类型 ENUM_SKILL_TYPE
	INT				m_nCooldownID;						// 冷却组ID
	const CHAR	*	m_lpszGatherLeadActionSetID;		// 引导/聚气动作组ID 
	const CHAR	*	m_lpszSendActionSetID;				// 发招招式ID
	INT				m_nEnvironmentSpecialEffect;		// 环境特效ID
	INT				m_nTargetMustInSpecialState;		// 目标必须是， 0:活的；1:死的; -1:没有要求
	INT				m_nClassByUser;						// 按使用者类型分类，0:玩家, 1:怪物, 2:宠物, 3:物品,
	INT				m_nPassiveFlag;						// 主动还是被动技能，0:主动技能,1:被动技能;
	INT				m_nSelectType;						// 点选类型 ENUM_SELECT_TYPE
	const CHAR	*	m_lpszAttachProjectTextureEffect;				// 选中环特效名称
	INT				m_nOperateModeForPetSkill;			// 宠物技能专用，操作模式: PET_SKILL_OPERATEMODE
	INT				m_nPetRateOfSkill; 					// 技能发动几率,只对宠物技能有效
	INT				m_nTypeOfPetSkill; 					// 宠物技能类型,0:物功,1:法功,2:护主,3:防御,4:复仇;
	INT			m_nImpactIDOfSkill;					// 宠物技能产生的效果ID
	INT				m_nReserved1;						// 预留数据域
	INT				m_nReserved2;						// 预留数据域
	INT				m_nReserved3;						// 预留数据域
	INT				m_nReserved4;						// 预留数据域

	INT				m_nBulletID;						// 子弹ID
	const CHAR	*	m_pszBulletSendLocator;				// 子弹发出的绑定点
	const CHAR	*	m_pszBulletNum;						// 子弹数量		多个子弹都是用同一ID
	INT				m_nAngle;							// 子弹夹角

	INT				m_nTargetingLogic;					// 目标选择逻辑	ENUM_TARGET_LOGIC
	INT				m_nSendTime;						// 发招时间(ms)
	FLOAT			m_fMinAttackRange;					// 最小攻击距离(m)
	FLOAT			m_fMaxAttackRange;					// 最大攻击距离(m)
	INT				m_nFriendness;						// 技能友好度，=0为中性技能，>0为正面技能，<0为 负面技能
	INT				m_nRelationshipLogic;				// 目标和使用者关系的合法性判断ID，参考相关子表格。
	INT				m_nTargetCheckByObjType;			// 目标必须是某一指定的obj_type的角色
	INT				m_nIsPartyOnly;						// 是否仅对队友有效。注：队友宠物算作队友。1为只对队友有效，0为无此限制。
	INT				m_nHitsOrINTerval;					// 连击的攻击次数或引导的时间间隔
	INT				m_bAutoRedo;						// 连续技能
	INT				m_nHitRate;							// 命中率
	INT				m_nCriticalRate; 					// 会心率
	INT			m_bUseNormalAttackRate;				// 冷却时间是否受攻击速度影响
	INT				m_nActiveTime;						// 激活时间
	const CHAR*			m_szDamageRange;						// 杀伤范围(m)
	FLOAT			m_nDamageAngle;						// 杀伤角度(0~360)
	INT				m_nTargetNum;						// 影响目标的最大数
	INT				m_nReserved5;						// 预留数据域
	INT				m_nCanInterruptAutoShot; 			// 是否能打断自动技能的连续释放
	INT			m_nDelayTime; 						// 延迟时间
	INT				m_nDamageDelay;						// 伤害计算的通用延迟时间（ms）
	INT				m_nImmediately;						// 是否瞬发技能
	INT				m_nBeatBackDist;					// 击退距离
	INT				m_nBeatBackRate;					// 击退概率
	INT				m_nBeatFlyDist;						// 击飞距离
	INT				m_nBeatFlyRate;						// 击飞概率
	INT			m_bBallSkill;						// 是否是带有球特效的技能
	INT				m_nReserved6;						// 预留数据域
	INT				m_nReserved7;						// 预留数据域

	const CHAR	*	m_lpszImpcatExplain_1;				// 效果说明_1
	const CHAR  *	m_lpszImpactParameter_1;			// 效果参数集合_1
	const CHAR	*	m_lpszImpcatExplain_2;				// 效果说明_2
	const CHAR  *	m_lpszImpactParameter_2;			// 效果参数集合_2
	const CHAR	*	m_lpszImpcatExplain_3;				// 效果说明_3
	const CHAR  *	m_lpszImpactParameter_3;			// 效果参数集合_3
	const CHAR	*	m_lpszImpcatExplain_4;				// 效果说明_4
	const CHAR  *	m_lpszImpactParameter_4;			// 效果参数集合_4
	const CHAR	*	m_lpszImpcatExplain_5;				// 效果说明_5
	const CHAR  *	m_lpszImpactParameter_5;			// 效果参数集合_5
	const CHAR	*	m_lpszImpcatExplain_6;				// 效果说明_6
	const CHAR  *	m_lpszImpactParameter_6;			// 效果参数集合_6
	const CHAR	*	m_lpszImpcatExplain_7;				// 效果说明_7
	const CHAR  *	m_lpszImpactParameter_7;			// 效果参数集合_7
	const CHAR	*	m_lpszImpcatExplain_8;				// 效果说明_8
	const CHAR  *	m_lpszImpactParameter_8;			// 效果参数集合_8
	const CHAR	*	m_lpszImpcatExplain_9;				// 效果说明_9
	const CHAR  *	m_lpszImpactParameter_9;			// 效果参数集合_9
	const CHAR	*	m_lpszImpcatExplain_10;				// 效果说明_10
	const CHAR  *	m_lpszImpactParameter_10;			// 效果参数集合_10
	const CHAR	*	m_lpszImpcatExplain_11;				// 效果说明_11
	const CHAR  *	m_lpszImpactParameter_11;			// 效果参数集合_11
	const CHAR	*	m_lpszImpcatExplain_12;				// 效果说明_12
	const CHAR  *	m_lpszImpactParameter_12;			// 效果参数集合_12

	const CHAR	*	m_lpszChildSkillID;					// 子技能集合
	const CHAR	*	M_lpszChildSkillOdds;				// 子技能触发几率

	INT				m_nTransferSkill;					// 转移类技能，ENUM_TRANSFER_SKILL

	INT				m_bJiTui;								//是否被击退
	INT				m_JiTuiBaifenbi;								//回退百分比
	INT				m_JiTuiRate;								//回退概率
	INT				m_bSouXiaSkill;								//是否是搜侠录技能

//	INT				m_bAutoRedo;						// 自动技能

};
typedef _DBC_SKILL_DATA SSkillData;
typedef _DBC_SKILL_DATA _SKILL_DATA;

///////////////////////////////////////////////////////////////////////
// 心法
struct _DBC_XINFA_DATA
{
	//
	UINT		m_uID;							// ID
	UINT		m_uIDMenpai;					// 门派ID
	const CHAR	*m_lpszName;					// 名称
	CHAR		*m_pszDesc;						// 技能描述
	const CHAR	*m_lpszIconName;				// Icon名称
};
///////////////////////////////////////////////////////////////////////
struct _OWN_SKILL
{
	enum _OWN_SKILL_STATE
	{
		OWN_SKILL_DISABLE,					//不能学习，也不能使用和升级
		OWN_SKILL_CAN_LEARN,				//可以学习但不可以使用和升级
		OWN_SKILL_CAN_USE,					//可以使用不能升级
		OWN_SKILL_CAN_UPGRADE				//可以升级
	};
	SkillID_t			m_nSkillID;			//拥有的技能ID
	_OWN_SKILL_STATE	m_eState;			//技能所处的状态
	BYTE				m_nLevel ;			//技能等级
};

struct _OWN_XINFA
{
	SkillID_t		m_nXinFaID ;			//拥有的心法ID
	BYTE			m_nLevel ;				//心法等级
};



////////////////////////////////////////////
// 伤害的结构
#define DAMAGE_INFO_PARAM_NUMBER	(8)

struct _DAMAGE_INFO
{
	enum
	{
		TYPE_INVALID = INVALID_ID,
		TYPE_EFFECT = 0,
		TYPE_HEAL_AND_DAMAGE = 1,
		TYPE_DROP_BOX = 2,
		TYPE_SKILL_TEXT = 3,
		TYPE_DIE = 4,
	};
	SkillID_t		m_nSkillID;				// 技能ID
	ObjID_t 		m_nTargetID;			// 目标对象的ID
	ObjID_t			m_nSenderID;			// 攻击者的ID
	INT				m_nSenderLogicCount;	// 攻击者的逻辑计数
	ImpactID_t		m_nImpactID;			// 效果ID //参考GameStruct_Impact.h的DIRECT_IMPACT_SEID
	ID_t			m_nType;				// 效果、伤害和治疗数值、掉落盒
	INT				m_nHealthIncrement;
	BOOL			m_bHealthDirty;
	INT				m_nManaIncrement;
	BOOL			m_bManaDirty;
	INT				m_nRageIncrement;
	BOOL			m_bRageDirty;
	INT				m_nStrikePointIncrement;
	BOOL			m_bStrikePointDirty;
	BOOL			m_bIsCriticalHit;
	
	BOOL			m_bBeatBack;
	BOOL			m_bBeatFly;
	WORLD_POS		m_BeatBackPos;
	WORLD_POS		m_CloseToPos;
	FLOAT			m_fBeatFlyDist;
	FLOAT			m_fAttackerAngle;
	BOOL			m_bCloseTo;

	INT				m_nDelayTime;			// 延迟时间

	INT 			m_aAttachedParams[DAMAGE_INFO_PARAM_NUMBER]; // 附加的参数

	_DAMAGE_INFO( VOID )
	{
		Reset();
	}

	VOID Reset( VOID )
	{
		m_nTargetID				= INVALID_ID;
		m_nSenderID				= INVALID_ID;
		m_nSenderLogicCount		= -1;
		m_nImpactID				= INVALID_ID;
		m_nType					= TYPE_INVALID;
		m_nHealthIncrement		= 0;
		m_bHealthDirty			= FALSE;
		m_nManaIncrement		= 0;
		m_bManaDirty			= FALSE;
		m_nRageIncrement		= 0;
		m_bRageDirty			= FALSE;		
		m_nStrikePointIncrement	= 0;
		m_bStrikePointDirty		= FALSE;
		m_bIsCriticalHit		= FALSE;
		m_bBeatBack				= FALSE;
		m_bBeatFly				= FALSE;
		m_fBeatFlyDist			= 0;
		m_fAttackerAngle		= 0;
		m_bCloseTo				= FALSE;
		m_nDelayTime			= 0;
		memset( m_aAttachedParams, 0, sizeof( m_aAttachedParams ) );
	}

	const _DAMAGE_INFO &operator = ( const _DAMAGE_INFO &in )
	{
		if ( this != &in )
		{
			m_nTargetID				= in.m_nTargetID;
			m_nSenderID				= in.m_nSenderID;
			m_nSenderLogicCount		= in.m_nSenderLogicCount;
			m_nImpactID				= in.m_nImpactID;
			m_nType					= in.m_nType;
			m_nHealthIncrement		= in.m_nHealthIncrement;
			m_bHealthDirty			= in.m_bHealthDirty;
			m_nManaIncrement		= in.m_nManaIncrement;
			m_bManaDirty			= in.m_bManaDirty;
			m_nRageIncrement		= in.m_nRageIncrement;
			m_bRageDirty			= in.m_bRageDirty;
			m_nStrikePointIncrement = in.m_nStrikePointIncrement;
			m_bStrikePointDirty		= in.m_bStrikePointDirty;
			m_bIsCriticalHit		= in.m_bIsCriticalHit;
			m_bBeatBack				= in.m_bBeatBack;
			m_bBeatFly				= in.m_bBeatFly;
			m_BeatBackPos			= in.m_BeatBackPos;
			m_CloseToPos			= in.m_CloseToPos;
			m_fBeatFlyDist			= in.m_fBeatFlyDist;
			m_fAttackerAngle		= in.m_fAttackerAngle;
			m_bCloseTo				= in.m_bCloseTo;
			m_nDelayTime			= in.m_nDelayTime;

			memcpy( m_aAttachedParams, in.m_aAttachedParams, sizeof( m_aAttachedParams ) );
		}
		return *this;
	}
};

////////////////////////////////////////////
// 技能BUFF的结构
#define BUFF_IMPACT_INFO_PARAM_NUMBER	(8)

struct _BUFF_IMPACT_INFO
{
	ObjID_t			m_nReceiverID;			// 效果接受者的ID
	ObjID_t			m_nSenderID;			// 效果释放者的ID
	ImpactID_t		m_nBuffID;				// 特效数据的ID(索引)
	SkillID_t		m_nSkillID;  			// Skill ID
	INT				m_nSenderLogicCount;	// 效果创建者的逻辑计数
	UINT			m_nSN;					// 效果序列号
	INT				m_nTimer;				// 剩余时间
	INT				m_nDelayTime;			// 延迟处理时间

	_BUFF_IMPACT_INFO( VOID )
	{
		Reset();
	}

	VOID Reset( VOID )
	{
		m_nReceiverID			= INVALID_ID;
		m_nSenderID				= INVALID_ID;
		m_nBuffID				= INVALID_ID;
		m_nSkillID				= INVALID_ID;
		m_nSenderLogicCount		= -1;
		m_nSN					= -1;
		m_nTimer				= 0;
		m_nDelayTime			= 0;
	}

	const _BUFF_IMPACT_INFO &operator = ( const _BUFF_IMPACT_INFO &in )
	{
		if ( &in != this )
		{
			m_nReceiverID			= in.m_nReceiverID;
			m_nSenderID				= in.m_nSenderID;
			m_nBuffID				= in.m_nBuffID;
			m_nSkillID				= in.m_nSkillID;
			m_nSenderLogicCount		= in.m_nSenderLogicCount;
			m_nSN					= in.m_nSN;
			m_nTimer				= in.m_nTimer;
			m_nDelayTime			= in.m_nDelayTime;
		}
		return *this;
	}
};

// 传说中的拉人的信息结构
struct _CALLOF_INFO
{
	GUID_t			m_guidCaller;	// 召唤者的GUID
	SceneID_t		m_SceneID;		// 场景ID
	WORLD_POS		m_Pos;			// 场景位置
	UINT			m_uDuration;	// 持续时间

	_CALLOF_INFO( VOID )
	{
		m_guidCaller		= INVALID_GUID;
		m_SceneID			= INVALID_ID;
		m_Pos				= WORLD_POS( -1.f, -1.f );
		m_uDuration			= 0;
	}

	VOID Reset( VOID )
	{
		m_guidCaller		= INVALID_GUID;
		m_SceneID			= INVALID_ID;
		m_Pos				= WORLD_POS( -1.f, -1.f );
		m_uDuration			= 0;
	}
};

//cooldown
class Cooldown_T
{
	public:
		Cooldown_T(VOID): m_nID(INVALID_ID), m_nCooldown(0), m_nCooldownElapsed(0){};
		Cooldown_T(Cooldown_T const& rhs)
		{
			m_nID = rhs.GetID();
			m_nCooldown = rhs.GetCooldownTime();
			m_nCooldownElapsed = rhs.GetCooldownElapsed();
		};
		~Cooldown_T(VOID){}
		Cooldown_T& operator=(Cooldown_T const& rhs)
		{
			m_nID = rhs.GetID();
			m_nCooldown = rhs.GetCooldownTime();
			m_nCooldownElapsed = rhs.GetCooldownElapsed();
			return *this;
		};
		VOID HeartBeat(Time_t nDeltaTime)
		{
			if(0>m_nID)
			{
				return;
			}
			if(m_nCooldown<=m_nCooldownElapsed)
			{
				return;
			}
			m_nCooldownElapsed +=nDeltaTime;
			if(m_nCooldown<m_nCooldownElapsed)
			{
				m_nCooldown=m_nCooldownElapsed;
			}
		};
		BOOL IsCooldowned(VOID) const
		{
			return m_nCooldown<=m_nCooldownElapsed;
		};
		Time_t GetRemainTime(VOID) const
		{
			return m_nCooldown - m_nCooldownElapsed;
		};
		VOID Reset(VOID)
		{
			m_nID = INVALID_ID;
			m_nCooldown = 0;
			m_nCooldownElapsed = 0;
		};
		ID_t GetID(VOID) const {return m_nID;}
		VOID SetID(ID_t nID) {m_nID = nID;}
		Time_t GetCooldownTime(VOID) const {return m_nCooldown;}
		VOID SetCooldownTime(Time_t nTime) {m_nCooldown = nTime;}
		Time_t GetCooldownElapsed(VOID) const {return m_nCooldownElapsed;}
		VOID SetCooldownElapsed(Time_t nTime){m_nCooldownElapsed = nTime;}
	protected:
	private:
		ID_t	m_nID;
		Time_t	m_nCooldown;
		Time_t	m_nCooldownElapsed;
};
template <INT nSize>
class CooldownList_T
{
	public:
		enum
		{
			LIST_SIZE = nSize,
		};
		CooldownList_T(VOID){}
		CooldownList_T(CooldownList_T const& rhs)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx] = rhs.GetCooldownInfoByIndex(nIdx);
			}
		}
		~CooldownList_T(VOID){}
		VOID CleanUp(VOID) {Reset();}
		VOID Reset(VOID)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx].Reset();
			}
		}
		CooldownList_T& operator=(CooldownList_T const& rhs)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx] = rhs.GetCooldownInfoByIndex(nIdx);
			}
			return *this;
		}
		Cooldown_T const& GetCooldownInfoByIndex(INT nIdx) const
		{
			if(0>nIdx || LIST_SIZE<=nIdx)
			{
				AssertEx(FALSE,"[CooldownList_T::GetCooldownByIndex]:Illegal index found!!");
				return m_aCooldown[0];
			}
			return m_aCooldown[nIdx];
		}
		Time_t GetRemainTimeByID(CooldownID_t nCooldown) const
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					return m_aCooldown[nIdx].GetRemainTime();
				}
			}
			return TRUE;
		}
		INT GetSlotSize(VOID)
		{
			return LIST_SIZE;
		}
		INT GetByteSize(VOID)
		{
			return sizeof(CooldownList_T);
		}
		VOID HeartBeat(Time_t nDeltaTime)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx].HeartBeat(nDeltaTime);
			}
		}
		BOOL IsSpecificSlotCooldownedByID(CooldownID_t nCooldown) const
		{
			if(0 > nCooldown)
			{
				return TRUE;
			}
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					return m_aCooldown[nIdx].IsCooldowned();
				}
			}
			return TRUE;
		}
		VOID RegisterCooldown(CooldownID_t nCooldown, Time_t nCooldownTime)
		{
			INT nEmptySlot = INVALID_ID;
			if(0 > nCooldown)
			{
				return;
			}
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(INVALID_ID==nEmptySlot)
				{
					if(INVALID_ID==m_aCooldown[nIdx].GetID())
					{
						nEmptySlot = nIdx;
					}
					else if(TRUE==m_aCooldown[nIdx].IsCooldowned())
					{
						nEmptySlot = nIdx;
					}
				}
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					m_aCooldown[nIdx].Reset();
					m_aCooldown[nIdx].SetID(nCooldown);
					m_aCooldown[nIdx].SetCooldownTime(nCooldownTime);
					return;
				}
			}
			if(INVALID_ID!=nEmptySlot)
			{
					m_aCooldown[nEmptySlot].Reset();
					m_aCooldown[nEmptySlot].SetID(nCooldown);
					m_aCooldown[nEmptySlot].SetCooldownTime(nCooldownTime);
					return;
			}
			AssertEx(FALSE, "[CooldownList_T::RegisterCooldown]: Cooldown list full!!");
		}
	protected:
	private:
		Cooldown_T m_aCooldown[LIST_SIZE];
};

class MissFlag_T //empty class for wrap constants
{
	public:
		enum
		{
			FLAG_NORMAL =0,
			FLAG_MISS,
			FLAG_IMMU,
			FLAG_ABSORB,
			FLAG_COUNTERACT,
			FLAG_TRANSFERED,
		};
};

enum
{
	MAX_WAY_POINT_NUM = 4
};

//用于延迟m_nDelayTime后才向client发送 SkillMiss 消息
struct _SkillMissMSGDelayTime
{
	ObjID_t		m_nSender;		
	SkillID_t	m_nSkill;
	Time_t		m_nDelayTime;
	INT			m_nLogicCount;
	_SkillMissMSGDelayTime()
	{
		Cleanup();
	}
	VOID Cleanup()
	{
		memset( this, -1, sizeof(_SkillMissMSGDelayTime) );
	}
};

struct _Monster_Pet_DB_Struct
{
	INT			m_iDataIndex;							//monsterAttrex.tab 表索引值
	INT			m_iHP;									//召唤兽当前血量
	INT			iLifeTime;								//召唤兽剩余生命时间
	_OWN_IMPACT m_aImpactList[MONSTER_PET_MAX_IMPACT];	//召唤兽身上中的buff
	INT			m_aSkillList[MONSTER_PET_MAX_SKILL];	//召唤兽当前血量
};

#endif
