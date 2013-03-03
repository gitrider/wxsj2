// GameStruct_Pet.h

#ifndef __GAMESTRUCT_PET_H__
#define __GAMESTRUCT_PET_H__

#include "Type.h"
#include "GameDefine2.h"
#include "DB_Struct.h"

////////////////////////////////////////////////
// ���ӳ������Զ���
////////////////////////////////////////////////
enum ENUM_PET_FOOD_TYPE
{
	PET_FOOD_TYPE_INVALID = 0,
	PET_FOOD_TYPE_MEAT,				//��ʳ����
	PET_FOOD_TYPE_GRASS,			//�������
	PET_FOOD_TYPE_WORM,				//�������
	PET_FOOD_TYPE_PADDY,			//�������

	PET_FOOD_TYPE_NUMBER,
};

struct PET_ATTR
{
	INT		m_Type;						// ������
	CHAR	m_Name[NPC_NAME_LEN];		// ����
	INT		m_TakeLevel;				// ��Я���ȼ�
	INT		m_bVarPet;					// �Ƿ�Ϊ�������
	INT		m_bBabyPet;					// �Ƿ�Ϊ��������
	INT		m_AI;						// �����Ը�
	INT		m_FoodType;					// ʳ����
	INT		m_PassiveSkillCount;		// �Զ�������
	INT		m_VoluntarySkill;			// �ֶ�����
	INT		m_PassiveSkill1;			// �Զ�����1
	INT		m_PassiveSkill2;			// �Զ�����2
	INT		m_PassiveSkill3;			// �Զ�����3
	INT		m_PassiveSkill4;			// �Զ�����4
	//INT		m_Life;					// ��׼����

	INT		m_iStrengthPerception;		// ��������
	INT		m_iSmartnessPerception;		// ��������
	INT 	m_iMindPerception;			// ��������
	INT 	m_iConstitutionPerception;	// ��������

	INT		m_GrowRate0;				// �ɳ���1
	INT		m_GrowRate1;				// �ɳ���2
	INT		m_GrowRate2;				// �ɳ���3
	INT		m_GrowRate3;				// �ɳ���4
	INT		m_GrowRate4;				// �ɳ���5
	INT		m_CowardiceRate;			// ��С����
	INT		m_WarinessRate;				// ��������	
	INT		m_LoyalismRate;				// �ҳϼ���
	INT		m_CanninessRate;			// ��������
	INT		m_ValourRate;				// ���ͼ���
};

////////////////////////////////////////////////
// _PET_DETAIL_ATTRIB ��������
////////////////////////////////////////////////
struct _PET_DETAIL_ATTRIB
{
public:
	_PET_DETAIL_ATTRIB(VOID);

	BOOL Init(const _PET_DB *pPetDB,
		ObjID_t idObj,
		INT nAtt_Near,
		INT nDef_Near,
		INT nAtt_Far,	
		INT nDef_Far,	
		INT nAtt_MagicNear,
		INT nAtt_MagicFar,
		INT nDef_MagicNear,
		INT nDef_MagicFar,
		INT nHit,
		INT nMiss,
		INT nCritical,
		INT nModelID,
		INT nMountID);

	VOID CleanUp(VOID);

public:
	PET_GUID_t		m_GUID;							// ID

	ObjID_t			m_ObjID;						// ����Obj���͵�ObjID
	INT				m_nDataID;						// ģ��ID,��������
	CHAR			m_szName[MAX_CHARACTER_NAME];	// ����
	INT				m_nAIType;						// �Ը�
	PET_GUID_t		m_SpouseGUID;					// ��ż��GUID
	INT				m_nLevel;						// �ȼ�
	INT				m_nExp;							// ����
	INT				m_nHP;							// Ѫ��ǰֵ
	INT				m_nHPMax;						// Ѫ���ֵ

	INT				m_nLife;						// ��ǰ����
	BYTE			m_byGeneration;					// ������
	BYTE			m_byHappiness;					// ���ֶ�

	INT m_nAtt_Near;
	INT m_nDef_Near;
	INT m_nAtt_Far;	
	INT m_nDef_Far;	

	INT				m_nAtt_MagicNear;				// �����ڹ�������
	INT				m_nAtt_MagicFar;				// �����⹦������
	INT				m_nDef_MagicNear;				// �����ڹ�������
	INT				m_nDef_MagicFar;				// �����⹦������

	INT				m_nHit;							// ������
	INT				m_nMiss;						// ������
	INT				m_nCritical;					// ������

	INT				m_nModelID;						// ����
	INT				m_nMountID;						// ����ID

	INT				m_iStrengthPerception;			// ��������
	INT				m_iSmartnessPerception;			// ��������
	INT 			m_iMindPerception;				// ��������
	INT 			m_iConstitutionPerception;		// ��������

	INT				m_Str;							// ����
	INT				m_Con;							// ����
	INT 			m_Dex;							// ��
	//INT				m_Spr;						// ����
	INT 			m_Int;							// ����
	INT 			m_GenGu;						// ����

	INT				m_nRemainPoint;					// Ǳ�ܵ�

	_OWN_SKILL		m_aSkill[MAX_PET_SKILL_COUNT];	// �����б�
};


////////////////////////////////////////////////
// _PET_PLACARD_ITEM ���﹫����һ��
////////////////////////////////////////////////

// �ͻ��˳��﹫������ʾ����
#define MAX_PETPLACARD_LIST_ITEM_NUM		(2)

#define PET_PLACARD_ITEM_MESSAGE_SIZE		(64)

struct _PET_PLACARD_ITEM
{
public:

	_PET_PLACARD_ITEM(VOID);
	~_PET_PLACARD_ITEM(VOID);

	BOOL Init(	GUID_t HumanGUID,
				const CHAR *pszHumanName,
				INT nHumanLevel,
				const CHAR *pszHumanGuildName,
				INT nHumanMenPai,
				const _PET_DETAIL_ATTRIB *pPetAttr,
				const CHAR *pszMessage,
				UINT uCreateTime);

	VOID CleanUp(VOID);

	BOOL IsInvalid(VOID)const
	{
		return (m_HumanGUID == INVALID_ID)?(TRUE):(FALSE);
	}

public:
	UINT GetCreateTime(VOID)const
	{
		return m_uCreateTime;
	}

	GUID_t GetHumanGUID(VOID)const
	{
		return m_HumanGUID;
	}

	const CHAR *GetHumanName(VOID)const
	{
		return m_szHumanName;
	}

	INT GetHumanLevel(VOID)const
	{
		return m_nHumanLevel;
	}

	const CHAR *GetHumanGuildName(VOID)const
	{
		return m_szHumanGuildName;
	}

	const _PET_DETAIL_ATTRIB *GetPetAttr(VOID)const
	{
		return &m_PetAttr;
	}

	const CHAR *GetMessage(VOID)const
	{
		return m_szMessage;
	}

	const INT GetHumanMenPai(VOID) const
	{
		return m_nHumanMenPai;
	}

protected:
	// ������Ϣ
	UINT				m_uCreateTime;

	// ������Ϣ
	GUID_t				m_HumanGUID;
	CHAR				m_szHumanName[MAX_CHARACTER_NAME];
	INT					m_nHumanLevel;
	CHAR				m_szHumanGuildName[MAX_GUILD_NAME_SIZE];
	INT					m_nHumanMenPai;

	// ������Ϣ
	_PET_DETAIL_ATTRIB	m_PetAttr;

	// ����
	CHAR				m_szMessage[PET_PLACARD_ITEM_MESSAGE_SIZE];
};

const INT MaxSlotSkillNum		= 64;
const INT MaxSlotNum			= 6;
const INT MaxReturnToChildIDNum	= 8;
const INT MaxHorseDropNum		= MaxReturnToChildIDNum;
const INT MaxHorseDropGroupNum	= 10;

struct _HORSE_SKILL_SLOT
{
	INT m_aSkillSlot[MaxSlotSkillNum];		//���ܲۿ�ѡ�ļ���ID
	INT m_iSkillCount;						//�˼��ܲ�ʵ�ʿ�ѡ�ļ���ID
};

struct _HORSE_SKILL
{
	INT m_iSkillRealizationID;					//��������ID
	INT m_iSkillSlotCount;						//���ܲ����������Ϊ6
	_HORSE_SKILL_SLOT m_oSkillSlot[MaxSlotNum];	//���ܲ�1��ѡ�ļ���ID
};

