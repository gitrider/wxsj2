

#ifndef __GSASKDATA_H__
#define __GSASKDATA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GSAskData : public Packet 
{
public:
	GSAskData( ){} ;
	virtual ~GSAskData( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GS_ASKDATA ; }
	virtual UINT			GetPacketSize()const { return	sizeof(GUID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;


private:
	GUID_t					m_GUID ;//��ҵ�GUID

};


class GSAskDataFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GSAskData() ; }
	PacketID_t	GetPacketID()const { return PACKET_GS_ASKDATA ; }
	UINT		GetPacketMaxSize()const { return	sizeof(GUID_t) ; }
};


class GSAskDataHandler 
{
public:
	static UINT Execute( GSAskData* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
