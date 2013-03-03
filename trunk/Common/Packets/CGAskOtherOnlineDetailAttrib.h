// CGAskOtherOnlineDetailAttrib.h
// 
// ȡ��ɫ����ϸ����
// 
//////////////////////////////////////////////////////

#ifndef __CGAskOtherOnlineDetailAttrib_H__
#define __CGAskOtherOnlineDetailAttrib_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskOtherOnlineDetailAttrib : public Packet
{
public:
	CGAskOtherOnlineDetailAttrib( ){
		m_Guid = INVALID_GUID;
	}
	virtual ~CGAskOtherOnlineDetailAttrib( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKOTHERONLINEDETAILATTRIB ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t)+sizeof(GUID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }
	GUID_t					GetGUID() {return m_Guid;}
	VOID					SetGUID(GUID_t guid) {m_Guid = guid;}

private:
	GUID_t					m_Guid;
	//����
	ObjID_t					m_ObjID;	//�Է���ObjID
};


class CGAskOtherOnlineDetailAttribFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskOtherOnlineDetailAttrib() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKOTHERONLINEDETAILATTRIB; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t)+sizeof(GUID_t) ; }
};

class CGAskOtherOnlineDetailAttribHandler 
{
public:
	static UINT Execute( CGAskOtherOnlineDetailAttrib* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif