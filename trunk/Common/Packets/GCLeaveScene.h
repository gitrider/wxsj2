#ifndef __GCLEAVESCENE_H__
#define __GCLEAVESCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class GCLeaveScene : public Packet
{
public:
	GCLeaveScene( ){} ;
	virtual ~GCLeaveScene( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_LEAVESCENE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + sizeof(BYTE); } 
	
public:
	//ʹ�����ݽӿ�
	ObjID_t			getObjID( ){ return m_ObjID ; } ;
	VOID			setObjID( ObjID_t id ){ m_ObjID = id ; } ;

	BYTE			getLeaveCode(VOID) { return m_byLeaveCode; }
	VOID			setLeaveCode(BYTE byLeaveCode) { m_byLeaveCode = byLeaveCode; }

private:
	//����
	ObjID_t			m_ObjID;			//��ҵ�ObjID
	BYTE			m_byLeaveCode;		//�뿪��ԭ��
										// 0 - ��������˳�
										// 1 - ��ʱ��Ϊ��Ӧ������
										// ...
};

class GCLeaveSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCLeaveScene() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_LEAVESCENE ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + sizeof(BYTE); }
};

class GCLeaveSceneHandler 
{
public:
	static UINT Execute( GCLeaveScene* pPacket, Player* pPlayer ) ;
};


}

using namespace Packets;

#endif