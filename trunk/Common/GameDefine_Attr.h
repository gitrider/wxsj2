#ifndef __GAMEDEFINE_ATTR_H__
#define __GAMEDEFINE_ATTR_H__
#include "GameDefine.h"
#include "GameDefine_Item.h"
//#include "DB_Struct.h"
//[[武侠世界内部名称注释]]
//角色基本属性(一级属性)
enum CHAR_ATTR_LEVEL1
{
	CATTR_LEVEL1_STR			=0, //力量 力量	[[力道]]
	CATTR_LEVEL1_CON			=1,	//体制 体制	[[体质]]
	CATTR_LEVEL1_INT			=2,	//定力 智力	[[内力]]
	CATTR_LEVEL1_DEX			=3,	//身法 敏捷	[[身法]]

	CATTR_LEVEL1_NUMBER, //角色基本属性总数
};

enum CHAR_ATTR_LEVEL
{
	CATTR_LEVEL_STR			=0, //力量 力量
	CATTR_LEVEL_CON			=1,	//体制 体制
	CATTR_LEVEL_INT			=2,	//定力 智力
	CATTR_LEVEL_DEX			=3,	//身法 敏捷
	CATTR_LEVEL_SKILL		,
	CATTR_LEVEL_NUMBER, //角色基本属性总数
};

//角色二级属性
enum CHAR_ATTR_LEVEL2
{
	//CATTR_LEVEL2_ATTACKRATE_P =0,	//物理攻击
	//CATTR_LEVEL2_DEFENCE_P,			//物理防御

	CATTR_LEVEL2_DEFENCE_NEAR =0,		//近程防御
	CATTR_LEVEL2_DEFENCE_FAR,		//远程防御
	CATTR_LEVEL2_ATTACK_NEAR,		//近程攻击
	CATTR_LEVEL2_ATTACK_FAR,		//远程攻击
	CATTR_LEVEL2_TOUGHNESS,         //韧性

	CATTR_LEVEL2_ATTACKRATE_MAGIC_NEAR,	//近程内功击力
	CATTR_LEVEL2_ATTACKRATE_MAGIC_FAR,	//近程外功击力

	CATTR_LEVEL2_DEFENCE_MAGIC_NEAR,	//近程内功防御
	CATTR_LEVEL2_DEFENCE_MAGIC_FAR,	//近程外功防御

	CATTR_LEVEL2_MAXHP,				//HP上限
	CATTR_LEVEL2_MAXMP,				//MP上限
	CATTR_LEVEL2_RESTOREHP,			//HP回复
	CATTR_LEVEL2_RESTOREMP,			//MP回复
	CATTR_LEVEL2_HIT,				//命中率
	CATTR_LEVEL2_MISS,				//闪避
	CATTR_LEVEL2_CRITRATE,			//会心率
	CATTR_LEVEL2_SPEED,				//移动速度
	CATTR_LEVEL2_ATTACKSPEED,		//攻击速度

	CATTR_LEVEL2_ATTACK_GOLD,		//金攻击
	CATTR_LEVEL2_DEFENCE_GOLD,		//金防御

	CATTR_LEVEL2_ATTACK_WOOD,		//木攻击
	CATTR_LEVEL2_DEFENCE_WOOD,		//木防御

	CATTR_LEVEL2_ATTACK_WATER,		//水攻击
	CATTR_LEVEL2_DEFENCE_WATER,		//水防御

	CATTR_LEVEL2_ATTACK_FIRE,		//火攻击
	CATTR_LEVEL2_DEFENCE_FIRE,		//火防御

	CATTR_LEVEL2_ATTACK_SOIL,		//土攻击
	CATTR_LEVEL2_DEFENCE_SOIL,		//土防御

	CATTR_LEVEL2_NUMBER,			//二级属性数量
};



