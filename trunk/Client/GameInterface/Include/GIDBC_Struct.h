
/** GIDBC_Struct.h
 *	
 *	���ܣ�	�������ݿ�ṹ
 *
 *	�޸ļ�¼��
 *			080304	���� ������ �� common.tab ���й̶���ID
 *
 *	�޸ļ�¼��
 *			08_03_07 �޸������
 *
 *			080331	���� DayTips.tab
 *			080410	���� TopList.tab
 *			080410	���� questlog.tab
 *			100517	���� Mappic.tab
 *			100517	���� Chathelp.tab
 *
 */



#pragma once



//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


// ����� �̶���ID
#define EQUIP_CHIEFTAIN_TAG		11990001



// װ���ϵ���Ч
#define  EQUIP_EFFECT_MAX_NUM    16
struct EQUIP_EFFECT
{
	LPCTSTR szEffect;     // ��Чid
	LPCTSTR	szBindPoint;  // ��Ч���
}; 

// �����ϵ���Ч
#define  MONSTER_EFFECT_MAX_NUM     4
struct MONSTER_EFFECT
{
	LPCTSTR	szEffect;
	LPCTSTR	szBind;

	MONSTER_EFFECT()
	{
		szEffect = NULL;
		szBind	 = NULL;
	};
};




/////////////////////////////////////////////////////////////////////
//��������							(CHAR_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_RACE			(101)		//��������(�У�Ů)
struct _DBC_CHAR_RACE		// bc
{
	INT		nID;
//	INT		nIsPlayer;
//	INT		nGender;
	INT		nModelID;			// ģ��
	INT		nDefHairGeo;		// ͷ
	INT		nDefFaceGeo;		// ��
	INT		nDefShoulder;		// ��
	INT		nDefBody;			// ����
	INT		nDefArm;			// ��
	INT		nDefFoot;			// ��
	INT		nIdleInterval;		// ����ʱ��
};

/////////////////////////////////////////////////////////////////////
// ����ͷ��ģ��						(CHAR_02)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HAIR_GEO		(102)		//����ͷ��ģ��
struct _DBC_CHAR_HAIR_GEO
{
	INT		nID;
	INT		nRace;
	LPCSTR	pMeshFile;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
// ��������ģ��						(CHAR_04)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HEAD_GEO		(104)		//��������ģ��
struct _DBC_CHAR_HEAD_GEO		// bc
{
	INT		nID;
	LPCSTR	pMeshFile;
	INT		nRace;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
// ���Ƕ�����						(CHAR_06)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_ACTION_SET	(106)
#define MAX_WEAPON_TYPE_NUMBER		(14)	// ������������  ���ֵ����,WEAPON_TYPE_NUMBERS
struct _DBC_CHARACTER_ACTION_SET
{
	INT		nID;
	LPCSTR	pWeapon_Set[MAX_WEAPON_TYPE_NUMBER];	// ��Ӧ�������� ENUM_WEAPON_TYPE
	BOOL	bHideWeapon;							// ��������
	INT		nAppointedWeaponID;						// ָ��������ID
	LPCSTR	pszDesc;								// ����
};

/////////////////////////////////////////////////////////////////////
// ������Ч							(CHAR_07)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EFFECT		(107)
struct _DBC_CHARACTER_EFFECT
{
	INT		nID;
	INT		nEffect1;
	INT		nEffect2;
	LPCSTR	pLocator;
	INT		nSoundID;
	LPCSTR	pWeaponLocator;
};

/////////////////////////////////////////////////////////////////////
// ������������ֵ						(CHAR_08)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EXPLEVEL		(108)
struct _DBC_CHARACTER_EXPLEVEL
{
	INT		nID;
	INT		nEffectNeed;
};

/////////////////////////////////////////////////////////////////////
// ����ID��Ӧģ������					(CHAR_09)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_MODEL			(109)
#define MAX_MOUNT_NUMBER			(20)				// 
#define MAX_SOUND_NUMBER			(6)					// ����ֵ���ݵ�������͵ó�
struct _DBC_CHARACTER_MODEL		// bc
{
	INT		m_nID;
	FLOAT	m_fFuseTime;								// �����ۺ�ʱ��
	LPCSTR	m_pszModelName;								// ģ���ļ�
	INT		m_nSoundId[MAX_SOUND_NUMBER];				// ��ͬ�����ܲ�������
	LPCSTR	m_pszActionSetName_None;					// ��ʹ������Ķ����ļ�	
	LPCSTR	m_apszActionSetName_Mount[MAX_MOUNT_NUMBER];// ʹ�������Ķ����ļ�	
};

/////////////////////////////////////////////////////////////////////
// ����ͷ��
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_FACE		(110)		//����ͷ��
struct _DBC_CHAR_FACE		/// bc
{
	INT		nID;
	LPCSTR	pImageSetName;
	INT		nRace;
};

/////////////////////////////////////////////////////////////////////
// �����
/////////////////////////////////////////////////////////////////////
#define MOUNT_EFFECT_MAX_NUM 8
struct MOUNT_EFFECT
{
	const CHAR * pszEffectLocator;	// ��Ч�ﶨ��
	const CHAR * pszEffectName;		// ��Ч����
};

#define DBC_CHARACTER_MOUNT		(111)
struct _DBC_CHARACTER_MOUNT		// bc
{
	INT				m_nID;								// ���ID
	FLOAT			m_fAddHeight;						// �������ľ��룬���� ����
	INT				m_nModelID;							// ģ��ID
	INT				m_nCharActionIndex;					// �������ʱ�Ķ�����,��Ӧchar_model.tab��mount0~19
	MOUNT_EFFECT	m_EffectInfo[MOUNT_EFFECT_MAX_NUM];	// ��Ч��Ϣ
};




//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
// ���ﶨ���ܱ�						(CREATURE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_ATT			(201)
struct _DBC_CREATURE_ATT
{
	INT		nID;						// ������
	LPCSTR	pName;						// ����
	INT		nLevel;						// �ȼ�
	INT		NOTCARE1[3];				//* ˢ������
										// ��С��Ѫ�ٷֱ�(Hppercent)
										// ����������(EXP)
	INT		nAttackRate_Near;			// ���̹������(short range att)
	INT		nAttackRate_Far;			// Զ�̹������(long range att)
	INT		nAttackRate_Magic_Near;		// �����⹦����
	INT		nAttackRate_Magic_Far;		// Զ���⹦����
	INT		nDefence_Near;				// ���̷���(short range recovery)
	INT		nDefence_Far;				// Զ�̷���(long range recovery)
	INT		nDefence_Magic_Near;		// �����⹦����
	INT		nDefence_Magic_Far;			// Զ���⹦����
	INT		nMaxHP;						// HP����(hp limit)
	INT		nMaxMP;						// MP����(mp limit)
	INT		nRestoreHP;					// HP�ظ�(hp reversion)
	INT		nRestoreMP;					// MP�ظ�(mp reversion)
	INT		nHit;						// ����(hit rate)
	INT		nMiss;						// ���ܣ�volt��
	INT		nCritrate;					// �����ʣ�huixin)
	INT     nToughness;					// ����
	INT		nAttackGold;				// ���Թ�����(shuxingatt1 )	��
	INT		nAttackWood;				// ���Թ�����(shuxingatt2 ) ľ
	INT		nAttackWater;				// ���Թ�����(shuxingatt3) ˮ
	INT		nAttackFire;				// ���Թ�����(shuxingatt4) ��
	INT		nAttackSoil;				// ���Թ���5 (shuxingatt4) ��
	INT		nDefenceGold;				// ���Է�����(shuxingrecovery1)
	INT		nDefenceWood;				// ���Է�����(shuxingrecovery2)
	INT		nDefenceWater;				// ���Է�����(shuxingrecovery3)
	INT		nDefenceFire;				// ���Է�����(shuxingrecovery4)
	INT		nDefenceSoil;				// ���Է�����(shuxingrecovery4)
	INT		NOTCARE2;					//* ���ýű�ID
	INT		nModelID;					// ����ID
	LPCSTR	szIconName;					// ����ͷ��
	INT		nIsCanSwerve;				// �Ƿ������ת��
	INT		nIsDisplayerName;			// �Ƿ���ʾͷ����Ϣ��
	FLOAT	fBoardHeight;				// ���ְ�߶�
	FLOAT	fProjTexRange;				// ѡ�л���С
	FLOAT	fShadowRange;				// ��Ӱ��С
	INT		NOTCARE3[2];				// ��������ʱ��
										// ������ȴʱ��
	INT		nIdleInterval;				// ���м��ʱ��
	INT		nSpeed;						// �ƶ��ٶ�
	INT		nMonsterWalkSpeed;			// �����ٶ�
	INT		nAttackSpeed;				// �����ٶ�
	LPCSTR	nMArmID;					// ��������ID
	INT		NOTCARE4[4];				//* �����ID
										// ����AI
										// ��չAI
										// ��Ӫ
	INT		nCannotBeAttack;			// ���ɱ�����
	INT		nCanHandle;					// �Ƿ���Խ���
	INT		NOTCARE5;					// �Ѻ�ֵ
	INT		nMonsterBossFlag;			// BOSS���
	INT		nMonsterBossSnapImmID;		// ˲ʱЧ������ID
	INT		nMonsterBossDurationImmID;	// ����Ч������ID
	INT		NOTCARE6[4];				// �������뾶(��)
										// ���������Ч��
										// ������Ʒ��
										// �������
	MONSTER_EFFECT	sMonsterEffect[MONSTER_EFFECT_MAX_NUM];		// ���ڹ����ϵ���Ч

	LPCSTR	pHorseDropID;				// �����ID
	INT		nDropID;					// ����ID
	INT		nNPCType;					// NPC����
	LPCSTR	pNPCTypeName;				// NPC������
	LPCSTR	pFilterName;				// NPC����ƴ������
	INT			nCriticalHitDamage;	//�����˺� ,��֪����;,�������
	INT			nRage;							//ŭ�� ,��֪����;,�������
	INT	nIsPlayAnimation;			//�Ƿ��и鵲�������ܶ���
};



/////////////////////////////////////////////////////////////////////
//���︽������						(DBC_PET_EX_ATT)
/////////////////////////////////////////////////////////////////////
#define DBC_PET_EX_ATT				(202)
struct _DBC_PET_EX_ATT		// bc
{
	INT		m_Type;						// ������
	INT		m_TakeLevel;				// ��Я���ȼ�
	LPTSTR	m_Name;						// ����
	INT		m_Camp;						// ��Ӫ
	INT		m_Reserve1;					// ����
	INT		m_Reserve2;					// ����
	INT		m_FoodType;					// ʳ����
	INT		m_SkillLearnNum;			// ����ѧ�ļ�����
	INT		m_PositiveSkill;			// ��������
	INT		m_PassiveSkill1;			// ��������1
	INT		m_PassiveSkill2;			// ��������2
	INT		m_PassiveSkill3;			// ��������3
	INT		m_PassiveSkill4;			// ��������4
	INT		m_Life;						// ��׼����
	INT		m_StrPerception;			// ��׼��������
	INT		m_ConPerception;			// ��׼��������
	INT		m_DexPerception;			// ��׼��������
//	INT		m_SprPerception;			// ��׼������
	INT		m_IntPerception;			// ��׼��������
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

/////////////////////////////////////////////////////////////////////
//�������������					(CREATURE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_SOUND			(203)
struct _DBC_CREATURE_SOUND	// bc
{
	INT		nID;
	INT		nUnderAttack;
	INT		nAttack;
	INT		nDeath;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								��Ʒ���								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//װ������Ʒ��ʾ����				(ITEM_01)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_CHAR		(301)
struct _DBC_ITEM_VISUAL_CHAR	// bc
{
	#define CHAR_RACE_NUM	(4)

	INT		nID;
//	LPCSTR	pVisualEntity[ 6 * 2 * CHAR_RACE_NUM ];
	INT		pVisualEntityIndex[38];//8������*2������*2���Ա� +������*2 + ��ģ*2
};

#define  EQUIP_LOC_EFFECT_MAX_NUM    8
#define	DBC_EQUIP_LOC		(303)
struct _DBC_EQUIP_LOC
{
	INT		nID;		// 
	LPCSTR	szDesc;		// 
	LPCSTR	pMeshFile;	// 
	LPCSTR	pMatFile;	// 
	EQUIP_EFFECT	szREffect[EQUIP_LOC_EFFECT_MAX_NUM];	// ������Ч
};


/////////////////////////////////////////////////////////////////////
// �ҽ�����Ʒ��ʾ����				(ITEM_02)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_LOCATOR				(302)
struct _DBC_ITEM_VISUAL_LOCATOR
{
	INT		nID;					// 

	LPCSTR	pObjFile_Right;			// ����������
	LPCSTR	pMatName_Right;			// ��������������

	LPCSTR	pObjFile_Left;			// ����������
	LPCSTR	pMatName_Left;			// ��������������

	LPCSTR	pObjShieldFile;			// ���ֶ���
	LPCSTR	pMatName_Shield;		// ���ֶܲ�����

	INT		pWeaponType;			// ��������
	INT		nWeaponActor;			// ��ʾ�������Ƿ��ж���
	LPCSTR	pActorFile;				// ���������ļ�

	LPCSTR	pEffectName[30];		// ����30�и��ֱ�ʯ����Ч
};

// ������
#define	DBC_ITEM_MONSTER_VISUAL_LOCATOR		(320)
struct _DBC_ITEM_MONSTER_VISUAL_LOCATOR
{
	INT				nID;								// id

	LPCSTR			pObjFile_Right;						// ����������
	LPCSTR			pMatName_Right;						// ��������������
	EQUIP_EFFECT	szREffect[MONSTER_EFFECT_MAX_NUM];	// ����������Ч

	LPCSTR			pObjFile_Left;						// ����������
	LPCSTR			pMatName_Left;						// ��������������
	EQUIP_EFFECT	szLEffect[MONSTER_EFFECT_MAX_NUM];	// ����������Ч

	LPCSTR			pObjShieldFile;						// ���ֶ���
	LPCSTR			pMatName_Shield;					// ���ֶܲ�����
	EQUIP_EFFECT	szLSEffect[MONSTER_EFFECT_MAX_NUM];	// ���ֶܲ���Ч

	INT		pWeaponType;								// ��������
	INT		nWeaponActor;								// ��ʾ�������Ƿ��ж���
	LPCSTR	pActorFile;									// ���������ļ�
};





#define  EQUIP_EFFECT_ATTR_MAXNUM      16

// װ�����������Ե�Ӱ��
struct   EQUIP_EFFECT_ATTR
{
	INT nAttrID;     // Ӱ������Id
	INT nEffectNum;  // Ӱ��������ֵ
};

//���Ӽ���������
#define  EQUIP_ADDEFFECT_SKILL_MAXNUM     4  
//Ӱ�켼��������
#define  EQUIP_EFFECT_SKILL_MAXNUM     4 


struct   EQUIP_EFFECT_SKILL
{

	INT nSkillID;     // Ӱ�켼��id
	INT nEffectLevel; // Ӱ�켼�ܵȼ�
}; 

/////////////////////////////////////////////////////////////////////
//��Ʒ�����_װ��										
/////////////////////////////////////////////////////////////////////

#define  DBC_ITEM_EQUIP         (318)
struct  _DBC_ITEM_EQUIP
{
	INT		nID;			// װ�����к�
	INT		nClass;			// ��������class
	INT		nType;			// ��� type
	INT		nIndex;			// ��Ʒ��
	INT		nEquipPoint;	// װ����     
	INT		nRule;          // ��Ӧ����
	INT		nSetID;         // װ����װ���
	LPCSTR	nLevelRequire;  // �ȼ�����
	INT		nBasePrice;     // �����۸�
	INT		nBMaxDur;       // max�;�
	INT     nStarLeveL;     // �Ǽ�
	INT     nNatureHoleNum;	// ��Ȼ����Ŀ 
	INT     nMaxHoleNum;    // ������Ŀ
	INT     nProfessionalReq;// ְҵҪ��
	INT		nPaixing;	    	// װ������
	INT		nEquipScore;	 // װ������
	INT     nBaseAttackNear; // �������̹���
	INT     nBaseAttackFar;  // ����Զ�̹���
	INT     nBaseAttackMagic;// ����ħ������
	INT     nBaseDEefNear ;   // �������̷���
    INT     nBaseDefFar;	 // ����Զ�̷���
    INT     nBaseDefMagic;   // ����ħ������

	// װ�����������Ե�Ӱ��   
	EQUIP_EFFECT_ATTR stEffectAttr[EQUIP_EFFECT_ATTR_MAXNUM];

	// װ���Լ��ܵ�Ӱ��
	EQUIP_EFFECT_SKILL   stADDEffectSkill[EQUIP_ADDEFFECT_SKILL_MAXNUM];// ���Ӽ���
	EQUIP_EFFECT_SKILL   stEffectSkill[EQUIP_EFFECT_SKILL_MAXNUM];  // Ӱ�켼��

	/*

		// װ�����������Ե�Ӱ��   
		INT   stEffectAttr[ EQUIP_EFFECT_ATTR_MAXNUM * 2] ;
		// װ���Լ��ܵ�Ӱ��
		INT		ADDEffectSkill[EQUIP_ADDEFFECT_SKILL_MAXNUM * 2];  // ���Ӽ���
		INT		EffectSkill[EQUIP_EFFECT_SKILL_MAXNUM * 2];     // Ӱ�켼��
	*/

};

/////////////////////////////////////////////////////////////////////
// ��Ʒ�����_ҩƿ				(ITEM_06)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_MEDIC			(306)
struct _DBC_ITEM_MEDIC		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //����
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //����
	LPCSTR	szDesc;   //����
	INT		nLevel; //�ȼ�
	INT		nBasePrice;	//�����۸�
	INT     nSalePrice;     //���ۼ۸�
	INT		nRule;		//��Ӧ����
	INT		nPileCount; //��������
	INT		nScriptID;	//�ű����
	INT		nSkillID;	//���ܱ��
	INT		NOTCARE[2];	//�����ܵȼ�
	INT		nMaxHold;	//����������
	INT		nTargetType;	// ��Զ������� ENUM_ITEM_TARGET_TYPE
	INT     nBroadcast ;//�Ƿ�㲥
	LPCTSTR szTypeDesc;		// ��Ʒ����������
	LPCTSTR     nDropModelId;         //����ʱ������
	LPCTSTR		szDropEffect;			//����ģ�͸�����Ч
	INT			szDropSound;            //��������
	LPCTSTR		szDropVisColor;         //��ʾ������ɫ

	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
// ��Ʒ����� 		������Ʒ��		 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_TASK			(323)
struct _DBC_ITEM_TASK		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //����
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //����
	LPCSTR	szDesc;   //����
	INT		nLevelRequire; //�ȼ�
	INT		nBasePrice;	//�����۸�
	INT     nSalePrice;     //���ۼ۸�
	INT		nRule;		//��Ӧ����
	INT		nPileCount; //��������
	INT		nScriptID;	//�ű����
	INT		nSkillID;	//���ܱ��
	INT		NOTCARE[2];	//�����ܵȼ�
	INT		nMaxHold;	//����������
	INT		nTargetType;	// ��Զ������� ENUM_ITEM_TARGET_TYPE
	INT     nBroadcast ;//�Ƿ�㲥
	LPCTSTR szTypeDesc;		// ��Ʒ����������

	//... NOT CARE
};


/////////////////////////////////////////////////////////////////////
// ��Ʒ����� 		��չ������Ʒ��		 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EXTRABAG		(324)
struct _DBC_ITEM_EXTRABAG		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //����
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //����
	LPCSTR	szDesc;   //����
	INT		nRule;		//��Ӧ����
	INT		nBasePrice;	//�����۸�
	INT		nSalePrice;     //���ۼ۸�
	INT		nEffectTime;	//��Чʱ��
	INT		nEffectGridNum;		//��Ч������
	LPCTSTR szTypeDesc;		// ��Ʒ����������
	LPCTSTR     nDropModelId;         //����ʱ������
	LPCTSTR		szDropEffect;			//����ģ�͸�����Ч
	INT			szDropSound;            //��������
	LPCTSTR		szDropVisColor;         //��ʾ������ɫ

	//... NOT CARE
};


/////////////////////////////////////////////////////////////////////
// ��Ʒ����� 		������Ʒ��		 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_SOULBEAD		(325)
struct _DBC_ITEM_SOULBEAD		// ef
{
	INT		nID;         //id
	INT		nClass;    //class
	INT		nType;    //����
	INT		nIndex;   
	LPCSTR	szIcon;   //icon
	LPCSTR	szName;   //����
	LPCSTR	szDesc;   //����
	INT		nRule;		//��Ӧ����
	INT		nEffectTime;	//��Чʱ��
	INT		nImpactId;		//ʹ��Ч�����
	INT		nIsRadio;		//�Ƿ�㲥
	LPCTSTR szTypeDesc;		// ��Ʒ����������
	INT		nSelectObjType;		//ѡ���������
	LPCTSTR     nDropModelId;         //����ʱ������
	LPCTSTR		szDropEffect;			//����ģ�͸�����Ч
	INT			szDropSound;            //��������
	LPCTSTR		szDropVisColor;         //��ʾ������ɫ

	//... NOT CARE
};



//��ʯ����������
#define  EQUIP_GEMATTR_MAXNUM     4  
//����ʯ�������
#define  EQUIP_GEMNEED_MAXNUM    3  

struct   EQUIP_GEM_ATTR
{

	INT nAttr;     // ǿ������
	INT nNum;    // ǿ����ֵ
}; 

struct   EQUIP_GEM_NEED
{

