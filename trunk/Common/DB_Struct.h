

#ifndef __DBSTRUCT_H__
#define __DBSTRUCT_H__

 #include "Type.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "GameDefine_Attr.h"
#include "GameStruct_Impact.h"
#include "GameStruct_Skill.h"
#include "GameStruct_Country.h"

#pragma pack(push, 1)

class HumanDB;

//��ȴʱ��
typedef CooldownList_T<MAX_COOLDOWN_LIST_SIZE_FOR_HUMAN> CooldownListForHuman_T;
typedef CooldownListForHuman_T _COOLDOWN_DB_LOAD_FOR_HUMAN;
typedef CooldownList_T<MAX_COOLDOWN_LIST_SIZE_FOR_PET> CooldownListForPet_T;
typedef CooldownListForPet_T _COOLDOWN_DB_LOAD_FOR_PET;

// ��Ҷ�Ӧ�������ʱ����ŵĶ���������ͼ��ʾ��
//
// FF  FF  FF  FF
// |   |   |   |
//Ԥ�� |   |   |
//   ʡ��  |   |
//       ����  |
//           ����
//

struct POSTCODE
{
	INT m_PostCode;
	POSTCODE(){
		m_PostCode = 0;
	}
	INT GetPostCode() const{
		return m_PostCode;
	}
	BYTE GetProvinceCode() const {
		return (BYTE)((m_PostCode & 0x00FF0000) >> 16);
	}
	BYTE GetCityCode() const {
		return (BYTE)((m_PostCode & 0x0000FF00) >> 8);
	}
	BYTE GetSectionCode() const {
		return (BYTE)((m_PostCode & 0x000000FF));
	}
	VOID Assign(BYTE byProvinceCode, BYTE byCityCode, BYTE bySectionCode){
		m_PostCode = (INT)((byProvinceCode << 16) | (byCityCode << 8) | bySectionCode);
	}
	POSTCODE& operator=(const POSTCODE& value){
		this->m_PostCode = value.m_PostCode;
		return *this;
	}
	POSTCODE& operator=(const INT& value){
		this->m_PostCode = value;
		return *this;
	}

};
// HighSection����ҵ�GUID,LowSection�ǵ�ǰ��ϵͳʱ��
struct PET_GUID_t
{
	PET_GUID_t()
	{
		m_uHighSection = 0;
		m_uLowSection = 0;
	}

	PET_GUID_t& operator=(const PET_GUID_t& val) 
	{
		m_uHighSection = val.m_uHighSection;
		m_uLowSection = val.m_uLowSection;
		return *this; 
	}

	BOOL operator==(const PET_GUID_t& val) const 
	{
		if (m_uHighSection == val.m_uHighSection &&
			m_uLowSection == val.m_uLowSection)
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL operator!=(const PET_GUID_t& val) const 
	{
		if (m_uHighSection != val.m_uHighSection ||
			m_uLowSection != val.m_uLowSection)
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL IsNull(VOID) const 
	{
		if (!m_uHighSection && !m_uLowSection)
		{
			return TRUE;
		}
		return FALSE;
	}

	VOID Init(const GUID_t& GUID, UINT CurTime)
	{
		m_uHighSection = GUID;
		m_uLowSection = CurTime;
	}
	
	VOID Reset(VOID)
	{
		m_uHighSection = 0;
		m_uLowSection = 0;
	}
	
	UINT	GetHighSection(VOID) const { return m_uHighSection; }
	UINT	GetLowSection(VOID) const { return m_uLowSection; }

	UINT	m_uHighSection;
	UINT	m_uLowSection;
};

// ��Ӫ����
struct _CAMP_DATA
{
	CampID_t	m_nCampID;
	UINT		m_uActiveFlags;
	UINT		m_uRelationFlags;

	_CAMP_DATA( VOID )
	{
		m_nCampID			= -1;
		m_uActiveFlags		= 0;
		m_uRelationFlags	= 0;
	}

	VOID CleanUp( VOID )
	{
		m_nCampID			= -1;
		m_uActiveFlags		= 0;
		m_uRelationFlags	= 0;
	}

	BOOL IsActiveFlag_Dirty( INT nID )const
	{
		if ( nID < 0 || nID >= 32 )
		{
			return FALSE;
		}
		return (m_uActiveFlags & (1<<nID)) != 0;
	}

	VOID SetActiveFlag_Dirty( INT nID, BOOL bDirty )
	{
		if ( nID < 0 || nID >= 32 )
		{
			return ;
		}
		if ( bDirty )
			m_uActiveFlags |= (1<<nID);
		else
			m_uActiveFlags &= ~(1<<nID);
	}

	BOOL IsRelationFlag_Dirty( INT nID )const
	{
		if ( nID < 0 || nID >= 32 )
		{
			return FALSE;
		}
		return (m_uRelationFlags & (1<<nID)) != 0;
	}

	VOID SetRelationFlag_Dirty( INT nID, BOOL bDirty )
	{
		if ( nID < 0 || nID >= 32 )
		{
			return ;
		}
		if ( bDirty )
			m_uRelationFlags |= (1<<nID);
		else
			m_uRelationFlags &= ~(1<<nID);
	}

	BOOL operator == ( const _CAMP_DATA &in )
	{
		if ( m_nCampID != in.m_nCampID
			|| m_uActiveFlags != in.m_uActiveFlags
			|| m_uRelationFlags != in.m_uRelationFlags )
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL operator != ( const _CAMP_DATA &in )
	{
		if ( m_nCampID != in.m_nCampID
			|| m_uActiveFlags != in.m_uActiveFlags
			|| m_uRelationFlags != in.m_uRelationFlags )
		{
			return TRUE;
		}
		return FALSE;
	}

	_CAMP_DATA &operator = ( const _CAMP_DATA &in )
	{
		if ( &in != this )
		{
			m_nCampID			= in.m_nCampID;
			m_uActiveFlags		= in.m_uActiveFlags;
			m_uRelationFlags	= in.m_uRelationFlags;
		}
		return *this;
	}
};

//��ɫ�̵�����

#define MAX_RESERVE 10
//��Ҫ����Ľ�ɫ����
struct _HUMAN_DB_LOAD
{

	struct _PSHOP_INFO
	{
		_PLAYERSHOP_GUID	m_ShopGuid[MAX_SHOP_NUM_PER_PLAYER];//�Լ���ӵ�е�����̵�ĵ�GUID 0=��Ʒ������ 1=���������
		_PLAYERSHOP_GUID	m_FavoriteList[MAX_FAVORITE_SHOPS];	//�ղؼ��б�
		
		_PSHOP_INFO()
		{
			CleanUp();

		}

		VOID CleanUp()
		{
			memset(this,0,sizeof(*this));
		}
	};
	//
	//������Ϣ
	//
	BOOL			m_bFirstEnterScence;			//��һ�ν�����
	INT				m_PortraitID;					//��ɫͷ��
	CHAR			m_Name[MAX_CHARACTER_NAME];		//��ɫ����
	CHAR			m_Title[MAX_CHARACTER_TITLE];	//��ɫ����
	GUID_t			m_GUID;							//��ɫ��ȫΨһ��
	BYTE			m_Sex;							//��ɫ�Ա�
	UINT			m_CreateDate;					//��ɫ��������
	_TITLE			m_Titles;						//��ɫ����ϸ�ƺ�
	INT				m_Level;						//��ɫ�ȼ�
	INT				m_Vigor;						//����
	INT				m_MaxVigor;						//��������
	INT				m_VigorRegeneRate;				//�����ָ��ٶ�
	INT				m_Energy;						//����
	INT				m_MaxEnergy;					//��������
	INT				m_EnergyRegeneRate;				//�����ָ��ٶ�
	UINT			m_Exp;							//��ɫ����ֵ
	UINT			m_Money;						//��ɫ����
	UINT			m_Gold;		    				//Ԫ��
    UINT			m_Ticket;						//����ȯ
	INT				m_nGoodBadValue;				//�ƶ�ֵ
	CHAR			m_Passwd[MAX_PWD];				//��������
	UINT			m_uPwdDelTime;					//����ǿ�ƽ�������ʱ��
	UINT			m_HairColor;					//ͷ����ɫ	
	BYTE			m_FaceColor;					//������ɫ
	BYTE			m_HairModel;					//ͷ��ģ��
	BYTE			m_FaceModel;					//����ģ��
	INT				m_nMountID;				    	//����ID                                           [7/8/2010 �¾���]
	INT				m_nMountIndex;			    	//����ITEM����                                     [7/8/2010 �¾���]
	INT				m_nMountList[MAX_MOUNT_SIZE];   //ӵ�е�����ID                                     [7/14/2010 �¾���]

	//
	//����״̬
	//
	SceneID_t		m_StartScene;					//��ɫ���ڳ���
	SceneID_t		m_BakScene;						//��ɫ���ڱ��ݳ���
	GuildID_t		m_GuildID ;						//����
	INT     		m_FamilyID ;					//����
	INT     		m_nGuildOfficial ;				//����ְ
	INT     		m_nGP;	            			//��ṱ�׶�
    INT             m_nHonor;                       //  ����ֵ
	TeamID_t		m_TeamID;						//��ɫ���ڵĶ����
	WORLD_POS		m_Position;						//��ɫλ��
	WORLD_POS		m_BakPosition;					//��ɫ����λ��
	UINT			m_OnlineTime;					//������ʱ��
	UINT			m_LastLoginTime;				//���һ�ε���ʱ��
	UINT			m_LastLogoutTime;				//���һ�εǳ�ʱ��
	UINT			m_LeftDieTime;					//ʣ������ʱ��
	UINT			m_DBVersion;					//���ݿ�汾
	time_t			m_LeaveGuildTime;		        //�˳�ǰһ������ʱ��
	
	//
	//ս����Ϣ
	//
	_CAMP_DATA		m_CampData;					//��Ӫ
	INT				m_nCountry;					//����
	INT				m_MenPai;					//���� MENPAI_ATTRIBUTE
	INT				m_NeiShang;					//����
	INT				m_HP;						//����ֵ
	INT				m_MP;						//ħ��
	INT 			m_StrikePoint;				//������
	INT 			m_Rage;						//ŭ��
	INT				m_nPneuma;					//��ǰ��Ԫ��ֵ
	INT				m_nPKValue;					//ɱ��
	PET_GUID_t		m_guidCurrentPet;			//��ǰ����
	_ATTR_LEVEL1	m_BaseAttrLevel1;			//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	INT				m_Level1Points;				//һ������ʣ�����
	INT				m_RemainSkillPoints;		//ʣ�༼�ܵ�
	BOOL			m_bHorseMating;				//��ֳ���
	INT				m_S_SkillPoints[COMMON_S_NUMBER];	//ϵ���ܵ�
	//
	//����̵�
	//
	_PLAYERSHOP_GUID	m_ShopGuid[MAX_SHOP_NUM_PER_PLAYER];//�Լ���ӵ�е�����̵�ĵ�GUID 0=��Ʒ������ 1=���������
	_PLAYERSHOP_GUID	m_FavoriteList[MAX_FAVORITE_SHOPS];	//�ղؼ��б�
	POSTCODE			m_PostCode;				//��ҵ��ʱ��
	//��������
	INT					m_Reserve[MAX_RESERVE];

	//
	_HUMAN_DB_LOAD()
	{
		CleanUp( );
	};
	VOID			CleanUp( );

	UINT			GetCrc(BOOL bLog)
	{
		UINT Crc= 0;

		Crc+= ConvertUChar(m_GUID,bLog);
		Crc+= ConvertUChar(m_Level,bLog);
		Crc+= ConvertUChar(m_Exp,bLog);
		Crc+= ConvertUChar(m_Money,bLog);
		Crc+= ConvertUChar(m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR],bLog);
		//Crc+= ConvertUChar(m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_SPR],bLog);
		Crc+= ConvertUChar(m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON],bLog);
		Crc+= ConvertUChar(m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT],bLog);
		Crc+= ConvertUChar(m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX],bLog);
		Crc+= ConvertUChar(m_Level1Points,bLog);
		//Crc+= ConvertUChar(m_iYuanBao,bLog);
		//Crc+= ConvertUChar(m_nZengDian,bLog);
		//Crc+= ConvertUChar(m_nBuyYuanBao,bLog);

		return Crc;

	}

};

//�����ɫ��ӵ�е����������Ʒ��Ϣ
struct _BANK_DB_LOAD
{
	_ITEM			m_pItem[MAX_BANK_SIZE];
	INT				m_Count;
	BYTE			m_CurEndIndex;//���е�ǰ��С
	INT				m_Money;
    INT             m_Gold;
    INT             m_Ticket;

	_BANK_DB_LOAD( )
	{
		CleanUp( );
	};
	VOID			CleanUp( );
};

//�����ɫ��ӵ�е����ϵı�����Ϣ
struct _BAG_DB_LOAD
{
	friend	class	HumanDB;

	_ITEM				m_pItem[MAX_BAG_SIZE];
	
	//��������С
	INT					m_BaseBagSize;
	////�������С
	//INT					m_TaskBagSize;
	////���ϰ���С
	//INT					m_MatBagSize;

	INT					m_Count;

	_BAG_DB_LOAD( )
	{
		CleanUp( );
	};
	VOID				CleanUp();
	
private:
	const _ITEM*		GetBagDB(UINT	iBagIndex) const
	{
		Assert(iBagIndex<MAX_BAG_SIZE);
		return		&m_pItem[iBagIndex];
	}

	VOID				PutBagDB(const _ITEM* pItem,UINT iBagIndex)
	{
		Assert(iBagIndex<MAX_BAG_SIZE);
		Assert(pItem);

		memcpy(&m_pItem[iBagIndex],pItem,sizeof(_ITEM));
	}

	VOID				SwapBagDB(UINT iAIndex,UINT iBIndex)
	{
		Assert(iAIndex<MAX_BAG_SIZE);
		Assert(iBIndex<MAX_BAG_SIZE);
		
		SwapItem(&m_pItem[iAIndex],&m_pItem[iBIndex]);

	}

	BOOL				IncCount(UINT iBagIndex)
	{
		return	m_pItem[iBagIndex].IncCount();
	}

	BOOL				DecCount(UINT iBagIndex)
	{
		return	m_pItem[iBagIndex].DecCount();
	}

	VOID				SetItemBind(UINT iBagIndex,BOOL	bBind)
	{
		
		m_pItem[iBagIndex].SetItemBind(bBind);
		
	}

	VOID				SetItemDur(UINT BagIndex, INT Dur)
	{
		m_pItem[BagIndex].GetEquipData()->m_CurDurPoint = Dur; 
	}

};

//�����ɫ���ϵ�װ����Ϣ
struct _EQUIP_DB_LOAD
{
	_ITEM			m_pItem[HEQUIP_NUMBER];//װ����

	USHORT			m_Flags;	//�������ϵĵ�nλֵ�ͱ�ʾ��n��װ�������Ƿ���װ��

	_EQUIP_DB_LOAD( )
	{
		CleanUp( );
	};
	VOID			CleanUp( );
	
	BOOL			IsSet( HUMAN_EQUIP eEquip )	const
	{ 
		return (m_Flags & (1<<(INT)eEquip))?(TRUE):(FALSE);
	}
	
	VOID			Set( HUMAN_EQUIP eEquip, BYTE bMark )
	{
		if ( bMark )
			m_Flags |= (1<<(INT)eEquip);
		else
			m_Flags &= (~(1<<(INT)eEquip));
	}
	
	const _ITEM*	GetEquipDB(HUMAN_EQUIP	EquipPoint)
	{
		return &m_pItem[EquipPoint];
	}

	VOID			SetEquipDur(HUMAN_EQUIP	EquipPoint, INT Dur)
	{
		m_pItem[EquipPoint].GetEquipData()->m_CurDurPoint = Dur; 
	}


