#ifndef _CG_EUIPSUITEXCHANGE_H_
#define _CG_EUIPSUITEXCHANGE_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGEquipSuitExchange:public Packet
	{

	public:
		CGEquipSuitExchange(){};
		virtual				~CGEquipSuitExchange(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_EQUIPSUITEXCHANGE; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE);
		}

	public:
		VOID			setEquipSuitNum(BYTE nNum){m_EquipSuitNum = nNum;}
		BYTE			getEquipSuitNum(){return m_EquipSuitNum;}

	private:
		BYTE				m_EquipSuitNum;	//Ò»¼ü»»×°Ì××°±àºÅ
	};


	class CGEquipSuitExchangeFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGEquipSuitExchange(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_EQUIPSUITEXCHANGE; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE); }
	};


	class CGEquipSuitExchangeHandler
	{
	public:
		static UINT	Execute( CGEquipSuitExchange* pPacket, Player* pPlayer );
	};

}


#endif