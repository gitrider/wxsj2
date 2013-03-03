// CGAskDetailSkillList.h
// 
// ȡ��ǰ���ǵļ��ܱ�
// 
//////////////////////////////////////////////////////

#ifndef __CGASKDETAILSKILLLIST_H__
#define __CGASKDETAILSKILLLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskDetailSkillList : public Packet
{
public:
	CGAskDetailSkillList( ){}
	virtual ~CGAskDetailSkillList( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKDETAILSKILLLIST ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

private:
	//����
	ObjID_t					m_ObjID;	//�Է���ObjID
};


class CGAskDetailSkillListFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskDetailSkillList() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKDETAILSKILLLIST; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};

class CGAskDetailSkillListHandler 
{
public:
	static UINT Execute( CGAskDetailSkillList* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif