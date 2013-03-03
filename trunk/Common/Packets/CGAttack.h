

#ifndef __CGATTACK_H__
#define __CGATTACK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class CGAttack : public Packet 
{
public:
	CGAttack( ){} ;
	virtual ~CGAttack( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ATTACK ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

private:
	//����
	ObjID_t					m_ObjID;	//�Է���ObjID
};


class CGAttackFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAttack() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ATTACK ; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};


class CGAttackHandler 
{
public:
	static UINT Execute( CGAttack* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
