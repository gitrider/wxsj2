#ifndef __LCSTATUS_H__
#define __LCSTATUS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

	
	class LCStatus : public Packet 
	{
	public:
		LCStatus( ){} ;
		virtual ~LCStatus( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_STATUS;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(USHORT) + sizeof(CLIENT_TURN_STATUS);
		}

	public:
		//ʹ�����ݽӿ�

		USHORT					GetTurnNumber()
		{
			return TurnNumber;
		}

		VOID					SetTurnNumber(USHORT uTurn)
		{
			TurnNumber = uTurn;
		}
		
		CLIENT_TURN_STATUS		GetClientStatus()
		{
			return ClientStatus;
		}

		VOID					SetClientStatus(CLIENT_TURN_STATUS cts)
		{
			ClientStatus = cts;
		}

	private:
		//����
		USHORT					TurnNumber;		//�Ŷ�����
		CLIENT_TURN_STATUS		ClientStatus;	//��ǰ״̬
	};

	class LCStatusFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCStatus() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_STATUS ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(USHORT) + sizeof(CLIENT_TURN_STATUS);
		}
	};


	class LCStatusHandler 
	{
	public:
		static UINT Execute( LCStatus* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif