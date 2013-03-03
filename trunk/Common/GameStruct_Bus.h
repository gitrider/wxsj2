// GameStruct_Bus.h

#ifndef __GAMESTRUCT_BUS_H__
#define __GAMESTRUCT_BUS_H__

#include "Type.h"

// <=��ֵΪ��Ч�߶�
#define DEF_BUS_PATH_NODE_INVALID_Y		(-100000.f)

// �Ǵ�˿�����
#define DEF_BUS_MAX_PASSENGER_COUNT		(8)

// �ṩ���ű��õĲ�������
#define DEF_BUS_SCRIPT_PARAM_BUF_SIZE	(8)


struct _BUS_SEAT_INFO
{
	INT		m_nActionSetIndex;			// ��CharModel.txt�еĶ���������
	CHAR	*m_pszLocator;				// �˿���λ�İ󶨵�����
};

enum ENUM_BUS_TYPE
{
	BUS_TYPE_INVALID	= -1,
	BUS_TYPE_ONE_WAY,				// ���̳�
	BUS_TYPE_SHUTTLE,				// ������

	BUS_TYPE_NUMBERS
};

struct _BUS_INFO
{
	INT				m_nDataID;					// ���ݱ��
	CHAR			*m_pszName;					// ����
	INT				m_nScriptID;				// �ű�ID
	INT				m_nType;					// ���� ENUM_BUS_TYPE
	INT				m_nStopTime;				// ͣվʱ��
	INT				m_nModelID;					// ģ������ID
	FLOAT			m_fModelHeight;				// ģ�͸߶�
	INT				m_nMaxPassengerCount;		// ׼������
	FLOAT			m_fMoveSpeed;				// ��ʻ�ٶ�
	_BUS_SEAT_INFO	m_aSeatInfo[DEF_BUS_MAX_PASSENGER_COUNT];	// �˿���λ�İ󶨵�����

	_BUS_INFO(VOID)
	{
		CleanUp();
	}

	VOID CleanUp(VOID)
	{
		m_nDataID					= INVALID_ID;
		m_nScriptID					= INVALID_ID;
		m_pszName					= NULL;
		m_nType						= BUS_TYPE_INVALID;
		m_nStopTime					= 0;
		m_nModelID					= INVALID_ID;
		m_fModelHeight				= -1.f;
		m_nMaxPassengerCount		= -1;
		m_fMoveSpeed				= -1.f;
		memset(m_aSeatInfo, 0, sizeof(m_aSeatInfo));
	}
};

/////////////////////////////////////////////////////////////////////
//�����
/////////////////////////////////////////////////////////////////////
//
//struct _DBC_CHARACTER_MOUNT
//{
//	INT				m_nID;
//	LPCSTR			m_pszName;					// �������
//	INT				m_nMaxPassengerCount;		// ׼������
//	INT				m_nModelID;
//	FLOAT			m_fAddHeight;
//	INT				m_nRecipe;					// ��Ӧ������䷽
//	INT				m_nMainActionFileIndex;		// ����������
//	LPCSTR			m_pszMainAttachLocator;		// ���󶨵�
//	INT				m_nSecondActionFileIndex;	// ����������
//	LPCSTR			m_pszSecondAttachLocator;	// ���󶨵�
//	INT				m_nMoveSoundID;				// ������ƶ�����ID
//	INT				m_nCanJump;					// �Ƿ�����Ծ -1������Ծ 1����Ծ
//
//	_DBC_CHARACTER_MOUNT(VOID)
//	{
//		CleanUp();
//	}
//
//	VOID CleanUp(VOID)
//	{
//		m_nID = INVALID_ID;
//		m_pszName = NULL;
//		m_nMaxPassengerCount = 0;
//		m_nModelID = INVALID_ID;
//		m_fAddHeight = -1.f;
//		m_nRecipe = INVALID_ID;
//		m_nMainActionFileIndex = -1;
//		m_pszMainAttachLocator = NULL;
//		m_nSecondActionFileIndex = -1;
//		m_pszSecondAttachLocator = NULL;
//		m_nMoveSoundID = INVALID_ID;
//		m_nCanJump = -1;
//	}
//};

#endif // __GAMESTRUCT_BUS_H__
