#pragma once

//========================================================
//
//			��ɫ��������ȫ�ɷ�������������������
//
//========================================================
#include "GameDefine.h"
#include "GameDefine2.h"
#include "TDDBC_Struct.h"
#include "GameStruct_Skill.h"

//=================================================================
//-->0. Character ��������
//=================================================================
struct SDATA_CHARACTER
{
	INT				m_nRaceID;					// ���ݱ��е�ID						

	STRING			m_strName;					// ��ɫ�����֣��ӷ���������			
	STRING			m_strTitle;					//��ǰ�ƺ�
	FLOAT			m_fHPPercent;				// ����ֵ�ٷֱ�						
	FLOAT			m_fMPPercent;				// ħ��ֵ�ٷֱ�						
	FLOAT			m_fMoveSpeed;				// �ƶ����ٶ�						
	INT				m_nCampID;					// ��ӪID							
	INT				m_nLevel;					// �ȼ�								
	BOOL			m_bFightState;				// ս��״̬							
};

//=================================================================
//-->1. Character_NPC ����NPC
//=================================================================
struct SDATA_NPC : public SDATA_CHARACTER
{

};

//=================================================================
//-->2. Character_PlayerOther �����������
//=================================================================
struct _TITLE_
{
	enum
	{
		INVALID_TITLE = 0,
		ID_TITLE,
		STRING_TITLE,
	};
	BOOL		bFlag;							//������title�Ƿ���id
	BYTE		bType;							//������title������
	union
	{
		INT		ID;								//�����id,������Ǳ���id
		CHAR	szTitleData[MAX_CHARACTER_TITLE];	//�������id���������title����
	};
	_TITLE_()
	{
		bFlag		=	INVALID_TITLE;
		bType		=	_TITLE::NO_TITLE;	
		memset(szTitleData, 0, MAX_CHARACTER_TITLE);
	}
};
struct SDATA_PLAYER_OTHER : public SDATA_NPC
{
	INT				m_nMenPai;			// ����ID									
										/*
										|	0������
										|	1������
										|	2��ؤ��
										|	3���䵱
										|	4������
										|	5������
										|	6������
										|	7����ɽ
										|	8����ң
										|	9��������
										*/

	//ͷ������
	INT			m_nHairMeshID;			//-> DBC_CHARACTER_HAIR_GEO					
	INT			m_nHairMaterialID;		//-> DBC_CHARACTER_HAIR_MAT					

	//��������
	INT			m_nFaceMeshID;			//-> DBC_CHARACTER_HEAD_GEO					
	INT			m_nFaceMaterialID;		//-> DBC_CHARACTER_HEAD_MAT					

	INT			m_nEquipVer;//��ɫ��װ��״̬�汾�ţ����ںͷ�����ͬ�����ݵ�����		
	INT			m_nEquipmentID[HEQUIP_NUMBER];	//װ����,����������ʾ				

	BOOL		m_bTeamFlag; // �Ƿ��ж���
	BOOL		m_bTeamLeaderFlag; // �Ƿ��Ƕӳ�
	BOOL		m_bTeamFullFlag; // �Ƿ�������
	
	INT			m_nTitleNum;
	INT			m_nCurTitleIndex;
	_TITLE_		m_nTitleList[MAX_TITLE_SIZE];		//�ƺű�							
};


//=================================================================
//-->3. Character_PlayerMySelf ��������Լ�
//=================================================================

/*
	0)�ķ����ݽṹ
*/
struct SCLIENT_XINFA
{
	const _DBC_SKILL_XINFA*		m_pDefine;		//�ķ�����
	INT							m_nPosIndex;	//�ڼ����ķ� (UIͬ��)
	BOOL						m_bLeaned;		//�Ƿ��Ѿ�ѧ��
	INT							m_nLevel;		//�ķ��ȼ�
};
typedef std::map< INT, SCLIENT_XINFA >			SXINFA_MAP;

/*
	1) �������ݽṹ
*/
struct SCLIENT_SKILL
{
	const _DBC_SKILL_DATA		*m_pDefine;		//���ܱ���
	INT							m_nPosIndex;	//�ڼ������� (UIͬ��)
	BOOL						m_bLeaned;		//�Ƿ��Ѿ�ѧ��
	Time_t						m_nActiveTime;  //����ʱ��
};
typedef std::map< INT, SCLIENT_SKILL >			SSKILL_MAP;


/*
	2) �ͻ�����������ݽṹ
*/
struct SCLIENT_LIFEABILITY
{
	const _DBC_LIFEABILITY_DEFINE *	m_pDefine;		//����ܱ��еĶ���
	INT								m_nPosIndex;	// �ڼ������� (UIͬ��)
	INT								m_nLevel;		// ���ܵȼ�
	INT								m_nExp;			// ����������
};
typedef std::map< INT, SCLIENT_LIFEABILITY >	SLIFEABILITY_MAP;

/*
	3) �ͻ����䷽���ݽṹ
*/
struct SCLIENT_PRESCR
{
	const _DBC_LIFEABILITY_ITEMCOMPOSE*		m_pDefine;		//���䷽���еĶ���
};
typedef std::map< INT, SCLIENT_PRESCR >			SPRESCR_MAP;

class tObject_Item;

struct SDATA_PLAYER_MYSELF : public SDATA_PLAYER_OTHER
{
	//-----------------------------------------------------
	INT				m_nHP;				// ������						
	INT				m_nMP;				// ħ����						
	INT				m_nExp;				// ����ֵ						
	//INT			m_nMaxExp;			// ����ֵ����					
	INT				m_nMoney;			// ��Ϸ����						

	//-----------------------------------------------------
	//һ��ս������
	INT				m_nSTR;				// �⹦							
	INT				m_nSPR;				// �ڹ�							
	INT				m_nCON;				// ��							
	INT				m_nINT;				// ����							
	INT				m_nDEX;				// �ǻ�							
	INT				m_nPoint_Remain;	// ʣ����������				

	//-----------------------------------------------------
	//����ս������
	INT				m_nAtt_Physics;		// ��������					
	INT				m_nAtt_Magic;		// ħ��������					
	INT				m_nDef_Physics;		// ���������					
	INT				m_nDef_Magic;		// ħ��������					
	INT				m_nMaxHP;			// ���������					
	INT				m_nMaxMP;			// ���ħ����					
	INT				m_nMaxPneuma;		// ���Ԫ����
	INT				m_nHP_ReSpeed;		// HP�ָ��ٶ�  ��/��			
	INT				m_nMP_ReSpeed;		// MP�ָ��ٶ�  ��/��			
	INT				m_nHit;				// ������						
	INT				m_nMiss;			// ������						
	INT				m_nCritRate;		// ������						
	//FLOAT			m_fMoveSpeed;		// �ƶ��ٶ�(�ڻ���������)					
	INT				m_nAttackSpeed;		// �����ٶ�						
	INT				m_nAtt_Cold;		// ������						
	INT				m_nDef_Cold;		// ������						
	INT				m_nAtt_Fire;		// �𹥻�						
	INT				m_nDef_Fire;		// �����						
	INT				m_nAtt_Light;		// �繥��						
	INT				m_nDef_Light;		// �����						
	INT				m_nAtt_Posion;		// ������						
	INT				m_nDef_Posion;		// ������						

	//-----------------------------------------------------
	//��������
	SXINFA_MAP			m_theXinFa;			//�ķ�
	SSKILL_MAP			m_theSkill;			//��������
	SLIFEABILITY_MAP	m_theLifeAbility;	//���������
	SPRESCR_MAP			m_theSprescr;		//�䷽����
};
