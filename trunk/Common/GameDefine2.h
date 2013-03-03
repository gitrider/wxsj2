//***************************************************************************************************
// �ļ�����:	GameDefine2.h
// ϵͳ����:	
// ģ������:	GameDefine
// ����˵��:	��Ϸ��ö��ֵ
//
// ����ĵ�:	
// ��    ��:	liam
// ��������:	
// ��    ��:	
// �����Ȩ:	
//
//�޸ļ�¼:
//          1.
//				�� �� ��:liam
//				�޸�ʱ��:2007/10/30
//				�޸�����:���Ե�һ�����ع�(����μ��ĵ�)
//          2.
//				�� �� ��:liam
//				�޸�ʱ��:2007/11/02
//				�޸�����:���Եڶ������ع�(����μ��ĵ�)
//***************************************************************************************************
#ifndef __GAMEDEFINE2_H__
#define __GAMEDEFINE2_H__

#include "GameDefine.h"

enum BAGPOS_TYPE
{
	BAG_BASE	=-1,
	BAG_EXTRA1	,
	BAG_EXTRA2	,
	BAG_EXTRA3	,
	BAG_EXTRA4	,
	BAG_NUM		,
};
//��Ʒ���䷽ʽ
enum ITEM_DISTRIB_MODE
{
	ITEM_FREE_DISTRIB       = 0,//����ʰȡ
	ITEM_TEAMLEADER_DISTRIB = 1,//�ӳ�ʰȡ
	ITEM_INTURNS_DISTRIB	= 2,//����ʰȡ
	ITEM_SEPARATE_DISTRIB	= 3,//�ֱ�ʰȡ
	ITEM_DISTRIB_NUM,
};

//��ɫ������ĸ������
enum ENUM_DIE_RESULT_CODE
{
	DIE_RESULT_CODE_INVALID	= -1,
	ORIGIN_LOSSLESS_RELIVE,			//ԭ�����𸴻�
	ORIGIN_DAMAGE_RELIVE,			//ԭ�����𸴻�
	CITY_POS_RELIVE,				//���ж��㸴��
	DIE_RESULT_CODE_NUMBERS
};


enum COUNTRY_ATTRIBUTE
{
	COUATTRIBUTE_XINYUE			=0, //���º���
	COUATTRIBUTE_JINZHANG		,	//���ʺ���
	COUATTRIBUTE_XIYU			,	//���򺹹�
	COUATTRIBUTE_DAMO			,	//��Į����
	COUATTRIBUTE_NUMBER			,//��������
};
//����ģʽ
enum SUB_TRAIN_MODE
{
	SUBTRAIN_MODE1				=0, //��ͨģʽ
	SUBTRAIN_MODE1POINT5		=1,
	SUBTRAIN_MODE2				=2,
	SUBTRAIN_MODE3				=3,
};

//�������
enum MENPAI_ATTRIBUTE
{
	MATTRIBUTE_UNKNOW			=-1,//�Ƿ���ֵ
	MATTRIBUTE_SHAOLIN_GWS		=0,	//������[����ɮ]
	MATTRIBUTE_SHAOLIN_TLH		=1,	//������[���޺�]
	MATTRIBUTE_GAIBANG_GGB		=2,	//ؤ��[��ؤ��]
	MATTRIBUTE_GAIBANG_ZGB		=3,	//ؤ��[��ؤ��]
	MATTRIBUTE_WUDANG_JWD		=4,	//�䵱��[���䵱]
	MATTRIBUTE_WUDANG_QWD		=5,	//�䵱��[���䵱]
	MATTRIBUTE_TANGMEN_DT		=6,	//����[����]
	MATTRIBUTE_TANGMEN_XJT		=7,	//����[������]
	MATTRIBUTE_XIAOYAO_XXY		=8,	//��ң��[����ң]
	MATTRIBUTE_XIAOYAO_ZXY		=9,	//��ң��[ָ��ң]
	MATTRIBUTE_YAOWANGGU_KYW	=10,	//ҩ����[��ҩ��]
	MATTRIBUTE_YAOWANGGU_YYW	=11,	//ҩ����[ҽҩ��]
	MATTRIBUTE_TAOHUAGU_DTH		=12,	//�һ���[���һ�]
	MATTRIBUTE_TAOHUAGU_YTH		=13,	//�һ���[���һ�]
	MATTRIBUTE_EMEI_CEM			=14,	//������[�̶�ü]
	MATTRIBUTE_EMEI_FEM			=15,	//������[����ü]
	MATTRIBUTE_WUMENPAI			=16,	//������
	MATTRIBUTE_NUMBER,				//��������(8�����ɣ�ÿ�ɷ��ڹ����⹦)
};

#define	CLIENT_GAOMO_MENPAI	17
#define  CLIENT_MENPAI_NUM	(MATTRIBUTE_NUMBER +1)

//ϵ��
enum COMMON_S
{
	COMMON_S1		=0,
	COMMON_S2,
	COMMON_S3,
	COMMON_S4,
	COMMON_S5,
	COMMON_S_NUMBER,
};

// ��ϵ
enum ENUM_RELATION
{
	RELATION_INVALID	= -1,
	RELATION_ENEMY,			// �ж�
	RELATION_FRIEND,			// �Ѻ�

	RELATION_NUMBERS
};

enum ENUM_NPC_AI_ATTR
{
	NPC_AI_TYPE_INVALID		= -1,	// INVALID
	NPC_AI_TYPE_SCANNPC		= 0,	// ����NPC
	NPC_AI_TYPE_NOTSCANNPC		= 1,	// ������NPC
	NPC_AI_TYPE_CANNOTATTACK	= 2,	// ���ɹ���NPC

};

enum ITEM_POS_TYPE
{
	ENUM_BAG				=0,
	ENUM_EQUIP_POINT,
	INVALID_ITEM_POS_TYPE, 
};

//������Ϣ���(��ӦBaseValueTable.ini��Ϣ)
enum ATTRIBUTEINFO_TYPE
{
	AINFOTYPE_BASE_HP			=0,		//��ʼ��������														9
	AINFOTYPE_LEVEL_CON         ,       //�ȼ������ʵ�Ӱ��ϵ��												4
	AINFOTYPE_CON_HP			,		//���ƶ���������Ӱ��ϵ��											10
	AINFOTYPE_LEVEL_HP			,		//�ȼ�����������Ӱ��ϵ��											11
	AINFOTYPE_BASE_HPRESTORE	,		//��ʼ�����ظ�														16

	AINFOTYPE_LEVEL_HPRESTORE	,		//�ȼ��������ظ�Ӱ��ϵ��											15
	AINFOTYPE_BASE_MP			,		//��ʼ��������														12
	AINFOTYPE_INT_MP			,		//�������������޵�Ӱ��ϵ��											13
	AINFOTYPE_LEVEL_MP			,		//�ȼ�����������Ӱ��ϵ��											14
	AINFOTYPE_BASE_MPRESTORE	,		//��ʼ�����ظ�														18

	AINFOTYPE_LEVEL_MPRESTORE	,		//�ȼ��������ظ�Ӱ��ϵ��											17
	AINFOTYPE_BASE_RAGE			,		//��ʼŭ������														40
	AINFOTYPE_INT_RAGE			,		//������ŭ������Ӱ��ϵ��											78
	AINFOTYPE_LEVEL_RAGE		,		//�ȼ���ŭ��Ӱ��ϵ��												62
	AINFOTYPE_LEVEL_RAGERESTORE	,		//�ȼ���ŭ���ظ�Ӱ��ϵ��											65

	AINFOTYPE_LEVEL_STRENGTH      ,     //�ȼ�������Ӱ��													1
	AINFOTYPE_BASE_NEARATTACK     ,     //��ʼԶ���ڹ�����													30
	AINFOTYPE_INT_NEARATTACK	 ,     //������Զ���ڹ�������Ӱ��ϵ��										29	
	AINFOTYPE_LEVEL_NEARATTACK    ,     //�ȼ���Զ���ڹ�������Ӱ��ϵ��										28
	AINFOTYPE_LEVEL_DEX	          ,     //�ȼ�������Ӱ��ϵ��												2

	AINFOTYPE_BASE_FARATTACK      ,     //��ʼԶ���⹦����													24
	AINFOTYPE_STR_FARATTACK       ,     //������Զ���⹦������Ӱ��ϵ��										23
	AINFOTYPE_LEVEL_FARATTACK     ,      //�ȼ���Զ���⹦������Ӱ��ϵ��										22	
	AINFOTYPE_BASE_NEARDEFENCE_P	,	//��ʼ���̷���														79
	AINFOTYPE_LEVEL_NEARDEFENCE_P	,	//�ȼ���Զ���ڹ�������Ӱ��ϵ��										34

