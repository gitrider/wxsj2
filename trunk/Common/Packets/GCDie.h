


#ifndef __GCDIE_H__
#define __GCDIE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCDie : public Packet 
{
public:
	GCDie( ){} ;
	virtual ~GCDie( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_DIE ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t)*2 ; }
	

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setCorpseID(ObjID_t id) { m_CorpseID = id; }
	ObjID_t			getCorpseID(VOID)const { return m_CorpseID; }


private:
	//����
	ObjID_t		m_ObjID;		//��������ObjectID
	ObjID_t		m_CorpseID;		//ʬ��ObjectID

};


class GCDieFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCDie() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_DIE ; }
	UINT		GetPacketMaxSize()const { return 	sizeof(ObjID_t)*2 ; }
};


class GCDieHandler 
{
public:
	static UINT Execute( GCDie* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
