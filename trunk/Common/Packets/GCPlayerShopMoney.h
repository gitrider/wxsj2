// GCPlayerShopMoney.h
// 
// ֪ͨ�ͻ��˽�Ǯ��ȡ
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPMONEY_H__
#define __GCPLAYERSHOPMONEY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPlayerShopMoney : public Packet
	{
	public:
		enum
		{
			TYPE_BASE_MONEY = 0,
			TYPE_PROFIT_MONEY,
		};
	public:
		GCPlayerShopMoney( )
		{
			m_Type		=	TYPE_BASE_MONEY;	//�浽��
			m_Amount	=	0;					//����
			m_nShopSerial=  0;					//�̵����к�
		};
		virtual ~GCPlayerShopMoney( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPMONEY; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) +
																sizeof(BYTE) +
																sizeof(BYTE) +
																sizeof(UINT);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetType(VOID) const {return m_Type;}
		VOID					SetType(BYTE nType) {m_Type = nType;}

		UINT					GetAmount(VOID) const {return m_Amount;}
		VOID					SetAmount(UINT nAmount) {m_Amount = nAmount;}

		BYTE					GetShopSerial(VOID) const {return m_nShopSerial;}
		VOID					SetShopSerial(BYTE nSerial) {m_nShopSerial = nSerial;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
		BYTE					m_Type;				//�浽��
		UINT					m_Amount;			//����
		BYTE					m_nShopSerial;			//�̵����к�
		};

	class GCPlayerShopMoneyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopMoney() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPMONEY; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) +
														sizeof(BYTE) +
														sizeof(BYTE) +
														sizeof(UINT);}
	};

	class GCPlayerShopMoneyHandler 
	{
	public:
		static UINT Execute( GCPlayerShopMoney* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
