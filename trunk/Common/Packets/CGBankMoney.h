// CGBankMoney.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKMONEY_H__
#define __CGBANKMONEY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankMoney : public Packet
	{
	public:
		enum
		{
			SAVE_MONEY = 0,
			PUTOUT_MONEY,
			UPDATE_MONEY,
		};
	public:
		CGBankMoney( )
		{
			m_Save		= SAVE_MONEY;//1,��;0,ȡ;2,����ǰ�����еĽ�Ǯ����;
			m_Amount	=	0;
		};
		virtual ~CGBankMoney( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKMONEY; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(INT);}

	public:

		BYTE					GetIsSave(VOID) const {return m_Save;};
		VOID					SetIsSave(BYTE Save) {m_Save = Save;};

		INT						GetAmount(VOID) const {return m_Amount;};
		VOID					SetAmount(INT Amount) {m_Amount = Amount;};

	private:
		BYTE					m_Save;
		INT						m_Amount;
	};

	class CGBankMoneyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankMoney() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKMONEY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(INT);};
	};

	class CGBankMoneyHandler 
	{
	public:
		static UINT Execute( CGBankMoney* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
