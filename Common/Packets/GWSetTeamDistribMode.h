#ifndef _GW_SETTEAMDISTRIBMODE_H_
#define _GW_SETTEAMDISTRIBMODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class GWSetTeamDistribMode:public Packet
	{

	public:
		GWSetTeamDistribMode(){};
		virtual				~GWSetTeamDistribMode(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_SETTEAMDISTRIBMODE; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(TeamID_t)+sizeof(GUID_t);
		}

	public:

		VOID			setMode(BYTE	Ret){ m_nMode = Ret;}
		BYTE			getMode(){return m_nMode;}

		VOID			setTeamID(TeamID_t	Ret){ m_TeamID = Ret;}
		TeamID_t		getTeamID(){return m_TeamID;}

		VOID			setLeaderID(GUID_t Ret){ m_LeaderGuid = Ret;}
		GUID_t			getLeaderID(){return m_LeaderGuid;}
	private:
		BYTE				m_nMode;				//
		TeamID_t			m_TeamID;
		GUID_t				m_LeaderGuid;
	};


	class GWSetTeamDistribModeFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWSetTeamDistribMode(); }
		PacketID_t	GetPacketID() const { return PACKET_GW_SETTEAMDISTRIBMODE; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)+sizeof(TeamID_t)+sizeof(GUID_t); }
	};


	class GWSetTeamDistribModeHandler
	{
	public:
		static UINT Execute( GWSetTeamDistribMode* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif