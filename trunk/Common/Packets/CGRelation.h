

#ifndef __CGRELATION_H__
#define __CGRELATION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class CGRelation : public Packet 
{
public:
	CGRelation( ){} ;
	virtual ~CGRelation( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_CG_RELATION ; }
	virtual UINT			GetPacketSize() const 
	{ 
		return m_Relation.GetSize() ; 
	}
	
public:
	//ʹ�����ݽӿ�
	CG_RELATION*			GetRelation( ){ return &m_Relation ; }
	VOID					SetRelation( CG_RELATION* pRelation ){ m_Relation = *pRelation ; } ;

private:
	//����
	CG_RELATION				m_Relation ;

};


class CGRelationFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGRelation() ; }
	PacketID_t	GetPacketID() const { return PACKET_CG_RELATION ; }
	UINT		GetPacketMaxSize() const { return sizeof(CG_RELATION) ; }
};


class CGRelationHandler 
{
public:
	static UINT Execute( CGRelation* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