	VOID			PutEquipDB(const _ITEM*	pItem,HUMAN_EQUIP	EquipPoint)
	{
		Assert(pItem);
		
		if(m_pItem[EquipPoint].IsNullType())
			memcpy(&m_pItem[EquipPoint],pItem,sizeof(_ITEM));
		else
		{
			Assert(FALSE);
		}

	}


};
// add by gh for souxia 2010/05/12

struct	_SOUXIA_DB_LOAD
{
	SOUXIA_DATA		m_SouXiaData[MAX_SOUXIA_CONTAINER];

	_SOUXIA_DB_LOAD()
	{
		CleanUp();
	};

	void CleanUp()
	{
		for (int i=0; i<MAX_SOUXIA_CONTAINER; ++i)
		{
			m_SouXiaData[i].CleanUp();
		}
	}
};
typedef struct _SOUXIA_DB_LOAD _SOUXIA_LIST;


// end of add 

//�����ɫ��������Ϣ  [7/8/2010 �¾���]
struct _MOUNT_DB_LOAD
{
    _ITEM  m_pItem[MAX_MOUNT_SIZE];

	_MOUNT_DB_LOAD( )
	{
		CleanUp( );
	};

	VOID  CleanUp( );
};

struct _ABILITY_DB_LOAD
{
	CHAR							m_aPrescr[MAX_CHAR_PRESCRIPTION_BYTE];
	_OWN_ABILITY					m_aABility[MAX_CHAR_ABILITY_NUM];

	_ABILITY_DB_LOAD()
	{
		CleanUp();
	}

	_OWN_ABILITY					&Get_Ability(AbilityID_t id)
	{
		INT idx;
		idx = (INT)id;
		Assert(idx>-1 && idx<MAX_CHAR_ABILITY_NUM);
		return m_aABility[idx];
	}

	_OWN_ABILITY					Get_Ability(AbilityID_t id) const
	{
		INT idx;
		idx = (INT)id;
		Assert(idx>-1 && idx<MAX_CHAR_ABILITY_NUM);
		return m_aABility[idx];
	}

	VOID	CleanUp();
};
typedef struct _ABILITY_DB_LOAD _ABILITY_LIST;

struct _SKILL_DB_LOAD
{
	BYTE			m_Count;
	_OWN_SKILL		m_aSkill[MAX_CHAR_SKILL_NUM];

	_SKILL_DB_LOAD( )
	{
		CleanUp( );
	}
	VOID			CleanUp( );
};
typedef struct _SKILL_DB_LOAD _SKILL_LIST;

//�ٻ��޿��ר�ã���Ҫд���ݿ�
struct _Monster_Pet_DB
{
	_Monster_Pet_DB_Struct m_aMonsterPet[MAX_MONSTER_PET_TAKE];
	_Monster_Pet_DB()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		memset( this, -1, sizeof(_Monster_Pet_DB) );
	}
};
typedef struct _Monster_Pet_DB _MONSTER_PET_DB;

struct _XINFA_DB_LOAD
{
	BYTE			m_Count;
	_OWN_XINFA		m_aXinFa[MAX_CHAR_XINFA_NUM];

	_XINFA_DB_LOAD( )
	{
		CleanUp( );
	}
	VOID			CleanUp( );
};
typedef struct _XINFA_DB_LOAD _XINFA_LIST;

typedef struct _IMPACT_DB_LOAD
{
	BYTE			m_Count;
	_OWN_IMPACT		m_aImpacts[MAX_IMPACT_NUM];

	_IMPACT_DB_LOAD( )
	{
		CleanUp( );
	};
	VOID			CleanUp( );
} _IMPACT_LIST;

// ����DB
#define PET_MAX_SKILL_COUNT 12
//(PET_SKILL_INDEX_NUMBERS)

struct _PET_SKILL
{
	SkillID_t		m_nSkillID;
	BOOL			m_bCanUse;

	_PET_SKILL( VOID )
	{
		m_nSkillID	= INVALID_ID;
		m_bCanUse	= FALSE;
	}

	VOID CleanUp( VOID );
};

struct _PET_DB_LOAD
{
	enum PET_TYPE
	{
		PET_TYPE_INVALID	= -1,
		PET_TYPE_BABY		= 0,
		PET_TYPE_VARIANCE,
		PET_TYPE_WILENESS,
	};

	enum PET_AI_TYPE
	{
		PET_AI_INVALID = -1,
		PET_AI_COWARDICE = 0,	// ��С
		PET_AI_WARINESS,		// ����	
		PET_AI_LOYALISM,		// �ҳ�
		PET_AI_CANNINESS,		// ����
		PET_AI_VALOUR,			// ����
	};

	PET_GUID_t		m_GUID;								// Ψһ���
	PET_GUID_t		m_SpouseGUID;						// ��żΨһ���
	INT				m_iSex;								// �Ա�
	INT				m_nDataID;							// ����ģ��
	CHAR			m_szName[MAX_CHARACTER_NAME];		// ����
	CHAR			m_szNick[MAX_NICK_NAME];			// �ǳ�
	INT				m_nLevel;							// �ȼ�
	INT				m_nTakeLevel;						// Я���ȼ�(���ٵȼ�����ҿ���Я���˳���)
	INT				m_nAttackType;						// �������ͣ���/����
	INT				m_AIType;							// AI����
	_CAMP_DATA		m_CampData;							// ��Ӫ

	BYTE			m_byGeneration;						// ������
	BYTE			m_byHappiness;						// ���ֶ�

	INT				m_iSavvy;							// ����
	INT				m_nGenGu;							// ����
	FLOAT			m_fGrowRate;						// �ɳ���

	INT				m_iStrengthPerception;				// ��������
	INT				m_iSmartnessPerception;				// ��������
	INT 			m_iMindPerception;					// ��������
	INT 			m_iConstitutionPerception;			// ��������

	INT				m_nRemainPoint;						// һ������ʣ�����
	INT				m_nExp;								// ����ֵ
	BOOL			m_bCheckup;							// �Ƿ����
	INT				m_iMatingLevel;						// ��һ�ν���ʱ�ĵȼ�,0����û�н����		

	_ATTR_LEVEL1	m_BaseAttrLevel1;					// ����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�

	_PET_SKILL		m_SkillList[PET_MAX_SKILL_COUNT];	// ���＼���б�,ǰ6���������ۣܲ���6����ѧϰ��

	_COOLDOWN_DB_LOAD_FOR_PET m_CooldownList;			//�������ȴ�б�
	BOOL			m_bMating;							//�Ƿ��ڽ���״̬
	UINT			m_uMatingStartTime;					//���俪ʼʱ��,5Сʱ�������ȡ,48Сʱ����ȡ��ɾ��
	BOOL			m_bMatingMsgSend;					//�Ƿ��ѷ�ֳ��ɣ�ͬʱ����Ƿ��ͷ�ֳ�ɹ��ʼ�
	
	_PET_DB_LOAD()
	{
		 CleanUp();
	}

	VOID CleanUp( VOID );
};

typedef _PET_DB_LOAD	_PET_DB;


#define HUMAN_PET_MAX_COUNT	(6)

enum 
{
	PET_INDEX_INVALID	= -1,
	PET_INDEX_SELF_BEGIN,	// �Լ����ϵĵ�һֻ
	PET_INDEX_SELF_2,		// �Լ����ϵڶ�ֻ
	PET_INDEX_SELF_3,		// �Լ����ϵ���ֻ
	PET_INDEX_SELF_4,		// �Լ����ϵ���ֻ
	PET_INDEX_SELF_5,		// �Լ����ϵ���ֻ
	PET_INDEX_SELF_6,		// �Լ����ϵ���ֻ

	PET_INDEX_SELF_NUMBERS,

	TARGETPET_INDEX		=100,		//
/*
	PET_INDEX_EXCHANGE_BEGIN  = 100,	// ����ʱ�ĵ�һֻ
	PET_INDEX_EXCHANGE_2,			// ����ʱ�ĵڶ�ֻ
	PET_INDEX_EXCHANGE_3,			// ����ʱ�ĵ���ֻ
	PET_INDEX_EXCHANGE_4,			// ����ʱ�ĵ���ֻ
	PET_INDEX_EXCHANGE_5,			// ����ʱ�ĵ���ֻ

	PET_INDEX_EXCHANGE_NUMBERS,

	PET_INDEX_STALL_BEGIN  = 200,	// ��̯ʱ�ĵ�һֻ
	PET_INDEX_STALL_2,				// ��̯ʱ�ĵڶ�ֻ
	PET_INDEX_STALL_3,				// ��̯ʱ�ĵ���ֻ
	PET_INDEX_STALL_4,				// ��̯ʱ�ĵ���ֻ
	PET_INDEX_STALL_5,				// ��̯ʱ�ĵ���ֻ

	PET_INDEX_STALL_NUMBERS,

	PET_INDEX_PLAYERSHOP_BEGIN  = 300,		// ����̵��е�
	PET_INDEX_PLAYERSHOP_2,					// ����̵�ĵڶ�ֻ
	PET_INDEX_PLAYERSHOP_3,					// ����̵�ĵ���ֻ
	PET_INDEX_PLAYERSHOP_4,					// ����̵�ĵ���ֻ
	PET_INDEX_PLAYERSHOP_5,					// ����̵�ĵ���ֻ

	PET_INDEX_PLAYERSHOP_NUMBERS,
*/
};

struct _PET_DB_LIST_LOAD
{
	// GUIDΪ��Чʱ������λΪ��
	_PET_DB_LOAD	m_aPetDB[HUMAN_PET_MAX_COUNT];

	VOID CleanUp( VOID );
};
typedef _PET_DB_LIST_LOAD	_PET_DB_LIST;

#define MAX_MISSION_PARAM_NUM (8)
struct _OWN_MISSION
{
	MissionID_t		m_idMission;
	ScriptID_t		m_idScript;
	// 0x0000|PetChanged�¼�|ItemChanged�¼�|EnterZone�¼�|KillObject�¼�|
	BYTE			m_yFlags;			
	
	union
	{
		UINT	m_aParam[MAX_MISSION_PARAM_NUM];
		FLOAT	m_afParam[MAX_MISSION_PARAM_NUM];
		INT		m_anParam[MAX_MISSION_PARAM_NUM];
		BOOL	m_abParam[MAX_MISSION_PARAM_NUM];
		VOID	*m_apParam[MAX_MISSION_PARAM_NUM];
	};

	VOID Cleanup( VOID )
	{
		m_idScript	= INVALID_ID;
		m_idMission = INVALID_ID;
		m_yFlags	= 0;
		memset( m_aParam, 0, sizeof ( m_aParam ) );
	}

	BOOL IsActiveMission( )const{
		if( m_idMission != INVALID_ID &&
			m_idScript != INVALID_ID )
			return TRUE ;
		else
			return FALSE ;
	};

	//kill obj event
	VOID SetFlags_KillObject( BOOL bSet ){
		( bSet )?(m_yFlags |= 0x00000001):(m_yFlags &= ~0x00000001);
	}
	BOOL IsFlags_KillObject( VOID )const{
		return (m_yFlags & 0x00000001) != 0;
	}
	//enter area event
	VOID SetFlags_EnterArea( BOOL bSet ){
		( bSet )?(m_yFlags |= (0x00000001<<1)):(m_yFlags &= ~(0x00000001<<1));
	}
	BOOL IsFlags_EnterArea( VOID )const{
		return (m_yFlags & (0x00000001<<1)) != 0;
	}
	//item changed event
	VOID SetFlags_ItemChanged( BOOL bSet ){
		( bSet )?(m_yFlags |= (0x00000001<<2)):(m_yFlags &= ~(0x00000001<<2));
	}
	BOOL IsFlags_ItemChanged( VOID )const{
		return (m_yFlags & (0x00000001<<2)) != 0;
	}
	//pet changed event
	VOID SetFlags_PetChanged( BOOL bSet ){
		( bSet )?(m_yFlags |= (0x00000001<<3)):(m_yFlags &= ~(0x00000001<<3));
	}
	BOOL IsFlags_PetChanged( VOID )const{
		return (m_yFlags & (0x00000001<<3)) != 0;
	}
};

struct _MISSION_DB_LOAD
{
	BYTE			m_Count;
	_OWN_MISSION	m_aMission[MAX_CHAR_MISSION_NUM];//��ɫ���ӵ�������Ϣ
	UINT			m_aMissionHaveDoneFlags[MAX_CHAR_MISSION_FLAG_LEN];//��ɫ��������ɱ�־
	INT				m_aMissionData[MAX_CHAR_MISSION_DATA_NUM] ;//��ɫ���ϵ������Զ�������

	_MISSION_DB_LOAD( )
	{
		CleanUp( );
	}
	VOID			CleanUp( );
};
typedef struct _MISSION_DB_LOAD _MISSION_LIST;

struct _OWN_RELATION
{
	RELATION_MEMBER		m_Member;
	INT					m_FriendPoint;		//�Ѻö�
	BYTE				m_Type;				//��ϵ��־, ��enum RELATION_TYPE
	BYTE				m_Group;			//���ڵ���
	CHAR				m_GroupName[MAX_GROUP_NAME];

	_OWN_RELATION( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_Member.CleanUp( ) ;
		m_FriendPoint = 0 ;
		m_Type = RELATION_TYPE_NONE ;
		m_Group = 0;
		memset(m_GroupName,0,MAX_GROUP_NAME);
	};
	VOID Init(const _OWN_RELATION* pRelation)
	{
		Assert(pRelation);
		memcpy( (void*)&m_Member, &(pRelation->m_Member), sizeof(m_Member) );
		m_FriendPoint = pRelation->m_FriendPoint;
		m_Type = pRelation->m_Type;
		m_Group = pRelation->m_Group;
		memcpy(m_GroupName,pRelation->m_GroupName,sizeof(m_GroupName));
	}
};

struct _RELATION_DB_LOAD
{
	//��������
	struct RelationData
	{
		UCHAR			m_aRelationCount[RELATION_GROUP_NUMBER];		// �����������
	
		// �Զ����������
		CHAR			m_szName[RELATION_GROUP_F5][MAX_GROUP_NAME];	

		CHAR			m_szMood[MOOD_DATA_SIZE];						// ����
		MarriageInfo		m_MarriageInfo;								// ������Ϣ
		PrenticeInfo		m_PrenticeInfo;								// ͽ����Ϣ
		MasterInfo			m_MasterInfo;								// ʦ����Ϣ

		RelationData()
		{
			for( int i=0;i<RELATION_GROUP_NUMBER;i++)
				m_aRelationCount[i] = 0;
		
			memset((void*)m_szName, 0, sizeof(m_szName));		
			memset((void*)m_szMood, 0, sizeof(m_szMood));
			m_MarriageInfo.CleanUp() ;
			m_PrenticeInfo.CleanUp() ;
			m_MasterInfo.CleanUp() ;
		}
	};

	_OWN_RELATION		m_aRelation[MAX_RELATION_SIZE];				// ��ϵ�����ݳ�
	UCHAR				m_aRelationCount[RELATION_GROUP_NUMBER];	// �����������
	// �Զ����������
	CHAR				m_szName[RELATION_GROUP_F5][MAX_GROUP_NAME];
	CHAR				m_szMood[MOOD_DATA_SIZE];					// ����
	MarriageInfo		m_MarriageInfo;								// ������Ϣ
	PrenticeInfo		m_PrenticeInfo;								// ͽ����Ϣ
	MasterInfo			m_MasterInfo;								// ʦ����Ϣ

	_RELATION_DB_LOAD( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( ) ;


	RelationData	GetRelationData()
	{
		RelationData	ReData;

		memcpy(ReData.m_aRelationCount,m_aRelationCount,sizeof(UCHAR)*RELATION_GROUP_NUMBER);	// �����������
		
		size_t len = sizeof(CHAR)*(RELATION_GROUP_F5)*MAX_GROUP_NAME;
		memcpy(ReData.m_szName,m_szName,len);	

		memcpy(ReData.m_szMood,m_szMood,sizeof(CHAR)*MOOD_DATA_SIZE);			// ����
		memcpy(&ReData.m_MarriageInfo,&m_MarriageInfo,sizeof(MarriageInfo));	// ������Ϣ
		memcpy(&ReData.m_PrenticeInfo,&m_PrenticeInfo,sizeof(PrenticeInfo));	// ͽ����Ϣ
		memcpy(&ReData.m_MasterInfo,&m_MasterInfo,sizeof(MasterInfo));			// ʦ����Ϣ
		return ReData;
	}
	
	VOID	SetRelationData(RelationData& ReData)
	{
		memcpy(m_aRelationCount,ReData.m_aRelationCount,sizeof(UCHAR)*RELATION_GROUP_NUMBER);	// �����������
	
		size_t len = sizeof(CHAR)*(RELATION_GROUP_F5)*MAX_GROUP_NAME;
		memcpy(m_szName,ReData.m_szName,len);	

		memcpy(m_szMood,ReData.m_szMood,sizeof(CHAR)*MOOD_DATA_SIZE);			// ����
		memcpy(&m_MarriageInfo,&ReData.m_MarriageInfo,sizeof(MarriageInfo));	// ������Ϣ
		memcpy(&m_PrenticeInfo,&ReData.m_PrenticeInfo,sizeof(PrenticeInfo));	// ͽ����Ϣ
		memcpy(&m_MasterInfo,&ReData.m_MasterInfo,sizeof(MasterInfo));			// ʦ����Ϣ

	}
};

#define MAX_SETTING_DATA 64
//�ı�_OWN_SETTING�ṹ�����Ⱥ�������ۣ�лл������
struct _OWN_SETTING
{
	BYTE				m_SettingType ;
	INT					m_SettingData ;

	_OWN_SETTING( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		m_SettingType = 0 ;
		m_SettingData = 0 ;
	}
};

#define MAX_SUIT_SETTING 10
#define MAX_SUIT_NAME_SIZE 12
struct _SUIT_SETTING
{
	CHAR				m_SuitName[MAX_SUIT_NAME_SIZE];
	_ITEM_GUID			m_EquipData[HEQUIP_NUMBER];

	_SUIT_SETTING( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		memset(m_SuitName, 0, sizeof(CHAR)*MAX_SUIT_NAME_SIZE);
		memset(m_EquipData, 0, sizeof(_ITEM_GUID)*HEQUIP_NUMBER);
	}
};

struct _SUBTRAIN_SETTING 
{
	BOOL			m_bSubTrainOpen;					//��������״̬
	INT				m_SubTrainMode;						//����ģʽ
	UINT			m_SubTrainBeginTime;				//������ʼʱ��
	UINT			m_SubTrainMoney;					//��Ǯ����
	UINT			m_SubTrainExper;					//�������
	UINT			m_SubTrainExpRate;					//��������Ч��
	UINT			m_SubTrainMoneyRate;				//������ǮЧ��
	_SUBTRAIN_SETTING( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		m_bSubTrainOpen = FALSE;
		m_SubTrainMode  = SUBTRAIN_MODE1;
		m_SubTrainBeginTime = 0;
		m_SubTrainMoney	= 0;
		m_SubTrainExper = 0;
		m_SubTrainExpRate  = 0;
		m_SubTrainMoneyRate = 0;
	}
};

struct _SETTING_DB_LOAD
{
	_OWN_SETTING		m_aSetting[MAX_SETTING_DATA] ;//����ֵΪ��SETTING_TYPE
	_SUIT_SETTING		m_SuitSetting[MAX_SUIT_SETTING];
	_SUBTRAIN_SETTING   m_SubTrainSetting;

	_SETTING_DB_LOAD( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		for( INT i=0; i<MAX_SETTING_DATA; i++ )
		{
			m_aSetting[i].CleanUp() ;
		}

		for(INT i=0; i<MAX_SUIT_SETTING; ++i)
		{
			m_SuitSetting[i].CleanUp();
		}

		m_SubTrainSetting.CleanUp();
	}
};

typedef struct _SETTING_DB_LOAD _SETTING_LIST;

struct _PRIVATE_INFO_DB_LOAD 
{
	BYTE		m_InfoType;			//���̶ܳ�
	UINT		m_CharGUID;			//��ɫ���
	BYTE		m_Age;				//����
	BYTE		m_Sex;				//�Ա�
	CHAR		m_JobInfo[MAX_JOBINFO_LENGTH];		//ְҵ��Ϣ
	CHAR		m_SchoolInfo[MAX_SCHOOLINFO_LENGTH];//��ҵԺУ
	BYTE		m_BloodType;		//Ѫ��
	BYTE		m_YearAnimal;		//��Ф
	BYTE		m_Consella;			//����
	BYTE		m_Province;			//ʡ��
	CHAR		m_City[MAX_CITY_LENGTH];	//����
	CHAR		m_EmailInfo[MAX_EMAIL_LENGTH];//�ʼ�
	CHAR		m_LuckWord[MAX_LUCKWORD_LENGTH];//�������

	_PRIVATE_INFO_DB_LOAD()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		memset(this,0,sizeof(*this));
		m_CharGUID = INVALID_ID ;
	}
};

struct HUMAN_DB_ATTR_FLAG
{
	HUMAN_DB_ATTR_FLAG()
	{
		CleanUp();
	}

	VOID		CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	

	BOOL		IsUpdated(INT BitOffSet)
	{
		if(m_Flags[BitOffSet/8] & (1<<(BitOffSet%8)))
			return TRUE;
		return FALSE;
	}
	VOID		SetBitFlag(INT BitOffSet,BOOL	bValue)
	{
		if ( bValue )
			m_Flags[BitOffSet/8] |= (1<<(BitOffSet%8));
		else
			m_Flags[BitOffSet/8] &= (~(1<<(BitOffSet%8)));
	}
	
	CHAR		m_Flags[HUMAN_DB_ATTR_FLAG_LENGTH];
};

struct HUMAN_DB_ATTR_REG
{
	HUMAN_DB_ATTR_REG()
	{
		CleanUp();
	}
	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	VOID*							m_AttrPtr[CATTR_DB_NUMBER];	//��ɫ����ע��
	INT								m_AttrSize[CATTR_DB_NUMBER];//��ɫ���Դ�С
};

struct FULLUSERDATA
{
	FULLUSERDATA()
	{
		CleanUp();
	}
	_HUMAN_DB_LOAD				m_Human;
	_BANK_DB_LOAD				m_Bank;
	_BAG_DB_LOAD				m_Bag;
	_EQUIP_DB_LOAD				m_Equip;
	_SKILL_DB_LOAD				m_Skill;
	_COOLDOWN_DB_LOAD_FOR_HUMAN m_Cooldown;
	_XINFA_DB_LOAD				m_XinFa;
	_IMPACT_DB_LOAD				m_Impact;
	_ABILITY_DB_LOAD			m_Ability;
	_MISSION_DB_LOAD			m_Mission;
	_SETTING_DB_LOAD			m_Setting;
	_PET_DB_LIST_LOAD			m_PetList;
	_RELATION_DB_LOAD			m_Relation ;
	_PRIVATE_INFO_DB_LOAD		m_PrivateInfo;
	_SOUXIA_DB_LOAD				m_SouXia;
	_MOUNT_DB_LOAD              m_Mount;           // [7/8/2010 �¾���]
	BOOL						m_bIsPasswdUnlock;

	VOID	CleanUp( )
	{
		m_Human.CleanUp();
		m_Equip.CleanUp();
		m_Bag.CleanUp();
		m_Bank.CleanUp();
		m_Skill.CleanUp();
		m_Cooldown.CleanUp();
		m_XinFa.CleanUp();
		m_Impact.CleanUp();
		m_Ability.CleanUp();
		m_Mission.CleanUp();
		m_Setting.CleanUp();
		m_PetList.CleanUp();
		m_Relation.CleanUp() ;
		m_PrivateInfo.CleanUp();
		m_SouXia.CleanUp();
		m_Mount.CleanUp();           // [7/8/2010 �¾���]
		m_bIsPasswdUnlock = FALSE;
	};

	_ITEM*	GetItem(UINT uDBPos)
	{
		Assert(uDBPos<=DB_BANK_POS_END);
		if(uDBPos>=DB_BANK_POS_START) 
		{
			return &m_Bank.m_pItem[uDBPos-DB_BANK_POS_START];
		}
		else if(uDBPos>=DB_EQUIP_POS_START)
		{
			return &m_Equip.m_pItem[uDBPos-DB_EQUIP_POS_START];
		}
		else if(uDBPos>=DB_BAG_POS_START)
		{
			return &m_Bag.m_pItem[uDBPos-DB_BAG_POS_START];
		}

		return NULL;
	}

	INT&	GetMagicIntSetting()
	{
		return m_Setting.m_aSetting[SETTING_TYPE_MAGICINT].m_SettingData;
	}

	BYTE&	GetMagicIntType()
	{
		return m_Setting.m_aSetting[SETTING_TYPE_MAGICINT].m_SettingType;
	}

	VOID OutputToDisk( char* filename )
	{
		if( filename==NULL || filename[0]==0 )
			return ;

		_MY_TRY
		{
			FILE* fp = fopen( filename, "wb" ) ;
			if( fp )
			{
				fwrite( &m_Human,sizeof(_HUMAN_DB_LOAD), 1, fp ) ;
				fwrite( &m_Equip, sizeof(_EQUIP_DB_LOAD), 1, fp ) ;
				fwrite( &m_Bag, sizeof(_BAG_DB_LOAD), 1, fp ) ;
				fwrite( &m_Bank, sizeof(_BANK_DB_LOAD), 1, fp ) ;
				fwrite( &m_Skill, sizeof(_SKILL_DB_LOAD), 1, fp ) ;
				fwrite( &m_Cooldown, sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN), 1, fp ) ;
				fwrite( &m_XinFa, sizeof(_XINFA_DB_LOAD), 1, fp ) ;
				fwrite( &m_Impact, sizeof(_IMPACT_DB_LOAD), 1, fp ) ;
				fwrite( &m_Ability, sizeof(_ABILITY_DB_LOAD), 1, fp ) ;
				fwrite( &m_Mission, sizeof(_MISSION_DB_LOAD), 1, fp ) ;
				fwrite( &m_Setting, sizeof(_SETTING_DB_LOAD), 1, fp ) ;
				fwrite( &m_PetList, sizeof(_PET_DB_LIST_LOAD), 1, fp ) ;
				fwrite( &m_Relation, sizeof(_RELATION_DB_LOAD), 1, fp ) ;
				fwrite( &m_PrivateInfo, sizeof(_PRIVATE_INFO_DB_LOAD), 1, fp ) ;
				fwrite( &m_bIsPasswdUnlock, sizeof(BOOL), 1, fp ) ;
				fwrite( &m_SouXia, sizeof(_SOUXIA_DB_LOAD), 1, fp ) ;
				fwrite( &m_Mount, sizeof(_MOUNT_DB_LOAD), 1, fp ) ;                      // [7/8/2010 �¾���] 
				//fwrite( &m_DRideData, sizeof(_DRIDE_DATA), 1, fp ) ;

				fclose( fp ) ;
			}
		}
		_MY_CATCH
		{
		}
	}
};

#define SM_USE_FREE				0
#define SM_USE_READYFREE		1
#define SM_USE_FREEED			2
#define SM_USE_HOLDDATA			3

struct  SMUHead
{
	UINT		PoolId;
	SMUID_t		SMUid;
	INT			UseStatus;
	CHAR		flag;
	PlayerID_t	PID;
	UINT		SaveTime;	//Ӧ���Ǳ������ʱ�䣬
							//��Ϊǣ�浽��ͬ����֮��ͨѶ
							//��ȷ����
	SMUHead()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		PoolId			=	-1;
		SMUid			=	0;
		PID				=	-1;
		UseStatus		=	SM_USE_FREE;
		flag			=	SM_FREE;
		SaveTime		=	0;
	}

}; 


struct HumanSMU	
{
	 SMUHead						m_SMUHead;
	 HUMAN_DB_ATTR_FLAG				m_AttrFlag;	//��ɫ���Ա�־λ
	_HUMAN_DB_LOAD					m_HumanSM ;	//��ɫ������Ϣ
	_BANK_DB_LOAD					m_BankSM ;	//��ɫ������Ʒ��Ϣ
	_SKILL_DB_LOAD					m_SkillSM ;	//��ɫ����ӵ�еļ�����Ϣ
	_COOLDOWN_DB_LOAD_FOR_HUMAN		m_CooldownSM ; //��ɫ���ϵ���ȴ��Ϣ
	_XINFA_DB_LOAD					m_XinFaSM ;	//��ɫѧ����ķ���Ϣ
	_IMPACT_DB_LOAD					m_ImpactSM ;//��ɫ������ʩ�ӵĸ���Ч����Ϣ
	_ABILITY_DB_LOAD				m_AbilitySM; // ��ɫѧ����������Ϣ�Լ��䷽��
	_MISSION_DB_LOAD				m_MissionSM; // �����б�
	_SETTING_DB_LOAD				m_SettingSM; // �����б�
	_PET_DB_LIST_LOAD				m_PetListSM; // �����б�
	_BAG_DB_LOAD					m_BagSM;	//��ɫ������Ʒ��Ϣ
	_EQUIP_DB_LOAD					m_EquipSM;	//��ɫװ����Ϣ
	_RELATION_DB_LOAD				m_RelationSM; //��ɫ��ϵ�ˣ����ѡ���������
	_PRIVATE_INFO_DB_LOAD			m_PrivateInfoSM;//˽����Ϣ
	_SOUXIA_DB_LOAD					m_SouXiaSM; // ����¼��Ϣ add by gh 2010/05/10
	_MOUNT_DB_LOAD                  m_MountSM;  // ������Ϣ   [7/8/2010 �¾���]

public:

	VOID	CleanUp()
	{
		
		m_SMUHead.CleanUp();
		m_AttrFlag.CleanUp();
		m_HumanSM.CleanUp();
		m_BankSM.CleanUp();
		m_SkillSM.CleanUp();
		m_CooldownSM.CleanUp();
		m_XinFaSM.CleanUp();
		m_ImpactSM.CleanUp();
		m_AbilitySM.CleanUp();
		m_MissionSM.CleanUp();
		m_SettingSM.CleanUp();
		m_PetListSM.CleanUp();
		m_BagSM.CleanUp();
		m_EquipSM.CleanUp();
		m_RelationSM.CleanUp() ;
		m_PrivateInfoSM.CleanUp();
		m_SouXiaSM.CleanUp();
		m_MountSM.CleanUp();        // [7/8/2010 �¾���]
	}


	VOID	SetPoolID(UINT poolID)
	{
		m_SMUHead.PoolId = poolID;
	}	

	UINT	GetPoolID()
	{
		return m_SMUHead.PoolId;
	}


	BOOL	SetUseStatus(INT Use,CHAR Type)
	{
		Lock(Type);

		m_SMUHead.UseStatus	= Use;

		UnLock(Type);

		return TRUE;
	}

	INT	GetUseStatus(CHAR Type)
	{
		INT iRet;
		Lock(Type);
		
		iRet = m_SMUHead.UseStatus;

		UnLock(Type);
		
		return iRet;

	}

	INT		GetUseStatusFast()
	{
		return m_SMUHead.UseStatus;
	}

	UINT	GetTime2Save(CHAR Type)
	{
		UINT	uTime;
		Lock(Type);

		uTime= m_SMUHead.SaveTime;

		UnLock(Type);

		return uTime;
	}

	INT		GetTime2SaveFast()
	{
		return m_SMUHead.SaveTime;
	}

	VOID	SetTime2Save(UINT uTime,CHAR Type)
	{
		Lock(Type);
		m_SMUHead.SaveTime = uTime;
		UnLock(Type);
	}

