// GCAbilityLevel.h

#ifndef _GC_ABILITY_LEVEL_H_
#define _GC_ABILITY_LEVEL_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCAbilityLevel:public Packet
	{

	public:
		GCAbilityLevel(){};
		virtual				~GCAbilityLevel(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_ABILITYLEVEL; }
		virtual UINT			GetPacketSize() const { return sizeof(AbilityID_t)+sizeof(UINT)+sizeof(ABILITY_RESULT); }

	public:
		
		VOID			SetAbilityId(AbilityID_t id) {m_AbilityID = id;};
		AbilityID_t		GetAbilityId() {return m_AbilityID;}
		
		VOID			SetLevel(UINT lvl) {m_Level = lvl;}
		UINT			GetLevel() {return m_Level;}
		
		VOID			SetResult(ABILITY_RESULT res) {m_Result = res;}
		ABILITY_RESULT			GetResult() {return m_Result;}

	private:
		AbilityID_t		m_AbilityID;
		UINT			m_Level;
		ABILITY_RESULT			m_Result;

	};
	
	class GCAbilityLevelFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCAbilityLevel(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_ABILITYLEVEL; }
		UINT		GetPacketMaxSize() const {return sizeof(AbilityID_t)+sizeof(UINT)+sizeof(ABILITY_RESULT); }
	};

	class GCAbilityLevelHandler
	{
	public:
		static UINT Execute( GCAbilityLevel* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // _GC_ABILITY_LEVEL_H_