	INT nGem;     // ����ʯ
	INT nGemNum;    // ����ʯNUM
}; 


/////////////////////////////////////////////////////////////////////
// ��Ʒ�����_��ʯ				(ITEM_07)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_GEM			(307)
struct _DBC_ITEM_GEM	// ef
{
	INT		nID;
	INT		nClass; //��Ʒ����
	INT		nType;   //��Ʒ����
	INT		nGemIndex ;//��Ʒ���
	LPCTSTR	szIcon;   //ͼ��
	INT		nRule;  //��Ӧ����
	LPCTSTR	szWhere; //��Ƕλ��
	LPCTSTR	szName;
	LPCTSTR szDesc;
	INT		nPrice;			// ��ʯ�����ļ۸�
	INT		nSalePrice;		// ��ʯ���۵ļ۸�
	INT     nBroadcast;		//�Ƿ�㲥
//	INT		nAttr[66];		// ��ʯ�ĸ�������	// 59

	EQUIP_GEM_ATTR  stGemAttr[EQUIP_GEMATTR_MAXNUM];// ��ʯ�ĸ�������
	EQUIP_GEM_NEED  stGemNeed[EQUIP_GEMNEED_MAXNUM]; //����������ʯ����Ϣ

	LPCTSTR	szColor;		// �������Ե���ɫ
	LPCTSTR szTypeDesc;		// ��������
	LPCTSTR szEffectColor;		//��Ч��ɫֵ
	INT		nEffectIndex;	//��Ч����
	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
// ��Ʒ�����_��ͼ				(ITEM_08)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_STOREMAP			(308)
struct _DBC_ITEM_STOREMAP		// ef
{
	INT		nID;
	INT		nClass;
//	INT		nQuality;//remove
	INT		nType;
	LPCSTR	szIcon;
	INT		nIndex;
	LPCSTR	szName;
	LPCSTR	szDesc;
	INT		nLevelRequire;
	INT		nRule;
	//.......................
	//... NOT CARE
};
/////////////////////////////////////////////////////////////////////
// ��װ��Ϻ󸽼�����			(ITEM_15)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_SETATT			(315)
struct _DBC_ITEM_SETATT
{
	INT		nID;
	INT		nAtt[66];	// 59
};

/////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
/*
	/////////////////////////////////////////////////////////////////////
	// ��ͨװ�����α�
	/////////////////////////////////////////////////////////////////////
	#define DBC_ITEM_COMMON_EQUIP_VISUAL    (316)
	struct _DBC_ITEM_COMMON_EQUIP_VISUAL
	{ 
		INT			nIndex;          // ��� ����ͬһid��װ��
		INT			nID;             // ���� 10010000   ��ͨװ��ĩ4λΪ0000
		INT         level;           // client��ʹ��
		INT			nModelID;        // ģ��id��������ģ�ͱ����ҵ���Ӧ�ľ���ģ��
		LPCTSTR		szName;          // װ������ ���� xx��
		LPCTSTR		szDes;           // ˵�� ������Ϣ
		LPCTSTR		szIcon;          // ͼ�� ui��ʾ��
		LPCTSTR		szType;          // ��, ��������


		LPCTSTR         nDropModelId;         //����ʱ������
	//	LPCTSTR		szDropEffect;			//����ģ�͸�����Ч
		INT			szDropSound;            //��������
		LPCTSTR		szDropVisColor;         //��ʾ������ɫ


		//LPCTSTR     szEffect[EQUIP_EFFECT_MAX_NUM*2];//��Ч
		EQUIP_EFFECT  szEffect[EQUIP_EFFECT_MAX_NUM];		//��Ч

	};
*/
	#define DBC_ITEM__EQUIP_VISUAl_MAXNUM    99
	#define DBC_ITEM__EQUIP_GEM_MAXNUM    7
/*

	/////////////////////////////////////////////////////////////////////
	//�߼�װ�����α�					
	/////////////////////////////////////////////////////////////////////

	#define DBC_ITEM_ADVANCED_EQUIP_VISUAL    (317)
	struct _DBC_ITEM_ADVANCED_EQUIP_VISUAL
	{ 
		INT			nIndex;          // ��� �Ը߼�װ����˵ û�õ���
		INT			nID;             // ���� 10010001
		INT			nModelID;        // ģ��id��������ģ�ͱ����ҵ���Ӧ�ľ���ģ��
		LPCTSTR		szName;          // װ������ ���� xx��
		LPCTSTR		szDes;           // ˵�� ������Ϣ
		LPCTSTR		szIcon;          // ͼ�� ui��ʾ��
		LPCTSTR		szType;          // ��

		LPCTSTR      nDropModelId;         //����ʱ������
	//	LPCTSTR		szDropEffect;			//����ģ�͸�����Ч
		INT			szDropSound;            //��������
		LPCTSTR		szDropVisColor;         //��ʾ������ɫ

		//	LPCTSTR     szEffect[EQUIP_EFFECT_MAX_NUM*2]; //��Ч
		EQUIP_EFFECT  szEffect[EQUIP_EFFECT_MAX_NUM];		//��Ч

	};


*/

/////////////////////////////////////////////////////////////////////
//�߼�װ�����α�					
/////////////////////////////////////////////////////////////////////

#define DBC_ITEM_EQUIP_VISUAL    (316)
struct _DBC_ITEM_EQUIP_VISUAL
{ 

	INT			nID;             // ���� 10010001
	INT			nModelID;        // ģ��id��������ģ�ͱ����ҵ���Ӧ�ľ���ģ��
	LPCTSTR		szName;          // װ������ ���� xx��
	LPCTSTR		szDes;           // ˵�� ������Ϣ
	LPCTSTR		szIcon;          // ͼ�� ui��ʾ��
	LPCTSTR		szType;          // ��

	LPCTSTR      nDropModelId;         //����ʱ������
	LPCTSTR		szDropEffect;			//����ģ�͸�����Ч
	INT			szDropSound;            //��������
	LPCTSTR		szDropVisColor;         //��ʾ������ɫ

	EQUIP_EFFECT  szEffect[EQUIP_EFFECT_MAX_NUM];		//��Ч

};

/////////////////////////////////////////////////////////////////////
//װ���������	�� ��ͨװ���Ż��õ�����Ӧ��װ�������ͨװ�������Ի����-1	
/////////////////////////////////////////////////////////////////////
#define  DBC_ITEM_ATTR_GROUP		322    
struct  _DBC_ITEM_ATTR_GROUP
{	
	INT			nGroupId;		// ������Id
	LPCTSTR		szGroupName;    // ����������
	INT			nProbability;   // �����鼸��

	INT         nAttrId[EQUIP_EFFECT_ATTR_MAXNUM]; // Ӱ������id ��16�� ��Ӧ��װ�����16��
};



#define    EQUIP_LEVEL_MAXNUM               100
/////////////////////////////////////////////////////////////////////
//װ�����Եȼ��� ��ͨװ���Ż��õ���					
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_ATTR_LEVEL			321
struct _DBC_ITEM_ATTR_LEVEL
{	
	INT			nAttrID; //����id
	LPCSTR      szName; //������  

	INT         nLevel[EQUIP_LEVEL_MAXNUM]; //װ��Ӱ������
	 
};


//װ���淨
////////////////////////////////////////////////////////////////////////////


//װ�����
//Equip_HoleReqInfo.tab
#define  DBC_EQUIP_HOLE_INFO             330

struct	_DBC_EQUIP_HOLE_INFO
{
	
	INT  nHoleNum; //�׵���Ŀ
	INT	 nItemNeed; //��Ҫ�Ĵ�׵���
    INT  nItemNeedNum;//��Ҫ�ĵ�������
	INT  nMoneyNeed; //��Ҫ�Ľ�Ǯ����
	INT  nSuccessRatio;//�ɹ���
//	LPCTSTR szName;//��������

};


//װ����
//Equip_bind.tab
#define  DBC_EQUIP_BINDING_INFO             331

struct	_DBC_EQUIP_BINDING_INFO
{
	INT nBindingLevel; //�󶨼���
    INT nGemNeed;     //����ʯ
    INT nIntensifyInfo; //ǿ������
	 
};


// װ������
//Equip_levelup.tab
#define  DBC_EQUIP_ADD_START             332

struct	_DBC_EQUIP_ADD_START
{			
	INT  nStarLevel;							//�Ǽ�	
	INT	m_LowLevelGemID;
	INT	m_HighLevelGemID;
	INT	m_OtherLevelGemID;
	INT  nProOneGem;							//1�ű�ʯ�ɹ�����	
	INT  nProTwoGem;							//2�ű�ʯ�ɹ�����	
	INT  nProThrGem;							//3�ű�ʯ�ɹ�����	
	INT  nProFourGem;							//4�ű�ʯ�ɹ�����	
	INT  nPunishment;							//ʧ�ܳͷ�	
	INT  nNeedMoney;							//�����Ǯ	
	INT nIntensifyInfo;  						//ǿ������

};


//װ�����ޱ�
//Equip_SpecialRepair.tab
#define  DBC_EQUIP_SP_REPAIR             333

struct	_DBC_EQUIP_SP_REPAIR
{			
	INT  nNeedItem;							//������Ʒ
	INT  nIntensifyInfo;  						//�ָ��;õ�

};

#define  MAX_EQUIP_PROMATL_NUM         4
struct	 _DBC_EQUIP_PRO_MAT
{


	INT		nMaterial;				//������
	INT		nMaterialNum;			//��������
	INT		nMatLowestLv;		//������͵�1	
	INT		nMaterialLoss;			//�����ı���4

};

#define  MAX_EQUIP_RWMATL_NUM        3       //�����������

struct	 _DBC_EQUIP_REWORK_MAT
{
	INT		nReWorkMaterial;		//����1������
	INT		nReWorkMatNum;			//����1��������
	INT		nReWorkMatLowestLv;	    //����1������͵ȼ�

};

#define  MAX_EQUIP_SPIRIT_RWMATL_NUM        3       //�������������

#define   DBC_EQUIP_PRODUCT_INFO			334

struct	 _DBC_EQUIP_PRODUCT_INFO	 
{
	INT		nFormulaId;				//�䷽��
	INT		nProductId;				//��ƷID					 	
	LPCSTR	szFormulaName;			//�䷽����	
	INT		nProductNum;			//��Ʒ����

	//INT		nMaterial1;				//������1
	//INT		nMaterial1Num;			//��������1
	//INT		nMat1LowestLv;		//������͵ȼ�1	
	//INT		nMaterial1Loss;			//�����ı���1	

