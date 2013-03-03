
/**	Obj_Item_Equip.cpp
 *	
 *	功能：	
 *	
 *	修改记录：
 *			080221	
			080409    玩家套装信息tip  wangzhen
 *			
 */

#include "StdAfx.h"
#include "Obj_Item_Equip.h"
#include "GameStruct_Item.h"
#include "DataPool/GMDataPool.h"
#include "DBC/GMDataBase.h"
#include "GIDBC_Struct.h"
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
//
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
//
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
//IATTRIBUTE_NUMBER, //物品属性类型总数



const char* g_szAttachSkillName[] =

{
	     "equip_skill_fire",												 //附技能 地狱火焰 等级																			
		 "equip_skill_thunder",			        							//雷电等级 
		 "equip_skill_hit"                                                  //重击等级

};
											
const char* g_szAttachAttName[] = 											
{	                                                                
		"equip_attr_pointstr",												//力量点数																				
		"equip_attr_ratestr",			        							//力量百分比
		"equip_attr_pointdex",		     									//敏捷点数
		"equip_attr_ratedex",			     								//敏捷百分比
		"equip_attr_pointint",			    								//智力点数
		"equip_attr_rateint",		                    					//智力百分比
		"equip_attr_pointcon",                          					//体质点数
		"equip_attr_ratecon",	 											//体质百分比
		"equip_attr_allbasepoint",	        								//所有属性点数
		"equip_attr_allbaserate",											//所有属性百分比
		"equip_attr_pointattack_near",	     								//近程内功点数
		"equip_attr_pointattack_far",	     								//近程外功点数
		"equip_attr_rateattack_near",	    								//近程内功百分比
		"equip_attr_rateattack_far",	    								//近程外功百分比
		"equip_attr_attack_nearpoint",										//近攻点数
		"equip_attr_attack_nearrate",										//近攻百分比
		"equip_attr_attack_farpoint",										//远攻点数
		"equip_attr_attack_farrate",	     								//远攻百分比
		"equip_attr_defence_mpoint_near",	 								//近程内功防御点数
		"equip_attr_defence_mpoint_far",	 								//近程外功防御点数
		"equip_attr_defence_mrate_near",	 								//近程内功防御百分比
		"equip_attr_defence_mrate_far",	 									//近程外功防御百分比
		"equip_attr_defence_nearpoint",										//近防点数
		"equip_attr_defence_nearrate",										//近防百分比
		"equip_attr_defence_farpoint",										//远防点数
		"equip_attr_defence_farrate",										//远防百分比
		"equip_attr_point_maxhp",											//生命值上限点数
		"equip_attr_rate_maxhp",											//生命值上限百分比
		"equip_attr_point_maxmp",											//魔法值上限点数
		"equip_attr_rate_maxmp",											//魔法值上限百分比
		"equip_attr_maxyuanqi",												//元气上限		
		"equip_attr_maxyuanqi_rate",										//元气上限百分比
		"equip_attr_maxrage",												//怒气上限		
		"equip_attr_maxrage_rate",											//怒气上限百分比
		"equip_attr_rageres_point",											//怒气回复点												
		"equip_attr_rageres_rate",											//怒气回复百分比
		"equip_attr_hit",													//命中
		"equip_attr_miss",													//闪避
		"equip_attr_rate_hit",												//命中率
		"equip_attr_sock",			     									//暴击
		"equip_attr_toughness",			                					//韧性
		"equip_attr_sock_rate",												//爆击率
		"equip_attr_gold_attack",											//金攻击点数
		"equip_attr_rate_gold_attack",										//金攻击百分比
		"equip_attr_wood_attack",			 								//木攻击点数
		"equip_attr_rate_wood_attack",				 						//木攻击百分比
		"equip_attr_water_attack",											//水攻击点数
		"equip_attr_rate_water_attack",										//水攻击百分比
		"equip_attr_fire_attack",											//火攻击点数
		"equip_attr_rate_fire_attack",										//火攻击百分比
		"equip_attr_soil_attack",											//土攻击点数
		"equip_attr_rate_soil_attack",										//土攻击百分比
		"equip_attr_gold_resist",											//金防御点数
		"equip_rate_gold_resist",											//金防御百分比
		"equip_attr_wood_resist",											//木防御点数
		"equip_rate_wood_resist",											//木防御百分比
		"equip_water_resist",												//水防御点数
		"equip_rate_water_resist",											//水防御百分比
		"equip_attr_fire_resist",											//火防御点数
		"equip_attr_rate_fire_resist",										//火防御百分比
		"equip_attr_soil_resist",											//土防御点数
		"equip_attr_rate_soil_resist",										//土防御百分比
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
		"equip_attr_yuanqi_restore",										//元气回复
		"equip_attr_rage_restore",											//怒气回复
		"equip_attr_crit_hurt",												//增加爆击伤害
		"equip_attr_shield_all",											//所有盾
		"equip_attr_shield_near",											//近程盾
		"equip_attr_shield_m_near",											//近程内功盾
		"equip_attr_shield_m_far",											//近程外功盾
		"equip_attr_shield_far",											//远程盾
		"equip_attr_shield_gold",											//金盾
		"equip_attr_shield_wood",											//木盾
		"equip_attr_shield_water",											//水盾
		"equip_attr_shield_fire",											//火盾
		"equip_attr_shield_soil",											//土盾
		"equip_attr_shield_mp_reduce",										//内力抵消伤害盾
		"equip_attr_skill_level_inc",										//增加单个技能等级
		"equip_attr_allskill_level_inc",									//增加所有技能等级
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
		"equip_base_rate_attack_m_near",									//基础近程内功攻击百分比
		"equip_base_rate_attack_m_far",										//基础近程外功攻击百分比
		"equip_base_rate_defence_enear",									//基础近程防御百分比
		"equip_base_rate_defence_far",										//基础远程防御百分比
		"equip_base_rate_defence_m_near",									//基础近程内功防御百分比	
		"equip_base_rate_defence_m_far",									//基础近程外功防御百分比	
		"equip_base_attack_near",											//基础近程攻击
		"equip_base_attack_far",										    //基础远程攻击
		"equip_base_attack_m_near",											//基础近程内功攻击
		"equip_base_attack_m_far",											//基础近程外功攻击
		"equip_base_defence_near",											//基础近程防御
		"equip_base_defence_far",											//基础远程防御
		"equip_base_defence_m_near"	,										//基础近程内功防御
		"equip_base_defence_m_far"	,										//基础近程外功防御
		"equip_iattribute_rate_nearatt_reduce",					    		//按百分比抵消近程攻击
		"equip_iiattribute_rate_faratt_reduce",								//按百分比抵消远程攻击
		"equip_iattribute_rate_matt_reduce_near",							//按百分比抵消近程内功攻击
		"equip_iattribute_rate_matt_reduce_far",							//按百分比抵消近程外功攻击
		"equip_iattribute_rate_goldatt_reduce",								//按百分比抵消金攻击
		"equip_iattribute_rate_woodatt_reduce",								//按百分比抵消木攻击
		"equip_iattribute_rate_wateratt_reduce",							//按百分比抵消水攻击
		"equip_iattribute_rate_fireatt_reduce",								//按百分比抵消火攻击
		"equip_iattribute_rate_soilatt_reduce",								//按百分比抵消土攻击
		"equip_rate_kongju_reduce",											//恐惧抗性
		"equip_rate_hongmi_reduce",											//昏迷抗性
		"equip_rate_chengmo_reduce",										//沉默抗性
		"equip_rate_jiaoxie_reduce",										//缴械抗性
		"equip_rate_speed_attack_reduce",									//减攻速抗性
		"equip_rate_speed_add_reduce",										//减施速抗性
		"equip_rate_speed_sub_reduce",										//减移速抗性


		"equip_reduce_gunwuseng",					    					//减少伤害--少林派[棍武僧]
		"equip_reduce_tieluohan",											//减少伤害--少林派[铁罗汉]	
		"equip_reduce_gungaibang",											//减少伤害--丐帮[棍丐帮]
		"equip_reduce_zhanggaibang",										//减少伤害--丐帮[掌丐帮]
		"equip_reduce_jianwudang",											//减少伤害--武当派[剑武当]	
		"equip_reduce_qiwudang",											//减少伤害--武当派[气武当]
		"equip_reduce_daotangmen",											//减少伤害--唐门[刀唐]		
		"equip_reduce_xianjingtang",										//减少伤害--唐门[陷阱唐]	
		"equip_reduce_xiaoyaoshan",											//减少伤害--逍遥派[扇逍遥]
		"equip_reduce_zhixiaoyao",											//减少伤害--逍遥派[指逍遥]
		"equip_reduce_kuangyaowang",										//减少伤害--药王谷[狂药王]
		"equip_reduce_yiyaowang",											//减少伤害--药王谷[医药王]	
		"equip_reduce_ditaohua",											//减少伤害--桃花岛[笛桃花]
		"equip_reduce_yintaohua",											//减少伤害--桃花岛[音桃花]
		"equip_reduce_ciermei",												//减少伤害--峨嵋派[刺峨眉]
		"equip_reduce_fuermei",												//减少伤害--峨嵋派[辅峨眉]
		"equip_reduce_xinshou",												//减少伤害--无门派，如新手。。等

};								
enum EQUIP_PAI_ClIENT
{ 
	C_EQUIPPAI_DONG		=	15	,//东
	C_EQUIPPAI_NAN		=	16	,//南
	C_EQUIPPAI_XI		=	17	,//西
	C_EQUIPPAI_BEI		=	18	,//北
	C_EQUIPPAI_ZHONG	=	19	,//中
	C_EQUIPPAI_FA		=	20	,//发
	C_EQUIPPAI_BAI		=	21	,//白
	C_EQUIPPAI_1TIAO	=	31	,//1条
	C_EQUIPPAI_2TIAO	=	32	,//2条
	C_EQUIPPAI_3TIAO	=	33	,//3条
	C_EQUIPPAI_4TIAO	=	34	,//4条
	C_EQUIPPAI_5TIAO	=	35	,//5条
	C_EQUIPPAI_6TIAO	=	36	,//6条
	C_EQUIPPAI_7TIAO	=	37	,//7条
	C_EQUIPPAI_8TIAO	=	38	,//8条
	C_EQUIPPAI_9TIAO	=	39	,//9条
	C_EQUIPPAI_1TONG	=	41	,//1筒
	C_EQUIPPAI_2TONG	=	42	,//2筒
	C_EQUIPPAI_3TONG	=	43	,//3筒
	C_EQUIPPAI_4TONG	=	44	,//4筒
	C_EQUIPPAI_5TONG	=	45	,//5筒
	C_EQUIPPAI_6TONG	=	46	,//6筒
	C_EQUIPPAI_7TONG	=	47	,//7筒
	C_EQUIPPAI_8TONG	=	48	,//8筒
	C_EQUIPPAI_9TONG	=	49	,//9筒
	C_EQUIPPAI_1WAN		=	51	,//1万
	C_EQUIPPAI_2WAN		=	52	,//2万
	C_EQUIPPAI_3WAN		=	53	,//3万
	C_EQUIPPAI_4WAN		=	54	,//4万
	C_EQUIPPAI_5WAN		=	55	,//5万
	C_EQUIPPAI_6WAN		=	56	,//6万
	C_EQUIPPAI_7WAN		=	57	,//7万
	C_EQUIPPAI_8WAN		=	58	,//8万
	C_EQUIPPAI_9WAN		=	59	,//9万
	C_EQUIP_PAI_NUM,
};	
const char* g_szCardName[] = 	
{
 			
	"East_Card",				//东
	"South_Card",				//南
	"West_Card",				//西
	"North_Card",				//北
	"Center_Card",				//中
	"Fa_Card",					//发
	"White_Card",				//白
 "",

 "",

 "",
 "",
 "",
 "",
 "",
 "",
 "",

	"One_Tiao",					//1条
	"Two_Tiao",					//2条
	"Three_Tiao",				//3条
	"Four_Tiao"	,				//4条
	"Five_Tiao"	,				//5条
	"Six_Tiao",					//6条
	"Seven_Tiao",				//7条
	"Eight_Tiao",				//8条
	"Nine_Tiao",				//9条

	 "",
	
	"One_Tong",					//1筒
	"Two_Tong",					//2筒
	"Three_Tong",				//3筒
	"Four_Tong"	,				//4筒
	"Five_Tong"	,				//5筒
	"Six_Tong",					//6筒
	"Seven_Tong",				//7筒
	"Eight_Tong",				//8筒
	"Nine_Tong",				//9筒

	 "",
	"One_Wan",					//1万
	"Two_Wan",					//2万
	"Three_Wan",				//3万
	"Four_Wan",					//4万
	"Five_Wan",					//5万
	"Six_Wan",					//6万
	"Seven_Wan",				//7万
	"Eight_Wan",				//8万
	"Nine_Tiao",				//9万

	"anyCard",					//任意牌
	"anyFengCard",				//任意风牌
	"anyJianCard",				//任意箭牌
	"anyZiCard",				//任意字牌
	"anyNoZiCard",				//任意非字牌
	"anyTiaoCard",				//任意条牌
	"anyTongCard",				//任意筒牌
	"anyWanCard",				//任意万牌
	"anyShunCard",				//任意顺
	"anyKeCard",				//任意刻
	"anyJiang",					//任意将		

};

