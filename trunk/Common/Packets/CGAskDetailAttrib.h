// CGAskDetailAttrib.h
// 
// ȡ��ɫ����ϸ����
// 
//////////////////////////////////////////////////////

#ifndef __CGASKDETAILATTRIB_H__
#define __CGASKDETAILATTRIB_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskDetailAttrib : public Packet
{
public:
	CGAskDetailAttrib( ){}
	virtual ~CGAskDetailAttrib( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKDETAILATTRIB ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

private:
	//����
	ObjID_t					m_ObjID;	//�Է���ObjID
};


class CGAskDetailAttribFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskDetailAttrib() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKDETAILATTRIB; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};

class CGAskDetailAttribHandler 
{
public:
	static UINT Execute( CGAskDetailAttrib* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif