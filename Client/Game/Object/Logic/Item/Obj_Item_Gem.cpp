
#include "StdAfx.h"
#include "Obj_Item_Gem.h"
#include "GIException.h"
#include "Procedure/GameProcedure.h"


//IATTRIBUTE_POINT_STR			,	//力量点数
//IATTRIBUTE_RATE_STR				,	//力量百分比
//IATTRIBUTE_POINT_DEX			,	//敏捷点数
//IATTRIBUTE_RATE_DEX 			,	//敏捷百分比
//IATTRIBUTE_POINT_INT			,	//智力点数
//IATTRIBUTE_RATE_INT 			,	//智力百分比
//IATTRIBUTE_POINT_CON			,	//体质点数
//IATTRIBUTE_RATE_CON				,	//体质百分比
//IATTRIBUTE_POINT_ALL_BASEATT	,	//所有属性点数
//IATTRIBUTE_RATE_ALL_BASEATT		,	//所有属性百分比
//IATTRIBUTE_POINT_ATTACK_M		,	//魔攻点数
//IATTRIBUTE_RATE_ATTACK_M		,	//魔攻百分比
//IATTRIBUTE_POINT_ATTACK_NEAR	,	//近攻点数
//IATTRIBUTE_RATE_ATTACK_NEAR		,	//近攻百分比
//IATTRIBUTE_POINT_ATTACK_FAR		,	//远攻点数
//IATTRIBUTE_RATE_ATTACK_FAR		,	//远攻百分比
//IATTRIBUTE_POINT_DEFENCE_M		,	//魔防点数
//IATTRIBUTE_RATE_DEFENCE_M		,	//魔防百分比
//IATTRIBUTE_POINT_DEFENCE_NEAR	,	//近防点数
//IATTRIBUTE_RATE_DEFENCE_NEAR	,	//近防百分比
//IATTRIBUTE_POINT_DEFENCE_FAR	,	//远防点数
//IATTRIBUTE_RATE_DEFENCE_FAR		,	//远防百分比
//IATTRIBUTE_POINT_MAXHP			,	//生命值上限点数
//IATTRIBUTE_RATE_MAXHP			,	//生命值上限百分比
//IATTRIBUTE_POINT_MAXMP			,	//魔法值上限点数
//IATTRIBUTE_RATE_MAXMP			,	//魔法值上限百分比
//IATTRIBUTE_WUSHI_MAXRAGE		,	//武士怒气上限
//IATTRIBUTE_JIANXIA_MAXRAGE		,	//剑侠怒气上限
//IATTRIBUTE_QISHE_MAXRAGE		,	//骑射手怒气上限
//IATTRIBUTE_HUOQIANG_MAXRAGE		,	//火枪手怒气上限
//IATTRIBUTE_XIANZHI_MAXRAGE		,	//先知怒气上限
//IATTRIBUTE_SAMAN_MAXRAGE		,	//萨满怒气上限



//IATTRIBUTE_RATE_WUSHI_MAXRAGE	,	//武士怒气上限百分比
//IATTRIBUTE_RATE_JIANXIA_MAXRAGE	,	//剑侠怒气上限百分比
//IATTRIBUTE_RATE_QISHE_MAXRAGE	,	//骑射手怒气上限百分比
//IATTRIBUTE_RATE_HUOQIANG_MAXRAGE,	//火枪手怒气上限百分比
//IATTRIBUTE_RATE_XIANZHI_MAXRAGE	,	//先知怒气上限百分比
//IATTRIBUTE_RATE_SAMAN_MAXRAGE	,	//萨满怒气上限百分比
//IATTRIBUTE_POINT_WUSHI_RAGE_RESTORE	,	//武士怒气回复点
//IATTRIBUTE_POINT_JIANXIA_RAGE_RESTORE	,	//剑侠怒气回复点
//IATTRIBUTE_POINT_QISHE_RAGE_RESTORE	,	//骑射手怒气回复点
//IATTRIBUTE_POINT_HUOQIANG_RAGE_RESTORE,	//火枪手怒气回复点
//IATTRIBUTE_POINT_XIANZHI_RAGE_RESTORE	,	//先知怒气回复点
//IATTRIBUTE_POINT_SAMAN_RAGE_RESTORE	,	//萨满怒气回复点
//IATTRIBUTE_RATE_WUSHI_RAGE_RESTORE	,	//武士怒气回复百分比
//IATTRIBUTE_RATE_JIANXIA_RAGE_RESTORE	,	//剑侠怒气回复百分比
//IATTRIBUTE_RATE_QISHE_RAGE_RESTORE	,	//骑射手怒气回复百分比
//IATTRIBUTE_RATE_HUOQIANG_RAGE_RESTORE,	//火枪手怒气回复百分比
//IATTRIBUTE_RATE_XIANZHI_RAGE_RESTORE	,	//先知怒气回复百分比
//IATTRIBUTE_RATE_SAMAN_RAGE_RESTORE	,	//萨满怒气回复百分比




//IATTRIBUTE_HIT					,	//命中
//IATTRIBUTE_MISS					,	//闪避
//IATTRIBUTE_RATE_HIT				,	//命中率
//IATTRIBUTE_CRIT					,	//暴击
//IATTRIBUTE_TOUGHNESS			,	//韧性
//IATTRIBUTE_RATE_CRIT			,	//爆击率
//IATTRIBUTE_COLD_ATTACK			,	//冰攻击点数
//IATTRIBUTE_RATE_COLD_ATTACK		,	//冰攻击百分比
//IATTRIBUTE_FIRE_ATTACK			,	//火攻击点数
//IATTRIBUTE_RATE_FIRE_ATTACK		,	//火攻击百分比
//IATTRIBUTE_LIGHT_ATTACK			,	//电攻击点数
//IATTRIBUTE_RATE_LIGHT_ATTACK	,	//电攻击百分比
//IATTRIBUTE_POISON_ATTACK		,	//毒攻击点数
//IATTRIBUTE_RATE_POISON_ATTACK	,	//毒攻击百分比
//IATTRIBUTE_COLD_RESIST			,	//冰防御点数
//IATTRIBUTE_RATE_COLD_RESIST  	,	//冰防御百分比
//IATTRIBUTE_FIRE_RESIST			,	//火防御点数
//IATTRIBUTE_RATE_FIRE_RESIST		,	//火防御百分比
//IATTRIBUTE_LIGHT_RESIST			,	//电防御点数
//IATTRIBUTE_RATE_LIGHT_RESIST	,	//电防御百分比
//IATTRIBUTE_POISON_RESIST		,	//毒防御点数
//IATTRIBUTE_RATE_POISON_RESIST	,	//毒防御百分比
//IATTRIBUTE_RESIST_ALL			,	//按点数抵消所有属性攻击
//IATTRIBUTE_RATE_RESIST_ALL		,	//按百分比抵消所有属性攻击
//IATTRIBUTE_RATE_DREAD			,	//恐惧几率
//IATTRIBUTE_TIME_DREAD			,	//恐惧时间
//IATTRIBUTE_RATE_COMA			,	//昏迷几率
//IATTRIBUTE_TIME_COMA			,	//昏迷时间
//IATTRIBUTE_RATE_HUSH			,	//沉默几率
//IATTRIBUTE_TIME_HUSH			,	//沉默时间
//IATTRIBUTE_RATE_UNARM			,	//缴械几率
//IATTRIBUTE_TIME_UNARM			,	//缴械时间
//IATTRIBUTE_ROLL_RESIST			,	//抵抗碾压
//IATTRIBUTE_RATE_ATTACK_SPEED	,	//减攻击速度几率
//IATTRIBUTE_RATE_SKILL_SPEED		,	//减施法速度几率
//IATTRIBUTE_RATE_MOVE_REDUCE		,	//减移动速度几率
//IATTRIBUTE_HURT_DOT				,	//dot伤害
//IATTRIBUTE_DEBUFF_RESIST		,	//抵抗debuff
//IATTRIBUTE_SPEED_MOVE			,	//移动速度
//IATTRIBUTE_SPEED_SKILL			,	//施法速度
//IATTRIBUTE_ATTACK_SPEED			,	//攻击速度
//IATTRIBUTE_MODEL				,	//模型
//IATTRIBUTE_COLOR_HAIR			,	//发色
//IATTRIBUTE_MODEL_HAIR			,	//发型
//IATTRIBUTE_HP_RESTORE			,	//生命回复
//IATTRIBUTE_MP_RESTORE			,	//魔法回复
//IATTRIBUTE_RAGE_RESTORE			,	//怒气回复
//IATTRIBUTE_CRIT_HURT			,	//增加爆击伤害
//IATTRIBUTE_SHIELD_ALL			,	//所有盾
//IATTRIBUTE_SHIELD_NEAR			,	//近程盾
//IATTRIBUTE_SHIELD_M				,	//魔法盾
//IATTRIBUTE_SHIELD_FAR			,	//远程盾
//IATTRIBUTE_SHIELD_COLD			,	//冰盾
//IATTRIBUTE_SHIELD_FIRE			,	//火盾
//IATTRIBUTE_SHIELD_LIGHT			,	//电盾
//IATTRIBUTE_SHIELD_POISON		,	//毒盾
//IATTRIBUTE_SHIELD_MP_REDUCE		,	//内力抵消伤害盾
//IATTRIBUTE_SKILL_LEVEL_INC		,	//增加单个技能等级
//IATTRIBUTE_ALLSKILL_LEVEL_INC	,	//增加所有技能等级
//IATTRIBUTE_WUSHI_HURT_RESIST	,	//抵抗武士职业伤害
//IATTRIBUTE_JIANXIA_HURT_RESIST	,	//抵抗剑侠职业伤害
//IATTRIBUTE_QISHE_HURT_RESIST	,	//抵抗骑射手职业伤害
//IATTRIBUTE_HUOQIANG_HURT_RESIST	,	//抵抗火枪手职业伤害
//IATTRIBUTE_XIANZHI_HURT_RESIST	,	//抵抗先知职业伤害
//IATTRIBUTE_SAMAN_HURT_RESIST	,	//抵抗萨满职业伤害
//IATTRIBUTE_EXPRENCE_GET			,	//增加经验获得
//IATTRIBUTE_MONEY_GET			,	//增加金钱获得
//IATTRIBUTE_POINT_ABILITY1		,	//增加通用点获得1点数
//IATTRIBUTE_RATE_ABILITY1		,	//增加通用点获得1百分比
//IATTRIBUTE_POINT_ABILITY2		,	//增加通用点获得2点数
//IATTRIBUTE_RATE_ABILITY2		,	//增加通用点获得2百分比
//IATTRIBUTE_POINT_ABILITY3		,	//增加通用点获得3点数
//IATTRIBUTE_RATE_ABILITY3		,	//增加通用点获得3百分比
//IATTRIBUTE_POINT_ABILITY4		,	//增加通用点获得4点数
//IATTRIBUTE_RATE_ABILITY4		,	//增加通用点获得4百分比
//IATTRIBUTE_POINT_ABILITY5		,	//增加通用点获得5点数
//IATTRIBUTE_RATE_ABILITY5		,	//增加通用点获得5百分比
//IATTRIBUTE_POINT_ABILITY6		,	//增加通用点获得6点数
//IATTRIBUTE_RATE_ABILITY6		,	//增加通用点获得6百分比
//IATTRIBUTE_POINT_ABILITY7		,	//增加通用点获得7点数
//IATTRIBUTE_RATE_ABILITY7		,	//增加通用点获得7百分比
//IATTRIBUTE_POINT_ABILITY8		,	//增加通用点获得8点数
//IATTRIBUTE_RATE_ABILITY8		,	//增加通用点获得8百分比
//IATTRIBUTE_POINT_ABILITY9		,	//增加通用点获得9点数
//IATTRIBUTE_RATE_ABILITY9		,	//增加通用点获得9百分比
//IATTRIBUTE_POINT_ABILITY10		,	//增加通用点获得10点数
//IATTRIBUTE_RATE_ABILITY10		,	//增加通用点获得10百分比
//
//
//IATTRIBUTE_BASE_RATE_ATTACK_ENEAR	,   //基础近程攻击百分比
//IATTRIBUTE_BASE_RATE_ATTACK_FAR		,   //基础远程攻击百分比
//IATTRIBUTE_BASE_RATE_ATTACK_M		,   //基础魔法攻击百分比
//IATTRIBUTE_BASE_RATE_DEFENCE_ENEAR	,   //基础近程防御百分比
//IATTRIBUTE_BASE_RATE_DEFENCE_FAR	,   //基础远程防御百分比
//IATTRIBUTE_BASE_RATE_DEFENCE_M		,   //基础魔法防御百分比
//
//IATTRIBUTE_BASE_ATTACK_NEAR		,	//基础近程攻击
//IATTRIBUTE_BASE_ATTACK_FAR		,	//基础远程攻击
//IATTRIBUTE_BASE_ATTACK_M		,	//基础魔法攻击
//IATTRIBUTE_BASE_DEFENCE_NEAR	,	//基础近程防御
//IATTRIBUTE_BASE_DEFENCE_FAR		,	//基础远程防御
//IATTRIBUTE_BASE_DEFENCE_M		,	//基础魔法防御

//
//IATTRIBUTE_RATE_NEARATT_REDUCE		,	//按百分比抵消近程攻击
//IATTRIBUTE_RATE_FARATT_REDUCE		,	//按百分比抵消远程攻击
//IATTRIBUTE_RATE_MATT_REDUCE			,	//按百分比抵消魔法攻击
//IATTRIBUTE_RATE_COLDATT_REDUCE		,	//按百分比抵消冰攻击
//IATTRIBUTE_RATE_FIREATT_REDUCE		,	//按百分比抵消火攻击
//IATTRIBUTE_RATE_LIGHTATT_REDUCE		,	//按百分比抵消电攻击
//IATTRIBUTE_RATE_POISON_REDUCE		,	//按百分比抵消毒攻击


const char* g_szGemAttName[] = 
{

	"equip_attr_pointstr",												//力量百分比																					
		"equip_attr_ratestr",			        							//力量点数
		"equip_attr_pointdex",		     									//敏捷点数
		"equip_attr_ratedex",			     								//敏捷百分比
		"equip_attr_pointint",			    								//智力点数
		"equip_attr_rateint",		                    					//智力百分比
		"equip_attr_pointcon",                          					//体质点数
		"equip_attr_ratecon",	 											//体质百分比
		"equip_attr_allbasepoint",	        								//所有属性点数
		"equip_attr_allbaserate",											//所有属性百分比
		"equip_attr_pointattack",	     									//魔攻点数
		"equip_attr_rateattack",	    									//魔攻百分比
		"equip_attr_attack_nearpoint",										//近攻点数
		"equip_attr_attack_nearrate",										//近攻百分比
		"equip_attr_attack_farpoint",										//远攻点数
		"equip_attr_attack_farrate",	     								//远攻百分比
		"equip_attr_defence_mpoint",	 									//魔防点数
		"equip_attr_defence_mrate",	 										//魔防百分比
		"equip_attr_defence_nearpoint",										//近防点数
		"equip_attr_defence_nearrate",										//近防百分比
		"equip_attr_defence_farpoint",										//远防点数
		"equip_attr_defence_farrate",										//远防百分比
		"equip_attr_point_maxhp",											//生命值上限点数
		"equip_attr_rate_maxhp",											//生命值上限百分比
		"equip_attr_point_maxmp",											//魔法值上限点数
		"equip_attr_rate_maxmp",											//魔法值上限百分比

		"equip_attr_wushi_maxrage",											//武士怒气上限
		"equip_attr_jianxia_maxrage",										//剑侠怒气上限
		"equip_attr_qishe_maxrage",											//骑射手怒气上限
		"equip_attr_huoqiang_maxrage",										//火枪手怒气上限
		"equip_attr_xianzhi_maxrage",										//先知怒气上限
		"equip_attr_saman_maxrage",											//萨满怒气上限

		"equip_attr_wushi_maxrage_rate",									//武士怒气上限百分比
		"equip_attr_jianxia_maxrage_rate",									//剑侠怒气上限百分比
		"equip_attr_qishe_maxrage_rate",									//骑射手怒气上限百分比
		"equip_attr_huoqiang_maxrage_rate",									//火枪手怒气上限百分比
		"equip_attr_xianzhi_maxrage_rate",									//先知怒气上限百分比
		"equip_attr_saman_maxrage_rate",									//萨满怒气上限百分比

		"equip_attr_wushi_rageres_point",									//武士怒气回复点
		"equip_attr_jianxia_rageres_point",									//剑侠怒气回复点
		"equip_attr_qishe_rageres_point",									//骑射手怒气回复点
		"equip_attr_huoqiang_rageres_pointe",								//火枪手怒气回复点
		"equip_attr_xianzhi_rageres_point",									//先知怒气回复点
		"equip_attr_saman_rageres_point",									//萨满怒气回复点


		"equip_attr_wushi_rageres_rate",									//武士怒气回复百分比
		"equip_attr_jianxia_rageres_rate",									//剑侠怒气回复百分比
		"equip_attr_qishe_rageres_rate",									//骑射手怒气回复百分比
		"equip_attr_huoqiang_rageres_rate",									//火枪手怒气回复百分比
		"equip_attr_xianzhi_rageres_rate",									//先知怒气回复百分比
		"equip_attr_saman_rageres_rate",									//萨满怒气回复百分比

		"equip_attr_hit",													//命中
		"equip_attr_miss",													//闪避
		"equip_attr_rate_hit",												//命中率
		"equip_attr_sock",			     									//暴击
		"equip_attr_toughness",			                					//韧性
		"equip_attr_sock_rate",												//爆击率
		"equip_attr_cold_attack",											//冰攻击点数
		"equip_attr_rate_cold_attack",										//冰攻击百分比
		"equip_attr_fire_attack",											//火攻击点数
		"equip_attr_rate_fire_attack",										//火攻击百分比
		"equip_attr_light_attack",			 								//电攻击点数
		"equip_attr_rate_light_attack",				 						//电攻击百分比
		"equip_attr_poison_attack",											//毒攻击点数
		"equip_attr_rate_poison_attack",									//毒攻击百分比
		"equip_attr_cold_resist",											//冰防御点数
		"equip_rate_old_resist",											//冰防御百分比
		"equip_attr_fire_resist",											//火防御点数
		"equip_attr_rate_fire_resist",										//火防御百分比
		"equip_attr_light_resist",											//电防御点数
		"equip_rate_light_resist",											//电防御百分比
		"equip_poison_resist",												//毒防御点数
		"equip_rate_poison_resist",											//毒防御百分比
		"equip_resist_all",													//按点数抵消所有属性攻击
		"equip_rate_resist_all",											//按百分比抵消所有属性攻

		"equip_base_dread_rate",											//恐惧几率
		"equip_time_dread",													//恐惧时间
		"equip_rate_coma",													//昏迷几率
		"equip_time_coma",													//昏迷时间
		"equip_rate_hush",													//沉默几率
		"equip_time_hush",													//沉默时间
		"equip_rate_unarm",					 								//缴械几率
		"equip_time_unarm",							 						//缴械时间
		"equip_base_miss",													//抵抗碾压
		"equip_rate_attack_speed",					 						//减攻击速度几率
		"equip_rate_skill_speed",											//减施法速度几率
		"equip_rate_move_reduce",											//减移动速度几率
		"equip_attr_hurt_dot	",		  									//dot伤害
		"equip_attr_debuff_resist",					 						//抵抗debuff
		"equip_attr_speed_move	",					 						//移动速度
		"equip_attr_speed_skill",											//施法速度
		"equip_attr_attack_speed",											//攻击速度
		"equip_attr_model",													//模型
		"equip_attr_color_hair",											//发色
		"equip_attr_model_hair",											//发型
		"equip_attr_hp_restore",											//生命回复
		"equip_attr_mp_restore",											//魔法回复
		"equip_attr_rage_restore",											//怒气回复
		"equip_attr_crit_hurt",												//增加爆击伤害
		"equip_attr_shield_all",											//所有盾
		"equip_attr_shield_near",											//近程盾
		"equip_attr_shield_m",												//魔法盾
		"equip_attr_shield_far",											//远程盾
		"equip_attr_shield_cold",											//冰盾
		"equip_attr_shield_fire",											//火盾
		"equip_attr_shield_light",											//电盾
		"equip_attr_shield_poison",											//毒盾
		"equip_attr_shield_mp_reduce",										//内力抵消伤害盾
		"equip_attr_skill_level_inc",										//增加单个技能等级
		"equip_attr_allskill_level_inc",									//增加所有技能等级
		"equip_attr_wushi_hurt_resist",										//抵抗武士职业伤害
		"equip_attr_jianxia_hurt_resist",									//抵抗剑侠职业伤害
		"equip_attr_qishe_hurt_resist",			 							//抵抗骑射手职业伤害
		"equip_attr_huoqiang_hurt_resist",									//抵抗火枪手职业伤害
		"equip_attr_xianzhi_hurt_resist",									//抵抗先知职业伤害
		"equip_attr_saman_hurt_resist",			 							//抵抗萨满职业伤害
		"equip_attr_exprence_get",											//增加经验获得
		"equip_attr_money_get",				 								//增加金钱获得
		"equip_attr_point_ability1",			 							//增加通用点获得1点数
		"equip_attr_rate_ability1",				 							//增加通用点获得1百分比
		"equip_attr_point_ability2",										//增加通用点获得2点数
		"equip_attr_rate_ability2",											//增加通用点获得2百分比
		"equip_attr_point_ability3",										//增加通用点获得3点数
		"equip_attr_nodefence",												//增加通用点获得3百分比
		"equip_attr_rate_ability3",		 									//增加通用点获得4点数
		"equip_attr_rate_ability4	",										//增加通用点获得4百分比
		"equip_attr_point_ability5",				 						//增加通用点获得5点数
		"equip_attr_rate_ability5",											//增加通用点获得5百分比
		"equip_attr_point_ability6",										//增加通用点获得6点数
		"equip_attr_rate_ability6",											//增加通用点获得6百分比
		"equip_attr_point_ability7",										//增加通用点获得7点数
		"equip_attr_rate_ability7",											//增加通用点获得7百分比
		"equip_attr_point_ability8",										//增加通用点获得8点数
		"equip_attr_rate_ability8",											//增加通用点获得8百分比
		"equip_attr_point_ability9",										//增加通用点获得9点数
		"equip_attr_rate_ability9",											//增加通用点获得9百分比
		"equip_point_ability10",											//增加通用点获得10点数
		"equip_rate_ability10",						 						//增加通用点获得10百分比	

		"equip_base_rate_attack_near",										//基础近程攻击百分比
		"equip_base_rate_attack_far",										//基础远程攻击百分比
		"equip_base_rate_attack_m",											//基础魔法攻击百分比
		"equip_base_rate_defence_enear",									//基础近程防御百分比
		"equip_base_rate_defence_far",										//基础远程防御百分比
		"equip_base_rate_defence_m",										//基础魔法防御百分比	

		"equip_base_attack_near",											//基础近程攻击
		"equip_base_attack_far",										    //基础远程攻击
		"equip_base_attack_m",												//基础魔法攻击
		"equip_base_defence_near",											//基础近程防御
		"equip_base_defence_far",											//基础远程防御
		"equip_base_defence_m"	,										    //基础魔法防御

		"equip_iattribute_rate_nearatt_reduce",					    		//按百分比抵消近程攻击
		"equip_iiattribute_rate_faratt_reduce",								//按百分比抵消远程攻击
		"equip_iattribute_rate_matt_reduce",									//按百分比抵消魔法攻击
		"equip_iattribute_rate_coldatt_reduce",								//按百分比抵消冰攻击
		"equip_iattribute_rate_fireatt_reduce",								//按百分比抵消火攻击
		"equip_iattribute_rate_lightatt_reduce",							//按百分比抵消电攻击
		"equip_iattribute_rate_poison_reduce",								//按百分比抵消毒攻击

		 
};

CObject_Item_Gem::CObject_Item_Gem(INT id)   
		: CObject_Item(id)				  
{
	m_theBaseDef = NULL;
//	m_HostOfAppend = NULL;
}

CObject_Item_Gem::~CObject_Item_Gem()
{
}

VOID CObject_Item_Gem::AsGem(const _DBC_ITEM_GEM* pGemDefine)
{
	KLAssert(pGemDefine);

	m_theBaseDef = pGemDefine;
	m_nParticularID = ( ( m_theBaseDef->nClass * 100 + m_theBaseDef->nType ) * 10000 ) + m_theBaseDef->nGemIndex;


}

LPCTSTR  CObject_Item_Gem::GetIconName(VOID) const
{
	return m_theBaseDef->szIcon;
}

//设置详细解释
VOID CObject_Item_Gem::SetExtraInfo(const _ITEM * pItemInfo)
{
	KLAssert(pItemInfo);

	SetNumber(pItemInfo->GetItemCount());

	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}
}

LPCTSTR  CObject_Item_Gem::GetExtraDesc(VOID)
{
	//CHAR szTemp[1024];

	//_snprintf(szTemp, 1024, 
	//	"ID=%d\n"
	//	"名称: %s\n"
	//	"-------------------\n"
	//	"%s",
	//	m_ID.m_idOrg.m_uSerial, 
	//	m_theBaseDef->szName,
	//	m_theBaseDef->szDesc);//
	
	if (m_theBaseDef)
	{
		m_strExtraDesc = m_theBaseDef->szDesc;
		return m_strExtraDesc.c_str();
	}
	
	return NULL;
	
}

VOID	CObject_Item_Gem::Clone(const CObject_Item * pItemSource)
{
	SetNumber(((CObject_Item_Gem*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

// 得到宝石的等级
INT		CObject_Item_Gem::GetGemLevel()
{
	if(m_theBaseDef)
	{
		return  1;
	}

	return 0;
}


// 20100408 AddCodeBegin
// 得到物品买入的价格
INT		CObject_Item_Gem::GetItemBasePrice()
{
	if(m_theBaseDef)
	{
		return (m_theBaseDef->nPrice );
	}

	return 0;
}
// 20100408 AddCodeEnd


// 得到物品卖给npc的价格
INT		CObject_Item_Gem::GetItemPrice()
{
	if(m_theBaseDef)
	{	
		return m_theBaseDef->nSalePrice;
	}

	return -1;
}


// 得到宝石属性信息
LPCTSTR	CObject_Item_Gem::GetGemArribInfo()
{
	
	static TCHAR szAttribInfo[1024];
	TCHAR	szInfo[512];

	memset(szAttribInfo, 0, sizeof(szAttribInfo));
	STRING strAttrib = _T("");

	STRING strTemp = "";
		
	if(m_theBaseDef)
	{
		for(int i = 0; i < EQUIP_GEMATTR_MAXNUM; i++)
		{
			memset(szInfo, 0, sizeof(szInfo));
			if(-1 != m_theBaseDef->stGemAttr[i].nNum && m_theBaseDef->stGemAttr[i].nAttr < sizeof(g_szGemAttName)/sizeof(char*))
			{
				strTemp = NOCOLORMSGFUNC(g_szGemAttName[m_theBaseDef->stGemAttr[i].nAttr]);
				_stprintf(szInfo, _T("%s+%d\n"), strTemp.c_str(), m_theBaseDef->stGemAttr[i].nNum);
				//_stprintf(szInfo, _T("%s:%d\n"), g_szGemAttName[ m_theBaseDef->stGemAttr[i].nAttr], m_theBaseDef->stGemAttr[i].nNum);
				strAttrib += szInfo;
			}
		}

		_stprintf(szAttribInfo, _T("%s"), strAttrib.c_str());
		return szAttribInfo;
	}
	 

	return szAttribInfo;

}


// 得到type信息
INT	CObject_Item_Gem::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
	return -1;
}


// 得到物品的类型描述 
LPCTSTR	CObject_Item_Gem::GetItemTableTypeDesc()
{
	if(m_theBaseDef)
		return m_theBaseDef->szTypeDesc;

	return NULL;
}
//掉落外形

 LPCTSTR	CObject_Item_Gem::GetDropVisualID(VOID)	const


{
	return NULL;

}

//显示名称颜色
LPCTSTR	 CObject_Item_Gem::GetDropVisColor(VOID) const 
  {
	  return NULL;
  }