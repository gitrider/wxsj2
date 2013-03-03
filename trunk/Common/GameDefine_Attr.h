#ifndef __GAMEDEFINE_ATTR_H__
#define __GAMEDEFINE_ATTR_H__
#include "GameDefine.h"
#include "GameDefine_Item.h"
//#include "DB_Struct.h"
//[[���������ڲ�����ע��]]
//��ɫ��������(һ������)
enum CHAR_ATTR_LEVEL1
{
	CATTR_LEVEL1_STR			=0, //���� ����	[[����]]
	CATTR_LEVEL1_CON			=1,	//���� ����	[[����]]
	CATTR_LEVEL1_INT			=2,	//���� ����	[[����]]
	CATTR_LEVEL1_DEX			=3,	//�� ����	[[��]]

	CATTR_LEVEL1_NUMBER, //��ɫ������������
};

enum CHAR_ATTR_LEVEL
{
	CATTR_LEVEL_STR			=0, //���� ����
	CATTR_LEVEL_CON			=1,	//���� ����
	CATTR_LEVEL_INT			=2,	//���� ����
	CATTR_LEVEL_DEX			=3,	//�� ����
	CATTR_LEVEL_SKILL		,
	CATTR_LEVEL_NUMBER, //��ɫ������������
};

//��ɫ��������
enum CHAR_ATTR_LEVEL2
{
	//CATTR_LEVEL2_ATTACKRATE_P =0,	//������
	//CATTR_LEVEL2_DEFENCE_P,			//�������

	CATTR_LEVEL2_DEFENCE_NEAR =0,		//���̷���
	CATTR_LEVEL2_DEFENCE_FAR,		//Զ�̷���
	CATTR_LEVEL2_ATTACK_NEAR,		//���̹���
	CATTR_LEVEL2_ATTACK_FAR,		//Զ�̹���
	CATTR_LEVEL2_TOUGHNESS,         //����

	CATTR_LEVEL2_ATTACKRATE_MAGIC_NEAR,	//�����ڹ�����
	CATTR_LEVEL2_ATTACKRATE_MAGIC_FAR,	//�����⹦����

	CATTR_LEVEL2_DEFENCE_MAGIC_NEAR,	//�����ڹ�����
	CATTR_LEVEL2_DEFENCE_MAGIC_FAR,	//�����⹦����

	CATTR_LEVEL2_MAXHP,				//HP����
	CATTR_LEVEL2_MAXMP,				//MP����
	CATTR_LEVEL2_RESTOREHP,			//HP�ظ�
	CATTR_LEVEL2_RESTOREMP,			//MP�ظ�
	CATTR_LEVEL2_HIT,				//������
	CATTR_LEVEL2_MISS,				//����
	CATTR_LEVEL2_CRITRATE,			//������
	CATTR_LEVEL2_SPEED,				//�ƶ��ٶ�
	CATTR_LEVEL2_ATTACKSPEED,		//�����ٶ�

	CATTR_LEVEL2_ATTACK_GOLD,		//�𹥻�
	CATTR_LEVEL2_DEFENCE_GOLD,		//�����

	CATTR_LEVEL2_ATTACK_WOOD,		//ľ����
	CATTR_LEVEL2_DEFENCE_WOOD,		//ľ����

	CATTR_LEVEL2_ATTACK_WATER,		//ˮ����
	CATTR_LEVEL2_DEFENCE_WATER,		//ˮ����

	CATTR_LEVEL2_ATTACK_FIRE,		//�𹥻�
	CATTR_LEVEL2_DEFENCE_FIRE,		//�����

	CATTR_LEVEL2_ATTACK_SOIL,		//������
	CATTR_LEVEL2_DEFENCE_SOIL,		//������

	CATTR_LEVEL2_NUMBER,			//������������
};



//��ɫ��������
enum CHAR_ATTR_LEVEL3
{
	CATTR_LEVEL3_NUQI			=0,	//ŭ��ֵ
	CATTR_LEVEL3_SKILLPOINT		=1,	//������
	CATTR_LEVEL3_XIANJIN		=2,	//������Ϣ
	CATTR_LEVEL3_YINSHEN		=3,	//������Ϣ

	CATTR_LEVEL3_NUMBER, //������������
};

//��ɫ�����ݿ��ShareMem�ж�Ӧ������
enum	CHAR_ATTR_DB
{
	//////////////////////////////////////////////////////////////////////////
	//�����ױ仯��������
	CATTR_DB_HP					=	0,				//����
	CATTR_DB_MP					,					//ħ��
	CATTR_DB_PNEUMA				,					//Ԫ��
	CATTR_DB_STRIKEPOINT		,					//����
	CATTR_DB_VIGOR				,					//����
	CATTR_DB_MAX_VIGOR			,					//��������
	CATTR_DB_VIGOR_REGENE_RATE	,					//�����ָ��ٶ�
	CATTR_DB_ENERGY				,					//����
	CATTR_DB_MAX_ENERGY			,					//��������
	CATTR_DB_ENERGY_REGENE_RATE	,					//�����ָ��ٶ�
	CATTR_DB_RAGE				,					//ŭ��
	CATTR_DB_LEVEL				,					//�ȼ�
	CATTR_DB_PKVALUE,								//ɱ��
	CATTR_DB_CURRENTPET,							//��ǰ����GUID
	CATTR_DB_EXP,									//����
	CATTR_DB_AT_POSITION,							//���λ��
	CATTR_DB_BK_POSITION,							//��ұ���λ��
	CATTR_DB_AT_SCENE,								//��ҳ���
	CATTR_DB_BK_SCENE,								//��ұ��ݳ���
	//////////////////////////////////////////////////////////////////////////
	//һ������
	CATTR_DB_STR,									//���� ����
	//CATTR_DB_SPR,									//���� ����
	CATTR_DB_CON,									//���� ����
	CATTR_DB_INT,									//���� ����
	CATTR_DB_DEX,									//�� ����
	CATTR_DB_REMAINPOINT,							//ʣ�����
	CATTR_DB_REMAINSKILLPOINT,						//ʣ�༼�ܵ�
	CATTR_DB_S_SKILLPOINT,							//ϵ���ܵ�
	//////////////////////////////////////////////////////////////////////////
	//�������
	CATTR_DB_HORSE_MATTING_FLAG,					//���ﷱֳ���
	//////////////////////////////////////////////////////////////////////////
	//���ٱ仯����
	CATTR_DB_CAMP,									//��Ӫ���
	CATTR_DB_COUNTRY,								//����
	CATTR_DB_MENPAI,								//���ɱ��
	CATTR_DB_GUILDID,								//���ID
	CATTR_DB_FAMILYID,								//����ID
	CATTR_DB_GUILDOFFICIAL,							//����ְ
	CATTR_DB_GUILDPOINT,							//��ṱ�׶�
	CATTR_DB_HONOR,							        //����
	CATTR_DB_TEAMID,								//�����
	CATTR_DB_GUID,									//��ɫΨһ��
	CATTR_DB_PORTRAITID,							//��ɫͷ��
	CATTR_DB_NAME,									//��ɫ����
	CATTR_DB_SEX,									//��ɫ�Ա�
	CATTR_DB_CREATETIME,							//��������
	CATTR_DB_TITLE,									//��ɫ�ƺ�
	CATTR_DB_PASSWORD,								//��ɫ��������
	CATTR_DB_PWDDELTIME,							//����ǿ�ƽ�������ʱ��
	CATTR_DB_HAIR_COLOR,							//ͷ����ɫ	
	CATTR_DB_FACE_COLOR,							//������ɫ
	CATTR_DB_HAIR_MODEL,							//ͷ��ģ��
	CATTR_DB_FACE_MODEL,							//����ģ��
	CATTR_DB_ONLINETIME,							//������ʱ��
	CATTR_DB_LASTLOGINTIME,							//���һ�ε���ʱ��
	CATTR_DB_LASTLOGOUTTIME,						//���һ�εǳ�ʱ��
	CATTR_DB_DBVERSION,								//�浵�汾
	
	CATTR_DB_MONEY,									//��Ǯ
	CATTR_DB_GOODBAD,								//�ƶ�ֵ
	CATTR_DB_BANK_MONEY,							//���н�Ǯ
	CATTR_DB_BANK_OFFSIZE,							//����end
    CATTR_DB_LEAVEGUILDTIME,						//�˳�����ʱ��

	//////////////////////////////////////////////////////////////////////////
	//����̵�����
	CATTR_DB_PLAYER_SHOP_BEGIN,						//����̵꿪ʼ��λ��
	CATTR_DB_PLAYER_SHOP_END	=					//����̵����λ��
	CATTR_DB_PLAYER_SHOP_BEGIN+MAX_SHOP_NUM_PER_PLAYER-1,
	CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN,			//����̵��ղؼ���ʼ
	CATTR_DB_PLAYER_SHOP_FAVORITE_END = 
	CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN + MAX_FAVORITE_SHOPS,//����̵��ղؼн���

	//////////////////////////////////////////////////////////////////////////
	//��������
    CATTR_DB_BAG_BEGIN,								//������ʼλ��
	CATTR_DB_BAG_END			=					//��������λ��
	CATTR_DB_BAG_BEGIN+MAX_BAG_SIZE-1,
	//////////////////////////////////////////////////////////////////////////
	//װ������
	CATTR_DB_EQUIP_BEGIN,							//װ����ʼλ��
	CATTR_DB_EQUIP_END		=						//װ������λ��
	CATTR_DB_EQUIP_BEGIN+HEQUIP_NUMBER-1,	
	//////////////////////////////////////////////////////////////////////////
	//��������
	CATTR_DB_BANK_BEGIN,							//���п�ʼλ��
	CATTR_DB_BANK_END			=					//���н���λ��
	CATTR_DB_BANK_BEGIN+MAX_BANK_SIZE-1,	
	//////////////////////////////////////////////////////////////////////////
	//��������
	CATTR_DB_SKILL,									//����
	//////////////////////////////////////////////////////////////////////////
	//CoolDown����
	CATTR_DB_COOLDOWN,								//��ȴʱ��
	CATTR_DB_XINFA,									//�ķ�����
	CATTR_DB_IMPACT,								//Ч������
	CATTR_DB_ABILITY,								//�����
	CATTR_DB_MISSION,								//��������
	CATTR_DB_SETTING,								//��ɫ��������
	CATTR_DB_PET,									//��������
	CATTR_DB_RELATION,								//��ϵ�����ݣ����ѡ��������ȣ�
	CATTR_DB_PRIVATEINFO,							//˽����Ϣ
	CATTR_DB_TITLEINFO,								//�ƺ��б�
	CATTR_DB_RESERVE,								//������Ϣ
	CATTR_DB_SOUXIA_BEGIN,							//������Ϣ��ʼλ��
	CATTR_DB_SOUXIA_END			= 
	CATTR_DB_SOUXIA_BEGIN + MAX_SOUXIA_CONTAINER - 1,//������Ϣ����λ��
	CATTR_DB_NUMBER,
};

#define HUMAN_DB_ATTR_FLAG_LENGTH	((CATTR_DB_NUMBER>>3)+1)

//����: һЩ��ɫ���Ե����ֵ
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
//����: һЩ��ɫ���Ե����ֵ
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
//���ߺ�����У�����Ե���Чֵ
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


//����: ������ε���������Ŀ
enum
{
	STRIKE_POINT_SEGMENT_SIZE = 3,
};
//����: ������������
enum
{
	MAX_STRIKE_POINT_SEGMENT = MAX_STRIKE_POINT/STRIKE_POINT_SEGMENT_SIZE,
};
//����: Ч����󼶱�
enum
{
	MAX_IMPACT_LEVEL = 12,//�ķ�level/10
};


//�����������
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
//����: һЩ�������Ե����ֵ
enum PET_ATTR_CONSTANT1
{
	PET_BASE_MAX_HAPPINESS = 100,
};

// ����״̬
#define	INVALID_MOOD_STATE		(-1)	// ��Ч�ı���״̬

#endif
