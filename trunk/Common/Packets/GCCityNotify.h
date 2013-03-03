// GCCityNotify.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCCITYNOTIFY_H__
#define __GCCITYNOTIFY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCCityNotify : public Packet
	{
	public:
		GCCityNotify( )
		{
			m_ID = 0;
		}
		virtual ~GCCityNotify( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CITYNOTIFY; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetID(VOID) const {return m_ID;};
		VOID					SetID(BYTE EID) {m_ID = EID;};

	private:
		BYTE					m_ID;		//�ɹ����
	};

	class GCCityNotifyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCityNotify() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CITYNOTIFY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCCityNotifyHandler 
	{
	public:
		static UINT Execute( GCCityNotify* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
