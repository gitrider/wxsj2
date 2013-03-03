

#ifndef __GCPACKUPPACKET_H__
#define __GCPACKUPPACKET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

class GCPackUpPacket : public Packet 
{
public:
	GCPackUpPacket( ){} ;
	virtual ~GCPackUpPacket( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_PACKUP_PACKET ; }
	virtual UINT			GetPacketSize()const { return	sizeof(UINT); }
	
public:
	//ʹ�����ݽӿ�

private:
	//����
	//INT			m_nCon

};


class GCPackUpPacketFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCPackUpPacket() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_PACKUP_PACKET ; }
	UINT		GetPacketMaxSize()const { return	sizeof(UINT); }
};


class GCPackUpPacketHandler 
{
public:
	static UINT Execute( GCPackUpPacket* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;



#endif