//角色三级属性
enum CHAR_ATTR_LEVEL3
{
	CATTR_LEVEL3_NUQI			=0,	//怒气值
	CATTR_LEVEL3_SKILLPOINT		=1,	//连技点
	CATTR_LEVEL3_XIANJIN		=2,	//陷阱信息
	CATTR_LEVEL3_YINSHEN		=3,	//隐身信息

	CATTR_LEVEL3_NUMBER, //三级属性数量
};

//角色在数据库和ShareMem中对应的属性
enum	CHAR_ATTR_DB
{
	//////////////////////////////////////////////////////////////////////////
	//最容易变化基本数据
	CATTR_DB_HP					=	0,				//生命
	CATTR_DB_MP					,					//魔法
	CATTR_DB_PNEUMA				,					//元气
	CATTR_DB_STRIKEPOINT		,					//连击
	CATTR_DB_VIGOR				,					//活力
	CATTR_DB_MAX_VIGOR			,					//活力上限
	CATTR_DB_VIGOR_REGENE_RATE	,					//活力恢复速度
	CATTR_DB_ENERGY				,					//精力
	CATTR_DB_MAX_ENERGY			,					//精力上限
	CATTR_DB_ENERGY_REGENE_RATE	,					//精力恢复速度
	CATTR_DB_RAGE				,					//怒气
	CATTR_DB_LEVEL				,					//等级
	CATTR_DB_PKVALUE,								//杀气
	CATTR_DB_CURRENTPET,							//当前宠物GUID
	CATTR_DB_EXP,									//经验
	CATTR_DB_AT_POSITION,							//玩家位置
	CATTR_DB_BK_POSITION,							//玩家备份位置
	CATTR_DB_AT_SCENE,								//玩家场景
	CATTR_DB_BK_SCENE,								//玩家备份场景
	//////////////////////////////////////////////////////////////////////////
	//一级属性
	CATTR_DB_STR,									//力量 力量
	//CATTR_DB_SPR,									//灵气 灵力
	CATTR_DB_CON,									//体制 体制
	CATTR_DB_INT,									//定力 智力
	CATTR_DB_DEX,									//身法 敏捷
	CATTR_DB_REMAINPOINT,							//剩余点数
	CATTR_DB_REMAINSKILLPOINT,						//剩余技能点
	CATTR_DB_S_SKILLPOINT,							//系技能点
	//////////////////////////////////////////////////////////////////////////
	//坐骑相关
	CATTR_DB_HORSE_MATTING_FLAG,					//坐骑繁殖标记
	//////////////////////////////////////////////////////////////////////////
	//较少变化数据
	CATTR_DB_CAMP,									//阵营编号
	CATTR_DB_COUNTRY,								//国家
	CATTR_DB_MENPAI,								//门派编号
	CATTR_DB_GUILDID,								//帮会ID
	CATTR_DB_FAMILYID,								//家族ID
	CATTR_DB_GUILDOFFICIAL,							//帮会官职
	CATTR_DB_GUILDPOINT,							//帮会贡献度
	CATTR_DB_HONOR,							        //荣誉
	CATTR_DB_TEAMID,								//队伍号
	CATTR_DB_GUID,									//角色唯一号
	CATTR_DB_PORTRAITID,							//角色头像
	CATTR_DB_NAME,									//角色名称
	CATTR_DB_SEX,									//角色性别
	CATTR_DB_CREATETIME,							//创建日期
	CATTR_DB_TITLE,									//角色称号
	CATTR_DB_PASSWORD,								//角色二级密码
	CATTR_DB_PWDDELTIME,							//设置强制解除密码的时间
	CATTR_DB_HAIR_COLOR,							//头发颜色	
	CATTR_DB_FACE_COLOR,							//脸形颜色
	CATTR_DB_HAIR_MODEL,							//头发模型
	CATTR_DB_FACE_MODEL,							//脸形模型
	CATTR_DB_ONLINETIME,							//总在线时间
	CATTR_DB_LASTLOGINTIME,							//最后一次登入时间
	CATTR_DB_LASTLOGOUTTIME,						//最后一次登出时间
	CATTR_DB_DBVERSION,								//存档版本
	
