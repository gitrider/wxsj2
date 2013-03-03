

#ifndef __WGRELATION_H__
#define __WGRELATION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

class WGRelation : public Packet 
{
public:
	WGRelation( ){};
	virtual ~WGRelation( ){};

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_WG_RELATION; }
	virtual UINT			GetPacketSize() const { return m_Relation.GetSize(); }
	
public:
	//使用数据接口
	WG_RELATION*			GetRelation( ) { return &m_Relation; }
	VOID					SetRelation( WG_RELATION* pRelation ) { m_Relation = *pRelation; }

private:
	//数据
	WG_RELATION				m_Relation;
};


class WGRelationFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new WGRelation(); }
	PacketID_t	GetPacketID() const { return PACKET_WG_RELATION; }
	UINT		GetPacketMaxSize() const { return sizeof(WG_RELATION); }
};


class WGRelationHandler 
{
public:
	static UINT Execute( WGRelation* pPacket, Player* pPlayer );
};

};

using namespace Packets;

#endif