CObject_Item_Equip::CObject_Item_Equip(INT id)	
		: CObject_Item(id)						
{
	m_eEquipStyle	= INVAILD_EQUIP;
	m_pExtraDefine	= NULL;
	m_EquipAttrib	= EQUIP_ATTRIB_IDENTIFY; 
	m_theBaseDef.pDefineEquip = NULL;

	
}


CObject_Item_Equip::~CObject_Item_Equip()
{
	if(m_pExtraDefine)
	{
		delete m_pExtraDefine;
	}
	m_pExtraDefine = NULL;
}


// 基本类型class
ITEM_CLASS CObject_Item_Equip::GetItemClass(VOID) const 
{ 

		if (m_theBaseDef.pDefineEquip)
		{
			return (ITEM_CLASS)m_theBaseDef.pDefineEquip->nClass;
		}
			
		return ITEM_CLASS(-1);
		
}



//高级装备的属性可以读表 
VOID	CObject_Item_Equip::AsAdvEquip( const _DBC_ITEM_EQUIP* pDefine)
{
	//装备属性
	//_ITEM_ATTR	attr;
	//m_pExtraDefine->m_vEquipAttributes.clear();
	//for(INT i=0; i<EQUIP_EFFECT_ATTR_MAXNUM; i++)
	//{
	//	if(-1 != pDefine->stEffectAttr[i].nAttrID )
	//	{
	//		attr.m_AttrType = (BYTE)pDefine->stEffectAttr[i].nAttrID;
	//		attr.m_Value.m_Value =  (SHORT)pDefine->stEffectAttr[i].nEffectNum;  
	//		m_pExtraDefine->m_vEquipAttributes.push_back(attr);
	//	}
	//}

	//附加技能
	_ITEM_AFFIX_SKILL affx_skill;
	m_pExtraDefine->m_Affix_Skill.clear();    

	for(INT i=0; i< EQUIP_ADDEFFECT_SKILL_MAXNUM; i++)
	{
		if(-1 != pDefine->stADDEffectSkill[i].nSkillID )
		{
			affx_skill.m_nSkillID = (WORD)pDefine->stADDEffectSkill[i].nSkillID;
			affx_skill.m_nLevel =  (BYTE)pDefine->stADDEffectSkill[i].nEffectLevel;
			m_pExtraDefine->m_Affix_Skill.push_back(affx_skill);
		}
	}
	m_pExtraDefine->m_nAffix_SkillNum = m_pExtraDefine->m_Affix_Skill.size();

	//影响技能

	_ITEM_EFFECT_SKILL effect_skill;
	m_pExtraDefine->m_Effect_Skill.clear();    

	for(INT i=0; i< EQUIP_EFFECT_SKILL_MAXNUM; i++)
	{
		if(-1 != pDefine->stEffectSkill[i].nSkillID )
		{
			effect_skill.m_nSkillID = (WORD)pDefine->stEffectSkill[i].nSkillID;
			effect_skill.m_nLevel =  (BYTE)pDefine->stEffectSkill[i].nEffectLevel;
			m_pExtraDefine->m_Effect_Skill.push_back(effect_skill);
		}
	}
	m_pExtraDefine->m_nEffect_SkillNum = m_pExtraDefine->m_Effect_Skill.size();


     
}
 

//生成一件装备
VOID CObject_Item_Equip::AsEquip(const   _DBC_ITEM_EQUIP* pDefine )
{
	  
	m_theBaseDef.pDefineEquip = pDefine;

	m_nParticularID = (( m_theBaseDef.pDefineEquip->nClass * 100 + m_theBaseDef.pDefineEquip->nType ) * 10000 ) + m_theBaseDef.pDefineEquip->nIndex;

	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	m_pExtraDefine->m_nSellPrice = pDefine->nBasePrice;			//售出价格
	m_pExtraDefine->m_nLevelNeed = atoi(pDefine->nLevelRequire);//需求等级 
	m_pExtraDefine->m_nMaxDurPoint  = pDefine->nBMaxDur;		//最大耐久值
	m_pExtraDefine->m_SetNum  = pDefine->nSetID;				//套装id
//	m_pExtraDefine->m_nSellPrice  = pDefine->nBasePrice;        //价格

	//当前耐久值 
	m_pExtraDefine->m_CurDurPoint  = pDefine->nBMaxDur;       // max耐久

	//打开数据表
	DBC_DEFINEHANDLE(s_pItem_Visual, DBC_ITEM_EQUIP_VISUAL);
	//搜索纪录
	const _DBC_ITEM_EQUIP_VISUAL* pVisual =NULL;

	pVisual =(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_Visual->Search_Index_EQU(pDefine->nID);
	if(!pVisual) return ;

	m_theBaseDef.pEquipVisual =  pVisual;

   m_theBaseDef.m_Base.m_nBase_Atack_Near = pDefine->nBaseAttackNear;
   m_theBaseDef.m_Base.m_nBase_Atack_Far = pDefine->nBaseAttackFar;
   m_theBaseDef.m_Base.m_nBase_Atack_Magic =pDefine->nBaseAttackMagic;

   m_theBaseDef.m_Base.m_nBase_Def_Near = pDefine->nBaseDEefNear;
   m_theBaseDef.m_Base.m_nBase_Def_Far = pDefine->nBaseDefFar;
   m_theBaseDef.m_Base.m_nBase_Def_Magic = pDefine->nBaseDefMagic;

	// 高级装备
	if( !ISCommonEquip(pDefine->nID) )
	{
		m_eEquipStyle = ADVANCED_EQUIP;
		AsAdvEquip( pDefine);
	} 
     //普通装备
	else  
	{
		m_eEquipStyle = COMMON_EQUIP;
	}


}

INT		CObject_Item_Equip::GetEquipScore()
{

		 if (m_pExtraDefine)
		 {
			 return m_pExtraDefine->m_nEquipScore;
		 }

		 return 0;
}


//得到装备是否被附魔
BOOL	CObject_Item_Equip::GetIsFumo()
{
	return m_pExtraDefine->m_SoulBeadAttrCount>0 ;
}



//装备牌型是否将
BOOL					Equip_Card_Is_Jiang( INT  c1 ,INT c2, BOOL isSpecial)
{
	if ( c1 ==0 || c2 == 0 )
	{
		return FALSE;
	}
	if ( c1 == c2  )
	{
		if(isSpecial)
		{
			switch( c1)
			{
			case EQUIPPAI_2TIAO:
			case EQUIPPAI_5TIAO:
			case EQUIPPAI_8TIAO:
			case EQUIPPAI_2TONG:
			case EQUIPPAI_5TONG:
			case EQUIPPAI_8TONG:
			case EQUIPPAI_2WAN:
			case EQUIPPAI_5WAN:
			case EQUIPPAI_8WAN:

				return TRUE;
			default :
				return FALSE;
			}

		}
		
		return TRUE;
	
			
	}
	return FALSE;

}

//装备牌型是否想等
BOOL			 Equip_Card_Is_Equal( INT  c1 ,INT c2,INT c3)

{
	if ( (c1 == 0 )||( c2 == 0) || (c3 == 0))
	{
		return FALSE ;
	}
	if (  (c1 == c2)  && ( c2 == c3))
	{
		return TRUE;
	}
	return FALSE;

}
//装备牌型是否连续
BOOL			Equip_Card_Is_Series( INT  c1 ,INT c2,INT c3)
{
	if ( (c1 == 0 )||( c2 == 0) || (c3 == 0))
	{
		return FALSE ;
	}
	if (  (c1 == c2)  || ( c2 == c3 || (c1 ==c3)))
	{
		return FALSE;
	}

	INT max = (  c1 > c2 ? c1: c2) > c3 ? (  c1 > c2 ? c1: c2)  : c3; 

	INT min =  ( c1 <c2 ? c1: c2) < c3 ? (  c1 < c2 ? c1: c2)  : c3; 

	 

	if ( ( max - min ) == 2)
	{
		return TRUE;
	}

	return FALSE;



}

//返回装备牌型最小的
INT			Equip_Card_Min( INT  c1 ,INT c2, INT c3)
{

		return ( c1 <c2 ? c1: c2) < c3 ? (  c1 < c2 ? c1: c2)  : c3; 
}
//装备点
HUMAN_EQUIP	CObject_Item_Equip::GetItemType(VOID) const
{
     CDataPool* pDataPool = CDataPool::GetMe();

     HUMAN_EQUIP point = (HUMAN_EQUIP)-1;


	if(m_theBaseDef.pDefineEquip)
	{	
		 point = (HUMAN_EQUIP) m_theBaseDef.pDefineEquip->nEquipPoint;

		if ( point == HEQUIP_RING1 ) 
			{
				if( pDataPool->UserEquip_GetItem(HEQUIP_RING1))

					point = HEQUIP_RING2;

				if( pDataPool->UserEquip_GetItem(HEQUIP_RING2))

					point = HEQUIP_RING1;

			}
		else if ( point == HEQUIP_ADORN1 )
			{
				if( pDataPool->UserEquip_GetItem(HEQUIP_ADORN1))

					point= HEQUIP_ADORN2;

				if( pDataPool->UserEquip_GetItem(HEQUIP_ADORN2))

					point= HEQUIP_ADORN1;
				
			}

	}

	return point ;

}


//模型id
INT CObject_Item_Equip::GetVisualID(VOID) const
{
	
	if( m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->nModelID;
		}

	return -1;
	
}

//得到掉落时的模型id
LPCTSTR	 CObject_Item_Equip::GetDropVisualID(VOID)	const
{	
	if (m_theBaseDef.pEquipVisual)
		{
			
			return m_theBaseDef.pEquipVisual->nDropModelId;
		}

	return NULL;

}
 
//显示名称颜色
LPCTSTR		CObject_Item_Equip::GetDropVisColor(VOID) const 
 {

	if (m_theBaseDef.pEquipVisual)
		 {
			 return m_theBaseDef.pEquipVisual->szDropVisColor;
		 }
	 return NULL;

 }

//武器类型
ENUM_WEAPON_TYPE CObject_Item_Equip::GetWeaponType(VOID) const
{
	if(!m_theBaseDef.pDefineEquip)
		return WEAPON_TYPE_INVALID;

	INT nDefine = WEAPON_TYPE_INVALID;

	if (m_theBaseDef.pDefineEquip)
	{
		nDefine = (INT)(m_theBaseDef.pDefineEquip->nType);
	}
	
	//转化为ENUM_WEAPON_TYPE
	switch(nDefine)//临时性注释代码,留待方远修改代码(-_-修改完了)
	{
		case EQUIP_GUN	     :		return WEAPON_TYPE_GUN;	// 棍
		case EQUIP_BANG	 :		return WEAPON_TYPE_BANG;		// 棒
		case EQUIP_JIAN	 :		return WEAPON_TYPE_JIAN;			// 剑
		case EQUIP_DADAO :		return WEAPON_TYPE_DAO;		// 刀
		case EQUIP_SHANZI 	 :		return WEAPON_TYPE_SHANZI;		// 扇子
		case EQUIP_HULU 	 :		return WEAPON_TYPE_HULU;		// 葫芦
		case EQUIP_DIZI		:	return WEAPON_TYPE_DIZI;							//笛子
		case EQUIP_CI		:	return WEAPON_TYPE_CI;							//刺
		case EQUIP_TOOL	:	return WEAPON_TYPE_NONE;			//工具
		default	:
			return  WEAPON_TYPE_INVALID ;
	}

}


//名字。。
LPCTSTR CObject_Item_Equip::GetName(VOID) const
{
	 
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szName;
		}
	 
		return NULL;
 
		
}
//ui图标
LPCTSTR CObject_Item_Equip::GetIconName(VOID) const
{ 
	 
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szIcon;
		}
	 
		return NULL;
	 
}
//得到装备牌型
INT  CObject_Item_Equip::GetEquipCard(VOID) 
{
		if (  m_pExtraDefine)
		{
			return m_pExtraDefine->m_PaiXing;
		}

	return 0;

}

