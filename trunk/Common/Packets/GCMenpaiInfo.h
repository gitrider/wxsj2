// GCMenpaiInfo.h
// 
// ����NPC������ID
// 
//////////////////////////////////////////////////////

#ifndef __GCMENPAIINFO_H__
#define __GCMENPAIINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCMenpaiInfo : public Packet
	{
	public:
		GCMenpaiInfo( ){};
		virtual ~GCMenpaiInfo( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MENPAIINFO ; }
		virtual UINT			GetPacketSize()const { return	sizeof(short);}

	public:
		short			GetMenpaiInfo_ID(VOID) const {return m_idMenpai;};
		VOID			SetMenpaiInfo_ID(short idMenpai) {m_idMenpai = idMenpai;};

	private:
		short			m_idMenpai;

	};


	class GCMenpaiInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMenpaiInfo() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MENPAIINFO; };
		UINT		GetPacketMaxSize()const { return sizeof(short);};
	};

	class GCMenpaiInfoHandler 
	{
	public:
		static UINT Execute( GCMenpaiInfo* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


