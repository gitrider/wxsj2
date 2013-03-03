// CGAskLeanAbility.h
// 
// 发送NPC的门派ID
// 
//////////////////////////////////////////////////////

#ifndef __CGASKSTUDYABILITY_H__
#define __CGASKSTUDYABILITY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGAskLeanAbility : public Packet
	{
	public:
		CGAskLeanAbility(){};
		virtual ~CGAskLeanAbility(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKSTUDYABILITY; }
		virtual UINT			GetPacketSize()const { return	sizeof(AbilityID_t) + sizeof(INT);}

	public:
		AbilityID_t				GetAbilityID(VOID) const {return m_nAbilityID;};
		VOID					SetAbilityID(AbilityID_t nAbilityID) {m_nAbilityID = nAbilityID;}
		
		INT						GetNpcId(VOID)const {return m_nNpcID;}
		VOID					SetNpcId(INT npcId) {m_nNpcID = npcId;}
	private:
		AbilityID_t				m_nAbilityID;
		INT						m_nNpcID;

	};


	class CGAskLeanAbilityFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskLeanAbility() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKSTUDYABILITY; };
		UINT		GetPacketMaxSize()const { return sizeof(AbilityID_t) + sizeof(INT);};
	};

	class CGAskLeanAbilityHandler 
	{
	public:
		static UINT Execute( CGAskLeanAbility* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


