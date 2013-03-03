

#ifndef __CGCHARASKEQUIPMENT_H__
#define __CGCHARASKEQUIPMENT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class CGCharAskEquipment : public Packet 
{
public:
	CGCharAskEquipment( ){} ;
	virtual ~CGCharAskEquipment( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARASKEQUIPMENT ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getObjID(VOID) { return m_ObjID; }
	VOID					setObjID(ObjID_t idObjID) { m_ObjID = idObjID; }

private:
	//����
	ObjID_t					m_ObjID;	//�Է���ObjID
};


class CGCharAskEquipmentFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGCharAskEquipment() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_CHARASKEQUIPMENT ; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};


class CGCharAskEquipmentHandler 
{
public:
	static UINT Execute( CGCharAskEquipment* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