	CATTR_DB_MONEY,									//金钱
	CATTR_DB_GOODBAD,								//善恶值
	CATTR_DB_BANK_MONEY,							//银行金钱
	CATTR_DB_BANK_OFFSIZE,							//银行end
    CATTR_DB_LEAVEGUILDTIME,						//退出帮会的时间

	//////////////////////////////////////////////////////////////////////////
	//玩家商店数据
	CATTR_DB_PLAYER_SHOP_BEGIN,						//玩家商店开始的位置
	CATTR_DB_PLAYER_SHOP_END	=					//玩家商店结束位置
	CATTR_DB_PLAYER_SHOP_BEGIN+MAX_SHOP_NUM_PER_PLAYER-1,
	CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN,			//玩家商店收藏夹起始
	CATTR_DB_PLAYER_SHOP_FAVORITE_END = 
	CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN + MAX_FAVORITE_SHOPS,//玩家商店收藏夹结束

	//////////////////////////////////////////////////////////////////////////
	//背包数据
    CATTR_DB_BAG_BEGIN,								//背包开始位置
	CATTR_DB_BAG_END			=					//背包结束位置
	CATTR_DB_BAG_BEGIN+MAX_BAG_SIZE-1,
	//////////////////////////////////////////////////////////////////////////
	//装备数据
	CATTR_DB_EQUIP_BEGIN,							//装备开始位置
	CATTR_DB_EQUIP_END		=						//装备结束位置
	CATTR_DB_EQUIP_BEGIN+HEQUIP_NUMBER-1,	
	//////////////////////////////////////////////////////////////////////////
	//银行数据
	CATTR_DB_BANK_BEGIN,							//银行开始位置
	CATTR_DB_BANK_END			=					//银行结束位置
	CATTR_DB_BANK_BEGIN+MAX_BANK_SIZE-1,	
	//////////////////////////////////////////////////////////////////////////
	//技能数据
	CATTR_DB_SKILL,									//技能
	//////////////////////////////////////////////////////////////////////////
	//CoolDown数据
	CATTR_DB_COOLDOWN,								//冷却时间
	CATTR_DB_XINFA,									//心法数据
	CATTR_DB_IMPACT,								//效果数据
	CATTR_DB_ABILITY,								//生活技能
	CATTR_DB_MISSION,								//任务数据
	CATTR_DB_SETTING,								//角色设置数据
	CATTR_DB_PET,									//宠物数据
	CATTR_DB_RELATION,								//联系人数据（好友、黑名单等）
	CATTR_DB_PRIVATEINFO,							//私人信息
	CATTR_DB_TITLEINFO,								//称号列表
	CATTR_DB_RESERVE,								//保留信息
	CATTR_DB_SOUXIA_BEGIN,							//搜侠信息开始位置
	CATTR_DB_SOUXIA_END			= 
	CATTR_DB_SOUXIA_BEGIN + MAX_SOUXIA_CONTAINER - 1,//搜侠信息结束位置
	CATTR_DB_NUMBER,
};

#define HUMAN_DB_ATTR_FLAG_LENGTH	((CATTR_DB_NUMBER>>3)+1)

//常量: 一些角色属性的最大值
enum CHAR_ATTR_CONSTANT1
{
	MAX_STRIKE_POINT = 9,
	BASE_MAX_STRIKE_POINT = 6,
	BASE_MAX_RAGE = 1000,
	MAX_RAGE = 1500,
	BASE_ATTACK_SPEED = 100,
	BASE_VISION_RANGE = 16,
	MAX_EXP_REFIX_IN_CHAR = 400,
	MAX_EXP_REFIX_IN_SYSTEM = 400,
	MAX_COOLDOWN_LIST_SIZE_FOR_HUMAN = 32,
	MAX_COOLDOWN_LIST_SIZE_FOR_PET = 8,
};
//常量: 一些角色属性的最大值
enum CHAR_ATTR_CONSTANT2
{
	MAX_EFFECTIVE_RESIST = 100,
	LL_GENERAL_ATTACK_DEFENCE = 0,
	LL_TRAIT_ATTACK = 0,
	LL_RESIST = 0,
	UL_RESIST = 100,
};
enum CHAR_ATTR_CONSTANT3
{
	LL_CRITICAL = 0,
	LL_HIT_MISS = 0,
	UL_CRITICAL = 100,
	UL_HIT_MISS = 1000,
};
//工具函数，校验属性的有效值
inline INT Attr_VerifyGeneralAttack(INT nAttr)
{
	if(LL_GENERAL_ATTACK_DEFENCE>nAttr)
	{
		nAttr = LL_GENERAL_ATTACK_DEFENCE;
	}
	return nAttr;
}

inline INT Attr_VerifyTraitAttack(INT nAttr)
{
	if(LL_TRAIT_ATTACK>nAttr)
	{
		nAttr = LL_TRAIT_ATTACK;
	}
	return nAttr;
}
inline INT Attr_VerifyDefence(INT nAttr)
{
	if(LL_GENERAL_ATTACK_DEFENCE>nAttr)
	{
		nAttr = LL_GENERAL_ATTACK_DEFENCE;
	}
	return nAttr;
}
inline INT Attr_VerifyResist(INT nAttr)
{
	if(LL_RESIST>nAttr)
	{
		nAttr = LL_RESIST;
	}
	if(UL_RESIST<nAttr)
	{
		nAttr = UL_RESIST;
	}
	return nAttr;
}

inline INT Attr_VerifyToughness(INT nAttr)
{
	return nAttr;
}

inline INT Attr_VerifyHitMiss(INT nAttr)
{
	if(LL_HIT_MISS>nAttr)
	{
		nAttr = LL_HIT_MISS;
	}
	if(UL_HIT_MISS<nAttr)
	{
		nAttr = UL_HIT_MISS;
	}
	return nAttr;
}
inline INT Attr_VerifyCritical(INT nAttr)
{
	if(LL_CRITICAL>nAttr)
	{
		nAttr = LL_CRITICAL;
	}
	if(UL_CRITICAL<nAttr)
	{
		nAttr = UL_CRITICAL;
	}
	return nAttr;
}


//常量: 连击点段的连击点数目
enum
{
	STRIKE_POINT_SEGMENT_SIZE = 3,
};
//常量: 最大连击点段数
enum
{
	MAX_STRIKE_POINT_SEGMENT = MAX_STRIKE_POINT/STRIKE_POINT_SEGMENT_SIZE,
};
//常量: 效果最大级别
enum
{
	MAX_IMPACT_LEVEL = 12,//心法level/10
};


//宠物属性相关
enum ENUM_PET_ATTR
{
	PET_ATTR_TAKELEVEL = 0,
	PET_ATTR_PETTYPE,
	PET_ATTR_FOODTYPE,
	PET_ATTR_LIFE,
	PET_ATTR_STRPERCEPTION,
	PET_ATTR_CONPERCEPTION,
	PET_ATTR_DEXPERCEPTION,
	//PET_ATTR_SPRPERCEPTION,
	PET_ATTR_INTPERCEPTION,
	PET_ATTR_GROWRATE1,
	PET_ATTR_GROWRATE2,
	PET_ATTR_GROWRATE3,
	PET_ATTR_GROWRATE4,
	PET_ATTR_GROWRATE5,

	PET_ATTR_NUM,
};
//常量: 一些宠物属性的最大值
enum PET_ATTR_CONSTANT1
{
	PET_BASE_MAX_HAPPINESS = 100,
};

// 表情状态
#define	INVALID_MOOD_STATE		(-1)	// 无效的表情状态

#endif