//得到装备牌型
VOID      CObject_Item_Equip::GetEquipCardForTip(STRING& strValue) 

{

	INT id = this->GetEquipCard();


	if ( id < C_EQUIPPAI_DONG || id > C_EQUIP_PAI_NUM)
	{
		return ;

	}

	TCHAR bufInfo[256]={0};
	_stprintf(bufInfo, "#{%s} \n", g_szCardName[id -C_EQUIPPAI_DONG]);
	strValue += bufInfo ;		 

 
}

// 交换装备牌型
BOOL	CObject_Item_Equip::SwapEquipCard(CObject_Item_Equip &ep)

{
	if (  m_pExtraDefine && ep.m_pExtraDefine)
	{
		// swap 装备牌型  
		m_pExtraDefine->m_PaiXing = m_pExtraDefine->m_PaiXing + ep.m_pExtraDefine->m_PaiXing;

		ep.m_pExtraDefine->m_PaiXing = m_pExtraDefine->m_PaiXing -ep.m_pExtraDefine->m_PaiXing;

		m_pExtraDefine->m_PaiXing= m_pExtraDefine->m_PaiXing -ep.m_pExtraDefine->m_PaiXing;
		return TRUE;

	}
	return FALSE;

}


//设置装备牌型
VOID  CObject_Item_Equip::SetEquipCard( INT nPaiXing )  
{
	if (  m_pExtraDefine)
	{
		  m_pExtraDefine->m_PaiXing =  nPaiXing;
	}

}

// 得到星级
INT CObject_Item_Equip::GetStartLevel(VOID) const
{
	if (m_pExtraDefine)
	{
		return m_pExtraDefine->m_StartLevel;
	}

	return -1;
}
// 装备描述
LPCTSTR CObject_Item_Equip::GetDesc(VOID) const
{
	 
		if(m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szDes;
		}
 
		return NULL;
	 
}

INT CObject_Item_Equip::GetGemMaxCount(VOID) const
{
	if (m_pExtraDefine)
	{
		return  m_pExtraDefine->m_GemCount;
	}

	return  -1;
}

//克隆详细信息
VOID CObject_Item_Equip::Clone(const CObject_Item * pItemSource)
{
	ClonetExtraInfo((CObject_Item_Equip*)pItemSource);
	CObject_Item::Clone(pItemSource);
}


 //设置详细解释
VOID CObject_Item_Equip::SetExtraInfo(const _ITEM * pItemInfo)
{		
	if( !m_pExtraDefine )
		m_pExtraDefine = new EXTRA_DEFINE;


	EQUIP_INFO* ed = pItemInfo->GetEquipData();
	if( !ed )
	{
		delete m_pExtraDefine;
		m_pExtraDefine = NULL;

		return ;
	}

	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}

	//属性组
	m_pExtraDefine->m_nAttSet			= ed->m_nAttSet;
	//星级
	m_pExtraDefine->m_StartLevel		= ed->m_Level;
	//当前损伤度 
	m_pExtraDefine->m_CurDamagePoint	= ed->m_CurDamagePoint;
	//当前耐久值 
	m_pExtraDefine->m_CurDurPoint		= ed->m_CurDurPoint;

	//镶嵌的宝石个数
	m_pExtraDefine->m_GemCount			= ed->m_StoneCount;
    //等级需求
	m_pExtraDefine->m_nLevelNeed        = ed->m_NeedLevel;
   //装备孔数目
	m_pExtraDefine->m_nEquipHoleNum		= ed->m_CurGemHole;

	//装备分数
	m_pExtraDefine->m_nEquipScore		= ed->m_EquipScore;
    			
   //属性的数量
    m_pExtraDefine->m_AttrCount        = ed->m_AttrCount;

	//装备的类型 ib 玩家合成的
	 m_pExtraDefine->m_EquipType       = ed->m_EquipType;

    m_pExtraDefine->m_RulerID          =  ed->m_RulerID;

    //特修后会上升，但不会超过表中的最大耐久值
	 m_pExtraDefine->m_nMaxDurPoint    = ed->m_CurMaxDur;		//当前最大耐久值

	 m_pExtraDefine->m_PaiXing = ed->m_PaiType  ;                //装备的牌型

	 m_pExtraDefine->m_SoulBeadType = ed->m_SoulBeadType;			//附魔的魂珠类型

	 m_pExtraDefine->m_SoulBeadAttrCount = ed->m_SoulBeadAttrCount;		//魂珠属性的数量

	AxTrace(1, 0, "%d", pItemInfo->m_Equip.m_StoneCount);

	//打开物品规则表
	DBC_DEFINEHANDLE(s_pItemRule, DBC_ITEM_RULE);

	const _DBC_ITEM_RULE* pItemRule = (const _DBC_ITEM_RULE*)s_pItemRule->Search_Index_EQU(  m_pExtraDefine->m_RulerID );
		if(pItemRule )
		{
			m_Rule.bCanDrop		=	pItemRule->bCanDrop;
			m_Rule.bCanOverLay	=	pItemRule->bCanOverLay;
			m_Rule.bCanQuick		=	pItemRule->bCanQuick;
			m_Rule.bCanSale		=	pItemRule->bCanSale;
			m_Rule.bCanExchange	=	pItemRule->bCanExchange;
			m_Rule.bCanUse		=	pItemRule->bCanUse;
			m_Rule.bPickBind		=	pItemRule->bPickBind;
			m_Rule.bEquipBind		=	pItemRule->bEquipBind;
			m_Rule.bUnique		=	pItemRule->bUnique;
			m_Rule.bCheck         =   pItemRule->bCheck;
			m_Rule.bUnRealItem    =	pItemRule->bUnRealItem;
			m_Rule.bAccessInBack  =	pItemRule->bAccessInBack;
			m_Rule.bWearOut       =   pItemRule->bWearOut;
		}

	// 高级
	//test 不用判断， 只是普通装备不显示此属性
//	if( EquipStyle == ADVANCED_EQUIP )
	{
		// 得到鉴定信息。
		if( pItemInfo->GetItemIdent() || !m_Rule.bCheck)
		{
			// 已经鉴定
			m_EquipAttrib = CObject_Item_Equip::EQUIP_ATTRIB_IDENTIFY;
		}
		else
		{
			// 未鉴定
			m_EquipAttrib = CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY;
		}
	}

	// 得到绑定信息,装备是否绑定
	if( pItemInfo->GetItemBind() )
	{
		m_pExtraDefine->m_nEquipBindInfo = BINDED;
		
	}
	else
	{
		if(Rule(RULE_PICKBIND))
			m_pExtraDefine->m_nEquipBindInfo = GETUP_BIND;
		else if(Rule(RULE_EQUIPBIND))
			m_pExtraDefine->m_nEquipBindInfo = EQUIP_BIND;
	}
     // 得到绑定信息,装备是否2级绑定
	if (ed->m_Bind == 1)
	{
		m_pExtraDefine->m_nEquipBindInfo2 = EQUIP_BIND_TWO;
	}

 

	//   附加的宝石
	m_pExtraDefine->m_vEquipAttachGem.clear();
	for(INT i=0; i<  MAX_ITEM_GEM /*pItemInfo->GetEquipData()->m_StoneCount*/; i++)
	{


		if (pItemInfo->GetEquipData()->m_pGemInfo[i].m_GemIndex !=INVALID_ITEM_ID)
		{
			m_pExtraDefine->m_vEquipAttachGem.push_back(pItemInfo->GetEquipData()->m_pGemInfo[i]);
		}
		m_pExtraDefine->m_nEquipAttachGemNum = m_pExtraDefine->m_vEquipAttachGem.size();


	}

	SetNumber(pItemInfo->GetItemCount());


	// 普通装备
	//if( m_eEquipStyle == COMMON_EQUIP )
	//{
	//
	//	//装备属性
	//	_ITEM_ATTR	attr;
	//	m_pExtraDefine->m_vEquipAttributes.clear();

	//	//打开数据表	
	//	DBC_DEFINEHANDLE(s_pItem_AttrGroup, DBC_ITEM_ATTR_GROUP);	// attribute_set.tab 装备属性组表
	//	//搜索纪录
	//	const _DBC_ITEM_ATTR_GROUP* pEquipAttrGroup = (const _DBC_ITEM_ATTR_GROUP*)s_pItem_AttrGroup->Search_Index_EQU((UINT) m_pExtraDefine->m_nAttSet);
	//	if(!pEquipAttrGroup) return ;
	//	 m_pExtraDefine->m_szAttrGroup = pEquipAttrGroup->szGroupName;
	//	//打开数据表
	//	DBC_DEFINEHANDLE(s_pItem_AttrLevel, DBC_ITEM_ATTR_LEVEL);	// attribute_level.tab 装备属性等级表
	//	const _DBC_ITEM_ATTR_LEVEL* s_AttrLevel = NULL;

	//	for(INT i=0; i< EQUIP_EFFECT_ATTR_MAXNUM; ++i)
	//	{
	//		if( -1 != pEquipAttrGroup->nAttrId[i] )
	//		{
	//	        attr.m_AttrType = (BYTE)pEquipAttrGroup->nAttrId[i];
	//			//搜索纪录
	//			 s_AttrLevel = (const _DBC_ITEM_ATTR_LEVEL*)s_pItem_AttrLevel->Search_Index_EQU((UINT)pEquipAttrGroup->nAttrId[i]);
	//			 if(!s_AttrLevel) 
	//				 return ;

	//			 attr.m_Value.m_Value =  (SHORT)s_AttrLevel->nLevel[m_pExtraDefine ->m_nLevelNeed - 1];
	//			 m_pExtraDefine->m_vEquipAttributes.push_back(attr);
	//		}
	//	}

	//}
	
	m_pExtraDefine->m_vEquipAttributes.clear();

	INT nAttNum =  m_pExtraDefine->m_AttrCount;
	for (INT  i = 0;i< nAttNum;++i)
	{

		for (INT  j = i+1 ; j<nAttNum;++j)
		{

		 	 if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType  == pItemInfo->GetEquipData()->m_pAttr[j].m_AttrType)
			 {
				pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value += pItemInfo->GetEquipData()->m_pAttr[j].m_Value.m_Value;

				pItemInfo->GetEquipData()->m_pAttr[j].m_AttrType = -1;
				pItemInfo->GetEquipData()->m_pAttr[j].m_Value.m_Value    = -1;

			 }

		}
	
	}
 

	for (INT  i = 0;i< nAttNum;++i)
	{
		if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_ATTACK_NEAR )
		{

			m_theBaseDef.m_Base.m_nBase_Atack_Near =  INT (pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);  // 基础近程攻击
		}
		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_ATTACK_FAR )
		{
			 
			m_theBaseDef.m_Base.m_nBase_Atack_Far = INT ( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // 基础远程攻击
		}
		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR )
		{
			 
			m_theBaseDef.m_Base.m_nBase_Atack_Magic = INT (pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // 基础魔法攻击
		}

		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_DEFENCE_NEAR )
		{
			
			m_theBaseDef.m_Base.m_nBase_Def_Near =INT( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);  // 基础近程防御
		}
		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_DEFENCE_FAR )
		{
 
			m_theBaseDef.m_Base.m_nBase_Def_Far = INT( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // 基础远程防御
		}

		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR )
		{
			 
			m_theBaseDef.m_Base.m_nBase_Def_Magic = INT( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // 基础魔法防御		
		}


		else

		{
			m_pExtraDefine->m_vEquipAttributes.push_back(pItemInfo->GetEquipData()->m_pAttr[i]);
			m_pExtraDefine->m_nEquipAttributesNum = m_pExtraDefine->m_vEquipAttributes.size();
		}

			
	}

}

VOID CObject_Item_Equip::SetExtraInfo( CHAR* pstrIn )
{
	if( !m_pExtraDefine )
		m_pExtraDefine = new EXTRA_DEFINE;

	m_pExtraDefine->WriteVarAttr( pstrIn );
}

