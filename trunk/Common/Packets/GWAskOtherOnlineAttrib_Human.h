#ifndef _GWAskOtherOnlineAttrib_Human_H_
#define _GWAskOtherOnlineAttrib_Human_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class GWAskOtherOnlineAttrib_Human : public Packet
{
public:
	GWAskOtherOnlineAttrib_Human( ){}
	virtual ~GWAskOtherOnlineAttrib_Human( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GW_ASKOTHERONLINEATTRIB_HUMAN; }
	virtual UINT			GetPacketSize()const { return sizeof(GUID_t)*2+sizeof(PlayerID_t) ; }
	
public:
	//使用数据接口
	GUID_t					GetSourceGUID() {return m_SourceGuid;}
	VOID					SetSourceGUID(GUID_t guid) {m_SourceGuid = guid;}
	GUID_t					GetDestGUID() {return m_DestGuid;}
	VOID					SetDestGUID(GUID_t guid) {m_DestGuid = guid;}

	VOID				SetPlayerID(PlayerID_t id) { m_PlayerID = id; }
	PlayerID_t			GetPlayerID(VOID)const { return m_PlayerID; }

private:
	PlayerID_t				m_PlayerID;
	GUID_t					m_SourceGuid;	//发起者的GUID
	GUID_t					m_DestGuid;		//目标的GUID
};


class GWAskOtherOnlineAttrib_HumanFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWAskOtherOnlineAttrib_Human() ; }
	PacketID_t	GetPacketID()const { return PACKET_GW_ASKOTHERONLINEATTRIB_HUMAN; }
	UINT		GetPacketMaxSize()const { return sizeof(GUID_t)*2 + sizeof(PlayerID_t); }
};

class GWAskOtherOnlineAttrib_HumanHandler 
{
public:
	static UINT Execute( GWAskOtherOnlineAttrib_Human* pPacket, Player* pPlayer ) ;
};




}


#endif