	//INT		nMaterial2;				//������2	
	//INT		nMaterial2Num;			//��������2	
	//INT		nMat2LowestLv;		//������͵ȼ�2	
	//INT		nMaterial2Loss;			//�����ı���2
	//INT		nMaterial3;				//������3
	//INT		nMaterial3Num;			//��������3
	//INT		nMat3LowestLv;		//������͵ȼ�3
	//INT		nMaterial3Loss;			//�����ı���3
	//INT		nMaterial4;				//������4	
	//INT		nMaterial4Num;			//��������4	
	//INT		nMat4LowestLv;		//������͵ȼ�4
	//INT		nMaterial4Loss;			//�����ı���4
	_DBC_EQUIP_PRO_MAT  szMat[MAX_EQUIP_PROMATL_NUM];



	//INT		nReWorkMaterial1;		//����1������
	//INT		nReWorkMat1Num;			//����1��������
	//INT		nReWorkMat1LowestLv;	//����1������͵ȼ�
	//INT		nReWorkMaterial2;		//����2������
	//INT		nReWorkMat2Num;			//����2��������
	//INT		nReWorkMat2LowestLv;	//����2������͵ȼ�
	//INT		nReWorkMaterial3;		//����3������
	//INT		nReWorkMat3Num;			//����3��������
	//INT		nReWorkMat3LowestLv;	//����3������͵ȼ�

   _DBC_EQUIP_REWORK_MAT szRwMat[MAX_EQUIP_RWMATL_NUM];

	INT		nOperateTime;			//����ʱ��
	INT		nScriptId;				//�ű�ID

};

//MaterialLevel_info.tab


#define  DBC_EQUIP_MATERIAL_INFO			335

struct	 _DBC_EQUIP_MATERIAL_INFO	 
{

	INT		nMaterialGrade;	//���ϵȼ�	
	INT		nMaterialValues;//���Ϸ�ֵ

};

///pai_info.tab
// ÿ����
#define  DBC_EQUIP_CARD_INFO			336

struct	 _DBC_EQUIP_CARD_INFO	 
{

	INT			nCardId;	//��id	 ��Ӧ��ö��  ���� EQUIPPAI_ANY ������
	LPCTSTR		szCardName; //����
	INT			nAttrId;	//����id
	INT			nAttrNum;   //����ֵ
	INT			nRata;		//����

};

//chain_info.tab
 
//3������

#define  DBC_EQUIP_CARD_GROUP_INFO			337

struct	 _DBC_EQUIP_CARD_GROUP_INFO	 
{

	INT			nGroupId;		//��id
	LPCTSTR		sznGroupName;	//�������
	INT			nNeedCardId1;  //��������id
	INT			nNeedCardId2;  //��������id
	INT			nNeedCardId3;  //��������id
	INT			nAttrId;	   //ǿ������
	INT			nAttrNum;	   //ǿ����ֵ
	LPCTSTR		szEffect;	   //��Ч	
};

//soul_info.tab


//������ ���ڼ���

#define  EQUIP_CARD_ACTIVE_ATTR_NUM		3

struct   EQUIP_CARD_ATTR
{
	INT			nAttrId;	   //ǿ������
	INT			nAttrNum;	   //ǿ����ֵ

};


#define  DBC_EQUIP_CARD_ALL		338

struct	 _DBC_EQUIP_CARD_ALL	 
{

	INT			nGroupId;		//��id
	LPCTSTR		sznGroupName;	//�������
	INT			nNeedGropId1;  //�������id
	INT			nNeedGropId2;  //�������id
	INT			nNeedGropId3;  //�������id
	INT			nNeedGropId4;  //�������id
	INT			nNeedGropId5;  //�������id

	EQUIP_CARD_ATTR stAttr[EQUIP_CARD_ACTIVE_ATTR_NUM];//ǿ������
	LPCTSTR		szEffect;	   //��Ч	
};



//���Ʊ��
 //ChangePai_info.tab
#define  DBC_EQUIP_CARD_TARGET    339

struct  _DBC_EQUIP_CARD_TARGET

{
	INT   nId;     //����i
	LPCTSTR szName; //˵��
	INT   nTargetId; //Ŀ��id


};



//equipset_attr.tab


//װ����װ��

#define  DBC_EQUIP_SET_INFO			340
#define		EQUIP_SET_ATTR_NUM		4

struct   EQUIP_SET_ATTR

{
   INT   nAttrId;  //����id
   INT   nAttrNum; //������ֵ
   INT	 nAttrActiveNum; //����������Ҫ����װ��Ŀ

};

struct  _DBC_EQUIP_SET_INFO

{
	INT   nSetId;     //��װid
	LPCTSTR szSetName; //��װ����
	INT   nSetNum;    //��װ����
	LPCTSTR  szId;    //��װ������ϵ��id
	EQUIP_SET_ATTR  stAttr[EQUIP_SET_ATTR_NUM]; //��װ����

};

////////////////////////////////////////////////////////////////////////////////




//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// �����ļ�����						(SOUND_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SOUND_INFO				(401)
struct _DBC_SOUND_INFO
{
	INT		nID;
	LPCSTR	pSoundFile;
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								��Ч���								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// ��Ч����							(EFFECT_01)
/////////////////////////////////////////////////////////////////////
#define DBC_EFFECT_DEFINE			(501)
struct _DBC_EFFECT_DEFINE		// bc
{
	INT		nID;
	LPCSTR	pEffectType;
	LPCSTR	pParam1;
	LPCSTR	pParam2;
	LPCSTR	pParam3;
	LPCSTR	pParam4;
	LPCSTR	pParam5;
	LPCSTR	pParam6;
};



//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// �ķ������						(SKILL_01)
/////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_XINFA				(601)
//struct _DBC_SKILL_XINFA		// bc
//{
//	INT		nID;
//	LPCSTR	pszName;
//	INT		nMenpaiID;
//	LPCSTR	pszDesc;
//	LPCSTR	pszIconName;
//};

/////////////////////////////////////////////////////////////////////
// ����Ч��							(SKILL_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_ADDIMPACT			(602)
struct _DBC_SKILL_ADDIMPACT		// bc
{
	INT		nID;
	INT		nParamNum;
	LPCSTR	pDesc;
	INT		nAddAttributeNum;
	LPCSTR	pAddAttribute1;
	LPCSTR	pAddAttribute2;
	LPCSTR	pAddAttribute3;
	LPCSTR	pAddAttribute4;
	INT		nSpliceID;
	LPCSTR	pIcon;
};


//
///////////////////////////////////////////////////////////////////////
// �ӵ�Ч������						(SKILL_03)
///////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_BULLET			(603)
//struct _DBC_SKILL_BULLET
//{
//	INT		nID;							//�ӵ�����
//	INT		nType;							/*
//											|	0 - ˲�䵽��
//											|	1 - �ƶ�����
//											*/
//	INT		nEffect;						//��ЧID
//	FLOAT	fSpeed;							//�ƶ��ٶ�	(m/s)
//	INT		nHitEffect;						//������Ч
//	INT		nHitEffectLiveTime;				//������Чά��ʱ��
//};

/////////////////////////////////////////////////////////////////////
// BUFF����Ч��						(BUFF_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_BUFF_IMPACT					(604)
//_DBC_BUFF_IMPACT

/////////////////////////////////////////////////////////////////////
// DIRECTLY����Ч��						(DIRECTLY_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_DIRECTLY_IMPACT				(605)
//_DBC_DIRECTLY_IMPACT

/////////////////////////////////////////////////////////////////////
// �ӵ�						(BULLET)
/////////////////////////////////////////////////////////////////////
#define DBC_BULLET_DATA					(606)
//_DBC_BULLET_DATA

/////////////////////////////////////////////////////////////////////
// ����						(SKILL)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_DATA					(607)
//_DBC_SKILL_DATA

/////////////////////////////////////////////////////////////////////
// ����						(SKILL_DEPLETE)
/////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_DEPLETE					(608)
//struct _DBC_SKILL_DEPLETE	// bc
//{
//	INT			m_nID;
//	INT			m_nMP;
//	INT			m_nHP;
//	INT			m_nSP;
//	INT			m_nStrikePoint;
//	INT			m_nItemID;
//};
// $$
/////////////////////////////////////////////////////////////////////
// ����						(MISSION)
/////////////////////////////////////////////////////////////////////
//#define DBC_MISSION_DATA				(609)
//_DBC_MISSION_DATA

/////////////////////////////////////////////////////////////////////
// ����OBJ����						(SPECIAL_OBJ_DATA)
/////////////////////////////////////////////////////////////////////
#define DBC_SPECIAL_OBJ_DATA			(610)
struct _DBC_SPECIAL_OBJ_DATA		// cd
{
	INT			m_nID;					// ID
	LPCSTR		m_lpszName;				// ����
	INT			m_nType;				// ��𣨷�����ר�ã�
	LPCSTR		m_lpszToolTips;			// ToolTips
	INT			m_nLogicID;				// �߼�ID��������ר�ã�
	INT			m_nStealthLevel;		// ���μ��𣨷�����ר�ã�����ר�ã�
	INT			m_nTrapFlags;			// ������λ���ϣ�������ר�ã�����ר�ã�
	INT			m_nActiveTimes;			// ���Լ����Ĵ�����������ר�ã�
	LPCSTR		m_lpszEffect_Normal;	// �����ڳ�����Ч
	LPCSTR		m_lpszSound_Normal;		// �����ڳ�����Ч
	LPCSTR		m_lpszEffect_Active;	// ������Ч
	LPCSTR		m_lpszSound_Active;		// ������Ч
	LPCSTR		m_lpszEffect_Die;		// ������Ч
	LPCSTR		m_lpszSound_Die;		// ������Ч
	INT			m_nReserve1;			// Ԥ��
	INT			m_nReserve2;			// Ԥ��
	INT			m_nReserve3;			// Ԥ��
	INT			m_nBulletID;			// �ӵ�ID
	INT			m_nDuration;			// ����ʱ�䣨������ר�ã�
	INT			m_nInterval;			// ����򴥷�ʱ������������ר�ã�
	FLOAT		m_nTriggerRadius;		// �����뾶��������ר�ã�����ר�ã�
	FLOAT		m_nEffectRadius;		// Ӱ��뾶��������ר�ã�����ר�ã�
	INT			m_nEffectUnitNum;		// Ӱ�������Ŀ��������ר�ã�����ר�ã�
	INT			m_nReserve4;			// Ԥ��
	INT			m_nReserve5;			// Ԥ��
	INT			m_nReserve6;			// Ԥ��
	INT			m_nReserve7;			// Ԥ��
	INT			m_nReserve8;			// Ԥ��
	LPCSTR		m_lpszParamDesc0;		// ����˵����������ר�ã�
	INT			m_nParam0;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc1;		// ����˵����������ר�ã�
	INT			m_nParam1;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc2;		// ����˵����������ר�ã�
	INT			m_nParam2;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc3;		// ����˵����������ר�ã�
	INT			m_nParam3;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc4;		// ����˵����������ר�ã�
	INT			m_nParam4;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc5;		// ����˵����������ר�ã�
	INT			m_nParam5;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc6;		// ����˵����������ר�ã�
	INT			m_nParam6;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc7;		// ����˵����������ר�ã�
	INT			m_nParam7;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc8;		// ����˵����������ר�ã�
	INT			m_nParam8;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc9;		// ����˵����������ר�ã�
	INT			m_nParam9;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc10;		// ����˵����������ר�ã�
	INT			m_nParam10;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc11;		// ����˵����������ר�ã�
	INT			m_nParam11;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc12;		// ����˵����������ר�ã�
	INT			m_nParam12;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc13;		// ����˵����������ר�ã�
	INT			m_nParam13;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc14;		// ����˵����������ר�ã�
	INT			m_nParam14;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc15;		// ����˵����������ר�ã�
	INT			m_nParam15;				// ����ֵ��������ר�ã�
	LPCSTR		m_lpszInfo;				// ��ϸ˵��(�ڲ�ʹ��)
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								����������							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
// ��������							(SCENE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_DEFINE			(701)
struct _DBC_SCENE_DEFINE	// cd
{
	static const int SCENE_SERVER_ID_COLUMN = 1;

