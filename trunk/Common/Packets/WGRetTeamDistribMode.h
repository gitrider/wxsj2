#ifndef _WG_RETTEAMDISTRIBMODE_H_
#define _WG_RETTEAMDISTRIBMODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class WGRetTeamDistribMode:public Packet
	{

	public:
		WGRetTeamDistribMode(){};
		virtual				~WGRetTeamDistribMode(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_RETTEAMDISTRIBMODE; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(TeamID_t)+sizeof(PlayerID_t);
		}

	public:

		VOID			setMode(BYTE	Ret){ m_nMode = Ret;}
		BYTE			getMode(){return m_nMode;}

		VOID			setTeamID(TeamID_t	Ret){ m_TeamID = Ret;}
		TeamID_t		getTeamID(){return m_TeamID;}

		VOID			setLeaderID(PlayerID_t Ret){ m_LeaderID = Ret;}
		PlayerID_t		getLeaderID(){return m_LeaderID;}
	private:
		BYTE				m_nMode;				//是否成功，不成功则是错误号	
		TeamID_t			m_TeamID;
		PlayerID_t			m_LeaderID;
	};


	class WGRetTeamDistribModeFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new WGRetTeamDistribMode(); }
		PacketID_t	GetPacketID() const { return PACKET_WG_RETTEAMDISTRIBMODE; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)+sizeof(TeamID_t)+sizeof(PlayerID_t); }
	};


	class WGRetTeamDistribModeHandler
	{
	public:
		static UINT Execute( WGRetTeamDistribMode* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif