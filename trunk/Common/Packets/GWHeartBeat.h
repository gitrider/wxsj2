

#ifndef __GWHEARTBEAT_H__
#define __GWHEARTBEAT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GWHeartBeat : public Packet 
{
public:
	GWHeartBeat( ){} ;
	virtual ~GWHeartBeat( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GW_HEARTBEAT ; }
	virtual UINT			GetPacketSize()const { return	sizeof(GUID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;


private:
	GUID_t					m_GUID ;//��ҵ�GUID

};


class GWHeartBeatFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWHeartBeat() ; }
	PacketID_t	GetPacketID()const { return PACKET_GW_HEARTBEAT ; }
	UINT		GetPacketMaxSize()const { return	sizeof(GUID_t) ; }
};


class GWHeartBeatHandler 
{
public:
	static UINT Execute( GWHeartBeat* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