	INT		nLocalID;
	INT		nServerID;
	LPCSTR	szName;
	INT		nCityLevel;
	INT		nXSize;
	INT		nZSize;
	LPCSTR	szSceneName;
	LPCSTR	szPOVFile;
	LPCSTR	szRegionFile;
	LPCSTR  szCollisionfile;				// �������������ļ���
	LPCSTR	szMiniMap;
	INT		nBackSound;
	INT		SilentTime;						//
	LPCSTR  szSceneMap;

	INT		nWroldMapPosX;
	INT		nWroldMapPosY;					// ����ͼ�����ڵ�λ��
	INT		nNameWroldMapPosX;
	INT		nNameWroldMapPosY;				// �����������ڵ�λ��
	LPCSTR	szSceneType;					// ������ͼ��ȼ�
	LPCSTR	szCityNameNormalImageSet;		// ���������������õ�ͼƬ��Դ
	LPCSTR	szCityNameNormalImage;			// ���������������õ�ͼƬ��Դ
	LPCSTR	szCityNameHoverImageSet;		// ���������������õ�ͼƬ��Դ
	LPCSTR	szCityNameHoverImage;			// ���������������õ�ͼƬ��Դ
	LPCSTR	SceneInfo;						//		
	LPCSTR	Blur_And_Shine_Amount;			//

	FLOAT	nCameraAngle;					// ���������ƫ���� 

};

/////////////////////////////////////////////////////////////////////
// ���������ﶨ��					(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_BUILDING_DEINFE			(702)
struct _DBC_BUILDING_DEFINE		// bc
{
	static const int MAX_LEVEL = 5;

	INT		nID;				// ��Դid
	LPCSTR	szResType;			// ��Դ����
	LPCSTR	szLevel[MAX_LEVEL];	// �ȼ�1-5��Դ
};

/////////////////////////////////////////////////////////////////////
// ���н����ﶨ��					(SCENE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CITY_BUILDING			(703)
struct _DBC_CITY_BUILDING		// de
{
	INT		nID;					// id
	INT		nCityID;				// ����id(DBC_CITY_DEINFE)
	INT		nBuildingID;			// ������id(DBC_BUILDING_DEINFE)
	LPCSTR	szGfxPosition;			// λ��(gfx)
	INT		nInitLevel;				// ��ʼ�ȼ�
	LPCSTR	szGfxOrientation;		// ��ת��Ԫ��(gfx)
};

/////////////////////////////////////////////////////////////////////
// ����ת�ĵ�ͼ��Ϣ����							(SCENE_04)
/////////////////////////////////////////////////////////////////////
#define DBC_WORLDPOS_DEFINE			(704)
struct _DBC_WORLDPOS_DEFINE
{
	INT		nID;				// id
	INT		nSceneID;			// ������id
	LPCSTR	szSceneName;		// ����������
	INT		nXPos;				// ���͵��λ��
	INT		nYPos;				// ���͵��λ��
	INT		nSceneType;			// ��������
	INT		nMapLevel;			// ��ͼ�ȼ�
	INT		nEnterLevel;		// ����ȼ�
	LPCSTR	szPinyinIndex;		// ƴ������
};

/////////////////////////////////////////////////////////////////////
// ������ͼ�ϵ�����NPC��Ϣ����							(SCENE_05)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENENPC_DENFINE		(705)
struct _DBC_SCENENPC_DENFINE
{
	INT		nID;				// id
	INT		nSceneID;			// ������id
	LPCSTR	szNpcName;			// NPC������
	LPCSTR	szNpcUIName;		// NPCͷ����ʾ������
	INT		nXPos;				// X����
	INT		nYPos;				// Y����
	INT		nIsTalkNpc;			// �Ƿ񽲻�
	INT		nNpcType;			// NPC����
	LPCSTR	szNpcTypeName;		// NPC������
	LPCSTR	szPinyinIndex;		// ƴ������
};


/////////////////////////////////////////////////////////////////////
// �������͵㶨��							(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_POS_DEFINE			(917)
struct _DBC_SCENE_POS_DEFINE
{
	INT		nID;				// id
	INT		nSceneID;			// ������id
	LPCSTR	szSceneName;		// ����������
	INT		nXPos;				// ���͵��λ��
	INT		nZPos;				// ���͵��λ��
	INT		nDestSceneID;			// ���͵��ĸ�����
	LPCSTR	szDestSceneName;			// ���͵��ĸ�����
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								UI���									//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
// UI�鶨��						(UI_01)
/////////////////////////////////////////////////////////////////////
#define DBC_UI_LAYOUTDEFINE		(801)
struct _DBC_UI_LAYOUTDEFINE	// de
{
	INT		nID;
	LPCSTR	szName;
	LPCSTR	szLayout;
	INT		nDemise;
	LPCSTR	szScript;
	INT		nOpenSoundID;
	INT		nCloseSoundID;
	LPCSTR szDefaultPosition;
	LPCSTR	szHelp;
	LPCSTR	szDes;
};



//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								��������							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//


/////////////////////////////////////////////////////////////////////
// ��Դ������							(LIFE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_GROWPOINT		(901)
struct _DBC_LIFEABILITY_GROWPOINT	// cd
{
	INT		nID;
	LPCSTR	szName;
	INT		nLifeAbility;
	LPCSTR	szMeshFile;
	LPCSTR	szTips;
	INT		nTimeNeed;
	LPCSTR	szNotcare[4];
	INT		nAnimOpen;
};

/////////////////////////////////////////////////////////////////////
// �����								(LIFE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_DEFINE	(902)
struct _DBC_LIFEABILITY_DEFINE		// bc
{
	INT		nID;						//ID
	LPCSTR	szName;				//����
	INT		nLevelNeed;			//����ȼ�
	INT		nLevelMax;			//��ߵȼ�
	LPCSTR	szLevelupTable;		//������������ı�
	INT		nTimeOperation;	//����ʱ�����
	INT		nToolNeed;			//����ID
	FLOAT	fPlatformDist;			//����ƽ̨����Ч����
	INT		nPlatformID;			//����ƽ̨ID
	INT		nAnimOperation;	//��������ID
	LPCSTR	szIconName;			//ͼ��
	INT		nItemVisualLocator; // �����ҽ�
	LPCSTR	szExplain;			// ��ϸ����
	INT		nPopup;				// �Ƿ񵯳��¼�����
	LPCSTR	szProgressbarName;	// ����������
	INT		nconsume;				//���Ļ������Ǿ���
	INT		nBeginSound;		//��ʼʱ���ŵ�����
	INT		nIsCycle;				//�Ƿ�ѭ��
	INT		nSuccedSound;		//�ɹ�ʱ���ŵ�����
	INT		nFailedSound;		//ʧ��ʱ���ŵ�����
};


/////////////////////////////////////////////////////////////////////
// �����	������Ϣ							(LIFE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_CAIKUANG_LEVELINFO		(811)			//�ɿ�
#define DBC_SHOUGE_LEVELINFO		(812)			//�ո�
#define DBC_BUZHUO_LEVELINFO		(813)			//��׽
#define DBC_ZHUZAO_LEVELINFO		(814)			//����
#define DBC_ZHIYI_LEVELINFO		(815)			//����
#define DBC_GONGJIANG_LEVELINFO		(816)			//����
#define DBC_PENGREN_LEVELINFO		(817)			//���
struct _DBC_LIFEABILITY_LEVELINFO
{
	INT		nLevel;					//����
	LPCSTR	szName;				//����
	INT		nMoneyNeed;		//�����Ǯ
	INT		nExpNeed;				//������
	INT		nSkillPro;				//��������������
	INT		nSkillProClient;		//�ͻ�����ʾ��������������
	INT		nSkillMaxProLevel;	//��������������
	INT		nLevelNeed;			//�ȼ�����
	LPCSTR	szNPCName;			//NPC����
	INT		nNpcMoneyNeed;	//NPC�����Ǯ
	INT		nNpcExpNeed;		//NPC������
};


/////////////////////////////////////////////////////////////////////
// �����	 ��Ʒ�ϳ�							(LIFE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_ITEMCOMPOSE		(903)
struct _DBC_LIFEABILITY_ITEMCOMPOSE		// bc
{
	INT		nID;							//�䷽ID
	LPCSTR	szName;					//����
	INT		nResultID;					//��ƷID
	INT		nResultNum;				//��Ʒ����	
	INT		nLifeAbility;				//���������ID
	INT		nLifeAbility_Level;		//��������ܵȼ�
	INT		nStuff1ID;					//�������1ID
	INT		nStuff1num;				//�������1����
	INT		nStuff2ID;					
	INT		nStuff2num;
	INT		nStuff3ID;
	INT		nStuff3num;
	INT		nStuff4ID;
	INT		nStuff4num;
	INT		nStuff5ID;
	INT		nStuff5num;
	INT		nAttribute_ID1;		//��������
	INT		nAttribute_Level1;	//��������
	INT		nAttribute_ID2;		//���ɹ��׶�����
	INT		nAttribute_Level2;	//
	INT		nCoolDown_Time;	//��ȴʱ��
	INT		nFacility;				// ��Ҫ�Ĺ���	
	INT		nCoolDoon_Troop;		// ��ȴ��
	INT		nProficiencyRequire;	// ����������
	INT		nProficiencyIncrease;	// ����������
	INT		NOTCARE;				// �����ĵ�1��
	INT		nProficiencyTime;		// �䷽ʱ��
	INT		nScriptID;				// �ű�ID
	INT		nFailItem;				// ʧ�ܺ�����Ʒ
	LPCSTR	szExplain;				// ��ϸ����


	// ���ܷ���

	//NOT CARE...
};

/////////////////////////////////////////////////////////////////////
// ����� ����ƽ̨								(LIFE_04)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_PLATFORM		(904)
struct _DBC_LIFEABILITY_PLATFORM	// bc
{
	INT		nID;
	LPCSTR	szName;
	INT		nType;
	LPCSTR	szMeshFile;
	INT		nClickType;
};
/////////////////////////////////////////////////////////////////////
// �������ɫת���ַ�
/////////////////////////////////////////////////////////////////////
#define DBC_CODE_CONVERT			( 905 )
struct _DBC_CODE_CONVERT	// bc
{
	INT		nID;		// ID
	LPCSTR  szCode;		// �ַ�����ʽ��ʮ����������
	LPCSTR  szName;		// ����
};

/////////////////////////////////////////////////////////////////////
// �ķ�����								(XINFA_02)
/////////////////////////////////////////////////////////////////////
//#define DBC_XINFA_XINFADEFINE	(902)
//struct _DBC_XINFA_XINFADEFINE
//{
//	INT		nID;			// �ķ�ID
//	INT		nIDMenpai;		// ����ID
//	LPCSTR	szName;			// �ķ�����
//	LPCSTR	szTips;			// �ķ�����
//	LPCSTR	szIconName;		// ͼ������
//};
/////////////////////////////////////////////////////////////////////
// �ķ���������						(XINFA_02)
/////////////////////////////////////////////////////////////////////
struct Spend
{
	INT		dwSpendMoney;			// ��Ҫ���ѵĽ�Ǯ��
	INT		dwSpendExperience;		// ��Ҫ���ѵľ���ֵ
};
//#define DBC_XINFA_STUDYSPEND	(907)
//struct _DBC_XINFA_STUDYSPEND
//{
//	INT		nStudyLevel;			// Ҫѧϰ�ĵȼ�
//	Spend	StudySpend[54];
//};

/////////////////////////////////////////////////////////////////////
// �ƺ��б�						(TITLE)
/////////////////////////////////////////////////////////////////////
/*
#define DBC_TITLE_NAME			(908)
struct _DBC_TITLE_NAME		// bc
{
	INT		nTitleIndex;			// �ƺŵ�����
	LPCSTR	szTitle;				// �ƺŵ�����
	UINT	nReserve;				// ���ͱ���
	LPCSTR	szTitleDesc;			// �ƺŵ�����
};
*/

#define DBC_TITLE				(908)		// title_info.tab
struct _DBC_TITLE
{
	INT			nId;				// �ƺ�ID
	INT			nType;				// TYPE
	LPCSTR		szColor;			// ��ʾ��ɫ
	INT			bShow;				// Ĭ���Ƿ���ʾ
	LPCSTR		szManTitle;			// ���Գƺ�
	LPCSTR		szWomanTitle;		// Ů�Գƺ�
	LPCSTR		szTitleDesc;		// �ƺ�����
	INT			nTimeLimit;			// �ƺ�ʱ��
	LPCSTR		szAcquireDesc;		// ��ȡ;��
	INT			nEffectId;			// Ч��ID
	LPCSTR		szReclaimCause;		// ����ԭ��
	INT			nCombinationId;		// ��ϳƺ�ID
};


/////////////////////////////////////////////////////////////////////
// ��Ӫ����	
/////////////////////////////////////////////////////////////////////
#define DBC_CAMP_DATA	(909)

/////////////////////////////////////////////////////////////////////
// ����	skillData_v1.txt(��ȡ��)					
/////////////////////////////////////////////////////////////////////
//#define DBC_SKILLDATA_V1_DEPLETE					(910)
//struct _DBC_SKILLDATA_V1_DATA_PAIR		
//{
//	INT		nData;
//	LPCSTR  szDesc;
//	
//};
//struct _DBC_SKILLDATA_V1_DEPLETE	// cd
//{
//
//	INT		nId;				//  ID	  
//	INT		nCeHuaId;			//	�߻�ר��ID	
//	INT		nSkillLogicid;		//	�����߼�ID
//	LPCSTR	szEffectDesc;		//	Ч���ļ�Ҫ�߼�˵��	
//	INT		nCooldownTime;		//	��ȴʱ��	
//	INT		nJuqiTime;			//	����ʱ��(ms)	
//	INT		nDirectTime;		//	����ʱ��(ms)	
//	LPCSTR  szCondition1Desc;	//	���������Ĳ���˵��
//	INT		nCondition11;		//	���������Ĳ���ֵ	
//	INT	    nCondition12;		//	���������Ĳ���ֵ	
//	LPCSTR  szCondition2Desc;	//	���������Ĳ���˵��
//	INT		nCondition21;		//	���������Ĳ���ֵ	
//	INT		nCondition22;		//	���������Ĳ���ֵ	
//	LPCSTR  szCondition3Desc;	//	���������Ĳ���˵��
//	INT		nCondition31;		//	���������Ĳ���ֵ	
//	INT		nCondition32;		//	���������Ĳ���ֵ	
//	LPCSTR	szCondition4Desc;	//	���������Ĳ���˵��
//	INT		nCondition41;		//	���������Ĳ���ֵ	
//	INT		nCondition42;		//	���������Ĳ���ֵ	
//	LPCSTR  szCondition5Desc;	//	���������Ĳ���˵��
//	INT		nCondition51;		//	���������Ĳ���ֵ	
//	INT		nCondition52;		//	���������Ĳ���ֵ	
//	LPCSTR  szCondition6Desc;	//	����˵��
//	    
//	// ������
//	_DBC_SKILLDATA_V1_DATA_PAIR	paraPair[12];
//
//	LPCSTR Skill_Desc_Interface;
//
//};//$$

/////////////////////////////////////////////////////////////////////
// ��������						(MissionKind)
/////////////////////////////////////////////////////////////////////
#define DBC_MISSION_KIND			(911)
struct _DBC_MISSION_KIND
{
	INT		nIndex;					// �������������
	LPCSTR	szKindName;				// �������������
};

// ����NPC��Ϣ
#define DBC_MISSION_INFO		(930)
struct _DBC_MISSION_INFO
{
	INT		nIndex;
	LPCSTR	nNpcName;
	LPCSTR	szDefault;
	INT		nScript;
	LPCSTR	szEventList;
	LPCSTR	szEventList1;
	LPCSTR	szEventList2;
	LPCSTR	szEventList3;
	LPCSTR	szEventList4;
};


/////////////////////////////////////////////////////////////////////
// �������ɫת���ַ�
/////////////////////////////////////////////////////////////////////
#define DBC_STRING_DICT			( 912 )
struct _DBC_STRING_DICT		// bc
{
	INT		nID;			// ID
	LPCSTR  szString;		// �ַ���
	LPCSTR  szKey;			// �ؼ���
};


/////////////////////////////////////////////////////////////////////
// ������Ч�б�
/////////////////////////////////////////////////////////////////////
#define DBC_ENV_SOUND			( 913 )
struct _DBC_ENV_SOUND
{
	INT		nID;			// ID
	UINT	nSoundID;		// ��ЧID
	UINT	nXPos;			// ��Դ��X����
	UINT	nZPos;			// ��Դ��Z����
	UINT	nDistance;		// ������������������
	INT		n1;				// ��������
	INT		n2;				// ���ʱ��(ms)
	INT		n3;				// ��һ���������ʱ��(ms)
};

/////////////////////////////////////////////////////////////////////
// ��Ʒ�����
/////////////////////////////////////////////////////////////////////
#define DBC_ITEM_RULE			( 914 )
struct _DBC_ITEM_RULE		// bc
{
	INT		nID;			// ID
	LPCSTR  szName;         //�������� �������� �߻���
	INT		bCanOverLay;	// �Ƿ���ص�
	INT		bCanDrop;		// �Ƿ�ɶ���
	INT		bCanQuick;		// �Ƿ�ɷ�������
	INT		bCanSale;		// �Ƿ���Գ��۸�NPC�̵�
	INT		bCanExchange;	// �Ƿ���Խ���
	INT		bCanUse;		// �Ƿ����ʹ��
	INT		bPickBind;		// �Ƿ���ʰȡ�
	INT		bEquipBind;		// �Ƿ���װ���
	INT		bUnique;		// �Ƿ���Ψһ 
	INT     bCheck;			//�Ƿ���Ҫ����
	INT     bUnRealItem;    //�Ƿ����������
	INT     bAccessInBack;	//�Ƿ�ɴ�������
	INT     bWearOut;		//�Ƿ�����	     		
};

/////////////////////////////////////////////////////////////////////
// ���˴ʴʻ��
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_FILTER			( 915 )
struct _DBC_TALK_FILTER
{
	INT		nID;				// ID
	LPCSTR	szString;			// ���˴ʻ㣨��������˵�Ĵʻ㣩

	// ���˹���
	// 0��ʾ���ԣ�1��ʾ���о͹��ˣ�2��ʾ��ȫƥ�����
	INT		chatFilter;			// ����ʱ����
	INT		roleFilter;			// ��������ʱ����
	INT		nameFilter;			// ����ʱ����
	INT		stallFilter;		// ��̯ʱ����
};

/////////////////////////////////////////////////////////////////////
// �ȼ���Ǯ��Ӧ��
/////////////////////////////////////////////////////////////////////
#define DBC_LV_MAXMONEY			( 916 )
struct _DBC_LV_MAXMONEY		// bc
{
	INT		nID;			// ID
	INT		nMaxMoney;		// ����Ǯ
	INT		nLv;			// �ȼ�
};

/////////////////////////////////////////////////////////////////////
// ���춯�������
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_ACT			( 918 )
struct _DBC_TALK_ACT	// ef
{
	INT		nID;			// ID
	LPCSTR	szCmd;			// ����
	LPCSTR	szNobody;		// û��Ŀ��ʱ���ִ�
	LPCSTR	szMyself;		// Ŀ���Լ�ʱ���ִ�
	INT		n0;				// ά��״̬��
	LPCSTR	szOther;		// Ŀ�����������ʱ���ִ�
	LPCSTR	szAct;			// ����ʼ���յ�������Ϣʱ��Ҫ���Ķ������
	LPCSTR	szActPersist;	// ��������
	LPCSTR	szActEnd;		// ��������

	LPCSTR	szIconName;		//ͼ��
	LPCSTR	szToolTip;		//��ʾ��Ϣ
};

/////////////////////////////////////////////////////////////////////
// �����������ñ�
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_CONFIG			( 919 )
struct _DBC_TALK_CONFIG		// cd
{
	INT		nID;				// ID ����ENUM_CHAT_TYPE
	LPCSTR	szChannelName;		// Ƶ������
	INT		nTimeSlice;			// ʱ��������λ���룩
	LPCSTR	szChannelHeader;	// Ƶ��ǰ���ַ�
	INT		nNeedType1;			// ��������1 ����CHAT_NEED_TYPE
	INT		nNeedValue1;		// ������ֵ1
	INT		nNeedType2;			// ��������2 ����CHAT_NEED_TYPE
	INT		nNeedValue2;		// ������ֵ2
	INT		nNeedType3;			// ��������3 ����CHAT_NEED_TYPE
	INT		nNeedValue3;		// ������ֵ3
	INT		nNeedLv;			// �ȼ����ƣ������Ƶ�����Ͷ��ټ������ڴ�Ƶ������
};

/////////////////////////////////////////////////////////////////////
// ���͸ı����ı�
/////////////////////////////////////////////////////////////////////
#define DBC_HAIR_STYLE			( 920 )
struct _DBC_HAIR_STYLE	// bc
{
	INT		nID;			// ID����
	INT		nItemTableID;	// ������ƷID
	INT		nRaceID;		// �Ա�����
	INT		nItemCount;		// ������Ʒ����
};

/////////////////////////////////////////////////////////////////////
// ����ͷ�����ݱ�
/////////////////////////////////////////////////////////////////////
#define DBC_MONSTER_PAOPAO		( 921 )
struct _DBC_MONSTER_PAOPAO
{
	INT		nID;			// ID����
	LPCSTR	szPaoPaoTxt;	// ����Ҫ˵��ͷ������
};


#define DBC_CAMP_AND_STAND		(922)	//��Ӫ


/////////////////////////////////////////////////////////////////////
// ������Ϣ��ɫ��
/////////////////////////////////////////////////////////////////////
#define DBC_SYSTEM_COLOR		(923)
struct _DBC_SYSTEM_COLOR	// bc
{
	const static INT NAME_PLAYER_MYSELF		 = 1;	// ����_����
	const static INT NAME_PLAYER_OTHER		 = 2;	// ����_�������
	const static INT NAME_NPC_PEACE			 = 3;	// ����_��ƽNPC
	const static INT NAME_NPC_ATTACK_ACTIVE	 = 4;	// ����_��������NPC
	const static INT NAME_NPC_ATTACK_PASS	 = 5;	// ����_����������NPC
	const static INT NAME_NPC_CANNT_ATTACK	 = 6;	// ���ɹ�����NPC
	const static INT NAME_PET				 = 7;	// ��������
	const static INT NANE_SELF_PET			 = 8;	// �Լ��ĳ���

	const static INT TITLE_NORMAL			 = 100;	// �ƺ�_��ͨ
	const static INT TITLE_BANGPAI			 = 101;	// �ƺ�_����ְ��
	const static INT TITLE_MOOD				 = 102;	// �ƺ�_����Զ�������
	const static INT TITLE_PLAYER_SHOP		 = 103;	// �ƺ�_����̵�

	const static INT UI_DEBUGLISTBOX		 = 1000; // debuglistbox����ɫ

	// 20100513 AddCodeBegin
	// �����������������Ｖ����ʾ��������ɫ
	// �����=�Է�����-��Ҽ���
	const static INT NAME_DIFF_1			 = 201;	// �����>=20    (���)
	const static INT NAME_DIFF_2			 = 202;	// �����[5,20)  (��)
	const static INT NAME_DIFF_3			 = 203;	// �����(-5,5)  (��)
	const static INT NAME_DIFF_4			 = 204;	// �����(-20,-5](��)
	const static INT NAME_DIFF_5			 = 205;	// �����<= -20  (��)
	// 20100513 AddCodeEnd

	INT		m_nID;
	LPCSTR	strFontColor;
	LPCSTR  strDesc;
	LPCSTR	strExtenColor;
};

/////////////////////////////////////////////////////////////////////
// ��ȫƥ����˱����ڴ�����ᡢ������֮��ĵط�������ʹ��ϵͳ����
/////////////////////////////////////////////////////////////////////
#define DBC_FULLCMP_FILTER		( 924 )
struct _DBC_FULLCMP_FILTER	// bc
{
	INT		nID;			// ID����
	LPCSTR	szFullTxt;		// ��ȫ���δ�
	LPCSTR	szFilterType;	// ����ʲô�ط������磺all,guild,team,.....��ʾ�������еط�������ᣬ����ӵȵȡ�
};

/////////////////////////////////////////////////////////////////////
// ����������Ҫ�ľ����
/////////////////////////////////////////////////////////////////////
#define DBC_PET_LEVELUP			( 925 )
struct _DBC_PET_LEVELUP
{
	INT		nID;			// ID����
	INT		nExp;			// ����Ҫ����
};

/////////////////////////////////////////////////////////////////////
// ����ѧϰ������
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_STUDY_DATA	(926)

/////////////////////////////////////////////////////////////////////
// �����Ʒ��
/////////////////////////////////////////////////////////////////////
#define DBC_MOUNT_ITEM_DATA		(927)
struct _DBC_MOUNT_ITEM_DATA
{
	UINT			nItemID;		// ��ƷID
	const CHAR	*	szItemDec;		// ��Ʒ˵��,�߻���
	INT				nType;			// ��Ʒ����
	INT				nRate;			// ���Ա���
	INT				nNum;			// ����ֵ
	INT				nSkillID;		// ����ID
	INT				nSucceedRate;	// �ɹ���

};

/////////////////////////////////////////////////////////////////////
// ���������
/////////////////////////////////////////////////////////////////////
#define DBC_MOUNT_BASE_DATA		(928)
struct _DBC_MOUNT_BASE_DATA
{
	INT				nMountID;			// ��˵�ID
	const CHAR	*	szMountName;		// �������
	INT				nLevel;				// ��˵ȼ�
	INT				nRequirementLevel;	// ���Я���ȼ�
	const CHAR	*	szChildID;			// ��ͯID
	const CHAR	*	szMatingID;			// ����ID
	INT				bMating;			// �Ƿ���Խ���
	INT				bAberrance;			// �Ƿ����
	INT				nDaiShu;			// ��ֳ����
	INT				nAttach;			// ��˵Ĺ���
	INT				nSurvivalTime;		// ������˴��ʱ��
	INT				nMinStr;			// ��ʼ��С����
	INT				nMaxStr;			// ��ʼ�������
	INT				nMinDex;			// ��ʼ��С����
	INT				nMaxDex;			// ��ʼ�������
	INT				nMinInt;			// ��ʼ��С����
	INT				nMaxInt;			// ��ʼ�������
	INT				nMinCon;			// ��ʼ��С����
	INT				nMaxCon;			// ��ʼ�������
	INT				nMinStrApt;			// ��ʼ��С��������
	INT				nMaxStrApt;			// ��ʼ�����������
	INT				nMinDexApt;			// ��ʼ��С��������
	INT				nMaxDexApt;			// ��ʼ�����������
	INT				nMinIntApt;			// ��ʼ��С��������
	INT				nMaxIntApt;			// ��ʼ�����������
	INT				nMinConApt;			// ��ʼ��С��������
	INT				nMaxConApt;			// ��ʼ�����������
	INT				nMinSavvy;			// ��ʼ��С����
	INT				nMaxSavvy;			// ��ʼ�������
	INT				nMinBasic;			// ��ʼ��С����
	INT				nMaxBasic;			// ��ʼ������
	INT				nMinGrowRate;		// ��ʼ��С�ɳ���
	INT				nMaxGrowRate;		// ��ʼ���ɳ���
	INT				nMoveSpeed;			// �ƶ��ٶ�
	INT				nModelID;			// ��Ӧ���֣ɣ�
	INT				nModelID_A;			// ��Ӧ����ID
	INT				nSkillNum;			// ��˼�����
	const CHAR	*	szSkill_1;			// ��˼���1
	const CHAR	*	szSkill_2;			// ��˼���2
	const CHAR	*	szSkill_3;			// ��˼���3
	const CHAR	*	szSkill_4;			// ��˼���4
	const CHAR	*	szSkill_5;			// ��˼���5
	const CHAR	*	szSkill_6;			// ��˼���6
	INT				nSavvySklll;		// ��������ܣɣ�

};

/////////////////////////////////////////////////////////////////////
// ÿ���������
/////////////////////////////////////////////////////////////////////
#define MAX_NPC_DIALOG_LEN 70
#define MAX_SCENE_LEN 10
#define MAX_MIS_NAME_LEN 24
#define MAX_MIS_TEXT_LEN 24			//��������
#define MAX_COUNTRY_NUM 4
#define GUILD_FAMILY_SIZE 99  
#define NPC_NAME_LEN 32

const INT MAX_LEVEL_NUM		= 100;
const INT MAX_DAY_TIPS_NUM	= 256;
const INT MONTH_WEEK_NUM	= 7;	//ÿ�����6��
const INT WEEK_DAY_NUM		= 7;	//ÿ�����7��

//�������Чʱ��Ƭ
struct VALID_TIME_SLICE
{
	tm		m_Begin;		//��ʼʱ��
	tm		m_End;			//����ʱ��
};


/////////////////////////////////////////////////////////////////////
// ÿ�����ѹ�������
/////////////////////////////////////////////////////////////////////
#define DBC_DAY_TIPS		(929)
struct _DBC_DAY_TIPS
{
	INT		m_Index;					//index
	BOOL	m_Used;						//�Ƿ��ѳ�ʼ��
	INT		m_DayNominate;				//���Ƽ�
	INT		m_MonthNominate;			//���Ƽ�
	INT     m_ShowAll;					//�Ƿ�ȫ���ɼ�	0Ϊ��1Ϊ�����˿ɼ�

	//�ɼ��ȼ�	������0��ʾ���еȼ��Ρ�
	//��������0ʱ���Ὣ��������Ԫ����1,�����о�û��0��.
	//�˺�0��ʾ���ɼ���1��ʾ�ɼ���
	INT     m_ShowLevel[MAX_LEVEL_NUM];	

	//�ɼ�����,	0��ʾ���й��ҡ�
	//����ͬm_ShowLevel
	INT		m_ShowCountry[MAX_COUNTRY_NUM];	

	//�ɼ����		0��ʾ���а��
	//����ͬm_ShowLevel
	INT		m_ShowGuild[GUILD_FAMILY_SIZE];	

	//�ɼ�����		0��ʾ�м���		
	INT		m_ShowFamily[GUILD_FAMILY_SIZE];		

	INT		m_MisType;					//��������		0��ʾѭ������1��ʾ��ʱ�淨��
	CHAR	m_MisName[MAX_MIS_NAME_LEN];//��������

	//������Ч�·�		0��ʾ�����£������ʾ������������·�
	INT		m_MisValidMonth[13];		//Ϊ���������±��ʾ�·ݣ�m_MisValidMonth[0]����, m_MisValidMonth[1]~[12]��ʾ�·�
	INT		m_WeekOrDay;				//���������ڻ�����������0��ʾ����

	//m_WeekOrDay=1ʱ��������Ч����,0��ʾ����ʱ��Ҫע��ÿ���²��Ƕ���31��
	INT    m_MisValidDate[32]; // m_MisValidDate[1]~[31] ��Ч

	//��������Ϊ��λ����
	//��һ���ֶα�ʾ�ڼ��ܣ�Ϊ0��ʾ������
	//�ڶ����ֶα�ʾ�ܼ���Ϊ0��ʾ����
	INT		m_MisValidWeek[MONTH_WEEK_NUM][WEEK_DAY_NUM];	

