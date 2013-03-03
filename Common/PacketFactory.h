


#ifndef __PACKETFACTORY_H__
#define __PACKETFACTORY_H__


#include "Packet.h"


class PacketFactory 
{
public :
	
	virtual ~PacketFactory ()  {}

	virtual Packet*		CreatePacket ()  = 0;

	virtual PacketID_t	GetPacketID ()const  = 0;

	virtual UINT		GetPacketMaxSize ()const  = 0;

};

class BillPacketFactory 
{
public :

	virtual ~BillPacketFactory ()  {}

	virtual BillPacket*		CreatePacket ()  = 0;

	virtual BPacketID_t		GetPacketID ()const  = 0;

	virtual UINT			GetPacketMaxSize ()const  = 0;

};


#endif
