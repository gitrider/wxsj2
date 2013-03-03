// GCShopBuy.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPBUY_H__
#define __GCSHOPBUY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopBuy : public Packet
	{
	public:
		enum
		{
			BUY_OK = 0,				// ����ɹ�
			BUY_BACK_OK,			// �ع��ɹ�
			BUY_MONEY_FAIL,			// ûǮ��
			BUY_NO_MERCH,			// �Ѿ�������
			BUY_BAG_FULL,			// �Ų�����
			BUY_MUST_FRESH,			// ��Ҫˢ�·����б�
		};
	public:
		GCShopBuy( )
		{
			m_IsBuyOk = BUY_OK;
		};
		virtual ~GCShopBuy( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPBUY ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*2+sizeof(UINT);}

	public:
		BYTE					GetBuyOk(VOID) const {return m_IsBuyOk;};
		VOID					SetBuyOk(BYTE BuyOk) {m_IsBuyOk = BuyOk;};

		UINT					GetIndex(VOID) const {return m_ItemIndex;};
		VOID					SetIndex(UINT ItemIndex) {m_ItemIndex = ItemIndex;};

		BYTE					GetNum(VOID) const {return m_ItemNum;};
		VOID					SetNum(BYTE ItemNum) {m_ItemNum = ItemNum;};
	private:
		BYTE					m_IsBuyOk;		//�ɹ����
		UINT					m_ItemIndex;	//�������Ʒ����
		BYTE					m_ItemNum;		//�������Ʒ����

	};

	class GCShopBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPBUY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2+sizeof(UINT);};
	};

	class GCShopBuyHandler 
	{
	public:
		static UINT Execute( GCShopBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
