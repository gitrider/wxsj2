
/** ���ݳ����ݽṹ -��ɫ����
 *	
 *	���ܣ�	
 *
 *	�޸ļ�¼��
 *			080325	�޸� SDATA_PLAYER_OTHER �гƺű������ 20 -> 256
 *			
 *			
 */

#pragma once


//========================================================
//
//			��ɫ��������ȫ�ɷ�������������������
//
//========================================================

#include "GameDefine.h"
#include "GameDefine2.h"
#include "GIDBC_Struct.h"
#include "DB_Struct.h"
#include "GameStruct_Skill.h"

#include "GMDP_Struct_Skill.h"
#include "GMDP_Struct_Title.h"
#include "GMDP_Struct_Pet.h"

//=================================================================
//-->0. Character ��������
//=================================================================
struct SDATA_CHARACTER
{
	INT				m_nRaceID;					// ���ݱ��е�ID						

	INT				m_nPortraitID;				// ͷ��ID		
	STRING			m_strName;					// ��ɫ�����֣��ӷ���������	
	STRING			m_strUtf8Name;				// ��ɫ��Utf8 ����
	STRING			m_strCountryTitle;			// ��ǰ���ҳƺ�
	STRING			m_strGuildTitle;			// ��ǰ���ƺ�
	STRING			m_strTitle;					// ��ǰ�淨�ƺ�
	BYTE			m_TitleType;				// ��ǰ�ƺ�����
	FLOAT			m_fHPPercent;				// ����ֵ�ٷֱ�						
	FLOAT			m_fMPPercent;				// ħ��ֵ�ٷֱ�
	FLOAT			m_fPenumaPercent;			//Ԫ���ٷֱ�
	INT				m_nRage;					// ŭ��
	FLOAT			m_fMoveSpeed;				// �ƶ����ٶ�						
	_CAMP_DATA		m_CampData;					// ��ӪID	
	INT				m_nOwnerID;					// ������ID
	GUID_t			m_OccupantGUID;				// ռ����(��˭��ģ�������˭��)
	INT				m_nRelative;				// ��Թ�ϵ��������Ƿ�ɱ��������Ƿ���������
	INT				m_nModelID;					// ģ��ID
	INT				m_nMountID;					// ��ʹ�õ�����ID
	INT				m_nMountIndex;				// ��ʹ�õ�������MountContainer��λ������            [7/23/2010 �¾���]
	INT				m_nLevel;					// �ȼ�								
	BOOL			m_bFightState;				// ս��״̬							
	INT				m_nStealthLevel;			// ������
	BOOL			m_bSit;						// ����״̬
	INT				m_nAIType;					// AI����
    INT             m_nCountry;                 // ����
	INT				m_nMountEquipID[HEQUIP_HORSE_NUMBER];		// ����ID
	INT				m_nMountList[MAX_MOUNT_SIZE];// ӵ�е�����ID                                     [7/14/2010 �¾���]
};

//=================================================================
//-->1. Character_NPC ����NPC
//=================================================================
struct SDATA_NPC : public SDATA_CHARACTER
{
	INT				m_nWeaponID;				// ����ID
};

//=================================================================
//-->2. Character_PlayerOther �����������
//=================================================================


struct SDATA_PLAYER_OTHER : public SDATA_NPC
{
	INT				m_nProfession;							// ְҵ//8������ÿ����2����չ����,����������16����ְҵ(������)����
																/*
																|	0��������-����ɮ
																|	1��������-���޺�
																|	2��ؤ��-��ؤ��" 
																|	3��ؤ��-��ؤ��" 
																|	4���䵱��-���䵱
																|	5���䵱��-���䵱
																|	6������-����" );
																|   7������-������" 
																|	8����ң��-����ң
																|	9����ң��-ָ��ң
																|	10��ҩ����-��ҩ��
																|	11��ҩ����-ҽҩ��
																|	12���һ���-���һ�
																|	13���һ���-���һ�
																|	14��������-�̶���
																|   15��������-������
																*/
	
	INT				m_nHairMeshID;							// ͷ������	-> DBC_CHARACTER_HAIR_GEO					
	INT				m_nFaceMeshID;							// ��������	-> DBC_CHARACTER_HEAD_GEO( char_head.tab )
	UINT			m_uHairColor;							// ͷ����ɫ
	INT				m_nEquipVer;							// ��ɫ��װ��״̬�汾�ţ����ںͷ�����ͬ�����ݵ�����		
	INT				m_nEquipmentID[HEQUIP_NUMBER];			// װ����;����������ʾ				
	INT				m_nEquipmentGemID[HEQUIP_NUMBER];		// װ����ʯ��;����������ʾ	
	_SUIT_SETTING	m_oChangeSuit[MAX_EQUIP_SUIT_NUM];		// һ����װ����

	BOOL			m_bTeamFlag;							// �Ƿ��ж���
	BOOL			m_bTeamLeaderFlag;						// �Ƿ��Ƕӳ�
	BOOL			m_bTeamFullFlag;						// �Ƿ�������
	BOOL			m_bTeamFollowFlag;						// �Ƿ���Ӹ���
	INT				m_nTeamFollowListCount;					// �����б��������
	ObjID_t			m_aTeamFollowList[MAX_TEAM_MEMBER];		// �����б�
	
	INT				m_nTitleNum;							// �ƺ�����
	INT				m_nCurTitleIndex;						// ��ǰʹ�õĳƺ�����
	_TITLE_			m_nTitleList[MAX_TITLE_SIZE];			// �ƺű�

	BOOL			m_bIsInStall;							// �Ƿ��̯��
	STRING			m_strStallName;							// ̯λ��	
	BOOL			m_bIsOpenPublic;						// ̯λ�Ƿ���(������ʾ��̯�Ƹ��������)
	GUID_t		m_nGuid;						//�Լ���GUID
	ObjID_t		m_BusObjID;
};


//=================================================================
//-->3. Character_PlayerMySelf ��������Լ�
//=================================================================

class tObject_Item;
struct SDATA_PLAYER_MYSELF : public SDATA_PLAYER_OTHER
{
	//-----------------------------------------------------
	INT				m_nHP;				// ������						
	INT				m_nMP;				// ħ����
	INT				m_nPneuma;			// Ԫ��
	INT				m_nExp;				// ����ֵ						
	//INT			m_nMaxExp;			// ����ֵ����(���ݵȼ������)					
	INT				m_nMoney;			// ��Ϸ����	
	INT				m_nStrikePoint;		// ������
	INT				m_nVigor;			// ����ֵ
	INT				m_nMaxVigor;		// ����ֵ����
	INT				m_nEnergy;			// ����ֵ
	INT				m_nMaxEnergy;		// ����ֵ����
	INT				m_nGoodBadValue;	// �ƶ�ֵ
	PET_GUID_t		m_guidCurrentPet;	// ��ǰ����GUID

	//-----------------------------------------------------
	INT				m_nGuild;			// ����ID 

	// ----------------------------------------------------
	// �Ѿ������ϵ�����
	INT				m_nSeriesPoint_1;		// ϵ�� 1
	INT				m_nSeriesPoint_2;		// ϵ�� 2
	INT				m_nSeriesPoint_3;		// ϵ�� 3

	INT				m_nSkillPoint;

	//-----------------------------------------------------
	//һ��ս������
	INT				m_nSTR;				// �⹦							
//	INT				m_nSPR;				// �ڹ�							
	INT				m_nCON;				// ��							
	INT				m_nINT;				// ����							
	INT				m_nDEX;				// �ǻ�							
	INT				m_nPoint_Remain;	// ʣ����������				

	//-----------------------------------------------------
	//����ս������
//	INT				m_nAtt_Physics;			// ��������
//	INT				m_nDef_Physics;			// ���������
	INT				m_nAtt_Near;			// ����
	INT				m_nDef_Near;			// ����
	INT				m_nAtt_Far;				// Զ��
	INT				m_nDef_Far;				// Զ��

	INT				m_nAtt_Magic_Near;		// �����ڹ�������										
	INT				m_nDef_Magic_Near;		// �����ڹ�������			
	INT				m_nAtt_Magic_Far;		// Զ���ڹ�������										
	INT				m_nDef_Magic_Far;		// Զ���ڹ�������	
	INT				m_nMaxHP;				// ���������					
	INT				m_nMaxMP;				// ���ħ����
	INT				m_nMaxPneuma;			// ���Ԫ����
	INT				m_nMaxRage;				// ���ŭ����
	INT				m_nHP_ReSpeed;			// HP�ָ��ٶ�  ��/��			
	INT				m_nMP_ReSpeed;			// MP�ָ��ٶ�  ��/��	
	INT				m_nToughness;			// ����
	INT				m_nHit;					// ������						
	INT				m_nMiss;				// ������						
	INT				m_nCritRate;			// ������						
	//FLOAT			m_fMoveSpeed;			// �ƶ��ٶ�(�ڻ���������)					
	INT				m_nAttackSpeed;			// �����ٶ�						
	INT				m_nAtt_Gold;			// �𹥻�						
	INT				m_nDef_Gold;			// �����						
	INT				m_nAtt_Fire;			// �𹥻�
	INT				m_nDef_Fire;			// �����
	INT				m_nAtt_Wood;			// ľ����
	INT				m_nDef_Wood;			// ľ����
	INT				m_nAtt_Water;			// ˮ����
	INT				m_nDef_Water;			// ˮ����
	INT				m_nAtt_Soil;			// ������
	INT				m_nDef_Soil;			// ������

	BOOL			m_bLimitMove;			//�Ƿ����Ʋ����ƶ�
	BOOL			m_bCanActionFlag1;		//�������ޱ��1,���ڻ��Դ���
	BOOL			m_bCanActionFlag2;		//�������ޱ��2,���ڳ�Ĭ
	//BOOL			m_bLimitUseSkill;		//�Ƿ����Ʋ���ʩ��
	//BOOL			m_bLimitHandle;			//�Ƿ����Ʋ��ܽ���һ�в���

	BOOL			m_bIsMinorPwdSetup;		// �Ƿ������ö�������
	BOOL			m_bIsMinorPwdUnlocked;	// �Ƿ��Ѿ�������������

	//-----------------------------------------------------
	//��������
//$$	SXINFA_MAP			m_theXinFa;			//�ķ�
	SSKILL_MAP			m_theSkill;			//��������
	SLIFEABILITY_MAP	m_theLifeAbility;	//���������
	SPRESCR_MAP			m_theSprescr;		//�䷽����
	INT					m_nDefaultSkillID;	//Ĭ�ϵ��Զ�����ID

	//��������
//	SDATA_PET			m_thePet;			//����
};
