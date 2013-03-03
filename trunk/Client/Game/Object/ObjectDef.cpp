
/** ObjectDef.cpp
 */

#include "StdAfx.h"
#include "ObjectDef.h"


/*
enum ENUM_LOCATOR_UNIT
{
	LOCATOR_UNIT_INVALID	= -1,
	LOCATOR_UNIT_CENTER,			// ��������
	LOCATOR_UNIT_HEAD,				// ͷ��
	LOCATOR_UNIT_HAND_L,			// ����
	LOCATOR_UNIT_HAND_R,			// ����
	LOCATOR_UNIT_WEAPON_L,			// ������
	LOCATOR_UNIT_WEAPON_R,			// ������
	LOCATOR_UNIT_FOOT_L,			// ���
	LOCATOR_UNIT_FOOT_R,			// �ҽ�
	LOCATOR_UNIT_FOOT_CENTER,		// �ŵ�����
	LOCATOR_UNIT_CALVARIA,			// ͷ��ƫ��
	LOCATOR_UNIT_HAND_CENTER,		// ��������

	LOCATOR_UNIT_NUMBERS
};
*/

/*
INT GetLocatorName( ENUM_LOCATOR eLocator, const char **paszOutLocatorName )
{
	static LPCSTR lpszLocatorInvalid = "���¹�Ȧ��";
	static LPCSTR alpszLocatorName[LOCATOR_UNIT_NUMBERS] = {
			"�����ܻ���",		//LOCATOR_UNIT_CENTER,		// ��������
			"ͷ��״̬��",		//LOCATOR_UNIT_HEAD,		// ͷ��
			"�������󶨵�",		//LOCATOR_UNIT_HAND_L,		// ����
			"�������󶨵�",		//LOCATOR_UNIT_HAND_R,		// ����
			"�������󶨵�",		//LOCATOR_UNIT_WEAPON_L,	// ������
			"�������󶨵�",		//LOCATOR_UNIT_WEAPON_R,	// ������
			"����ȵص�01",		//LOCATOR_UNIT_FOOT_L,		// ���
			"����ȵص�",		//LOCATOR_UNIT_FOOT_R,		// �ҽ�
			"���¹�Ȧ��",		//LOCATOR_UNIT_FOOT_CENTER,	// �ŵ�����
			"ͷ��״̬��",		//LOCATOR_UNIT_CALVARIA,	// ͷ��ƫ��
			//"Center",			//LOCATOR_UNIT_CENTER,		// ��������
			//"Head",			//LOCATOR_UNIT_HEAD,		// ͷ��
			//"Hand_L",			//LOCATOR_UNIT_HAND_L,		// ����
			//"Hand_R",			//LOCATOR_UNIT_HAND_R,		// ����
			//"Weapon_L",		//LOCATOR_UNIT_WEAPON_L,	// ������
			//"Weapon_R",		//LOCATOR_UNIT_WEAPON_R,	// ������
			//"Foot_L",			//LOCATOR_UNIT_FOOT_L,		// ���
			//"Foot_R",			//LOCATOR_UNIT_FOOT_R,		// �ҽ�
			//"Foot_Center",	//LOCATOR_UNIT_FOOT_CENTER,	// �ŵ�����
			//"Calvaria",		//LOCATOR_UNIT_CALVARIA,	// ͷ��ƫ��
	};

	INT nNumResult = 0;
	switch ( eLocator )
	{
	case LOCATOR_CENTER:		// ��������
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_CENTER];
		break;
	case LOCATOR_HEAD:			// ͷ��
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_HEAD];
		break;
	case LOCATOR_HAND_L:		// ����
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_HAND_L];
		break;
	case LOCATOR_HAND_R:		// ����
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_HAND_R];
		break;
	case LOCATOR_WEAPON_L:		// ������
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_WEAPON_L];
		break;
	case LOCATOR_WEAPON_R:		// ������
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_WEAPON_R];
		break;
	case LOCATOR_FOOT_L:		// ���
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_FOOT_L];
		break;
	case LOCATOR_FOOT_R:		// �ҽ�
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_FOOT_R];
		break;
	case LOCATOR_FOOT_CENTER:	// �ŵ�����
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_FOOT_CENTER];
		break;
	case LOCATOR_CALVARIA:		// ͷ��ƫ��
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_CALVARIA];
		break;
	case LOCATOR_HAND_L_AND_R:	// ���ֺ�����
		nNumResult = 2;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_HAND_L];
		paszOutLocatorName[1]	= alpszLocatorName[LOCATOR_UNIT_HAND_R];
		break;
	case LOCATOR_HAND_CENTER:	// ��������
		nNumResult = 1;
		paszOutLocatorName[0]	= alpszLocatorName[LOCATOR_UNIT_HAND_CENTER];
		break;
	default:
		nNumResult = 0;
		break;
	}
	return nNumResult;
}
*/


LPCSTR GetCharaLocatorName( ENUM_CHARACTER_LOCATOR eLocator )
{
	switch( eLocator )
	{
	case LOCATOR_CHAR_HEAD:			return "ͷ���󶨵�";
	case LOCATOR_CHAR_FRONT:		return "����ǰ�õ�";
	case LOCATOR_CHAR_ATTACK:		return "�����ܻ���";
	case LOCATOR_CHAR_CENTER:		return "�������ĵ�";
	case LOCATOR_CHAR_FOOT:			return "�Ų��󶨵�";
	case LOCATOR_CHAR_HAND_L:		return "���ְ󶨵�";
	case LOCATOR_CHAR_HAND_R:		return "���ְ󶨵�";
	case LOCATOR_CHAR_SHOULDER_L:	return "���󶨵�";
	case LOCATOR_CHAR_SHOULDER_R:	return "�Ҽ�󶨵�";
	case LOCATOR_CHAR_SHIELD_L:		return "��۶��ư󶨵�";
	};

	return "";
}

LPCSTR GetMountLocatorName( ENUM_MOUNT_LOCATOR eLocator )
{
	switch( eLocator )
	{
	case LOCATOR_MOUNT_LF:		return "��������ǰ��󶨵�";
	case LOCATOR_MOUNT_RF:		return "��������ǰ��󶨵�";
	case LOCATOR_MOUNT_LB:		return "�����������󶨵�";
	case LOCATOR_MOUNT_RB:		return "�������Һ���󶨵�";
	case LOCATOR_MOUNT_FOOT:	return "������Ų��󶨵�";
	case LOCATOR_MOUNT_HEAD:	return "������ͷ���󶨵�";
	case LOCATOR_MOUNT_BOSOM:	return "�������ز��󶨵�";
	case LOCATOR_MOUNT_BACK:	return "���������󶨵�";	 //test
		// ��������NOX��󶨵�
	};

	return "";
}

LPCSTR GetWeaponLocatorName( ENUM_WEAPON_LOCATOR eLocator )
{
	switch( eLocator )
	{
	case LOCATOR_WEAPON_1:		return "�����󶨵�_01";
	case LOCATOR_WEAPON_2:		return "�����󶨵�_02";
	case LOCATOR_WEAPON_3:		return "�����󶨵�_03";
	case LOCATOR_WEAPON_4:		return "�����󶨵�_04";
	};

	return "";
}




//================
// ����װ��

//const char *szIDSTRING_WEAPON			= ("����");
//const char *szIDSTRING_CAP			= ("ñ��");
//const char *szIDSTRING_ARMOUR			= ("�·�");
//const char *szIDSTRING_CUFF			= ("����");
//const char *szIDSTRING_BOOT			= ("ѥ��");
//const char *szIDSTRING_NECKLACE		= ("����");
//const char *szIDSTRING_SASH			= ("����");
//const char *szIDSTRING_RING			= ("��ָ");

const char *szIDSTRING_FACE_MESH		= ("FaceMesh");
const char *szIDSTRING_FACE_MAT			= ("FaceMat");

const char *szIDSTRING_HAIR_MESH		= ("HairMesh");
const char *szIDSTRING_HAIR_MAT			= ("HairMat");

const char *szIDSTRING_SHOULDER_MESH	= ("ShoulderMesh");
const char *szIDSTRING_SHOULDER_MAT		= ("ShoulderMat");

const char *szIDSTRING_MAINBODY_MESH	= ("MainBodyMesh");
const char *szIDSTRING_MAINBODY_MAT		= ("MainBodyMat");

const char *szIDSTRING_FOOT_MESH		= ("FootMesh");
const char *szIDSTRING_FOOT_MAT			= ("FootMat");

const char *szIDSTRING_CAP_MESH			= ("CapMesh");
const char *szIDSTRING_CAP_MAT			= ("CapMat");

const char *szIDSTRING_ARM_MESH			= ("ArmMesh");
const char *szIDSTRING_ARM_MAT			= ("ArmMat");

//const char *szIDSTRING_CURRENT_ACTION	= ("CurrAction");

const char *szIDSTRING_CURRENT_LEFTWEAPON	= ("LeftWeaponObj");
const char *szIDSTRING_CURRENT_RIGHTWEAPON	= ("RightWeaponObj");
const char *szIDSTRING_SHIELD_RIGHTWEAPON   = ("LeftShieldObj");


//const char *szIDSTRING_MOUNT_BACK_ATTACH	= ("���������󶨵�");

//const char *szIDSTRING_BODY_ATTACK_ATTACH	= ("���������ܻ���");