	CHAR	GetLockValueFast()
	{
		return m_SMUHead.flag;
	}

	VOID	SetPlayerID(CHAR Type,PlayerID_t	nID)
	{
		Lock(Type);
		m_SMUHead.PID	=	nID;
		UnLock(Type);
	}

	PlayerID_t	GetPlayerID(CHAR Type)
	{
		PlayerID_t	id;
		Lock(Type);
		id = m_SMUHead.PID;
		UnLock(Type);

		return id;
	}

	BOOL	TryLock(CHAR Type)
	{
		return sm_trylock(m_SMUHead.flag,Type);
	}

	VOID	Lock(CHAR Type)
	{
		sm_lock(m_SMUHead.flag,Type);

	}

	VOID	UnLock(CHAR Type)
	{
		sm_unlock(m_SMUHead.flag,Type);
	}

	VOID	Copy2FullData(CHAR Type,FULLUSERDATA* pData)
	{
		Lock(Type);
		
		Assert(pData);
		
		memcpy(&pData->m_Ability,&m_AbilitySM,sizeof(m_AbilitySM));
		memcpy(&pData->m_Bag,&m_BagSM,sizeof(m_BagSM));
		memcpy(&pData->m_Bank,&m_BankSM,sizeof(m_BankSM));
		memcpy(&pData->m_Cooldown,&m_CooldownSM,sizeof(m_CooldownSM));
		memcpy(&pData->m_Equip,&m_EquipSM,sizeof(m_EquipSM));
		memcpy(&pData->m_Human,&m_HumanSM,sizeof(m_HumanSM));
		memcpy(&pData->m_Impact,&m_ImpactSM,sizeof(m_ImpactSM));
		memcpy(&pData->m_Mission,&m_MissionSM,sizeof(m_MissionSM));
		memcpy(&pData->m_Setting,&m_SettingSM,sizeof(m_SettingSM));
		memcpy(&pData->m_PetList,&m_PetListSM,sizeof(m_PetListSM));
		memcpy(&pData->m_Relation,&m_RelationSM,sizeof(m_RelationSM));
		memcpy(&pData->m_Skill,&m_SkillSM,sizeof(m_SkillSM));
		memcpy(&pData->m_XinFa,&m_XinFaSM,sizeof(m_XinFaSM));
		memcpy(&pData->m_PrivateInfo,&m_PrivateInfoSM,sizeof(m_PrivateInfoSM));
		memcpy(&pData->m_SouXia,&m_SouXiaSM,sizeof(m_SouXiaSM));	// add by gh for souxia 1010/05/10
		memcpy(&pData->m_Mount,&m_MountSM,sizeof(m_MountSM));       // [7/8/2010 �¾���]

		UnLock(Type);
	}

	BOOL	TryCopy2FullData(CHAR Type,FULLUSERDATA* pData)
	{
		if(!pData)
			return FALSE;

		if(TryLock(Type))
		{
			memcpy(&pData->m_Ability,&m_AbilitySM,sizeof(m_AbilitySM));
			memcpy(&pData->m_Bag,&m_BagSM,sizeof(m_BagSM));
			memcpy(&pData->m_Bank,&m_BankSM,sizeof(m_BankSM));
			memcpy(&pData->m_Cooldown,&m_CooldownSM,sizeof(m_CooldownSM));
			memcpy(&pData->m_Equip,&m_EquipSM,sizeof(m_EquipSM));
			memcpy(&pData->m_Human,&m_HumanSM,sizeof(m_HumanSM));
			memcpy(&pData->m_Impact,&m_ImpactSM,sizeof(m_ImpactSM));
			memcpy(&pData->m_Mission,&m_MissionSM,sizeof(m_MissionSM));
			memcpy(&pData->m_Setting,&m_SettingSM,sizeof(m_SettingSM));
			memcpy(&pData->m_PetList,&m_PetListSM,sizeof(m_PetListSM));
			memcpy(&pData->m_Relation,&m_RelationSM,sizeof(m_RelationSM));
			memcpy(&pData->m_Skill,&m_SkillSM,sizeof(m_SkillSM));
			memcpy(&pData->m_XinFa,&m_XinFaSM,sizeof(m_XinFaSM));
			memcpy(&pData->m_PrivateInfo,&m_PrivateInfoSM,sizeof(m_PrivateInfoSM));
			memcpy(&pData->m_SouXia,&m_SouXiaSM,sizeof(m_SouXiaSM));	// add by gh for souxia 1010/05/10
			memcpy(&pData->m_Mount,&m_MountSM,sizeof(m_MountSM));       // [7/8/2010 �¾���]

			UnLock(Type);

			return TRUE;
		}

		return FALSE;
	}

};

struct SMHead
{
	SM_KEY			m_Key;
	unsigned long	m_Size;
	UINT			m_HeadVer;//�����̰汾
	SMHead()
	{
		m_Key		=	0;
		m_Size		=	0;
		m_HeadVer	=	0;
	}
};

struct GUILDUSER_DB
{
	GUID_t				m_UserGUID;							//���GUID
	CHAR				m_szUserName[MAX_CHARACTER_NAME];	//�������
	GUILD_POSITION		m_Position;							//ְ��
	UINT				m_uAccess;							//Ȩ��
	UINT				m_uLastLoginTime;					//�������ʱ��
	UINT				m_uLevel;							//���Ｖ��
	BYTE				m_bMenPaiID;						//����
	INT					m_iCurContribute;					//����
	INT					m_iMaxContribute;					//�����
	INT					m_iJoinTime;						//����ʱ��
    BYTE                m_iSex;                             //�Ա�
    CHAR                m_szTitleName[MAX_CHARACTER_TITLE];


	GUILDUSER_DB( )
	{
		CleanUp( );
	};
	VOID CleanUp( )
	{	
		m_UserGUID = INVALID_ID;
		memset( m_szUserName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME );
		memset( m_szTitleName, 0, sizeof(CHAR)*MAX_CHARACTER_TITLE );
		m_Position = GUILD_POSITION_TRAINEE;
		m_uAccess = 0;
		m_uLastLoginTime = 0;
		m_uLevel	=	0;							//���Ｖ��
		m_bMenPaiID	=	INVALID_JOB;				//����
		m_iCurContribute	=	0;					//����
		m_iMaxContribute	=	0;					//�����
		m_iJoinTime	=	0;						    //����ʱ��
        m_iSex      = 3;
	}
};

struct FAMILYUSER_DB
{
    INT                 m_aFamilyUser[GUILD_FAMILY_SIZE][USER_FAMILY_SIZE]; //�����Ա ֻ��¼m_aGuildUser�±�

    enum ORESULT
    {
        RET_SUCCESS = 0,
        RET_POS_FULL,	//�˶�����
        RET_NO_EXIST,	//������

        RET_UNKOWN,
    };

    FAMILYUSER_DB &operator = ( const FAMILYUSER_DB &in )
    {
        if ( &in != this )
        {
            for (INT i=0; i<GUILD_FAMILY_SIZE; ++i)
            {
                for (INT j=0; j<USER_FAMILY_SIZE ; ++j)
                {
                    m_aFamilyUser[i][j] = in.m_aFamilyUser[i][j];
                }
            }
        }
        return *this;
    } 

    INT Add (INT iFamilyId, INT iIndex)   //����ID�������m_aGuildUser���±�
    {
        for (INT i=0; i<USER_FAMILY_SIZE ; ++i)
        {
            if (INVALID_ID == m_aFamilyUser[iFamilyId][i])
            {
                m_aFamilyUser[iFamilyId][i] = iIndex;
                return RET_SUCCESS;
            }
        }
        return RET_POS_FULL;
    }

    INT Remove (INT iFamilyId, INT iIndex)
    {
        for (INT i=0; i<USER_FAMILY_SIZE; ++i)
        {
            if (iIndex == m_aFamilyUser[iFamilyId][i])
            {
                m_aFamilyUser[iFamilyId][i] = INVALID_ID;
                return RET_SUCCESS;
            }
        }
        return RET_NO_EXIST;
    }