	AINFOTYPE_BASE_FARDEFENCE_P 	,	//��ʼԶ���⹦����													48
	AINFOTYPE_LEVEL_FARDEFENCE_P	,	//�ȼ���Զ���⹦������Ӱ��ϵ��										32
	AINFOTYPE_BASE_TOUGHNESS      ,     //��ʼ����															42	
	AINFOTYPE_LEVEL_TOUGHNESS     ,     //�ȼ������Ե�Ӱ��ϵ��												41
	AINFOTYPE_INT_TOUGHNESS		,   //���ʶ����Ե�Ӱ��ϵ��							
	AINFOTYPE_LEVEL_INT			,   //�ȼ���������Ӱ��ϵ��													3
	

	AINFOTYPE_BASE_ATTACK_MAGIC_NEAR ,	//��ʼ�����ڹ�����													27
	AINFOTYPE_DEX_ATTACK_MAGIC_NEAR	,	//���Խ����ڹ�������Ӱ��ϵ��										26
	AINFOTYPE_LEVEL_ATTACK_MAGIC_NEAR	,	//�ȼ��Խ����ڹ�������Ӱ��ϵ��									25
	AINFOTYPE_BASE_DEFENCE_MAGIC_NEAR	,	//��ʼ�����ڹ�����												49
	AINFOTYPE_LEVEL_DEFENCE_MAGIC_NEAR	,	//�ȼ��Խ����ڹ�������Ӱ��ϵ��									33

	AINFOTYPE_BASE_HIT			,	//��ʼ����																80	
	AINFOTYPE_STR_HIT			,	//���������е�Ӱ��ϵ��													5
	AINFOTYPE_LEVEL_HIT			,	//�ȼ������е�Ӱ��ϵ��													6
	AINFOTYPE_BASE_MISS			,	//��ʼ����																81
	AINFOTYPE_DEX_MISS			,	//�������ܵ�Ӱ��ϵ��													7

	AINFOTYPE_LEVEL_MISS        ,	//�ȼ������ܵ�Ӱ��ϵ��													8
	AINFOTYPE_BASE_CRITRATE		,	//��ʼ����																82
	AINFOTYPE_INT_CRITRATE		,	//�����Ի����ʵ�Ӱ��ϵ��												35
	AINFOTYPE_DEX_CRITRATE		,	//���ݶԻ����ʵ�Ӱ��ϵ��												36
	AINFOTYPE_LEVEL_CRITRATE	,	//�ȼ��Ա�����Ӱ��ϵ��													83

	AINFOTYPE_ATTACK_FLUCTUATION,	//����������															61
	AINFOTYPE_BASE_ATTACK_MAGIC_FAR ,	//��ʼ�����⹦����													21
	AINFOTYPE_STR_ATTACK_MAGIC_FAR	,	//�����Խ����⹦������Ӱ��ϵ��										20
	AINFOTYPE_LEVEL_ATTACK_MAGIC_FAR	,	//�ȼ��Խ����⹦������Ӱ��ϵ��									19
	AINFOTYPE_BASE_DEFENCE_MAGIC_FAR	,	//��ʼ�����⹦����												47

	AINFOTYPE_LEVEL_DEFENCE_MAGIC_FAR	,	//�ȼ��Խ����⹦������Ӱ��ϵ��									31
	AINFOTYPE_MAX_PNEUMA,				//��ʼԪ�����ֵ													38
	AINFOTYPE_LEVEL_MAX_PNEUMA,			//�ȼ���Ԫ�����ֵ��Ӱ��ϵ��										63
	AINFOTYPE_RESTORE_PNEUMA,			//��ʼԪ���ظ�														37
	AINFOTYPE_LEVEL_RESTORE_PNEUMA,		//�ȼ���Ԫ���ظ�Ӱ��ϵ��											66

	AINFOTYPE_BASE_ATTACK_GOLD,		//��ʼ��ϵ������														51
	AINFOTYPE_BASE_ATTACK_WOOD ,	//��ʼľϵ������														52
	AINFOTYPE_BASE_ATTACK_WATER,	//��ʼˮϵ������														53
	AINFOTYPE_BASE_ATTACK_FIRE ,	//��ʼ��ϵ������														54
	AINFOTYPE_BASE_ATTACK_SOIL,		//��ʼ��ϵ������														55

	AINFOTYPE_BASE_DEFENCE_GOLD,	//��ʼ��ϵ������														56
	AINFOTYPE_BASE_DEFENCE_WOOD ,	//��ʼľϵ������														57
	AINFOTYPE_BASE_DEFENCE_WATER,	//��ʼˮϵ������														58
	AINFOTYPE_BASE_DEFENCE_FIRE ,	//��ʼ��ϵ������														59
	AINFOTYPE_BASE_DEFENCE_SOIL,	//��ʼ��ϵ������														60

	AINFOTYPE_LEVEL_MAX_VIGOR,		//ÿ��һ�������������ӵ���												64
	AINFOTYPE_LEVEL_RESTORE_VIGOR,	//�ȼ��Ի����ظ���Ӱ��ϵ��												67
	AINFOTYPE_LEVEL_ATTACK_GOLD,	//�ȼ��Խ�ϵ��������Ӱ��ϵ��											68
	AINFOTYPE_LEVEL_ATTACK_WOOD ,	//�ȼ�����ϵ��������Ӱ��ϵ��											69
	AINFOTYPE_LEVEL_ATTACK_WATER,	//�ȼ���ˮϵ��������Ӱ��ϵ��											70

	AINFOTYPE_LEVEL_ATTACK_FIRE ,	//�ȼ��Ի�ϵ��������Ӱ��ϵ��											71
	AINFOTYPE_LEVEL_ATTACK_SOIL,	//�ȼ���ľϵ��������Ӱ��ϵ��											72
	AINFOTYPE_LEVEL_DEFENCE_GOLD,	//�ȼ��Խ�ϵ�ֿ�����Ӱ��ϵ��											73
	AINFOTYPE_LEVEL_DEFENCE_WOOD ,	//�ȼ���ľϵ�ֿ�����Ӱ��ϵ��											74
	AINFOTYPE_LEVEL_DEFENCE_WATER,	//�ȼ���ˮϵ�ֿ�����Ӱ��ϵ��											75

	AINFOTYPE_LEVEL_DEFENCE_FIRE ,	//�ȼ��Ի�ϵ�ֿ�����Ӱ��ϵ��											76
	AINFOTYPE_LEVEL_DEFENCE_SOIL,	//�ȼ�����ϵ�ֿ�����Ӱ��ϵ��											77
	AINFOTYPE_RESTORE_RAGE_FROM_EXP,//����ת��ϵ��(����ŭ��)												39

	AINFOTYPE_BASE_VIGOR_RESTORE,	//��ʼ�����ָ�ֵ

	AINFOTYPE_NUMBER,					//������Ϣ����
};
//��������
enum ATTACK_ERROR
{
	AERROR_NOTLIVE		=-1,	//Ŀ�����ˣ�������Ч		
	AERROR_NOTSCENE		=-2,	//Ŀ�겻�ڵ�ǰ����
	AERROR_OUTRANGE		=-3,	//��������Ŀ�����̫Զ
	AERROR_INVALID		=-4,	//��ЧĿ��
	AERROR_HIT			=-5,	//û������
	AERROR_IAMDEAD		=-6,	//�������Ǹ�����
	AERROR_BREAK		=-7,	//���������
	AERROR_CANNOTATTACK =-8,	//���ܹ�����Ŀ��

};
//��������
enum ESPECIAL_ABILITY_NUMBER
{
	EAN_GEM_COMPOUNDED	= MAX_CHAR_ABILITY_NUM + 1,	// ��ʯ�ϳ�
	EAN_GEM_EMBEDDED,								// ��ʯ��Ƕ
};

// ��ȴID, ��д����������
enum
{
		COOLDOWN_LIST_SIZE = 255,
};

enum
{
	COMMONCOOLDOWN_TIME = 750,
};
//��������
enum SKILL_DEPLETE_LOGIC
{
	DEPLETE_INVALID = -1,
	DEPLETE_MANA = 0,	//
	DEPLETE_RAGE,		//ŭ��
	DEPLETE_STRIKE_POINT,//
	DEPLETE_ITEM_STD,
};
//����״̬
enum EFFECT_STATUS
{
	ESTATUS_COLD		=0,	//�ٻ�
	ESTATUS_FIRE		=1, //����
	ESTATUS_LIGHT		=2,	//ѣ��
	ESTATUS_POISON		=3,	//�ж�

	ESTATUS_NUMBER, //����״̬����
};

enum SKILL_ATTRIBUTE
{
	SATTRIBUTE_ATTACK_P			=0, //��������������
	SATTRIBUTE_RATE_ATTACK_P	=1, //�����������ٷֱ�

	SATTRIBUTE_NUMBER, //����Ӱ�����Ե�����
};

