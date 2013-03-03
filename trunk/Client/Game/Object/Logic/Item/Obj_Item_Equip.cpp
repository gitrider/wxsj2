
/**	Obj_Item_Equip.cpp
 *	
 *	���ܣ�	
 *	
 *	�޸ļ�¼��
 *			080221	
			080409    �����װ��Ϣtip  wangzhen
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

				
//IATTRIBUTE_POINT_STR			,	//��������
//IATTRIBUTE_RATE_STR				,	//�����ٷֱ�
//IATTRIBUTE_POINT_DEX			,	//���ݵ���
//IATTRIBUTE_RATE_DEX 			,	//���ݰٷֱ�
//IATTRIBUTE_POINT_INT			,	//��������
//IATTRIBUTE_RATE_INT 			,	//�����ٷֱ�
//IATTRIBUTE_POINT_CON			,	//���ʵ���
//IATTRIBUTE_RATE_CON				,	//���ʰٷֱ�
//IATTRIBUTE_POINT_ALL_BASEATT	,	//�������Ե���
//IATTRIBUTE_RATE_ALL_BASEATT		,	//�������԰ٷֱ�
//IATTRIBUTE_POINT_ATTACK_M		,	//ħ������
//IATTRIBUTE_RATE_ATTACK_M		,	//ħ���ٷֱ�
//IATTRIBUTE_POINT_ATTACK_NEAR	,	//��������
//IATTRIBUTE_RATE_ATTACK_NEAR		,	//�����ٷֱ�
//IATTRIBUTE_POINT_ATTACK_FAR		,	//Զ������
//IATTRIBUTE_RATE_ATTACK_FAR		,	//Զ���ٷֱ�
//IATTRIBUTE_POINT_DEFENCE_M		,	//ħ������
//IATTRIBUTE_RATE_DEFENCE_M		,	//ħ���ٷֱ�
//IATTRIBUTE_POINT_DEFENCE_NEAR	,	//��������
//IATTRIBUTE_RATE_DEFENCE_NEAR	,	//�����ٷֱ�
//IATTRIBUTE_POINT_DEFENCE_FAR	,	//Զ������
//IATTRIBUTE_RATE_DEFENCE_FAR		,	//Զ���ٷֱ�
//IATTRIBUTE_POINT_MAXHP			,	//����ֵ���޵���
//IATTRIBUTE_RATE_MAXHP			,	//����ֵ���ްٷֱ�
//
//IATTRIBUTE_POINT_MAXMP			,	//ħ��ֵ���޵���
//IATTRIBUTE_RATE_MAXMP			,	//ħ��ֵ���ްٷֱ�
//IATTRIBUTE_WUSHI_MAXRAGE		,	//��ʿŭ������
//IATTRIBUTE_JIANXIA_MAXRAGE		,	//����ŭ������
//IATTRIBUTE_QISHE_MAXRAGE		,	//������ŭ������
//IATTRIBUTE_HUOQIANG_MAXRAGE		,	//��ǹ��ŭ������
//IATTRIBUTE_XIANZHI_MAXRAGE		,	//��֪ŭ������
//IATTRIBUTE_SAMAN_MAXRAGE		,	//����ŭ������
//IATTRIBUTE_RATE_WUSHI_MAXRAGE	,	//��ʿŭ�����ްٷֱ�
//IATTRIBUTE_RATE_JIANXIA_MAXRAGE	,	//����ŭ�����ްٷֱ�
//IATTRIBUTE_RATE_QISHE_MAXRAGE	,	//������ŭ�����ްٷֱ�
//IATTRIBUTE_RATE_HUOQIANG_MAXRAGE,	//��ǹ��ŭ�����ްٷֱ�
//IATTRIBUTE_RATE_XIANZHI_MAXRAGE	,	//��֪ŭ�����ްٷֱ�
//IATTRIBUTE_RATE_SAMAN_MAXRAGE	,	//����ŭ�����ްٷֱ�
//IATTRIBUTE_POINT_WUSHI_RAGE_RESTORE	,	//��ʿŭ���ظ���
//IATTRIBUTE_POINT_JIANXIA_RAGE_RESTORE	,	//����ŭ���ظ���
//IATTRIBUTE_POINT_QISHE_RAGE_RESTORE	,	//������ŭ���ظ���
//IATTRIBUTE_POINT_HUOQIANG_RAGE_RESTORE,	//��ǹ��ŭ���ظ���
//IATTRIBUTE_POINT_XIANZHI_RAGE_RESTORE	,	//��֪ŭ���ظ���
//IATTRIBUTE_POINT_SAMAN_RAGE_RESTORE	,	//����ŭ���ظ���
//IATTRIBUTE_RATE_WUSHI_RAGE_RESTORE	,	//��ʿŭ���ظ��ٷֱ�
//IATTRIBUTE_RATE_JIANXIA_RAGE_RESTORE	,	//����ŭ���ظ��ٷֱ�
//IATTRIBUTE_RATE_QISHE_RAGE_RESTORE	,	//������ŭ���ظ��ٷֱ�
//IATTRIBUTE_RATE_HUOQIANG_RAGE_RESTORE,	//��ǹ��ŭ���ظ��ٷֱ�
//IATTRIBUTE_RATE_XIANZHI_RAGE_RESTORE	,	//��֪ŭ���ظ��ٷֱ�
//IATTRIBUTE_RATE_SAMAN_RAGE_RESTORE	,	//����ŭ���ظ��ٷֱ�
//
//IATTRIBUTE_HIT					,	//����
//IATTRIBUTE_MISS					,	//����
//IATTRIBUTE_RATE_HIT				,	//������
//IATTRIBUTE_CRIT					,	//����
//IATTRIBUTE_TOUGHNESS			,	//����
//IATTRIBUTE_RATE_CRIT			,	//������
//IATTRIBUTE_COLD_ATTACK			,	//����������
//IATTRIBUTE_RATE_COLD_ATTACK		,	//�������ٷֱ�
//IATTRIBUTE_FIRE_ATTACK			,	//�𹥻�����
//IATTRIBUTE_RATE_FIRE_ATTACK		,	//�𹥻��ٷֱ�
//IATTRIBUTE_LIGHT_ATTACK			,	//�繥������
//IATTRIBUTE_RATE_LIGHT_ATTACK	,	//�繥���ٷֱ�
//IATTRIBUTE_POISON_ATTACK		,	//����������
//IATTRIBUTE_RATE_POISON_ATTACK	,	//�������ٷֱ�
//IATTRIBUTE_COLD_RESIST			,	//����������
//IATTRIBUTE_RATE_COLD_RESIST  	,	//�������ٷֱ�
//IATTRIBUTE_FIRE_RESIST			,	//���������
//IATTRIBUTE_RATE_FIRE_RESIST		,	//������ٷֱ�
//IATTRIBUTE_LIGHT_RESIST			,	//���������
//IATTRIBUTE_RATE_LIGHT_RESIST	,	//������ٷֱ�
//IATTRIBUTE_POISON_RESIST		,	//����������
//IATTRIBUTE_RATE_POISON_RESIST	,	//�������ٷֱ�
//IATTRIBUTE_RESIST_ALL			,	//�����������������Թ���
//IATTRIBUTE_RATE_RESIST_ALL		,	//���ٷֱȵ����������Թ���
//IATTRIBUTE_RATE_DREAD			,	//�־弸��
//IATTRIBUTE_TIME_DREAD			,	//�־�ʱ��
//IATTRIBUTE_RATE_COMA			,	//���Լ���
//IATTRIBUTE_TIME_COMA			,	//����ʱ��
//IATTRIBUTE_RATE_HUSH			,	//��Ĭ����
//IATTRIBUTE_TIME_HUSH			,	//��Ĭʱ��
//IATTRIBUTE_RATE_UNARM			,	//��е����
//IATTRIBUTE_TIME_UNARM			,	//��еʱ��
//IATTRIBUTE_ROLL_RESIST			,	//�ֿ���ѹ
//IATTRIBUTE_RATE_ATTACK_SPEED	,	//�������ٶȼ���
//IATTRIBUTE_RATE_SKILL_SPEED		,	//��ʩ���ٶȼ���
//IATTRIBUTE_RATE_MOVE_REDUCE		,	//���ƶ��ٶȼ���
//IATTRIBUTE_HURT_DOT				,	//dot�˺�
//IATTRIBUTE_DEBUFF_RESIST		,	//�ֿ�debuff
//IATTRIBUTE_SPEED_MOVE			,	//�ƶ��ٶ�
//IATTRIBUTE_SPEED_SKILL			,	//ʩ���ٶ�
//IATTRIBUTE_ATTACK_SPEED			,	//�����ٶ�
//IATTRIBUTE_MODEL				,	//ģ��
//IATTRIBUTE_COLOR_HAIR			,	//��ɫ
//IATTRIBUTE_MODEL_HAIR			,	//����
//IATTRIBUTE_HP_RESTORE			,	//�����ظ�
//IATTRIBUTE_MP_RESTORE			,	//ħ���ظ�
//IATTRIBUTE_RAGE_RESTORE			,	//ŭ���ظ�
//IATTRIBUTE_CRIT_HURT			,	//���ӱ����˺�
//IATTRIBUTE_SHIELD_ALL			,	//���ж�
//IATTRIBUTE_SHIELD_NEAR			,	//���̶�
//IATTRIBUTE_SHIELD_M				,	//ħ����
//IATTRIBUTE_SHIELD_FAR			,	//Զ�̶�
//IATTRIBUTE_SHIELD_COLD			,	//����
//IATTRIBUTE_SHIELD_FIRE			,	//���
//IATTRIBUTE_SHIELD_LIGHT			,	//���
//IATTRIBUTE_SHIELD_POISON		,	//����
//IATTRIBUTE_SHIELD_MP_REDUCE		,	//���������˺���
//IATTRIBUTE_SKILL_LEVEL_INC		,	//���ӵ������ܵȼ�
//IATTRIBUTE_ALLSKILL_LEVEL_INC	,	//�������м��ܵȼ�
//IATTRIBUTE_WUSHI_HURT_RESIST	,	//�ֿ���ʿְҵ�˺�
//IATTRIBUTE_JIANXIA_HURT_RESIST	,	//�ֿ�����ְҵ�˺�
//IATTRIBUTE_QISHE_HURT_RESIST	,	//�ֿ�������ְҵ�˺�
//IATTRIBUTE_HUOQIANG_HURT_RESIST	,	//�ֿ���ǹ��ְҵ�˺�
//IATTRIBUTE_XIANZHI_HURT_RESIST	,	//�ֿ���ְ֪ҵ�˺�
//IATTRIBUTE_SAMAN_HURT_RESIST	,	//�ֿ�����ְҵ�˺�
//IATTRIBUTE_EXPRENCE_GET			,	//���Ӿ�����
//IATTRIBUTE_MONEY_GET			,	//���ӽ�Ǯ���
//IATTRIBUTE_POINT_ABILITY1		,	//����ͨ�õ���1����
//IATTRIBUTE_RATE_ABILITY1		,	//����ͨ�õ���1�ٷֱ�
//IATTRIBUTE_POINT_ABILITY2		,	//����ͨ�õ���2����
//IATTRIBUTE_RATE_ABILITY2		,	//����ͨ�õ���2�ٷֱ�
//IATTRIBUTE_POINT_ABILITY3		,	//����ͨ�õ���3����
//IATTRIBUTE_RATE_ABILITY3		,	//����ͨ�õ���3�ٷֱ�
//IATTRIBUTE_POINT_ABILITY4		,	//����ͨ�õ���4����
//IATTRIBUTE_RATE_ABILITY4		,	//����ͨ�õ���4�ٷֱ�
//IATTRIBUTE_POINT_ABILITY5		,	//����ͨ�õ���5����
//IATTRIBUTE_RATE_ABILITY5		,	//����ͨ�õ���5�ٷֱ�
//IATTRIBUTE_POINT_ABILITY6		,	//����ͨ�õ���6����
//IATTRIBUTE_RATE_ABILITY6		,	//����ͨ�õ���6�ٷֱ�
//IATTRIBUTE_POINT_ABILITY7		,	//����ͨ�õ���7����
//IATTRIBUTE_RATE_ABILITY7		,	//����ͨ�õ���7�ٷֱ�
//IATTRIBUTE_POINT_ABILITY8		,	//����ͨ�õ���8����
//IATTRIBUTE_RATE_ABILITY8		,	//����ͨ�õ���8�ٷֱ�
//IATTRIBUTE_POINT_ABILITY9		,	//����ͨ�õ���9����
//IATTRIBUTE_RATE_ABILITY9		,	//����ͨ�õ���9�ٷֱ�
//IATTRIBUTE_POINT_ABILITY10		,	//����ͨ�õ���10����
//IATTRIBUTE_RATE_ABILITY10		,	//����ͨ�õ���10�ٷֱ�
//
//
//IATTRIBUTE_BASE_RATE_ATTACK_ENEAR	,   //�������̹����ٷֱ�
//IATTRIBUTE_BASE_RATE_ATTACK_FAR		,   //����Զ�̹����ٷֱ�
//IATTRIBUTE_BASE_RATE_ATTACK_M		,   //����ħ�������ٷֱ�
//IATTRIBUTE_BASE_RATE_DEFENCE_ENEAR	,   //�������̷����ٷֱ�
//IATTRIBUTE_BASE_RATE_DEFENCE_FAR	,   //����Զ�̷����ٷֱ�
//IATTRIBUTE_BASE_RATE_DEFENCE_M		,   //����ħ�������ٷֱ�
//
//IATTRIBUTE_BASE_ATTACK_NEAR		,	//�������̹���
//IATTRIBUTE_BASE_ATTACK_FAR		,	//����Զ�̹���
//IATTRIBUTE_BASE_ATTACK_M		,	//����ħ������
//IATTRIBUTE_BASE_DEFENCE_NEAR	,	//�������̷���
//IATTRIBUTE_BASE_DEFENCE_FAR		,	//����Զ�̷���
//IATTRIBUTE_BASE_DEFENCE_M		,	//����ħ������
//
//IATTRIBUTE_RATE_NEARATT_REDUCE		,	//���ٷֱȵ������̹���
//IATTRIBUTE_RATE_FARATT_REDUCE		,	//���ٷֱȵ���Զ�̹���
//IATTRIBUTE_RATE_MATT_REDUCE			,	//���ٷֱȵ���ħ������
//IATTRIBUTE_RATE_COLDATT_REDUCE		,	//���ٷֱȵ���������
//IATTRIBUTE_RATE_FIREATT_REDUCE		,	//���ٷֱȵ����𹥻�
//IATTRIBUTE_RATE_LIGHTATT_REDUCE		,	//���ٷֱȵ����繥��
//IATTRIBUTE_RATE_POISON_REDUCE		,	//���ٷֱȵ���������
//IATTRIBUTE_NUMBER, //��Ʒ������������



const char* g_szAttachSkillName[] =

{
	     "equip_skill_fire",												 //������ �������� �ȼ�																			
		 "equip_skill_thunder",			        							//�׵�ȼ� 
		 "equip_skill_hit"                                                  //�ػ��ȼ�

};
											
const char* g_szAttachAttName[] = 											
{	                                                                
		"equip_attr_pointstr",												//��������																				
		"equip_attr_ratestr",			        							//�����ٷֱ�
		"equip_attr_pointdex",		     									//���ݵ���
		"equip_attr_ratedex",			     								//���ݰٷֱ�
		"equip_attr_pointint",			    								//��������
		"equip_attr_rateint",		                    					//�����ٷֱ�
		"equip_attr_pointcon",                          					//���ʵ���
		"equip_attr_ratecon",	 											//���ʰٷֱ�
		"equip_attr_allbasepoint",	        								//�������Ե���
		"equip_attr_allbaserate",											//�������԰ٷֱ�
		"equip_attr_pointattack_near",	     								//�����ڹ�����
		"equip_attr_pointattack_far",	     								//�����⹦����
		"equip_attr_rateattack_near",	    								//�����ڹ��ٷֱ�
		"equip_attr_rateattack_far",	    								//�����⹦�ٷֱ�
		"equip_attr_attack_nearpoint",										//��������
		"equip_attr_attack_nearrate",										//�����ٷֱ�
		"equip_attr_attack_farpoint",										//Զ������
		"equip_attr_attack_farrate",	     								//Զ���ٷֱ�
		"equip_attr_defence_mpoint_near",	 								//�����ڹ���������
		"equip_attr_defence_mpoint_far",	 								//�����⹦��������
		"equip_attr_defence_mrate_near",	 								//�����ڹ������ٷֱ�
		"equip_attr_defence_mrate_far",	 									//�����⹦�����ٷֱ�
		"equip_attr_defence_nearpoint",										//��������
		"equip_attr_defence_nearrate",										//�����ٷֱ�
		"equip_attr_defence_farpoint",										//Զ������
		"equip_attr_defence_farrate",										//Զ���ٷֱ�
		"equip_attr_point_maxhp",											//����ֵ���޵���
		"equip_attr_rate_maxhp",											//����ֵ���ްٷֱ�
		"equip_attr_point_maxmp",											//ħ��ֵ���޵���
		"equip_attr_rate_maxmp",											//ħ��ֵ���ްٷֱ�
		"equip_attr_maxyuanqi",												//Ԫ������		
		"equip_attr_maxyuanqi_rate",										//Ԫ�����ްٷֱ�
		"equip_attr_maxrage",												//ŭ������		
		"equip_attr_maxrage_rate",											//ŭ�����ްٷֱ�
		"equip_attr_rageres_point",											//ŭ���ظ���												
		"equip_attr_rageres_rate",											//ŭ���ظ��ٷֱ�
		"equip_attr_hit",													//����
		"equip_attr_miss",													//����
		"equip_attr_rate_hit",												//������
		"equip_attr_sock",			     									//����
		"equip_attr_toughness",			                					//����
		"equip_attr_sock_rate",												//������
		"equip_attr_gold_attack",											//�𹥻�����
		"equip_attr_rate_gold_attack",										//�𹥻��ٷֱ�
		"equip_attr_wood_attack",			 								//ľ��������
		"equip_attr_rate_wood_attack",				 						//ľ�����ٷֱ�
		"equip_attr_water_attack",											//ˮ��������
		"equip_attr_rate_water_attack",										//ˮ�����ٷֱ�
		"equip_attr_fire_attack",											//�𹥻�����
		"equip_attr_rate_fire_attack",										//�𹥻��ٷֱ�
		"equip_attr_soil_attack",											//����������
		"equip_attr_rate_soil_attack",										//�������ٷֱ�
		"equip_attr_gold_resist",											//���������
		"equip_rate_gold_resist",											//������ٷֱ�
		"equip_attr_wood_resist",											//ľ��������
		"equip_rate_wood_resist",											//ľ�����ٷֱ�
		"equip_water_resist",												//ˮ��������
		"equip_rate_water_resist",											//ˮ�����ٷֱ�
		"equip_attr_fire_resist",											//���������
		"equip_attr_rate_fire_resist",										//������ٷֱ�
		"equip_attr_soil_resist",											//����������
		"equip_attr_rate_soil_resist",										//�������ٷֱ�
		"equip_resist_all",													//�����������������Թ���
		"equip_rate_resist_all",											//���ٷֱȵ����������Թ�
		"equip_base_dread_rate",											//�־弸��
		"equip_time_dread",													//�־�ʱ��
		"equip_rate_coma",													//���Լ���
		"equip_time_coma",													//����ʱ��
		"equip_rate_hush",													//��Ĭ����
		"equip_time_hush",													//��Ĭʱ��
		"equip_rate_unarm",					 								//��е����
		"equip_time_unarm",							 						//��еʱ��
		"equip_base_miss",													//�ֿ���ѹ
		"equip_rate_attack_speed",					 						//�������ٶȼ���
		"equip_rate_skill_speed",											//��ʩ���ٶȼ���
		"equip_rate_move_reduce",											//���ƶ��ٶȼ���
		"equip_attr_hurt_dot	",		  									//dot�˺�
		"equip_attr_debuff_resist",					 						//�ֿ�debuff
		"equip_attr_speed_move	",					 						//�ƶ��ٶ�
		"equip_attr_speed_skill",											//ʩ���ٶ�
		"equip_attr_attack_speed",											//�����ٶ�
		"equip_attr_model",													//ģ��
		"equip_attr_color_hair",											//��ɫ
		"equip_attr_model_hair",											//����
		"equip_attr_hp_restore",											//�����ظ�
		"equip_attr_mp_restore",											//ħ���ظ�
		"equip_attr_yuanqi_restore",										//Ԫ���ظ�
		"equip_attr_rage_restore",											//ŭ���ظ�
		"equip_attr_crit_hurt",												//���ӱ����˺�
		"equip_attr_shield_all",											//���ж�
		"equip_attr_shield_near",											//���̶�
		"equip_attr_shield_m_near",											//�����ڹ���
		"equip_attr_shield_m_far",											//�����⹦��
		"equip_attr_shield_far",											//Զ�̶�
		"equip_attr_shield_gold",											//���
		"equip_attr_shield_wood",											//ľ��
		"equip_attr_shield_water",											//ˮ��
		"equip_attr_shield_fire",											//���
		"equip_attr_shield_soil",											//����
		"equip_attr_shield_mp_reduce",										//���������˺���
		"equip_attr_skill_level_inc",										//���ӵ������ܵȼ�
		"equip_attr_allskill_level_inc",									//�������м��ܵȼ�
		"equip_attr_exprence_get",											//���Ӿ�����
		"equip_attr_money_get",				 								//���ӽ�Ǯ���
		"equip_attr_point_ability1",			 							//����ͨ�õ���1����
		"equip_attr_rate_ability1",				 							//����ͨ�õ���1�ٷֱ�
		"equip_attr_point_ability2",										//����ͨ�õ���2����
		"equip_attr_rate_ability2",											//����ͨ�õ���2�ٷֱ�
		"equip_attr_point_ability3",										//����ͨ�õ���3����
		"equip_attr_nodefence",												//����ͨ�õ���3�ٷֱ�
		"equip_attr_rate_ability3",		 									//����ͨ�õ���4����
		"equip_attr_rate_ability4	",										//����ͨ�õ���4�ٷֱ�
		"equip_attr_point_ability5",				 						//����ͨ�õ���5����
		"equip_attr_rate_ability5",											//����ͨ�õ���5�ٷֱ�
		"equip_attr_point_ability6",										//����ͨ�õ���6����
		"equip_attr_rate_ability6",											//����ͨ�õ���6�ٷֱ�
		"equip_attr_point_ability7",										//����ͨ�õ���7����
		"equip_attr_rate_ability7",											//����ͨ�õ���7�ٷֱ�
		"equip_attr_point_ability8",										//����ͨ�õ���8����
		"equip_attr_rate_ability8",											//����ͨ�õ���8�ٷֱ�
		"equip_attr_point_ability9",										//����ͨ�õ���9����
		"equip_attr_rate_ability9",											//����ͨ�õ���9�ٷֱ�
		"equip_point_ability10",											//����ͨ�õ���10����
		"equip_rate_ability10",						 						//����ͨ�õ���10�ٷֱ�	
		"equip_base_rate_attack_near",										//�������̹����ٷֱ�
		"equip_base_rate_attack_far",										//����Զ�̹����ٷֱ�
		"equip_base_rate_attack_m_near",									//���������ڹ������ٷֱ�
		"equip_base_rate_attack_m_far",										//���������⹦�����ٷֱ�
		"equip_base_rate_defence_enear",									//�������̷����ٷֱ�
		"equip_base_rate_defence_far",										//����Զ�̷����ٷֱ�
		"equip_base_rate_defence_m_near",									//���������ڹ������ٷֱ�	
		"equip_base_rate_defence_m_far",									//���������⹦�����ٷֱ�	
		"equip_base_attack_near",											//�������̹���
		"equip_base_attack_far",										    //����Զ�̹���
		"equip_base_attack_m_near",											//���������ڹ�����
		"equip_base_attack_m_far",											//���������⹦����
		"equip_base_defence_near",											//�������̷���
		"equip_base_defence_far",											//����Զ�̷���
		"equip_base_defence_m_near"	,										//���������ڹ�����
		"equip_base_defence_m_far"	,										//���������⹦����
		"equip_iattribute_rate_nearatt_reduce",					    		//���ٷֱȵ������̹���
		"equip_iiattribute_rate_faratt_reduce",								//���ٷֱȵ���Զ�̹���
		"equip_iattribute_rate_matt_reduce_near",							//���ٷֱȵ��������ڹ�����
		"equip_iattribute_rate_matt_reduce_far",							//���ٷֱȵ��������⹦����
		"equip_iattribute_rate_goldatt_reduce",								//���ٷֱȵ����𹥻�
		"equip_iattribute_rate_woodatt_reduce",								//���ٷֱȵ���ľ����
		"equip_iattribute_rate_wateratt_reduce",							//���ٷֱȵ���ˮ����
		"equip_iattribute_rate_fireatt_reduce",								//���ٷֱȵ����𹥻�
		"equip_iattribute_rate_soilatt_reduce",								//���ٷֱȵ���������
		"equip_rate_kongju_reduce",											//�־忹��
		"equip_rate_hongmi_reduce",											//���Կ���
		"equip_rate_chengmo_reduce",										//��Ĭ����
		"equip_rate_jiaoxie_reduce",										//��е����
		"equip_rate_speed_attack_reduce",									//�����ٿ���
		"equip_rate_speed_add_reduce",										//��ʩ�ٿ���
		"equip_rate_speed_sub_reduce",										//�����ٿ���


		"equip_reduce_gunwuseng",					    					//�����˺�--������[����ɮ]
		"equip_reduce_tieluohan",											//�����˺�--������[���޺�]	
		"equip_reduce_gungaibang",											//�����˺�--ؤ��[��ؤ��]
		"equip_reduce_zhanggaibang",										//�����˺�--ؤ��[��ؤ��]
		"equip_reduce_jianwudang",											//�����˺�--�䵱��[���䵱]	
		"equip_reduce_qiwudang",											//�����˺�--�䵱��[���䵱]
		"equip_reduce_daotangmen",											//�����˺�--����[����]		
		"equip_reduce_xianjingtang",										//�����˺�--����[������]	
		"equip_reduce_xiaoyaoshan",											//�����˺�--��ң��[����ң]
		"equip_reduce_zhixiaoyao",											//�����˺�--��ң��[ָ��ң]
		"equip_reduce_kuangyaowang",										//�����˺�--ҩ����[��ҩ��]
		"equip_reduce_yiyaowang",											//�����˺�--ҩ����[ҽҩ��]	
		"equip_reduce_ditaohua",											//�����˺�--�һ���[���һ�]
		"equip_reduce_yintaohua",											//�����˺�--�һ���[���һ�]
		"equip_reduce_ciermei",												//�����˺�--������[�̶�ü]
		"equip_reduce_fuermei",												//�����˺�--������[����ü]
		"equip_reduce_xinshou",												//�����˺�--�����ɣ������֡�����

};								
enum EQUIP_PAI_ClIENT
{ 
	C_EQUIPPAI_DONG		=	15	,//��
	C_EQUIPPAI_NAN		=	16	,//��
	C_EQUIPPAI_XI		=	17	,//��
	C_EQUIPPAI_BEI		=	18	,//��
	C_EQUIPPAI_ZHONG	=	19	,//��
	C_EQUIPPAI_FA		=	20	,//��
	C_EQUIPPAI_BAI		=	21	,//��
	C_EQUIPPAI_1TIAO	=	31	,//1��
	C_EQUIPPAI_2TIAO	=	32	,//2��
	C_EQUIPPAI_3TIAO	=	33	,//3��
	C_EQUIPPAI_4TIAO	=	34	,//4��
	C_EQUIPPAI_5TIAO	=	35	,//5��
	C_EQUIPPAI_6TIAO	=	36	,//6��
	C_EQUIPPAI_7TIAO	=	37	,//7��
	C_EQUIPPAI_8TIAO	=	38	,//8��
	C_EQUIPPAI_9TIAO	=	39	,//9��
	C_EQUIPPAI_1TONG	=	41	,//1Ͳ
	C_EQUIPPAI_2TONG	=	42	,//2Ͳ
	C_EQUIPPAI_3TONG	=	43	,//3Ͳ
	C_EQUIPPAI_4TONG	=	44	,//4Ͳ
	C_EQUIPPAI_5TONG	=	45	,//5Ͳ
	C_EQUIPPAI_6TONG	=	46	,//6Ͳ
	C_EQUIPPAI_7TONG	=	47	,//7Ͳ
	C_EQUIPPAI_8TONG	=	48	,//8Ͳ
	C_EQUIPPAI_9TONG	=	49	,//9Ͳ
	C_EQUIPPAI_1WAN		=	51	,//1��
	C_EQUIPPAI_2WAN		=	52	,//2��
	C_EQUIPPAI_3WAN		=	53	,//3��
	C_EQUIPPAI_4WAN		=	54	,//4��
	C_EQUIPPAI_5WAN		=	55	,//5��
	C_EQUIPPAI_6WAN		=	56	,//6��
	C_EQUIPPAI_7WAN		=	57	,//7��
	C_EQUIPPAI_8WAN		=	58	,//8��
	C_EQUIPPAI_9WAN		=	59	,//9��
	C_EQUIP_PAI_NUM,
};	
const char* g_szCardName[] = 	
{
 			
	"East_Card",				//��
	"South_Card",				//��
	"West_Card",				//��
	"North_Card",				//��
	"Center_Card",				//��
	"Fa_Card",					//��
	"White_Card",				//��
 "",

 "",

 "",
 "",
 "",
 "",
 "",
 "",
 "",

	"One_Tiao",					//1��
	"Two_Tiao",					//2��
	"Three_Tiao",				//3��
	"Four_Tiao"	,				//4��
	"Five_Tiao"	,				//5��
	"Six_Tiao",					//6��
	"Seven_Tiao",				//7��
	"Eight_Tiao",				//8��
	"Nine_Tiao",				//9��

	 "",
	
	"One_Tong",					//1Ͳ
	"Two_Tong",					//2Ͳ
	"Three_Tong",				//3Ͳ
	"Four_Tong"	,				//4Ͳ
	"Five_Tong"	,				//5Ͳ
	"Six_Tong",					//6Ͳ
	"Seven_Tong",				//7Ͳ
	"Eight_Tong",				//8Ͳ
	"Nine_Tong",				//9Ͳ

	 "",
	"One_Wan",					//1��
	"Two_Wan",					//2��
	"Three_Wan",				//3��
	"Four_Wan",					//4��
	"Five_Wan",					//5��
	"Six_Wan",					//6��
	"Seven_Wan",				//7��
	"Eight_Wan",				//8��
	"Nine_Tiao",				//9��

	"anyCard",					//������
	"anyFengCard",				//�������
	"anyJianCard",				//�������
	"anyZiCard",				//��������
	"anyNoZiCard",				//���������
	"anyTiaoCard",				//��������
	"anyTongCard",				//����Ͳ��
	"anyWanCard",				//��������
	"anyShunCard",				//����˳
	"anyKeCard",				//�����
	"anyJiang",					//���⽫		

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


// ��������class
ITEM_CLASS CObject_Item_Equip::GetItemClass(VOID) const 
{ 

		if (m_theBaseDef.pDefineEquip)
		{
			return (ITEM_CLASS)m_theBaseDef.pDefineEquip->nClass;
		}
			
		return ITEM_CLASS(-1);
		
}



//�߼�װ�������Կ��Զ��� 
VOID	CObject_Item_Equip::AsAdvEquip( const _DBC_ITEM_EQUIP* pDefine)
{
	//װ������
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

	//���Ӽ���
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

	//Ӱ�켼��

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
 

//����һ��װ��
VOID CObject_Item_Equip::AsEquip(const   _DBC_ITEM_EQUIP* pDefine )
{
	  
	m_theBaseDef.pDefineEquip = pDefine;

	m_nParticularID = (( m_theBaseDef.pDefineEquip->nClass * 100 + m_theBaseDef.pDefineEquip->nType ) * 10000 ) + m_theBaseDef.pDefineEquip->nIndex;

	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	m_pExtraDefine->m_nSellPrice = pDefine->nBasePrice;			//�۳��۸�
	m_pExtraDefine->m_nLevelNeed = atoi(pDefine->nLevelRequire);//����ȼ� 
	m_pExtraDefine->m_nMaxDurPoint  = pDefine->nBMaxDur;		//����;�ֵ
	m_pExtraDefine->m_SetNum  = pDefine->nSetID;				//��װid
//	m_pExtraDefine->m_nSellPrice  = pDefine->nBasePrice;        //�۸�

	//��ǰ�;�ֵ 
	m_pExtraDefine->m_CurDurPoint  = pDefine->nBMaxDur;       // max�;�

	//�����ݱ�
	DBC_DEFINEHANDLE(s_pItem_Visual, DBC_ITEM_EQUIP_VISUAL);
	//������¼
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

	// �߼�װ��
	if( !ISCommonEquip(pDefine->nID) )
	{
		m_eEquipStyle = ADVANCED_EQUIP;
		AsAdvEquip( pDefine);
	} 
     //��ͨװ��
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


//�õ�װ���Ƿ񱻸�ħ
BOOL	CObject_Item_Equip::GetIsFumo()
{
	return m_pExtraDefine->m_SoulBeadAttrCount>0 ;
}



//װ�������Ƿ�
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

//װ�������Ƿ����
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
//װ�������Ƿ�����
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

//����װ��������С��
INT			Equip_Card_Min( INT  c1 ,INT c2, INT c3)
{

		return ( c1 <c2 ? c1: c2) < c3 ? (  c1 < c2 ? c1: c2)  : c3; 
}
//װ����
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


//ģ��id
INT CObject_Item_Equip::GetVisualID(VOID) const
{
	
	if( m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->nModelID;
		}

	return -1;
	
}

//�õ�����ʱ��ģ��id
LPCTSTR	 CObject_Item_Equip::GetDropVisualID(VOID)	const
{	
	if (m_theBaseDef.pEquipVisual)
		{
			
			return m_theBaseDef.pEquipVisual->nDropModelId;
		}

	return NULL;

}
 
//��ʾ������ɫ
LPCTSTR		CObject_Item_Equip::GetDropVisColor(VOID) const 
 {

	if (m_theBaseDef.pEquipVisual)
		 {
			 return m_theBaseDef.pEquipVisual->szDropVisColor;
		 }
	 return NULL;

 }

//��������
ENUM_WEAPON_TYPE CObject_Item_Equip::GetWeaponType(VOID) const
{
	if(!m_theBaseDef.pDefineEquip)
		return WEAPON_TYPE_INVALID;

	INT nDefine = WEAPON_TYPE_INVALID;

	if (m_theBaseDef.pDefineEquip)
	{
		nDefine = (INT)(m_theBaseDef.pDefineEquip->nType);
	}
	
	//ת��ΪENUM_WEAPON_TYPE
	switch(nDefine)//��ʱ��ע�ʹ���,������Զ�޸Ĵ���(-_-�޸�����)
	{
		case EQUIP_GUN	     :		return WEAPON_TYPE_GUN;	// ��
		case EQUIP_BANG	 :		return WEAPON_TYPE_BANG;		// ��
		case EQUIP_JIAN	 :		return WEAPON_TYPE_JIAN;			// ��
		case EQUIP_DADAO :		return WEAPON_TYPE_DAO;		// ��
		case EQUIP_SHANZI 	 :		return WEAPON_TYPE_SHANZI;		// ����
		case EQUIP_HULU 	 :		return WEAPON_TYPE_HULU;		// ��«
		case EQUIP_DIZI		:	return WEAPON_TYPE_DIZI;							//����
		case EQUIP_CI		:	return WEAPON_TYPE_CI;							//��
		case EQUIP_TOOL	:	return WEAPON_TYPE_NONE;			//����
		default	:
			return  WEAPON_TYPE_INVALID ;
	}

}


//���֡���
LPCTSTR CObject_Item_Equip::GetName(VOID) const
{
	 
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szName;
		}
	 
		return NULL;
 
		
}
//uiͼ��
LPCTSTR CObject_Item_Equip::GetIconName(VOID) const
{ 
	 
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szIcon;
		}
	 
		return NULL;
	 
}
//�õ�װ������
INT  CObject_Item_Equip::GetEquipCard(VOID) 
{
		if (  m_pExtraDefine)
		{
			return m_pExtraDefine->m_PaiXing;
		}

	return 0;

}

//�õ�װ������
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

// ����װ������
BOOL	CObject_Item_Equip::SwapEquipCard(CObject_Item_Equip &ep)

{
	if (  m_pExtraDefine && ep.m_pExtraDefine)
	{
		// swap װ������  
		m_pExtraDefine->m_PaiXing = m_pExtraDefine->m_PaiXing + ep.m_pExtraDefine->m_PaiXing;

		ep.m_pExtraDefine->m_PaiXing = m_pExtraDefine->m_PaiXing -ep.m_pExtraDefine->m_PaiXing;

		m_pExtraDefine->m_PaiXing= m_pExtraDefine->m_PaiXing -ep.m_pExtraDefine->m_PaiXing;
		return TRUE;

	}
	return FALSE;

}


//����װ������
VOID  CObject_Item_Equip::SetEquipCard( INT nPaiXing )  
{
	if (  m_pExtraDefine)
	{
		  m_pExtraDefine->m_PaiXing =  nPaiXing;
	}

}

// �õ��Ǽ�
INT CObject_Item_Equip::GetStartLevel(VOID) const
{
	if (m_pExtraDefine)
	{
		return m_pExtraDefine->m_StartLevel;
	}

	return -1;
}
// װ������
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

//��¡��ϸ��Ϣ
VOID CObject_Item_Equip::Clone(const CObject_Item * pItemSource)
{
	ClonetExtraInfo((CObject_Item_Equip*)pItemSource);
	CObject_Item::Clone(pItemSource);
}


 //������ϸ����
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

	//������
	m_pExtraDefine->m_nAttSet			= ed->m_nAttSet;
	//�Ǽ�
	m_pExtraDefine->m_StartLevel		= ed->m_Level;
	//��ǰ���˶� 
	m_pExtraDefine->m_CurDamagePoint	= ed->m_CurDamagePoint;
	//��ǰ�;�ֵ 
	m_pExtraDefine->m_CurDurPoint		= ed->m_CurDurPoint;

	//��Ƕ�ı�ʯ����
	m_pExtraDefine->m_GemCount			= ed->m_StoneCount;
    //�ȼ�����
	m_pExtraDefine->m_nLevelNeed        = ed->m_NeedLevel;
   //װ������Ŀ
	m_pExtraDefine->m_nEquipHoleNum		= ed->m_CurGemHole;

	//װ������
	m_pExtraDefine->m_nEquipScore		= ed->m_EquipScore;
    			
   //���Ե�����
    m_pExtraDefine->m_AttrCount        = ed->m_AttrCount;

	//װ�������� ib ��Һϳɵ�
	 m_pExtraDefine->m_EquipType       = ed->m_EquipType;

    m_pExtraDefine->m_RulerID          =  ed->m_RulerID;

    //���޺�������������ᳬ�����е�����;�ֵ
	 m_pExtraDefine->m_nMaxDurPoint    = ed->m_CurMaxDur;		//��ǰ����;�ֵ

	 m_pExtraDefine->m_PaiXing = ed->m_PaiType  ;                //װ��������

	 m_pExtraDefine->m_SoulBeadType = ed->m_SoulBeadType;			//��ħ�Ļ�������

	 m_pExtraDefine->m_SoulBeadAttrCount = ed->m_SoulBeadAttrCount;		//�������Ե�����

	AxTrace(1, 0, "%d", pItemInfo->m_Equip.m_StoneCount);

	//����Ʒ�����
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

	// �߼�
	//test �����жϣ� ֻ����ͨװ������ʾ������
//	if( EquipStyle == ADVANCED_EQUIP )
	{
		// �õ�������Ϣ��
		if( pItemInfo->GetItemIdent() || !m_Rule.bCheck)
		{
			// �Ѿ�����
			m_EquipAttrib = CObject_Item_Equip::EQUIP_ATTRIB_IDENTIFY;
		}
		else
		{
			// δ����
			m_EquipAttrib = CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY;
		}
	}

	// �õ�����Ϣ,װ���Ƿ��
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
     // �õ�����Ϣ,װ���Ƿ�2����
	if (ed->m_Bind == 1)
	{
		m_pExtraDefine->m_nEquipBindInfo2 = EQUIP_BIND_TWO;
	}

 

	//   ���ӵı�ʯ
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


	// ��ͨװ��
	//if( m_eEquipStyle == COMMON_EQUIP )
	//{
	//
	//	//װ������
	//	_ITEM_ATTR	attr;
	//	m_pExtraDefine->m_vEquipAttributes.clear();

	//	//�����ݱ�	
	//	DBC_DEFINEHANDLE(s_pItem_AttrGroup, DBC_ITEM_ATTR_GROUP);	// attribute_set.tab װ���������
	//	//������¼
	//	const _DBC_ITEM_ATTR_GROUP* pEquipAttrGroup = (const _DBC_ITEM_ATTR_GROUP*)s_pItem_AttrGroup->Search_Index_EQU((UINT) m_pExtraDefine->m_nAttSet);
	//	if(!pEquipAttrGroup) return ;
	//	 m_pExtraDefine->m_szAttrGroup = pEquipAttrGroup->szGroupName;
	//	//�����ݱ�
	//	DBC_DEFINEHANDLE(s_pItem_AttrLevel, DBC_ITEM_ATTR_LEVEL);	// attribute_level.tab װ�����Եȼ���
	//	const _DBC_ITEM_ATTR_LEVEL* s_AttrLevel = NULL;

	//	for(INT i=0; i< EQUIP_EFFECT_ATTR_MAXNUM; ++i)
	//	{
	//		if( -1 != pEquipAttrGroup->nAttrId[i] )
	//		{
	//	        attr.m_AttrType = (BYTE)pEquipAttrGroup->nAttrId[i];
	//			//������¼
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

			m_theBaseDef.m_Base.m_nBase_Atack_Near =  INT (pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);  // �������̹���
		}
		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_ATTACK_FAR )
		{
			 
			m_theBaseDef.m_Base.m_nBase_Atack_Far = INT ( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // ����Զ�̹���
		}
		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR )
		{
			 
			m_theBaseDef.m_Base.m_nBase_Atack_Magic = INT (pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // ����ħ������
		}

		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_DEFENCE_NEAR )
		{
			
			m_theBaseDef.m_Base.m_nBase_Def_Near =INT( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);  // �������̷���
		}
		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_DEFENCE_FAR )
		{
 
			m_theBaseDef.m_Base.m_nBase_Def_Far = INT( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // ����Զ�̷���
		}

		else if (pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType == IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR )
		{
			 
			m_theBaseDef.m_Base.m_nBase_Def_Magic = INT( pItemInfo->GetEquipData()->m_pAttr[i].m_Value.m_Value);   // ����ħ������		
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

//��¡������Ϣ
VOID CObject_Item_Equip::ClonetExtraInfo( CObject_Item_Equip * pItemSource)
{
	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	EXTRA_DEFINE* pNewExtraDefine = (EXTRA_DEFINE*)(pItemSource->GetExtraInfo());



	//������
	m_pExtraDefine->m_nAttSet = pNewExtraDefine->m_nAttSet;
	//�Ǽ�
	m_pExtraDefine->m_StartLevel  = pNewExtraDefine->m_StartLevel;
	//��ǰ���˶� 
	m_pExtraDefine->m_CurDamagePoint = pNewExtraDefine->m_CurDamagePoint;
	//��ǰ�;�ֵ 
	m_pExtraDefine->m_CurDurPoint  = pNewExtraDefine->m_CurDurPoint;
	// ������Ƕ�ı�ʯ����
	m_pExtraDefine->m_GemCount  = pNewExtraDefine->m_GemCount;


	//�۳��۸�
	m_pExtraDefine->m_nSellPrice = pNewExtraDefine->m_nSellPrice;
	//����ȼ� 
	m_pExtraDefine->m_nLevelNeed = pNewExtraDefine->m_nLevelNeed;
	//����;�ֵ
	m_pExtraDefine->m_nMaxDurPoint = pNewExtraDefine->m_nMaxDurPoint;
	//����
	m_pExtraDefine->m_nAttSet = pNewExtraDefine->m_nAttSet;


	//��ʯ
	for(UINT i=0; i<pNewExtraDefine->m_vEquipAttachGem.size(); i++)
	{
		m_pExtraDefine->m_vEquipAttachGem.push_back(pNewExtraDefine->m_vEquipAttachGem[i]);
	}
	m_pExtraDefine->m_nEquipAttachGemNum = m_pExtraDefine->m_vEquipAttachGem.size();

    
	if (pItemSource->m_eEquipStyle == ADVANCED_EQUIP)
	{
		// װ���Ƿ����������
		m_EquipAttrib = pItemSource->m_EquipAttrib;

		//���Ӽ���
		for(UINT i=0; i<pNewExtraDefine->m_Affix_Skill.size(); i++)
		{
			m_pExtraDefine->m_Affix_Skill.push_back(pNewExtraDefine->m_Affix_Skill[i]);
		}
		m_pExtraDefine->m_nAffix_SkillNum = m_pExtraDefine->m_Affix_Skill.size();

		//Ӱ�켼��
		for(UINT i=0; i<pNewExtraDefine->m_Effect_Skill.size(); i++)
		{
			m_pExtraDefine->m_Effect_Skill.push_back(pNewExtraDefine->m_Effect_Skill[i]);
		}
		m_pExtraDefine->m_nEffect_SkillNum = m_pExtraDefine->m_Effect_Skill.size();
	}


	SetNumber(pItemSource->GetNumber());

	m_theBaseDef = pItemSource->GetEquipBaseDefine();
	
}

//�õ���װ��� 
INT CObject_Item_Equip::GetSetID(VOID) const
{
	 
	if (  m_pExtraDefine)
	{
		return m_pExtraDefine->m_SetNum;
	}

	return -1;

}
//��ϸ���� 
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

//����װ����ǰ�;ö� 
VOID	CObject_Item_Equip::SetDur(INT Dur)

{
	if( m_pExtraDefine )

	 m_pExtraDefine->m_CurDurPoint = Dur;

}
//��õ�ǰ�;ö�
INT		CObject_Item_Equip::GetDur(VOID) const
{
	if (m_pExtraDefine)
	{
		return	m_pExtraDefine->m_CurDurPoint;
	}
	else
		return 0;
	
}
//�������;ö�
INT		CObject_Item_Equip::GetMaxDur(VOID) const
{

	if (m_pExtraDefine)
	{
		return	m_pExtraDefine->m_nMaxDurPoint;
	}
	else
		return 0;
}


//����װ����ǰ����;ö�
VOID		CObject_Item_Equip::SetMaxDur(INT Dur)  
{

	if (m_pExtraDefine)
	{
		m_pExtraDefine->m_nMaxDurPoint  = Dur;
	}
	 
}
//�������;ö�
INT		CObject_Item_Equip::GetMaxDurFromTable(VOID) const
{

	if (m_theBaseDef.pDefineEquip)
	{
		return	m_theBaseDef.pDefineEquip->nBMaxDur;
	}
	else

		return -1;
}


//���װ����Ч
const EQUIP_EFFECT*  CObject_Item_Equip::GetEffect() 
{
	
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szEffect;
		}

		return NULL;
	
	
}

//�õ�װ����������
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
	att.m_Value.m_Value  =	m_theBaseDef.m_Base.m_nBase_Atack_Near;  // �������̹���

	if (att.m_Value.m_Value > 0 )
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}


	 

	att.m_AttrType = IATTRIBUTE_BASE_ATTACK_FAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Atack_Far ;   // ����Զ�̹���
	if (att.m_Value.m_Value  > 0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}

	att.m_AttrType = IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Atack_Magic; // ����ħ������

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}

	att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_NEAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Def_Near;    // �������̷���

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}

	att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_FAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Def_Far ;	 // ����Զ�̷���

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}


	att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR;
	att.m_Value.m_Value  =  m_theBaseDef.m_Base.m_nBase_Def_Magic;    // ����ħ������		

	if (att.m_Value.m_Value >0)
	{
		SetAttri(att, szAtt);
		strncat(szAttachAtt, szAtt, 1024);
	}


	strValue = szAttachAtt;
}



//�õ�װ����������
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
	//att.m_Value.m_Value  =	m_theBaseDef.pDefineEquip->nBaseAttackNear;  // �������̹���

	//if (att.m_Value.m_Value > 0 )
	//{
	//	SetAttri(att, szAtt);
	//    strncat(szAttachAtt, szAtt, 1024);
	//}
	//
	// 

	//att.m_AttrType = IATTRIBUTE_BASE_ATTACK_FAR;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseAttackFar;   // ����Զ�̹���
	//if (att.m_Value.m_Value  > 0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
	//
	//att.m_AttrType = IATTRIBUTE_BASE_ATTACK_M;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseAttackMagic; // ����ħ������

	//if (att.m_Value.m_Value >0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
	//
	//att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_NEAR;
 //   att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseDEefNear;    // �������̷���

	//if (att.m_Value.m_Value >0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
	// 
	//att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_FAR;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip-> nBaseDefFar;	 // ����Զ�̷���

	//if (att.m_Value.m_Value >0)
	//{
	//	SetAttri(att, szAtt);
	//	strncat(szAttachAtt, szAtt, 1024);
	//}
 //

	//att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_M;
	//att.m_Value.m_Value  =  m_theBaseDef.pDefineEquip->nBaseDefMagic;    // ����ħ������		

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
//����װ���������� 

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


//��ʯ����
INT CObject_Item_Equip::GetGemCount()
{
	if( !m_pExtraDefine)
	{
		return 0;
	}

	return  (INT)m_pExtraDefine->m_vEquipAttachGem.size();
}
//�õ���ʯ 
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


// �õ���ʯ��ͼ��
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

	//�򿪱�ʯ���ݱ�
	DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
	//������¼
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

// �õ���ʯ�ĸ��ӹ���
VOID CObject_Item_Equip::GetGemExtAttr(INT iGemIndex, STRING& strGemAttr)
{
	//�򿪱�ʯ���ݱ�
	DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
	//������¼
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

  	            

 
//�õ�����ֵ
STRING CObject_Item_Equip::GetAttributeValue(LPCTSTR szValueName)
{
	if(!szValueName) return "";

	CHAR szTemp[1024];
	

	//װ��ͼ��

	 if (_stricmp(szValueName,ITEMATTRIBUTE_ICON) == 0)
	{
		return GetIconName();

	}
	 //�齫����

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

	//��Ʒ�ȼ�
	else if(_stricmp(szValueName, ITEMATTRIBUTE_LEVEL) == 0)
	{
		if (-1 == GetNeedLevel())
		{
			return "";
		}

		_snprintf(szTemp, 1024, "%d", GetNeedLevel());
		return szTemp;
			 		 
	}

   //ְҵ
	else if(_stricmp(szValueName, ITEMATTRIBUTE_PROFESSION) == 0)
	{
		if (-1 == GetItemProfession())
		{
			return "";
		}
		_snprintf(szTemp, 1024, "%d", GetItemProfession());
		return szTemp;  
	}

    //����;�
	else  if(_stricmp(szValueName, ITEMATTRIBUTE_MAXDUR) == 0)
	{
		if (-1 == GetItemMaxDur())
		{
			return "";
		}
		_snprintf(szTemp, 1024, "%d", GetItemMaxDur()); 
		return szTemp;
		 
	}
	 //��ǰ�;� 
	else if(_stricmp(szValueName,ITEMATTRIBUTE_CURDUR ) == 0)
	{
		if (-1 == GetItemDur())
		{
			return "";
		}
		 _snprintf(szTemp, 1024, "%d", GetItemDur() );		 
		 return szTemp;	 
	}
	 //�۸�
	else if(_stricmp(szValueName, ITEMATTRIBUTE_PRICE) == 0)
	{
		if (-1 == GetItemPrice())
		{
			return "";
		}
		_snprintf(szTemp, 1024, "%d",GetItemPrice());
		return szTemp;
	}	
	//�Ǽ�
	else if(_stricmp(szValueName, ITEMATTRIBUTE_STARTLEVEL) == 0)
	{
		if(-1 != GetStartLevel())
		{
			_snprintf(szTemp, 1024, "%d", GetStartLevel());

			return szTemp;
		}
		return "";
	}
	 //����
	else if(_stricmp(szValueName, ITEMATTRIBUTE_PRODUCER) == 0)
	{
		
		return	GetManufacturer();
					
	}

	//��ʯ����
	else if(_stricmp(szValueName, ITEMATTRIBUTE_GEMSNUM) == 0)
	{
		if ( 0 != GetGemCount() )
		{
			_snprintf(szTemp, 1024, "%d", GetGemCount());
			return szTemp;
		} 
		return "";
	}

	//װ������ 
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_TYPE) == 0)
	{

		return	GetItemTableTypeDesc();

	}
 					
	//װ������Ϣ
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_BINDINFO) == 0)
	{

		_snprintf(szTemp, 1024, "%d",GetItemBindInfo());		 
		return szTemp;
	}
	 						
	//����װ����������
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_BASEATT) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;
			GetAllEquipBaseAttribute(strValue);
			return strValue;
		}
	}


	//����װ����������
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_ADDATT) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;

			GetAllEquipAddAttribute(strValue);
			return strValue;
		}
	}
	//����װ�����Ӽ�������
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_SKILL_ATTR) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;
			GetAllEquipSkillAttr(strValue);
			return strValue;
		}
	}

	//װ����ʯ������
	else if(_stricmp(szValueName, ITEMATTRIBUTE_EQUIP_GEMSATT) == 0)
	{
		 
			STRING strValue;
			for (int i = 0 ;i < GetGemCount() ;++i)
			{
				GetGemExtAttr(i,strValue);
			}
			
			return strValue;
		
	}
      //װ�������ã���ħ��������Ϣ
 
	else if (_stricmp(szValueName,ITEMATTRIBUTE_EQUIP_FOREVERATTRINFO) == 0)
	{
		return "";
	}
	//װ������ʱ����ħ��������Ϣ
	else if (_stricmp(szValueName,ITEMATTRIBUTE_EQUIP_TEMPATTRINFO) == 0)
	{
		return "";
	}

	///װ������װ�����Ϣ
	else if (_stricmp(szValueName,ITEMATTRIBUTE_EQUIP_TEMPATTRINFO) == 0)
	{
		return "";
	}
	//��װ���������
	else if (_stricmp(szValueName,ITEMATTRIBUTE_SET_ACCTIVEATTR) == 0)
	{
		return "";
	}
 				
 				
	  //��ϸ��Ϣ
	else if (_stricmp(szValueName,ITEMATTRIBUTE_DESC) ==0)
	{
			return GetExtraDesc();
	}
	//������ɫ
	else if (_stricmp(szValueName,ITEMATTRIBUTE_DROP_COLOR) ==0)
	{

		return GetDropVisColor();
	}


	//װ������ 
	else if (_stricmp(szValueName,ITEMATTRIBUTE_NAME) ==0)
	{
		return CObject_Item::GetAttributeValue(szValueName);
	}

	return "";
	
}


// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
INT	CObject_Item_Equip::GetNeedLevel()
{
   if (m_theBaseDef.pDefineEquip)
   {
	   return  m_pExtraDefine ->m_nLevelNeed;
   }

     return -1;
}

// �õ���Ʒ�;ö�
INT	CObject_Item_Equip::GetItemDur()
{
	
	return GetDur();
	
}

// �õ���Ʒ����;ö�
INT	CObject_Item_Equip::GetItemMaxDur()
{
	
	return GetMaxDur();
	
}


// �õ���Ʒ�İ���Ϣ 
INT	CObject_Item_Equip::GetItemBindInfo()
{
	// -1 �ް���Ϣ
	if(m_pExtraDefine)
	{
		return m_pExtraDefine->m_nEquipBindInfo;
	}
	
	return -1;
}

// �õ���Ʒ�İ���Ϣ 
INT	CObject_Item_Equip::GetItemSecLevelBindInfo()
{
	// -1 �ް���Ϣ
	if(m_pExtraDefine)
	{
		return m_pExtraDefine->m_nEquipBindInfo2;
	}

	return -1;
}


//�󶨼ӳ���Ϣ
LPCTSTR	CObject_Item_Equip::GetItemBindAddInfo()
{
	
	 
	//�����ݱ� Equip_bind.tab
	DBC_DEFINEHANDLE(s_Equip_Bind, DBC_EQUIP_BINDING_INFO);
		//������¼
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
							att.m_AttrType = IATTRIBUTE_BASE_ATTACK_NEAR;  // �������̹���
							att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Atack_Near *  fIntensifyInfo)/100.f + 0.5f  ;
							SetAttri(att, szAtt);
							strncat(szAttachAtt, szAtt, 1024);

					}
					
					if (m_theBaseDef.m_Base.m_nBase_Atack_Far > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_ATTACK_FAR;   // ����Զ�̹���	 
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Atack_Far *  fIntensifyInfo )/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}

					if ( m_theBaseDef.m_Base.m_nBase_Atack_Magic > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR;   //  ����ħ������
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Atack_Magic *  fIntensifyInfo)/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}

					if (m_theBaseDef.m_Base.m_nBase_Def_Near > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_NEAR;    // �������̷���
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Def_Near *  fIntensifyInfo )/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}
					if (m_theBaseDef.m_Base.m_nBase_Def_Far > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_FAR;    // ����Զ�̷���	
						att.m_Value.m_Value   =( m_theBaseDef.m_Base.m_nBase_Def_Far *  fIntensifyInfo )/100.f + 0.5f  ;
						SetAttri(att, szAtt);
						strncat(szAttachAtt, szAtt, 1024);

					}

					if (m_theBaseDef.m_Base.m_nBase_Def_Magic > 0 )
					{
						att.m_AttrType = IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR;   // ����ħ������		
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

// �õ���Ʒ����npc�ļ۸�
INT	CObject_Item_Equip::GetItemPrice()
{

	if (m_theBaseDef.pDefineEquip)
	{
		return	m_theBaseDef.pDefineEquip->nBasePrice;
	}
	return -1;
}
//�õ���Ʒְҵ
INT  CObject_Item_Equip::GetItemProfession() 
{
	if (m_theBaseDef.pDefineEquip)
	{
		return	m_theBaseDef.pDefineEquip->nProfessionalReq;
	}
	return -1;

}
//�õ�װ��Ŀǰ��׵���Ŀ
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



//�õ�װ����װ�����������Ϣ
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
		"��װ���Լӳ�"
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

	//�õ�װ����װ�����������Ϣ
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
			"��װ���Լӳ�"
			);
		_stprintf(bufInfo, _T("%s%s"), szAtt, szAttachAtt);
	}
	
	return bufInfo;
	

}


//�õ�װ����װ��Ϣ   
LPCTSTR		CObject_Item_Equip::GetOtherEquipSetInfo()
{
	INT  setId = GetSetID();
	if (setId == -1)
		return "";

	if(   0 ==	 CDataPool::GetMe()->AllSetEquipList.count(setId))  

		return "û�и���װ��Ϣ";

	if(   0 ==	 CDataPool::GetMe()->AllSetInfo.count(setId))

		return "û�и���װ��Ϣ";
	if(   0 ==	 CDataPool::GetMe()->AllSetEquipName.count(setId))

		return "û�и���װ��Ϣ";



	CHAR szAtt[MAX_PATH] = {0};
	CHAR szAttachAtt[2048] = {0};


	INT  setNum = CDataPool::GetMe()->AllSetInfo[setId];
	INT  ActsetNum = 0;
	BOOL isHas = FALSE;


	strncat(szAttachAtt, szAtt, 1024);
	//�����ݱ�
	DBC_DEFINEHANDLE(s_pItem_Visual, DBC_ITEM_EQUIP_VISUAL);
	//������¼
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
//�õ��������װ���󼤻����Ϣ

LPCSTR				CObject_Item_Equip::GetOtherEquipSpriteAllActiveInfo()
{

	CDataPool::GetMe()->OtherUserEquipSprite_IsActiveAll();

	if (CDataPool::GetMe()->m_OtherAllSprite_Active == 0)
	{
		return "";
	}else
	{
		//�����ݱ�//chain_info.tab

		CHAR szAtt[MAX_PATH] = {0};
		CHAR szAttachAtt[1024] = {0};
		_ITEM_ATTR  attr ;

		DBC_DEFINEHANDLE(s_pCardAll_Equip, DBC_EQUIP_CARD_ALL);
		//������¼
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


//�õ�װ���󼤻����Ϣ
LPCSTR				CObject_Item_Equip::GetEquipSpriteAllActiveInfo()
{

	CDataPool::GetMe()->UserEquipSprite_IsActiveAll();

	if (CDataPool::GetMe()->m_SelfAllSprite_Active == 0)
	{
		return "";
	}else
	{
		//�����ݱ�//chain_info.tab

		CHAR szAtt[MAX_PATH] = {0};
		CHAR szAttachAtt[1024] = {0};
		 _ITEM_ATTR  attr ;

		DBC_DEFINEHANDLE(s_pCardAll_Equip, DBC_EQUIP_CARD_ALL);
		//������¼
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

//�õ�װ�����ͼ������Ϣ
LPCSTR		CObject_Item_Equip::GetEquipSpritePaiInfo()
{

 

	DBC_DEFINEHANDLE(s_pCard_Equip, DBC_EQUIP_CARD_INFO);
	//������¼
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

//�õ��������װ�����ͼ������Ϣ 

LPCSTR		CObject_Item_Equip::GetOtherEquipSpriteChainInfo()


{
	//��Ʒ 1 ��Ʒ2  ��					 0
	//ñ  ���� ����                      1
	//��ָ1 ��ָ2  Ь��					 2		
	//�� �·� ����						 3
	// ���� ����						 4
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


		//�����ݱ�//chain_info.tab

		DBC_DEFINEHANDLE(s_pCardChain_Equip, DBC_EQUIP_CARD_GROUP_INFO);
		//������¼
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

//�õ�װ��С�������Ϣ
LPCSTR				CObject_Item_Equip::GetEquipSpriteChainInfo()
{
	//��Ʒ 1 ��Ʒ2  ��					 0
	//ñ  ���� ����                      1
	//��ָ1 ��ָ2  Ь��					 2		
	//�� �·� ����						 3
	// ���� ����						 4
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
	 


		//�����ݱ�//chain_info.tab
	 
		DBC_DEFINEHANDLE(s_pCardChain_Equip, DBC_EQUIP_CARD_GROUP_INFO);
		//������¼
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




//�õ�װ����װ��Ϣ
LPCTSTR		CObject_Item_Equip::GetEquipSetInfo()

{

	 INT  setId = GetSetID();
	 if (setId == -1)
		return "";

	 if(   0 ==	 CDataPool::GetMe()->AllSetEquipList.count(setId))  

		 return "û�и���װ��Ϣ";

	 if(   0 ==	 CDataPool::GetMe()->AllSetInfo.count(setId))

		 return "û�и���װ��Ϣ";
	 if(   0 ==	 CDataPool::GetMe()->AllSetEquipName.count(setId))

		 return "û�и���װ��Ϣ";

 

	 CHAR szAtt[MAX_PATH] = {0};
	 CHAR szAttachAtt[2048] = {0};


	 INT  setNum = CDataPool::GetMe()->AllSetInfo[setId];
     INT  ActsetNum = 0;
	 BOOL isHas = FALSE;

	strncat(szAttachAtt, szAtt, 1024);
	 //�����ݱ�
	 DBC_DEFINEHANDLE(s_pItem_Visual, DBC_ITEM_EQUIP_VISUAL);
	 //������¼
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
	
	//ȥ������\n
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




//����װ��Ŀǰ��׵���Ŀ

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


// �õ���Ʒ��������
LPCTSTR	CObject_Item_Equip::GetManufacturer()
{
	return _T("");
}


// �õ�װ���Ļ�������
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

// �õ�װ������
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




// �õ�װ������
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
// ��ʽ�������ַ���
void CObject_Item_Equip::SetAttri(const _ITEM_ATTR& att, char* const pszBuf)
{

	switch(att.m_AttrType)
		{

		case IATTRIBUTE_RATE_STR:               //�����ٷֱ� 
		case IATTRIBUTE_RATE_DEX:				//���ݰٷֱ�:
		case IATTRIBUTE_RATE_INT:				//�����ٷֱ�:
		case IATTRIBUTE_RATE_CON:		        //���ʰٷֱ�
		case IATTRIBUTE_RATE_ALL_BASEATT:		//�������԰ٷֱ�
		case IATTRIBUTE_RATE_ATTACK_MAGIC_NEAR:	//�����ڹ��ٷֱ�
		case IATTRIBUTE_RATE_ATTACK_NEAR:		//�����ٷֱ�
		case IATTRIBUTE_RATE_ATTACK_FAR:		//Զ���ٷֱ�
		case IATTRIBUTE_RATE_DEFENCE_MAGIC_NEAR://�����ڹ��ٷֱ�
		case IATTRIBUTE_RATE_DEFENCE_NEAR:	    //�����ٷֱ�
		case IATTRIBUTE_RATE_DEFENCE_FAR:		//Զ���ٷֱ�:
		case IATTRIBUTE_RATE_MAXHP:			    //����ֵ���ްٷֱ�
		case IATTRIBUTE_RATE_MAXMP:			    //ħ��ֵ���ްٷֱ�
		case IATTRIBUTE_RATE_GOLD_ATTACK:		 //�𹥻��ٷֱ�
		case IATTRIBUTE_RATE_FIRE_ATTACK:		//�𹥻��ٷֱ�
		case IATTRIBUTE_RATE_WOOD_ATTACK:	 	//ľ�����ٷֱ�
		case IATTRIBUTE_RATE_WATER_ATTACK: 		//ˮ�����ٷֱ�
		case IATTRIBUTE_RATE_GOLD_RESIST: 		//������ٷֱ�
		case IATTRIBUTE_RATE_FIRE_RESIST:		//������ٷֱ�
		case IATTRIBUTE_RATE_WOOD_RESIST:	 	//ľ�����ٷֱ�
		case IATTRIBUTE_RATE_WATER_RESIST:		//ˮ�����ٷֱ�
		case IATTRIBUTE_BASE_RATE_ATTACK_NEAR:	   //�������̹����ٷֱ�
		case IATTRIBUTE_BASE_RATE_ATTACK_FAR:		//����Զ�̹����ٷֱ�
		case IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR://���������ڹ������ٷֱ�
		case IATTRIBUTE_BASE_RATE_DEFENCE_ENEAR:   //�������̷����ٷֱ�
		case IATTRIBUTE_BASE_RATE_DEFENCE_FAR:	   //����Զ�̷����ٷֱ�
		case IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_NEAR://���������ڹ������ٷֱ�
		//case IATTRIBUTE_RATE_RAGE_RESTORE:		//ŭ���ظ��ٷֱ�
		case IATTRIBUTE_RATE_MAXRAGE:				//ŭ�����ްٷֱ�
	
			{
				if(att.m_Value.m_Value > 0)             // ����       ����+5%
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s} + %d%%\n",                        
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				else                                     // ����       ����-5%
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
		case	IATTRIBUTE_RATE_RESIST_ALL:				// ���ٷֱȵ����������Թ���	
		case	IATTRIBUTE_RATE_NEARATT_REDUCE:			//���ٷֱȵ������̹���
		case	IATTRIBUTE_RATE_FARATT_REDUCE:			//���ٷֱȵ���Զ�̹���
		case	IATTRIBUTE_RATE_MAGIC_NEAR_ATT_REDUCE:	//���ٷֱȵ��������ڹ�����
		case	IATTRIBUTE_RATE_GOLD_ATT_REDUCE:		//���ٷֱȵ����𹥻�
		case	IATTRIBUTE_RATE_FIREATT_REDUCE:			//���ٷֱȵ����𹥻�
		case	IATTRIBUTE_RATE_WOOD_ATT_REDUCE:		//���ٷֱȵ���ľ����
		case	IATTRIBUTE_RATE_WATER_REDUCE:			//���ٷֱȵ���ˮ����
												
			{											// ���� 2%�����������Թ���                       
				_snprintf(
							pszBuf,      
							MAX_PATH, 
							"%d%%#{%s}\n", 
							att.m_Value.m_Value,
							g_szAttachAttName[att.m_AttrType]
							);
				break;
			}
		//case IATTRIBUTE_RATE_ATTACK_SPEED:		//�������ٶȼ���
		//case IATTRIBUTE_RATE_MOVE_REDUCE:		//���ƶ��ٶȼ���
		//case IATTRIBUTE_RATE_SKILL_SPEED:		//��ʩ���ٶȼ���
		//case IATTRIBUTE_RATE_DREAD:				//�־弸��
		//case IATTRIBUTE_TIME_DREAD:			//�־�ʱ��
		//case IATTRIBUTE_RATE_COMA:				//���Լ���
		//case IATTRIBUTE_TIME_COMA:			//����ʱ��
		//case IATTRIBUTE_RATE_HUSH:				//��Ĭ����
		//case IATTRIBUTE_TIME_HUSH:			//��Ĭʱ��
		//case IATTRIBUTE_RATE_UNARM:			//��е����
		//case IATTRIBUTE_TIME_UNARM:			//��еʱ��
		//case IATTRIBUTE_ROLL_RESIST:			//�ֿ���ѹ
		//case IATTRIBUTE_WUSHI_HURT_RESIST:	//�ֿ���ʿְҵ�˺�
		//case IATTRIBUTE_JIANXIA_HURT_RESIST:	//�ֿ�����ְҵ�˺�
		//case IATTRIBUTE_QISHE_HURT_RESIST:	//�ֿ�������ְҵ�˺�
		//case IATTRIBUTE_HUOQIANG_HURT_RESIST:	//�ֿ���ǹ��ְҵ�˺�
		//case IATTRIBUTE_XIANZHI_HURT_RESIST:	//�ֿ���ְ֪ҵ�˺�
		//case IATTRIBUTE_SAMAN_HURT_RESIST:	//�ֿ�����ְҵ�˺�
	
		//									     
		//	{
		//		// ���� ���ƶ��ٶ�5%   //�־弸�� 3%
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

		case IATTRIBUTE_BASE_ATTACK_NEAR:			//�������̹���
		case IATTRIBUTE_BASE_ATTACK_FAR:			//����Զ�̹���
		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:		//���������ڹ�����
		case IATTRIBUTE_BASE_DEFENCE_NEAR:			//�������̷���
		case IATTRIBUTE_BASE_DEFENCE_FAR:			//����Զ�̷���
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:	//���������ڹ�����

			{
				// ����  �������̹���: 10
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
				if(att.m_Value.m_Value > 0)          //�������� + 5
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s} + %d\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				else                                 //�������� - 5
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

//�õ��Ƿ����
CObject_Item_Equip::EQUIP_ATTRIB	CObject_Item_Equip::GetEquipAttrib()
{
	
	return m_EquipAttrib;
}



// �õ���Ʒ����typeһ�� 
INT	CObject_Item_Equip::GetItemTableType(VOID)
{
	if (m_theBaseDef.pDefineEquip)
	{	
		return m_theBaseDef.pDefineEquip->nType;
	}
	
	return 0; 
}



// �Ƿ�������   //��Ҫ�޸�
BOOL CObject_Item_Equip::IsWeapon()
{
	if (m_theBaseDef.pDefineEquip)
	{
		return m_theBaseDef.pDefineEquip->nEquipPoint == HEQUIP_MAINHAND || 
			m_theBaseDef.pDefineEquip->nEquipPoint == HEQUIP_ASSIHAND;
	}

	return FALSE;
}

	// �Ƿ��Ƿ���
BOOL CObject_Item_Equip::IsDefence()
{
	if (m_theBaseDef.pDefineEquip)
	{
		switch( m_theBaseDef.pDefineEquip->nEquipPoint )
		{
		case HEQUIP_HEAD	:		//ͷ
		case HEQUIP_SHOULDER:		//��
		case HEQUIP_BACK	:		//����
		case HEQUIP_WRIST	:		//��
		case HEQUIP_HAND	:		//��
		case HEQUIP_WAIST	:		//��
		case HEQUIP_FEET	:		//��
			return TRUE;
		}
	}
	return FALSE;
}

	// �Ƿ���װ����
BOOL CObject_Item_Equip::IsAppend()
{
	if (m_theBaseDef.pDefineEquip)
	{
		switch( m_theBaseDef.pDefineEquip->nEquipPoint )
		{
		case HEQUIP_NECK:	//��
		case HEQUIP_RING1:	//��ָ1	
		case HEQUIP_RING2:	//��ָ2
		case HEQUIP_ADORN1:	//��Ʒ1
		case HEQUIP_ADORN2:	//��Ʒ2
			return TRUE;
		}
	}
	return FALSE;
}

// �õ���Ʒ���������� 
LPCTSTR	CObject_Item_Equip::GetItemTableTypeDesc()
{
	 
		if (m_theBaseDef.pEquipVisual)
		{
			return m_theBaseDef.pEquipVisual->szType;
		}
	
	 return NULL;
}


//һ��Ҫ�����л��̶�����
//CHAR* pIn ʵ������
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
	m_nAttSet = *( (INT*)pBuff );		//������
	pBuff += sizeof( INT );

	m_StartLevel = *( (BYTE*)pBuff );	//�Ǽ�
	pBuff += sizeof(BYTE);

	m_CurDurPoint = *( (INT*)pBuff );	//��ǰ�;�ֵ 
	pBuff += sizeof(INT);

	m_CurDamagePoint = *( (USHORT*)pBuff );	//��ǰ���˶�
	pBuff += sizeof(USHORT);

	m_GemCount = *( (BYTE*)pBuff );		//��Ƕ�ı�ʯ����
	pBuff += sizeof( BYTE );

	m_nEquipBindInfo = *( (EQUIP_BIND_INFO*)pBuff );		//װ���Ƿ��
	pBuff += sizeof( EQUIP_BIND_INFO );

	m_nEquipBindInfo2 = *( (EQUIP_BIND_INFO*)pBuff );	//װ���Ƿ��ǿ��(2����)
	pBuff += sizeof( EQUIP_BIND_INFO );

	m_nEquipHoleNum = *( (INT*)pBuff );			//װ���Ժ�Ŀ׶�����Ŀ
	pBuff += sizeof( INT );

	m_nEquipScore = *( (INT*)pBuff );			//װ���ķ������÷�������ĳֵ��װ��������������Լ�����װ��������
	pBuff += sizeof( INT );

	m_nSellPrice = *( (INT*)pBuff );			//�۳��۸�
	pBuff += sizeof( INT );

	m_nLevelNeed = *( (INT*)pBuff );		//����ȼ�
	pBuff += sizeof(INT);

	m_nMaxDurPoint = *( (INT*)pBuff );		//����;�ֵ
	pBuff += sizeof( INT );

	m_SetNum = *( (INT*)pBuff );		//��װid
	pBuff += sizeof( INT );

	m_RulerID = *( (CHAR*)pBuff );		//�¹���
	pBuff += sizeof( CHAR );

	m_EquipType = *( (BYTE*)pBuff );		//װ������
	pBuff += sizeof( BYTE );

	m_AttrCount = *( (BYTE*)pBuff );		//���Ե�����
	pBuff += sizeof( BYTE );

	m_PaiXing = *( (INT*)pBuff );		//װ��������
	pBuff += sizeof( INT );

	m_nEquipAttachGemNum = *( (BYTE*)pBuff );		//���ӱ�ʯ������
	pBuff += sizeof( BYTE );
	m_vEquipAttachGem.clear();
	for( int i=0; i<m_nEquipAttachGemNum; ++i )		//���ӵı�ʯ
	{
		m_vEquipAttachGem.push_back( *( (_ITEM_GEMINFO*)pBuff ) );
		pBuff += sizeof( _ITEM_GEMINFO );
	}

	m_nEquipAttributesNum = *( (BYTE*)pBuff );	//װ�������Ե�����
	m_vEquipAttributes.clear();
	pBuff += sizeof( BYTE );	//װ������
	for( int i=0; i<m_nEquipAttributesNum; ++i )
	{
		m_vEquipAttributes.push_back( *( (_ITEM_ATTR*)pBuff ) );
		pBuff += sizeof( _ITEM_ATTR );
	}

	m_nAffix_SkillNum = *( (BYTE*)pBuff );		//���ӵļ��ܵ�����
	pBuff += sizeof( BYTE );
	m_Affix_Skill.clear();
	for( int i=0; i<m_nAffix_SkillNum; ++i )	//���Ӽ���
	{
		m_Affix_Skill.push_back( *( (_ITEM_AFFIX_SKILL*)pBuff ) );
		pBuff += sizeof( _ITEM_AFFIX_SKILL );
	}

	m_nEffect_SkillNum = *( (BYTE*)pBuff );	//Ӱ��ļ��ܵ�����
	pBuff += sizeof( BYTE );
	m_Effect_Skill.clear();
	for( int i=0; i<m_nEffect_SkillNum; ++i )	//Ӱ�켼��
	{
		m_Effect_Skill.push_back( *( (_ITEM_EFFECT_SKILL*)pBuff ) );
		pBuff += sizeof( _ITEM_EFFECT_SKILL );
	}

	m_nszAttrGroupNum = *( (BYTE*)pBuff );		//���������Ĵ�С
	pBuff += sizeof( BYTE );
	m_szAttrGroup.clear();
	for ( int i=0; i<m_nszAttrGroupNum; ++i )	//��������
	{
		m_szAttrGroup.push_back( *( (CHAR*)pBuff ) );
		pBuff += sizeof( CHAR );
	}

	m_SoulBeadType = *( (BYTE*)pBuff );		//��ħ�Ļ�������
	pBuff += sizeof( BYTE );

	m_SoulBeadAttrCount = *( (BYTE*)pBuff );		//�������Ե�����
	pBuff += sizeof( BYTE );
	
	return (INT)(pBuff-pIn);

}


BOOL	CObject_Item_Equip::EXTRA_DEFINE::ReadVarAttr( CHAR* pOut,INT& OutLength)
{
	Assert( pOut );
	CHAR *pBuff = pOut;

	*( (INT*)pBuff ) = m_nAttSet;		//������
	pBuff += sizeof( INT );

	*( (BYTE*)pBuff ) = m_StartLevel;	//�Ǽ�
	pBuff += sizeof(BYTE);

	*( (INT*)pBuff ) = m_CurDurPoint;	//��ǰ�;�ֵ 
	pBuff += sizeof(INT);

	*( (USHORT*)pBuff ) = m_CurDamagePoint;	//��ǰ���˶�
	pBuff += sizeof(USHORT);

	*( (BYTE*)pBuff ) = m_GemCount;		//��Ƕ�ı�ʯ����
	pBuff += sizeof( BYTE );

	*( (EQUIP_BIND_INFO*)pBuff ) = m_nEquipBindInfo;		//װ���Ƿ��
	pBuff += sizeof( EQUIP_BIND_INFO );

	*( (EQUIP_BIND_INFO*)pBuff ) = m_nEquipBindInfo2;	//װ���Ƿ��ǿ��(2����)
	pBuff += sizeof( EQUIP_BIND_INFO );

	*( (INT*)pBuff ) = m_nEquipHoleNum;			//װ���Ժ�Ŀ׶�����Ŀ
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_nEquipScore;			//װ���ķ������÷�������ĳֵ��װ��������������Լ�����װ��������
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_nSellPrice;			//�۳��۸�
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_nLevelNeed;		//����ȼ�
	pBuff += sizeof(INT);

	*( (INT*)pBuff ) = m_nMaxDurPoint;		//����;�ֵ
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_SetNum;		//��װid
	pBuff += sizeof( INT );

	*( (CHAR*)pBuff ) = m_RulerID;		//�¹���
	pBuff += sizeof( CHAR );

	*( (BYTE*)pBuff ) = m_EquipType;		//װ������
	pBuff += sizeof( BYTE );

	*( (BYTE*)pBuff ) = m_AttrCount;		//���Ե�����
	pBuff += sizeof( BYTE );

	*( (INT*)pBuff ) = m_PaiXing;		//װ��������
	pBuff += sizeof( INT );

	*( (BYTE*)pBuff ) = m_nEquipAttachGemNum;		//���ӱ�ʯ������
	pBuff += sizeof( BYTE );
	for( int i=0; i<m_nEquipAttachGemNum; ++i )		//���ӵı�ʯ
	{
		*( (_ITEM_GEMINFO*)pBuff ) = m_vEquipAttachGem[i];
		pBuff += sizeof( _ITEM_GEMINFO );
	}

	*( (BYTE*)pBuff ) = m_nEquipAttributesNum;	//װ�������Ե�����
	pBuff += sizeof( BYTE );	//װ������
	for( int i=0; i<m_nEquipAttributesNum; ++i )
	{
		*( (_ITEM_ATTR*)pBuff ) = m_vEquipAttributes[i];
		pBuff += sizeof( _ITEM_ATTR );
	}

	*( (BYTE*)pBuff ) = m_nAffix_SkillNum;		//���ӵļ��ܵ�����
	pBuff += sizeof( BYTE );
	for( int i=0; i<m_nAffix_SkillNum; ++i )	//���Ӽ���
	{
		*( (_ITEM_AFFIX_SKILL*)pBuff ) = m_Affix_Skill[i];
		pBuff += sizeof( _ITEM_AFFIX_SKILL );
	}

	*( (BYTE*)pBuff ) = m_nEffect_SkillNum;	//Ӱ��ļ��ܵ�����
	pBuff += sizeof( BYTE );
	for( int i=0; i<m_nEffect_SkillNum; ++i )	//Ӱ�켼��
	{
		*( (_ITEM_EFFECT_SKILL*)pBuff ) = m_Effect_Skill[i];
		pBuff += sizeof( _ITEM_EFFECT_SKILL );
	}

	*( (BYTE*)pBuff ) = m_nszAttrGroupNum;		//���������Ĵ�С
	pBuff += sizeof( BYTE );
	for ( int i=0; i<m_nszAttrGroupNum; ++i )	//��������
	{
		*( (CHAR*)pBuff ) = m_szAttrGroup[i];
		pBuff += sizeof( CHAR );
	}

	*( (INT*)pBuff ) = m_SoulBeadType;		//��ħ�Ļ�������
	pBuff += sizeof( INT );

	*( (INT*)pBuff ) = m_SoulBeadAttrCount;		//�������Ե�����
	pBuff += sizeof( INT );

	OutLength = (INT)(pBuff-pOut);

	return false;
}