LPCTSTR	CObject_Item_Equip::GetExtranInfo()
{
	CHAR    out[MAX_ITEM_STRING_LENGTH] = {0};
	CHAR*	pOut = out;
	CHAR* pBuff = pOut;
	INT OutLength = 0;
	*((UINT*)pBuff)	=	m_idTable;
	pBuff+=sizeof(UINT);
	//*((CHAR*)pBuff) = 'i';
	//pBuff+=sizeof(CHAR);

	if ( m_pExtraDefine != NULL )
	{
		m_pExtraDefine->ReadVarAttr( pBuff, OutLength );
	}
	else
	{
		return "";
	}

	OutLength	+=	sizeof(INT);
	//OutLength	+=	sizeof(CHAR);
	CHAR		p[255];
	memset(p,0,255);
	Binary2String(pOut,OutLength,p);
	strcpy( pOut, "i");
	strcat( pOut, p );
	OutLength = (INT)strlen(pOut);

	return pOut;
}

//克隆额外信息
VOID CObject_Item_Equip::ClonetExtraInfo( CObject_Item_Equip * pItemSource)
{
	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	EXTRA_DEFINE* pNewExtraDefine = (EXTRA_DEFINE*)(pItemSource->GetExtraInfo());



	//属性组
	m_pExtraDefine->m_nAttSet = pNewExtraDefine->m_nAttSet;
	//星级
	m_pExtraDefine->m_StartLevel  = pNewExtraDefine->m_StartLevel;
	//当前损伤度 
	m_pExtraDefine->m_CurDamagePoint = pNewExtraDefine->m_CurDamagePoint;
	//当前耐久值 
	m_pExtraDefine->m_CurDurPoint  = pNewExtraDefine->m_CurDurPoint;
	// 可以镶嵌的宝石个数
	m_pExtraDefine->m_GemCount  = pNewExtraDefine->m_GemCount;


	//售出价格
	m_pExtraDefine->m_nSellPrice = pNewExtraDefine->m_nSellPrice;
	//需求等级 
	m_pExtraDefine->m_nLevelNeed = pNewExtraDefine->m_nLevelNeed;
	//最大耐久值
	m_pExtraDefine->m_nMaxDurPoint = pNewExtraDefine->m_nMaxDurPoint;
	//牌型
	m_pExtraDefine->m_nAttSet = pNewExtraDefine->m_nAttSet;


	//宝石
	for(UINT i=0; i<pNewExtraDefine->m_vEquipAttachGem.size(); i++)
	{
		m_pExtraDefine->m_vEquipAttachGem.push_back(pNewExtraDefine->m_vEquipAttachGem[i]);
	}
	m_pExtraDefine->m_nEquipAttachGemNum = m_pExtraDefine->m_vEquipAttachGem.size();

    
	if (pItemSource->m_eEquipStyle == ADVANCED_EQUIP)
	{
		// 装备是否鉴定过属性
		m_EquipAttrib = pItemSource->m_EquipAttrib;

		//附加技能
		for(UINT i=0; i<pNewExtraDefine->m_Affix_Skill.size(); i++)
		{
			m_pExtraDefine->m_Affix_Skill.push_back(pNewExtraDefine->m_Affix_Skill[i]);
		}
		m_pExtraDefine->m_nAffix_SkillNum = m_pExtraDefine->m_Affix_Skill.size();

		//影响技能
		for(UINT i=0; i<pNewExtraDefine->m_Effect_Skill.size(); i++)
		{
			m_pExtraDefine->m_Effect_Skill.push_back(pNewExtraDefine->m_Effect_Skill[i]);
		}
		m_pExtraDefine->m_nEffect_SkillNum = m_pExtraDefine->m_Effect_Skill.size();
	}


	SetNumber(pItemSource->GetNumber());

	m_theBaseDef = pItemSource->GetEquipBaseDefine();
	
}

//得到套装编号 
INT CObject_Item_Equip::GetSetID(VOID) const
{
	 
	if (  m_pExtraDefine)
	{
		return m_pExtraDefine->m_SetNum;
	}

	return -1;

}
//详细描述 
LPCTSTR CObject_Item_Equip::GetExtraDesc(VOID)
{
	 
		if (m_theBaseDef.pEquipVisual)
		{
			m_strExtraDesc =  m_theBaseDef.pEquipVisual->szDes;
		}
		else
			m_strExtraDesc = "";

 
	return m_strExtraDesc.c_str();
}

//设置装备当前耐久度 
VOID	CObject_Item_Equip::SetDur(INT Dur)

{
	if( m_pExtraDefine )

	 m_pExtraDefine->m_CurDurPoint = Dur;

}
//获得当前耐久度
INT		CObject_Item_Equip::GetDur(VOID) const
{
	if (m_pExtraDefine)
	{
		return	m_pExtraDefine->m_CurDurPoint;
	}
	else
		return 0;
	
}
//获得最大耐久度
INT		CObject_Item_Equip::GetMaxDur(VOID) const
{

	if (m_pExtraDefine)
	{
		return	m_pExtraDefine->m_nMaxDurPoint;
	}
	else
		return 0;
}


//设置装备当前最大耐久度
VOID		CObject_Item_Equip::SetMaxDur(INT Dur)  
{

	if (m_pExtraDefine)
	{
		m_pExtraDefine->m_nMaxDurPoint  = Dur;
	}
	 
}
//获得最大耐久度
INT		CObject_Item_Equip::GetMaxDurFromTable(VOID) const
{

	if (m_theBaseDef.pDefineEquip)
	{
		return	m_theBaseDef.pDefineEquip->nBMaxDur;
	}
	else

		return -1;
}


//获得装备特效
const EQUIP_EFFECT*  CObject_Item_Equip::GetEffect() 
{
	
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szEffect;
		}

		return NULL;
	
	
}

//得到装备基本属性
VOID	CObject_Item_Equip::GetAllEquipBaseAttribute(STRING& strValue)
{
	if(!m_pExtraDefine)

	{
		strValue = "";
	}
	if(!m_pExtraDefine)

		return ;

	CHAR szAttachAtt[2048] = {0};
	CHAR szAtt[MAX_PATH] = {0};


	_ITEM_ATTR  att ;

	att.m_AttrType = IATTRIBUTE_BASE_ATTACK_NEAR;
	att.m_Value.m_Value  =	m_theBaseDef.m_Base.m_nBase_Atack_Near;  // 基础近程攻击

	if (att.m_Value.m_Value > 0 )
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}


	 

	att.m_AttrType = IATTRIBUTE_BASE_ATTACK_FAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Atack_Far ;   // 基础远程攻击
	if (att.m_Value.m_Value  > 0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}

	att.m_AttrType = IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Atack_Magic; // 基础魔法攻击

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}

	att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_NEAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Def_Near;    // 基础近程防御

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}

	att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_FAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Def_Far ;	 // 基础远程防御

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}


	att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Def_Magic;    // 基础魔法防御		

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}


	strValue = szAttachAtt;
}



//得到装备附加属性
VOID CObject_Item_Equip::GetAllEquipAddAttribute(STRING& strValue)
{
	if(!m_pExtraDefine)
	{
		strValue = "";
	}

	if(!m_pExtraDefine)
		return ;

	CHAR szAttachAtt[2048] = {0};
	CHAR szAtt[MAX_PATH] = {0};
	

	// _ITEM_ATTR  att ;

	//att.m_AttrType = IATTRIBUTE_BASE_ATTACK_NEAR;
	//att.m_Value.m_Value  =	m_theBaseDef.pDefineEquip->nBaseAttackNear;  // 基础近程攻击

	//if (att.m_Value.m_Value > 0 )
	//{
	//	SetAttri(att, szAtt);
	//    strncat(szAttachAtt, szAtt, 1024);
	//}
	//
	// 

	//att.m_AttrType = IATTRIBUTE_BASE_ATTACK_FAR;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseAttackFar;   // 基础远程攻击
	//if (att.m_Value.m_Value  > 0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
	//
	//att.m_AttrType = IATTRIBUTE_BASE_ATTACK_M;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseAttackMagic; // 基础魔法攻击

	//if (att.m_Value.m_Value >0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
	//
	//att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_NEAR;
 //   att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseDEefNear;    // 基础近程防御

	//if (att.m_Value.m_Value >0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
	// 
	//att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_FAR;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip-> nBaseDefFar;	 // 基础远程防御

	//if (att.m_Value.m_Value >0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
 //

	//att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_M;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseDefMagic;    // 基础魔法防御		

	//if (att.m_Value.m_Value >0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}


	for(INT i=0; i<(INT)m_pExtraDefine->m_vEquipAttributes.size(); i++)
	{
		const _ITEM_ATTR& att = m_pExtraDefine->m_vEquipAttributes[i];

		if((att.m_AttrType >= 0)&&(att.m_AttrType < IATTRIBUTE_BASE_ATTACK_NEAR))
		{ 
			if (att.m_Value.m_Value> 0)
			{
				SetAttri(att, szAtt);
				strncat(szAttachAtt, szAtt, 1024);
			}	
			//AxTrace(0, 1, "%s", szAtt);
		}
	}


	strValue = szAttachAtt;
}
//所有装备技能属性 

VOID	CObject_Item_Equip::GetAllEquipSkillAttr(STRING& strValue)
{	
	
     if ( ISCommonEquip( m_idTable ) )
     {
		return ;
     }


	 TCHAR skillInfo[1024]= {0};

	 for(INT i=0; i<(INT)m_pExtraDefine->m_Affix_Skill.size(); i++)
	 {
		 const _ITEM_AFFIX_SKILL& skill = m_pExtraDefine->m_Affix_Skill[i];


			 memset(skillInfo, 0, sizeof(skillInfo));
			 _stprintf(skillInfo, "#{%s}%d\n", g_szAttachSkillName[skill.m_nSkillID], skill.m_nLevel);
			 strValue += skillInfo;		 
	 }
	
	 for(INT i=0; i<(INT)m_pExtraDefine->m_Effect_Skill.size(); i++)
	 {
		 const _ITEM_EFFECT_SKILL& skill = m_pExtraDefine->m_Effect_Skill[i];

		
		 memset(skillInfo, 0, sizeof(skillInfo));
		 _stprintf(skillInfo, "#{%s}+%d\n", g_szAttachSkillName[skill.m_nSkillID], skill.m_nLevel);
		 strValue += skillInfo;		 
	 }


};


//宝石数量
INT CObject_Item_Equip::GetGemCount()
{
	if( !m_pExtraDefine)
	{
		return 0;
	}

	return  (INT)m_pExtraDefine->m_vEquipAttachGem.size();
}
//得到宝石 
BOOL CObject_Item_Equip::GetEquipGem( INT  index ,_ITEM_GEMINFO &gem)
{
  if(index < 0 || index > GetGemCount())
	 
	  return FALSE;


	if( m_pExtraDefine)
	{
		gem.m_bEnable = m_pExtraDefine->m_vEquipAttachGem[index].m_bEnable;
		gem.m_GemIndex = m_pExtraDefine->m_vEquipAttachGem[index].m_GemIndex;
			return TRUE;
	}

	return FALSE;
}


// 得到宝石的图标
VOID CObject_Item_Equip::GetGemIcon(INT iGemIndex, STRING& strIconName)
{
	if(NULL == m_pExtraDefine)
	{
		strIconName = _T("");
		return;
	}

	INT iGemCount = (INT)m_pExtraDefine->m_vEquipAttachGem.size();
	if(iGemIndex >= iGemCount)
	{
		strIconName = _T("");
		return;
	}

	//打开宝石数据表
	DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
	//搜索纪录
	UINT iGemTableIndex = 0;
	iGemTableIndex = m_pExtraDefine->m_vEquipAttachGem[iGemIndex].m_GemIndex;
	const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(iGemTableIndex);

	if(!pGem)
	{
		strIconName = _T("");
		return;
	}

	strIconName = pGem->szIcon;
	return;
	
}

// 得到宝石的附加功能
VOID CObject_Item_Equip::GetGemExtAttr(INT iGemIndex, STRING& strGemAttr)
{
	//打开宝石数据表
	DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
	//搜索纪录
	UINT iGemTableIndex = -1;
	iGemTableIndex = m_pExtraDefine->m_vEquipAttachGem[iGemIndex].m_GemIndex;
	const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(iGemTableIndex);

	if (pGem)
	{
		TCHAR bufInfo[1024]={0};
		for(int i = 0; i < EQUIP_GEMATTR_MAXNUM ; i++)
		{
			if(pGem->stGemAttr[i].nNum > -1 && pGem->stGemAttr[i].nAttr < sizeof(g_szAttachAttName)/sizeof(char*))
			{
				memset(bufInfo, 0, sizeof(bufInfo));
				_stprintf(bufInfo, "#{%s} +%d\n", g_szAttachAttName[pGem->stGemAttr[i].nAttr], pGem->stGemAttr[i].nNum);
				strGemAttr += bufInfo;
			}
		}
	}



}

  	            

 
//得到属性值
STRING CObject_Item_Equip::GetAttributeValue(LPCTSTR szValueName)
{
	if(!szValueName) return "";

	CHAR szTemp[1024];
	

	//装备图标

	 if (_stricmp(szValueName,ITEMATTRIBUTE_ICON) == 0)
	{
		return GetIconName();

	}
	 //麻将牌型

	else if (_stricmp(szValueName,ITEMATTRIBUTE_CARD) == 0)
	{
		
		if ( -1 == GetEquipCard())
		{
			return "";
		}

		STRING strValue;
		GetEquipCardForTip(strValue);
		return strValue;

	}

	//物品等级
	else if(_stricmp(szValueName, ITEMATTRIBUTE_LEVEL) == 0)
	{
		if (-1 == GetNeedLevel())
		{
			return "";
		}

		_snprintf(szTemp, 1024, "%d", GetNeedLevel());
		return szTemp;
			 		 
	}

   //职业
	else if(_stricmp(szValueName, ITEMATTRIBUTE_PROFESSION) == 0)
	{
		if (-1 == GetItemProfession())
		{
			return "";
		}
		_snprintf(szTemp, 1024, "%d", GetItemProfession());
		return szTemp;  
	}

    //最大耐久
	else  if(_stricmp(szValueName, ITEMATTRIBUTE_MAXDUR) == 0)
	{
		if (-1 == GetItemMaxDur())
		{
			return "";
		}
		_snprintf(szTemp, 1024, "%d", GetItemMaxDur()); 
		return szTemp;
		 
	}
	 //当前耐久 
	else if(_stricmp(szValueName,ITEMATTRIBUTE_CURDUR ) == 0)
	{
		if (-1 == GetItemDur())
		{
			return "";
		}
		 _snprintf(szTemp, 1024, "%d", GetItemDur() );		 
		 return szTemp;	 
	}
	 //价格
	else if(_stricmp(szValueName, ITEMATTRIBUTE_PRICE) == 0)
	{
		if (-1 == GetItemPrice())
		{
			return "";
		}
		_snprintf(szTemp, 1024, "%d",GetItemPrice());
		return szTemp;
	}	
	//星级
	else if(_stricmp(szValueName, ITEMATTRIBUTE_STARTLEVEL) == 0)
	{
		if(-1 != GetStartLevel())
		{
			_snprintf(szTemp, 1024, "%d", GetStartLevel());

			return szTemp;
		}
		return "";
	}
	 //作者
	else if(_stricmp(szValueName, ITEMATTRIBUTE_PRODUCER) == 0)
	{
		
		return	GetManufacturer();
					
	}

	//宝石数量
	else if(_stricmp(szValueName, ITEMATTRIBUTE_GEMSNUM) == 0)
	{
		if ( 0 != GetGemCount() )
		{
			_snprintf(szTemp, 1024, "%d", GetGemCount());
			return szTemp;
		} 
		return "";
	}

	//装备类型 
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_TYPE) == 0)
	{

		return	GetItemTableTypeDesc();

	}
 					
	//装备绑定信息
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_BINDINFO) == 0)
	{

		_snprintf(szTemp, 1024, "%d",GetItemBindInfo());		 
		return szTemp;
	}
	 						
	//所有装备基本属性
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_BASEATT) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;
			GetAllEquipBaseAttribute(strValue);
			return strValue;
		}
	}


	//所有装备附加属性
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_ADDATT) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;

			GetAllEquipAddAttribute(strValue);
			return strValue;
		}
	}
	//所有装备附加技能属性
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_SKILL_ATTR) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;
			GetAllEquipSkillAttr(strValue);
			return strValue;
		}
	}

	//装备宝石的属性
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_GEMSATT) == 0)
	{
		 
			STRING strValue;
			for (int i = 0 ;i < GetGemCount() ;++i)
			{
				GetGemExtAttr(i,strValue);
			}
			
			return strValue;
		
	}
      //装备的永久（附魔）属性信息
 
	else if (_stricmp(szValueName,ITEMATTRIBUTE_EQUIP_FOREVERATTRINFO) == 0)
	{
		return "";
	}
	//装备的临时（附魔）属性信息
	else if (_stricmp(szValueName,ITEMATTRIBUTE_EQUIP_TEMPATTRINFO) == 0)
	{
		return "";
	}

	///装备的套装组合信息
	else if (_stricmp(szValueName,ITEMATTRIBUTE_EQUIP_TEMPATTRINFO) == 0)
	{
		return "";
	}
	//套装激活的属性
	else if (_stricmp(szValueName,ITEMATTRIBUTE_SET_ACCTIVEATTR) == 0)
	{
		return "";
	}
 				
 				
	  //详细信息
	else if (_stricmp(szValueName,ITEMATTRIBUTE_DESC) ==0)
	{
			return GetExtraDesc();
	}
	//掉落颜色
	else if (_stricmp(szValueName,ITEMATTRIBUTE_DROP_COLOR) ==0)
	{

		return GetDropVisColor();
	}


	//装备名字 
	else if (_stricmp(szValueName,ITEMATTRIBUTE_NAME) ==0)
	{
		return CObject_Item::GetAttributeValue(szValueName);
	}

	return "";
	
}


// 得到玩家使用这个物品需要的等级
INT	CObject_Item_Equip::GetNeedLevel()
{
   if (m_theBaseDef.pDefineEquip)
   {
	   return  m_pExtraDefine ->m_nLevelNeed;
   }

     return -1;
}

// 得到物品耐久度
INT	CObject_Item_Equip::GetItemDur()
{
	
	return GetDur();
	
}

// 得到物品最大耐久度
INT	CObject_Item_Equip::GetItemMaxDur()
{
	
	return GetMaxDur();
	
}


// 得到物品的绑定信息 
INT	CObject_Item_Equip::GetItemBindInfo()
{
	// -1 无绑定信息
	if(m_pExtraDefine)
	{
		return m_pExtraDefine->m_nEquipBindInfo;
	}
	
	return -1;
}

// 得到物品的绑定信息 
INT	CObject_Item_Equip::GetItemSecLevelBindInfo()
{
	// -1 无绑定信息
	if(m_pExtraDefine)
	{
		return m_pExtraDefine->m_nEquipBindInfo2;
	}

	return -1;
}


//绑定加成信息
LPCTSTR	CObject_Item_Equip::GetItemBindAddInfo()
{
	
	 
	//打开数据表 Equip_bind.tab
	DBC_DEFINEHANDLE(s_Equip_Bind, DBC_EQUIP_BINDING_INFO);
		//搜索纪录
	const _DBC_EQUIP_BINDING_INFO* pGEquipBind = (const _DBC_EQUIP_BINDING_INFO*)s_Equip_Bind->Search_Index_EQU((UINT) GetItemSecLevelBindInfo() );
	if(pGEquipBind) 
		{
			 
		CHAR szAtt[MAX_PATH] = {0};
		CHAR szAttachAtt[2048] = {0};
	 
		 _ITEM_ATTR  att ;

		 float   fIntensifyInfo    = pGEquipBind->nIntensifyInfo;
 
		 att.m_Value.m_Value =  fIntensifyInfo;
		if (m_theBaseDef.pDefineEquip)
		{
				 if (att.m_Value.m_Value > 0 )
				 {
					if (m_theBaseDef.m_Base.m_nBase_Atack_Near > 0 )
					{
							att.m_AttrType = IATTRIBUTE_BASE_ATTACK_NEAR;  // 基础近程攻击
							att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Atack_Near *  fIntensifyInfo)/100.f + 0.5f  ;
							SetAttri(att, szAtt);
							strncat(szAttachAtt, szAtt, 1024);

					}
					
					if (m_theBaseDef.m_Base.m_nBase_Atack_Far > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_ATTACK_FAR;   // 基础远程攻击	 
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Atack_Far *  fIntensifyInfo )/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}

					if ( m_theBaseDef.m_Base.m_nBase_Atack_Magic > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR;   //  基础魔法攻击
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Atack_Magic *  fIntensifyInfo)/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}

					if (m_theBaseDef.m_Base.m_nBase_Def_Near > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_NEAR;    // 基础近程防御
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Def_Near *  fIntensifyInfo )/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}
					if (m_theBaseDef.m_Base.m_nBase_Def_Far > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_FAR;    // 基础远程防御	
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Def_Far *  fIntensifyInfo )/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}

					if (m_theBaseDef.m_Base.m_nBase_Def_Magic > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR;   // 基础魔法防御		
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Def_Magic *  fIntensifyInfo )/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}


				}

		 }

		
		static TCHAR bufInfo[1024];
		memset(bufInfo, 0, sizeof(bufInfo));
		_stprintf(bufInfo, _T("%s"), szAttachAtt);
		return bufInfo;

		}
  

	return "";

}

// 得到物品卖给npc的价格
INT	CObject_Item_Equip::GetItemPrice()
{

	if (m_theBaseDef.pDefineEquip)
	{
		return	m_theBaseDef.pDefineEquip->nBasePrice;
	}
	return -1;
}
//得到物品职业
INT  CObject_Item_Equip::GetItemProfession() 
{
	if (m_theBaseDef.pDefineEquip)
	{
		return	m_theBaseDef.pDefineEquip->nProfessionalReq;
	}
	return -1;

}
//得到装备目前打孔的数目
INT		CObject_Item_Equip::GetEquipHoleNum()

{
	if(m_pExtraDefine)
	{
		int HoleNum = m_pExtraDefine->m_nEquipHoleNum;
		if (HoleNum < 0 ||HoleNum > GetEquipHoleMaxNum() )
		{
			HoleNum = 0;
		}
		return HoleNum;
	}

	return 0;
	

}
INT		CObject_Item_Equip::GetEquipHoleMaxNum()

{

	if (m_theBaseDef.pDefineEquip)
	{
		return	m_theBaseDef.pDefineEquip->nMaxHoleNum;
	}

	return 0;

}



//得到装备套装激活的属性信息
LPCTSTR		CObject_Item_Equip::GetOtherEquipSetActiveAttr()
{


	CDataPool::GetMe()->OtherPlayerEquip_IsUnion();
	INT  setId = GetSetID();
	if (setId == -1)
		return "";

	if  (  0  == CDataPool::GetMe()->AllSetEquipName.count( setId))
	{
		return "";
	}

	INT nCurrentActiveEquipNum = 0;
	for(INT i =0 ; i<CDataPool::GetMe()->m_OtherActiveSetNum;++i )
	{

		if ( setId == CDataPool::GetMe()->m_OtherActiveSetId[i] )
		{
			nCurrentActiveEquipNum = CDataPool::GetMe()->m_OtherActiveSetEquipNUm[i];
			break;
		}

	}
	if (!nCurrentActiveEquipNum)
	{
		return "";
	}

	CHAR szAtt[MAX_PATH] = {0};
	CHAR szAttachAtt[2048] = {0};

	_snprintf(
		szAtt, 
		MAX_PATH, 
		"\n#c00ff00%s :\n ",                        
		"套装属性加成"
		);

	strncat(szAttachAtt, szAtt, 1024);

	_ITEM_ATTR  att ;
	for( INT i = 0 ; i< EQUIP_SET_ATTR_NUM;++i)

	{

		att.m_AttrType = ITEM_ATTRIBUTE( CDataPool::GetMe()->AllSetEquipName[setId].m_stAttr[i].nAttrId);
		att.m_Value.m_Value  = CDataPool::GetMe()->AllSetEquipName[setId].m_stAttr[i].nAttrNum;
		INT nActiveAttrNeedNum = CDataPool::GetMe()->AllSetEquipName[setId].m_nAttrActiveNum[i];
		if (att.m_AttrType  >= 0 &&
			nActiveAttrNeedNum <= nCurrentActiveEquipNum&&
			att.m_Value.m_Value >0)
		{	
			SetAttri(att, szAtt);
			strncat(szAttachAtt, szAtt, 1024);
		}

	}
 
	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), szAttachAtt);
	return bufInfo;

}

	//得到装备套装激活的属性信息
LPCTSTR		CObject_Item_Equip::GetEquipSetActiveAttr()

{
	 CDataPool::GetMe()->UserEquip_IsUnion();


	INT  setId = GetSetID();
	if (setId == -1)
		return "";

	if  (  0  == CDataPool::GetMe()->AllSetEquipName.count( setId))
	{
		return "";
	}

	INT nCurrentActiveEquipNum = 0;
	for(INT i =0 ; i<CDataPool::GetMe()->m_SelfActiveSetNum;++i )
	{

		if ( setId == CDataPool::GetMe()->m_SelfActiveSetId[i] )
		{
				nCurrentActiveEquipNum = CDataPool::GetMe()->m_SelfActiveSetEquipNum[i];
				break;
		}
			
	}
	if (!nCurrentActiveEquipNum)
	{
		return "";
	}

	CHAR szAtt[MAX_PATH] = {0};
	CHAR szAttachAtt[2048] = {0};

	_ITEM_ATTR  att ;
	BOOL haveAttr = FALSE;
	for( INT i = 0 ; i< EQUIP_SET_ATTR_NUM;++i)

	{

		att.m_AttrType = ITEM_ATTRIBUTE( CDataPool::GetMe()->AllSetEquipName[setId].m_stAttr[i].nAttrId);
		att.m_Value.m_Value  = CDataPool::GetMe()->AllSetEquipName[setId].m_stAttr[i].nAttrNum;
		INT ActiveAttrNeedNum = CDataPool::GetMe()->AllSetEquipName[setId].m_nAttrActiveNum[i];
		if (att.m_AttrType  >= 0 &&
			att.m_Value.m_Value >0&&
			ActiveAttrNeedNum <= nCurrentActiveEquipNum)
		{	if (att.m_Value.m_Value >0)
			{
				SetAttri(att, szAtt);
				strncat(szAttachAtt, szAtt, 1024);
				haveAttr = TRUE;
			}
		}
			
	}

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	if (haveAttr)
	{
		_snprintf(
			szAtt, 
			MAX_PATH, 
			"\n#c00ff00%s :\n",                        
			"套装属性加成"
			);
		_stprintf(bufInfo, _T("%s%s"), szAtt, szAttachAtt);
	}
	
	return bufInfo;
	

}


//得到装备套装信息   
LPCTSTR		CObject_Item_Equip::GetOtherEquipSetInfo()
{
	INT  setId = GetSetID();
	if (setId == -1)
		return "";

	if(   0 ==	 CDataPool::GetMe()->AllSetEquipList.count(setId))  

		return "没有该套装信息";

	if(   0 ==	 CDataPool::GetMe()->AllSetInfo.count(setId))

		return "没有该套装信息";
	if(   0 ==	 CDataPool::GetMe()->AllSetEquipName.count(setId))

		return "没有该套装信息";



	CHAR szAtt[MAX_PATH] = {0};
	CHAR szAttachAtt[2048] = {0};


	INT  setNum = CDataPool::GetMe()->AllSetInfo[setId];
	INT  ActsetNum = 0;
	BOOL isHas = FALSE;


	strncat(szAttachAtt, szAtt, 1024);
	//打开数据表
	DBC_DEFINEHANDLE(s_pItem_Visual, DBC_ITEM_EQUIP_VISUAL);
	//搜索纪录
	const _DBC_ITEM_EQUIP_VISUAL* pVisual =NULL;

	for (INT   idTable  =  0 ,i = 0 ; i< CDataPool::GetMe()->AllSetEquipList[setId].nValue  ;i++) 
	{

		idTable =	CDataPool::GetMe()->AllSetEquipList[setId].nIdList[i] ;

		pVisual =(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_Visual->Search_Index_EQU(idTable);

		if(!pVisual) continue ;

		isHas  = FALSE;
		for ( INT i = HEQUIP_MAINHAND ; i< HEQUIP_SUIT;++i)
		{
			if( CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( i )))

				if( CDataPool::GetMe()->OtherPlayerEquip_GetItem( HUMAN_EQUIP( i ))->GetIdTable() == idTable)
				{
					isHas  = TRUE;
					ActsetNum ++;
					break;
				}

		}

		if (isHas)
		{
			_snprintf(
				szAtt, 
				MAX_PATH, 
				"#c00ff00%s ",                        
				pVisual->szName
				);
		}else
		{
			_snprintf(
				szAtt, 
				MAX_PATH, 
				"#cff0000%s ",                        
				pVisual->szName
				);
		}

		strncat(szAttachAtt, szAtt, 1024);
	}

	_snprintf(
		szAtt, 
		MAX_PATH, 
		"#c00ff00%s( %d/%d )\n",                        
		CDataPool::GetMe()->AllSetEquipName[setId].m_Name.c_str(),
		ActsetNum,
		setNum
		);

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s%s"), szAtt,szAttachAtt);
	return bufInfo;
}
//得到其他玩家装备大激活的信息

LPCSTR				CObject_Item_Equip::GetOtherEquipSpriteAllActiveInfo()
{

	CDataPool::GetMe()->OtherUserEquipSprite_IsActiveAll();

	if (CDataPool::GetMe()->m_OtherAllSprite_Active == 0)
	{
		return "";
	}else
	{
		//打开数据表//chain_info.tab

		CHAR szAtt[MAX_PATH] = {0};
		CHAR szAttachAtt[1024] = {0};
		_ITEM_ATTR  attr ;

		DBC_DEFINEHANDLE(s_pCardAll_Equip, DBC_EQUIP_CARD_ALL);
		//搜索纪录
		_DBC_EQUIP_CARD_ALL* pEquipCardAll = ( _DBC_EQUIP_CARD_ALL*)s_pCardAll_Equip->Search_Index_EQU((UINT)CDataPool::GetMe()->m_SelfAllSprite_Active);

		if(pEquipCardAll)
		{

			for ( INT i = 0 ; i <EQUIP_CARD_ACTIVE_ATTR_NUM ;++i )
			{

				if (pEquipCardAll->stAttr[i].nAttrNum >0)
				{
					attr.m_AttrType  =  pEquipCardAll->stAttr[i].nAttrId;
					attr.m_Value.m_Value  =	pEquipCardAll->stAttr[i].nAttrNum;
					SetAttri(attr, szAtt);
					strncat(szAttachAtt, szAtt, 1024);
				}
			}


			static TCHAR bufInfo[1024];
			memset(bufInfo, 0, sizeof(bufInfo));
			_stprintf(bufInfo, _T("%s"), szAttachAtt);
			return bufInfo;

		}


		return "";

	}

}


//得到装备大激活的信息
LPCSTR				CObject_Item_Equip::GetEquipSpriteAllActiveInfo()
{

	CDataPool::GetMe()->UserEquipSprite_IsActiveAll();

	if (CDataPool::GetMe()->m_SelfAllSprite_Active == 0)
	{
		return "";
	}else
	{
		//打开数据表//chain_info.tab

		CHAR szAtt[MAX_PATH] = {0};
		CHAR szAttachAtt[1024] = {0};
		 _ITEM_ATTR  attr ;

		DBC_DEFINEHANDLE(s_pCardAll_Equip, DBC_EQUIP_CARD_ALL);
		//搜索纪录
		_DBC_EQUIP_CARD_ALL* pEquipCardAll = ( _DBC_EQUIP_CARD_ALL*)s_pCardAll_Equip->Search_Index_EQU((UINT)CDataPool::GetMe()->m_SelfAllSprite_Active);

		if(pEquipCardAll)
		{

			for ( INT i = 0 ; i <EQUIP_CARD_ACTIVE_ATTR_NUM ;++i )
			{

				if (pEquipCardAll->stAttr[i].nAttrNum >0)
				{
					attr.m_AttrType  =  pEquipCardAll->stAttr[i].nAttrId;
					attr.m_Value.m_Value  =	pEquipCardAll->stAttr[i].nAttrNum;
					SetAttri(attr, szAtt);
					strncat(szAttachAtt, szAtt, 1024);
				}
			}
			

			static TCHAR bufInfo[1024];
			memset(bufInfo, 0, sizeof(bufInfo));
			_stprintf(bufInfo, _T("%s"), szAttachAtt);
			return bufInfo;

		}


		return "";

	}

}

//得到装备牌型激活的信息
LPCSTR		CObject_Item_Equip::GetEquipSpritePaiInfo()
{

 

	DBC_DEFINEHANDLE(s_pCard_Equip, DBC_EQUIP_CARD_INFO);
	//搜索纪录
	_DBC_EQUIP_CARD_INFO* pEquipCard  = ( _DBC_EQUIP_CARD_INFO*)s_pCard_Equip->Search_Index_EQU((UINT)GetEquipCard());

	if (pEquipCard)
	{
		CHAR szAtt[MAX_PATH] = {0};
		CHAR szAttachAtt[1024] = {0};
		_ITEM_ATTR  attr ;


		if (pEquipCard->nAttrNum >0)
		{
			attr.m_AttrType   =  pEquipCard->nAttrId;
			attr.m_Value.m_Value  =	pEquipCard->nAttrNum;
			SetAttri(attr, szAtt);
			strncat(szAttachAtt, szAtt, 1024);
		}
		static TCHAR bufInfo[1024];
		memset(bufInfo, 0, sizeof(bufInfo));
		_stprintf(bufInfo, _T("%s"), szAttachAtt);
		return bufInfo;

	}
    

	return "";

}

//得到其他玩家装备牌型激活的信息 

LPCSTR		CObject_Item_Equip::GetOtherEquipSpriteChainInfo()


{
	//饰品 1 饰品2  腰					 0
	//帽  项链 护腕                      1
	//戒指1 戒指2  鞋子					 2		
	//肩 衣服 手套						 3
	// 主手 副手						 4
	EM_EQUIP_CARD_GROUP tempIndex = EM_EQUIP_CARD_GROUP_INVAILD; 
	switch(GetItemType())
	{

	case	HEQUIP_ADORN1:
	case	HEQUIP_ADORN2:	
	case	HEQUIP_WAIST:

		tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN1;
		break;

	case		HEQUIP_HEAD:
	case		HEQUIP_NECK:
	case		HEQUIP_WRIST:

		tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN2; 

		break;

	case 		HEQUIP_RING1:
	case		HEQUIP_RING2:
	case		HEQUIP_FEET:

		tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN3;
		break;
	case		HEQUIP_SHOULDER:
	case		HEQUIP_BACK:
	case		HEQUIP_HAND:
		tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN4;
		break;


	case	HEQUIP_MAINHAND:
	case	HEQUIP_ASSIHAND:

		tempIndex  = EM_EQUIP_CARD_GROUP_JIANG;
		break;

	}
	if(   CDataPool::GetMe()->OtherUserEquipSprite_IsActive(tempIndex))  
	{


		INT idTable = CDataPool::GetMe()->m_OterSpritActive[tempIndex];


		//打开数据表//chain_info.tab

		DBC_DEFINEHANDLE(s_pCardChain_Equip, DBC_EQUIP_CARD_GROUP_INFO);
		//搜索纪录
		_DBC_EQUIP_CARD_GROUP_INFO* pEquipCardChain  = ( _DBC_EQUIP_CARD_GROUP_INFO*)s_pCardChain_Equip->Search_Index_EQU((UINT)idTable);

		if (pEquipCardChain)
		{ 
			CHAR szAtt[MAX_PATH] = {0};
			CHAR szAttachAtt[1024] = {0};
			_ITEM_ATTR  attr ;



			if (pEquipCardChain->nAttrNum >0)
			{
				attr.m_AttrType  =  pEquipCardChain->nAttrId;
				attr.m_Value.m_Value  =	 pEquipCardChain->nAttrNum;
				SetAttri(attr, szAtt);
				strncat(szAttachAtt, szAtt, 1024);
			}
			static TCHAR bufInfo[1024];
			memset(bufInfo, 0, sizeof(bufInfo));
			_stprintf(bufInfo, _T("%s"), szAttachAtt);
			return bufInfo;


		}else
		{
			return "";
		}


	}


	return "";
 

}

//得到装备小激活的信息
LPCSTR				CObject_Item_Equip::GetEquipSpriteChainInfo()
{
	//饰品 1 饰品2  腰					 0
	//帽  项链 护腕                      1
	//戒指1 戒指2  鞋子					 2		
	//肩 衣服 手套						 3
	// 主手 副手						 4
	EM_EQUIP_CARD_GROUP tempIndex = EM_EQUIP_CARD_GROUP_INVAILD; 
	switch(GetItemType())
	{
		 	
		case	HEQUIP_ADORN1:
		case	HEQUIP_ADORN2:	
		case	HEQUIP_WAIST:

				tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN1;
				break;

		case		HEQUIP_HEAD:
		case		HEQUIP_NECK:
		case		HEQUIP_WRIST:

					tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN2; 

					break;

		case 		HEQUIP_RING1:
		case		HEQUIP_RING2:
		case		HEQUIP_FEET:

					 tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN3;
			break;
		case		HEQUIP_SHOULDER:
		case		HEQUIP_BACK:
		case		HEQUIP_HAND:
					tempIndex  = EM_EQUIP_CARD_GROUP_CHAIN4;
			break;
		 

		case		HEQUIP_MAINHAND:
		case		HEQUIP_ASSIHAND:

					tempIndex  = EM_EQUIP_CARD_GROUP_JIANG;
		break;

	}
	if(   CDataPool::GetMe()->UserEquipSprite_IsActive(tempIndex))  
	{

		
		INT idTable = CDataPool::GetMe()->m_SelfSpritActive[tempIndex];
	 


		//打开数据表//chain_info.tab
	 
		DBC_DEFINEHANDLE(s_pCardChain_Equip, DBC_EQUIP_CARD_GROUP_INFO);
		//搜索纪录
		_DBC_EQUIP_CARD_GROUP_INFO* pEquipCardChain  = ( _DBC_EQUIP_CARD_GROUP_INFO*)s_pCardChain_Equip->Search_Index_EQU((UINT)idTable);

        if (pEquipCardChain)
        { 
			CHAR szAtt[MAX_PATH] = {0};
			CHAR szAttachAtt[1024] = {0};
			_ITEM_ATTR  attr ;

 

		   if (pEquipCardChain->nAttrNum >0)
				{
					  attr.m_AttrType  =  pEquipCardChain->nAttrId;
					  attr.m_Value.m_Value  =	 pEquipCardChain->nAttrNum;
					  SetAttri(attr, szAtt);
					  strncat(szAttachAtt, szAtt, 1024);
				 }
			 static TCHAR bufInfo[1024];
			 memset(bufInfo, 0, sizeof(bufInfo));
			_stprintf(bufInfo, _T("%s"), szAttachAtt);
			 return bufInfo;


        }else
		{
			return "";
		}



	}
 

	return "";


}




//得到装备套装信息
LPCTSTR		CObject_Item_Equip::GetEquipSetInfo()

{

	 INT  setId = GetSetID();
	 if (setId == -1)
		return "";

	 if(   0 ==	 CDataPool::GetMe()->AllSetEquipList.count(setId))  

		 return "没有该套装信息";

	 if(   0 ==	 CDataPool::GetMe()->AllSetInfo.count(setId))

		 return "没有该套装信息";
	 if(   0 ==	 CDataPool::GetMe()->AllSetEquipName.count(setId))

		 return "没有该套装信息";

 

	 CHAR szAtt[MAX_PATH] = {0};
	 CHAR szAttachAtt[2048] = {0};


	 INT  setNum = CDataPool::GetMe()->AllSetInfo[setId];
     INT  ActsetNum = 0;
	 BOOL isHas = FALSE;

	strncat(szAttachAtt, szAtt, 1024);
	 //打开数据表
	 DBC_DEFINEHANDLE(s_pItem_Visual, DBC_ITEM_EQUIP_VISUAL);
	 //搜索纪录
	 const _DBC_ITEM_EQUIP_VISUAL* pVisual =NULL;

	for (INT   idTable  =  0 ,i = 0 ; i< CDataPool::GetMe()->AllSetEquipList[setId].nValue  ;i++) 
	{
		  
		idTable =	CDataPool::GetMe()->AllSetEquipList[setId].nIdList[i] ;

		pVisual =(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_Visual->Search_Index_EQU(idTable);

				 if(!pVisual) continue ;
				
		 		 isHas  = FALSE;
				 for ( INT i = HEQUIP_MAINHAND ; i< HEQUIP_SUIT;++i)
				 {
					 if( CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( i )))

						 if( CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( i ))->GetIdTable() == idTable)
					 {
							isHas  = TRUE;
							ActsetNum ++;
							break;
					 }

				 }

				 if (isHas)
				 {
					 _snprintf(
						 szAtt, 
						 MAX_PATH, 
						 "#c00ff00%s\n",                        
						 pVisual->szName
						 );
				 }else
				 {
					 _snprintf(
						 szAtt, 
						 MAX_PATH, 
						 "#cff0000%s\n",                        
						 pVisual->szName
						 );
				 }
				
				strncat(szAttachAtt, szAtt, 1024);
		}
	
	//去掉最后的\n
	size_t len = strlen( szAttachAtt);
	if( len > 2)
	{
		szAttachAtt[len-2] = 0;
	}

	_snprintf(
		szAtt, 
		MAX_PATH, 
		"#c00ff00%s( %d/%d )\n",                        
		CDataPool::GetMe()->AllSetEquipName[setId].m_Name.c_str(),
		ActsetNum,
		setNum
		);

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s%s"), szAtt, szAttachAtt);
	return bufInfo;
 
}




//设置装备目前打孔的数目

VOID	CObject_Item_Equip::SetEquipHoleNum( INT num)
{

	if (num < 0 || num >7)
	{
		return ;
	}
	if(m_pExtraDefine)
	{
		 m_pExtraDefine->m_nEquipHoleNum = num;
	}


}


// 得到物品的制作人
LPCTSTR	CObject_Item_Equip::GetManufacturer()
{
	return _T("");
}


// 得到装备的基本属性
LPCTSTR CObject_Item_Equip::GetEquipBaseAttrInfo()
{
	STRING strValue("");
	GetAllEquipBaseAttribute(strValue);

	if(_T("") == strValue)
	{
		return _T("");
	}

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), strValue.c_str());
	return bufInfo;

}

// 得到装备属性
LPCTSTR	CObject_Item_Equip::GetEquipAddAttrInfo()
{
	STRING strValue("");
	GetAllEquipAddAttribute(strValue);

	if(_T("") == strValue)
	{
		return _T("");
	}

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), strValue.c_str());
	return bufInfo;
}




// 得到装备属性
LPCTSTR		CObject_Item_Equip::GetEquipSkillInfo()
{

	STRING strValue("");
	GetAllEquipSkillAttr(strValue);

	if(_T("") == strValue)
	{
		return _T("");
	}

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), strValue.c_str());
	return bufInfo;


}
// 格式化属性字符串
void CObject_Item_Equip::SetAttri(const _ITEM_ATTR& att, char* const pszBuf)
{

	switch(att.m_AttrType)
		{

		case IATTRIBUTE_RATE_STR:               //力量百分比 
		case IATTRIBUTE_RATE_DEX:				//敏捷百分比:
		case IATTRIBUTE_RATE_INT:				//智力百分比:
		case IATTRIBUTE_RATE_CON:		        //体质百分比
		case IATTRIBUTE_RATE_ALL_BASEATT:		//所有属性百分比
		case IATTRIBUTE_RATE_ATTACK_MAGIC_NEAR:	//近程内功百分比
		case IATTRIBUTE_RATE_ATTACK_NEAR:		//近攻百分比
		case IATTRIBUTE_RATE_ATTACK_FAR:		//远攻百分比
		case IATTRIBUTE_RATE_DEFENCE_MAGIC_NEAR://近程内功百分比
		case IATTRIBUTE_RATE_DEFENCE_NEAR:	    //近防百分比
		case IATTRIBUTE_RATE_DEFENCE_FAR:		//远防百分比:
		case IATTRIBUTE_RATE_MAXHP:			    //生命值上限百分比
		case IATTRIBUTE_RATE_MAXMP:			    //魔法值上限百分比
		case IATTRIBUTE_RATE_GOLD_ATTACK:		 //金攻击百分比
		case IATTRIBUTE_RATE_FIRE_ATTACK:		//火攻击百分比
		case IATTRIBUTE_RATE_WOOD_ATTACK:	 	//木攻击百分比
		case IATTRIBUTE_RATE_WATER_ATTACK: 		//水攻击百分比
		case IATTRIBUTE_RATE_GOLD_RESIST: 		//金防御百分比
		case IATTRIBUTE_RATE_FIRE_RESIST:		//火防御百分比
		case IATTRIBUTE_RATE_WOOD_RESIST:	 	//木防御百分比
		case IATTRIBUTE_RATE_WATER_RESIST:		//水防御百分比
		case IATTRIBUTE_BASE_RATE_ATTACK_NEAR:	   //基础近程攻击百分比
		case IATTRIBUTE_BASE_RATE_ATTACK_FAR:		//基础远程攻击百分比
		case IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR://基础近程内功攻击百分比
		case IATTRIBUTE_BASE_RATE_DEFENCE_ENEAR:   //基础近程防御百分比
		case IATTRIBUTE_BASE_RATE_DEFENCE_FAR:	   //基础远程防御百分比
		case IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_NEAR://基础近程内功防御百分比
		//case IATTRIBUTE_RATE_RAGE_RESTORE:		//怒气回复百分比
		case IATTRIBUTE_RATE_MAXRAGE:				//怒气上限百分比
	
			{
				if(att.m_Value.m_Value > 0)             // 例如       力量+5%
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s} + %d%%\n",                        
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				else                                     // 例如       力量-5%
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s}- %d%%\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				break;
			}
		case	IATTRIBUTE_RATE_RESIST_ALL:				// 按百分比抵消所有属性攻击	
		case	IATTRIBUTE_RATE_NEARATT_REDUCE:			//按百分比抵消近程攻击
		case	IATTRIBUTE_RATE_FARATT_REDUCE:			//按百分比抵消远程攻击
		case	IATTRIBUTE_RATE_MAGIC_NEAR_ATT_REDUCE:	//按百分比抵消近程内功攻击
		case	IATTRIBUTE_RATE_GOLD_ATT_REDUCE:		//按百分比抵消金攻击
		case	IATTRIBUTE_RATE_FIREATT_REDUCE:			//按百分比抵消火攻击
		case	IATTRIBUTE_RATE_WOOD_ATT_REDUCE:		//按百分比抵消木攻击
		case	IATTRIBUTE_RATE_WATER_REDUCE:			//按百分比抵消水攻击
												
			{											// 例如 2%抵消所有属性攻击                       
				_snprintf(
							pszBuf,      
							MAX_PATH, 
							"%d%%#{%s}\n", 
							att.m_Value.m_Value,
							g_szAttachAttName[att.m_AttrType]
							);
				break;
			}
		//case IATTRIBUTE_RATE_ATTACK_SPEED:		//减攻击速度几率
		//case IATTRIBUTE_RATE_MOVE_REDUCE:		//减移动速度几率
		//case IATTRIBUTE_RATE_SKILL_SPEED:		//减施法速度几率
		//case IATTRIBUTE_RATE_DREAD:				//恐惧几率
		//case IATTRIBUTE_TIME_DREAD:			//恐惧时间
		//case IATTRIBUTE_RATE_COMA:				//昏迷几率
		//case IATTRIBUTE_TIME_COMA:			//昏迷时间
		//case IATTRIBUTE_RATE_HUSH:				//沉默几率
		//case IATTRIBUTE_TIME_HUSH:			//沉默时间
		//case IATTRIBUTE_RATE_UNARM:			//缴械几率
		//case IATTRIBUTE_TIME_UNARM:			//缴械时间
		//case IATTRIBUTE_ROLL_RESIST:			//抵抗碾压
		//case IATTRIBUTE_WUSHI_HURT_RESIST:	//抵抗武士职业伤害
		//case IATTRIBUTE_JIANXIA_HURT_RESIST:	//抵抗剑侠职业伤害
		//case IATTRIBUTE_QISHE_HURT_RESIST:	//抵抗骑射手职业伤害
		//case IATTRIBUTE_HUOQIANG_HURT_RESIST:	//抵抗火枪手职业伤害
		//case IATTRIBUTE_XIANZHI_HURT_RESIST:	//抵抗先知职业伤害
		//case IATTRIBUTE_SAMAN_HURT_RESIST:	//抵抗萨满职业伤害
	
		//									     
		//	{
		//		// 例如 减移动速度5%   //恐惧几率 3%
		//		_snprintf(
		//					pszBuf, 
		//					MAX_PATH, 
		//					"#{%s} %d%%\n", 
		//					 g_szAttachAttName[att.m_AttrType], 
		//					att.m_Value.m_Value
		//					);
		//	
		//		break;
		//	}	

		case IATTRIBUTE_BASE_ATTACK_NEAR:			//基础近程攻击
		case IATTRIBUTE_BASE_ATTACK_FAR:			//基础远程攻击
		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:		//基础近程内功攻击
		case IATTRIBUTE_BASE_DEFENCE_NEAR:			//基础近程防御
		case IATTRIBUTE_BASE_DEFENCE_FAR:			//基础远程防御
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:	//基础近程内功防御

			{
				// 例如  基础近程攻击: 10
				_snprintf(
					pszBuf, 
					MAX_PATH, 
					"#{%s} + %d\n", 
					 g_szAttachAttName[att.m_AttrType], 
					att.m_Value.m_Value
					);

				break;
			}
	
		default:
			{
				if(att.m_Value.m_Value > 0)          //例如力量 + 5
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s} + %d\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				else                                 //例如力量 - 5
				{
					_snprintf(    
								pszBuf, 
								MAX_PATH, 
								"#{%s} - %d\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}



				break;
			}

		}
}

//得到是否鉴定
CObject_Item_Equip::EQUIP_ATTRIB	CObject_Item_Equip::GetEquipAttrib()
{
	
	return m_EquipAttrib;
}



// 得到物品表中type一列 
INT	CObject_Item_Equip::GetItemTableType(VOID)
{
	if (m_theBaseDef.pDefineEquip)
	{	
		return m_theBaseDef.pDefineEquip->nType;
	}
	
	return 0; 
}



// 是否是武器   //需要修改
BOOL CObject_Item_Equip::IsWeapon()
{
	if (m_theBaseDef.pDefineEquip)
	{
		return m_theBaseDef.pDefineEquip->nEquipPoint == HEQUIP_MAINHAND || 
			m_theBaseDef.pDefineEquip->nEquipPoint == HEQUIP_ASSIHAND;
	}

	return FALSE;
}

	// 是否是防俱
BOOL CObject_Item_Equip::IsDefence()
{
	if (m_theBaseDef.pDefineEquip)
	{
		switch( m_theBaseDef.pDefineEquip->nEquipPoint )
		{
		case HEQUIP_HEAD	:		//头
		case HEQUIP_SHOULDER:		//肩
		case HEQUIP_BACK	:		//躯干
		case HEQUIP_WRIST	:		//腕
		case HEQUIP_HAND	:		//手
		case HEQUIP_WAIST	:		//腰
		case HEQUIP_FEET	:		//脚
			return TRUE;
		}
	}
	return FALSE;
}

	// 是否是装饰物
BOOL CObject_Item_Equip::IsAppend()
{
	if (m_theBaseDef.pDefineEquip)
	{
		switch( m_theBaseDef.pDefineEquip->nEquipPoint )
		{
		case HEQUIP_NECK:	//颈
		case HEQUIP_RING1:	//戒指1	
		case HEQUIP_RING2:	//戒指2
		case HEQUIP_ADORN1:	//饰品1
		case HEQUIP_ADORN2:	//饰品2
			return TRUE;
		}
	}
	return FALSE;
}

// 得到物品的类型描述 
LPCTSTR	CObject_Item_Equip::GetItemTableTypeDesc()
{
	 
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szType;
		}
	
	 return NULL;
}


//一定要先序列化固定属性
//CHAR* pIn 实际上是
INT	CObject_Item_Equip::EXTRA_DEFINE::WriteVarAttr( CHAR* pIn )
{
	Assert( pIn );

	CHAR* pBuff = pIn;
	pBuff += sizeof( UINT );

	//m_nLevelNeed = *((BYTE*)pBuff);
	//pBuff += sizeof(BYTE);
	//
	//m_PaiXing = *( (BYTE*)pBuff );
	//pBuff += sizeof(BYTE);

	//m_nEquipScore = *( (BYTE*)pBuff );
	//pBuff += sizeof(BYTE);

	//m_StartLevel = *( (BYTE*)pBuff );
	//pBuff += sizeof(BYTE);

	//m_nMaxDurPoint = *( (BYTE*)pBuff );
	//pBuff += sizeof(BYTE);
	m_nAttSet = *( (INT*)pBuff );		//属性组
	pBuff += sizeof( INT );

	m_StartLevel = *( (BYTE*)pBuff );	//星级
	pBuff += sizeof(BYTE);

	m_CurDurPoint = *( (INT*)pBuff );	//当前耐久值 
	pBuff += sizeof(INT);

	m_CurDamagePoint = *( (USHORT*)pBuff );	//当前损伤度
	pBuff += sizeof(USHORT);

	m_GemCount = *( (BYTE*)pBuff );		//镶嵌的宝石个数
	pBuff += sizeof( BYTE );

	m_nEquipBindInfo = *( (EQUIP_BIND_INFO*)pBuff );		//装备是否绑定
	pBuff += sizeof( EQUIP_BIND_INFO );

	m_nEquipBindInfo2 = *( (EQUIP_BIND_INFO*)pBuff );	//装备是否绑定强化(2级绑定)
	pBuff += sizeof( EQUIP_BIND_INFO );

	m_nEquipHoleNum = *( (INT*)pBuff );			//装备以后的孔洞的数目
	pBuff += sizeof( INT );

	m_nEquipScore = *( (INT*)pBuff );			//装备的分数（该分数大于某值后装备产生灵魂属性以及区分装备质量）
	pBuff += sizeof( INT );

	m_nSellPrice = *( (INT*)pBuff );			//售出价格
	pBuff += sizeof( INT );

	m_nLevelNeed = *( (INT*)pBuff );		//需求等级
	pBuff += sizeof(INT);

	m_nMaxDurPoint = *( (INT*)pBuff );		//最大耐久值
	pBuff += sizeof( INT );

	m_SetNum = *( (INT*)pBuff );		//套装id
	pBuff += sizeof( INT );

	m_RulerID = *( (CHAR*)pBuff );		//新规则
	pBuff += sizeof( CHAR );

	m_EquipType = *( (BYTE*)pBuff );		//装备类型
	pBuff += sizeof( BYTE );

	m_AttrCount = *( (BYTE*)pBuff );		//属性的数量
	pBuff += sizeof( BYTE );

	m_PaiXing = *( (INT*)pBuff );		//装备的牌型
	pBuff += sizeof( INT );

	m_nEquipAttachGemNum = *( (BYTE*)pBuff );		//附加宝石的数量
	pBuff += sizeof( BYTE );
	m_vEquipAttachGem.clear();
	for( int i=0; i<m_nEquipAttachGemNum; ++i )		//附加的宝石
	{
		m_vEquipAttachGem.push_back( *( (_ITEM_GEMINFO*)pBuff ) );
		pBuff += sizeof( _ITEM_GEMINFO );
	}

	m_nEquipAttributesNum = *( (BYTE*)pBuff );	//装备的属性的数量
	m_vEquipAttributes.clear();
	pBuff += sizeof( BYTE );	//装备属性
	for( int i=0; i<m_nEquipAttributesNum; ++i )
	{
		m_vEquipAttributes.push_back( *( (_ITEM_ATTR*)pBuff ) );
		pBuff += sizeof( _ITEM_ATTR );
	}

	m_nAffix_SkillNum = *( (BYTE*)pBuff );		//附加的技能的数量
	pBuff += sizeof( BYTE );
	m_Affix_Skill.clear();
	for( int i=0; i<m_nAffix_SkillNum; ++i )	//附加技能
	{
		m_Affix_Skill.push_back( *( (_ITEM_AFFIX_SKILL*)pBuff ) );
		pBuff += sizeof( _ITEM_AFFIX_SKILL );
	}

	m_nEffect_SkillNum = *( (BYTE*)pBuff );	//影响的技能的数量
	pBuff += sizeof( BYTE );
	m_Effect_Skill.clear();
	for( int i=0; i<m_nEffect_SkillNum; ++i )	//影响技能
	{
		m_Effect_Skill.push_back( *( (_ITEM_EFFECT_SKILL*)pBuff ) );
		pBuff += sizeof( _ITEM_EFFECT_SKILL );
	}

	m_nszAttrGroupNum = *( (BYTE*)pBuff );		//属性组名的大小
	pBuff += sizeof( BYTE );
	m_szAttrGroup.clear();
	for ( int i=0; i<m_nszAttrGroupNum; ++i )	//属性组名
	{
		m_szAttrGroup.push_back( *( (CHAR*)pBuff ) );
		pBuff += sizeof( CHAR );
	}

	m_SoulBeadType = *( (BYTE*)pBuff );		//附魔的魂珠类型
	pBuff += sizeof( BYTE );

	m_SoulBeadAttrCount = *( (BYTE*)pBuff );		//魂珠属性的数量
	pBuff += sizeof( BYTE );
	
	return (INT)(pBuff-pIn);

}


BOOL	CObject_Item_Equip::EXTRA_DEFINE::ReadVarAttr( CHAR* pOut,INT& OutLength)
{
	Assert( pOut );
	CHAR *pBuff = pOut;

	*( (INT*)pBuff ) = m_nAttSet;		//属性组
	pBuff += sizeof( INT );

	*( (BYTE*)pBuff ) = m_StartLevel;	//星级
	pBuff += sizeof(BYTE);

	*( (INT*)pBuff ) = m_CurDurPoint;	//当前耐久值 
	pBuff += sizeof(INT);

	*( (USHORT*)pBuff ) = m_CurDamagePoint;	//当前损伤度
	pBuff += sizeof(USHORT);

	*( (BYTE*)pBuff ) = m_GemCount;		//镶嵌的宝石个数
	pBuff += sizeof( BYTE );

	*( (EQUIP_BIND_INFO*)pBuff ) = m_nEquipBindInfo;		//装备是否绑定
	pBuff += sizeof( EQUIP_BIND_INFO );

	*( (EQUIP_BIND_INFO*)pBuff ) = m_nEquipBindInfo2;	//装备是否绑定强化(2级绑定)
	pBuff += sizeof( EQUIP_BIND_INFO );

	*( (INT*)pBuff ) = m_nEquipHoleNum;			//装备以后的孔洞的数目
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_nEquipScore;			//装备的分数（该分数大于某值后装备产生灵魂属性以及区分装备质量）
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_nSellPrice;			//售出价格
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_nLevelNeed;		//需求等级
	pBuff += sizeof(INT);

	*( (INT*)pBuff ) = m_nMaxDurPoint;		//最大耐久值
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_SetNum;		//套装id
	pBuff += sizeof( INT );

	*( (CHAR*)pBuff ) = m_RulerID;		//新规则
	pBuff += sizeof( CHAR );

	*( (BYTE*)pBuff ) = m_EquipType;		//装备类型
	pBuff += sizeof( BYTE );

	*( (BYTE*)pBuff ) = m_AttrCount;		//属性的数量
	pBuff += sizeof( BYTE );

	*( (INT*)pBuff ) = m_PaiXing;		//装备的牌型
	pBuff += sizeof( INT );

	*( (BYTE*)pBuff ) = m_nEquipAttachGemNum;		//附加宝石的数量
	pBuff += sizeof( BYTE );
	for( int i=0; i<m_nEquipAttachGemNum; ++i )		//附加的宝石
	{
		*( (_ITEM_GEMINFO*)pBuff ) = m_vEquipAttachGem[i];
		pBuff += sizeof( _ITEM_GEMINFO );
	}

	*( (BYTE*)pBuff ) = m_nEquipAttributesNum;	//装备的属性的数量
	pBuff += sizeof( BYTE );	//装备属性
	for( int i=0; i<m_nEquipAttributesNum; ++i )
	{
		*( (_ITEM_ATTR*)pBuff ) = m_vEquipAttributes[i];
		pBuff += sizeof( _ITEM_ATTR );
	}

	*( (BYTE*)pBuff ) = m_nAffix_SkillNum;		//附加的技能的数量
	pBuff += sizeof( BYTE );
	for( int i=0; i<m_nAffix_SkillNum; ++i )	//附加技能
	{
		*( (_ITEM_AFFIX_SKILL*)pBuff ) = m_Affix_Skill[i];
		pBuff += sizeof( _ITEM_AFFIX_SKILL );
	}

	*( (BYTE*)pBuff ) = m_nEffect_SkillNum;	//影响的技能的数量
	pBuff += sizeof( BYTE );
	for( int i=0; i<m_nEffect_SkillNum; ++i )	//影响技能
	{
		*( (_ITEM_EFFECT_SKILL*)pBuff ) = m_Effect_Skill[i];
		pBuff += sizeof( _ITEM_EFFECT_SKILL );
	}

	*( (BYTE*)pBuff ) = m_nszAttrGroupNum;		//属性组名的大小
	pBuff += sizeof( BYTE );
	for ( int i=0; i<m_nszAttrGroupNum; ++i )	//属性组名
	{
		*( (CHAR*)pBuff ) = m_szAttrGroup[i];
		pBuff += sizeof( CHAR );
	}

	*( (INT*)pBuff ) = m_SoulBeadType;		//附魔的魂珠类型
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_SoulBeadAttrCount;		//魂珠属性的数量
	pBuff += sizeof( INT );

	OutLength = (INT)(pBuff-pOut);

	return false;
}
