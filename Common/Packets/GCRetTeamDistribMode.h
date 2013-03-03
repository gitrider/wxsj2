#ifndef _GC_RETTEAMDISTRIBMODE_H_
#define _GC_RETTEAMDISTRIBMODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class GCRetTeamDistribMode:public Packet
	{

	public:
		GCRetTeamDistribMode(){};
		virtual				~GCRetTeamDistribMode(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_RETTEAMDISTRIBMODE; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE);
		}

	public:

		VOID			setMode(BYTE	Ret){ m_nMode = Ret;}
		BYTE			getMode(){return m_nMode;}

	private:
		BYTE				m_nMode;				//是否成功，不成功则是错误号	
	};


	class GCRetTeamDistribModeFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCRetTeamDistribMode(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_RETTEAMDISTRIBMODE; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE); }
	};


	class GCRetTeamDistribModeHandler
	{
	public:
		static UINT Execute( GCRetTeamDistribMode* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif