// CGPlayerShopMoney.h
// 
// ֪ͨ��������Ǯ����ȡ��
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPMONEY_H__
#define __CGPLAYERSHOPMONEY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGPlayerShopMoney : public Packet
	{
	public:
		enum
		{
			OPT_SAVE_MONEY = 0,
			OPT_GET_MONEY,
		};
		enum
		{
			TYPE_BASE_MONEY = 0,
			TYPE_PROFIT_MONEY,
		};
	public:
		CGPlayerShopMoney( )
		{
			m_Opt		=	OPT_SAVE_MONEY;		//����
			m_Type		=	TYPE_BASE_MONEY;	//�浽��
			m_Amount	=	0;					//����

		};
		virtual ~CGPlayerShopMoney( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPMONEY; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) +
																sizeof(BYTE) +
																sizeof(BYTE) +
																sizeof(BYTE) +
																sizeof(UINT);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetOpt(VOID) const {return m_Opt;}
		VOID					SetOpt(BYTE Opt) {m_Opt = Opt;}

		BYTE					GetType(VOID) const {return m_Type;}
		VOID					SetType(BYTE nType) {m_Type = nType;}

		UINT					GetAmount(VOID) const {return m_Amount;}
		VOID					SetAmount(UINT nAmount) {m_Amount = nAmount;}

		BYTE					GetShopSerial(VOID) const {return m_nSerial;}
		VOID					SetShopSerial(BYTE nSerial) {m_nSerial = nSerial;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
		BYTE					m_Opt;				//����
		BYTE					m_Type;				//�浽��
		UINT					m_Amount;			//����
		BYTE					m_nSerial;			//�̵����к�
	};

	class CGPlayerShopMoneyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopMoney() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPMONEY; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) +
														sizeof(BYTE) +
														sizeof(BYTE) +
														sizeof(BYTE) +
														sizeof(UINT);}
	};

	class CGPlayerShopMoneyHandler 
	{
	public:
		static UINT Execute( CGPlayerShopMoney* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