    FAMILYUSER_DB( )
    {
        CleanUp( );
    };

    VOID CleanUp( )
    {
        for( INT i=0; i<GUILD_FAMILY_SIZE; ++i )
        {
            for( INT j=0; j<USER_FAMILY_SIZE; ++j )
            {
                m_aFamilyUser[i][j] = INVALID_ID;
            }
        }
    }
};

struct GUILD_DB
{
	GuildID_t				m_GuildID;							//���ID
	CHAR					m_GuildName[MAX_GUILD_NAME_SIZE];   //�������
	CHAR					m_GuildDesc[MAX_GUILD_DESC_SIZE];   //�����ּ
	GUILD_STATUS			m_Status;							//����״̬
	GUID_t					m_ChieftainGUID;					//������ GUID
	INT						m_nProposerCount;					//��������
	INT						m_UserCount;						//����û�����
	INT						m_MaxUserSize;						//����˿�����
	INT						m_GuildPoint;						//��ṱ�׵�
	INT						m_GuildMoney;						//����ʽ�
	GUILDUSER_DB			m_aGuildUser[USER_ARRAY_SIZE];		//����Ա
	CityID_t				m_CityID;							//�󶨵ĳ���ID
	INT						m_nTime;
	INT						m_Longevity;				        //����
	INT						m_Contribute;				        //���׶�
	INT						m_Honor;					        //����
	GUILD_ADMIN_t			m_AdminLayout;
    FAMILYUSER_DB           m_FamilyUser;                       //������Ϣ
    BYTE                    m_iSex;
    CHAR                    m_szTitleName[MAX_CHARACTER_TITLE];   //�ƺ�����

	GUILD_DB()
	{
		CleanUp();	
	}

	VOID CleanUp()
	{
		m_GuildID = INVALID_ID;
		memset( m_GuildName, 0, MAX_GUILD_NAME_SIZE*sizeof(CHAR) );
		memset( m_szTitleName, 0, MAX_CHARACTER_TITLE*sizeof(CHAR) );
		memset( m_GuildDesc, 0, MAX_GUILD_DESC_SIZE*sizeof(CHAR) );
		m_ChieftainGUID		= INVALID_ID;
		m_nProposerCount	= 0;
		m_UserCount = 0;
		m_MaxUserSize = 0;
		m_GuildPoint = 0;
		m_GuildMoney = 0;
        m_iSex       = 2;
        m_FamilyUser.CleanUp();

		for( INT i=0; i<USER_ARRAY_SIZE; i++ )
		{
			m_aGuildUser[i].CleanUp();
		}

		m_CityID = INVALID_ID;
		m_nTime	 = 0;
		m_Longevity		=	0;			//����
		m_Contribute	=	0;			//���׶�
		m_Honor			=	0;			//����

		m_AdminLayout.CleanUp();
	}
};

enum	GUILD_USE_STATS
{
	GUILD_ADDNEW	=0,		//�¼ӵ���Ҫ����
	GUILD_DELETE,			//ɾ������Ҫ����
	GUILD_SAVED,			//�Ѿ������
};

struct GuildSMU
{
	SMUHead				m_SMUHead;
	GUILD_DB			m_GuildSM;


	VOID	Lock(CHAR Type)
	{
		sm_lock(m_SMUHead.flag,Type);

	}

	VOID	UnLock(CHAR Type)
	{
		sm_unlock(m_SMUHead.flag,Type);
	}
	
	VOID	SetPoolID(UINT poolID)
	{
		m_SMUHead.PoolId = poolID;
	}	

	UINT	GetPoolID()
	{
		return m_SMUHead.PoolId;
	}
	BOOL	SetUseStatus(INT Use,CHAR Type)
	{
		Lock(Type);
		m_SMUHead.UseStatus	= Use;
		UnLock(Type);
		return TRUE;
	}
	INT	GetUseStatus(CHAR Type)
	{
		INT iRet;
		Lock(Type);
		iRet = m_SMUHead.UseStatus;
		UnLock(Type);
		return iRet;
	}
	UINT	GetTime2Save(CHAR Type)
	{
		UINT	uTime;
		Lock(Type);
		uTime= m_SMUHead.SaveTime;
		UnLock(Type);
		return uTime;
	}

	VOID	SetTime2Save(UINT uTime,CHAR Type)
	{
		Lock(Type);
		m_SMUHead.SaveTime = uTime;
		UnLock(Type);
	}

	VOID	Init()
	{
		m_GuildSM.CleanUp();
	}
};

enum	MAIL_USE_STATS
{
	MAIL_ADDNEW		= 0,	//�¼ӵ���Ҫ����
	MAIL_DELETE,			//ɾ������Ҫ����
	MAIL_REFRESH,			//�ʼ���Ϣˢ��
	MAIL_SAVED,				//�Ѿ������
};

struct MailSMU 
{
	SMUHead				m_SMUHead;
	MAIL				m_MailSM;

	VOID	Lock(CHAR Type)
	{
		sm_lock(m_SMUHead.flag,Type);

	}

	VOID	UnLock(CHAR Type)
	{
		sm_unlock(m_SMUHead.flag,Type);
	}

	VOID	SetPoolID(UINT poolID)
	{
		m_SMUHead.PoolId = poolID;
	}	

	UINT	GetPoolID()
	{
		return m_SMUHead.PoolId;
	}
	BOOL	SetUseStatus(INT Use,CHAR Type)
	{
		Lock(Type);
		m_SMUHead.UseStatus	= Use;
		UnLock(Type);
		return TRUE;
	}
	INT	GetUseStatus(CHAR Type)
	{
		INT iRet;
		Lock(Type);
		iRet = m_SMUHead.UseStatus;
		UnLock(Type);
		return iRet;
	}
	UINT	GetTime2Save(CHAR Type)
	{
		UINT	uTime;
		Lock(Type);
		uTime= m_SMUHead.SaveTime;
		UnLock(Type);
		return uTime;
	}

	VOID	SetTime2Save(UINT uTime,CHAR Type)
	{
		Lock(Type);
		m_SMUHead.SaveTime = uTime;
		UnLock(Type);
	}

	VOID	Init()
	{
		m_MailSM.CleanUp();
	}
};


struct ItemSerialKeySMU
{
	SMUHead				m_SMUHead;
	UINT				m_ItemSerial;

	VOID	Lock(CHAR Type)
	{
		sm_lock(m_SMUHead.flag,Type);

	}

	VOID	UnLock(CHAR Type)
	{
		sm_unlock(m_SMUHead.flag,Type);
	}

	VOID	SetPoolID(UINT poolID)
	{
		m_SMUHead.PoolId = poolID;
	}	

	UINT	GetPoolID()
	{
		return m_SMUHead.PoolId;
	}
	BOOL	SetUseStatus(INT Use,CHAR Type)
	{
		Lock(Type);
		m_SMUHead.UseStatus	= Use;
		UnLock(Type);
		return TRUE;
	}
	INT	GetUseStatus(CHAR Type)
	{
		INT iRet;
		Lock(Type);
		iRet = m_SMUHead.UseStatus;
		UnLock(Type);
		return iRet;
	}
	UINT	GetTime2Save(CHAR Type)
	{
		UINT	uTime;
		Lock(Type);
		uTime= m_SMUHead.SaveTime;
		UnLock(Type);
		return uTime;
	}

	VOID	SetTime2Save(UINT uTime,CHAR Type)
	{
		Lock(Type);
		m_SMUHead.SaveTime = uTime;
		UnLock(Type);
	}

	UINT	GetSerial(CHAR Type)
	{
		UINT Serial;
		Lock(Type);
		Serial = m_ItemSerial;
		UnLock(Type);

		return Serial;
	}

	VOID	SetSerial(CHAR Type,UINT Serial)
	{
		Lock(Type);
		m_ItemSerial = Serial;
		UnLock(Type);
	}


	VOID	Init()
	{
		m_ItemSerial = 0;
	}
};



#pragma pack(pop)

#endif
