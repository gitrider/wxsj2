
#ifndef __GAMEDEFINE_ITEM_H__
#define __GAMEDEFINE_ITEM_H__
//人物装备
enum HUMAN_EQUIP
{
	HEQUIP_MAINHAND		=0,     //主手
	HEQUIP_ASSIHAND		=1,     //副手
	HEQUIP_HEAD			=2,		//头
	HEQUIP_NECK			=3,		//颈
	HEQUIP_SHOULDER		=4,		//肩
	HEQUIP_BACK			=5,		//躯干
	HEQUIP_WRIST		=6,		//腕
	HEQUIP_HAND			=7,		//手
	HEQUIP_WAIST		=8,		//腰
	HEQUIP_FEET			=9,		//脚
	HEQUIP_RING1		=10,	//戒指1
	HEQUIP_RING2		=11,	//戒指2
	HEQUIP_ADORN1		=12,	//饰品1
	HEQUIP_ADORN2		=13,	//饰品2
	HEQUIP_SUIT			=14,	//外装
	HEQUIP_RIDER		=15,	//坐骑
	HEQUIP_SOUXIA		=16,	//搜侠
	HEQUIP_NUMBER,				//主角装备点总数
};
//坐骑装备
enum HORSE_EQUIP
{
	HEQUIP_HORSE_FACE	=0,	//马面
	HEQUIP_HORSE_BODY	,	//马铠
	HEQUIP_HORSE_NECK	,	//项圈
	HEQUIP_HORSE_BACK	,	//马鞍
	HEQUIP_HORSE_HALTER	,	//缰绳
	HEQUIP_HORSE_BOOT	,	//马掌
	HEQUIP_HORSE_NUMBER ,
};
//物品属性
enum ITEM_ATTRIBUTE
{
	IATTRIBUTE_POINT_STR			,	//力道点数								0
	IATTRIBUTE_RATE_STR				,	//力道百分比							1		
	IATTRIBUTE_POINT_DEX			,	//身法点数								2
	IATTRIBUTE_RATE_DEX 			,	//身法百分比							3
	IATTRIBUTE_POINT_INT			,	//内力点数								4	

	IATTRIBUTE_RATE_INT 			,	//内力百分比							5
	IATTRIBUTE_POINT_CON			,	//体质点数								6	
	IATTRIBUTE_RATE_CON				,	//体质百分比							7
	IATTRIBUTE_POINT_ALL_BASEATT	,	//所有属性点数							8
	IATTRIBUTE_RATE_ALL_BASEATT		,	//所有属性百分比						9

	IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR,	//近程内功点数							10
	IATTRIBUTE_POINT_ATTACK_MAGIC_FAR,	//近程外功点数							12
	IATTRIBUTE_RATE_ATTACK_MAGIC_NEAR,	//近程内功百分比						11
	IATTRIBUTE_RATE_ATTACK_MAGIC_FAR,	//近程外功百分比						13

	IATTRIBUTE_POINT_ATTACK_NEAR	,	//远程内功点数		近->内				16
	IATTRIBUTE_RATE_ATTACK_NEAR		,	//远程内功百分比	近->内				17
	IATTRIBUTE_POINT_ATTACK_FAR		,	//远程外功点数							14
	IATTRIBUTE_RATE_ATTACK_FAR		,	//远程外功百分比						15

	IATTRIBUTE_POINT_DEFENCE_MAGIC_NEAR,//近程内功防御点数						18
	IATTRIBUTE_POINT_DEFENCE_MAGIC_FAR,	//近程外功防御点数						20
	IATTRIBUTE_RATE_DEFENCE_MAGIC_NEAR,	//近程内功防御百分比					19
	IATTRIBUTE_RATE_DEFENCE_MAGIC_FAR,	//近程外功防御百分比					21
	
	IATTRIBUTE_POINT_DEFENCE_NEAR	,	//远程内功防点数
	IATTRIBUTE_RATE_DEFENCE_NEAR	,	//远防百分比
	IATTRIBUTE_POINT_DEFENCE_FAR	,	//远程外功防御点数						22
	IATTRIBUTE_RATE_DEFENCE_FAR		,	//远程外功防御百分比					23

	IATTRIBUTE_POINT_MAXHP			,	//生命值上限点数						24
	IATTRIBUTE_RATE_MAXHP			,	//生命值上限百分比						25

	IATTRIBUTE_POINT_MAXMP			,	//真气值上限点数						26
	IATTRIBUTE_RATE_MAXMP			,	//真气值上限百分比						27

	IATTRIBUTE_POINT_MAX_PNEUMA		,	//元气值上限点数						28
	IATTRIBUTE_RATE_MAX_PNEUMA		,	//元气值上限百分比						29
	
	IATTRIBUTE_MAXRAGE				,	//怒气上限								31
	IATTRIBUTE_RATE_MAXRAGE			,	//怒气上限百分比						32

	IATTRIBUTE_MAX_VIGOR			,	//活力上限点数							33
	IATTRIBUTE_RATE_MAX_VIGOR		,	//活力上限百分比						34
	IATTRIBUTE_VIGOR_REGENERATE		,	//活力回复								35
	
	IATTRIBUTE_HIT					,	//命中									36
	IATTRIBUTE_MISS					,	//闪避									37
	IATTRIBUTE_RATE_HIT				,	//命中率								38
	IATTRIBUTE_CRIT					,	//暴击									39
	IATTRIBUTE_TOUGHNESS			,	//韧性									40
	IATTRIBUTE_RATE_CRIT			,	//爆击率								41

	IATTRIBUTE_GOLD_ATTACK			,	//金攻击点数							42
	IATTRIBUTE_RATE_GOLD_ATTACK		,	//金攻击百分比							43

	IATTRIBUTE_WOOD_ATTACK			,	//木攻击点数							44
	IATTRIBUTE_RATE_WOOD_ATTACK		,	//木攻击百分比							45

	IATTRIBUTE_WATER_ATTACK			,	//水攻击点数							46
	IATTRIBUTE_RATE_WATER_ATTACK	,	//水攻击百分比							47

	IATTRIBUTE_FIRE_ATTACK			,	//火攻击点数							48
	IATTRIBUTE_RATE_FIRE_ATTACK		,	//火攻击百分比							49

	IATTRIBUTE_SOIL_ATTACK			,	//土攻击点数							50
	IATTRIBUTE_RATE_SOIL_ATTACK		,	//土攻击百分比							51
	
	IATTRIBUTE_GOLD_RESIST			,	//金防御点数							52
	IATTRIBUTE_RATE_GOLD_RESIST  	,	//金防御百分比							53

	IATTRIBUTE_WOOD_RESIST			,	//木防御点数							54
	IATTRIBUTE_RATE_WOOD_RESIST		,	//木防御百分比							55

	IATTRIBUTE_WATER_RESIST			,	//水防御点数							56
	IATTRIBUTE_RATE_WATER_RESIST	,	//水防御百分比							57

	IATTRIBUTE_FIRE_RESIST			,	//火防御点数							58
	IATTRIBUTE_RATE_FIRE_RESIST		,	//火防御百分比							59

	IATTRIBUTE_SOIL_RESIST			,	//土防御点数							60
	IATTRIBUTE_RATE_SOIL_RESIST		,	//土防御百分比							61

	IATTRIBUTE_RESIST_ALL			,	//按点数抵消所有属性攻击				62
	IATTRIBUTE_RATE_RESIST_ALL		,	//按百分比抵消所有属性攻击				63

	IATTRIBUTE_SPEED_MOVE			,	//移动速度								64
	IATTRIBUTE_ATTACK_SPEED			,	//攻击速度								65
	IATTRIBUTE_COLOR_HAIR			,	//发色									66
	IATTRIBUTE_MODEL_HAIR			,	//发型									67
	IATTRIBUTE_HP_RESTORE			,	//生命回复								68
	IATTRIBUTE_MP_RESTORE			,	//真气回复								69
	IATTRIBUTE_PNEUMA_RESTORE		,	//元气回复								30
	IATTRIBUTE_CRIT_HURT			,	//增加爆击伤害							70
	IATTRIBUTE_SHIELD_ALL			,	//所有盾								71
	IATTRIBUTE_SHIELD_NEAR			,	//远程内功盾							75
	IATTRIBUTE_SHIELD_MAGIC_NEAR	,	//近程内功盾							74
	IATTRIBUTE_SHIELD_MAGIC_FAR		,	//近程外功盾							72	
	IATTRIBUTE_SHIELD_FAR			,	//远程外功盾							73
	IATTRIBUTE_SHIELD_GOLD			,	//金盾									76
	IATTRIBUTE_SHIELD_WOOD			,	//木盾									77
	IATTRIBUTE_SHIELD_WATER			,	//水盾									78
	IATTRIBUTE_SHIELD_FIRE			,	//火盾									79
	IATTRIBUTE_SHIELD_SOIL			,	//土盾									80
	IATTRIBUTE_ALLSKILL_LEVEL_INC	,	//增加所有技能等级						81
	IATTRIBUTE_EXPRENCE_GET			,	//增加经验获得							99
	IATTRIBUTE_MONEY_GET			,	//增加金钱获得							100

	IATTRIBUTE_BASE_RATE_ATTACK_NEAR	,   //基础远程内功攻击百分比			104
	IATTRIBUTE_BASE_RATE_ATTACK_FAR		,   //基础远程外功攻击百分比			102
	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR,   //基础近程内功攻击百分比			103
	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_FAR	,   //基础近程外功攻击百分比		101
	IATTRIBUTE_BASE_RATE_DEFENCE_ENEAR	,   //基础远程内功防御百分比			108
	IATTRIBUTE_BASE_RATE_DEFENCE_FAR	,   //基础远程外功防御百分比			106

	IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_NEAR	, //基础近程内功防御百分比			107
	IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_FAR,		//基础近程外功防御百分比		105

	IATTRIBUTE_BASE_ATTACK_NEAR		,	//基础远程内功攻击						112
	IATTRIBUTE_BASE_ATTACK_FAR		,	//基础远程外功攻击						110
	IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR,	//基础近程内功攻击						111
	IATTRIBUTE_BASE_ATTACK_MAGIC_FAR	,//基础近程外功攻击						109
	IATTRIBUTE_BASE_DEFENCE_NEAR	,	//基础远程内功防御						116
	IATTRIBUTE_BASE_DEFENCE_FAR		,	//基础远程外功防御						114

	IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR	,//基础近程内功防御						115
	IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR	,	//基础近程外功防御					113

	IATTRIBUTE_RATE_NEARATT_REDUCE		,	//按百分比抵消远程内功攻击			120
	IATTRIBUTE_RATE_FARATT_REDUCE		,	//按百分比抵消远程外功攻击			118
	IATTRIBUTE_RATE_MAGIC_NEAR_ATT_REDUCE,	//按百分比抵消近程内功攻击			119
	IATTRIBUTE_RATE_MAGIC_FAR_ATT_REDUCE,	//按百分比抵消近程外功攻击			117

	IATTRIBUTE_RATE_GOLD_ATT_REDUCE		,	//按百分比抵消金攻击				121
	IATTRIBUTE_RATE_WOOD_ATT_REDUCE		,	//按百分比抵消木攻击				122

	IATTRIBUTE_RATE_WATER_REDUCE		,	//按百分比抵消水攻击				123
	IATTRIBUTE_RATE_FIREATT_REDUCE		,	//按百分比抵消火攻击				124
	IATTRIBUTE_RATE_SOIL_ATT_REDUCE		,	//按百分比抵消土攻击				125
	
	IATTRIBUTE_RATE_DREAD_RESIST		,	//恐惧抗性							126
	IATTRIBUTE_RATE_COMA_RESIST			,	//昏迷抗性							127
	IATTRIBUTE_RATE_HUSH_RESIST			,	//沉默抗性							128
	IATTRIBUTE_RATE_UNARM_RESIST		,	//缴械抗性							129
	IATTRIBUTE_RATE_ATTACKSPEED_RESIST	,	//减攻速抗性						130
	IATTRIBUTE_RATE_SKILLSPEED_RESIST	,	//减施速抗性						131
	IATTRIBUTE_RATE_MOVEREDUCE_RESIST	,	//减移速抗性						132

	//门派减少伤害枚举--开始。请保证以下枚举的连贯性
	IATTRIBUTE_REDUCE_DAMAGE_SHAOLIN_GWS	,		//减少伤害--少林派[棍武僧]			82
	IATTRIBUTE_REDUCE_DAMAGE_ID_SHAOLIN_TLH	,		//减少伤害--少林派[铁罗汉]			83
	IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_GGB	,		//减少伤害--丐帮[棍丐帮]			84
	IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_ZGB	,		//减少伤害--丐帮[掌丐帮]			85
	IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_JWD	,		//减少伤害--武当派[剑武当]			86
	IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_QWD	,		//减少伤害--武当派[气武当]			87
	IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_DT	,		//减少伤害--唐门[刀唐]				88
	IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_XJT	,		//减少伤害--唐门[陷阱唐]			89
	IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_XXY	,		//减少伤害--逍遥派[扇逍遥]			90
	IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_ZXY	,		//减少伤害--逍遥派[指逍遥]			91
	IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_KYW,		//减少伤害--药王谷[狂药王]			92
	IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_YYW,		//减少伤害--药王谷[医药王]			93
	IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_DTH,		//减少伤害--桃花岛[笛桃花]			94
	IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_YTH,		//减少伤害--桃花岛[音桃花]			95
	IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_CEM,			//减少伤害--峨嵋派[刺峨眉]			96
	IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_FEM,			//减少伤害--峨嵋派[辅峨眉]			97
	IATTRIBUTE_REDUCE_DAMAGE_ID_WUMENPAI,			//减少伤害--无门派，如新手。。等	98
	//门派减少伤害枚举--结束。
	
	IATTRIBUTE_ID_IMPACT				=500,	//影响impact的ID
	IATTRIBUTE_NUMBER, //物品属性类型总数
};

enum EQUIP_PAI
{
	EQUIPPAI_ANY		=	1	,//任意牌
	EQUIPPAI_ANYFENG	=	2	,//任意风牌
	EQUIPPAI_ANYJIAN	=	3	,//任意箭牌
	EQUIPPAI_ANYZI		=	4	,//任意字牌
	EQUIPPAI_ANYFEIZI	=	5	,//任意非字牌
	EQUIPPAI_ANYTIAO	=	6	,//任意条牌
	EQUIPPAI_ANYTONG	=	7	,//任意筒牌
	EQUIPPAI_ANYWAN		=	8	,//任意万牌
	EQUIPPAI_ANYSHUN	=	9	,//任意顺
	EQUIPPAI_ANYKE		=	10	,//任意刻
	EQUIPPAI_ANYJIANG	=	11	,//任意将
	EQUIPPAI_DONG		=	15	,//东
	EQUIPPAI_NAN		=	16	,//南
	EQUIPPAI_XI			=	17	,//西
	EQUIPPAI_BEI		=	18	,//北
	EQUIPPAI_ZHONG		=	19	,//中
	EQUIPPAI_FA			=	20	,//发
	EQUIPPAI_BAI		=	21	,//白
	EQUIPPAI_1TIAO		=	31	,//1条
	EQUIPPAI_2TIAO		=	32	,//2条
	EQUIPPAI_3TIAO		=	33	,//3条
	EQUIPPAI_4TIAO		=	34	,//4条
	EQUIPPAI_5TIAO		=	35	,//5条
	EQUIPPAI_6TIAO		=	36	,//6条
	EQUIPPAI_7TIAO		=	37	,//7条
	EQUIPPAI_8TIAO		=	38	,//8条
	EQUIPPAI_9TIAO		=	39	,//9条
	EQUIPPAI_1TONG		=	41	,//1筒
	EQUIPPAI_2TONG		=	42	,//2筒
	EQUIPPAI_3TONG		=	43	,//3筒
	EQUIPPAI_4TONG		=	44	,//4筒
	EQUIPPAI_5TONG		=	45	,//5筒
	EQUIPPAI_6TONG		=	46	,//6筒
	EQUIPPAI_7TONG		=	47	,//7筒
	EQUIPPAI_8TONG		=	48	,//8筒
	EQUIPPAI_9TONG		=	49	,//9筒
	EQUIPPAI_1WAN		=	51	,//1万
	EQUIPPAI_2WAN		=	52	,//2万
	EQUIPPAI_3WAN		=	53	,//3万
	EQUIPPAI_4WAN		=	54	,//4万
	EQUIPPAI_5WAN		=	55	,//5万
	EQUIPPAI_6WAN		=	56	,//6万
	EQUIPPAI_7WAN		=	57	,//7万
	EQUIPPAI_8WAN		=	58	,//8万
	EQUIPPAI_9WAN		=	59	,//9万
	EQUIP_PAI_NUM,
};

enum	INDEX_PAITYPE
{
	INDEX_ANYSHUN	=	0	,//任意顺
	INDEX_ANYKE		=	1	,//任意刻
	INDEX_ANYJIANG	=	2	,//任意将
	INDEX_NOANYPAINUM,
};

enum    EM_EQUIP_CARD_GROUP
{
	//饰品 1 饰品2  腰					 0
	//帽  项链 护腕                      1
	//戒指1 戒指2  鞋子					 2		
	//肩 衣服 手套						 3
	// 主手 副手						 4
	 EM_EQUIP_CARD_GROUP_INVAILD    = -1,
     EM_EQUIP_CARD_GROUP_CHAIN1		= 0,
	 EM_EQUIP_CARD_GROUP_CHAIN2	,	 
	 EM_EQUIP_CARD_GROUP_CHAIN3	, 
	 EM_EQUIP_CARD_GROUP_CHAIN4	,		 
	 EM_EQUIP_CARD_GROUP_JIANG	,		 
	 EM_EQUIP_CARD_GROUP_MAXNUM,
};

//物品类型
enum ITEM_CLASS
{
	ICLASS_EQUIP		=10,	//装备
	ICLASS_MATERIAL		,	//原料
	ICLASS_COMITEM		,	//可使用物品
	ICLASS_TASKITEM		,	//任务物品
	ICLASS_GEM			,   //宝石
	ICLASS_STOREMAP		,	//藏宝图
	ICLASS_TALISMAN		,	//法宝---???
	ICLASS_GUILDITEM	,	//帮会物品
	ICLASS_IDENT		,	//鉴定券轴
	ICLASS_EXTRABAG		,	//扩展背包
	ICLASS_SOUL_BEAD	,	//魂珠类型
	ICLASS_MOUNT        ,   //坐骑             [7/8/2010 陈军龙]
	ICLASS_NUMBER       ,   //物品的类别数量
};

enum ITEM_CREATE_TYPE
{
	ITEM_IB			=0,
	ITEM_DROP,
	ITEM_TASK,
	ITEM_CREATE,
	ITEM_NUM,
};
//装备分类
enum EQUIP_TYPE
{
	EQUIP_GUN			=1,	//棍
	EQUIP_CHANSHOU		,	//缠手
	EQUIP_BANG			,	//棒
	EQUIP_CHANGTAO		,	//掌套
	EQUIP_JIAN			,	//剑
	EQUIP_FUZHOU		,	//符咒
	EQUIP_DADAO			,	//刀
	EQUIP_ANQI			,	//暗器
	EQUIP_SHANZI		,	//扇子
	EQUIP_JINSHA		,//金沙
	EQUIP_HULU			,//葫芦
	EQUIP_YAOFEN		,//药粉
	EQUIP_DIZI			,//笛子
	EQUIP_BANZHI		,//扳指
	EQUIP_CI			,//刺
	EQUIP_ZHEN			,//针
	
	EQUIP_RESERVE1		,//预留1
	EQUIP_RESERVE2		,//预留2
	EQUIP_RESERVE3		,//预留3

	EQUIP_CAP			=20,//头盔
	EQUIP_NECKLACE		,//项链
	EQUIP_SHOULDER		,//衬肩
	EQUIP_ARMOR			,//衣服
	EQUIP_CUFF			,//护腕
	EQUIP_HAND			,//手套
	EQUIP_SASH			,//腰带
	EQUIP_BOOT			,//鞋子
	EQUIP_RING			,//戒指
	EQUIP_SOUXIA		,//预留1改成搜侠by gh 2010/05/07
	EQUIP_RESERVE5		,//预留2
	EQUIP_RESERVE6		,//预留3
	EQUIP_ADORN		    ,//饰品
	EQUIP_SUIT			,//外装
	EQUIP_TOOL			,//工具
	EQUIP_TYPE_NUM,
};
//鉴定卷轴类型
enum IDENT_ITEM_TYPE
{
	IDENT_ITEM_TYPE    =0,
	IDENT_WPIDENT	   =6,	// 武器鉴定卷轴
	IDENT_ARIDENT	   =7,	// 防具鉴定卷轴
	IDENT_NCIDENT	   =8,	// 项链鉴定卷轴
};
//宝石类型
enum GEM_TYPE
{
	GEM_COLORFUL		=0,	//多彩宝石
	GEM_RED				,	//红宝石
	GEM_GREEN			,	//绿宝石
	GEM_YELLOW			,	//黄宝石
	GEM_BLUE			,	//蓝宝石
	GEM_TYPE_NUM		,
};

enum MATERIAL_TYPE
{

};

enum SOUL_BEAD_TYPE
{
	SOUL_BEAD_WU		=1,	//武魂珠
	SOUL_BEAD_QI		,	//器魂珠
	SOUL_BEAD_TYPE_NUM	,
};
//--------------------------------------------------------------
// ICLASS_COMITEM
enum COMMON_ITEM_TYPE
{
	COMMON_ITEM_TYPE    =0,
	COMMON_ITEM_HP,			// 红药
	COMMON_ITEM_MP,			// 蓝药
	COMMON_ITEM_3,			// 暂时未定义
	COMITEM_PRESCR		=4, // 配方
	COMMON_HORSE_ITEM	=5, // 马物品
	COMITEM_WPIDENT		=6,	// 武器鉴定卷轴
	COMITEM_ARIDENT		=7,	// 防具鉴定卷轴
	COMITEM_NCIDENT		=8,	// 项链鉴定卷轴
	COMITEM_TREASUREBOX = 9,		// 宝箱
	COMITEM_TREASUREBOX_KEY = 10,	// 宝箱钥匙
	COMITEM_SOUXIA			= 11,	//搜侠物品
	COMITEM_SOUXIA_RELIC	= 12,	//搜侠物品残片
	COMITEM_EQUIP_REFINING	= 13,	//炼器神符
	COMITEM_EQUIP_CANCEL_MAGIC=14,	//退魔石
};

enum ENUM_UPDATE_EQIPMENT
{
	UPDATE_EQIPMENT_WEAPON_ID	= 0,	// 武器
	UPDATE_EQIPMENT_CAP_ID,				// 帽子
	UPDATE_EQIPMENT_ARMOUR_ID,			// 衣服
	UPDATE_EQIPMENT_WRIST_ID,			// 护腕
	UPDATE_EQIPMENT_FOOT_ID,			// 靴子
	UPDATE_EQIPMENT_NUMBERS
};

enum OBJITEM_TYPE
{
	ITYPE_DROPBOX	=	-1,
	ITYPE_GROWPOINT,
	ITYPE_MONSTERDROP_OBJ_ITEM ,
	ITYPE_PERSONDROP_OBJ_ITEM ,
	ITYPE_OBJ_ITEM,
};

enum	DURSPOIL_RULER
{
	DURSPOIL_NO		 = -1,  //无损耗
	DURSPOIL_ATTACT  = 1,   //攻击损耗
	DURSPOIL_DEFENCE = 2,   //受击损耗
	DURSPOIL_NUMBER	 ,
};


//拾取规则
enum	PICK_RULER
{
	IPR_FREE_PICK,	//自由拾取
	IPR_BET_PICK,	//投塞子拾取
	IPR_TURN_PICK,	//轮流拾取

};

//分配规则
enum	BOX_DISTRIBUTE_RULER
{
	BDR_COMMON	=	 0,
	BDR_BOSS	=	 1,
	BDR_BLUE	=    2,
	BDR_GOLD	=    3,
	BDR_UNKNOW	=	 0xFFFF
};

//物品分配规则
enum	ITEM_DISTRIBUTE_RULER
{
	ITEMDR_RANDOM,
	ITEMDR_ALLGET
};

// 坐骑物品类型
enum	HORSE_ITEM_TYPE
{
	HORSE_ITEM_INVALID	= -1,

	HORSE_ITEM_SAVVY,			// 悟性道具
	HORSE_ITEM_BASIC,			// 根骨
	HORSE_ITEM_HAPPINESS,		// 快乐度
	HORSE_ITEM_SKILL_GIRD,		// 技能格
	HORSE_ITEM_SKILL_FORGET,	// 技能遗忘
	HORSE_ITEM_SKILL_APPER,		// 技能领悟，对应前6格的技能
	HORSE_ITEM_SKILL_STUDY,		// 技能学习，对应后6格的技能
	HORSE_ITEM_CHILD,			// 还童

	HORSE_ITEM_NUM    
};

//ItemContainer 类型
enum	ITEM_CONTAINER_TYPE
{
	ICT_UNKNOW_CONTAINER,
	ICT_BASE_CONTAINER,
	ICT_BAG_CONTAINER,
	ICT_BASEBAG_CONTAINER,
	ICT_EXTRABAG_CONTAINER,
	ICT_EXTRA_CONTAINER,
	ICT_TASK_CONTAINER,
	ICT_MAT_CONTAINER,
	ICT_EQUIP_CONTAINER,
	ICT_BANK_CONTAINER,
	ICT_ITEMBOX_CONTAINER,
	ICT_EXCHANGEBOX_CONTAINER,
	ICT_HUMAN_PET_CONTAINER,
	ICT_MOUNT_CONTAINER,       // [7/8/2010 陈军龙]
};

// 目标类型
enum ENUM_ITEM_TARGET_TYPE
{
	ITEM_TARGET_TYPE_INVALID	= -1,
	ITEM_TARGET_TYPE_NONE,				// 无需目标		:	无
	ITEM_TARGET_TYPE_POS,				// 位置			:	TargetPos
	ITEM_TARGET_TYPE_DIR,				// 方向			:	TargetDir
	ITEM_TARGET_TYPE_ITEM,				// 道具			:	TargetItemIndex
	ITEM_TARGET_TYPE_SELF,				// 自已			:	TargetObj
	ITEM_TARGET_TYPE_SELF_PET,			// 自已的宠物	:	TargetObj,TargetPetGUID
	ITEM_TARGET_TYPE_FRIEND,			// 友好目标		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_PLAYER,		// 友好玩家		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_MONSTER,	// 友好怪物		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_PET,		// 友好宠物		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY,				// 敌对目标		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_PLAYER,		// 敌对玩家		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_MONSTER,		// 敌对怪物		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_PET,			// 敌对宠物		:	TargetObj
	ITEM_TARGET_TYPE_ALL_CHARACTER,		// 所有角色		:	TargetObj

	ITEM_TARGET_TYPE_NUMBERS
};

// 道具分类编号
#define ITEM_PET_SKILL_STUDY_BEGIN		30402000	//宠物技能书最小编号
#define ITEM_PET_SKILL_STUDY_END		30403000	//宠物技能书最大编号

#define ITEM_PET_RETURN_BABAY_BEGIN		30503011	//宠物还童丹最小编号（注意30503011本身也是合法的还童丹编号）
#define ITEM_PET_RETURN_BABAY_END		30503020	//宠物还童丹最大编号（注意30503020本身也是合法的还童丹编号）

#define	ITEM_DOME_MEDICINE				30601000	//驯养道具 
#define	ITEM_MEAT_MEDICINE				30602000	//肉食宠粮 
#define	ITEM_GRASS_MEDICINE				30603000	//草类宠粮 
#define	ITEM_WORM_MEDICINE				30604000	//虫类宠粮 
#define	ITEM_PADDY_MEDICINE				30605000	//谷类宠粮
#define ITEM_RELIC_SOUXIA				12120001    //捜侠录残页道具add by gh
#define ITEM_WU_SOUL_BEAD				20010001	//武魂珠
#define ITEM_QI_SOUL_BEAD				20020001    //器魂珠
#define ITEM_PET_FEED_MEDICINE_MAX		ITEM_PADDY_MEDICINE+1000

#define ITEM_PET_ADD_LIFE_BEGIN			ITEM_PET_FEED_MEDICINE_MAX	//宠物延长寿命最小编号
#define ITEM_PET_ADD_LIFE_END			30607000	//宠物延长寿命最大编号

//物品信息位定义
enum ITEM_EXT_INFO
{
	IEI_BIND_INFO		=	0x00000001,	//绑定信息
	IEI_IDEN_INFO		=	0x00000002, //鉴定信息
	IEI_PLOCK_INFO		=	0x00000004, //二级密码已经处理
	IEI_BLUE_ATTR		=	0x00000008, //是否有蓝属性
	IEL_CREATOR			=	0x00000010,	//是否有创造者
};


#endif