//����AI��Ԫ��
enum MONSTER_AI_PRAM
{
	AIPARAM_SCANTIME			=0,	//�����ֵ����0����Ϊ����������ֵ�ĺ������ɨ����
									//���С�ڵ���0����Ϊ����������
	AIPARAM_RANDMOVETIME		=1,	//����ƶ�λ�õ�ʱ���������룩
	AIPARAM_CANNOTATTACK		=2,	//�޷������Ķ���(�Ѿ�����, �Ƿ��޵�����MonsterAttrExTable.txt����Ϊ��)
	AIPARAM_RETURN				=3, //�����ֵ����0���򵱹����������ش��ڴ�ֵʱ���������׷��
	AIPARAM_SCANENEMYDIST		=4, //ɨ����˵�������	
	AIPARAM_SCANTEAMMATEDIST	=5, //ɨ����ѵ�������	
	AIPARAM_RESETTARGET_DIST	=6, //�����ǰ���ƶ�Ŀ��͵��˵�λ��֮��ľ�����ڴ���ֵ
									//����Ҫ�����趨�ƶ�Ŀ��
	AIPARAM_PATROLTIME			=7,//Ѳ�ߵ�ʱ��������С�ڵ���0���򲻽���Ѳ��
	AIPARAM_STRIKEBACK			=8,//�Ƿ�ỹ��

	AIPARAM_NUMBER,
};
//��ɫ����
enum ENUM_UPDATE_CHAR_ATT
{
	UPDATE_CHAR_ATT_JOB		=0,			//ְҵ
	UPDATE_CHAR_ATT_COUNTRY,			//����
	UPDATE_CHAR_ATT_DATA_ID,		    // ������ԴID
	UPDATE_CHAR_ATT_NAME,				// ����
	UPDATE_CHAR_ATT_COUNTRYTITLE,		// ����ͷ��
	UPDATE_CHAR_ATT_GUILDTITLE,			// ���ͷ��
    UPDATE_CHAR_ATT_NORMALTITLE,		// �淨ͷ��
	UPDATE_CHAR_ATT_LEVEL,				// �ȼ�
	UPDATE_CHAR_ATT_HP_PERCENT,			// HP�ٷֱ�
	UPDATE_CHAR_ATT_MP_PERCENT,			// MP�ٷֱ�
	UPDATE_CHAR_ATT_PNEUMA_PERCENT,		// Ԫ���ٷֱ�
	UPDATE_CHAR_ATT_RAGE,				// ŭ��
	UPDATE_CHAR_ATT_STEALTH_LEVEL,		// ������
	UPDATE_CHAR_ATT_SIT,				// �Ƿ�Ϊ����״̬
	UPDATE_CHAR_ATT_MOVE_SPEED,			// �ƶ��ٶ�
	UPDATE_CHAR_ATT_ATTACK_SPEED,		// �����ٶ�
	UPDATE_CHAR_ATT_CAMP_ID,			// ��ӪID
	UPDATE_CHAR_ATT_PORTRAIT_ID,		// ͷ��ID
	UPDATE_CHAR_ATT_MODEL_ID,			// ģ��ID
	UPDATE_CHAR_ATT_MOUNT_ID,			// ����ID
	UPDATE_CHAR_ATT_MOUNT_INDEX,		// ����Index                             [7/23/2010 �¾���]
	UPDATE_CHAR_ATT_AITYPE,				// AI����
	UPDATE_CHAR_ATT_PLAYER_DATA,		// 00|0000|0000|0000|0000
										//     |    |    |    |  
										//    ͷ�� ���� ͷ�� ����
										//    ��ɫ ��ɫ ģ�� ģ��
	UPDATE_CHAR_IS_IN_STALL,			// �Ƿ��̯״̬
	UPDATE_CHAR_STALL_NAME,				// ̯λ��

	UPDATE_CHAR_STALL_PUBLIC,			// ̯λ�Ƿ���(������ʾ̯λ�Ƹ��������)	20100706 BLL

	UPDATE_CHAR_OCCUPANT,				// ռ����(��˭��ģ�������˭��)
	UPDATE_CHAR_OWNER,					// ӵ����(��˭���Ƶ�)
	UPDATE_CHAR_BUSOBJID,					//Bus ID

	UPDATE_CHAR_ATT_NUMBERS //������Ϣ����,(Ŀǰ���ܳ���32)
};
//������ϸ��Ϣ
enum ENUM_DETAIL_ATTRIB
{
	DETAIL_ATTRIB_LEVEL		= 0,		//�ȼ�
	DETAIL_ATTRIB_HP,					//����ֵ
	DETAIL_ATTRIB_MP,					//ħ��ֵ
	DETAIL_ATTRIB_PNEUMA,				//Ԫ��ֵ
	DETAIL_ATTRIB_EXP,					//����
	DETAIL_ATTRIB_MONEY,				//����

	DETAIL_ATTRIB_STR,					//���� ����
	DETAIL_ATTRIB_CON,					//���� ����
	DETAIL_ATTRIB_INT,					//���� ����
	DETAIL_ATTRIB_DEX,					//�� ����
	DETAIL_ATTRIB_POINT_REMAIN,			//ʣ�����
	DETAIL_ATTRIB_SKILLPOINT_REMAIN,    //ʣ�༼�ܵ�
	DETAIL_ATTRIB_S_SKILLPOINT,

	DETAIL_ATTRIB_ATT_NEAR,             //���̹���
	DETAIL_ATTRIB_ATT_FAR,              //Զ�̹���
	DETAIL_ATTRIB_DEF_NEAR,             //���̷���
	DETAIL_ATTRIB_DEF_FAR,              //Զ�̷���

	DETAIL_ATTRIB_ATT_MAGIC_NEAR,		//�����ڹ�������
	DETAIL_ATTRIB_ATT_MAGIC_FAR,		//�����⹦������

	DETAIL_ATTRIB_DEF_MAGIC_NEAR,		//�����ڹ�������
	DETAIL_ATTRIB_DEF_MAGIC_FAR,		//�����⹦������

	DETAIL_ATTRIB_MAXHP,				//�������ֵ
	DETAIL_ATTRIB_MAXMP,				//���ħ��ֵ
	DETAIL_ATTRIB_MAXPNEUMA,			//���Ԫ��ֵ
	DETAIL_ATTRIB_HP_RESPEED,			//HP�ָ��ٶ�  ��/��
	DETAIL_ATTRIB_MP_RESPEED,			//MP�ָ��ٶ�  ��/��
	DETAIL_ATTRIB_VIGOR_RESPEED,		//�����ָ��ٶ�  ��/��

	DETAIL_ATTRIB_TOUGHNESS,            //����

	DETAIL_ATTRIB_HIT,					//������
	DETAIL_ATTRIB_MISS,					//������
	DETAIL_ATTRIB_CRITRATE,				//������
	
	DETAIL_ATTRIB_MAXRAGE,				//ŭ������
	DETAIL_ATTRIB_RAGE,					//ŭ��
	DETAIL_ATTRIB_RAGE_RESPEED,			//ŭ���ָ�
	DETAIL_ATTRIB_CRITICALHURT,			//�����˺�
	DETAIL_ATTRIB_STRIKE_POINT,			//������

	DETAIL_ATTRIB_MOVESPEED,			//�ƶ��ٶ�
	DETAIL_ATTRIB_ATTACKSPEED,			//�����ٶ�

	DETAIL_ATTRIB_ATTACK_GOLD,			//�𹥻�	
	DETAIL_ATTRIB_DEFENCE_GOLD,			//�����

	DETAIL_ATTRIB_ATTACK_WOOD,			//ľ����
	DETAIL_ATTRIB_DEFENCE_WOOD,			//ľ����

	DETAIL_ATTRIB_ATTACK_WATER,			//ˮ����
	DETAIL_ATTRIB_DEFENCE_WATER,		//ˮ����

	DETAIL_ATTRIB_ATTACK_FIRE,			//�𹥻�	
	DETAIL_ATTRIB_DEFENCE_FIRE,			//�����

	DETAIL_ATTRIB_ATTACK_SOIL,			//������	
	DETAIL_ATTRIB_DEFENCE_SOIL,			//������

	DETAIL_ATTRIB_NEARATT_REDUCE,		///���̼���
	DETAIL_ATTRIB_FARATT_REDUCE,		///Զ�̼���
	DETAIL_ATTRIB_MAGIC_NEAR_ATT_REDUCE,	///�����ڹ�����
	DETAIL_ATTRIB_MAGIC_FAR_ATT_REDUCE,	///�����⹦����
	DETAIL_ATTRIB_DREAD_RESIST,			///�־忹��
	DETAIL_ATTRIB_COMA_RESIST,			///���Կ���
	DETAIL_ATTRIB_HUSH_RESIST,			///��Ĭ����
	DETAIL_ATTRIB_UNARM_RESIST,			///��е����
	DETAIL_ATTRIB_ATTACKSPEED_RESIST,	///�����ٿ���
	DETAIL_ATTRIB_SKILLSPEED_RESIST,	///��ʩ�ٿ���
	DETAIL_ATTRIB_MOVEREDUCE_RESIST,	///�����ٿ���

	DETAIL_ATTRIB_COUNTRY,              //����
	DETAIL_ATTRIB_MEMPAI,				//����
	DETAIL_ATTRIB_GUILD,				//����
    DETAIL_ATTRIB_GUILD_POINT,			//���ɹ��׶�
    DETAIL_ATTRIB_PK_VALUE,			    //ɱ��
    DETAIL_ATTRIB_HONOR,			    //����
	DETAIL_ATTRIB_FAMILY,				//����
	DETAIL_ATTRIB_GUILD_OFFICIAL,		//����ְ
	DETAIL_ATTRIB_CAMP,					//��Ӫ
	DETAIL_ATTRIB_DATAID,				//DataID
	DETAIL_ATTRIB_PORTRAITID,			//ͷ��
	DETAIL_ATTRIB_FACEMODEL,
	DETAIL_ATTRIB_MODELID,				//����
	DETAIL_ATTRIB_MOUNTID,				//����
	DETAIL_ATTRIB_MOUNTINDEX,			//��������                  [7/23/2010 �¾���]
	DETAIL_ATTRIB_CURRENT_PET_GUID,		//��ǰ�ĳ���GUID

	DETAIL_ATTRIB_LIMIT_MOVE,			//�Ƿ����Ʋ����ƶ�
	DETAIL_ATTRIB_CAN_ACTION1,			//�������Ʊ��1.
	DETAIL_ATTRIB_CAN_ACTION2,			//�������Ʊ��2.	

	DETAIL_ATTRIB_VIGOR,				//����
	DETAIL_ATTRIB_MAX_VIGOR,			//��������
	DETAIL_ATTRIB_ENERGY,				//����
	DETAIL_ATTRIB_MAX_ENERGY,			//��������

	DETAIL_ATTRIB_GOODBADVALUE,			//�ƶ�ֵ

	DETAIL_ATTRIB_GUID,			//GUID

	//DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS,	//�������ɱ�־
	//DETAIL_ATTRIB_MISSION_KILLOBJECT_FLAGS,	//�����ɱ�ֱ�־
	//DETAIL_ATTRIB_MISSION_ENTERAREA_FLAGS,	//����������־
	//DETAIL_ATTRIB_MISSION_ITEMCHANGED_FLAGS,	//����ĵ��߸��±�־


	DETAIL_ATTRIB_NUMBERS				
};

//�˺�����
typedef enum DAMAGE_TYPE
{
	//DAMAGE_NOSPECIALTYPE = 0,
	DAMAGE_TYPE_NEAR,
	DAMAGE_TYPE_FAR,		//Զ���˺�
	DAMAGE_TYPE_MAGIC_NEAR,	//�����ڹ��˺�
	DAMAGE_TYPE_MAGIC_FAR,		//�����⹦�˺�
	DAMAGE_TYPE_GOLD,	//��ϵ�˺�
	DAMAGE_TYPE_WOOD,	//ľϵ�˺�
	DAMAGE_TYPE_WATER,	//ˮϵ�˺�
	DAMAGE_TYPE_FIRE,	//��ϵ�˺�
	DAMAGE_TYPE_SOIL,	//��ϵ�˺�
	DAMAGE_TYPE_NUMBER,	//�˺���������
} DamageType_T;


// ��ɫ����
enum ENUM_CHAR_ATT
{
	CHAR_ATT_INVALID	= -1,
	CHAR_ATT_STR,				// ���� ����
	//CHAR_ATT_SPR,				// ���� ����
	CHAR_ATT_CON,				// ���� ����
	CHAR_ATT_INT,				// ���� ����
	CHAR_ATT_DEX,				// �� ����

	CHAR_ATT_HP,				// HP
	CHAR_ATT_MP,				// MP
	CHAR_ATT_MAX_HP,			// HP����
	CHAR_ATT_MAX_MP,			// MP����

	CHAR_ATT_RESTORE_HP,		// HP�ظ�
	CHAR_ATT_RESTORE_MP,		// MP�ظ�

	CHAR_ATT_HIT,				// ������
	CHAR_ATT_MISS,				// ����
	CHAR_ATT_CRIT_RATE,			// ������

	CHAR_ATT_SPEED,				// �ƶ��ٶ�
	CHAR_ATT_ATTACK_SPEED,		// �����ٶ�

	//CHAR_ATT_ATTACK_PHYSICS,	// ������
	//CHAR_ATT_DEFENCE_PHYSICS,	// �������
	
	CHAR_ATT_ATTACK_NEAR,	    // ���̹���
	CHAR_ATT_DEFENCE_NEAR,	    // ���̷���

	CHAR_ATT_ATTACK_FAR,	    // Զ�̹���
	CHAR_ATT_DEFENCE_FAR,	    // Զ�̷���

	CHAR_ATT_ATTACK_MAGIC_NEAR,	// �����ڹ�����
	CHAR_ATT_DEFENCE_MAGIC_NEAR,// �����ڹ�����

	CHAR_ATT_ATTACK_MAGIC_FAR,	// �����⹦����
	CHAR_ATT_DEFENCE_MAGIC_FAR,	// �����⹦����

	CHAR_ATT_ATTACK_GOLD,		// �𹥻�
	CHAR_ATT_DEFENCE_GOLD,		// �����

	CHAR_ATT_ATTACK_WOOD,		// ľ����
	CHAR_ATT_DEFENCE_WOOD,		// ľ����

	CHAR_ATT_ATTACK_WATER,		// ˮ����
	CHAR_ATT_DEFENCE_WATER,		// ˮ����

	CHAR_ATT_ATTACK_FIRE,		// �𹥻�
	CHAR_ATT_DEFENCE_FIRE,		// �����

	CHAR_ATT_ATTACK_SOIL,		// ������
	CHAR_ATT_DEFENCE_SOIL,		// ������

	CHAR_ATT_ANGER,				// ŭ��ֵ
	CHAR_ATT_SKILL_POINT,		// ������

	CHAR_ATT_NUMBERS
};
//���������Ϣ
enum TEAM_ERROR
{
	TEAM_ERROR_INVITEDESTHASTEAM=0 ,	//��������Ѿ�����ĳ������
	TEAM_ERROR_INVITEREFUSE ,			//�������˾ܾ�����
	TEAM_ERROR_INVITETEAMFULL ,			//�����˵Ķ��������Ѿ�����
	TEAM_ERROR_INVITELEADERREFUSE ,		//�ӳ��ܾ��³�Ա���������
	TEAM_ERROR_DISMISSNOTLEADER ,		//��ɢ������˲��Ƕӳ�
	TEAM_ERROR_KICKNOTLEADER ,			//�����߲��Ƕӳ�
	TEAM_ERROR_APPLYSOURHASTEAM ,		//�������Ѿ�����ĳ������
	TEAM_ERROR_APPLYDESTHASNOTTEAM ,	//�������߲�����ĳ����
	TEAM_ERROR_APPLYLEADERREFUSE ,		//�ӳ���ͬ�������˼������
	TEAM_ERROR_APPLYTEAMFULL ,			//�������˵Ķ��������Ѿ�����
	TEAM_ERROR_APPLYLEADERGUIDERROR ,	//�����������ڶ���Ķӳ�GUID�����仯
	TEAM_ERROR_APPOINTSOURNOTEAM ,		//�ɶӳ����Ƕ����Ա
	TEAM_ERROR_APPOINTDESTNOTEAM ,		//�¶ӳ����Ƕ����Ա
	TEAM_ERROR_APPOINTNOTSAMETEAM ,		//����ʱ�����˲�����ͬһ������
	TEAM_ERROR_APPOINTSOURNOLEADER ,	//�ɶӳ����Ƕ���Ķӳ���
	TEAM_ERROR_APPLYLEADERCANTANSWER,	//�ӳ�Ŀǰ�޷���
	TEAM_ERROR_INVITERNOTINTEAM,		//�����˲��ڶӳ��Ķ�����
	TEAM_ERROR_APPLYWHENINTEAM,			//���������ж����������������
	TEAM_ERROR_TEAMFULL,				//��������������
	TEAM_ERROR_REFUSEINVITESETTING,		//�������������˾ܾ�����
	TEAM_ERROR_TARGETNOTONLINE,			//�Է��Ѿ����ߣ�����ʧ�ܡ�
	TEAM_ERROR_LEAVETEAM,				//�Է��Ѿ��뿪�����ˣ��������������
	TEAM_ERROR_DISTRIBNOLEADER,			//��leader���÷��䷽ʽ
	TEAM_ERROR_INVITEEOFFLINE,			//�������˴�������״̬
	
	TEAM_ERROR_NUMBER ,	//���鹦�ܴ���ŵ����ֵ
};
//������Ϣ
enum TEAM_RESULT
{
	TEAM_RESULT_MEMBERENTERTEAM=0 ,		//��Ա�������
	TEAM_RESULT_MEMBERLEAVETEAM ,		//��ͨ��Ա�뿪����
	TEAM_RESULT_LEADERLEAVETEAM ,		//�ӳ��뿪����
	TEAM_RESULT_TEAMDISMISS ,			//�����ɢ
	TEAM_RESULT_TEAMKICK ,				//�߳���Ա
	TEAM_RESULT_TEAMAPPOINT ,			//�����¶ӳ�
	TEAM_RESULT_TEAMREFRESH,			// �������������Ϣ�Ļظ�
	TEAM_RESULT_STARTCHANGESCENE,		// ��ʼ�л�����
	TEAM_RESULT_ENTERSCENE,				// ���ѽ����³���
	TEAM_RESULT_REFRESHSERVERINFO,		// �����ת�����󣬸�������ˢ�¶�����Ϣ
	TEAM_RESULT_MEMBEROFFLINE,			// �������

	TEAM_RESULT_NUMBER , //���������͵����ֵ
};
//������Ϣ֪ͨ
enum NOTIFY_TEAMINFO
{
	NOTIFY_USERCHANGESCENE=0 ,	//��Աת�Ƴ���
	NOTIFY_USERENTERSCENE,		//��Ա���볡��

};

//ϵͳ����ʱָ������ʾλ��
enum ENUM_SYSTEM_CHAR_SHOW_POS
{
	CHAT_MAIN_WINDOW = 0,	//��ʾ������������촰����
	CHAT_LEFTDOWN,			//���½Ǹ��˵�ϵͳ��ʾ��
	CHAT_RIGHTDOWN,			//���½�ϵͳ��ϵͳ��ʾ��
	CHAT_PLUMB_SCROLL,		//��ֱ������ʾ��
	CHAT_PLANE_SCROLL,		//ˮƽ������ʾ��
	CHAR_SHOW_POS_NUM

};
//��������
enum ENUM_CHAT_TYPE
{
	CHAT_TYPE_INVALID = -1,

	CHAT_TYPE_NEAR =0 ,	//��ͨ˵����Ϣ
	CHAT_TYPE_TEAM ,		//������Ϣ
	CHAT_TYPE_WORLD,		//����
	CHAT_TYPE_USER ,		//˽����Ϣ
	CHAT_TYPE_SYSTEM ,		//ϵͳ��Ϣ
	CHAT_TYPE_JIAZU ,		//������Ϣ
	CHAT_TYPE_GUILD ,		//������Ϣ
	CHAT_TYPE_COUNTRY,//////////����
//����8���� �߻�Ҫ���

	CHAT_TYPE_MENPAI,//����
	
	CHAT_TYPE_CHANNEL ,		//�Խ�����Ƶ����Ϣ
	CHAT_TYPE_SCENE ,		//������Ϣ
	CHAT_TYPE_SELF,			//���ͻ���ʹ�õ���Ϣ

	CHAT_TYPE_DMA_FORCE,		//˫��ǿ��
	CHAT_TYPE_DMA_REQUEST,		//˫������
	CHAT_TYPE_DMA_AGREE,		//˫��ͬ��
	CHAT_TYPE_DMA_REFUSE,		//˫�˾ܾ�
	CHAT_TYPE_DMA_CANCEL,		//˫��ֹͣ
 
 
	CHAT_TYPE_NUMBER,			//�������͵����ֵ
};

//��������by czg 2009-12-9
#define CHAT_TYPE_NORMAL CHAT_TYPE_NEAR
#define CHAT_TYPE_TELL CHAT_TYPE_USER
//�������� 
enum CHAT_NEED_TYPE
{
	CHAT_NEED_NONE		= -1,					
	CHAT_NEED_MP		= 0 ,				//��Ҫ����MP
	CHAT_NEED_ENERGY	= 1 ,				//��Ҫ����
	CHAT_NEED_VIGOR		= 2 ,				//��Ҫ����

	CHAT_NEED_NUMBER,					//�������͵����ֵ
};
//�������
enum CHANNEL_ERROR
{
	CHANNEL_ERROR_HASCHANNEL = 0 ,		//���û��Ѿ�������һ������Ƶ����
	CHANNEL_ERROR_CHANNELFULL ,			//����Ƶ������ʧ��
	CHANNEL_ERROR_NOCHANNEL ,			//û��Ƶ����Ϣ
	CHANNEL_ERROR_CHANNELMEMBERFULL ,	//����Ƶ������������
	CHANNEL_ERROR_MEMBEREXIST ,			//�û��Ѿ���������Ƶ������
	CHANNEL_ERROR_NOTINCHANNEL ,		//�û����ڴ�����Ƶ����


	CHANNEL_ERROR_NUMBER , //�Խ�����Ƶ������ŵ����ֵ
};
//������ʧ
enum CHANNEL_RESULT
{
	CHANNEL_RESULT_CREATE = 0 ,			//��������Ƶ��
	CHANNEL_RESULT_ADDMEMBER ,			//����Ƶ����������³�Ա
	CHANNEL_RESULT_DISMISS ,			//�Խ�����Ƶ����ɢ
	CHANNEL_RESULT_KICKMEMBER ,			//�߳���Ա


	CHANNEL_RESULT_NUMBER , //�Խ�����Ƶ��������͵����ֵ
};

// ��Ա��Ϣ
enum ENUM_TEAM_MEMBER_ATT
{
	TEAM_MEMBER_ATT_INVALID	= -1,

	TEAM_MEMBER_ATT_FAMILY,			// ����
	TEAM_MEMBER_ATT_LEVEL,			// �ȼ�
	TEAM_MEMBER_ATT_POSITION,		// λ��

	TEAM_MEMBER_ATT_HP,				// HP
	TEAM_MEMBER_ATT_MAX_HP,			// HP����
	TEAM_MEMBER_ATT_MP,				// MP
	TEAM_MEMBER_ATT_MAX_MP,			// MP����
	TEAM_MEMBER_ATT_ANGER,			// ŭ��ֵ

	TEAM_MEMBER_ATT_WEAPON,			// ����
	TEAM_MEMBER_ASSIHAND,			// ������
	TEAM_MEMBER_ATT_CAP,			// ñ��
	TEAM_MEMBER_ATT_SCAPULAR,		// ���
	TEAM_MEMBER_ATT_ARMOR,			// �·�
	TEAM_MEMBER_ATT_CUFF,			// ����
	TEAM_MEMBER_ATT_BOOT,			// ѥ��

	TEAM_MEMBER_ATT_BUFF,			// ���� buff

	TEAM_MEMBER_ATT_DEADLINK,		// ����
	TEAM_MEMBER_ATT_DEAD,			// ����
	TEAM_MEMBER_ATT_FACEMESH,		// �沿ģ��
	TEAM_MEMBER_ATT_HAIRMESH,		// ͷ��ģ��
	TEAM_MEMBER_ATT_HAIRCOLOR,		// ͷ����ɫ

	TEAM_MEMBER_ATT_NUMBERS
};

//�����ڴ�����
enum	SHAREMEM_TYPE
{
	ST_INVAILD		=  -1,
	ST_HUMAN_SMU	=	1,
	ST_GUILD_SMU	=	2,
	ST_MAIL_SMU		=	3,
	ST_PSHOP_SMU	=	4,
	ST_USERDATA_SMU	=	5,
	ST_HUMANEXT_SMU	=	6,
	ST_ITEMSERIAL_SMU = 7,
};


//�����ڴ�ʹ��״̬
enum   SHAREMEM_USE_STATUS
{
	SUS_NEVER_USE		= 0,				//û�б�ʹ�ù�
    SUS_SERVER_USED				= 1,		//Server ռ��
	SUS_SERVER_HAVE_CLEANED		= 2,		//Server ����ShutDown ���Ѿ����
	SUS_WORLD_USED				= 3,		//World ռ��
	SUS_WORLD_HAVE_CLEANED		= 4,		//World ����ShutDown ���Ѿ����
};

enum UDRETURN
{
	UDR_NOTFINDUSERDATA = 0 ,	//û�з����û�����
	UDR_USERDATA ,				//ӵ���û���Ϣ
	UDR_USERDATALIVING ,		//�û���Ϣ����Ŀ�ķ������Ļ�����
	UDR_USERSERVERCRASH,		//�û�Server�Ѿ�Crash,���ܻ��ڻ�����,Server���ڴ�����黺��
	UDR_KEYERROR ,				//��֤�����
};

enum ENUM_COMBAT_EVENT //���ܺ�Ч�����ĵ��¼�
{
	EVENT_ON_DAMAGE = 0,
	EVENT_ON_HIT_BY_SKILL,
	EVENT_ON_HIT_TARGET,
	EVENT_ON_BE_CRITICAL_HIT,
	EVENT_ON_CRITICAL_HIT_TARGET,
	EVENT_ON_HEAL_TARGET,
	EVENT_ON_BE_HEAL,
	EVENT_ON_DEAD,
	NUMBER_OF_EVENTS
};

// ɱ��ֵ
enum ENUM_PK_VALUE_RANGE
{
	PK_VALUE_RANGE_1	= 1,
	PK_VALUE_RANGE_20	= 20,
	PK_VALUE_RANGE_40	= 40,
	PK_VALUE_RANGE_60	= 60,
	PK_VALUE_RANGE_80	= 80,
	PK_VALUE_RANGE_MAX	= 1000,
};
//���������Ϣ
enum TEAM_FOLLOW_RESULT
{
	TF_RESULT_REFUSE_FOLLOW = 0,	// ��Ա�ܾ�����ӳ�
	TF_RESULT_ENTER_FOLLOW,			// ��Ա������Ӹ���״̬
	TF_RESULT_STOP_FOLLOW,			// ��Ա�˳���Ӹ���״̬
	TF_RESULT_FOLLOW_FLAG,			// ֪ͨ�ͻ��˽�����Ӹ���״̬�ı�ǣ�ENTER_FOLLOW �� quiet ģʽ��
};
//������������Ϣ
enum TEAM_FOLLOW_ERROR
{
	TF_ERROR_TOO_FAR = 0,			// ��ӳ�̫Զ�ˣ������ܸ��棩
	TF_ERROR_IN_TEAM_FOLLOW,		// �Ѿ�������Ӹ���״̬����������ĳЩ������
	TF_ERROR_STALL_OPEN,			// ���ڰ�̯
	TF_ERROR_NOT_IN_FOLLOW_MODE,	// ����Ŀǰ�����ڸ���״̬
};

enum SERVER_TYPE			//�������˳�������
{
	SERVER_GAME = 0 ,		//��Ϸ�������˳���
	SERVER_LOGIN = 1 ,		//��½�������˳���
	SERVER_SHAREMEN = 2 ,	//����洢�˳���
	SERVER_WORLD = 3,
	SERVER_BILLING = 4,		//���������?
	SERVER_NUMBER ,
};
//��ϵ��
enum RELATION_GROUP
{
	RELATION_GROUP_FRIEND_ALL		= 0,		//ȫ������
	RELATION_GROUP_F1,							//����1��
	RELATION_GROUP_F2,							//����2��
	RELATION_GROUP_F3,							//����3��
	RELATION_GROUP_F4,							//����4��
	RELATION_GROUP_F5,							//����5��
	RELATION_GROUP_BLACK,						//��������
	RELATION_GROUP_ENEMY,						//������
	
	RELATION_GROUP_NUMBER ,						//�洢������

	RELATION_GROUP_TEMPFRIEND,					//��ʱ����
};

enum RELATION_TYPE //��ϵ�˹�ϵ
{
	RELATION_TYPE_NONE				= 0,		//��
	RELATION_TYPE_FRIEND,						//����
	RELATION_TYPE_BROTHER,						//���
	RELATION_TYPE_MARRY,						//���
	RELATION_TYPE_BLACKNAME,					//������
	RELATION_TYPE_ENEMY,						//����
	RELATION_TYPE_TEMPFRIEND,					//��ʱ����
	RELATION_TYPE_STRANGER,						//İ���˹�ϵ
	RELATION_TYPE_MASTER,						//ʦ����ϵ
	RELATION_TYPE_PRENTICE,						//ͽ�ܹ�ϵ
	RELATION_TYPE_SIZE,							//��ϵ����
};



#define RELATION_FRIEND_OFFSET 0
#define RELATION_BLACKNAME_OFFSET 200
#define RELATION_ENEMY_OFFSET	  230


enum ENUM_SKILL_CLASS_BY_USER_TYPE//���ܵ�ʹ���߷���
{
	A_SKILL_FOR_PLAYER 	= 0,
	A_SKILL_FOR_MONSTER = 1,
	A_SKILL_FOR_PET 	= 2,
	A_SKILL_FOR_ITEM 	= 3,
};
enum CONSTANT_VALUE //һЩ����
{
	DEFAULT_ATTACK_FLUCTUATION = 8, //�������� +/- 8%
};



//GMָ������
enum COMMAND_TYPE
{
	COMMAND_TYPE_NONE = 0 ,				//��ָ��
	COMMAND_TYPE_TELEPORT ,				//��һ��OBJ�ƶ���ĳ��
	COMMAND_TYPE_MODIFYMENPAI ,			//�޸Ľ�ɫ��������
	COMMAND_TYPE_MODIFYXINFA ,			//�޸Ľ�ɫ�ķ��ȼ�
	COMMAND_TYPE_LEVELUPALLXINFA,		//�����ķ��ȼ��޸�
	COMMAND_TYPE_CREATEITEM ,			//������Ʒ
	COMMAND_TYPE_RELIVE ,				//�����Լ�
	COMMAND_TYPE_CHAT ,					//����Chat��Ϣ
	COMMAND_TYPE_ENABLEGOD ,			//�����޵�
	COMMAND_TYPE_DISABLEGOD ,			//��ֹ�޵�
	COMMAND_TYPE_IAMGOD ,				//����޵�
	COMMAND_TYPE_IAMDOG ,				//ȡ���޵�
	COMMAND_TYPE_DEBUG_RELOADCONFIG ,	//���¶�ȡConfig�����ļ�
	COMMAND_TYPE_SYSCHAT ,				//ϵͳ������Ϣ
	COMMAND_TYPE_DEBUG_SHUTDOWN ,		//�رշ�����
	COMMAND_TYPE_TEAM_INVITE ,			//����-����
	COMMAND_TYPE_TEAM_RETINVITE ,		//����-�����Ӧ
	COMMAND_TYPE_TEAM_LEADERRETINVITE , //����-�ӳ������Ӧ
	COMMAND_TYPE_TEAM_APPLY ,			//����-����
	COMMAND_TYPE_TEAM_RETAPPLY ,		//����-�����Ӧ
	COMMAND_TYPE_TEAM_LEAVE ,			//����-�뿪
	COMMAND_TYPE_TEAM_KICK ,			//����-����
	COMMAND_TYPE_TEAM_DISMISS ,			//����-��ɢ
	COMMAND_TYPE_TEAM_APPOINT ,			//����-����
	COMMAND_TYPE_ABILITY_DETAIL,		//�����������ϸ��Ϣ
	COMMAND_TYPE_USE_ABILITY,			//ʹ�������
	COMMAND_TYPE_COMBOUND_GEM,			//�ϳɱ�ʯ
	COMMAND_TYPE_USE_GEM,				//��ʯ��Ƕ
	COMMAND_TYPE_CHANNEL_CREATE,		//�Խ�����Ƶ��-����
	COMMAND_TYPE_CHANNEL_INVITE,		//�Խ�����Ƶ��-����
	COMMAND_TYPE_CHANNEL_DISMISS,		//�Խ�����Ƶ��-��ɢ
	COMMAND_TYPE_CHANNEL_KICK,			//�Խ�����Ƶ��-�߳���Ա
	COMMAND_TYPE_GOTO ,					//������ұ���Ŀ��λ�ã����³�����
	COMMAND_TYPE_FULLRECOVER ,			//�ظ���Ѫ��ħ
	COMMAND_TYPE_CREATEPET ,			//��������
	COMMAND_TYPE_DELETEPET ,			//ɾ������
	COMMAND_TYPE_CAPTUREPET ,			//��׽����
	COMMAND_TYPE_CALLUPPET ,			//�ٻ�����
	COMMAND_TYPE_RECALLPET ,			//�ջس���
	COMMAND_TYPE_FREEPETTONATURE ,		//��������
	COMMAND_TYPE_RETURNTOCHILD ,		//���ﻹͯ
	COMMAND_TYPE_SKILL_RELIVE,			//���ܸ���
	COMMAND_TYPE_CREATESCENE ,			//����һ����������
	COMMAND_TYPE_CLOSESCENE ,			//ǿ�ƹر�һ����������
	COMMAND_TYPE_SET_SCENE_TYPE,		//���ó�������
	COMMAND_TYPE_SET_PK_VALUE,			//����ɱ��
	COMMAND_TYPE_SET_MODEL_ID,			//����ģ��ID
	COMMAND_TYPE_SET_EQUIP_DUR,			//����װ���;ö�
	COMMAND_TYPE_SETDAMAGE,				//�����˺�
	COMMAND_TYPE_KILLOBJ,				//ɱ��һ��OBJ
	COMMAND_TYPE_CREATEMONSTER,			//����һֻ����
	COMMAND_TYPE_DELETEMONSTER,			//ɾ��һֻ���ɵĹ���
	COMMAND_TYPE_SETAITYPE,				//�޸�AI����
	COMMAND_TYPE_MODIFYMONEY,			//�޸Ľ�Ǯ
	COMMAND_TYPE_MAKETEAMFOLLOW,		//ģ��������
	COMMAND_TYPE_SENDMAIL,				//�����ʼ�
	COMMAND_TYPE_RECVMAIL,				//�����ʼ�
	COMMAND_TYPE_CREATEGUILD,			// �������
	COMMAND_TYPE_JOINGUILD,				// ������
	COMMAND_TYPE_GUILDRECRUIT,			// �������
	COMMAND_TYPE_GUILDEXPEL,			// �������
	COMMAND_TYPE_LEAVEGUILD,			// �뿪���
	COMMAND_TYPE_GUILDDISMISS,			// ��ɢ���
	COMMAND_TYPE_ASKGUILDLIST,			// ����б�
	COMMAND_TYPE_SETMOVEMODE,			// �����ƶ�ģʽ ��/��/����
	COMMAND_TYPE_ASKRELATIONLIST,		// ��ϵ�б�
	COMMAND_TYPE_ADDFRIEND,				// ����һ������
	COMMAND_TYPE_SETFRIENDPOINT,		// �����Ѻö�
	COMMAND_TYPE_ADDBLACKNAME,			// ���������
	COMMAND_TYPE_DELFRIEND,				// ɾ��һ������
	COMMAND_TYPE_DELBLACK,				// �Ӻ�������ɾ��һ�����
	COMMAND_TYPE_TRANSITION,			// ���ת��
	COMMAND_TYPE_SETCAMPID,				// ������Ӫ
	COMMAND_TYPE_SAVE,					// �����û�����
	COMMAND_TYPE_GETOBJINFO,			// ȡ��ĳ��obj�ķ�����������
	COMMAND_TYPE_LEVELUP,				// ����
	COMMAND_TYPE_ABILITYUP,				// ���������
	COMMAND_TYPE_WHO,					// ��ǰ����
	COMMAND_TYPE_ALLWHO,				// ���г���
	COMMAND_TYPE_CATCHPLAYER,			// ȡ��������ĳ����ҵĻ�����Ϣ
	COMMAND_TYPE_PLAYERINFO,			// ȡ�ó�����ĳ����ҵ���ϸ��Ϣ
	COMMAND_TYPE_SETPETHAPPINESS,		// ���ó���Ŀ��ֶ�
	COMMAND_TYPE_SETPETHP,				// ���ó���HP
	COMMAND_TYPE_SETPETLIFE,			// ���ó��������
	COMMAND_TYPE_PETLEVELUP,			// ������ǰ����
	COMMAND_TYPE_SETPETSKILL,			// ���ó���ļ���
	COMMAND_TYPE_USEPETSKILL,			// ʹ�ó���ļ���
	COMMAND_TYPE_LEVELDOWN,				// ������ֻ���ͼ�����ֵ������������ص�����
	COMMAND_TYPE_ADDEXP,				// ���Ӿ���
	COMMAND_TYPE_CREATECITY,			// ��������
	COMMAND_TYPE_DELETECITY,			// �ݻٳ���
	COMMAND_TYPE_LOADMONSTER,			// �񳡾�����һ������
	COMMAND_TYPE_SETHAIRCOLOR,			// �������ͷ����ɫ
	COMMAND_TYPE_SETHAIRMODEL,			// �������ͷ��ģ��
	COMMAND_TYPE_CREATEGUARD,			// ��������
	COMMAND_TYPE_CREATETRAP,			// ��������
	COMMAND_TYPE_USEITEM,				// ʹ����Ʒ
	COMMAND_TYPE_FORGETRECIPE,			// �����䷽
	COMMAND_TYPE_SEND_IMPACT_TO_UNIT,	// ��ĳ����ɫһ��Ч��
	COMMAND_TYPE_RELOAD_COMBAT_TABLES,	// ����װ��ս�����ݱ�
	COMMAND_TYPE_USE_SKILL,				// ʩ��ħ��
	COMMAND_TYPE_SAVEPACKETINFO,		// ������Ϣ������Ϣ
	COMMAND_TYPE_SETVIGOR,				// ���û���ֵ
	COMMAND_TYPE_SETENERGY,				// ���þ���ֵ
	COMMAND_TYPE_PETPULLULATE,			// ����ɳ���
	COMMAND_TYPE_ADD_SKILL,				// ��Ӽ���
	COMMAND_TYPE_REMOVE_SKILL,			// ɾ������
	COMMAND_TYPE_FINGERGUID,			// �� GUID �������Ϣ
	COMMAND_TYPE_FINGERNAME,			// �������������Ϣ
	COMMAND_TYPE_ADVANCEDFINGER,		// �߼�����
	COMMAND_TYPE_SETABILITYEXP,			// ���������������
	COMMAND_TYPE_SETGOODBAD,			// �����ƶ�ֵ
	COMMAND_TYPE_SETCURDUR,				// ���õ�ǰ�;�
	COMMAND_TYPE_NUMBER , //ָ������
};

// ���＼���б�
enum ENUM_PET_SKILL_INDEX
{
	PET_SKILL_INDEX_INVALID = -1,
	PET_SKILL_INDEX_CONTROL_BY_PLAYER,
	PET_SKILL_INDEX_CONTROL_BY_AI0,
	PET_SKILL_INDEX_CONTROL_BY_AI1,
	PET_SKILL_INDEX_CONTROL_BY_AI2,
	PET_SKILL_INDEX_CONTROL_BY_AI3,
	PET_SKILL_INDEX_MENPAI,

	PET_SKILL_INDEX_NUMBERS
};

// ���＼�ܲ���ģʽ
enum PET_SKILL_OPERATEMODE
{
	PET_SKILL_OPERATE_INVALID			= -1, // INVALID
	PET_SKILL_OPERATE_NEEDOWNER			= 0,  // ��Ҫ���˴���
	PET_SKILL_OPERATE_AISTRATEGY		= 1,  // ��AI������	
	PET_SKILL_OPERATE_INCEACEATTR		= 2,  // ��ǿ��������

	PET_SKILL_OPERATE_NUMBERS,
	
};

// �ʼ�����
enum MAIL_TYPE
{
	MAIL_TYPE_NORMAL = 0 ,//��ͨ�ʼ�
	MAIL_TYPE_SYSTEM = 1 ,//ϵͳ�ʼ�
	MAIL_TYPE_SCRIPT = 2 ,//�ű��ʼ����������˽��յ����ʼ�������һ���ű�
	MAIL_TYPE_NORMAL2 = 3 , // ϵͳ����Ҵ���
};

// ��ִ���ʼ�����
//!!!!�˶��������ű�ScriptGlobal.lua�еĶ�����ȫ��ͬ��
//!!!!�����ڲ��˽�������ִ���ʼ����̵������������
#define MAIL_REPUDIATE			1			//ǿ�����
#define MAIL_BETRAYMASTER		2			//��ʦ
#define MAIL_EXPELPRENTICE		3			//����ͽ��
#define MAIL_UPDATE_ATTR		4			//���²������������(ͨ����ִ���ʼ�ϵͳ�Զ����������ʱ����)
#define MAIL_DELFRIEND			5			//ɾ������

enum
{
	MAIL_ATTR_GUILD = 1,					//����ID
	MAIL_ATTR_MONEY,						//���ϵĽ�Ǯ
};

//�����ʼ�����������
enum ASK_TYPE
{
	ASK_TYPE_LOGIN = 0 ,//�û��յ�½��Ϸʱ���͵��ʼ������Ϣ,
						//������ʼ�������������֪ͨ��Ϣ
	ASK_TYPE_MAIL ,	//�û�������������Ҫ�ʼ�����Ϣ
					//������ʼ������ʼ�����
};

// ��Ϸ���ñ�־
// �� 16 λ����Ҫ���� World �ģ��� 16 λ��ֻ��Ҫ Server �����
enum GAME_SETTING_FLAG
{
	GSF_CANNOT_ADD_FRIEND			= 0,		//�ܾ���Ӻ���
	GSF_CANNOT_RECV_MAIL			= 1,		//�ܾ������ʼ�
	GSF_CANNOT_RECV_STRANGER_MAIL	= 2,		//�ܾ�����İ�����ʼ�
	GSF_REFUSE_TEAM_INVITE			= 3,		// �ܾ��������
	GSF_REFUSE_LOOK_SPOUSE_INFO		= 16,		// �ܾ��鿴��ż����
	GSF_REFUSE_TRADE				= 17,		// �ܾ�����
	GSF_CLOSE_POPUP					= 18,		// �رյ�ǰ���ݿ�
	GSE_REFUSE_FACTION_INVITE		= 19,		// �ܾ���������
	GSE_LOCKED_SHORTCUT				= 20,		// ���������
	GSE_SHOW_OTHERPLAYER_NAME		= 21,		// ��ʾ������ҵ�����
	GSE_SHOW_RESTURN_NAME			= 22,		// ��ʾ��Ʒ��������
	GSE_SHOW_NPC_NAME				= 23,		// ��ʾ��������
};
enum SETTING_TYPE
{
	SETTING_TYPE_NONE = 0 ,
	SETTING_TYPE_GAME ,		//��ϵ������, bit����� enum GAME_SETTING_FLAG
	SETTING_TYPE_K0 ,		//0�ſ��������
	SETTING_TYPE_K1 ,		//1�ſ��������
	SETTING_TYPE_K2 ,		//2�ſ��������
	SETTING_TYPE_K3 ,		//3�ſ��������
	SETTING_TYPE_K4 ,		//4�ſ��������
	SETTING_TYPE_K5 ,		//5�ſ��������
	SETTING_TYPE_K6 ,		//6�ſ��������
	SETTING_TYPE_K7 ,		//7�ſ��������
	SETTING_TYPE_K8 ,		//8�ſ��������
	SETTING_TYPE_K9 ,		//9�ſ��������
	SETTING_TYPE_K10 ,	//�ұ�0�ſ��������
	SETTING_TYPE_K11 ,	//�ұ�1�ſ��������
	SETTING_TYPE_K12 ,	//�ұ�2�ſ��������
	SETTING_TYPE_K13 ,	//�ұ�3�ſ��������
	SETTING_TYPE_K14 ,	//�ұ�4�ſ��������
	SETTING_TYPE_K15 ,	//�ұ�5�ſ��������
	SETTING_TYPE_K16 ,	//�ұ�6�ſ��������
	SETTING_TYPE_K17 ,	//�ұ�7�ſ��������
	SETTING_TYPE_K18 ,	//�ұ�8�ſ��������
	SETTING_TYPE_K19 ,	//�ұ�9�ſ��������
	SETTING_TYPE_K20 ,	//�ұ�0�ſ��������
	SETTING_TYPE_K21 ,	//�ұ�1�ſ��������
	SETTING_TYPE_K22 ,	//�ұ�2�ſ��������
	SETTING_TYPE_K23 ,	//�ұ�3�ſ��������
	SETTING_TYPE_K24 ,	//�ұ�4�ſ��������
	SETTING_TYPE_K25 ,	//�ұ�5�ſ��������
	SETTING_TYPE_K26 ,	//�ұ�6�ſ��������
	SETTING_TYPE_K27 ,	//�ұ�7�ſ��������
	SETTING_TYPE_K28 ,	//�ұ�8�ſ��������
	SETTING_TYPE_K29 ,	//�ұ�9�ſ��������

