#ifndef _WGAskOtherOnlineAttrib_Human_H_
#define _WGAskOtherOnlineAttrib_Human_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class WGAskOtherOnlineAttrib_Human : public Packet
{
public:
	WGAskOtherOnlineAttrib_Human( ){}
	virtual ~WGAskOtherOnlineAttrib_Human( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_WG_ASKOTHERONLINEATTRIB_HUMAN; }
	virtual UINT			GetPacketSize()const { return sizeof(m_Guid)+sizeof(PlayerID_t)*2 ; }
	
public:
	//ʹ�����ݽӿ�
	GUID_t					GetGUID() {return m_Guid;}
	VOID					SetGUID(GUID_t guid) {m_Guid = guid;}
	VOID				SetSourcePlayerID(PlayerID_t id) { m_SourcePlayerID = id; }
	PlayerID_t			GetSourcePlayerID(VOID)const { return m_SourcePlayerID; }
	VOID				SetDestPlayerID(PlayerID_t id) { m_DestPlayerID = id; }
	PlayerID_t			GetDestPlayerID(VOID)const { return m_DestPlayerID; }

private:
	PlayerID_t				m_SourcePlayerID;	// �����������ҵ�PlayerID
	PlayerID_t				m_DestPlayerID;		// Ŀ����ҵ�PlayerID
	GUID_t					m_Guid;
};


class WGAskOtherOnlineAttrib_HumanFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new WGAskOtherOnlineAttrib_Human() ; }
	PacketID_t	GetPacketID()const { return PACKET_WG_ASKOTHERONLINEATTRIB_HUMAN; }
	UINT		GetPacketMaxSize()const { return sizeof(GUID_t) + sizeof(PlayerID_t)*2; }
};

class WGAskOtherOnlineAttrib_HumanHandler 
{
public:
	static UINT Execute( WGAskOtherOnlineAttrib_Human* pPacket, Player* pPlayer ) ;
};




}


#endif

