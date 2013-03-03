/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   10:16
	filename: 	MapServer\Server\Obj\Horse\HorseStruct.h
	file path:	MapServer\Server\Obj\Horse
	file base:	HorseStruct
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef HorseStruct_h__
#define HorseStruct_h__

#include "Obj_Character.h"

//////////////////////////////////////////////////////
// ��ʼ��Obj_Horse�Ľṹ
//////////////////////////////////////////////////////
struct _OBJ_HORSE_INIT :
	public _OBJ_CHARACTER_INIT
{
	GUID_t			m_GUID;			// ��ɫ�ڳ����е�GUID����PetGUID��������
	_PET_DB			m_PetDB;
	ObjID_t			m_CreatorID;	// �����ߵ�ID�������������
	INT				m_nIndex;		// �����˳�����е�����
	INT				m_RespawnTime;

	_OBJ_HORSE_INIT( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;
	}

	virtual VOID CleanUp( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;

		_OBJ_CHARACTER_INIT::CleanUp();
	}
};

//////////////////////////////////////////////////////
// ����ˢ�µ���һ�����Եı���ṹ
//////////////////////////////////////////////////////
struct _HORSE_ATTR_BACKUP
{
public:
	BOOL		m_bNameModified;
	BOOL		m_bTitleModified;

	FLOAT       m_MoveSpeed;
	_CAMP_DATA	m_CampData;						//��Ӫ		
	ObjID_t		m_nOwnerID;
	PET_GUID_t	m_SpouseGUID;					// ��żGUID
	INT			m_nAIType;						// �Ը�
	INT			m_Level;						// �ȼ�
	INT			m_iSex;							// �Ա�

	INT			m_nDataID;						// ����ID
	INT			m_nModelID;						// ����
	INT			m_nMountID;						// ����ID

	PET_GUID_t	m_GUID;							// ID
	BYTE		m_byGeneration;					// ������
	BYTE		m_byHappiness;					// ���ֶ�

	INT			m_nExp;							// ����
	//����
	INT			m_iStrengthPerception;			// ��������
	INT			m_iSmartnessPerception;			// ��������
	INT 		m_iMindPerception;				// ��������
	INT 		m_iConstitutionPerception;		// ��������
	//��������
	INT			m_iSavvy;						// ����
	INT			m_GenGu;						// ����
	INT			m_iPullulationRate;				// �ɳ���

	INT			m_nRemainPoint;					// һ������ʣ�����

	//һ��ս������
	INT			m_iStrength;					// ����
	INT			m_iSmartness;					// ����
	INT 		m_iMind;						// ����
	INT 		m_iConstitution;				// ����

	//����ս������
	INT			m_nAtt_Near;           
	INT			m_nAtt_Far;            

	INT			m_nAtt_MagicNear;		
	INT			m_nAtt_MagicFar;		

	INT			m_nDef_Near;           
	INT			m_nDef_Far;      

	INT			m_nDef_MagicNear;
	INT			m_nDef_MagicFar;

	INT			m_nHit;				
	INT			m_nMiss;
	INT			m_nCritical;

public:
	_HORSE_ATTR_BACKUP( VOID );

	VOID CleanUp( VOID );
};

#endif