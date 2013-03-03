#ifndef __GCDELOBJECT_H__
#define __GCDELOBJECT_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCDelObject : public Packet
{
public:
	GCDelObject( ){} ;
	virtual ~GCDelObject( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_DELOBJECT; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + sizeof( SceneID_t ); }

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setSceneID( SceneID_t idScene ){ m_idScene = idScene; }
	SceneID_t		getSceneID( VOID )const{ return m_idScene; }

private:
	//����
	ObjID_t			m_ObjID;	// ����Obj���͵�ObjID
	SceneID_t		m_idScene;	// ����ID
};


class GCDelObjectFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCDelObject() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_DELOBJECT; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + sizeof( SceneID_t ); }
};

class GCDelObjectHandler 
{
public:
	static UINT Execute( GCDelObject* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif