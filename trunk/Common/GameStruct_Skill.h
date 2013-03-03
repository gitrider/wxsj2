// GameStruct_Skill.h

#ifndef __GAMESTRUCT_SKILL_H__
#define __GAMESTRUCT_SKILL_H__

#include "Type.h"
#include "GameDefine2.h"
#include "GameStruct_Impact.h"
/////////////////////////////////////////////////////////////////////////////////
// �ͻ�������������õ�DBC�ṹ

// �����˺��ṹ
struct _DBC_DIRECT_IMPACT
{
	UINT				m_uID;					// ID
	CHAR				*m_pszEffect;			// ��ЧID
	CHAR				*m_pszSound;			// ��ЧID
	CHAR				*m_pszEffectLocator;	// ��Ч�󶨵�
	CHAR				*m_pszReserved1;		// Ԥ��1
	CHAR				*m_pszReserved2;		// Ԥ��2
	CHAR				*m_pszInfo;				// Ч������
};
typedef _DBC_DIRECT_IMPACT	SDirectImpact;
typedef _DBC_DIRECT_IMPACT	_DIRECT_IMPACT;


// ����BUFF�ṹ
struct _DBC_BUFF_IMPACT
{
	UINT				m_uID;						// ID
	UINT				m_uMutexID;					// ������
	INT					m_nPri;						// ���ȼ�����
	UINT				m_uMaxLayerNum;				// ����ۻ�����
	LPCSTR				m_szIconName;				// ͼ����ļ���
	LPCSTR				m_lpszEffect_Active;		// ������ЧID
	LPCSTR				m_lpszSound_Active;			// ������Ч����ЧID
	LPCSTR				m_lpszBind_Active;			// ������Ч�İ󶨵�
	LPCSTR				m_lpszEffect_Continuous;	// ������ЧID
	LPCSTR				m_lpszSound_Continuous;		// ������Ч����ЧID
	LPCSTR				m_lpszBind_Continuous;		// ������Ч�İ󶨵�
	BOOL				m_bStillOnWhenOwnerDead;	// ���������Ƿ���
	BOOL				m_bCanBeDispeled;			// �Ƿ���Ա���ɢ
	BOOL				m_bHostileFlag;				// �Ƿ��Ǹ���Ч��
	BOOL				m_bCanBeManualCancel;		// �Ƿ���Ա��ֶ�ȡ��
	BOOL				m_bRotateEffect;			// �Ƿ�����Ч����ת����
	BOOL				m_bLineEffect;				// �Ƿ�Ϊ������Ч
	CHAR				*m_pszCreatorLocator;		// ������Ч��Ŀ��󶨵�
	CHAR				*m_pszInfo;					// Ч������
};
typedef _DBC_BUFF_IMPACT SBuffImpact;
typedef _DBC_BUFF_IMPACT _BUFF_IMPACT;

// �ӵ��켣����
enum ENUM_BULLET_CONTRAIL_TYPE
{
	BULLET_CONTRAIL_TYPE_INVALID = -1,
	BULLET_CONTRAIL_TYPE_BEELINE,			// ֱ��
	BULLET_CONTRAIL_TYPE_PARABOLA,			// ������
	BULLET_CONTRAIL_TYPE_NONE,				// �޹켣��ֱ�ӱ�ը
	BULLET_CONTRAIL_TYPE_ROTATE,			// ��ת
	BULLET_CONTRAIL_TYPE_EXP,

	BULLET_CONTRAIL_TYPE_NUMBERS
};

// �ӵ�
struct _DBC_BULLET_DATA
{
	INT				m_nID;					// ID
	CHAR const* 	m_szDesc;				// ˵��������
	CHAR const* 	szKeyword;				// �ؼ���
	CHAR const* 	m_szBulletSystemName;	// �ӵ�ģ����
	CHAR const* 	m_szReserved1;			// Ԥ��1
	CHAR const* 	m_szReserved2;			// Ԥ��2

	INT				m_bIsLogicBullet;		// �Ƿ�ʹ�ÿͻ����߼��ӵ�
	INT				m_nContrailType;		// �켣���� ENUM_BULLET_CONTRAIL_TYPE
	INT				m_nContrailParam;		// �켣����(��������Ϊ���������߶�)
	CHAR const*		m_szFlyEffect;			// ������Ч
	CHAR const*		m_szFlySound;			// ������Ч
	FLOAT			m_fSpeed;				// �ٶ�(m/s)
	FLOAT			m_fAcceleration;		// ���ٶ�
	CHAR const*		m_szHitEffect;			// ������Ч
	CHAR const* 	m_szHitSound;			// ������Ч
	CHAR const*		m_szHitEffectLocator;	// ������Ч�İ󶨵�	
	
	
};
typedef _DBC_BULLET_DATA SSkillObjData;
typedef _DBC_BULLET_DATA _SKILLOBJ_DATA;

enum ENUM_SKILL_TYPE
{
	SKILL_TYPE_INVALID	= -1,
	SKILL_TYPE_GATHER,
	SKILL_TYPE_LEAD,
	SKILL_TYPE_LAUNCH,
	SKILL_TYPE_PASSIVE,

	SKILL_TYPE_NUMBERS
};

enum ENUM_TRANSFER_SKILL
{
	TRANSFER_INVALID	= -1,
	TRANSFER_TWINKLING,			// ˲��
	TRANSFER_TANTIVY,			// ���
};

enum ENUM_SELECT_TYPE
{
	SELECT_TYPE_INVALID	= -1,
	SELECT_TYPE_NONE,			// ����ѡ��
	SELECT_TYPE_CHARACTER,		// ��ɫ
	SELECT_TYPE_POS,			// λ��
	SELECT_TYPE_DIR,			// ����
	SELECT_TYPE_SELF,			// ���Լ����в���
	SELECT_TYPE_HUMAN_GUID,		// ���
	SELECT_TYPE_NUMBERS
};
enum ENUM_TARGET_LOGIC
{
	TARGET_LOGIC_INVALID	= -1,
	TARGET_SELF,					// ֻ���Լ���Ч
	TARGET_MY_PET,					// ֻ���Լ��ĳ�����Ч
	TARGET_MY_SHADOW_GUARD,			// ֻ���Լ��ķ�����Ч
	TARGET_MY_MASTER,				// ֻ���Լ���������Ч������ר��
	TARGET_AE_AROUND_SELF,			// ���Լ�Ϊ���ģ���Χ��Ч
	TARGET_SPECIFIC_UNIT,			// ��׼�Ķ�����Ч
	TARGET_AE_AROUND_UNIT,			// ����׼�Ķ���Ϊ���ģ���Χ��Ч
	TARGET_AE_AROUND_POSITION,		// ����׼��λ�õ�Ϊ���ģ���Χ��Ч
	TARGET_POS,						// ����׼λ����Ч

	TARGET_LOGIC_NUMBERS			// �߼�����
};

enum ENUM_BEHAVIOR_TYPE
{
	BEHAVIOR_TYPE_HOSTILITY = -1, //�ж���Ϊ
	BEHAVIOR_TYPE_NEUTRALITY = 0, //������Ϊ
	BEHAVIOR_TYPE_AMITY = 1, //�Ѻ���Ϊ
};
// ��ʽ����
enum ENUM_SKILL_ACTION_TYPE
{
	SKILL_ACTION_TYPE_INVALID	= -1,
	SKILL_ACTION_TYPE_NONE,					// ��ͨ��ʽ(�������)
	SKILL_ACTION_TYPE_CONCATENATION_EX,		// ����(1,2,2,2,2,3)
	SKILL_ACTION_TYPE_CONCATENATION,		// ����(1,2,3,1,2,3)

	SKILL_ACTION_TYPE_NUMBERS
};

#define MAX_SKILL_FRIEND_TMPACT		(2)
#define MAX_SKILL_ENEMY_TMPACT		(2)

// ���ܵ�ѧϰ���� -- ��Ӧ��
struct _DBC_SKILL_STUDY_DATA
{
	INT				m_nSkillID;					// ����ID
	const CHAR*		m_szLevelQua;				// ���� ����
	INT				m_nVocationQua;				// ְҵ ����
	const CHAR*		m_szSeriesSkillPointQua;	// ��ϵ���ܵ��ܹ����� ����
	INT				m_nFatherSkillIDQua;		// ������ ����
	const CHAR*		m_szFatherSkillLevelQua;	// �����ܵļ��� ����
	const CHAR*		m_szSkillPointQua;			// ӵ�еļ��ܵ��� ����
	const CHAR*		m_szMoneyQua;				// ��Ǯ ����
	const CHAR*		m_szExpQua;					// ���� ����
	const CHAR*		m_szItem1Qua;				// ��Ʒ1
	const CHAR*		m_szItem1NumQua;			// ��Ʒ1 ����
	const CHAR*		m_szItem2Qua;				// ��Ʒ2
	const CHAR*		m_szItem2NumQua;			// ��Ʒ2 ����
	const CHAR*		m_szItem3Qua;				// ��Ʒ3 
	const CHAR*		m_szItem3NumQua;			// ��Ʒ3 ����
};


// ���� ֻ�ǹ��ͻ���ʹ�õļ���ģ��ṹ	skl_template.tab
struct _DBC_SKILL_DATA
{
	INT				m_nID;								// ID
	INT				m_nIDForManagement;					// �߻��ڲ�ʹ�õĹ�����ID
	INT				m_nMenPai;							// ����ID
	const CHAR	*	m_lpszName;							// ����
	const CHAR	*	m_lpszSkillExplain;					// ��������
	const CHAR	*	m_lpszSkillExplainForTip;			// ��������
	const CHAR	*	m_lpszSkillExplainForInterface;		// ��������
	const CHAR	*	m_lpszIconName;						// Icon����

	INT				m_nSkillActionType;					// ��ʽ����ENUM_SKILL_ACTION_TYPE
	INT				m_nSkillLogicID;					// �����߼�ID
	INT				m_nSkillSeries;						// ����ϵ��
	INT				m_nMaxLevel;						// �ȼ�����
	const CHAR	*	m_lpsznCoolDownTime;					// ��ȴʱ��
	const CHAR	*	m_lpszJiQiTime;						// ����ʱ��
	const CHAR	*	m_lpszDirectTime;						// ����ʱ��
	const CHAR	*	m_lpszConditionExplain1;			// ���������Ĳ���˵��1
	INT				m_ConditionLogic1;					// �����������߼�1
	INT				m_nCondition11;						// ���������Ĳ���ֵ11
	INT				m_nCondition12;						// ���������Ĳ���ֵ12
	const CHAR	*	m_lpszConditionExplain2;			// ���������Ĳ���˵��2
	INT				m_ConditionLogic2;					// �����������߼�2
	INT				m_nCondition21;						// ���������Ĳ���ֵ21
	INT				m_nCondition22;						// ���������Ĳ���ֵ22
	const CHAR	*	m_lpszConditionExplain3;			// ���������Ĳ���˵��3
	INT				m_ConditionLogic3;					// �����������߼�3
	INT				m_nCondition31;						// ���������Ĳ���ֵ31
	INT				m_nCondition32;						// ���������Ĳ���ֵ32
	INT				m_nTargetLevel;						// Ŀ�꼶��

	INT			m_bMustUseWeapon; 					// �Ƿ��Ǳ���ʹ�������ļ���
	INT				m_nDisableByFlag1; 					// ���������Ʊ��1, ���ڻ���,�Ȼ��
	INT				m_nDisableByFlag2; 					// ���������Ʊ��2, ���ڳ�Ĭ����
	INT				m_nDisableByFlag3; 					// ���������Ʊ��3, ���ڱ������
	INT				m_nRangeSkillFlag;					// �Ƿ���Զ�̼���
	INT			m_bBreakPreSkill;					// �Ƿ��ж��Լ���ǰʹ�õļ���
	INT				m_nType;							// �������� ENUM_SKILL_TYPE
	INT				m_nCooldownID;						// ��ȴ��ID
	const CHAR	*	m_lpszGatherLeadActionSetID;		// ����/����������ID 
	const CHAR	*	m_lpszSendActionSetID;				// ������ʽID
	INT				m_nEnvironmentSpecialEffect;		// ������ЧID
	INT				m_nTargetMustInSpecialState;		// Ŀ������ǣ� 0:��ģ�1:����; -1:û��Ҫ��
	INT				m_nClassByUser;						// ��ʹ�������ͷ��࣬0:���, 1:����, 2:����, 3:��Ʒ,
	INT				m_nPassiveFlag;						// �������Ǳ������ܣ�0:��������,1:��������;
	INT				m_nSelectType;						// ��ѡ���� ENUM_SELECT_TYPE
	const CHAR	*	m_lpszAttachProjectTextureEffect;				// ѡ�л���Ч����
	INT				m_nOperateModeForPetSkill;			// ���＼��ר�ã�����ģʽ: PET_SKILL_OPERATEMODE
	INT				m_nPetRateOfSkill; 					// ���ܷ�������,ֻ�Գ��＼����Ч
	INT				m_nTypeOfPetSkill; 					// ���＼������,0:�﹦,1:����,2:����,3:����,4:����;
	INT			m_nImpactIDOfSkill;					// ���＼�ܲ�����Ч��ID
	INT				m_nReserved1;						// Ԥ��������
	INT				m_nReserved2;						// Ԥ��������
	INT				m_nReserved3;						// Ԥ��������
	INT				m_nReserved4;						// Ԥ��������

	INT				m_nBulletID;						// �ӵ�ID
	const CHAR	*	m_pszBulletSendLocator;				// �ӵ������İ󶨵�
	const CHAR	*	m_pszBulletNum;						// �ӵ�����		����ӵ�������ͬһID
	INT				m_nAngle;							// �ӵ��н�

	INT				m_nTargetingLogic;					// Ŀ��ѡ���߼�	ENUM_TARGET_LOGIC
	INT				m_nSendTime;						// ����ʱ��(ms)
	FLOAT			m_fMinAttackRange;					// ��С��������(m)
	FLOAT			m_fMaxAttackRange;					// ��󹥻�����(m)
	INT				m_nFriendness;						// �����Ѻöȣ�=0Ϊ���Լ��ܣ�>0Ϊ���漼�ܣ�<0Ϊ ���漼��
	INT				m_nRelationshipLogic;				// Ŀ���ʹ���߹�ϵ�ĺϷ����ж�ID���ο�����ӱ��
	INT				m_nTargetCheckByObjType;			// Ŀ�������ĳһָ����obj_type�Ľ�ɫ
	INT				m_nIsPartyOnly;						// �Ƿ���Զ�����Ч��ע�����ѳ����������ѡ�1Ϊֻ�Զ�����Ч��0Ϊ�޴����ơ�
	INT				m_nHitsOrINTerval;					// �����Ĺ���������������ʱ����
	INT				m_bAutoRedo;						// ��������
	INT				m_nHitRate;							// ������
	INT				m_nCriticalRate; 					// ������
	INT			m_bUseNormalAttackRate;				// ��ȴʱ���Ƿ��ܹ����ٶ�Ӱ��
	INT				m_nActiveTime;						// ����ʱ��
	const CHAR*			m_szDamageRange;						// ɱ�˷�Χ(m)
	FLOAT			m_nDamageAngle;						// ɱ�˽Ƕ�(0~360)
	INT				m_nTargetNum;						// Ӱ��Ŀ��������
	INT				m_nReserved5;						// Ԥ��������
	INT				m_nCanInterruptAutoShot; 			// �Ƿ��ܴ���Զ����ܵ������ͷ�
	INT			m_nDelayTime; 						// �ӳ�ʱ��
	INT				m_nDamageDelay;						// �˺������ͨ���ӳ�ʱ�䣨ms��
	INT				m_nImmediately;						// �Ƿ�˲������
	INT				m_nBeatBackDist;					// ���˾���
	INT				m_nBeatBackRate;					// ���˸���
	INT				m_nBeatFlyDist;						// ���ɾ���
	INT				m_nBeatFlyRate;						// ���ɸ���
	INT			m_bBallSkill;						// �Ƿ��Ǵ�������Ч�ļ���
	INT				m_nReserved6;						// Ԥ��������
	INT				m_nReserved7;						// Ԥ��������

	const CHAR	*	m_lpszImpcatExplain_1;				// Ч��˵��_1
	const CHAR  *	m_lpszImpactParameter_1;			// Ч����������_1
	const CHAR	*	m_lpszImpcatExplain_2;				// Ч��˵��_2
	const CHAR  *	m_lpszImpactParameter_2;			// Ч����������_2
	const CHAR	*	m_lpszImpcatExplain_3;				// Ч��˵��_3
	const CHAR  *	m_lpszImpactParameter_3;			// Ч����������_3
	const CHAR	*	m_lpszImpcatExplain_4;				// Ч��˵��_4
	const CHAR  *	m_lpszImpactParameter_4;			// Ч����������_4
	const CHAR	*	m_lpszImpcatExplain_5;				// Ч��˵��_5
	const CHAR  *	m_lpszImpactParameter_5;			// Ч����������_5
	const CHAR	*	m_lpszImpcatExplain_6;				// Ч��˵��_6
	const CHAR  *	m_lpszImpactParameter_6;			// Ч����������_6
	const CHAR	*	m_lpszImpcatExplain_7;				// Ч��˵��_7
	const CHAR  *	m_lpszImpactParameter_7;			// Ч����������_7
	const CHAR	*	m_lpszImpcatExplain_8;				// Ч��˵��_8
	const CHAR  *	m_lpszImpactParameter_8;			// Ч����������_8
	const CHAR	*	m_lpszImpcatExplain_9;				// Ч��˵��_9
	const CHAR  *	m_lpszImpactParameter_9;			// Ч����������_9
	const CHAR	*	m_lpszImpcatExplain_10;				// Ч��˵��_10
	const CHAR  *	m_lpszImpactParameter_10;			// Ч����������_10
	const CHAR	*	m_lpszImpcatExplain_11;				// Ч��˵��_11
	const CHAR  *	m_lpszImpactParameter_11;			// Ч����������_11
	const CHAR	*	m_lpszImpcatExplain_12;				// Ч��˵��_12
	const CHAR  *	m_lpszImpactParameter_12;			// Ч����������_12

	const CHAR	*	m_lpszChildSkillID;					// �Ӽ��ܼ���
	const CHAR	*	M_lpszChildSkillOdds;				// �Ӽ��ܴ�������

	INT				m_nTransferSkill;					// ת���༼�ܣ�ENUM_TRANSFER_SKILL

	INT				m_bJiTui;								//�Ƿ񱻻���
	INT				m_JiTuiBaifenbi;								//���˰ٷֱ�
	INT				m_JiTuiRate;								//���˸���
	INT				m_bSouXiaSkill;								//�Ƿ�������¼����

//	INT				m_bAutoRedo;						// �Զ�����

};
typedef _DBC_SKILL_DATA SSkillData;
typedef _DBC_SKILL_DATA _SKILL_DATA;

///////////////////////////////////////////////////////////////////////
// �ķ�
struct _DBC_XINFA_DATA
{
	//
	UINT		m_uID;							// ID
	UINT		m_uIDMenpai;					// ����ID
	const CHAR	*m_lpszName;					// ����
	CHAR		*m_pszDesc;						// ��������
	const CHAR	*m_lpszIconName;				// Icon����
};
///////////////////////////////////////////////////////////////////////
struct _OWN_SKILL
{
	enum _OWN_SKILL_STATE
	{
		OWN_SKILL_DISABLE,					//����ѧϰ��Ҳ����ʹ�ú�����
		OWN_SKILL_CAN_LEARN,				//����ѧϰ��������ʹ�ú�����
		OWN_SKILL_CAN_USE,					//����ʹ�ò�������
		OWN_SKILL_CAN_UPGRADE				//��������
	};
	SkillID_t			m_nSkillID;			//ӵ�еļ���ID
	_OWN_SKILL_STATE	m_eState;			//����������״̬
	BYTE				m_nLevel ;			//���ܵȼ�
};

struct _OWN_XINFA
{
	SkillID_t		m_nXinFaID ;			//ӵ�е��ķ�ID
	BYTE			m_nLevel ;				//�ķ��ȼ�
};



////////////////////////////////////////////
// �˺��Ľṹ
#define DAMAGE_INFO_PARAM_NUMBER	(8)

struct _DAMAGE_INFO
{
	enum
	{
		TYPE_INVALID = INVALID_ID,
		TYPE_EFFECT = 0,
		TYPE_HEAL_AND_DAMAGE = 1,
		TYPE_DROP_BOX = 2,
		TYPE_SKILL_TEXT = 3,
		TYPE_DIE = 4,
	};
	SkillID_t		m_nSkillID;				// ����ID
	ObjID_t 		m_nTargetID;			// Ŀ������ID
	ObjID_t			m_nSenderID;			// �����ߵ�ID
	INT				m_nSenderLogicCount;	// �����ߵ��߼�����
	ImpactID_t		m_nImpactID;			// Ч��ID //�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID
	ID_t			m_nType;				// Ч�����˺���������ֵ�������
	INT				m_nHealthIncrement;
	BOOL			m_bHealthDirty;
	INT				m_nManaIncrement;
	BOOL			m_bManaDirty;
	INT				m_nRageIncrement;
	BOOL			m_bRageDirty;
	INT				m_nStrikePointIncrement;
	BOOL			m_bStrikePointDirty;
	BOOL			m_bIsCriticalHit;
	
	BOOL			m_bBeatBack;
	BOOL			m_bBeatFly;
	WORLD_POS		m_BeatBackPos;
	WORLD_POS		m_CloseToPos;
	FLOAT			m_fBeatFlyDist;
	FLOAT			m_fAttackerAngle;
	BOOL			m_bCloseTo;

	INT				m_nDelayTime;			// �ӳ�ʱ��

	INT 			m_aAttachedParams[DAMAGE_INFO_PARAM_NUMBER]; // ���ӵĲ���

	_DAMAGE_INFO( VOID )
	{
		Reset();
	}

	VOID Reset( VOID )
	{
		m_nTargetID				= INVALID_ID;
		m_nSenderID				= INVALID_ID;
		m_nSenderLogicCount		= -1;
		m_nImpactID				= INVALID_ID;
		m_nType					= TYPE_INVALID;
		m_nHealthIncrement		= 0;
		m_bHealthDirty			= FALSE;
		m_nManaIncrement		= 0;
		m_bManaDirty			= FALSE;
		m_nRageIncrement		= 0;
		m_bRageDirty			= FALSE;		
		m_nStrikePointIncrement	= 0;
		m_bStrikePointDirty		= FALSE;
		m_bIsCriticalHit		= FALSE;
		m_bBeatBack				= FALSE;
		m_bBeatFly				= FALSE;
		m_fBeatFlyDist			= 0;
		m_fAttackerAngle		= 0;
		m_bCloseTo				= FALSE;
		m_nDelayTime			= 0;
		memset( m_aAttachedParams, 0, sizeof( m_aAttachedParams ) );
	}

	const _DAMAGE_INFO &operator = ( const _DAMAGE_INFO &in )
	{
		if ( this != &in )
		{
			m_nTargetID				= in.m_nTargetID;
			m_nSenderID				= in.m_nSenderID;
			m_nSenderLogicCount		= in.m_nSenderLogicCount;
			m_nImpactID				= in.m_nImpactID;
			m_nType					= in.m_nType;
			m_nHealthIncrement		= in.m_nHealthIncrement;
			m_bHealthDirty			= in.m_bHealthDirty;
			m_nManaIncrement		= in.m_nManaIncrement;
			m_bManaDirty			= in.m_bManaDirty;
			m_nRageIncrement		= in.m_nRageIncrement;
			m_bRageDirty			= in.m_bRageDirty;
			m_nStrikePointIncrement = in.m_nStrikePointIncrement;
			m_bStrikePointDirty		= in.m_bStrikePointDirty;
			m_bIsCriticalHit		= in.m_bIsCriticalHit;
			m_bBeatBack				= in.m_bBeatBack;
			m_bBeatFly				= in.m_bBeatFly;
			m_BeatBackPos			= in.m_BeatBackPos;
			m_CloseToPos			= in.m_CloseToPos;
			m_fBeatFlyDist			= in.m_fBeatFlyDist;
			m_fAttackerAngle		= in.m_fAttackerAngle;
			m_bCloseTo				= in.m_bCloseTo;
			m_nDelayTime			= in.m_nDelayTime;

			memcpy( m_aAttachedParams, in.m_aAttachedParams, sizeof( m_aAttachedParams ) );
		}
		return *this;
	}
};

////////////////////////////////////////////
// ����BUFF�Ľṹ
#define BUFF_IMPACT_INFO_PARAM_NUMBER	(8)

struct _BUFF_IMPACT_INFO
{
	ObjID_t			m_nReceiverID;			// Ч�������ߵ�ID
	ObjID_t			m_nSenderID;			// Ч���ͷ��ߵ�ID
	ImpactID_t		m_nBuffID;				// ��Ч���ݵ�ID(����)
	SkillID_t		m_nSkillID;  			// Skill ID
	INT				m_nSenderLogicCount;	// Ч�������ߵ��߼�����
	UINT			m_nSN;					// Ч�����к�
	INT				m_nTimer;				// ʣ��ʱ��
	INT				m_nDelayTime;			// �ӳٴ���ʱ��

	_BUFF_IMPACT_INFO( VOID )
	{
		Reset();
	}

	VOID Reset( VOID )
	{
		m_nReceiverID			= INVALID_ID;
		m_nSenderID				= INVALID_ID;
		m_nBuffID				= INVALID_ID;
		m_nSkillID				= INVALID_ID;
		m_nSenderLogicCount		= -1;
		m_nSN					= -1;
		m_nTimer				= 0;
		m_nDelayTime			= 0;
	}

	const _BUFF_IMPACT_INFO &operator = ( const _BUFF_IMPACT_INFO &in )
	{
		if ( &in != this )
		{
			m_nReceiverID			= in.m_nReceiverID;
			m_nSenderID				= in.m_nSenderID;
			m_nBuffID				= in.m_nBuffID;
			m_nSkillID				= in.m_nSkillID;
			m_nSenderLogicCount		= in.m_nSenderLogicCount;
			m_nSN					= in.m_nSN;
			m_nTimer				= in.m_nTimer;
			m_nDelayTime			= in.m_nDelayTime;
		}
		return *this;
	}
};

// ��˵�е����˵���Ϣ�ṹ
struct _CALLOF_INFO
{
	GUID_t			m_guidCaller;	// �ٻ��ߵ�GUID
	SceneID_t		m_SceneID;		// ����ID
	WORLD_POS		m_Pos;			// ����λ��
	UINT			m_uDuration;	// ����ʱ��

	_CALLOF_INFO( VOID )
	{
		m_guidCaller		= INVALID_GUID;
		m_SceneID			= INVALID_ID;
		m_Pos				= WORLD_POS( -1.f, -1.f );
		m_uDuration			= 0;
	}

	VOID Reset( VOID )
	{
		m_guidCaller		= INVALID_GUID;
		m_SceneID			= INVALID_ID;
		m_Pos				= WORLD_POS( -1.f, -1.f );
		m_uDuration			= 0;
	}
};

//cooldown
class Cooldown_T
{
	public:
		Cooldown_T(VOID): m_nID(INVALID_ID), m_nCooldown(0), m_nCooldownElapsed(0){};
		Cooldown_T(Cooldown_T const& rhs)
		{
			m_nID = rhs.GetID();
			m_nCooldown = rhs.GetCooldownTime();
			m_nCooldownElapsed = rhs.GetCooldownElapsed();
		};
		~Cooldown_T(VOID){}
		Cooldown_T& operator=(Cooldown_T const& rhs)
		{
			m_nID = rhs.GetID();
			m_nCooldown = rhs.GetCooldownTime();
			m_nCooldownElapsed = rhs.GetCooldownElapsed();
			return *this;
		};
		VOID HeartBeat(Time_t nDeltaTime)
		{
			if(0>m_nID)
			{
				return;
			}
			if(m_nCooldown<=m_nCooldownElapsed)
			{
				return;
			}
			m_nCooldownElapsed +=nDeltaTime;
			if(m_nCooldown<m_nCooldownElapsed)
			{
				m_nCooldown=m_nCooldownElapsed;
			}
		};
		BOOL IsCooldowned(VOID) const
		{
			return m_nCooldown<=m_nCooldownElapsed;
		};
		Time_t GetRemainTime(VOID) const
		{
			return m_nCooldown - m_nCooldownElapsed;
		};
		VOID Reset(VOID)
		{
			m_nID = INVALID_ID;
			m_nCooldown = 0;
			m_nCooldownElapsed = 0;
		};
		ID_t GetID(VOID) const {return m_nID;}
		VOID SetID(ID_t nID) {m_nID = nID;}
		Time_t GetCooldownTime(VOID) const {return m_nCooldown;}
		VOID SetCooldownTime(Time_t nTime) {m_nCooldown = nTime;}
		Time_t GetCooldownElapsed(VOID) const {return m_nCooldownElapsed;}
		VOID SetCooldownElapsed(Time_t nTime){m_nCooldownElapsed = nTime;}
	protected:
	private:
		ID_t	m_nID;
		Time_t	m_nCooldown;
		Time_t	m_nCooldownElapsed;
};
template <INT nSize>
class CooldownList_T
{
	public:
		enum
		{
			LIST_SIZE = nSize,
		};
		CooldownList_T(VOID){}
		CooldownList_T(CooldownList_T const& rhs)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx] = rhs.GetCooldownInfoByIndex(nIdx);
			}
		}
		~CooldownList_T(VOID){}
		VOID CleanUp(VOID) {Reset();}
		VOID Reset(VOID)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx].Reset();
			}
		}
		CooldownList_T& operator=(CooldownList_T const& rhs)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx] = rhs.GetCooldownInfoByIndex(nIdx);
			}
			return *this;
		}
		Cooldown_T const& GetCooldownInfoByIndex(INT nIdx) const
		{
			if(0>nIdx || LIST_SIZE<=nIdx)
			{
				AssertEx(FALSE,"[CooldownList_T::GetCooldownByIndex]:Illegal index found!!");
				return m_aCooldown[0];
			}
			return m_aCooldown[nIdx];
		}
		Time_t GetRemainTimeByID(CooldownID_t nCooldown) const
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					return m_aCooldown[nIdx].GetRemainTime();
				}
			}
			return TRUE;
		}
		INT GetSlotSize(VOID)
		{
			return LIST_SIZE;
		}
		INT GetByteSize(VOID)
		{
			return sizeof(CooldownList_T);
		}
		VOID HeartBeat(Time_t nDeltaTime)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx].HeartBeat(nDeltaTime);
			}
		}
		BOOL IsSpecificSlotCooldownedByID(CooldownID_t nCooldown) const
		{
			if(0 > nCooldown)
			{
				return TRUE;
			}
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					return m_aCooldown[nIdx].IsCooldowned();
				}
			}
			return TRUE;
		}
		VOID RegisterCooldown(CooldownID_t nCooldown, Time_t nCooldownTime)
		{
			INT nEmptySlot = INVALID_ID;
			if(0 > nCooldown)
			{
				return;
			}
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(INVALID_ID==nEmptySlot)
				{
					if(INVALID_ID==m_aCooldown[nIdx].GetID())
					{
						nEmptySlot = nIdx;
					}
					else if(TRUE==m_aCooldown[nIdx].IsCooldowned())
					{
						nEmptySlot = nIdx;
					}
				}
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					m_aCooldown[nIdx].Reset();
					m_aCooldown[nIdx].SetID(nCooldown);
					m_aCooldown[nIdx].SetCooldownTime(nCooldownTime);
					return;
				}
			}
			if(INVALID_ID!=nEmptySlot)
			{
					m_aCooldown[nEmptySlot].Reset();
					m_aCooldown[nEmptySlot].SetID(nCooldown);
					m_aCooldown[nEmptySlot].SetCooldownTime(nCooldownTime);
					return;
			}
			AssertEx(FALSE, "[CooldownList_T::RegisterCooldown]: Cooldown list full!!");
		}
	protected:
	private:
		Cooldown_T m_aCooldown[LIST_SIZE];
};

class MissFlag_T //empty class for wrap constants
{
	public:
		enum
		{
			FLAG_NORMAL =0,
			FLAG_MISS,
			FLAG_IMMU,
			FLAG_ABSORB,
			FLAG_COUNTERACT,
			FLAG_TRANSFERED,
		};
};

enum
{
	MAX_WAY_POINT_NUM = 4
};

//�����ӳ�m_nDelayTime�����client���� SkillMiss ��Ϣ
struct _SkillMissMSGDelayTime
{
	ObjID_t		m_nSender;		
	SkillID_t	m_nSkill;
	Time_t		m_nDelayTime;
	INT			m_nLogicCount;
	_SkillMissMSGDelayTime()
	{
		Cleanup();
	}
	VOID Cleanup()
	{
		memset( this, -1, sizeof(_SkillMissMSGDelayTime) );
	}
};

struct _Monster_Pet_DB_Struct
{
	INT			m_iDataIndex;							//monsterAttrex.tab ������ֵ
	INT			m_iHP;									//�ٻ��޵�ǰѪ��
	INT			iLifeTime;								//�ٻ���ʣ������ʱ��
	_OWN_IMPACT m_aImpactList[MONSTER_PET_MAX_IMPACT];	//�ٻ��������е�buff
	INT			m_aSkillList[MONSTER_PET_MAX_SKILL];	//�ٻ��޵�ǰѪ��
};

#endif
