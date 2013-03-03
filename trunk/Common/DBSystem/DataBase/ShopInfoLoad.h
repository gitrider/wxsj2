/********************************************************************
	created:	2007-9-28   12:47
	filename: 	Common\DBSystem\DataBase\ShopInfoLoad.h
	author:		YangJun
	
	purpose:	������ͬ�汾�̵�/���ﷱֳ�ļ��ؽṹ
*********************************************************************/

#ifndef _SHOPINFOLOAD_H_
#define _SHOPINFOLOAD_H_

#include "Type.h"
#include "DB_Struct.h"

//////////////////////////////////////////////////////////////////////////
//��ǰ�̵��̨�İ汾��(ÿ���޸Ĺ�̨��Ϣ����Ҫ�ֶ����Ӵ˰汾��)
#define		SHOP_STALL_VERSION_0				0
			/*
			<Version0>
			��չ�����޳ƺŹ��ܡ�֮ǰ
			_PET_TITLE		m_PetTitle[PET_TITLE_NUM];		// ���޳ƺ��б�
			TitleID_t		m_tCurPetTitleID;				// ��ǰͷ���ƺ�
			*/
#define		SHOP_STALL_VERSION_1				1
			/*
			<Version1>
			��չ�����޽������ܡ�֮ǰ
			USHORT			m_usPWUnlockTime;				// �����������ʱ�䣨��λ��Сʱ��
			USHORT			m_usReserve;					// Ԥ��λ
			*/
#define		SHOP_CURRENT_STALL_VERSION			2

namespace PLAYER_SHOP
{
	struct StallBoxDB_t;
	struct PlayerShopBuyShopStallDB_t;
	struct PlayerShopBuyShopDB_t;
}
//////////////////////////////////////////////////////////////////////////
//��ǰ���ﷱֳ�İ汾��
#define		PETPROCREATE_ITEM_VERSION_0			0
			/*
			<Version0>
			��չ�����޳ƺŹ��ܡ�֮ǰ
			_PET_TITLE		m_PetTitle[PET_TITLE_NUM];		// ���޳ƺ��б�
			TitleID_t		m_tCurPetTitleID;				// ��ǰͷ���ƺ�
			*/
#define		PETPROCREATE_ITEM_VERSION_1			1
			/*
			<Version1>
			��չ�����޽������ܡ�֮ǰ
			USHORT			m_usPWUnlockTime;				// �����������ʱ�䣨��λ��Сʱ��
			USHORT			m_usReserve;					// Ԥ��λ
			*/
#define		PETPROCREATE_CURRENT_ITEM_VERSION	2

class PetProcreateItem;
//////////////////////////////////////////////////////////////////////////
//����ƺŴ����Ľṹ����չ
#pragma pack(push, 1)
struct _PET_DB_LOAD_V0
{
	PET_GUID_t		m_GUID;							// Ψһ���
	PET_GUID_t		m_SpouseGUID;					// ��żΨһ���
	INT				m_nDataID;						// ����ģ��
	CHAR			m_szName[MAX_CHARACTER_NAME];	// ����
	CHAR			m_szNick[MAX_NICK_NAME];		// �ǳ�
	INT				m_nLevel;						// �ȼ�
	INT				m_nLastProcreateLevel;			// ���ֳ�ĵȼ�
	INT				m_nTakeLevel;					// Я���ȼ�
	INT				m_nAttackType;					// �������ͣ���/����
	INT				m_AIType;						// AI���� PET_AI_TYPE
	INT				m_nHP;							// ����ֵ
	INT				m_nLife;						// ��ǰ����
	BYTE			m_byPetType;					// PET_TYPE ���������죬Ұ��
	BYTE			m_byGeneration;					// ������
	BYTE			m_byHappiness;					// ���ֶ�

	INT				m_nStrPerception;				// ��������
	INT				m_nConPerception;				// ��������
	INT 			m_nDexPerception;				// ������
	INT				m_nSprPerception;				// ��������
	INT 			m_nIntPerception;				// ��������
	INT				m_nGenGu;						// ����
	FLOAT			m_fGrowRate;					// �ɳ���
	INT				m_nRemainPoint;					// һ������ʣ�����
	INT				m_nExp;							// ����ֵ
	BYTE			m_yFlags;						// ��־λ���� PET_FLAG_MASK_PASSWORD_LOCKED
	_ATTR_LEVEL1	m_BaseAttrLevel1;				// ����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�

	_PET_SKILL		m_SkillList[PET_MAX_SKILL_COUNT];	// ���޼����б�,���е�0�����������ܲ�

	_COOLDOWN_DB_LOAD_FOR_PET m_CooldownList;			//���޵���ȴ�б�
	INT				m_nSavvy;						// ����

	_PET_DB_LOAD_V0()
	{
		CleanUp();
	}

	VOID CleanUp( VOID );
};
struct _PET_DB_LOAD_V1
{
	_PET_DB_LOAD_V0	m_V0;

	//��Lybin Sep.13 2007��
	// ��չ�����޳ƺ�
	_PET_TITLE		m_PetTitle[PET_TITLE_NUM];		// ���޳ƺ��б�
	TitleID_t		m_tCurPetTitleID;				// ��ǰͷ���ƺ�

	_PET_DB_LOAD_V1()
	{
		CleanUp();
	}

	VOID CleanUp( VOID );
};
#pragma pack(pop)
class PetInfoLoad
{
public:
	static VOID LoadFrom_VO(_PET_DB_LOAD& petDB,_PET_DB_LOAD_V0& petDB_V0);
	static VOID LoadFrom_V1(_PET_DB_LOAD& petDB,_PET_DB_LOAD_V1& petDB_V1);
};

struct _ITEM_V1
{
	_ITEM_GUID			m_ItemGUID ;				//��Ʒ�̶���Ϣ�����ܸı䣩
	UINT				m_ItemIndex;				//��Ʒ����
													/*
													|	    1 ~ 10000		��ɫװ��
													|	10001 ~ 20000		��ɫװ��	
													|	20001 ~ 30000		��ɫװ��
													|	30001 ~ 35000		ҩƿ
													|	35001 ~ 40000		��ʯ
													|						...
													*/

	CHAR				m_RulerID;
	CHAR				m_nsBind;			//��Ʒ���
	CHAR				m_Creator[MAX_ITEM_CREATOR_NAME];
	INT					m_Param[MAX_ITEM_PARAM];
	
	union 
	{
		EQUIP_INFO			m_Equip;
		GEM_INFO			m_Gem;
		MEDIC_INFO			m_Medic;
		STORE_MAP_INFO		m_StoreMap;
	};

	_ITEM_V1()
	{
		CleanUp();
	}

	VOID CleanUp( VOID )
	{
		memset( this, 0, sizeof(*this) ) ;
	}
};
class ItemInfoLoad
{
public:
	static VOID LoadFrom_V1(_ITEM& itemDB,_ITEM_V1& itemDB_V1);
};

//��̨�汾0	��չ����_PET_DB_LOAD�ĳ���ƺŲ���
struct StallBoxDB_t_V0
{
	BYTE			m_StallStatus;							//��ǰ��̨��״̬�����ţ����ȣ����ţ�
	BYTE			m_StallType;							//��ǰ��̨�����ͣ���Ʒ1������2��
	BYTE			m_ValidNum;								//��Ч����
	UINT			m_nFirstPage;							//��ʼҳ
	BOOL			m_bItemIsOnSale[STALL_BOX_SIZE];		//��ʾ��λ�õ���Ʒ�Ƿ��ϼ�
	UINT			m_ItemPrice[STALL_BOX_SIZE];			//��ʾ��λ�õ���Ʒ�ļ۸�
	_ITEM			m_ItemList[STALL_BOX_SIZE];				//��Ʒ����
	_PET_DB_LOAD_V0	m_PetList[STALL_PET_BOX_SIZE];			//��������

	VOID CleanUp()
	{
		memset(this, 0, sizeof(StallBoxDB_t_V0));
		m_StallType	= 1;									//Ĭ������Ʒ��̨
	}
};
struct StallBoxDB_t_V1
{
	BYTE			m_StallStatus;							//��ǰ��̨��״̬�����ţ����ȣ����ţ�
	BYTE			m_StallType;							//��ǰ��̨�����ͣ���Ʒ1������2��
	BYTE			m_ValidNum;								//��Ч����
	UINT			m_nFirstPage;							//��ʼҳ
	BOOL			m_bItemIsOnSale[STALL_BOX_SIZE];		//��ʾ��λ�õ���Ʒ�Ƿ��ϼ�
	UINT			m_ItemPrice[STALL_BOX_SIZE];			//��ʾ��λ�õ���Ʒ�ļ۸�
	_ITEM_V1		m_ItemList[STALL_BOX_SIZE];				//��Ʒ����
	_PET_DB_LOAD_V1	m_PetList[STALL_PET_BOX_SIZE];			//��������

	VOID CleanUp()
	{
		memset(this, 0, sizeof(StallBoxDB_t_V1));
		m_StallType	= 1;									//Ĭ������Ʒ��̨
	}
};

//�ع��̵�
struct PlayerShopBuyShopStallDB_t_V1
{
	INT				m_iNeededItemNum[PLAYER_SHOP_BUY_STALL_BOX_SIZE];
	UINT			m_iNeededItemIdx[PLAYER_SHOP_BUY_STALL_BOX_SIZE];
	INT				m_iPrice[PLAYER_SHOP_BUY_STALL_BOX_SIZE];								
	_ITEM_V1		m_ItemList[PLAYER_SHOP_BUY_STALL_BOX_SIZE];		//��Ʒ����		
	BYTE			m_InStallNum[PLAYER_SHOP_BUY_STALL_BOX_SIZE];	//���ڹ�̨���Ѿ��չ�����
	CHAR			m_Reserve[20];									//Ԥ��

	VOID CleanUp()
	{
		memset(this, 0, sizeof(PlayerShopBuyShopStallDB_t_V1));
	}
};
struct PlayerShopBuyShopDB_t_V1
{
	CHAR							m_szDesc[PLAYER_SHOP_BUY_STALL_DESC_DB];
	PlayerShopBuyShopStallDB_t_V1	m_StallList[PLAYER_SHOP_BUY_STALL_NUM];
	CHAR							m_Reserve[40];					//Ԥ��

	VOID CleanUp()
	{
		memset(this, 0, sizeof(PlayerShopBuyShopDB_t_V1));
	}
};

class ShopInfoLoad
{
public:
	static VOID LoadFrom_V0(PLAYER_SHOP::StallBoxDB_t& stBox,StallBoxDB_t_V0& stBox_V0);
	static VOID LoadFrom_V1(PLAYER_SHOP::StallBoxDB_t& stBox,StallBoxDB_t_V1& stBox_V1);
	static VOID LoadFrom_BuyShop_V1(PLAYER_SHOP::PlayerShopBuyShopDB_t& stBuyShop,PlayerShopBuyShopDB_t_V1& stBuyShop_V1);
};

//////////////////////////////////////////////////////////////////////////
//���ﷱֳ�ṹ�Գ���ƺŵ���չ
struct PetProcreateItem_VO
{
	CHAR			m_cState;				// ״̬ ENUM_STATE
	GUID_t			m_guidHumanFirst;
	GUID_t			m_guidHumanSecond;
	CHAR			m_szFirstHumanName[MAX_CHARACTER_NAME];
	CHAR			m_szSecondHumanName[MAX_CHARACTER_NAME];
	_PET_DB_LOAD_V0			m_dbPetFirst;
	_PET_DB_LOAD_V0			m_dbPetSecond;
	UINT			m_uProcreateInterval;	// ����Է�ֳ��ʱ����
	UINT			m_uWaitTakeOutTime;		// ��ֳ�Ѿ�����˶���ʱ��
	UINT			m_uPrevMailToHumanTime;	// �ϴ�֪ͨȡ�����޵�ʱ��
	UINT			m_uStateInterval;		// ״̬���õ�ʱ����
};
struct PetProcreateItem_V1
{
	CHAR			m_cState;				// ״̬ ENUM_STATE
	GUID_t			m_guidHumanFirst;
	GUID_t			m_guidHumanSecond;
	CHAR			m_szFirstHumanName[MAX_CHARACTER_NAME];
	CHAR			m_szSecondHumanName[MAX_CHARACTER_NAME];
	_PET_DB_LOAD_V1			m_dbPetFirst;
	_PET_DB_LOAD_V1			m_dbPetSecond;
	UINT			m_uProcreateInterval;	// ����Է�ֳ��ʱ����
	UINT			m_uWaitTakeOutTime;		// ��ֳ�Ѿ�����˶���ʱ��
	UINT			m_uPrevMailToHumanTime;	// �ϴ�֪ͨȡ�����޵�ʱ��
	UINT			m_uStateInterval;		// ״̬���õ�ʱ����
};

class PetProcreateInfoLoad
{
public:
	static VOID LoadFrom_V0(PetProcreateItem& petPro,PetProcreateItem_VO& petPro_V0);
	static VOID LoadFrom_V1(PetProcreateItem& petPro,PetProcreateItem_V1& petPro_V1);
};
#endif