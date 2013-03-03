#ifndef _CG_SETTEAMDISTRIBMODE_H_
#define _CG_SETTEAMDISTRIBMODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class CGSetTeamDistribMode:public Packet
	{

	public:
		CGSetTeamDistribMode(){};
		virtual				~CGSetTeamDistribMode(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_SETTEAMDISTRIBMODE; }

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


	class CGSetTeamDistribModeFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGSetTeamDistribMode(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_SETTEAMDISTRIBMODE; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE); }
	};


	class CGSetTeamDistribModeHandler
	{
	public:
		static UINT Execute( CGSetTeamDistribMode* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif