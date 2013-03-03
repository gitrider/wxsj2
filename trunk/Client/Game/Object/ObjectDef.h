
/** ObjectDef.h
 */

#pragma once


#include "GameDefine.h"




// �ͻ���Ԥ��ָ����߼�������ֵ
#define DEF_CLIENT_LOGIC_COUNT_ADD_STEP		(100)

// ��ɫλ��У���ľ���
#define DEF_CHARACTER_POS_ADJUST_DIST		(2.f)

// �ͻ��˷���ɫλ�ý�����Ϣ�ľ��뷧ֵ
#define DEF_CLIENT_ADJUST_POS_WARP_DIST		(DEF_SERVER_ADJUST_POS_WARP_DIST * 0.8f)

// ����ս��״̬��Ҫ������ʱ��
#define FIGHT_STATE_DURATION_TIME			(8000)



// �Ա�
enum ENUM_GENDER
{
	GENDER_INVALID	= -1,
	GENDER_MALE,			// ��
	GENDER_FEMALE			// Ů
};


// ��������
enum ENUM_WEAPON_TYPE
{
	WEAPON_TYPE_INVALID	= -1,

	WEAPON_TYPE_NONE,		// ͽ��
	WEAPON_TYPE_GUN,	// ��
	WEAPON_TYPE_BANG,		// ��
	WEAPON_TYPE_JIAN,		// ��
	WEAPON_TYPE_DAO,		// ��
	WEAPON_TYPE_SHANZI,		// ����
	WEAPON_TYPE_HULU,		// ��«
	WEAPON_TYPE_DIZI,		// ����
	WEAPON_TYPE_CI,		// ��
	WEAPON_TYPE_ANQI,	//����
	WEAPON_TYPE_SHUANGCHUI,	//˫��
	WEAPON_TYPE_QING,	//��
	WEAPON_TYPE_ZHONGJIAN,	//�ؽ�
	WEAPON_TYPE_NPC,		// NPC�����ǲ����Ա������� Monster
	WEAPON_TYPE_TOOL,	//����ܹ���

	WEAPON_TYPE_NUMBERS,
};

/// ���ֻ�������
enum ENUM_WEAPON_LOCATOR_TYPE
{
	WL_LEFT,	// ��
	WL_RIGHT,	// ��
	WL_L_ARM,	// ���ֱۣ������ã�
	WL_BOTH,	// ˫��
};



#define ACTION_TYPE_NUMBERS	(10)
// ������
enum ENUM_BASE_ACTION
{
	BASE_ACTION_INVALID	= -1,

	BASE_ACTION_N_IDLE			=  0,			// (��ƽ)վ��		��Ϣ
	BASE_ACTION_N_IDLE_EX0		=  1,			// (��ƽ)����1		��ϢС����1
	BASE_ACTION_N_IDLE_EX1		=  2,			// (��ƽ)����2		��ϢС����2
	BASE_ACTION_N_WALK			=  3,			// (��ƽ)����
	BASE_ACTION_N_RUN			=  4,			// (��ƽ)�ܲ�

// û�õ�
	BASE_ACTION_N_SIT_DOWN		=  5,			// (��ƽ)����
	BASE_ACTION_N_SIT_IDLE		=  6,			// (��ƽ)������Ϣ
	BASE_ACTION_N_SIT_STAND		=  7,			// (��ƽ)��������
	BASE_ACTION_N_RESERVE_00	=  8,			// ����
	BASE_ACTION_N_JUMP_N		=  9,			// (��ƽ)������Ծ��
	BASE_ACTION_N_RESERVE_01	= 10,			// ����
	BASE_ACTION_N_RESERVE_02	= 11,			// ����
	BASE_ACTION_N_JUMP_R		= 12,			// (��ƽ)�ƶ�����Ծ
	BASE_ACTION_N_RESERVE_03	= 13,			// ����

