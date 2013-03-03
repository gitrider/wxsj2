
/** ObjectDef.h
 */

#pragma once


#include "GameDefine.h"




// 客户端预测指令的逻辑计数加值
#define DEF_CLIENT_LOGIC_COUNT_ADD_STEP		(100)

// 角色位置校正的距离
#define DEF_CHARACTER_POS_ADJUST_DIST		(2.f)

// 客户端发角色位置较正消息的距离阀值
#define DEF_CLIENT_ADJUST_POS_WARP_DIST		(DEF_SERVER_ADJUST_POS_WARP_DIST * 0.8f)

// 进入战斗状态后要持续的时间
#define FIGHT_STATE_DURATION_TIME			(8000)



// 性别
enum ENUM_GENDER
{
	GENDER_INVALID	= -1,
	GENDER_MALE,			// 男
	GENDER_FEMALE			// 女
};


// 武器类型
enum ENUM_WEAPON_TYPE
{
	WEAPON_TYPE_INVALID	= -1,

	WEAPON_TYPE_NONE,		// 徒手
	WEAPON_TYPE_GUN,	// 棍
	WEAPON_TYPE_BANG,		// 棒
	WEAPON_TYPE_JIAN,		// 剑
	WEAPON_TYPE_DAO,		// 刀
	WEAPON_TYPE_SHANZI,		// 扇子
	WEAPON_TYPE_HULU,		// 葫芦
	WEAPON_TYPE_DIZI,		// 笛子
	WEAPON_TYPE_CI,		// 刺
	WEAPON_TYPE_ANQI,	//暗器
	WEAPON_TYPE_SHUANGCHUI,	//双锤
	WEAPON_TYPE_QING,	//琴
	WEAPON_TYPE_ZHONGJIAN,	//重剑
	WEAPON_TYPE_NPC,		// NPC，就是不可以被攻击的 Monster
	WEAPON_TYPE_TOOL,	//生活技能工具

	WEAPON_TYPE_NUMBERS,
};

/// 左手还是右手
enum ENUM_WEAPON_LOCATOR_TYPE
{
	WL_LEFT,	// 左
	WL_RIGHT,	// 右
	WL_L_ARM,	// 左手臂（盾牌用）
	WL_BOTH,	// 双手
};



#define ACTION_TYPE_NUMBERS	(10)
// 动作组
enum ENUM_BASE_ACTION
{
	BASE_ACTION_INVALID	= -1,

	BASE_ACTION_N_IDLE			=  0,			// (和平)站立		休息
	BASE_ACTION_N_IDLE_EX0		=  1,			// (和平)休闲1		休息小动作1
	BASE_ACTION_N_IDLE_EX1		=  2,			// (和平)休闲2		休息小动作2
	BASE_ACTION_N_WALK			=  3,			// (和平)行走
	BASE_ACTION_N_RUN			=  4,			// (和平)跑步

// 没用到
	BASE_ACTION_N_SIT_DOWN		=  5,			// (和平)坐下
	BASE_ACTION_N_SIT_IDLE		=  6,			// (和平)坐下休息
	BASE_ACTION_N_SIT_STAND		=  7,			// (和平)坐下起身
	BASE_ACTION_N_RESERVE_00	=  8,			// 保留
	BASE_ACTION_N_JUMP_N		=  9,			// (和平)正常跳跃中
	BASE_ACTION_N_RESERVE_01	= 10,			// 保留
	BASE_ACTION_N_RESERVE_02	= 11,			// 保留
	BASE_ACTION_N_JUMP_R		= 12,			// (和平)移动中跳跃
	BASE_ACTION_N_RESERVE_03	= 13,			// 保留

	BASE_ACTION_F_IDLE			= 14,			// (战斗)警戒		休息

// 没用到
	BASE_ACTION_F_WALK			= 15,			// (战斗)行走
	BASE_ACTION_F_RUN			= 16,			// (战斗)跑步
	BASE_ACTION_F_RESERVE_04	= 17,			// 保留
	BASE_ACTION_F_JUMP_N		= 18,			// (战斗)正常跳跃中
	BASE_ACTION_F_RESERVE_05	= 19,			// 保留
	BASE_ACTION_F_RESERVE_06	= 20,			// 保留
	BASE_ACTION_F_JUMP_R		= 21,			// (战斗)移动中跳跃
	BASE_ACTION_F_RESERVE_07	= 22,			// 保留

	BASE_ACTION_F_BE_HIT0		= 23,			// (战斗)受伤1
	BASE_ACTION_F_BE_HIT1		= 24,			// (战斗)受伤2
	BASE_ACTION_F_DIE0			= 25,			// (战斗)死亡1
	BASE_ACTION_F_DIE1			= 26,			// (战斗)死亡2
	BASE_ACTION_F_DIEFLY0		= 27,			// 死亡击飞1
	BASE_ACTION_F_DIEFLY1		= 28,			// 死亡击飞2
	BASE_ACTION_F_CADAVER0		= 29,			// (战斗)尸体1
	BASE_ACTION_F_CADAVER1		= 30,			// (战斗)尸体2
	BASE_ACTION_F_CADAVER2		= 31,			// (战斗)尸体3
	BASE_ACTION_F_CADAVER3		= 32,			// (战斗)尸体4

	BASE_ACTION_FP				= 33,			// 战斗准备
	BASE_ACTION_F_SYNCRETIZE		= 34,			// (战斗)被击融合

	//BASE_ACTION_GATHER_MINE	= 26,			// 采矿
	//BASE_ACTION_GATHER_HERBS	= 27,			// 采草药
	//BASE_ACTION_FISH			= 28,			// 钓鱼

	//BASE_ACTION_M_IDLE_0			= 29,		// (坐骑上)休息
	//BASE_ACTION_M_IDLE_EX0_0		= 30,		// (坐骑上)休息小动作1
	//BASE_ACTION_M_IDLE_EX1_0		= 31,		// (坐骑上)休息小动作2
	//BASE_ACTION_M_WALK_0			= 32,		// (坐骑上)行走
	//BASE_ACTION_M_RUN_0			= 33,		// (坐骑上)跑步
	//BASE_ACTION_M_RESERVE_08		= 34,		// 保留
	//BASE_ACTION_M_JUMP_N_0		= 35,		// (坐骑上)正常跳跃中
	//BASE_ACTION_M_RESERVE_09		= 36,		// (保留
	//BASE_ACTION_M_RESERVE_10		= 37,		// 保留
	//BASE_ACTION_M_JUMP_R_0		= 38,		// (坐骑上)移动中跳跃
	//BASE_ACTION_M_RESERVE_11		= 39,		// 保留

	//BASE_ACTION_M_IDLE_1			= 40,		// (坐骑上)休息
	//BASE_ACTION_M_IDLE_EX0_1		= 41,		// (坐骑上)休息小动作1
	//BASE_ACTION_M_IDLE_EX1_1		= 42,		// (坐骑上)休息小动作2
	//BASE_ACTION_M_WALK_1			= 43,		// (坐骑上)行走
	//BASE_ACTION_M_RUN_1			= 44,		// (坐骑上)跑步
	//BASE_ACTION_M_RESERVE_12		= 45,		// 保留
	//BASE_ACTION_M_JUMP_N_1		= 46,		// (坐骑上)正常跳跃中
	//BASE_ACTION_M_RESERVE_13		= 47,		// 保留
	//BASE_ACTION_M_RESERVE_14		= 48,		// 保留
	//BASE_ACTION_M_JUMP_R_1		= 49,		// (坐骑上)移动中跳跃
	//BASE_ACTION_M_RESERVE_15		= 50,		// 保留

// 没用到
	BASE_ACTION_ABILITY_LEVEL_UP	= 92,		// 生活技能升级
//	BASE_ACTION_XINFA_LEVEL_UP		= 93,		// 心法升级

	BASE_ACTION_LEVEL_UP			= 99,		// 升级

	BASE_ACTION_NUMBERS
};

// 动作分类
enum ENUM_ACTION_TYPE
{
	ACTION_INVALID,
	ACTION_DOWN,		// 下面
	ACTION_TOP,			// 上面
	ACTION_BODY,		// 全身
};



//================
// 绑定点

/*
enum ENUM_LOCATOR
{
	LOCATOR_INVALID	= -1,
	LOCATOR_CENTER,			// 身体中心
	LOCATOR_HEAD,			// 头部
	LOCATOR_HAND_L,			// 左手
	LOCATOR_HAND_R,			// 右手
	LOCATOR_WEAPON_L,		// 左武器
	LOCATOR_WEAPON_R,		// 右武器
	LOCATOR_FOOT_L,			// 左脚
	LOCATOR_FOOT_R,			// 右脚
	LOCATOR_FOOT_CENTER,	// 脚底中心
	LOCATOR_CALVARIA,		// 头顶偏上
	LOCATOR_HAND_L_AND_R,	// 左手和右手
	LOCATOR_HAND_CENTER,	// 两手中心

	LOCATOR_NUMBERS
};
*/

// 人物绑定点
enum ENUM_CHARACTER_LOCATOR
{
	LOCATOR_CHAR_INVALID	= -1,

	LOCATOR_CHAR_HEAD,			// 头部绑定点
	LOCATOR_CHAR_FRONT,			// 身体前置点
	LOCATOR_CHAR_ATTACK,		// 身体受击点
	LOCATOR_CHAR_CENTER,		// 身体中心点
	LOCATOR_CHAR_FOOT,			// 脚部绑定点
	LOCATOR_CHAR_HAND_L,		// 左手绑定点
	LOCATOR_CHAR_HAND_R,		// 右手绑定点
	LOCATOR_CHAR_SHOULDER_L,	// 左肩绑定点
	LOCATOR_CHAR_SHOULDER_R,	// 右肩绑定点
	LOCATOR_CHAR_SHIELD_L,		// 左臂盾牌绑定点
	
	LOCATOR_CHAR_NUMBERS
};
// return	:	几个有效的绑定点
extern LPCSTR GetCharaLocatorName( ENUM_CHARACTER_LOCATOR eLocator );


// 坐骑绑定点
enum ENUM_MOUNT_LOCATOR
{
	LOCATOR_MOUNT_INVALID	= -1,

	LOCATOR_MOUNT_LF,		// 坐骑马左前蹄绑定点
	LOCATOR_MOUNT_RF,		// 坐骑马右前蹄绑定点
	LOCATOR_MOUNT_LB,		// 坐骑马左后蹄绑定点
	LOCATOR_MOUNT_RB,		// 坐骑马右后蹄绑定点
	LOCATOR_MOUNT_FOOT,		// 坐骑马脚部绑定点
	LOCATOR_MOUNT_HEAD,		// 坐骑马头部绑定点
	LOCATOR_MOUNT_BOSOM,	// 坐骑马胸部绑定点
	LOCATOR_MOUNT_BACK,		// 坐骑马背部NOX轴绑定点

	LOCATOR_MOUNT_NUMBERS,
};
extern LPCSTR GetMountLocatorName( ENUM_MOUNT_LOCATOR eLocator );


// 武器绑定点
enum ENUM_WEAPON_LOCATOR
{
	LOCATOR_WEAPON_INVALID	= -1,

	LOCATOR_WEAPON_1	= 1,// 武器绑定点1
	LOCATOR_WEAPON_2,		// 武器绑定点2
	LOCATOR_WEAPON_3,		// 武器绑定点3
	LOCATOR_WEAPON_4,		// 武器绑定点3

	LOCATOR_WEAPON_NUMBERS,
};
 
extern LPCSTR GetWeaponLocatorName( ENUM_WEAPON_LOCATOR eLocator );




//extern const char *szIDSTRING_WEAPON;
//extern const char *szIDSTRING_CAP;
//extern const char *szIDSTRING_ARMOUR;
//extern const char *szIDSTRING_CUFF;
//extern const char *szIDSTRING_BOOT;
//extern const char *szIDSTRING_NECKLACE;
//extern const char *szIDSTRING_SASH;
//extern const char *szIDSTRING_RING;

extern const char *szIDSTRING_FACE_MESH;
extern const char *szIDSTRING_FACE_MAT;

extern const char *szIDSTRING_HAIR_MESH;
extern const char *szIDSTRING_HAIR_MAT;

extern const char *szIDSTRING_SHOULDER_MESH;
extern const char *szIDSTRING_SHOULDER_MAT;

extern const char *szIDSTRING_MAINBODY_MESH;
extern const char *szIDSTRING_MAINBODY_MAT;

extern const char *szIDSTRING_FOOT_MESH;
extern const char *szIDSTRING_FOOT_MAT;

extern const char *szIDSTRING_CAP_MESH;
extern const char *szIDSTRING_CAP_MAT;

extern const char *szIDSTRING_ARM_MESH;
extern const char *szIDSTRING_ARM_MAT;

//extern const char *szIDSTRING_CURRENT_ACTION;

extern const char *szIDSTRING_CURRENT_LEFTWEAPON;
extern const char *szIDSTRING_CURRENT_RIGHTWEAPON;

extern const char *szIDSTRING_SHIELD_RIGHTWEAPON;


// 绑定点定义

//extern const char *szIDSTRING_MOUNT_BACK_ATTACH;

//extern const char *szIDSTRING_BODY_ATTACK_ATTACH;


 