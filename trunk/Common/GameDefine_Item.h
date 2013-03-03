
#ifndef __GAMEDEFINE_ITEM_H__
#define __GAMEDEFINE_ITEM_H__
//����װ��
enum HUMAN_EQUIP
{
	HEQUIP_MAINHAND		=0,     //����
	HEQUIP_ASSIHAND		=1,     //����
	HEQUIP_HEAD			=2,		//ͷ
	HEQUIP_NECK			=3,		//��
	HEQUIP_SHOULDER		=4,		//��
	HEQUIP_BACK			=5,		//����
	HEQUIP_WRIST		=6,		//��
	HEQUIP_HAND			=7,		//��
	HEQUIP_WAIST		=8,		//��
	HEQUIP_FEET			=9,		//��
	HEQUIP_RING1		=10,	//��ָ1
	HEQUIP_RING2		=11,	//��ָ2
	HEQUIP_ADORN1		=12,	//��Ʒ1
	HEQUIP_ADORN2		=13,	//��Ʒ2
	HEQUIP_SUIT			=14,	//��װ
	HEQUIP_RIDER		=15,	//����
	HEQUIP_SOUXIA		=16,	//����
	HEQUIP_NUMBER,				//����װ��������
};
//����װ��
enum HORSE_EQUIP
{
	HEQUIP_HORSE_FACE	=0,	//����
	HEQUIP_HORSE_BODY	,	//����
	HEQUIP_HORSE_NECK	,	//��Ȧ
	HEQUIP_HORSE_BACK	,	//��
	HEQUIP_HORSE_HALTER	,	//����
	HEQUIP_HORSE_BOOT	,	//����
	HEQUIP_HORSE_NUMBER ,
};
//��Ʒ����
enum ITEM_ATTRIBUTE
{
	IATTRIBUTE_POINT_STR			,	//��������								0
	IATTRIBUTE_RATE_STR				,	//�����ٷֱ�							1		
	IATTRIBUTE_POINT_DEX			,	//������								2
	IATTRIBUTE_RATE_DEX 			,	//���ٷֱ�							3
	IATTRIBUTE_POINT_INT			,	//��������								4	

	IATTRIBUTE_RATE_INT 			,	//�����ٷֱ�							5
	IATTRIBUTE_POINT_CON			,	//���ʵ���								6	
	IATTRIBUTE_RATE_CON				,	//���ʰٷֱ�							7
	IATTRIBUTE_POINT_ALL_BASEATT	,	//�������Ե���							8
	IATTRIBUTE_RATE_ALL_BASEATT		,	//�������԰ٷֱ�						9

	IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR,	//�����ڹ�����							10
	IATTRIBUTE_POINT_ATTACK_MAGIC_FAR,	//�����⹦����							12
	IATTRIBUTE_RATE_ATTACK_MAGIC_NEAR,	//�����ڹ��ٷֱ�						11
	IATTRIBUTE_RATE_ATTACK_MAGIC_FAR,	//�����⹦�ٷֱ�						13

	IATTRIBUTE_POINT_ATTACK_NEAR	,	//Զ���ڹ�����		��->��				16
	IATTRIBUTE_RATE_ATTACK_NEAR		,	//Զ���ڹ��ٷֱ�	��->��				17
	IATTRIBUTE_POINT_ATTACK_FAR		,	//Զ���⹦����							14
	IATTRIBUTE_RATE_ATTACK_FAR		,	//Զ���⹦�ٷֱ�						15

	IATTRIBUTE_POINT_DEFENCE_MAGIC_NEAR,//�����ڹ���������						18
	IATTRIBUTE_POINT_DEFENCE_MAGIC_FAR,	//�����⹦��������						20
	IATTRIBUTE_RATE_DEFENCE_MAGIC_NEAR,	//�����ڹ������ٷֱ�					19
	IATTRIBUTE_RATE_DEFENCE_MAGIC_FAR,	//�����⹦�����ٷֱ�					21
	
	IATTRIBUTE_POINT_DEFENCE_NEAR	,	//Զ���ڹ�������
	IATTRIBUTE_RATE_DEFENCE_NEAR	,	//Զ���ٷֱ�
	IATTRIBUTE_POINT_DEFENCE_FAR	,	//Զ���⹦��������						22
	IATTRIBUTE_RATE_DEFENCE_FAR		,	//Զ���⹦�����ٷֱ�					23

	IATTRIBUTE_POINT_MAXHP			,	//����ֵ���޵���						24
	IATTRIBUTE_RATE_MAXHP			,	//����ֵ���ްٷֱ�						25

	IATTRIBUTE_POINT_MAXMP			,	//����ֵ���޵���						26
	IATTRIBUTE_RATE_MAXMP			,	//����ֵ���ްٷֱ�						27

	IATTRIBUTE_POINT_MAX_PNEUMA		,	//Ԫ��ֵ���޵���						28
	IATTRIBUTE_RATE_MAX_PNEUMA		,	//Ԫ��ֵ���ްٷֱ�						29
	
	IATTRIBUTE_MAXRAGE				,	//ŭ������								31
	IATTRIBUTE_RATE_MAXRAGE			,	//ŭ�����ްٷֱ�						32

	IATTRIBUTE_MAX_VIGOR			,	//�������޵���							33
	IATTRIBUTE_RATE_MAX_VIGOR		,	//�������ްٷֱ�						34
	IATTRIBUTE_VIGOR_REGENERATE		,	//�����ظ�								35
	
	IATTRIBUTE_HIT					,	//����									36
	IATTRIBUTE_MISS					,	//����									37
	IATTRIBUTE_RATE_HIT				,	//������								38
	IATTRIBUTE_CRIT					,	//����									39
	IATTRIBUTE_TOUGHNESS			,	//����									40
	IATTRIBUTE_RATE_CRIT			,	//������								41

	IATTRIBUTE_GOLD_ATTACK			,	//�𹥻�����							42
	IATTRIBUTE_RATE_GOLD_ATTACK		,	//�𹥻��ٷֱ�							43

	IATTRIBUTE_WOOD_ATTACK			,	//ľ��������							44
	IATTRIBUTE_RATE_WOOD_ATTACK		,	//ľ�����ٷֱ�							45

	IATTRIBUTE_WATER_ATTACK			,	//ˮ��������							46
	IATTRIBUTE_RATE_WATER_ATTACK	,	//ˮ�����ٷֱ�							47

	IATTRIBUTE_FIRE_ATTACK			,	//�𹥻�����							48
	IATTRIBUTE_RATE_FIRE_ATTACK		,	//�𹥻��ٷֱ�							49

	IATTRIBUTE_SOIL_ATTACK			,	//����������							50
	IATTRIBUTE_RATE_SOIL_ATTACK		,	//�������ٷֱ�							51
	
	IATTRIBUTE_GOLD_RESIST			,	//���������							52
	IATTRIBUTE_RATE_GOLD_RESIST  	,	//������ٷֱ�							53

	IATTRIBUTE_WOOD_RESIST			,	//ľ��������							54
	IATTRIBUTE_RATE_WOOD_RESIST		,	//ľ�����ٷֱ�							55

	IATTRIBUTE_WATER_RESIST			,	//ˮ��������							56
	IATTRIBUTE_RATE_WATER_RESIST	,	//ˮ�����ٷֱ�							57

	IATTRIBUTE_FIRE_RESIST			,	//���������							58
	IATTRIBUTE_RATE_FIRE_RESIST		,	//������ٷֱ�							59

	IATTRIBUTE_SOIL_RESIST			,	//����������							60
	IATTRIBUTE_RATE_SOIL_RESIST		,	//�������ٷֱ�							61

	IATTRIBUTE_RESIST_ALL			,	//�����������������Թ���				62
	IATTRIBUTE_RATE_RESIST_ALL		,	//���ٷֱȵ����������Թ���				63

	IATTRIBUTE_SPEED_MOVE			,	//�ƶ��ٶ�								64
	IATTRIBUTE_ATTACK_SPEED			,	//�����ٶ�								65
	IATTRIBUTE_COLOR_HAIR			,	//��ɫ									66
	IATTRIBUTE_MODEL_HAIR			,	//����									67
	IATTRIBUTE_HP_RESTORE			,	//�����ظ�								68
	IATTRIBUTE_MP_RESTORE			,	//�����ظ�								69
	IATTRIBUTE_PNEUMA_RESTORE		,	//Ԫ���ظ�								30
	IATTRIBUTE_CRIT_HURT			,	//���ӱ����˺�							70
	IATTRIBUTE_SHIELD_ALL			,	//���ж�								71
	IATTRIBUTE_SHIELD_NEAR			,	//Զ���ڹ���							75
	IATTRIBUTE_SHIELD_MAGIC_NEAR	,	//�����ڹ���							74
	IATTRIBUTE_SHIELD_MAGIC_FAR		,	//�����⹦��							72	
	IATTRIBUTE_SHIELD_FAR			,	//Զ���⹦��							73
	IATTRIBUTE_SHIELD_GOLD			,	//���									76
	IATTRIBUTE_SHIELD_WOOD			,	//ľ��									77
	IATTRIBUTE_SHIELD_WATER			,	//ˮ��									78
	IATTRIBUTE_SHIELD_FIRE			,	//���									79
	IATTRIBUTE_SHIELD_SOIL			,	//����									80
	IATTRIBUTE_ALLSKILL_LEVEL_INC	,	//�������м��ܵȼ�						81
	IATTRIBUTE_EXPRENCE_GET			,	//���Ӿ�����							99
	IATTRIBUTE_MONEY_GET			,	//���ӽ�Ǯ���							100

	IATTRIBUTE_BASE_RATE_ATTACK_NEAR	,   //����Զ���ڹ������ٷֱ�			104
	IATTRIBUTE_BASE_RATE_ATTACK_FAR		,   //����Զ���⹦�����ٷֱ�			102
	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_NEAR,   //���������ڹ������ٷֱ�			103
	IATTRIBUTE_BASE_RATE_ATTACK_MAGIC_FAR	,   //���������⹦�����ٷֱ�		101
	IATTRIBUTE_BASE_RATE_DEFENCE_ENEAR	,   //����Զ���ڹ������ٷֱ�			108
	IATTRIBUTE_BASE_RATE_DEFENCE_FAR	,   //����Զ���⹦�����ٷֱ�			106

	IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_NEAR	, //���������ڹ������ٷֱ�			107
	IATTRIBUTE_BASE_RATE_DEFENCE_MAGIC_FAR,		//���������⹦�����ٷֱ�		105

	IATTRIBUTE_BASE_ATTACK_NEAR		,	//����Զ���ڹ�����						112
	IATTRIBUTE_BASE_ATTACK_FAR		,	//����Զ���⹦����						110
	IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR,	//���������ڹ�����						111
	IATTRIBUTE_BASE_ATTACK_MAGIC_FAR	,//���������⹦����						109
	IATTRIBUTE_BASE_DEFENCE_NEAR	,	//����Զ���ڹ�����						116
	IATTRIBUTE_BASE_DEFENCE_FAR		,	//����Զ���⹦����						114

	IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR	,//���������ڹ�����						115
	IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR	,	//���������⹦����					113

	IATTRIBUTE_RATE_NEARATT_REDUCE		,	//���ٷֱȵ���Զ���ڹ�����			120
	IATTRIBUTE_RATE_FARATT_REDUCE		,	//���ٷֱȵ���Զ���⹦����			118
	IATTRIBUTE_RATE_MAGIC_NEAR_ATT_REDUCE,	//���ٷֱȵ��������ڹ�����			119
	IATTRIBUTE_RATE_MAGIC_FAR_ATT_REDUCE,	//���ٷֱȵ��������⹦����			117

	IATTRIBUTE_RATE_GOLD_ATT_REDUCE		,	//���ٷֱȵ����𹥻�				121
	IATTRIBUTE_RATE_WOOD_ATT_REDUCE		,	//���ٷֱȵ���ľ����				122

	IATTRIBUTE_RATE_WATER_REDUCE		,	//���ٷֱȵ���ˮ����				123
	IATTRIBUTE_RATE_FIREATT_REDUCE		,	//���ٷֱȵ����𹥻�				124
	IATTRIBUTE_RATE_SOIL_ATT_REDUCE		,	//���ٷֱȵ���������				125
	
	IATTRIBUTE_RATE_DREAD_RESIST		,	//�־忹��							126
	IATTRIBUTE_RATE_COMA_RESIST			,	//���Կ���							127
	IATTRIBUTE_RATE_HUSH_RESIST			,	//��Ĭ����							128
	IATTRIBUTE_RATE_UNARM_RESIST		,	//��е����							129
	IATTRIBUTE_RATE_ATTACKSPEED_RESIST	,	//�����ٿ���						130
	IATTRIBUTE_RATE_SKILLSPEED_RESIST	,	//��ʩ�ٿ���						131
	IATTRIBUTE_RATE_MOVEREDUCE_RESIST	,	//�����ٿ���						132

	//���ɼ����˺�ö��--��ʼ���뱣֤����ö�ٵ�������
	IATTRIBUTE_REDUCE_DAMAGE_SHAOLIN_GWS	,		//�����˺�--������[����ɮ]			82
	IATTRIBUTE_REDUCE_DAMAGE_ID_SHAOLIN_TLH	,		//�����˺�--������[���޺�]			83
	IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_GGB	,		//�����˺�--ؤ��[��ؤ��]			84
	IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_ZGB	,		//�����˺�--ؤ��[��ؤ��]			85
	IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_JWD	,		//�����˺�--�䵱��[���䵱]			86
	IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_QWD	,		//�����˺�--�䵱��[���䵱]			87
	IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_DT	,		//�����˺�--����[����]				88
	IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_XJT	,		//�����˺�--����[������]			89
	IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_XXY	,		//�����˺�--��ң��[����ң]			90
	IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_ZXY	,		//�����˺�--��ң��[ָ��ң]			91
	IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_KYW,		//�����˺�--ҩ����[��ҩ��]			92
	IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_YYW,		//�����˺�--ҩ����[ҽҩ��]			93
	IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_DTH,		//�����˺�--�һ���[���һ�]			94
	IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_YTH,		//�����˺�--�һ���[���һ�]			95
	IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_CEM,			//�����˺�--������[�̶�ü]			96
	IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_FEM,			//�����˺�--������[����ü]			97
	IATTRIBUTE_REDUCE_DAMAGE_ID_WUMENPAI,			//�����˺�--�����ɣ������֡�����	98
	//���ɼ����˺�ö��--������
	
	IATTRIBUTE_ID_IMPACT				=500,	//Ӱ��impact��ID
	IATTRIBUTE_NUMBER, //��Ʒ������������
};

enum EQUIP_PAI
{
	EQUIPPAI_ANY		=	1	,//������
	EQUIPPAI_ANYFENG	=	2	,//�������
	EQUIPPAI_ANYJIAN	=	3	,//�������
	EQUIPPAI_ANYZI		=	4	,//��������
	EQUIPPAI_ANYFEIZI	=	5	,//���������
	EQUIPPAI_ANYTIAO	=	6	,//��������
	EQUIPPAI_ANYTONG	=	7	,//����Ͳ��
	EQUIPPAI_ANYWAN		=	8	,//��������
	EQUIPPAI_ANYSHUN	=	9	,//����˳
	EQUIPPAI_ANYKE		=	10	,//�����
	EQUIPPAI_ANYJIANG	=	11	,//���⽫
	EQUIPPAI_DONG		=	15	,//��
	EQUIPPAI_NAN		=	16	,//��
	EQUIPPAI_XI			=	17	,//��
	EQUIPPAI_BEI		=	18	,//��
	EQUIPPAI_ZHONG		=	19	,//��
	EQUIPPAI_FA			=	20	,//��
	EQUIPPAI_BAI		=	21	,//��
	EQUIPPAI_1TIAO		=	31	,//1��
	EQUIPPAI_2TIAO		=	32	,//2��
	EQUIPPAI_3TIAO		=	33	,//3��
	EQUIPPAI_4TIAO		=	34	,//4��
	EQUIPPAI_5TIAO		=	35	,//5��
	EQUIPPAI_6TIAO		=	36	,//6��
	EQUIPPAI_7TIAO		=	37	,//7��
	EQUIPPAI_8TIAO		=	38	,//8��
	EQUIPPAI_9TIAO		=	39	,//9��
	EQUIPPAI_1TONG		=	41	,//1Ͳ
	EQUIPPAI_2TONG		=	42	,//2Ͳ
	EQUIPPAI_3TONG		=	43	,//3Ͳ
	EQUIPPAI_4TONG		=	44	,//4Ͳ
	EQUIPPAI_5TONG		=	45	,//5Ͳ
	EQUIPPAI_6TONG		=	46	,//6Ͳ
	EQUIPPAI_7TONG		=	47	,//7Ͳ
	EQUIPPAI_8TONG		=	48	,//8Ͳ
	EQUIPPAI_9TONG		=	49	,//9Ͳ
	EQUIPPAI_1WAN		=	51	,//1��
	EQUIPPAI_2WAN		=	52	,//2��
	EQUIPPAI_3WAN		=	53	,//3��
	EQUIPPAI_4WAN		=	54	,//4��
	EQUIPPAI_5WAN		=	55	,//5��
	EQUIPPAI_6WAN		=	56	,//6��
	EQUIPPAI_7WAN		=	57	,//7��
	EQUIPPAI_8WAN		=	58	,//8��
	EQUIPPAI_9WAN		=	59	,//9��
	EQUIP_PAI_NUM,
};

enum	INDEX_PAITYPE
{
	INDEX_ANYSHUN	=	0	,//����˳
	INDEX_ANYKE		=	1	,//�����
	INDEX_ANYJIANG	=	2	,//���⽫
	INDEX_NOANYPAINUM,
};

enum    EM_EQUIP_CARD_GROUP
{
	//��Ʒ 1 ��Ʒ2  ��					 0
	//ñ  ���� ����                      1
	//��ָ1 ��ָ2  Ь��					 2		
	//�� �·� ����						 3
	// ���� ����						 4
	 EM_EQUIP_CARD_GROUP_INVAILD    = -1,
     EM_EQUIP_CARD_GROUP_CHAIN1		= 0,
	 EM_EQUIP_CARD_GROUP_CHAIN2	,	 
	 EM_EQUIP_CARD_GROUP_CHAIN3	, 
	 EM_EQUIP_CARD_GROUP_CHAIN4	,		 
	 EM_EQUIP_CARD_GROUP_JIANG	,		 
	 EM_EQUIP_CARD_GROUP_MAXNUM,
};

//��Ʒ����
enum ITEM_CLASS
{
	ICLASS_EQUIP		=10,	//װ��
	ICLASS_MATERIAL		,	//ԭ��
	ICLASS_COMITEM		,	//��ʹ����Ʒ
	ICLASS_TASKITEM		,	//������Ʒ
	ICLASS_GEM			,   //��ʯ
	ICLASS_STOREMAP		,	//�ر�ͼ
	ICLASS_TALISMAN		,	//����---???
	ICLASS_GUILDITEM	,	//�����Ʒ
	ICLASS_IDENT		,	//����ȯ��
	ICLASS_EXTRABAG		,	//��չ����
	ICLASS_SOUL_BEAD	,	//��������
	ICLASS_MOUNT        ,   //����             [7/8/2010 �¾���]
	ICLASS_NUMBER       ,   //��Ʒ���������
};

enum ITEM_CREATE_TYPE
{
	ITEM_IB			=0,
	ITEM_DROP,
	ITEM_TASK,
	ITEM_CREATE,
	ITEM_NUM,
};
//װ������
enum EQUIP_TYPE
{
	EQUIP_GUN			=1,	//��
	EQUIP_CHANSHOU		,	//����
	EQUIP_BANG			,	//��
	EQUIP_CHANGTAO		,	//����
	EQUIP_JIAN			,	//��
	EQUIP_FUZHOU		,	//����
	EQUIP_DADAO			,	//��
	EQUIP_ANQI			,	//����
	EQUIP_SHANZI		,	//����
	EQUIP_JINSHA		,//��ɳ
	EQUIP_HULU			,//��«
	EQUIP_YAOFEN		,//ҩ��
	EQUIP_DIZI			,//����
	EQUIP_BANZHI		,//��ָ
	EQUIP_CI			,//��
	EQUIP_ZHEN			,//��
	
	EQUIP_RESERVE1		,//Ԥ��1
	EQUIP_RESERVE2		,//Ԥ��2
	EQUIP_RESERVE3		,//Ԥ��3

	EQUIP_CAP			=20,//ͷ��
	EQUIP_NECKLACE		,//����
	EQUIP_SHOULDER		,//�ļ�
	EQUIP_ARMOR			,//�·�
	EQUIP_CUFF			,//����
	EQUIP_HAND			,//����
	EQUIP_SASH			,//����
	EQUIP_BOOT			,//Ь��
	EQUIP_RING			,//��ָ
	EQUIP_SOUXIA		,//Ԥ��1�ĳ�����by gh 2010/05/07
	EQUIP_RESERVE5		,//Ԥ��2
	EQUIP_RESERVE6		,//Ԥ��3
	EQUIP_ADORN		    ,//��Ʒ
	EQUIP_SUIT			,//��װ
	EQUIP_TOOL			,//����
	EQUIP_TYPE_NUM,
};
//������������
enum IDENT_ITEM_TYPE
{
	IDENT_ITEM_TYPE    =0,
	IDENT_WPIDENT	   =6,	// ������������
	IDENT_ARIDENT	   =7,	// ���߼�������
	IDENT_NCIDENT	   =8,	// ������������
};
//��ʯ����
enum GEM_TYPE
{
	GEM_COLORFUL		=0,	//��ʱ�ʯ
	GEM_RED				,	//�챦ʯ
	GEM_GREEN			,	//�̱�ʯ
	GEM_YELLOW			,	//�Ʊ�ʯ
	GEM_BLUE			,	//����ʯ
	GEM_TYPE_NUM		,
};

enum MATERIAL_TYPE
{

};

enum SOUL_BEAD_TYPE
{
	SOUL_BEAD_WU		=1,	//�����
	SOUL_BEAD_QI		,	//������
	SOUL_BEAD_TYPE_NUM	,
};
//--------------------------------------------------------------
// ICLASS_COMITEM
enum COMMON_ITEM_TYPE
{
	COMMON_ITEM_TYPE    =0,
	COMMON_ITEM_HP,			// ��ҩ
	COMMON_ITEM_MP,			// ��ҩ
	COMMON_ITEM_3,			// ��ʱδ����
	COMITEM_PRESCR		=4, // �䷽
	COMMON_HORSE_ITEM	=5, // ����Ʒ
	COMITEM_WPIDENT		=6,	// ������������
	COMITEM_ARIDENT		=7,	// ���߼�������
	COMITEM_NCIDENT		=8,	// ������������
	COMITEM_TREASUREBOX = 9,		// ����
	COMITEM_TREASUREBOX_KEY = 10,	// ����Կ��
	COMITEM_SOUXIA			= 11,	//������Ʒ
	COMITEM_SOUXIA_RELIC	= 12,	//������Ʒ��Ƭ
	COMITEM_EQUIP_REFINING	= 13,	//�������
	COMITEM_EQUIP_CANCEL_MAGIC=14,	//��ħʯ
};

enum ENUM_UPDATE_EQIPMENT
{
	UPDATE_EQIPMENT_WEAPON_ID	= 0,	// ����
	UPDATE_EQIPMENT_CAP_ID,				// ñ��
	UPDATE_EQIPMENT_ARMOUR_ID,			// �·�
	UPDATE_EQIPMENT_WRIST_ID,			// ����
	UPDATE_EQIPMENT_FOOT_ID,			// ѥ��
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
	DURSPOIL_NO		 = -1,  //�����
	DURSPOIL_ATTACT  = 1,   //�������
	DURSPOIL_DEFENCE = 2,   //�ܻ����
	DURSPOIL_NUMBER	 ,
};


//ʰȡ����
enum	PICK_RULER
{
	IPR_FREE_PICK,	//����ʰȡ
	IPR_BET_PICK,	//Ͷ����ʰȡ
	IPR_TURN_PICK,	//����ʰȡ

};

//�������
enum	BOX_DISTRIBUTE_RULER
{
	BDR_COMMON	=	 0,
	BDR_BOSS	=	 1,
	BDR_BLUE	=    2,
	BDR_GOLD	=    3,
	BDR_UNKNOW	=	 0xFFFF
};

//��Ʒ�������
enum	ITEM_DISTRIBUTE_RULER
{
	ITEMDR_RANDOM,
	ITEMDR_ALLGET
};

// ������Ʒ����
enum	HORSE_ITEM_TYPE
{
	HORSE_ITEM_INVALID	= -1,

	HORSE_ITEM_SAVVY,			// ���Ե���
	HORSE_ITEM_BASIC,			// ����
	HORSE_ITEM_HAPPINESS,		// ���ֶ�
	HORSE_ITEM_SKILL_GIRD,		// ���ܸ�
	HORSE_ITEM_SKILL_FORGET,	// ��������
	HORSE_ITEM_SKILL_APPER,		// �������򣬶�Ӧǰ6��ļ���
	HORSE_ITEM_SKILL_STUDY,		// ����ѧϰ����Ӧ��6��ļ���
	HORSE_ITEM_CHILD,			// ��ͯ

	HORSE_ITEM_NUM    
};

//ItemContainer ����
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
	ICT_MOUNT_CONTAINER,       // [7/8/2010 �¾���]
};

// Ŀ������
enum ENUM_ITEM_TARGET_TYPE
{
	ITEM_TARGET_TYPE_INVALID	= -1,
	ITEM_TARGET_TYPE_NONE,				// ����Ŀ��		:	��
	ITEM_TARGET_TYPE_POS,				// λ��			:	TargetPos
	ITEM_TARGET_TYPE_DIR,				// ����			:	TargetDir
	ITEM_TARGET_TYPE_ITEM,				// ����			:	TargetItemIndex
	ITEM_TARGET_TYPE_SELF,				// ����			:	TargetObj
	ITEM_TARGET_TYPE_SELF_PET,			// ���ѵĳ���	:	TargetObj,TargetPetGUID
	ITEM_TARGET_TYPE_FRIEND,			// �Ѻ�Ŀ��		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_PLAYER,		// �Ѻ����		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_MONSTER,	// �Ѻù���		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_PET,		// �Ѻó���		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY,				// �ж�Ŀ��		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_PLAYER,		// �ж����		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_MONSTER,		// �жԹ���		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_PET,			// �жԳ���		:	TargetObj
	ITEM_TARGET_TYPE_ALL_CHARACTER,		// ���н�ɫ		:	TargetObj

	ITEM_TARGET_TYPE_NUMBERS
};

// ���߷�����
#define ITEM_PET_SKILL_STUDY_BEGIN		30402000	//���＼������С���
#define ITEM_PET_SKILL_STUDY_END		30403000	//���＼���������

#define ITEM_PET_RETURN_BABAY_BEGIN		30503011	//���ﻹͯ����С��ţ�ע��30503011����Ҳ�ǺϷ��Ļ�ͯ����ţ�
#define ITEM_PET_RETURN_BABAY_END		30503020	//���ﻹͯ������ţ�ע��30503020����Ҳ�ǺϷ��Ļ�ͯ����ţ�

#define	ITEM_DOME_MEDICINE				30601000	//ѱ������ 
#define	ITEM_MEAT_MEDICINE				30602000	//��ʳ���� 
#define	ITEM_GRASS_MEDICINE				30603000	//������� 
#define	ITEM_WORM_MEDICINE				30604000	//������� 
#define	ITEM_PADDY_MEDICINE				30605000	//�������
#define ITEM_RELIC_SOUXIA				12120001    //����¼��ҳ����add by gh
#define ITEM_WU_SOUL_BEAD				20010001	//�����
#define ITEM_QI_SOUL_BEAD				20020001    //������
#define ITEM_PET_FEED_MEDICINE_MAX		ITEM_PADDY_MEDICINE+1000

#define ITEM_PET_ADD_LIFE_BEGIN			ITEM_PET_FEED_MEDICINE_MAX	//�����ӳ�������С���
#define ITEM_PET_ADD_LIFE_END			30607000	//�����ӳ����������

//��Ʒ��Ϣλ����
enum ITEM_EXT_INFO
{
	IEI_BIND_INFO		=	0x00000001,	//����Ϣ
	IEI_IDEN_INFO		=	0x00000002, //������Ϣ
	IEI_PLOCK_INFO		=	0x00000004, //���������Ѿ�����
	IEI_BLUE_ATTR		=	0x00000008, //�Ƿ���������
	IEL_CREATOR			=	0x00000010,	//�Ƿ��д�����
};


#endif
