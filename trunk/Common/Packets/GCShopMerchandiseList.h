// GCShopMerchandiseList.h
// 
// ���˷����ͻ��˵���Ʒ�б�
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPMERCHANDISELIST_H__
#define __GCSHOPMERCHANDISELIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopMerchandiseList : public Packet
	{
	public:
		//����������Ʒ�ṹ
		struct _MERCHANDISE_ITEM
		{
			UINT		idTable;		//��Դid
			BYTE		byNumber;		//����
            BYTE		nPriceType;		//�۸�����
			UINT		nPrice;			//�۸�
			INT			MaxNumber;		//������Ʒ������
			_MERCHANDISE_ITEM()
			{
				idTable		=	0;		//��Դid
				byNumber	=	0;		//����
				nPrice		=	0;   	//�۸�
                nPriceType  =   0;
				MaxNumber	=	0;		//������Ʒ������
			}
		};

	public:
		GCShopMerchandiseList( )
		{
			m_nObjID = INVALID_ID;
			m_nMerchadiseNum = 0;
			m_UniqueID = 0;
            m_uVersion  =   0;
		}
		virtual ~GCShopMerchandiseList( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPMERCHANDISELIST ; }
		virtual UINT			GetPacketSize()const { return	
			sizeof(ObjID_t) + sizeof(BYTE)+ sizeof(INT)*4+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(UINT)+
			m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM)+sizeof(BOOL)+sizeof(UINT);}

	public:
		BYTE			GetMerchadiseNum(VOID) const {return m_nMerchadiseNum;};
		VOID			SetMerchadiseNum(BYTE nNumber) {m_nMerchadiseNum = nNumber;};

		_MERCHANDISE_ITEM*	GetMerchadiseList(VOID) { return m_MerchadiseList; }
		VOID				SetMerchadiseList(_MERCHANDISE_ITEM* pSour)
		{
			memcpy(m_MerchadiseList, pSour, m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM));
		}

		VOID			SetBuyType(INT nBuyType){m_nBuyType = nBuyType;}
		INT				GetBuyType(VOID){return m_nBuyType;}

		VOID			SetRepairType(INT nRepairType){m_nRepairType = nRepairType;}
		INT				GetRepairType(VOID){return m_nRepairType;}

		VOID			SetBuyLevel(INT nBuyLevel){m_nBuyLevel = nBuyLevel;}
		INT				GetBuyLevel(VOID){return m_nBuyLevel;}

		VOID			SetRepairLevel(INT nRepairLevel){m_nRepairLevel = nRepairLevel;}
		INT				GetRepairLevel(VOID){return m_nRepairLevel;}

		VOID			SetRepairSpend(FLOAT nRepairSpend){m_nRepairSpend = nRepairSpend;}
		FLOAT			GetRepairSpend(VOID){return m_nRepairSpend;}

		VOID			SetRepairOkProb(FLOAT nRepairOkProb){m_nRepairOkProb = nRepairOkProb;}
		FLOAT			GetRepairOkProb(VOID){return m_nRepairOkProb;}

		VOID			SetObjID(ObjID_t id) { m_nObjID = id; }
		ObjID_t			GetObjID(void) { return m_nObjID; }

		VOID			SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT			GetUniqueID(void) { return m_UniqueID; }

		VOID			SetScale(FLOAT fScale) { m_fScale = fScale; }
		FLOAT			GetScale(void) { return m_fScale; }

        VOID            SetDyShop(BOOL bDyShop) { m_bDyShop = bDyShop; }
        BOOL            GetDyShop() { return m_bDyShop; }

        VOID			SetVersion(UINT uVersion) { m_uVersion = uVersion; }
        UINT			GetVersion(void) { return m_uVersion; }
	private:
		//���˵�id
		ObjID_t						m_nObjID;
		//��Ʒ����
		BYTE						m_nMerchadiseNum;
		INT							m_nRepairLevel;		// ����ȼ�
		INT							m_nBuyLevel;		// �չ��ȼ�
		INT							m_nRepairType;		// ��������
		INT							m_nBuyType;			// �̵���չ�����
		FLOAT						m_nRepairSpend;		// ������
		FLOAT						m_nRepairOkProb;	// ����ɹ�����
		UINT						m_UniqueID;
        BOOL                        m_bDyShop;          //�Ƿ�̬�̵�
        UINT                        m_uVersion;         // ��̬�̵�汾�ţ�У����

		FLOAT						m_fScale;			// �̵�ϵ��

		//������Ʒ�б�
		_MERCHANDISE_ITEM			m_MerchadiseList[MAX_BOOTH_NUMBER];
	};


	class GCShopMerchandiseListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopMerchandiseList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPMERCHANDISELIST; };
		UINT		GetPacketMaxSize()const { return 
			sizeof(ObjID_t) + sizeof(GCShopMerchandiseList::_MERCHANDISE_ITEM)*MAX_BOOTH_NUMBER + 
			sizeof(BYTE)+ sizeof(INT)*4+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(UINT)+sizeof(BOOL)+sizeof(UINT);};
	};

	class GCShopMerchandiseListHandler 
	{
	public:
		static UINT Execute( GCShopMerchandiseList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


