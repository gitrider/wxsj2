

#ifndef __GWRELATION_H__
#define __GWRELATION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

class GWRelation : public Packet 
{
public:
	GWRelation( ){}
	virtual ~GWRelation( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_GW_RELATION; }
	virtual UINT			GetPacketSize() const { return m_Relation.GetSize(); }
	
public:
	//ʹ�����ݽӿ�
	GW_RELATION*			GetRelation( ) { return &m_Relation; }
	VOID					SetRelation( GW_RELATION* pRelation ) { m_Relation = *pRelation; }

private:
	//����
	GW_RELATION				m_Relation;
};


class GWRelationFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWRelation(); }
	PacketID_t	GetPacketID() const { return PACKET_GW_RELATION; }
	UINT		GetPacketMaxSize() const { return sizeof(GW_RELATION); }
};


class GWRelationHandler 
{
public:
	static UINT Execute( GWRelation* pPacket, Player* pPlayer );
};

};

using namespace Packets;

#endif
