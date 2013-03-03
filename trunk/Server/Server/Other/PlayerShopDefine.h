#ifndef _PLAYER_SHOP_DEFINE_H_
#define _PLAYER_SHOP_DEFINE_H_
#include "Type.h"
#include "DB_Struct.h"
#include "Record.h"
#define MAX_PLAYER_SHOP_POOL_PER_SERVER	1000

namespace PLAYER_SHOP
{
	//-------------------------------------------------------------------------------------------------
	//ö�ٶ���
	enum STATUS_PLAYER_SHOP
	{
		STATUS_PLAYER_SHOP_INVALID = 0,		//��Ч����û�з����ȥ
		STATUS_PLAYER_SHOP_CLOSE,			//���ȣ�������ʱ�رոõ�
		STATUS_PLAYER_SHOP_OPEN,			//���ţ��������ھ�Ӫ�˵�
		STATUS_PLAYER_SHOP_SHUT_DOWN,		//���գ���Ӫ���Ƶ��±�ϵͳǿ�йر�
		STATUS_PLAYER_SHOP_ON_SALE,			//���̳������˿��������������̵�
	};
	enum TYPE_PLAYER_SHOP
	{
		TYPE_PLAYER_SHOP_INVALID = 0,		//��Ч��𣬻�û�з����ȥ
		TYPE_PLAYER_SHOP_ITEM,				//��Ʒ��
		TYPE_PLAYER_SHOP_GEM,				//��ʯ��
		TYPE_PLAYER_SHOP_WEAPON,			//������
		TYPE_PLAYER_SHOP_ARMOR,				//���׵�
		TYPE_PLAYER_SHOP_MATERIAL,			//���ϵ�
		TYPE_PLAYER_SHOP_PET,				//�����
	};
	enum TYPE_STALL
	{
		TYPE_STALL_INVALID = 0,				//��Ч��𣬻�û�з����ȥ
		TYPE_STALL_ITEM,					//��Ʒ��̨
		TYPE_STALL_PET,						//�����̨
	};
	enum RET_TYPE_PARTNER
	{
		RET_TYPE_INVALID = -1,				//�Ƿ�����
		RET_TYPE_ALREADY_IN_LIST,			//�����������б���
		RET_TYPE_NOT_FIND_IN_WORLD,			//������û���ҵ�
		RET_TYPE_NOT_FIND_IN_LIST,			//�����߲����б�
		RET_TYPE_LIST_FULL,					//�б�����
		RET_TYPE_LIST_EMPTY,				//�б�����
		RET_TYPE_SUCCESS,					//�����ɹ�
	};

	//-------------------------------------------------------------------------------------------------
	//���ݽṹ
	//�����ݿ���ʵĹ�̨�ṹ
	struct StallBoxDB_t
	{
		struct StallData 
		{
			UINT			m_nFirstPage;							//��ʼҳ
			BOOL			m_bItemIsOnSale[STALL_BOX_SIZE];		//��ʾ��λ�õ���Ʒ�Ƿ��ϼ�
			UINT			m_ItemPrice[STALL_BOX_SIZE];			//��ʾ��λ�õ���Ʒ�ļ۸�
			_ITEM			m_ItemList[STALL_BOX_SIZE];				//��Ʒ����
			_PET_DB_LOAD	m_PetList[STALL_PET_BOX_SIZE];			//��������
		};

		BYTE			m_StallStatus;							//��ǰ��̨��״̬�����ţ����ȣ����ţ�
		BYTE			m_StallType;							//��ǰ��̨�����ͣ���Ʒ1������2��
		UINT			m_nFirstPage;							//��ʼҳ
		BOOL			m_bItemIsOnSale[STALL_BOX_SIZE];		//��ʾ��λ�õ���Ʒ�Ƿ��ϼ�
		UINT			m_ItemPrice[STALL_BOX_SIZE];			//��ʾ��λ�õ���Ʒ�ļ۸�
		_ITEM			m_ItemList[STALL_BOX_SIZE];				//��Ʒ����
		_PET_DB_LOAD	m_PetList[STALL_PET_BOX_SIZE];			//��������

		VOID CleanUp()
		{
			memset(this, 0, sizeof(StallBoxDB_t));
			m_StallType	= 1;									//Ĭ������Ʒ��̨
		}
	};

	//�������
	struct Partner_t
	{
		GUID_t		m_Guid;
		CHAR		m_szName[MAX_CHARACTER_NAME];
		VOID		CleanUp()
		{
			m_Guid = INVALID_GUID;
			memset(m_szName, 0, MAX_CHARACTER_NAME);
		}
	};

	//�����ݿ���ʵ��̵�ṹ
	struct PlayerShopDB_t
	{
		BYTE							m_ShopStatus;								//���̵�������״̬
		BYTE							m_ShopType;									//�̵����
		_PLAYERSHOP_GUID				m_ShopGUID;									//ÿ���̵��ΨһID
		UINT							m_MaxBaseMoney;								//�̵걾������
		UINT							m_BaseMoney;								//�̵걾��
		UINT							m_ProfitMoney;								//�̵�ӯ����
		BYTE							m_FoundedSec;								//�����ʱ��
		BYTE							m_FoundedMin;								//�����ʱ��
		BYTE							m_FoundedHour;								//�����ʱ��
		BYTE							m_FoundedDay;								//�����ʱ��
		BYTE							m_FoundedMonth;								//�����ʱ��
		UINT							m_FoundedYear;								//�����ʱ��
		CHAR							m_szShopName[MAX_SHOP_NAME_SIZE];			//��ʾ�̵���
		CHAR							m_szShopDesc[MAX_SHOP_DESC_SIZE];			//��ʾ�̵������
		CHAR							m_szOwnerName[MAX_CHARACTER_NAME];			//��ʾ������
		GUID_t							m_OwnerGuid;								//����guid
		BYTE							m_NumStallOpened;							//�Ѿ����ŵĹ�̨��
		BYTE							m_NumStallOnSale;							//�Ѿ����ŵĹ�̨��
		UINT							m_uSaleOutPrice;							//�̳��۸�
		StallBoxDB_t					m_StallListData[MAX_STALL_NUM_PER_SHOP];	//��̨�����б�
		//ServerBBS						m_StallBBS;									//�̵����԰�
		RecordContainer_t				m_ExchangeRecord;							//���׼�¼
		RecordContainer_t				m_ManagerRecord;							//�����¼
		Partner_t						m_PartnerList[MAX_PARTNER_PER_SHOP];		//�������
		BYTE							m_PartnerNum;

		VOID CleanUp()
		{

			m_ShopStatus		=	0;												//���̵�������״̬
			m_ShopType			=	0;												//�̵����
			m_MaxBaseMoney		=	0;												//�̵걾������
			m_BaseMoney			=	0;												//�̵걾��
			m_ProfitMoney		=	0;												//�̵�ӯ����
			m_FoundedSec		=	0;												//�����ʱ��
			m_FoundedMin		=	0;												//�����ʱ��
			m_FoundedHour		=	0;												//�����ʱ��
			m_FoundedDay		=	0;												//�����ʱ��
			m_FoundedMonth		=	0;												//�����ʱ��
			m_FoundedYear		=	0;												//�����ʱ��
			memset(m_szShopName, 0, MAX_SHOP_NAME_SIZE);							//��ʾ�̵���
			memset(m_szShopDesc, 0, MAX_SHOP_DESC_SIZE);							//��ʾ�̵������
			memset(m_szOwnerName, 0, MAX_CHARACTER_NAME);							//��ʾ������
			m_OwnerGuid			=	INVALID_ID;										//����guid
			m_NumStallOpened	=	0;												//�Ѿ����ŵĹ�̨��
			m_NumStallOnSale	=	0;												//�Ѿ����ŵĹ�̨��
			m_uSaleOutPrice		=	0;												//�̳��۸�
			for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				m_StallListData[i].CleanUp();										//��̨�����б�
			}
			for(UINT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
			{
				m_PartnerList[i].CleanUp();											//�������
			}
			m_PartnerNum = 0;
			//m_StallBBS.CleanUp();													//��̨���԰�
			//m_ExchangeRecord.CleanUp();												//���׼�¼
			//m_ManagerRecord.CleanUp();												//�����¼
			m_ShopGUID.Reset();
		}
	};
	//��ShareMemory���ʵ��̵�ṹ
	struct  PlayerShopSM
	{
		SMUHead				m_SMUHead;
		PlayerShopDB_t		m_PlayerShop;

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
			m_PlayerShop.CleanUp();
		}
	};

};
#endif;