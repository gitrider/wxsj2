// CGAskDetailAbilityInfo.h
// 
// ȡ��ǰ���ǵ��������Ϣ
// 
//////////////////////////////////////////////////////

#ifndef __CGASKDETAILABILITYINFO_H__
#define __CGASKDETAILABILITYINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskDetailAbilityInfo : public Packet
{
public:
	CGAskDetailAbilityInfo( ){}
	virtual ~CGAskDetailAbilityInfo( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKDETAILABILITYINFO ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

private:
	//����
	ObjID_t					m_ObjID;	//�Է���ObjID
};


class CGAskDetailAbilityInfoFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskDetailAbilityInfo() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKDETAILABILITYINFO; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};

class CGAskDetailAbilityInfoHandler 
{
public:
	static UINT Execute( CGAskDetailAbilityInfo* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif // __CGASKDETAILABILITYINFO_H__
