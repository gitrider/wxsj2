// GameStruct_Pet.h

#ifndef __GAMESTRUCT_PET_H__
#define __GAMESTRUCT_PET_H__

#include "Type.h"
#include "GameDefine2.h"
#include "DB_Struct.h"

////////////////////////////////////////////////
// 附加宠物属性定义
////////////////////////////////////////////////
enum ENUM_PET_FOOD_TYPE
{
	PET_FOOD_TYPE_INVALID = 0,
	PET_FOOD_TYPE_MEAT,				//肉食宠粮
	PET_FOOD_TYPE_GRASS,			//草类宠粮
	PET_FOOD_TYPE_WORM,				//虫类宠粮
	PET_FOOD_TYPE_PADDY,			//谷类宠粮

	PET_FOOD_TYPE_NUMBER,
};

struct PET_ATTR
{
	INT		m_Type;						// 宠物编号
	CHAR	m_Name[NPC_NAME_LEN];		// 名称
	INT		m_TakeLevel;				// 可携带等级
	INT		m_bVarPet;					// 是否为变异宠物
	INT		m_bBabyPet;					// 是否为宝宝宠物
	INT		m_AI;						// 宠物性格
	INT		m_FoodType;					// 食物类
	INT		m_PassiveSkillCount;		// 自动技能数
	INT		m_VoluntarySkill;			// 手动技能
	INT		m_PassiveSkill1;			// 自动技能1
	INT		m_PassiveSkill2;			// 自动技能2
	INT		m_PassiveSkill3;			// 自动技能3
	INT		m_PassiveSkill4;			// 自动技能4
	//INT		m_Life;					// 标准寿命

	INT		m_iStrengthPerception;		// 力量资质
	INT		m_iSmartnessPerception;		// 敏捷资质
	INT 	m_iMindPerception;			// 智力资质
	INT 	m_iConstitutionPerception;	// 体质资质

	INT		m_GrowRate0;				// 成长率1
	INT		m_GrowRate1;				// 成长率2
	INT		m_GrowRate2;				// 成长率3
	INT		m_GrowRate3;				// 成长率4
	INT		m_GrowRate4;				// 成长率5
	INT		m_CowardiceRate;			// 胆小几率
	INT		m_WarinessRate;				// 谨慎几率	
	INT		m_LoyalismRate;				// 忠诚几率
	INT		m_CanninessRate;			// 精明几率
	INT		m_ValourRate;				// 勇猛几率
};

////////////////////////////////////////////////
// _PET_DETAIL_ATTRIB 宠物属性
////////////////////////////////////////////////
struct _PET_DETAIL_ATTRIB
{
public:
	_PET_DETAIL_ATTRIB(VOID);

	BOOL Init(const _PET_DB *pPetDB,
		ObjID_t idObj,
		INT nAtt_Near,
		INT nDef_Near,
		INT nAtt_Far,	
		INT nDef_Far,	
		INT nAtt_MagicNear,
		INT nAtt_MagicFar,
		INT nDef_MagicNear,
		INT nDef_MagicFar,
		INT nHit,
		INT nMiss,
		INT nCritical,
		INT nModelID,
		INT nMountID);

	VOID CleanUp(VOID);

public:
	PET_GUID_t		m_GUID;							// ID

	ObjID_t			m_ObjID;						// 所有Obj类型的ObjID
	INT				m_nDataID;						// 模型ID,宠物类型
	CHAR			m_szName[MAX_CHARACTER_NAME];	// 名称
	INT				m_nAIType;						// 性格
	PET_GUID_t		m_SpouseGUID;					// 配偶的GUID
	INT				m_nLevel;						// 等级
	INT				m_nExp;							// 经验
	INT				m_nHP;							// 血当前值
	INT				m_nHPMax;						// 血最大值

	INT				m_nLife;						// 当前寿命
	BYTE			m_byGeneration;					// 几代宠
	BYTE			m_byHappiness;					// 快乐度

	INT m_nAtt_Near;
	INT m_nDef_Near;
	INT m_nAtt_Far;	
	INT m_nDef_Far;	

	INT				m_nAtt_MagicNear;				// 近程内功攻击力
	INT				m_nAtt_MagicFar;				// 近程外功攻击力
	INT				m_nDef_MagicNear;				// 近程内功防御力
	INT				m_nDef_MagicFar;				// 近程外功防御力

	INT				m_nHit;							// 命中率
	INT				m_nMiss;						// 闪避率
	INT				m_nCritical;					// 会心率

	INT				m_nModelID;						// 外形
	INT				m_nMountID;						// 座骑ID

	INT				m_iStrengthPerception;			// 力量资质
	INT				m_iSmartnessPerception;			// 敏捷资质
	INT 			m_iMindPerception;				// 智力资质
	INT 			m_iConstitutionPerception;		// 体质资质

	INT				m_Str;							// 力量
	INT				m_Con;							// 体力
	INT 			m_Dex;							// 身法
	//INT				m_Spr;						// 灵气
	INT 			m_Int;							// 定力
	INT 			m_GenGu;						// 根骨

	INT				m_nRemainPoint;					// 潜能点

	_OWN_SKILL		m_aSkill[MAX_PET_SKILL_COUNT];	// 技能列表
};


////////////////////////////////////////////////
// _PET_PLACARD_ITEM 宠物公告板的一项
////////////////////////////////////////////////

// 客户端宠物公告板的显示条数
#define MAX_PETPLACARD_LIST_ITEM_NUM		(2)

#define PET_PLACARD_ITEM_MESSAGE_SIZE		(64)

struct _PET_PLACARD_ITEM
{
public:

	_PET_PLACARD_ITEM(VOID);
	~_PET_PLACARD_ITEM(VOID);

	BOOL Init(	GUID_t HumanGUID,
				const CHAR *pszHumanName,
				INT nHumanLevel,
				const CHAR *pszHumanGuildName,
				INT nHumanMenPai,
				const _PET_DETAIL_ATTRIB *pPetAttr,
				const CHAR *pszMessage,
				UINT uCreateTime);

	VOID CleanUp(VOID);

	BOOL IsInvalid(VOID)const
	{
		return (m_HumanGUID == INVALID_ID)?(TRUE):(FALSE);
	}

public:
	UINT GetCreateTime(VOID)const
	{
		return m_uCreateTime;
	}

	GUID_t GetHumanGUID(VOID)const
	{
		return m_HumanGUID;
	}

	const CHAR *GetHumanName(VOID)const
	{
		return m_szHumanName;
	}

	INT GetHumanLevel(VOID)const
	{
		return m_nHumanLevel;
	}

	const CHAR *GetHumanGuildName(VOID)const
	{
		return m_szHumanGuildName;
	}

	const _PET_DETAIL_ATTRIB *GetPetAttr(VOID)const
	{
		return &m_PetAttr;
	}

	const CHAR *GetMessage(VOID)const
	{
		return m_szMessage;
	}

	const INT GetHumanMenPai(VOID) const
	{
		return m_nHumanMenPai;
	}

protected:
	// 创建信息
	UINT				m_uCreateTime;

	// 宠主信息
	GUID_t				m_HumanGUID;
	CHAR				m_szHumanName[MAX_CHARACTER_NAME];
	INT					m_nHumanLevel;
	CHAR				m_szHumanGuildName[MAX_GUILD_NAME_SIZE];
	INT					m_nHumanMenPai;

	// 宠物信息
	_PET_DETAIL_ATTRIB	m_PetAttr;

	// 留言
	CHAR				m_szMessage[PET_PLACARD_ITEM_MESSAGE_SIZE];
};

const INT MaxSlotSkillNum		= 64;
const INT MaxSlotNum			= 6;
const INT MaxReturnToChildIDNum	= 8;
const INT MaxHorseDropNum		= MaxReturnToChildIDNum;
const INT MaxHorseDropGroupNum	= 10;

struct _HORSE_SKILL_SLOT
{
	INT m_aSkillSlot[MaxSlotSkillNum];		//技能槽可选的技能ID
	INT m_iSkillCount;						//此技能槽实际可选的技能ID
};

struct _HORSE_SKILL
{
	INT m_iSkillRealizationID;					//技能领悟ID
	INT m_iSkillSlotCount;						//技能槽数量，最大为6
	_HORSE_SKILL_SLOT m_oSkillSlot[MaxSlotNum];	//技能槽1可选的技能ID
};

//马属性
struct _HORSE_ATTR
{
	INT			m_nDataID;												// 数据ID
	CHAR		m_sName[64];											// 名称
	INT			m_iSex;													// 性别
	INT			m_iLevel;												// 初始等级
	INT			m_iTakeLevel;											// 可携带等级
	INT			m_aReturnToChildID[MaxReturnToChildIDNum];				// 还童后的ID
	INT			m_iReturnToChildIDCount;								// 还童后可选ID的实际个数
	INT			m_aChildID[MaxReturnToChildIDNum];						// 交配后，2代的ID,对应QiChengJichu.tab中的ID
	INT			m_iChildIDCount;										// 交配后，2代可选ID的个数
	INT			m_bCanMating;											// 是否可以交配
	BOOL		m_bVariation;											// 是否变异
	BYTE		m_byGeneration;											// 繁殖代数
	BYTE		m_iAttach;												// 骑乘的归属
	INT			m_LifeNoMaster;											// 无主时的生命周期
	INT			m_iStrengthMin;											// 初始最小力量
	INT			m_iStrengthMax;											// 初始最大力量
	INT			m_iSmartnessMin;										// 初始最小敏捷
	INT			m_iSmartnessMax;										// 初始最大敏捷
	INT 		m_iMindMin;												// 初始最小智力
	INT 		m_iMindMax;												// 初始最大智力
	INT 		m_iConstitutionMin;										// 初始最小体质
	INT 		m_iConstitutionMax;										// 初始最大体质
	INT			m_iStrengthMinPer;										// 初始最小力量资质
	INT			m_iStrengthMaxPer;										// 初始最大力量资质
	INT			m_iSmartnessMinPer;										// 初始最小敏捷资质
	INT			m_iSmartnessMaxPer;										// 初始最大敏捷资质
	INT 		m_iMindMinPer;											// 初始最小智力资质	
	INT 		m_iMindMaxPer;											// 初始最大智力资质
	INT 		m_iConstitutionMinPer;									// 初始最小体质资质	
	INT 		m_iConstitutionMaxPer;									// 初始最大体质资质	
	INT			m_iSavvyMin;											// 初始最小悟性	
	INT			m_iSavvyMax;											// 初始最大悟性	
	INT			m_GenGuMin;												// 初始最小根骨	
	INT			m_GenGuMax;												// 初始最大根骨	
	INT			m_iPullulationRateMin;									// 初始最小成长率	
	INT			m_iPullulationRateMax;									// 初始最大成长率
	INT			m_iMoveSpeed;											// 移动速度
	INT			m_iBehaveID;											// 对应表现ID
	INT			m_iMountID;												// 对应在坐骑表中的索引
	

	INT			m_iSkillSlotCount;										//马初始技能个数，最大为6
	_HORSE_SKILL_SLOT m_oSkillSlot[MaxSlotNum];							//马初始技能列表

	INT			m_iRealizationID;										// 技能领悟ID
};

struct _HORSE_ITEM
{
	//道具ID
	INT	m_iItemID;								

	//道具类型
	//道具分别对应着不同的使用类型,表示的后面数值的作用的对象不同.
	//其中1表示的是增加骑乘悟性的道具,2表示增加骑乘根骨的道具 3表
	//示的是增加快乐度4表示增加可使用技能格子数5表示对应位置的洗去
	//技能格子的笑忘书,-1表示无作用6表示领悟型技能道具, 增加领悟技
	//能7表示扩展位置的技能书8表示还童书
	INT m_iItemType;							

	//改变对应的骑乘的属性的百分比数值
	//对应1类型的数值填写表示的含义是按照百分比增加骑乘的悟性,对应
	//2表示百分比增加根骨,3表示百分比增加快度,4,5,6,7，8该项数值无
	//作用
	INT m_iAttrChangePercent;

	//改变对应的骑乘的属性的点数数值
	//对应1类型的数值填写表示的含义是按照数值增加骑乘的悟性,对应2表
	//示按照数值增加根骨,3表示按照数值增加快乐度, 4表示扩展格子的数
	//目,5表示对应洗去的格子位置,8表示新骑乘的成长率6,7该数值无作用
	INT m_iAttrChangePoint;

	//技能领悟和学习对应技能ID
	//当道具类型填写6和7时候,填写该ID添加相应的技能, 对应技能表格的
	//ID号码,按照成功率添加技能.
	INT m_iStudySkillID;
	//技能领悟和学习的成功率
	INT m_iSuccessRate;
};

struct _HORSE_DROP_GROUP
{
	INT m_iGroupImportance;										//马掉落组的价值，计算掉落概率时作分子
	INT m_iGroupMaxDropTimes;									//掉落组最大的掉落次数
	INT m_aGroupHorseID[MaxHorseDropNum];						//马ID列表，随机选取一个，对应马基础表中的ID
	INT m_iHorseIDCount;										//m_aGroupHorseID中有效数值的个数
};

struct _HORSE_DROP
{
	INT m_iDropID;												//马掉落ID,怪物表中索引此ID
	INT m_iMonsterImportance;									//怪物的价值，计算概率时做分母
	INT m_iMaxDropNum;											//怪物的最大掉落数量
	_HORSE_DROP_GROUP m_aDropGroup[MaxHorseDropGroupNum];		//掉落组的数组
};


//召唤兽初始化属性
struct _MONSTER_PET_ATTR
{
	INT iLifeTime;
	INT iImpactToSelf;
	INT iImpactToOnwer;
	INT aSkillID[MONSTER_PET_MAX_SKILL];
};

#endif // __GAMESTRUCT_PET_H__