	BASE_ACTION_F_IDLE			= 14,			// (ս��)����		��Ϣ

// û�õ�
	BASE_ACTION_F_WALK			= 15,			// (ս��)����
	BASE_ACTION_F_RUN			= 16,			// (ս��)�ܲ�
	BASE_ACTION_F_RESERVE_04	= 17,			// ����
	BASE_ACTION_F_JUMP_N		= 18,			// (ս��)������Ծ��
	BASE_ACTION_F_RESERVE_05	= 19,			// ����
	BASE_ACTION_F_RESERVE_06	= 20,			// ����
	BASE_ACTION_F_JUMP_R		= 21,			// (ս��)�ƶ�����Ծ
	BASE_ACTION_F_RESERVE_07	= 22,			// ����

	BASE_ACTION_F_BE_HIT0		= 23,			// (ս��)����1
	BASE_ACTION_F_BE_HIT1		= 24,			// (ս��)����2
	BASE_ACTION_F_DIE0			= 25,			// (ս��)����1
	BASE_ACTION_F_DIE1			= 26,			// (ս��)����2
	BASE_ACTION_F_DIEFLY0		= 27,			// ��������1
	BASE_ACTION_F_DIEFLY1		= 28,			// ��������2
	BASE_ACTION_F_CADAVER0		= 29,			// (ս��)ʬ��1
	BASE_ACTION_F_CADAVER1		= 30,			// (ս��)ʬ��2
	BASE_ACTION_F_CADAVER2		= 31,			// (ս��)ʬ��3
	BASE_ACTION_F_CADAVER3		= 32,			// (ս��)ʬ��4

	BASE_ACTION_FP				= 33,			// ս��׼��
	BASE_ACTION_F_SYNCRETIZE		= 34,			// (ս��)�����ں�

	//BASE_ACTION_GATHER_MINE	= 26,			// �ɿ�
	//BASE_ACTION_GATHER_HERBS	= 27,			// �ɲ�ҩ
	//BASE_ACTION_FISH			= 28,			// ����

	//BASE_ACTION_M_IDLE_0			= 29,		// (������)��Ϣ
	//BASE_ACTION_M_IDLE_EX0_0		= 30,		// (������)��ϢС����1
	//BASE_ACTION_M_IDLE_EX1_0		= 31,		// (������)��ϢС����2
	//BASE_ACTION_M_WALK_0			= 32,		// (������)����
	//BASE_ACTION_M_RUN_0			= 33,		// (������)�ܲ�
	//BASE_ACTION_M_RESERVE_08		= 34,		// ����
	//BASE_ACTION_M_JUMP_N_0		= 35,		// (������)������Ծ��
	//BASE_ACTION_M_RESERVE_09		= 36,		// (����
	//BASE_ACTION_M_RESERVE_10		= 37,		// ����
	//BASE_ACTION_M_JUMP_R_0		= 38,		// (������)�ƶ�����Ծ
	//BASE_ACTION_M_RESERVE_11		= 39,		// ����

	//BASE_ACTION_M_IDLE_1			= 40,		// (������)��Ϣ
	//BASE_ACTION_M_IDLE_EX0_1		= 41,		// (������)��ϢС����1
	//BASE_ACTION_M_IDLE_EX1_1		= 42,		// (������)��ϢС����2
	//BASE_ACTION_M_WALK_1			= 43,		// (������)����
	//BASE_ACTION_M_RUN_1			= 44,		// (������)�ܲ�
	//BASE_ACTION_M_RESERVE_12		= 45,		// ����
	//BASE_ACTION_M_JUMP_N_1		= 46,		// (������)������Ծ��
	//BASE_ACTION_M_RESERVE_13		= 47,		// ����
	//BASE_ACTION_M_RESERVE_14		= 48,		// ����
	//BASE_ACTION_M_JUMP_R_1		= 49,		// (������)�ƶ�����Ծ
	//BASE_ACTION_M_RESERVE_15		= 50,		// ����

// û�õ�
	BASE_ACTION_ABILITY_LEVEL_UP	= 92,		// ���������
//	BASE_ACTION_XINFA_LEVEL_UP		= 93,		// �ķ�����

	BASE_ACTION_LEVEL_UP			= 99,		// ����

	BASE_ACTION_NUMBERS
};

// ��������
enum ENUM_ACTION_TYPE
{
	ACTION_INVALID,
	ACTION_DOWN,		// ����
	ACTION_TOP,			// ����
	ACTION_BODY,		// ȫ��
};



//================
// �󶨵�

/*
enum ENUM_LOCATOR
{
	LOCATOR_INVALID	= -1,
	LOCATOR_CENTER,			// ��������
	LOCATOR_HEAD,			// ͷ��
	LOCATOR_HAND_L,			// ����
	LOCATOR_HAND_R,			// ����
	LOCATOR_WEAPON_L,		// ������
	LOCATOR_WEAPON_R,		// ������
	LOCATOR_FOOT_L,			// ���
	LOCATOR_FOOT_R,			// �ҽ�
	LOCATOR_FOOT_CENTER,	// �ŵ�����
	LOCATOR_CALVARIA,		// ͷ��ƫ��
	LOCATOR_HAND_L_AND_R,	// ���ֺ�����
	LOCATOR_HAND_CENTER,	// ��������

	LOCATOR_NUMBERS
};
*/

// ����󶨵�
enum ENUM_CHARACTER_LOCATOR
{
	LOCATOR_CHAR_INVALID	= -1,

	LOCATOR_CHAR_HEAD,			// ͷ���󶨵�
	LOCATOR_CHAR_FRONT,			// ����ǰ�õ�
	LOCATOR_CHAR_ATTACK,		// �����ܻ���
	LOCATOR_CHAR_CENTER,		// �������ĵ�
	LOCATOR_CHAR_FOOT,			// �Ų��󶨵�
	LOCATOR_CHAR_HAND_L,		// ���ְ󶨵�
	LOCATOR_CHAR_HAND_R,		// ���ְ󶨵�
	LOCATOR_CHAR_SHOULDER_L,	// ���󶨵�
	LOCATOR_CHAR_SHOULDER_R,	// �Ҽ�󶨵�
	LOCATOR_CHAR_SHIELD_L,		// ��۶��ư󶨵�
	
	LOCATOR_CHAR_NUMBERS
};
// return	:	������Ч�İ󶨵�
extern LPCSTR GetCharaLocatorName( ENUM_CHARACTER_LOCATOR eLocator );


// ����󶨵�
enum ENUM_MOUNT_LOCATOR
{
	LOCATOR_MOUNT_INVALID	= -1,

	LOCATOR_MOUNT_LF,		// ��������ǰ��󶨵�
	LOCATOR_MOUNT_RF,		// ��������ǰ��󶨵�
	LOCATOR_MOUNT_LB,		// �����������󶨵�
	LOCATOR_MOUNT_RB,		// �������Һ���󶨵�
	LOCATOR_MOUNT_FOOT,		// ������Ų��󶨵�
	LOCATOR_MOUNT_HEAD,		// ������ͷ���󶨵�
	LOCATOR_MOUNT_BOSOM,	// �������ز��󶨵�
	LOCATOR_MOUNT_BACK,		// ��������NOX��󶨵�

	LOCATOR_MOUNT_NUMBERS,
};
extern LPCSTR GetMountLocatorName( ENUM_MOUNT_LOCATOR eLocator );


// �����󶨵�
enum ENUM_WEAPON_LOCATOR
{
	LOCATOR_WEAPON_INVALID	= -1,

	LOCATOR_WEAPON_1	= 1,// �����󶨵�1
	LOCATOR_WEAPON_2,		// �����󶨵�2
	LOCATOR_WEAPON_3,		// �����󶨵�3
	LOCATOR_WEAPON_4,		// �����󶨵�3

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


// �󶨵㶨��

//extern const char *szIDSTRING_MOUNT_BACK_ATTACH;

//extern const char *szIDSTRING_BODY_ATTACK_ATTACH;


 