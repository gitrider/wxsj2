// CGAskPrivateInfo.h
// 
// ȡ��ǰ���ǵļ��ܱ�
// 
//////////////////////////////////////////////////////

#ifndef __CGASKPRIVATEINFO_H__
#define __CGASKPRIVATEINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskPrivateInfo : public Packet
{
public:
	CGAskPrivateInfo( ){}
	virtual ~CGAskPrivateInfo( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKPRIVATEINFO ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					GetPlayerID(VOID) { return m_PlayerID; }
	VOID					SetPlayerID(ObjID_t PlayerID) { m_PlayerID = PlayerID; }

private:
	//����
	ObjID_t					m_PlayerID;
};


class CGAskPrivateInfoFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskPrivateInfo() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKPRIVATEINFO; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};

class CGAskPrivateInfoHandler 
{
public:
	static UINT Execute( CGAskPrivateInfo* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;



#endif