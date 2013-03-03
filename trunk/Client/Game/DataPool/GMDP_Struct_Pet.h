
/** ���ݳ����ݽṹ -����
*/


#include "Type.h"
#include "DB_Struct.h"
#include "../../GameInterface/Include/GIDBC_Struct.h"

#pragma once


//-------------------------------------------
//	1) ���＼�����ݽṹ
//-------------------------------------------
struct PET_SKILL
{
	const _DBC_SKILL_DATA	*	m_pDefine;		// ���ܱ���
	INT							m_nLevel;		// ���ܵĵ�ǰ����
	INT							m_nPosIndex;	// �ڼ������� (UIͬ��)
	BOOL						m_bCanUse;		// �����Ƿ���Ч

	PET_SKILL()
	{
		m_pDefine	= NULL;
		m_nLevel	= 0;
		m_nPosIndex	= -1;
		m_bCanUse	= FALSE;
	}
	VOID CleanUp( VOID )
	{
		m_pDefine	= NULL;
		m_nLevel	= 0;
		m_nPosIndex	= -1;
		m_bCanUse	= FALSE;
	}
	PET_SKILL& operator=(const PET_SKILL& val)
	{
		m_pDefine	= val.m_pDefine;
		m_nLevel	= val.m_nLevel;
		m_nPosIndex	= val.m_nPosIndex;
		m_bCanUse	= val.m_bCanUse;
		return *this;
	}
};

//-------------------------------------------
//	�������ݽṹ
//------------------------------------------
struct SDATA_PET
{
public:	
	const _DBC_CHARACTER_MOUNT	 *	m_pModelData;	// ģ��ID����Ӧchar_mount.tab

	PET_GUID_t			m_GUID;			// ������ID
	INT					m_nBaseID;		// ����ID�� ��Ӧ _DBC_MOUNT_BASE_DATA ����

	BOOL				m_bLocked;		// ����״̬�����緱ֳ��
	
	STRING				m_szName;		// ����
	STRING				m_szNickname;	// �ǳ�
	INT					m_nGender;		// �Ա�
	INT					m_nLevel;		// �ȼ�
	INT 				m_nExp;			// ����
	INT 				m_nEraCount;	// ������
	INT 				m_nHappiness;	// ���ֶ�
	BOOL				m_bCanBeMating;	// �ɷ���
	INT					m_nSkillGrid;	// ����������

	INT					m_nAttrStr;		// ����
	INT					m_nAttrDex;		// ��
	INT					m_nAttrInt;		// ����
	INT					m_nAttrCon;		// ����

	INT					m_nUpRate;		// �ɳ���
	INT					m_nPotential;	// Ǳ��	
	INT					m_nBasic;		// ����
	INT					m_nSavvy;		// ����

	INT					m_nAttrStrApt;	// ��������
	INT					m_nAttrDexApt;	// ������
	INT					m_nAttrIntApt;  // ��������
	INT					m_nAttrConApt;	// ��������


	INT					m_nAIType;									// �Ը�
	PET_GUID_t		m_SpouseGUID;								// ��ż��GUID
	INT				m_nHP;										// Ѫ��ǰֵ
	INT				m_nHPMax;									// Ѫ���ֵ
	INT				m_nLife;									// ��ǰ����

	INT				m_nHit;										// ������
	INT				m_nMiss;									// ������
	INT				m_nCritical;								// ������

	INT				m_nAtt_Near;		//�����⹦������
	INT				m_nAtt_Far;		//Զ���⹦������

	INT				m_nAtt_MagicNear;		// �����ڹ�������
	INT				m_nAtt_MagicFar;			// Զ���ڹ�������

	INT				m_nDef_Near;	//�����⹦������
	INT				m_nDef_Far;		//Զ���⹦������

	INT				m_nDef_MagicNear;		// �����ڹ�������
	INT				m_nDef_MagicFar;			// Զ���ڹ�������


	PET_SKILL			m_Skill[PET_MAX_SKILL_COUNT];	// �����б�

public:

	SDATA_PET()
	{
		m_pModelData	= NULL;	
		m_nBaseID		= -1;
		m_bLocked		= FALSE;
		m_nGender		= 0;
		m_szName		= "";
		m_szNickname	= "";
		m_nLevel		= 0;
		m_nExp			= 0;
		m_nEraCount		= 0;
		m_nHappiness	= 0;
		m_bCanBeMating	= TRUE;
		m_nSkillGrid	= 0;

		m_nAttrStr		= 0;
		m_nAttrDex		= 0;
		m_nAttrInt		= 0;
		m_nAttrCon		= 0;

		m_nUpRate		= 0;
		m_nPotential	= 0;
		m_nBasic		= 0;
		m_nSavvy		= 0;

		m_nAttrStrApt	= 0;
		m_nAttrDexApt	= 0;
		m_nAttrIntApt	= 0;
		m_nAttrConApt	= 0;

		m_nAIType		 = -1 ;									// �Ը�
		m_nHP = 0;										// Ѫ��ǰֵ
		m_nHPMax = 0 ;									// Ѫ���ֵ
		m_nLife = 0 ;									// ��ǰ����

		m_nHit = 0;										// ������
		m_nMiss = 0;									// ������
		m_nCritical = 0;								// ������

		m_nAtt_Near = 0;		//�����⹦������
		m_nAtt_Far = 0;		//Զ���⹦������

		m_nAtt_MagicNear = 0;		// �����ڹ�������
		m_nAtt_MagicFar = 0;			// Զ���ڹ�������

		m_nDef_Near = 0;	//�����⹦������
		m_nDef_Far = 0;		//Զ���⹦������

		m_nDef_MagicNear = 0;		// �����ڹ�������
		m_nDef_MagicFar = 0;			// Զ���ڹ�������

		m_GUID.Reset();
		m_SpouseGUID.Reset();								// ��ż��GUID

		for (INT i=0; i<PET_MAX_SKILL_COUNT; ++i)
		{
			m_Skill[i].CleanUp();
		}
	}

