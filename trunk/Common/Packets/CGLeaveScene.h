#ifndef __CGLEAVESCENE_H__
#define __CGLEAVESCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class CGLeaveScene : public Packet
{
public:
	CGLeaveScene( ){} ;
	virtual ~CGLeaveScene( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_LEAVESCENE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t); } 
	
public:
	//ʹ�����ݽӿ�
	ObjID_t			getObjID( ){ return m_ObjID ; } ;
	VOID			setObjID( ObjID_t id ){ m_ObjID = id ; } ;

private:
	//����
	ObjID_t			m_ObjID;			//��ҵ�ObjID
};

class CGLeaveSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGLeaveScene() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_LEAVESCENE ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t); }
};

class CGLeaveSceneHandler 
{
public:
	static UINT Execute( CGLeaveScene* pPacket, Player* pPlayer ) ;
};


}

using namespace Packets;

#endif