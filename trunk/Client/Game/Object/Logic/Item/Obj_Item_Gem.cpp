
#include "StdAfx.h"
#include "Obj_Item_Gem.h"
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


const char* g_szGemAttName[] = 
{

	"equip_attr_pointstr",												//�����ٷֱ�																					
		"equip_attr_ratestr",			        							//��������
		"equip_attr_pointdex",		     									//���ݵ���
		"equip_attr_ratedex",			     								//���ݰٷֱ�
		"equip_attr_pointint",			    								//��������
		"equip_attr_rateint",		                    					//�����ٷֱ�
		"equip_attr_pointcon",                          					//���ʵ���
		"equip_attr_ratecon",	 											//���ʰٷֱ�
		"equip_attr_allbasepoint",	        								//�������Ե���
		"equip_attr_allbaserate",											//�������԰ٷֱ�
		"equip_attr_pointattack",	     									//ħ������
		"equip_attr_rateattack",	    									//ħ���ٷֱ�
		"equip_attr_attack_nearpoint",										//��������
		"equip_attr_attack_nearrate",										//�����ٷֱ�
		"equip_attr_attack_farpoint",										//Զ������
		"equip_attr_attack_farrate",	     								//Զ���ٷֱ�
		"equip_attr_defence_mpoint",	 									//ħ������
		"equip_attr_defence_mrate",	 										//ħ���ٷֱ�
		"equip_attr_defence_nearpoint",										//��������
		"equip_attr_defence_nearrate",										//�����ٷֱ�
		"equip_attr_defence_farpoint",										//Զ������
		"equip_attr_defence_farrate",										//Զ���ٷֱ�
		"equip_attr_point_maxhp",											//����ֵ���޵���
		"equip_attr_rate_maxhp",											//����ֵ���ްٷֱ�
		"equip_attr_point_maxmp",											//ħ��ֵ���޵���
		"equip_attr_rate_maxmp",											//ħ��ֵ���ްٷֱ�

		"equip_attr_wushi_maxrage",											//��ʿŭ������
		"equip_attr_jianxia_maxrage",										//����ŭ������
		"equip_attr_qishe_maxrage",											//������ŭ������
		"equip_attr_huoqiang_maxrage",										//��ǹ��ŭ������
		"equip_attr_xianzhi_maxrage",										//��֪ŭ������
		"equip_attr_saman_maxrage",											//����ŭ������

		"equip_attr_wushi_maxrage_rate",									//��ʿŭ�����ްٷֱ�
		"equip_attr_jianxia_maxrage_rate",									//����ŭ�����ްٷֱ�
		"equip_attr_qishe_maxrage_rate",									//������ŭ�����ްٷֱ�
		"equip_attr_huoqiang_maxrage_rate",									//��ǹ��ŭ�����ްٷֱ�
		"equip_attr_xianzhi_maxrage_rate",									//��֪ŭ�����ްٷֱ�
		"equip_attr_saman_maxrage_rate",									//����ŭ�����ްٷֱ�

		"equip_attr_wushi_rageres_point",									//��ʿŭ���ظ���
		"equip_attr_jianxia_rageres_point",									//����ŭ���ظ���
		"equip_attr_qishe_rageres_point",									//������ŭ���ظ���
		"equip_attr_huoqiang_rageres_pointe",								//��ǹ��ŭ���ظ���
		"equip_attr_xianzhi_rageres_point",									//��֪ŭ���ظ���
		"equip_attr_saman_rageres_point",									//����ŭ���ظ���


		"equip_attr_wushi_rageres_rate",									//��ʿŭ���ظ��ٷֱ�
		"equip_attr_jianxia_rageres_rate",									//����ŭ���ظ��ٷֱ�
		"equip_attr_qishe_rageres_rate",									//������ŭ���ظ��ٷֱ�
		"equip_attr_huoqiang_rageres_rate",									//��ǹ��ŭ���ظ��ٷֱ�
		"equip_attr_xianzhi_rageres_rate",									//��֪ŭ���ظ��ٷֱ�
		"equip_attr_saman_rageres_rate",									//����ŭ���ظ��ٷֱ�

		"equip_attr_hit",													//����
		"equip_attr_miss",													//����
		"equip_attr_rate_hit",												//������
		"equip_attr_sock",			     									//����
		"equip_attr_toughness",			                					//����
		"equip_attr_sock_rate",												//������
		"equip_attr_cold_attack",											//����������
		"equip_attr_rate_cold_attack",										//�������ٷֱ�
		"equip_attr_fire_attack",											//�𹥻�����
		"equip_attr_rate_fire_attack",										//�𹥻��ٷֱ�
		"equip_attr_light_attack",			 								//�繥������
		"equip_attr_rate_light_attack",				 						//�繥���ٷֱ�
		"equip_attr_poison_attack",											//����������
		"equip_attr_rate_poison_attack",									//�������ٷֱ�
		"equip_attr_cold_resist",											//����������
		"equip_rate_old_resist",											//�������ٷֱ�
		"equip_attr_fire_resist",											//���������
		"equip_attr_rate_fire_resist",										//������ٷֱ�
		"equip_attr_light_resist",											//���������
		"equip_rate_light_resist",											//������ٷֱ�
		"equip_poison_resist",												//����������
		"equip_rate_poison_resist",											//�������ٷֱ�
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
		"equip_attr_rage_restore",											//ŭ���ظ�
		"equip_attr_crit_hurt",												//���ӱ����˺�
		"equip_attr_shield_all",											//���ж�
		"equip_attr_shield_near",											//���̶�
		"equip_attr_shield_m",												//ħ����
		"equip_attr_shield_far",											//Զ�̶�
		"equip_attr_shield_cold",											//����
		"equip_attr_shield_fire",											//���
		"equip_attr_shield_light",											//���
		"equip_attr_shield_poison",											//����
		"equip_attr_shield_mp_reduce",										//���������˺���
		"equip_attr_skill_level_inc",										//���ӵ������ܵȼ�
		"equip_attr_allskill_level_inc",									//�������м��ܵȼ�
		"equip_attr_wushi_hurt_resist",										//�ֿ���ʿְҵ�˺�
		"equip_attr_jianxia_hurt_resist",									//�ֿ�����ְҵ�˺�
		"equip_attr_qishe_hurt_resist",			 							//�ֿ�������ְҵ�˺�
		"equip_attr_huoqiang_hurt_resist",									//�ֿ���ǹ��ְҵ�˺�
		"equip_attr_xianzhi_hurt_resist",									//�ֿ���ְ֪ҵ�˺�
		"equip_attr_saman_hurt_resist",			 							//�ֿ�����ְҵ�˺�
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
		"equip_base_rate_attack_m",											//����ħ�������ٷֱ�
		"equip_base_rate_defence_enear",									//�������̷����ٷֱ�
		"equip_base_rate_defence_far",										//����Զ�̷����ٷֱ�
		"equip_base_rate_defence_m",										//����ħ�������ٷֱ�	

		"equip_base_attack_near",											//�������̹���
		"equip_base_attack_far",										    //����Զ�̹���
		"equip_base_attack_m",												//����ħ������
		"equip_base_defence_near",											//�������̷���
		"equip_base_defence_far",											//����Զ�̷���
		"equip_base_defence_m"	,										    //����ħ������

		"equip_iattribute_rate_nearatt_reduce",					    		//���ٷֱȵ������̹���
		"equip_iiattribute_rate_faratt_reduce",								//���ٷֱȵ���Զ�̹���
		"equip_iattribute_rate_matt_reduce",									//���ٷֱȵ���ħ������
		"equip_iattribute_rate_coldatt_reduce",								//���ٷֱȵ���������
		"equip_iattribute_rate_fireatt_reduce",								//���ٷֱȵ����𹥻�
		"equip_iattribute_rate_lightatt_reduce",							//���ٷֱȵ����繥��
		"equip_iattribute_rate_poison_reduce",								//���ٷֱȵ���������

		 
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

//������ϸ����
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
	//	"����: %s\n"
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

// �õ���ʯ�ĵȼ�
INT		CObject_Item_Gem::GetGemLevel()
{
	if(m_theBaseDef)
	{
		return  1;
	}

	return 0;
}


// 20100408 AddCodeBegin
// �õ���Ʒ����ļ۸�
INT		CObject_Item_Gem::GetItemBasePrice()
{
	if(m_theBaseDef)
	{
		return (m_theBaseDef->nPrice );
	}

	return 0;
}
// 20100408 AddCodeEnd


// �õ���Ʒ����npc�ļ۸�
INT		CObject_Item_Gem::GetItemPrice()
{
	if(m_theBaseDef)
	{	
		return m_theBaseDef->nSalePrice;
	}

	return -1;
}


// �õ���ʯ������Ϣ
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


// �õ�type��Ϣ
INT	CObject_Item_Gem::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
	return -1;
}


// �õ���Ʒ���������� 
LPCTSTR	CObject_Item_Gem::GetItemTableTypeDesc()
{
	if(m_theBaseDef)
		return m_theBaseDef->szTypeDesc;

	return NULL;
}
//��������

 LPCTSTR	CObject_Item_Gem::GetDropVisualID(VOID)	const


{
	return NULL;

}

//��ʾ������ɫ
LPCTSTR	 CObject_Item_Gem::GetDropVisColor(VOID) const 
  {
	  return NULL;
  }