//������
struct _HORSE_ATTR
{
	INT			m_nDataID;												// ����ID
	CHAR		m_sName[64];											// ����
	INT			m_iSex;													// �Ա�
	INT			m_iLevel;												// ��ʼ�ȼ�
	INT			m_iTakeLevel;											// ��Я���ȼ�
	INT			m_aReturnToChildID[MaxReturnToChildIDNum];				// ��ͯ���ID
	INT			m_iReturnToChildIDCount;								// ��ͯ���ѡID��ʵ�ʸ���
	INT			m_aChildID[MaxReturnToChildIDNum];						// �����2����ID,��ӦQiChengJichu.tab�е�ID
	INT			m_iChildIDCount;										// �����2����ѡID�ĸ���
	INT			m_bCanMating;											// �Ƿ���Խ���
	BOOL		m_bVariation;											// �Ƿ����
	BYTE		m_byGeneration;											// ��ֳ����
	BYTE		m_iAttach;												// ��˵Ĺ���
	INT			m_LifeNoMaster;											// ����ʱ����������
	INT			m_iStrengthMin;											// ��ʼ��С����
	INT			m_iStrengthMax;											// ��ʼ�������
	INT			m_iSmartnessMin;										// ��ʼ��С����
	INT			m_iSmartnessMax;										// ��ʼ�������
	INT 		m_iMindMin;												// ��ʼ��С����
	INT 		m_iMindMax;												// ��ʼ�������
	INT 		m_iConstitutionMin;										// ��ʼ��С����
	INT 		m_iConstitutionMax;										// ��ʼ�������
	INT			m_iStrengthMinPer;										// ��ʼ��С��������
	INT			m_iStrengthMaxPer;										// ��ʼ�����������
	INT			m_iSmartnessMinPer;										// ��ʼ��С��������
	INT			m_iSmartnessMaxPer;										// ��ʼ�����������
	INT 		m_iMindMinPer;											// ��ʼ��С��������	
	INT 		m_iMindMaxPer;											// ��ʼ�����������
	INT 		m_iConstitutionMinPer;									// ��ʼ��С��������	
	INT 		m_iConstitutionMaxPer;									// ��ʼ�����������	
	INT			m_iSavvyMin;											// ��ʼ��С����	
	INT			m_iSavvyMax;											// ��ʼ�������	
	INT			m_GenGuMin;												// ��ʼ��С����	
	INT			m_GenGuMax;												// ��ʼ������	
	INT			m_iPullulationRateMin;									// ��ʼ��С�ɳ���	
	INT			m_iPullulationRateMax;									// ��ʼ���ɳ���
	INT			m_iMoveSpeed;											// �ƶ��ٶ�
	INT			m_iBehaveID;											// ��Ӧ����ID
	INT			m_iMountID;												// ��Ӧ��������е�����
	

	INT			m_iSkillSlotCount;										//���ʼ���ܸ��������Ϊ6
	_HORSE_SKILL_SLOT m_oSkillSlot[MaxSlotNum];							//���ʼ�����б�

	INT			m_iRealizationID;										// ��������ID
};

struct _HORSE_ITEM
{
	//����ID
	INT	m_iItemID;								

	//��������
	//���߷ֱ��Ӧ�Ų�ͬ��ʹ������,��ʾ�ĺ�����ֵ�����õĶ���ͬ.
	//����1��ʾ��������������Եĵ���,2��ʾ������˸��ǵĵ��� 3��
	//ʾ�������ӿ��ֶ�4��ʾ���ӿ�ʹ�ü��ܸ�����5��ʾ��Ӧλ�õ�ϴȥ
	//���ܸ��ӵ�Ц����,-1��ʾ������6��ʾ�����ͼ��ܵ���, ��������
	//��7��ʾ��չλ�õļ�����8��ʾ��ͯ��
	INT m_iItemType;							

	//�ı��Ӧ����˵����Եİٷֱ���ֵ
	//��Ӧ1���͵���ֵ��д��ʾ�ĺ����ǰ��հٷֱ�������˵�����,��Ӧ
	//2��ʾ�ٷֱ����Ӹ���,3��ʾ�ٷֱ����ӿ��,4,5,6,7��8������ֵ��
	//����
	INT m_iAttrChangePercent;

	//�ı��Ӧ����˵����Եĵ�����ֵ
	//��Ӧ1���͵���ֵ��д��ʾ�ĺ����ǰ�����ֵ������˵�����,��Ӧ2��
	//ʾ������ֵ���Ӹ���,3��ʾ������ֵ���ӿ��ֶ�, 4��ʾ��չ���ӵ���
	//Ŀ,5��ʾ��Ӧϴȥ�ĸ���λ��,8��ʾ����˵ĳɳ���6,7����ֵ������
	INT m_iAttrChangePoint;

	//���������ѧϰ��Ӧ����ID
	//������������д6��7ʱ��,��д��ID�����Ӧ�ļ���, ��Ӧ���ܱ���
	//ID����,���ճɹ�����Ӽ���.
	INT m_iStudySkillID;
	//���������ѧϰ�ĳɹ���
	INT m_iSuccessRate;
};

struct _HORSE_DROP_GROUP
{
	INT m_iGroupImportance;										//�������ļ�ֵ������������ʱ������
	INT m_iGroupMaxDropTimes;									//���������ĵ������
	INT m_aGroupHorseID[MaxHorseDropNum];						//��ID�б����ѡȡһ������Ӧ��������е�ID
	INT m_iHorseIDCount;										//m_aGroupHorseID����Ч��ֵ�ĸ���
};

struct _HORSE_DROP
{
	INT m_iDropID;												//�����ID,�������������ID
	INT m_iMonsterImportance;									//����ļ�ֵ���������ʱ����ĸ
	INT m_iMaxDropNum;											//���������������
	_HORSE_DROP_GROUP m_aDropGroup[MaxHorseDropGroupNum];		//�����������
};


//�ٻ��޳�ʼ������
struct _MONSTER_PET_ATTR
{
	INT iLifeTime;
	INT iImpactToSelf;
	INT iImpactToOnwer;
	INT aSkillID[MONSTER_PET_MAX_SKILL];
};

#endif // __GAMESTRUCT_PET_H__
