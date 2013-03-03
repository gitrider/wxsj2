// CGBankClose.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKCLOSE_H__
#define __CGBANKCLOSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankClose : public Packet
	{
	public:
		CGBankClose( )
		{
			m_BankID = 0;
		};
		virtual ~CGBankClose( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKCLOSE; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetBankID(VOID) const {return m_BankID;};
		VOID					SetBankID(BYTE BankID) {m_BankID = BankID;};

	private:
		BYTE					m_BankID;		//�ɹ����
	};

	class CGBankCloseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankClose() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKCLOSE; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class CGBankCloseHandler 
	{
	public:
		static UINT Execute( CGBankClose* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
