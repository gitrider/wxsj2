/************************************************************************/
/*
	��������:	2006��2��13��
	����ʱ��:	11:54
	�ļ�����:	PlayerShop.h
	�ļ�·��:	d:\Prj\Server\Server\Other\PlayerShop.h
	������:		sunyu

	�ļ����ܣ�	����̵�
	�޸ļ�¼��
*/
/************************************************************************/

#ifndef _PLAYER_SHOP_H_
#define _PLAYER_SHOP_H_

#include "PlayerShopDefine.h"
#include "PlayerStallBox.h"
/*
����̵�
*/
using namespace PLAYER_SHOP;
class PlayerShop
{
	friend class PlayerShopPool;
public:
	//-------------------------------------------------------------------------------------------------
	//����ʱ�ṹ
	struct PlayerShopRT_t
	{
		PlayerStallBox					m_StallList[MAX_STALL_NUM_PER_SHOP];		//����ʱ��̨�б�
		BYTE							m_bSerial;									//�̵��Լ������к�,�����̵�Ľ���
		BOOL							m_PlayerShopChanged;						//�̵����ݸı�

	};
public:
	PlayerShop();
	~PlayerShop();
public:
	//-------------------------------------------------------------------------------------------------
	//��ʼ��,�������̵�
	VOID Init();

	VOID CleanUp();
public:
	//-------------------------------------------------------------------------------------------------
	//һ���̵��ΨһID
	INT					GetID(){return GetPoolPos();}
	SceneID_t			GetSceneID(){return m_pPlayerShopDB->m_ShopGUID.m_Scene;}

	STATUS_PLAYER_SHOP	GetShopStatus(){return (STATUS_PLAYER_SHOP)m_pPlayerShopDB->m_ShopStatus;}
	VOID				SetShopStatus(const STATUS_PLAYER_SHOP ShopStatus)
	{
		m_pPlayerShopDB->m_ShopStatus = (BYTE)ShopStatus;
		DataChanged();
	}

	TYPE_PLAYER_SHOP	GetShopType(){return (TYPE_PLAYER_SHOP)m_pPlayerShopDB->m_ShopType;}
	VOID				SetShopType(const TYPE_PLAYER_SHOP ShopType)
	{
		m_pPlayerShopDB->m_ShopType = (BYTE)ShopType;
		DataChanged();
	}

	_PLAYERSHOP_GUID	GetShopGUID(){return m_pPlayerShopDB->m_ShopGUID;}
	VOID				SetShopGUID(const _PLAYERSHOP_GUID ShopGUID)
	{
		m_pPlayerShopDB->m_ShopGUID = ShopGUID;
		DataChanged();
	}

	UINT				GetMaxBaseMoney(){return m_pPlayerShopDB->m_MaxBaseMoney;}
	VOID				SetMaxBaseMoney(UINT MaxBaseMoney)
	{
		m_pPlayerShopDB->m_MaxBaseMoney = MaxBaseMoney;
		DataChanged();
	}
	UINT				UpdateMaxBaseMoney(FLOAT fComFactor);

	BOOL				MinusMoney(UINT uMoney);
	VOID				AddMoney(UINT uMoney);
	VOID				UpdateMoneyEvent();

	UINT				GetBaseMoney(){return m_pPlayerShopDB->m_BaseMoney;}
	VOID				SetBaseMoney(UINT BaseMoney)
	{
		m_pPlayerShopDB->m_BaseMoney = BaseMoney;
		DataChanged();
	}

	UINT				GetProfitMoney(){return m_pPlayerShopDB->m_ProfitMoney;}
	VOID				SetProfitMoney(UINT ProfitMoney)
	{
		m_pPlayerShopDB->m_ProfitMoney = ProfitMoney;
		DataChanged();
	}

	CHAR*				GetShopName(){return m_pPlayerShopDB->m_szShopName;}
	VOID				SetShopName(CHAR* pszShopName, UINT uSize)
	{
		memset(m_pPlayerShopDB->m_szShopName, 0, MAX_SHOP_NAME_SIZE);
		memcpy(m_pPlayerShopDB->m_szShopName, pszShopName, uSize);
		DataChanged();
	}

	CHAR*				GetShopDesc(){return m_pPlayerShopDB->m_szShopDesc;}
	VOID				SetShopDesc(CHAR* pszShopDesc, UINT uSize)
	{
		memset(m_pPlayerShopDB->m_szShopDesc, 0, MAX_SHOP_DESC_SIZE);
		memcpy(m_pPlayerShopDB->m_szShopDesc, pszShopDesc, uSize);
		DataChanged();
	}

	CHAR*				GetOwnerName(){return m_pPlayerShopDB->m_szOwnerName;}
	VOID				SetOwnerName(const CHAR* pszOwnerName)
	{
		memset(m_pPlayerShopDB->m_szOwnerName, 0, MAX_CHARACTER_NAME);
		memcpy(m_pPlayerShopDB->m_szOwnerName, pszOwnerName, strlen(pszOwnerName));
		DataChanged();
	}

	GUID_t				GetOwnerGuid(){ return m_pPlayerShopDB->m_OwnerGuid; }
	VOID				SetOwnerGuid(GUID_t OwnerGuid)
	{
		m_pPlayerShopDB->m_OwnerGuid = OwnerGuid; 
		DataChanged();
	}
	
	//-------------------------------------------------------------------------------------------------
	//�򿪵�,���ŵĹ�̨
	BYTE				GetNumStallOpened(){ return m_pPlayerShopDB->m_NumStallOpened; }
	VOID				SetNumStallOpened(BYTE NumStallOpened)
	{
		m_pPlayerShopDB->m_NumStallOpened = NumStallOpened; 
		DataChanged();
	}

	BYTE				GetNumStallOnSale(){ return m_pPlayerShopDB->m_NumStallOnSale; }
	VOID				SetNumStallOnSale(BYTE NumStallOnSale)
	{
		m_pPlayerShopDB->m_NumStallOnSale = NumStallOnSale; 
		DataChanged();
	}

	UINT				GetSaleOutPrice(){ return m_pPlayerShopDB->m_uSaleOutPrice; }
	VOID				SetSaleOutPrice(UINT uSaleOutPrice)
	{
		m_pPlayerShopDB->m_uSaleOutPrice = uSaleOutPrice; 
		DataChanged();
	}

	//-------------------------------------------------------------------------------------------------
	//����ʱ��
	UINT				GetFoundedYear(){ return m_pPlayerShopDB->m_FoundedYear; }
	VOID				SetFoundedYear(UINT nFoundedYear)
	{
		m_pPlayerShopDB->m_FoundedYear = nFoundedYear; 
		DataChanged();
	}

	BYTE				GetFoundedMonth(){ return m_pPlayerShopDB->m_FoundedMonth; }
	VOID				SetFoundedMonth(BYTE nFoundedMonth)
	{
		m_pPlayerShopDB->m_FoundedMonth = nFoundedMonth; 
		DataChanged();
	}

	BYTE				GetFoundedDay(){ return m_pPlayerShopDB->m_FoundedDay; }
	VOID				SetFoundedDay(BYTE nFoundedDay)
	{
		m_pPlayerShopDB->m_FoundedDay = nFoundedDay; 
		DataChanged();
	}

	BYTE				GetFoundedHour(){ return m_pPlayerShopDB->m_FoundedHour; }
	VOID				SetFoundedHour(BYTE nFoundedHour)
	{
		m_pPlayerShopDB->m_FoundedHour = nFoundedHour; 
		DataChanged();
	}

	BYTE				GetFoundedMin(){ return m_pPlayerShopDB->m_FoundedMin; }
	VOID				SetFoundedMin(BYTE nFoundedMin)
	{
		m_pPlayerShopDB->m_FoundedMin = nFoundedMin; 
		DataChanged();
	}

	//-------------------------------------------------------------------------------------------------
	//���к�
	BYTE				GetSerial(){ return m_PlayerShopRT.m_bSerial; }
	VOID				SetSerial(BYTE nSerial){ m_PlayerShopRT.m_bSerial = nSerial; }
	BYTE				IncSerial(){return ++m_PlayerShopRT.m_bSerial;}

	//-------------------------------------------------------------------------------------------------
	//��ý��׼�¼
	RecordContainer_t*	GetExchangeRecord() {return &m_pPlayerShopDB->m_ExchangeRecord;}

	//-------------------------------------------------------------------------------------------------
	//��ù����¼
	RecordContainer_t*	GetManagerRecord() {return &m_pPlayerShopDB->m_ManagerRecord;}

	//-------------------------------------------------------------------------------------------------
	//��Ӻ������
	RET_TYPE_PARTNER	AddPartner( GUID_t Guid);

	//-------------------------------------------------------------------------------------------------
	//ɾ���������
	RET_TYPE_PARTNER	RemovePartner( GUID_t Guid );

	//-------------------------------------------------------------------------------------------------
	//��������б�
	Partner_t*			GetPartnerList(){return m_pPlayerShopDB->m_PartnerList;}

	//-------------------------------------------------------------------------------------------------
	//�Ƿ��Ǻ������
	BOOL				IsPartner(GUID_t Guid);


	//-------------------------------------------------------------------------------------------------
	//����
	BOOL				HeartBeat(UINT uTime, FLOAT	fComFactor);
    
	PlayerStallBox*		GetPlayerStallBoxByIndex(UINT uIndex);

	/*ServerBBS*			GetServerBBS(){return &m_pPlayerShopDB->m_StallBBS;}*/

	//-------------------------------------------------------------------------------------------------
	//���ݸı���Ҫ����
	VOID				DataChanged(){	m_PlayerShopRT.m_PlayerShopChanged = TRUE;}

private:
	//-------------------------------------------------------------------------------------------------
	//��λ��,һ���̵��ΨһID
	INT					GetPoolPos(){ return m_pPlayerShopDB->m_ShopGUID.m_PoolPos;}
	VOID				SetPoolPos(INT	nPoolPos){m_pPlayerShopDB->m_ShopGUID.m_PoolPos = nPoolPos;}

private:
	//-------------------------------------------------------------------------------------------------
	//��Ҫ�洢���ݿ����Ϣ
	PlayerShopDB_t*		m_pPlayerShopDB;
	//-------------------------------------------------------------------------------------------------
	//ShareMemory ����
	PlayerShopSM*		m_pPlayerShopSM;
	//-------------------------------------------------------------------------------------------------
	//����ʱ��Ϣ
	PlayerShopRT_t		m_PlayerShopRT;
};

#endif