/********************************************************************
	created:	2007/10/31
	created:	31:10:2007   17:23
	filename: 	d:\project\MapServer\Server\GCUseSectorSkill.h
	file path:	d:\project\MapServer\Server
	file base:	GCUseSectorSkill
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef GCUseSectorSkill_h__31_10_2007_17_23
#define GCUseSectorSkill_h__31_10_2007_17_23

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class GCUseSectorSkill : public Packet 
	{
	public:
		GCUseSectorSkill( ){};
		virtual ~GCUseSectorSkill( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ATTACK ; }
		virtual UINT			GetPacketSize()const
		{ 
			return	sizeof(UINT) +
					sizeof(BYTE);
		}

	public:
		//使用数据接口

	private:

	};


	class GCUseSectorSkillFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCUseSectorSkill() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_USE_SECTORSKILL; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(UINT) +
					sizeof(BYTE);
		}
	};


	class GCUseSectorSkillHandler 
	{
	public:
		static UINT Execute( GCUseSectorSkill* pPacket, Player* pPlayer ) ;
	};
}

#endif // GCUseSectorSkill_h__