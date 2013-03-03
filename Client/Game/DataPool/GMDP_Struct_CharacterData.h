
/** 数据池数据结构 -角色数据
 *	
 *	功能：	
 *
 *	修改记录：
 *			080325	修改 SDATA_PLAYER_OTHER 中称号表的数量 20 -> 256
 *			
 *			
 */

#pragma once


//========================================================
//
//			角色对象中完全由服务器决定的属性数据
//
//========================================================

#include "GameDefine.h"
#include "GameDefine2.h"
#include "GIDBC_Struct.h"
#include "DB_Struct.h"
#include "GameStruct_Skill.h"

#include "GMDP_Struct_Skill.h"
#include "GMDP_Struct_Title.h"
#include "GMDP_Struct_Pet.h"

//=================================================================
//-->0. Character 基本属性
//=================================================================
struct SDATA_CHARACTER
{
	INT				m_nRaceID;					// 数据表中的ID						

	INT				m_nPortraitID;				// 头像ID		
	STRING			m_strName;					// 角色的名字，从服务器传来	
	STRING			m_strUtf8Name;				// 角色的Utf8 名称
	STRING			m_strCountryTitle;			// 当前国家称号
	STRING			m_strGuildTitle;			// 当前帮会称号
	STRING			m_strTitle;					// 当前玩法称号
	BYTE			m_TitleType;				// 当前称号类型
	FLOAT			m_fHPPercent;				// 生命值百分比						
	FLOAT			m_fMPPercent;				// 魔法值百分比
	FLOAT			m_fPenumaPercent;			//元气百分比
	INT				m_nRage;					// 怒气
	FLOAT			m_fMoveSpeed;				// 移动的速度						
	_CAMP_DATA		m_CampData;					// 阵营ID	
	INT				m_nOwnerID;					// 所有者ID
	GUID_t			m_OccupantGUID;				// 占有者(是谁打的，死了算谁的)
	INT				m_nRelative;				// 相对关系，这个人是否可被攻击，是否主动攻击
	INT				m_nModelID;					// 模型ID
	INT				m_nMountID;					// 现使用的坐骑ID
	INT				m_nMountIndex;				// 现使用的坐骑在MountContainer的位置索引            [7/23/2010 陈军龙]
	INT				m_nLevel;					// 等级								
	BOOL			m_bFightState;				// 战斗状态							
	INT				m_nStealthLevel;			// 隐身级别
	BOOL			m_bSit;						// 坐下状态
	INT				m_nAIType;					// AI类型
    INT             m_nCountry;                 // 国家
	INT				m_nMountEquipID[HEQUIP_HORSE_NUMBER];		// 坐骑ID
	INT				m_nMountList[MAX_MOUNT_SIZE];// 拥有的坐骑ID                                     [7/14/2010 陈军龙]
};

//=================================================================
//-->1. Character_NPC 对于NPC
//=================================================================
struct SDATA_NPC : public SDATA_CHARACTER
{
	INT				m_nWeaponID;				// 武器ID
};

//=================================================================
//-->2. Character_PlayerOther 对于其他玩家
//=================================================================


struct SDATA_PLAYER_OTHER : public SDATA_NPC
{
	INT				m_nProfession;							// 职业//8个门派每个有2个发展方向,服务器按照16个个职业(或门派)处理
																/*
																|	0、少林派-棍武僧
																|	1、少林派-铁罗汉
																|	2、丐帮-棍丐帮" 
																|	3、丐帮-掌丐帮" 
																|	4、武当派-剑武当
																|	5、武当派-气武当
																|	6、唐门-刀唐" );
																|   7、唐门-陷阱唐" 
																|	8、逍遥派-扇逍遥
																|	9、逍遥派-指逍遥
																|	10、药王谷-狂药王
																|	11、药王谷-医药王
																|	12、桃花岛-笛桃花
																|	13、桃花岛-音桃花
																|	14、峨嵋派-刺峨嵋
																|   15、峨嵋派-辅峨嵋
																*/
	
	INT				m_nHairMeshID;							// 头发外形	-> DBC_CHARACTER_HAIR_GEO					
	INT				m_nFaceMeshID;							// 脸部外形	-> DBC_CHARACTER_HEAD_GEO( char_head.tab )
	UINT			m_uHairColor;							// 头发颜色
	INT				m_nEquipVer;							// 角色的装备状态版本号，用于和服务器同步数据的依据		
	INT				m_nEquipmentID[HEQUIP_NUMBER];			// 装备表;用于外形显示				
	INT				m_nEquipmentGemID[HEQUIP_NUMBER];		// 装备宝石表;用于外形显示	
	_SUIT_SETTING	m_oChangeSuit[MAX_EQUIP_SUIT_NUM];		// 一键换装数据

	BOOL			m_bTeamFlag;							// 是否有队伍
	BOOL			m_bTeamLeaderFlag;						// 是否是队长
	BOOL			m_bTeamFullFlag;						// 是否是满队
	BOOL			m_bTeamFollowFlag;						// 是否组队跟随
	INT				m_nTeamFollowListCount;					// 跟随列表里的人数
	ObjID_t			m_aTeamFollowList[MAX_TEAM_MEMBER];		// 跟随列表
	
	INT				m_nTitleNum;							// 称号数量
	INT				m_nCurTitleIndex;						// 当前使用的称号索引
	_TITLE_			m_nTitleList[MAX_TITLE_SIZE];			// 称号表

	BOOL			m_bIsInStall;							// 是否摆摊中
	STRING			m_strStallName;							// 摊位名	
	BOOL			m_bIsOpenPublic;						// 摊位是否开市(公开显示摆摊牌给其他玩家)
	GUID_t		m_nGuid;						//自己的GUID
	ObjID_t		m_BusObjID;
};


//=================================================================
//-->3. Character_PlayerMySelf 对于玩家自己
//=================================================================

class tObject_Item;
struct SDATA_PLAYER_MYSELF : public SDATA_PLAYER_OTHER
{
	//-----------------------------------------------------
	INT				m_nHP;				// 生命点						
	INT				m_nMP;				// 魔法点
	INT				m_nPneuma;			// 元气
	INT				m_nExp;				// 经验值						
	//INT			m_nMaxExp;			// 经验值上限(根据等级查表获得)					
	INT				m_nMoney;			// 游戏币数	
	INT				m_nStrikePoint;		// 连技点
	INT				m_nVigor;			// 活力值
	INT				m_nMaxVigor;		// 活力值上限
	INT				m_nEnergy;			// 精力值
	INT				m_nMaxEnergy;		// 精力值上限
	INT				m_nGoodBadValue;	// 善恶值
	PET_GUID_t		m_guidCurrentPet;	// 当前宠物GUID

	//-----------------------------------------------------
	INT				m_nGuild;			// 帮派ID 

	// ----------------------------------------------------
	// 已经分配的系别点数
	INT				m_nSeriesPoint_1;		// 系别 1
	INT				m_nSeriesPoint_2;		// 系别 2
	INT				m_nSeriesPoint_3;		// 系别 3

	INT				m_nSkillPoint;

	//-----------------------------------------------------
	//一级战斗属性
	INT				m_nSTR;				// 外功							
//	INT				m_nSPR;				// 内功							
	INT				m_nCON;				// 身法							
	INT				m_nINT;				// 体魄							
	INT				m_nDEX;				// 智慧							
	INT				m_nPoint_Remain;	// 剩余待分配点数				

	//-----------------------------------------------------
	//二级战斗属性
//	INT				m_nAtt_Physics;			// 物理攻击力
//	INT				m_nDef_Physics;			// 物理防御力
	INT				m_nAtt_Near;			// 近攻
	INT				m_nDef_Near;			// 近防
	INT				m_nAtt_Far;				// 远攻
	INT				m_nDef_Far;				// 远防

	INT				m_nAtt_Magic_Near;		// 近程内功攻击力										
	INT				m_nDef_Magic_Near;		// 近程内功防御力			
	INT				m_nAtt_Magic_Far;		// 远程内功攻击力										
	INT				m_nDef_Magic_Far;		// 远程内功防御力	
	INT				m_nMaxHP;				// 最大生命点					
	INT				m_nMaxMP;				// 最大魔法点
	INT				m_nMaxPneuma;			// 最大元气点
	INT				m_nMaxRage;				// 最大怒气点
	INT				m_nHP_ReSpeed;			// HP恢复速度  点/秒			
	INT				m_nMP_ReSpeed;			// MP恢复速度  点/秒	
	INT				m_nToughness;			// 韧性
	INT				m_nHit;					// 命中率						
	INT				m_nMiss;				// 闪避率						
	INT				m_nCritRate;			// 会心率						
	//FLOAT			m_fMoveSpeed;			// 移动速度(在基本属性中)					
	INT				m_nAttackSpeed;			// 攻击速度						
	INT				m_nAtt_Gold;			// 金攻击						
	INT				m_nDef_Gold;			// 金防御						
	INT				m_nAtt_Fire;			// 火攻击
	INT				m_nDef_Fire;			// 火防御
	INT				m_nAtt_Wood;			// 木攻击
	INT				m_nDef_Wood;			// 木防御
	INT				m_nAtt_Water;			// 水攻击
	INT				m_nDef_Water;			// 水防御
	INT				m_nAtt_Soil;			// 土攻击
	INT				m_nDef_Soil;			// 土防御

	BOOL			m_bLimitMove;			//是否限制不能移动
	BOOL			m_bCanActionFlag1;		//技能受限标记1,用于昏迷催眠
	BOOL			m_bCanActionFlag2;		//技能受限标记2,用于沉默
	//BOOL			m_bLimitUseSkill;		//是否限制不能施法
	//BOOL			m_bLimitHandle;			//是否限制不能进行一切操作

	BOOL			m_bIsMinorPwdSetup;		// 是否已设置二级密码
	BOOL			m_bIsMinorPwdUnlocked;	// 是否已经解锁二级密码

	//-----------------------------------------------------
	//其他属性
//$$	SXINFA_MAP			m_theXinFa;			//心法
	SSKILL_MAP			m_theSkill;			//技能数据
	SLIFEABILITY_MAP	m_theLifeAbility;	//生活技能数据
	SPRESCR_MAP			m_theSprescr;		//配方数据
	INT					m_nDefaultSkillID;	//默认的自动技能ID

	//宠物数据
//	SDATA_PET			m_thePet;			//宠物
};