	//����ʼ��������ʱ��,Ϊ0��ʾ����ʱ
	VALID_TIME_SLICE	m_VailidTimeSlice;

	INT		m_MisPos;					//��ʾ��ȡ�������������ر������ű�λ��,Խ��Խ��һͷ��ˮ
	INT		m_MisCount;					//ÿ�տ�������
	CHAR	m_MisText[MAX_MIS_TEXT_LEN];//��������
	CHAR	m_NpcName[NPC_NAME_LEN];	//������
	INT		m_ScendId;					//OBJ���ڳ���ID
	CHAR	m_SceneName[MAX_SCENE_LEN];		
	INT		m_PosX;
	INT		m_PosZ;

	// �������������������������ţ���ӦScriptGlobal.lua��MissionData�и������ݵĶ���
	INT		m_DoneCountIndex;	    // ���ɴ����±� 
	INT		m_DoneCircleIndex;  	// ���ɻ����±�
	INT		m_LastDoneTimeIndex;    // ����һ�����ʱ���±�
	INT		m_LastAbandonTimeIndex; // ����һ�η���ʱ���±�
};


/////////////////////////////////////////////////////////////////////
// ���а��
/////////////////////////////////////////////////////////////////////
#define DBC_TOP_LIST		(931)
struct _DBC_TOP_LIST
{
	UINT			nID;		// ID
	const CHAR*		szName;		// ���а���
	const CHAR*		szType;		// ������(�����������)
	FLOAT			fRefresh;	// ˢ�¼��(Сʱ)
	INT				nListNum;	// ��������
};

/////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////
#define DBC_QUEST_LOG		(932)
struct _DBC_QUEST_LOG
{
	UINT			nScriptID;		// �ű� ScriptID
	INT				nMissionID;		// ���� MissionID
	const CHAR*		szName;			// ��������
	INT				nSceneID;		// �������ͼID
	const CHAR*		szNPCName;		// ������NPC����
	const CHAR*		szNPCCommit;	// ������NPC���� // 20100511 ModifyCode
	INT				nNPC_X;			// ������NPC_X����
	INT				nNPC_Z;			// ������NPC_Z����
	INT				nMinLevel;		// ��������͵ȼ�
	INT				nMaxLevel;		// ��������ߵȼ�

	// ǰ������ MissionID
	INT				nPrevMission1;	// ǰ������1 
	INT				nPrevMission2;	// ǰ������2 
	INT				nPrevMission3;	// ǰ������3 
	INT				nPrevMission4;	// ǰ������4 
	INT				nPrevMission5;	// ǰ������5 
};

/////////////////////////////////////////////////
//ģ�����ñ��
/////////////////////////////////////////////////
#define DBC_MODEL_PARAMETER (933)
struct _DBC_MODEL_PARAMETER
{
	INT		nID;				// Index
	LPCSTR	szModelName;		// Model Name
	FLOAT	nHeight;			// Height
	FLOAT	nDistance;			// Distance
};

/////////////////////////////////////////////////////////////////////
// ����¼������ݱ�
/////////////////////////////////////////////////////////////////////
#define DBC_SOUXIA_DATA			(934)
struct	_DBC_SOUXIA_DATA
{
	UINT			nID;			//���
	const CHAR*		szName;	//����
	INT				nNeedLevel;		//ʹ�õȼ�
	INT				nType;				//��������(	0.����¼���ߣ� 1.����¼�������	2.��ж���ߣ� 3.�������ߣ� 4.��ϵ���		)

	//����¼��������
	INT				nSouxiaLevel;	//����¼���ߵĵȼ�Ʒ��(0,1,2,3,4�ֱ�������ż����������м����߼�����������¼)
	INT				nSouxiaParam0_AllPageNum;		//����¼����ҳ������
	INT				nSouxiaParam1_SkillPageNum;		//����¼����ҳ��������
	INT				nSouxiaParam2_ArmPageNum;	//����¼�����䷽ҳ��������
	INT				nSouxiaParam3_PetPageNum;		//����¼����ҳ������
	INT				nSouxiaParam4_RidePageNum;	//����¼����ҳ������

	//����¼���������
	INT				nSouxiaUsbType;		//����¼��������ߵ�����(1,2,3,4 �ֱ��ʾ���ܣ������䷽�������ٻ��������ٻ�)
	const CHAR*		szSouxiaUsbParam0_SkillID;		//�洢�ļ��ܱ��
	const CHAR*		szSouxiaUsbParam1_ArmID;		//�洢�������䷽��ID
	const CHAR*		szSouxiaUsbParam2_PetID;		//�洢�������ٻ����ܵ�ID
	const CHAR*		szSouxiaUsbParam3_RideID;		//�洢�������ٻ����ܵ�ID

	//����¼��ж��������
	INT				nSouxiaStripLevel;	//����¼��ж���ߵ�Ʒ��
	INT				nSouxiaStripParam0_Skill;				//��ж���ܣ���ʾ������������ҳ
	INT				nSouxiaStripParam1_SkillOdds;		//��ж����ҳ�ĳɹ�����
	INT				nSouxiaStripParam2_Arm;			//��ж���ܣ���ʾ�������������䷽ҳ
	INT				nSouxiaStripParam3_ArmOdds;	//��ж�����䷽ҳ�ĳɹ�����
	INT				nSouxiaStripParam4_Pet;				//��ж���ܣ���ʾ�������������ٻ�����ҳ
	INT				nSouxiaStripParam5_PetOdds;		//��ж�����ٻ�����ҳ�ĳɹ�����
	INT				nSouxiaStripParam6_Ride;			//��ж���ܣ���ʾ�������������ٻ�����ҳ
	INT				nSouxiaStripParam7_RideOdds;	//��ж�����ٻ�����ҳ�ĳɹ�����

	//����¼������������
	INT				nSouxiaSmeltLevel;	//����¼�������ߵ�Ʒ�ʣ�0-4���ֱ��ʾ�ͼ����������м����߼��������������ߣ�
	INT				nSouxiaSmeltParam0_Skill;			//�������ܣ���ʾ������������ҳ
	INT				nSouxiaSmeltParam1_SkillOdds;	//��������ҳ�ĳɹ�����
	INT				nSouxiaSmeltParam2_Arm;			//�������ܣ���ʾ�������������䷽ҳ
	INT				nSouxiaSmeltParam3_ArmOdds;	//���������䷽ҳ�ĳɹ�����
	INT				nSouxiaSmeltParam4_Pet;			//�������ܣ���ʾ�������������ٻ�����ҳ
	INT				nSouxiaSmeltParam5_PetOdds;	//���������ٻ�ҳ�ĳɹ�����
	INT				nSouxiaSmeltParam6_Ride;			//�������ܣ���ʾ�������������ٻ�����ҳ
	INT				nSouxiaSmeltParam7_RideOdds;	//���������ٻ�����ҳ�ĳɹ�����

	INT				nSutrueOdds;		//����¼��ϵ��ߵĳɹ�����
	INT				nConjureNum;		//��ʾ��Ӧ������/����ٻ�����ʹ�õĴ���
};


/////////////////////////////////////////////////////////////////////
// ������תͼƬ���ݱ� 20100517 AddCode
/////////////////////////////////////////////////////////////////////
#define DBC_MAPPIC_DATA			(935)
struct	_DBC_MAPPIC_DATA
{
	static const int MAPPIC_SCENEID_COLUMN = 1;	// ���в���ʱ��Ĭ�ϲ��ҳ���ID��

	INT		nID;				// Index
	INT		nSceneID;			// ����ID
	LPCTSTR	szPicName;			// ��תͼƬ����
};

/////////////////////////////////////////////////////////////////////
// ������ת������ʾ���ݱ� 20100517 AddCode
/////////////////////////////////////////////////////////////////////
#define DBC_CHATHELP_DATA			(936)
struct	_DBC_CHATHELP_DATA
{
	static const int CHATHELP_ID_COLUMN = 0;

	INT		nID;				// Index
	LPCTSTR	szTip;				// ��ʾ����
};


//#define DBC_LOGINSERVER_DATA			(937)
struct	_DBC_LOGINSERVER_DATA
{
	INT	nID;
	const CHAR* szAreaName;		//��������
	const CHAR* szServerName;		//��������
	const CHAR* szTip;			//Tip˵��
	INT	nProvider;					//0��ͨ��1����
	INT	nServerAreaID;
	INT	nServerStatus;			//���������أ�0Ϊ������1Ϊ��æ��2Ϊ���ã�3Ϊ���ѣ�4Ϊά����10Ϊ���÷�����
	INT nTuiJian;						//�Ƿ�Ϊ�Ƽ�������
	INT	nIsNewServer;			//�Ƿ�Ϊ�¿�������
	const CHAR* szServerIP;	//LoginServer IP��Port
};

/** �����ٻ�������Ҫת�����ٻ��޵�ID */
#define DBC_IMPACT_SOUXIAMODEL		(938)
struct	_DBC_IMPACT_SOUXIAMODEL
{
	INT nID;				//Ч��ID
	INT nModelID;		//�ٻ���ID
};

#define DBC_BUS_DATA		(939)

#define DBC_IB_SHOP			(940)
struct	_DBC_IB_SHOP
{
	UINT			nID;		//�̳����
	UINT			nItemID;	//����ID
	const CHAR*		szDesc;		//����
	UINT			nPrice;		//�۸�
	INT				bIsNewSell; //�Ƿ�������Ʒ
	INT				bIsHotSell; //�Ƿ�����������Ʒ
	INT				bIsCutPrice;//�Ƿ����ڴ��۴���
	INT				CutPriceRate;//���۴�������
	INT				bIsPetShop;		//�Ƿ����ڳ�������
	INT				bIsEquipShop;	//�Ƿ�����װ������
	INT				bIsMedicShop;	//�Ƿ�����ҩƷ����
	INT				bIsSpecialShop;	//�Ƿ�������������
	INT				bIsColligateShop;//�Ƿ������ۺ�����
};

/////////////////////////////////////////////////////////////////////
// �������Ա�    [7/8/2010 �¾���] 
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_MOUNT_ATT		(941)
struct _DBC_ITEM_MOUNT_ATT		// ef
{
	INT		         nID;                 //id
	LPCSTR	         szName;              //����
	LPCSTR	         szDesc;              //����
	LPCSTR	         szIcon;              //icon
	INT		         nRule;		          //��Ӧ����
	INT              nNeedLevel;          //��Ҫ�ﵽ�ĵȼ�
	INT		         nAddtionSpeed;       //���ӵ��ٶ�
	INT              nLife;               //����ʱ�ޣ���λ���죩
	INT              nBindType;           //������ MOUNT_BIND_TYPE
	INT              nPrice;              //�����Ǯ
    INT              nCharMountID;        //����ID

	//... NOT CARE
};