	VOID CleanUp( VOID )
	{
		m_pModelData	= NULL;
		m_nBaseID		= -1;
		m_bLocked		= FALSE;
		m_nGender		= 0;
		m_szName		= "";
		m_szNickname	= "";
		m_nLevel		= 0;
		m_nExp			= 0;
		m_nEraCount		= 0;
		m_nHappiness	= 0;
		m_bCanBeMating	= TRUE;
		m_nSkillGrid	= 0;

		m_nAttrStr		= 0;
		m_nAttrDex		= 0;
		m_nAttrInt		= 0;
		m_nAttrCon		= 0;

		m_nUpRate		= 0;
		m_nPotential	= 0;
		m_nBasic		= 0;
		m_nSavvy		= 0;

		m_nAttrStrApt	= 0;
		m_nAttrDexApt	= 0;
		m_nAttrIntApt	= 0;
		m_nAttrConApt	= 0;


		m_nAIType		 = -1 ;						// �Ը�
		m_nHP = 0;									// Ѫ��ǰֵ
		m_nHPMax = 0 ;								// Ѫ���ֵ
		m_nLife = 0 ;									// ��ǰ����

		m_nHit = 0;									// ������
		m_nMiss = 0;									// ������
		m_nCritical = 0;								// ������

		m_nAtt_Near = 0;		//�����⹦������
		m_nAtt_Far = 0;		//Զ���⹦������

		m_nAtt_MagicNear = 0;		// �����ڹ�������
		m_nAtt_MagicFar = 0;			// Զ���ڹ�������

		m_nDef_Near = 0;	//�����⹦������
		m_nDef_Far = 0;		//Զ���⹦������

		m_nDef_MagicNear = 0;		// �����ڹ�������
		m_nDef_MagicFar = 0;			// Զ���ڹ�������

		m_GUID.Reset();
		m_SpouseGUID.Reset();								// ��ż��GUID

		for (INT i=0; i<PET_MAX_SKILL_COUNT; ++i)
		{
			m_Skill[i].CleanUp();
		}
	}
	SDATA_PET& operator=(const SDATA_PET& val)
	{
		m_pModelData	= val.m_pModelData;
		m_nBaseID		= val.m_nBaseID;
		m_bLocked		= val.m_bLocked;
		m_nGender		= val.m_nGender;
		m_szName		= val.m_szName;
		m_szNickname	= val.m_szNickname;
		m_nLevel		= val.m_nLevel;
		m_nExp			= val.m_nExp;
		m_nEraCount		= val.m_nEraCount;
		m_nHappiness	= val.m_nHappiness;
		m_bCanBeMating	= val.m_bCanBeMating;
		m_nSkillGrid	= val.m_nSkillGrid;

		m_nAttrStr		= val.m_nAttrStr;
		m_nAttrDex		= val.m_nAttrDex;
		m_nAttrInt		= val.m_nAttrInt;	
		m_nAttrCon		= val.m_nAttrCon;	

		m_nUpRate		= val.m_nUpRate;
		m_nPotential	= val.m_nPotential;
		m_nBasic		= val.m_nBasic;
		m_nSavvy		= val.m_nSavvy;

		m_nAttrStrApt	= val.m_nAttrStrApt;
		m_nAttrDexApt	= val.m_nAttrDexApt;
		m_nAttrIntApt	= val.m_nAttrIntApt;
		m_nAttrConApt	= val.m_nAttrConApt;

		m_nAIType		 = val.m_nAIType ;									// �Ը�
		m_SpouseGUID	 = val.m_SpouseGUID ;								// ��ż��GUID
		m_nHP = val.m_nHP;										// Ѫ��ǰֵ
		m_nHPMax = val.m_nHPMax ;									// Ѫ���ֵ
		m_nLife = val.m_nLife ;									// ��ǰ����

		m_nHit = val.m_nHit;										// ������
		m_nMiss = val.m_nMiss;									// ������
		m_nCritical = val.m_nCritical;							// ������

		m_nAtt_Near = val.m_nAtt_Near;		//�����⹦������
		m_nAtt_Far = val.m_nAtt_Far;			//Զ���⹦������

		m_nAtt_MagicNear = val.m_nAtt_MagicNear;		// �����ڹ�������
		m_nAtt_MagicFar = val.m_nAtt_MagicFar;				// Զ���ڹ�������

		m_nDef_Near = val.m_nDef_Near;	//�����⹦������
		m_nDef_Far = val.m_nDef_Far;			//Զ���⹦������

		m_nDef_MagicNear = val.m_nDef_MagicNear;		// �����ڹ�������
		m_nDef_MagicFar = val.m_nDef_MagicFar;			// Զ���ڹ�������

		m_GUID = val.m_GUID;

		for (INT i=0; i<PET_MAX_SKILL_COUNT; ++i)
		{
			m_Skill[i].CleanUp();
			m_Skill[i] = val.m_Skill[i];
		}
		return *this;
	}
};

