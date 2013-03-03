// GWCityAskInitInfo.h
// 
// �������뽨���µĳ���
// 
//////////////////////////////////////////////////////

#ifndef __GWCITYASKINITINFO_H__
#define __GWCITYASKINITINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWCityAskInitInfo : public Packet 
	{
	public:
		GWCityAskInitInfo( ){} ;
		virtual ~GWCityAskInitInfo( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CITYASKINITINFO; }
		virtual UINT			GetPacketSize() const { return 0; }

	public:

	private:

	};

	class GWCityAskInitInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWCityAskInitInfo() ; }
		PacketID_t	GetPacketID() const { return PACKET_GW_CITYASKINITINFO ; }
		UINT		GetPacketMaxSize() const { return 0; }
	};

	class GWCityAskInitInfoHandler 
	{
	public:
		static UINT Execute( GWCityAskInitInfo* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