	SETTING_TYPE_CHAT_TAB1_PART1,	//�����Զ���tab1���õ�һ����
	SETTING_TYPE_CHAT_TAB1_PART2,	//�����Զ���tab1���õڶ�����
	SETTING_TYPE_CHAT_TAB2_PART1,	//�����Զ���tab2���õ�һ����
	SETTING_TYPE_CHAT_TAB2_PART2,	//�����Զ���tab2���õڶ�����
	SETTING_TYPE_CHAT_TAB3_PART1,	//�����Զ���tab3���õ�һ����
	SETTING_TYPE_CHAT_TAB3_PART2,	//�����Զ���tab3���õڶ�����
	SETTING_TYPE_CHAT_TAB4_PART1,	//�����Զ���tab4���õ�һ����
	SETTING_TYPE_CHAT_TAB4_PART2,	//�����Զ���tab4���õڶ�����

	SETTING_TYPE_ATTRI_AUTO_POINT,	//�����Զ����Է������,��INT�ֳ����ĸ�BYTE,�պñ���
	SETTING_TYPE_MAGICINT,			//MagicInt

	SETTING_TYPE_NUMBER ,//�û��������ݵ���������
};

// �Ա�
enum ENUM_SEX
{
	SEX_INVALID	= -1,
	SEX_FEMALE,			// ��
	SEX_MALE,			// ��

	SEX_NUMBERS
};

// �����������
enum ENUM_HUMAN_DIE_TYPE
{
	HUMAN_DIE_TYPE_INVALID = -1,
	HUMAN_DIE_TYPE_INTERCHANGE,		// �д�
	HUMAN_DIE_TYPE_MONSTER_KILL,	// ����ɱ��
	HUMAN_DIE_TYPE_PLAYER_KILL,		// �����ɱ��

	HUMAN_DIE_TYPE_NUMBERS
};

enum TASK_EVENT
{
	TASK_EVENT_KILLOBJ		= 0 ,	//ɱ������
	TASK_EVENT_ENTERAREA	= 1 ,	//�����¼���
	TASK_EVENT_ITEMCHANGED	= 2 ,	//��Ʒ�仯
	TASK_EVENT_PETCHANGED	= 3 ,	//����仯
};

enum	CMD_MODE
{
	CMD_MODE_CLEARALL	=	1,		//���ģʽ
	CMD_MODE_LOADDUMP	=	2,		//����dumpģʽ
};


enum	WORLD_TIME
{
	WT_IND_1	=	0,	//��ʱ
	WT_IND_2,			//��ʱ
	WT_IND_3,			//��ʱ
	WT_IND_4,			//îʱ
	WT_IND_5,			//��ʱ
	WT_IND_6,			//��ʱ
	WT_IND_7,			//��ʱ
	WT_IND_8,			//δʱ
	WT_IND_9,			//��ʱ
	WT_IND_10,			//��ʱ
	WT_IND_11,			//��ʱ
	WT_IND_12			//��ʱ
};

enum	NPC_SHOP_TYPE
{
	SHOP_All		= 1,	//
	SHOP_DEFENCE	= 2,	//����
	SHOP_ADORN		= 3,	//����
	SHOP_WEAPON		= 4,	//����

	SHOP_FOOD		= 5,	//ʳ��
	SHOP_MATERIAL	= 6,	//����
	SHOP_COMITEM	= 7,	//ҩƷ
};

typedef enum _SYSTEM_USE_SKILL_ID
{
	MELEE_ATTACK = 0,
	CAPTURE_PET = 1,
	CALL_UP_PET = 2,
	SYSTEM_RESERVED_3	= 3,
	SYSTEM_RESERVED_4	= 4,
	SYSTEM_RESERVED_5	= 5,
	SYSTEM_RESERVED_6	= 6,
	SYSTEM_RESERVED_7	= 7,
	SHADOW_GUARD 		= 8,
	XIAOYAO_TRAPS		= 9,
} SystemUseSkillID_T;

typedef enum _SYSTEM_USE_IMPACT_ID
{
	IMP_DAMAGES_OF_ATTACKS = 0,
	IMP_NOTYPE_DAMAGE,
} SystemUseImpactID_T;

namespace AcceptableDistanceError_NS //ս��������㣬�ɽ��ܵľ������
{
	enum
	{
		ADE_FOR_HUMAN = 2,
		ADE_FOR_NPC = 1,
	};
};

enum CURRENCY_UNIT	// ���ҵ�λ
{
	CU_MONEY,		// Ǯ
	CU_GOODBAD,		// �ƶ�ֵ
	CU_MORALPOINT,	// ʦ�µ�
};


enum DEFAULT_CHAR_DATA
{
	CHAR_ENTERSCENCE		=0, //��ʼ����ID
	CHAR_BORNX             ,   //����X����
	CHAR_BORNY             ,   //����Y����
	CHAR_CAMP			  ,   //��ʼ��Ӫ

	CHAR_INILEVEL          ,   //��ʼ�ȼ�	
	CHAR_INIEXP            ,   //��ʼ����
	CHAR_INIHP             ,   //��ʼ����
	CHAR_INIRAGE            ,   //��ʼŭ��

	CHAR_INIPNEUMA         ,   //��ʼԪ��
	CHAR_MAX_VIGOR			,  //��ʼ��������
	//CHAR_INISTR            ,   //��ʼ����
	//CHAR_INICON            ,   //��ʼ����

	//CHAR_INIINT            ,   //��ʼ����
	//CHAR_INIDEX            ,   //��ʼ����
	CHAR_INISKILL1         ,   //��ʼս������1	
	CHAR_INISKILL2         ,   //��ʼս������2	

	CHAR_INISKILL3         ,   //��ʼս������3	
	CHAR_INISKILL4         ,   //��ʼս������4	
	CHAR_INISKILL5         ,   //��ʼս������5	
	CHAR_INIABILITY1		,   //��ʼ�����1	

	CHAR_INIABILITY2		,   //��ʼ�����2	
	CHAR_INIABILITY3		,   //��ʼ�����3	
	CHAR_INIABILITY4		,   //��ʼ�����4	
	CHAR_INIABILITY5		,   //��ʼ�����5	

	CHAR_INICOMMON_SKILL1	,   //��ʼ�����1
	CHAR_INICOMMON_SKILL2	,   //��ʼ�����2
	CHAR_INICOMMON_SKILL3	,   //��ʼ�����3
	CHAR_INICOMMON_SKILL4	,   //��ʼ�����4
	CHAR_INICOMMON_SKILL5	,   //��ʼ�����5

	CHAR_INIMONEY          ,   //��ʼ��Ǯ	

	CHAR_ININUM			   ,
};

#define MAX_MONSTER_PET_TAKE	3		//������Я���ٻ��޵�����
#define MONSTER_PET_MAX_SKILL	6		//�ٻ��������ü�������
#define MONSTER_PET_MAX_IMPACT	8		//�ٻ����ڿ糡��ʱ�ɼ�¼���Ѿ��е�buff������

#define MAX_MONSTER_HORSE_TAKE	2	//����������ٻ����������

//ͬ��Ƶ�����ñ�
#define IPREGION_NAME_LENGTH		12
#define IPREGION_UNKNOWN			-1		//�Ƿ�Ƶ�����
#define IPREGION_SPARK				1		//����Ƶ�����

// add by gh for souxia 2010/05/11
enum SOUXIA_TYPE
{
	ITEM_SOUXIALU = 0, // ����¼����
	ITEM_MEMORYCLUB	,  // ���������
	ITEM_DISMANTLE	,  // ������
	ITEM_REFINING	,  // ��������
	ITEM_SEW		,  // ��ϵ���
};


enum SOUXIA_PAGE_TYPE
{
	ITEM_SOUXIASKILL = 1, // ��������
	ITEM_PRODUCT		, // �����䷽   
	ITEM_PET_ZHAOHUAN	, // �����ٻ�
	ITEM_ZUOJI_ZHAOHUAN	, // �����ٻ�
};
#endif
