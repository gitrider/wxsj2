// GCAskLeanAbilityResult.h
// 
// 发送NPC的门派ID
// 
//////////////////////////////////////////////////////

#ifndef __GCASKSTUDYABILITY_H__
#define __GCASKSTUDYABILITY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCAskLeanAbility : public Packet
	{
	public:
		GCAskLeanAbility(){};
		virtual ~GCAskLeanAbility(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ASKSTUDYABILITY; }
		virtual UINT			GetPacketSize()const { return	sizeof(AbilityID_t) + sizeof(INT) ;}

	public:
		AbilityID_t				GetAbilityID(VOID) const {return m_nAbilityID;};
		VOID					SetAbilityID(AbilityID_t nAbilityID) {m_nAbilityID = nAbilityID;}

		INT						GetStudyResult(VOID)const {return m_nResult;}
		VOID					SetStudyResult(INT nResult) {m_nResult = nResult;}
	private:
		AbilityID_t				m_nAbilityID;
		INT						m_nResult;

	};


	class GCAskLeanAbilityFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCAskLeanAbility() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_ASKSTUDYABILITY; };
		UINT		GetPacketMaxSize()const { return sizeof(AbilityID_t) + sizeof(INT);};
	};

	class GCAskLeanAbilityHandler 
	{
	public:
		static UINT Execute( GCAskLeanAbility* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


