
/** 数据池数据结构 -宠物
*/


#include "Type.h"
#include "DB_Struct.h"
#include "../../GameInterface/Include/GIDBC_Struct.h"

#pragma once


//-------------------------------------------
//	1) 宠物技能数据结构
//-------------------------------------------
struct PET_SKILL
{
	const _DBC_SKILL_DATA	*	m_pDefine;		// 技能表定义
	INT							m_nLevel;		// 技能的当前级别
	INT							m_nPosIndex;	// 第几个技能 (UI同步)
	BOOL						m_bCanUse;		// 格子是否有效

	PET_SKILL()
	{
		m_pDefine	= NULL;
		m_nLevel	= 0;
		m_nPosIndex	= -1;
		m_bCanUse	= FALSE;
	}
	VOID CleanUp( VOID )
	{
		m_pDefine	= NULL;
		m_nLevel	= 0;
		m_nPosIndex	= -1;
		m_bCanUse	= FALSE;
	}
	PET_SKILL& operator=(const PET_SKILL& val)
	{
		m_pDefine	= val.m_pDefine;
		m_nLevel	= val.m_nLevel;
		m_nPosIndex	= val.m_nPosIndex;
		m_bCanUse	= val.m_bCanUse;
		return *this;
	}
};

//-------------------------------------------
//	宠物数据结构
//------------------------------------------
struct SDATA_PET
{
public:	
	const _DBC_CHARACTER_MOUNT	 *	m_pModelData;	// 模型ID，对应char_mount.tab

	PET_GUID_t			m_GUID;			// 服务器ID
	INT					m_nBaseID;		// 基础ID， 对应 _DBC_MOUNT_BASE_DATA 索引

	BOOL				m_bLocked;		// 锁定状态，比如繁殖中
	
	STRING				m_szName;		// 名称
	STRING				m_szNickname;	// 昵称
	INT					m_nGender;		// 性别
	INT					m_nLevel;		// 等级
	INT 				m_nExp;			// 经验
	INT 				m_nEraCount;	// 几代宠
	INT 				m_nHappiness;	// 快乐度
	BOOL				m_bCanBeMating;	// 可否交配
	INT					m_nSkillGrid;	// 技能栏数量

	INT					m_nAttrStr;		// 力量
	INT					m_nAttrDex;		// 身法
	INT					m_nAttrInt;		// 定力
	INT					m_nAttrCon;		// 体力

	INT					m_nUpRate;		// 成长率
	INT					m_nPotential;	// 潜能	
	INT					m_nBasic;		// 根骨
	INT					m_nSavvy;		// 悟性

	INT					m_nAttrStrApt;	// 力量资质
	INT					m_nAttrDexApt;	// 身法资质
	INT					m_nAttrIntApt;  // 定力资质
	INT					m_nAttrConApt;	// 体力资质


	INT					m_nAIType;									// 性格
	PET_GUID_t		m_SpouseGUID;								// 配偶的GUID
	INT				m_nHP;										// 血当前值
	INT				m_nHPMax;									// 血最大值
	INT				m_nLife;									// 当前寿命

	INT				m_nHit;										// 命中率
	INT				m_nMiss;									// 闪避率
	INT				m_nCritical;								// 会心率

	INT				m_nAtt_Near;		//近程外功攻击力
	INT				m_nAtt_Far;		//远程外功攻击力

	INT				m_nAtt_MagicNear;		// 近程内功攻击力
	INT				m_nAtt_MagicFar;			// 远程内功攻击力

	INT				m_nDef_Near;	//近程外功防御力
	INT				m_nDef_Far;		//远程外功防御力

	INT				m_nDef_MagicNear;		// 近程内功防御力
	INT				m_nDef_MagicFar;			// 远程内功防御力


	PET_SKILL			m_Skill[PET_MAX_SKILL_COUNT];	// 技能列表

public:

	SDATA_PET()
	{
		m_pModelData	= NULL;	
		m_nBaseID		= -1;
		m_bLocked		= FALSE;
		m_nGender		= 0;
		m_szName		= "";
		m_szNickname	= "";
		m_nLevel		= 0;
		m_nExp			= 0;
		m_nEraCount		= 0;
		m_nHappiness	= 0;
		m_bCanBeMating	= TRUE;
		m_nSkillGrid	= 0;

		m_nAttrStr		= 0;
		m_nAttrDex		= 0;
		m_nAttrInt		= 0;
		m_nAttrCon		= 0;

		m_nUpRate		= 0;
		m_nPotential	= 0;
		m_nBasic		= 0;
		m_nSavvy		= 0;

		m_nAttrStrApt	= 0;
		m_nAttrDexApt	= 0;
		m_nAttrIntApt	= 0;
		m_nAttrConApt	= 0;

		m_nAIType		 = -1 ;									// 性格
		m_nHP = 0;										// 血当前值
		m_nHPMax = 0 ;									// 血最大值
		m_nLife = 0 ;									// 当前寿命

		m_nHit = 0;										// 命中率
		m_nMiss = 0;									// 闪避率
		m_nCritical = 0;								// 会心率

		m_nAtt_Near = 0;		//近程外功攻击力
		m_nAtt_Far = 0;		//远程外功攻击力

		m_nAtt_MagicNear = 0;		// 近程内功攻击力
		m_nAtt_MagicFar = 0;			// 远程内功攻击力

		m_nDef_Near = 0;	//近程外功防御力
		m_nDef_Far = 0;		//远程外功防御力

		m_nDef_MagicNear = 0;		// 近程内功防御力
		m_nDef_MagicFar = 0;			// 远程内功防御力

		m_GUID.Reset();
		m_SpouseGUID.Reset();								// 配偶的GUID

		for (INT i=0; i<PET_MAX_SKILL_COUNT; ++i)
		{
			m_Skill[i].CleanUp();
		}
	}

	VOID CleanUp( VOID )
	{
		m_pModelData	= NULL;
		m_nBaseID		= -1;
		m_bLocked		= FALSE;
		m_nGender		= 0;
		m_szName		= "";
		m_szNickname	= "";
		m_nLevel		= 0;
		m_nExp			= 0;
		m_nEraCount		= 0;
		m_nHappiness	= 0;
		m_bCanBeMating	= TRUE;
		m_nSkillGrid	= 0;

		m_nAttrStr		= 0;
		m_nAttrDex		= 0;
		m_nAttrInt		= 0;
		m_nAttrCon		= 0;

		m_nUpRate		= 0;
		m_nPotential	= 0;
		m_nBasic		= 0;
		m_nSavvy		= 0;

		m_nAttrStrApt	= 0;
		m_nAttrDexApt	= 0;
		m_nAttrIntApt	= 0;
		m_nAttrConApt	= 0;


		m_nAIType		 = -1 ;						// 性格
		m_nHP = 0;									// 血当前值
		m_nHPMax = 0 ;								// 血最大值
		m_nLife = 0 ;									// 当前寿命

		m_nHit = 0;									// 命中率
		m_nMiss = 0;									// 闪避率
		m_nCritical = 0;								// 会心率

		m_nAtt_Near = 0;		//近程外功攻击力
		m_nAtt_Far = 0;		//远程外功攻击力

		m_nAtt_MagicNear = 0;		// 近程内功攻击力
		m_nAtt_MagicFar = 0;			// 远程内功攻击力

		m_nDef_Near = 0;	//近程外功防御力
		m_nDef_Far = 0;		//远程外功防御力

		m_nDef_MagicNear = 0;		// 近程内功防御力
		m_nDef_MagicFar = 0;			// 远程内功防御力

		m_GUID.Reset();
		m_SpouseGUID.Reset();								// 配偶的GUID

		for (INT i=0; i<PET_MAX_SKILL_COUNT; ++i)
		{
			m_Skill[i].CleanUp();
		}
	}
	SDATA_PET& operator=(const SDATA_PET& val)
	{
		m_pModelData	= val.m_pModelData;
		m_nBaseID		= val.m_nBaseID;
		m_bLocked		= val.m_bLocked;
		m_nGender		= val.m_nGender;
		m_szName		= val.m_szName;
		m_szNickname	= val.m_szNickname;
		m_nLevel		= val.m_nLevel;
		m_nExp			= val.m_nExp;
		m_nEraCount		= val.m_nEraCount;
		m_nHappiness	= val.m_nHappiness;
		m_bCanBeMating	= val.m_bCanBeMating;
		m_nSkillGrid	= val.m_nSkillGrid;

		m_nAttrStr		= val.m_nAttrStr;
		m_nAttrDex		= val.m_nAttrDex;
		m_nAttrInt		= val.m_nAttrInt;	
		m_nAttrCon		= val.m_nAttrCon;	

		m_nUpRate		= val.m_nUpRate;
		m_nPotential	= val.m_nPotential;
		m_nBasic		= val.m_nBasic;
		m_nSavvy		= val.m_nSavvy;

		m_nAttrStrApt	= val.m_nAttrStrApt;
		m_nAttrDexApt	= val.m_nAttrDexApt;
		m_nAttrIntApt	= val.m_nAttrIntApt;
		m_nAttrConApt	= val.m_nAttrConApt;

		m_nAIType		 = val.m_nAIType ;									// 性格
		m_SpouseGUID	 = val.m_SpouseGUID ;								// 配偶的GUID
		m_nHP = val.m_nHP;										// 血当前值
		m_nHPMax = val.m_nHPMax ;									// 血最大值
		m_nLife = val.m_nLife ;									// 当前寿命

		m_nHit = val.m_nHit;										// 命中率
		m_nMiss = val.m_nMiss;									// 闪避率
		m_nCritical = val.m_nCritical;							// 会心率

		m_nAtt_Near = val.m_nAtt_Near;		//近程外功攻击力
		m_nAtt_Far = val.m_nAtt_Far;			//远程外功攻击力

		m_nAtt_MagicNear = val.m_nAtt_MagicNear;		// 近程内功攻击力
		m_nAtt_MagicFar = val.m_nAtt_MagicFar;				// 远程内功攻击力

		m_nDef_Near = val.m_nDef_Near;	//近程外功防御力
		m_nDef_Far = val.m_nDef_Far;			//远程外功防御力

		m_nDef_MagicNear = val.m_nDef_MagicNear;		// 近程内功防御力
		m_nDef_MagicFar = val.m_nDef_MagicFar;			// 远程内功防御力

		m_GUID = val.m_GUID;

		for (INT i=0; i<PET_MAX_SKILL_COUNT; ++i)
		{
			m_Skill[i].CleanUp();
			m_Skill[i] = val.m_Skill[i];
		}
		return *this;
	}
};

